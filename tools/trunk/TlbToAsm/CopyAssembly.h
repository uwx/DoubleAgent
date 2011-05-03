#pragma once
#include "CopyILBinary.h"
#include "LogAssembly.h"

using namespace System::Runtime::InteropServices;
using namespace System::Runtime::InteropServices::ComTypes;
using namespace System::Reflection;
using namespace System::Reflection::Emit;
using namespace System::Collections::Generic;
using namespace System::ComponentModel;

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
	virtual bool FixupType (Type^ pSourceType, String^& pTypeName, Type^& pBaseType, TypeAttributes & pTypeAttributes) {return false;}
	virtual bool FixupTypeArgument (Type^ pSourceType, Type^& pTargetType) {return false;}
	virtual bool FixupInterface (Type^ pSourceType, Type^ pSourceInterface, TypeBuilder^ pTargetType, Type^& pTargetInterface) {return false;}
	virtual bool FixupEnum (Type^ pSourceType, TypeAttributes & pTypeAttributes) {return false;}
	virtual bool FixupConstructor (ConstructorInfo^ pSourceConstructor, MethodAttributes & pConstructorAttributes) {return false;}
	virtual bool FixupMethod (MethodInfo^ pSourceMethod, String^& pMethodName, MethodAttributes & pMethodAttributes) {return false;}
	virtual bool FixupMethodImpl (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, MethodImplAttributes & pMethodImplAttributes) {return false;}
	virtual bool FixupReturnType (MethodInfo^ pSourceMethod, Type^& pReturnType) {return false;}
	virtual bool FixupReturnType (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, ParameterBuilder^ pReturnType) {return false;}
	virtual bool FixupParameter (MethodBase^ pSourceMethod, ParameterInfo^ pSourceParameter, String^& pParameterName, ParameterAttributes & pParameterAttributes) {return false;}
	virtual bool FixupParameter (MethodBase^ pSourceMethod, ParameterInfo^ pSourceParameter, Type^& pParameterType) {return false;}
	virtual bool FixupParameter (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, ParameterInfo^ pSourceParameter, ParameterBuilder^ pTargetParameter) {return false;}
	virtual bool FixupProperty (PropertyInfo^ pSourceProperty, String^& pPropertyName, Reflection::PropertyAttributes & pPropertyAttributes, Type^& pPropertyType) {return false;}
	virtual bool FixupField (FieldInfo^ pSourceField, String^& pFieldName, FieldAttributes & pFieldAttributes, Type^& pFieldType) {return false;}
	virtual bool FixupField (FieldInfo^ pSourceField, String^& pFieldName, EnumBuilder^ pEnumBuilder) {return false;}
	virtual bool FixupEvent (EventInfo^ pSourceEvent, String^& pEventName, EventAttributes & pEventAttributes) {return false;}
	virtual bool FixupCustomAttribute (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues) {return false;}
	virtual void FixupCustomAttributes (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) {}
	virtual bool FixupMarshalAttribute (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, ParameterInfo^ pSourceParameter, ParameterBuilder^ pTargetParameter, MarshalAsAttribute^ pCustomAttribute, CustomAttributeBuilder^& pAttributeBuilder) {return false;}

	virtual bool CreateClassWrappers () {return false;}
	virtual bool IsCoClassWrapper (Type^ pSourceType) {return false;}
	virtual Type^ GetCoClassInterface (Type^ pSourceType) {return nullptr;}
	virtual Type^ GetCoClassWrapper (Type^ pSourceType) {return nullptr;}

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
	AssemblyBuilder^ MakeCopy (Assembly^ pSourceAssembly, String^ pTargetName, String^ pModuleName);
	AssemblyBuilder^ MakeCopy (Assembly^ pSourceAssembly, String^ pTargetName, String^ pModuleName, Version^ pAssemblyVersion);
	AssemblyBuilder^ MakeCopy (Assembly^ pSourceAssembly, String^ pTargetName, String^ pModuleName, Version^ pAssemblyVersion, StrongNameKeyPair^ pStrongName);
	AssemblyBuilder^ CreateCopy ();

	void CopyTypes ();
	void CopyType (Type^ pSourceType, TypeBuilder^ pTypeBuilder);
	void CopyTypeInterfaces (Type^ pSourceType, TypeBuilder^ pTypeBuilder, CopyFixups^ pFixups);
	void CopyTypeAttributes (Type^ pSourceType, Type^ pTargetType, CopyFixups^ pFixups);
	void CopiedType (Type^ pSourceType, Type^ pTypeBuilder);

	DefinedMethods^ CopyMethods (Type^ pSourceType, TypeBuilder^ pTypeBuilder);
	void CopyMethod (MethodInfo^ pSourceMethod, MethodBuilder^ pMethodBuilder);
	void CopyMethod (MethodInfo^ pSourceMethod, MethodBuilder^ pMethodBuilder, Type^ pReturnType);
	void CopyMethodAttributes (MethodInfo^ pSourceMethod, MethodBuilder^ pMethodBuilder);

	void CopyMethodBodies ();
	void CopyMethodBody (MethodBase^ pSourceMethod, MethodBuilder^ pMethodBuilder);
	void CopyMethodBody (MethodBase^ pSourceMethod, MethodBuilder^ pMethodBuilder, CopyILBinary^ pCopier);
	void CopyMethodBody (MethodBase^ pSourceMethod, ConstructorBuilder^ pConstructorBuilder);
	void CopyMethodBody (MethodBase^ pSourceMethod, ConstructorBuilder^ pConstructorBuilder, CopyILBinary^ pCopier);
	void CopyMethodOverride (MethodBase^ pSourceMethod, MethodBuilder^ pMethodBuilder);

	void CreateTypes ();
	Type^ CreateType (Type^ pSourceType, Type^ pTargetType);

	static int AssemblyRuntimeVersion ();
	static int AssemblyRuntimeVersion (Assembly^ pAssembly);

// Implementation
private:
	void Construct ();

protected:
	Type^ CopyType (Type^ pSourceType);
	Type^ CopyType (Type^ pSourceType, String^ pTargetName, TypeAttributes pTargetAttrs);
	Type^ CopyEnum (Type^ pSourceType, String^ pTargetName, TypeAttributes pTargetAttrs);

	void CopyFields (Type^ pSourceType, TypeBuilder^ pTypeBuilder);
	void CopyConstructors (Type^ pSourceType, TypeBuilder^ pTypeBuilder);
	void CopyMethods (Type^ pSourceType, TypeBuilder^ pTypeBuilder, array<MethodInfo^>^ pSourceMethods, DefinedMethods^ pDefinedMethods);
	void CopyProperties (Type^ pSourceType, TypeBuilder^ pTypeBuilder, DefinedMethods^ pDefinedMethods);
	void CopyEvents (Type^ pSourceType, TypeBuilder^ pTypeBuilder, DefinedMethods^ pDefinedMethods);
	void CopyGenericParameters (Type^ pSourceType, TypeBuilder^ pTypeBuilder);
	void CopyGenericParameters (MethodInfo^ pSourceMethod, MethodBuilder^ pMethodBuilder);

	List<CustomAttributeBuilder^>^ CopyAttributes (Object^ pSource, Object^ pTarget, CustomAttrDataList^ pAttributes);
	static List<CustomAttributeBuilder^>^ CopyAttributes (Object^ pSource, Object^ pTarget, CustomAttrDataList^ pAttributes, CopyFixups^ pFixups);
	CustomAttributeBuilder^ CopyMarshalAttribute (MethodInfo^ pSourceMethod, MethodBuilder^ pMethodBuilder, ParameterInfo^ pSourceParameter, ParameterBuilder^ pParameterBuilder, array<Object^>^ pAttributes);

	MethodInfo^ FindMethodOverride (MethodBase^ pSourceMethod, MethodBuilder^ pMethodBuilder);
	Assembly^ ResolveType (Object^ pSender, ResolveEventArgs^ pEventArgs);
	Assembly^ ResolveAssembly (Object^ pSender, ResolveEventArgs^ pEventArgs);

public:
	Type^ GetSourceType (String^ pSourceTypeName);
	Type^ GetTargetType (Type^ pSourceType, bool pCreate);
	Type^ GetTargetType (String^ pSourceTypeName, bool pCreate);
	Type^ GetTargetReturnType (MethodInfo^ pSourceMethod, bool pCreate);
	Type^ GetTargetParameterType (MethodBase^ pSourceMethod, ParameterInfo^ pSourceParameter, bool pCreate);
	Type^ GetTargetArgumentType (Type^ pSourceType, bool pCreate);
	bool GetTargetArgumentTypes (array<Type^>^ pArgumentTypes, bool pCreate);

	array<Type^>^ GetParameterTypes (MethodBase^ pMethod, bool pTargetTypes);
	array<Type^>^ GetParameterTypes (ConstructorInfo^ pConstuctor, bool pTargetTypes);
	String^ GetMethodSignature (MethodBase^ pMethod);
	static Type^ GetEnumerableType (Type^ pType);

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
	CoClassAttribute^ GetCoClass (Type^ pType);

	String^ LogIndent ();

public:
	virtual bool TranslateType (System::Type^ pSourceType, System::Type^& pTargetType);
	virtual bool TranslateMethod (System::Reflection::MethodBase^ pSourceMethod, System::Reflection::MethodInfo^& pTargetMethod);
	virtual bool TranslateConstructor (System::Reflection::MethodBase^ pSourceConstructor, System::Reflection::ConstructorInfo^& pTargetConstructor);
	virtual bool TranslateField (System::Reflection::FieldInfo^ pSourceField, System::Reflection::FieldInfo^& pTargetField);
	virtual bool TranslateParameter (System::Reflection::ParameterInfo^ pSourceParameter, System::Type^& pParameterType);

	virtual bool IsCoClassWrapper (Type^ pSourceType);
	virtual Type^ GetCoClassInterface (Type^ pSourceType);
	virtual Type^ GetCoClassWrapper (Type^ pSourceInterface);

	virtual void PutWrapperConstructor (System::Type^ pWrapperType, System::Reflection::Emit::ConstructorBuilder^ pWrapperConstructor);
	virtual System::Reflection::Emit::ConstructorBuilder^ GetWrapperConstructor (System::Type^ pWrapperType);
	virtual void PutWrapperAccessor (System::Type^ pWrapperType, System::Reflection::Emit::MethodBuilder^ pWrapperAccessor);
	virtual System::Reflection::Emit::MethodBuilder^ GetWrapperAccessor (System::Type^ pWrapperType);

public:
	Dictionary <Type^, Type^>^							mCopiedTypes;
	Dictionary <MethodInfo^, MethodBuilder^>^			mCopiedMethods;
	Dictionary <ConstructorInfo^, ConstructorBuilder^>^	mCopiedConstructors;
	Dictionary <FieldInfo^, FieldBuilder^>^				mCopiedFields;
	Dictionary <Type^, Type^>^							mTranslateTypes;
	Dictionary <MethodBase^, MethodInfo^>^				mTranslateMethods;
	Dictionary <MethodBase^, ConstructorInfo^>^			mTranslateConstructors;
	Dictionary <FieldInfo^, FieldInfo^>^				mTranslateFields;
	Dictionary <PropertyInfo^, PropertyInfo^>^			mTranslateProperties;
	Dictionary <ParameterInfo^, Type^>^					mTranslateParameters;
	Dictionary <Type^, ConstructorBuilder^>^			mWrapperConstructors;
	Dictionary <Type^, MethodBuilder^>^					mWrapperAccessors;
protected:
	ResolveEventHandler^								mTypeResolveHandler;
	ResolveEventHandler^								mAssemblyResolveHandler;
	int													mLogIndent;
};

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
