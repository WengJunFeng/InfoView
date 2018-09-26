#if !defined(AFX_PROCESSPAGE_H__A97057A2_82F7_11D1_A147_0080AD1A7DB4__INCLUDED_)
#define AFX_PROCESSPAGE_H__A97057A2_82F7_11D1_A147_0080AD1A7DB4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ProcessPage.h : header file
//

#include "95Processes.h"
#include "NTProcesses.h"
#include "WindowInfo.h"
#include <afxtempl.h>
#include "StaticEx.h"
/////////////////////////////////////////////////////////////////////////////
// C95ProcessPage dialog

class C95ProcessPage : public CPropertyPage
{
	DECLARE_DYNCREATE(C95ProcessPage)

// Construction
public:
	C95ProcessPage();
	~C95ProcessPage();

// Dialog Data
	//{{AFX_DATA(C95ProcessPage)
	enum { IDD = IDD_95PROCESSES };
	CStaticEx	m_InvalidWindow;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(C95ProcessPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(C95ProcessPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	LRESULT OnRefresh(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

private:
	WindowInfo*						m_pWindowInfo;
	CBrush*							m_pBrush;
	CList <WindowInfo, WindowInfo>	m_processList;
	CEnumProcess*					m_pEnumProcesses;

	BOOL updateList();
public:
	void updateDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSPAGE_H__A97057A2_82F7_11D1_A147_0080AD1A7DB4__INCLUDED_)
