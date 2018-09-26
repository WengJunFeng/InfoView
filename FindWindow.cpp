////////////////////////////////////////////////////////////////
// CFindWindow
// If this code works, it was written by Suresh.

// FindWindow.cpp : implementation file
//

#include "stdafx.h"
#include "InfoView.h"
#include "FindWindow.h"
#include "MainFrm.h"
#include ".\findwindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFindWindow dialog


CFindWindow::CFindWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFindWindow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindWindow)
	m_GivenHwnd = _T("");
	//}}AFX_DATA_INIT
	m_selectedHwnd = NULL;
	m_bWndHide	   = FALSE;
	m_bFindWindow  = FALSE;
	m_hwndLastSubject = NULL;
}


void CFindWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindWindow)
	DDX_Text(pDX, IDC_HWND, m_GivenHwnd);
	DDX_Control(pDX, IDC_HWND, m_Edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFindWindow, CDialog)
	//{{AFX_MSG_MAP(CFindWindow)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_HIDE_SPY, OnHideSpy)
	ON_WM_DESTROY()
	ON_WM_CAPTURECHANGED()
	ON_EN_CHANGE(IDC_HWND, OnChangeHwnd)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindWindow message handlers

void CFindWindow::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rect;
	CStatic *pStatic = (CStatic *)GetDlgItem(IDC_FRAME_ICON);
	ASSERT(pStatic);
	pStatic->GetWindowRect(&rect);
	this->ScreenToClient(&rect);
	if (rect.PtInRect(point))
	{
		SetCapture();
		((CStatic *)GetDlgItem(IDC_FRAME_ICON))->SetIcon(AfxGetApp()->LoadIcon(IDI_FRAME));
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CIRCLE));
		m_bFindWindow = TRUE;

		// Set the window handle of the last viewed window to NULL.
	 	m_hwndLastSubject = NULL;

	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CFindWindow::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (TRUE == m_bFindWindow)
	{
		m_bFindWindow = FALSE;
		CPoint currentMousePosition;
		
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));

		GetCursorPos(&currentMousePosition);
		HWND hwndSubject = SmallestWindowFromPoint(currentMousePosition);
		CWnd *pWnd = FromHandle(hwndSubject);
		ASSERT(pWnd);

		if (GetWindowThreadProcessId(pWnd->m_hWnd, NULL) == GetCurrentThreadId())
		{
			m_selectedHwnd = NULL;
			((CStatic *)GetDlgItem(IDC_FRAME_ICON))->SetIcon(AfxGetApp()->LoadIcon(IDI_FRAME_CIRCLE));
		}
		else
		{
			m_selectedHwnd = hwndSubject;
			CDialog::OnLButtonUp(nFlags, point);

			// close the dialog after selecting the window
			CDialog::OnOK();
		}
	}
	ReleaseCapture();
	CDialog::OnLButtonUp(nFlags, point);
}

HWND CFindWindow::getSelectedWindow()
{
	return m_selectedHwnd;
}

void CFindWindow::OnMouseMove(UINT nFlags, CPoint point) 
{
	HWND hwndSubject = NULL;
	CPoint currentMousePosition;

	GetCursorPos(&currentMousePosition);
	hwndSubject = SmallestWindowFromPoint(currentMousePosition);
	CWnd *pCurrentWnd = FromHandle(hwndSubject);
	ASSERT(pCurrentWnd);

   // If the window is created by us, ignore it.
   if (GetWindowThreadProcessId(hwndSubject, NULL) == GetCurrentThreadId())
      return;

   if (m_hwndLastSubject == hwndSubject)
      return;

	// Remove the frame, if any, around the currently selected window.
	if (m_hwndLastSubject != NULL)
	{
		DrawWindowFrame(m_hwndLastSubject);
	}

	// draw frame
	DrawWindowFrame (pCurrentWnd->GetSafeHwnd());

	CEdit *pEdit = (CEdit *) GetDlgItem(IDC_CLASS_NAME);
	ASSERT(pEdit);

	TCHAR strClassName[512 + 1];
	::GetClassName(hwndSubject, strClassName, 512);
	pEdit->SetWindowText(strClassName);

	m_GivenHwnd.Format("%08x", hwndSubject);

	// Save the handle to the most recent subject window.
	m_hwndLastSubject = hwndSubject;

	// show the HWND in HWND edit control.
	UpdateData(FALSE);

	CDialog::OnMouseMove(nFlags, point);
}

void CFindWindow::OnHideSpy() 
{
	CMainFrame *pMainWnd = (CMainFrame *)AfxGetMainWnd();
	ASSERT(pMainWnd);

	if(!m_bWndHide)
	{
		pMainWnd->ShowWindow(SW_HIDE);
		m_bWndHide = TRUE;
	}
	else
	{
		pMainWnd->ShowWindow(SW_SHOW);
		m_bWndHide = FALSE;
	}
}

void CFindWindow::OnDestroy() 
{
	CDialogEx::OnDestroy();
	CMainFrame *pMainWnd = (CMainFrame *)AfxGetMainWnd();
	ASSERT(pMainWnd);
	pMainWnd->ShowWindow(SW_SHOW);	
}

void CFindWindow::DrawWindowFrame (HWND hwndSubject)
{
   HDC hdc;
   RECT rc;
   HPEN hpen;
   
   // Get the window rect of given window
   ::GetWindowRect(hwndSubject, &rc);

   // Get a DC of selected window
   hdc = ::GetWindowDC(hwndSubject);

   // Save the original device context attributes.
   SaveDC(hdc);
   
   // To guarantee that the frame will be visible, tell Windows to draw the
   // frame using the inverse screen color.
   SetROP2(hdc, R2_NOT);

   // Create a pen that is three times the width of a nonsizeable border
   hpen = CreatePen(PS_INSIDEFRAME, 3 * GetSystemMetrics(SM_CXBORDER),
      RGB(255, 0, 0));
   SelectObject(hdc, hpen);

   // We must select a NULL brush so that the contents of the window 
   // will not be overwritten.
   SelectObject(hdc, GetStockObject(NULL_BRUSH));

   // Draw the frame. The DC is relative to the window, the top-left corner is (0, 0)
   // and the lower, right corner is width and height.
   Rectangle(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top);

   // Restore the original attributes
   RestoreDC(hdc, -1);

   // release DC
   ::ReleaseDC(hwndSubject, hdc);

   DeleteObject(hpen);
}

void CFindWindow::OnCaptureChanged(CWnd *pWnd) 
{
	if (m_hwndLastSubject != NULL) 
	{
		DrawWindowFrame(m_hwndLastSubject);

		// Force our window to appear on top of all other windows.
		BringWindowToTop();
	}
	m_hwndLastSubject = NULL;
	
	CDialog::OnCaptureChanged(pWnd);
}

void CFindWindow::OnOK() 
{
	UpdateData(TRUE);
	if (!m_GivenHwnd.IsEmpty())
	{
		sscanf( m_GivenHwnd, "%08x", (int *)&m_selectedHwnd);
	}
	else
	{
		m_selectedHwnd = NULL;
	}
	CDialog::OnOK();
}

BOOL CFindWindow::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_HWND);
	ASSERT(pEdit);
	CButton* pHideSpyBtn = (CButton *)GetDlgItem(IDC_HIDE_SPY);
	ASSERT(pHideSpyBtn);

	// create the tooltip control with a pointer from the tooltip control's parent
	m_ToolTips.Create(this);
	m_ToolTips.AddTool(pEdit, "Enter HWND of window or select HWND using finder tool");
	m_ToolTips.AddTool(pHideSpyBtn, "Click this to hide the InfoView main window");

	CButton *pButton = (CButton *)GetDlgItem(IDOK);
	ASSERT(NULL != pButton);
	pButton->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CFindWindow::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)		
	{			
		case WM_LBUTTONDOWN:					
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:				
			m_ToolTips.RelayEvent(pMsg);		
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CFindWindow::OnChangeHwnd() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_CHANGE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	m_GivenHwnd.TrimLeft();
	m_GivenHwnd.TrimRight();

	CButton *pButton = (CButton *)GetDlgItem(IDOK);
	ASSERT(NULL != pButton);
	if (!m_GivenHwnd.GetLength())
	{
		pButton->EnableWindow(FALSE);
	}
	else
	{
		pButton->EnableWindow(TRUE);
	}
}

void CFindWindow::OnBnClickedOk()
{
	this->OnOK();
}

HWND CFindWindow::SmallestWindowFromPoint(const POINT &point)
{
	// Find the smallest "window" still containing the point
	// Doing this prevents us from stopping at the first window containing the point
	RECT rect, rectSearch;
	HWND hParentWnd, hWnd, hSearchWnd;

	hWnd = ::WindowFromPoint(point);
	if(hWnd != NULL)
	{
		// Get the size and parent for compare later
		::GetWindowRect(hWnd, &rect);
		hParentWnd = ::GetParent(hWnd);

		// We only search further if the window has a parent
		if(hParentWnd != NULL)
		{
			// Search from the window down in the Z-Order
			hSearchWnd = hWnd;
			do{
				hSearchWnd = ::GetWindow(hSearchWnd, GW_HWNDNEXT);

				// Does the search window also contain the point, have the same parent, and is visible?
				::GetWindowRect(hSearchWnd, &rectSearch);
				if(::PtInRect(&rectSearch, point) && ::GetParent(hSearchWnd) == hParentWnd && ::IsWindowVisible(hSearchWnd))
				{
					// It does, but is it smaller?
					if(((rectSearch.right - rectSearch.left) * (rectSearch.bottom - rectSearch.top)) < ((rect.right - rect.left) * (rect.bottom - rect.top)))
					{
						// Found new smaller window, update compare window
						hWnd = hSearchWnd;
						::GetWindowRect(hWnd, &rect);
					}
				}
			}while(hSearchWnd != NULL);
		}
	}

	return hWnd;
}

BOOL CFindWindow::PreCreateWindow(CREATESTRUCT& cs)
{
	return CDialogEx::PreCreateWindow(cs);
}