////////////////////////////////////////////////////////////////
// CGeneralPage
// If this code works, it was written by Suresh.

// GeneralPage.cpp : implementation file
//

#include "stdafx.h"
#include "WindowInfo.h"
#include "GeneralPage.h"
#include "InfoUtil.h"
#include "TabSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGeneralPage property page

IMPLEMENT_DYNCREATE(CGeneralPage, CPropertyPage)

CGeneralPage::CGeneralPage() : 
			CPropertyPage(CGeneralPage::IDD)
{
	//{{AFX_DATA_INIT(CGeneralPage)
	m_WindowCaption = _T("");
	//}}AFX_DATA_INIT
}

CGeneralPage::~CGeneralPage()
{
}

void CGeneralPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeneralPage)
	DDX_Control(pDX, IDC_INVALID_WINDOW, m_InvalidWindow);
	DDX_Text(pDX, IDC_WND_CAPTION, m_WindowCaption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGeneralPage, CPropertyPage)
	//{{AFX_MSG_MAP(CGeneralPage)
	ON_EN_CHANGE(IDC_WND_CAPTION, OnChangeWndCaption)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_REFRESH, OnRefresh)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeneralPage message handlers

BOOL CGeneralPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	m_InvalidWindow.SetFontBold(TRUE);
	m_InvalidWindow.SetTextColor( 706 );	// Red color
	updateDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CGeneralPage::OnChangeWndCaption() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_CHANGE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	CWnd *pWnd = CWnd :: FromHandle(m_pWindowInfo->m_hwnd);
	if (pWnd && IsWindow(m_pWindowInfo->m_hwnd))
	{
		UpdateData(TRUE);
		pWnd->SetWindowText(m_WindowCaption);
		pWnd->Invalidate();
	}
}

BOOL CGeneralPage::PreCreateWindow(CREATESTRUCT& cs) 
{
	return CPropertyPage::PreCreateWindow(cs);
}

LRESULT CGeneralPage::OnRefresh(WPARAM wp, LPARAM lp)
{
	updateDialog();
	return 0;
}

void CGeneralPage::updateDialog()
{
	CTabSheet *pSheet = (CTabSheet *)GetParent();
	ASSERT(NULL != pSheet);

	m_pWindowInfo = pSheet->getWindowInfoObj();

	CStatic *pStatic = (CStatic *)GetDlgItem(IDC_INVALID_WINDOW);
	ASSERT(NULL != pStatic);
	if ((NULL == m_pWindowInfo) || (!::IsWindow(m_pWindowInfo->m_hwnd)))
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

	//AfxGetApp()->m_pMainWnd->LockWindowUpdate();

	CWnd *pWnd = CWnd :: FromHandle(m_pWindowInfo->m_hwnd);
	if (pWnd && IsWindow(m_pWindowInfo->m_hwnd))
	{
		CRect wndRect, clientRect, restoredRect;
		wndRect.SetRectEmpty();
		clientRect.SetRectEmpty();
		restoredRect.SetRectEmpty();

		// Window title
		CEdit *pWindowCaption = (CEdit *)GetDlgItem(IDC_WND_CAPTION);
		ASSERT(pWindowCaption);
		pWindowCaption->SetWindowText(m_pWindowInfo->m_windowTitle);

		// Window rect
		CString wndRectangle;
		pWnd->GetWindowRect(&wndRect);
		wndRectangle.Format( "(%d,%d)-(%d,%d), %dx%d", wndRect.left, wndRect.top, wndRect.right, wndRect.bottom, wndRect.Width(), wndRect.Height());
		CStatic *pWndRect = (CStatic *)GetDlgItem(IDC_WND_RECTANGLE);
		ASSERT(pWndRect );
		wndRectangle.MakeUpper();
		pWndRect->SetWindowText(wndRectangle);

		// Window client rect
		CString wndClientRect;
		pWnd->GetClientRect(&clientRect);
		wndClientRect.Format( "(%d,%d)-(%d,%d), %dx%d", clientRect.left, clientRect.top, clientRect.right, clientRect.bottom, clientRect.Width(), clientRect.Height());
		CStatic *pClientRect = (CStatic *)GetDlgItem(IDC_WND_CLIENT_RECT);
		ASSERT(pClientRect);
		wndClientRect.MakeUpper();
		pClientRect->SetWindowText(wndClientRect);

		// Window Handle
		CString wndHandle;
		wndHandle.Format( "0x%08x", m_pWindowInfo->m_hwnd);
		CEdit *pWindowHandle = (CEdit *)GetDlgItem(IDC_WND_HANDLE);
		ASSERT(pWindowHandle);
		wndHandle.MakeUpper();
		pWindowHandle->SetWindowText(wndHandle);

		// restored position
		CString wndRestoredRect;
		WINDOWPLACEMENT wndPlacement;
		pWnd->GetWindowPlacement(&wndPlacement);
		restoredRect = wndPlacement.rcNormalPosition;
		wndRestoredRect.Format( "(%d,%d)-(%d,%d), %dx%d", restoredRect.left, restoredRect.top, restoredRect.right, restoredRect.bottom, restoredRect.Width(), restoredRect.Height());
		CStatic *pRestoredRect = (CStatic *)GetDlgItem(IDC_WND_RESTORED_RECT);
		ASSERT(pRestoredRect);
		wndRestoredRect.MakeUpper();
		pRestoredRect->SetWindowText(wndRestoredRect);

		// user data
		long userData = ::GetWindowLong(m_pWindowInfo->m_hwnd, GWL_USERDATA);
		CString strUserData;
		strUserData.Format("0x%08x", userData);
		CStatic *pUserData = (CStatic *)GetDlgItem(IDC_USER_DATA);
		ASSERT(pUserData);
		strUserData.MakeUpper();
		pUserData->SetWindowText(strUserData);

		// instance Handle
		long instanceHandle = ::GetWindowLong(m_pWindowInfo->m_hwnd, GWL_HINSTANCE);
		CString strInstanceHandle;
		strInstanceHandle.Format("0x%08x", instanceHandle);
		CStatic *pInstanceHandle = (CStatic *)GetDlgItem(IDC_INSTANCE_HANDLE);
		ASSERT(pInstanceHandle);
		strInstanceHandle.MakeUpper();
		pInstanceHandle->SetWindowText(strInstanceHandle);

		// window proc
		long windowProc = ::GetWindowLong(m_pWindowInfo->m_hwnd, GWL_WNDPROC);
		CString strWindowProc;
		strWindowProc.Format("0x%08x", windowProc);
		CStatic *pwndProc = (CStatic *)GetDlgItem(IDC_WND_PROC);
		ASSERT(pwndProc);
		strWindowProc.MakeUpper();
		pwndProc->SetWindowText(strWindowProc);
		
		// control ID
		if ( pWnd->GetStyle() & WS_CHILD)
		{
			long controlID =  ::GetWindowLong(m_pWindowInfo->m_hwnd, GWL_ID);
			CString strID;
			strID.Format("0x%08x", controlID);
			CStatic *pcontrolID = (CStatic *)GetDlgItem(IDC_CONTROL_ID);
			ASSERT(pcontrolID);
			strID.MakeUpper();
			pcontrolID->SetWindowText(strID);
		}
		else
		{
			CStatic *pcontrolID = (CStatic *)GetDlgItem(IDC_CONTROL_ID);
			ASSERT(pcontrolID);
			
			CStatic *pcontrolIDSatic = (CStatic *)GetDlgItem(IDC_STATIC_CONTROL_ID);
			ASSERT(pcontrolIDSatic);
			pcontrolIDSatic->ShowWindow(SW_HIDE);
		}
	}
	//AfxGetApp()->m_pMainWnd->UnlockWindowUpdate();
}

BOOL CGeneralPage::OnSetActive() 
{
	updateDialog();	
	return CPropertyPage::OnSetActive();
}