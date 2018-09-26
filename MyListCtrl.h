#pragma once

// MyListCtrl

class MyListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(MyListCtrl)

public:
	enum EHighlight { HIGHLIGHT_NORMAL, HIGHLIGHT_ALLCOLUMNS, HIGHLIGHT_ROW };
	int m_nHighlight; // Indicate type of selection highlighting

public:
	MyListCtrl();
	virtual ~MyListCtrl();

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void RepaintSelectedItems();
	void OnPaint();
	void OnKillFocus(CWnd* pNewWnd);
	void OnSetFocus(CWnd* pOldWnd);
	int SetHighlightType(EHighlight hilite);

protected:
	DECLARE_MESSAGE_MAP()

	friend class CMessagesView;
};