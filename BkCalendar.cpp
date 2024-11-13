////////////////////////////////////////////////////////////////////////////////////
// Template file for plugin.
//
// You can modify and redistribute this file without any permission.
//
// Note:
// Create a sub folder under "PlugInSDK" folder. e.g. "PlugInSDK\MyProject\" and
// place your project files there.

#include "stdafx.h"
#include "../BeckyAPI.h"
#include "../BkCommon.h"
#include "resource.h"
#include "Utils.h"
#include "VCalendar.h"
#include "VCalendarParser.h"
#include "VCalendarDialog.h"


/////////////////////////////////////////////////////////////////////////////
// DLL entry point
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    Utils::setModuleInstance((HINSTANCE) hModule);
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            if (!Utils::getBeckyApi().InitAPI()) {
                return FALSE;
            }
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////
// static functions

/// <summary>カレンダーダイアログ</summary>
static VCalendarDialog dialog;

/// <summary>メソッドマップ</summary>
static map<tstring, tstring> methodMap;
/// <summary>部分ステータスマップ</summary>
static map<tstring, tstring> partStatMap;

/// <summary>メソッドをパースする。</summary>
/// <param name="text">メソッド</param>
/// <returns>メソッド文字列</returns>
static tstring parseMethod(LPCTSTR text) {
    if (methodMap.empty()) {
        methodMap[tstring(_T("REQUEST"))] = Utils::LoadString(IDS_REQUEST_METHOD); // 招待
        methodMap[tstring(_T("CANCEL"))] = Utils::LoadString(IDS_CANCEL_METHOD); // キャンセル
        methodMap[tstring(_T("REPLY"))] = Utils::LoadString(IDS_REPLY_METHOD); // 返信
        methodMap[tstring(_T("PUBLISH"))] = Utils::LoadString(IDS_PUBLISH_METHOD); // 発行
        methodMap[tstring(_T("ADD"))] = Utils::LoadString(IDS_ADD_METHOD); // 追加
        methodMap[tstring(_T("REFRESH"))] = Utils::LoadString(IDS_REFRESH_METHOD); // 最新情報要求
        methodMap[tstring(_T("COUNTER"))] = Utils::LoadString(IDS_COUNTER_METHOD); // 対案
        methodMap[tstring(_T("DECLINECOUNTER"))] = Utils::LoadString(IDS_DECLINECOUNTER_METHOD); // 対案の拒否
    }
    return methodMap.count(text) == 0 ? text : methodMap[text];
}

/// <summary>部分ステータスをパースする。</summary>
/// <param name="text">部分ステータス</param>
/// <returns>部分ステータス文字列</returns>
static tstring parsePartStat(LPCTSTR text) {
    if (partStatMap.empty()) {
        partStatMap[tstring(_T("ACCEPTED"))] = Utils::LoadString(IDS_ACCEPTED_PARTSTAT); // 承諾
        partStatMap[tstring(_T("TENTATIVE"))] = Utils::LoadString(IDS_TENTATIVE_PARTSTAT); // 参加未定
        partStatMap[tstring(_T("DECLINED"))] = Utils::LoadString(IDS_DECLINED_PARTSTAT); // 辞退
        partStatMap[tstring(_T("NEEDS-ACTION"))] = Utils::LoadString(IDS_NEED_ACTION_PARTSTAT); // 要対応
        partStatMap[tstring(_T("DELEGATED"))] = Utils::LoadString(IDS_DELEGATED_PARTSTAT); // 委任
    }
    return partStatMap.count(text) == 0 ? text : partStatMap[text];
}


/////////////////////////////////////////////////////////////////////////////////////////////
// Callbacks from Becky!

#ifdef __cplusplus
extern "C"{
#endif

    ////////////////////////////////////////////////////////////////////////
    // Called when plug-in information is being retrieved.
    typedef struct tagBKPLUGININFO {
        char szPlugInName[80]; // Name of the plug-in
        char szVendor[80]; // Name of the vendor
        char szVersion[80]; // Version string
        char szDescription[256]; // Short description about this plugin
    } BKPLUGININFO, *LPBKPLUGININFO;

    int WINAPI BKC_OnPlugInInfo(LPBKPLUGININFO lpPlugInInfo) {
        strcpy(lpPlugInInfo->szPlugInName, Utils::LoadStringA(IDS_PLUGIN_NAME).c_str());
        strcpy(lpPlugInInfo->szVendor, Utils::LoadStringA(IDS_PLUGIN_VENDER).c_str());
        strcpy(lpPlugInInfo->szVersion, Utils::LoadStringA(IDS_PLUGIN_VERSION).c_str());
        strcpy(lpPlugInInfo->szDescription, Utils::LoadStringA(IDS_PLUGIN_DESCRIPTION).c_str());
        return 0;
    }

    ////////////////////////////////////////////////////////////////////////
    // Called when the program is started and the main window is created.
    int WINAPI BKC_OnStart() {
        // カレンダー情報表示ダイアログの作成
        HWND hWnd, hDummy;
        Utils::getBeckyApi().GetWindowHandles(&hWnd, &hDummy, &hDummy, &hDummy);
        dialog.create(Utils::getModuleInstance(), hWnd);

        return 0;
    }

    ////////////////////////////////////////////////////////////////////////
    // Called when the main window is closing.
    int WINAPI BKC_OnExit() {
        // カレンダー情報表示ダイアログの破棄
        dialog.destroy();

        return 0; // 0=quit; -1=not quit
    }

    /// <summary>メールからiCalendarを探す。
    /// iCalendarが見つからなかった場合はNULLを返す。</summary>
    /// <param name="item">MIMEアイテム</param>
    static VCalendar* FindCalendar(CMIMEItem* item) {
        VCalendarParser parser(item);
        VCalendar* calendar = parser.parseCalendar();
        if (calendar != NULL) {
            return calendar;
        }

        item = item->GetChild();
        while (item != NULL) {
            calendar = FindCalendar(item);
            if (calendar != NULL) {
                break;
            }
            item = item->GetNext();
        }
        return calendar;
    }

    ////////////////////////////////////////////////////////////////////////
    // Called when a mail is selected.
    int WINAPI BKC_OnOpenMail(LPCTSTR lpMailID) {
        // iCalendarを取得する
        LPSTR source = Utils::getBeckyApi().GetSource(lpMailID);
        if (source == NULL) {
            // カレンダー情報ウィンドウを閉じる
            dialog.hide();
            return 0;
        }
        CMIMEItem item;
        item.FromString(source);
        VCalendar* calendar = FindCalendar(&item);
        if (calendar == NULL) {
            // カレンダー情報ウィンドウを閉じる
            dialog.hide();
            return 0;
        }

        tstring methodOrg = calendar->getMethod();
        tstring method = parseMethod(methodOrg.c_str());
        tstring location;
        tstring date;
        const VCalEvent* event = calendar->getEvent(0);
        if (event != NULL) {
            location = event->getLocation();
            date = event->getDate();

            if (methodOrg == _T("REPLY")) {
                VCalValue* value = event->getValue(_T("ATTENDEE"));
                if (value != NULL) {
                    VCalAttribute* attr = value->getAttribute(_T("PARTSTAT"));
                    if (attr != NULL) {
                        method += _T(": ") + parsePartStat(attr->getValue().c_str());
                    }
                }
            }

            if (event->getValue(_T("RRULE")) != NULL) {
                date += Utils::LoadString(IDS_REPEAT_EVENT);
            }
        }

        // カレンダー情報ウィンドウを開く
        dialog.setInfo(method.c_str(), date.c_str(), location.c_str());
        dialog.show();

        delete calendar;

        return 0;
    }

    ////////////////////////////////////////////////////////////////////////
    // Called when a folder is opened.
    int WINAPI BKC_OnOpenFolder(LPCTSTR lpFolderID) {
        // カレンダー情報ウィンドウを閉じる
        dialog.hide();

        return 0;
    }





    ////////////////////////////////////////////////////////////////////////
    // Called when menu is intialized.
    int WINAPI BKC_OnMenuInit(HWND hWnd, HMENU hMenu, int nType) {
        switch (nType) {
            case BKC_MENU_MAIN:
            {
                /* Sample of adding menu items
                HMENU hSubMenu = GetSubMenu(hMenu, 4);
                // Define CmdProc as "void WINAPI CmdProc(HWND, LPARAM)"
                UINT nID = bka.RegisterCommand("Information about this Command", nType,CmdProc);
                AppendMenu(hSubMenu, MF_STRING, nID, "&Menu item");
                */
                /* If needed, you can register the command UI update callback.
                // Define CmdUIProc as "UINT WINAPI CmdUIProc(HWND, LPARAM)"
                bka.RegisterUICallback(nID, CmdUIProc);
                */
            }
            break;
            case BKC_MENU_LISTVIEW:
                break;
            case BKC_MENU_TREEVIEW:
                break;
            case BKC_MENU_MSGVIEW:
                break;
            case BKC_MENU_MSGEDIT:
                break;
            case BKC_MENU_TASKTRAY:
                break;
            case BKC_MENU_COMPOSE:
                break;
            case BKC_MENU_COMPEDIT:
                break;
            case BKC_MENU_COMPREF:
                break;
            default:
                break;
        }
        // Always return 0.
        return 0;
    }

    ////////////////////////////////////////////////////////////////////////
    // Called every minute.
    int WINAPI BKC_OnEveryMinute() {
        // Always return 0.
        return 0;
    }

    ////////////////////////////////////////////////////////////////////////
    // Called when a compose windows is opened.
    int WINAPI BKC_OnOpenCompose(HWND hWnd, int nMode/* See COMPOSE_MODE_* in BeckyApi.h */) {
        // Always return 0.
        return 0;
    }

    ////////////////////////////////////////////////////////////////////////
    // Called when the composing message is saved.
    int WINAPI BKC_OnOutgoing(HWND hWnd, int nMode/* 0:SaveToOutbox, 1:SaveToDraft, 2:SaveToReminder*/) {
        // Return -1 if you do not want to send it yet.
        return 0;
    }

    ////////////////////////////////////////////////////////////////////////
    // Called when a key is pressed.
    int WINAPI BKC_OnKeyDispatch(HWND hWnd, int nKey/* virtual key code */, int nShift/* Shift state. 0x40=Shift, 0x20=Ctrl, 0x60=Shift+Ctrl, 0xfe=Alt*/) {
        // Return TRUE if you want to suppress subsequent command associated to this key.
        return 0;
    }

    ////////////////////////////////////////////////////////////////////////
    // Called when a message is retrieved and saved to a folder
    int WINAPI BKC_OnRetrieve(LPCTSTR lpMessage/* Message source*/, LPCTSTR lpMailID/* Mail ID*/) {
        // Always return 0.
        return 0;
    }

    ////////////////////////////////////////////////////////////////////////
    // Called when a message is spooled
    int WINAPI BKC_OnSend(LPCTSTR lpMessage/* Message source */) {
        // Return BKC_ONSEND_PROCESSED, if you have processed this message
        // and don't need Becky! to send it.
        // Becky! will move this message to Sent box when the sending
        // operation is done.
        // CAUTION: You are responsible for the destination of this
        // message if you return BKC_ONSEND_PROCESSED.

        // Return BKC_ONSEND_ERROR, if you want to cancel the sending operation.
        // You are responsible for displaying an error message.

        // Return 0 to proceed the sending operation.
        return 0;
    }

    ////////////////////////////////////////////////////////////////////////
    // Called when all messages are retrieved
    int WINAPI BKC_OnFinishRetrieve(int nNumber/* Number of messages*/) {
        // Always return 0.
        return 0;
    }


    ////////////////////////////////////////////////////////////////////////
    // Called when plug-in setup is needed.
    int WINAPI BKC_OnPlugInSetup(HWND hWnd) {
        // Return nonzero if you have processed.
        // return 1;
        return 0;
    }

    ////////////////////////////////////////////////////////////////////////
    // Called when drag and drop operation occurs.
    int WINAPI BKC_OnDragDrop(LPCSTR lpTgt, LPCSTR lpSrc, int nCount, int dropEffect) {
        /*
        lpTgt:	A folder ID of the target folder.
        You can assume it is a root mailbox, if the string
        contains only one '\' character.
        lpSrc:	Either a folder ID or mail IDs. Multiple mail IDs are
        separated by '\n' (0x0a).
        You can assume it is a folder ID, if the string
        doesn't contain '?' character.
        nCount:	Number of items to be dropped.
        It can be more than one, if you drop mail items.
        dropEffect: Type of drag and drop operation
        1: Copy
        2: Move
        4: Link (Used for filtering setup in Becky!)
        */
        // If you want to cancel the default drag and drop action,
        // return -1;
        // Do not assume the default action (copy, move, etc.) is always
        // processed, because other plug-ins might cancel the operation.
        return 0;
    }


    ////////////////////////////////////////////////////////////////////////
    // Called when a message was retrieved and about to be filtered.
    int WINAPI BKC_OnBeforeFilter2(LPCSTR lpMessage, LPCSTR lpMailBox, int* lpnAction, char** lppParam) {
        /*
        lpMessage: A complete message source, which ends with
        "<CRLF>.<CRLF>".
        lpnAction:	[out] Returns the filtering action to be applied.
        #define ACTION_NOTHING		-1	// Do nothing
        #define ACTION_MOVEFOLDER	0	// Move to a folder
        #define ACTION_COLORLABEL	1	// Set the color label
        #define ACTION_SETFLAG		2	// Set the flag
        #define ACTION_SOUND		3	// Make a sound
        #define ACTION_RUNEXE		4	// Run executable file
        #define ACTION_REPLY		5	// Reply to the message
        #define ACTION_FORWARD		6	// Forward the message
        #define ACTION_LEAVESERVER	7	// Leave/delete on the server.
        #define ACTION_ADDHEADER	8	// Add "X-" header to the top of the message. (Plug-in only feature)
        lpMailBox: ID of the mailbox that is retrieving the message. (XXXXXXXX.mb\)
        lppParam:	[out] Returns the pointer to the filtering parameter string.
        ACTION_MOVEFOLDER:	Folder name.
        e.g. XXXXXXXX.mb\FolderName\
        or \FolderName\ (begin with '\') to use
        the mailbox the message belongs.
        ACTION_COLORLABEL:	Color code(BGR) in hexadecimal. e.g. 0088FF
        ACTION_SETFLAG:		"F" to set flag. "R" to set read.
        ACTION_SOUND:		Name of the sound file.
        ACTION_RUNEXE:		Command line to execute. %1 will be replaced with the path to the file that contains the entire message.
        ACTION_REPLY:		Path to the template file without extension.
        e.g. #Reply\MyReply
        ACTION_FORWARD:		Path to the template file without extension. + "*" + Address to forward.
        e.g. #Forward\MyForward*mail@address
        *mail@address (no template)
        ACTION_LEAVESERVER:	The string consists of one or two decimals. The second decimal is optional.
        Two decimals must be separated with a space.
        First decimal	1: Delete the message from the server.
        0: Leave the message on the server.
        Second decimal	1: Do not store the message to the folder.
        0: Store the message to the folder. (default action)
        e.g. 0 (Leave the message on the server.)
        1 1 (Delete the message on the server and do not save. (Means KILL))
        ACTION_ADDHEADER	"X-Header:data" that will be added at the top of the incoming message.
        You can specify multiple headers by separating CRLF, but each header must
        begin with "X-". e.g. "X-Plugindata1: test\r\nX-Plugindata2: test2";
        */

        /* Return values
        BKC_FILTER_DEFAULT	Do nothing and apply default filtering rules.
        BKC_FILTER_PASS		Apply default filtering rules after applying the rule it returns.
        BKC_FILTER_DONE		Do not apply default rules.
        BKC_FILTER_NEXT		Request Becky! to call this callback again so that another rules can be added.
        */
        return BKC_FILTER_DEFAULT;
    }

#ifdef __cplusplus
}
#endif
