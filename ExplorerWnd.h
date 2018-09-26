#pragma once

#include "ThreadsListView.h"
#include "ProcessTreeView.h"

// CExplorerWnd frame with splitter

class CExplorerWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CExplorerWnd)

public:
	CExplorerWnd();           // protected constructor used by dynamic creation
	virtual ~CExplorerWnd();

public:
	CSplitterWnd m_wndSplitter;

protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	DECLARE_MESSAGE_MAP()

public:
	CProcessTreeView* GetLeftPane();
	CThreadsListView* GetRightPane();
};