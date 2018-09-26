#if !defined(AFX_TABSHEET_H__1953F0BA_706E_11D1_A146_0080AD1A7DB4__INCLUDED_)
#define AFX_TABSHEET_H__1953F0BA_706E_11D1_A146_0080AD1A7DB4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TabSheet.h : header file
//
#define WM_REFRESH WM_USER+5001

#include "WindowInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CTabSheet

class CTabSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CTabSheet)

// Construction
public:
	CTabSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0, WindowInfo* pWindowInfo = NULL);
	CTabSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0, WindowInfo* pWindowInfo = NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabSheet)
	protected:
	virtual void PostNcDestroy();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTabSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTabSheet)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	//}}AFX_MSG
	virtual void OnOK();
	virtual void OnCancel();
	void OnRefresh();

	DECLARE_MESSAGE_MAP()
private:
	WindowInfo* m_pWindowInfo;
	CButton 	m_OKButton;
	CButton 	m_CancelButton;
	CButton 	m_RefreshButton;
public:
	WindowInfo* getWindowInfoObj(void)const;
	void setWindowInfoObj(WindowInfo* pWndInfo);
	void update(WindowInfo* m_pWindowInfo);
	CWnd*		m_pParent;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABSHEET_H__1953F0BA_706E_11D1_A146_0080AD1A7DB4__INCLUDED_)
