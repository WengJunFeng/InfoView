#if !defined(AFX_MSGOPTIONSDLG_H__82381E21_BF5B_11D1_BAD4_0080AD1A7DB4__INCLUDED_)
#define AFX_MSGOPTIONSDLG_H__82381E21_BF5B_11D1_BAD4_0080AD1A7DB4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MsgOptionsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMsgOptionsDlg dialog

class CMsgOptionsDlg : public CDialog
{
// Construction
public:
	CMsgOptionsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMsgOptionsDlg)
	enum { IDD = IDD_MSGOPTIONS };
	BOOL	m_Animate;
	BOOL	m_Button;
	BOOL	m_ComboBox;
	BOOL	m_DDE;
	BOOL	m_DlgBox;
	BOOL	m_Edit;
	BOOL	m_Header;
	BOOL	m_Hotkey;
	BOOL	m_ListBox;
	BOOL	m_ListView;
	BOOL	m_Progress;
	BOOL	m_ScrollBar;
	BOOL	m_Spin;
	BOOL	m_StatusBar;
	BOOL	m_TabCtrl;
	BOOL	m_ToolTip;
	BOOL	m_TrackBar;
	BOOL	m_TreeView;
	BOOL	m_UnKnown;
	BOOL	m_User;
	BOOL	m_SelectAll;
	BOOL	m_MouseMove;
	BOOL	m_Keyboard;
	CString	m_GivenHwnd;
	BOOL	m_OnlyChild;
	CString	m_ClassName;
	BOOL	m_OnlyParent;
	CString	m_ProcessId;
	BOOL	m_OnlySameProcess;
	BOOL	m_OnlySameThread;
	CString	m_ThreadId;
	BOOL	m_ToolBar;
	BOOL	m_WholeSystem;
	int		m_MaxLines;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgOptionsDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMsgOptionsDlg)
	afx_msg void OnSelectAll();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnHideSpy();
	afx_msg void OnDestroy();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSameprocess();
	afx_msg void OnSamethread();
	afx_msg void OnWholeSystem();
	afx_msg void OnParent();
	//}}AFX_MSG
	afx_msg void OnSelection(UINT nID);
	DECLARE_MESSAGE_MAP()
private:
	BOOL	m_bWndHide;
	BOOL	m_bFindWindow ;
	HWND	m_selectedHwnd;
	HWND	m_hwndLastSubject;
	CToolTipCtrl m_ToolTips;

	void DrawWindowFrame (HWND hwndSubject);
public:
	HWND getSelectedWindow();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGOPTIONSDLG_H__82381E21_BF5B_11D1_BAD4_0080AD1A7DB4__INCLUDED_)
