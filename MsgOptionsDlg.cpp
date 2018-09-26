// MsgOptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "infoview.h"
#include "MsgOptionsDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgOptionsDlg dialog


CMsgOptionsDlg::CMsgOptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgOptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMsgOptionsDlg)
	m_Animate = TRUE;
	m_Button = TRUE;
	m_ComboBox = TRUE;
	m_DDE = TRUE;
	m_DlgBox = TRUE;
	m_Edit = TRUE;
	m_Header = TRUE;
	m_Hotkey = TRUE;
	m_ListBox = TRUE;
	m_ListView = TRUE;
	m_Progress = TRUE;
	m_ScrollBar = TRUE;
	m_Spin = TRUE;
	m_StatusBar = TRUE;
	m_TabCtrl = TRUE;
	m_ToolTip = TRUE;
	m_TrackBar = TRUE;
	m_TreeView = TRUE;
	m_UnKnown = TRUE;
	m_User = TRUE;
	m_SelectAll = FALSE;
	m_MouseMove = FALSE;
	m_GivenHwnd = _T("");
	m_OnlyChild = TRUE;
	m_ClassName = _T("");
	m_OnlyParent = FALSE;
	m_ProcessId = _T("");
	m_OnlySameProcess = FALSE;
	m_OnlySameThread = FALSE;
	m_ThreadId = _T("");
	m_ToolBar = FALSE;
	m_WholeSystem = FALSE;
	m_MaxLines = 750;
	m_Keyboard = FALSE;
	//}}AFX_DATA_INIT
	m_selectedHwnd = NULL;
	m_bWndHide	   = FALSE;
	m_bFindWindow  = FALSE;
	m_hwndLastSubject = NULL;
}


void CMsgOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgOptionsDlg)
	DDX_Check(pDX, IDC_ANIMATE_MSG, m_Animate);
	DDX_Check(pDX, IDC_BUTTON_MSG, m_Button);
	DDX_Check(pDX, IDC_COMBOBOX_MSG, m_ComboBox);
	DDX_Check(pDX, IDC_DDE_MSG, m_DDE);
	DDX_Check(pDX, IDC_DIALOGBOX_MSG, m_DlgBox);
	DDX_Check(pDX, IDC_EDIT_MSG, m_Edit);
	DDX_Check(pDX, IDC_HEADER_MSG, m_Header);
	DDX_Check(pDX, IDC_HOTKEY_MSG, m_Hotkey);
	DDX_Check(pDX, IDC_LISTBOX_MSG, m_ListBox);
	DDX_Check(pDX, IDC_LISTVIEW_MSG, m_ListView);
	DDX_Check(pDX, IDC_PROGRESS_MSG, m_Progress);
	DDX_Check(pDX, IDC_SCROLLBAR_MSG, m_ScrollBar);
	DDX_Check(pDX, IDC_SPIN_MSG, m_Spin);
	DDX_Check(pDX, IDC_STATUSBAR_MSG, m_StatusBar);
	DDX_Check(pDX, IDC_TABCTRL_MSG, m_TabCtrl);
	DDX_Check(pDX, IDC_TOOLTIP_MSG, m_ToolTip);
	DDX_Check(pDX, IDC_TRACKBAR_MSG, m_TrackBar);
	DDX_Check(pDX, IDC_TREEVIEW_MSG, m_TreeView);
	DDX_Check(pDX, IDC_UNKNOWN_MSG, m_UnKnown);
	DDX_Check(pDX, IDC_USER_MSG, m_User);
	DDX_Check(pDX, IDC_SELECT_ALL, m_SelectAll);
	DDX_Check(pDX, IDC_MOUSEMOVE_MSG, m_MouseMove);
	DDX_Text(pDX, IDC_HWND, m_GivenHwnd);
	DDX_Check(pDX, IDC_CHILD, m_OnlyChild);
	DDX_Text(pDX, IDC_CLASSNAME, m_ClassName);
	DDX_Check(pDX, IDC_PARENT, m_OnlyParent);
	DDX_Text(pDX, IDC_PROCESSID, m_ProcessId);
	DDX_Check(pDX, IDC_SAMEPROCESS, m_OnlySameProcess);
	DDX_Check(pDX, IDC_SAMETHREAD, m_OnlySameThread);
	DDX_Text(pDX, IDC_THREADID, m_ThreadId);
	DDX_Check(pDX, IDC_TOOLBAR_MSG, m_ToolBar);
	DDX_Check(pDX, IDC_WHOLE_SYSTEM, m_WholeSystem);
	DDX_Text(pDX, IDC_MAX_LINES, m_MaxLines);
	DDV_MinMaxInt(pDX, m_MaxLines, 1, 9999);
	DDX_Check(pDX, IDC_KEYBOARD_MSG, m_Keyboard);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMsgOptionsDlg, CDialog)
	//{{AFX_MSG_MAP(CMsgOptionsDlg)
	ON_BN_CLICKED(IDC_SELECT_ALL, OnSelectAll)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_HIDE_SPY, OnHideSpy)
	ON_WM_DESTROY()
	ON_WM_CAPTURECHANGED()
	ON_BN_CLICKED(IDC_SAMEPROCESS, OnSameprocess)
	ON_BN_CLICKED(IDC_SAMETHREAD, OnSamethread)
	ON_BN_CLICKED(IDC_WHOLE_SYSTEM, OnWholeSystem)
	ON_BN_CLICKED(IDC_PARENT, OnParent)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_BUTTON_MSG, IDC_MOUSEMOVE_MSG, OnSelection)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgOptionsDlg message handlers

void CMsgOptionsDlg::OnSelectAll()
{
	m_SelectAll = !m_SelectAll;

	m_Animate = m_SelectAll;
	m_Button = m_SelectAll;
	m_ComboBox = m_SelectAll;
	m_DDE = m_SelectAll;
	m_DlgBox = m_SelectAll;
	m_Edit = m_SelectAll;
	m_Header = m_SelectAll;
	m_Hotkey = m_SelectAll;
	m_ListBox = m_SelectAll;
	m_ListView = m_SelectAll;
	m_Progress = m_SelectAll;
	m_ScrollBar = m_SelectAll;
	m_Spin = m_SelectAll;
	m_StatusBar = m_SelectAll;
	m_TabCtrl = m_SelectAll;
	m_ToolTip = m_SelectAll;
	m_TrackBar = m_SelectAll;
	m_TreeView = m_SelectAll;
	m_UnKnown = m_SelectAll;
	m_User = m_SelectAll;
	m_MouseMove = m_SelectAll;
	m_ToolBar = m_SelectAll;
	m_Keyboard = m_SelectAll;
	UpdateData(FALSE);
}


void CMsgOptionsDlg::OnLButtonDown(UINT nFlags, CPoint point) 
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

void CMsgOptionsDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (TRUE == m_bFindWindow)
	{
		m_bFindWindow = FALSE;
		CPoint currentMousePosition;
		
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
		ReleaseCapture();

		GetCursorPos(&currentMousePosition);
		CWnd *pWnd = WindowFromPoint(currentMousePosition);
		ASSERT(pWnd);

		if (GetWindowThreadProcessId(pWnd->m_hWnd, NULL) == GetCurrentThreadId())
		{
			m_selectedHwnd = NULL;
			((CStatic *)GetDlgItem(IDC_FRAME_ICON))->SetIcon(AfxGetApp()->LoadIcon(IDI_FRAME_CIRCLE));
		}
		else
		{
			m_selectedHwnd = pWnd->GetSafeHwnd();
			((CStatic *)GetDlgItem(IDC_FRAME_ICON))->SetIcon(AfxGetApp()->LoadIcon(IDI_FRAME_CIRCLE));
			CDialog::OnLButtonUp(nFlags, point);
			// close the dialog after selecting the window
		}
	}
	CDialog::OnLButtonUp(nFlags, point);
}

void CMsgOptionsDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (TRUE == m_bFindWindow)
	{
		// show the HWND in HWND edit control.
		UpdateData(TRUE);

		HWND hwndSubject;
		CPoint currentMousePosition;

		GetCursorPos(&currentMousePosition);
		CWnd *pCurrentWnd = WindowFromPoint(currentMousePosition);
		ASSERT(pCurrentWnd);

		hwndSubject = pCurrentWnd->GetSafeHwnd();

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

		// show class name and HWND of selected window
		::GetClassName(hwndSubject, m_ClassName.GetBuffer(512), 512);
		m_ClassName.ReleaseBuffer();
		m_GivenHwnd.Format("%08x", hwndSubject);

		// show PID and TID of selected window
		DWORD processId = 0;
		DWORD threadId = 0;
		threadId = GetWindowThreadProcessId(hwndSubject, &processId);

		m_ThreadId.Format("%08x", threadId);
		m_ProcessId.Format("%08x", processId);

		m_ThreadId.MakeUpper();
		m_ProcessId.MakeUpper();

		// show the HWND in HWND edit control.
		UpdateData(FALSE);

		// Save the handle to the most recent subject window.
		m_hwndLastSubject = hwndSubject;

		// Save the handle to the most recent subject window.
		m_hwndLastSubject = hwndSubject;
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CMsgOptionsDlg::OnHideSpy()
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

void CMsgOptionsDlg::OnDestroy() 
{
	CMainFrame *pMainWnd = (CMainFrame *)AfxGetMainWnd();
	ASSERT(pMainWnd);
	pMainWnd->ShowWindow(SW_SHOW);
	CDialog::OnDestroy();
}

void CMsgOptionsDlg::OnCaptureChanged(CWnd *pWnd) 
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

void CMsgOptionsDlg::DrawWindowFrame (HWND hwndSubject)
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

HWND CMsgOptionsDlg::getSelectedWindow()
{
	return m_selectedHwnd;
}

void CMsgOptionsDlg::OnSelection(UINT nID)
{
	UpdateData(TRUE);
	switch(nID)
	{
	case IDC_BUTTON_MSG:
		m_Button = ! m_Button;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "BtnMsg",  m_Button);
		break;
	case IDC_LISTBOX_MSG:
		m_ListBox = ! m_ListBox;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "ListBoxMsg",  m_ListBox);
		break;
	case IDC_SCROLLBAR_MSG:
		m_ScrollBar = ! m_ScrollBar;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "ScrollBarMsg",  m_ScrollBar);
		break;
	case IDC_COMBOBOX_MSG:
		m_ComboBox = ! m_ComboBox;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "ComboBoxMsg",  m_ComboBox);
		break;
	case IDC_EDIT_MSG:
		m_Edit = ! m_Edit;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "EditMsg",  m_Edit);
		break;
	case IDC_TREEVIEW_MSG:
		m_TreeView = ! m_TreeView;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "TreeViewMsg",  m_TreeView);
		break;
	case IDC_LISTVIEW_MSG:
		m_ListView = ! m_ListView;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "ListViewMsg",  m_ListView);
		break;
	case IDC_DIALOGBOX_MSG:
		m_DlgBox = ! m_DlgBox;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "DlgBoxMsg",  m_DlgBox);
		break;
	case IDC_TOOLTIP_MSG:
		m_ToolTip = ! m_ToolTip;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "ToolTipMsg",  m_ToolTip);
		break;
	case IDC_SPIN_MSG:
		m_Spin = ! m_Spin;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "SpinMsg",  m_Spin);
		break;
	case IDC_TABCTRL_MSG:
		m_TabCtrl = ! m_TabCtrl;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "TabCtrlMsg",  m_TabCtrl);
		break;
	case IDC_TRACKBAR_MSG:
		m_TrackBar = ! m_TrackBar;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "TrackBarMsg",  m_TrackBar);
		break;
	case IDC_STATUSBAR_MSG:
		m_StatusBar = ! m_StatusBar;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "StatusBarMsg",  m_StatusBar);
		break;
	case IDC_PROGRESS_MSG:
		m_Progress = ! m_Progress;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "ProgressMsg",  m_Progress);
		break;
	case IDC_HOTKEY_MSG:
		m_Hotkey = ! m_Hotkey;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "HotkeyMsg",  m_Hotkey);
		break;
	case IDC_HEADER_MSG:
		m_Header = ! m_Header;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "HeaderMsg",  m_Header);
		break;
	case IDC_ANIMATE_MSG:
		m_Animate = ! m_Animate;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "AnimateMsg",  m_Animate);
		break;
	case IDC_DDE_MSG:
		m_DDE = ! m_DDE;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "DDEMsg",  m_DDE);
		break;
	case IDC_UNKNOWN_MSG:
		m_UnKnown = ! m_UnKnown;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "UnKnownMsg",  m_UnKnown);
		break;
	case IDC_USER_MSG:
		m_User = ! m_User;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "UserMsg",  m_User);
		break;
	case IDC_MOUSEMOVE_MSG:
		m_MouseMove = ! m_MouseMove;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "MouseMoveMsg",  m_MouseMove);
		break;
	case IDC_KEYBOARD_MSG:
		m_Keyboard = ! m_Keyboard;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "KeyBoard",  m_Keyboard);
		break;
	case IDC_TOOLBAR_MSG:
		m_ToolBar = ! m_ToolBar;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "ToolBarMsg",  m_ToolBar);
		break;
	case IDC_CHILD:
		m_OnlyChild = ! m_OnlyChild;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "OnlyChildMsg",  m_OnlyChild);
		break;
	case IDC_PARENT:
		m_OnlyParent = ! m_OnlyParent;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "OnlyParentMsg",  m_OnlyParent);
		break;
	case IDC_SAMEPROCESS:
		m_OnlySameProcess = ! m_OnlySameProcess;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "SameProcessMsg",  m_OnlySameProcess);
		break;
	case IDC_SAMETHREAD:
		m_OnlySameThread = ! m_OnlySameThread;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "SameThreadMsg",  m_OnlySameThread);
		break;
	case IDC_WHOLE_SYSTEM:
		m_WholeSystem = ! m_WholeSystem;
		((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "WholeSystemMsg",  m_WholeSystem);
		break;
	}
}

BOOL CMsgOptionsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strToolTip;
	m_ToolTips.Create(this);
	m_ToolTips.Activate(TRUE);
	CWnd *pChildWnd = GetWindow(GW_CHILD);
	while(pChildWnd)
	{
		int nID = pChildWnd->GetDlgCtrlID();
		if (strToolTip.LoadString(nID))
		{
			m_ToolTips.AddTool(pChildWnd, strToolTip);
		}
		pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
	}

	// restore the previous settings
	m_Button =		((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "BtnMsg",		1);
	m_ListBox =		((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "ListBoxMsg",	1);
	m_ScrollBar =	((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "ScrollBarMsg", 1);
	m_ComboBox =	((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "ComboBoxMsg",	1);
	m_Edit =		((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "EditMsg",		1);
	m_TreeView =	((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "TreeViewMsg",	1);
	m_ListView =	((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "ListViewMsg",	1);
	m_DlgBox =		((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "DlgBoxMsg",	1);
	m_ToolTip =		((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "ToolTipMsg",	1);
	m_Spin =		((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "SpinMsg",		1);
	m_TabCtrl =		((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "TabCtrlMsg",	1);
	m_TrackBar =	((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "TrackBarMsg",	1);
	m_StatusBar =	((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "StatusBarMsg", 1);
	m_Progress =	((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "ProgressMsg",	1);
	m_Hotkey =		((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "HotkeyMsg",	1);
	m_Header =		((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "HeaderMsg",	1);
	m_Animate =		((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "AnimateMsg",	1);
	m_DDE =			((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "DDEMsg",		1);
	m_UnKnown =		((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "UnKnownMsg",	1);
	m_User =		((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "UserMsg",		1);
	m_MouseMove =	((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "MouseMoveMsg", 1);
	m_Keyboard	=	((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "KeyBoard",		1);
	m_SelectAll	=	((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "SelectAll",	0);
	m_ToolBar	=	((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "ToolBarMsg",	1);
	m_OnlyChild =	((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "OnlyChildMsg", 1);
	m_OnlyParent =	((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "OnlyParentMsg",0);
	m_OnlySameProcess = ((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "SameProcessMsg", 0);
	m_OnlySameThread =	((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "SameThreadMsg",  0);
	m_WholeSystem	=	((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "WholeSystemMsg", 0);
	m_MaxLines		=   ((CInfoViewApp*)AfxGetApp())->GetProfileInt("Settings", "MaxLines", 750);

	UpdateData(FALSE);
	if(m_WholeSystem)
	{
		OnWholeSystem();
	}
	if (m_OnlySameProcess)
	{
		OnSameprocess();
	}
	if (m_OnlySameThread)
	{
		OnSamethread();
	}
	if (m_OnlyParent)
	{
		OnParent();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMsgOptionsDlg::OnOK() 
{
	UpdateData(TRUE);
	if (m_GivenHwnd.IsEmpty())
	{
		::MessageBox(m_hWnd, "Please enter HWND or Select the window by using \"Finder Tool\"", "Select Window", MB_OK | MB_ICONINFORMATION);
		CEdit *pEdit = (CEdit *)GetDlgItem(IDC_HWND);
		ASSERT(NULL != pEdit);
		pEdit->SetFocus();
		return;
	}

	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "BtnMsg",  m_Button);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "ListBoxMsg",  m_ListBox);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "ScrollBarMsg",  m_ScrollBar);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "ComboBoxMsg",  m_ComboBox);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "EditMsg",  m_Edit);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "TreeViewMsg",  m_TreeView);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "ListViewMsg",  m_ListView);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "DlgBoxMsg",  m_DlgBox);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "ToolTipMsg",  m_ToolTip);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "SpinMsg",  m_Spin);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "TabCtrlMsg",  m_TabCtrl);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "TrackBarMsg",  m_TrackBar);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "StatusBarMsg",  m_StatusBar);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "ProgressMsg",  m_Progress);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "HotkeyMsg",  m_Hotkey);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "HeaderMsg",  m_Header);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "AnimateMsg",  m_Animate);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "DDEMsg",  m_DDE);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "UnKnownMsg",  m_UnKnown);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "UserMsg",  m_User);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "MouseMoveMsg",  m_MouseMove);	
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "KeyBoard",  m_Keyboard);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "SelectAll",	m_SelectAll);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "ToolBarMsg",  m_ToolBar);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "OnlyChildMsg",  m_OnlyChild);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "OnlyParentMsg",  m_OnlyParent);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "SameProcessMsg",  m_OnlySameProcess);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "SameThreadMsg",  m_OnlySameThread);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "WholeSystemMsg",  m_WholeSystem);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Settings", "MaxLines",  m_MaxLines);
	
	CDialog::OnOK();
}

void CMsgOptionsDlg::OnParent() 
{
	m_OnlyParent = !m_OnlyParent;
	CButton *pBtn = (CButton *)GetDlgItem(IDC_PARENT);
	ASSERT(pBtn);
	int nState = pBtn->GetState();
	if (nState & 0x0003)
	{
		CButton *pButton = (CButton *)GetDlgItem(IDC_CHILD);
		ASSERT(NULL != pButton);
		pButton->EnableWindow(FALSE);
	}
	else
	{
		CButton *pButton = (CButton *)GetDlgItem(IDC_CHILD);
		ASSERT(NULL != pButton);
		pButton->EnableWindow(TRUE);
	}
}

void CMsgOptionsDlg::OnSamethread()
{
	m_OnlySameThread = !m_OnlySameThread;
	CButton *pBtn = (CButton *)GetDlgItem(IDC_SAMETHREAD);
	ASSERT(pBtn);
	int nState = pBtn->GetState();
	if (nState & 0x0003)
	{
		CButton *pButton1 = (CButton *)GetDlgItem(IDC_PARENT);
		ASSERT(NULL != pButton1);
		pButton1->EnableWindow(FALSE);

		CButton *pButton2 = (CButton *)GetDlgItem(IDC_CHILD);
		ASSERT(NULL != pButton2);
		pButton2->EnableWindow(FALSE);
	}
	else
	{
		CButton *pButton1 = (CButton *)GetDlgItem(IDC_PARENT);
		ASSERT(NULL != pButton1);
		pButton1->EnableWindow(TRUE);

		CButton *pButton2 = (CButton *)GetDlgItem(IDC_CHILD);
		ASSERT(NULL != pButton2);
		pButton2->EnableWindow(TRUE);
		OnParent();
	}
}

void CMsgOptionsDlg::OnSameprocess()
{
	m_OnlySameProcess = ! m_OnlySameProcess;
	CButton *pBtn = (CButton *)GetDlgItem(IDC_SAMEPROCESS);
	ASSERT(pBtn);
	int nState = pBtn->GetState();
	if (nState & 0x0003)
	{
		CButton *pButton1 = (CButton *)GetDlgItem(IDC_PARENT);
		ASSERT(NULL != pButton1);
		pButton1->EnableWindow(FALSE);

		CButton *pButton2 = (CButton *)GetDlgItem(IDC_CHILD);
		ASSERT(NULL != pButton2);
		pButton2->EnableWindow(FALSE);

		CButton *pButton3 = (CButton *)GetDlgItem(IDC_SAMETHREAD);
		ASSERT(NULL != pButton3);
		pButton3->EnableWindow(FALSE);
	}
	else
	{
		CButton *pButton1 = (CButton *)GetDlgItem(IDC_PARENT);
		ASSERT(NULL != pButton1);
		pButton1->EnableWindow(TRUE);

		CButton *pButton2 = (CButton *)GetDlgItem(IDC_CHILD);
		ASSERT(NULL != pButton2);
		pButton2->EnableWindow(TRUE);

		CButton *pButton3 = (CButton *)GetDlgItem(IDC_SAMETHREAD);
		ASSERT(NULL != pButton3);
		pButton3->EnableWindow(TRUE);
		
		OnSamethread();
	}
}

void CMsgOptionsDlg::OnWholeSystem()
{
	m_WholeSystem = !m_WholeSystem;
	CButton *pBtn = (CButton *)GetDlgItem(IDC_WHOLE_SYSTEM);
	ASSERT(pBtn);
	int nState = pBtn->GetState();
	if (nState & 0x0003)
	{
		CButton *pButton2 = (CButton *)GetDlgItem(IDC_CHILD);
		ASSERT(NULL != pButton2);
		pButton2->EnableWindow(FALSE);

		CButton *pButton1 = (CButton *)GetDlgItem(IDC_PARENT);
		ASSERT(NULL != pButton1);
		pButton1->EnableWindow(FALSE);

		CButton *pButton3 = (CButton *)GetDlgItem(IDC_SAMETHREAD);
		ASSERT(NULL != pButton3);
		pButton3->EnableWindow(FALSE);

		CButton *pButton4 = (CButton *)GetDlgItem(IDC_SAMEPROCESS);
		ASSERT(NULL != pButton4);
		pButton4->EnableWindow(FALSE);
	}
	else
	{
		CButton *pButton2 = (CButton *)GetDlgItem(IDC_CHILD);
		ASSERT(NULL != pButton2);
		pButton2->EnableWindow(TRUE);

		CButton *pButton1 = (CButton *)GetDlgItem(IDC_PARENT);
		ASSERT(NULL != pButton1);
		pButton1->EnableWindow(TRUE);

		CButton *pButton3 = (CButton *)GetDlgItem(IDC_SAMETHREAD);
		ASSERT(NULL != pButton3);
		pButton3->EnableWindow(TRUE);

		CButton *pButton4 = (CButton *)GetDlgItem(IDC_SAMEPROCESS);
		ASSERT(NULL != pButton4);
		pButton4->EnableWindow(TRUE);

		OnSameprocess();
	}
}

BOOL CMsgOptionsDlg::PreTranslateMessage(MSG* pMsg) 
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