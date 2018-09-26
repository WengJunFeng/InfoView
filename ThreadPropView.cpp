// CCThreadPropView message handlers
// CThreadPropView.cpp : implementation file
//

#include "stdafx.h"
#include "InfoView.h"
#include "ThreadPropView.h"
#include "Infoutil.h"

// CThreadPropView

IMPLEMENT_DYNCREATE(CThreadPropView, CFormView)

CThreadPropView::CThreadPropView()
	: CFormView(CThreadPropView::IDD)
{
	m_brush.CreateSolidBrush(RGB(255, 251, 255));
}

CThreadPropView::~CThreadPropView()
{

}

void CThreadPropView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CThreadPropView, CFormView)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CThreadPropView diagnostics

#ifdef _DEBUG
void CThreadPropView::AssertValid() const
{
	CFormView::AssertValid();
}

void CThreadPropView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

// CThreadPropView message handlers

HBRUSH CThreadPropView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    return m_brush;
}

BOOL CThreadPropView::SetCurrentThreadEntry (DWORD dwThreadID)
{ 
    HANDLE        hThreadSnap = NULL;
    BOOL          bRet        = FALSE; 
 
    // Take a snapshot of all threads currently in the system. 
    hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0); 
    if (hThreadSnap == INVALID_HANDLE_VALUE)
        return (FALSE); 
 
    // Fill in the size of the structure before using it
    m_te32.dwSize = sizeof(THREADENTRY32);
 
    // Walk the thread snapshot to find all threads of the process. 
    // If the thread belongs to the process, add its information 
    // to the display list. 
    if (Thread32First(hThreadSnap, &m_te32)) 
    { 
        do { 
            if (m_te32.th32ThreadID == dwThreadID) {
				// Do not forget to clean up the snapshot object
			    CloseHandle (hThreadSnap);
				return true;
            } 
        } 
        while (Thread32Next(hThreadSnap, &m_te32));
        bRet = TRUE;
    } 
    else 
        bRet = FALSE;          // could not walk the list of threads 
    return (bRet);
}

void CThreadPropView::RefreshDialog(DWORD dwThreadID) 
{
	m_currentThreadID = dwThreadID;

	// thread ID
	// thread State
	// wait reason
	// context switches
	// creation time
	// exit time
	// kernel time
	// user time

	CWnd *pChildWnd = this->GetWindow(GW_CHILD);
	while (pChildWnd)
	{
		TCHAR clsname[512];
		::GetClassName(pChildWnd->GetSafeHwnd(), clsname, 511 );
		if ( _tcsicmp(clsname, "Edit") == 0)
		{
			pChildWnd->SetWindowText("--");
		}
		pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
	}
	
	HANDLE threadHandle = OpenThread (THREAD_QUERY_INFORMATION, TRUE, m_currentThreadID); 
	FILETIME creationTime;
	FILETIME exitTime;
	FILETIME kernelTime;
	FILETIME userTime;

	BOOL bRet = GetThreadTimes(threadHandle, &creationTime, &exitTime, &kernelTime, &userTime );
	if (!bRet)
	{
		if ( NULL != threadHandle )
			CloseHandle(threadHandle);
		return;
	}

	ShowTime(creationTime, IDC_THREAD_CREATION_TIME);
	ShowTime(exitTime, IDC_THREAD_EXIT_TIME);
	ShowTime(kernelTime, IDC_THREAD_KERNEL_TIME);
	ShowTime(userTime, IDC_THREAD_USER_TIME);

	// set the thread id
	CString strThreadID;
	strThreadID.Format("%d", m_currentThreadID);
	SetEditText(IDC_THREAD_ID, strThreadID);

	/*
		IDC_THREAD_ID
		IDC_THREAD_STATE
		IDC_THREAD_WAIT_REASON
		IDC_THREAD_CONTEXT_SWITCHES

		IDC_THREAD_CREATION_TIME
		IDC_THREAD_EXIT_TIME
		IDC_THREAD_KERNEL_TIME
		IDC_THREAD_USER_TIME
	*/
	int threadPriorityLevel = GetThreadPriority(threadHandle);
	TCHAR *pPriorityDescription = "Normal";
	switch(threadPriorityLevel)
	{
		case THREAD_PRIORITY_ABOVE_NORMAL:
			pPriorityDescription = "Above Normal";
			break;
		case THREAD_PRIORITY_BELOW_NORMAL:
			pPriorityDescription = "Below Normal";
			break;
		case THREAD_PRIORITY_HIGHEST:
			pPriorityDescription = "Highest";
			break;
		case THREAD_PRIORITY_IDLE:
			pPriorityDescription = "Idle";
			break;
		case THREAD_PRIORITY_LOWEST:
			pPriorityDescription = "Lowest";
			break;
		case THREAD_PRIORITY_NORMAL:
			pPriorityDescription = "Normal";
			break;
		case THREAD_PRIORITY_TIME_CRITICAL:
			pPriorityDescription = "Critical";
			break;
	}

	SetEditText(IDC_THREAD_STATE, pPriorityDescription);

	CloseHandle(threadHandle);
}

CString CThreadPropView::GetFormattedTime(SYSTEMTIME sysTime)
{
	// returns in month/day/year hour:Minutes:sec format
	CString strTime;
	strTime.Format("%d/%d/%d %d:%d:%d %4ld", sysTime.wMonth, sysTime.wDay, 
											sysTime.wYear, sysTime.wHour, 
											sysTime.wMinute, sysTime.wSecond, 
											sysTime.wMilliseconds);
	return strTime;
}

void CThreadPropView::SetEditText(UINT nID, CString strData)
{
	CEdit *pEdit = (CEdit *)this->GetDlgItem(nID);
	ASSERT(pEdit);

	pEdit->SetWindowText(strData);
}

BOOL CThreadPropView::ConvertFileTimeToSystemTime(FILETIME ft, SYSTEMTIME* st)
{
	return FileTimeToSystemTime(&ft, st);
}

void CThreadPropView::ShowTime(FILETIME fTime, UINT nID)
{
	SYSTEMTIME sTime;
	if ( TRUE == ConvertFileTimeToSystemTime(fTime, &sTime) )
	{
		CString theTime = GetFormattedTime(sTime);
		SetEditText(nID, theTime);
	}
}