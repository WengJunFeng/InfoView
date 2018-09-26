#include "stdafx.h"
#include "NTProcesses.h"
#include "InfoUtil.h"

// constructor
CWinNTProcesses::CWinNTProcesses(CList<WindowInfo,WindowInfo> &processList)
				:m_processList(processList)
{
    m_psapiHandle	= LoadLibrary("PSAPI.DLL");
    m_vdmdbgHandle	= LoadLibrary("VDMDBG.DLL");

	if ( NULL == m_psapiHandle)
	{
		ShowError("Unable to load the library (PSAPI.DLL)", "Load Library");
        return;
	}
	if ( NULL == m_vdmdbgHandle )
	{
		ShowError("Unable to load the library (VDMDBG.DLL)", "Load Library");
        return;
	}

	EnumProcesses		= (ENUMPROCESSES)GetProcAddress((HINSTANCE)m_psapiHandle,		"EnumProcesses");
	VDMEnumTaskWOWEx	= (VDMENUMTASKWOWEX)GetProcAddress((HINSTANCE)m_vdmdbgHandle,	"VDMEnumTaskWOWEx");
	GetModuleFileName	= (GETMODULEFILENAME)GetProcAddress((HINSTANCE)m_psapiHandle,	"GetModuleFileNameExA");
	GetModuleBaseName	= (GETMODULEBASENAME)GetProcAddress((HINSTANCE)m_psapiHandle,	"GetModuleBaseNameA");
	EnumProcessModules	= (ENUMPROCESSMODULES)GetProcAddress((HINSTANCE)m_psapiHandle,	"EnumProcessModules");

	if (NULL == EnumProcesses)
	{
		ShowError("PSAPI.DLL::Unable to get procedure addresses", "EnumProcesses");
		return;
	}
	if (NULL == GetModuleFileName)
	{
		ShowError("PSAPI.DLL::Unable to get procedure addresses", "GetModuleFileName");
		return;
	}
	if (NULL == GetModuleBaseName)
	{
		ShowError("PSAPI.DLL::Unable to get procedure addresses", "GetModuleBaseName");
		return;
	}
	if (NULL == EnumProcessModules )
	{
		ShowError("PSAPI.DLL::Unable to get procedure addresses", "EnumProcessModules");
        return;
	}
	if (NULL == VDMEnumTaskWOWEx)
	{
		ShowError("VDMDBG.DLL::Unable to get procedure addresses", "VDMEnumTaskWOWEx");
        return;
	}
}

CWinNTProcesses::~CWinNTProcesses()
{
	// Destructor
	if (m_psapiHandle)
	{
		::FreeLibrary((HINSTANCE)m_psapiHandle);
		m_psapiHandle = NULL;
	}
	if (m_vdmdbgHandle)
	{
		::FreeLibrary((HINSTANCE)m_vdmdbgHandle);
		m_psapiHandle = NULL;
	}
}

BOOL CWinNTProcesses::ShowProcesses()
{
	DWORD process_ids[MAX_NUM];
	DWORD num_processes;
	
    if (NULL == VDMEnumTaskWOWEx	||
		NULL == EnumProcesses		||
		NULL == GetModuleFileName	||
		NULL == GetModuleBaseName	||
		NULL == EnumProcessModules  )
        return FALSE;
	
    int success = EnumProcesses(process_ids, sizeof(process_ids), &num_processes);
    num_processes /= sizeof(process_ids[0]);
	
	if ( !success )
	{
		ShowLastError("EnumProcesses");
		return FALSE;
	}  
    for ( unsigned i = 0; i < num_processes; i++ )
	{
        HANDLE CurrentProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
									  FALSE, 
									  process_ids[i]);
		HMODULE modules[MAX_NUM];
        DWORD num_modules;
        TCHAR file_name[MAX_PATH];
		
        EnumProcessModules(CurrentProcess, modules, sizeof(modules), &num_modules);
		num_modules /= sizeof(modules[0]);
		
        if (GetModuleFileName(CurrentProcess, modules[0], file_name, sizeof(file_name)/sizeof(TCHAR)))
        {
			// Add to list
			AddToList(file_name, process_ids[i]);

			GetModuleBaseName(CurrentProcess, modules[0], file_name, sizeof(file_name)/sizeof(TCHAR));
            if ( 0 == _stricmp(file_name, "NTVDM.EXE"))
            {
                // We've got an NT VDM -- enumerate the processes
                // it contains.
                VDMEnumTaskWOWEx(process_ids[i], EnumTasks, (LPARAM)this);
            }
        }
        CloseHandle(CurrentProcess);
    }
	FreeLibrary((HINSTANCE)m_vdmdbgHandle);
	FreeLibrary((HINSTANCE)m_psapiHandle);
	m_vdmdbgHandle = NULL;
	m_psapiHandle = NULL;
	return TRUE;
}

BOOL WINAPI CWinNTProcesses::EnumTasks( DWORD	dwThreadId,
										WORD	hMod16,
										WORD	hTask16,
										PSZ		pszModName,
										PSZ		FileName,
										LPARAM	lpUserDefined)
{
	CWinNTProcesses *pWinNTProcesses = (CWinNTProcesses *)lpUserDefined;
	ASSERT(pWinNTProcesses);
	pWinNTProcesses->AddToList(FileName, hTask16);
	return TRUE;
}

CString CWinNTProcesses::getFormattedString(char const *pFileName, DWORD dwProcessID)
{
	CString strTemp = _T(""), strPID = _T(""), strExeName = _T("");

	strPID.Format("PID: 0x%08x", dwProcessID);
	strExeName.Format("%s", pFileName);

	strTemp =  strPID + _T(" ") + strExeName;
	return strTemp;
}

void CWinNTProcesses::AddToList(char const *pFileName, DWORD dwProcessID)
{
	WindowInfo wndInfo;
	wndInfo.m_pid				= dwProcessID;
	_tcscpy(wndInfo.m_exeFile, pFileName);
	// add to list.
	m_processList.AddTail(wndInfo);
}