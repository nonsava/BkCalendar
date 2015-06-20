#pragma once

/// <summary>�J�����_�[�_�C�A���O�N���X</summary>
class VCalendarDialog {
public:
    /// <summary>�J�����_�[�_�C�A���O�𐶐�����B</summary>
    /// <param name="hInstance">���\�[�X��񋟂��郂�W���[���C���X�^���X</param>
    /// <param name="hWndParent">�e�E�B���h�E�n���h��</param>
    void create(HINSTANCE hInstance, HWND hWndParent);
    /// <summary>�_�C�A���O��j������B</summary>
    void destroy();

    /// <summary>�_�C�A���O��\������B</summary>
    void show();
    /// <summary>�_�C�A���O���\���ɂ���B</summary>
    void hide();

    /// <summary>�J�����_�[�����Z�b�g����B</summary>
    /// <param name="method">�ړI</param>
    /// <param name="date">����</param>
    /// <param name="location">�ꏊ</param>
    void setInfo(LPCTSTR method, LPCTSTR date, LPCTSTR location);

private:
    // �_�C�A���O�{�b�N�X�̃n���h��
    HWND hDlgWnd;

    // �E�B���h�E�ʒu/�T�C�Y
    static int x, y, cx, cy;

    // �E�B���h�E�ړ��p
    static bool moving; // �E�B���h�E�ړ������ǂ���
    static int offsetX, offsetY; // �J�[�\���ʒu�ƃE�B���h�E����ʒu�̍�

    // �E�B���h�E���T�C�Y�p
    static bool resizing; // �E�B���h�E���T�C�Y�����ǂ���
    static HCURSOR cursors[11]; // ���T�C�Y�p�J�[�\���}�b�v
    static int minx, miny; // �E�B���h�E�̍ŏ��T�C�Y����
    static int resizeArea; // ���T�C�Y8����
    static int editxOrg; // ���T�C�Y�J�n�O�̃G�f�B�b�g�{�b�N�X��
    static RECT rectOrg; // ���T�C�Y�J�n�O�̃E�B���h�E��`
    static POINT posOrg; // ���T�C�Y�J�n�O�̃J�[�\���ʒu

    /// <summary>�_�C�A���O�v���V�[�W��</summary>
    /// <param name="hDlg">�E�B���h�E�n���h��</param>
    /// <param name="message">�E�B���h�E���b�Z�[�W</param>
    /// <param name="wParam">wParam</param>
    /// <param name="lParam">lParam</param>
    /// <returns>�v���V�[�W���߂�l</returns>
    static INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};
