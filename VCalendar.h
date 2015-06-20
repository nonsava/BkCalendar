#pragma once

class VCalendar;

/// <summary>�l�̊��N���X</summary>
class VCalObject {
public:
    /// <summary>�R���X�g���N�^</summary>
    /// <param name="root">�eiCalendar</param>
    VCalObject(const VCalendar* root) : root(root) {}
    /// <summary>�f�X�g���N�^</summary>
    virtual ~VCalObject() {}
protected:
    const VCalendar* root; // �eiCalendar
};

/// <summary>�l�̑���������킷�N���X</summary>
class VCalAttribute {
public:
    /// <summary>�R���X�g���N�^</summary>
    /// <param name="name">������</param>
    /// <param name="value">�����l</param>
    VCalAttribute(const tstring& name, const tstring& value) : name(name), value(value) {}
    /// <summary>�R���X�g���N�^</summary>
    /// <param name="name">������</param>
    VCalAttribute(const tstring& name) : name(name) {}

    /// <summary>���������擾����B</summary>
    /// <returns>������</returns>
    inline tstring getName() const { return name; }
    /// <summary>���������Z�b�g����B</summary>
    /// <param name="name">������</param>
    inline void setName(const tstring& name) { this->name = name; }
    /// <summary>�����l���擾����B</summary>
    /// <returns>�����l</returns>
    inline tstring getValue() const { return value; }
    /// <summary>�����l���Z�b�g����B</summary>
    /// <param name="value">�����l</param>
    inline void setValue(const tstring& value) { this->value = value; }

protected:
    tstring name; // ������
    tstring value; // �����l
};

/// <summary>�l������킷�N���X</summary>
class VCalValue : protected VCalObject {
public:
    /// <summary>�R���X�g���N�^</summary>
    /// <param name="root">�eiCalendar</param>
    /// <param name="name">���O</param>
    /// <param name="value">�l</param>
    /// <param name="attributes">�������X�g</param>
    VCalValue(const VCalendar* root, const tstring& name, const tstring& value, const list<VCalAttribute*>& attributes) : VCalObject(root), name(name), value(value), attributes(attributes) {}
    /// <summary>�R���X�g���N�^</summary>
    /// <param name="root">�eiCalendar</param>
    /// <param name="name">���O</param>
    /// <param name="value">�l</param>
    VCalValue(const VCalendar* root, const tstring& name, const tstring& value) : VCalObject(root), name(name), value(value) {}
    /// <summary>�R���X�g���N�^</summary>
    /// <param name="root">�eiCalendar</param>
    /// <param name="name">���O</param>
    VCalValue(const VCalendar* root, const tstring& name) : VCalObject(root), name(name) {}
    /// <summary>�R���X�g���N�^</summary>
    /// <param name="root">�eiCalendar</param>
    VCalValue(const VCalendar* root) : VCalObject(root) {}
    /// <summary>�f�X�g���N�^</summary>
    virtual ~VCalValue();

    /// <summary>���O���擾����B</summary>
    /// <returns>���O</returns>
    inline tstring getName() const { return name; }
    /// <summary>���O���Z�b�g����B</summary>
    /// <param name="name">���O</param>
    inline void setName(const tstring& name) { this->name = name; }
    /// <summary>�l���擾����B</summary>
    /// <returns>�l</returns>
    inline tstring getValue() const { return value; }
    /// <summary>�l���Z�b�g����B</summary>
    /// <param name="value">�l</param>
    inline void setValue(const tstring& value) { this->value = value; }
    /// <summary>�������X�g���擾����B</summary>
    /// <returns>�������X�g</returns>
    inline list<VCalAttribute*>& getAttributes() { return attributes; }
    /// <summary>�w�肵�������������������擾����B</summary>
    /// <param name="name">������</param>
    /// <returns>�����B������Ȃ������ꍇ��NULL��Ԃ��B</returns>
    VCalAttribute* getAttribute(const tstring& name) const;
    /// <summary>�������X�g���Z�b�g����B</summary>
    /// <param name="attributes">�������X�g</param>
    inline void setAttributes(const list<VCalAttribute*>& attributes) { this->attributes = attributes; }
    /// <summary>�������X�g�ɑ�����ǉ�����B</summary>
    /// <param name="attribute">����</param>
    inline void addAttribute(VCalAttribute* attribute) { attributes.push_back(attribute); }

protected:
    tstring name; // ���O
    tstring value; // �l
    list<VCalAttribute*> attributes; // �������X�g
};

/// <summary>�l�u���b�N������킷�N���X</summary>
/// <remarks>
/// BEGIN:xxx����END:xxx�܂ł��l�u���b�N�ƂȂ�B
/// xxx���l�u���b�N�̖��O(name)�ƂȂ�B
/// </remarks>
class VCalValueBlock : protected VCalObject {
public:
    /// <summary>�R���X�g���N�^</summary>
    /// <param name="root">�eiCalendar</param>
    /// <param name="name">�u���b�N��</param>
    /// <param name="values">�q�̒l���X�g</param>
    /// <param name="blocks">�q�̃u���b�N���X�g</param>
    VCalValueBlock(const VCalendar* root, const tstring& name, const list<VCalValue*>& values, const list<VCalValueBlock*>& blocks) : VCalObject(root), name(name), values(values), blocks(blocks) {}
    /// <summary>�R���X�g���N�^</summary>
    /// <param name="root">�eiCalendar</param>
    /// <param name="name">�u���b�N��</param>
    VCalValueBlock(const VCalendar* root, const tstring& name) : VCalObject(root), name(name) {}
    /// <summary>�R���X�g���N�^</summary>
    /// <param name="root">�eiCalendar</param>
    VCalValueBlock(const VCalendar* root) : VCalObject(root) {}
    /// <summary>�f�X�g���N�^</summary>
    virtual ~VCalValueBlock();

    /// <summary>�u���b�N�����擾����B</summary>
    /// <returns>�u���b�N��</returns>
    inline tstring getName() const { return name; }
    /// <summary>�u���b�N�����Z�b�g����B</summary>
    /// <param name="name">�u���b�N��</param>
    inline void setName(const tstring& name) { this->name = name; }
    /// <summary>�q�̒l���X�g���擾����B</summary>
    /// <returns>�l���X�g</returns>
    inline list<VCalValue*>& getValues() { return values; }
    /// <summary>�w�肵�����O�����l���擾����B</summary>
    /// <param name="name">�l�̖��O</param>
    /// <returns>�l�B������Ȃ������ꍇ��NULL��Ԃ��B</returns>
    VCalValue* getValue(const tstring& name) const;
    /// <summary>�q�̒l���X�g���Z�b�g����B</summary>
    /// <param name="values">�q�̒l���X�g</param>
    inline void setValues(const list<VCalValue*>& values) { this->values = values; }
    /// <summary>�l���X�g�ɒl��ǉ�����B</summary>
    /// <param name="value">�l</param>
    inline void addValue(VCalValue* value) { values.push_back(value); }
    /// <summary>�q�̃u���b�N���X�g���擾����B</summary>
    /// <returns>�u���b�N���X�g</returns>
    inline list<VCalValueBlock*>& getBlocks() { return blocks; }
    /// <summary>�w�肵�����O�����u���b�N���擾����B</summary>
    /// <param name="name">�u���b�N�̖��O</param>
    /// <returns>�u���b�N�B������Ȃ������ꍇ��NULL��Ԃ��B</returns>
    VCalValueBlock* getBlock(const tstring& name) const;
    /// <summary>�q�̃u���b�N���X�g���Z�b�g����B</summary>
    /// <param name="blocks">�q�̃u���b�N���X�g</param>
    inline void setBlocks(const list<VCalValueBlock*>& blocks) { this->blocks = blocks; }
    /// <summary>�u���b�N���X�g�ɒl��ǉ�����B</summary>
    /// <param name="block">�u���b�N</param>
    inline void addBlock(VCalValueBlock* block) { blocks.push_back(block); }

    /// <summary>�u���b�N�̏������������Ԃ��B</summary>
    /// <param name="indent">�C���f���g</param>
    /// <returns>������������</returns>
    tstring toString(const tstring& indent = _T("")) const;

protected:
    tstring name; // �u���b�N��
    list<VCalValue*> values; // �q�̒l���X�g
    list<VCalValueBlock*> blocks; // �q�̃u���b�N���X�g
};

/// <summary>�^�C���]�[���l�u���b�N������킷�N���X</summary>
class VCalTimeZone : public VCalValueBlock {
public:
    /// <summary>�R���X�g���N�^</summary>
    /// <param name="root">�eiCalendar</param>
    VCalTimeZone(const VCalendar* root) : VCalValueBlock(root) {}

    /// <summary>�^�C���]�[��ID���擾����B</summary>
    /// <returns>�^�C���]�[��ID</returns>
    tstring getTzId() const;
    /// <summary>�^�C���]�[���I�t�Z�b�g���擾����B</summary>
    /// <returns>�^�C���]�[���I�t�Z�b�g</returns>
    /// <remarks>���܂ЂƂ�VTIMEZONE�̃t�H�[�}�b�g�𗝉����Ă��Ȃ��̂Ŏ�����̂���K���Ɏ����B</remarks>
    tstring getTimeZoneOffset() const;
};

/// <summary>�\�莞��������킷�N���X</summary>
class VCalTime {
public:
    /// <summary>�R���X�g���N�^</summary>
    /// <param name="time">����</param>
    /// <param name="allday">�S���t���O</param>
    VCalTime(time_t time, bool allday = false) : time(time), allday(allday) {}

    /// <summary>��������</summary>
    enum TIMEFORMAT {
        FORMAT_ALL, // ���t�Ǝ���
        FORMAT_DATE, // ���t�̂�
        FORMAT_TIME // �����̂�
    };

    /// <summary>�������擾����B</summary>
    /// <returns>����</returns>
    time_t getTime() const { return time; }
    /// <summary>�S���t���O���擾����B</summary>
    /// <returns>�S���t���O</returns>
    bool isAllDay() const { return allday; }
    /// <summary>�\�莞�����e�L�X�g������B</summary>
    /// <param name="start">�J�n�����̂Ƃ���true�A�I�������̂Ƃ���false</param>
    /// <param name="format">����</param>
    /// <returns>�\�莞���̃e�L�X�g�\��</returns>
    tstring toString(bool start, TIMEFORMAT format = FORMAT_ALL) const;

private:
    time_t time; // ����
    bool allday; // �S���t���O
};

/// <summary>�C�x���g�l�u���b�N������킷�N���X</summary>
class VCalEvent : public VCalValueBlock {
public:
    /// <summary>�R���X�g���N�^</summary>
    /// <param name="root">�eiCalendar</param>
    VCalEvent(const VCalendar* root) : VCalValueBlock(root) {}

    /// <summary>�C�x���g�J�n����(UTC)���擾����B</summary>
    /// <returns>�C�x���g�J�n����</returns>
    VCalTime* getStartTime() const;
    /// <summary>�C�x���g�I������(UTC)���擾����B</summary>
    /// <returns>�C�x���g�I������</returns>
    VCalTime* getEndTime() const;
    /// <summary>�C�x���g�ꏊ���擾����B</summary>
    /// <returns>�C�x���g�ꏊ</returns>
    tstring getLocation() const;
    /// <summary>�C�x���g�������擾����B</summary>
    /// <returns>�C�x���g����</returns>
    tstring getDate() const;

private:
    /// <summary>�C�x���g����(UTC)���擾����B</summary>
    /// <param name="value">�l</param>
    /// <returns>�C�x���g����</returns>
    VCalTime* parseTime(const VCalValue* value) const;
};

/// <summary>iCalendar�f�[�^������킷�N���X</summary>
class VCalendar : public VCalValueBlock {
public:
    /// <summary>�R���X�g���N�^</summary>
    VCalendar() : VCalValueBlock(NULL) {}

    /// <summary>���\�b�h���擾����B</summary>
    /// <returns>���\�b�h</returns>
    tstring getMethod() const;
    /// <summary>�w�肵���^�C���]�[��ID�����^�C���]�[���u���b�N���擾����B</summary>
    /// <param name="tzId">�^�C���]�[��ID</param>
    /// <returns>�^�C���]�[���u���b�N�B������Ȃ������ꍇ��NULL��Ԃ��B</returns>
    VCalTimeZone* getTimeZone(const tstring& tzId) const;
    /// <summary>iCalendar�Ɋ܂ރC�x���g�u���b�N�̐����擾����B</summary>
    /// <returns>�C�x���g�u���b�N�̐�</returns>
    int getEventCount() const;
    /// <summary>�w�肵���C���f�N�X�����C�x���g�u���b�N���擾����B</summary>
    /// <param name="index">�C���f�N�X</param>
    /// <returns>�C�x���g�u���b�N�B������Ȃ������ꍇ��NULL��Ԃ��B</returns>
    VCalEvent* getEvent(int index) const;
};
