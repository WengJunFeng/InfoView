////////////////////////////////////////////////////////////////
// CStylesPage
// If this code works, it was written by Suresh.

// StylesPage.cpp : implementation file
//

#include "stdafx.h"
#include "WindowInfo.h"
#include "StylesPage.h"
#include "InfoUtil.h"
#include "TabSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStylesPage property page

WindowsStylesS CStylesPage::wndStyles[] = {
	{	WS_GROUP,			"WS_GROUP"				},
	{	WS_CHILD,			"WS_CHILD"				},
	{	WS_BORDER,			"WS_BORDER"				},
	{	WS_TABSTOP,			"WS_TABSTOP"			},
	{	WS_VSCROLL,			"WS_VSCROLL"			},
	{	WS_VISIBLE,			"WS_VISIBLE"			},
	{	WS_SYSMENU,			"WS_SYSMENU"			},
	{	WS_HSCROLL,			"WS_HSCROLL"			},
	{	WS_CAPTION,			"WS_CAPTION"			},
	{	WS_DISABLED,		"WS_DISABLED"			},
	{	WS_DLGFRAME,		"WS_DLGFRAME"			},
	{	WS_MAXIMIZE,		"WS_MAXIMIZE"			},
	{	WS_MINIMIZE,		"WS_MINIMIZE"			},
	{	WS_OVERLAPPED,		"WS_OVERLAPPED"			},
	{	WS_THICKFRAME,		"WS_THICKFRAME"			},
	{	WS_MINIMIZEBOX,		"WS_MINIMIZEBOX"		},
	{	WS_MAXIMIZEBOX,		"WS_MAXIMIZEBOX"		},
	{	WS_POPUPWINDOW,		"WS_POPUPWINDOW"		},
	{	WS_CLIPCHILDREN,	"WS_CLIPCHILDREN"		},
	{	WS_CLIPSIBLINGS,	"WS_CLIPSIBLINGS"		},
	{	WS_OVERLAPPEDWINDOW,"WS_OVERLAPPEDWINDOW"	},
	{	TVS_HASLINES,		"TVS_HASLINES"			},
	{	TVS_LINESATROOT,	"TVS_LINESATROOT"		},
	{	TVS_HASBUTTONS,		"TVS_HASBUTTONS"		},
	{	TVS_EDITLABELS,		"TVS_EDITLABELS"		},
	{	LVS_ICON,			"LVS_ICON"				},
	{	LVS_SMALLICON,		"LVS_SMALLICON"			},
	{	LVS_LIST,			"LVS_LIST"				},
	{	LVS_REPORT,			"LVS_REPORT"			}
};

WindowsStylesS CStylesPage::wndExStyles[] = 
{
	{	WS_EX_LEFT,				"WS_EX_LEFT"				},
	{	WS_EX_RIGHT,			"WS_EX_RIGHT"				},
	{	WS_EX_TOPMOST,			"WS_EX_TOPMOST"				},
	{	WS_EX_MDICHILD,			"WS_EX_MDICHILD"			},
	{	WS_EX_RTLREADING,		"WS_EX_RTLREADING"			},
	{	WS_EX_STATICEDGE,		"WS_EX_STATICEDGE"			},
	{	WS_EX_TOOLWINDOW,		"WS_EX_TOOLWINDOW"			},
	{	WS_EX_WINDOWEDGE,		"WS_EX_WINDOWEDGE"			},
	{	WS_EX_CLIENTEDGE,		"WS_EX_CLIENTEDGE"			},
	{	WS_EX_LTRREADING,		"WS_EX_LTRREADING"			},
	{	WS_EX_TRANSPARENT,		"WS_EX_TRANSPARENT"			},
	{	WS_EX_CONTEXTHELP,		"WS_EX_CONTEXTHELP"			},
	{	WS_EX_ACCEPTFILES,		"WS_EX_ACCEPTFILES"			},
	{	WS_EX_CONTROLPARENT,	"WS_EX_CONTROLPARENT"		},
	{	WS_EX_DLGMODALFRAME,	"WS_EX_DLGMODALFRAME"		},
	{	WS_EX_LEFTSCROLLBAR,	"WS_EX_LEFTSCROLLBAR"		},
	{	WS_EX_NOPARENTNOTIFY,	"WS_EX_NOPARENTNOTIFY"		},
	{	WS_EX_PALETTEWINDOW,	"WS_EX_PALETTEWINDOW"		},
	{	WS_EX_RIGHTSCROLLBAR,	"WS_EX_RIGHTSCROLLBAR"		},
	{	WS_EX_OVERLAPPEDWINDOW,	"WS_EX_OVERLAPPEDWINDOW"	}
};

IMPLEMENT_DYNCREATE(CStylesPage, CPropertyPage)

CStylesPage::CStylesPage() : CPropertyPage(CStylesPage::IDD)
{
	//{{AFX_DATA_INIT(CStylesPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CStylesPage::~CStylesPage()
{
}

void CStylesPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStylesPage)
	DDX_Control(pDX, IDC_INVALID_WINDOW, m_InvalidWindow);
	DDX_Control(pDX, IDC_EX_STYLES, m_ExStyles);
	DDX_Control(pDX, IDC_STYLES, m_Styles);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStylesPage, CPropertyPage)
	//{{AFX_MSG_MAP(CStylesPage)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_REFRESH, OnRefresh)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStylesPage message handlers

BOOL CStylesPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	m_InvalidWindow.SetFontBold(TRUE);
	m_InvalidWindow.SetTextColor( 706 );	// Red color
	updateDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CStylesPage::FillStyles(const CWnd* pWnd)
{
	ASSERT(pWnd != NULL);
	DWORD word = pWnd->GetStyle();
	int num = sizeof(wndStyles) / sizeof(WindowsStylesS);
	for (int i=0; i<num; i++)
	{	
		if (wndStyles[i].style & word)
			m_Styles.AddString(wndStyles[i].styleName);
	}
	return TRUE;
}

BOOL CStylesPage::FillExtendedStyles(const CWnd* pWnd)
{
	ASSERT(pWnd != NULL);
	DWORD word = pWnd->GetExStyle();
	int num = sizeof(wndExStyles) / sizeof(WindowsStylesS);

	for (int i=0; i<num; i++)
	{	
		if (wndExStyles[i].style & word)
			m_ExStyles.AddString(wndExStyles[i].styleName);
	}
	return TRUE;
}


LRESULT CStylesPage::OnRefresh(WPARAM wp, LPARAM lp)
{
	updateDialog();
	return 0;
}

void CStylesPage::updateDialog()
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

	m_Styles.ResetContent();
	m_ExStyles.ResetContent();
	long windowStyles	= ::GetWindowLong(m_pWindowInfo->m_hwnd, GWL_STYLE);
	long windowExStyles = ::GetWindowLong(m_pWindowInfo->m_hwnd, GWL_EXSTYLE);

	CString strWndStyles, wndExStyles1;
	strWndStyles.Format("0x%08x", windowStyles);
	wndExStyles1.Format("0x%08x", windowExStyles);
	CStatic *pStyles = (CStatic *)GetDlgItem(IDC_STYLE_LONG);
	ASSERT(pStyles);
	pStyles->SetWindowText(strWndStyles);

	CStatic *pExStyles = (CStatic *)GetDlgItem(IDC_EX_STYLE_LONG);
	ASSERT(pExStyles);
	pExStyles->SetWindowText(wndExStyles1);

	CWnd *pWnd = CWnd :: FromHandle(m_pWindowInfo->m_hwnd);
	if (pWnd && IsWindow(m_pWindowInfo->m_hwnd))
	{
		FillStyles(pWnd);
		FillExtendedStyles(pWnd);
	}
}

BOOL CStylesPage::OnSetActive() 
{
	updateDialog();	
	return CPropertyPage::OnSetActive();
}
