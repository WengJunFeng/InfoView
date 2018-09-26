#if !defined(AFX_GENERALPAGE_H__1953F0B6_706E_11D1_A146_0080AD1A7DB4__INCLUDED_)
#define AFX_GENERALPAGE_H__1953F0B6_706E_11D1_A146_0080AD1A7DB4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// GeneralPage.h : header file
//
#include "resource.h"
#include "WindowInfo.h"
#include "StaticEx.h"
/////////////////////////////////////////////////////////////////////////////
// CGeneralPage dialog

class CGeneralPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CGeneralPage)

// Construction
public:
	CGeneralPage();
	~CGeneralPage();

// Dialog Data
	//{{AFX_DATA(CGeneralPage)
	enum { IDD = IDD_GENERAL };
	CStaticEx	m_InvalidWindow;
	CString	m_WindowCaption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CGeneralPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CGeneralPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeWndCaption();
	//}}AFX_MSG
	LRESULT OnRefresh(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
	
private:
	WindowInfo* m_pWindowInfo;

public:
	void updateDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENERALPAGE_H__1953F0B6_706E_11D1_A146_0080AD1A7DB4__INCLUDED_)
