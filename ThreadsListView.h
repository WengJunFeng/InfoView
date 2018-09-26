#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

// CThreadsListView view

class CThreadsListView : public CListView
{
	DECLARE_DYNCREATE(CThreadsListView)

protected:
	CThreadsListView();           // protected constructor used by dynamic creation
	virtual ~CThreadsListView();

protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
	void UpdateView(DWORD_PTR dwData);

private:
	BOOL RefreshThreadList (DWORD dwOwnerPID);
	void AddItem(DWORD, DWORD, LONG, LONG);
	void OnSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult);

private:
	HICON				m_hIcon;
	CImageList			m_ImageList;
};