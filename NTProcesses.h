#ifndef __NTPROCESSES_H__
#define __NTPROCESSES_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <vdmdbg.h>
#include <string.h>
#include <afxtempl.h>

#include "psapi.h"
#include "WindowInfo.h"
#include "EnumProcess.h"

//Windows NT Functions
typedef BOOL (WINAPI *ENUMPROCESSES)
(
	DWORD*	lpidProcess,  // array to receive the process identifiers
	DWORD	cb,           // size of the array
	DWORD*	cbNeeded      // receives the number of bytes returned
);

typedef BOOL (WINAPI *ENUMPROCESSMODULES)
(
	HANDLE		hProcess,   // handle to the process
	HMODULE*	lphModule,  // array to receive the module handles
	DWORD		cb,         // size of the array
	LPDWORD		lpcbNeeded  // receives the number of bytes returned
);

typedef DWORD (WINAPI *GETMODULEFILENAME)
(
	HANDLE	hProcess,		// handle to the process
	HMODULE	hModule,		// handle to the module
	LPTSTR	lpstrFileName,	// array to receive filename
	DWORD	nSize			// size of filename array.
);

typedef DWORD (WINAPI *GETMODULEBASENAME)
(
	HANDLE	hProcess,		// handle to the process
	HMODULE	hModule,		// handle to the module
	LPTSTR	lpstrFileName,	// array to receive base name of module
	DWORD	nSize			// size of module name array.
);

typedef INT (WINAPI *VDMENUMTASKWOWEX)
(
	DWORD			dwProcessId,	// ID of NTVDM process
	TASKENUMPROCEX	fp,				// address of our callback function
	LPARAM			lparam			// anything we want to pass to the callback function.
);

class CWinNTProcesses : public CEnumProcess
{
public:
	// constructor
	CWinNTProcesses(CList<WindowInfo, WindowInfo> &processList);
	~CWinNTProcesses();
	enum { MAX_NUM = 1024 };

    HANDLE m_psapiHandle;
	HANDLE m_vdmdbgHandle;

	// Function Pointers
    ENUMPROCESSES       EnumProcesses;
    GETMODULEFILENAME   GetModuleFileName;
    ENUMPROCESSMODULES  EnumProcessModules;  
	VDMENUMTASKWOWEX	VDMEnumTaskWOWEx;
	GETMODULEBASENAME	GetModuleBaseName;

	// callback function
	static BOOL WINAPI EnumTasks(DWORD dwThreadId,
								 WORD hMod16,
								 WORD hTask16,
								 PSZ pszModName,
								 PSZ FileName,
                                 LPARAM lpUserDefined);
public:
	virtual BOOL ShowProcesses();
private:
	CString getFormattedString(char const *pFileName, DWORD dwProcessID);
	CList <WindowInfo, WindowInfo>& m_processList;
	void AddToList(char const *pFileName, DWORD dwProcessID);
};

#endif	// __NTPROCESSES_H__