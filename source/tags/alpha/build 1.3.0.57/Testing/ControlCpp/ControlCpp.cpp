#include "StdAfx.h"
#include "DaGuid.h"
#include "DaVersion.h"
#include "ControlCpp.h"
#include "ControlCppDlg.h"
#include "WerOpt.h"

#pragma comment(lib, "comctl32.lib")

/////////////////////////////////////////////////////////////////////////////
#define _LOG_LEVEL_DEBUG		LogNormal
#define	_LOG_ROOT_PATH			_T("Software\\")_T(_DOUBLEAGENT_NAME)_T("\\")
#define	_LOG_SECTION_NAME		_T("ControlCpp")
#define _LOG_DEF_LOGNAME		_T("ControlCpp.log")
#include "Log.inl"
/////////////////////////////////////////////////////////////////////////////

CControlCppMod					_AtlModule;
LPCTSTR __declspec(selectany)	_AtlProfileName = _LOG_SECTION_NAME;
LPCTSTR __declspec(selectany)	_AtlProfilePath = _LOG_ROOT_PATH;

/////////////////////////////////////////////////////////////////////////////

CControlCppMod::CControlCppMod()
{
	LogStart (true);
	WerOptOut (true);
}

CControlCppMod::~CControlCppMod()
{
	LogStop (LogIfActive);
}

HRESULT CControlCppMod::Run (int nShowCmd)
{
	CControlCppDlg	lDlg;

	tSS <INITCOMMONCONTROLSEX, DWORD> lInitControls;
	lInitControls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx (&lInitControls);

	lDlg.DoModal ();

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

extern "C" int WINAPI _tWinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{
    return _AtlModule.WinMain (nShowCmd);
}
