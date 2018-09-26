#if !defined(AFX_WINDOWSPAGE_H__1953F0B8_706E_11D1_A146_0080AD1A7DB4__INCLUDED_)
#define AFX_WINDOWSPAGE_H__1953F0B8_706E_11D1_A146_0080AD1A7DB4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// WindowsPage.h : header file
//
#include "resource.h"
#include "WindowInfo.h"
#include "TabSheet.h"
#include "StaticEx.h"

#define WM_FIND_HWND_TEXT WM_USER+5002

/////////////////////////////////////////////////////////////////////////////
// CWindowsPage dialog

class CWindowsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CWindowsPage)

// Construction
public:
	CWindowsPage();
	~CWindowsPage();

// Dialog Data
	//{{AFX_DATA(CWindowsPage)
	enum { IDD = IDD_WINDOWS };
	CStaticEx	m_InvalidWindow;
	BOOL	m_ReflectSelection;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWindowsPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWindowsPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnCloseWindow();
	afx_msg void OnHideWindow();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDisableWindow();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	LRESULT OnRefresh(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
private:
	WindowInfo* m_pWindowInfo;
	CBrush*		m_pBrush;
	BOOL		m_bWindowHide;
	BOOL		m_bWindowClose;
	CString		getFormattedString(const HWND& hwnd) const;
	CTabSheet*	m_pTabSheet;
public:
	void updateDialog();
private:
	CFont		m_font;				// underline font for text control
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDOWSPAGE_H__1953F0B8_706E_11D1_A146_0080AD1A7DB4__INCLUDED_)
