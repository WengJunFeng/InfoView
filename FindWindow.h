#if !defined(AFX_FINDWINDOW_H__F9454901_8196_11D1_AE0C_0080AD1A7E48__INCLUDED_)
#define AFX_FINDWINDOW_H__F9454901_8196_11D1_AE0C_0080AD1A7E48__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FindWindow.h : header file
//
#include "MyEditCtrl.h"
#include "DialogEx.h"

/////////////////////////////////////////////////////////////////////////////
// CFindWindow dialog

class CFindWindow : public CDialogEx
{
// Construction
public:
	CFindWindow(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFindWindow)
	enum { IDD = IDD_FINDWINDOW };
	CString	m_GivenHwnd;
	MyEditCtrl m_Edit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindWindow)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFindWindow)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnHideSpy();
	afx_msg void OnDestroy();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeHwnd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HWND	m_selectedHwnd;
	BOOL	m_bWndHide;
	BOOL	m_bFindWindow;
	BOOL	m_UsedFinderTool;
	HWND	m_hwndLastSubject;
	void DrawWindowFrame (HWND hwndSubject);
	HWND SmallestWindowFromPoint(const POINT &point);

	CToolTipCtrl m_ToolTips;
public:
	HWND getSelectedWindow();
	afx_msg void OnBnClickedOk();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDWINDOW_H__F9454901_8196_11D1_AE0C_0080AD1A7E48__INCLUDED_)
