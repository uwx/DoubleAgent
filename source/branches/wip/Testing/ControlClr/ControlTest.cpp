#include "StdAfx.h"

/////////////////////////////////////////////////////////////////////////////
#pragma managed(push,off)
#ifdef	_DEBUG
#define _LOG_LEVEL_DEBUG	LogNormal
#endif
#define	_LOG_ROOT_PATH		_T("Software\\")_T(_DOUBLEAGENT_NAME)_T("\\")
#define	_LOG_SECTION_NAME	_T(_CONTROL_REGNAME)
#define _LOG_DEF_LOGNAME	_T(_DOUBLEAGENT_NAME) _T(".log")
#define	_LOG_PREFIX			_T("Test ")
#include "LogAccess.inl"
#include "Log.inl"
#pragma comment(lib, "advapi32.lib")
#pragma managed(pop)
/////////////////////////////////////////////////////////////////////////////

#include "ControlTestForm.h"
using namespace DoubleAgent;
using namespace DoubleAgent::Control;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	LogStart (true);
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew ControlTestForm());
	LogStop (LogIfActive);
	return 0;
}
