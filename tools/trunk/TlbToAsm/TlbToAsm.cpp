#include "stdafx.h"
#include "TlbToAsm.h"
#include "FixupAssemblySpecific.h"
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
					String^					lSourcePath;
					String^					lTargetPath;
					String^					lNamespace ("DoubleAgent");
					Generic::List<String^>^	lFriendAssemblies = nullptr;
					bool					lLog = false;
					bool					lShow = false;
					bool					lSave = false;

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

					if	(mStrongName)
					{
						int		lNextArgNdx = lCmdArgNdx;
						String^	lNextOpt;

						for	(lNextArgNdx = lCmdArgNdx+1; lNextArgNdx < pCmdArgs->Length; lNextArgNdx++)
						{
							if	(
									(lNextOpt = CmdOpt (pCmdArgs, lNextArgNdx))
								&&	(String::Compare (lNextOpt, "Namespace", true) == 0)
								)
							{
								if	(!lFriendAssemblies)
								{
									lFriendAssemblies = gcnew Generic::List<String^>;
								}
								lFriendAssemblies->Add (CmdArg (pCmdArgs, ++lNextArgNdx));
							}
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

						if	(lAssembly = FixAssembly (lAssembly, lTargetPath, lNamespace, lFriendAssemblies, lSave))
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

AssemblyBuilder^ TlbToAsm::FixAssembly (Assembly^ pAssembly, String^ pAssemblyName, String^ pModuleName, Generic::List<String^>^ pFriendAssemblies, bool pSaveAssembly)
{
	AssemblyBuilder^	lRet = nullptr;

	try
	{
		AssemblyBuilder^	lAssemblyBuilder = nullptr;
		Assembly^			lSavedAssembly;

		pAssemblyName = pAssemblyName->Replace ("\\\\", "\\");

		if	(String::IsNullOrEmpty (pModuleName))
		{
			Console::WriteLine ("Copying {0} to {1}", pAssembly->GetModules()[0]->FullyQualifiedName, pAssemblyName);
		}
		else
		{
			Console::WriteLine ("Copying {0} to {1} Namespace {2}", pAssembly->GetModules()[0]->FullyQualifiedName, pAssemblyName, pModuleName);
		}

		if	(!mCopyAssembly)
		{
			mCopyAssembly = gcnew CopyAssembly;
			mCopyAssembly->mFixups = gcnew FixupAssemblySpecific (mCopyAssembly);
		}

		if	(lAssemblyBuilder = mCopyAssembly->MakeCopy (pAssembly, pAssemblyName, pModuleName, mStrongName))
		{
			try
			{
				mCopyAssembly->CopyTypes ();
				mCopyAssembly->CopyMethodBodies ();
			}
			catch AnyExceptionDebug

			if	(lAssemblyBuilder = mCopyAssembly->CreateCopy ())
			{
				lAssemblyBuilder->DefineVersionInfoResource (_DOUBLEAGENT_NAME, _DOUBLEAGENT_VERSION_STR, _DOUBLEAGENT_COMPANY, _DOUBLEAGENT_COPYRIGHT, nullptr);

				if	(mStrongName)
				{
					MarkPrimaryAssembly (lAssemblyBuilder);

#if	FALSE					
					if	(pFriendAssemblies)
					{
						String^	lFriendAssembly;

						for each (lFriendAssembly in pFriendAssemblies)
						{			
							try
							{		
								CustomAttributeBuilder^	lAttributeBuilder;
								array<Type^>^			lConstructorTypes = gcnew array<Type^> (1);
								array<Object^>^			lConstructorArgs = gcnew array<Object^> (1);
								array<PropertyInfo^>^	lPropertyTypes = gcnew array<PropertyInfo^> (3);
								array<Object^>^			lPropertyArgs = gcnew array<Object^> (3);
								array<PropertyInfo^>^	lProperties = System::Security::Permissions::StrongNameIdentityPermissionAttribute::typeid->GetProperties();
								PropertyInfo^			lProperty;
								StringBuilder^			lPublicKey = gcnew StringBuilder;
								int						lPublicKeyNdx;

								for (lPublicKeyNdx = 0; lPublicKeyNdx < mStrongName->PublicKey->Length; lPublicKeyNdx++)
								{
									lPublicKey->AppendFormat ("{0:X2}", mStrongName->PublicKey[lPublicKeyNdx]);
								}

								lConstructorTypes[0] = System::Security::Permissions::SecurityAction::typeid;
								lConstructorArgs[0] = System::Security::Permissions::SecurityAction::Assert;
								
								for each (lProperty in lProperties)
								{
									if	(lProperty->Name == "Name")
									{
										lPropertyTypes[0] = lProperty;
									}			
									else
									if	(lProperty->Name == "PublicKey")
									{
										lPropertyTypes[1] = lProperty;
									}			
									else
									if	(lProperty->Name == "Unrestricted")
									{
										lPropertyTypes[2] = lProperty;
									}			
								}
								lPropertyArgs[0] = lFriendAssembly;
								lPropertyArgs[1] = lPublicKey->ToString();
								lPropertyArgs[2] = true;

								if	(lAttributeBuilder = gcnew CustomAttributeBuilder (System::Security::Permissions::StrongNameIdentityPermissionAttribute::typeid->GetConstructor(lConstructorTypes), lConstructorArgs, lPropertyTypes, lPropertyArgs))
								{
									lAssemblyBuilder->SetCustomAttribute (lAttributeBuilder);
								}
							}
							catch AnyExceptionDebug
						}
					}
#endif					
#if	FALSE
					if	(pFriendAssemblies)
					{
						String^					lFriendAssembly;
						String^					lPublicKey;
						array<Type^>^			lAttrArgTypes = gcnew array<Type^> (1);
						array<Object^>^			lAttrArgValues = gcnew array<Object^> (1);
						CustomAttributeBuilder^	lAttributeBuilder;

						lPublicKey = lAssemblyBuilder->FullName->Substring (lAssemblyBuilder->FullName->IndexOf("PublicKeyToken="));
						lAttrArgTypes[0] = String::typeid;

						for each (lFriendAssembly in pFriendAssemblies)
						{
							lAttrArgValues[0] = String::Format ("{0}, {1}", lFriendAssembly, lPublicKey);
							lAttributeBuilder = gcnew CustomAttributeBuilder (System::Runtime::CompilerServices::InternalsVisibleToAttribute::typeid->GetConstructor(lAttrArgTypes), lAttrArgValues);
							mCopyAssembly->mAssemblyBuilder->SetCustomAttribute (lAttributeBuilder);
						}
					}
#endif					
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
							if	(mStrongName)
							{
								Console::WriteLine ("Saved {0} (strong named)", pAssemblyName);
							}
							else
							{
								Console::WriteLine ("Saved {0}", pAssemblyName);
							}
						}
					}
					catch AnyExceptionSilent
				}
				lRet = lAssemblyBuilder;
			}
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
