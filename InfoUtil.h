#ifndef __INFOUTIL_H__
#define __INFOUTIL_H__

	CString GetResourceString(UINT uid);

	void HideAllControls(CWnd *pParentWnd);
	void UnHideAllControls(CWnd *pParentWnd);

	BOOL IsWindowsNT(void);
	BOOL IsWindows95(void);

	void ShowLastError(const char* pTitle);
	void ShowError(const char* pErrorString, const char* pTitle);

	// Clipboard helper
	void Copy2Clipboard(CString& str);

	BOOL SetPrivilege( HANDLE hToken,          // token handle
						LPCTSTR Privilege,      // Privilege to enable/disable
						BOOL bEnablePrivilege   // TRUE to enable.  FALSE to disable
					);

	HANDLE GetProcessHandle(DWORD pid);

#endif // __INFOUTIL_H__