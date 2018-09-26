////////////////////////////////////////////////////////////////
// CAboutInfoViewer
// If this code works, it was written by Suresh.

// AboutInfoViewer.cpp : implementation file
//

#include "stdafx.h"
#include "InfoView.h"
#include "AboutInfoViewer.h"
#include "InfoUtil.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include ".\aboutinfoviewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAboutInfoViewer dialog

CAboutInfoViewer::CAboutInfoViewer(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAboutInfoViewer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAboutInfoViewer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_OKPressed = 0;
	m_timer		= 0;
	m_Red		= 0;
	m_Green		= 0;
	m_Blue		= 0;
	m_CurrentColor = 0;
}


void CAboutInfoViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutInfoViewer)
	DDX_Control(pDX, IDC_EMAIL_ID, m_EMailID);
	DDX_Control(pDX, IDC_HOMEPAGE_URL, m_HomePageURL);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutInfoViewer, CDialog)
	//{{AFX_MSG_MAP(CAboutInfoViewer)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_HOMEPAGE_URL, OnStnClickedHomepageUrl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutInfoViewer message handlers

void CAboutInfoViewer::OnOK() 
{
	m_OKPressed++;
	// TODO: Add extra validation here
	if (m_OKPressed == 1)
	{
		HideAllControls(this);
		CStatic *pStatic = (CStatic *)GetDlgItem(IDC_AUTHOR);
		ASSERT(pStatic);
		pStatic->ShowWindow(SW_SHOW);

		CStatic *pFeedBack = (CStatic *)GetDlgItem(ID_VERY_BAD);
		ASSERT(pFeedBack);
		pFeedBack->ShowWindow(SW_SHOW);

		CStatic *pStatic1 = (CStatic *)GetDlgItem(IDOK);
		ASSERT(pStatic1);
		pStatic1->ShowWindow(SW_SHOW);

		CButton *pOK = (CButton *)GetDlgItem(IDOK);
		ASSERT(pOK);
		pOK->SetWindowText(_T("Thank You !"));

		CButton *pBad = (CButton *)GetDlgItem(ID_VERY_BAD);
		ASSERT(pBad);
		pBad->SetWindowText(_T("No Thanks !"));
	}
	if (m_OKPressed > 1)
	{
		CDialog::OnOK();
	}
}

void CAboutInfoViewer::OnCancel()
{
	// do not call base class OnCancel();
}

BOOL CAboutInfoViewer::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	m_HomePageURL.SetLink(TRUE);
	m_HomePageURL.SetTextColor(RGB(0,0,255));
	m_HomePageURL.SetFontUnderline(TRUE);
	m_HomePageURL.SetLinkCursor(AfxGetApp()->LoadCursor(IDC_HAND));

	m_EMailID.SetLink(TRUE);
	m_EMailID.SetTextColor(RGB(0,0,255));
	m_EMailID.SetFontUnderline(TRUE);
	m_EMailID.SetLinkCursor(AfxGetApp()->LoadCursor(IDC_HAND));

	m_BadButton.SubclassDlgItem(ID_VERY_BAD, this);
	m_timer = SetTimer(ID_INFOVIEW_TIMER_ID, INFOVIEW_TIMER_INTERVAL, NULL);

	ClearHoles();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutInfoViewer::OnTimer(UINT nIDEvent) 
{
	static CRect dlgRect, staticRect;
	static int staticWidth;

	CStatic* pStatic3 = (CStatic *)GetDlgItem(IDC_STATIC3);
	ASSERT(NULL != pStatic3);
	pStatic3->GetWindowRect(&dlgRect);

	CStatic* pStatic = (CStatic *)GetDlgItem(IDC_HOMEPAGE_URL);
	ASSERT(pStatic);
	pStatic->GetWindowRect(&staticRect);
	staticWidth = staticRect.Width();

	this->ScreenToClient(&dlgRect);
	this->ScreenToClient(&staticRect);

	staticRect.left  -= LEFT_MOTION_INTERVAL;
	staticRect.right -= LEFT_MOTION_INTERVAL;

	if (staticRect.right <= (dlgRect.left + 25))
	{
		staticRect.left  = dlgRect.right - 25;
		staticRect.right = staticRect.left + staticWidth;
	}

	pStatic->MoveWindow(staticRect, TRUE);

	switch( m_CurrentColor )
	{
	case 0:
		m_Red += 5;
		break;
	case 1:
		m_Green += 5;
		break;
	case 2:
		m_Blue += 5;
		break;
	}
/*
	CRgn	region;
	CRect	authorRect;
	CBrush	brush(RGB(m_Red, m_Green, m_Blue));

	CStatic *pStaticAuthor = (CStatic *)GetDlgItem(IDC_AUTHOR);
	ASSERT(pStaticAuthor);
	
	pStaticAuthor->GetWindowRect(&authorRect);
	pStaticAuthor->ScreenToClient(&authorRect);

	CDC *pDc = pStaticAuthor->GetDC();
	region.CreateRectRgn(authorRect.left, authorRect.top, authorRect.right, authorRect.bottom);
	pDc->FrameRgn(&region, &brush, 2,2);
	pStatic->ReleaseDC(pDc);
*/
	if ( m_Red >= 255 ) {
		m_CurrentColor = 1;
		m_Red = 0;
	}
	if ( m_Green >= 255 ) {
		m_CurrentColor = 2;
		m_Green = 0;
	}
	if ( m_Blue >= 255 ) {
		m_CurrentColor = 0;
		m_Blue = 0;
	}

	if ( m_OKPressed == 1 )
	{
		int nRandNum1 = rand() % 500;	// left
		int nRandNum2 = rand() % 500;	// top

		CPoint pt;
		pt.x = nRandNum1;
		pt.y = nRandNum2;

		CRect rect;
		CRgn rgn;

		this->GetWindowRect(&rect);
		this->ScreenToClient(&rect);

		CRect thanksRect, noThanksRect, authorRect;

		CButton *pOK = (CButton *)GetDlgItem(IDOK);			// Thanks Button
		ASSERT(pOK);
		pOK->GetWindowRect(&thanksRect);
		this->ScreenToClient(&thanksRect);

		CButton *pBad = (CButton *)GetDlgItem(ID_VERY_BAD);	// No Thanks Button
		ASSERT(pBad);
		pBad->GetWindowRect(&noThanksRect);
		this->ScreenToClient(&noThanksRect);

		CStatic *pAuthor = (CStatic *)GetDlgItem(IDC_AUTHOR);
		ASSERT(pAuthor);
		pAuthor->GetWindowRect(&authorRect);
		this->ScreenToClient(&authorRect);

		if ( rect.PtInRect(pt) )
		{
			if ((thanksRect.PtInRect(pt)) || (noThanksRect.PtInRect(pt)) || (authorRect.PtInRect(pt)))
			{
				// Do not paint on Buttons & Image
			}
			else
			{
				rgn.CreateRoundRectRgn(nRandNum1, nRandNum2, nRandNum1 + 5, nRandNum2 + 5, 10, 10);

				CDC *pDc = this->GetDC();
				CBrush	brush(RGB(m_Red, m_Green, m_Blue));
				pDc->FillRgn(&rgn, &brush);
				this->ReleaseDC(pDc);
			}
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CAboutInfoViewer::OnDestroy() 
{
	CDialogEx::OnDestroy();
	
	if (m_timer != 0)
		KillTimer(m_timer);	
}

void CAboutInfoViewer::OnStnClickedHomepageUrl()
{
	// TODO: Add your control notification handler code here
}

void CAboutInfoViewer::ClearHoles()
{
    CRect WindowRect;
    CRgn WindowRgn;
    GetWindowRect (WindowRect);
    WindowRgn.CreateRectRgn (0,0,WindowRect.Width (), WindowRect.Height());
    // Get back to the classic border
    SetWindowRgn ((HRGN__*)WindowRgn.m_hObject, TRUE);
}

void CAboutInfoViewer::CreateHole(CRgn& rgn)
{
    CRgn WindowRgn;
    CRgn HoleRgn;
    static CRgn ThisRgn;

    CRect WindowRect;
    static bool Start = true;
    GetWindowRect (WindowRect);   // Get the Window rect
    WindowRgn.CreateRectRgn (0,0,WindowRect.Width (), WindowRect.Height());

    // initialize the hole region
    HoleRgn.CreateRectRgn (0,0,0,0);
 
    if (Start)
    {
       // initialize ThisRgn
       ThisRgn.CreateRectRgn (0,0,0,0);
       // First copy the region himself in ThisRgn
       ThisRgn.CopyRgn (&rgn);
    }
    else
    {
       // Other times, Add the new region to ThisRgn
       ThisRgn.CombineRgn (&ThisRgn, &rgn, RGN_OR);
    }
 
    Start = false;
    // create the border of the hole(s) with ThisRgn and set it in that
    // border by reversing it
    HoleRgn.CombineRgn (&ThisRgn, &WindowRgn, RGN_XOR);
    SetWindowRgn ((HRGN__*)HoleRgn.m_hObject, TRUE);
}

BOOL CAboutInfoViewer::PreCreateWindow(CREATESTRUCT& cs)
{
	return CDialogEx::PreCreateWindow(cs);
}