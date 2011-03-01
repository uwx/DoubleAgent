// MsaWithDa_C8_ATL8.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
#include "MsaWithDaDlg_C8_ATL8.h"
#pragma comment(lib, "comctl32.lib")

class CMsaWithDaModule : public CAtlExeModuleT< CMsaWithDaModule >
{
public :
	HRESULT Run(int nShowCmd = SW_HIDE)
	{
		INITCOMMONCONTROLSEX lInitControls;
		memset (&lInitControls, 0, sizeof (lInitControls));
		lInitControls.dwSize = sizeof (lInitControls);
		lInitControls.dwICC = ICC_WIN95_CLASSES;
		InitCommonControlsEx (&lInitControls);

		CMsaWithDaDlg Dlg;
		Dlg.DoModal ();
		return S_OK;
	}
};

CMsaWithDaModule _AtlModule;

//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    return _AtlModule.WinMain(nShowCmd);
}

