#include "stdafx.h"
#include "Utils.h"
#include "VCalendar.h"


/// <summary>VCalValueデストラクタ</summary>
VCalValue::~VCalValue() {
    while (!attributes.empty()) {
        delete attributes.back();
        attributes.pop_back();
    }
}


/// <summary>指定した名前を持つ属性を取得する。</summary>
/// <param name="name">名前</param>
/// <returns>属性。見つからなかった場合はNULLを返す。</returns>
VCalAttribute* VCalValue::getAttribute(const tstring& name) const {
    for (auto it = attributes.cbegin(); it != attributes.cend(); it++) {
        if ((*it)->getName() == name) {
            return *it;
        }
    }
    return NULL;
}


/// <summary>VCalValueBlockデストラクタ</summary>
VCalValueBlock::~VCalValueBlock() {
    while (!values.empty()) {
        delete values.back();
        values.pop_back();
    }
    while (!blocks.empty()) {
        delete blocks.back();
        blocks.pop_back();
    }
}


/// <summary>ブロックの書式文字列化</summary>
/// <param name="indent">インデント</param>
/// <returns>書式化文字列</returns>
tstring VCalValueBlock::toString(const tstring& indent) const {
    tstring text = indent + _T("[") + name + _T("]\n");
    tstring indnt = indent + _T("  ");
    for (auto it = blocks.cbegin(); it != blocks.cend(); it++) {
        text += (*it)->toString(indnt);
    }
    for (auto it = values.cbegin(); it != values.cend(); it++) {
        tstring attr;
        for (auto ita = (*it)->getAttributes().cbegin(); ita != (*it)->getAttributes().cend(); ita++) {
            if (!attr.empty()) {
                attr += _T(", ");
            }
            attr += (*ita)->getName() + _T("=") + (*ita)->getValue();
        }
        if (!attr.empty()) {
            attr = _T("(") + attr + _T(")");
        }
        text += indnt + (*it)->getName() + attr + _T(": ") + (*it)->getValue() + _T("\n");
    }
    return text;
}

/// <summary>指定した名前を持つ値を取得する。</summary>
/// <param name="name">名前</param>
/// <returns>値。見つからなかった場合はNULLを返す。</returns>
VCalValue* VCalValueBlock::getValue(const tstring& name) const {
    for (auto it = values.cbegin(); it != values.cend(); it++) {
        if ((*it)->getName() == name) {
            return *it;
        }
    }
    return NULL;
}

/// <summary>指定した名前を持つブロックを取得する。</summary>
/// <param name="name">名前</param>
/// <returns>ブロック。見つからなかった場合はNULLを返す。</returns>
VCalValueBlock* VCalValueBlock::getBlock(const tstring& name) const {
    for (auto it = blocks.cbegin(); it != blocks.cend(); it++) {
        if ((*it)->getName() == name) {
            return *it;
        }
    }
    return NULL;
}


/// <summary>タイムゾーンIDを取得する。</summary>
/// <returns>タイムゾーンID</returns>
tstring VCalTimeZone::getTzId() const {
    VCalValue* value = getValue(_T("TZID"));
    return value == NULL ? _T("") : value->getValue();
}

/// <summary>タイムゾーンオフセットを取得する。</summary>
/// <returns>タイムゾーンオフセット</returns>
/// <remarks>いまひとつVTIMEZONEのフォーマットを理解していないので取れるものから適当に実装。</remarks>
tstring VCalTimeZone::getTimeZoneOffset() const {
    //TZID: Tokyo Standard Time
    //[VTIMEZONE]
    //  [STANDARD]
    //    DTSTART: 16010101T000000
    //    TZOFFSETFROM: +0900
    //    TZOFFSETTO: +0900
    //  [DAYLIGHT]
    //    DTSTART: 16010101T000000
    //    TZOFFSETFROM: +0900
    //    TZOFFSETTO: +0900
    VCalValueBlock* standard = getBlock(_T("STANDARD"));
    if (standard != NULL) {
        VCalValue* value = standard->getValue(_T("TZOFFSETFROM"));
        if (value != NULL) {
            return value->getValue();
        }
        value = standard->getValue(_T("TZOFFSETTO"));
        if (value != NULL) {
            return value->getValue();
        }
    }
    VCalValueBlock* daylight = getBlock(_T("DAYLIGHT"));
    if (daylight != NULL) {
        VCalValue* value = standard->getValue(_T("TZOFFSETFROM"));
        if (value != NULL) {
            return value->getValue();
        }
        value = standard->getValue(_T("TZOFFSETTO"));
        if (value != NULL) {
            return value->getValue();
        }
    }
    return _T("+0000");
}


/// <summary>予定時刻をテキスト化する。</summary>
/// <param name="start">開始時刻のときはtrue、終了時刻のときはfalse</param>
/// <param name="format">書式</param>
/// <returns>予定時刻のテキスト表現</returns>
tstring VCalTime::toString(bool start, TIMEFORMAT format) const {
    // 現在の年を取得
    time_t current = ::time(NULL);
    struct tm tm;
    localtime_s(&tm, &current);
    int currentYear = tm.tm_year;

    // tmに変換
    _tsetlocale(LC_TIME, _T(""));
    if (allday) {
        // 終日のときは00:00:00なのでgmtime()で取得する(localtime()だと時差分おかしくなるため)。
        // 終了日は次の日が入るので、1秒引いて前の日にしてやる。
        time_t time = this->time - (start ? 0 : 1);
        gmtime_s(&tm, &time);
    } else {
        localtime_s(&tm, &time);
    }

    // 書式の選択
    tstring tformat = tm.tm_year == currentYear ? _T("%m/%d(%a)") : _T("%Y/%m/%d(%a)");
    if (allday) {
        if (format == FORMAT_TIME) {
            tformat = _T("");
        }
    } else {
        if (format == FORMAT_TIME) {
            tformat = _T("%H:%M");
        } else if (format == FORMAT_ALL) {
            tformat += _T(" %H:%M");
        }
    }

    TCHAR t[100];
    _tcsftime(t, 1024, tformat.c_str(), &tm);
    return t;
}


/// <summary>イベント開始時刻(UTC)を取得する。</summary>
/// <returns>イベント開始時刻</returns>
VCalTime* VCalEvent::getStartTime() const {
    VCalValue* value = getValue(_T("DTSTART"));
    return parseTime(value);
}

/// <summary>イベント終了時刻(UTC)を取得する。</summary>
/// <returns>イベント終了時刻</returns>
VCalTime* VCalEvent::getEndTime() const {
    VCalValue* value = getValue(_T("DTEND"));
    return parseTime(value);
}

/// <summary>イベント場所を取得する。</summary>
/// <returns>イベント場所</returns>
tstring VCalEvent::getLocation() const {
    VCalValue* value = getValue(_T("LOCATION"));
    return value == NULL ? _T("") : value->getValue();
}

/// <summary>イベント日時を取得する。</summary>
/// <returns>イベント日時</returns>
tstring VCalEvent::getDate() const {
    //・今年のときは年を省略
    //・終了日が開始日と同じときは終了日の日付を省略
    //・期間
    //	06/23(火) 10:00 - 11:00
    //	06/23(火) 23:00 - 06/24(水) 01:00
    //	12/31(木) 23:00 - 2016/01/01(金) 01:00
    //	06/23(火)
    //	06/23(火) - 06/24(水)
    //	12/31(木) - 2016/01/01(金)

    VCalTime* start = getStartTime();
    VCalTime* end = getEndTime();
    tstring dstart = start == NULL ? _T("???") : start->toString(true, VCalTime::FORMAT_DATE);
    tstring dend = end == NULL ? _T("???") : end->toString(false, VCalTime::FORMAT_DATE);
    tstring tstart = start == NULL ? _T("???") : start->toString(true, VCalTime::FORMAT_TIME);
    tstring tend = end == NULL ? _T("???") : end->toString(false, VCalTime::FORMAT_TIME);
    delete start;
    delete end;

    tstring date = dstart + (tstart.empty() ? _T("") : _T(" ") + tstart);
    if (dstart != dend || tstart != tend) {
        date += _T(" -");
        date += dstart != dend ? _T(" ") + dend : _T("");
        date += tend.empty() ? _T("") : _T(" ") + tend;
    }
    return date;
}

/// <summary>イベント時刻(UTC)を取得する。</summary>
/// <param name="value">値</param>
/// <returns>イベント時刻</returns>
VCalTime* VCalEvent::parseTime(const VCalValue* value) const {
    if (value == NULL) {
        return NULL;
    }
    tstring start = value->getValue();
    VCalAttribute* attribute = value->getAttribute(_T("TZID"));
    tstring tzid = attribute == NULL ? _T("") : attribute->getValue();
    attribute = value->getAttribute(_T("VALUE"));
    bool allday = attribute != NULL && attribute->getValue() == _T("DATE");
    VCalTimeZone* tz = root->getTimeZone(tzid);
    tstring timezone = tz == NULL ? _T("+0000") : tz->getTimeZoneOffset();

    // 終日:
    // DTSTART;VALUE=DATE:20150617
    // DTEND;VALUE=DATE:20150618
    // 時刻指定:
    // DTSTART;TZID=Tokyo Standard Time:20150616T170000
    // DTEND;TZID=Tokyo Standard Time:20150616T173000
    // DTSTART:20150617T033400Z
    // DTEND:20150617T044500Z
    if (start.length() < 8) {
        return NULL;
    }
    struct tm tm;
    memset(&tm, 0, sizeof(tm));
    tm.tm_year = _ttoi(start.substr(0, 4).c_str()) - 1900;
    tm.tm_mon = _ttoi(start.substr(4, 2).c_str()) - 1;
    tm.tm_mday = _ttoi(start.substr(6, 2).c_str());
    if (!allday) {
        if (start.length() < 15) {
            return NULL;
        }
        tm.tm_hour = _ttoi(start.substr(9, 2).c_str());
        tm.tm_min = _ttoi(start.substr(11, 2).c_str());
        tm.tm_sec = _ttoi(start.substr(13, 2).c_str());
    }
    time_t time = _mkgmtime(&tm);
    // 末尾に"Z"があるときはUTC、ないときはローカル時刻だが、"Z"はチェックせずとも大丈夫のはず
    time -= (_ttoi(timezone.substr(0, 3).c_str()) * 60 + _ttoi(timezone.substr(3, 2).c_str())) * 60;

    return new VCalTime(time, allday);
}


/// <summary>メソッドを取得する。</summary>
/// <returns>メソッド</returns>
tstring VCalendar::getMethod() const {
    VCalValue* value = getValue(_T("METHOD"));
    return value == NULL ? _T("") : value->getValue();
}

/// <summary>指定したタイムゾーンIDを持つタイムゾーンブロックを取得する。</summary>
/// <param name="tzId">タイムゾーンID</param>
/// <returns>タイムゾーンブロック。見つからなかった場合はNULLを返す。</returns>
VCalTimeZone* VCalendar::getTimeZone(const tstring& tzId) const {
    if (tzId.empty()) {
        return NULL;
    }
    for (auto it = blocks.cbegin(); it != blocks.cend(); it++) {
        VCalValueBlock* block = *it;
        if (typeid(*block) == typeid(VCalTimeZone)) {
            VCalTimeZone* tz = (VCalTimeZone*) block;
            if (tz->getTzId() == tzId) {
                return tz;
            }
        }
    }
    return NULL;
}

/// <summary>iCalendarに含むイベントブロックの数を取得する。</summary>
/// <returns>イベントブロックの数</returns>
int VCalendar::getEventCount() const {
    int count = 0;
    for (auto it = blocks.cbegin(); it != blocks.cend(); it++) {
        VCalValueBlock* block = *it;
        if (typeid(*block) == typeid(VCalEvent)) {
            count++;
        }
    }
    return count;
}

/// <summary>指定したインデクスを持つイベントブロックを取得する。</summary>
/// <param name="index">インデクス</param>
/// <returns>イベントブロック。見つからなかった場合はNULLを返す。</returns>
VCalEvent* VCalendar::getEvent(int index) const {
    int count = 0;
    for (auto it = blocks.cbegin(); it != blocks.cend(); it++) {
        VCalValueBlock* block = *it;
        if (typeid(*block) == typeid(VCalEvent)) {
            if (index == count) {
                return (VCalEvent*) block;
            }
            count++;
        }
    }
    return NULL;
}
