#include "stdafx.h"
#include "Sizer.h"

CSizer::CSizer()
{
	m_minWidth = 0;
	m_minHeight = 0;
}

void CSizer::init(CWnd& parentWnd)
{
	parentWnd.GetWindowRect(&m_rectDlg);
	m_minWidth = m_rectDlg.Width();
	m_minHeight = m_rectDlg.Height();
}

void CSizer::getMinMaxInfo(MINMAXINFO *lpMMI)
{
	lpMMI->ptMinTrackSize.x = m_minWidth;
	lpMMI->ptMinTrackSize.y = m_minHeight;
}

void CSizer::resize(CWnd& parentWnd)
{
	ASSERT(FALSE);
}
