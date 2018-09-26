#if !defined(AFX_DLLLISTDLG_H__ED094781_BEC8_11D1_BAD4_0080AD1A7DB4__INCLUDED_)
#define AFX_DLLLISTDLG_H__ED094781_BEC8_11D1_BAD4_0080AD1A7DB4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DLLListDlg.h : header file
//

#include "DllListSizer.h"

/////////////////////////////////////////////////////////////////////////////
// CDLLListDlg dialog

class CDLLListDlg : public CDialog
{
// Construction
public:
	CDLLListDlg(HWND hwnd, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLLListDlg)
	enum { IDD = IDD_DLLLIST };
	CListBox	m_ListBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLLListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLLListDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HWND			m_selectedWindow;
	BOOL			m_bInitCalled;
	CDllListSizer	m_DllListSizer;
public:
	afx_msg void OnCopylist();
	bool CopyTextToClipBoard(CString& str);
	bool CreateTemporaryTextFile(CString& str) const;
	void ExecuteTextFile(const CString strFileToOpen) const;
	void InsertHeader(CFile& file) const;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLLLISTDLG_H__ED094781_BEC8_11D1_BAD4_0080AD1A7DB4__INCLUDED_)
