////////////////////////////////////////////////////////////////
// CTabSheet
// If this code works, it was written by Suresh.

// TabSheet.cpp : implementation file
//

#include "stdafx.h"
#include "TabSheet.h"
#include "InfoView.h"
#include "95ProcessPage.h"
#include "NTProcessPage.h"
#include "ProcessMemInfo.h"
#include "WindowsView.h"
#include "ClassPage.h"
#include "StylesPage.h"
#include "WindowsPage.h"
#include "GeneralPage.h"
#include "95ProcessPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabSheet

IMPLEMENT_DYNAMIC(CTabSheet, CPropertySheet)

CTabSheet::CTabSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage, WindowInfo* pWindowInfo)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage),
	m_pWindowInfo(pWindowInfo), m_pParent(pParentWnd)
{	
}

CTabSheet::CTabSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage, WindowInfo* pWindowInfo)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage),
	m_pWindowInfo(pWindowInfo), m_pParent(pParentWnd)
{	
}

CTabSheet::~CTabSheet()
{
}

BEGIN_MESSAGE_MAP(CTabSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CTabSheet)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_COMMAND(100, OnOK)
	ON_COMMAND(101, OnCancel)
	ON_COMMAND(102, OnRefresh)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabSheet message handlers
WindowInfo* CTabSheet::getWindowInfoObj(void) const
{
	return m_pWindowInfo;
}

void CTabSheet::setWindowInfoObj(WindowInfo* pWndInfo)
{
	ASSERT(pWndInfo);
	m_pWindowInfo = pWndInfo;
}

void CTabSheet::PostNcDestroy() 
{
	CPropertySheet::PostNcDestroy();
}

int CTabSheet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

BOOL CTabSheet::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style &= ~(WS_VISIBLE);
	return CPropertySheet::PreCreateWindow(cs);
}

BOOL CTabSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	CRect rectWnd;
	GetWindowRect(rectWnd);	
	// Increase the height of property sheet and add OK,Cancel and Refresh buttons at the bottom.
	SetWindowPos(NULL, 0, 0, rectWnd.Width(), rectWnd.Height() + 35,
					SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	CRect OKRect;
	OKRect.left = 8;
	OKRect.top  = rectWnd.Height() - 25;
	OKRect.right = 85;
	OKRect.bottom = rectWnd.Height();
	m_OKButton.Create("&OK", WS_VISIBLE | WS_CHILD | WS_TABSTOP, OKRect, this, 100);
	m_OKButton.SetFont(GetFont());

	CRect CancelRect;
	CancelRect.left = 95;
	CancelRect.top  = rectWnd.Height() - 25;
	CancelRect.right = 165;
	CancelRect.bottom = rectWnd.Height();
	m_CancelButton.Create("&Cancel", WS_VISIBLE | WS_CHILD | WS_TABSTOP, CancelRect, this, 101);
	m_CancelButton.SetFont(GetFont());

	CRect RefreshRect;
	RefreshRect.left = 175;
	RefreshRect.top  = rectWnd.Height() - 25;
	RefreshRect.right = 250;
	RefreshRect.bottom = rectWnd.Height();
	m_RefreshButton.Create("&Refresh", WS_VISIBLE | WS_CHILD | WS_TABSTOP, RefreshRect, this, 102);
	m_RefreshButton.SetFont(GetFont());

	CenterWindow();
	return bResult;
}

void CTabSheet::OnOK()
{
	ShowWindow(SW_HIDE);
}

void CTabSheet::OnCancel()
{
	ShowWindow(SW_HIDE);
}

void CTabSheet::OnRefresh()
{
	CPropertyPage* pPage = GetActivePage();
	if (NULL != pPage)
	{
		if (pPage->IsKindOf(RUNTIME_CLASS(C95ProcessPage)))
		{
			((C95ProcessPage*)pPage)->updateDialog();
		}
		else if (pPage->IsKindOf(RUNTIME_CLASS(CClassPage)))
		{
			((CClassPage*)pPage)->updateDialog();
		}
		else if (pPage->IsKindOf(RUNTIME_CLASS(CStylesPage)))
		{
			((CStylesPage*)pPage)->updateDialog();
		}
		else if (pPage->IsKindOf(RUNTIME_CLASS(CWindowsPage)))
		{
			((CWindowsPage*)pPage)->updateDialog();
		}
		else if (pPage->IsKindOf(RUNTIME_CLASS(CGeneralPage)))
		{
			((CGeneralPage*)pPage)->updateDialog();
		}
		else if (pPage->IsKindOf(RUNTIME_CLASS(CNTProcessPage)))
		{
			((CNTProcessPage*)pPage)->updateDialog();
		}
		else if (pPage->IsKindOf(RUNTIME_CLASS(CProcessMemInfo)))
		{
			((CProcessMemInfo*)pPage)->updateDialog();
		}
	}
}

void CTabSheet::update(WindowInfo* pWindowInfo)
{
	ASSERT(pWindowInfo != NULL);
	if (pWindowInfo)
	{
		m_pWindowInfo = pWindowInfo;
		OnRefresh();
	}
}

void CTabSheet::OnDestroy() 
{
	CPropertySheet::OnDestroy();
}

void CTabSheet::OnClose() 
{
	ShowWindow(SW_HIDE);
}
