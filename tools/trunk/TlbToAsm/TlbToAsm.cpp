#include "stdafx.h"
#include "TlbToAsm.h"
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

int main(array<System::String ^> ^args)
{
	bool								lRestartLog = true;
	DoubleAgent::TlbToAsm::TlbToAsm^	lTlbToAsm;

#ifdef	_DEBUG
	LogStart (true);
	lRestartLog = false;
#endif

	lTlbToAsm = gcnew DoubleAgent::TlbToAsm::TlbToAsm (lRestartLog);
	lTlbToAsm->ProcessCmdLine (Environment::GetCommandLineArgs ());

    LogStop (LogIfActive);
    return 0;
}

/////////////////////////////////////////////////////////////////////////////
namespace DoubleAgent {
namespace TlbToAsm {
/////////////////////////////////////////////////////////////////////////////

int TlbToAsm::ProcessCmdLine (array <String^>^ pCmdArgs)
{
	int	lRet = 0;

	try
	{
		int			lCmdArgNdx;
		String^		lLastCmdArg;
		Assembly^	lAssembly = nullptr;

		for	(lCmdArgNdx = 1; lCmdArgNdx < pCmdArgs->Length; lCmdArgNdx++)
		{
			String^	lCmdOpt = CmdOpt (pCmdArgs, lCmdArgNdx);

			if	(lCmdOpt == nullptr)
			{
				lLastCmdArg = pCmdArgs[lCmdArgNdx];
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

					while (lCmdOpt = CmdOpt (pCmdArgs, lCmdArgNdx+1))
					{
						if	(String::Compare (lCmdOpt, "Log", true) == 0)
						{
							lCmdArgNdx++;
							lLog = true;
							LogStart (mRestartLog);
							mRestartLog = false;
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
							mStrongName = GetStrongName (CmdArg (pCmdArgs, ++lCmdArgNdx));
						}
						else
						{
							break;
						}
					}

					if	(lSourcePath = CmdArg (pCmdArgs, lCmdArgNdx+1))
					{
						lCmdArgNdx++;
						if	(lTargetPath = CmdArg (pCmdArgs, lCmdArgNdx+1))
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

						if	(lAssembly = ConvertTypeLib (lSourcePath, lTargetPath, lSave))
						{
							if	(lLog)
							{
								LogAssembly (lAssembly, false);
							}
							if	(lShow)
							{
								ShowAssembly (lAssembly);
							}
						}
						else
						{
							lRet = 8;
							break;
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

					while (lCmdOpt = CmdOpt (pCmdArgs, lCmdArgNdx+1))
					{
						if	(String::Compare (lCmdOpt, "Log", true) == 0)
						{
							lCmdArgNdx++;
							lLog = true;
							LogStart (mRestartLog);
							mRestartLog = false;
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
							lNamespace = CmdArg (pCmdArgs, ++lCmdArgNdx);
						}
						else
						if	(String::Compare (lCmdOpt, "Keyfile", true) == 0)
						{
							lCmdArgNdx++;
							mStrongName = GetStrongName (CmdArg (pCmdArgs, ++lCmdArgNdx));
						}
						else
						{
							break;
						}
					}

					if	(lSourcePath = CmdArg (pCmdArgs, lCmdArgNdx+1))
					{
						lCmdArgNdx++;
						if	(lTargetPath = CmdArg(pCmdArgs,lCmdArgNdx+1))
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

						if	(lAssembly = FixAssembly (lAssembly, lTargetPath, lNamespace, lSave))
						{
							if	(lLog)
							{
								LogAssembly (lAssembly, false);
							}
							if	(lShow)
							{
								ShowAssembly (lAssembly);
							}
						}
						else
						{
							lRet = 8;
							break;
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
							(lCmdOpt = CmdOpt (pCmdArgs, lCmdArgNdx+1))
						&&	(String::Compare (lCmdOpt, "Code", true) == 0)
						)
					{
						lCmdArgNdx++;
						lLogCode = true;
					}
					LogStart (mRestartLog);
					mRestartLog = false;
					lLastCmdArg = CmdArg(pCmdArgs,lCmdArgNdx+1) ? CmdArg(pCmdArgs,++lCmdArgNdx) : lLastCmdArg;
					OutputAssembly (lLastCmdArg, true, lLogCode, false);
				}
				catch AnyExceptionDebug
			}
			else
			if	(String::Compare (lCmdOpt, "Show", true) == 0)
			{
				try
				{
					lLastCmdArg = CmdArg(pCmdArgs,lCmdArgNdx+1) ? CmdArg(pCmdArgs,++lCmdArgNdx) : lLastCmdArg;
					OutputAssembly (lLastCmdArg, false, false, true);
				}
				catch AnyExceptionDebug
			}
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

String^ TlbToAsm::CmdOpt (array <String^>^ pCmdArgs, int pCmdArgNdx)
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

String^ TlbToAsm::CmdArg (array <String^>^ pCmdArgs, int pCmdArgNdx)
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
#pragma page()
/////////////////////////////////////////////////////////////////////////////

AssemblyBuilder^ TlbToAsm::FixAssembly (Assembly^ pAssembly, String^ pAssemblyName, String^ pModuleName, bool pSaveAssembly)
{
	AssemblyBuilder^	lRet = nullptr;

	try
	{
		AssemblyBuilder^	lAssemblyBuilder = nullptr;
		Assembly^			lSavedAssembly;

		if	(!mCopyAssembly)
		{
			mCopyAssembly = gcnew CopyAssembly;
			mCopyAssembly->mFixups = gcnew FixupAssembly (mCopyAssembly);
		}

		if	(lAssemblyBuilder = mCopyAssembly->DoCopy (pAssembly, pAssemblyName, pModuleName, mStrongName))
		{
			lAssemblyBuilder->DefineVersionInfoResource (_DOUBLEAGENT_NAME, _DOUBLEAGENT_VERSION_STR, _DOUBLEAGENT_COMPANY, _DOUBLEAGENT_COPYRIGHT, nullptr);

			if	(mStrongName)
			{
				MarkPrimaryAssembly (lAssemblyBuilder);
			}
			if	(pSaveAssembly)
			{
				LogMessage (LogNormal, _T("Save %s"), _B(pAssemblyName));
				lAssemblyBuilder->Save (IO::Path::GetFileName (pAssemblyName));

				try
				{
					if	(lSavedAssembly = Assembly::LoadFrom (pAssemblyName))
					{
						mCopyAssembly->mSavedAssemblies->Add (lSavedAssembly);
					}
				}
				catch AnyExceptionSilent
			}
			lRet = lAssemblyBuilder;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

Assembly^ TlbToAsm::ConvertTypeLib (String^ pTlbFileName, String^ pAssemblyFileName, bool pSaveAssembly)
{
	Assembly^	lRet = nullptr;

	try
	{
		AssemblyBuilder^	lAssemblyBuilder = nullptr;
		TlbConverter^		lTlbConverter = gcnew TlbConverter;

		lTlbConverter->mStdOleAssembly = Assembly::Load ("stdole, Version=7.0.3300.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a");
		LogMessage (LogNormal, _T("Convert %s"), _B(pTlbFileName));
		if	(lAssemblyBuilder = lTlbConverter->ConvertTypeLib (pTlbFileName, pAssemblyFileName, mStrongName))
		{
			if	(pSaveAssembly)
			{
				LogMessage (LogNormal, _T("Save %s"), _B(pAssemblyFileName));
				lAssemblyBuilder->Save (IO::Path::GetFileName (pAssemblyFileName));
			}
			lRet = lAssemblyBuilder;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

Assembly^ TlbToAsm::OutputAssembly (String^ pFileName, bool pLogAssembly, bool pLogCode, bool pShowAssembly)
{
	Assembly^	lAssembly = nullptr;

	try
	{
		LogMessage (LogNormal, _T("Load %s"), _B(pFileName));
		lAssembly = Assembly::LoadFrom (pFileName);

		if	(pLogAssembly)
		{
			LogAssembly (lAssembly, pLogCode);
		}
		if	(pShowAssembly)
		{
			ShowAssembly (lAssembly);
		}
	}
	catch AnyExceptionDebug

	return lAssembly;
}

void TlbToAsm::LogAssembly (Assembly^ pAssembly, bool pLogCode)
{
	if	(pAssembly)
	{
		try
		{
			DoubleAgent::TlbToAsm::LogAssembly^	lLogAssembly = gcnew DoubleAgent::TlbToAsm::LogAssembly;

			lLogAssembly->Log (pAssembly, pLogCode);
		}
		catch AnyExceptionDebug
	}
}

void TlbToAsm::ShowAssembly (Assembly^ pAssembly)
{
	if	(pAssembly)
	{
		try
		{
			DoubleAgent::TlbToAsm::AssemblyDlg^	lAssemblyDlg = gcnew DoubleAgent::TlbToAsm::AssemblyDlg;

			lAssemblyDlg->ShowAssembly (pAssembly);
			lAssemblyDlg->ShowDialog ();
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

StrongNameKeyPair^ TlbToAsm::GetStrongName (String^ pKeyFilePath)
{
	StrongNameKeyPair^	mStrongName = nullptr;

	try
	{
		IO::FileStream^ lFile = gcnew IO::FileStream (pKeyFilePath, IO::FileMode::Open, IO::FileAccess::Read);
		mStrongName = gcnew StrongNameKeyPair (lFile);
		LogMessage (LogNormal, _T("StrongName [%s] [%d]"), _B(lFile->Name), mStrongName->PublicKey->Length);
	}
	catch AnyExceptionDebug

	return mStrongName;
}

bool TlbToAsm::MarkPrimaryAssembly (AssemblyBuilder^ pAssembly)
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
} // namespace TlbToAsm
} // namespace DoubleAgent
