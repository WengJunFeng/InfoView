#pragma once

#include "ProcessInfoDlg.h"

// CProcessTreeView view

class CProcessTreeView : public CTreeView
{
	DECLARE_DYNCREATE(CProcessTreeView)

protected:
	CProcessTreeView();           // protected constructor used by dynamic creation
	virtual ~CProcessTreeView();

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	afx_msg void OnClose( );
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnProperties( );

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

private:
	HTREEITEM AddItem(CString str, HTREEITEM hParent, LPARAM itemData);
	BOOL AddProcesses();
	void CopyStruct(PROCESSENTRY32* pDestination, PROCESSENTRY32* pSource);
	BOOL InitView(void);

private:
	HTREEITEM m_root;
	HICON				m_hIcon[2];
	CImageList			m_ImageList;
public:
	afx_msg void OnRefresh();
};