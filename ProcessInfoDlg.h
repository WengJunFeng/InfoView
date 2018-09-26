#pragma once


// CProcessInfoDlg dialog

class CProcessInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CProcessInfoDlg)

public:
	CProcessInfoDlg(DWORD_PTR dwData, CWnd* pParent = NULL);   // standard constructor
	virtual ~CProcessInfoDlg();
	virtual BOOL OnInitDialog( );

// Dialog Data
	enum { IDD = IDD_PROCESS_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	DWORD			m_currentProcessID;
	PROCESSENTRY32	m_pe;
private:
	CEdit* GetEditCtrl(UINT nID);
	void SetEditText(CEdit *pEdit, long lData);
	void SetEditText(CEdit *pEdit, CString strData);
	
	CString GetLastErrorString();
	void SetCurrentProcessEntry();
	void ShowHandlesCount();
	void ShowProcessTimes();
	HANDLE GetProcessHandle();
	BOOL ConvertFileTimeToSystemTime(FILETIME ft, SYSTEMTIME* st);
	CString GetFormattedTime(SYSTEMTIME sysTime);

	void ShowPriority();
	void ShowGUIObjCount();
	void ShowProcessExeName();
	void ShowThreadCount();
	void ShowPriorityBase();
	void ShowCurrentUsage();
	void ShowIOCounters();
public:
	afx_msg void OnBnClickedBtnRefresh();
	void SetProcessID(DWORD dwProcID);
	void RefreshDialog();
};