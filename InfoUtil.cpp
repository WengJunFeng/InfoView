#include "stdafx.h"
#include "InfoUtil.h"

#define RTN_OK 0
#define RTN_USAGE 1
#define RTN_ERROR 13


CString GetResourceString(UINT uid)
{
	CString resStr;
	resStr.LoadString(uid);
	return resStr;
}

////////////////////////////////////////////////////////////////
// HideAllControls() : Function to Hide all the child controls.
////////////////////////////////////////////////////////////////
void HideAllControls(CWnd *pParent)
{
	ASSERT(NULL != pParent);
	CWnd *pChildWnd = pParent->GetWindow(GW_CHILD);
	while(pChildWnd)
	{
		int nID = pChildWnd->GetDlgCtrlID();
		pChildWnd->ShowWindow(SW_HIDE);
		pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
	}
}

void UnHideAllControls(CWnd *pParent)
{
	ASSERT(NULL != pParent);
	CWnd *pChildWnd = pParent->GetWindow(GW_CHILD);
	while(pChildWnd)
	{
		pChildWnd->ShowWindow(SW_SHOW);
		pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
	}
}

BOOL IsWindows95()
{
	OSVERSIONINFO info;
	info.dwOSVersionInfoSize = sizeof(info);
	::GetVersionEx(&info);
    if (info.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
	{
		return TRUE;
	}
	return FALSE;
}

BOOL IsWindowsNT()
{
	OSVERSIONINFO info;
	info.dwOSVersionInfoSize = sizeof(info);
	::GetVersionEx(&info);
    if (info.dwPlatformId == VER_PLATFORM_WIN32_NT )
	{
		return TRUE;
	}
	return FALSE;
}

void ShowLastError(const char* pErrorTitle)
{
	char *ptr = NULL;
	::FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM,
				0,
				GetLastError(),
				0,
				(char *)&ptr,
				1024,
				NULL);
	::MessageBox(GetActiveWindow(), ptr, pErrorTitle, MB_OK | MB_ICONERROR);
	LocalFree(ptr);
}

void ShowError(const char* pErrorString, const char* pTitle)
{
	if (pErrorString == NULL || pTitle == NULL)
	{
		return;
	}
	::MessageBox(GetActiveWindow(), pErrorString, pTitle, MB_OK | MB_ICONERROR);
}

void Copy2Clipboard(CString& str)
{
	HANDLE		m_hMem = NULL;
	LPSTR		m_lpStr = NULL;
	CString		strData = "";

	strData += "Infoview for Microsoft Windows family: Copyright (c) reserved 1998, Mahasri Kalavala\r\n";
	strData += "===================================================================================\r\n";
	strData += str;
	strData += "\r\n";
	strData += "===================================================================================\r\n";

	// Allocate memory for clipboard
	m_hMem = GlobalAlloc( GHND, strData.GetLength() + 1 );
	m_lpStr = (LPSTR) GlobalLock(m_hMem);
	lstrcpy(m_lpStr, strData);

	GlobalUnlock(m_hMem);

	// Open clipboard
	OpenClipboard (NULL);

	// clear the contents first
	EmptyClipboard();

	// set the new data
	HANDLE handle = SetClipboardData (CF_TEXT, m_hMem);

	// close clipboard
	CloseClipboard();

	// update the status to the user
	if ( handle == NULL )
	{
		ShowLastError("InfoView");
	}
	else
	{
		MessageBox(NULL, "Contents are copied to clipboard successfully.", "InfoView", MB_ICONINFORMATION | MB_OK );
	}
}

HANDLE GetProcessHandle(DWORD pid)
{
    HANDLE hProcess;
    HANDLE hToken;
    int dwRetVal=RTN_OK; // assume success from main()

    if(!OpenProcessToken(
            GetCurrentProcess(),
            TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
            &hToken
            )) return NULL;

    // enable SeDebugPrivilege
    if(!SetPrivilege(hToken, SE_DEBUG_NAME, TRUE))
    {
        ShowLastError("SetPrivilege");

        // close token handle
        CloseHandle(hToken);

        // indicate failure
        return NULL;
    }

   // open the process
    if((hProcess = OpenProcess(
            PROCESS_ALL_ACCESS,
            FALSE,
            pid
            )) == NULL)
    {
		ShowLastError("OpenProcess");
        return NULL;
    }

	/*
    // disable SeDebugPrivilege
    SetPrivilege(hToken, SE_DEBUG_NAME, FALSE);

    if(!TerminateProcess(hProcess, 0xffffffff))
    {
        ShowLastError("TerminateProcess");
        dwRetVal=RTN_ERROR;
    }
*/

    // close handles
    CloseHandle(hToken);
    // CloseHandle(hProcess);

    return hProcess;
}

BOOL SetPrivilege(
    HANDLE hToken,          // token handle
    LPCTSTR Privilege,      // Privilege to enable/disable
    BOOL bEnablePrivilege   // TRUE to enable.  FALSE to disable
    )
{
    TOKEN_PRIVILEGES tp;
    LUID luid;
    TOKEN_PRIVILEGES tpPrevious;
    DWORD cbPrevious=sizeof(TOKEN_PRIVILEGES);

    if(!LookupPrivilegeValue( NULL, Privilege, &luid )) return FALSE;

    // 
    // first pass.  get current privilege setting
    // 
    tp.PrivilegeCount           = 1;
    tp.Privileges[0].Luid       = luid;
    tp.Privileges[0].Attributes = 0;

    AdjustTokenPrivileges(
            hToken,
            FALSE,
            &tp,
            sizeof(TOKEN_PRIVILEGES),
            &tpPrevious,
            &cbPrevious
            );

    if (GetLastError() != ERROR_SUCCESS) return FALSE;

    // 
    // second pass.  set privilege based on previous setting
    // 
    tpPrevious.PrivilegeCount       = 1;
    tpPrevious.Privileges[0].Luid   = luid;

    if(bEnablePrivilege) {
        tpPrevious.Privileges[0].Attributes |= (SE_PRIVILEGE_ENABLED);
    }
    else {
        tpPrevious.Privileges[0].Attributes ^= (SE_PRIVILEGE_ENABLED &
            tpPrevious.Privileges[0].Attributes);
    }

    AdjustTokenPrivileges(
            hToken,
            FALSE,
            &tpPrevious,
            cbPrevious,
            NULL,
            NULL
            );

    if (GetLastError() != ERROR_SUCCESS) return FALSE;

    return TRUE;
}