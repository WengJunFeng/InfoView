#pragma once

// CThreadPropView form view

class CThreadPropView : public CFormView
{
	DECLARE_DYNCREATE(CThreadPropView)

protected:
	CThreadPropView();           // protected constructor used by dynamic creation
	virtual ~CThreadPropView();

public:
	enum { IDD = IDD_THREADPROPVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
private:
	CBrush			m_brush;
	THREADENTRY32	m_te32;
	DWORD			m_currentThreadID;

private:
	BOOL SetCurrentThreadEntry (DWORD dwThreadID);

public:
	void RefreshDialog(DWORD dwThreadID);
	CString GetFormattedTime(SYSTEMTIME sysTime);
	void SetEditText(UINT nID, CString strData);
	BOOL ConvertFileTimeToSystemTime(FILETIME ft, SYSTEMTIME* st);
	void ShowTime(FILETIME fTime, UINT nID);
};