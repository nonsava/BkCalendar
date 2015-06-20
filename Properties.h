#pragma once

/// <summary>プロパティ操作クラス</summary>
class Properties {
    public:
        /// <summary>コンストラクタ</summary>
        /// <param name="inFile">プロパティファイル</param>
        Properties(const TCHAR* iniFile);
        /// <summary>コンストラクタ</summary>
        /// <param name="inFile">プロパティファイル</param>
        Properties(const tstring& iniFile);

        /// <summary>文字列型のプロパティ値を取得する。</summary>
        /// <param name="section">プロパティの所属するセクション名</param>
        /// <param name="name">プロパティのキー名</param>
        /// <param name="value">プロパティ値</param>
        /// <returns>取得できたときはtrueを、できなかったときはfalse</returns>
        bool getString(const TCHAR* section, const TCHAR* name, tstring* value);
        /// <summary>int型のプロパティ値を取得する。</summary>
        /// <param name="section">プロパティの所属するセクション名</param>
        /// <param name="name">プロパティのキー名</param>
        /// <param name="value">プロパティ値</param>
        /// <returns>取得できたときはtrueを、できなかったときはfalse</returns>
        bool getInteger(const TCHAR* section, const TCHAR* name, int* value);
        /// <summary>long型のプロパティ値を取得する。</summary>
        /// <param name="section">プロパティの所属するセクション名</param>
        /// <param name="name">プロパティのキー名</param>
        /// <param name="value">プロパティ値</param>
        /// <returns>取得できたときはtrueを、できなかったときはfalse</returns>
        bool getLong(const TCHAR* section, const TCHAR* name, long* value);
        /// <summary>
        /// 論理値型のプロパティ値を取得する。
        /// 指定値が"TRUE"(大文字小文字区別なし)もしくは"1"のときはtrue、
        /// "FALSE"(大文字小文字区別なし)もしくは"0"のときはfalseとする。
        /// </summary>
        /// <param name="section">プロパティの所属するセクション名</param>
        /// <param name="name">プロパティのキー名</param>
        /// <param name="value">プロパティ値</param>
        /// <returns>取得できたときはtrueを、できなかったときはfalse</returns>
        bool getBool(const TCHAR* section, const TCHAR* name, bool* value);

        /// <summary>文字列型のプロパティ値を永続化する。</summary>
        /// <param name="section">プロパティの所属するセクション名</param>
        /// <param name="name">プロパティのキー名</param>
        /// <param name="value">プロパティ値</param>
        /// <returns>セットできたときはtrueを、できなかったときはfalse</returns>
        bool setString(const TCHAR* section, const TCHAR* name, const TCHAR* value);
        /// <summary>文字列型のプロパティ値を永続化する。</summary>
        /// <param name="section">プロパティの所属するセクション名</param>
        /// <param name="name">プロパティのキー名</param>
        /// <param name="value">プロパティ値</param>
        /// <returns>セットできたときはtrueを、できなかったときはfalse</returns>
        bool setString(const TCHAR* section, const TCHAR* name, const tstring& value);
        /// <summary>int型のプロパティ値を永続化する。</summary>
        /// <param name="section">プロパティの所属するセクション名</param>
        /// <param name="name">プロパティのキー名</param>
        /// <param name="value">プロパティ値</param>
        /// <returns>セットできたときはtrueを、できなかったときはfalse</returns>
        bool setInteger(const TCHAR* section, const TCHAR* name, int value);
        /// <summary>long型のプロパティ値を永続化する。</summary>
        /// <param name="section">プロパティの所属するセクション名</param>
        /// <param name="name">プロパティのキー名</param>
        /// <param name="value">プロパティ値</param>
        /// <returns>セットできたときはtrueを、できなかったときはfalse</returns>
        bool setLong(const TCHAR* section, const TCHAR* name, long value);
        /// <summary>論理値型のプロパティ値を永続化する。</summary>
        /// <param name="section">プロパティの所属するセクション名</param>
        /// <param name="name">プロパティのキー名</param>
        /// <param name="value">プロパティ値</param>
        /// <returns>セットできたときはtrueを、できなかったときはfalse</returns>
        bool setBool(const TCHAR* section, const TCHAR* name, bool value);

    private:
        /// <summary>プロパティファイル名</summary>
        tstring iniFile;
};
