#include "stdafx.h"
#include "resource.h"
#include "Utils.h"
#include "Properties.h"
#include "VCalendarDialog.h"


int VCalendarDialog::x;
int VCalendarDialog::y;
int VCalendarDialog::cx;
int VCalendarDialog::cy;

bool VCalendarDialog::moving;
int VCalendarDialog::offsetX;
int VCalendarDialog::offsetY;

bool VCalendarDialog::resizing;
HCURSOR VCalendarDialog::cursors[11];
int VCalendarDialog::minx;
int VCalendarDialog::miny;
int VCalendarDialog::resizeArea;
int VCalendarDialog::editxOrg;
RECT VCalendarDialog::rectOrg;
POINT VCalendarDialog::posOrg;


/// <summary>ダイアログを生成する。</summary>
/// <param name="hInstance">モジュールインスタンス</param>
/// <param name="hWndParent">親ウィンドウハンドル</param>
void VCalendarDialog::create(HINSTANCE hInstance, HWND hWndParent) {
    // カレンダーウィンドウの作成
    hDlgWnd = ::CreateDialog(hInstance, MAKEINTRESOURCE(IDD_VCALENDAR_DIALOG), hWndParent, DialogProc);

    // 情報名スタティックテキストを太字化
    HWND hStatic = ::GetDlgItem(hDlgWnd, IDC_METHOD_STATIC);
    HFONT hFont = (HFONT) ::SendMessage(hStatic, WM_GETFONT, 0, 0L);
    LOGFONT lf;
    ::GetObject(hFont, sizeof(LOGFONT), &lf);
    lf.lfWeight = FW_BOLD;
    hFont = ::CreateFontIndirect(&lf);
    ::SendMessage(hStatic, WM_SETFONT, (WPARAM) hFont, MAKELPARAM(FALSE, 0));
    hStatic = ::GetDlgItem(hDlgWnd, IDC_DATE_STATIC);
    ::SendMessage(hStatic, WM_SETFONT, (WPARAM) hFont, MAKELPARAM(FALSE, 0));
    hStatic = ::GetDlgItem(hDlgWnd, IDC_LOCATION_STATIC);
    ::SendMessage(hStatic, WM_SETFONT, (WPARAM) hFont, MAKELPARAM(FALSE, 0));

    // リサイズ用カーソルマップ
    if (cursors[0] == NULL) {
        HCURSOR cursor = (HCURSOR) ::GetClassLong(hDlgWnd, GCL_HCURSOR);
        cursors[0] = cursor;
        cursor = ::LoadCursor(NULL, IDC_SIZEWE);
        cursors[1] = cursor;
        cursors[2] = cursor;
        cursor = ::LoadCursor(NULL, IDC_SIZENS);
        cursors[4] = cursor;
        cursors[8] = cursor;
        cursor = ::LoadCursor(NULL, IDC_SIZENESW);
        cursors[9] = cursor;
        cursors[6] = cursor;
        cursor = ::LoadCursor(NULL, IDC_SIZENWSE);
        cursors[5] = cursor;
        cursors[10] = cursor;
    }
}

/// <summary>ダイアログを破棄する。</summary>
void VCalendarDialog::destroy() {
    ::DestroyWindow(hDlgWnd);
}

/// <summary>ダイアログを表示する。</summary>
void VCalendarDialog::show() {
    ::ShowWindow(hDlgWnd, SW_SHOWNOACTIVATE);
}

/// <summary>ダイアログを非表示にする。</summary>
void VCalendarDialog::hide() {
    ::ShowWindow(hDlgWnd, SW_HIDE);
}

/// <summary>カレンダー情報をセットする。</summary>
/// <param name="method">目的</param>
/// <param name="date">日時</param>
/// <param name="location">場所</param>
void VCalendarDialog::setInfo(LPCTSTR method, LPCTSTR date, LPCTSTR location) {
    ::SetWindowText(::GetDlgItem(hDlgWnd, IDC_METHOD_EDIT), method);
    ::SetWindowText(::GetDlgItem(hDlgWnd, IDC_DATE_EDIT), date);
    ::SetWindowText(::GetDlgItem(hDlgWnd, IDC_LOCATION_EDIT), location);
}


/// <summary>マウスイベントのカーソル位置をスクリーン座標系に変換して返す。</summary>
/// <param name="hWnd">ウィンドウ</param>
/// <param name="hWnd">マウスイベントのLPARAM(クライアント座標カーソル位置)</param>
/// <returns>スクリーン座標カーソル位置</returns>
static POINT getCursorScreenPos(HWND hWnd, LPARAM lParam) {
    POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
    ::ClientToScreen(hWnd, &point);
    return point;
}

/// <summary>リサイズ帯の幅</summary>
static int RESIZE_WIDTH = 5;

/// <summary>カーソル位置のヒットテスト</summary>
/// <param name="hWnd">ウィンドウ</param>
/// <param name="pos">カーソル位置(スクリーン座標)</param>
/// <returns>カーソル位置情報。1=左端, 2=右端, 4=上端, 8=下端, 5=左上角, 6=右上角, 9=左下角, 10=右下角, 0=それ以外。</returns>
static int hitTest(HWND hWnd, const POINT& pos) {
    RECT rect;
    ::GetWindowRect(hWnd, &rect);
    int x1 = pos.x - rect.left;
    int x2 = rect.right - pos.x;
    int y1 = pos.y - rect.top;
    int y2 = rect.bottom - pos.y;
    int ret = (0 <= x1 && x1 <= RESIZE_WIDTH) ? 1 : 0;
    ret |= (0 <= x2 && x2 <= RESIZE_WIDTH) ? 2 : 0;
    ret |= (0 <= y1 && y1 <= RESIZE_WIDTH) ? 4 : 0;
    ret |= (0 <= y2 && y2 <= RESIZE_WIDTH) ? 8 : 0;
    return ret;
}


/// <summary>ダイアログプロシージャ</summary>
/// <param name="hDlg">ウィンドウハンドル</param>
/// <param name="message">ウィンドウメッセージ</param>
/// <param name="wParam">wParam</param>
/// <param name="lParam">lParam</param>
/// <returns>プロシージャ戻り値</returns>
INT_PTR CALLBACK VCalendarDialog::DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_INITDIALOG: {
            // ウィンドウの初期サイズ(最小サイズ)を取得
            RECT rect;
            ::GetWindowRect(hDlg, &rect);
            minx = rect.right - rect.left;
            miny = rect.bottom - rect.top;

            // カレンダーウィンドウの位置/サイズ復元
            Properties props(Utils::getPropertiesFile());
            props.getInteger(_T("window"), _T("x"), &x);
            props.getInteger(_T("window"), _T("y"), &y);
            props.getInteger(_T("window"), _T("cx"), &cx);
            props.getInteger(_T("window"), _T("cy"), &cy);

            HWND hParent = ::GetParent(hDlg);
            POINT point = { x, y };
            ::ClientToScreen(hParent, &point);
            cx = max(cx, minx);
            cy = max(cy, miny);
            ::SetWindowPos(hDlg, NULL, point.x, point.y, cx, cy, SWP_NOZORDER | SWP_NOACTIVATE);

            HWND hEdit = ::GetDlgItem(hDlg, IDC_METHOD_EDIT);
            ::GetWindowRect(hEdit, &rect);
            cx = rect.right - rect.left + (cx - minx);
            cy = rect.bottom - rect.top;
            ::SetWindowPos(hEdit, NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER);
            hEdit = ::GetDlgItem(hDlg, IDC_DATE_EDIT);
            ::SetWindowPos(hEdit, NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER);
            hEdit = ::GetDlgItem(hDlg, IDC_LOCATION_EDIT);
            ::SetWindowPos(hEdit, NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER);
            return (INT_PTR) FALSE;
        }

        case WM_DESTROY: {
            // カレンダーウィンドウの位置/サイズ保存
            RECT rect;
            ::GetWindowRect(hDlg, &rect);
            HWND hParent = ::GetParent(hDlg);
            POINT point = { rect.left, rect.top };
            ::ScreenToClient(hParent, &point);

            Properties props(Utils::getPropertiesFile());
            props.setInteger(_T("window"), _T("x"), point.x);
            props.setInteger(_T("window"), _T("y"), point.y);
            props.setInteger(_T("window"), _T("cx"), rect.right - rect.left);
            props.setInteger(_T("window"), _T("cy"), rect.bottom - rect.top);
            return (INT_PTR) TRUE;
        }

        case WM_LBUTTONDOWN: {
            RECT rect;
            ::GetWindowRect(hDlg, &rect);
            POINT pos = getCursorScreenPos(hDlg, lParam);
            int area = hitTest(hDlg, pos);
            if (area == 0) {
                // ウィンドウ移動開始
                offsetX = pos.x - rect.left;
                offsetY = pos.y - rect.top;
                moving = true;
            } else {
                // ウィンドウリサイズ開始
                resizeArea = area;
                posOrg = pos;
                rectOrg = rect;
                resizing = true;

                HWND hEdit = ::GetDlgItem(hDlg, IDC_METHOD_EDIT);
                ::GetWindowRect(hEdit, &rect);
                editxOrg = rect.right - rect.left;
            }
            ::SetCapture(hDlg);
            return (INT_PTR) TRUE;
        }

        case WM_LBUTTONUP:
            if (moving) {
                // ウィンドウ移動終了
                ::ReleaseCapture();
                moving = false;
            } else if (resizing) {
                // ウィンドウリサイズ終了
                ::ReleaseCapture();
                resizing = false;
            }
            return (INT_PTR) TRUE;

        case WM_MOUSEMOVE:
            if (moving) {
                // ウィンドウ移動中
                POINT pos = getCursorScreenPos(hDlg, lParam);
                ::SetWindowPos(hDlg, NULL, pos.x - offsetX, pos.y - offsetY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
            } else if (resizing) {
                // ウィンドウリサイズ中
                POINT pos = getCursorScreenPos(hDlg, lParam);
                int x = rectOrg.left;
                int y = rectOrg.top;
                int cx = rectOrg.right - rectOrg.left;
                int cy = rectOrg.bottom - rectOrg.top;
                if ((resizeArea & 1) != 0) {
                    x += pos.x - posOrg.x;
                    cx -= pos.x - posOrg.x;
                    if (cx < minx) {
                        x -= minx - cx;
                        cx = minx;
                    }
                } else if ((resizeArea & 2) != 0) {
                    cx += pos.x - posOrg.x;
                    cx = max(cx, minx);
                }
                if ((resizeArea & 4) != 0) {
                    y += pos.y - posOrg.y;
                    cy -= pos.y - posOrg.y;
                    if (cy < miny) {
                        y -= miny - cy;
                        cy = miny;
                    }
                } else if ((resizeArea & 8) != 0) {
                    cy += pos.y - posOrg.y;
                    cy = max(cy, miny);
                }
                ::SetWindowPos(hDlg, NULL, x, y, cx, cy, SWP_NOZORDER);

                RECT rect;
                HWND hEdit = ::GetDlgItem(hDlg, IDC_METHOD_EDIT);
                ::GetWindowRect(hEdit, &rect);
                cx = editxOrg + (cx - (rectOrg.right - rectOrg.left));
                cy = rect.bottom - rect.top;
                ::SetWindowPos(hEdit, NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER);
                hEdit = ::GetDlgItem(hDlg, IDC_DATE_EDIT);
                ::SetWindowPos(hEdit, NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER);
                hEdit = ::GetDlgItem(hDlg, IDC_LOCATION_EDIT);
                ::SetWindowPos(hEdit, NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER);
            }
            return (INT_PTR) TRUE;

        case WM_SETCURSOR: {
            // カーソル表示の変更
            int area = 0;
            if (LOWORD(lParam) != HTBORDER) {
                POINT pos;
                ::GetCursorPos(&pos);
                area = hitTest(hDlg, pos);
            }
            ::SetCursor(cursors[area]);
            return (INT_PTR) TRUE;
        }
    }
    return (INT_PTR) FALSE;
}
