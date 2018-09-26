#ifndef __HHOOK_H__
#define __HHOOK_H__

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>

#ifdef _AFXDLL
	#define FUNCTION_DECL  extern "C" __declspec(dllimport)	// Imported function declaration
#else
	#define FUNCTION_DECL __declspec(dllexport)				// Exported function declaration
#endif

#pragma warning(disable: 4100)	// unreferenced formal parameter
#pragma warning(disable: 4201)	// nonstandard extension used : nameless struct/union

	FUNCTION_DECL void WINAPI ReleaseKeybHook(void);
	FUNCTION_DECL void WINAPI InstallKeybHook(HWND hwnd, HWND outputtextView);
	LRESULT	CALLBACK KeybProc( int nCode, WPARAM wParam, LPARAM lParam );

	// Functions for getting password string from a given password edit control's handle
	FUNCTION_DECL void WINAPI FetchText( HWND hwnd );
	FUNCTION_DECL void WINAPI EndFetchTextHook( void );
	LRESULT	CALLBACK GetTextWndProc( int nCode, WPARAM wParam, LPARAM lParam );

	// Functions for getting DLL's list of a given application's handle
	FUNCTION_DECL void WINAPI InstallGetMsgHook(HWND hwnd, HWND hwndOutput);
	FUNCTION_DECL void WINAPI RemoveGetMsgHook( void );
	LRESULT	CALLBACK GetMsgProc( int nCode, WPARAM wParam, LPARAM lParam );

	// Functions for showing DLL's list for a specific application
	FUNCTION_DECL BOOL WINAPI ShowDllList(HWND hwnd, HWND hwndOutput);
	LRESULT	CALLBACK ShowDllListProc( int nCode, WPARAM wParam, LPARAM lParam );
	FUNCTION_DECL void WINAPI ReleaseDLLHook(void);

	FUNCTION_DECL void WINAPI GetThreadsList(DWORD dwPid, HWND hwnd, HWND hwndOutput);
	LRESULT	CALLBACK GetThreadsListProc( int nCode, WPARAM wParam, LPARAM lParam );

	// Functions for watching Posted messages for a given window
	FUNCTION_DECL void WINAPI ShowMessages(HWND hwnd, HWND hwndOutput);
	FUNCTION_DECL void WINAPI StopShowingMessages( void );
	LRESULT	CALLBACK ShowMessageProc( int nCode, WPARAM wParam, LPARAM lParam );
	LRESULT	CALLBACK CallWndProc( int nCode, WPARAM wParam, LPARAM lParam );
	LRESULT	CALLBACK CallWndProcRet( int nCode, WPARAM wParam, LPARAM lParam );

	// Helper function to show extra information
	FUNCTION_DECL void WINAPI ShowExtraInfo(UINT message, const char* pMsgType, WPARAM wParam, LPARAM lParam);

	BOOL getRegParam(int* nKeyValue, const char* strKeyName);

	void ShowOnListView(const char* msgName, const char* pMsgType, WPARAM wParam, LPARAM lParam);
	void showDLLList(const char *pStr);
	void ShowLastError(const char* pErrorTitle);

	typedef struct
	{
		TCHAR	strMsgName[256];
		TCHAR	strMsgType[256];
		HWND	hwndFrom;
		WPARAM	wParam;
		LPARAM	lParam;
		long	nMsgDepth;
	}INFO_COPYDATA, *LPINFO_COPYDATA;

	typedef struct
	{
		TCHAR	strDllPath[1024];
		HWND	hwndFrom;
	}DLL_LIST, *LPDLL_LIST;
#endif // __HHOOK_H__