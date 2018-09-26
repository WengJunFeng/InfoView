#ifndef __CSIZER_H
#define __CSIZER_H

#include <afxwin.h>

class CSizer
{
	public:
		CSizer();
		
		void getMinMaxInfo(MINMAXINFO *lpMMI);
		void init(CWnd& parentWnd);
		virtual void resize(CWnd& parentWnd);

		int m_minWidth;
		int m_minHeight;
		CRect m_rectDlg;
};

#endif