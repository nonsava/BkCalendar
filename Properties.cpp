#include "StdAfx.h"
#include <boost/algorithm/string.hpp>
#include "Utils.h"
#include "Properties.h"


/// <summary>�R���X�g���N�^</summary>
/// <param name="inFile">�v���p�e�B�t�@�C��</param>
Properties::Properties(const TCHAR* iniFile) {
    this->iniFile = iniFile;
}

/// <summary>�R���X�g���N�^</summary>
/// <param name="inFile">�v���p�e�B�t�@�C��</param>
Properties::Properties(const tstring& iniFile) {
    this->iniFile = iniFile;
}


/// <summary>������^�̃v���p�e�B�l���擾����B</summary>
/// <param name="section">�v���p�e�B�̏�������Z�N�V������</param>
/// <param name="name">�v���p�e�B�̃L�[��</param>
/// <param name="value">�v���p�e�B�l</param>
/// <returns>�擾�ł����Ƃ���true���A�ł��Ȃ������Ƃ���false</returns>
bool Properties::getString(const TCHAR* section, const TCHAR* name, tstring* value) {
    const TCHAR* dummy = _T("__dummy_&%$#__");
    for (DWORD size = 1024;; size *= 2) {
        TCHAR* buffer = new TCHAR[size];
        DWORD length = ::GetPrivateProfileString(section, name, dummy, buffer, size, iniFile.c_str());
        *value = buffer;
        delete[] buffer;
        if (length < size - 1) {
            if (*value == dummy) {
                break;
            }
            return true;
        }
    }
    value->empty();
    return false;
}

/// <summary>int�^�̃v���p�e�B�l���擾����B</summary>
/// <param name="section">�v���p�e�B�̏�������Z�N�V������</param>
/// <param name="name">�v���p�e�B�̃L�[��</param>
/// <param name="value">�v���p�e�B�l</param>
/// <returns>�擾�ł����Ƃ���true���A�ł��Ȃ������Ƃ���false</returns>
bool Properties::getInteger(const TCHAR* section, const TCHAR* name, int* value) {
    *value = 0;
    tstring v;
    if (!getString(section, name, &v)) {
        return false;
    }
    *value = _ttoi(v.c_str());
    return true;
}

/// <summary>long�^�̃v���p�e�B�l���擾����B</summary>
/// <param name="section">�v���p�e�B�̏�������Z�N�V������</param>
/// <param name="name">�v���p�e�B�̃L�[��</param>
/// <param name="value">�v���p�e�B�l</param>
/// <returns>�擾�ł����Ƃ���true���A�ł��Ȃ������Ƃ���false</returns>
bool Properties::getLong(const TCHAR* section, const TCHAR* name, long* value) {
    *value = 0L;
    tstring v;
    if (!getString(section, name, &v)) {
        return false;
    }
    *value = _ttol(v.c_str());
    return true;
}

/// <summary>
/// �_���l�^�̃v���p�e�B�l���擾����B
/// �w��l��"TRUE"(�啶����������ʂȂ�)��������"1"�̂Ƃ���true�A
/// "FALSE"(�啶����������ʂȂ�)��������"0"�̂Ƃ���false�Ƃ���B
/// </summary>
/// <param name="section">�v���p�e�B�̏�������Z�N�V������</param>
/// <param name="name">�v���p�e�B�̃L�[��</param>
/// <param name="value">�v���p�e�B�l</param>
/// <returns>�擾�ł����Ƃ���true���A�ł��Ȃ������Ƃ���false</returns>
bool Properties::getBool(const TCHAR* section, const TCHAR* name, bool* value) {
    *value = false;
    tstring v;
    if (!getString(section, name, &v)) {
        return false;
    }
    boost::to_upper(v);
    if (v == _T("TRUE") || v == _T("1")) {
        *value = true;
    } else if (v == _T("FALSE") || v == _T("0")) {
        *value = false;
    } else {
        return false;
    }
    return true;
}


/// <summary>������^�̃v���p�e�B�l���i��������B</summary>
/// <param name="section">�v���p�e�B�̏�������Z�N�V������</param>
/// <param name="name">�v���p�e�B�̃L�[��</param>
/// <param name="value">�v���p�e�B�l</param>
/// <returns>�Z�b�g�ł����Ƃ���true���A�ł��Ȃ������Ƃ���false</returns>
bool Properties::setString(const TCHAR* section, const TCHAR* name, const TCHAR* value) {
    BOOL b = ::WritePrivateProfileString(section, name, value, iniFile.c_str());
    return b ? true : false;
}

/// <summary>������^�̃v���p�e�B�l���i��������B</summary>
/// <param name="section">�v���p�e�B�̏�������Z�N�V������</param>
/// <param name="name">�v���p�e�B�̃L�[��</param>
/// <param name="value">�v���p�e�B�l</param>
/// <returns>�Z�b�g�ł����Ƃ���true���A�ł��Ȃ������Ƃ���false</returns>
bool Properties::setString(const TCHAR* section, const TCHAR* name, const tstring& value) {
    return setString(section, name, value.c_str());
}

/// <summary>int�^�̃v���p�e�B�l���i��������B</summary>
/// <param name="section">�v���p�e�B�̏�������Z�N�V������</param>
/// <param name="name">�v���p�e�B�̃L�[��</param>
/// <param name="value">�v���p�e�B�l</param>
/// <returns>�Z�b�g�ł����Ƃ���true���A�ł��Ȃ������Ƃ���false</returns>
bool Properties::setInteger(const TCHAR* section, const TCHAR* name, int value) {
    return setString(section, name, Utils::format(_T("%d"), value));
}

/// <summary>long�^�̃v���p�e�B�l���i��������B</summary>
/// <param name="section">�v���p�e�B�̏�������Z�N�V������</param>
/// <param name="name">�v���p�e�B�̃L�[��</param>
/// <param name="value">�v���p�e�B�l</param>
/// <returns>�Z�b�g�ł����Ƃ���true���A�ł��Ȃ������Ƃ���false</returns>
bool Properties::setLong(const TCHAR* section, const TCHAR* name, long value) {
    return setString(section, name, Utils::format(_T("%l"), value));
}

/// <summary>�_���l�^�̃v���p�e�B�l���i��������B</summary>
/// <param name="section">�v���p�e�B�̏�������Z�N�V������</param>
/// <param name="name">�v���p�e�B�̃L�[��</param>
/// <param name="value">�v���p�e�B�l</param>
/// <returns>�Z�b�g�ł����Ƃ���true���A�ł��Ȃ������Ƃ���false</returns>
bool Properties::setBool(const TCHAR* section, const TCHAR* name, bool value) {
    return setString(section, name, value ? _T("true") : _T("false"));
}
