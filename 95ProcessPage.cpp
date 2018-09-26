////////////////////////////////////////////////////////////////
// C95ProcessPage
// If this code works, it was written by Suresh.

// ProcessPage.cpp : implementation file
//

#include "stdafx.h"
#include "infoview.h"
#include "WindowInfo.h"
#include "95ProcessPage.h"
#include "InfoUtil.h"
#include "TabSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C95ProcessPage property page

IMPLEMENT_DYNCREATE(C95ProcessPage, CPropertyPage)

C95ProcessPage::C95ProcessPage() :
			  CPropertyPage(C95ProcessPage::IDD)
{
	//{{AFX_DATA_INIT(C95ProcessPage)
	//}}AFX_DATA_INIT
  	m_pEnumProcesses = new CWin95Processes(m_processList);
}

C95ProcessPage::~C95ProcessPage()
{
	if (m_pEnumProcesses)
	{
		delete m_pEnumProcesses;
		m_pEnumProcesses = NULL;
	}
}

void C95ProcessPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C95ProcessPage)
	DDX_Control(pDX, IDC_INVALID_WINDOW, m_InvalidWindow);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C95ProcessPage, CPropertyPage)
	//{{AFX_MSG_MAP(C95ProcessPage)
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_REFRESH, OnRefresh)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C95ProcessPage message handlers

BOOL C95ProcessPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	m_InvalidWindow.SetFontBold(TRUE);
	m_InvalidWindow.SetTextColor( 706 );	// Red color
	updateDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void C95ProcessPage::OnDestroy() 
{
	CPropertyPage::OnDestroy();
	if (m_pEnumProcesses)
	{
		delete m_pEnumProcesses;
		m_pEnumProcesses = NULL;
	}
}

void C95ProcessPage::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect rectProcess, rectThread;
	
	CStatic *pProcess = (CStatic *)GetDlgItem(IDC_PROCESS_ID);
	ASSERT(pProcess);
	pProcess->GetWindowRect(&rectProcess);

	CStatic *pThread = (CStatic *)GetDlgItem(IDC_THREAD_ID);
	ASSERT(pThread);
	pThread->GetWindowRect(&rectThread);
	
	this->ScreenToClient(&rectThread);
	this->ScreenToClient(&rectProcess);

	if (rectThread.PtInRect(point) || rectProcess.PtInRect(point))
		::SetCursor(AfxGetApp()->LoadCursor(IDC_HAND));

	CPropertyPage::OnMouseMove(nFlags, point);
}

BOOL C95ProcessPage::updateList()
{
	if (NULL != m_pEnumProcesses)
	{
		m_processList.RemoveAll();
		if ( TRUE == m_pEnumProcesses->ShowProcesses())
		{
			POSITION pos = m_processList.GetHeadPosition();
			while(pos)
			{
				DWORD dwThreadID, dwProcessID;
				CString strThread, strProcess;
				dwThreadID = GetWindowThreadProcessId(m_pWindowInfo->m_hwnd, &dwProcessID);
				WindowInfo wndInfo = m_processList.GetNext(pos);
				if (dwProcessID == wndInfo.m_pid)
				{
					CWnd *pWnd = CWnd :: FromHandle(m_pWindowInfo->m_hwnd);
					if (pWnd && IsWindow(m_pWindowInfo->m_hwnd))
					{
						strThread.Format("0x%08x", dwThreadID);
						strProcess.Format("0x%08x", dwProcessID);
						strProcess.MakeUpper();
						strThread.MakeUpper();

						CStatic *pProcess = (CStatic *)GetDlgItem(IDC_PROCESS_ID);
						ASSERT(pProcess);
						pProcess->SetWindowText(strProcess);

						CStatic *pThread = (CStatic *)GetDlgItem(IDC_THREAD_ID);
						ASSERT(pThread);
						pThread->SetWindowText(strThread);
					}

					CEdit *pModuleName = (CEdit *)GetDlgItem(IDC_MODUE_NAME);
					ASSERT(NULL != pModuleName);
					pModuleName->SetWindowText(wndInfo.m_exeFile);
				
					CString strBuffer = _T("");
					strBuffer.Format("%d", wndInfo.m_noOfThreads);
					CStatic *pNoTh = (CStatic *)GetDlgItem(IDC_NUM_THREADS);
					pNoTh->SetWindowText(strBuffer);

					strBuffer.Empty();
					strBuffer.Format("%08x", wndInfo.m_parendPID);
					strBuffer.MakeUpper();
					CStatic *pParentPID = (CStatic *)GetDlgItem(IDC_STATIC_PARENTPID);
					pParentPID->SetWindowText(strBuffer);

					CStatic *pPriority = (CStatic *)GetDlgItem(IDC_PRIORITY_BASE);
					ASSERT(NULL != pPriority);
					strBuffer.Empty();
					strBuffer.Format("%d", wndInfo.m_threadPriority);
					strBuffer.MakeUpper();
					pPriority->SetWindowText(strBuffer);

					// Found our PID, come out of loop
					break;
				}
			}
		}
	}
	return TRUE;
}

LRESULT C95ProcessPage::OnRefresh(WPARAM wp, LPARAM lp)
{
	updateDialog();
	return 0;
}

void C95ProcessPage::updateDialog()
{
	CTabSheet *pSheet = (CTabSheet *)GetParent();
	ASSERT(NULL != pSheet);

	m_pWindowInfo = pSheet->getWindowInfoObj();

	CStatic *pStatic = (CStatic *)GetDlgItem(IDC_INVALID_WINDOW);
	ASSERT(NULL != pStatic);
	if (!::IsWindow(m_pWindowInfo->m_hwnd))
	{
		HideAllControls(this);
		pStatic->ShowWindow(SW_SHOW);
		return;
	}
	else
	{
		UnHideAllControls(this);
		pStatic->ShowWindow(SW_HIDE);
	}
	updateList();
}

BOOL C95ProcessPage::OnSetActive() 
{
	updateDialog();	
	return CPropertyPage::OnSetActive();
}
