// ProcessMemInfo.cpp : implementation file
//

#include "stdafx.h"
#include "infoview.h"
#include "ProcessMemInfo.h"
#include "InfoUtil.h"
#include "TabSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProcessMemInfo property page

IMPLEMENT_DYNCREATE(CProcessMemInfo, CPropertyPage)

CProcessMemInfo::CProcessMemInfo() : CPropertyPage(CProcessMemInfo::IDD)
{
	//{{AFX_DATA_INIT(CProcessMemInfo)
	m_PageFaultCount = _T("");
	m_PeakWorkingSetSize= _T("");
	m_WorkingSetSize= _T("");
	m_QuotaPeakPagedPoolUsage= _T("");
	m_QuotaPagedPoolUsage= _T("");
	m_QuotaPeakNonPagedPoolUsage= _T("");
	m_QuotaNonPagedPoolUsage= _T("");
	m_PagefileUsage= _T("");
	m_PeakPagefileUsage= _T("");
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

CProcessMemInfo::~CProcessMemInfo()
{
	if (m_psapiHandle)
	{
		::FreeLibrary((HINSTANCE)m_psapiHandle);
		m_psapiHandle = NULL;
	}
	if (m_vdmdbgHandle)
	{
		::FreeLibrary((HINSTANCE)m_psapiHandle);
		m_psapiHandle = NULL;
	}
}

void CProcessMemInfo::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessMemInfo)
	DDX_Text(pDX, IDC_PAGEFAULTCOUNT, m_PageFaultCount);
	DDX_Text(pDX, IDC_PEAKWORKINGSETSIZE, m_PeakWorkingSetSize);
	DDX_Text(pDX, IDC_WORKINGSETSIZE, m_WorkingSetSize);
	DDX_Text(pDX, IDC_QUOTAPEAKPAGEDPOOLUSAGE, m_QuotaPeakPagedPoolUsage);
	DDX_Text(pDX, IDC_QUOTAPAGEDPOOLUSAGE, m_QuotaPagedPoolUsage);
	DDX_Text(pDX, IDC_QUOTAPEAKNONPAGEDPOOLUSAGE, m_QuotaPeakNonPagedPoolUsage);
	DDX_Text(pDX, IDC_QUOTANONPAGEDPOOLUSAGE, m_QuotaNonPagedPoolUsage);
	DDX_Text(pDX, IDC_PAGEFILEUSAGE, m_PagefileUsage);
	DDX_Text(pDX, IDC_PEAKPAGEFILEUSAGE, m_PeakPagefileUsage);
	DDX_Control(pDX, IDC_INVALID_WINDOW, m_InvalidWindow);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProcessMemInfo, CPropertyPage)
	//{{AFX_MSG_MAP(CProcessMemInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcessMemInfo message handlers

BOOL CProcessMemInfo::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	m_InvalidWindow.SetFontBold(TRUE);
	m_InvalidWindow.SetTextColor( 706 );	// Red color
	updateDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProcessMemInfo::updateDialog()
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
		dwThreadID = GetWindowThreadProcessId(m_pWindowInfo->m_hwnd, &dwProcessID);
		
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessID);
		if (hProcess)
		{
			// use PSAPI.DLL to collect information about the memory usage
			// of the process.  show each field in the PROCESS_MEMORY_COUNTERS structure
			PROCESS_MEMORY_COUNTERS pmc;
			if ( m_pFnGetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)))
			{
				m_PageFaultCount.Format("0x%08X (%d) ",				pmc.PageFaultCount,				pmc.PageFaultCount);
				m_PeakWorkingSetSize.Format("0x%08X %s",			pmc.PeakWorkingSetSize,			static_cast<LPCTSTR>(GetFormattedString(pmc.PeakWorkingSetSize)));
				m_WorkingSetSize.Format("0x%08X %s",				pmc.WorkingSetSize,				static_cast<LPCTSTR>(GetFormattedString(pmc.WorkingSetSize)));
				m_QuotaPeakPagedPoolUsage.Format("0x%08X %s",		pmc.QuotaPeakPagedPoolUsage,	static_cast<LPCTSTR>(GetFormattedString(pmc.QuotaPeakPagedPoolUsage)));
				m_QuotaPagedPoolUsage.Format("0x%08X %s",			pmc.QuotaPagedPoolUsage,		static_cast<LPCTSTR>(GetFormattedString(pmc.QuotaPagedPoolUsage)));
				m_QuotaPeakNonPagedPoolUsage.Format("0x%08X %s",	pmc.QuotaPeakNonPagedPoolUsage, static_cast<LPCTSTR>(GetFormattedString(pmc.QuotaPeakNonPagedPoolUsage)));
				m_QuotaNonPagedPoolUsage.Format("0x%08X %s", pmc.QuotaNonPagedPoolUsage,			static_cast<LPCTSTR>(GetFormattedString(pmc.QuotaNonPagedPoolUsage)));
				m_PagefileUsage.Format("0x%08X %s", pmc.PagefileUsage,								static_cast<LPCTSTR>(GetFormattedString(pmc.PagefileUsage)));
				m_PeakPagefileUsage.Format("0x%08X %s", pmc.PeakPagefileUsage,						static_cast<LPCTSTR>(GetFormattedString(pmc.PeakPagefileUsage)));
			
				UpdateData(FALSE);
			}
		}
	}
}

BOOL CProcessMemInfo::OnSetActive() 
{
	updateDialog();
	return CPropertyPage::OnSetActive();
}

CString CProcessMemInfo::GetFormattedString(DWORD dwNumber)
{
	CString strTemp;
	if (0 == dwNumber) {
		strTemp = _T(" (0 Kb) ");
		return strTemp;
	}
	strTemp.Format(" (%d Kb) ", dwNumber / 1000);
	return strTemp;
}