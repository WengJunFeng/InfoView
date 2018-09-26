////////////////////////////////////////////////////////////////
// CMainFrm
// If this code works, it was written by Suresh.
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "InfoView.h"
#include "MainFrm.h"

#include "./InfoHook/HHook.h"
#include "Windowsx.h"
#include ".\mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDM_ALLWAYS_ON_TOP 0x0010

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_SYSCOMMAND()
	ON_UPDATE_COMMAND_UI(IDM_SPY_SHOW, OnUpdateShow)
	//}}AFX_MSG_MAP
	ON_COMMAND(IDM_FILE_QUIT, OnQuitApplication)
	ON_COMMAND(IDM_SPY_SHOW, OnShow)
	ON_MESSAGE(INFOVIEW_NOTIFYICON, OnShellMessage)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_hIcon			= NULL;
	m_currentIconID = 0;
	m_bWindowVisible= TRUE;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.SetWindowText("Tool Bar");

	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	
	// load the icon to display in the task bar.
	m_hIcon = ((CInfoViewApp *)AfxGetApp())->LoadIcon(IDI_FRAME_CIRCLE);
	ASSERT(m_hIcon != NULL);
	m_currentIconID = IDI_FRAME_CIRCLE;	// currently loaded icon

	CString strWindowTitle("InfoView");

	// Add icon in Taskbar.
	AddIconToTaskBar(strWindowTitle.GetBuffer());

	// Set the window text
	SetWindowText(strWindowTitle);

	// Release buffer
	strWindowTitle.ReleaseBuffer();

	// Add "Always On Top" menu item to system menu.
	// IDM_ALLWAYS_ON_TOP must be in the system command range.
	ASSERT((IDM_ALLWAYS_ON_TOP & 0xFFF0) == IDM_ALLWAYS_ON_TOP);
	ASSERT(IDM_ALLWAYS_ON_TOP < 0xF000);

	BOOL bSaveSettings = ((CInfoViewApp*)AfxGetApp())->GetProfileInt("Window Position", "AlwaysOnTop", 1);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	ASSERT(NULL != pSysMenu);

	pSysMenu->AppendMenu(MF_SEPARATOR|MF_SEPARATOR);

	if (bSaveSettings)
	{
		pSysMenu->AppendMenu(MF_CHECKED |MF_ENABLED, IDM_ALLWAYS_ON_TOP, "&Always On Top");
		SetWindowPos(&wndTopMost, 10, 10, 100, 100 , SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
	}
	else
	{
		pSysMenu->AppendMenu(MF_UNCHECKED |MF_ENABLED, IDM_ALLWAYS_ON_TOP, "&Always On Top");
		SetWindowPos(&wndNoTopMost, 10, 10, 100, 100 , SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
	}

	pSysMenu->AppendMenu(MF_SEPARATOR|MF_SEPARATOR);
	pSysMenu->AppendMenu(MF_UNCHECKED |MF_ENABLED, IDM_FILE_QUIT, "&Quit\tAlt-X");

	return 0;
}

void CMainFrame::DrawBitMap(HDC hdc,int xleft,int ytop,HBITMAP hbitmap)
  {
  HDC     hmemdc;
  BITMAP  bm;
  POINT   point;
  HBITMAP old_hbitmap;
  
       hmemdc=NULL;
       old_hbitmap=NULL;
  
       //create memory device context & select bitmap
       hmemdc=CreateCompatibleDC(hdc);
       if(hmemdc==NULL) goto done;
       old_hbitmap = (HBITMAP) SelectObject(hmemdc,hbitmap);
  
       SetMapMode(hmemdc,GetMapMode(hdc));     //same as for hdc
  
       //get bitmap dimensions & convert to logical
       GetObject(hbitmap,sizeof(bm),&bm);
       point.x=bm.bmWidth;
       point.y=bm.bmHeight;
       DPtoLP(hdc,&point,1);
  
       BitBlt(hdc,xleft,ytop,                  //destination
            point.x,point.y,                //width, height
            hmemdc,0,0,                     //source
            SRCCOPY);
  
  done:   if(old_hbitmap!=NULL) SelectObject(hmemdc,old_hbitmap);
       if(hmemdc!=NULL) DeleteDC(hmemdc);
       return;
  }

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	// the CREATESTRUCT cs

    // Use the specific class name we established earlier
	cs.lpszClass = lpszUniqueClass;
	cs.style &= ~FWS_ADDTOTITLE;	// no title
	cs.style &= ~FWS_PREFIXTITLE;   // no prefix title

	// get the previous window positions
	cs.x  = ((CInfoViewApp*)AfxGetApp())->GetProfileInt("Window Position", "x", 20);
	cs.y  = ((CInfoViewApp*)AfxGetApp())->GetProfileInt("Window Position", "y", 20);
	cs.cx = ((CInfoViewApp*)AfxGetApp())->GetProfileInt("Window Position", "cx", 400);
	cs.cy =	((CInfoViewApp*)AfxGetApp())->GetProfileInt("Window Position", "cy", 400);

	CRect rectWnd(cs.x, cs.y, cs.x + cs.cx, cs.y + cs.cy);
	MakeWndVisible(rectWnd);

	cs.x  = rectWnd.left;
	cs.y  = rectWnd.top;
	cs.cx = rectWnd.Width();
	cs.cy = rectWnd.Height();

	return CMDIFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
BOOL CMainFrame::AddIconToTaskBar(const LPSTR lpszTip) const
{
    BOOL res; 
    NOTIFYICONDATA tnid; 

    tnid.cbSize = sizeof(NOTIFYICONDATA); 
    tnid.hWnd = m_hWnd;
    tnid.uID = m_currentIconID;
    tnid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; 
    tnid.uCallbackMessage = INFOVIEW_NOTIFYICON; 
    tnid.hIcon = m_hIcon; 
    if (lpszTip) 
        lstrcpyn(tnid.szTip, lpszTip, sizeof(tnid.szTip)); 
    else 
        tnid.szTip[0] = '\0';
 
    res = Shell_NotifyIcon(NIM_ADD, &tnid); 
    return res; 
}

BOOL CMainFrame::DeleteIconFromTaskBar(void) const
{
    BOOL res; 
    NOTIFYICONDATA tnid; 
 
    tnid.cbSize = sizeof(NOTIFYICONDATA); 
    tnid.hWnd = m_hWnd; 
    tnid.uID = m_currentIconID; 
         
    res = Shell_NotifyIcon(NIM_DELETE, &tnid); 
    return res; 
}

LRESULT CMainFrame::OnShellMessage(WPARAM wParam, LPARAM lParam)
{
	//UINT uID = (UINT)wParam;
	UINT uMouseMsg = (UINT)lParam;
	if (uMouseMsg == WM_RBUTTONDOWN)
	{
		CMenu menu;
		CMenu *submenu;
		// Load the menu.
		menu.LoadMenu(IDR_WINDOWS_POPUPMENU);
		// Get the pop-up menu.
		submenu = menu.GetSubMenu(1);

		CPoint mousePos;
		::GetCursorPos(&mousePos);

		if ( m_bWindowVisible )
		{
			submenu->CheckMenuItem(IDM_SPY_SHOW, MF_BYCOMMAND | MF_CHECKED);
		}
		else
		{
			submenu->CheckMenuItem(IDM_SPY_SHOW, MF_BYCOMMAND | MF_UNCHECKED);
		}
		// Post the menu.
		this->SetForegroundWindow();
		submenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, mousePos.x, mousePos.y, this, NULL);
		this->PostMessage(WM_NULL, 0, 0);
	}
	else if ( uMouseMsg == WM_LBUTTONDBLCLK ) 
	{
		::ShowWindow(m_hWnd, SW_SHOW);
		m_bWindowVisible = TRUE;
		this->SetActiveWindow();

		WINDOWPLACEMENT wndPlacement;
		if (GetWindowPlacement(&wndPlacement))
		{
			//if window is maximized or minimized do not store its coordinates
			if (SW_SHOWMINIMIZED == wndPlacement.showCmd)
			{
				::ShowWindow(m_hWnd, SW_RESTORE);
			}
		}
	}
	return 0;
}

void CMainFrame::OnClose() 
{
	::ShowWindow(m_hWnd, SW_HIDE);
	m_bWindowVisible = FALSE;
}

void CMainFrame::OnUpdateShow(CCmdUI* pCmdUI)
{
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();
	ASSERT(NULL != pMenu);
	if ( FALSE == m_bWindowVisible )
	{
		pMenu->CheckMenuItem(IDM_SPY_SHOW, MF_BYCOMMAND | MF_UNCHECKED);
	}
	else
	{
		pMenu->CheckMenuItem(IDM_SPY_SHOW, MF_BYCOMMAND | MF_CHECKED);
	}
}

void CMainFrame::OnShow()
{
	if ( m_bWindowVisible )
	{
		::ShowWindow(m_hWnd, SW_HIDE);
		m_bWindowVisible = FALSE;
	}
	else
	{
		::ShowWindow(m_hWnd, SW_SHOW);
		m_bWindowVisible = TRUE;
	}
}

void CMainFrame::OnQuitApplication()
{
	ASSERT(::IsWindow(m_hWnd));

	// Delete icon from taskbar
	DeleteIconFromTaskBar();
	WINDOWPLACEMENT wndPlacement;
	if (GetWindowPlacement(&wndPlacement))
	{
		//if window is maximized or minimized do not store its coordinates
		if (SW_SHOWNORMAL == wndPlacement.showCmd)
		{
			CRect wndRect;
			GetWindowRect(&wndRect);

			// save the window positions
			((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Window Position", "x",  wndRect.left);
			((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Window Position", "y",  wndRect.top);
			((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Window Position", "cx", wndRect.Width());
			((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Window Position", "cy", wndRect.Height());
		}
	}
	CMDIFrameWnd::OnClose();
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ALLWAYS_ON_TOP)
	{ 
		CMenu* pSysMenu = GetSystemMenu(FALSE);
		ASSERT(NULL != pSysMenu);

		if (MF_UNCHECKED == pSysMenu->GetMenuState(IDM_ALLWAYS_ON_TOP, MF_BYCOMMAND))
		{
			pSysMenu->CheckMenuItem(IDM_ALLWAYS_ON_TOP, MF_BYCOMMAND | MF_CHECKED);
			SetWindowPos(&wndTopMost, 10, 10, 100, 100 , SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
			((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Window Position", "AlwaysOnTop", 1);
		}
		else
		{
			pSysMenu->CheckMenuItem(IDM_ALLWAYS_ON_TOP, MF_BYCOMMAND | MF_UNCHECKED);
			SetWindowPos(&wndNoTopMost, 10, 10, 100, 100 , SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
			((CInfoViewApp*)AfxGetApp())->WriteProfileInt("Window Position", "AlwaysOnTop", 0);
		}
	}
	else if (nID == IDM_FILE_QUIT)
	{
		OnQuitApplication();
	}
	else
	{
		CMDIFrameWnd::OnSysCommand(nID, lParam);
	}
}

void CMainFrame::MakeWndVisible(CRect& rectWnd)
{
	CRect screen;

	screen.left = 0;
	screen.top  = 0;
	screen.right = GetSystemMetrics(SM_CXSCREEN);
	screen.bottom = GetSystemMetrics(SM_CYSCREEN);
	
	if (rectWnd.left < screen.left)
	{
		int change = (screen.left - rectWnd.left);
		rectWnd.left  += change;
		rectWnd.right += change;
	}

	if (rectWnd.right > screen.right)
	{
		int change = (rectWnd.right - screen.right);
		rectWnd.right -= change;
		rectWnd.left  -= change;
	}

	if (rectWnd.top < screen.top)
	{
		int change = (screen.top - rectWnd.top);
		rectWnd.top	  += change;
		rectWnd.bottom += change;
	}

	if (rectWnd.bottom > screen.bottom)
	{
		int change = (rectWnd.bottom - screen.bottom);
		rectWnd.bottom -= change;
		rectWnd.top	  -= change;
	}
}

void CMainFrame::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CMDIFrameWnd::OnPaint() for painting messages

}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIFrameWnd::OnSize(nType, cx, cy);
}

BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
	return CMDIFrameWnd::OnEraseBkgnd(pDC);
}