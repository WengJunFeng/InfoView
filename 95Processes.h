#ifndef __95PROCESSES_H__
#define __95PROCESSES_H__

#include <tlhelp32.h>   // Toolhelp 32
#include <afxtempl.h>
#include "EnumProcess.h"
#include "WindowInfo.h"

// Win95 functions
typedef BOOL (WINAPI *PROCESSWALK)
(
    HANDLE hSnapshot,    
    LPPROCESSENTRY32 lppe
);
 
typedef HANDLE (WINAPI *CREATESNAPSHOT)
(
    DWORD dwFlags, 
    DWORD th32ProcessID
); 

class CWin95Processes : public CEnumProcess
{
public:
	// constructor
    CWin95Processes(CList<WindowInfo, WindowInfo> &processList);

private:
	// private member variable(s)
	HANDLE			m_kernelHandle;
    HANDLE			m_SnapshotHandle;
    
	PROCESSENTRY32	m_processEntry32S;
    CREATESNAPSHOT	m_pFnCreateToolhelp32Snapshot; 
    PROCESSWALK		m_pFnProcess32First; 
    PROCESSWALK		m_pFnProcess32Next; 

public:
	// publicly Exported Functions
	virtual BOOL ShowProcesses();

private:
	// Helper functions
	CString getFormattedString(const PROCESSENTRY32& pe32S);
	void AddToList(const PROCESSENTRY32& pe32S);

private:
	// member variable(s)
	CList <WindowInfo, WindowInfo>& m_processList;
};

#endif	// __95PROCESSES_H__