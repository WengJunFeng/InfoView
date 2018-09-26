#if !defined(AFX_PROCESSESVIEW_H__9BE1551B_8394_11D2_BC6B_0080AD1A7DB4__INCLUDED_)
#define AFX_PROCESSESVIEW_H__9BE1551B_8394_11D2_BC6B_0080AD1A7DB4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ProcessesView.h : header file
//
#include "WindowInfo.h"
#include <afxtempl.h>
#include <windows.h>
#include <vdmdbg.h>
#include <string.h>
#include <afxtempl.h>
#include "StaticEx.h"

#include "psapi.h"

typedef BOOL (WINAPI *GETPROCESSMEMORYINFO)
(
    HANDLE	Process,
    PPROCESS_MEMORY_COUNTERS ppsmemCounters,
    DWORD cb
);

//Windows NT Functions
typedef BOOL (WINAPI *ENUMPROCESSES)
(
	DWORD*	lpidProcess,  // array to receive the process identifiers
	DWORD	cb,           // size of the array
	DWORD*	cbNeeded      // receives the number of bytes returned
);

typedef BOOL (WINAPI *ENUMPROCESSMODULES)
(
	HANDLE		hProcess,   // handle to the process
	HMODULE*	lphModule,  // array to receive the module handles
	DWORD		cb,         // size of the array
	LPDWORD		lpcbNeeded  // receives the number of bytes returned
);

typedef DWORD (WINAPI *GETMODULEFILENAME)
(
	HANDLE	hProcess,		// handle to the process
	HMODULE	hModule,		// handle to the module
	LPTSTR	lpstrFileName,	// array to receive filename
	DWORD	nSize			// size of filename array.
);

typedef DWORD (WINAPI *GETMODULEBASENAME)
(
	HANDLE	hProcess,		// handle to the process
	HMODULE	hModule,		// handle to the module
	LPTSTR	lpstrFileName,	// array to receive base name of module
	DWORD	nSize			// size of module name array.
);

typedef INT (WINAPI *VDMENUMTASKWOWEX)
(
	DWORD			dwProcessId,	// ID of NTVDM process
	TASKENUMPROCEX	fp,				// address of our callback function
	LPARAM			lparam			// anything we want to pass to the callback function.
);

/////////////////////////////////////////////////////////////////////////////
// CProcessesView view

class CProcessesView : public CTreeView
{
protected:
	CProcessesView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CProcessesView)

// Attributes
public:
	CInfoViewDoc* GetDocument();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcessesView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CProcessesView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CProcessesView)
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnDestroy();
	afx_msg void OnFindText();
	afx_msg void OnRefresh();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HICON							m_hIcon[2];
	CImageList						m_ImageList;
	HTREEITEM						m_SelTreeItem;
	CMapPtrToWord					m_pidMap;
	CList <WindowInfo, WindowInfo>	m_processList;
	BOOL							m_bExpandedAll;
	static CString					m_strDummyItemText;
	BOOL							m_bShowProcessID;

private:
	void ExpandAll();
	void ExpandBranch( HTREEITEM hti );
	BOOL UpdateTree(void);
	void AddToProcessList(DWORD, HWND);
	virtual BOOL IsFindValid( HTREEITEM );
	LRESULT OnMarkAll(WPARAM wParam, LPARAM lParam);
	void ChangeBackColor( HTREEITEM hItem );
	void InsertDummyItem(HTREEITEM hItem );
	static BOOL CALLBACK enumWnds(HWND hWnd, LPARAM lParam);
	LRESULT OnFindNext(WPARAM wParam, LPARAM lParam);
	BOOL DeleteAllChildren(const HTREEITEM& hParent) const;
	void DeleteFirstChild(const HTREEITEM& hParent) const;
	
	virtual HTREEITEM FindItem( CString &sSearch, 
								BOOL bCaseSensitive = FALSE, 
								BOOL bDownDir = TRUE, 
								BOOL bWholeWord = FALSE,
								HTREEITEM hItem = NULL);

	HTREEITEM GetNextItem( HTREEITEM hItem);
	HTREEITEM GetPrevItem( HTREEITEM hItem );
	HTREEITEM GetLastItem( HTREEITEM hItem );

	HANDLE m_psapiHandle;
    ENUMPROCESSES       m_pFnEnumProcesses;
    GETMODULEFILENAME   m_pFnGetModuleFileName;
    ENUMPROCESSMODULES  m_pFnEnumProcessModules;  
	VDMENUMTASKWOWEX	m_pFnVDMEnumTaskWOWEx;
	GETMODULEBASENAME	m_pFnGetModuleBaseName;
	GETPROCESSMEMORYINFO m_pFnGetProcessMemoryInfo;

	CString getExecutableName(DWORD dwPID);
public:
	afx_msg void OnShowProcessId();
	afx_msg void OnUpdateShowProcessId(CCmdUI *pCmdUI);
	afx_msg void OnExpandAll();
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSESVIEW_H__9BE1551B_8394_11D2_BC6B_0080AD1A7DB4__INCLUDED_)
