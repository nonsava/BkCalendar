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


/// <summary>�_�C�A���O�𐶐�����B</summary>
/// <param name="hInstance">���W���[���C���X�^���X</param>
/// <param name="hWndParent">�e�E�B���h�E�n���h��</param>
void VCalendarDialog::create(HINSTANCE hInstance, HWND hWndParent) {
    // �J�����_�[�E�B���h�E�̍쐬
    hDlgWnd = ::CreateDialog(hInstance, MAKEINTRESOURCE(IDD_VCALENDAR_DIALOG), hWndParent, DialogProc);

    // ��񖼃X�^�e�B�b�N�e�L�X�g�𑾎���
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

    // ���T�C�Y�p�J�[�\���}�b�v
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

/// <summary>�_�C�A���O��j������B</summary>
void VCalendarDialog::destroy() {
    ::DestroyWindow(hDlgWnd);
}

/// <summary>�_�C�A���O��\������B</summary>
void VCalendarDialog::show() {
    ::ShowWindow(hDlgWnd, SW_SHOWNOACTIVATE);
}

/// <summary>�_�C�A���O���\���ɂ���B</summary>
void VCalendarDialog::hide() {
    ::ShowWindow(hDlgWnd, SW_HIDE);
}

/// <summary>�J�����_�[�����Z�b�g����B</summary>
/// <param name="method">�ړI</param>
/// <param name="date">����</param>
/// <param name="location">�ꏊ</param>
void VCalendarDialog::setInfo(LPCTSTR method, LPCTSTR date, LPCTSTR location) {
    ::SetWindowText(::GetDlgItem(hDlgWnd, IDC_METHOD_EDIT), method);
    ::SetWindowText(::GetDlgItem(hDlgWnd, IDC_DATE_EDIT), date);
    ::SetWindowText(::GetDlgItem(hDlgWnd, IDC_LOCATION_EDIT), location);
}


/// <summary>�}�E�X�C�x���g�̃J�[�\���ʒu���X�N���[�����W�n�ɕϊ����ĕԂ��B</summary>
/// <param name="hWnd">�E�B���h�E</param>
/// <param name="hWnd">�}�E�X�C�x���g��LPARAM(�N���C�A���g���W�J�[�\���ʒu)</param>
/// <returns>�X�N���[�����W�J�[�\���ʒu</returns>
static POINT getCursorScreenPos(HWND hWnd, LPARAM lParam) {
    POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
    ::ClientToScreen(hWnd, &point);
    return point;
}

/// <summary>���T�C�Y�т̕�</summary>
static int RESIZE_WIDTH = 5;

/// <summary>�J�[�\���ʒu�̃q�b�g�e�X�g</summary>
/// <param name="hWnd">�E�B���h�E</param>
/// <param name="pos">�J�[�\���ʒu(�X�N���[�����W)</param>
/// <returns>�J�[�\���ʒu���B1=���[, 2=�E�[, 4=��[, 8=���[, 5=����p, 6=�E��p, 9=�����p, 10=�E���p, 0=����ȊO�B</returns>
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


/// <summary>�_�C�A���O�v���V�[�W��</summary>
/// <param name="hDlg">�E�B���h�E�n���h��</param>
/// <param name="message">�E�B���h�E���b�Z�[�W</param>
/// <param name="wParam">wParam</param>
/// <param name="lParam">lParam</param>
/// <returns>�v���V�[�W���߂�l</returns>
INT_PTR CALLBACK VCalendarDialog::DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_INITDIALOG: {
            // �E�B���h�E�̏����T�C�Y(�ŏ��T�C�Y)���擾
            RECT rect;
            ::GetWindowRect(hDlg, &rect);
            minx = rect.right - rect.left;
            miny = rect.bottom - rect.top;

            // �J�����_�[�E�B���h�E�̈ʒu/�T�C�Y����
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
            // �J�����_�[�E�B���h�E�̈ʒu/�T�C�Y�ۑ�
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
                // �E�B���h�E�ړ��J�n
                offsetX = pos.x - rect.left;
                offsetY = pos.y - rect.top;
                moving = true;
            } else {
                // �E�B���h�E���T�C�Y�J�n
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
                // �E�B���h�E�ړ��I��
                ::ReleaseCapture();
                moving = false;
            } else if (resizing) {
                // �E�B���h�E���T�C�Y�I��
                ::ReleaseCapture();
                resizing = false;
            }
            return (INT_PTR) TRUE;

        case WM_MOUSEMOVE:
            if (moving) {
                // �E�B���h�E�ړ���
                POINT pos = getCursorScreenPos(hDlg, lParam);
                ::SetWindowPos(hDlg, NULL, pos.x - offsetX, pos.y - offsetY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
            } else if (resizing) {
                // �E�B���h�E���T�C�Y��
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
            // �J�[�\���\���̕ύX
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
