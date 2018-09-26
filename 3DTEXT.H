#ifndef _C3DTEXT_
#define _C3DTEXT_

/////////////////////////////////////////////////////////////////////////////
// C3dText controlclass 

class C3dText : public CButton 
{
	BOOL m_bUse3D;
public:     
	C3dText() : m_bUse3D(true) 
	{
		// constructor
	}

	void Use3D(BOOL bUse3D = true)
	{ 
		m_bUse3D = bUse3D;
	}
protected:
	void Draw(CDC* pDC, const CRect& rect, UINT state);
	//{{AFX_MSG(C3dText)
	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);     
	//}}AFX_MSG
	//{{AFX_VIRTUAL(C3dText)     
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL     

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif