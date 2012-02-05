#pragma once

#include "TlbConverter.h"
#include "CopyAssembly.h"
#include "FixupAssembly.h"
#include "LogAssembly.h"
#include "AssemblyDlg.h"

namespace DoubleAgent {
namespace TlbToAsm {
/////////////////////////////////////////////////////////////////////////////

ref class TlbToAsm
{
public:
	TlbToAsm (bool pRestartLog);
	~TlbToAsm () {}

	int ProcessCmdLine (array <String^>^ pCmdArgs);

	AssemblyBuilder^ FixAssembly (Assembly^ pAssembly, String^ pAssemblyName, String^ pModuleName, bool pSaveAssembly);
	Assembly^ ConvertTypeLib (String^ pTlbFileName, String^ pAssemblyFileName, bool pSaveAssembly);

	Assembly^ OutputAssembly (String^ pFileName, bool pLogAssembly, bool pLogCode, bool pShowAssembly);
	void LogAssembly (Assembly^ pAssembly, bool pLogCode);
	void ShowAssembly (Assembly^ pAssembly);

	StrongNameKeyPair^ GetStrongName (String^ pKeyFilePath);
	bool MarkAssemblyVersion (AssemblyBuilder^ pAssembly);
	bool MarkPrimaryAssembly (AssemblyBuilder^ pAssembly);

protected:
	static String^ CmdOpt (array <String^>^ pCmdArgs, int pCmdArgNdx);
	static String^ CmdArg (array <String^>^ pCmdArgs, int pCmdArgNdx);

public:
	bool				mRestartLog;
	StrongNameKeyPair^	mStrongName;
	System::Version^	mAssemblyVersion;
	CopyAssembly^		mCopyAssembly;
};

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
