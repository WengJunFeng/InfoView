#if !defined(AFX_CLASSPAGE_H__1953F0B9_706E_11D1_A146_0080AD1A7DB4__INCLUDED_)
#define AFX_CLASSPAGE_H__1953F0B9_706E_11D1_A146_0080AD1A7DB4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ClassPage.h : header file
//
#include "resource.h"
#include "WindowInfo.h"
#include "TabSheet.h"
#include "StaticEx.h"
/////////////////////////////////////////////////////////////////////////////
// CClassPage dialog

struct ClassStylesS {
	UINT	ClassStyle;
	LPCTSTR ClassName;
};

struct cursorStylesS {
	LPCTSTR	cursorStyle;
	LPCTSTR cursorName;
};

class CClassPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CClassPage)

// Construction
public:
	CClassPage();
	~CClassPage();

// Dialog Data
	//{{AFX_DATA(CClassPage)
	enum { IDD = IDD_CLASS };
	CStaticEx	m_InvalidWindow;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CClassPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CClassPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	LRESULT OnRefresh(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

private:
	WindowInfo* m_pWindowInfo;
	static ClassStylesS clsStyle[];
	static ClassStylesS wndHBackground[];
	static cursorStylesS wndHCursor[];
	void FillClassStylesInCombo(const WNDCLASSEX& wndClass);
public:
	void updateDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLASSPAGE_H__1953F0B9_706E_11D1_A146_0080AD1A7DB4__INCLUDED_)
