#pragma once
#include "CopyILBinary.h"
#include "LogAssembly.h"

using namespace System::Runtime::InteropServices;
using namespace System::Runtime::InteropServices::ComTypes;
using namespace System::Reflection;
using namespace System::Reflection::Emit;
using namespace System::Collections::Generic;

namespace DoubleAgent {
namespace TlbToAsm {
/////////////////////////////////////////////////////////////////////////////

typedef System::Collections::Generic::IList <System::Reflection::CustomAttributeData^>	CustomAttrDataList;
typedef System::Collections::Generic::Dictionary <String^, MethodBuilder^>				DefinedMethods;

ref class CopyAssembly : public LogAssembly, public TranslateILBinary
{
public:
	CopyAssembly () {}
	~CopyAssembly () {}

public:
	AssemblyBuilder^ DoCopy (Assembly^ pSourceAssembly, String^ pTargetName, String^ pModuleName, StrongNameKeyPair^ pStrongName);

protected:
	void CopyTypes ();
	Type^ CopyType (Type^ pSourceType);
	Type^ CopyType (Type^ pSourceType, String^ pTargetName, TypeAttributes pTargetAttrs);
	Type^ CopyEnum (Type^ pSourceType, String^ pTargetName, TypeAttributes pTargetAttrs);

	void CopyInterfaces (Type^ pSourceType, TypeBuilder^ pTypeBuilder);
	void CopyFields (Type^ pSourceType, TypeBuilder^ pTypeBuilder);
	void CopyConstructors (Type^ pSourceType, TypeBuilder^ pTypeBuilder);
	DefinedMethods^ CopyMethods (Type^ pSourceType, TypeBuilder^ pTypeBuilder);
	void CopyMethods (Type^ pSourceType, TypeBuilder^ pTypeBuilder, array<MethodInfo^>^ pSourceMethods, DefinedMethods^ pDefinedMethods);
	void CopyProperties (Type^ pSourceType, TypeBuilder^ pTypeBuilder, DefinedMethods^ pDefinedMethods);
	void CopyEvents (Type^ pSourceType, TypeBuilder^ pTypeBuilder, DefinedMethods^ pDefinedMethods);
	List<CustomAttributeBuilder^>^ CopyAttributes (Object^ pTarget, CustomAttrDataList^ pAttributes);


	void CreateTypes ();
	Type^ CreateType (Type^ pSourceType, Type^ pTargetType);
	Assembly^ ResolveType (Object^ pSender, ResolveEventArgs^ pEventArgs);

	Type^ GetTargetType (Type^ pSourceType, bool pCreate);
	Type^ GetTargetType (String^ pSourceTypeName, bool pCreate);
	array<Type^>^ GetParameterTypes (MethodBase^ pMethod);
	array<Type^>^ GetParameterTypes (ConstructorInfo^ pConstuctor);

	String^ LogIndent ();

	virtual bool FixupType (Type^ pSourceType, String^& pTypeName, TypeAttributes & pTypeAttributes) {return false;}
	virtual bool FixupEnum (Type^ pSourceType, TypeAttributes & pTypeAttributes);
	virtual bool FixupMethod (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes) {return false;}
	virtual bool FixupProperty (PropertyInfo^ pSourceProperty, Reflection::PropertyAttributes & pPropertyAttributes) {return false;}
	virtual bool FixupEvent (EventInfo^ pSourceEvent,EventAttributes & pEventAttributes) {return false;}
	virtual bool FixupCustomAttribute (Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues) {return false;}
	virtual void FixupCustomAttributes (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) {}

	void CopyMethodBodies ();
public:
	virtual bool TranslateType (System::Type^ pSourceType, System::Type^& pTargetType);
	virtual bool TranslateMethod (System::Reflection::MethodBase^ pSourceMethod, System::Reflection::MethodInfo^& pTargetMethod);
	virtual bool TranslateConstructor (System::Reflection::MethodBase^ pSourceConstructor, System::Reflection::ConstructorInfo^& pTargetConstructor);
	virtual bool TranslateField (System::Reflection::FieldInfo^ pSourceField, System::Reflection::FieldInfo^& pTargetField);

protected:
	Assembly^										mSourceAssembly;
	Module^											mSourceModule;
	AssemblyBuilder^								mAssemblyBuilder;
	String^											mModuleName;
	ModuleBuilder^									mModuleBuilder;
	Dictionary <Type^, Type^>^						mCopiedTypes;
	Dictionary <MethodBase^, MethodBuilder^>^		mCopiedMethods;
	Dictionary <MethodBase^, ConstructorBuilder^>^	mCopiedConstructors;
	Dictionary <FieldInfo^, FieldBuilder^>^			mCopiedFields;
	int												mLogIndent;
};

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
