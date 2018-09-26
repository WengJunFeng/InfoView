////////////////////////////////////////////////////////////////
// CWindowInfo
// If this code works, it was written by Suresh.


#include "stdafx.h"
#include "WindowInfo.h"

// constructor
WindowInfo::WindowInfo()
{
	m_hwnd			= NULL;
	m_pid			= 0;
	m_noOfThreads		= 1;
	m_parendPID			= 0;
	m_threadPriority	= 0;
	m_usageCount		= 0;
	_tcscpy(m_exeFile, _T(""));
	_tcscpy(m_windowTitle, _T(""));
	_tcscpy(m_windowClassName, _T(""));
}

WindowInfo::~WindowInfo()
{
	// default destructor
}

WindowInfo& WindowInfo::operator = (const WindowInfo& other)
{
	m_hwnd			= other.m_hwnd;
	m_pid			= other.m_pid;
	_tcscpy(m_windowTitle, other.m_windowTitle);
	_tcscpy(m_windowClassName, other.m_windowClassName);

	m_noOfThreads		= other.m_noOfThreads;
	m_parendPID			= other.m_parendPID;
	m_threadPriority	= other.m_threadPriority;
	m_usageCount		= other.m_usageCount;
	_tcscpy(m_exeFile, other.m_exeFile);

	return *this;
}

WindowInfo::WindowInfo(const WindowInfo& other)
{
	(*this) = other;
}

TCHAR* WindowInfo::getClassName()
{
	return m_windowClassName;
}

TCHAR* WindowInfo::getWindowTitle()
{
	return m_windowTitle;
}

void WindowInfo::setClassName(const HWND& hwnd)
{
	if (::IsWindow(hwnd))
	{
		::GetClassName(hwnd, m_windowClassName, 511 );
	}
	else
	{
		_tcscpy(m_windowClassName, _T(""));
	}
}

void WindowInfo::setWindowTitle(const HWND& hwnd)
{
	::GetWindowText(hwnd, m_windowTitle, 1023);
}