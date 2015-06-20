#pragma once

class CBeckyAPI;

/// <summary>���[�e�B���e�B�N���X</summary>
class Utils {
private:
    /// <summary>�R���X�g���N�^</summary>
    Utils() {}

public:
    /// <summary>DLL���W���[���C���X�^���X���Z�b�g����B</summary>
    /// <param name="h">DLL���W���[���C���X�^���X</param>
    static void setModuleInstance(HINSTANCE h) { hInstance = h; }
    /// <summary>DLL���W���[���C���X�^���X���擾����B</summary>
    /// <returns>DLL���W���[���C���X�^���X</returns>
    static HINSTANCE getModuleInstance() { return hInstance; }
    /// <summary>CBeckyAPI���擾����B</summary>
    /// <returns>CBeckyAPI</returns>
    static CBeckyAPI& getBeckyApi() { return beckyApi; }
    /// <summary>�v���p�e�B�t�@�C�������擾����B</summary>
    /// <returns>�v���p�e�B�t�@�C����</returns>
    static tstring getPropertiesFile();

    /// <summary>�����񃊃\�[�X����w��ID�̕�������擾����B</summary>
    /// <param name="uID">������ID</param>
    /// <returns>������</returns>
    static string LoadStringA(UINT uID);
    /// <summary>�����񃊃\�[�X����w��ID�̕�������擾����B</summary>
    /// <param name="uID">������ID</param>
    /// <returns>������</returns>
    static wstring LoadStringW(UINT uID);
    /// <summary>������������𐶐�����B</summary>
    /// <param name="format">����</param>
    /// <param name="...">�������ΏۃI�u�W�F�N�g(��)</param>
    /// <returns>������������</returns>
    static tstring format(const TCHAR* format, ...);
    /// <summary>�}���`�o�C�g�����񂩂�T������֕ϊ�����B</summary>
    /// <param name="text">�}���`�o�C�g������</param>
    /// <returns>T������</returns>
    static tstring stot(const string& text);

    /// <summary>
    ///     �t�@�C���Ƀg���[�X���o�͂���B
    ///     DLL�Ɠ����t�H���_����BkCalendar.log�֏o�͂���B
    /// </summary>
    /// <param name="format">����</param>
    /// <param name="...">�g���[�X�o�͗p�I�u�W�F�N�g(��)</param>
    static void Trace(LPCTSTR format, ...);

private:
    /// <summary>DLL���W���[���C���X�^���X</summary>
    static HINSTANCE hInstance;
    /// <summary>CBeckyAPI�C���X�^���X</summary>
    static CBeckyAPI beckyApi;
};
