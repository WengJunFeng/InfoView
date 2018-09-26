////////////////////////////////////////////////////////////////
// CWindowsPage
// If this code works, it was written by Suresh.

// WindowsPage.cpp : implementation file
//

#include "stdafx.h"
#include "WindowInfo.h"
#include "WindowsPage.h"
#include "InfoUtil.h"
#include "TabSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWindowsPage property page

IMPLEMENT_DYNCREATE(CWindowsPage, CPropertyPage)

CWindowsPage::CWindowsPage() : CPropertyPage(CWindowsPage::IDD)
{
	//{{AFX_DATA_INIT(CWindowsPage)
	m_ReflectSelection = TRUE;
	//}}AFX_DATA_INIT
	m_bWindowHide = FALSE;
	m_bWindowClose = FALSE;
	m_pBrush = new CBrush(::GetSysColor(COLOR_3DFACE));
}

CWindowsPage::~CWindowsPage()
{
	if (m_pBrush)
	{
		delete m_pBrush;
		m_pBrush = NULL;
	}
}

void CWindowsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWindowsPage)
	DDX_Control(pDX, IDC_INVALID_WINDOW, m_InvalidWindow);
	DDX_Check(pDX, IDC_REFLECT_SELECTION, m_ReflectSelection);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWindowsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CWindowsPage)
	ON_BN_CLICKED(IDC_CLOSE_WINDOW, OnCloseWindow)
	ON_BN_CLICKED(IDC_HIDE_WINDOW, OnHideWindow)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_DISABLE_WINDOW, OnDisableWindow)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_UNHIDE_WINDOW, OnHideWindow)
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_REFRESH, OnRefresh)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindowsPage message handlers

BOOL CWindowsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	m_InvalidWindow.SetFontBold(TRUE);
	m_InvalidWindow.SetTextColor( 706 );	// Red color
	updateDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWindowsPage::OnCloseWindow() 
{
	// The user should not be able to terminate Infoviewer.
	if (!(GetWindowThreadProcessId(m_pWindowInfo->m_hwnd, NULL) == GetCurrentThreadId()))
	{
		if (IDOK == ::MessageBox(m_hWnd, "You may not be able to access the window after closing it. Do you want to close it?", "Close", MB_OKCANCEL | MB_ICONQUESTION))
		{
			// close the window
			::SendMessage(m_pWindowInfo->m_hwnd, WM_CLOSE, 0L, 0L);	

			// Disable "Hide window" and "close window" controls.
			CButton *pHideButton = (CButton *)GetDlgItem(IDC_HIDE_WINDOW);
			ASSERT(pHideButton);
			pHideButton->ShowWindow(SW_HIDE);

			CButton *pUnHideButton = (CButton *)GetDlgItem(IDC_UNHIDE_WINDOW);
			ASSERT(pUnHideButton);
			pUnHideButton->ShowWindow(SW_HIDE);

			CButton *pCloseButton = (CButton *)GetDlgItem(IDC_CLOSE_WINDOW);
			ASSERT(pCloseButton);
			pCloseButton->ShowWindow(SW_HIDE);
		}
		else
		{
			CButton *pButton = (CButton *)GetDlgItem(IDC_CLOSE_WINDOW);
			ASSERT(pButton);
			pButton->SetCheck(0);
		}
	}
	else
	{
		::MessageBox(m_hWnd, "It is not a good idea to terminate InfoView from InfoViewer. Please select any other window to close :-)", "Suicide", MB_OKCANCEL | MB_ICONQUESTION);
		CButton *pButton = (CButton *)GetDlgItem(IDC_CLOSE_WINDOW);
		ASSERT(pButton);
		pButton->SetCheck(0);
	}
}

void CWindowsPage::OnHideWindow() 
{
	CButton* pHideButton	= (CButton *)GetDlgItem(IDC_HIDE_WINDOW);
	CButton* pUnHideButton	= (CButton *)GetDlgItem(IDC_UNHIDE_WINDOW);
	
	ASSERT(pHideButton);
	ASSERT(pUnHideButton);

	if (FALSE == m_bWindowHide)
	{
		::ShowWindow(m_pWindowInfo->m_hwnd, SW_HIDE);
		pHideButton->EnableWindow(FALSE);
		pUnHideButton->EnableWindow(TRUE);
		m_bWindowHide = TRUE;
	}
	else
	{
		::ShowWindow(m_pWindowInfo->m_hwnd, SW_SHOW);
		pHideButton->EnableWindow(TRUE);
		pUnHideButton->EnableWindow(FALSE);
		m_bWindowHide = FALSE;
	}

	pHideButton->SetCheck(0);
	pUnHideButton->SetCheck(0);
}

CString CWindowsPage::getFormattedString(const HWND& hwnd) const
{
	CString strHwnd;
	if (NULL == hwnd)
		strHwnd = _T("(None)");
	else
	{
		strHwnd.Format("0x%08x", hwnd);
		strHwnd.MakeUpper();
	}
	return strHwnd;
}

HBRUSH CWindowsPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	if ((pWnd->GetDlgCtrlID() == IDC_PARENT_WINDOW) ||
		(pWnd->GetDlgCtrlID() == IDC_CHILD_WINDOW)  ||
		(pWnd->GetDlgCtrlID() == IDC_NEXT_WINDOW)   ||
		(pWnd->GetDlgCtrlID() == IDC_PREV_WINDOW)	||
		(pWnd->GetDlgCtrlID() == IDC_OWNER_WINDOW))
	{
		CString strText;
		pWnd->GetWindowText(strText);
		if (0 == strText.CompareNoCase(_T("(None)")))
		{
			return hbr;
		}
		DWORD dwStyle = pWnd->GetStyle();
		if ((dwStyle & 0xFF) <= SS_RIGHT)
		{
			// this is a text control: set up font and colors
			if (!(HFONT)m_font) 
			{
				// first time init: create font
				LOGFONT lf;
				GetFont()->GetObject(sizeof(lf), &lf);
				lf.lfUnderline = TRUE;
				m_font.CreateFontIndirect(&lf);
			}
			// use underline font and visited/unvisited colors
			pDC->SelectObject(&m_font);
			pDC->SetTextColor(RGB(0,0,255));
			pDC->SetBkColor(::GetSysColor(COLOR_3DFACE));
			pDC->SetBkMode(TRANSPARENT);			
			return (HBRUSH)m_pBrush->GetSafeHandle();
		}
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CWindowsPage::OnDestroy() 
{
	CPropertyPage::OnDestroy();	
	if (m_pBrush)
	{
		delete m_pBrush;
		m_pBrush = NULL;
	}
}

void CWindowsPage::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect rectParent, rectChild, rectNext, rectPrev, rectOwner;
	CStatic* pParentWndStatic = (CStatic *)GetDlgItem(IDC_PARENT_WINDOW);
	ASSERT(pParentWndStatic);
	pParentWndStatic->GetWindowRect(&rectParent);

	CStatic* pChildWndStatic  = (CStatic *)GetDlgItem(IDC_CHILD_WINDOW);
	ASSERT(pChildWndStatic);
	pChildWndStatic->GetWindowRect(&rectChild);

	CStatic* pNextWndStatic	  = (CStatic *)GetDlgItem(IDC_NEXT_WINDOW);
	ASSERT(pNextWndStatic);
	pNextWndStatic->GetWindowRect(&rectNext);

	CStatic* pPrevWndStatic   = (CStatic *)GetDlgItem(IDC_PREV_WINDOW);
	ASSERT(pPrevWndStatic);
	pPrevWndStatic->GetWindowRect(&rectPrev);

	CStatic* pOwnerWndStatic  = (CStatic *)GetDlgItem(IDC_OWNER_WINDOW);
	ASSERT(pOwnerWndStatic);
	pOwnerWndStatic->GetWindowRect(&rectOwner);
	
	this->ScreenToClient(&rectParent);
	this->ScreenToClient(&rectChild);
	this->ScreenToClient(&rectNext);
	this->ScreenToClient(&rectPrev);
	this->ScreenToClient(&rectOwner);

	if (rectParent.PtInRect(point) || rectChild.PtInRect(point) ||
		rectNext.PtInRect(point)   || rectPrev.PtInRect(point)  ||	rectOwner.PtInRect(point))
	{
		::SetCursor(AfxGetApp()->LoadCursor(IDC_HAND));
	}
	CPropertyPage::OnMouseMove(nFlags, point);
}

void CWindowsPage::OnDisableWindow() 
{
	if (!(GetWindowThreadProcessId(m_pWindowInfo->m_hwnd, NULL) == GetCurrentThreadId()))
	{
		if (::IsWindowEnabled(m_pWindowInfo->m_hwnd)) 
		{
			::EnableWindow(m_pWindowInfo->m_hwnd, FALSE);
		}
		else
		{
			::EnableWindow(m_pWindowInfo->m_hwnd, TRUE);
		}
	}
}

LRESULT CWindowsPage::OnRefresh(WPARAM wp, LPARAM lp)
{
	updateDialog();
	return 0;
}

void CWindowsPage::updateDialog()
{
	CTabSheet *pSheet = (CTabSheet *)GetParent();
	ASSERT(NULL != pSheet);

	m_pWindowInfo = pSheet->getWindowInfoObj();

	CStatic *pStatic = (CStatic *)GetDlgItem(IDC_INVALID_WINDOW);
	ASSERT(NULL != pStatic);
	if ((NULL == m_pWindowInfo) || !::IsWindow(m_pWindowInfo->m_hwnd))
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

	HWND tmpHWND;

	CStatic* pParentWndStatic	= (CStatic *)GetDlgItem(IDC_PARENT_WINDOW);
	CStatic* pChildWndStatic	= (CStatic *)GetDlgItem(IDC_CHILD_WINDOW);
	CStatic* pNextWndStatic		= (CStatic *)GetDlgItem(IDC_NEXT_WINDOW);
	CStatic* pPrevWndStatic		= (CStatic *)GetDlgItem(IDC_PREV_WINDOW);
	CStatic* pOwnerWndStatic	= (CStatic *)GetDlgItem(IDC_OWNER_WINDOW);
	CButton* pHideButton		= (CButton *)GetDlgItem(IDC_HIDE_WINDOW);
	CButton* pUnHideButton		= (CButton *)GetDlgItem(IDC_UNHIDE_WINDOW);
	
	ASSERT(NULL != pParentWndStatic);
	ASSERT(NULL != pChildWndStatic);
	ASSERT(NULL != pNextWndStatic);
	ASSERT(NULL != pPrevWndStatic);
	ASSERT(NULL != pOwnerWndStatic);
	ASSERT(NULL != pHideButton);
	ASSERT(NULL != pUnHideButton);
	
	tmpHWND = ::GetParent(m_pWindowInfo->m_hwnd);
	pParentWndStatic->SetWindowText(getFormattedString(tmpHWND));
	tmpHWND = NULL;

	tmpHWND = ::GetWindow(m_pWindowInfo->m_hwnd, GW_CHILD);
	pChildWndStatic->SetWindowText(getFormattedString(tmpHWND));
	tmpHWND = NULL;

	tmpHWND = ::GetWindow(m_pWindowInfo->m_hwnd, GW_HWNDNEXT);
	pNextWndStatic->SetWindowText(getFormattedString(tmpHWND));
	tmpHWND = NULL;
	
	tmpHWND = ::GetWindow(m_pWindowInfo->m_hwnd, GW_HWNDPREV);
	pPrevWndStatic->SetWindowText(getFormattedString(tmpHWND));
	tmpHWND = NULL;

	tmpHWND = ::GetWindow(m_pWindowInfo->m_hwnd, GW_OWNER);
	pOwnerWndStatic->SetWindowText(getFormattedString(tmpHWND));
	tmpHWND = NULL;

	CButton *pBtn = (CButton *)GetDlgItem(IDC_DISABLE_WINDOW);
	ASSERT(pBtn);
	if (::IsWindowEnabled(m_pWindowInfo->m_hwnd)) 
	{
		pBtn->SetCheck(0);
	}
	else
	{
		pBtn->SetCheck(1);
	}

	if (::IsWindowVisible(m_pWindowInfo->m_hwnd))
	{
		pHideButton->EnableWindow(TRUE);
		pUnHideButton->EnableWindow(FALSE);
		m_bWindowHide = FALSE;
	}
	else
	{
		pHideButton->EnableWindow(FALSE);
		pUnHideButton->EnableWindow(TRUE);
		m_bWindowHide = TRUE;
	}

	pHideButton->SetCheck(0);
	pUnHideButton->SetCheck(0);
}

void CWindowsPage::OnLButtonDown(UINT nFlags, CPoint point)
{
	UpdateData(TRUE);
	CRect parent, child, next, prev, owner;
	CStatic* pParentWndStatic	= (CStatic *)GetDlgItem(IDC_PARENT_WINDOW);
	CStatic* pChildWndStatic	= (CStatic *)GetDlgItem(IDC_CHILD_WINDOW);
	CStatic* pNextWndStatic		= (CStatic *)GetDlgItem(IDC_NEXT_WINDOW);
	CStatic* pPrevWndStatic		= (CStatic *)GetDlgItem(IDC_PREV_WINDOW);
	CStatic* pOwnerWndStatic	= (CStatic *)GetDlgItem(IDC_OWNER_WINDOW);
	
	ASSERT(pParentWndStatic);
	ASSERT(pChildWndStatic);
	ASSERT(pNextWndStatic);
	ASSERT(pPrevWndStatic);
	ASSERT(pOwnerWndStatic);
	
	pParentWndStatic->GetWindowRect(&parent);
	pChildWndStatic->GetWindowRect(&child);
	pNextWndStatic->GetWindowRect(&next);
	pPrevWndStatic->GetWindowRect(&prev);
	pOwnerWndStatic->GetWindowRect(&owner);

	this->ScreenToClient(&parent);
	this->ScreenToClient(&child);
	this->ScreenToClient(&next);
	this->ScreenToClient(&prev);
	this->ScreenToClient(&owner);

	TCHAR buffer[256];
	CTabSheet *pTabSheet = (CTabSheet *)GetParent();
	ASSERT(NULL != pTabSheet);
	memcpy(buffer, "\0", 255);

	if (parent.PtInRect(point))
	{
		if (pParentWndStatic->GetWindowText(buffer, 255))
		{
			sscanf(buffer, "%x", (int*)&m_pWindowInfo->m_hwnd);
			m_pWindowInfo->setClassName(m_pWindowInfo->m_hwnd);
			m_pWindowInfo->setWindowTitle(m_pWindowInfo->m_hwnd);
			pTabSheet->update(m_pWindowInfo);
			updateDialog();
			CWnd *pWnd = (CWnd *)pTabSheet->m_pParent;
			if (NULL != pWnd)
			{
				TCHAR str[256];
				sprintf(str, "0x%08x", (int)m_pWindowInfo->m_hwnd);
				if (m_ReflectSelection)
				{
					pWnd->SendMessage(WM_FIND_HWND_TEXT,(WPARAM)0, (LPARAM)str);
				}
			}
		}
	}
	else if (child.PtInRect(point))
	{
		if (pChildWndStatic->GetWindowText(buffer, 255))
		{
			sscanf(buffer, "%x", (int*)&m_pWindowInfo->m_hwnd);
			m_pWindowInfo->setClassName(m_pWindowInfo->m_hwnd);
			m_pWindowInfo->setWindowTitle(m_pWindowInfo->m_hwnd);
			pTabSheet->update(m_pWindowInfo);
			updateDialog();
			CWnd *pWnd = (CWnd *)pTabSheet->m_pParent;
			if (NULL != pWnd)
			{
				TCHAR str[256];
				sprintf(str, "0x%08x", (int)m_pWindowInfo->m_hwnd);
				if (m_ReflectSelection)
				{
					pWnd->SendMessage(WM_FIND_HWND_TEXT,(WPARAM)0, (LPARAM)str);
				}
			}
		}
	}
	else if (next.PtInRect(point))
	{
		if (pNextWndStatic->GetWindowText(buffer, 255))
		{
			sscanf(buffer, "%x", (int*)&m_pWindowInfo->m_hwnd);
			m_pWindowInfo->setClassName(m_pWindowInfo->m_hwnd);
			m_pWindowInfo->setWindowTitle(m_pWindowInfo->m_hwnd);
			pTabSheet->update(m_pWindowInfo);
			updateDialog();
			CWnd *pWnd = (CWnd *)pTabSheet->m_pParent;
			if (NULL != pWnd)
			{
				TCHAR str[256];
				sprintf(str, "0x%08x", (int)m_pWindowInfo->m_hwnd);
				if (m_ReflectSelection)
				{
					pWnd->SendMessage(WM_FIND_HWND_TEXT,(WPARAM)0, (LPARAM)str);
				}
			}
		}
	}
	else if (prev.PtInRect(point))
	{
		if (pPrevWndStatic->GetWindowText(buffer, 255))
		{
			sscanf(buffer, "%x", (int*)&m_pWindowInfo->m_hwnd);
			m_pWindowInfo->setClassName(m_pWindowInfo->m_hwnd);
			m_pWindowInfo->setWindowTitle(m_pWindowInfo->m_hwnd);
			pTabSheet->update(m_pWindowInfo);
			updateDialog();
			CWnd *pWnd = (CWnd *)pTabSheet->m_pParent;
			if (NULL != pWnd)
			{
				TCHAR str[256];
				sprintf(str, "0x%08x", (int)m_pWindowInfo->m_hwnd);
				if (m_ReflectSelection)
				{
					pWnd->SendMessage(WM_FIND_HWND_TEXT,(WPARAM)0, (LPARAM)str);
				}
			}
		}
	}
	else if (owner.PtInRect(point))
	{
		if (pOwnerWndStatic->GetWindowText(buffer, 255))
		{
			sscanf(buffer, "%x", (int *)&m_pWindowInfo->m_hwnd);
			m_pWindowInfo->setClassName(m_pWindowInfo->m_hwnd);
			m_pWindowInfo->setWindowTitle(m_pWindowInfo->m_hwnd);
			pTabSheet->update(m_pWindowInfo);
			updateDialog();
			CWnd *pWnd = (CWnd *)pTabSheet->m_pParent;
			if (NULL != pWnd)
			{
				TCHAR str[256];
				sprintf(str, "0x%08x", (int)m_pWindowInfo->m_hwnd);
				if (m_ReflectSelection)
				{
					pWnd->SendMessage(WM_FIND_HWND_TEXT,(WPARAM)0, (LPARAM)str);
				}
			}
		}
	}
	CPropertyPage::OnLButtonDown(nFlags, point);
}


void CWindowsPage::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CRect parent, child, next, prev, owner;
	CStatic* pParentWndStatic	= (CStatic *)GetDlgItem(IDC_PARENT_WINDOW);
	CStatic* pChildWndStatic	= (CStatic *)GetDlgItem(IDC_CHILD_WINDOW);
	CStatic* pNextWndStatic		= (CStatic *)GetDlgItem(IDC_NEXT_WINDOW);
	CStatic* pPrevWndStatic		= (CStatic *)GetDlgItem(IDC_PREV_WINDOW);
	CStatic* pOwnerWndStatic	= (CStatic *)GetDlgItem(IDC_OWNER_WINDOW);
	
	ASSERT(pParentWndStatic);
	ASSERT(pChildWndStatic);
	ASSERT(pNextWndStatic);
	ASSERT(pPrevWndStatic);
	ASSERT(pOwnerWndStatic);
	
	pParentWndStatic->GetWindowRect(&parent);
	pChildWndStatic->GetWindowRect(&child);
	pNextWndStatic->GetWindowRect(&next);
	pPrevWndStatic->GetWindowRect(&prev);
	pOwnerWndStatic->GetWindowRect(&owner);

	this->ScreenToClient(&parent);
	this->ScreenToClient(&child);
	this->ScreenToClient(&next);
	this->ScreenToClient(&prev);
	this->ScreenToClient(&owner);

	if (parent.PtInRect(point))
	{
		::SetCursor(AfxGetApp()->LoadCursor(IDC_HAND));
	}
	if (child.PtInRect(point))
	{
		::SetCursor(AfxGetApp()->LoadCursor(IDC_HAND));
	}
	if (prev.PtInRect(point))
	{
		::SetCursor(AfxGetApp()->LoadCursor(IDC_HAND));
	}
	if (next.PtInRect(point))
	{
		::SetCursor(AfxGetApp()->LoadCursor(IDC_HAND));
	}
	if (owner.PtInRect(point))
	{
		::SetCursor(AfxGetApp()->LoadCursor(IDC_HAND));
	}
	CPropertyPage::OnLButtonUp(nFlags, point);
}

BOOL CWindowsPage::OnSetActive() 
{
	updateDialog();
	return CPropertyPage::OnSetActive();
}
