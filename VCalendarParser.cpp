#include "stdafx.h"
#include <boost/algorithm/string.hpp>
#include "../BeckyAPI.h"
#include "../BkCommon.h"
#include "Utils.h"
#include "VCalendar.h"
#include "VCalendarParser.h"


// iCalendarのMIMEタイプ
static const char* TEXT_CALENDAR = "text/calendar";


/// <summary>コンストラクタ</summary>
/// <param name="item">MIMEアイテム</param>
VCalendarParser::VCalendarParser(CMIMEItem* item) : item(item) {
}

/// <summary>MIMEアイテムをパースし、iCalendarを取得する。</summary>
/// <returns>
/// iCalendarオブジェクト。
/// MIMEアイテムがiCalendarでないときはNULLを返す。
/// </returns>
VCalendar* VCalendarParser::parseCalendar() {
    // Content-Typeの取得
    // text/calendarのみ許容
    char header[1024];
    item->GetHeader(_T("Content-Type"), header, 1024);
    char* next = strchr(header, ';');
    if (next == NULL) {
        return NULL;
    }
    contentType = string(header, next - header);
    boost::trim(contentType);
    if (contentType != TEXT_CALENDAR) {
        return NULL;
    }

    // charset, methodの取得
    char* p = strtok(next + 1, ";");
    do {
        char* charSetValue = strstr(p, "charset=");
        if (charSetValue != NULL) {
            charSet = charSetValue + 8;
            boost::trim_if(charSet, boost::is_any_of(" \t\""));
        } else {
            char* methodValue = strstr(p, "method=");
            if (methodValue != NULL) {
                method = methodValue + 7;
                boost::trim_if(method, boost::is_any_of(" \t\""));
            }
        }
        p = strtok(NULL, ";");
    } while (p != NULL);
    if (method.empty()) {
        // methodは必須
        return NULL;
    }

    // Content-Transfer-Encodingの取得
    item->GetHeader(_T("Content-Transfer-Encoding"), header, 1024);
    if (header != NULL) {
        transferEncoding = header;
        boost::trim_if(transferEncoding, boost::is_any_of("\xa\xd \t"));
    }

    // iCalendarの取得
    CPointerItem* pItem = item->m_lstBody.GetTop();
    while (pItem) {
        if (pItem->GetLength() == 0) {
            break;
        }
        rawText += pItem->GetData();
        pItem = pItem->GetNext();
    }

    decodeTransferEncoding();
    decodeCharSet();

    return parse();
}

/// <summary>VCalendarの書式化文字列を返す。</summary>
/// <returns>書式化文字列</returns>
tstring VCalendarParser::toString() {
    tstring text;
    text += _T("********************\n");
    text += _T("content_type:      ") + Utils::stot(contentType) + _T("\n");
    text += _T("char_set:          ") + Utils::stot(charSet) + _T("\n");
    text += _T("method:            ") + Utils::stot(method) + _T("\n");
    text += _T("transfer_encoding: ") + Utils::stot(transferEncoding) + _T("\n");
    text += _T("raw_text:\n") + Utils::stot(rawText) + _T("\n");
    text += _T("********************\n");
    return text;
}


/// 符号方式変換関数プロトタイプ
typedef BOOL (__stdcall* DECODE_PROC)(LPCTSTR, LPCTSTR, BOOL);

/// <summary>指定された符号方式変換関数を使用してデコードする。</summary>
/// <param name="text">デコード対象のテキスト</param>
/// <param name="transfer_encoding">符号方式変換関数</param>
/// <returns>デコード結果</returns>
string decodeTransferEncoding(const string& text, DECODE_PROC proc) {
    LPCTSTR src = Utils::getBeckyApi().GetTempFileName(_T("src"));
    LPCTSTR dst = Utils::getBeckyApi().GetTempFileName(_T("dst"));
    FILE* file;
    _tfopen_s(&file, src, _T("w"));
    fwrite(text.c_str(), sizeof(char), text.length(), file);
    fclose(file);
    (*proc)(dst, src, false);
    string newtext = FileToString(dst);
    ::DeleteFile(src);
    ::DeleteFile(dst);
    return newtext;
}

/// <summary>Content-Transfer-Encodingで指定された符号方式でデコードする。</summary>
void VCalendarParser::decodeTransferEncoding() {
    string transfer_encoding = transferEncoding;
    boost::to_upper(transfer_encoding);
    if (transfer_encoding == "BASE64") {
        rawText = ::decodeTransferEncoding(rawText, Utils::getBeckyApi().B64Convert);
    } else if (transfer_encoding == "QUOTED-PRINTABLE") {
        rawText = ::decodeTransferEncoding(rawText, Utils::getBeckyApi().QPConvert);
    }
}

/// <summary>charsetで指定された文字セットからMBCS文字列へ変換する。</summary>
void VCalendarParser::decodeCharSet() {
    string char_set = charSet;
    boost::to_upper(char_set);
    if (char_set == "UTF-8" || char_set == "UTF8") {
        rawText = Utils::getBeckyApi().UTF_8(Utils::stot(rawText).c_str(), false);
    }
}


/// <summary>パーサーをリセットする。</summary>
void VCalendarParser::resetParser() {
    if (calendar != NULL) {
        delete calendar;
    }
    calendar = NULL;
    currentBlock = NULL;
}

/// <summary>iCalendarテキストをパースする。</summary>
/// <returns>iCalendarオブジェクト</returns>
VCalendar* VCalendarParser::parse() {
    resetParser();

    // 改行がおかしなことになっているので、いらない\x0Dと\x0Aは削除。
    // 行の先頭が空白文字のときは、先頭の空白文字を取り除いて前の行に接続。
    istringstream ss(rawText);
    string text;
    string mline;
    string line;
    while (getline(ss, line)) {
        boost::trim_if(line, boost::is_any_of("\xa\xd"));
        if (line.empty()) {
            continue;
        } else if (line[0] == ' ') {
            line = line.substr(1);
            mline += line;
            text += line;
            continue;
        } else {
            if (!mline.empty()) {
                if (!parseLine(mline)) {
                    resetParser();
                    return NULL;
                }
            }
            if (!text.empty()) {
                text += '\n';
            }
            mline = line;
            text += line;
        }
    }
    if (!mline.empty()) {
        if (!parseLine(mline)) {
            resetParser();
            return NULL;
        }
    }
    rawText = text;

    // currentBlockがNULLでないということは、END:xxxで閉じられていないということ。
    if (currentBlock != NULL) {
        resetParser();
    }

    return calendar;
}

/// <summary>パース対象行がブロック区切り行かどうかチェックする。</summary>
/// <param name="line">パース対象行</param>
/// <returns>ブロック行(BEGIN:xxxもしくはEND:xxx)ならtrueを、それ以外はfalse</returns>
bool VCalendarParser::checkBlock(const string& line) {
    bool ret = false;
    if (line.find("BEGIN:") == 0 && line.length() > 6) {
        VCalValueBlock* block;
        string name = line.substr(6);
        if (calendar == NULL) {
            if (name != "VCALENDAR") {
                throw 1;
            }
            calendar = new VCalendar();
            block = calendar;
        } else {
            if (name == "VTIMEZONE") {
                block = new VCalTimeZone(calendar);
            } else if (name == "VEVENT") {
                block = new VCalEvent(calendar);
            } else {
                block = new VCalValueBlock(calendar);
            }
            VCalValueBlock* parent = blocks.back();
            parent->addBlock(block);
        }
        blocks.push_back(block);
        currentBlock = block;
        currentBlock->setName(Utils::stot(name));
        ret = true;
    } else if (line.find("END:") == 0 && line.length() > 4) {
        string name = line.substr(4);
        if (currentBlock == NULL || currentBlock->getName() != Utils::stot(name)) {
            throw 1;
        }
        blocks.pop_back();
        currentBlock = blocks.empty() ? NULL : blocks.back();
        ret = true;
    }
    return ret;
}

/// <summary>iCalendarの1行をパースする。</summary>
/// <param name="line">パース対象行</param>
/// <returns>パースの成否</returns>
bool VCalendarParser::parseLine(const string& line) {
    // ブロック区切り行をチェック
    try {
        if (checkBlock(line)) {
            return true;
        }
    } catch (int) {
        return false;
    }

    // 値行をパース
    int index;
    if ((index = line.find(':')) == -1) {
        return false;
    }
    string value = line.substr(index + 1);
    string name = line.substr(0, index);

    list<VCalAttribute*> attributes;
    for (;;) {
        if ((index = name.rfind(';')) == -1) {
            break;
        }
        string attr = name.substr(index + 1);
        name = name.substr(0, index);

        if ((index = attr.find('=')) == -1) {
            return false;
        }
        VCalAttribute* attribute = new VCalAttribute(Utils::stot(attr.substr(0, index)), Utils::stot(attr.substr(index + 1)));
        attributes.push_back(attribute);
    }

    VCalValue* val = new VCalValue(calendar, Utils::stot(name), Utils::stot(value), attributes);
    currentBlock->addValue(val);

    return true;
}
