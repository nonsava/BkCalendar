#include "StdAfx.h"
#include <boost/algorithm/string.hpp>
#include "Utils.h"
#include "Properties.h"


/// <summary>コンストラクタ</summary>
/// <param name="inFile">プロパティファイル</param>
Properties::Properties(const TCHAR* iniFile) {
    this->iniFile = iniFile;
}

/// <summary>コンストラクタ</summary>
/// <param name="inFile">プロパティファイル</param>
Properties::Properties(const tstring& iniFile) {
    this->iniFile = iniFile;
}


/// <summary>文字列型のプロパティ値を取得する。</summary>
/// <param name="section">プロパティの所属するセクション名</param>
/// <param name="name">プロパティのキー名</param>
/// <param name="value">プロパティ値</param>
/// <returns>取得できたときはtrueを、できなかったときはfalse</returns>
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

/// <summary>int型のプロパティ値を取得する。</summary>
/// <param name="section">プロパティの所属するセクション名</param>
/// <param name="name">プロパティのキー名</param>
/// <param name="value">プロパティ値</param>
/// <returns>取得できたときはtrueを、できなかったときはfalse</returns>
bool Properties::getInteger(const TCHAR* section, const TCHAR* name, int* value) {
    *value = 0;
    tstring v;
    if (!getString(section, name, &v)) {
        return false;
    }
    *value = _ttoi(v.c_str());
    return true;
}

/// <summary>long型のプロパティ値を取得する。</summary>
/// <param name="section">プロパティの所属するセクション名</param>
/// <param name="name">プロパティのキー名</param>
/// <param name="value">プロパティ値</param>
/// <returns>取得できたときはtrueを、できなかったときはfalse</returns>
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
/// 論理値型のプロパティ値を取得する。
/// 指定値が"TRUE"(大文字小文字区別なし)もしくは"1"のときはtrue、
/// "FALSE"(大文字小文字区別なし)もしくは"0"のときはfalseとする。
/// </summary>
/// <param name="section">プロパティの所属するセクション名</param>
/// <param name="name">プロパティのキー名</param>
/// <param name="value">プロパティ値</param>
/// <returns>取得できたときはtrueを、できなかったときはfalse</returns>
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


/// <summary>文字列型のプロパティ値を永続化する。</summary>
/// <param name="section">プロパティの所属するセクション名</param>
/// <param name="name">プロパティのキー名</param>
/// <param name="value">プロパティ値</param>
/// <returns>セットできたときはtrueを、できなかったときはfalse</returns>
bool Properties::setString(const TCHAR* section, const TCHAR* name, const TCHAR* value) {
    BOOL b = ::WritePrivateProfileString(section, name, value, iniFile.c_str());
    return b ? true : false;
}

/// <summary>文字列型のプロパティ値を永続化する。</summary>
/// <param name="section">プロパティの所属するセクション名</param>
/// <param name="name">プロパティのキー名</param>
/// <param name="value">プロパティ値</param>
/// <returns>セットできたときはtrueを、できなかったときはfalse</returns>
bool Properties::setString(const TCHAR* section, const TCHAR* name, const tstring& value) {
    return setString(section, name, value.c_str());
}

/// <summary>int型のプロパティ値を永続化する。</summary>
/// <param name="section">プロパティの所属するセクション名</param>
/// <param name="name">プロパティのキー名</param>
/// <param name="value">プロパティ値</param>
/// <returns>セットできたときはtrueを、できなかったときはfalse</returns>
bool Properties::setInteger(const TCHAR* section, const TCHAR* name, int value) {
    return setString(section, name, Utils::format(_T("%d"), value));
}

/// <summary>long型のプロパティ値を永続化する。</summary>
/// <param name="section">プロパティの所属するセクション名</param>
/// <param name="name">プロパティのキー名</param>
/// <param name="value">プロパティ値</param>
/// <returns>セットできたときはtrueを、できなかったときはfalse</returns>
bool Properties::setLong(const TCHAR* section, const TCHAR* name, long value) {
    return setString(section, name, Utils::format(_T("%l"), value));
}

/// <summary>論理値型のプロパティ値を永続化する。</summary>
/// <param name="section">プロパティの所属するセクション名</param>
/// <param name="name">プロパティのキー名</param>
/// <param name="value">プロパティ値</param>
/// <returns>セットできたときはtrueを、できなかったときはfalse</returns>
bool Properties::setBool(const TCHAR* section, const TCHAR* name, bool value) {
    return setString(section, name, value ? _T("true") : _T("false"));
}
