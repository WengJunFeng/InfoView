////////////////////////////////////////////////////////////////
// CInfoView
// If this code works, it was written by Suresh.
// InfoView.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "InfoView.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "WindowsView.h"
#include "MessagesView.h"
#include "ProcessesView.h"
#include "AboutInfoViewer.h"
#include "SplashDialog.h"
#include "InfoUtil.h"
#include "FindWindow.h"
#include "DLLListDlg.h"
#include ".\infoview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LPCTSTR lpszUniqueClass = _T("InfoViewer For Windows Family");

// To use CMY colors with the Windows GDI instead of RGB, you can define the
//  following macro to supplement the standard RGB one:
#define  CMY(c, m, y)     RGB(255 - c, 255 - m, 255 - y)

/////////////////////////////////////////////////////////////////////////////
// CInfoViewApp

BEGIN_MESSAGE_MAP(CInfoViewApp, CWinApp)
	//{{AFX_MSG_MAP(CInfoViewApp)
	ON_COMMAND(IDD_ABOUTINFOVIEWER, OnAppAbout)
	ON_COMMAND(IDM_SHOW_DLLS, OnShowDlls)
	ON_COMMAND(IDM_FILE_NEW_WINDOWS_VIEW, OnWindowsView)
	ON_COMMAND(IDM_FILE_NEW_MESSAGES_VIEW, OnMessagesView)
	ON_COMMAND(IDM_FILE_NEW_PROCESSES_VIEW, OnProcessesView)
	ON_COMMAND(IDM_FILE_NEW_PROCESSES_TREE, OnProcessesTree)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(IDM_COPYWINDOWTOCLIPBOARD, OnCopywindowtoclipboard)
	ON_COMMAND(ID_WINDOW_CLOSEALLWINDOWS, OnCloseAllWindows)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoViewApp construction

CInfoViewApp::CInfoViewApp()
{
	m_pWindowsTemplate	= NULL;
	m_pInfoViewDoc		= NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CInfoViewApp object

CInfoViewApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CInfoViewApp initialization

// Keep track of whether the class was registered so we can
// unregister it upon exit
static BOOL bClassRegistered = FALSE;

BOOL CInfoViewApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	if(!FirstInstance())
	  return FALSE;

	HRSRC   hRsrc;
	HGLOBAL hDriverResource;
	DWORD   dwDriverSize;
	LPVOID  lpvDriver;
	HFILE   hfTempFile;

	hRsrc = FindResource ( AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_INFOHOOKDLL1), "InfoHookDLL" );

	hDriverResource = LoadResource(AfxGetApp()->m_hInstance, hRsrc);
	dwDriverSize = SizeofResource(AfxGetApp()->m_hInstance, hRsrc);
	lpvDriver = LockResource(hDriverResource);

	hfTempFile = _lcreat ( "c:\\msj.tmp", 0 );
	_hwrite ( hfTempFile, (LPCSTR) lpvDriver, dwDriverSize );
	_lclose ( hfTempFile );

	// show the about splash screen.
	CSplashDialog splashDialog;
	splashDialog.Create(IDD_SPLASH);
	splashDialog.ShowWindow(SW_SHOW);
	splashDialog.UpdateWindow();
	// show the spash screen for some time
	::Sleep(1800);	// 3 seconds

	// Register our unique class name that we wish to use
    WNDCLASS wndcls;
    memset(&wndcls, 0, sizeof(WNDCLASS));   // start with NULL defaults

    wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
    wndcls.lpfnWndProc = ::DefWindowProc;
    wndcls.hInstance = AfxGetInstanceHandle();
    wndcls.hIcon = LoadIcon(IDR_MAINFRAME); // or load a different icon
    wndcls.hCursor = LoadCursor( IDC_ARROW );
    wndcls.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wndcls.lpszMenuName = NULL;

    // Specify our own class name for using FindWindow later
    wndcls.lpszClassName = lpszUniqueClass;

    // Register new class and exit if it fails
    if(!AfxRegisterClass(&wndcls))
    {
      TRACE("Class Registration Failed\n");
      return FALSE;
    }
    bClassRegistered = TRUE;

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Suresh's Infoviewer"));

	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	// serve as the connection between documents, frame windows and views.

	m_pWindowsTemplate = new CMultiDocTemplate(
		IDR_INFOVITYPE,
		RUNTIME_CLASS(CInfoViewDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CWindowsView));
	AddDocTemplate(m_pWindowsTemplate);

	m_pProcessesTemplate = new CMultiDocTemplate(
		IDR_INFOVITYPE,
		RUNTIME_CLASS(CInfoViewDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CProcessesView));
	AddDocTemplate(m_pProcessesTemplate);

	m_pMessagesTemplate = new CMultiDocTemplate(
		IDR_INFOVITYPE,
		RUNTIME_CLASS(CInfoViewDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CMessagesView));
	AddDocTemplate(m_pMessagesTemplate);

	m_pProcessesTree = new CMultiDocTemplate(
		IDR_INFOVITYPE,
		RUNTIME_CLASS(CInfoViewDoc),
		RUNTIME_CLASS(CExplorerWnd), // custom MDI child frame
		RUNTIME_CLASS(CProcessTreeView));
	AddDocTemplate(m_pProcessesTree);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	m_pMainWnd = pMainFrame;

	m_pInfoViewDoc = new CInfoViewDoc;
	m_pInfoViewDoc->SetTitle("Infoviewer");

	CCreateContext context;
	context.m_pCurrentDoc		= m_pInfoViewDoc;
	context.m_pNewViewClass		= NULL;
	context.m_pNewDocTemplate	= NULL;
	context.m_pLastView			= NULL;
	context.m_pCurrentFrame		= NULL;

	// DWORD dwStyles = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE;
	DWORD dwStyles = WS_OVERLAPPEDWINDOW;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME, dwStyles, NULL, &context ))
		return FALSE;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Bypass OnFileNew();
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;


	splashDialog.DestroyWindow();
	
	// By default, show windows view
	OnWindowsView();

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	pMainFrame->SetWindowText("InfoViewer");

	InitCommonControls();
	CWinApp::InitInstance();
	return TRUE;
}

// App command to run the dialog
void CInfoViewApp::OnAppAbout()
{
	CAboutInfoViewer aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CInfoViewApp commands
//***********************************************************************
// Function: BOOL COnetimeApp::FirstInstance()
//
// Purpose: Check for a previous instance of this application by 
//          searching for a window with our specific pre-registered
//          class name. If one is found, then activate it and return
//          FALSE
//
//
// Parameters: none
//
// Returns: TRUE indicating that a previous instance was NOT found, or
//          FALSE if one was found and activated.
//
// Comments:
//
//***********************************************************************
BOOL CInfoViewApp::FirstInstance()
{
  CWnd *pWndPrev, *pWndChild;
  
  // Determine if another window with our class name exists...
  if (pWndPrev = CWnd::FindWindow(lpszUniqueClass,NULL))
  {
    pWndChild = pWndPrev->GetLastActivePopup(); // if so, does it have any popups?

    if (pWndPrev->IsIconic()) 
       pWndPrev->ShowWindow(SW_RESTORE);      // If iconic, restore the main window
	else
		pWndPrev->ShowWindow(SW_SHOWDEFAULT);

    pWndChild->SetForegroundWindow();         // Bring the main window or it's popup to
                                              // the foreground
    // and we are done activating the previous one.
    return FALSE;                             
  }
  else
    return TRUE;                              // First instance. Proceed as normal.
}

int CInfoViewApp::ExitInstance() 
{
	if (m_pInfoViewDoc)
		delete m_pInfoViewDoc;

    if(bClassRegistered)
      ::UnregisterClass(lpszUniqueClass,AfxGetInstanceHandle());

	return CWinApp::ExitInstance();
}

void CInfoViewApp::OnWindowsView() 
{
	CChildFrame* pFrame = new CChildFrame();
	CCreateContext context;
	context.m_pCurrentDoc		=	m_pInfoViewDoc;	// This is how I avoid creating a new document every time I open a new child window
	context.m_pNewViewClass		=	RUNTIME_CLASS(CWindowsView);
	context.m_pNewDocTemplate	=	m_pWindowsTemplate;
	context.m_pLastView			=	(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ?
									((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);

	context.m_pCurrentFrame		=	((CMainFrame *)m_pMainWnd)->GetActiveFrame();

	// DWORD dwStyle = WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE;
	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	if (!pFrame->LoadFrame(IDR_INFOVITYPE, dwStyle, m_pMainWnd, &context ))
		return;

	pFrame->InitialUpdateFrame(m_pInfoViewDoc, TRUE);

	m_childWindowz.AddTail(pFrame);
}

void CInfoViewApp::OnMessagesView() 
{
	CChildFrame* pFrame = new CChildFrame();
	CCreateContext context;
	context.m_pCurrentDoc		=	m_pInfoViewDoc;	// This is how I avoid creating a new document every time I open a new child window
	context.m_pNewViewClass		=	RUNTIME_CLASS(CMessagesView);
	context.m_pNewDocTemplate	=	m_pMessagesTemplate;
	context.m_pLastView			=	(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ?
									((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);

	context.m_pCurrentFrame		=	((CMainFrame *)m_pMainWnd)->GetActiveFrame();

	//if (!pFrame->LoadFrame(IDR_INFOVITYPE, WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE , m_pMainWnd, &context ))
	if (!pFrame->LoadFrame(IDR_INFOVITYPE, WS_OVERLAPPEDWINDOW, m_pMainWnd, &context ))
		return;
	pFrame->InitialUpdateFrame(m_pInfoViewDoc, TRUE);

	m_childWindowz.AddTail(pFrame);
}

void CInfoViewApp::OnProcessesView() 
{
	CChildFrame* pFrame = new CChildFrame();
	CCreateContext context;
	context.m_pCurrentDoc		=	m_pInfoViewDoc;	// This is how I avoid creating a new document every time I open a new child window
	context.m_pNewViewClass		=	RUNTIME_CLASS(CProcessesView);
	context.m_pNewDocTemplate	=	m_pProcessesTemplate;
	context.m_pLastView			=	(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ?
									((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);

	context.m_pCurrentFrame		=	((CMainFrame *)m_pMainWnd)->GetActiveFrame();

	// if (!pFrame->LoadFrame(IDR_INFOVITYPE, WS_OVERLAPPEDWINDOW | FWS_PREFIXTITLE , m_pMainWnd, &context ))
	if (!pFrame->LoadFrame(IDR_INFOVITYPE, WS_OVERLAPPEDWINDOW, m_pMainWnd, &context ))
		return;
	pFrame->InitialUpdateFrame(m_pInfoViewDoc, TRUE);	
	m_childWindowz.AddTail(pFrame);
}

void CInfoViewApp::OnProcessesTree()
{
	CExplorerWnd* pFrame = new CExplorerWnd();
	CCreateContext context;
	context.m_pCurrentDoc		=	m_pInfoViewDoc;	// This is how I avoid creating a new document every time I open a new child window
	context.m_pNewViewClass		=	RUNTIME_CLASS(CProcessTreeView);
	context.m_pNewDocTemplate	=	m_pProcessesTree;
	context.m_pLastView			=	(((CMainFrame *)m_pMainWnd)->GetActiveFrame() ?
									((CMainFrame *)m_pMainWnd)->GetActiveFrame()->GetActiveView() : NULL);

	context.m_pCurrentFrame		=	((CMainFrame *)m_pMainWnd)->GetActiveFrame();

	if (!pFrame->LoadFrame(IDR_INFOVITYPE, WS_OVERLAPPEDWINDOW, m_pMainWnd, &context ))
		return;

	pFrame->InitialUpdateFrame(m_pInfoViewDoc, TRUE);
	
	m_childWindowz.AddTail(pFrame);
}

void CInfoViewApp::OnShowDlls() 
{
	CFindWindow findWindow;
	if(IDOK == findWindow.DoModal())
	{
		HWND selectedWindow = findWindow.getSelectedWindow();

		if ( ::IsWindow(selectedWindow))
		{
			// selected current window
			if (GetWindowThreadProcessId(selectedWindow, NULL) == GetCurrentThreadId())
			{
				return;
			}
			else
			{
				CDLLListDlg dlg(selectedWindow);
				dlg.DoModal();
			}
		}
	}
}

HBITMAP CInfoViewApp::SaveClientAreaAsBitmap(HWND hwnd)
{
	RECT rect;
	HDC hdc = NULL, hmemdc = NULL;
	HBITMAP hbitmap = NULL, old_hbitmap = NULL;

    //get source device context for the client area
    // hdc = GetDC ( hwnd );
	hdc = GetDCEx ( hwnd, NULL, DCX_WINDOW | DCX_EXCLUDEUPDATE | DCX_LOCKWINDOWUPDATE);
    if( NULL == hdc ) 
		goto done;

    //get destination memory hdc compatible with client area
    hmemdc = CreateCompatibleDC(hdc);
    if(NULL == hmemdc ) 
		goto done;

    //create compatible bitmap for client area
    GetWindowRect(hwnd, &rect);      //.top & .left are both 0
	CWnd* pWnd = CWnd::FromHandle(hwnd);
	ASSERT(pWnd);
	pWnd->ScreenToClient(&rect);

    hbitmap = CreateCompatibleBitmap( hdc, rect.right, rect.bottom );

    if( NULL == hbitmap ) 
		goto done;

    //select client area bitmap into device context so we can write on it
    old_hbitmap = (HBITMAP) SelectObject ( hmemdc, hbitmap );

    //and copy it to the new hmemdc
    BitBlt( hmemdc,0,0,             //destination (x,y)
        rect.right,rect.bottom, //width, height
        hdc,0,0,                //source (x,y)
        SRCCOPY);

    //all done
    SelectObject ( hmemdc, old_hbitmap);

done:
	if ( NULL != hdc ) 
		ReleaseDC(hwnd,hdc);
    if ( NULL != hmemdc ) 
		DeleteDC(hmemdc);

    return ( hbitmap );
}

void CInfoViewApp::OnCopywindowtoclipboard()
{
	CFindWindow findWindow;
	if ( IDOK == findWindow.DoModal())
	{
		HWND targetWnd = findWindow.getSelectedWindow();
		if ( NULL != targetWnd )
		{
			HBITMAP bitmap = SaveClientAreaAsBitmap ( targetWnd );
			OpenClipboard(m_pMainWnd->GetSafeHwnd());
			EmptyClipboard();
			SetClipboardData ( CF_BITMAP, bitmap );
			CloseClipboard();
		}
	}
}

void CInfoViewApp::OnCloseAllWindows()
{
	POSITION pos = m_childWindowz.GetHeadPosition();
	while ( pos )
	{
		CWnd* pWnd = (CWnd *) m_childWindowz.GetNext(pos);
		if ( NULL != pWnd )
		{
			if ( ::IsWindow(pWnd->m_hWnd))
			{
				SendMessage(pWnd->m_hWnd, WM_CLOSE, 0, 0 );
			}
		}
	}
	m_childWindowz.RemoveAll();
}