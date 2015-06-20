#pragma once

class CMIMEItem;
class VCalendar;

/// <summary>iCalendar�p�[�T�[</summary>
class VCalendarParser {
public:
    /// <summary>�R���X�g���N�^</summary>
    /// <param name="item">MIME�A�C�e��</param>
    VCalendarParser(CMIMEItem* item);

    /// <summary>MIME�A�C�e�����p�[�X���AiCalendar���擾����B</summary>
    /// <returns>
    /// iCalendar�I�u�W�F�N�g�B
    /// MIME�A�C�e����iCalendar�łȂ��Ƃ���NULL��Ԃ��B
    /// </returns>
    VCalendar* parseCalendar();

    /// <summary>VCalendar�̏������������Ԃ��B</summary>
    /// <returns>������������</returns>
    tstring toString();

private:
    CMIMEItem* item; // MIME�A�C�e��

    string contentType;
    string charSet;
    string transferEncoding;
    string method;
    string rawText;

    VCalendar* calendar;
    list<VCalValueBlock*> blocks;
    VCalValueBlock* currentBlock;

    /// <summary>�p�[�T�[�����Z�b�g����B</summary>
    void resetParser();
    /// <summary>Content-Transfer-Encoding�Ŏw�肳�ꂽ���������Ńf�R�[�h����B</summary>
    void decodeTransferEncoding();
    /// <summary>charset�Ŏw�肳�ꂽ�����Z�b�g����MBCS������֕ϊ�����B</summary>
    void decodeCharSet();
    /// <summary>iCalendar�e�L�X�g���p�[�X����B</summary>
    /// <returns>iCalendar�I�u�W�F�N�g</returns>
    VCalendar* parse();
    /// <summary>�p�[�X�Ώۍs���u���b�N��؂�s���ǂ����`�F�b�N����B</summary>
    /// <param name="line">�p�[�X�Ώۍs</param>
    /// <returns>�u���b�N�s(BEGIN:xxx��������END:xxx)�Ȃ�true���A����ȊO��false</returns>
    bool parseLine(const string& line);
    /// <summary>iCalendar��1�s���p�[�X����B</summary>
    /// <param name="line">�p�[�X�Ώۍs</param>
    /// <returns>�p�[�X�̐���</returns>
    bool checkBlock(const string& line);
};
