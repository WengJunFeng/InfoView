// InfoView.h : main header file for the INFOVIEW application
//

#if !defined(AFX_INFOVIEW_H__06928106_78EA_11D1_A147_0080AD1A7DB4__INCLUDED_)
#define AFX_INFOVIEW_H__06928106_78EA_11D1_A147_0080AD1A7DB4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "InfoViewDoc.h"
#include "ExplorerWnd.h"
#include "afxtempl.h"

/////////////////////////////////////////////////////////////////////////////
// CInfoViewApp:
// See InfoView.cpp for the implementation of this class
//

class CInfoViewApp : public CWinApp
{
public:
	CInfoViewApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfoViewApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL FirstInstance();
public:
	//{{AFX_MSG(CInfoViewApp)
	afx_msg void OnAppAbout();
	afx_msg void OnWindowsView();
	afx_msg void OnMessagesView();
	afx_msg void OnProcessesView();
	afx_msg void OnProcessThreads();
	afx_msg void OnProcessesTree();
	afx_msg void OnShowDlls();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CMultiDocTemplate*	m_pWindowsTemplate;
	CMultiDocTemplate*	m_pMessagesTemplate;
	CMultiDocTemplate*	m_pProcessesTemplate;
	CMultiDocTemplate*	m_pProcessesThreadsTemplate;
	CMultiDocTemplate*	m_pProcessesTree;
	CInfoViewDoc*		m_pInfoViewDoc;

private:
	HBITMAP SaveClientAreaAsBitmap(HWND hwnd);
public:
	afx_msg void OnCopywindowtoclipboard();
	afx_msg void OnCloseAllWindows();

private:
	CList<CWnd*, CWnd*> m_childWindowz;
};

extern LPCTSTR lpszUniqueClass;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFOVIEW_H__06928106_78EA_11D1_A147_0080AD1A7DB4__INCLUDED_)