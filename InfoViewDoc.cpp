////////////////////////////////////////////////////////////////
// CInfoViewDoc
// If this code works, it was written by Suresh.

// InfoViewDoc.cpp : implementation of the CInfoViewDoc class
//

#include "stdafx.h"
#include "InfoView.h"

#include "InfoViewDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInfoViewDoc

IMPLEMENT_DYNCREATE(CInfoViewDoc, CDocument)

BEGIN_MESSAGE_MAP(CInfoViewDoc, CDocument)
	//{{AFX_MSG_MAP(CInfoViewDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoViewDoc construction/destruction

CInfoViewDoc::CInfoViewDoc()
{
	m_bAutoDelete = FALSE;
}

CInfoViewDoc::~CInfoViewDoc()
{
}

BOOL CInfoViewDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CInfoViewDoc serialization

void CInfoViewDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CInfoViewDoc diagnostics

#ifdef _DEBUG
void CInfoViewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CInfoViewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CInfoViewDoc commands
