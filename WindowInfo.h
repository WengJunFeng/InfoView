#ifndef __WINDOWINFO_H__
#define __WINDOWINFO_H__

class WindowInfo : public CObject
{
public:
	HWND		m_hwnd;
	DWORD		m_pid;
	TCHAR		m_windowTitle[1024];
	TCHAR		m_windowClassName[512];
	DWORD		m_noOfThreads;
	DWORD		m_parendPID;
	LONG		m_threadPriority;
	DWORD		m_usageCount;
	TCHAR		m_exeFile[256];

public:
	WindowInfo();
	~WindowInfo();

	WindowInfo(const WindowInfo& other);
	WindowInfo& operator = (const WindowInfo& other); 		// copy operator

	// member functions
	TCHAR* getWindowTitle();
	TCHAR* getClassName();

	void setClassName(const HWND& hwnd);
	void setWindowTitle(const HWND& hwnd);
};

#endif	// __WINDOWINFO_H__