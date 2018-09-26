// NTProcessPage.cpp : implementation file
//

#include "stdafx.h"
#include "infoview.h"
#include "NTProcessPage.h"
#include "InfoUtil.h"
#include "TabSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNTProcessPage property page

IMPLEMENT_DYNCREATE(CNTProcessPage, CPropertyPage)

CNTProcessPage::CNTProcessPage() : CPropertyPage(CNTProcessPage::IDD)
{
	//{{AFX_DATA_INIT(CNTProcessPage)
	m_pid = _T("");
	m_tid = _T("");
	m_ExeName = _T("");
	//}}AFX_DATA_INIT
    m_psapiHandle	= LoadLibrary("PSAPI.DLL");
    m_vdmdbgHandle	= LoadLibrary("VDMDBG.DLL");

	if ( NULL == m_psapiHandle || NULL == m_vdmdbgHandle )
        return;
	m_pFnEnumProcesses		= (ENUMPROCESSES)GetProcAddress((HINSTANCE)m_psapiHandle,		"EnumProcesses");
	m_pFnGetModuleFileName	= (GETMODULEFILENAME)GetProcAddress((HINSTANCE)m_psapiHandle,	"GetModuleFileNameExA");
	m_pFnGetModuleBaseName	= (GETMODULEBASENAME)GetProcAddress((HINSTANCE)m_psapiHandle,	"GetModuleBaseNameA");
	m_pFnEnumProcessModules	= (ENUMPROCESSMODULES)GetProcAddress((HINSTANCE)m_psapiHandle,	"EnumProcessModules");
	m_pFnGetProcessMemoryInfo= (GETPROCESSMEMORYINFO)GetProcAddress((HINSTANCE)m_psapiHandle,	"GetProcessMemoryInfo");

	if (NULL == m_pFnEnumProcesses		||
		NULL == m_pFnGetModuleFileName	||
		NULL == m_pFnGetModuleBaseName	||
		NULL == m_pFnGetProcessMemoryInfo ||
		NULL == m_pFnEnumProcessModules  )
        return;
}

CNTProcessPage::~CNTProcessPage()
{
	// Unload libraries
	if (m_psapiHandle)
	{
		::FreeLibrary((HINSTANCE)m_psapiHandle);
		m_psapiHandle = NULL;
	}

	if (m_vdmdbgHandle)
	{
		::FreeLibrary((HINSTANCE)m_vdmdbgHandle);
		m_vdmdbgHandle = NULL;
	}
}

void CNTProcessPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNTProcessPage)
	DDX_Control(pDX, IDC_INVALID_WINDOW, m_InvalidWindow);
	DDX_Text(pDX, IDC_PROCESS_ID, m_pid);
	DDX_Text(pDX, IDC_THREAD_ID, m_tid);
	DDX_Text(pDX, IDC_MODULENAME, m_ExeName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNTProcessPage, CPropertyPage)
	//{{AFX_MSG_MAP(CNTProcessPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNTProcessPage message handlers

BOOL CNTProcessPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	m_InvalidWindow.SetFontBold(TRUE);
	m_InvalidWindow.SetTextColor( 706 );	// Red color
	updateDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNTProcessPage::updateDialog()
{
	CTabSheet *pSheet = (CTabSheet *)GetParent();
	ASSERT(NULL != pSheet);

	m_pWindowInfo = pSheet->getWindowInfoObj();

	CStatic *pStatic = (CStatic *)GetDlgItem(IDC_INVALID_WINDOW);
	ASSERT(NULL != pStatic);

	if ((NULL == m_pWindowInfo) || !::IsWindow(m_pWindowInfo->m_hwnd))
	{
		HideAllControls(this);
		pStatic->ShowWindow(SW_SHOW);
		return;
	}
	else
	{
		UnHideAllControls(this);
		pStatic->ShowWindow(SW_HIDE);
	}

	CWnd *pWnd = CWnd :: FromHandle(m_pWindowInfo->m_hwnd);
	if (pWnd && IsWindow(m_pWindowInfo->m_hwnd))
	{
		DWORD dwThreadID, dwProcessID;
		CString strThread, strProcess;

		dwThreadID = GetWindowThreadProcessId(m_pWindowInfo->m_hwnd, &dwProcessID);
		m_tid.Format("%08x", dwThreadID);
		m_pid.Format("%08x", dwProcessID);
		m_tid.MakeUpper();
		m_pid.MakeUpper();
		m_tid = _T("0x") + m_tid;
		m_pid = _T("0x") + m_pid;

		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessID);
		if (hProcess)
		{
			TCHAR szModName[MAX_PATH];

			// get the Executable path
			if (m_pFnGetModuleFileName((HMODULE)hProcess, NULL, szModName, sizeof(szModName)/sizeof(TCHAR)))
			{
				m_ExeName = szModName;
			}
		}

		UpdateData(FALSE);
	}
}

BOOL CNTProcessPage::OnSetActive() 
{
	updateDialog();
	return CPropertyPage::OnSetActive();
}
