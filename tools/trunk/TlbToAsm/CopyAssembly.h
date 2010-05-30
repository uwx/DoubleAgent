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

/////////////////////////////////////////////////////////////////////////////

ref class CopyFixups
{
public:
	CopyFixups (ref class CopyAssembly^ pCopy) : mCopy (pCopy) {}
	~CopyFixups () {}

public:
	virtual bool FixupType (Type^ pSourceType, String^& pTypeName, TypeAttributes & pTypeAttributes) {return false;}
	virtual bool FixupEnum (Type^ pSourceType, TypeAttributes & pTypeAttributes) {return false;}
	virtual bool FixupMethod (MethodInfo^ pSourceMethod, String^& pMethodName, MethodAttributes & pMethodAttributes) {return false;}
	virtual bool FixupReturnType (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, Type^& pReturnType) {return false;}
	virtual bool FixupReturnType (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, ParameterBuilder^ pReturnType) {return false;}
	virtual bool FixupParameter (MethodBase^ pSourceMethod, ParameterInfo^ pSourceParameter, String^& pParameterName, ParameterAttributes & pParameterAttributes) {return false;}
	virtual bool FixupParameter (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, ParameterInfo^ pSourceParameter, Type^& pParameterType) {return false;}
	virtual bool FixupParameter (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, ParameterInfo^ pSourceParameter, ParameterBuilder^ pTargetParameter) {return false;}
	virtual bool FixupProperty (PropertyInfo^ pSourceProperty, String^& pPropertyName, Reflection::PropertyAttributes & pPropertyAttributes, Type^& pPropertyType) {return false;}
	virtual bool FixupField (FieldInfo^ pSourceField, String^& pFieldName, FieldAttributes & pFieldAttributes) {return false;}
	virtual bool FixupField (FieldInfo^ pSourceField, String^& pFieldName, EnumBuilder^ pEnumBuilder) {return false;}
	virtual bool FixupEvent (EventInfo^ pSourceEvent, String^& pEventName, EventAttributes & pEventAttributes) {return false;}
	virtual bool FixupCustomAttribute (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues) {return false;}
	virtual void FixupCustomAttributes (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) {}

protected:
	ref class CopyAssembly^	mCopy;
};

/////////////////////////////////////////////////////////////////////////////

ref class CopyAssembly : public LogAssembly, public TranslateILBinary
{
public:
	CopyAssembly ();
	~CopyAssembly ();

public:
	Assembly^			mSourceAssembly;
	Module^				mSourceModule;
	AssemblyBuilder^	mAssemblyBuilder;
	String^				mModuleName;
	ModuleBuilder^		mModuleBuilder;
	CopyFixups^			mFixups;
	List<Assembly^>^	mSavedAssemblies;

public:
	AssemblyBuilder^ DoCopy (Assembly^ pSourceAssembly, String^ pTargetName, String^ pModuleName, StrongNameKeyPair^ pStrongName);

	Type^ GetTargetType (Type^ pSourceType, bool pCreate);
	Type^ GetTargetType (String^ pSourceTypeName, bool pCreate);

	array<Type^>^ GetParameterTypes (MethodBase^ pMethod);
	array<Type^>^ GetParameterTypes (ConstructorInfo^ pConstuctor);
	String^ GetMethodSignature (MethodBase^ pMethod);

	static TypeLibFuncFlags GetTypeLibFuncFlags (MethodInfo^ pMethod);
	static TypeLibVarFlags GetTypeLibVarFlags (PropertyInfo^ pProperty);
	static TypeLibTypeFlags GetTypeLibTypeFlags (Type^ pType);
	TypeLibTypeFlags GetTypeLibTypeFlags (String^ pTypeName);

	String^ LogIndent ();

protected:
	void CopyTypes ();
	void CopyMethodBodies ();
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
	List<CustomAttributeBuilder^>^ CopyAttributes (Object^ pSource, Object^ pTarget, CustomAttrDataList^ pAttributes);
	CustomAttributeBuilder^ CopyMarshalAttribute (array<Object^>^ pAttributes);

	void CreateTypes ();
	Type^ CreateType (Type^ pSourceType, Type^ pTargetType);
	Assembly^ ResolveType (Object^ pSender, ResolveEventArgs^ pEventArgs);
	Assembly^ ResolveAssembly (Object^ pSender, ResolveEventArgs^ pEventArgs);

public:
	virtual bool TranslateType (System::Type^ pSourceType, System::Type^& pTargetType);
	virtual bool TranslateMethod (System::Reflection::MethodBase^ pSourceMethod, System::Reflection::MethodInfo^& pTargetMethod);
	virtual bool TranslateConstructor (System::Reflection::MethodBase^ pSourceConstructor, System::Reflection::ConstructorInfo^& pTargetConstructor);
	virtual bool TranslateField (System::Reflection::FieldInfo^ pSourceField, System::Reflection::FieldInfo^& pTargetField);

protected:
	Dictionary <Type^, Type^>^						mCopiedTypes;
	Dictionary <MethodBase^, MethodBuilder^>^		mCopiedMethods;
	Dictionary <MethodBase^, ConstructorBuilder^>^	mCopiedConstructors;
	Dictionary <FieldInfo^, FieldBuilder^>^			mCopiedFields;
	Dictionary <Type^, Type^>^						mTranslateTypes;
	Dictionary <MethodBase^, MethodInfo^>^			mTranslateMethods;
	Dictionary <MethodBase^, ConstructorInfo^>^		mTranslateConstructors;
	Dictionary <FieldInfo^, FieldInfo^>^			mTranslateFields;
	int												mLogIndent;
};

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
