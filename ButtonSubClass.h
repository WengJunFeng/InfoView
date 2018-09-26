#if !defined(AFX_BUTTONSUBCLASS_H__BBD81B62_8056_11D1_AE0C_0080AD1A7E48__INCLUDED_)
#define AFX_BUTTONSUBCLASS_H__BBD81B62_8056_11D1_AE0C_0080AD1A7E48__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ButtonSubClass.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CButtonSubClass window

class CButtonSubClass : public CButton
{
// Construction
public:
	CButtonSubClass();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonSubClass)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CButtonSubClass();

	// Generated message map functions
protected:
	//{{AFX_MSG(CButtonSubClass)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUTTONSUBCLASS_H__BBD81B62_8056_11D1_AE0C_0080AD1A7E48__INCLUDED_)
