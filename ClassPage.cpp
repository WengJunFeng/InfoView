///////////////////////////////////////////////////////////////
// CClassPage
// If this code works, it was written by Suresh.

// ClassPage.cpp : implementation file
//

#include "stdafx.h"
#include "WindowInfo.h"
#include "ClassPage.h"
#include "TabSheet.h"
#include "InfoUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClassPage property page

ClassStylesS CClassPage::clsStyle[] = {	
	{	CS_BYTEALIGNCLIENT,	"CS_BYTEALIGNCLIENT"	},
	{	CS_BYTEALIGNWINDOW,	"CS_BYTEALIGNWINDOW"	},
	{	CS_CLASSDC,			"CS_CLASSDC"			},
	{	CS_DBLCLKS,			"CS_DBLCLKS"			},
	{	CS_GLOBALCLASS,		"CS_GLOBALCLASS"		},
	{	CS_HREDRAW,			"CS_HREDRAW"			},
	{	CS_VREDRAW,			"CS_VREDRAW"			},
	{	CS_NOCLOSE,			"CS_NOCLOSE"			},
	{	CS_OWNDC,			"CS_OWNDC"				},
	{	CS_PARENTDC,		"CS_PARENTDC"			},
	{	CS_SAVEBITS,		"CS_SAVEBITS"			}
};

ClassStylesS CClassPage::wndHBackground[] = {	
	{	COLOR_ACTIVEBORDER,		"COLOR_ACTIVEBORDER"	},
	{	COLOR_ACTIVECAPTION,	"COLOR_ACTIVECAPTION"	},
	{	COLOR_APPWORKSPACE,		"COLOR_APPWORKSPACE"	},
	{	COLOR_BACKGROUND,		"COLOR_BACKGROUND"		},
	{	COLOR_BTNFACE,			"COLOR_BTNFACE"			},
	{	COLOR_BTNSHADOW,		"COLOR_BTNSHADOW"		},
	{	COLOR_WINDOWTEXT,		"COLOR_WINDOWTEXT"		},
	{	COLOR_WINDOW,			"COLOR_WINDOW"			},
	{	COLOR_WINDOWFRAME,		"COLOR_WINDOWFRAME"		},
	{	COLOR_SCROLLBAR,		"COLOR_SCROLLBAR"		},
	{	COLOR_MENUTEXT,			"COLOR_MENUTEXT"		},
	{	COLOR_MENU,				"COLOR_MENU"			},
	{	COLOR_INACTIVECAPTION,	"COLOR_INACTIVECAPTION" },
	{	COLOR_INACTIVEBORDER,	"COLOR_INACTIVEBORDER"	},
	{	COLOR_HIGHLIGHTTEXT,	"COLOR_HIGHLIGHTTEXT"	},
	{	COLOR_HIGHLIGHT,		"COLOR_HIGHLIGHT"		},
	{	COLOR_GRAYTEXT,			"COLOR_GRAYTEXT"		},
	{	COLOR_CAPTIONTEXT,		"COLOR_CAPTIONTEXT"		},
	{	COLOR_BTNTEXT,			"COLOR_BTNTEXT"			}
};

cursorStylesS CClassPage::wndHCursor[] = {
	{	IDC_APPSTARTING,		"IDC_APPSTARTING"	},
	{	IDC_ARROW,				"IDC_ARROW"			},
	{	IDC_CROSS,				"IDC_CROSS"			},
	{	IDC_IBEAM,				"IDC_IBEAM"			},
	{	IDC_ICON,				"IDC_ICON"			},
	{	IDC_NO,					"IDC_NO"			},
	{	IDC_SIZE,				"IDC_SIZE"			},
	{	IDC_SIZEALL,			"IDC_SIZEALL"		},
	{	IDC_SIZENESW,			"IDC_SIZENESW"		},
	{	IDC_SIZENS,				"IDC_SIZENS"		},
	{	IDC_SIZENWSE,			"IDC_SIZENWSE"		},
	{	IDC_SIZEWE,				"IDC_SIZEWE"		},
	{	IDC_UPARROW,			"IDC_UPARROW"		},
	{	IDC_WAIT,				"IDC_WAIT"			}
};

IMPLEMENT_DYNCREATE(CClassPage, CPropertyPage)

CClassPage::CClassPage() : CPropertyPage(CClassPage::IDD)
{
	//{{AFX_DATA_INIT(CClassPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CClassPage::~CClassPage()
{
}

void CClassPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClassPage)
	DDX_Control(pDX, IDC_INVALID_WINDOW, m_InvalidWindow);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CClassPage, CPropertyPage)
	//{{AFX_MSG_MAP(CClassPage)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_REFRESH, OnRefresh)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassPage message handlers

BOOL CClassPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	m_InvalidWindow.SetFontBold(TRUE);
	m_InvalidWindow.SetTextColor( 706 );	// Red color
	updateDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CClassPage::FillClassStylesInCombo(const WNDCLASSEX& wndClass)
{	
	UINT style = wndClass.style;
	int num = sizeof(clsStyle) / sizeof(ClassStylesS);
	CComboBox *pComboBox = (CComboBox *)GetDlgItem(IDC_CLASS_STYLE_COMBO);
	ASSERT(pComboBox);
	for (int i=0; i< num; i++)
	{
		if (style & clsStyle[i].ClassStyle)
		{
			pComboBox->AddString(clsStyle[i].ClassName);
		}
	}
	if (pComboBox->GetCount() >= 0 )
		pComboBox->SetCurSel(0);
}

LRESULT CClassPage::OnRefresh(WPARAM wp, LPARAM lp)
{
	updateDialog();
	return 0;
}

void CClassPage::updateDialog()
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

	CComboBox *pComboBox = (CComboBox *)GetDlgItem(IDC_CLASS_STYLE_COMBO);
	ASSERT(pComboBox);
	pComboBox->ResetContent();
	
	WNDCLASSEX wndClass;
	// class Name
	CStatic *pClassName = (CStatic *)GetDlgItem(IDC_CLASS_NAME);
	ASSERT(pClassName);
	pClassName->SetWindowText(m_pWindowInfo->m_windowClassName);

	// instance Handle
	HINSTANCE instanceHandle = (HINSTANCE)::GetWindowLong(m_pWindowInfo->m_hwnd, GWL_HINSTANCE);
	::GetClassInfoEx(instanceHandle, m_pWindowInfo->m_windowClassName, &wndClass);

	// set class style
	UINT style = wndClass.style;
	CString strClassStyle;
	strClassStyle.Format("0x%08x", style);
	CStatic *pClassStyle = (CStatic *)GetDlgItem(IDC_CLASS_STYLE);
	ASSERT(pClassStyle);
	pClassStyle->SetWindowText(strClassStyle);
	FillClassStylesInCombo(wndClass);

	// set class bytes
	UINT bytes = wndClass.cbSize;
	CString strClassBytes;
	strClassBytes.Format("0x%08x", bytes);
	CStatic *pClassBytes = (CStatic *)GetDlgItem(IDC_CLASS_BYTES);
	ASSERT(pClassBytes);
	pClassBytes->SetWindowText(strClassBytes);
	// FillClassBytesInCombo(IDC_CLASS_BYTES_COMBO);

	CStatic *pMenuName = (CStatic *)GetDlgItem(IDC_MENU_NAME);
	ASSERT(pMenuName);
	pMenuName->SetWindowText("(None)");

	CStatic *pIconHandle = (CStatic *)GetDlgItem(IDC_ICON_HANDLE);
	ASSERT(pIconHandle);
	if (NULL != wndClass.hIcon)
	{
		CString strIconHandle;
		strIconHandle.Format("0x%08x", wndClass.hIcon);
		pIconHandle->SetWindowText(strIconHandle);
	}
	else
		pIconHandle->SetWindowText("(None)");
	
	CStatic *pCursor = (CStatic *)GetDlgItem(IDC_CURSOR_HANDLE);
	ASSERT(pCursor);
	if (NULL == wndClass.hCursor)
	{
		pCursor->SetWindowText(_T("IDI_APPLICATION"));
	}
	else
	{
		BOOL bCursorFound = FALSE;
		int num = sizeof(wndHCursor) / sizeof(cursorStylesS);
		for (int i=0; i< num; i++)
		{
			if (wndClass.hCursor == (HCURSOR)wndHCursor[i].cursorName)
			{
				pCursor->SetWindowText(wndHCursor[i].cursorName);
				bCursorFound = TRUE;
				break;
			}
		}
	}

	CStatic *pBackground = (CStatic *)GetDlgItem(IDC_BKGND_BRUSH);
	if (NULL != wndClass.hbrBackground)
	{
		BOOL bBackgroundFound = FALSE;
		int num = sizeof(wndHBackground) / sizeof(ClassStylesS);
		ASSERT(pBackground);
		for (int i=0; i< num; i++)
		{
			if (wndClass.hbrBackground == (HBRUSH)wndHBackground[i].ClassStyle)
			{
				pBackground->SetWindowText(wndHBackground[i].ClassName);
				bBackgroundFound = TRUE;
				break;
			}
			if (!bBackgroundFound)
				pBackground->SetWindowText(_T("(None)"));
		}
	}		
	else
		pBackground->SetWindowText(_T("(None)"));
}

BOOL CClassPage::OnSetActive() 
{
	updateDialog();	
	return CPropertyPage::OnSetActive();
}
