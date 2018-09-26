#pragma once

// CProcessPropView form view

class CProcessPropView : public CFormView
{
	DECLARE_DYNCREATE(CProcessPropView)

protected:
	CProcessPropView();           // protected constructor used by dynamic creation
	virtual ~CProcessPropView();
	virtual void OnInitialUpdate();

public:
	enum { IDD = IDD_PROCESS_INFO_FORM };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
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
	void SetProcessID(DWORD dwProcID);
	void RefreshDialog();

private:
	DWORD			m_currentProcessID;
	PROCESSENTRY32	m_pe;
	CBrush			m_brush;
};