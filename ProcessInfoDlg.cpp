// ProcessInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InfoView.h"
#include "ProcessInfoDlg.h"
#include <winbase.h>
#include ".\processinfodlg.h"

// CProcessInfoDlg dialog

IMPLEMENT_DYNAMIC(CProcessInfoDlg, CDialog)
CProcessInfoDlg::CProcessInfoDlg(DWORD_PTR dwData, CWnd* pParent /*=NULL*/)
	: CDialog(CProcessInfoDlg::IDD, pParent),
	m_currentProcessID(dwData)
{

}

CProcessInfoDlg::~CProcessInfoDlg()
{

}

void CProcessInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CProcessInfoDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_REFRESH, OnBnClickedBtnRefresh)
END_MESSAGE_MAP()


// CProcessInfoDlg message handlers
BOOL CProcessInfoDlg::OnInitDialog()
{
	RefreshDialog();
	return TRUE;
}

//
CEdit* CProcessInfoDlg::GetEditCtrl(UINT nID)
{
	CEdit *pEdit = (CEdit *)this->GetDlgItem(nID);
	ASSERT(pEdit);

	return pEdit;
}

//
void CProcessInfoDlg::SetEditText(CEdit *pEdit, CString strData)
{
	pEdit->SetWindowText(strData);
}

//
void CProcessInfoDlg::SetEditText(CEdit *pEdit, long lData)
{
	CString str;
	str.Format("%ld", lData);
	SetEditText(pEdit, str);
}
//
CString CProcessInfoDlg::GetLastErrorString()
{
	CString str;
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
	str = ptr;
	LocalFree(ptr);

	return str;
}

//
void CProcessInfoDlg::ShowHandlesCount()
{
	HANDLE hProcess = GetProcessHandle();
	if ( (hProcess == NULL ) || (hProcess == INVALID_HANDLE_VALUE ))
	{
		SetEditText( GetEditCtrl(IDC_PROCESS_HANDLE_COUNT), "--" );
	}
	else
	{
		DWORD dwHandlesCount = (DWORD) -1;
		GetProcessHandleCount(hProcess, &dwHandlesCount );
		if ( dwHandlesCount != -1 )
		{
			SetEditText( GetEditCtrl(IDC_PROCESS_HANDLE_COUNT), dwHandlesCount );
		}
		else
		{
			SetEditText( GetEditCtrl(IDC_PROCESS_HANDLE_COUNT), "--" );
		}
	}
}

//
void CProcessInfoDlg::ShowProcessTimes()
{
	HANDLE hProcess = GetProcessHandle();
	if ( (hProcess == NULL) || (hProcess == INVALID_HANDLE_VALUE) )
	{
		CString strErr = "--";
		SetEditText( GetEditCtrl(IDC_PROCESS_CREATION_TIME), strErr);
		SetEditText( GetEditCtrl(IDC_PROCESS_EXIT_TIME), strErr);
		SetEditText( GetEditCtrl(IDC_PROCESS_KERNEL_TIME), strErr);
		SetEditText( GetEditCtrl(IDC_PROCESS_USER_TIME), strErr);
	}
	else
	{
		FILETIME creationTime, exitTime, kernelTime, userTime;
		BOOL bRet = GetProcessTimes(hProcess, &creationTime, &exitTime, &kernelTime, &userTime );
		if ( bRet == FALSE )
		{
			CString strErr = "--";
			SetEditText( GetEditCtrl(IDC_PROCESS_CREATION_TIME), strErr);
			SetEditText( GetEditCtrl(IDC_PROCESS_EXIT_TIME), strErr);
			SetEditText( GetEditCtrl(IDC_PROCESS_KERNEL_TIME), strErr);
			SetEditText( GetEditCtrl(IDC_PROCESS_USER_TIME), strErr);
		}
		else
		{
			SYSTEMTIME creationSysTime;
			BOOL retVal = ConvertFileTimeToSystemTime(creationTime, &creationSysTime);
			if ( retVal )
			{
				SetEditText( GetEditCtrl(IDC_PROCESS_CREATION_TIME), GetFormattedTime(creationSysTime));
			}
			else
			{
				SetEditText( GetEditCtrl(IDC_PROCESS_CREATION_TIME), "--");
			}

			SYSTEMTIME exitSysTime;
			retVal = ConvertFileTimeToSystemTime(exitTime, &exitSysTime);
			if ( retVal )
			{
				SetEditText( GetEditCtrl(IDC_PROCESS_EXIT_TIME), GetFormattedTime(exitSysTime));
			}
			else
			{
				SetEditText( GetEditCtrl(IDC_PROCESS_EXIT_TIME), "--");
			}

			SYSTEMTIME kernelSysTime;
			retVal = ConvertFileTimeToSystemTime(kernelTime, &kernelSysTime);
			if ( retVal )
			{
				SetEditText( GetEditCtrl(IDC_PROCESS_KERNEL_TIME), GetFormattedTime(kernelSysTime));
			}
			else
			{
				SetEditText( GetEditCtrl(IDC_PROCESS_KERNEL_TIME), "--");
			}

			SYSTEMTIME userSysTime;
			retVal = ConvertFileTimeToSystemTime(userTime, &userSysTime);
			if ( retVal )
			{
				SetEditText( GetEditCtrl(IDC_PROCESS_USER_TIME), GetFormattedTime(userSysTime));
			}
			else
			{
				SetEditText( GetEditCtrl(IDC_PROCESS_USER_TIME), "--");
			}
		}
	}
}

//
HANDLE CProcessInfoDlg::GetProcessHandle()
{
	HANDLE hProcess = NULL;
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, TRUE, m_currentProcessID);
	return hProcess;
}

//
BOOL CProcessInfoDlg::ConvertFileTimeToSystemTime(FILETIME ft, SYSTEMTIME* st)
{
	return FileTimeToSystemTime(&ft, st);
}

// 
CString CProcessInfoDlg::GetFormattedTime(SYSTEMTIME sysTime)
{
	// returns in month/day/year hour:Minutes:sec format
	CString strTime;
	strTime.Format("%d/%d/%d %d:%d:%d %4ld", sysTime.wMonth, sysTime.wDay, 
											sysTime.wYear, sysTime.wHour, 
											sysTime.wMinute, sysTime.wSecond, 
											sysTime.wMilliseconds);
	return strTime;
}
//
void CProcessInfoDlg::ShowPriority()
{
	HANDLE hProcess = GetProcessHandle();
	DWORD dwPriority = GetPriorityClass(hProcess);

	switch(dwPriority)
	{
		case ABOVE_NORMAL_PRIORITY_CLASS:
			SetEditText(GetEditCtrl(IDC_PROCESS_PRIORITY), "Above Normal");
			break;
		case BELOW_NORMAL_PRIORITY_CLASS:
			SetEditText(GetEditCtrl(IDC_PROCESS_PRIORITY), "Below Normal");
			break;
		case HIGH_PRIORITY_CLASS:
			SetEditText(GetEditCtrl(IDC_PROCESS_PRIORITY), "High");
			break;
		case IDLE_PRIORITY_CLASS:
			SetEditText(GetEditCtrl(IDC_PROCESS_PRIORITY), "Idle");
			break;
		case NORMAL_PRIORITY_CLASS:
			SetEditText(GetEditCtrl(IDC_PROCESS_PRIORITY), "Normal");
			break;
		case REALTIME_PRIORITY_CLASS:
			SetEditText(GetEditCtrl(IDC_PROCESS_PRIORITY), "Realtime");
			break;
	}
}
//
void CProcessInfoDlg::ShowGUIObjCount()
{
	HANDLE hProcess = GetProcessHandle();
	DWORD dwGDIObjCount = GetGuiResources( hProcess, GR_GDIOBJECTS );
	DWORD dwUserObjCount = GetGuiResources( hProcess, GR_USEROBJECTS );

	if ( 0 == dwGDIObjCount )
	{
		DWORD dwErr = GetLastError();
		if ( dwErr != ERROR_SUCCESS )
		{
			SetEditText( GetEditCtrl(IDC_PROCESS_GDI_OBJECTS_COUNT), "--");
		}
		else
		{
			SetEditText( GetEditCtrl(IDC_PROCESS_GDI_OBJECTS_COUNT), dwGDIObjCount);
		}
	}
	else
	{
		SetEditText( GetEditCtrl(IDC_PROCESS_GDI_OBJECTS_COUNT), dwGDIObjCount);
	}
	if ( 0 == dwUserObjCount )
	{
		DWORD dwErr = GetLastError();
		if ( dwErr != ERROR_SUCCESS )
		{
			SetEditText( GetEditCtrl(IDC_PROCESS_USER_OBJECTS_COUNT), "--");
		}
		else
		{
			SetEditText( GetEditCtrl(IDC_PROCESS_USER_OBJECTS_COUNT), dwUserObjCount);
		}
	}
	else
	{
		SetEditText( GetEditCtrl(IDC_PROCESS_USER_OBJECTS_COUNT), dwUserObjCount);
	}
}
//
void CProcessInfoDlg::ShowProcessExeName()
{
	SetEditText( GetEditCtrl(IDC_PROCESS_MODULE_NAME), m_pe.szExeFile);
}

//
void CProcessInfoDlg::ShowThreadCount()
{
	SetEditText( GetEditCtrl(IDC_PROCESS_THREAD_COUNT), m_pe.cntThreads);
}
//
void CProcessInfoDlg::ShowPriorityBase()
{
	SetEditText( GetEditCtrl(IDC_PROCESS_PRIORITY_BASE), m_pe.pcPriClassBase);
}

//
void CProcessInfoDlg::ShowCurrentUsage()
{
	SetEditText( GetEditCtrl(IDC_PROCESS_CURRENT_USAGE), m_pe.cntUsage);
}
//
void CProcessInfoDlg::ShowIOCounters()
{
	HANDLE hProcess = GetProcessHandle();
	IO_COUNTERS ioCtrs;

	if ( 0 == GetProcessIoCounters(hProcess, &ioCtrs) )
	{
		CString strError = "--";
		SetEditText( GetEditCtrl(IDC_PROCESS_READ_OPERATIONS), strError);
		SetEditText( GetEditCtrl(IDC_PROCESS_WRITE_OPERATIONS), strError);
		SetEditText( GetEditCtrl(IDC_PROCESS_OTHER_OPERATIONS), strError);

		SetEditText( GetEditCtrl(IDC_PROCESS_READ_TRANSFERS), strError);
		SetEditText( GetEditCtrl(IDC_PROCESS_WRITE_TRANSFERS), strError);
		SetEditText( GetEditCtrl(IDC_PROCESS_OTHER_TRANSFERS), strError);

		return;
	}
	else
	{
		SetEditText( GetEditCtrl(IDC_PROCESS_READ_OPERATIONS), (LONG)ioCtrs.ReadOperationCount);
		SetEditText( GetEditCtrl(IDC_PROCESS_WRITE_OPERATIONS), (LONG)ioCtrs.WriteOperationCount);
		SetEditText( GetEditCtrl(IDC_PROCESS_OTHER_OPERATIONS), (LONG)ioCtrs.OtherOperationCount);

		SetEditText( GetEditCtrl(IDC_PROCESS_READ_TRANSFERS), (LONG)ioCtrs.ReadTransferCount);
		SetEditText( GetEditCtrl(IDC_PROCESS_WRITE_TRANSFERS), (LONG)ioCtrs.WriteTransferCount);
		SetEditText( GetEditCtrl(IDC_PROCESS_OTHER_TRANSFERS), (LONG)ioCtrs.OtherTransferCount);
	}
}

void CProcessInfoDlg::RefreshDialog()
{
	SetCurrentProcessEntry();

	SetEditText( GetEditCtrl(IDC_PROCESS_ID), m_currentProcessID );
	
	// Handles Count
	ShowHandlesCount();

	// Show Times
	ShowProcessTimes();

	// Priority
	ShowPriority();

	// GUI Usage
	ShowGUIObjCount();

	// ModuleName
	ShowProcessExeName();

	// Thread Count
	ShowThreadCount();

	// Current reference
	ShowCurrentUsage();

	// Priority Base
	ShowPriorityBase();

	// IO Counters
	ShowIOCounters();
}

void CProcessInfoDlg::SetCurrentProcessEntry()
{
		// .ctor
	HANDLE         hProcessSnap = NULL;
    BOOL           bRet			= FALSE;

	//  Take a snapshot of all processes in the system. 
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 

    if (hProcessSnap == INVALID_HANDLE_VALUE) 
        return; 
 
    //  Fill in the size of the structure before using it. 
    m_pe.dwSize = sizeof(PROCESSENTRY32); 

	//  Walk the snapshot of the processes
	if (Process32First(hProcessSnap, &m_pe)) 
    {
         do
        {
			if (m_pe.th32ProcessID == m_currentProcessID)
			{
				// Got the current processentry structure!
				return;
			}
        } 
        while (Process32Next(hProcessSnap, &m_pe));
    } 
}

void CProcessInfoDlg::OnBnClickedBtnRefresh()
{
	RefreshDialog();
}

void CProcessInfoDlg::SetProcessID(DWORD dwProcID)
{
	m_currentProcessID = dwProcID;
}