#include "stdafx.h"
#include "95Processes.h"

// CWin95Processes Constructor
CWin95Processes::CWin95Processes(CList<WindowInfo,WindowInfo> &processList)
				:m_processList(processList)
{
	m_kernelHandle = GetModuleHandle("KERNEL32.DLL");
	if ( NULL == m_kernelHandle )
        return ;

	m_pFnCreateToolhelp32Snapshot	= (CREATESNAPSHOT)GetProcAddress((HINSTANCE)m_kernelHandle, "CreateToolhelp32Snapshot");
	m_pFnProcess32First				= (PROCESSWALK)GetProcAddress((HINSTANCE)m_kernelHandle, "Process32First");
	m_pFnProcess32Next				= (PROCESSWALK)GetProcAddress((HINSTANCE)m_kernelHandle, "Process32Next");
	if (NULL == m_pFnCreateToolhelp32Snapshot	||
		NULL == m_pFnProcess32First				||
        NULL == m_pFnProcess32First)
        return;
    m_processEntry32S.dwSize = sizeof(m_processEntry32S);
}

BOOL CWin95Processes::ShowProcesses()
{
    if (NULL == m_pFnCreateToolhelp32Snapshot	||
		NULL == m_pFnProcess32First				||
        NULL == m_pFnProcess32Next)
        return FALSE;

		m_SnapshotHandle = m_pFnCreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	/*
	Take a snapshot of the system state, and walk the list by calling
	Process32First once, and then by calling Proces32Next repeatedly until
	we get to the end of the list.
	*/

    m_pFnProcess32First(m_SnapshotHandle, &m_processEntry32S);
	
	// call the base class function to show the Process
    // DisplayProcess(m_outPutWindowHandle, m_processEntry32S.szExeFile, m_processEntry32S.th32ProcessID);

	AddToList(m_processEntry32S);
    while (TRUE == m_pFnProcess32Next(m_SnapshotHandle, &m_processEntry32S))
	{
		AddToList(m_processEntry32S);
	}

	/*	
	This should happen automatically when we terminate, but it never
	hurts to clean up after ourselves.
	*/
    CloseHandle(m_SnapshotHandle);
    return TRUE;
}

CString CWin95Processes::getFormattedString(const PROCESSENTRY32& pe32S)
{
	CString strThreadPriority = _T(""), strPID = _T(""), strNoThreads = _T(""), strExeName = _T(""), strTemp = _T("");

	strPID.Format("PID: 0x%08x", m_processEntry32S.th32ProcessID);
	strExeName.Format("%s", m_processEntry32S.szExeFile);
	strNoThreads.Format(" (%d) ", m_processEntry32S.cntThreads);
	strThreadPriority.Format(" [ %d ] ", m_processEntry32S.pcPriClassBase);

	strTemp = strNoThreads + strThreadPriority + strExeName + _T(" ") + strPID;
	return strTemp;
}

void CWin95Processes::AddToList(const PROCESSENTRY32& pe32S)
{
	WindowInfo wndInfo;
	wndInfo.m_pid				= m_processEntry32S.th32ProcessID;
	wndInfo.m_noOfThreads		= m_processEntry32S.cntThreads;
	wndInfo.m_parendPID			= m_processEntry32S.th32ParentProcessID;
	wndInfo.m_threadPriority	= m_processEntry32S.pcPriClassBase;
	wndInfo.m_usageCount		= m_processEntry32S.cntUsage;
	_tcscpy(wndInfo.m_exeFile, m_processEntry32S.szExeFile);

	m_processList.AddTail(wndInfo);
}