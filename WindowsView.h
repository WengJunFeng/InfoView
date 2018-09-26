// WindowsView.h : interface of the CWindowsView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFOVIEWVIEW_H__06928110_78EA_11D1_A147_0080AD1A7DB4__INCLUDED_)
#define AFX_INFOVIEWVIEW_H__06928110_78EA_11D1_A147_0080AD1A7DB4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Tree.h"
#include "WindowInfo.h"
#include "TabSheet.h"
#include <shlobj.h>
#include <afxtempl.h>

// Property page(s)
#include "ClassPage.h"
#include "StylesPage.h"
#include "WindowsPage.h"
#include "GeneralPage.h"
#include "95ProcessPage.h"
#include "NTProcessPage.h"
#include "ProcessMemInfo.h"

#define LD_USEDESC     0x00000001
#define LD_USEARGS     0x00000002
#define LD_USEICON     0x00000004
#define LD_USEWORKDIR  0x00000008
#define LD_USEHOTKEY   0x00000010
#define LD_USESHOWCMD  0x00000020

typedef struct {
	// Mandatory members
	LPTSTR pszPathname;	// Pathname of original object  
	DWORD fdwFlags;		// LD_* flags ORed together for optional members
	
	// Optional members  
	LPTSTR pszDesc;		// Description of link file (its filename)
	LPTSTR pszArgs;		// command-line arguments  
	LPTSTR pszIconPath;
	
	// Pathname of file containing the icon  
	int  nIconIndex;	// Index of icon in pszIconPath  
	LPTSTR pszWorkingDir;// Working directory when process starts  
	int  nShowCmd;		// How to show the initial window  
	WORD  wHotkey;		// Hot key for the link

}LINKDATA, *PLINKDATA;

class CWindowsView : public CTreeView
{
protected: // create from serialization only
	CWindowsView();
	DECLARE_DYNCREATE(CWindowsView)

// Attributes
public:
	CInfoViewDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindowsView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWindowsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWindowsView)
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowProperties();
	afx_msg void OnHighlightWindow();
	afx_msg void OnRefresh();
	afx_msg void OnFindWindow();
	afx_msg void OnGetText();
	afx_msg void OnFindText();
	afx_msg void OnCreateShortcut();
	afx_msg void OnCopyHwnd();
	afx_msg void OnCopyWindowCaption();
	afx_msg void OnCopyClassName();
	afx_msg void OnCopyEntiretext();
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnShowClassname();
	afx_msg void OnShowProcessId();
	afx_msg void OnShowThreadId();
	afx_msg void OnUpdateShowClassname(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShowProcessId(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShowThreadId(CCmdUI* pCmdUI);
	afx_msg void OnShowWindowcaption();
	afx_msg void OnUpdateShowWindowcaption(CCmdUI* pCmdUI);
	afx_msg void OnShowHiddenWindows();
	afx_msg void OnUpdateOnShowHiddenWindows(CCmdUI* pCmdUI);
	afx_msg void OnHideWindow();
	afx_msg void OnUpdateHideWindow(CCmdUI* pCmdUI);
	afx_msg void OnEnableWindow();
	afx_msg void OnUpdateEnableWindow(CCmdUI* pCmdUI);
	afx_msg void OnCloseWindow();
	afx_msg void OnUpdateCloseWindow(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	// member variables
	HICON				m_hIcon[2];
	CImageList			m_ImageList;
	MTree<WindowInfo> 	m_Tree;
	HWND				m_selectedWindow;

	// member functions
	void UpdateTree();
	void FillTreeItemList(const HWND& hwnd, MTree<WindowInfo>* pTree = NULL);
	void InsertTreeItems(const HTREEITEM& hParent, MTree<WindowInfo>* const pTree);
	void ExpandBranch( HTREEITEM hti );
	void CollapseBranch( HTREEITEM hti);

	void DeleteFirstChild(const HTREEITEM& hParent) const;
	void DeleteAllChildren(const HTREEITEM& hParent) const;
	void ViewPropertyPage();
	void ReplaceTreeItemText(HTREEITEM hItem, CString oldString, CString newString);
	void AppendTreeItemText(HTREEITEM hItem, CString newString);

	BOOL HasChild(const HWND& hwnd) const;

	// FIND WINDOW FUNCTIONS
	LRESULT OnFindNext(WPARAM wParam, LPARAM lParam);
	LRESULT OnFindHwndText(WPARAM wParam, LPARAM lParam);
	LRESULT OnMarkAll(WPARAM wParam, LPARAM lParam);

public:
	void ShowPopupMenu(BOOL bClickedWithMouse = TRUE);

public:
	virtual HTREEITEM FindItem(CString &sSearch, BOOL bCaseSensitive = FALSE, 
				BOOL bDownDir = TRUE, BOOL bWholeWord = FALSE,
				HTREEITEM hItem = NULL);

protected:
	virtual BOOL IsFindValid( HTREEITEM );
	HTREEITEM GetNextItem( HTREEITEM hItem);
	HTREEITEM GetPrevItem( HTREEITEM hItem );
	HTREEITEM GetLastItem( HTREEITEM hItem );
	// void CollapseBranch( HTREEITEM hti);
	void CollapseAll();

	// void ExpandBranch( HTREEITEM hti);
	void ExpandAll();
private:

	BOOL		m_bExpandedAll;
	WindowInfo* m_pSelWndInfo;
	BOOL		m_bUpdate;

	BOOL		m_bShowClassName;
	BOOL		m_bShowProcessID;
	BOOL		m_bShowThreadID;
	BOOL		m_bShowWindowCaption;
	BOOL		m_bPropertySheetCreated;
	BOOL		m_bShowHiddenWindows;
private:
	CGeneralPage*		m_pGeneralPage;
	CStylesPage*		m_pStylesPage;
	CClassPage*			m_pClassPage;
	CWindowsPage*		m_pWindowsPage;
	C95ProcessPage*		m_p95ProcessPage;
	CNTProcessPage*		m_pNTProcessPage;
	CProcessMemInfo*	m_pProcessMemInfoPage;

public:
	CTabSheet*			m_pTabSheet;
};

#ifndef _DEBUG  // debug version in WindowsView.cpp
inline CInfoViewDoc* CWindowsView::GetDocument()
   { return (CInfoViewDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFOVIEWVIEW_H__06928110_78EA_11D1_A147_0080AD1A7DB4__INCLUDED_)