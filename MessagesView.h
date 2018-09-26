#if !defined(AFX_MESSAGESVIEW_H__801CF704_BEF2_11D1_BAD4_0080AD1A7DB4__INCLUDED_)
#define AFX_MESSAGESVIEW_H__801CF704_BEF2_11D1_BAD4_0080AD1A7DB4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MessagesView.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CMessagesView view

#include <AFXCVIEW.H>
#include "MyListCtrl.h"

class CMessagesView : public CListView
{
protected:
	CMessagesView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMessagesView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessagesView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMessagesView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMessagesView)
	afx_msg void OnDestroy();
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClearView();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

protected:
	MyListCtrl			m_listctrl;

private:
	BOOL				m_OnInitalUpdateCalled;
	HICON				m_hIcon;
	CImageList			m_ImageList;
	int					m_MaxLines;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESSAGESVIEW_H__801CF704_BEF2_11D1_BAD4_0080AD1A7DB4__INCLUDED_)