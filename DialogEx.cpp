// DialogEx.cpp : implementation file
//

#include "stdafx.h"
#include "InfoView.h"
#include "DialogEx.h"
#include ".\dialogex.h"

// CDialogEx dialog

IMPLEMENT_DYNAMIC(CDialogEx, CDialog)
CDialogEx::CDialogEx(UINT id, CWnd* pParent /*=NULL*/)
	: CDialog(id, pParent)
{
	
}

CDialogEx::~CDialogEx()
{
	
}

void CDialogEx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDialogEx, CDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CDialogEx::MakeDialogVisible(CRect& rectWnd)
{
	CRect screen;

	screen.left = 0;
	screen.top  = 0;
	screen.right = GetSystemMetrics(SM_CXSCREEN);
	screen.bottom = GetSystemMetrics(SM_CYSCREEN);

	if (rectWnd.left < screen.left)
	{
		int change = (screen.left - rectWnd.left);
		rectWnd.left  += change;
		rectWnd.right += change;
	}

	if (rectWnd.right > screen.right)
	{
		int change = (rectWnd.right - screen.right);
		rectWnd.right -= change;
		rectWnd.left  -= change;
	}

	if (rectWnd.top < screen.top)
	{
		int change = (screen.top - rectWnd.top);
		rectWnd.top	  += change;
		rectWnd.bottom += change;
	}

	if (rectWnd.bottom > screen.bottom)
	{
		int change = (rectWnd.bottom - screen.bottom);
		rectWnd.bottom -= change;
		rectWnd.top	  -= change;
	}
}

void CDialogEx::OnDestroy()
{
	CDialog::OnDestroy();

	CRect wndRect;
	this->GetWindowRect(&wndRect);

	CString caption;
	GetWindowText(caption);

	// save the window positions
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt(caption, "x",  wndRect.left);
	((CInfoViewApp*)AfxGetApp())->WriteProfileInt(caption, "y",  wndRect.top);
}

void CDialogEx::PreInitDialog()
{
	int left = 0, top = 0;

	CString caption;
	GetWindowText(caption);

	// get the previous dialog positions
	left = ((CInfoViewApp*)AfxGetApp())->GetProfileInt(caption, "x", -1);
	top  = ((CInfoViewApp*)AfxGetApp())->GetProfileInt(caption, "y", -1);

	if ( left == -1 || top == -1 )
	{
		CenterWindow();
		return;
	}

	CRect rect;
	GetWindowRect(&rect);

	MakeDialogVisible(rect);
	MoveWindow( left, top, rect.Width(), rect.Height() );

	CDialog::PreInitDialog();
}