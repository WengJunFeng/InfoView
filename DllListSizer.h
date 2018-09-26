#ifndef __DLLLISTSIZER_H
#define __DLLLISTSIZER_H

#include <afxwin.h>
#include "Sizer.h"

class CDllListSizer : public CSizer
{
	public:
		CDllListSizer();
		
		int getMinWidth(CWnd& parentWnd);
		int getMinHeight(CWnd& parentWnd);
		void resize(CWnd& parentWnd);

		CRect m_CopyButton;
		CRect m_rectDlgNew;
		CRect m_rectListBox;
		CRect m_rectOK;
}; 

#endif	// __DLLLISTSIZER_H