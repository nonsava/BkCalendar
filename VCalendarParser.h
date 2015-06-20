#pragma once

class CMIMEItem;
class VCalendar;

/// <summary>iCalendarパーサー</summary>
class VCalendarParser {
public:
    /// <summary>コンストラクタ</summary>
    /// <param name="item">MIMEアイテム</param>
    VCalendarParser(CMIMEItem* item);

    /// <summary>MIMEアイテムをパースし、iCalendarを取得する。</summary>
    /// <returns>
    /// iCalendarオブジェクト。
    /// MIMEアイテムがiCalendarでないときはNULLを返す。
    /// </returns>
    VCalendar* parseCalendar();

    /// <summary>VCalendarの書式化文字列を返す。</summary>
    /// <returns>書式化文字列</returns>
    tstring toString();

private:
    CMIMEItem* item; // MIMEアイテム

    string contentType;
    string charSet;
    string transferEncoding;
    string method;
    string rawText;

    VCalendar* calendar;
    list<VCalValueBlock*> blocks;
    VCalValueBlock* currentBlock;

    /// <summary>パーサーをリセットする。</summary>
    void resetParser();
    /// <summary>Content-Transfer-Encodingで指定された符号方式でデコードする。</summary>
    void decodeTransferEncoding();
    /// <summary>charsetで指定された文字セットからMBCS文字列へ変換する。</summary>
    void decodeCharSet();
    /// <summary>iCalendarテキストをパースする。</summary>
    /// <returns>iCalendarオブジェクト</returns>
    VCalendar* parse();
    /// <summary>パース対象行がブロック区切り行かどうかチェックする。</summary>
    /// <param name="line">パース対象行</param>
    /// <returns>ブロック行(BEGIN:xxxもしくはEND:xxx)ならtrueを、それ以外はfalse</returns>
    bool parseLine(const string& line);
    /// <summary>iCalendarの1行をパースする。</summary>
    /// <param name="line">パース対象行</param>
    /// <returns>パースの成否</returns>
    bool checkBlock(const string& line);
};
