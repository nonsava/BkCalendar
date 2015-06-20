#pragma once

/// <summary>�v���p�e�B����N���X</summary>
class Properties {
    public:
        /// <summary>�R���X�g���N�^</summary>
        /// <param name="inFile">�v���p�e�B�t�@�C��</param>
        Properties(const TCHAR* iniFile);
        /// <summary>�R���X�g���N�^</summary>
        /// <param name="inFile">�v���p�e�B�t�@�C��</param>
        Properties(const tstring& iniFile);

        /// <summary>������^�̃v���p�e�B�l���擾����B</summary>
        /// <param name="section">�v���p�e�B�̏�������Z�N�V������</param>
        /// <param name="name">�v���p�e�B�̃L�[��</param>
        /// <param name="value">�v���p�e�B�l</param>
        /// <returns>�擾�ł����Ƃ���true���A�ł��Ȃ������Ƃ���false</returns>
        bool getString(const TCHAR* section, const TCHAR* name, tstring* value);
        /// <summary>int�^�̃v���p�e�B�l���擾����B</summary>
        /// <param name="section">�v���p�e�B�̏�������Z�N�V������</param>
        /// <param name="name">�v���p�e�B�̃L�[��</param>
        /// <param name="value">�v���p�e�B�l</param>
        /// <returns>�擾�ł����Ƃ���true���A�ł��Ȃ������Ƃ���false</returns>
        bool getInteger(const TCHAR* section, const TCHAR* name, int* value);
        /// <summary>long�^�̃v���p�e�B�l���擾����B</summary>
        /// <param name="section">�v���p�e�B�̏�������Z�N�V������</param>
        /// <param name="name">�v���p�e�B�̃L�[��</param>
        /// <param name="value">�v���p�e�B�l</param>
        /// <returns>�擾�ł����Ƃ���true���A�ł��Ȃ������Ƃ���false</returns>
        bool getLong(const TCHAR* section, const TCHAR* name, long* value);
        /// <summary>
        /// �_���l�^�̃v���p�e�B�l���擾����B
        /// �w��l��"TRUE"(�啶����������ʂȂ�)��������"1"�̂Ƃ���true�A
        /// "FALSE"(�啶����������ʂȂ�)��������"0"�̂Ƃ���false�Ƃ���B
        /// </summary>
        /// <param name="section">�v���p�e�B�̏�������Z�N�V������</param>
        /// <param name="name">�v���p�e�B�̃L�[��</param>
        /// <param name="value">�v���p�e�B�l</param>
        /// <returns>�擾�ł����Ƃ���true���A�ł��Ȃ������Ƃ���false</returns>
        bool getBool(const TCHAR* section, const TCHAR* name, bool* value);

        /// <summary>������^�̃v���p�e�B�l���i��������B</summary>
        /// <param name="section">�v���p�e�B�̏�������Z�N�V������</param>
        /// <param name="name">�v���p�e�B�̃L�[��</param>
        /// <param name="value">�v���p�e�B�l</param>
        /// <returns>�Z�b�g�ł����Ƃ���true���A�ł��Ȃ������Ƃ���false</returns>
        bool setString(const TCHAR* section, const TCHAR* name, const TCHAR* value);
        /// <summary>������^�̃v���p�e�B�l���i��������B</summary>
        /// <param name="section">�v���p�e�B�̏�������Z�N�V������</param>
        /// <param name="name">�v���p�e�B�̃L�[��</param>
        /// <param name="value">�v���p�e�B�l</param>
        /// <returns>�Z�b�g�ł����Ƃ���true���A�ł��Ȃ������Ƃ���false</returns>
        bool setString(const TCHAR* section, const TCHAR* name, const tstring& value);
        /// <summary>int�^�̃v���p�e�B�l���i��������B</summary>
        /// <param name="section">�v���p�e�B�̏�������Z�N�V������</param>
        /// <param name="name">�v���p�e�B�̃L�[��</param>
        /// <param name="value">�v���p�e�B�l</param>
        /// <returns>�Z�b�g�ł����Ƃ���true���A�ł��Ȃ������Ƃ���false</returns>
        bool setInteger(const TCHAR* section, const TCHAR* name, int value);
        /// <summary>long�^�̃v���p�e�B�l���i��������B</summary>
        /// <param name="section">�v���p�e�B�̏�������Z�N�V������</param>
        /// <param name="name">�v���p�e�B�̃L�[��</param>
        /// <param name="value">�v���p�e�B�l</param>
        /// <returns>�Z�b�g�ł����Ƃ���true���A�ł��Ȃ������Ƃ���false</returns>
        bool setLong(const TCHAR* section, const TCHAR* name, long value);
        /// <summary>�_���l�^�̃v���p�e�B�l���i��������B</summary>
        /// <param name="section">�v���p�e�B�̏�������Z�N�V������</param>
        /// <param name="name">�v���p�e�B�̃L�[��</param>
        /// <param name="value">�v���p�e�B�l</param>
        /// <returns>�Z�b�g�ł����Ƃ���true���A�ł��Ȃ������Ƃ���false</returns>
        bool setBool(const TCHAR* section, const TCHAR* name, bool value);

    private:
        /// <summary>�v���p�e�B�t�@�C����</summary>
        tstring iniFile;
};
