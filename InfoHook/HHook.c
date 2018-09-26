#include "HHook.h"
#include "WinMsg.h"
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include <winbase.h>
#include <Tlhelp32.h>

#define DWRESERVED			 0
#define RETURNED_MESSAGE	_T("Received")
#define SENT_MESSAGE		_T("Sent    ")
#define POSTED_MESSAGE		_T("Posted  ")
#define DOT					_T(".")

// Shared memory area.
// This data will be shared among different processes
#pragma data_seg("CommMem")
	DWORD	g_pid				= 0;
	HHOOK	g_hGetTextHook		= NULL;
	HWND	g_hGetTextHwnd		= NULL;

	HHOOK	g_hGetMsgHook		= NULL;
	HWND	g_hGetMsgHwnd		= NULL;
	HWND	g_hDLLListBoxHwnd	= NULL;

	HHOOK	g_hShowMsgHook		= NULL;
	HHOOK	g_hCallWndHook		= NULL;
	HHOOK	g_hCallWndRetHook	= NULL;
	HHOOK	g_hwndTextViewHook	= NULL;

	HWND	g_hSpyMessagesOn	= NULL;
	HWND	g_hMessagesWnd		= NULL;
	HWND	g_hwndTextView		= NULL;
	HWND	g_outputtextView	= NULL;

	long	g_MessagesDepth	= 0;
	TCHAR	g_msgNestingStr[256];
	
	HWND	g_getMsgDLLWnd		= NULL;
	HWND	g_hTreeWnd			= NULL;
	HHOOK	g_hDLLGetMsgHook	= NULL;
	

	// Get the following values from registry
	int g_Button		= 0;
	int g_ListBox		= 0;
	int g_ScrollBar		= 0;
	int g_ComboBox		= 0;
	int g_Edit			= 0;
	int g_TreeView		= 0;
	int g_ListView		= 0;
	int g_DlgBox		= 0;
	int g_ToolTip		= 0;
	int g_Spin			= 0;
	int g_TabCtrl		= 0;
	int g_TrackBar		= 0;
	int g_StatusBar		= 0;
	int g_Progress		= 0;
	int g_Hotkey		= 0;
	int g_Header		= 0;
	int g_Animate		= 0;
	int g_DDE			= 0;
	int g_UnKnown		= 0;
	int g_User			= 0;
	int g_MouseMove		= 0;
	int	g_ToolBar		= 0;
	int	g_OnlyParent	= 0;
	int g_OnlyChild		= 0;
	int g_Keyboard		= 0;
	int	g_WholeSystem	= 0;
	int	g_OnlySameThread	= 0;
	int	g_OnlySameProcess	= 0;
	
#pragma data_seg()

// current Dll's Instance handle
HINSTANCE hDllInst = 0;

#define ARRAY_SIZE(Array)	\
	(sizeof(Array) / sizeof((Array)[0]))

#define STRUCT_SIZE(Struct)	\
	(sizeof(Struct) / sizeof((Struct)[0]))

/////////////////////////////////////////////////////////////////////////////
// DllMain : Main function for dll
BOOL WINAPI DllMain( HANDLE hModule, DWORD dwFunction, LPVOID lpNot )
{
	// store the current instance handle in a global variable
	hDllInst = hModule;
	switch(dwFunction)
	{
	case DLL_PROCESS_ATTACH:
		OutputDebugString("Attaching InfoHook.dll...");
		break;
	case DLL_PROCESS_DETACH:
		OutputDebugString("Detaching InfoHook.dll...");
		hDllInst = NULL;
		break;
	default:
		break;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// Utility Functions
// Reads int value from registry
BOOL getRegParam(int* nKeyValue,		//requested integer type data
				 const char* strKeyName	//name of the Key, whose value is
				)
{
	HKEY	hKey, hChildKey;
	DWORD	dwKeyType;
	int		nKeyValueBuff;
	long	lpResult = 0;
	DWORD	dwBuffSize = sizeof(*nKeyValue);
	BOOL bRetVal = TRUE;
	const char*	strKey = "Software\\Suresh's Infoviewer\\Infoview\\Settings";

	// If the key name is NULL return FALSE.
	if(strKeyName[0] == '\0')
	{
		return FALSE;
	}

	hKey = HKEY_CURRENT_USER;
	lpResult = RegOpenKeyEx( hKey,
						     strKey,
						     DWRESERVED,
        				     KEY_READ,
						     &hChildKey );

   	if (ERROR_SUCCESS == lpResult)
    {
		// Key is opened
		// Query the key to fetch data
		// The second parameter is NULL to Query the default value of key
		lpResult = RegQueryValueEx( hChildKey, 
								    strKeyName,
									NULL,
									&dwKeyType,
									(BYTE*)&nKeyValueBuff,
									&dwBuffSize );

		if ( ERROR_SUCCESS == lpResult) //if
		{
			if ( dwKeyType == REG_DWORD )
			{
				*nKeyValue = nKeyValueBuff;
				bRetVal = TRUE;
			}
			else
			{
				bRetVal = FALSE;
			}
		}
		else
		{
			// Key could not be Queried
			bRetVal = FALSE;
		}
    } 
    else 
	{ 
       	return FALSE;
    }
	
	RegCloseKey(hChildKey);
	return bRetVal;
}

/////////////////////////////////////////////////////////////////////////////
// GetText : Function to get the text
FUNCTION_DECL void WINAPI FetchText(HWND hwnd)
{
	// Get the selected window's handle
	g_hGetTextHwnd = hwnd;
	if ( g_hGetTextHook == NULL)
	{
		DWORD dwProcess;

		// install thread level keyboard hook
		g_hGetTextHook = (HHOOK)SetWindowsHookEx( WH_GETMESSAGE, GetTextWndProc, hDllInst,
			GetWindowThreadProcessId(hwnd, &dwProcess));

		if (!g_hGetTextHook)
		{
			ShowLastError("InfoView");
			g_hGetTextHook = NULL;
			return;
		}
		// Messages that are sent (as opposed to posted) do not go through the message queue
		// Post the message so that the GetTextWndProc receives it
		PostMessage(g_hGetTextHwnd, WM_NULL, 0, 0);
	}
}

FUNCTION_DECL void WINAPI InstallKeybHook(HWND hwnd, HWND outputWnd)
{
	g_hwndTextView = hwnd;
	g_outputtextView = outputWnd;
	if ( g_hwndTextViewHook == NULL )
	{
		DWORD dwProcess;

		// install thread level keyboard hook
		g_hwndTextViewHook = (HHOOK)SetWindowsHookEx( WH_KEYBOARD, KeybProc, hDllInst,
			GetWindowThreadProcessId(hwnd, &dwProcess));
		if (!g_hwndTextViewHook)
		{
			ShowLastError("InfoView");
			g_hwndTextViewHook = NULL;
			return;
		}
		// PostMessage(g_hwndTextView, WM_NULL, 0, 0);
	}
}

/////////////////////////////////////////////////////////////////////////////
// InstallGetMsgHook : Function to install system level GetMsgHook
FUNCTION_DECL void WINAPI InstallGetMsgHook( HWND hwnd , HWND hwndOutput)
{
	// get the selected window's handle
	g_hGetMsgHwnd	= hwnd;
	g_hDLLListBoxHwnd = hwndOutput;
	if ( g_hGetMsgHook == NULL)
	{
		DWORD dwProcess;

		// install thread level keyboard hook
		g_hGetMsgHook = (HHOOK)SetWindowsHookEx( WH_GETMESSAGE, GetMsgProc, hDllInst, 
			GetWindowThreadProcessId(hwnd, &dwProcess));
		if (!g_hGetMsgHook)
		{
			ShowLastError("InfoView");
			g_hGetMsgHook = NULL;
			return;
		}
		PostMessage(g_hGetMsgHwnd, WM_NULL, 0, 0);
	}
}

/////////////////////////////////////////////////////////////////////////////
// GetThreadsList : 
FUNCTION_DECL void WINAPI GetThreadsList( DWORD dwPid, HWND hwnd, HWND hwndOutput )
{
	DWORD tid = 0;
	ReleaseDLLHook();
	g_hTreeWnd = hwndOutput;
	g_getMsgDLLWnd	= hwnd;

	if ( g_hDLLGetMsgHook == NULL)
	{
		g_pid = dwPid;
		tid = GetWindowThreadProcessId(hwnd, &dwPid);
		g_hDLLGetMsgHook = (HHOOK)SetWindowsHookEx(WH_GETMESSAGE, 
									GetThreadsListProc, hDllInst, tid);
		if (!g_hDLLGetMsgHook)
		{
			ShowLastError("InfoView");
			ReleaseDLLHook();
			return;
		}
		// Explicitely post a null message to the window, so that the "proc"
		// handles the message by getting into the process
		PostMessage(g_getMsgDLLWnd, WM_NULL, 0, 0);
	}
}

LRESULT	CALLBACK GetThreadsListProc( int nCode, WPARAM wParam, LPARAM lParam )
{
	if ((nCode == HC_ACTION) && (wParam == PM_REMOVE)
							 && (((MSG *)lParam)->hwnd == g_getMsgDLLWnd)
							 && (((MSG *)lParam)->message == WM_NULL))
	{
		HANDLE hModuleSnap = NULL; 
		THREADENTRY32 te;
		COPYDATASTRUCT cdstruct;

		hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, g_pid);
		if (hModuleSnap == INVALID_HANDLE_VALUE)
			return ( int )CallNextHookEx( g_hGetMsgHook, nCode, wParam, lParam );
		
		Thread32First(hModuleSnap, &te);
		te.dwSize = sizeof(THREADENTRY32);

		cdstruct.dwData = DWRESERVED;
		cdstruct.cbData = sizeof (THREADENTRY32);
		cdstruct.lpData = (void *)&te;
		if (IsWindow(g_hTreeWnd))
		{
			SendMessage(g_hTreeWnd, WM_COPYDATA, (WPARAM)(HWND)g_getMsgDLLWnd, (LPARAM)&cdstruct);
		}
		while (TRUE == Thread32Next(hModuleSnap, &te))
		{
			cdstruct.dwData = DWRESERVED;
			cdstruct.cbData = sizeof (THREADENTRY32);
			cdstruct.lpData = (void *)&te;
			if (IsWindow(g_hTreeWnd))
			{
				SendMessage(g_hTreeWnd, WM_COPYDATA, (WPARAM)(HWND)g_getMsgDLLWnd, (LPARAM)&cdstruct);
			}
		}

		CloseHandle(hModuleSnap);
		ReleaseDLLHook();
	}
	return ( int )CallNextHookEx( g_hGetMsgHook, nCode, wParam, lParam );
}

/////////////////////////////////////////////////////////////////////////////
// ShowDllList : 
FUNCTION_DECL BOOL WINAPI ShowDllList( HWND hwnd , HWND hwndOutput)
{
	ReleaseDLLHook();

	// get the selected window's handle
	g_getMsgDLLWnd	= hwnd;
	g_hTreeWnd = hwndOutput;
	if ( g_hDLLGetMsgHook == NULL)
	{
		DWORD dwProcess;

		g_hDLLGetMsgHook = (HHOOK)SetWindowsHookEx( WH_GETMESSAGE, ShowDllListProc, hDllInst,
									GetWindowThreadProcessId(g_getMsgDLLWnd, &dwProcess));
		if (!g_hDLLGetMsgHook)
		{
			ShowLastError("InfoView");
			ReleaseDLLHook();
			return FALSE;
		}
		// Explicitely post a null message to the window, so that the "proc" handles the 
		// message by getting into the process
		PostMessage(g_getMsgDLLWnd, WM_NULL, 0, 0);
	}
	return TRUE;
}

LRESULT	CALLBACK ShowDllListProc( int nCode, WPARAM wParam, LPARAM lParam )
{
	if ((nCode == HC_ACTION) && (wParam == PM_REMOVE)
							 && (((MSG *)lParam)->hwnd == g_getMsgDLLWnd)
							 && (((MSG *)lParam)->message == WM_NULL))
	{
		char szBuf[ MAX_PATH * 100 ], szModName[MAX_PATH];
		LPBYTE lp = NULL;
		MEMORY_BASIC_INFORMATION	mbi;
		int nLen;
		szBuf[0] = 0;
		szBuf[1] = 0;

		while( VirtualQuery(lp, &mbi, sizeof(mbi)) == sizeof(mbi) )
		{
			if (mbi.State == MEM_FREE)
				mbi.AllocationBase = mbi.BaseAddress;

			if ( (mbi.AllocationBase != mbi.BaseAddress) ||
				 (mbi.AllocationBase == NULL))
			{
				nLen = 0;
			}
			else
			{
				nLen = GetModuleFileName( (HINSTANCE) mbi.AllocationBase,
					szModName, ARRAY_SIZE(szModName) );
			}

			if (nLen > 0)
			{
				showDLLList(szModName);
			}
			lp += mbi.RegionSize;
		}
		ReleaseDLLHook();
	}
	return ( int )CallNextHookEx( g_hGetMsgHook, nCode, wParam, lParam );
}

/////////////////////////////////////////////////////////////////////////////
// GetTextWndProc :  This callback function is used to get the text
// from any control
LRESULT	CALLBACK GetTextWndProc( int nCode, WPARAM wParam, LPARAM lParam )
{
	if ( NULL == g_hGetTextHwnd )
	{
		EndFetchTextHook();
		return ( int )CallNextHookEx( g_hGetTextHook, nCode, wParam, lParam );
	}

	if ((nCode == HC_ACTION) && (wParam == PM_REMOVE)
							 && (((MSG *)lParam)->hwnd == g_hGetTextHwnd)
							 && (((MSG *)lParam)->message == WM_NULL))
	{
		int nTextLen = 0;
		TCHAR strBuffer[4096];

		ZeroMemory(strBuffer, sizeof(strBuffer)/sizeof(TCHAR));
		nTextLen = GetWindowTextLength ( g_hGetTextHwnd );

		GetWindowText( g_hGetTextHwnd, strBuffer, 4096);

		EndFetchTextHook();
		MessageBox( NULL, strBuffer, "InfoView", MB_OK | MB_ICONINFORMATION | MB_TOPMOST );
	}

	// release hook after getting the text
	EndFetchTextHook();
	return ( int )CallNextHookEx( g_hGetTextHook, nCode, wParam, lParam );
}

LRESULT	CALLBACK KeybProc( int nCode, WPARAM wParam, LPARAM lParam )
{
	// Skip if the key was released or if it's a repeat
	if (lParam & 0x80000000 || lParam & 0x40000000)
		return CallNextHookEx(g_hwndTextViewHook, nCode, wParam, lParam);

	SendMessage(g_outputtextView, ((MSG *)lParam)->message, wParam, lParam );
	return ( int )CallNextHookEx( g_hwndTextViewHook, nCode, wParam, lParam );
}

LRESULT	CALLBACK GetMsgProc( int nCode, WPARAM wParam, LPARAM lParam )
{
	if ((nCode == HC_ACTION) && (wParam == PM_REMOVE)
							 && (((MSG *)lParam)->hwnd == g_hGetMsgHwnd)
							 && (((MSG *)lParam)->message == WM_NULL))
	{
		char szBuf[ MAX_PATH * 100 ], szModName[MAX_PATH];
		LPBYTE lp = NULL;
		MEMORY_BASIC_INFORMATION	mbi;
		int nLen;
		szBuf[0] = 0;
		szBuf[1] = 0;

		while( VirtualQuery(lp, &mbi, sizeof(mbi)) == sizeof(mbi) )
		{
			if (mbi.State == MEM_FREE)
				mbi.AllocationBase = mbi.BaseAddress;

			if ( (mbi.AllocationBase == hDllInst)		 ||
				 (mbi.AllocationBase != mbi.BaseAddress) ||
				 (mbi.AllocationBase == NULL) )
			{
				nLen = 0;
			}
			else
			{
				nLen = GetModuleFileName( (HINSTANCE) mbi.AllocationBase,
					szModName, ARRAY_SIZE(szModName) );
			}

			if (nLen > 0)
			{
				SendMessage(g_hDLLListBoxHwnd, LB_ADDSTRING, 0, (LPARAM)szModName);
				// _stprintf( _tcschr( szBuf, 0), "%08x - %s\n", mbi.AllocationBase, szModName );
			}
			lp += mbi.RegionSize;
		}
	}
	return ( int )CallNextHookEx( g_hGetMsgHook, nCode, wParam, lParam );
}

/////////////////////////////////////////////////////////////////////////////
// EndFetchTextHook : Function to release hook handle
FUNCTION_DECL void WINAPI EndFetchTextHook()
{
	if (NULL != g_hGetTextHook)
	{
		UnhookWindowsHookEx(g_hGetTextHook);
		g_hGetTextHook  = NULL;
	}
	if ( NULL != g_hGetTextHwnd )
	{
		g_hGetTextHwnd = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// RemoveGetMsgHook : Function to remove system level GetMsg Hook
FUNCTION_DECL void WINAPI RemoveGetMsgHook()
{
	if (NULL != g_hGetMsgHook)
	{
		UnhookWindowsHookEx(g_hGetMsgHook);
		g_hGetMsgHook = NULL;
		g_hDLLListBoxHwnd = NULL;
	}
}

/*************************************************************************/
FUNCTION_DECL void WINAPI ShowMessages( HWND hwnd , HWND hwndOutput)
{
	DWORD dwThreadID = 0;
	DWORD dwProcess;

	// get the selected window's handle
	g_hSpyMessagesOn	= hwnd;
	g_hMessagesWnd = hwndOutput;
	if ( g_hGetMsgHook == NULL)
	{
		getRegParam(&g_Button,		"BtnMsg"		);
		getRegParam(&g_DlgBox,		"DlgBoxMsg"		);
		getRegParam(&g_ScrollBar,	"ScrollBarMsg"	);
		getRegParam(&g_TreeView,	"TreeViewMsg"	);
		getRegParam(&g_ComboBox,	"ComboBoxMsg"	);
		getRegParam(&g_Edit,		"EditMsg"		);
		getRegParam(&g_ListView,	"ListViewMsg"	);
		getRegParam(&g_ToolTip,		"ToolTipMsg"	);
		getRegParam(&g_Spin,		"SpinMsg"		);
		getRegParam(&g_TabCtrl,		"TabCtrlMsg"	);
		getRegParam(&g_TrackBar,	"TrackBarMsg"	);
		getRegParam(&g_StatusBar,	"StatusBarMsg"	);
		getRegParam(&g_Progress,	"ProgressMsg"	);
		getRegParam(&g_Hotkey,		"HotkeyMsg"		);
		getRegParam(&g_Header,		"HeaderMsg"		);
		getRegParam(&g_Animate,		"AnimateMsg"	);
		getRegParam(&g_DDE,			"DDEMsg"		);
		getRegParam(&g_UnKnown,		"UnKnownMsg"	);
		getRegParam(&g_User,		"UserMsg"		);
		getRegParam(&g_MouseMove,	"MouseMoveMsg"	);
		getRegParam(&g_ToolBar,		"ToolBarMsg"	);
		getRegParam(&g_OnlyChild,	"OnlyChildMsg"	);
		getRegParam(&g_OnlyParent,	"OnlyParentMsg"	);
		getRegParam(&g_WholeSystem,	"WholeSystemMsg");
		getRegParam(&g_OnlySameThread,	"SameThreadMsg"	);
		getRegParam(&g_OnlySameProcess,	"SameProcessMsg");
		getRegParam(&g_Keyboard,	"KeyBoard");

		if (1 == g_WholeSystem)
		{
			// For system level Hook, ThreadId should be ZERO.
			dwThreadID = 0;
		}
		else
		{
			dwThreadID = GetWindowThreadProcessId(hwnd, &dwProcess);
		}

		// install thread level keyboard hook
		g_hShowMsgHook = (HHOOK)SetWindowsHookEx( WH_GETMESSAGE, ShowMessageProc, hDllInst, dwThreadID);
		g_hCallWndHook = (HHOOK)SetWindowsHookEx( WH_CALLWNDPROC, CallWndProc, hDllInst, dwThreadID);
		g_hCallWndRetHook = (HHOOK)SetWindowsHookEx( WH_CALLWNDPROCRET, CallWndProcRet, hDllInst, dwThreadID);
		if ((!g_hShowMsgHook) || (!g_hCallWndHook) || (!g_hCallWndRetHook))
		{
			ShowLastError("InfoView");
			StopShowingMessages();
			return;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// StopShowingMessages : Function to remove system level ShowMsg Hook
FUNCTION_DECL void WINAPI StopShowingMessages()
{
	if (NULL != g_hShowMsgHook)
	{
		UnhookWindowsHookEx(g_hShowMsgHook);
		g_hShowMsgHook = NULL;
	}
	if (NULL != g_hCallWndHook)
	{
		UnhookWindowsHookEx(g_hCallWndHook);
		g_hCallWndHook = NULL;
	}
	if (NULL != g_hCallWndRetHook)
	{
		UnhookWindowsHookEx(g_hCallWndRetHook);
		g_hCallWndRetHook = NULL;
	}
	g_hSpyMessagesOn	= NULL;
	g_hMessagesWnd	= NULL;
	g_MessagesDepth		= 0;
}

LRESULT	CALLBACK ShowMessageProc( int nCode, WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = CallNextHookEx(g_hShowMsgHook, nCode, wParam, lParam);
	PMSG pmsg = (PMSG)lParam;
	DWORD dwCurTid = 0, dwCurPid = 0;
	DWORD dwWndTid = 0, dwWndPid = 0;

	if(nCode == HC_ACTION)
	{
		// get the thread ID  and process Id of the current message
		dwCurTid = GetWindowThreadProcessId(pmsg->hwnd, &dwCurPid);

		// get the thread ID  and process Id of the selected window
		dwWndTid = GetWindowThreadProcessId(g_hSpyMessagesOn, &dwWndPid);

		if (1 == g_WholeSystem)
		{
			// show only selected thread's messages
			ShowExtraInfo(pmsg->message, POSTED_MESSAGE, pmsg->wParam, pmsg->lParam);
		}
		else if (1 == g_OnlySameProcess)
		{
			// Show the message "If the current message's processId and selected
			// window's processID are same"
			if (dwCurPid == dwWndPid)
			{
				ShowExtraInfo(pmsg->message, POSTED_MESSAGE, pmsg->wParam, pmsg->lParam);
			}
		}
		else if (1 == g_OnlySameThread)
		{
			// Show the message "If the current message's threadId and selected
			// window's thread Id are same"
			if (dwCurTid == dwWndTid)
			{
				ShowExtraInfo(pmsg->message, POSTED_MESSAGE, pmsg->wParam, pmsg->lParam);
			}
		}
		else if (1 == g_OnlyParent)
		{
			// show only parent window messages
			if (((MSG *)lParam)->hwnd == GetParent(g_hSpyMessagesOn))
			{
				ShowExtraInfo(pmsg->message, POSTED_MESSAGE, pmsg->wParam, pmsg->lParam);
			}
		}
		else if (1 == g_OnlyChild)
		{
			// show only selected window's messages
			if (((MSG *)lParam)->hwnd == g_hSpyMessagesOn)
			{
				ShowExtraInfo(pmsg->message, POSTED_MESSAGE, pmsg->wParam, pmsg->lParam);
			}
		}
	}
	return(lResult);
}

LRESULT	CALLBACK CallWndProc( int nCode, WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult;
	DWORD dwCurTid = 0, dwCurPid = 0;
	DWORD dwWndTid = 0, dwWndPid = 0;

	CWPSTRUCT *CwpS = (CWPSTRUCT *)lParam;
	lResult = CallNextHookEx(g_hCallWndHook, nCode, wParam, (LPARAM)CwpS);
	if (nCode == HC_ACTION)
	{
		// get the thread ID  and process Id of the current message
		dwCurTid = GetWindowThreadProcessId(CwpS->hwnd, &dwCurPid);

		// get the thread ID  and process Id of the selected window
		dwWndTid = GetWindowThreadProcessId(g_hSpyMessagesOn, &dwWndPid);

		if (1 == g_WholeSystem)
		{
			// show only selected thread's messages
			ShowExtraInfo(CwpS->message, SENT_MESSAGE, CwpS->wParam, CwpS->lParam);
		}
		else if (1 == g_OnlySameProcess)
		{
			// Show the message "If the current message's processId and selected
			// window's processID are same"
			if (dwCurPid == dwWndPid)
			{
				// show only selected thread's messages
				ShowExtraInfo(CwpS->message, SENT_MESSAGE, CwpS->wParam, CwpS->lParam);
			}
		}
		else if (1 == g_OnlySameThread)
		{
			// Show the message "If the current message's threadId and selected
			// window's thread Id are same"
			if (dwCurTid == dwWndTid)
			{
				// show only selected thread's messages
				ShowExtraInfo(CwpS->message, SENT_MESSAGE, CwpS->wParam, CwpS->lParam);
			}
		}
		else if (1 == g_OnlyParent)
		{
			// show only parent window messages
			if (CwpS->hwnd == GetParent(g_hSpyMessagesOn))
			{
				ShowExtraInfo(CwpS->message, SENT_MESSAGE, CwpS->wParam, CwpS->lParam);
			}
		}
		else if (1 == g_OnlyChild)
		{
			// show only selected window's messages
			if (CwpS->hwnd == g_hSpyMessagesOn)
			{
				ShowExtraInfo(CwpS->message, SENT_MESSAGE, CwpS->wParam, CwpS->lParam);
			}
		}
	}

	g_MessagesDepth = InterlockedIncrement(&g_MessagesDepth);
	return(lResult);
}

LRESULT	CALLBACK CallWndProcRet( int nCode, WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult;
	DWORD dwCurTid = 0, dwCurPid = 0;
	DWORD dwWndTid = 0, dwWndPid = 0;

	CWPRETSTRUCT *CwpRetS = (CWPRETSTRUCT *)lParam;
	lResult = CallNextHookEx(g_hCallWndRetHook, nCode, wParam, (LPARAM)CwpRetS);
	if (nCode == HC_ACTION)
	{
		g_MessagesDepth = InterlockedDecrement(&g_MessagesDepth);

		// get the thread ID  and process Id of the current message
		dwCurTid = GetWindowThreadProcessId(CwpRetS->hwnd, &dwCurPid);

		// get the thread ID  and process Id of the selected window
		dwWndTid = GetWindowThreadProcessId(g_hSpyMessagesOn, &dwWndPid);
		if (1 == g_WholeSystem)
		{
			// show only selected thread's messages
			ShowExtraInfo(CwpRetS->message, RETURNED_MESSAGE, wParam, lParam);
		}
		else if (1 == g_OnlySameProcess)
		{
			// Show the message "If the current message's processId and selected
			// window's processID are same"
			if (dwCurPid == dwWndPid)
			{
				// show only selected thread's messages
				ShowExtraInfo(CwpRetS->message, RETURNED_MESSAGE, wParam, lParam);
			}
		}
		else if (1 == g_OnlySameThread)
		{
			// Show the message "If the current message's threadId and selected
			// window's thread Id are same"
			if (dwCurTid == dwWndTid)
			{
				// show only selected thread's messages
				ShowExtraInfo(CwpRetS->message, RETURNED_MESSAGE, wParam, lParam);
			}
		}
		else if (1 == g_OnlyParent)
		{
			// show only parent window messages
			if (CwpRetS->hwnd == GetParent(g_hSpyMessagesOn))
			{
				ShowExtraInfo(CwpRetS->message, RETURNED_MESSAGE, wParam, lParam);
			}
		}
		else if (1 == g_OnlyChild)
		{
			// show only selected window's messages
			if (CwpRetS->hwnd == g_hSpyMessagesOn)
			{
				ShowExtraInfo(CwpRetS->message, RETURNED_MESSAGE, wParam, lParam);
			}
		}
	}
	return(lResult);
}

/****************************************************************************/
FUNCTION_DECL void WINAPI ShowExtraInfo(UINT message, const char* pMsgType, WPARAM wParam, LPARAM lParam)
{
	int i = 0;
	if (g_MouseMove == 1)
	{
		// Window Messages
		for ( i = 0; i < STRUCT_SIZE(WindowsMessagesS); i++ )
		{
			if (message == WindowsMessagesS[i].MsgID)
			{
				ShowOnListView(WindowsMessagesS[i].MsgName, pMsgType, wParam, lParam);
			}
		}
	}

	if (g_Keyboard == 1)
	{
		// Window Messages
		for ( i = 0; i < STRUCT_SIZE(KeyBoardMessagesS); i++ )
		{
			if (message == KeyBoardMessagesS[i].MsgID)
			{
				ShowOnListView(KeyBoardMessagesS[i].MsgName, pMsgType, wParam, lParam);
			}
		}
	}

	if (1 == g_Button)
	{
		//********   Button Messages   *******	
		for ( i = 0; i < STRUCT_SIZE(ButtonMessagesS); i++ )
		{
			if (message == ButtonMessagesS[i].MsgID)
			{
				ShowOnListView(ButtonMessagesS[i].MsgName, pMsgType, wParam, lParam);
			}
		}
		//********   Button Notify Messages   *******	
		for ( i = 0; i < STRUCT_SIZE(ButtonNotifyMessagesS); i++ )
		{
			if (message == ButtonNotifyMessagesS[i].MsgID)
			{
				ShowOnListView(ButtonNotifyMessagesS[i].MsgName, pMsgType, wParam, lParam);
			}
		}
	}

	if (1 == g_ListBox)
	{
		//********   ListBox Messages   *******	
		for ( i = 0; i < STRUCT_SIZE(ListBoxMessagesS); i++ )
		{
			if (message == ListBoxMessagesS[i].MsgID)
			{
				ShowOnListView(ListBoxMessagesS[i].MsgName, pMsgType, wParam, lParam);
			}
		}
	}

	if (1 == g_DlgBox)
	{
		//********   DialogBox Messages   *******	
		for ( i = 0; i < STRUCT_SIZE(DialogBoxMessagesS); i++ )
		{
			if (message == DialogBoxMessagesS[i].MsgID)
			{
				ShowOnListView(DialogBoxMessagesS[i].MsgName, pMsgType, wParam, lParam);
			}
		}
	}

	if (1 == g_ComboBox)
	{
		//********   ComboBox Messages   *******	
		for ( i = 0; i < STRUCT_SIZE(ComboBoxMessagesS); i++ )
		{
			if (message == ComboBoxMessagesS[i].MsgID)
			{
				ShowOnListView(ComboBoxMessagesS[i].MsgName, pMsgType, wParam, lParam);
			}
		}
	}

	if (1 == g_Edit)
	{
		//********   EditBox Messages   *******	
		for ( i = 0; i < STRUCT_SIZE(EditBoxMessagesS); i++ )
		{
			if (message == EditBoxMessagesS[i].MsgID)
			{
				ShowOnListView(EditBoxMessagesS[i].MsgName, pMsgType, wParam, lParam);
			}
		}
	}

	if (1 == g_DDE)
	{
		//********   DDE Messages   *******	
		for ( i = 0; i < STRUCT_SIZE(DDEMessagesS); i++ )
		{
			if (message == DDEMessagesS[i].MsgID)
			{
				ShowOnListView(DDEMessagesS[i].MsgName, pMsgType, wParam, lParam);
			}
		}
	}

	if (1 == g_TreeView)
	{
		//********   TreeView Messages   *******	
		for ( i = 0; i < STRUCT_SIZE(TreeViewMessagesS); i++ )
		{
			if (message == TreeViewMessagesS[i].MsgID)
			{
				ShowOnListView(TreeViewMessagesS[i].MsgName, pMsgType, wParam, lParam);
			}
		}
	}

	if (1 == g_ToolTip)
	{
		//********   ToolTip Messages   *******	
		for ( i = 0; i < STRUCT_SIZE(ToolTipMessagesS); i++ )
		{
			if (message == ToolTipMessagesS[i].MsgID)
			{
				ShowOnListView(ToolTipMessagesS[i].MsgName, pMsgType, wParam, lParam);
			}
		}
	}

	if (1 == g_Spin)
	{
		//********   UpDown (Spin) Messages   *******	
		for ( i = 0; i < STRUCT_SIZE(UpDownCtrlMessagesS); i++ )
		{
			if (message == UpDownCtrlMessagesS[i].MsgID)
			{
				ShowOnListView(UpDownCtrlMessagesS[i].MsgName, pMsgType, wParam, lParam);
			}
		}
	}

	if (1 == g_TabCtrl)
	{
		//********   Tab Control Messages   *******	
		for ( i = 0; i < STRUCT_SIZE(TabCtrlMessagesS); i++ )
		{
			if (message == TabCtrlMessagesS[i].MsgID)
			{
				ShowOnListView(TabCtrlMessagesS[i].MsgName, pMsgType, wParam, lParam);
			}
		}
	}

	if (1 == g_TrackBar)
	{
		//********   Trackbar Control Messages   *******	
		for ( i = 0; i < STRUCT_SIZE(TrackBarMessagesS); i++ )
		{
			if (message == TrackBarMessagesS[i].MsgID)
			{
				ShowOnListView(TrackBarMessagesS[i].MsgName, pMsgType, wParam, lParam);
			}
		}
	}

	if (1 == g_ToolBar)
	{
		//********   Toolbar Messages   *******	
		for ( i = 0; i < STRUCT_SIZE(ToolBarMessagesS); i++ )
		{
			if (message == ToolBarMessagesS[i].MsgID)
			{
				ShowOnListView(ToolBarMessagesS[i].MsgName, pMsgType, wParam, lParam);
			}
		}
	}

	if (1 == g_ScrollBar)
	{
		//********   Scrollbar Messages   *******	
		for ( i = 0; i < STRUCT_SIZE(ScrollBarMessages); i++ )
		{
			if (message == ScrollBarMessages[i].MsgID)
			{
				ShowOnListView(ScrollBarMessages[i].MsgName, pMsgType, wParam, lParam);
			}
		}
	}

	if (1 == g_StatusBar)
	{
		//********   Statusbar Messages   *******	
		for ( i = 0; i < STRUCT_SIZE(StatusBarMessages); i++ )
		{
			if (message == StatusBarMessages[i].MsgID)
			{
				ShowOnListView(StatusBarMessages[i].MsgName, pMsgType, wParam, lParam);
			}
		}
	}

	if (1 == g_Progress)
	{
		//********   Progressbar Messages   *******	
		for ( i = 0; i < STRUCT_SIZE(ProgressBarMessages); i++ )
		{
			if (message == ProgressBarMessages[i].MsgID)
			{
				ShowOnListView(ProgressBarMessages[i].MsgName, pMsgType, wParam, lParam);
			}
		}
	}

	if (1 == g_ListView)
	{
		//********   ListView Messages   *******	
		for ( i = 0; i < STRUCT_SIZE(ListViewMessages); i++ )
		{
			if (message == ListViewMessages[i].MsgID)
			{
				ShowOnListView(ListViewMessages[i].MsgName, pMsgType, wParam, lParam);
			}
		}
	}

	if (1 == g_Hotkey)
	{
		//********   Hotkey Messages   *******	
		for ( i = 0; i < STRUCT_SIZE(HotkeyMessages); i++ )
		{
			if (message == HotkeyMessages[i].MsgID)
			{
				ShowOnListView(HotkeyMessages[i].MsgName, pMsgType, wParam, lParam);
			}
		}

	}
	
	if (1 == g_Header)
	{
		//********   Header Control Messages   *******	
		for ( i = 0; i < STRUCT_SIZE(HeaderCtrlMessages); i++ )
		{
			if (message == HeaderCtrlMessages[i].MsgID)
			{
				ShowOnListView(HeaderCtrlMessages[i].MsgName, pMsgType, wParam, lParam);
			}
		}
	}

	if (1 == g_Animate)
	{
		//********   Animate control Messages   *******	
		for ( i = 0; i < STRUCT_SIZE(AnimateCtrlMessages); i++ )
		{
			if (message == AnimateCtrlMessages[i].MsgID)
			{
				ShowOnListView(AnimateCtrlMessages[i].MsgName, pMsgType, wParam, lParam);
			}
		}
	}
	if (1 == g_User)
	{
		// user defined messages ranges from 0x400 to 0x7FFF
		if ((message >= WM_USER) && (message <= 0x7FFF))
		{
			TCHAR str[255];
			sprintf(str, "User-defined (0x%08x), WM_USER + %d", message, message - WM_USER);
			ShowOnListView(str, pMsgType, wParam, lParam);
		}

		// "Uniquely Registered Window Messages" ranges from 0xC000 to 0xFFFF
		if ((message >= 0xC000) && (message <= 0xFFFF))
		{
			TCHAR str[255];
			sprintf(str, "Register-window (0x%08x), WM_USER + %d", message, message - WM_USER);
			ShowOnListView(str, pMsgType, wParam, lParam);
		}
		// "Future use messages !!!" ranges from 0x8000 to 0xBFFF
		if ((message >= 0x8000) && (message <= 0xBFFF))
		{
			TCHAR str[255];
			sprintf(str, "Future-use (0x%08x)", message);
			ShowOnListView(str, pMsgType, wParam, lParam);
		}
	}
}

void ShowOnListView(const char* msgName, const char* pMsgType, WPARAM wParam, LPARAM lParam)
{
	INFO_COPYDATA info_DataS;
	COPYDATASTRUCT cds;

	// Fillup INFO_COPYDATA structure and pass it to INFOVIEW.EXE
	_tcscpy(info_DataS.strMsgName, msgName);
	_tcscpy(info_DataS.strMsgType, pMsgType);
	info_DataS.hwndFrom = g_hSpyMessagesOn;
	info_DataS.wParam = wParam;
	info_DataS.lParam = lParam;
	info_DataS.nMsgDepth = g_MessagesDepth;

	cds.dwData = DWRESERVED;
	cds.cbData = sizeof (INFO_COPYDATA);
	cds.lpData = (void *)&info_DataS;
	SendMessage(g_hMessagesWnd, WM_COPYDATA, (WPARAM)(HWND)g_hSpyMessagesOn, (LPARAM)&cds);
}

void showDLLList(const char *pStr)
{
	DLL_LIST dllDataS;
	COPYDATASTRUCT cdstruct;
	_tcscpy(dllDataS.strDllPath, pStr);
	dllDataS.hwndFrom = g_getMsgDLLWnd;

	cdstruct.dwData = DWRESERVED;
	cdstruct.cbData = sizeof (DLL_LIST);
	cdstruct.lpData = (void *)&dllDataS;
	if (IsWindow(g_hTreeWnd))
	{
		SendMessage(g_hTreeWnd, WM_COPYDATA, (WPARAM)(HWND)g_getMsgDLLWnd, (LPARAM)&cdstruct);
	}
}

FUNCTION_DECL void WINAPI ReleaseDLLHook(void)
{
	UnhookWindowsHookEx(g_hDLLGetMsgHook);
	g_hDLLGetMsgHook = NULL;
	g_getMsgDLLWnd = NULL;
	g_hTreeWnd = NULL;
}

FUNCTION_DECL void WINAPI ReleaseKeybHook(void)
{
	UnhookWindowsHookEx(g_hwndTextViewHook);
	g_hwndTextViewHook = NULL;
	g_hwndTextView = NULL;
}

void ShowLastError(const char* pErrorTitle)
{
	char *ptr = NULL;
	FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM,
				0,
				GetLastError(),
				0,
				(char *)&ptr,
				1024,
				NULL);
	MessageBox(GetActiveWindow(), ptr, pErrorTitle, MB_OK | MB_ICONERROR);
	LocalFree(ptr);
}

/*
Q) When I select the Applications tab in the Windows NT® Task Manager, 
in the listview control, the Task Manager shows which applications are
running and whether they are responding. What is the definition of 
"Not Responding," and how does the Task Manager determine whether 
an application is responding?

DWORD dwResult;
 BOOL fResponding = SendMessageTimeout(hwndInQuestion,
      WM_NULL, 0, 0, SMTO_ABORTIFHUNG, 5000, &dwResult);
 // fResponding is TRUE if the thread is responding and
 // FALSE if not.
*/