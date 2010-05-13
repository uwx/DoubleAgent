#include "stdafx.h"
#include "TlbConverter.h"
#include "CopyAssembly.h"
#include "FixupAssembly.h"
#include "LogAssembly.h"
#include "AssemblyDlg.h"
#include "DaVersion.h"

using namespace System::Runtime::InteropServices;
using namespace System::Runtime::InteropServices::ComTypes;
using namespace System::Reflection;
using namespace System::Reflection::Emit;
using namespace System::Security::Cryptography::X509Certificates;
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
#pragma page()
/////////////////////////////////////////////////////////////////////////////

static StrongNameKeyPair^ GetStrongName (String^ pKeyFilePath)
{
	StrongNameKeyPair^	lStrongName = nullptr;

	try
	{
		IO::FileStream^ lFile = gcnew IO::FileStream (pKeyFilePath, IO::FileMode::Open, IO::FileAccess::Read);
		lStrongName = gcnew StrongNameKeyPair (lFile);
		LogMessage (LogNormal, _T("StrongName [%s] [%d]"), _B(lFile->Name), lStrongName->PublicKey->Length);
	}
	catch AnyExceptionDebug

	return lStrongName;
}

static bool MarkPrimaryAssembly (AssemblyBuilder^ pAssembly)
{
	bool	lRet = false;

	try
	{
		TypeLibVersionAttribute^	lTypeLibVersionAttribute;
		CustomAttributeBuilder^		lAttributeBuilder;
		array<Type^>^				lTypes = gcnew array<Type^> (2);
		array<Object^>^				lVersion = gcnew array<Object^> (2);

		lTypeLibVersionAttribute = safe_cast <TypeLibVersionAttribute^> (Attribute::GetCustomAttribute (pAssembly, TypeLibVersionAttribute::typeid));
		if	(lTypeLibVersionAttribute)
		{
			lTypes [0] = Int32::typeid;
			lTypes [1] = Int32::typeid;
			lVersion [0] = gcnew Int32(lTypeLibVersionAttribute->MajorVersion);
			lVersion [1] = gcnew Int32(lTypeLibVersionAttribute->MinorVersion);
			lAttributeBuilder = gcnew CustomAttributeBuilder (PrimaryInteropAssemblyAttribute::typeid->GetConstructor(lTypes), lVersion);
			pAssembly->SetCustomAttribute (lAttributeBuilder);
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

static void LoadThisAssembly (String^ pFileName, bool pLogAssembly, bool pLogCode, bool pShowAssembly)
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
			lLogAssembly->Log (lAssembly, pLogCode);
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

static Assembly^ ConvertThisTypeLib (String^ pTlbFileName, String^ pAssemblyFileName, StrongNameKeyPair^ pStrongName, bool pLogAssembly, bool pShowAssembly, bool pSaveAssembly = false)
{
	Assembly^	lRet = nullptr;

	try
	{
		AssemblyBuilder^	lAssemblyBuilder = nullptr;
		TlbConverter^		lTlbConverter = gcnew TlbConverter;
		LogAssembly^		lLogAssembly = nullptr;
		AssemblyDlg^		lAssemblyDlg = nullptr;

		lTlbConverter->mStdOleAssembly = Assembly::Load ("stdole, Version=7.0.3300.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a");
		LogMessage (LogNormal, _T("Convert %s"), _B(pTlbFileName));
		if	(lAssemblyBuilder = lTlbConverter->ConvertTypeLib (pTlbFileName, pAssemblyFileName, pStrongName))
		{
			if	(pSaveAssembly)
			{
				LogMessage (LogNormal, _T("Save %s"), _B(pAssemblyFileName));
				lAssemblyBuilder->Save (IO::Path::GetFileName (pAssemblyFileName));
			}

			if	(pLogAssembly)
			{
				lLogAssembly = gcnew LogAssembly;
				lLogAssembly->Log (lAssemblyBuilder, false);
			}
			if	(pShowAssembly)
			{
				lAssemblyDlg = gcnew AssemblyDlg;
				lAssemblyDlg->ShowAssembly (lAssemblyBuilder);
				lAssemblyDlg->ShowDialog ();
			}
			lRet = lAssemblyBuilder;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

static AssemblyBuilder^ CopyThisAssembly (Assembly^ pAssembly, String^ pAssemblyName, String^ pModuleName, StrongNameKeyPair^ pStrongName, bool pLogAssembly, bool pShowAssembly, bool pSaveAssembly = true)
{
	AssemblyBuilder^	lRet = nullptr;

	try
	{
		AssemblyBuilder^	lAssemblyBuilder = nullptr;
		FixupAssembly^		lCopyAssembly = gcnew FixupAssembly;
		LogAssembly^		lLogAssembly = nullptr;
		AssemblyDlg^		lAssemblyDlg = nullptr;

		if	(lAssemblyBuilder = lCopyAssembly->DoCopy (pAssembly, pAssemblyName, pModuleName, pStrongName))
		{
			lAssemblyBuilder->DefineVersionInfoResource (_DOUBLEAGENT_NAME, _DOUBLEAGENT_VERSION_STR, _DOUBLEAGENT_COMPANY, _DOUBLEAGENT_COPYRIGHT, nullptr);

			if	(pStrongName)
			{
				MarkPrimaryAssembly (lAssemblyBuilder);
			}
			if	(pSaveAssembly)
			{
				LogMessage (LogNormal, _T("Save %s"), _B(pAssemblyName));
				lAssemblyBuilder->Save (IO::Path::GetFileName (pAssemblyName));
			}

			if	(pLogAssembly)
			{
				lLogAssembly = gcnew LogAssembly;
				lLogAssembly->Log (lAssemblyBuilder, false);
			}
			if	(pShowAssembly)
			{
				lAssemblyDlg = gcnew AssemblyDlg;
				lAssemblyDlg->ShowAssembly (lAssemblyBuilder);
				lAssemblyDlg->ShowDialog ();
			}

			lRet = lAssemblyBuilder;
		}
	}
	catch AnyExceptionDebug

	return lRet;
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
		StrongNameKeyPair^	lStrongName = nullptr;

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
						if	(String::Compare (lCmdOpt, "Keyfile", true) == 0)
						{
							lCmdArgNdx++;
							lStrongName = GetStrongName (CmdArg (lCmdArgs, ++lCmdArgNdx));
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
						lAssembly = ConvertThisTypeLib (lSourcePath, lTargetPath, lStrongName, lLog, lShow, lSave);
						if	(!lAssembly)
						{
							return 8;
						}
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
						if	(String::Compare (lCmdOpt, "Keyfile", true) == 0)
						{
							lCmdArgNdx++;
							lStrongName = GetStrongName (CmdArg (lCmdArgs, ++lCmdArgNdx));
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
						}
						else
						{
							lTargetPath = lSourcePath;
							lSourcePath = lLastCmdArg;
						}
						lLastCmdArg = lTargetPath;
						if	(lSourcePath)
						{
							LogMessage (LogNormal, _T("Load %s"), _B(lSourcePath));
							lAssembly = Assembly::LoadFrom (lSourcePath);
						}
						if	(!CopyThisAssembly (lAssembly, lTargetPath, lNamespace, lStrongName, lLog, lShow, lSave))
						{
							return 8;
						}
					}
				}
				catch AnyExceptionDebug
			}
			else
			if	(String::Compare (lCmdOpt, "Log", true) == 0)
			{
				try
				{
					bool	lLogCode = false;

					if	(
							(lCmdOpt = CmdOpt (lCmdArgs, lCmdArgNdx+1))
						&&	(String::Compare (lCmdOpt, "Code", true) == 0)
						)
					{
						lCmdArgNdx++;
						lLogCode = true;
					}
					LogStart (lRestartLog);
					lRestartLog = false;
					lLastCmdArg = CmdArg(lCmdArgs,lCmdArgNdx+1) ? CmdArg(lCmdArgs,++lCmdArgNdx) : lLastCmdArg;
					LoadThisAssembly (lLastCmdArg, true, lLogCode, false);
				}
				catch AnyExceptionDebug
			}
			else
			if	(String::Compare (lCmdOpt, "Show", true) == 0)
			{
				try
				{
					lLastCmdArg = CmdArg(lCmdArgs,lCmdArgNdx+1) ? CmdArg(lCmdArgs,++lCmdArgNdx) : lLastCmdArg;
					LoadThisAssembly (lLastCmdArg, false, false, true);
				}
				catch AnyExceptionDebug
			}
		}

/*
/convert /log /show "C:\DoubleAgent\Resources\DaServer(x86).tlb" "C:\BinDa\Bin\Debug(x86)\Interop\Test.dll"
/convert /save "C:\DoubleAgent\Resources\DaServer(x86).tlb" /fix /save /namespace DoubleAgent.Server "C:\BinDa\Bin\Debug(x86)\Interop\DoubleAgent.Server.dll" /log
/fix /namespace DoubleAgent.Server /save "C:\BinDa\Bin\Debug(x86)\Interop\DoubleAgentSvr.dll" "C:\BinDa\Bin\Debug(x86)\Interop\DoubleAgent.Server.dll" /log /show
/fix /namespace DoubleAgent.Control /save "C:\BinDa\Bin\Debug(x86)\Interop\DoubleAgentCtl.dll" "C:\BinDa\Bin\Debug(x86)\Interop\DoubleAgent.Control.dll" /log /show
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
