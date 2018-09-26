#ifndef __ENUMPROCESS_H__
#define __ENUMPROCESS_H__

class CEnumProcess
{
public:
    virtual BOOL ShowProcesses() = 0;
protected:
    void DisplayProcess(HWND outPutWindowHandle, char const *pFileName, DWORD dwProcessID)
	{
		if (::IsWindow(outPutWindowHandle) && outPutWindowHandle != NULL)
		{
			CString strPID = _T(""), strExeName = _T(""), strTemp = _T("");
			strPID.Format("PID: 0x%8x", dwProcessID);
			strExeName.Format("%s", pFileName);
			strTemp = strPID + _T(" | ") + strExeName;
			::SendMessage(outPutWindowHandle, LB_ADDSTRING, 0, (LPARAM)strTemp.GetBuffer(255));
			strTemp.ReleaseBuffer();
		}
    }
};

#endif	// __ENUMPROCESS_H__