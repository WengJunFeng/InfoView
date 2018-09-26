#if !defined(AFX_ABOUTINFOVIEWER_H__BBD81B61_8056_11D1_AE0C_0080AD1A7E48__INCLUDED_)
#define AFX_ABOUTINFOVIEWER_H__BBD81B61_8056_11D1_AE0C_0080AD1A7E48__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// AboutInfoViewer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAboutInfoViewer dialog
#include "ButtonSubClass.h"
#include "StaticEx.h"
#include "DialogEx.h"

#define ID_INFOVIEW_TIMER_ID	786
#define INFOVIEW_TIMER_INTERVAL	25
#define LEFT_MOTION_INTERVAL	2

class CAboutInfoViewer : public CDialogEx
{
// Construction
public:
	CAboutInfoViewer(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAboutInfoViewer)
	enum { IDD = IDD_ABOUTBOX };
	CStaticEx	m_EMailID;
	CStaticEx	m_HomePageURL;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutInfoViewer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAboutInfoViewer)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CButtonSubClass m_BadButton;
	int m_OKPressed;
	UINT m_timer;
	int m_Red;
	int m_Green;
	int m_Blue;
	int m_CurrentColor;
public:
	afx_msg void OnStnClickedHomepageUrl();
	void CreateHole(CRgn& rgn);
	void ClearHoles();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUTINFOVIEWER_H__BBD81B61_8056_11D1_AE0C_0080AD1A7E48__INCLUDED_)
