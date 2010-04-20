#include "stdafx.h"
#include "TlbConverter.h"

using namespace System::Runtime::InteropServices;
using namespace System::Runtime::InteropServices::ComTypes;
using namespace System::Reflection;

#ifdef	_DEBUG
//#define	_DEBUG_EVENTS	LogNormal
//#define	_DEBUG_RESOLVE	LogNormal
#endif

/////////////////////////////////////////////////////////////////////////////

namespace DoubleAgent {
namespace TlbToAsm {
/////////////////////////////////////////////////////////////////////////////

Object^ TlbConverter::LoadTypeLibrary (String^ pTypeLibPath)
{
	HRESULT		lResult;
	ITypeLibPtr	lTypeLib;

	lResult = LoadTypeLib (marshal_as<_bstr_t>(pTypeLibPath), &lTypeLib);
	LogComErrAnon (LogAlways, lResult, _T("Load [%s] Interface [%p]"), _B(pTypeLibPath), lTypeLib.GetInterfacePtr());
	return Marshal::GetObjectForIUnknown ((IntPtr)(LPUNKNOWN)lTypeLib.GetInterfacePtr());
}

/////////////////////////////////////////////////////////////////////////////

Emit::AssemblyBuilder^ TlbConverter::ConvertTypeLib (String^ pTypeLibPath, String^ pAssemblyPath)
{
	Object^					lTypeLibObj;
	ComTypes::ITypeLib^		lTypeLib;
	TypeLibConverter^		lConverter = gcnew TypeLibConverter;
	array<Byte>^			lPublicKey = nullptr;
	StrongNameKeyPair^		lKeyPair = nullptr;
	Emit::AssemblyBuilder^	lBuilder;

	lTypeLibObj = LoadTypeLibrary (pTypeLibPath);
	lTypeLib = safe_cast<ComTypes::ITypeLib^>(lTypeLibObj);
	lBuilder = lConverter->ConvertTypeLibToAssembly (lTypeLib, pAssemblyPath, (Int32)(TypeLibImporterFlags::ImportAsAgnostic|TypeLibImporterFlags::TransformDispRetVals|TypeLibImporterFlags::SafeArrayAsSystemArray), this, lPublicKey, lKeyPair, true);

	return lBuilder;
}

/////////////////////////////////////////////////////////////////////////////

void TlbConverter::ReportEvent (ImporterEventKind eventKind, int eventCode, String^ eventMsg)
{
#ifdef	_DEBUG_EVENTS
	LogMessage (_DEBUG_EVENTS, _T("  Kind [%d] Code [%8.8X] Msg [%s]"), (int)eventKind, eventCode, _B(eventMsg));
#endif
}

Assembly^ TlbConverter::ResolveRef (Object^ typeLib)
{
#ifdef	_DEBUG_RESOLVE
	Type^ lType = typeLib->GetType();
	LogMessage (_DEBUG_RESOLVE, _T("ResolveRef... [%s]"), _B(typeLib->ToString()));
#endif
	return mStdOleAssembly;
}

/////////////////////////////////////////////////////////////////////////////
};
};