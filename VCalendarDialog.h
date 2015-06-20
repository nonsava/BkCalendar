#pragma once

/// <summary>カレンダーダイアログクラス</summary>
class VCalendarDialog {
public:
    /// <summary>カレンダーダイアログを生成する。</summary>
    /// <param name="hInstance">リソースを提供するモジュールインスタンス</param>
    /// <param name="hWndParent">親ウィンドウハンドル</param>
    void create(HINSTANCE hInstance, HWND hWndParent);
    /// <summary>ダイアログを破棄する。</summary>
    void destroy();

    /// <summary>ダイアログを表示する。</summary>
    void show();
    /// <summary>ダイアログを非表示にする。</summary>
    void hide();

    /// <summary>カレンダー情報をセットする。</summary>
    /// <param name="method">目的</param>
    /// <param name="date">日時</param>
    /// <param name="location">場所</param>
    void setInfo(LPCTSTR method, LPCTSTR date, LPCTSTR location);

private:
    // ダイアログボックスのハンドル
    HWND hDlgWnd;

    // ウィンドウ位置/サイズ
    static int x, y, cx, cy;

    // ウィンドウ移動用
    static bool moving; // ウィンドウ移動中かどうか
    static int offsetX, offsetY; // カーソル位置とウィンドウ左上位置の差

    // ウィンドウリサイズ用
    static bool resizing; // ウィンドウリサイズ中かどうか
    static HCURSOR cursors[11]; // リサイズ用カーソルマップ
    static int minx, miny; // ウィンドウの最小サイズ制限
    static int resizeArea; // リサイズ8方向
    static int editxOrg; // リサイズ開始前のエディットボックス幅
    static RECT rectOrg; // リサイズ開始前のウィンドウ矩形
    static POINT posOrg; // リサイズ開始前のカーソル位置

    /// <summary>ダイアログプロシージャ</summary>
    /// <param name="hDlg">ウィンドウハンドル</param>
    /// <param name="message">ウィンドウメッセージ</param>
    /// <param name="wParam">wParam</param>
    /// <param name="lParam">lParam</param>
    /// <returns>プロシージャ戻り値</returns>
    static INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};
