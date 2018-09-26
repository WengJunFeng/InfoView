// MessagesView.cpp : implementation file
//

#include "stdafx.h"
#include "infoview.h"
#include "ChildFrm.h"
#include "MessagesView.h"
#include "FindWindow.h"
#include "./InfoHook/HHook.h"
#include "./InfoHook/WinMsg.h"
#include "MsgOptionsDlg.h"
#include "Windowsx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define STRUCT_SIZE(Struct)	\
	(sizeof(Struct) / sizeof((Struct)[0]))


/////////////////////////////////////////////////////////////////////////////
// CMessagesView

IMPLEMENT_DYNCREATE(CMessagesView, CListView)

CMessagesView::CMessagesView()
{
	m_OnInitalUpdateCalled = FALSE;
	m_MaxLines = 0;
}

CMessagesView::~CMessagesView()
{
}

BEGIN_MESSAGE_MAP(CMessagesView, CListView)
	//{{AFX_MSG_MAP(CMessagesView)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(IDM_CLEAR_VIEW, OnClearView)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_WM_COPYDATA()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessagesView drawing

void CMessagesView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CMessagesView diagnostics

#ifdef _DEBUG
void CMessagesView::AssertValid() const
{
	CListView::AssertValid();
}

void CMessagesView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

BOOL CMessagesView::PreTranslateMessage(MSG* pMsg) 
{
	if( ! CListView::PreTranslateMessage(pMsg) )
		return m_listctrl.PreTranslateMessage(pMsg);

	return FALSE;
}

LRESULT CMessagesView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	LRESULT lResult = 0;
	if (!OnWndMsg(message, wParam, lParam, &lResult))
		if( !m_listctrl.OnWndMsg(message, wParam, lParam, &lResult))
			lResult = DefWindowProc(message, wParam, lParam);
	return lResult;
}

BOOL CMessagesView::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	if( !CListView::OnChildNotify(message, wParam, lParam, pLResult) )
		return m_listctrl.OnChildNotify(message, wParam, lParam, pLResult) ;

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CMessagesView message handlers

void CMessagesView::OnInitialUpdate() 
{
	CWaitCursor waitCursor;

	CListView::OnInitialUpdate();
	LV_COLUMN column; 

	m_ImageList.Create(16, 16, ILC_MASK, 0, 8);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_OPEN_FOLDER);
	m_ImageList.Add(m_hIcon);

	GetListCtrl().SetImageList(&m_ImageList, LVSIL_SMALL);

	CString strColumnCaption("Window Message");
	// set the column properties
	column.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH ;
	column.fmt = LVCFMT_LEFT;
	column.cchTextMax= strColumnCaption.GetLength();
	column.iSubItem= 0;
	column.cx = 125;
	column.pszText = strColumnCaption.GetBuffer(strColumnCaption.GetLength());
    GetListCtrl().InsertColumn(0,&column);
	strColumnCaption.ReleaseBuffer();

	// 1st column
	strColumnCaption = _T("Message Type");
	column.pszText= strColumnCaption.GetBuffer(strColumnCaption.GetLength());
	column.cx = 100;
	GetListCtrl().InsertColumn(1,&column);
	strColumnCaption.ReleaseBuffer();

	// 2nd column
	strColumnCaption = _T("HWND");
	column.pszText= strColumnCaption.GetBuffer(strColumnCaption.GetLength());
	column.cx = 75;
	GetListCtrl().InsertColumn(2,&column);
	strColumnCaption.ReleaseBuffer();

	// 3rd column
	strColumnCaption = _T("WParam");
	column.pszText= strColumnCaption.GetBuffer(strColumnCaption.GetLength());
	column.cx = 100;
	GetListCtrl().InsertColumn(3,&column);
	strColumnCaption.ReleaseBuffer();

	// 4th column
	strColumnCaption = _T("LParam");
	column.pszText= strColumnCaption.GetBuffer(strColumnCaption.GetLength());
	column.cx = 100;
	GetListCtrl().InsertColumn(4,&column);
	strColumnCaption.ReleaseBuffer();

	// 5th column
	strColumnCaption = _T("Extra Information");
	column.pszText= strColumnCaption.GetBuffer(strColumnCaption.GetLength());
	column.cx = 100;
	GetListCtrl().InsertColumn(5,&column);
	strColumnCaption.ReleaseBuffer();

	m_OnInitalUpdateCalled = TRUE;

	CMsgOptionsDlg dlg;
	if(IDOK == dlg.DoModal())
	{
		m_MaxLines = ((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "MaxLines", 750);

		HWND selectedWindow = dlg.getSelectedWindow();
		if (::IsWindow(selectedWindow) && (selectedWindow != NULL))
		{
			StopShowingMessages();
			ShowMessages( selectedWindow, GetSafeHwnd());
		}
		else
		{
			::MessageBox(m_hWnd, "Oops, the selected window is not a valid window to show messages.", "InfoView", MB_OK | MB_ICONINFORMATION);
			CChildFrame *pChildFrame = (CChildFrame *) GetParent();
			ASSERT(NULL != pChildFrame);
			pChildFrame->PostMessage(WM_CLOSE, 0L, 0L);
			return;
		}
	}
	else
	{
		CChildFrame *pChildFrame = (CChildFrame *) GetParent();
		ASSERT(NULL != pChildFrame);
		pChildFrame->PostMessage(WM_CLOSE, 0L, 0L);
		return;
	}
	CChildFrame *pChildFrame = (CChildFrame *) GetParent();
	pChildFrame->SetWindowText("Messages");
}

void CMessagesView::OnDestroy() 
{
	CListView::OnDestroy();
	StopShowingMessages();
}

int CMessagesView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_listctrl.m_hWnd = m_hWnd;
	m_listctrl.m_pfnSuper = m_pfnSuper;
	m_listctrl.PreSubclassWindow();

	return 0;
}

void CMessagesView::OnSize(UINT nType, int cx, int cy) 
{
	CListView::OnSize(nType, cx, cy);
}

void CMessagesView::OnClearView() 
{
	GetListCtrl().DeleteAllItems();
}

BOOL CMessagesView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= LVS_SHOWSELALWAYS | LVS_REPORT | LVS_SINGLESEL;
	return CListView::PreCreateWindow(cs);
}

BOOL CMessagesView::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	INFO_COPYDATA *pData = (INFO_COPYDATA *)pCopyDataStruct->lpData;
	if (NULL != pData)
	{
		// if "max lines" reaches, delete all the items and add new entries
		if (GetListCtrl().GetItemCount() >= m_MaxLines)
		{
			OnClearView();
		}
		LV_ITEM item;
		CString str;

		item.mask			= LVIF_TEXT | LVIF_STATE | LVIF_IMAGE;
		item.iItem			= GetListCtrl().GetItemCount();
		item.iSubItem		= 0;
		item.state			= 0;
		item.stateMask		= 0;
		item.pszText		= pData->strMsgName;
		item.cchTextMax		= strlen(pData->strMsgName);
		item.iImage			= 0;
		item.lParam			= (LPARAM) 0;
		int nIndex = GetListCtrl().InsertItem(&item);
		GetListCtrl().SetItemData(nIndex, (DWORD_PTR)pData);

		item.mask			= LVIF_TEXT;
		item.iItem			= (GetListCtrl().GetItemCount() - 1);
		item.iSubItem		= 1;
		item.pszText		= pData->strMsgType;
		item.cchTextMax		= _tcslen(pData->strMsgType);
		item.lParam			= (LPARAM) 0;
		GetListCtrl().SetItem(&item);

		str.Format("0x%08x", pData->hwndFrom);
		item.iSubItem		= 2;
		item.pszText		= (LPTSTR)(LPCTSTR)str;
		item.lParam			= (LPARAM) 0;
		GetListCtrl().SetItem(&item);

		str.Empty();
		str.Format("0x%08x", pData->wParam);
		item.iSubItem		= 3;
		item.pszText		= (LPTSTR)(LPCTSTR)str;
		item.lParam			= (LPARAM) 0;
		GetListCtrl().SetItem(&item);

		str.Empty();
		str.Format("0x%08x", pData->lParam);
		item.iSubItem		= 4;
		item.pszText		= (LPTSTR)(LPCTSTR)str;
		item.lParam			= (LPARAM) 0;
		GetListCtrl().SetItem(&item);

		BOOL isKeyBoardMessage = FALSE;
		for ( int i = 0; i < STRUCT_SIZE(KeyBoardMessagesS); i++ )
		{
			if (_tcsicmp(KeyBoardMessagesS[i].MsgName, pData->strMsgName) == 0)
			{
				isKeyBoardMessage = TRUE;
			}
		}

		str.Empty();
		if ( isKeyBoardMessage )
		{
			str.Format("(%c) ScanCode: 0x%02X", (LPCSTR)pData->wParam, HIWORD (pData->lParam) & 0x00ff);
		}
		else
		{
			str.Format("ScanCode: 0x%02X", HIWORD (pData->lParam) & 0x00ff);
		}
		item.iSubItem		= 5;
		item.pszText		= (LPTSTR)(LPCTSTR)str;
		item.lParam			= (LPARAM) 0;
		GetListCtrl().SetItem(&item);
	}
	return CListView::OnCopyData(pWnd, pCopyDataStruct);
}

void CMessagesView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CWaitCursor waitcursor;
	CMenu menu;
	if (menu.LoadMenu(IDR_MESSAGES_POPUPMENU))
	{
		CPoint position;
		::GetCursorPos(&position);

		CMenu *pSubMenu = menu.GetSubMenu(0);
		ASSERT(pSubMenu);

		this->SetForegroundWindow();
		pSubMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, position.x, position.y, this, NULL);
		this->PostMessage(WM_NULL, 0, 0);
	}	
	*pResult = 0;
}