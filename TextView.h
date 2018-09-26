#pragma once


// CTextView view

class CTextView : public CEditView
{
	DECLARE_DYNCREATE(CTextView)

protected:
	CTextView();           // protected constructor used by dynamic creation
	virtual ~CTextView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

private:
	HWND m_selectedWindow;
};


