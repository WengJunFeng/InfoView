// TextView.cpp : implementation file
//

#include "stdafx.h"
#include "InfoView.h"
#include "TextView.h"
#include "./InfoHook/HHook.h"
#include "FindWindow.h"

// CTextView

IMPLEMENT_DYNCREATE(CTextView, CEditView)

CTextView::CTextView()
{
	CFindWindow findWindow;
	if(IDOK == findWindow.DoModal())
	{
		m_selectedWindow = findWindow.getSelectedWindow();

		if ( ::IsWindow(m_selectedWindow) && (m_selectedWindow != NULL))
		{
			// selected current window
			if (GetWindowThreadProcessId(m_selectedWindow, NULL) == GetCurrentThreadId())
			{
				return;
			}
			else
			{
				InstallKeybHook(m_selectedWindow, GetEditCtrl().GetSafeHwnd());
			}
		}
		else
		{
			::MessageBox(m_hWnd, "The Window you have selected is no longer valid. Please refresh and try again.", "Find", MB_OK | MB_ICONINFORMATION);
		}
		// re-set the selectedWindow
		m_selectedWindow = NULL;
	}
}

CTextView::~CTextView()
{
	ReleaseKeybHook();
}

BEGIN_MESSAGE_MAP(CTextView, CEditView)
END_MESSAGE_MAP()


// CTextView diagnostics

#ifdef _DEBUG
void CTextView::AssertValid() const
{
	CEditView::AssertValid();
}

void CTextView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG


// CTextView message handlers
