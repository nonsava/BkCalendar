#include "stdafx.h"
#include "Utils.h"
#include "VCalendar.h"


/// <summary>VCalValue�f�X�g���N�^</summary>
VCalValue::~VCalValue() {
    while (!attributes.empty()) {
        delete attributes.back();
        attributes.pop_back();
    }
}


/// <summary>�w�肵�����O�����������擾����B</summary>
/// <param name="name">���O</param>
/// <returns>�����B������Ȃ������ꍇ��NULL��Ԃ��B</returns>
VCalAttribute* VCalValue::getAttribute(const tstring& name) const {
    for (auto it = attributes.cbegin(); it != attributes.cend(); it++) {
        if ((*it)->getName() == name) {
            return *it;
        }
    }
    return NULL;
}


/// <summary>VCalValueBlock�f�X�g���N�^</summary>
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


/// <summary>�u���b�N�̏���������</summary>
/// <param name="indent">�C���f���g</param>
/// <returns>������������</returns>
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

/// <summary>�w�肵�����O�����l���擾����B</summary>
/// <param name="name">���O</param>
/// <returns>�l�B������Ȃ������ꍇ��NULL��Ԃ��B</returns>
VCalValue* VCalValueBlock::getValue(const tstring& name) const {
    for (auto it = values.cbegin(); it != values.cend(); it++) {
        if ((*it)->getName() == name) {
            return *it;
        }
    }
    return NULL;
}

/// <summary>�w�肵�����O�����u���b�N���擾����B</summary>
/// <param name="name">���O</param>
/// <returns>�u���b�N�B������Ȃ������ꍇ��NULL��Ԃ��B</returns>
VCalValueBlock* VCalValueBlock::getBlock(const tstring& name) const {
    for (auto it = blocks.cbegin(); it != blocks.cend(); it++) {
        if ((*it)->getName() == name) {
            return *it;
        }
    }
    return NULL;
}


/// <summary>�^�C���]�[��ID���擾����B</summary>
/// <returns>�^�C���]�[��ID</returns>
tstring VCalTimeZone::getTzId() const {
    VCalValue* value = getValue(_T("TZID"));
    return value == NULL ? _T("") : value->getValue();
}

/// <summary>�^�C���]�[���I�t�Z�b�g���擾����B</summary>
/// <returns>�^�C���]�[���I�t�Z�b�g</returns>
/// <remarks>���܂ЂƂ�VTIMEZONE�̃t�H�[�}�b�g�𗝉����Ă��Ȃ��̂Ŏ�����̂���K���Ɏ����B</remarks>
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


/// <summary>�\�莞�����e�L�X�g������B</summary>
/// <param name="start">�J�n�����̂Ƃ���true�A�I�������̂Ƃ���false</param>
/// <param name="format">����</param>
/// <returns>�\�莞���̃e�L�X�g�\��</returns>
tstring VCalTime::toString(bool start, TIMEFORMAT format) const {
    // ���݂̔N���擾
    time_t current = ::time(NULL);
    struct tm tm;
    localtime_s(&tm, &current);
    int currentYear = tm.tm_year;

    // tm�ɕϊ�
    _tsetlocale(LC_TIME, _T(""));
    if (allday) {
        // �I���̂Ƃ���00:00:00�Ȃ̂�gmtime()�Ŏ擾����(localtime()���Ǝ��������������Ȃ邽��)�B
        // �I�����͎��̓�������̂ŁA1�b�����đO�̓��ɂ��Ă��B
        time_t time = this->time - (start ? 0 : 1);
        gmtime_s(&tm, &time);
    } else {
        localtime_s(&tm, &time);
    }

    // �����̑I��
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


/// <summary>�C�x���g�J�n����(UTC)���擾����B</summary>
/// <returns>�C�x���g�J�n����</returns>
VCalTime* VCalEvent::getStartTime() const {
    VCalValue* value = getValue(_T("DTSTART"));
    return parseTime(value);
}

/// <summary>�C�x���g�I������(UTC)���擾����B</summary>
/// <returns>�C�x���g�I������</returns>
VCalTime* VCalEvent::getEndTime() const {
    VCalValue* value = getValue(_T("DTEND"));
    return parseTime(value);
}

/// <summary>�C�x���g�ꏊ���擾����B</summary>
/// <returns>�C�x���g�ꏊ</returns>
tstring VCalEvent::getLocation() const {
    VCalValue* value = getValue(_T("LOCATION"));
    return value == NULL ? _T("") : value->getValue();
}

/// <summary>�C�x���g�������擾����B</summary>
/// <returns>�C�x���g����</returns>
tstring VCalEvent::getDate() const {
    //�E���N�̂Ƃ��͔N���ȗ�
    //�E�I�������J�n���Ɠ����Ƃ��͏I�����̓��t���ȗ�
    //�E����
    //	06/23(��) 10:00 - 11:00
    //	06/23(��) 23:00 - 06/24(��) 01:00
    //	12/31(��) 23:00 - 2016/01/01(��) 01:00
    //	06/23(��)
    //	06/23(��) - 06/24(��)
    //	12/31(��) - 2016/01/01(��)

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

/// <summary>�C�x���g����(UTC)���擾����B</summary>
/// <param name="value">�l</param>
/// <returns>�C�x���g����</returns>
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

    // �I��:
    // DTSTART;VALUE=DATE:20150617
    // DTEND;VALUE=DATE:20150618
    // �����w��:
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
    // ������"Z"������Ƃ���UTC�A�Ȃ��Ƃ��̓��[�J�����������A"Z"�̓`�F�b�N�����Ƃ����v�̂͂�
    time -= (_ttoi(timezone.substr(0, 3).c_str()) * 60 + _ttoi(timezone.substr(3, 2).c_str())) * 60;

    return new VCalTime(time, allday);
}


/// <summary>���\�b�h���擾����B</summary>
/// <returns>���\�b�h</returns>
tstring VCalendar::getMethod() const {
    VCalValue* value = getValue(_T("METHOD"));
    return value == NULL ? _T("") : value->getValue();
}

/// <summary>�w�肵���^�C���]�[��ID�����^�C���]�[���u���b�N���擾����B</summary>
/// <param name="tzId">�^�C���]�[��ID</param>
/// <returns>�^�C���]�[���u���b�N�B������Ȃ������ꍇ��NULL��Ԃ��B</returns>
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

/// <summary>iCalendar�Ɋ܂ރC�x���g�u���b�N�̐����擾����B</summary>
/// <returns>�C�x���g�u���b�N�̐�</returns>
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

/// <summary>�w�肵���C���f�N�X�����C�x���g�u���b�N���擾����B</summary>
/// <param name="index">�C���f�N�X</param>
/// <returns>�C�x���g�u���b�N�B������Ȃ������ꍇ��NULL��Ԃ��B</returns>
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
