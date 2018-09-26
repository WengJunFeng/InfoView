// ProcessesView.cpp : implementation file
//

#include "stdafx.h"
#include "infoview.h"
#include "ProcessesView.h"
#include "FindTxtDialog.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "./InfoHook/HHook.h"
#include "InfoUtil.h"
#include ".\processesview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProcessesView

IMPLEMENT_DYNCREATE(CProcessesView, CTreeView)

CString CProcessesView::m_strDummyItemText = _T("You should not see this node...");

CProcessesView::CProcessesView()
{
	m_SelTreeItem	= NULL;
    m_psapiHandle	= LoadLibrary("PSAPI.DLL");

	if ( NULL == m_psapiHandle )
        return;

	m_pFnGetModuleFileName	= (GETMODULEFILENAME)GetProcAddress((HINSTANCE)m_psapiHandle,	"GetModuleFileNameExA");
}

CProcessesView::~CProcessesView()
{
	// Unload the library that was loaded on demand
	if (m_psapiHandle)
	{
		::FreeLibrary((HINSTANCE)m_psapiHandle);
		m_psapiHandle = NULL;
	}
}


BEGIN_MESSAGE_MAP(CProcessesView, CTreeView)
	//{{AFX_MSG_MAP(CProcessesView)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_WM_COPYDATA()
	ON_WM_DESTROY()
	ON_COMMAND(IDM_FIND_TEXT, OnFindText)
	ON_COMMAND(IDM_REFRESH, OnRefresh)
	ON_MESSAGE(WM_MARK_ALL, OnMarkAll)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_FIND_NEXT, OnFindNext)
	ON_COMMAND(IDM_SHOW_PROCESS_ID, OnShowProcessId)
	ON_UPDATE_COMMAND_UI(IDM_SHOW_PROCESS_ID, OnUpdateShowProcessId)
	ON_COMMAND(IDM_EXPAND_ALL, OnExpandAll)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcessesView drawing

void CProcessesView::OnDraw(CDC* pDC)
{
}

/////////////////////////////////////////////////////////////////////////////
// CProcessesView diagnostics

#ifdef _DEBUG
void CProcessesView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CProcessesView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CProcessesView message handlers

BOOL CProcessesView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |=  TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;
	return CTreeView::PreCreateWindow(cs);
}

void CProcessesView::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
	CWaitCursor waitcursor;
	// TODO: You may populate your TreeView with items by directly accessing
	// its tree control through a call to GetTreeCtrl().

	// create Imagelist and attach to Tree control.
	m_ImageList.Create(16, 16, ILC_MASK, 0, 8);
	m_hIcon[0] = AfxGetApp()->LoadIcon(IDI_CLOSE_FOLDER);
	m_hIcon[1] = AfxGetApp()->LoadIcon(IDI_DLLICON);
	for( int i=0; i< 2; i++)
		m_ImageList.Add(m_hIcon[i]);
	GetTreeCtrl().SetImageList(&m_ImageList, TVSIL_NORMAL);

	UpdateTree();

	CChildFrame *pChildFrame = (CChildFrame *) GetParent();
	pChildFrame->SetWindowText("Processes");
}

BOOL CALLBACK CProcessesView::enumWnds(HWND hWnd, LPARAM lParam)
{
	CWaitCursor waitcursor;
	DWORD pid;
	WORD wUnused;
	CProcessesView* pThisView = (CProcessesView *)lParam;
	ASSERT(NULL != pThisView);

	CMapPtrToWord* pidMap = &(pThisView->m_pidMap);
	GetWindowThreadProcessId(hWnd, &pid);
	if (!pidMap->Lookup((void *)pid, wUnused))
	{
		pidMap->SetAt((void*)pid, 0);
		pThisView->AddToProcessList(pid, hWnd);
	}
	return TRUE;
}

void CProcessesView::AddToProcessList(DWORD pid, HWND hwnd)
{
	CWaitCursor waitcursor;
	WindowInfo wndInfo;
	wndInfo.m_pid	= pid;
	wndInfo.m_hwnd	= hwnd;
	m_processList.AddTail(wndInfo);
}

BOOL CProcessesView::UpdateTree(void)
{
	CWaitCursor waitcursor;
	GetTreeCtrl().DeleteAllItems();
	m_processList.RemoveAll();
	m_pidMap.RemoveAll();

	EnumWindows(CProcessesView::enumWnds, (LPARAM)this);

	POSITION pos = NULL, prevPos = NULL;
	pos = m_processList.GetHeadPosition();
	while (pos != NULL)
	{
		CString str, wndTitle;
		prevPos = pos;
		WindowInfo wndInfo = m_processList.GetNext(pos);

		HWND hwndParent = ::GetParent(wndInfo.m_hwnd);
		HWND tmpParentWindow = wndInfo.m_hwnd;
		for (;;)
		{
			if ((NULL != hwndParent) && ::IsWindow(hwndParent))
			{
				tmpParentWindow = hwndParent;
				hwndParent = ::GetParent(hwndParent);
			}
			else
			{
				break;
			}
		}
		CWnd *pParent = NULL;
		if (NULL != hwndParent)
		{
			 pParent = CWnd::FromHandle(hwndParent);
		}
		else
		{
			pParent = CWnd::FromHandle(tmpParentWindow);
		}
		ASSERT(NULL != pParent);
		pParent->GetWindowText(wndTitle);
		if ( m_bShowProcessID )
		{
			str.Format("PID: 0x%08x - ", wndInfo.m_pid);
			str.MakeUpper();
		}
		if (wndTitle.GetLength() > 0)
		{
			str += wndTitle;
		}
		else
		{
			str += getExecutableName(wndInfo.m_pid);
		}
		
		TV_INSERTSTRUCT insertStruct;
		insertStruct.hParent				= NULL;
		insertStruct.hInsertAfter			= TVI_LAST;
		insertStruct.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		insertStruct.item.cChildren			= 1;
		insertStruct.item.state				= TVIS_BOLD;
		insertStruct.item.iImage			= 0;
		insertStruct.item.iSelectedImage	= 0;
		insertStruct.item.pszText = (LPTSTR)(LPCTSTR)str;
		insertStruct.item.lParam =(LPARAM)prevPos;

		HTREEITEM hCurrentItem;
		hCurrentItem = GetTreeCtrl().InsertItem(&insertStruct);

		// insert dummy item so that user can see "+" sign.
		GetTreeCtrl().InsertItem(m_strDummyItemText, 1, 1, hCurrentItem);
	}
	return TRUE;
}

CString CProcessesView::getExecutableName(DWORD dwProcessID)
{
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessID);
	if (hProcess)
	{
		char szModName[MAX_PATH];

		if ( NULL != m_pFnGetModuleFileName )
		{
			// get the Executable path
			if (m_pFnGetModuleFileName((HMODULE)hProcess, NULL, szModName, sizeof(szModName)))
			{
				return szModName;
			}
		}
	}
	return "";
}

void CProcessesView::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CWaitCursor waitcursor;
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;

	if( pNMTreeView->action == TVE_EXPAND)
	{
		POSITION pos = (POSITION)GetTreeCtrl().GetItemData(hItem);
		m_SelTreeItem = hItem;
		WindowInfo wndInfo = m_processList.GetAt(pos);
		if (::IsWindow(wndInfo.m_hwnd))
		{
			if ( FALSE == ShowDllList(wndInfo.m_hwnd, GetSafeHwnd()))
			{
				DeleteAllChildren(hItem);
			}
		}
	}
	else
	{
		InsertDummyItem(hItem);
	}
	*pResult = 0;
}

BOOL CProcessesView::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	CWaitCursor waitcursor;
	DLL_LIST *pDLLListS = (DLL_LIST *)pCopyDataStruct->lpData;
	if (NULL != pDLLListS)
	{
		TV_INSERTSTRUCT insertStruct;
		insertStruct.hParent				= m_SelTreeItem;
		insertStruct.hInsertAfter			= TVI_LAST;
		insertStruct.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		insertStruct.item.cChildren			= 1;
		insertStruct.item.state				= TVIS_BOLD | TVIS_EXPANDEDONCE;
		insertStruct.item.iImage			= 1;
		insertStruct.item.iSelectedImage	= 1;
		insertStruct.item.pszText = pDLLListS->strDllPath;
		insertStruct.item.lParam =(LPARAM)0;
		GetTreeCtrl().InsertItem(&insertStruct);
	}
	HTREEITEM child = GetTreeCtrl().GetChildItem(m_SelTreeItem);
	CString text = GetTreeCtrl().GetItemText(child);
	if ( text == m_strDummyItemText )
	{
		GetTreeCtrl().DeleteItem(child);
	}
	return CTreeView::OnCopyData(pWnd, pCopyDataStruct);
}

void CProcessesView::InsertDummyItem(HTREEITEM hItem)
{
	// delete the node item which has received the notifcation
	DeleteAllChildren(hItem);
	// Show the node item as closed folder by inserting a dummy item
	GetTreeCtrl().InsertItem(m_strDummyItemText, 1, 1, hItem);
}

BOOL CProcessesView::DeleteAllChildren(const HTREEITEM& hParent) const
{
	CWaitCursor waitcursor;
	HTREEITEM hItem;
	// Get the first child item of the parent 
	if((hItem = GetTreeCtrl().GetChildItem(hParent)) == NULL)
	{
		return TRUE;
	}

	// if first child found
	do
	{
		// Get the first sibling item of the child
		HTREEITEM hNextItem = GetTreeCtrl().GetNextSiblingItem(hItem);

		// Delete the sibling of the child
		GetTreeCtrl().DeleteItem(hItem);

		// store next item handle
		hItem = hNextItem;
	}
	while(hItem != NULL);
	return TRUE;
}

void CProcessesView::DeleteFirstChild(const HTREEITEM& hParent) const
{
	CWaitCursor waitcursor;
	HTREEITEM hchildItem = NULL;
	// Get the first child item of the parent 
	hchildItem = GetTreeCtrl().GetChildItem(hParent);
	if(hchildItem != NULL)
	{
		// if first child found delete it
		GetTreeCtrl().DeleteItem(hchildItem);
	}
}

void CProcessesView::OnDestroy() 
{
	CTreeView::OnDestroy();
	ReleaseDLLHook();
}

void CProcessesView::OnFindText() 
{
	CFindTxtDialog* pFindTextDlg = new CFindTxtDialog(this);
	ASSERT(pFindTextDlg);
	pFindTextDlg->Create(IDD_FIND_TEXT_DIALOG, this);
	pFindTextDlg->setIncludeSubFolders(false);
	pFindTextDlg->ShowWindow(SW_SHOW);
}

LRESULT CProcessesView::OnFindNext(WPARAM wParam, LPARAM lParam)
{
	CWaitCursor waitcursor;
	CFindTxtDialog *pFindTextDlg = (CFindTxtDialog *)lParam;
	if (pFindTextDlg)
	{
		CString str			= pFindTextDlg->getFindText();
		BOOL	bWholeWord	= pFindTextDlg->getWholeWord();
		BOOL	bMatchCase	= pFindTextDlg->getMatchCase();
		BOOL	bDirection	= pFindTextDlg->getDirection();
		if (!str.IsEmpty())
		{
			HTREEITEM treeItem = FindItem(str, bMatchCase, bDirection, bWholeWord,GetTreeCtrl().GetSelectedItem());
			if ( NULL != treeItem )
			{
				GetTreeCtrl().Select(treeItem, TVGN_CARET);
				GetTreeCtrl().SelectItem(treeItem);
				GetTreeCtrl().EnsureVisible(treeItem);
			}
			else
			{
				CString tmpStr = "Cannot find the string \'" + str + "\'";
				::MessageBox(m_hWnd, tmpStr, "Find String", MB_OK | MB_ICONINFORMATION);
			}
		}
	}
	return 1;
}

// FindItem			- Finds an item that contains the search string
// Returns			- Handle to the item or NULL
// str				- String to search for
// bCaseSensitive	- Should the search be case sensitive
// bDownDir			- Search direction - TRUE for down
// bWholeWord		- True if search should match whole words
// hItem			- Item to start searching from. NULL for
//					- currently selected item
HTREEITEM CProcessesView::FindItem(CString &str, BOOL bCaseSensitive /*= FALSE*/, 				
								 BOOL bDownDir /*= TRUE*/, BOOL bWholeWord /*= FALSE*/, 				
								 HTREEITEM hItem /*= NULL*/)
{
	CWaitCursor waitcursor;
	int lenSearchStr = str.GetLength();	
	if( lenSearchStr == 0 ) 
		return NULL;
	HTREEITEM htiSel = hItem ? hItem : GetTreeCtrl().GetSelectedItem();
	HTREEITEM htiCur = bDownDir ? GetNextItem( htiSel ) : GetPrevItem( htiSel );

	CString sSearch = str;	
	if( htiCur == NULL )	
	{
		if( bDownDir )  
			htiCur = GetTreeCtrl().GetRootItem();		
		else 
			htiCur = GetLastItem( NULL );	
	}
	
	if( !bCaseSensitive )		
		sSearch.MakeLower();	
	
	while( htiCur && htiCur != htiSel )
	{		
		CString sItemText = GetTreeCtrl().GetItemText( htiCur );		
		
		if( !bCaseSensitive )
			sItemText.MakeLower();		
		
		int n;
		while( (n = sItemText.Find( sSearch )) != -1 )		
		{			
			// Search string found
			if( bWholeWord )			
			{				
				// Check preceding char				
				if( n != 0 )				
				{
					if( isalpha(sItemText[n-1]) || sItemText[n-1] == '_' )
					{
						// Not whole word						
						sItemText = sItemText.Right(sItemText.GetLength() - n - lenSearchStr);
							continue;
					}
				}

				// Check succeeding char
				if( sItemText.GetLength() > n + lenSearchStr && ( isalpha(sItemText[n+lenSearchStr]) ||
					sItemText[n+lenSearchStr] == '_' ) )				
				{					
					// Not whole word
					sItemText = sItemText.Right( sItemText.GetLength() - n - sSearch.GetLength() );					
						continue;
				}			
			}		
			if( IsFindValid( htiCur ) )
				return htiCur;
			else
				break;
		}
		htiCur = bDownDir ? GetNextItem( htiCur ) : GetPrevItem( htiCur );

		if( htiCur == NULL )
		{
			if( bDownDir )
				htiCur = GetTreeCtrl().GetRootItem();
			else
				htiCur = GetLastItem( NULL );
		}
	}	
	return NULL;
}

void CProcessesView::OnRefresh() 
{
	CWaitCursor waitcursor;
	UpdateTree();
}

// IsFindValid	- Virtual function used by FindItem to allow this
//				  function to filter the result of FindItem
// Returns		- True if item matches the criteria
// Arg			- Handle of the item

BOOL CProcessesView::IsFindValid( HTREEITEM )
{	
	return TRUE;
}

// GetLastItem  - Gets last item in the branch
// Returns      - Last item
// hItem        - Node identifying the branch. NULL will 
//                return the last item in outine
HTREEITEM CProcessesView::GetLastItem( HTREEITEM hItem )
{
	CWaitCursor waitcursor;
	// Last child of the last child of the last child ...
	HTREEITEM htiNext;
	
	if( hItem == NULL )
	{
		// Get the last item at the top level
		htiNext = GetTreeCtrl().GetRootItem();
		while( htiNext )
		{
			hItem = htiNext;
			htiNext = GetTreeCtrl().GetNextSiblingItem( htiNext );
		}
	}
	
	while( GetTreeCtrl().ItemHasChildren( hItem ) )
	{
		htiNext = GetTreeCtrl().GetChildItem( hItem );
		while( htiNext )
		{
			hItem = htiNext;
			htiNext = GetTreeCtrl().GetNextSiblingItem( htiNext );
		}
	}
	return hItem;
}

// GetNextItem		- Get previous item as if outline was completely expanded
// Returns          - The item immediately above the reference item
// hItem            - The reference item
HTREEITEM CProcessesView::GetPrevItem( HTREEITEM hItem )
{
	CWaitCursor waitcursor;
	HTREEITEM       hti;
	
	hti = GetTreeCtrl().GetPrevSiblingItem(hItem);
	if( hti == NULL )
		hti = GetTreeCtrl().GetParentItem(hItem);
	else
		hti = GetLastItem(hti);
	return hti;
}

// GetNextItem  - Get next item as if outline was completely expanded
// Returns      - The item immediately below the reference item
// hItem        - The reference item
HTREEITEM CProcessesView::GetNextItem( HTREEITEM hItem )
{
	CWaitCursor waitcursor;
	HTREEITEM  hti;
	
	if( GetTreeCtrl().ItemHasChildren( hItem ) )
		return GetTreeCtrl().GetChildItem( hItem );           // return first child
	else
	{
		// return next sibling item
		// Go up the tree to find a parent's sibling if needed.
		while( (hti = GetTreeCtrl().GetNextSiblingItem( hItem )) == NULL )
		{
			if( (hItem = GetTreeCtrl().GetParentItem( hItem ) ) == NULL )
				return NULL;
		}
	}
	return hti;
}

//
// Tell the freaking user that he can't do this!
//
LRESULT CProcessesView::OnMarkAll(WPARAM wParam, LPARAM lParam)
{
	CFindTxtDialog *pFindTextDlg = (CFindTxtDialog *)lParam;
	if (pFindTextDlg)
	{
		CString str			= pFindTextDlg->getFindText();
		BOOL	bWholeWord	= pFindTextDlg->getWholeWord();
		BOOL	bMatchCase	= pFindTextDlg->getMatchCase();
		BOOL	bDirection	= pFindTextDlg->getDirection();
		// BOOL	bIncludeSubFolders = pFindTextDlg->getIncludeSubFolders();

		// Don't expand the tree!
		BOOL	bIncludeSubFolders = FALSE;
		if (!str.IsEmpty())
		{
			if (TRUE == bIncludeSubFolders && (m_bExpandedAll == FALSE))
			{
				// collapse the entire tree and Expand entire tree, and search
				GetTreeCtrl().Expand(GetTreeCtrl().GetRootItem(), TVE_COLLAPSE);
				ExpandAll();
			}
			int nItemFound = 0;
			HTREEITEM hNewItem = NULL;
			while(true)
			{
				hNewItem = FindItem(str, bMatchCase, bDirection, bWholeWord, GetTreeCtrl().GetSelectedItem());
				if ( NULL != hNewItem )
				{
					if (TVIS_BOLD == GetTreeCtrl().GetItemState(hNewItem, TVIS_BOLD))
						return 0;

					GetTreeCtrl().SetItemState(hNewItem, TVIS_BOLD, TVIS_BOLD);
					GetTreeCtrl().Select(hNewItem, TVGN_CARET);
					GetTreeCtrl().SelectItem(hNewItem);
					nItemFound++;
				}
				else
				{
					if (nItemFound == 0)
					{
						CString tmpStr = "The specified text \'" + str + "\' was not found.";
						::MessageBox(m_hWnd, tmpStr, "Find String", MB_OK | MB_ICONINFORMATION);
					}
					return 1;
				}
			}
		}
	}
	return 1;
}

void CProcessesView::ExpandAll()
{
    HTREEITEM hti = GetTreeCtrl().GetRootItem();
    do{
            ExpandBranch( hti );
    } while( (hti = GetTreeCtrl().GetNextSiblingItem( hti )) != NULL );
	m_bExpandedAll = TRUE;
}

void CProcessesView::ExpandBranch( HTREEITEM hti)
{
	if( GetTreeCtrl().ItemHasChildren( hti ) )
	{
		GetTreeCtrl().Expand( hti, TVE_EXPAND);
		hti = GetTreeCtrl().GetChildItem( hti );
		do
		{
			ExpandBranch( hti );
		} while( (hti = GetTreeCtrl().GetNextSiblingItem( hti )) != NULL );        
	}
}

void CProcessesView::ChangeBackColor( HTREEITEM hItem )
{
	CRect rect(0,0,0,0);
	GetTreeCtrl().GetItemRect(hItem, &rect, true);

	// Get the DC of the TreeView
	CDC* pDC = GetTreeCtrl().GetDC();

	// create a new SOLID brush with red color
	CBrush *pBrush = new CBrush();
	pBrush->CreateSolidBrush(RGB(255,0,0));

	// Fill the item background color with RED
	pDC->FillRect(&rect, pBrush);

	// Release memory
	delete pBrush;
}
void CProcessesView::OnShowProcessId()
{
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();
	ASSERT(NULL != pMenu);
	if (MF_UNCHECKED == pMenu->GetMenuState(IDM_SHOW_PROCESS_ID, MF_BYCOMMAND))
	{
		pMenu->CheckMenuItem(IDM_SHOW_PROCESS_ID, MF_BYCOMMAND | MF_CHECKED);
		m_bShowProcessID = TRUE;
		UpdateTree();
	}
	else
	{
		pMenu->CheckMenuItem(IDM_SHOW_PROCESS_ID, MF_BYCOMMAND | MF_UNCHECKED);
		m_bShowProcessID = FALSE;
		UpdateTree();
	}
}

void CProcessesView::OnUpdateShowProcessId(CCmdUI *pCmdUI)
{
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();
	ASSERT(NULL != pMenu);
	if (FALSE == m_bShowProcessID)
	{
		pMenu->CheckMenuItem(IDM_SHOW_PROCESS_ID, MF_BYCOMMAND | MF_UNCHECKED);
	}
	else
	{
		pMenu->CheckMenuItem(IDM_SHOW_PROCESS_ID, MF_BYCOMMAND | MF_CHECKED);
	}
}

void CProcessesView::OnExpandAll()
{
	HTREEITEM item = GetTreeCtrl().GetFirstVisibleItem();
	while(NULL != item)
	{
		GetTreeCtrl().Expand(item, TVE_EXPAND);
		item = GetTreeCtrl().GetNextItem(item, TVGN_NEXT);
	}
}