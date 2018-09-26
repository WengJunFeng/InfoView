#ifndef __REGISTRY_H
#define __REGISTRY_H

/////////////////////////////////////////////////////////////////////////////
//	Registry.h : Interface of the CRegistry class
//	CRegistry is a wrapper for the Windows Registry API.  It allows
//  easy modification of the Registry with easy to remember terms like
//  Read, Write, Open, and Close.
/////////////////////////////////////////////////////////////////////////////

#ifndef _WINREG_
#include <winreg.h>
#endif

#define REG_RECT	0x0001
#define REG_POINT	0x0002

class CRegistry : public CObject
{

public:
	// Constructor
	CRegistry(HKEY hKeyRoot = HKEY_LOCAL_MACHINE);
	// Destructor
	~CRegistry();


public:
	// Operations
	BOOL VerifyKey (HKEY hKeyRoot, LPCTSTR pszPath);
	BOOL VerifyKey (LPCTSTR pszPath);
	BOOL VerifyValue (LPCTSTR pszValue);
	BOOL CreateKey (HKEY hKeyRoot, LPCTSTR pszPath);
	BOOL Open (HKEY hKeyRoot, LPCTSTR pszPath);
	void Close();

	BOOL DeleteValue (LPCTSTR pszValue);
	BOOL DeleteValueKey (HKEY hKeyRoot, LPCTSTR pszPath);

	BOOL Write (LPCTSTR pszKey, int iVal);
	BOOL Write (LPCTSTR pszKey, DWORD dwVal);
	BOOL Write (LPCTSTR pszKey, LPCTSTR pszVal);
	BOOL Write (LPCTSTR pszKey, CStringList& scStringList);
	BOOL Write (LPCTSTR pszKey, CByteArray& bcArray);
	BOOL Write (LPCTSTR pszKey, CStringArray& scArray);
	BOOL Write (LPCTSTR pszKey, CDWordArray& dwcArray);
	BOOL Write (LPCTSTR pszKey, CWordArray& wcArray);
	BOOL Write (LPCTSTR pszKey, LPCRECT rcRect);
	BOOL Write (LPCTSTR pszKey, LPPOINT& lpPoint);

	BOOL Read (LPCTSTR pszKey, int& iVal);
	BOOL Read (LPCTSTR pszKey, DWORD& dwVal);
	BOOL Read (LPCTSTR pszKey, CString& sVal);
	BOOL Read (LPCTSTR pszKey, CStringList& scStringList);
	BOOL Read (LPCTSTR pszKey, CStringArray& scArray);
	BOOL Read (LPCTSTR pszKey, CDWordArray& dwcArray);
	BOOL Read (LPCTSTR pszKey, CWordArray& wcArray);
	BOOL Read (LPCTSTR pszKey, CByteArray& bcArray);
	BOOL Read (LPCTSTR pszKey, LPPOINT& lpPoint);
	BOOL Read (LPCTSTR pszKey, LPRECT& rcRect);

	LONG  GetRegInfoMessage() const;
	DWORD GetRegInfoType() const;
	DWORD GetRegInfoSize() const;
	DWORD GetRegKeyType(LPCTSTR pszKey) const;

protected:
	// Member Variables
	HKEY 	m_hKey;
	CString m_sPath;

	struct REGINFO
	{
		LONG lMessage;
		DWORD dwType;
		DWORD dwSize;
	} m_Info;
};

#endif // __REGISTRY_H_