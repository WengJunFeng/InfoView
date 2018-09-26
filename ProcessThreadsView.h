#if !defined(AFX_PROCESSTHREADSVIEW_H__977E0823_8C2A_11D2_BC6B_0080AD1A7DB4__INCLUDED_)
#define AFX_PROCESSTHREADSVIEW_H__977E0823_8C2A_11D2_BC6B_0080AD1A7DB4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ProcessThreadsView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProcessThreadsView view
#include "95Processes.h"
#include "NTProcesses.h"
#include "WindowInfo.h"
#include "pdhUtil.h"
#include <windows.h>
#include <tlhelp32.h>
#include <afxtempl.h>
#include "./InfoHook/HHook.h"

class CProcessThreadsView : public CTreeView
{
protected:
	CProcessThreadsView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CProcessThreadsView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcessThreadsView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CProcessThreadsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CProcessThreadsView)
	afx_msg void OnDestroy();
	afx_msg void OnRefresh();
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnKillProcess();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	private:
	HICON							m_hIcon[2];
	CImageList						m_ImageList;
	CList <WindowInfo, WindowInfo>	m_processList;
	CEnumProcess*					m_pEnumProcesses;

	static int						staticint;
private:
	BOOL UpdateTree(void);
	BOOL UpdateTreeNew(void);
	BOOL ShowThreads(const HTREEITEM& hItem, const PROCINFO& pi);
	BOOL ShowProperties(const HTREEITEM& hItem);

	BOOL IsWindows95(void);
	BOOL IsWindowsNT(void);

	void AddToList(const THREADENTRY32& pe32S);
	THREADENTRY32 m_List[1024];
	void foo(DWORD dwPID);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSTHREADSVIEW_H__977E0823_8C2A_11D2_BC6B_0080AD1A7DB4__INCLUDED_)
