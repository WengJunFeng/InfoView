////////////////////////////////////////////////////////////////
// CWindowsView
// If this code works, it was written by Suresh.
// WindowsView.cpp : implementation of the CWindowsView class
//

#include "stdafx.h"
#include "InfoView.h"

#include "InfoViewDoc.h"
#include "WindowsView.h"
#include "MainFrm.h"
#include "ChildFrm.h"

// for utility functions
#include "InfoUtil.h"

// Find Items
#include "FindWindow.h"
#include "FindTxtDialog.h"

// Dll Dialog
#include "DLLListDlg.h"

// For Hooks !
#include "./InfoHook/HHook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ARRAY_SIZE(Array)	\
	(sizeof(Array) / sizeof((Array)[0]))

HRESULT WINAPI Shell_CreateLink (LPCTSTR pszLinkFilePathname, PLINKDATA pld);

/////////////////////////////////////////////////////////////////////////////
// CWindowsView

IMPLEMENT_DYNCREATE(CWindowsView, CTreeView)

BEGIN_MESSAGE_MAP(CWindowsView, CTreeView)
	//{{AFX_MSG_MAP(CWindowsView)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_COMMAND(IDM_SHOWPROPERTIES, OnShowProperties)
	ON_COMMAND(IDM_HIGHLIGHT, OnHighlightWindow)
	ON_COMMAND(IDM_REFRESH, OnRefresh)
	ON_COMMAND(IDM_FIND_WINDOW, OnFindWindow)
	ON_COMMAND(IDM_GET_TEXT, OnGetText)
	ON_COMMAND(IDM_FIND_TEXT, OnFindText)
	ON_COMMAND(ID_UTILS_CREATESHORTCUTONDESKTOP, OnCreateShortcut)
	ON_COMMAND(ID_COPY_HWND, OnCopyHwnd)
	ON_COMMAND(ID_COPY_WINDOWTEXT, OnCopyWindowCaption)
	ON_COMMAND(ID_COPY_CLASSNAME, OnCopyClassName)
	ON_COMMAND(ID_COPY_ENTIRETEXT, OnCopyEntiretext)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_DESTROY()
	ON_COMMAND(IDM_SHOW_CLASSNAME, OnShowClassname)
	ON_COMMAND(IDM_SHOW_PROCESS_ID, OnShowProcessId)
	ON_COMMAND(IDM_SHOW_THREAD_ID, OnShowThreadId)
	ON_UPDATE_COMMAND_UI(IDM_SHOW_CLASSNAME, OnUpdateShowClassname)
	ON_UPDATE_COMMAND_UI(IDM_SHOW_PROCESS_ID, OnUpdateShowProcessId)
	ON_UPDATE_COMMAND_UI(IDM_SHOW_THREAD_ID, OnUpdateShowThreadId)
	ON_COMMAND(ID_SHOW_WINDOWCAPTION, OnShowWindowcaption)
	ON_UPDATE_COMMAND_UI(ID_SHOW_WINDOWCAPTION, OnUpdateShowWindowcaption)
	ON_COMMAND(ID_SHOW_SHOWHIDDENWINDOWS, OnShowHiddenWindows)
	ON_UPDATE_COMMAND_UI(ID_SHOW_SHOWHIDDENWINDOWS, OnUpdateOnShowHiddenWindows)
	ON_COMMAND(ID_HIDE_WINDOW, OnHideWindow)
	ON_UPDATE_COMMAND_UI(ID_HIDE_WINDOW, OnUpdateHideWindow)
	ON_COMMAND(ID_ENABLE_WINDOW, OnEnableWindow)
	ON_UPDATE_COMMAND_UI(ID_ENABLE_WINDOW, OnUpdateEnableWindow)
	ON_COMMAND(ID_CLOSE_WINDOW, OnCloseWindow)
	ON_UPDATE_COMMAND_UI(ID_CLOSE_WINDOW, OnUpdateCloseWindow)

	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_MESSAGE(WM_FIND_NEXT, OnFindNext)
	ON_MESSAGE(WM_FIND_HWND_TEXT, OnFindHwndText)
	ON_MESSAGE(WM_MARK_ALL, OnMarkAll)
	ON_COMMAND(ID_FILE_PRINT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CTreeView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindowsView construction/destruction

CWindowsView::CWindowsView()
{
	// TODO: add construction code here
	m_pSelWndInfo = new WindowInfo;
	m_selectedWindow = NULL;
	m_bExpandedAll = FALSE;
	m_bUpdate		= FALSE;

	m_bShowProcessID	=	FALSE;
	m_bShowThreadID		=	FALSE;
	m_bShowWindowCaption	= TRUE;
	m_bShowClassName		=	TRUE;
	m_bShowHiddenWindows	=	FALSE;

	m_pGeneralPage		  = NULL;
	m_pStylesPage		  = NULL;
	m_pClassPage		  = NULL;
	m_pWindowsPage		  = NULL;
	m_p95ProcessPage	  = NULL;
	m_pNTProcessPage	  = NULL;
	m_pProcessMemInfoPage = NULL;

	m_bPropertySheetCreated = FALSE;
}

CWindowsView::~CWindowsView()
{
	if (m_pSelWndInfo)
	{
		delete m_pSelWndInfo;
		m_pSelWndInfo = NULL;
	}
	if (m_pTabSheet)
	{
		m_pTabSheet->DestroyWindow();
		delete m_pTabSheet;
		m_pTabSheet = NULL;
	}
	
	delete m_pGeneralPage;
	delete m_pStylesPage;
	delete m_pClassPage;
	delete m_pWindowsPage;
	delete m_p95ProcessPage;
	delete m_pNTProcessPage;
	delete m_pProcessMemInfoPage;
	
	m_pGeneralPage		  = NULL;
	m_pStylesPage		  = NULL;
	m_pClassPage		  = NULL;
	m_pWindowsPage		  = NULL;
	m_p95ProcessPage	  = NULL;
	m_pNTProcessPage	  = NULL;
	m_pProcessMemInfoPage = NULL;
}

BOOL CWindowsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	// the CREATESTRUCT cs
	cs.style |=  TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;
	return CTreeView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CWindowsView drawing

void CWindowsView::OnDraw(CDC* pDC)
{
	CInfoViewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

/////////////////////////////////////////////////////////////////////////////
// OnInitialUpdate(): Function to create Image List and fill the Tree control
/////////////////////////////////////////////////////////////////////////////
void CWindowsView::OnInitialUpdate()
{
	CWaitCursor waitcursor;
	CTreeView::OnInitialUpdate();

	// TODO: You may populate your TreeView with items by directly accessing
	//  its tree control through a call to GetTreeCtrl().

	// create Imagelist and attach to Tree control.
	m_ImageList.Create(16, 16, ILC_MASK, 0, 8);
	m_hIcon[0] = AfxGetApp()->LoadIcon(IDI_OPEN_FOLDER);
	m_hIcon[1] = AfxGetApp()->LoadIcon(IDI_CLOSE_FOLDER);
	for( int i=0; i< 2; i++)
		m_ImageList.Add(m_hIcon[i]);
	GetTreeCtrl().SetImageList(&m_ImageList, TVSIL_NORMAL);

	m_bShowClassName = ((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "ShowClassName", 1);
	m_bShowProcessID = ((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "ShowPID", 0);
	m_bShowThreadID	 = ((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "ShowTID", 0);
	m_bShowWindowCaption = 	((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "ShowWindowCaption", 1);

	// Update the Tree control
	UpdateTree();

	// Refresh successful, if the property sheet is active, update the sheet.
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	MTree<WindowInfo>* pTree = (MTree<WindowInfo>*)GetTreeCtrl().GetItemData(hItem);
	if (pTree != NULL)
	{
		*m_pSelWndInfo = pTree->getValue();
		m_pSelWndInfo->setWindowTitle(m_pSelWndInfo->m_hwnd);
		m_pSelWndInfo->setClassName(m_pSelWndInfo->m_hwnd);
	}

	m_pTabSheet			  = new CTabSheet("Properties", this, 0,  m_pSelWndInfo);
	m_pGeneralPage		  = new CGeneralPage;
	m_pStylesPage		  = new CStylesPage;
	m_pClassPage		  = new CClassPage;
	m_pWindowsPage		  = new CWindowsPage;
	m_p95ProcessPage	  = new C95ProcessPage;
	m_pNTProcessPage	  = new CNTProcessPage;
	m_pProcessMemInfoPage = new CProcessMemInfo;

	m_pGeneralPage->m_psp.hIcon = ((CInfoViewApp*)AfxGetApp())->LoadIcon(IDI_GENERAL);
	m_pGeneralPage->m_psp.dwFlags = m_pGeneralPage->m_psp.dwFlags | PSP_USEHICON;

	m_pStylesPage->m_psp.hIcon = ((CInfoViewApp*)AfxGetApp())->LoadIcon(IDI_STYLES);
	m_pStylesPage->m_psp.dwFlags = m_pStylesPage->m_psp.dwFlags | PSP_USEHICON;

	m_pClassPage->m_psp.hIcon = ((CInfoViewApp*)AfxGetApp())->LoadIcon(IDI_CLASSES);
	m_pClassPage->m_psp.dwFlags = m_pClassPage->m_psp.dwFlags | PSP_USEHICON;

	m_pWindowsPage->m_psp.hIcon = ((CInfoViewApp*)AfxGetApp())->LoadIcon(IDI_WINDOWS);
	m_pWindowsPage->m_psp.dwFlags = m_pWindowsPage->m_psp.dwFlags | PSP_USEHICON;

	m_p95ProcessPage->m_psp.hIcon = ((CInfoViewApp*)AfxGetApp())->LoadIcon(IDI_PROCESSES);
	m_p95ProcessPage->m_psp.dwFlags = m_p95ProcessPage->m_psp.dwFlags | PSP_USEHICON;

	m_pNTProcessPage->m_psp.hIcon = ((CInfoViewApp*)AfxGetApp())->LoadIcon(IDI_PROCESSES);
	m_pNTProcessPage->m_psp.dwFlags = m_pNTProcessPage->m_psp.dwFlags | PSP_USEHICON;
	
	m_pProcessMemInfoPage->m_psp.hIcon = ((CInfoViewApp*)AfxGetApp())->LoadIcon(IDI_PROCESSES);
	m_pProcessMemInfoPage->m_psp.dwFlags = m_pProcessMemInfoPage->m_psp.dwFlags | PSP_USEHICON;

	// common property pages.
	m_pTabSheet->AddPage(m_pGeneralPage);
	m_pTabSheet->AddPage(m_pStylesPage);
	m_pTabSheet->AddPage(m_pClassPage);
	m_pTabSheet->AddPage(m_pWindowsPage);

	if (IsWindows95())
	{
		m_pTabSheet->AddPage(m_p95ProcessPage);
	}
	else if (IsWindowsNT())
	{
		m_pTabSheet->AddPage(m_pNTProcessPage);
		m_pTabSheet->AddPage(m_pProcessMemInfoPage);
	}
	m_pTabSheet->EnableStackedTabs(FALSE);

	CChildFrame *pChildFrame = (CChildFrame *) GetParent();
	pChildFrame->SetWindowText("Windows");
}

/////////////////////////////////////////////////////////////////////////////
// CWindowsView printing
BOOL CWindowsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWindowsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWindowsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CWindowsView diagnostics

#ifdef _DEBUG
void CWindowsView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CWindowsView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CInfoViewDoc* CWindowsView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CInfoViewDoc)));
	return (CInfoViewDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWindowsView message handlers

/////////////////////////////////////////////////////////////////////////////
// FillTreeItemList(): Function to fill the window's information in Tree List
// HWND& hwnd				: Window to fill in the Tree list
// MTree<WindowInfo>* pTree	: NULL in case of root.
/////////////////////////////////////////////////////////////////////////////
void CWindowsView::FillTreeItemList(const HWND& hwnd, MTree<WindowInfo>* pTree)
{
	CWaitCursor waitcursor;
	WindowInfo wndInfo;
	MTree<WindowInfo>* pSubTree = NULL;
	ASSERT(hwnd);

	wndInfo.m_hwnd = hwnd;
	wndInfo.setWindowTitle(hwnd);
	wndInfo.setClassName(hwnd);

	pSubTree = new MTree<WindowInfo>;
	ASSERT(pSubTree);
	pSubTree->setValue(wndInfo);

	if (NULL != pTree)
	{
		// add the tree item under the given tree
		pTree->addSubTree(pSubTree);
	}
	else
	{
		// add the tree item at root.( top level )
		m_Tree.addSubTree(pSubTree);
	}
}

/////////////////////////////////////////////////////////////////////////////
// UpdateTree(): Function to update the Tree control.
/////////////////////////////////////////////////////////////////////////////
void CWindowsView::UpdateTree()
{
	m_bUpdate = TRUE;
	CWaitCursor waitcursor;

	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0, "Refreshing Windows List ...");

	// Remove all tree view items and tree objects
	m_Tree.removeAll();

	GetTreeCtrl().DeleteAllItems();
	
	// start with Desktop ...
	FillTreeItemList(::GetDesktopWindow());

	// insert actual tree items in the tree control
	InsertTreeItems(NULL, &m_Tree);

	// select root item
	GetTreeCtrl().SelectItem(GetTreeCtrl().GetRootItem());

	// Expand the TreeCtrl
	GetTreeCtrl().Expand(GetTreeCtrl().GetRootItem(), TVE_EXPAND);

	// Ensure Visible
	GetTreeCtrl().EnsureVisible(GetTreeCtrl().GetRootItem());

	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0, "Ready");

	m_bExpandedAll = FALSE;

	m_bUpdate = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// HasChild(): returns TRUE if the given window has child window
/////////////////////////////////////////////////////////////////////////////
BOOL CWindowsView::HasChild(const HWND& hwnd)const
{
	if (::GetWindow( hwnd, GW_CHILD))
		return TRUE;
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// DeleteFirstChild(): Deletes first child under the given node in Tree control.
/////////////////////////////////////////////////////////////////////////////
void CWindowsView::DeleteFirstChild(const HTREEITEM& hParent) const
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

/////////////////////////////////////////////////////////////////////////////
// DeleteAllChildren(): Deletes all children under the given node in Tree control.
/////////////////////////////////////////////////////////////////////////////
void CWindowsView::DeleteAllChildren(const HTREEITEM& hParent) const
{
	CWaitCursor waitcursor;
	HTREEITEM hItem;
	// Get the first child item of the parent 
	if((hItem = GetTreeCtrl().GetChildItem(hParent)) == NULL)
	{
		return;
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
}

/////////////////////////////////////////////////////////////////////////////
// OnItemexpanding(): Function used to fill the child windows under the 
// selected node.
/////////////////////////////////////////////////////////////////////////////
void CWindowsView::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CWaitCursor waitcursor;
	
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hSelectedItem = GetTreeCtrl().GetSelectedItem();
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	MTree<WindowInfo>* pTree = (MTree<WindowInfo>*)GetTreeCtrl().GetItemData(hItem);
	
	// get the current selected node
	if (pTree != NULL)
	{
		m_bUpdate = TRUE;
		WindowInfo wndInfo = pTree->getValue();
		// if expanding
		if( pNMTreeView->action == TVE_EXPAND)
		{
			// delete the dummy item and insert actual tree items
			DeleteAllChildren(hItem);
			// remove the sub items if any
			pTree->removeAll();

			HWND hwnd = wndInfo.m_hwnd;
			hwnd = ::GetWindow(hwnd, GW_CHILD);
			if ((hwnd) && ::IsWindow(hwnd))
			{
				// Fill the child items
				FillTreeItemList(hwnd, pTree);
				for(;;)
				{
					hwnd = ::GetWindow(hwnd, GW_HWNDNEXT);
					if ((hwnd) && (::IsWindow(hwnd)))
					{
						// Fill the sibling items
						FillTreeItemList(hwnd, pTree);
					}			
					else
					{
						break;
					}
				}
			}
			InsertTreeItems(hItem, pTree);
		}
		else
		{
			m_bUpdate = TRUE;
			// delete the node item which has received the notifcation
			DeleteAllChildren(hItem);
			pTree->removeAll();
			if (TRUE == HasChild(wndInfo.m_hwnd))
			{
				// show the node item as closed folder by inserting a dummy item
				GetTreeCtrl().InsertItem(_T(""), 0, 1, hItem);
			}
		}
	}
	// Ensure Visible
	GetTreeCtrl().Select(hSelectedItem, TVGN_CARET);
	GetTreeCtrl().SelectItem(hSelectedItem);
	GetTreeCtrl().EnsureVisible(hSelectedItem);
	*pResult = 0;
	m_bUpdate = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// InsertTreeItems()  : Function to insert nodes in the Tree control.
// HTREEITEM& hParent : Handle to Parent node
// MTree<WindowInfo>* const pTree : List to insert
/////////////////////////////////////////////////////////////////////////////
void CWindowsView::InsertTreeItems(const HTREEITEM& hParent, MTree<WindowInfo>* const pTree)
{
	CWaitCursor waitcursor;
	if (pTree)
	{
		TV_INSERTSTRUCT insertStruct;
		insertStruct.hParent				= hParent; // parent node
		insertStruct.hInsertAfter			= TVI_LAST;
		insertStruct.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		insertStruct.item.cChildren			= 1;
		insertStruct.item.state				= TVIS_BOLD;

		int treeCount = pTree->getSubTreeCount();
		for( int i = 0; i< treeCount ; i++ )
		{
			MTree<WindowInfo> &subTree = pTree->getSubTree(i);
			WindowInfo wndInfo = subTree.getValue();

			if (HasChild(wndInfo.m_hwnd))
			{
				insertStruct.item.iImage			= 1;
				insertStruct.item.iSelectedImage	= 1;
			}
			else
			{
				insertStruct.item.iImage			= 0;
				insertStruct.item.iSelectedImage	= 0;
			}

			TCHAR treeItemText[1024], strTitle[512], className[512], hwndString[512];
			TCHAR strTid[512], strPid[512];

			memcpy(strPid,		 "\0", sizeof strPid);
			memcpy(strTid,		 "\0", sizeof strTid);
			memcpy(strTitle,	 "\0", sizeof strTitle);
			memcpy(className,	 "\0", sizeof className);
			memcpy(hwndString,	 "\0", sizeof hwndString);
			memcpy(treeItemText, "\0", sizeof treeItemText);
			
			TCHAR* hideStr		= _T(" [Hidden] ");
			TCHAR* disabledStr	= _T(" [Disabled] ");
			TCHAR* desktopStr	= _T(" (Desktop Window) ");
			TCHAR* winSwitchStr = _T(" (Win Switch) ");
			TCHAR* popupMenuStr = _T(" (Popup Menu) ");
			TCHAR* dialogStr	= _T(" (Dialog Box) ");
			TCHAR* iconTitleStr = _T(" (Icon Title Window) ");

			// get the HWND string
			sprintf( hwndString, " HWND = 0x%08x ", (int)wndInfo.m_hwnd);

			if (m_bShowWindowCaption)
			{
				// get the window title
				_tcscpy(strTitle, wndInfo.getWindowTitle());
				if (_tcslen(strTitle) > 0)
				{
					_tcscat(treeItemText, _T("\" "));
					_tcscat(treeItemText, strTitle);
					_tcscat(treeItemText, _T("\" "));
				}
			}
		
			// get the class name string
			if (m_bShowClassName)
			{
				_tcscpy(className, wndInfo.getClassName());

				if ( _tcsicmp(wndInfo.getClassName(), "#32768") == 0 )
				{
					// window is popup Menu
					_tcscat(className, popupMenuStr);
				}
				if ( _tcsicmp(wndInfo.getClassName(), "#32769") == 0 )
				{
					// window is desktop window.
					_tcscat(className, desktopStr);
				}
				if ( _tcsicmp(wndInfo.getClassName(), "#32770") == 0 )
				{
					// Dialog
					_tcscat(className, dialogStr);
				}
				if ( _tcsicmp(wndInfo.getClassName(), "#32771") == 0 )
				{
					// Win switch
					_tcscat(className, winSwitchStr);
				}
				if ( _tcsicmp(wndInfo.getClassName(), "#32772") == 0 )
				{
					// class for Icon Titles
					_tcscat(className, iconTitleStr);
				}
				// For more info, Refer "Window Classes in Win32" in MSDN

				// append class name string
				_tcscat(treeItemText, className);

			}
			// concatenate all the information and add to tree view.
			_tcscat(treeItemText, hwndString);

			DWORD dwPid = 0, dwTid = 0;
			dwTid = GetWindowThreadProcessId(wndInfo.m_hwnd, &dwPid);
			// get the processID string
			if (m_bShowProcessID)
			{
				sprintf(strPid, " (PID: 0x%08x) ", dwPid);
				// make upper
				_tcsupr(strPid);
				_tcscat(treeItemText, strPid);
			}
			// get the threadID string
			if (m_bShowThreadID)
			{
				sprintf(strTid, " (TID: 0x%08x) ", dwTid);
				_tcsupr(strTid);
				_tcscat(treeItemText, strTid);
			}

			if (!::IsWindowVisible(wndInfo.m_hwnd))
			{
				_tcscat(treeItemText, hideStr);
			}

			// Disabled
			if (!::IsWindowEnabled(wndInfo.m_hwnd))
			{
				_tcscat(treeItemText, disabledStr);
			}

			insertStruct.item.pszText = treeItemText;
			insertStruct.item.lParam =(LPARAM)&subTree;

			if ( ! m_bShowHiddenWindows )
			{
				HTREEITEM hCurrentItem = NULL;
				if (::IsWindowVisible(wndInfo.m_hwnd))
					hCurrentItem = GetTreeCtrl().InsertItem(&insertStruct);

				if (TRUE == HasChild(wndInfo.m_hwnd) && ::IsWindowVisible(wndInfo.m_hwnd))
				{
					// insert dummy item
					GetTreeCtrl().InsertItem(_T(""), 0, 1, hCurrentItem);
				}
			}
			else
			{
				HTREEITEM hCurrentItem = NULL;
				hCurrentItem = GetTreeCtrl().InsertItem(&insertStruct);
				if (TRUE == HasChild(wndInfo.m_hwnd))
				{
					// insert dummy item
					GetTreeCtrl().InsertItem(_T(""), 0, 1, hCurrentItem);
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// ViewPropertyPage() : Function to show the Modal Property Sheet
/////////////////////////////////////////////////////////////////////////////
void CWindowsView::ViewPropertyPage()
{
	CWaitCursor waitcursor;
	if (m_selectedWindow == NULL)
	{
		HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
		MTree<WindowInfo>* pTree = (MTree<WindowInfo>*)GetTreeCtrl().GetItemData(hItem);
		if (pTree != NULL)
		{
			*m_pSelWndInfo = pTree->getValue();
			m_pSelWndInfo->setWindowTitle(m_pSelWndInfo->m_hwnd);
			m_pSelWndInfo->setClassName(m_pSelWndInfo->m_hwnd);
		}
	}
	else
	{
		m_pSelWndInfo->m_hwnd = m_selectedWindow;
		m_pSelWndInfo->setWindowTitle(m_pSelWndInfo->m_hwnd);
		m_pSelWndInfo->setClassName(m_pSelWndInfo->m_hwnd);
	}

	if (FALSE == m_bPropertySheetCreated)
	{
		m_pTabSheet->Create(this);
		m_pTabSheet->update(m_pSelWndInfo);
		m_pTabSheet->ShowWindow(SW_SHOW);
		m_bPropertySheetCreated = TRUE;
	}
	else
	{
		m_pTabSheet->setWindowInfoObj(m_pSelWndInfo);
		m_pTabSheet->ShowWindow(SW_SHOW);
	}
}

/////////////////////////////////////////////////////////////////////////////
// OnRclick(): Function to show the PopupMenu
/////////////////////////////////////////////////////////////////////////////
void CWindowsView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ShowPopupMenu(/*TRUE*/);
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// OnShowProperties() : Function to show the Modal Property Sheet
/////////////////////////////////////////////////////////////////////////////
void CWindowsView::OnShowProperties() 
{
	ViewPropertyPage();
}

/////////////////////////////////////////////////////////////////////////////
// OnHighlightWindow() : Function used to Highlight the selected window
/////////////////////////////////////////////////////////////////////////////
void CWindowsView::OnHighlightWindow() 
{
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem( );
	MTree<WindowInfo>* pTree = (MTree<WindowInfo>*)GetTreeCtrl().GetItemData(hItem);
	if (pTree != NULL)
	{
		WindowInfo wndInfo = pTree->getValue();
		// check whether the window is valid or not
		if (!IsWindow(wndInfo.m_hwnd))
		{
			::MessageBox(m_hWnd, "The selected Window is no longer valid. Please try again after the page refresh.", "Highlight", MB_OK | MB_ICONEXCLAMATION);
			UpdateTree();
			return;
		}
		TRY
		{
			CWnd *pWnd = CWnd::FromHandle(wndInfo.m_hwnd);
			if (pWnd)
			{
				// Should not take input during the window flashing
				CWaitCursor cursor;
				CRect rect;
				rect.SetRectEmpty();
				pWnd->GetWindowRect(&rect);
				pWnd->ScreenToClient(&rect);

				// Flash 6 times. 3 times invert, 3 times revert!
				// In this function, we have if(pDc != NULL) conditions everywhere. 
				// This is needed because the user may close the window anytime during refreshes/sleep(s).
				for(int i = 0; i< 6; i++)
				{
					if (pWnd != NULL)
					{
						int nDCIdentifier = -1;
						CDC *pDc = NULL;
						if (pWnd != NULL)
							pDc = pWnd->GetWindowDC();
						if ((pDc != NULL) && (pWnd != NULL))
							nDCIdentifier = pDc->SaveDC();
						if ((pDc != NULL) && (pWnd != NULL) && (nDCIdentifier != -1))
							pDc->InvertRect(&rect);

						::SleepEx(240, FALSE);	// 1/4th second

						if ((pDc != NULL) && (nDCIdentifier != -1) && (pWnd != NULL))
						{
							pDc->RestoreDC(nDCIdentifier);
							nDCIdentifier = -1;
						}
						if ((pDc != NULL) && (pWnd != NULL))
							pWnd->ReleaseDC(pDc);
					}
				}
			}
		}
		CATCH(CException, pEx)
		{
			::MessageBox(this->m_hWnd, "Some error occured during highlighting. The Window may not be valid or doesn't exist.", "Highlight", MB_OK );
		}
		END_CATCH
	}
}

/////////////////////////////////////////////////////////////////////////////
// ShowPopupMenu() : Function to show popup menu
/////////////////////////////////////////////////////////////////////////////
void CWindowsView::ShowPopupMenu(BOOL bClickedWithMouse)
{
	CWaitCursor waitcursor;
	CMenu menu;
	if (menu.LoadMenu(IDR_WINDOWS_POPUPMENU))
	{
		CMenu *pSubMenu = menu.GetSubMenu(0);
		ASSERT(pSubMenu);
		CPoint position;
		// if the user right clicks in the client area, get the mouse position
		if (bClickedWithMouse)
			::GetCursorPos(&position);
		else
		{
			// get the left,top co-ordinates of the window to show the popup menu.
			CRect clientRect;
			clientRect.SetRectEmpty();
			this->GetWindowRect(&clientRect);
			position.x = clientRect.left;
			position.y = clientRect.top;
		}
		if (FALSE == m_bShowProcessID)
		{
			pSubMenu->CheckMenuItem(IDM_SHOW_PROCESS_ID, MF_BYCOMMAND | MF_UNCHECKED);
		}
		else
		{
			pSubMenu->CheckMenuItem(IDM_SHOW_PROCESS_ID, MF_BYCOMMAND | MF_CHECKED);
		}
		if (FALSE == m_bShowThreadID)
		{
			pSubMenu->CheckMenuItem(IDM_SHOW_THREAD_ID, MF_BYCOMMAND | MF_UNCHECKED);
		}
		else
		{
			pSubMenu->CheckMenuItem(IDM_SHOW_THREAD_ID, MF_BYCOMMAND | MF_CHECKED);
		}
		if (FALSE == m_bShowClassName)
		{
			pSubMenu->CheckMenuItem(IDM_SHOW_CLASSNAME, MF_BYCOMMAND | MF_UNCHECKED);
		}
		else
		{
			pSubMenu->CheckMenuItem(IDM_SHOW_CLASSNAME, MF_BYCOMMAND | MF_CHECKED);
		}
		if (FALSE == m_bShowWindowCaption)
		{
			pSubMenu->CheckMenuItem(ID_SHOW_WINDOWCAPTION, MF_BYCOMMAND | MF_UNCHECKED);
		}
		else
		{
			pSubMenu->CheckMenuItem(ID_SHOW_WINDOWCAPTION, MF_BYCOMMAND | MF_CHECKED);
		}
		if (FALSE == m_bShowHiddenWindows)
		{
			pSubMenu->CheckMenuItem(ID_SHOW_SHOWHIDDENWINDOWS, MF_BYCOMMAND | MF_UNCHECKED);
		}
		else
		{
			pSubMenu->CheckMenuItem(ID_SHOW_SHOWHIDDENWINDOWS, MF_BYCOMMAND | MF_CHECKED);
		}

		HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
		MTree<WindowInfo>* pTree = (MTree<WindowInfo>*)GetTreeCtrl().GetItemData(hItem);
		if (pTree != NULL)
		{
			WindowInfo windowInfo = pTree->getValue();
			HWND hwnd = windowInfo.m_hwnd;
			
			if (hwnd != NULL)
			{
				if ( ::IsWindow(hwnd))
				{
					pSubMenu->EnableMenuItem(ID_CLOSE_WINDOW, MF_BYCOMMAND | MF_ENABLED);
				}
				else
				{
					pSubMenu->EnableMenuItem(ID_CLOSE_WINDOW, MF_BYCOMMAND | MF_GRAYED);
				}
				
				if (::IsWindowVisible(hwnd))
				{
					pSubMenu->CheckMenuItem(ID_HIDE_WINDOW, MF_BYCOMMAND | MF_UNCHECKED);
				}
				else if (!::IsWindow(hwnd))
				{
					pSubMenu->EnableMenuItem(ID_HIDE_WINDOW, MF_BYCOMMAND | MF_GRAYED);
				}
				else
				{
					pSubMenu->CheckMenuItem(ID_HIDE_WINDOW, MF_BYCOMMAND | MF_CHECKED);
				}

				if (::IsWindowEnabled(hwnd))
				{
					pSubMenu->CheckMenuItem(ID_ENABLE_WINDOW, MF_BYCOMMAND | MF_CHECKED);
				}
				else if (!::IsWindow(hwnd))
				{
					pSubMenu->EnableMenuItem(ID_ENABLE_WINDOW, MF_BYCOMMAND | MF_GRAYED);
				}
				else
				{
					pSubMenu->CheckMenuItem(ID_ENABLE_WINDOW, MF_BYCOMMAND | MF_UNCHECKED);
				}
			}
		}

		this->SetForegroundWindow();
		pSubMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, position.x, position.y, this, NULL);
		this->PostMessage(WM_NULL, 0, 0);
	}
}

/////////////////////////////////////////////////////////////////////////////
// OnFindWindow() : Displays FindWindow dialog. If the FindWindow dialog
// returns a valid window, it shows the property sheet.
/////////////////////////////////////////////////////////////////////////////
void CWindowsView::OnFindWindow() 
{
	CFindWindow findWindow;
	if(IDOK == findWindow.DoModal())
	{
		m_selectedWindow = findWindow.getSelectedWindow();

		if ( ::IsWindow(m_selectedWindow) && (m_selectedWindow != NULL))
		{
			// selected current window
			if (GetWindowThreadProcessId(m_selectedWindow, NULL) == GetCurrentThreadId())
			{
				return;
			}
			ViewPropertyPage();
		}
		else
		{
			::MessageBox(m_hWnd, "The Window you have selected is no longer valid. Please refresh and try again.", "Find", MB_OK | MB_ICONINFORMATION);
		}

		// re-set the selectedWindow
		m_selectedWindow = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// OnRefresh() : Function refreshes the Tree control.
/////////////////////////////////////////////////////////////////////////////
void CWindowsView::OnRefresh() 
{
	// update the Tree
	UpdateTree();
}

/////////////////////////////////////////////////////////////////////////////
// OnGetText() : Function to get the text from any control.
/////////////////////////////////////////////////////////////////////////////
void CWindowsView::OnGetText() 
{
	HWND			selWindow = NULL;
	CFindWindow		findWindow;

	HTREEITEM hSelectedItem = GetTreeCtrl().GetSelectedItem();
	MTree<WindowInfo>* pTree = (MTree<WindowInfo>*)GetTreeCtrl().GetItemData(hSelectedItem);
	ASSERT(pTree != NULL);

	WindowInfo wndInfo = pTree->getValue();
	if(IDOK == findWindow.DoModal())
	{
		// get the selected window
		selWindow = findWindow.getSelectedWindow();
		CWnd *pWnd = CWnd::FromHandle(selWindow);
		ASSERT(pWnd);

		// Get the text of a given window
		FetchText(selWindow);		
	}
}

void CWindowsView::OnFindText() 
{
	CFindTxtDialog* pFindTextDlg = new CFindTxtDialog(this);
	ASSERT(pFindTextDlg);
	pFindTextDlg->Create(IDD_FIND_TEXT_DIALOG, this);
	pFindTextDlg->ShowWindow(SW_SHOW);
}

// FindItem			- Finds an item that contains the search string
// Returns			- Handle to the item or NULL
// str				- String to search for
// bCaseSensitive	- Should the search be case sensitive
// bDownDir			- Search direction - TRUE for down
// bWholeWord		- True if search should match whole words
// hItem			- Item to start searching from. NULL for
//					- currently selected item
HTREEITEM CWindowsView::FindItem(CString &str, BOOL bCaseSensitive /*= FALSE*/, 				
								 BOOL bDownDir /*= TRUE*/, BOOL bWholeWord /*= FALSE*/, 				
								 HTREEITEM hItem /*= NULL*/)
{
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


// IsFindValid	- Virtual function used by FindItem to allow this
//				  function to filter the result of FindItem
// Returns		- True if item matches the criteria
// Arg			- Handle of the item

BOOL CWindowsView::IsFindValid( HTREEITEM )
{	
	return TRUE;
}

// GetNextItem  - Get next item as if outline was completely expanded
// Returns      - The item immediately below the reference item
// hItem        - The reference item
HTREEITEM CWindowsView::GetNextItem( HTREEITEM hItem )
{
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

// GetNextItem		- Get previous item as if outline was completely expanded
// Returns          - The item immediately above the reference item
// hItem            - The reference item
HTREEITEM CWindowsView::GetPrevItem( HTREEITEM hItem )
{
        HTREEITEM       hti;

        hti = GetTreeCtrl().GetPrevSiblingItem(hItem);
        if( hti == NULL )
                hti = GetTreeCtrl().GetParentItem(hItem);
        else
                hti = GetLastItem(hti);
        return hti;
}

// GetLastItem  - Gets last item in the branch
// Returns      - Last item
// hItem        - Node identifying the branch. NULL will 
//                return the last item in outine
HTREEITEM CWindowsView::GetLastItem( HTREEITEM hItem )
{
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

LRESULT CWindowsView::OnFindHwndText(WPARAM wParam, LPARAM lParam)
{
	TCHAR* str = (TCHAR *)lParam;
	if (_tcslen(str) > 0)
	{
		CString searchStr = str;
		HTREEITEM treeItem = FindItem(searchStr, FALSE, TRUE, FALSE, GetTreeCtrl().GetSelectedItem());
		if ( NULL != treeItem )
		{
			GetTreeCtrl().Select(treeItem, TVGN_CARET);
			GetTreeCtrl().SelectItem(treeItem);
			GetTreeCtrl().EnsureVisible(treeItem);
		}
	}
	return 1;
}


LRESULT CWindowsView::OnMarkAll(WPARAM wParam, LPARAM lParam)
{
	CFindTxtDialog *pFindTextDlg = (CFindTxtDialog *)lParam;
	if (pFindTextDlg)
	{
		CString str			= pFindTextDlg->getFindText();
		BOOL	bWholeWord	= pFindTextDlg->getWholeWord();
		BOOL	bMatchCase	= pFindTextDlg->getMatchCase();
		BOOL	bDirection	= pFindTextDlg->getDirection();
		BOOL	bIncludeSubFolders = pFindTextDlg->getIncludeSubFolders();
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
						CString tmpStr = "Cannot find the string \'" + str + "\'";
						::MessageBox(m_hWnd, tmpStr, "Find String", MB_OK | MB_ICONINFORMATION);
					}
					return 1;
				}
			}
		}
	}
	return 1;
}

LRESULT CWindowsView::OnFindNext(WPARAM wParam, LPARAM lParam)
{
	CFindTxtDialog *pFindTextDlg = (CFindTxtDialog *)lParam;
	if (pFindTextDlg)
	{
		CString str			= pFindTextDlg->getFindText();
		BOOL	bWholeWord	= pFindTextDlg->getWholeWord();
		BOOL	bMatchCase	= pFindTextDlg->getMatchCase();
		BOOL	bDirection	= pFindTextDlg->getDirection();
		BOOL	bIncludeSubFolders = pFindTextDlg->getIncludeSubFolders();
		if (!str.IsEmpty())
		{
			if (TRUE == bIncludeSubFolders && (m_bExpandedAll == FALSE))
			{
				// collapse the entire tree and Expand entire tree, and search
				GetTreeCtrl().Expand(GetTreeCtrl().GetRootItem(), TVE_COLLAPSE);
				ExpandAll();
			}
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

HRESULT WINAPI Shell_CreateLink (LPCTSTR pszLinkFilePathname, PLINKDATA pld) 
{
	HRESULT hres;
	IShellLink* psl;
	IPersistFile* ppf;
	hres = CoInitialize(NULL);	// Create a shell link object
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (PVOID *) &psl);

	if (SUCCEEDED(hres))
	{
		// Initialize the shell link object
		psl->SetPath(pld->pszPathname);
		if (pld->fdwFlags & LD_USEARGS)
			psl->SetArguments(pld->pszArgs);
		
		if (pld->fdwFlags & LD_USEDESC) 
			psl->SetDescription(pld->pszDesc);
		
		if (pld->fdwFlags & LD_USEICON)
			psl->SetIconLocation(pld->pszIconPath, pld->nIconIndex);
		
		if (pld->fdwFlags & LD_USEWORKDIR)
			psl->SetWorkingDirectory(pld->pszWorkingDir);
		
		if (pld->fdwFlags & LD_USESHOWCMD)
			psl->SetShowCmd(pld->nShowCmd);
		
		if (pld->fdwFlags & LD_USEHOTKEY)
			psl->SetHotkey(pld->wHotkey);
		
		// Save the shell link object on the disk
		hres = psl->QueryInterface(IID_IPersistFile, (PVOID *) &ppf);
		
		if (SUCCEEDED(hres))
		{
			#ifndef UNICODE 	// Convert the ANSI path to a Unicode path
				WCHAR szPath[_MAX_PATH] = { 0 };
				MultiByteToWideChar(CP_ACP, 0, pszLinkFilePathname, strlen(pszLinkFilePathname), szPath, ARRAY_SIZE(szPath));
				hres = ppf->Save(szPath, TRUE);
			#else
				hres = ppf->Save(pszLinkFilePathname, TRUE);
			#endif
				
				ppf->Release();
		}	
		
		psl->Release();
	}
	CoUninitialize();
	return(hres);
}

void CWindowsView::OnCreateShortcut() 
{
	TCHAR strFileName[256 + 1];
	TCHAR winDir[256 + 1];

	DWORD dwRet = ::GetModuleFileName(NULL, strFileName, 256);
	if (dwRet == 0)
		return;

	LINKDATA linkData;
	linkData.pszPathname = strFileName;
	linkData.fdwFlags	 = LD_USEDESC | LD_USEICON;
	linkData.pszDesc	 = "Infoview By Suresh";
	linkData.pszIconPath = strFileName;
	linkData.nIconIndex  = 0;
	
	GetWindowsDirectory(winDir, 256);
	_tcscat(winDir, "\\Desktop\\Suresh's InfoViewer.lnk");
	Shell_CreateLink (winDir, &linkData);
}

void CWindowsView::OnCopyHwnd() 
{
	WindowInfo wndInfo;
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	MTree<WindowInfo>* pTree = (MTree<WindowInfo>*)GetTreeCtrl().GetItemData(hItem);
	if (pTree == NULL)
	{
		return;
	}
	wndInfo = pTree->getValue();

	CString HWNDString;
	HWNDString.Format("0x%08x", wndInfo.m_hwnd);
	HWNDString.MakeUpper();

	Copy2Clipboard(HWNDString);
}

void CWindowsView::OnCopyWindowCaption() 
{
	WindowInfo wndInfo;
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	MTree<WindowInfo>* pTree = (MTree<WindowInfo>*)GetTreeCtrl().GetItemData(hItem);
	if (pTree == NULL)
	{
		return;
	}
	wndInfo = pTree->getValue();
	CString strWindowCaption('\0', 255);
	::GetWindowText(wndInfo.m_hwnd, strWindowCaption.GetBuffer(255), 255);
	Copy2Clipboard(strWindowCaption);
}

void CWindowsView::OnCopyClassName() 
{
	WindowInfo wndInfo;
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	MTree<WindowInfo>* pTree = (MTree<WindowInfo>*)GetTreeCtrl().GetItemData(hItem);
	if (pTree == NULL)
	{
		return;
	}
	wndInfo = pTree->getValue();

	CString strClassName('\0', 256);
	::GetClassName(wndInfo.m_hwnd, strClassName.GetBuffer(255), 255);
	strClassName.ReleaseBuffer();
	Copy2Clipboard(strClassName);
}

void CWindowsView::OnCopyEntiretext() 
{
	WindowInfo wndInfo;
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CString strData = GetTreeCtrl().GetItemText(hItem);
	Copy2Clipboard(strData);
}

// CollapseAll  - Collapses the complete outline.
void CWindowsView::CollapseAll()
{
    HTREEITEM hti = GetTreeCtrl().GetRootItem();
    do
	{
            CollapseBranch( hti );
    }while( (hti = GetTreeCtrl().GetNextSiblingItem( hti )) != NULL );
}

// CollapseBranch       - Collapses a branch completely
// hti                  -  Handle of the tree item to collapse
void CWindowsView::CollapseBranch( HTREEITEM hti)
{
	if( GetTreeCtrl().ItemHasChildren( hti ) )
	{
		GetTreeCtrl().Expand( hti, TVE_COLLAPSE );
		hti = GetTreeCtrl().GetChildItem( hti );
		do
		{
			CollapseBranch( hti );
		}while( (hti = GetTreeCtrl().GetNextSiblingItem( hti )) != NULL );        
	}
}

void CWindowsView::ExpandAll()
{
    HTREEITEM hti = GetTreeCtrl().GetRootItem();
    do{
            ExpandBranch( hti );
    }while( (hti = GetTreeCtrl().GetNextSiblingItem( hti )) != NULL );
	m_bExpandedAll = TRUE;
}

void CWindowsView::ExpandBranch( HTREEITEM hti)
{
	if( GetTreeCtrl().ItemHasChildren( hti ) )
	{
		GetTreeCtrl().Expand( hti, TVE_EXPAND);
		hti = GetTreeCtrl().GetChildItem( hti );
		do
		{
			ExpandBranch( hti );
		}while( (hti = GetTreeCtrl().GetNextSiblingItem( hti )) != NULL );        
	}
}

void CWindowsView::AppendTreeItemText(HTREEITEM hItem, CString newString)
{
	if (hItem == NULL) return;

	CString strItemText;
	TCHAR* hideStr		= _T(" [Hidden] ");

	TVITEM item;
	TCHAR szText[1024];
	item.hItem = hItem;
	item.mask = TVIF_TEXT | TVIF_HANDLE;
	item.pszText = szText;
	item.cchTextMax = 1024;
	BOOL bWorked = GetTreeCtrl().GetItem(&item);
	if (bWorked)
	{
		strItemText = szText;

		// Append new text
		strItemText += _T("") + newString;
		strItemText = strItemText.Trim();
		item.pszText = strItemText.GetBuffer();
		GetTreeCtrl().SetItem(&item);
		strItemText.ReleaseBuffer();
	}
}

void CWindowsView::ReplaceTreeItemText(HTREEITEM hItem, CString oldString, CString newString)
{
	if (hItem == NULL) return;

	CString strItemText;

	TVITEM item;
	TCHAR szText[1024];
	item.hItem = hItem;
	item.mask = TVIF_TEXT | TVIF_HANDLE;
	item.pszText = szText;
	item.cchTextMax = 1024;
	BOOL bWorked = GetTreeCtrl().GetItem(&item);

	if (bWorked)
	{
		strItemText = szText;
		if ( -1 != strItemText.Find(oldString, 0))
		{
			strItemText.Replace(oldString, _T(""));
		}
		else // Didn't find the specified text
		{
			// Do not append the text if already exists
			if ( -1 == strItemText.Find(newString,0))
			{
				strItemText += newString;
			}
		}
		strItemText = strItemText.Trim();
		item.pszText = strItemText.GetBuffer();
		GetTreeCtrl().SetItem(&item);
		strItemText.ReleaseBuffer();
	}
}

void CWindowsView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (m_bUpdate || (FALSE == m_bPropertySheetCreated))
	{
		*pResult = 0;
		return;
	}

	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	MTree<WindowInfo>* pTree = (MTree<WindowInfo>*)GetTreeCtrl().GetItemData(hItem);
	if (pTree != NULL)
	{
		*m_pSelWndInfo = pTree->getValue();
		m_pSelWndInfo->setWindowTitle(m_pSelWndInfo->m_hwnd);
		m_pSelWndInfo->setClassName(m_pSelWndInfo->m_hwnd);
	}

	if (::IsWindow(m_pTabSheet->GetSafeHwnd()))
	{
		m_pTabSheet->setWindowInfoObj(m_pSelWndInfo);
		m_pTabSheet->update(m_pSelWndInfo);
	}
	*pResult = 0;
}

void CWindowsView::OnDestroy() 
{
	CTreeView::OnDestroy();
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "ShowClassName", m_bShowClassName);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "ShowPID", m_bShowProcessID);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "ShowTID", m_bShowThreadID);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "ShowWindowCaption", m_bShowWindowCaption);
}

void CWindowsView::OnShowClassname() 
{
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();
	ASSERT(NULL != pMenu);
	if (MF_UNCHECKED == pMenu->GetMenuState(IDM_SHOW_CLASSNAME, MF_BYCOMMAND))
	{
		pMenu->CheckMenuItem(IDM_SHOW_CLASSNAME, MF_BYCOMMAND | MF_CHECKED);
		m_bShowClassName = !m_bShowClassName;
		UpdateTree();
	}
	else
	{
		pMenu->CheckMenuItem(IDM_SHOW_CLASSNAME, MF_BYCOMMAND | MF_UNCHECKED);
		m_bShowClassName = !m_bShowClassName;
		UpdateTree();
	}		
}

void CWindowsView::OnShowProcessId() 
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

void CWindowsView::OnShowThreadId() 
{
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();
	ASSERT(NULL != pMenu);
	if (MF_UNCHECKED == pMenu->GetMenuState(IDM_SHOW_THREAD_ID, MF_BYCOMMAND))
	{
		pMenu->CheckMenuItem(IDM_SHOW_THREAD_ID, MF_BYCOMMAND | MF_CHECKED);
		m_bShowThreadID = !m_bShowThreadID;
		UpdateTree();
	}
	else
	{
		pMenu->CheckMenuItem(IDM_SHOW_THREAD_ID, MF_BYCOMMAND | MF_UNCHECKED);
		m_bShowThreadID = !m_bShowThreadID;
		UpdateTree();
	}		
}

void CWindowsView::OnUpdateShowClassname(CCmdUI* pCmdUI) 
{
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();
	ASSERT(NULL != pMenu);
	if (FALSE == m_bShowClassName)
	{
		pMenu->CheckMenuItem(IDM_SHOW_CLASSNAME, MF_BYCOMMAND | MF_UNCHECKED);
	}
	else
	{
		pMenu->CheckMenuItem(IDM_SHOW_CLASSNAME, MF_BYCOMMAND | MF_CHECKED);
	}
}

void CWindowsView::OnUpdateShowProcessId(CCmdUI* pCmdUI) 
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

void CWindowsView::OnUpdateShowThreadId(CCmdUI* pCmdUI) 
{
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();
	ASSERT(NULL != pMenu);
	if (FALSE == m_bShowThreadID)
	{
		pMenu->CheckMenuItem(IDM_SHOW_THREAD_ID, MF_BYCOMMAND | MF_UNCHECKED);
	}
	else
	{
		pMenu->CheckMenuItem(IDM_SHOW_THREAD_ID, MF_BYCOMMAND | MF_CHECKED);
	}
}

void CWindowsView::OnHideWindow()
{
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	MTree<WindowInfo>* pTree = (MTree<WindowInfo>*)GetTreeCtrl().GetItemData(hItem);
	if (pTree != NULL)
	{
		WindowInfo windowInfo = pTree->getValue();
		HWND hwnd = windowInfo.m_hwnd;
		if (hwnd == NULL) return;

		if (::IsWindowVisible(hwnd))
		{
			::ShowWindow(hwnd, SW_HIDE);
			AppendTreeItemText(hItem, _T("[Hidden]"));
		}
		else
		{
			::ShowWindow(hwnd, SW_SHOW);
			ReplaceTreeItemText(hItem, _T("[Hidden]"), _T(""));
		}
	}
}

void CWindowsView::OnUpdateHideWindow(CCmdUI* pCmdUI)
{
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	MTree<WindowInfo>* pTree = (MTree<WindowInfo>*)GetTreeCtrl().GetItemData(hItem);
	if (pTree != NULL)
	{
		WindowInfo windowInfo = pTree->getValue();
		HWND hwnd = windowInfo.m_hwnd;

		CMenu* pMenu = AfxGetMainWnd()->GetMenu();
		ASSERT(NULL != pMenu);

		if ((hwnd == NULL) || (!::IsWindow(hwnd)))
		{
			pMenu->EnableMenuItem(ID_HIDE_WINDOW, MF_BYCOMMAND | MF_GRAYED);
			return;
		}

		if (::IsWindowVisible(hwnd))
		{
			pMenu->EnableMenuItem(ID_HIDE_WINDOW, MF_BYCOMMAND | MF_CHECKED);
		}
		else
		{
			pMenu->EnableMenuItem(ID_HIDE_WINDOW, MF_BYCOMMAND | MF_UNCHECKED);
		}
	}
}

void CWindowsView::OnEnableWindow()
{
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	MTree<WindowInfo>* pTree = (MTree<WindowInfo>*)GetTreeCtrl().GetItemData(hItem);
	if (pTree != NULL)
	{
		WindowInfo windowInfo = pTree->getValue();
		HWND hwnd = windowInfo.m_hwnd;
		if (hwnd == NULL) return;

		if (::IsWindowEnabled(hwnd))
		{
			::EnableWindow(hwnd, FALSE);
			AppendTreeItemText(hItem, _T("[Disabled]"));
		}
		else
		{
			::EnableWindow(hwnd, TRUE);
			ReplaceTreeItemText(hItem, _T("[Disabled]"), _T(""));
		}
	}
}

void CWindowsView::OnUpdateEnableWindow(CCmdUI* pCmdUI)
{
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	MTree<WindowInfo>* pTree = (MTree<WindowInfo>*)GetTreeCtrl().GetItemData(hItem);
	if (pTree != NULL)
	{
		WindowInfo windowInfo = pTree->getValue();
		HWND hwnd = windowInfo.m_hwnd;

		CMenu* pMenu = AfxGetMainWnd()->GetMenu();
		ASSERT(NULL != pMenu);

		if ((hwnd == NULL) || (!::IsWindow(hwnd)))
		{
			pMenu->EnableMenuItem(ID_ENABLE_WINDOW, MF_BYCOMMAND | MF_GRAYED);
			return;
		}

		if (::IsWindowEnabled(hwnd))
		{
			pMenu->EnableMenuItem(ID_ENABLE_WINDOW, MF_BYCOMMAND | MF_CHECKED);
		}
		else
		{
			pMenu->EnableMenuItem(ID_ENABLE_WINDOW, MF_BYCOMMAND | MF_UNCHECKED);
		}
	}
}

void CWindowsView::OnCloseWindow()
{
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	MTree<WindowInfo>* pTree = (MTree<WindowInfo>*)GetTreeCtrl().GetItemData(hItem);
	if (pTree != NULL)
	{
		WindowInfo windowInfo = pTree->getValue();
		HWND hwnd = windowInfo.m_hwnd;
		if ((hwnd) && ::IsWindow(hwnd))
		{
			// window is valid, close it as per user's request!
			::SendMessage(hwnd, WM_CLOSE, 0,0);
			AppendTreeItemText(hItem, "[Closed]");
			GetTreeCtrl().SetItemState(hItem, TVIS_BOLD, TVIS_BOLD);
		}
	}
}

void CWindowsView::OnUpdateCloseWindow(CCmdUI* pCmdUI)
{
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	MTree<WindowInfo>* pTree = (MTree<WindowInfo>*)GetTreeCtrl().GetItemData(hItem);
	if (pTree != NULL)
	{
		WindowInfo windowInfo = pTree->getValue();
		HWND hwnd = windowInfo.m_hwnd;

		CMenu* pMenu = AfxGetMainWnd()->GetMenu();
		ASSERT(NULL != pMenu);

		if ((hwnd) && ::IsWindow(hwnd))
		{
			pMenu->EnableMenuItem(ID_CLOSE_WINDOW, MF_BYCOMMAND | MF_ENABLED);
		}
		else
		{
			pMenu->EnableMenuItem(ID_CLOSE_WINDOW, MF_BYCOMMAND | MF_DISABLED);
		}
	}
}

void CWindowsView::OnUpdateOnShowHiddenWindows(CCmdUI* pCmdUI)
{
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();
	ASSERT(NULL != pMenu);
	if (FALSE == m_bShowHiddenWindows)
	{
		pMenu->CheckMenuItem(ID_SHOW_SHOWHIDDENWINDOWS, MF_BYCOMMAND | MF_UNCHECKED);
	}
	else
	{
		pMenu->CheckMenuItem(ID_SHOW_SHOWHIDDENWINDOWS, MF_BYCOMMAND | MF_CHECKED);
	}
}

void CWindowsView::OnShowHiddenWindows()
{
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();
	ASSERT(NULL != pMenu);
	if (MF_UNCHECKED == pMenu->GetMenuState(ID_SHOW_SHOWHIDDENWINDOWS, MF_BYCOMMAND))
	{
		pMenu->CheckMenuItem(ID_SHOW_SHOWHIDDENWINDOWS, MF_BYCOMMAND | MF_CHECKED);
		m_bShowHiddenWindows = !m_bShowHiddenWindows;
		UpdateTree();
	}
	else
	{
		pMenu->CheckMenuItem(ID_SHOW_SHOWHIDDENWINDOWS, MF_BYCOMMAND | MF_UNCHECKED);
		m_bShowHiddenWindows = !m_bShowHiddenWindows;
		UpdateTree();
	}
}

void CWindowsView::OnShowWindowcaption() 
{	
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();
	ASSERT(NULL != pMenu);
	if (MF_UNCHECKED == pMenu->GetMenuState(ID_SHOW_WINDOWCAPTION, MF_BYCOMMAND))
	{
		pMenu->CheckMenuItem(ID_SHOW_WINDOWCAPTION, MF_BYCOMMAND | MF_CHECKED);
		m_bShowWindowCaption = !m_bShowWindowCaption;
		UpdateTree();
	}
	else
	{
		pMenu->CheckMenuItem(ID_SHOW_WINDOWCAPTION, MF_BYCOMMAND | MF_UNCHECKED);
		m_bShowWindowCaption = !m_bShowWindowCaption;
		UpdateTree();
	}			
}

void CWindowsView::OnUpdateShowWindowcaption(CCmdUI* pCmdUI) 
{
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();
	ASSERT(NULL != pMenu);
	if (FALSE == m_bShowWindowCaption)
	{
		pMenu->CheckMenuItem(ID_SHOW_WINDOWCAPTION, MF_BYCOMMAND | MF_UNCHECKED);
	}
	else
	{
		pMenu->CheckMenuItem(ID_SHOW_WINDOWCAPTION, MF_BYCOMMAND | MF_CHECKED);
	}
}