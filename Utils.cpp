#include "stdafx.h"
#include "../BeckyAPI.h"
#include "Utils.h"


/// <summary>DLLモジュールインスタンス</summary>
HINSTANCE Utils::hInstance = NULL;
/// <summary>CBeckyAPIインスタンス</summary>
CBeckyAPI Utils::beckyApi;


/// <summary>プロパティファイル名を取得する。</summary>
/// <returns>プロパティファイル名</returns>
tstring Utils::getPropertiesFile() {
    return tstring(Utils::getBeckyApi().GetDataFolder()) + _T("\\PlugIns\\BkCalendar.ini");
}

/// <summary>文字列リソースから指定IDの文字列を取得する。</summary>
/// <param name="uID">文字列ID</param>
/// <returns>文字列</returns>
string Utils::LoadStringA(UINT uID) {
    int size = 256;
    for (;;) {
        char* buffer = new char[size];
        int length = ::LoadStringA(hInstance, uID, buffer, size);
        if (length == 0) {
            delete[] buffer;
            return NULL;
        } else if (length == size) {
            delete[] buffer;
            size *= 2;
            continue;
        }
        string out = buffer;
        delete[] buffer;
        return out;
    }
}

/// <summary>文字列リソースから指定IDの文字列を取得する。</summary>
/// <param name="uID">文字列ID</param>
/// <returns>文字列</returns>
wstring Utils::LoadStringW(UINT uID) {
    int size = 256;
    for (;;) {
        LPWSTR buffer = new WCHAR[size];
        int length = ::LoadStringW(hInstance, uID, buffer, size);
        if (length == 0) {
            delete[] buffer;
            return NULL;
        } else if (length == size) {
            delete[] buffer;
            size *= 2;
            continue;
        }
        wstring out = buffer;
        delete[] buffer;
        return out;
    }
}

/// <summary>書式化文字列を生成する。</summary>
/// <param name="format">書式</param>
/// <param name="...">書式化対象オブジェクト(可変)</param>
/// <returns>書式化文字列</returns>
tstring Utils::format(const TCHAR* format, ...) {
    tstring value;
    va_list valist;
    va_start(valist, format);
    for (int size = 1024; size < 0x1000000; size *= 2) {
        TCHAR* buffer = new TCHAR[size];
        int length = _vstprintf_s(buffer, size, format, valist);
        value = buffer;
        delete[] buffer;
        if (length < size - 1) {
            return value;
        }
    }
    return _T("<<too big>>");
}

/// <summary>マルチバイト文字列からT文字列へ変換する。</summary>
/// <param name="text">マルチバイト文字列</param>
/// <returns>T文字列</returns>
tstring Utils::stot(const string& text) {
#ifdef UNICODE
    wchar_t* wcs = new wchar_t[text.length() + 1];
    size_t ret;
	::mbstowcs_s(&ret, wcs, text.length() + 1, text.c_str(), text.length() + 1);
    tstring ttext = wcs;
	delete [] wcs;
    return ttext;
#else
    return text;
#endif // !UNICODE
}

/// <summary>
///     ファイルにトレースを出力する。
///     DLLと同じフォルダ内のBkCalendar.logへ出力する。
/// </summary>
/// <param name="format">書式</param>
/// <param name="...">トレース出力用オブジェクト(可変)</param>
void Utils::Trace(LPCTSTR format, ...) {
    TCHAR path[_MAX_PATH];
    ::GetModuleFileName(hInstance, path, _MAX_PATH);
    _tcscpy_s(_tcsrchr(path, _T('\\')) + 1, 20, _T("BkCalendar.log"));

    FILE* file;
    if (0 != _tfopen_s(&file, path, _T("a"))) {
        return;
    }

    va_list args;
    va_start(args, format);
    _vftprintf(file, format, args);
    va_end(args);

    fclose(file);
}
