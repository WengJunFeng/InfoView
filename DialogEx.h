#pragma once


// CDialogEx dialog

class CDialogEx : public CDialog
{
	DECLARE_DYNAMIC(CDialogEx)

public:
	CDialogEx(UINT id, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogEx();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	void MakeDialogVisible(CRect& rectWnd);
public:
	afx_msg void OnDestroy();
protected:
	virtual void PreInitDialog();
};