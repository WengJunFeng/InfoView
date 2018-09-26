// ProcessTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "InfoView.h"
#include "ProcessTreeView.h"
#include "ProcessPropView.h"
#include "InfoUtil.h"
#include <psapi.h>
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include ".\processtreeview.h"

// CProcessTreeView

IMPLEMENT_DYNCREATE(CProcessTreeView, CTreeView)

CProcessTreeView::CProcessTreeView()
{
	m_root = NULL;
}

CProcessTreeView::~CProcessTreeView()
{
	m_root = NULL;
}

BEGIN_MESSAGE_MAP(CProcessTreeView, CTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_CLOSE()
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_COMMAND(IDM_SHOWPROPERTIES, OnProperties)
	ON_COMMAND(IDM_REFRESH, OnRefresh)
END_MESSAGE_MAP()

BOOL CProcessTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	cs.style |=  TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;
	return CTreeView::PreCreateWindow(cs);
}

// CProcessTreeView diagnostics

#ifdef _DEBUG
void CProcessTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CProcessTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

// CProcessTreeView message handlers
void CProcessTreeView::OnInitialUpdate()
{
	CWaitCursor waitcursor;
	CTreeView::OnInitialUpdate();

	// TODO: You may populate your TreeView with items by directly accessing
	//  its tree control through a call to GetTreeCtrl().

	// create Imagelist and attach to Tree control.
	m_ImageList.Create(16, 16, ILC_MASK, 0, 8);
	
	m_hIcon[0] = AfxGetApp()->LoadIcon(IDI_CLOSE_FOLDER);
	m_hIcon[1] = AfxGetApp()->LoadIcon(IDI_OPEN_FOLDER);
	m_ImageList.Add(m_hIcon[0]);
	m_ImageList.Add(m_hIcon[1]);

	GetTreeCtrl().SetImageList(&m_ImageList, TVSIL_NORMAL);

	InitView();
}

BOOL CProcessTreeView::InitView(void)
{
	// remove all items first
	GetTreeCtrl().DeleteAllItems();

	// Add root node as "My Computer"
	m_root = AddItem("My Computer", NULL, -1);

	// Add processes after the root
	AddProcesses();

	CExplorerWnd *pExplorerWndFrame = (CExplorerWnd *) GetParentFrame();
	ASSERT(pExplorerWndFrame);

	pExplorerWndFrame->SetWindowText("Processes & Threads");

	GetTreeCtrl().Expand(m_root, TVE_EXPAND);

	// Select root
	GetTreeCtrl().SelectItem(GetTreeCtrl().GetRootItem());	
	
	return TRUE;
}

// CProcessTreeView message handlers
BOOL CProcessTreeView::AddProcesses()
{
	HANDLE         hProcessSnap = NULL;
    BOOL           bRet			= FALSE;
    PROCESSENTRY32 pe32			= {0};

	//  Take a snapshot of all processes in the system. 
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 

    if (hProcessSnap == INVALID_HANDLE_VALUE) 
        return (FALSE); 
 
    //  Fill in the size of the structure before using it. 
    pe32.dwSize = sizeof(PROCESSENTRY32); 

	//  Walk the snapshot of the processes
	if (Process32First(hProcessSnap, &pe32)) 
    {
         do
        {
			// Add node
			AddItem(pe32.szExeFile, m_root, (LPARAM)pe32.th32ProcessID);
        } 
        while (Process32Next(hProcessSnap, &pe32));
        bRet = TRUE;
    } 
    else
	{
		// Could not walk the list of processes!
        bRet = FALSE;
	}
 
    // Do not forget to clean up the snapshot object. 
    CloseHandle (hProcessSnap);
    return (bRet); 
}

HTREEITEM CProcessTreeView::AddItem(CString strItemText, HTREEITEM hParent, LPARAM itemData)
{
	int icoIndex = 0;
	TCHAR szPath[512];
	ZeroMemory(szPath, sizeof(szPath));

	if ( (DWORD) itemData != -1 )
	{
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, (DWORD) itemData);
		if ( hProcess )
		{
			HMODULE hMod;
			DWORD cbNeeded;
			if ( EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
			{
				GetModuleFileNameEx(hProcess, hMod, szPath, sizeof(szPath));
				HICON icon = ExtractIcon(AfxGetApp()->m_hInstance, szPath, 0);
				if ( NULL != icon )
				{
					icoIndex = m_ImageList.Add(icon);
				}
				else
				{
					icoIndex = 1;
				}
			}
		}
		CloseHandle(hProcess);
	}

	TV_INSERTSTRUCT insertStruct;
	insertStruct.hParent			= hParent;
	insertStruct.hInsertAfter		= TVI_LAST;

	insertStruct.item.mask			= TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	insertStruct.item.cChildren		= 1;
	insertStruct.item.state			= TVIS_BOLD;
	insertStruct.item.pszText		= (LPSTR) (LPCTSTR) strItemText;
	//insertStruct.item.pszText		= szPath;
	insertStruct.item.lParam		= (LPARAM) itemData;

	if ( hParent == NULL )
	{
		insertStruct.item.lParam	= (LPARAM) -1;
	}

	if (_tcscmp(strItemText, "My Computer") == 0)
	{
		insertStruct.item.iImage			= icoIndex; //0;
		insertStruct.item.iSelectedImage	= icoIndex; // 0;
	}
	else
	{
		insertStruct.item.iImage			= icoIndex; // 1;
		insertStruct.item.iSelectedImage	= icoIndex; // 1;
	}
	return GetTreeCtrl().InsertItem(&insertStruct);
}

void CProcessTreeView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	DWORD_PTR dwData = 0;
	dwData = GetTreeCtrl().GetItemData(hItem);

	CExplorerWnd *pExplorerWndFrame = (CExplorerWnd *) GetParentFrame();
	ASSERT(pExplorerWndFrame);

	CWnd* pWnd = pExplorerWndFrame->m_wndSplitter.GetPane(1, 0);
	CThreadsListView* pView = DYNAMIC_DOWNCAST(CThreadsListView, pWnd);

	CWnd* pPropWnd = pExplorerWndFrame->m_wndSplitter.GetPane(0, 1);
	CProcessPropView* pPropView = DYNAMIC_DOWNCAST(CProcessPropView, pPropWnd);

	if ( (_tcsicmp("My Computer", GetTreeCtrl().GetItemText(hItem)) == 0) || (dwData == NULL) || (dwData < 1 ) )
	{
		pView->UpdateView((DWORD_PTR)-1);
		pPropView->SetProcessID((DWORD)-1);
		HideAllControls(pPropView);
	}
	else
	{
		pView->UpdateView(dwData);
		pPropView->SetProcessID(dwData);
		pPropView->RefreshDialog();
		UnHideAllControls(pPropView);
	}
	*pResult = 0;
}

void CProcessTreeView::OnClose( )
{
	// cleanup!
}

void CProcessTreeView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult)
{
	CWaitCursor waitcursor;
	CMenu menu;

	if (menu.LoadMenu(IDR_PROCESSES_POPUPMENU))
	{
		CMenu *pSubMenu = menu.GetSubMenu(0);
		ASSERT(pSubMenu);

		// Get the cursor position
		CPoint position;
		::GetCursorPos(&position);

		this->SetForegroundWindow();
		pSubMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, position.x, position.y, this, NULL);
		this->PostMessage(WM_NULL, 0, 0);
	}

	*pResult = 0;
}

void CProcessTreeView::OnProperties()
{
	HTREEITEM hSelecctedItem = GetTreeCtrl().GetSelectedItem();
		
	if ( hSelecctedItem )
	{
		DWORD_PTR dwData = GetTreeCtrl().GetItemData(hSelecctedItem);
		if ( dwData >= 1 )
		{
			CProcessInfoDlg dlg(dwData);
			dlg.DoModal();
		}
	}
}

void CProcessTreeView::OnRefresh()
{
	InitView();
}