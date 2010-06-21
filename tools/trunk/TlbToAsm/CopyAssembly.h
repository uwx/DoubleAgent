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
	virtual void PreCopyTypes () {}
	virtual void PostCopyTypes () {}
	virtual bool FixupType (Type^ pSourceType, String^& pTypeName, TypeAttributes & pTypeAttributes) {return false;}
	virtual bool FixupTypeArgument (Type^ pSourceType, Type^& pTargetType) {return false;}
	virtual bool FixupTypeTarget (Type^ pSourceType, TypeBuilder^ pTargetType) {return false;}
	virtual bool FixupInterface (Type^ pSourceType, Type^ pSourceInterface, TypeBuilder^ pTargetType, Type^& pTargetInterface) {return false;}
	virtual bool FixupEnum (Type^ pSourceType, TypeAttributes & pTypeAttributes) {return false;}
	virtual bool FixupMethod (MethodInfo^ pSourceMethod, String^& pMethodName, MethodAttributes & pMethodAttributes) {return false;}
	virtual bool FixupReturnType (MethodInfo^ pSourceMethod, Type^& pReturnType) {return false;}
	virtual bool FixupReturnType (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, ParameterBuilder^ pReturnType) {return false;}
	virtual bool FixupParameter (MethodBase^ pSourceMethod, ParameterInfo^ pSourceParameter, String^& pParameterName, ParameterAttributes & pParameterAttributes) {return false;}
	virtual bool FixupParameter (MethodBase^ pSourceMethod, ParameterInfo^ pSourceParameter, Type^& pParameterType) {return false;}
	virtual bool FixupParameter (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, ParameterInfo^ pSourceParameter, ParameterBuilder^ pTargetParameter) {return false;}
	virtual bool FixupProperty (PropertyInfo^ pSourceProperty, String^& pPropertyName, Reflection::PropertyAttributes & pPropertyAttributes, Type^& pPropertyType) {return false;}
	virtual bool FixupField (FieldInfo^ pSourceField, String^& pFieldName, FieldAttributes & pFieldAttributes) {return false;}
	virtual bool FixupField (FieldInfo^ pSourceField, String^& pFieldName, EnumBuilder^ pEnumBuilder) {return false;}
	virtual bool FixupEvent (EventInfo^ pSourceEvent, String^& pEventName, EventAttributes & pEventAttributes) {return false;}
	virtual bool FixupCustomAttribute (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues) {return false;}
	virtual void FixupCustomAttributes (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) {}
	virtual CustomAttributeBuilder^ FixupMarshalAttribute (Object^ pSource, Object^ pTarget, MarshalAsAttribute^ pCustomAttribute) {return nullptr;}

protected:
	ref class CopyAssembly^	mCopy;
};

/////////////////////////////////////////////////////////////////////////////

ref class CopyAssembly : public LogAssembly, public TranslateILBinary
{
public:
	CopyAssembly ();
	CopyAssembly (Assembly^ pSourceAssembly, CopyAssembly^ pCopyTarget);
	~CopyAssembly ();

// Attributes
public:
	Assembly^			mSourceAssembly;
	Module^				mSourceModule;
	AssemblyBuilder^	mAssemblyBuilder;
	String^				mModuleName;
	ModuleBuilder^		mModuleBuilder;
	CopyFixups^			mFixups;
	List<Assembly^>^	mSavedAssemblies;

// Operations
public:
	AssemblyBuilder^ MakeCopy (Assembly^ pSourceAssembly, String^ pTargetName, String^ pModuleName, StrongNameKeyPair^ pStrongName);
	AssemblyBuilder^ CreateCopy ();

	void CopyTypes ();
	void CopyType (Type^ pSourceType, TypeBuilder^ pTypeBuilder);
	void CopyTypeAttributes (Type^ pSourceType, Type^ pTargetType);
	void CopiedType (Type^ pSourceType, Type^ pTypeBuilder);

	void CopyMethod (MethodInfo^ pSourceMethod, MethodBuilder^ pMethodBuilder);
	void CopyMethod (MethodInfo^ pSourceMethod, MethodBuilder^ pMethodBuilder, Type^ pReturnType);
	void CopyMethodAttributes (MethodInfo^ pSourceMethod, MethodBuilder^ pMethodBuilder);

	void CopyMethodBodies ();
	void CopyMethodBody (MethodBase^ pSourceMethod, MethodBuilder^ pMethodBuilder);
	void CopyMethodBody (MethodBase^ pSourceMethod, MethodBuilder^ pMethodBuilder, CopyILBinary^ pCopier);
	void CopyMethodBody (MethodBase^ pSourceMethod, ConstructorBuilder^ pConstructorBuilder);
	void CopyMethodBody (MethodBase^ pSourceMethod, ConstructorBuilder^ pConstructorBuilder, CopyILBinary^ pCopier);

	void CreateTypes ();
	Type^ CreateType (Type^ pSourceType, Type^ pTargetType);

// Implementation
private:
	void Construct ();

protected:
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
	void CopyGenericParameters (Type^ pSourceType, TypeBuilder^ pTypeBuilder);
	void CopyGenericParameters (MethodInfo^ pSourceMethod, MethodBuilder^ pMethodBuilder);
	List<CustomAttributeBuilder^>^ CopyAttributes (Object^ pSource, Object^ pTarget, CustomAttrDataList^ pAttributes);
	CustomAttributeBuilder^ CopyMarshalAttribute (Object^ pSource, Object^ pTarget, array<Object^>^ pAttributes);

	Assembly^ ResolveType (Object^ pSender, ResolveEventArgs^ pEventArgs);
	Assembly^ ResolveAssembly (Object^ pSender, ResolveEventArgs^ pEventArgs);

public:
	Type^ GetTargetType (Type^ pSourceType, bool pCreate);
	Type^ GetTargetType (String^ pSourceTypeName, bool pCreate);
	Type^ GetTargetReturnType (MethodInfo^ pSourceMethod, bool pCreate);
	Type^ GetTargetParameterType (MethodBase^ pSourceMethod, ParameterInfo^ pSourceParameter, bool pCreate);
	Type^ GetTargetArgumentType (Type^ pSourceType, bool pCreate);

	array<Type^>^ GetParameterTypes (MethodBase^ pMethod, bool pTargetTypes);
	array<Type^>^ GetParameterTypes (ConstructorInfo^ pConstuctor, bool pTargetTypes);
	String^ GetMethodSignature (MethodBase^ pMethod);

	static TypeLibFuncFlags GetTypeLibFuncFlags (MethodInfo^ pMethod);
	static bool GetTypeLibFuncFlags (MethodInfo^ pMethod, TypeLibFuncFlags & pFuncFlags);
	static TypeLibFuncFlags GetTypeLibFuncFlags (PropertyInfo^ pProperty);
	static bool GetTypeLibFuncFlags (PropertyInfo^ pProperty, TypeLibFuncFlags & pFuncFlags);
	static TypeLibVarFlags GetTypeLibVarFlags (PropertyInfo^ pProperty);
	static bool GetTypeLibVarFlags (PropertyInfo^ pProperty, TypeLibVarFlags & pVarFlags);
	static TypeLibTypeFlags GetTypeLibTypeFlags (Type^ pType);
	static bool GetTypeLibTypeFlags (Type^ pType, TypeLibTypeFlags & pTypeFlags);
	TypeLibTypeFlags GetTypeLibTypeFlags (String^ pTypeName);
	static DispIdAttribute^ GetDispId (MethodInfo^ pMethod);
	static DispIdAttribute^ GetDispId (PropertyInfo^ pProperty);

	String^ LogIndent ();

public:
	virtual bool TranslateType (System::Type^ pSourceType, System::Type^& pTargetType);
	virtual bool TranslateMethod (System::Reflection::MethodBase^ pSourceMethod, System::Reflection::MethodInfo^& pTargetMethod);
	virtual bool TranslateConstructor (System::Reflection::MethodBase^ pSourceConstructor, System::Reflection::ConstructorInfo^& pTargetConstructor);
	virtual bool TranslateField (System::Reflection::FieldInfo^ pSourceField, System::Reflection::FieldInfo^& pTargetField);

public:	
	Dictionary <Type^, Type^>^						mCopiedTypes;
	Dictionary <MethodBase^, MethodBuilder^>^		mCopiedMethods;
	Dictionary <MethodBase^, ConstructorBuilder^>^	mCopiedConstructors;
	Dictionary <FieldInfo^, FieldBuilder^>^			mCopiedFields;
	Dictionary <Type^, Type^>^						mTranslateTypes;
	Dictionary <MethodBase^, MethodInfo^>^			mTranslateMethods;
	Dictionary <MethodBase^, ConstructorInfo^>^		mTranslateConstructors;
	Dictionary <FieldInfo^, FieldInfo^>^			mTranslateFields;
protected:
	int												mLogIndent;
};

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
