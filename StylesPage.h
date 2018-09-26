#if !defined(AFX_STYLESPAGE_H__1953F0B7_706E_11D1_A146_0080AD1A7DB4__INCLUDED_)
#define AFX_STYLESPAGE_H__1953F0B7_706E_11D1_A146_0080AD1A7DB4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// StylesPage.h : header file
//

#include "resource.h"
#include "WindowInfo.h"
#include "StaticEx.h"

// structure used to store styles/extended styles
struct WindowsStylesS
{
	DWORD style;
	LPTSTR styleName;
};

/////////////////////////////////////////////////////////////////////////////
// CStylesPage dialog

class CStylesPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CStylesPage)

// Construction
public:
	CStylesPage();
	~CStylesPage();

// Dialog Data
	//{{AFX_DATA(CStylesPage)
	enum { IDD = IDD_STYLES };
	CStaticEx	m_InvalidWindow;
	CListBox	m_ExStyles;
	CListBox	m_Styles;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStylesPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CStylesPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	LRESULT OnRefresh(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
private:
	WindowInfo* m_pWindowInfo;

	// To fill styles & Extended styles list control.
	BOOL FillStyles(const CWnd* pWnd);
	BOOL FillExtendedStyles(const CWnd* pWnd);

	static WindowsStylesS	wndStyles[];
	static WindowsStylesS	wndExStyles[];
public:
	void updateDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STYLESPAGE_H__1953F0B7_706E_11D1_A146_0080AD1A7DB4__INCLUDED_)
