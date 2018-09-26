////////////////////////////////////////////////////////////////
// CFindTxtDialog
// If this code works, it was written by Suresh.

// FindTxtDialog.cpp : implementation file
//

#include "stdafx.h"
#include "infoview.h"
#include "FindTxtDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFindTxtDialog dialog


CFindTxtDialog::CFindTxtDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFindTxtDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindTxtDialog)
	m_FindText = _T("");
	m_bMatchCase = FALSE;
	m_bMatchWholeWord = FALSE;
	m_IncludeSubFolders = FALSE;
	//}}AFX_DATA_INIT
	m_direction = TRUE;
	m_pParent = pParent;
}


void CFindTxtDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindTxtDialog)
	DDX_Text(pDX, IDC_FIND_EDIT, m_FindText);
	DDX_Check(pDX, IDC_MATCH_CASE, m_bMatchCase);
	DDX_Check(pDX, IDC_MATCH_WHOLEWORD, m_bMatchWholeWord);
	DDX_Check(pDX, IDC_SUB_FOLDERS, m_IncludeSubFolders);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFindTxtDialog, CDialog)
	//{{AFX_MSG_MAP(CFindTxtDialog)
	ON_BN_CLICKED(IDC_FIND, OnFindNext)
	ON_BN_CLICKED(IDC_MARKALL, OnMarkAll)
	ON_EN_CHANGE(IDC_FIND_EDIT, OnChangeFindEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindTxtDialog message handlers

void CFindTxtDialog::OnFindNext() 
{
	UpdateData(TRUE);
	CButton *pButton = (CButton *)GetDlgItem(IDC_RADIO_UP_DIR);
	ASSERT(pButton);

	if (1 == pButton->GetCheck())
		m_direction = FALSE;
	else
		m_direction = TRUE;

	m_pParent->PostMessage(WM_FIND_NEXT,(WPARAM)0, (LPARAM)this);
}

void CFindTxtDialog::OnMarkAll()
{
	UpdateData(TRUE);
	CButton *pButton = (CButton *)GetDlgItem(IDC_RADIO_UP_DIR);
	ASSERT(pButton);

	if (1 == pButton->GetCheck())
		m_direction = FALSE;
	else
		m_direction = TRUE;

	m_pParent->PostMessage(WM_MARK_ALL,(WPARAM)0, (LPARAM)this);
}

BOOL CFindTxtDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CenterWindow();

	CButton *pDirection = (CButton *)GetDlgItem(IDC_RADIO_DOWN_DIR);
	ASSERT(pDirection);
	pDirection->SetCheck(1);
	
	CButton *pFind = (CButton *)GetDlgItem(IDC_FIND);
	ASSERT(pFind);
	pFind->EnableWindow(FALSE);

	CButton *pMarkAll = (CButton *)GetDlgItem(IDC_MARKALL);
	ASSERT(pMarkAll);
	pMarkAll->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFindTxtDialog::OnChangeFindEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_CHANGE flag ORed into the lParam mask.
	UpdateData(TRUE);

	m_FindText.TrimLeft();
	m_FindText.TrimRight();

	CButton *pFind = (CButton *)GetDlgItem(IDC_FIND);
	CButton *pMarkAll = (CButton *)GetDlgItem(IDC_MARKALL);
	ASSERT(pMarkAll);
	ASSERT(pFind);

	if ( !m_FindText.GetLength() )
	{
		pFind->EnableWindow(FALSE);
		pMarkAll->EnableWindow(FALSE);
	}
	else
	{
		pFind->EnableWindow(TRUE);
		pMarkAll->EnableWindow(TRUE);
	}
}

CString CFindTxtDialog::getFindText() 
{
	return m_FindText;
}

BOOL CFindTxtDialog::getMatchCase()
{
	return m_bMatchCase;
}

BOOL CFindTxtDialog::getWholeWord()
{
	return m_bMatchWholeWord;
}

BOOL CFindTxtDialog::getDirection()
{
	return m_direction;
}

void CFindTxtDialog::PostNcDestroy() 
{
	CDialog::PostNcDestroy();
	delete this;
}

void CFindTxtDialog::OnCancel() 
{
	DestroyWindow();	
}

BOOL CFindTxtDialog::PreTranslateMessage(MSG* pMsg)
{
	if( IsDialogMessage( pMsg ) )
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}

BOOL CFindTxtDialog::getIncludeSubFolders()
{
	return m_IncludeSubFolders;
}

void CFindTxtDialog::setIncludeSubFolders(BOOL bInclude)
{
	m_IncludeSubFolders = bInclude;
	CWnd* pWnd = GetDlgItem(IDC_SUB_FOLDERS);
	if ( NULL != pWnd  && bInclude == FALSE)
	{
		pWnd->ShowWindow(SW_HIDE);
	}
	UpdateData(TRUE);
}