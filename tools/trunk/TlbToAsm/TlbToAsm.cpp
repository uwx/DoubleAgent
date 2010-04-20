#include "stdafx.h"
#include "TlbConverter.h"
#include "CopyAssembly.h"
#include "FixupAssembly.h"
#include "LogAssembly.h"
#include "AssemblyDlg.h"

using namespace System::Runtime::InteropServices;
using namespace System::Runtime::InteropServices::ComTypes;
using namespace System::Reflection;
using namespace System::Reflection::Emit;
using namespace DoubleAgent;
using namespace DoubleAgent::TlbToAsm;

/////////////////////////////////////////////////////////////////////////////
#pragma managed(push,off)
#ifdef	_DEBUG
#define _LOG_LEVEL_DEBUG	LogNormal
#endif
#define	_LOG_ROOT_PATH		_T("Software\\Cinnamon\\")
#define	_LOG_SECTION_NAME	_T("TlbToAsm")
#define _LOG_DEF_LOGNAME	_LOG_SECTION_NAME _T(".log")
#include "LogAccess.inl"
#include "Log.inl"
#pragma comment(lib, "advapi32.lib")
#pragma managed(pop)
/////////////////////////////////////////////////////////////////////////////

static void LoadThisAssembly (String^ pFileName, bool pLogAssembly, bool pShowAssembly)
{
	try
	{
		Assembly^		lAssembly = nullptr;
		LogAssembly^	lLogAssembly = nullptr;
		AssemblyDlg^	lAssemblyDlg = nullptr;

		LogMessage (LogNormal, _T("Load %s"), _B(pFileName));
		lAssembly = Assembly::LoadFrom (pFileName);

		if	(pLogAssembly)
		{
			lLogAssembly = gcnew LogAssembly;
			lLogAssembly->Log (lAssembly);
		}
		if	(pShowAssembly)
		{
			lAssemblyDlg = gcnew AssemblyDlg;
			lAssemblyDlg->ShowAssembly (lAssembly);
			lAssemblyDlg->ShowDialog ();
		}
	}
	catch AnyExceptionDebug
}

static Assembly^ ConvertThisTypeLib (String^ pTlbFileName, String^ pAssemblyFileName, bool pLogAssembly, bool pShowAssembly, bool pSaveAssembly = false)
{
	AssemblyBuilder^	lAssembly = nullptr;

	try
	{
		TlbConverter^	lTlbConverter = gcnew TlbConverter;
		LogAssembly^	lLogAssembly = nullptr;
		AssemblyDlg^	lAssemblyDlg = nullptr;

		lTlbConverter->mStdOleAssembly = Assembly::Load ("stdole, Version=7.0.3300.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a");
		LogMessage (LogNormal, _T("Convert %s"), _B(pTlbFileName)); 
		lAssembly = lTlbConverter->ConvertTypeLib (pTlbFileName, pAssemblyFileName);

		if	(pSaveAssembly)
		{
			LogMessage (LogNormal, _T("Save %s"), _B(pAssemblyFileName)); 
			lAssembly->Save (IO::Path::GetFileName (pAssemblyFileName));
		}

		if	(pLogAssembly)
		{
			lLogAssembly = gcnew LogAssembly;
			lLogAssembly->Log (lAssembly);
		}
		if	(pShowAssembly)
		{
			lAssemblyDlg = gcnew AssemblyDlg;
			lAssemblyDlg->ShowAssembly (lAssembly);
			lAssemblyDlg->ShowDialog ();
		}
	}
	catch AnyExceptionDebug

	return lAssembly;
}

static Assembly^ CopyThisAssembly (Assembly^ pAssembly, String^ pAssemblyName, String^ pModuleName, bool pLogAssembly, bool pShowAssembly, bool pSaveAssembly = true)
{
	Assembly^	lAssembly = nullptr;

	try
	{
		FixupAssembly^	lCopyAssembly = gcnew FixupAssembly;
		LogAssembly^	lLogAssembly = nullptr;
		AssemblyDlg^	lAssemblyDlg = nullptr;

		lAssembly = lCopyAssembly->DoCopy (pAssembly, pAssemblyName, pModuleName, pSaveAssembly);

		if	(pLogAssembly)
		{
			lLogAssembly = gcnew LogAssembly;
			lLogAssembly->Log (lAssembly);
		}
		if	(pShowAssembly)
		{
			lAssemblyDlg = gcnew AssemblyDlg;
			lAssemblyDlg->ShowAssembly (lAssembly);
			lAssemblyDlg->ShowDialog ();
		}
	}
	catch AnyExceptionDebug

	return lAssembly;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

static inline String^ CmdOpt (array <String^>^ pCmdArgs, int pCmdArgNdx)
{
	if	(
			(pCmdArgNdx < pCmdArgs->Length)
		&&	(
				(pCmdArgs[pCmdArgNdx][0] == '-')
			||	(pCmdArgs[pCmdArgNdx][0] == '/')
			)
		)
	{
		return pCmdArgs[pCmdArgNdx]->Substring (1);
	}
	return nullptr;
}

static inline String^ CmdArg (array <String^>^ pCmdArgs, int pCmdArgNdx)
{
	if	(
			(pCmdArgNdx < pCmdArgs->Length)
		&&	(pCmdArgs[pCmdArgNdx][0] != '-')
		&&	(pCmdArgs[pCmdArgNdx][0] != '/')
		)
	{
		return pCmdArgs[pCmdArgNdx];
	}
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////

int main(array<System::String ^> ^args)
{
	bool	lRestartLog = true;
#ifdef	_DEBUG
	LogStart (true);
	lRestartLog = false;
#endif	

	try
	{
		array <String^>^	lCmdArgs = Environment::GetCommandLineArgs ();
		int					lCmdArgNdx;
		String^				lLastCmdArg;
		Assembly^			lAssembly = nullptr;
		
		for	(lCmdArgNdx = 1; lCmdArgNdx < lCmdArgs->Length; lCmdArgNdx++)
		{
			String^	lCmdOpt = CmdOpt (lCmdArgs, lCmdArgNdx);
			
			if	(lCmdOpt == nullptr)
			{
				lLastCmdArg = lCmdArgs[lCmdArgNdx];
				continue;
			}

			if	(String::Compare (lCmdOpt, "Convert", true) == 0)
			{
				try
				{
					String^	lSourcePath;
					String^	lTargetPath;
					bool	lLog = false;
					bool	lShow = false;
					bool	lSave = false;
						
					while (lCmdOpt = CmdOpt (lCmdArgs, lCmdArgNdx+1))
					{
						if	(String::Compare (lCmdOpt, "Log", true) == 0)
						{
							lCmdArgNdx++;
							lLog = true;
							LogStart (lRestartLog);
							lRestartLog = false;
						}
						else
						if	(String::Compare (lCmdOpt, "Show", true) == 0)
						{
							lCmdArgNdx++;
							lShow = true;
						}
						else
						if	(String::Compare (lCmdOpt, "Save", true) == 0)
						{
							lCmdArgNdx++;
							lSave = true;
						}
						else
						{
							break;
						}
					}

					if	(lSourcePath = CmdArg (lCmdArgs, lCmdArgNdx+1))
					{
						lCmdArgNdx++;
						if	(lTargetPath = CmdArg (lCmdArgs, lCmdArgNdx+1))
						{
							lCmdArgNdx++;
						}
						else
						{
							lTargetPath = IO::Path::ChangeExtension (IO::Path::GetTempFileName(), ".dll");
						}
						if	(lSave)
						{
							lLastCmdArg = lTargetPath;
						}
						lAssembly = ConvertThisTypeLib (lSourcePath, lTargetPath, lLog, lShow, lSave);
					}
				}
				catch AnyExceptionDebug
			}
			else
			if	(String::Compare (lCmdOpt, "Fix", true) == 0)
			{
				try
				{
					String^	lSourcePath;
					String^	lTargetPath;
					String^	lNamespace ("DoubleAgent");
					bool	lLog = false;
					bool	lShow = false;
					bool	lSave = false;
						
					while (lCmdOpt = CmdOpt (lCmdArgs, lCmdArgNdx+1))
					{
						if	(String::Compare (lCmdOpt, "Log", true) == 0)
						{
							lCmdArgNdx++;
							lLog = true;
							LogStart (lRestartLog);
							lRestartLog = false;
						}
						else
						if	(String::Compare (lCmdOpt, "Show", true) == 0)
						{
							lCmdArgNdx++;
							lShow = true;
						}
						else
						if	(String::Compare (lCmdOpt, "Save", true) == 0)
						{
							lCmdArgNdx++;
							lSave = true;
						}
						else
						if	(String::Compare (lCmdOpt, "Namespace", true) == 0)
						{
							lCmdArgNdx++;
							lNamespace = CmdArg (lCmdArgs, ++lCmdArgNdx);
						}
						else
						{
							break;
						}
					}

					if	(lSourcePath = CmdArg (lCmdArgs, lCmdArgNdx+1))
					{
						lCmdArgNdx++;
						if	(lTargetPath = CmdArg(lCmdArgs,lCmdArgNdx+1))
						{
							lCmdArgNdx++;
							lLastCmdArg = lTargetPath;
						}
						else
						{
							lTargetPath = lSourcePath;
							lSourcePath = lLastCmdArg;
						}
						if	(lSourcePath)
						{
							LogMessage (LogNormal, _T("Load %s"), _B(lSourcePath));
							lAssembly = Assembly::LoadFrom (lSourcePath);
						}
						CopyThisAssembly (lAssembly, lTargetPath, lNamespace, lLog, lShow, lSave);
					}
				}
				catch AnyExceptionDebug
			}
			else
			if	(String::Compare (lCmdOpt, "Log", true) == 0)
			{
				try
				{
					LogStart (lRestartLog);
					lRestartLog = false;
					lLastCmdArg = CmdArg(lCmdArgs,lCmdArgNdx+1) ? CmdArg(lCmdArgs,++lCmdArgNdx) : lLastCmdArg;
					LoadThisAssembly (lLastCmdArg, true, false);
				}
				catch AnyExceptionDebug
			}
			else
			if	(String::Compare (lCmdOpt, "Show", true) == 0)
			{
				try
				{
					lLastCmdArg = CmdArg(lCmdArgs,lCmdArgNdx+1) ? CmdArg(lCmdArgs,++lCmdArgNdx) : lLastCmdArg;
					LoadThisAssembly (lLastCmdArg, false, true);
				}
				catch AnyExceptionDebug
			}
			else
			if	(
					(String::Compare (lCmdOpt, "LogShow", true) == 0)
				||	(String::Compare (lCmdOpt, "Log+Show", true) == 0)
				)
			{
				try
				{
					LogStart (lRestartLog);
					lRestartLog = false;
					lLastCmdArg = CmdArg(lCmdArgs,lCmdArgNdx+1) ? CmdArg(lCmdArgs,++lCmdArgNdx) : lLastCmdArg;
					LoadThisAssembly (lLastCmdArg, true, true);
				}
				catch AnyExceptionDebug
			}
		}

/*
/convert /log /show "C:\DoubleAgent\Resources\DaServer(x86).tlb" "C:\BinDa\Bin\Debug(x86)\Interop\TempAsm.dll"
/fix /namespace DoubleAgent.Server /save "C:\BinDa\Bin\Debug(x86)\Interop\DoubleAgentSvr.dll" "C:\BinDa\Bin\Debug(x86)\Interop\DupAsm.dll" /log /show 
/log /show "C:\BinDa\Bin\Debug(x86)\Interop\DoubleAgentSvr.dll"
/log /show "C:\BinDa\Bin\Debug(x86)\Interop\DoubleAgentCtl.dll"
/log /show "C:\BinDa\Bin\Debug(x86)\Interop\DaControl.NET.dll"
/log /show "C:\BinDa\Bin\Debug(x86)\Interop\AxDoubleAgentCtl.dll"
/log /show "C:\Program Files\Double Agent\Dev\DoubleAgentSvr.dll"
*/
	}
	catch AnyExceptionDebug
	{}

    LogStop (LogIfActive);
    return 0;
}
