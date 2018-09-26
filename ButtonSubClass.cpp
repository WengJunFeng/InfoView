////////////////////////////////////////////////////////////////
// CButtonSubClass
// If this code works, it was written by Suresh.

// ButtonSubClass.cpp : implementation file
//

#include "stdafx.h"
#include "InfoView.h"
#include "ButtonSubClass.h"
#include "AboutInfoViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonSubClass

CButtonSubClass::CButtonSubClass()
{
}

CButtonSubClass::~CButtonSubClass()
{
}


BEGIN_MESSAGE_MAP(CButtonSubClass, CButton)
	//{{AFX_MSG_MAP(CButtonSubClass)
	ON_WM_MOUSEMOVE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonSubClass message handlers

void CButtonSubClass::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect goodRect, badRect;
	badRect.SetRectEmpty();
	goodRect.SetRectEmpty();

	CAboutInfoViewer *pDialog = (CAboutInfoViewer *)GetParent();
	ASSERT(pDialog);
	
	CButton *pGood = (CButton *)pDialog->GetDlgItem(IDOK);
	ASSERT(pGood);
	
	CButton *pBad = (CButton *)pDialog->GetDlgItem(ID_VERY_BAD);
	ASSERT(pBad);
	
	pGood->GetWindowRect(&goodRect);
	pBad ->GetWindowRect(&badRect);

	pDialog->ScreenToClient(badRect);
	pDialog->ScreenToClient(goodRect);

	pGood->MoveWindow(badRect);
	pBad ->MoveWindow(goodRect);
	
	CButton::OnMouseMove(nFlags, point);
}

void CButtonSubClass::OnSetFocus(CWnd* pOldWnd) 
{
	CButton::OnSetFocus(pOldWnd);
	
	CAboutInfoViewer *pDialog = (CAboutInfoViewer *)GetParent();
	ASSERT(pDialog);

	CButton *pGood = (CButton *)pDialog->GetDlgItem(IDOK);
	ASSERT(pGood);
	
	pGood->SetFocus();
}
