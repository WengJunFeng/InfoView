#if !defined(AFX_SPLASHDIALOG_H__8D1D5937_8E7E_11D2_BC6B_0080AD1A7DB4__INCLUDED_)
#define AFX_SPLASHDIALOG_H__8D1D5937_8E7E_11D2_BC6B_0080AD1A7DB4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SplashDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSplashDialog dialog

class CSplashDialog : public CDialog
{
// Construction
public:
	CSplashDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSplashDialog)
	enum { IDD = IDD_SPLASH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplashDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSplashDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLASHDIALOG_H__8D1D5937_8E7E_11D2_BC6B_0080AD1A7DB4__INCLUDED_)
