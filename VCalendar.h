#pragma once

class VCalendar;

/// <summary>値の基底クラス</summary>
class VCalObject {
public:
    /// <summary>コンストラクタ</summary>
    /// <param name="root">親iCalendar</param>
    VCalObject(const VCalendar* root) : root(root) {}
    /// <summary>デストラクタ</summary>
    virtual ~VCalObject() {}
protected:
    const VCalendar* root; // 親iCalendar
};

/// <summary>値の属性をあらわすクラス</summary>
class VCalAttribute {
public:
    /// <summary>コンストラクタ</summary>
    /// <param name="name">属性名</param>
    /// <param name="value">属性値</param>
    VCalAttribute(const tstring& name, const tstring& value) : name(name), value(value) {}
    /// <summary>コンストラクタ</summary>
    /// <param name="name">属性名</param>
    VCalAttribute(const tstring& name) : name(name) {}

    /// <summary>属性名を取得する。</summary>
    /// <returns>属性名</returns>
    inline tstring getName() const { return name; }
    /// <summary>属性名をセットする。</summary>
    /// <param name="name">属性名</param>
    inline void setName(const tstring& name) { this->name = name; }
    /// <summary>属性値を取得する。</summary>
    /// <returns>属性値</returns>
    inline tstring getValue() const { return value; }
    /// <summary>属性値をセットする。</summary>
    /// <param name="value">属性値</param>
    inline void setValue(const tstring& value) { this->value = value; }

protected:
    tstring name; // 属性名
    tstring value; // 属性値
};

/// <summary>値をあらわすクラス</summary>
class VCalValue : protected VCalObject {
public:
    /// <summary>コンストラクタ</summary>
    /// <param name="root">親iCalendar</param>
    /// <param name="name">名前</param>
    /// <param name="value">値</param>
    /// <param name="attributes">属性リスト</param>
    VCalValue(const VCalendar* root, const tstring& name, const tstring& value, const list<VCalAttribute*>& attributes) : VCalObject(root), name(name), value(value), attributes(attributes) {}
    /// <summary>コンストラクタ</summary>
    /// <param name="root">親iCalendar</param>
    /// <param name="name">名前</param>
    /// <param name="value">値</param>
    VCalValue(const VCalendar* root, const tstring& name, const tstring& value) : VCalObject(root), name(name), value(value) {}
    /// <summary>コンストラクタ</summary>
    /// <param name="root">親iCalendar</param>
    /// <param name="name">名前</param>
    VCalValue(const VCalendar* root, const tstring& name) : VCalObject(root), name(name) {}
    /// <summary>コンストラクタ</summary>
    /// <param name="root">親iCalendar</param>
    VCalValue(const VCalendar* root) : VCalObject(root) {}
    /// <summary>デストラクタ</summary>
    virtual ~VCalValue();

    /// <summary>名前を取得する。</summary>
    /// <returns>名前</returns>
    inline tstring getName() const { return name; }
    /// <summary>名前をセットする。</summary>
    /// <param name="name">名前</param>
    inline void setName(const tstring& name) { this->name = name; }
    /// <summary>値を取得する。</summary>
    /// <returns>値</returns>
    inline tstring getValue() const { return value; }
    /// <summary>値をセットする。</summary>
    /// <param name="value">値</param>
    inline void setValue(const tstring& value) { this->value = value; }
    /// <summary>属性リストを取得する。</summary>
    /// <returns>属性リスト</returns>
    inline list<VCalAttribute*>& getAttributes() { return attributes; }
    /// <summary>指定した属性名を持つ属性を取得する。</summary>
    /// <param name="name">属性名</param>
    /// <returns>属性。見つからなかった場合はNULLを返す。</returns>
    VCalAttribute* getAttribute(const tstring& name) const;
    /// <summary>属性リストをセットする。</summary>
    /// <param name="attributes">属性リスト</param>
    inline void setAttributes(const list<VCalAttribute*>& attributes) { this->attributes = attributes; }
    /// <summary>属性リストに属性を追加する。</summary>
    /// <param name="attribute">属性</param>
    inline void addAttribute(VCalAttribute* attribute) { attributes.push_back(attribute); }

protected:
    tstring name; // 名前
    tstring value; // 値
    list<VCalAttribute*> attributes; // 属性リスト
};

/// <summary>値ブロックをあらわすクラス</summary>
/// <remarks>
/// BEGIN:xxxからEND:xxxまでが値ブロックとなる。
/// xxxが値ブロックの名前(name)となる。
/// </remarks>
class VCalValueBlock : protected VCalObject {
public:
    /// <summary>コンストラクタ</summary>
    /// <param name="root">親iCalendar</param>
    /// <param name="name">ブロック名</param>
    /// <param name="values">子の値リスト</param>
    /// <param name="blocks">子のブロックリスト</param>
    VCalValueBlock(const VCalendar* root, const tstring& name, const list<VCalValue*>& values, const list<VCalValueBlock*>& blocks) : VCalObject(root), name(name), values(values), blocks(blocks) {}
    /// <summary>コンストラクタ</summary>
    /// <param name="root">親iCalendar</param>
    /// <param name="name">ブロック名</param>
    VCalValueBlock(const VCalendar* root, const tstring& name) : VCalObject(root), name(name) {}
    /// <summary>コンストラクタ</summary>
    /// <param name="root">親iCalendar</param>
    VCalValueBlock(const VCalendar* root) : VCalObject(root) {}
    /// <summary>デストラクタ</summary>
    virtual ~VCalValueBlock();

    /// <summary>ブロック名を取得する。</summary>
    /// <returns>ブロック名</returns>
    inline tstring getName() const { return name; }
    /// <summary>ブロック名をセットする。</summary>
    /// <param name="name">ブロック名</param>
    inline void setName(const tstring& name) { this->name = name; }
    /// <summary>子の値リストを取得する。</summary>
    /// <returns>値リスト</returns>
    inline list<VCalValue*>& getValues() { return values; }
    /// <summary>指定した名前を持つ値を取得する。</summary>
    /// <param name="name">値の名前</param>
    /// <returns>値。見つからなかった場合はNULLを返す。</returns>
    VCalValue* getValue(const tstring& name) const;
    /// <summary>子の値リストをセットする。</summary>
    /// <param name="values">子の値リスト</param>
    inline void setValues(const list<VCalValue*>& values) { this->values = values; }
    /// <summary>値リストに値を追加する。</summary>
    /// <param name="value">値</param>
    inline void addValue(VCalValue* value) { values.push_back(value); }
    /// <summary>子のブロックリストを取得する。</summary>
    /// <returns>ブロックリスト</returns>
    inline list<VCalValueBlock*>& getBlocks() { return blocks; }
    /// <summary>指定した名前を持つブロックを取得する。</summary>
    /// <param name="name">ブロックの名前</param>
    /// <returns>ブロック。見つからなかった場合はNULLを返す。</returns>
    VCalValueBlock* getBlock(const tstring& name) const;
    /// <summary>子のブロックリストをセットする。</summary>
    /// <param name="blocks">子のブロックリスト</param>
    inline void setBlocks(const list<VCalValueBlock*>& blocks) { this->blocks = blocks; }
    /// <summary>ブロックリストに値を追加する。</summary>
    /// <param name="block">ブロック</param>
    inline void addBlock(VCalValueBlock* block) { blocks.push_back(block); }

    /// <summary>ブロックの書式化文字列を返す。</summary>
    /// <param name="indent">インデント</param>
    /// <returns>書式化文字列</returns>
    tstring toString(const tstring& indent = _T("")) const;

protected:
    tstring name; // ブロック名
    list<VCalValue*> values; // 子の値リスト
    list<VCalValueBlock*> blocks; // 子のブロックリスト
};

/// <summary>タイムゾーン値ブロックをあらわすクラス</summary>
class VCalTimeZone : public VCalValueBlock {
public:
    /// <summary>コンストラクタ</summary>
    /// <param name="root">親iCalendar</param>
    VCalTimeZone(const VCalendar* root) : VCalValueBlock(root) {}

    /// <summary>タイムゾーンIDを取得する。</summary>
    /// <returns>タイムゾーンID</returns>
    tstring getTzId() const;
    /// <summary>タイムゾーンオフセットを取得する。</summary>
    /// <returns>タイムゾーンオフセット</returns>
    /// <remarks>いまひとつVTIMEZONEのフォーマットを理解していないので取れるものから適当に実装。</remarks>
    tstring getTimeZoneOffset() const;
};

/// <summary>予定時刻をあらわすクラス</summary>
class VCalTime {
public:
    /// <summary>コンストラクタ</summary>
    /// <param name="time">日時</param>
    /// <param name="allday">全日フラグ</param>
    VCalTime(time_t time, bool allday = false) : time(time), allday(allday) {}

    /// <summary>日時書式</summary>
    enum TIMEFORMAT {
        FORMAT_ALL, // 日付と時刻
        FORMAT_DATE, // 日付のみ
        FORMAT_TIME // 時刻のみ
    };

    /// <summary>日時を取得する。</summary>
    /// <returns>日時</returns>
    time_t getTime() const { return time; }
    /// <summary>全日フラグを取得する。</summary>
    /// <returns>全日フラグ</returns>
    bool isAllDay() const { return allday; }
    /// <summary>予定時刻をテキスト化する。</summary>
    /// <param name="start">開始時刻のときはtrue、終了時刻のときはfalse</param>
    /// <param name="format">書式</param>
    /// <returns>予定時刻のテキスト表現</returns>
    tstring toString(bool start, TIMEFORMAT format = FORMAT_ALL) const;

private:
    time_t time; // 日時
    bool allday; // 全日フラグ
};

/// <summary>イベント値ブロックをあらわすクラス</summary>
class VCalEvent : public VCalValueBlock {
public:
    /// <summary>コンストラクタ</summary>
    /// <param name="root">親iCalendar</param>
    VCalEvent(const VCalendar* root) : VCalValueBlock(root) {}

    /// <summary>イベント開始時刻(UTC)を取得する。</summary>
    /// <returns>イベント開始時刻</returns>
    VCalTime* getStartTime() const;
    /// <summary>イベント終了時刻(UTC)を取得する。</summary>
    /// <returns>イベント終了時刻</returns>
    VCalTime* getEndTime() const;
    /// <summary>イベント場所を取得する。</summary>
    /// <returns>イベント場所</returns>
    tstring getLocation() const;
    /// <summary>イベント日時を取得する。</summary>
    /// <returns>イベント日時</returns>
    tstring getDate() const;

private:
    /// <summary>イベント時刻(UTC)を取得する。</summary>
    /// <param name="value">値</param>
    /// <returns>イベント時刻</returns>
    VCalTime* parseTime(const VCalValue* value) const;
};

/// <summary>iCalendarデータをあらわすクラス</summary>
class VCalendar : public VCalValueBlock {
public:
    /// <summary>コンストラクタ</summary>
    VCalendar() : VCalValueBlock(NULL) {}

    /// <summary>メソッドを取得する。</summary>
    /// <returns>メソッド</returns>
    tstring getMethod() const;
    /// <summary>指定したタイムゾーンIDを持つタイムゾーンブロックを取得する。</summary>
    /// <param name="tzId">タイムゾーンID</param>
    /// <returns>タイムゾーンブロック。見つからなかった場合はNULLを返す。</returns>
    VCalTimeZone* getTimeZone(const tstring& tzId) const;
    /// <summary>iCalendarに含むイベントブロックの数を取得する。</summary>
    /// <returns>イベントブロックの数</returns>
    int getEventCount() const;
    /// <summary>指定したインデクスを持つイベントブロックを取得する。</summary>
    /// <param name="index">インデクス</param>
    /// <returns>イベントブロック。見つからなかった場合はNULLを返す。</returns>
    VCalEvent* getEvent(int index) const;
};
