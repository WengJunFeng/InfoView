// ProcessThreadsView.cpp : implementation file
//

#include "stdafx.h"
#include "infoview.h"
#include "ChildFrm.h"
#include "MainFrm.h"
#include "ProcessThreadsView.h"
#include "InfoUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CProcessThreadsView::staticint = 0;
/////////////////////////////////////////////////////////////////////////////
// CProcessThreadsView

IMPLEMENT_DYNCREATE(CProcessThreadsView, CTreeView)

CProcessThreadsView::CProcessThreadsView()
{
	m_pEnumProcesses = NULL;
}

//CProcessThreadsView::CProcessThreadsView() : m_List()
//{
//	m_pEnumProcesses = NULL;
//}

CProcessThreadsView::~CProcessThreadsView()
{

}

BEGIN_MESSAGE_MAP(CProcessThreadsView, CTreeView)
	//{{AFX_MSG_MAP(CProcessThreadsView)
	ON_WM_DESTROY()
	ON_WM_COPYDATA()
	ON_COMMAND(IDM_REFRESH, OnRefresh)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_COMMAND(IDM_KILL_PROCESS, OnKillProcess)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcessThreadsView drawing

void CProcessThreadsView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CProcessThreadsView diagnostics

#ifdef _DEBUG
void CProcessThreadsView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CProcessThreadsView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CProcessThreadsView message handlers

BOOL CProcessThreadsView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |=  TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;
	return CTreeView::PreCreateWindow(cs);
}

void CProcessThreadsView::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
	// TODO: You may populate your TreeView with items by directly accessing
	// its tree control through a call to GetTreeCtrl().

	// create Imagelist and attach to Tree control.
	m_ImageList.Create(16, 16, ILC_MASK, 0, 8);
	m_hIcon[0] = AfxGetApp()->LoadIcon(IDI_CLOSE_FOLDER);
	m_hIcon[1] = AfxGetApp()->LoadIcon(IDI_OPEN_FOLDER);
	for( int i=0; i< 2; i++)
		m_ImageList.Add(m_hIcon[i]);
	GetTreeCtrl().SetImageList(&m_ImageList, TVSIL_NORMAL);

	if (IsWindows95())
	{
		m_pEnumProcesses = new CWin95Processes(m_processList);
	}
	else if (IsWindowsNT())
	{
        m_pEnumProcesses = new CWinNTProcesses(m_processList);
	}
    else
	{
		ShowError("This operation is currently not supported.\r\nInfoView runs only on Windows 95, 98, ME, NT, 2000 and XP opearating systems.", "InfoView");
		CChildFrame *pChildFrame = (CChildFrame *) GetParent();
		ASSERT(NULL != pChildFrame);
		pChildFrame->PostMessage(WM_CLOSE, 0L, 0L);
		return;
	}

	// Show status message and Wait cursor as it takes a little bit more time...
	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0, "Please Wait... (it should take less than a minute!)");
	CWaitCursor waitcursor;

	UpdateTree();
	// UpdateTreeNew();

	CChildFrame *pChildFrame = (CChildFrame *) GetParent();
	pChildFrame->SetWindowText("Processes & Threads");

}

BOOL CProcessThreadsView::UpdateTreeNew()
{
	// Delete all items before inserting
	GetTreeCtrl().DeleteAllItems();

	PROCINFO ProcInfo[256];
	DWORD dw, i;
	if (!EnumProcessNames(&ProcInfo[0], 256, &dw))
	{
		ShowError("Unable to enumerate processes. Make sure you have enough access to do the operation.", "InfoView");
	}
	else
	{
		// succeded
		HTREEITEM hParent = NULL;
		char buf[256];
		for (i = 0; i < dw; i++)
		{
			sprintf(buf, "%s (0x%08x)", ProcInfo[i].szProcName, ProcInfo[i].dwPID);

			TV_INSERTSTRUCT insertStruct;
			insertStruct.hParent				= hParent;
			insertStruct.hInsertAfter			= TVI_LAST;
			insertStruct.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			insertStruct.item.cChildren			= 1;
			insertStruct.item.state				= TVIS_BOLD;
			insertStruct.item.iImage			= 0;
			insertStruct.item.iSelectedImage	= 0;
			insertStruct.item.pszText			= buf;
			insertStruct.item.lParam			=(LPARAM)hParent;

			HTREEITEM hti = GetTreeCtrl().InsertItem(&insertStruct);
			// show the thread information
			ShowThreads(hti, ProcInfo[i]);
		}
	}
	return true;
}

BOOL CProcessThreadsView::ShowThreads(const HTREEITEM& hItem, const PROCINFO& pi)
{
	THREADINFO aThreadInfo[512];
	DWORD dw, i;

	char buf[1024];
	if (!EnumProcessThreadsEx(pi.dwPID, &aThreadInfo[0], 512, &dw))
	{
		ShowError("Unable to enumerate processes. Make sure you have enough access to do the operation.", "InfoView");
	}
	else
	{
		for (i = 0; i < dw; i++)
		{
			sprintf(buf, "ThreadId = (0x%08x), StartAddress : (0x%08x), ThreadState = (0x%08x), ThreadWaitReason = (0x%08x)", aThreadInfo[i].dwThreadId, (DWORD)aThreadInfo[i].lpStartAddress, aThreadInfo[i].dwThreadState, aThreadInfo[i].dwThreadWaitReason);
			TV_INSERTSTRUCT insertStruct;
			insertStruct.hParent				= hItem;
			insertStruct.hInsertAfter			= TVI_LAST;
			insertStruct.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			insertStruct.item.cChildren			= 1;
			insertStruct.item.state				= TVIS_BOLD;
			insertStruct.item.iImage			= 1;
			insertStruct.item.iSelectedImage	= 1;
			insertStruct.item.lParam			=(LPARAM)NULL;

			insertStruct.item.pszText = buf;
			GetTreeCtrl().InsertItem(&insertStruct);
		}
	}
	return TRUE;
}

BOOL CProcessThreadsView::UpdateTree()
{
	CWaitCursor waitcursor;
	if (NULL != m_pEnumProcesses)
	{
		GetTreeCtrl().DeleteAllItems();
		m_processList.RemoveAll();
		if ( TRUE == m_pEnumProcesses->ShowProcesses())
		{
			POSITION prevPos = NULL;
			POSITION pos = m_processList.GetHeadPosition();
			prevPos = pos;
			while(pos)
			{
				WindowInfo wndInfo = m_processList.GetNext(pos);
				TV_INSERTSTRUCT insertStruct;
				insertStruct.hParent				= NULL;
				insertStruct.hInsertAfter			= TVI_LAST;
				insertStruct.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
				insertStruct.item.cChildren			= 1;
				insertStruct.item.state				= TVIS_BOLD;
				insertStruct.item.iImage			= 0;
				insertStruct.item.iSelectedImage	= 0;
				insertStruct.item.pszText = wndInfo.m_exeFile;
				insertStruct.item.lParam =(LPARAM)prevPos;

				HTREEITEM hti = GetTreeCtrl().InsertItem(&insertStruct);
				// show the properties
				ShowProperties(hti);
				prevPos = pos;
			}
		}
	}
	return TRUE;
}

void CProcessThreadsView::OnDestroy() 
{
	CTreeView::OnDestroy();
	
	if (NULL != m_pEnumProcesses)
	{
		delete m_pEnumProcesses;
		m_pEnumProcesses = NULL;
	}
}

void CProcessThreadsView::OnRefresh()
{
	CWaitCursor wait;
	UpdateTree();
}

void CProcessThreadsView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CWaitCursor waitcursor;
	CMenu menu;
	if (menu.LoadMenu(IDR_PROCESSES_POPUPMENU))
	{
		CMenu *pSubMenu = menu.GetSubMenu(0);
		ASSERT(pSubMenu);
		CPoint position;
		::GetCursorPos(&position);
		this->SetForegroundWindow();
		pSubMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, position.x, position.y, this, NULL);
		this->PostMessage(WM_NULL, 0, 0);
	}	
	*pResult = 0;
}

void CProcessThreadsView::OnKillProcess()
{
	HTREEITEM hSelectedItem = GetTreeCtrl().GetSelectedItem();
	POSITION pos = (POSITION)GetTreeCtrl().GetItemData(hSelectedItem);
	if (NULL != pos)
	{
		WindowInfo wndInfo = m_processList.GetAt(pos);
		HANDLE process = OpenProcess(PROCESS_TERMINATE, 0, wndInfo.m_pid);
		if (process != NULL)
		{
			BOOL bKill = FALSE;
			if (IDYES == ::MessageBox(m_hWnd, "Killing the process may affect the system stability.\r\nDo you want to continue?", "InfoView", MB_YESNO | MB_ICONQUESTION))
			{
				TerminateProcess(process, (unsigned)-1);
				bKill = TRUE;
			}
			if ((bKill != FALSE) && (IDOK == ::MessageBox(m_hWnd, "One or more processes have been closed or created. Do you want to refresh the data?", "Refresh", MB_OKCANCEL | MB_ICONQUESTION)))
			{
				UpdateTree();
			}
		}
	}
	else
	{
		::MessageBox(m_hWnd, "Select the \"Executable\" name (the file name that ends with .exe) to kill the process", "Kill", MB_OKCANCEL | MB_ICONINFORMATION);
		return;
	}
}

BOOL CProcessThreadsView::IsWindows95()
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

BOOL CProcessThreadsView::IsWindowsNT()
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

BOOL CProcessThreadsView::ShowProperties(const HTREEITEM& hItem)
{
	CString strBuffer = _T("");
	POSITION pos = (POSITION)GetTreeCtrl().GetItemData(hItem);
	WindowInfo wndInfo = m_processList.GetAt(pos);

	TV_INSERTSTRUCT insertStruct;
	insertStruct.hParent				= hItem;
	insertStruct.hInsertAfter			= TVI_LAST;
	insertStruct.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	insertStruct.item.cChildren			= 1;
	insertStruct.item.state				= TVIS_BOLD;
	insertStruct.item.iImage			= 1;
	insertStruct.item.iSelectedImage	= 1;
	insertStruct.item.lParam =(LPARAM)NULL;

	strBuffer.Format("0x%08x", wndInfo.m_pid);
	strBuffer.MakeUpper();
	strBuffer = _T("Process Id: 0x") + strBuffer;
	insertStruct.item.pszText = (LPTSTR)(LPCTSTR)strBuffer;
	GetTreeCtrl().InsertItem(&insertStruct);
	strBuffer.Empty();

	GetThreadsList(wndInfo.m_pid, wndInfo.m_hwnd, GetSafeHwnd());

	//if (IsWindows95())
	//{
	//	strBuffer.Format("Threads: %d", wndInfo.m_noOfThreads);
	//	insertStruct.item.pszText = (LPTSTR)(LPCTSTR)strBuffer;
	//	GetTreeCtrl().InsertItem(&insertStruct);

	//	strBuffer.Empty();
	//	strBuffer.Format("0x%08x", wndInfo.m_parendPID);
	//	strBuffer.MakeUpper();
	//	strBuffer = _T("Parent Process ID: 0x") + strBuffer;
	//	insertStruct.item.pszText = (LPTSTR)(LPCTSTR)strBuffer;
	//	GetTreeCtrl().InsertItem(&insertStruct);

	//	strBuffer.Empty();
	//	strBuffer.Format("Thread's Priority: %d", wndInfo.m_threadPriority);
	//	insertStruct.item.pszText = (LPTSTR)(LPCTSTR)strBuffer;
	//	GetTreeCtrl().InsertItem(&insertStruct);

	//	strBuffer.Empty();
	//	strBuffer.Format("Usage Count: %d", wndInfo.m_usageCount);
	//	insertStruct.item.pszText = (LPTSTR)(LPCTSTR)strBuffer;
	//	GetTreeCtrl().InsertItem(&insertStruct);
	//}
	return TRUE;
}

void CProcessThreadsView::foo(DWORD dwPID)
{
	HANDLE hModuleSnap = NULL; 
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	if (hModuleSnap == INVALID_HANDLE_VALUE)
		return;
	
	THREADENTRY32 me32 = {0};

    Thread32First(hModuleSnap, &me32);
	me32.dwSize = sizeof(THREADENTRY32);
	AddToList(me32);
	while (TRUE == Thread32Next(hModuleSnap, &me32))
	{
		AddToList(me32);
	}

	CloseHandle(hModuleSnap);
}

void CProcessThreadsView::AddToList(const THREADENTRY32& pe32S)
{
	m_List[staticint].dwSize = pe32S.dwSize;
	m_List[staticint].dwSize = pe32S.dwSize; 
	m_List[staticint].cntUsage = pe32S.cntUsage; 
	m_List[staticint].th32ThreadID = pe32S.th32ThreadID; 
	m_List[staticint].th32OwnerProcessID = pe32S.th32OwnerProcessID;
	m_List[staticint].tpBasePri = pe32S.tpBasePri;
	m_List[staticint].tpDeltaPri = pe32S.tpDeltaPri;
	m_List[staticint].dwFlags = pe32S.dwFlags;
	staticint++;
}

BOOL CProcessThreadsView::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	CWaitCursor waitcursor;
	THREADENTRY32 *pMe = (THREADENTRY32 *)pCopyDataStruct->lpData;
	if (NULL != pMe)
	{
		char strPid[256];
		sprintf(strPid, "ThreadID: %d", pMe->th32ThreadID);
		TV_INSERTSTRUCT insertStruct;
		insertStruct.hParent				= GetTreeCtrl().GetSelectedItem();
		insertStruct.hInsertAfter			= TVI_LAST;
		insertStruct.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		insertStruct.item.cChildren			= 1;
		insertStruct.item.state				= TVIS_BOLD | TVIS_EXPANDEDONCE;
		insertStruct.item.iImage			= 1;
		insertStruct.item.iSelectedImage	= 1;
		insertStruct.item.pszText = strPid;
		insertStruct.item.lParam =(LPARAM)0;
		GetTreeCtrl().InsertItem(&insertStruct);
	}
	return CTreeView::OnCopyData(pWnd, pCopyDataStruct);
}