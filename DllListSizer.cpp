#include "stdafx.h"
#include <afxcmn.h>
#include "DllListSizer.h"
#include "Resource.h"

CDllListSizer::CDllListSizer()
{
	// constructor
}

void CDllListSizer::resize(CWnd& parentWnd)
{
	WINDOWPLACEMENT wndPlacement;
	if (parentWnd.GetWindowPlacement(&wndPlacement))
	{
		if ((SW_SHOWNORMAL == wndPlacement.showCmd) || (SW_SHOWMAXIMIZED == wndPlacement.showCmd))
		{
			parentWnd.GetWindowRect(&m_rectDlgNew);
			(parentWnd.GetDlgItem(IDC_DLL_LIST))->GetWindowRect(&m_rectListBox);
			(parentWnd.GetDlgItem(IDOK))->GetWindowRect(&m_rectOK);
			(parentWnd.GetDlgItem(IDC_COPYLIST))->GetWindowRect(&m_CopyButton);

			int changeinwidth = ((m_rectDlgNew.right - m_rectDlg.right) - (m_rectDlgNew.left - m_rectDlg.left));
			int changeinheight = ((m_rectDlgNew.bottom - m_rectDlg.bottom) - (m_rectDlgNew.top - m_rectDlg.top));

			m_rectDlg = m_rectDlgNew;

			if( changeinwidth || changeinheight )
			{
				m_rectListBox.right += changeinwidth;
				m_rectListBox.bottom += changeinheight;

				m_rectOK.left += changeinwidth;
				m_rectOK.right += changeinwidth;
				m_rectOK.top += changeinheight;
				m_rectOK.bottom += changeinheight;

				m_CopyButton.left += changeinwidth;
				m_CopyButton.right += changeinwidth;
				m_CopyButton.top += changeinheight;
				m_CopyButton.bottom += changeinheight;

				parentWnd.ScreenToClient(&m_rectOK);
				parentWnd.ScreenToClient(&m_rectListBox);
				parentWnd.ScreenToClient(&m_CopyButton);

				(parentWnd.GetDlgItem(IDC_DLL_LIST))->MoveWindow(&m_rectListBox, TRUE);
				(parentWnd.GetDlgItem(IDOK))->MoveWindow(&m_rectOK, TRUE);
				(parentWnd.GetDlgItem(IDC_COPYLIST))->MoveWindow(&m_CopyButton, TRUE);
			}
		}
	}
	else
	{
		ASSERT(FALSE);
	}
}