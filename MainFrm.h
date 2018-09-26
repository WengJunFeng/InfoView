// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__0692810A_78EA_11D1_A147_0080AD1A7DB4__INCLUDED_)
#define AFX_MAINFRM_H__0692810A_78EA_11D1_A147_0080AD1A7DB4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "CFlatBar.h"

#define INFOVIEW_NOTIFYICON WM_USER + 5

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CFlatToolBar	m_wndToolBar;
	HICON			m_hIcon;
	UINT			m_currentIconID;
	BOOL			m_bWindowVisible;
public:
	CStatusBar		m_wndStatusBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnQuitApplication();
	afx_msg void OnShow();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnUpdateShow(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	LRESULT OnShellMessage(WPARAM wParam, LPARAM lParam);
	BOOL AddIconToTaskBar(const LPSTR lpszTip) const;
	BOOL DeleteIconFromTaskBar(void) const;
	void MakeWndVisible(CRect& rectWnd);
	void DrawBitMap(HDC hdc,int xleft,int ytop,HBITMAP hbitmap);
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__0692810A_78EA_11D1_A147_0080AD1A7DB4__INCLUDED_)
