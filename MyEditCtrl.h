#pragma once

// MyEditCtrl

class MyEditCtrl : public CEdit
{
	DECLARE_DYNAMIC(MyEditCtrl)

public:
	MyEditCtrl();
	virtual ~MyEditCtrl();
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

protected:
	DECLARE_MESSAGE_MAP()
private:
	CBrush			m_brush;
};