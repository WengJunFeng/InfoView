#pragma once

#include "./InfoHook/HHook.h"

// CMsgPropertiesDlg dialog

class CMsgPropertiesDlg : public CDialog
{
	DECLARE_DYNAMIC(CMsgPropertiesDlg)

public:
	CMsgPropertiesDlg(INFO_COPYDATA*pData = NULL, CWnd* pParent = NULL);   // standard constructor
	virtual ~CMsgPropertiesDlg();

// Dialog Data
	enum { IDD = IDD_MSGPROPERTIESDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	void AddColumns( );
	void CMsgPropertiesDlg::AddString(CString heading, CString value);

private:
	INFO_COPYDATA*		m_pData;
	HICON				m_hIcon;
	CImageList			m_ImageList;
};