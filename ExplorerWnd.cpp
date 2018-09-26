// ExplorerWnd.cpp : implementation file
//

#include "stdafx.h"
#include "InfoView.h"
#include "ExplorerWnd.h"
#include "ProcessPropView.h"
#include "ThreadPropView.h"

// CExplorerWnd
IMPLEMENT_DYNCREATE(CExplorerWnd, CMDIChildWnd)

CExplorerWnd::CExplorerWnd()
{
	// .ctor
}

CExplorerWnd::~CExplorerWnd()
{
	// ~ctor
}

BOOL CExplorerWnd::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	// create splitter window
	if (!m_wndSplitter.CreateStatic(this, 2, 2))
		return FALSE;

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CProcessTreeView), CSize(200, 200), pContext) ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CProcessPropView), CSize(200, 200), pContext) ||
		!m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CThreadsListView), CSize(200, 200), pContext) ||
		!m_wndSplitter.CreateView(1, 1, RUNTIME_CLASS(CThreadPropView), CSize(200, 200), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}
	return TRUE;
}

BOOL CExplorerWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style  = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	cs.style |= WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

	// no title
	cs.style &= ~FWS_ADDTOTITLE;	// no title
	cs.style &= ~FWS_PREFIXTITLE;   // no prefix title

	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

BEGIN_MESSAGE_MAP(CExplorerWnd, CMDIChildWnd)

END_MESSAGE_MAP()

//CProcessTreeView* CExplorerWnd::GetLeftPane()
//{
//	CWnd* pWnd = m_wndSplitter.GetPane(0, 0);
//	CProcessTreeView* pView = DYNAMIC_DOWNCAST(CProcessTreeView, pWnd);
//
//	return pView;
//}
//
//CThreadsListView* CExplorerWnd::GetRightPane()
//{
//	CWnd* pWnd = m_wndSplitter.GetPane(0, 1);
//	CThreadsListView* pView = DYNAMIC_DOWNCAST(CThreadsListView, pWnd);
//
//	return pView;
//}