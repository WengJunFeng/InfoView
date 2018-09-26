// InfoViewDoc.h : interface of the CInfoViewDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFOVIEWDOC_H__0692810E_78EA_11D1_A147_0080AD1A7DB4__INCLUDED_)
#define AFX_INFOVIEWDOC_H__0692810E_78EA_11D1_A147_0080AD1A7DB4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CInfoViewDoc : public CDocument
{
protected: // create from serialization only
	DECLARE_DYNCREATE(CInfoViewDoc)

// Attributes
public:
	CInfoViewDoc();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfoViewDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CInfoViewDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CInfoViewDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFOVIEWDOC_H__0692810E_78EA_11D1_A147_0080AD1A7DB4__INCLUDED_)
