#if !defined(AFX_FINDTXTDIALOG_H__AE7FD0E4_B858_11D1_BAD4_0080AD1A7DB4__INCLUDED_)
#define AFX_FINDTXTDIALOG_H__AE7FD0E4_B858_11D1_BAD4_0080AD1A7DB4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FindTxtDialog.h : header file
//

#define WM_FIND_NEXT	WM_USER+5000
#define WM_MARK_ALL		WM_USER+5001

/////////////////////////////////////////////////////////////////////////////
// CFindTxtDialog dialog

class CFindTxtDialog : public CDialog
{
// Construction
public:
	CFindTxtDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFindTxtDialog)
	enum { IDD = IDD_FINDTEXT };
	CString	m_FindText;
	BOOL	m_bMatchCase;
	BOOL	m_bMatchWholeWord;
	BOOL	m_IncludeSubFolders;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindTxtDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFindTxtDialog)
	afx_msg void OnFindNext();
	afx_msg void OnMarkAll();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeFindEdit();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString getFindText();
	BOOL getDirection();
	BOOL getWholeWord();
	BOOL getMatchCase();
	BOOL getIncludeSubFolders();
	void setIncludeSubFolders(BOOL bInclude);
	BOOL m_direction;
	CWnd*	m_pParent;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDTXTDIALOG_H__AE7FD0E4_B858_11D1_BAD4_0080AD1A7DB4__INCLUDED_)
