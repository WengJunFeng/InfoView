// ThreadsListView.cpp : implementation file
//

#include "stdafx.h"
#include "InfoView.h"
#include "ThreadsListView.h"
#include "ThreadPropView.h"
#include "InfoUtil.h"

// CThreadsListView

IMPLEMENT_DYNCREATE(CThreadsListView, CListView)

CThreadsListView::CThreadsListView()
{

}

CThreadsListView::~CThreadsListView()
{

}

BEGIN_MESSAGE_MAP(CThreadsListView, CListView)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnSelectionChanged)
END_MESSAGE_MAP()


// CThreadsListView diagnostics

#ifdef _DEBUG
void CThreadsListView::AssertValid() const
{
	CListView::AssertValid();
}

void CThreadsListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

BOOL CThreadsListView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	// the CREATESTRUCT cs
	cs.style |= LVS_SHOWSELALWAYS | LVS_REPORT | LVS_SINGLESEL;

	return CListView::PreCreateWindow(cs);
}

void CThreadsListView::OnInitialUpdate()
{
	m_ImageList.Create(16, 16, ILC_MASK, 0, 8);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_OPEN_FOLDER);
	m_ImageList.Add(m_hIcon);
	GetListCtrl().SetImageList(&m_ImageList, LVSIL_SMALL);

	// Add Columns
	LV_COLUMN column; 

	CString strColumnCaption("Running Threads");
	column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	column.fmt = LVCFMT_LEFT;
	column.cchTextMax= strColumnCaption.GetLength();
	column.iSubItem= 0;
	column.cx = 100;
	column.pszText = strColumnCaption.GetBuffer(strColumnCaption.GetLength());
    GetListCtrl().InsertColumn(0,&column);
	strColumnCaption.ReleaseBuffer();

	//// 1st column
	//strColumnCaption = _T("Owner PID");
	//column.pszText= strColumnCaption.GetBuffer(strColumnCaption.GetLength());
	//column.cx = 100;
	//GetListCtrl().InsertColumn(1,&column);
	//strColumnCaption.ReleaseBuffer();

	//// Base Priority
	//strColumnCaption = _T("Base Priority");
	//column.pszText= strColumnCaption.GetBuffer(strColumnCaption.GetLength());
	//column.cx = 100;
	//GetListCtrl().InsertColumn(2,&column);
	//strColumnCaption.ReleaseBuffer();
	//
	//// Delta Priority
	//strColumnCaption = _T("Delta Priority");
	//column.pszText= strColumnCaption.GetBuffer(strColumnCaption.GetLength());
	//column.cx = 100;
	//GetListCtrl().InsertColumn(3,&column);
	//strColumnCaption.ReleaseBuffer();
	//// End of Add Columns
}

// CThreadsListView message handlers
void CThreadsListView::UpdateView(DWORD_PTR dwData)
{
	CWaitCursor wait;

	// Delete all items and refresh the list
	GetListCtrl().DeleteAllItems();

	CExplorerWnd *pExplorerWndFrame = (CExplorerWnd *) GetParentFrame();
	ASSERT(pExplorerWndFrame);

	CWnd* pPropWnd = pExplorerWndFrame->m_wndSplitter.GetPane(1, 1);
	CThreadPropView *pPropView = DYNAMIC_DOWNCAST(CThreadPropView, pPropWnd);

	if ( dwData != -1 )
	{
		RefreshThreadList(dwData);
		UnHideAllControls(pPropView);
	}
	else
	{
		HideAllControls(pPropView);
	}
}

BOOL CThreadsListView::RefreshThreadList (DWORD dwOwnerPID) 
{ 
    HANDLE        hThreadSnap = NULL; 
    BOOL          bRet        = FALSE; 
    THREADENTRY32 te32        = {0}; 
 
    // Take a snapshot of all threads currently in the system. 

    hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0); 
    if (hThreadSnap == INVALID_HANDLE_VALUE) 
        return (FALSE); 
 
    // Fill in the size of the structure before using it
    te32.dwSize = sizeof(THREADENTRY32); 
 
    // Walk the thread snapshot to find all threads of the process. 
    // If the thread belongs to the process, add its information 
    // to the display list. 
    if (Thread32First(hThreadSnap, &te32)) 
    { 
        do 
        { 
            if (te32.th32OwnerProcessID == dwOwnerPID) 
            {
				AddItem(te32.th32ThreadID, te32.th32OwnerProcessID, te32.tpDeltaPri, te32.tpBasePri);
            } 
        } 
        while (Thread32Next(hThreadSnap, &te32)); 
        bRet = TRUE; 
    } 
    else 
        bRet = FALSE;          // could not walk the list of threads 
 
    // Do not forget to clean up the snapshot object
    CloseHandle (hThreadSnap);
 
    return (bRet);
}

void CThreadsListView::AddItem(DWORD threadID, DWORD procID, LONG basePriority, LONG deltaPriority)
{
	CString buf;
	LV_ITEM item;

	buf.Format("0x%08x", threadID);
	item.mask			= LVIF_TEXT | LVIF_STATE;
	item.iItem			= GetListCtrl().GetItemCount();
	item.iSubItem		= 0;
	item.state			= 0;
	item.stateMask		= 0;
	item.pszText		= (LPSTR)(LPCTSTR) buf;
	item.cchTextMax		= 10;
	item.iImage			= 0;
	item.lParam			= (LPARAM) threadID;
	int nIndex = GetListCtrl().InsertItem(&item);
	GetListCtrl().SetItemData(nIndex, threadID);

	//buf.Empty();
	//buf.Format("0x%08x", procID);
	//item.mask			= LVIF_TEXT;
	//item.iItem			= (GetListCtrl().GetItemCount() - 1);
	//item.iSubItem		= 1;
	//item.pszText		= (LPSTR)(LPCTSTR) buf;
	//item.cchTextMax		= 10;
	//item.lParam			= (LPARAM) 0;
	//GetListCtrl().SetItem(&item);

	//buf.Empty();
	//buf.Format("%ld", basePriority);
	//item.mask			= LVIF_TEXT;
	//item.iItem			= (GetListCtrl().GetItemCount() - 1);
	//item.iSubItem		= 2;
	//item.pszText		= (LPSTR)(LPCTSTR) buf;
	//item.cchTextMax		= 10;
	//item.lParam			= (LPARAM) 0;
	//GetListCtrl().SetItem(&item);

	//buf.Empty();
	//buf.Format("%ld", deltaPriority);
	//item.mask			= LVIF_TEXT;
	//item.iItem			= (GetListCtrl().GetItemCount() - 1);
	//item.iSubItem		= 3;
	//item.pszText		= (LPSTR)(LPCTSTR) buf;
	//item.cchTextMax		= 10;
	//item.lParam			= (LPARAM) 0;
	//GetListCtrl().SetItem(&item);
}

void CThreadsListView::OnSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DWORD dwData = 0;

	int selectedItem = GetListCtrl().GetNextItem(-1, LVNI_SELECTED );
	if ( selectedItem == -1 )
		return;

	dwData = GetListCtrl().GetItemData(selectedItem);
	CExplorerWnd *pExplorerWndFrame = (CExplorerWnd *) GetParentFrame();
	ASSERT(pExplorerWndFrame);

	CWnd* pPropWnd = pExplorerWndFrame->m_wndSplitter.GetPane(1, 1);
	CThreadPropView *pPropView = DYNAMIC_DOWNCAST(CThreadPropView, pPropWnd);

	UnHideAllControls(pPropView);

	pPropView->RefreshDialog(dwData);
}