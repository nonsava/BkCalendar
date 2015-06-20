#include "stdafx.h"
#include "../BeckyAPI.h"
#include "Utils.h"


/// <summary>DLL���W���[���C���X�^���X</summary>
HINSTANCE Utils::hInstance = NULL;
/// <summary>CBeckyAPI�C���X�^���X</summary>
CBeckyAPI Utils::beckyApi;


/// <summary>�v���p�e�B�t�@�C�������擾����B</summary>
/// <returns>�v���p�e�B�t�@�C����</returns>
tstring Utils::getPropertiesFile() {
    return tstring(Utils::getBeckyApi().GetDataFolder()) + _T("\\PlugIns\\BkCalendar.ini");
}

/// <summary>�����񃊃\�[�X����w��ID�̕�������擾����B</summary>
/// <param name="uID">������ID</param>
/// <returns>������</returns>
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

/// <summary>�����񃊃\�[�X����w��ID�̕�������擾����B</summary>
/// <param name="uID">������ID</param>
/// <returns>������</returns>
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

/// <summary>������������𐶐�����B</summary>
/// <param name="format">����</param>
/// <param name="...">�������ΏۃI�u�W�F�N�g(��)</param>
/// <returns>������������</returns>
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

/// <summary>�}���`�o�C�g�����񂩂�T������֕ϊ�����B</summary>
/// <param name="text">�}���`�o�C�g������</param>
/// <returns>T������</returns>
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
///     �t�@�C���Ƀg���[�X���o�͂���B
///     DLL�Ɠ����t�H���_����BkCalendar.log�֏o�͂���B
/// </summary>
/// <param name="format">����</param>
/// <param name="...">�g���[�X�o�͗p�I�u�W�F�N�g(��)</param>
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
