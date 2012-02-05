#pragma once

namespace DoubleAgent {
namespace TlbToAsm {
/////////////////////////////////////////////////////////////////////////////

ref class TlbConverter : public System::Runtime::InteropServices::ITypeLibImporterNotifySink
{
public:
	TlbConverter () {}
	~TlbConverter () {}

	System::Reflection::Emit::AssemblyBuilder^ ConvertTypeLib (System::String^ pTypeLibPath, System::String^ pAssemblyPath, System::Reflection::StrongNameKeyPair^ pStrongName);

	virtual void ReportEvent (System::Runtime::InteropServices::ImporterEventKind eventKind, int eventCode, System::String^ eventMsg);
	virtual System::Reflection::Assembly^ ResolveRef (System::Object^ typeLib);

public:
	System::Reflection::Assembly^	mStdOleAssembly;
protected:
	System::Object^ LoadTypeLibrary (System::String^ pTypeLibPath);
};

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
