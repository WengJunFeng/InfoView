// MsgPropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InfoView.h"
#include "MsgPropertiesDlg.h"


// CMsgPropertiesDlg dialog

IMPLEMENT_DYNAMIC(CMsgPropertiesDlg, CDialog)
CMsgPropertiesDlg::CMsgPropertiesDlg(INFO_COPYDATA* pData, CWnd* pParent /*=NULL*/)
	: CDialog(CMsgPropertiesDlg::IDD, pParent),
	m_pData(pData)
{
	m_hIcon		= NULL;
}

CMsgPropertiesDlg::~CMsgPropertiesDlg()
{

}

void CMsgPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMsgPropertiesDlg, CDialog)
	
END_MESSAGE_MAP()


// CMsgPropertiesDlg message handlers
BOOL CMsgPropertiesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if ( m_pData != NULL )
	{
		AddColumns();

		CString str;
		str.Format("%s", m_pData->strMsgName);
		AddString("Message", str);

		str.Empty();
		str.Format("%s", m_pData->strMsgType);
		AddString("Post/Send", str);

		str.Empty();
		str.Format("0x%08x", m_pData->hwndFrom);
		AddString("Handle", str);

		str.Empty();
		str.Format("0x%08x", m_pData->lParam);
		AddString("lParam", str);

		str.Empty();
		str.Format("0x%08x", m_pData->wParam);
		AddString("wParam", str);

		str.Empty();
		str.Format("0x%02X", HIWORD (m_pData->lParam) & 0x00ff);
		AddString("ScanCode", str);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CMsgPropertiesDlg::AddString(CString heading, CString value)
{
	LV_ITEM item;

	CListCtrl* pListCtrl = (CListCtrl *)this->GetDlgItem(IDC_MSG_PROP_LISTCTRL);
	ASSERT(pListCtrl);

	item.mask			= LVIF_TEXT | LVIF_STATE;
	item.iItem			= pListCtrl->GetItemCount();
	item.iSubItem		= 0;
	item.state			= 0;
	item.stateMask		= 0;
	item.pszText		= (LPSTR)(LPCTSTR) heading;
	item.cchTextMax		= _tcslen(heading);
	item.iImage			= 0;
	item.lParam			= (LPARAM) 0;
	int nIndex = pListCtrl->InsertItem(&item);

	item.mask			= LVIF_TEXT;
	item.iItem			= (pListCtrl->GetItemCount() - 1);
	item.iSubItem		= 1;
	item.pszText		= (LPSTR)(LPCTSTR) value;
	item.cchTextMax		= _tcslen(value);
	item.lParam			= (LPARAM) 0;
	pListCtrl->SetItem(&item);
}

void CMsgPropertiesDlg::AddColumns()
{
	LV_COLUMN column;

	CListCtrl* pListCtrl = (CListCtrl *)this->GetDlgItem(IDC_MSG_PROP_LISTCTRL);
	ASSERT(pListCtrl);

	// add images to the list control
	m_ImageList.Create(16, 16, ILC_MASK, 0, 8);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_OPEN_FOLDER);
	m_ImageList.Add(m_hIcon);

	// set images
	pListCtrl->SetImageList(&m_ImageList, LVSIL_SMALL);

	// Add columns 
	// 0th column
	CString strColumnCaption("Item");
	column.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	column.fmt = LVCFMT_LEFT;
	column.cchTextMax= strColumnCaption.GetLength();
	column.iSubItem= 0;
	column.cx = 100;
	column.pszText = strColumnCaption.GetBuffer(strColumnCaption.GetLength());
    pListCtrl->InsertColumn(0, &column);
	strColumnCaption.ReleaseBuffer();

	// 1st column
	strColumnCaption = _T("Value");
	column.pszText= strColumnCaption.GetBuffer(strColumnCaption.GetLength());
	column.cx = 150;
	pListCtrl->InsertColumn(1, &column);
	strColumnCaption.ReleaseBuffer();
}