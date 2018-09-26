// MyEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "InfoView.h"
#include "MyEditCtrl.h"

// MyEditCtrl

IMPLEMENT_DYNAMIC(MyEditCtrl, CEdit)
MyEditCtrl::MyEditCtrl()
{
	m_brush.CreateSolidBrush(RGB(255, 0, 0));
}

MyEditCtrl::~MyEditCtrl()
{
}

BEGIN_MESSAGE_MAP(MyEditCtrl, CEdit)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// MyEditCtrl message handlers
HBRUSH MyEditCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return m_brush;
}