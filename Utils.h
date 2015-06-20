#pragma once

class CBeckyAPI;

/// <summary>ユーティリティクラス</summary>
class Utils {
private:
    /// <summary>コンストラクタ</summary>
    Utils() {}

public:
    /// <summary>DLLモジュールインスタンスをセットする。</summary>
    /// <param name="h">DLLモジュールインスタンス</param>
    static void setModuleInstance(HINSTANCE h) { hInstance = h; }
    /// <summary>DLLモジュールインスタンスを取得する。</summary>
    /// <returns>DLLモジュールインスタンス</returns>
    static HINSTANCE getModuleInstance() { return hInstance; }
    /// <summary>CBeckyAPIを取得する。</summary>
    /// <returns>CBeckyAPI</returns>
    static CBeckyAPI& getBeckyApi() { return beckyApi; }
    /// <summary>プロパティファイル名を取得する。</summary>
    /// <returns>プロパティファイル名</returns>
    static tstring getPropertiesFile();

    /// <summary>文字列リソースから指定IDの文字列を取得する。</summary>
    /// <param name="uID">文字列ID</param>
    /// <returns>文字列</returns>
    static string LoadStringA(UINT uID);
    /// <summary>文字列リソースから指定IDの文字列を取得する。</summary>
    /// <param name="uID">文字列ID</param>
    /// <returns>文字列</returns>
    static wstring LoadStringW(UINT uID);
    /// <summary>書式化文字列を生成する。</summary>
    /// <param name="format">書式</param>
    /// <param name="...">書式化対象オブジェクト(可変)</param>
    /// <returns>書式化文字列</returns>
    static tstring format(const TCHAR* format, ...);
    /// <summary>マルチバイト文字列からT文字列へ変換する。</summary>
    /// <param name="text">マルチバイト文字列</param>
    /// <returns>T文字列</returns>
    static tstring stot(const string& text);

    /// <summary>
    ///     ファイルにトレースを出力する。
    ///     DLLと同じフォルダ内のBkCalendar.logへ出力する。
    /// </summary>
    /// <param name="format">書式</param>
    /// <param name="...">トレース出力用オブジェクト(可変)</param>
    static void Trace(LPCTSTR format, ...);

private:
    /// <summary>DLLモジュールインスタンス</summary>
    static HINSTANCE hInstance;
    /// <summary>CBeckyAPIインスタンス</summary>
    static CBeckyAPI beckyApi;
};
