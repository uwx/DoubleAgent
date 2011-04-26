#include "stdafx.h"
#include <corhdr.h>
#include "CopyAssembly.h"
#include "MakeILWrapper.h"

#ifdef	_DEBUG
//#define	_DEBUG_SOURCE_TYPE		LogNormal|LogHighVolume
//#define	_DEBUG_TARGET_TYPE		LogNormal|LogHighVolume
//#define	_DEBUG_TARGET_ENUM		LogNormal|LogHighVolume
//#define	_DEBUG_TARGET_INTERFACE	LogNormal|LogHighVolume
//#define	_DEBUG_TARGET_FIELD		LogNormal|LogHighVolume
//#define	_DEBUG_TARGET_METHOD	LogNormal|LogHighVolume
//#define	_DEBUG_TARGET_BODY		LogNormal|LogHighVolume
//#define	_DEBUG_TARGET_PARAMETER	LogNormal|LogHighVolume
//#define	_DEBUG_TARGET_PROPERTY	LogNormal|LogHighVolume
//#define	_DEBUG_TARGET_EVENT		LogNormal|LogHighVolume
//#define	_DEBUG_CREATE_TYPE		LogNormal
#define	_DEBUG_RESOLVE_TYPE		LogNormal
//#define	_DEBUG_TARGET_CODE		LogDebug
#endif

//#pragma comment(linker, "\"/manifestdependency:name='mscorlib' version='2.0.0.0' Culture='neutral' publicKeyToken='b77a5c561934e089'\"")

namespace DoubleAgent {
namespace TlbToAsm {
/////////////////////////////////////////////////////////////////////////////

CopyAssembly::CopyAssembly ()
{
	Construct ();
}

CopyAssembly::CopyAssembly (Assembly^ pSourceAssembly, CopyAssembly^ pCopyTarget)
{
	Construct ();

	mSourceAssembly = pSourceAssembly;
	mSourceModule = mSourceAssembly->GetModules() [0];
	mModuleName = pCopyTarget->mModuleName;
	mModuleBuilder = pCopyTarget->mModuleBuilder;
	mAssemblyBuilder = pCopyTarget->mAssemblyBuilder;

	mTranslateTypes = pCopyTarget->mTranslateTypes;
	mTranslateMethods = pCopyTarget->mTranslateMethods;
	mTranslateConstructors = pCopyTarget->mTranslateConstructors;
	mTranslateFields = pCopyTarget->mTranslateFields;
	mTranslateProperties = pCopyTarget->mTranslateProperties;
	mTranslateParameters = pCopyTarget->mTranslateParameters;
	mWrapperConstructors = pCopyTarget->mWrapperConstructors;
	mWrapperAccessors = pCopyTarget->mWrapperAccessors;
}

CopyAssembly::~CopyAssembly ()
{
	AppDomain::CurrentDomain->TypeResolve::remove (mTypeResolveHandler);
	AppDomain::CurrentDomain->AssemblyResolve::remove (mAssemblyResolveHandler);
}

/////////////////////////////////////////////////////////////////////////////

void CopyAssembly::Construct ()
{
	mCopiedTypes = gcnew Dictionary <Type^, Type^>;
	mCopiedMethods = gcnew Dictionary <MethodInfo^, MethodBuilder^>;
	mCopiedConstructors = gcnew Dictionary <ConstructorInfo^, ConstructorBuilder^>;
	mCopiedFields = gcnew Dictionary <FieldInfo^, FieldBuilder^>;
	mTranslateTypes = gcnew Dictionary <Type^, Type^>;
	mTranslateMethods = gcnew Dictionary <MethodBase^, MethodInfo^>;
	mTranslateConstructors = gcnew Dictionary <MethodBase^, ConstructorInfo^>;
	mTranslateFields = gcnew Dictionary <FieldInfo^, FieldInfo^>;
	mTranslateProperties = gcnew Dictionary <PropertyInfo^, PropertyInfo^>;
	mTranslateParameters = gcnew Dictionary <ParameterInfo^, Type^>;
	mWrapperConstructors = gcnew Dictionary <Type^, ConstructorBuilder^>;
	mWrapperAccessors = gcnew Dictionary <Type^, MethodBuilder^>;
	mSavedAssemblies = gcnew List<Assembly^>;

	AppDomain::CurrentDomain->TypeResolve::add (mTypeResolveHandler = gcnew ResolveEventHandler (this, &CopyAssembly::ResolveType));
	AppDomain::CurrentDomain->AssemblyResolve::add (mAssemblyResolveHandler = gcnew ResolveEventHandler (this, &CopyAssembly::ResolveAssembly));
	mLogIndent = 0;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

AssemblyBuilder^ CopyAssembly::MakeCopy (Assembly^ pSourceAssembly, String^ pAssemblyName, String^ pModuleName, StrongNameKeyPair^ pStrongName)
{
	try
	{
		AssemblyName^	lAssemblyName = gcnew AssemblyName;
		String^			lFilePath = IO::Path::GetDirectoryName (pAssemblyName);
		String^			lFileName = IO::Path::GetFileName (pAssemblyName);

LogReferences (Assembly::GetExecutingAssembly());
try
{
	array<Assembly^>^	lAssemblies = AppDomain::CurrentDomain->GetAssemblies();

	if	(lAssemblies)
	{
		for each (Assembly^ lAssembly in lAssemblies)
		{
#ifdef	_DEBUG_RESOLVE_TYPE
			LogMessage (_DEBUG_RESOLVE_TYPE, _T("%s  Loaded [%s]"), _B(LogIndent()), _B(lAssembly->FullName));
#endif
		}
	}
}
catch AnyExceptionSilent

try
{
	array<AssemblyName^>^	lReferences = Assembly::GetExecutingAssembly()->GetReferencedAssemblies ();

	if	(lReferences)
	{
		for each (AssemblyName^ lReference in lReferences)
		{
			if	(lReference->Version->Major > 3)
			{
#ifdef	_DEBUG_RESOLVE_TYPE
				LogMessage (_DEBUG_RESOLVE_TYPE, _T("%s  Remove? [%s]"), _B(LogIndent()), _B(lReference->FullName));
#endif
			}
		}
	}
}
catch AnyExceptionSilent
LogReferences (Assembly::GetExecutingAssembly());

		lAssemblyName->Name = IO::Path::GetFileNameWithoutExtension (pAssemblyName);
		lAssemblyName->Version = safe_cast <System::Version^> (pSourceAssembly->GetName()->Version->Clone());
		lAssemblyName->ProcessorArchitecture = pSourceAssembly->GetName()->ProcessorArchitecture;
		lAssemblyName->KeyPair = pStrongName;

		mSourceAssembly = pSourceAssembly;
		mSourceModule = mSourceAssembly->GetModules() [0];
		mModuleName = pModuleName;

		mAssemblyBuilder = AppDomain::CurrentDomain->DefineDynamicAssembly (lAssemblyName, AssemblyBuilderAccess::Save, lFilePath);
		mModuleBuilder = mAssemblyBuilder->DefineDynamicModule (mModuleName, lFileName, true);

		LogMessage (LogNormal, _T("Copy [%s] Module [%s]"), _B(mSourceAssembly->FullName), _B(mSourceModule->FullyQualifiedName));
		LogMessage (LogNormal, _T("  to [%s] Module [%s]"), _B(mAssemblyBuilder->FullName), _B(mModuleBuilder->FullyQualifiedName));

//LogMessage (LogDebug, _T("Version [%s] [%s] [%s]"), _B(AppDomain::CurrentDomain->ApplicationIdentity->FullName), _B(pSourceAssembly->ImageRuntimeVersion), _B(mAssemblyBuilder->ImageRuntimeVersion));
LogMessage (LogDebug, _T("Version [%s] [%s] [%s] [%s]"), _B(Environment::Version->ToString()), _B(Assembly::GetExecutingAssembly()->ImageRuntimeVersion), _B(pSourceAssembly->ImageRuntimeVersion), _B(mAssemblyBuilder->ImageRuntimeVersion));
#ifdef	_DEBUG
		LogReferences (mSourceAssembly);
		LogReferences (mAssemblyBuilder);
#endif
	}
	catch AnyExceptionDebug
	{}

	return mAssemblyBuilder;
}

AssemblyBuilder^ CopyAssembly::CreateCopy ()
{
	try
	{
		CreateTypes ();

		try
		{
			List<CustomAttributeBuilder^>^	lBuilders;
			CustomAttributeBuilder^			lBuilder;

			if	(lBuilders = CopyAttributes (mSourceAssembly, mAssemblyBuilder, CustomAttributeData::GetCustomAttributes (mSourceAssembly)))
			{
				for each (lBuilder in lBuilders)
				{
					mAssemblyBuilder->SetCustomAttribute (lBuilder);
				}
			}
		}
		catch AnyExceptionDebug

		try
		{
			mModuleBuilder->CreateGlobalFunctions ();
		}
		catch AnyExceptionDebug

#ifdef	_DEBUG_NOT
		LogReferences (mSourceAssembly);
		LogReferences (mAssemblyBuilder);
#endif
	}
	catch AnyExceptionDebug
	{}

	return mAssemblyBuilder;
}

/////////////////////////////////////////////////////////////////////////////

void CopyAssembly::CopyTypes ()
{
	array<Type^>^					lSourceTypes = nullptr;
	Type^							lSourceType = nullptr;
	KeyValuePair <Type^, Type^>^	lCopiedType;
	Dictionary <Type^, Type^>^		lCopiedTypes = gcnew Dictionary <Type^, Type^>;

	mCopiedTypes = gcnew Dictionary <Type^, Type^>;
	mCopiedMethods = gcnew Dictionary <MethodInfo^, MethodBuilder^>;
	mCopiedConstructors = gcnew Dictionary <ConstructorInfo^, ConstructorBuilder^>;
	mCopiedFields = gcnew Dictionary <FieldInfo^, FieldBuilder^>;
	mLogIndent = 0;

	if	(lSourceTypes = mSourceAssembly->GetTypes())
	{
		GetInterfaceMappings (lSourceTypes);

		for each (lSourceType in lSourceTypes)
		{
			mCopiedTypes->Add (lSourceType, nullptr);
			mTranslateTypes->Add (lSourceType, nullptr);
		}
		for each (lCopiedType in mCopiedTypes)
		{
			lCopiedTypes->Add (lCopiedType->Key, lCopiedType->Value);
		}
		for each (lCopiedType in lCopiedTypes)
		{
			if	(
					(lCopiedType->Value == nullptr)
				&&	(mCopiedTypes [lCopiedType->Key] == nullptr)
				)
			{
				CopyType (lCopiedType->Key);
			}
		}
	}
}

Type^ CopyAssembly::CopyType (Type^ pSourceType)
{
	Type^	lTargetType = nullptr;

	try
	{
		TypeAttributes	lTypeAttrs = (TypeAttributes) ((int)pSourceType->Attributes & ~(int)TypeAttributes::ReservedMask);
		String^			lTargetName = String::Format ("{0}.{1}", mModuleName, pSourceType->Name);

		if	(pSourceType->IsEnum)
		{
#ifdef	_DEBUG_SOURCE_TYPE
			LogMessage (_DEBUG_SOURCE_TYPE, _T("%sSource Enum [%s]"), _B(LogIndent()), _BT(pSourceType));
#endif
			lTargetType = CopyEnum (pSourceType, lTargetName, lTypeAttrs);
		}
		else
		if	(pSourceType->IsInterface)
		{
#ifdef	_DEBUG_SOURCE_TYPE
			LogMessage (_DEBUG_SOURCE_TYPE, _T("%sSource Interface [%s]"), _B(LogIndent()), _BT(pSourceType));
#endif
			lTargetType = CopyType (pSourceType, lTargetName, lTypeAttrs);
		}
		else
		if	(pSourceType->IsClass)
		{
#ifdef	_DEBUG_SOURCE_TYPE
			LogMessage (_DEBUG_SOURCE_TYPE, _T("%sSource Class [%s]"), _B(LogIndent()), _BT(pSourceType));
#endif
			lTargetType = CopyType (pSourceType, lTargetName, lTypeAttrs);
		}

		if	(lTargetType)
		{
			CopiedType (pSourceType, lTargetType);
		}
	}
	catch AnyExceptionDebug
	{}

	return lTargetType;
}

/////////////////////////////////////////////////////////////////////////////

void CopyAssembly::CreateTypes ()
{
	KeyValuePair <Type^, Type^>^	lCopiedType;
	Dictionary <Type^, Type^>^		lCopiedTypes = gcnew Dictionary <Type^, Type^>;

	mLogIndent = 0;

	try
	{
#ifdef	_DEBUG_CREATE_TYPE
		LogMessage (_DEBUG_CREATE_TYPE, _T("Create [%d] Types"), mCopiedTypes->Count);
#endif
		for each (lCopiedType in mCopiedTypes)
		{
			lCopiedTypes->Add (lCopiedType->Key, lCopiedType->Value);
		}
		for each (lCopiedType in lCopiedTypes)
		{
			if	(ReferenceEquals (lCopiedType->Value, mCopiedTypes [lCopiedType->Key]))
			{
				CreateType (lCopiedType->Key, lCopiedType->Value);
			}
		}
		for each (lCopiedType in lCopiedTypes)
		{
			if	(
					(!lCopiedType->Key->IsGenericType)
				||	(lCopiedType->Key->IsGenericTypeDefinition)
				)
			{
				CopyTypeAttributes (lCopiedType->Key, lCopiedType->Value, mFixups);
			}
		}
	}
	catch AnyExceptionDebug
}

Type^ CopyAssembly::CreateType (Type^ pSourceType, Type^ pTargetType)
{
	Type^	lTargetType = nullptr;

	if	(pTargetType == nullptr)
	{
#ifdef	_DEBUG_CREATE_TYPE
		LogMessage (_DEBUG_CREATE_TYPE, _T("%s  Skip   Type [%s]"), _B(LogIndent()), _BT(pSourceType));
#endif
	}
	else
	{
		try
		{
			TypeBuilder^	lTypeBuilder = nullptr;
			EnumBuilder^	lEnumBuilder = nullptr;

			try
			{
				lTypeBuilder = safe_cast <TypeBuilder^> (pTargetType);
			}
			catch AnyExceptionSilent
			try
			{
				lEnumBuilder = safe_cast <EnumBuilder^> (pTargetType);
			}
			catch AnyExceptionSilent

			if	(lTypeBuilder)
			{
#ifdef	_DEBUG_CREATE_TYPE
				LogMessage (_DEBUG_CREATE_TYPE, _T("%s  Create Type [%s]"), _B(LogIndent()), _BT(lTypeBuilder));
#endif
				try
				{
					lTargetType = lTypeBuilder->CreateType ();
				}
				catch AnyExceptionDebug
			}
			else
			if	(lEnumBuilder)
			{
#ifdef	_DEBUG_CREATE_TYPE
				LogMessage (_DEBUG_CREATE_TYPE, _T("%s  Create Enum [%s]"), _B(LogIndent()), _BT(lEnumBuilder));
#endif
				try
				{
					lTargetType = lEnumBuilder->CreateType ();
				}
				catch AnyExceptionDebug
			}

			if	(
					(pSourceType)
				&&	(lTargetType)
				)
			{
				CopiedType (pSourceType, lTargetType);
				GetInterfaceMappings (lTargetType);
#ifdef	_DEBUG_CREATE_TYPE
				LogMessage (_DEBUG_CREATE_TYPE, _T("%s    Created   [%s] for [%s]"), _B(LogIndent()), _BT(lTargetType), _BT(pSourceType));
#endif
			}
		}
		catch AnyExceptionDebug
	}

	return lTargetType;
}

void CopyAssembly::CopyTypeAttributes (Type^ pSourceType, Type^ pTargetType, CopyFixups^ pFixups)
{
	if	(
			(pSourceType)
		&&	(pTargetType)
		&&	(!ReferenceEquals (pSourceType, pTargetType))
		)
	{
		TypeBuilder^	lTypeBuilder = nullptr;
		EnumBuilder^	lEnumBuilder = nullptr;

		try
		{
			lTypeBuilder = safe_cast <TypeBuilder^> (pTargetType);
		}
		catch AnyExceptionSilent
		try
		{
			lEnumBuilder = safe_cast <EnumBuilder^> (pTargetType);
		}
		catch AnyExceptionSilent

		if	(lTypeBuilder)
		{
			try
			{
				List<CustomAttributeBuilder^>^	lBuilders;
				CustomAttributeBuilder^			lBuilder;

				if	(lBuilders = CopyAttributes (pSourceType, lTypeBuilder, CustomAttributeData::GetCustomAttributes (pSourceType), pFixups))
				{
					for each (lBuilder in lBuilders)
					{
						lTypeBuilder->SetCustomAttribute (lBuilder);
					}
				}
			}
			catch AnyExceptionDebug
		}
		else
		if	(lEnumBuilder)
		{
			try
			{
				List<CustomAttributeBuilder^>^	lBuilders;
				CustomAttributeBuilder^			lBuilder;

				if	(lBuilders = CopyAttributes (pSourceType, lEnumBuilder, CustomAttributeData::GetCustomAttributes (pSourceType)))
				{
					for each (lBuilder in lBuilders)
					{
						lEnumBuilder->SetCustomAttribute (lBuilder);
					}
				}
			}
			catch AnyExceptionDebug
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

Assembly^ CopyAssembly::ResolveType (Object^ pSender, ResolveEventArgs^ pEventArgs)
{
	Assembly^	lRet = nullptr;

	mLogIndent++;

	try
	{
		KeyValuePair <Type^, Type^>^	lTranslateType;
		Type^							lTargetType = nullptr;

#ifdef	_DEBUG_RESOLVE_TYPE
		LogMessage (_DEBUG_RESOLVE_TYPE, _T("%s  Resolve   [%s]"), _B(LogIndent()), _B(pEventArgs->Name));
#endif
		for each (lTranslateType in mTranslateTypes)
		{
			if	(
					(lTranslateType->Value)
				&&	(lTranslateType->Value->FullName == pEventArgs->Name)
				)
			{
#ifdef	_DEBUG_RESOLVE_TYPE
				LogMessage (_DEBUG_RESOLVE_TYPE, _T("%s  Resolving [%s] from [%s]"), _B(LogIndent()), _BT(lTranslateType->Value), _BT(lTranslateType->Key));
#endif
				lTargetType = CreateType (lTranslateType->Key, lTranslateType->Value);

				if	(lTargetType)
				{
#ifdef	_DEBUG_RESOLVE_TYPE
					LogMessage (_DEBUG_RESOLVE_TYPE, _T("%s  Resolved [%s]"), _B(LogIndent()), _BT(lTargetType));
#endif
					lRet = lTargetType->Assembly;
				}
				break;
			}
		}
	}
	catch AnyExceptionSilent

	mLogIndent--;
	return lRet;
}

Assembly^ CopyAssembly::ResolveAssembly (Object^ pSender, ResolveEventArgs^ pEventArgs)
{
	Assembly^	lRet = nullptr;

	mLogIndent++;

#ifdef	_DEBUG_RESOLVE_TYPE
	LogMessage (_DEBUG_RESOLVE_TYPE, _T("%s  ResolveAssembly [%s]"), _B(LogIndent()), _B(pEventArgs->Name));
#endif
	try
	{
		array<AssemblyName^>^	lReferences = mSourceAssembly->GetReferencedAssemblies ();

		if	(lReferences)
		{
			for each (AssemblyName^ lReference in lReferences)
			{
#ifdef	_DEBUG_RESOLVE_TYPE
	LogMessage (_DEBUG_RESOLVE_TYPE, _T("%s  Reference? [%s]"), _B(LogIndent()), _B(lReference->FullName));
#endif
			}
		}
	}
	catch AnyExceptionSilent

	try
	{
		Assembly^	lAssembly;

		for each (lAssembly in mSavedAssemblies)
		{
			if	(String::Compare (lAssembly->FullName, pEventArgs->Name) == 0)
			{
				lRet = lAssembly;
				break;
			}
		}
	}
	catch AnyExceptionSilent

	mLogIndent--;
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

Type^ CopyAssembly::CopyType (Type^ pSourceType, String^ pTargetName, TypeAttributes pTargetAttrs)
{
	Type^	lTargetType = nullptr;

	mLogIndent++;

	try
	{
		TypeBuilder^	lTypeBuilder;
		Type^			lTargetBase = nullptr;
		DefinedMethods^	lDefinedMethods = nullptr;

		if	(pSourceType->BaseType)
		{
			if	(String::Compare (pSourceType->BaseType->FullName, "System.__ComObject", true) == 0)
			{
#ifdef	_DEBUG_TARGET_TYPE
				LogMessage (_DEBUG_TARGET_TYPE, _T("%s  Skip Base [%s]"), _B(LogIndent()), _BT(pSourceType->BaseType));
#endif
			}
			else
			{
				lTargetBase = GetTargetType (pSourceType->BaseType, false);
			}
		}

		if	(
				(!mFixups)
			||	(!mFixups->FixupType (pSourceType, pTargetName, lTargetBase, pTargetAttrs))
			)
		{
#ifdef	_DEBUG_TARGET_TYPE
			LogMessage (_DEBUG_TARGET_TYPE, _T("%sCopy [%s] [%s] to [%s] [%s]"), _B(LogIndent()), _BT(pSourceType), _B(TypeProps(pSourceType)), _B(pTargetName), _B(TypeAttrsStr(pTargetAttrs)));
#endif
			lTypeBuilder = mModuleBuilder->DefineType (pTargetName, pTargetAttrs);

			if	(lTargetBase)
			{
#ifdef	_DEBUG_TARGET_TYPE
				LogMessage (_DEBUG_TARGET_TYPE, _T("%s  Copy Base [%s] as [%s]"), _B(LogIndent()), _BT(pSourceType->BaseType), _BT(lTargetBase));
#endif
				lTypeBuilder->SetParent (lTargetBase);
			}
			CopiedType (pSourceType, lTypeBuilder);

			if	(pSourceType->IsGenericTypeDefinition)
			{
				CopyGenericParameters (pSourceType, lTypeBuilder);
			}
			CopyTypeInterfaces (pSourceType, lTypeBuilder, mFixups);
			CopyFields (pSourceType, lTypeBuilder);
			CopyConstructors (pSourceType, lTypeBuilder);
			lDefinedMethods = CopyMethods (pSourceType, lTypeBuilder);
			CopyProperties (pSourceType, lTypeBuilder, lDefinedMethods);
			CopyEvents (pSourceType, lTypeBuilder, lDefinedMethods);

			lTargetType = lTypeBuilder;
		}
	}
	catch AnyExceptionDebug
	{}

	mLogIndent--;
	return lTargetType;
}

/////////////////////////////////////////////////////////////////////////////

void CopyAssembly::CopyType (Type^ pSourceType, TypeBuilder^ pTypeBuilder)
{
	if	(
			(pSourceType)
		&&	(pTypeBuilder)
		)
	{
		GetInterfaceMappings (pSourceType);
		CopiedType (pSourceType, pTypeBuilder);
		mLogIndent++;

		try
		{
			DefinedMethods^	lDefinedMethods;

			if	(pSourceType->IsGenericTypeDefinition)
			{
				CopyGenericParameters (pSourceType, pTypeBuilder);
			}
			CopyTypeInterfaces (pSourceType, pTypeBuilder, mFixups);
			CopyFields (pSourceType, pTypeBuilder);
			CopyConstructors (pSourceType, pTypeBuilder);
			lDefinedMethods = CopyMethods (pSourceType, pTypeBuilder);
			CopyProperties (pSourceType, pTypeBuilder, lDefinedMethods);
			CopyEvents (pSourceType, pTypeBuilder, lDefinedMethods);
		}
		catch AnyExceptionDebug

		mLogIndent--;
	}
}

void CopyAssembly::CopiedType (Type^ pSourceType, Type^ pTypeBuilder)
{
	if	(
			(pSourceType)
		&&	(pTypeBuilder)
		)
	{
		mCopiedTypes [pSourceType] = pTypeBuilder;
		mTranslateTypes [pSourceType] = pTypeBuilder;
	}
}

/////////////////////////////////////////////////////////////////////////////

void CopyAssembly::CopyTypeInterfaces (Type^ pSourceType, TypeBuilder^ pTypeBuilder, CopyFixups^ pFixups)
{
	if	(
			(pSourceType)
		&&	(pTypeBuilder)
		)
	{
		try
		{
			array<Type^>^	lSourceInterfaces;
			Type^			lSourceInterface;
			Type^			lTargetInterface;

			if	(
					(lSourceInterfaces = pSourceType->GetInterfaces ())
				&&	(lSourceInterfaces->Length > 0)
				)
			{
#ifdef	_DEBUG_TARGET_INTERFACE
				LogMessage (_DEBUG_TARGET_INTERFACE, _T("%s  Copy [%d] Interfaces"), _B(LogIndent()), lSourceInterfaces->Length);
#endif
				for each (lSourceInterface in lSourceInterfaces)
				{
					if	(IsInterfaceInherited (pSourceType, lSourceInterface))
					{
#ifdef	_DEBUG_TARGET_INTERFACE
						LogMessage (_DEBUG_TARGET_INTERFACE, _T("%s    Skip Inherited [%s]"), _B(LogIndent()), _BT(lSourceInterface));
#endif
					}
					else
					if	(
							(!String::IsNullOrEmpty (lSourceInterface->FullName))
						&&	(lSourceInterface->FullName->IndexOf (_T("UnsafeNativeMethods")) > 0)
						)
					{
#ifdef	_DEBUG_TARGET_INTERFACE
						LogMessage (_DEBUG_TARGET_INTERFACE, _T("%s    Skip AxNative  [%s]"), _B(LogIndent()), _BT(lSourceInterface));
#endif
					}
					else
					{
						lTargetInterface = GetTargetType (lSourceInterface, false);
						if	(
								(pFixups)
							&&	(pFixups->FixupInterface (pSourceType, lSourceInterface, pTypeBuilder, lTargetInterface))
							)
						{
#ifdef	_DEBUG_TARGET_INTERFACE
							LogMessage (_DEBUG_TARGET_INTERFACE, _T("%s    Skip      [%s] for [%s]"), _B(LogIndent()), _BT(lSourceInterface), _BT(pSourceType));
#endif
						}
						else
						{
#ifdef	_DEBUG_TARGET_INTERFACE
							LogMessage (_DEBUG_TARGET_INTERFACE, _T("%s    Interface [%s] as [%s]"), _B(LogIndent()), _BT(lSourceInterface), _BT(lTargetInterface));
#endif
							pTypeBuilder->AddInterfaceImplementation (lTargetInterface);
						}
					}
				}
			}
		}
		catch AnyExceptionDebug
	}
}

void CopyAssembly::CopyFields (Type^ pSourceType, TypeBuilder^ pTypeBuilder)
{
	try
	{
		array<FieldInfo^>^	lSourceFields;
		FieldInfo^			lSourceField;

		if	(
				(lSourceFields = pSourceType->GetFields (BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
			&&	(lSourceFields->Length > 0)
			)
		{
#ifdef	_DEBUG_TARGET_FIELD
			LogMessage (_DEBUG_TARGET_FIELD, _T("%s  Copy [%d] fields"), _B(LogIndent()), lSourceFields->Length);
#endif
			for each (lSourceField in lSourceFields)
			{
				String^			lFieldName = lSourceField->Name;
				FieldAttributes	lFieldAttributes = lSourceField->Attributes;
				Type^			lFieldType;
				FieldBuilder^	lFieldBuilder;

				if	(!ReferenceEquals (lSourceField->DeclaringType, pSourceType))
				{
#ifdef	_DEBUG_TARGET_FIELD
					LogMessage (_DEBUG_TARGET_FIELD, _T("%s    Field [%s] from [%s] skipped for [%s]"), _B(LogIndent()), _BM(lSourceField), _BMT(lSourceField), _BT(pSourceType));
#endif
					continue;
				}
				lFieldType = GetTargetType (lSourceField->FieldType, false);
				if	(
						(mFixups)
					&&	(mFixups->FixupField (lSourceField, lFieldName, lFieldAttributes, lFieldType))
					)
				{
					continue;
				}
#ifdef	_DEBUG_TARGET_FIELD
				LogMessage (_DEBUG_TARGET_FIELD, _T("%s    Field [%s] [%s] [%s] as [%s]"), _B(LogIndent()), _B(lFieldName), _BT(lSourceField->FieldType), _B(FieldAttrsStr(lFieldAttributes)), _BT(lFieldType));
#endif
				lFieldBuilder = pTypeBuilder->DefineField (lFieldName, lFieldType, lFieldAttributes);
				mCopiedFields [lSourceField] = lFieldBuilder;
				mTranslateFields [lSourceField] = lFieldBuilder;
			}
		}
	}
	catch AnyExceptionDebug
	{}
}

/////////////////////////////////////////////////////////////////////////////

void CopyAssembly::CopyConstructors (Type^ pSourceType, TypeBuilder^ pTypeBuilder)
{
	if	(
			(pSourceType)
		&&	(pTypeBuilder)
		)
	{
		try
		{
			array<ConstructorInfo^>^	lSourceConstructors;
			ConstructorInfo^			lSourceConstructor;

			if	(
					(lSourceConstructors = pSourceType->GetConstructors (BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
				&&	(lSourceConstructors->Length > 0)
				)
			{
#ifdef	_DEBUG_TARGET_METHOD
				LogMessage (_DEBUG_TARGET_METHOD, _T("%s  Copy [%d] Constructors"), _B(LogIndent()), lSourceConstructors->Length);
#endif
				for each (lSourceConstructor in lSourceConstructors)
				{
					ConstructorBuilder^		lConstructorBuilder;
					MethodAttributes		lConstructorAttributes = lSourceConstructor->Attributes;
					array<ParameterInfo^>^	lParameters = nullptr;
					array<Type^>^			lParameterTypes = GetParameterTypes (lSourceConstructor, true);
					int						lParameterNdx;

					if	(
							(mFixups)
						&&	(mFixups->FixupConstructor (lSourceConstructor, lConstructorAttributes))
						)
					{
						continue;
					}

					try
					{
						lParameters = lSourceConstructor->GetParameters ();
					}
					catch AnyExceptionSilent
#ifdef	_DEBUG_TARGET_METHOD
					LogMessage (_DEBUG_TARGET_METHOD, _T("%s    Constructor [%s] [%s] [%s] [%8.8X]"), _B(LogIndent()), _B(MethodAttrsStr(lSourceConstructor->Attributes)), _B(MethodCallType(lSourceConstructor->CallingConvention)), _B(MethodImplementation(lSourceConstructor->GetMethodImplementationFlags())), lSourceConstructor->MetadataToken);
#endif
#ifdef	_DEBUG_TARGET_PARAMETER
					if	(
							(lParameters)
						&&	(lParameters->Length > 0)
						)
					{
						LogMessage (_DEBUG_TARGET_PARAMETER, _T("%s      Copy [%d] Parameters for [%s.%s]"), _B(LogIndent()), lParameters->Length, _BMT(lSourceConstructor), _BM(lSourceConstructor));
						if	(lParameterTypes)
						{
							for	(lParameterNdx = 0; lParameterNdx < lParameters->Length; lParameterNdx++)
							{
								if	(lParameters [lParameterNdx]->Name == nullptr)
								{
									LogMessage (_DEBUG_TARGET_PARAMETER, _T("%s        Parameter (unnamed) [%s] [%s] as [%s]"), _B(LogIndent()), _B(ParameterAttrsStr(lParameters [lParameterNdx]->Attributes)), _BT(lParameters [lParameterNdx]->ParameterType), _BT(lParameterTypes [lParameterNdx]));
								}
								else
								{
									LogMessage (_DEBUG_TARGET_PARAMETER, _T("%s        Parameter [%s] [%s] [%s] as [%s]"), _B(LogIndent()), _BM(lParameters [lParameterNdx]), _B(ParameterAttrsStr(lParameters [lParameterNdx]->Attributes)), _BT(lParameters [lParameterNdx]->ParameterType), _BT(lParameterTypes [lParameterNdx]));
								}
							}
						}
					}
#endif
					if	(lParameterTypes == nullptr)
					{
						lConstructorBuilder = pTypeBuilder->DefineDefaultConstructor (lConstructorAttributes);
					}
					else
					{
						lConstructorBuilder = pTypeBuilder->DefineConstructor (lConstructorAttributes, lSourceConstructor->CallingConvention, lParameterTypes);

						for	(lParameterNdx = 0; lParameterNdx < lParameters->Length; lParameterNdx++)
						{
							ParameterInfo^		lParameter = lParameters [lParameterNdx];
							String^				lParameterName = lParameter->Name;
							ParameterAttributes	lParameterAttributes = lParameter->Attributes;
							ParameterBuilder^	lParameterBuilder;

							if	(mFixups)
							{
								mFixups->FixupParameter (lSourceConstructor, lParameter, lParameterName, lParameterAttributes);
							}
							lParameterBuilder = lConstructorBuilder->DefineParameter (lParameterNdx+1, lParameterAttributes, lParameterName);
							if	(
									(lParameter->DefaultValue)
								&&	(!String::IsNullOrEmpty (lParameter->DefaultValue->ToString()))
								)
							{
								try
								{
									lParameterBuilder->SetConstant (lParameter->DefaultValue);
								}
								catch AnyExceptionSilent
							}
							if	(!ReferenceEquals (lParameter->ParameterType, lParameterTypes [lParameterNdx]))
							{
								mTranslateParameters [lParameter] = lParameterTypes [lParameterNdx];
							}
						}
					}

					lConstructorBuilder->SetImplementationFlags (lSourceConstructor->GetMethodImplementationFlags ());
					mCopiedConstructors [lSourceConstructor] = lConstructorBuilder;
					mTranslateConstructors [lSourceConstructor] = lConstructorBuilder;
				}
			}
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////

DefinedMethods^ CopyAssembly::CopyMethods (Type^ pSourceType, TypeBuilder^ pTypeBuilder)
{
	Dictionary <String^, MethodBuilder^>^	lRet = gcnew Dictionary <String^, MethodBuilder^>;

	try
	{
		array<MethodInfo^>^						lSourceMethods;
		Generic::SortedList <int, MethodInfo^>^	lSorted = gcnew Generic::SortedList <int, MethodInfo^>;
		MethodInfo^								lMethod;
		int										lMethodNdx;

		if	(lSourceMethods = pSourceType->GetMethods (BindingFlags::Static|BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
		{
			for	(lMethodNdx = 0; lMethodNdx < lSourceMethods->Length; lMethodNdx++)
			{
				lMethod = lSourceMethods [lMethodNdx];
				lSorted->Add (lMethod->MetadataToken, lMethod);
			}
			lSourceMethods = gcnew array<MethodInfo^> (lSourceMethods->Length);
			for	(lMethodNdx = 0; lMethodNdx < lSourceMethods->Length; lMethodNdx++)
			{
				lSourceMethods [lMethodNdx] = lSorted->Values [lMethodNdx];
			}

			CopyMethods (pSourceType, pTypeBuilder, lSourceMethods, lRet);
		}
	}
	catch AnyExceptionDebug
	{}

	return lRet;
}

void CopyAssembly::CopyMethods (Type^ pSourceType, TypeBuilder^ pTypeBuilder, array<MethodInfo^>^ pSourceMethods, DefinedMethods^ pDefinedMethods)
{
	if	(
			(pSourceMethods)
		&&	(pSourceMethods->Length > 0)
		)
	{
		try
		{
			MethodInfo^											lSourceMethod;
			Generic::Dictionary <MethodInfo^, MethodBuilder^>^	lTargetMethods = gcnew Generic::Dictionary <MethodInfo^, MethodBuilder^>;
			int													lMethodNdx;

#ifdef	_DEBUG_TARGET_METHOD
			LogMessage (_DEBUG_TARGET_METHOD, _T("%s  Copy [%d] Methods"), _B(LogIndent()), pSourceMethods->Length);
#endif
			for	(lMethodNdx = 0; lMethodNdx < pSourceMethods->Length; lMethodNdx++)
			{
				MethodBuilder^		lMethodBuilder;
				String^				lMethodName;
				MethodAttributes	lMethodAttrs;

				lSourceMethod = pSourceMethods [lMethodNdx];
				lMethodName = lSourceMethod->Name;
				lMethodAttrs = lSourceMethod->Attributes;

				if	(!ReferenceEquals (lSourceMethod->DeclaringType, pSourceType))
				{
#ifdef	_DEBUG_TARGET_METHOD
					LogMessage (_DEBUG_TARGET_METHOD, _T("%s    Method [%s.%s] skipped for [%s]"), _B(LogIndent()), _BMT(lSourceMethod), _BM(lSourceMethod), _BT(pSourceType));
#endif
					continue;
				}
				if	(
						(mFixups)
					&&	(mFixups->FixupMethod (lSourceMethod, lMethodName, lMethodAttrs))
					)
				{
					continue;
				}

#ifdef	_DEBUG_TARGET_METHOD
				LogMessage (_DEBUG_TARGET_METHOD, _T("%s    Method [%3.3d] [%s] [%s] [%s] [%s] [%8.8X]"), _B(LogIndent()), lMethodNdx, _BM(lSourceMethod), _B(MethodAttrsStr(lMethodAttrs)), _B(MethodCallType(lSourceMethod->CallingConvention)), _B(MethodImplementation(lSourceMethod->GetMethodImplementationFlags())), lSourceMethod->MetadataToken);
#endif
				try
				{
					if	(lMethodBuilder = pTypeBuilder->DefineMethod (lMethodName, lMethodAttrs, lSourceMethod->CallingConvention))
					{
						lTargetMethods->Add (lSourceMethod, lMethodBuilder);
					}
				}
				catch AnyExceptionDebug
			}

			for	(lMethodNdx = 0; lMethodNdx < pSourceMethods->Length; lMethodNdx++)
			{
				MethodBuilder^	lMethodBuilder = nullptr;

				lSourceMethod = pSourceMethods [lMethodNdx];
				if	(lTargetMethods->TryGetValue (lSourceMethod, lMethodBuilder))
				{
					CopyMethod (lSourceMethod, lMethodBuilder);
					CopyMethodAttributes (lSourceMethod, lMethodBuilder);

					try
					{
						pDefinedMethods->Add (GetMethodSignature (lSourceMethod), lMethodBuilder);
					}
					catch AnyExceptionDebug

					mCopiedMethods [lSourceMethod] = lMethodBuilder;
					mTranslateMethods [lSourceMethod] = lMethodBuilder;
				}
			}
		}
		catch AnyExceptionDebug
		{}
	}
}

/////////////////////////////////////////////////////////////////////////////

void CopyAssembly::CopyMethod (MethodInfo^ pSourceMethod, MethodBuilder^ pMethodBuilder)
{
	CopyMethod (pSourceMethod, pMethodBuilder, nullptr);
}

void CopyAssembly::CopyMethod (MethodInfo^ pSourceMethod, MethodBuilder^ pMethodBuilder, Type^ pReturnType)
{
	Type^	lReturnType;

	try
	{
		if	(pSourceMethod->IsGenericMethodDefinition)
		{
			CopyGenericParameters (pSourceMethod, pMethodBuilder);
		}

		if	(
				(lReturnType = pReturnType)
			||	(lReturnType = GetTargetReturnType (pSourceMethod, false))
			)
		{
#ifdef	_DEBUG_TARGET_PARAMETER
			LogMessage (_DEBUG_TARGET_PARAMETER, _T("%s      Copy ReturnType [%s] as [%s] for [%s.%s]"), _B(LogIndent()), _BT(pSourceMethod->ReturnType), _BT(lReturnType), _BMT(pSourceMethod), _BM(pSourceMethod));
#endif
#if	FALSE
			if	(
					(pSourceMethod->ReturnParameter)
				&&	(pSourceMethod->ReturnParameter->GetRequiredCustomModifiers())
				&&	(pSourceMethod->ReturnParameter->GetRequiredCustomModifiers()->Length > 0)
				)
			{
				LogMessage (LogDebug, _T("------ Return GetRequiredCustomModifiers [%d]"), pSourceMethod->ReturnParameter->GetRequiredCustomModifiers()->Length);
			}
			if	(
					(pSourceMethod->ReturnParameter)
				&&	(pSourceMethod->ReturnParameter->GetOptionalCustomModifiers())
				&&	(pSourceMethod->ReturnParameter->GetOptionalCustomModifiers()->Length > 0)
				)
			{
				LogMessage (LogDebug, _T("------ Return GetOptionalCustomModifiers [%d]"), pSourceMethod->ReturnParameter->GetOptionalCustomModifiers()->Length);
			}
#endif
			pMethodBuilder->SetReturnType (lReturnType);
			if	(
					(pSourceMethod->ReturnParameter)
				&&	(!ReferenceEquals (pSourceMethod->ReturnType, lReturnType))
				)
			{
				mTranslateParameters [pSourceMethod->ReturnParameter] = lReturnType;
			}
		}
	}
	catch AnyExceptionDebug

	try
	{
		array<ParameterInfo^>^	lParameters = pSourceMethod->GetParameters ();
		array<Type^>^			lParameterTypes;
		int						lParameterNdx;

		if	(
				(lParameters)
			&&	(lParameters->Length > 0)
			&&	(lParameterTypes = GetParameterTypes (pSourceMethod, true))
			)
		{
#ifdef	_DEBUG_TARGET_PARAMETER
			LogMessage (_DEBUG_TARGET_PARAMETER, _T("%s      Copy [%d] Parameters for [%s.%s]"), _B(LogIndent()), lParameters->Length, _BMT(pSourceMethod), _BM(pSourceMethod));
			for	(lParameterNdx = 0; lParameterNdx < lParameters->Length; lParameterNdx++)
			{
				if	(lParameters [lParameterNdx]->Name == nullptr)
				{
					LogMessage (_DEBUG_TARGET_PARAMETER, _T("%s        Parameter (unnamed) [%s] [%s] as [%s]"), _B(LogIndent()), _B(ParameterAttrsStr(lParameters [lParameterNdx]->Attributes)), _BT(lParameters [lParameterNdx]->ParameterType), _BT(lParameterTypes [lParameterNdx]));
				}
				else
				{
					LogMessage (_DEBUG_TARGET_PARAMETER, _T("%s        Parameter [%s] [%s] [%s] as [%s]"), _B(LogIndent()), _BM(lParameters [lParameterNdx]), _B(ParameterAttrsStr(lParameters [lParameterNdx]->Attributes)), _BT(lParameters [lParameterNdx]->ParameterType), _BT(lParameterTypes [lParameterNdx]));
				}
			}
#endif
			pMethodBuilder->SetParameters (lParameterTypes);

			for	(lParameterNdx = 0; lParameterNdx < lParameters->Length; lParameterNdx++)
			{
				ParameterInfo^		lParameter = lParameters [lParameterNdx];
				String^				lParameterName = lParameter->Name;
				ParameterAttributes	lParameterAttributes = lParameter->Attributes;
				ParameterBuilder^	lParameterBuilder;
#if	FALSE
				if	(
						(lParameter->GetRequiredCustomModifiers())
					&&	(lParameter->GetRequiredCustomModifiers()->Length > 0)
					)
				{
					LogMessage (LogDebug, _T("------ Parameter GetRequiredCustomModifiers [%d]"), lParameter->GetRequiredCustomModifiers()->Length);
				}
				if	(
						(lParameter->GetOptionalCustomModifiers())
					&&	(lParameter->GetOptionalCustomModifiers()->Length > 0)
					)
				{
					LogMessage (LogDebug, _T("------ Parameter GetOptionalCustomModifiers [%d]"), lParameter->GetOptionalCustomModifiers()->Length);
				}
#endif
				if	(mFixups)
				{
					mFixups->FixupParameter (pSourceMethod, lParameter, lParameterName, lParameterAttributes);
				}
				lParameterBuilder = pMethodBuilder->DefineParameter (lParameterNdx+1, lParameterAttributes, lParameterName);

				if	(
						(lParameter->DefaultValue)
					&&	(!String::IsNullOrEmpty (lParameter->DefaultValue->ToString()))
					)
				{
					try
					{
						lParameterBuilder->SetConstant (lParameter->DefaultValue);
					}
					catch AnyExceptionSilent
				}

				try
				{
					CustomAttributeBuilder^	lAttributeBuilder;

					if	(lAttributeBuilder = CopyMarshalAttribute (pSourceMethod, pMethodBuilder, lParameter, lParameterBuilder, lParameter->GetCustomAttributes (MarshalAsAttribute::typeid, false)))
					{
						lParameterBuilder->SetCustomAttribute (lAttributeBuilder);
					}
				}
				catch AnyExceptionDebug

				if	(mFixups)
				{
					mFixups->FixupParameter (pSourceMethod, pMethodBuilder, lParameter, lParameterBuilder);
				}
#if	FALSE
				try
				{
					List<CustomAttributeBuilder^>^	lBuilders;
					CustomAttributeBuilder^			lBuilder;

					if	(lBuilders = CopyAttributes (lParameter, lParameterBuilder, CustomAttributeData::GetCustomAttributes (lParameter)))
					{
						for each (lBuilder in lBuilders)
						{
							lParameterBuilder->SetCustomAttribute (lBuilder);
						}
					}
				}
				catch AnyExceptionDebug
#endif
				if	(!ReferenceEquals (lParameter->ParameterType, lParameterTypes [lParameterNdx]))
				{
					mTranslateParameters [lParameter] = lParameterTypes [lParameterNdx];
				}
			}
		}
	}
	catch AnyExceptionDebug

	if	(lReturnType)
	{
		try
		{
			ParameterBuilder^		lParameterBuilder = pMethodBuilder->DefineParameter (0, pSourceMethod->ReturnParameter->Attributes, String::Empty);
			CustomAttributeBuilder^	lAttributeBuilder;

			if	(
					(pSourceMethod->ReturnTypeCustomAttributes)
				&&	(lAttributeBuilder = CopyMarshalAttribute (pSourceMethod, pMethodBuilder, pSourceMethod->ReturnParameter, lParameterBuilder, pSourceMethod->ReturnTypeCustomAttributes->GetCustomAttributes (MarshalAsAttribute::typeid, false)))
				)
			{
				lParameterBuilder->SetCustomAttribute (lAttributeBuilder);
			}

			if	(mFixups)
			{
				mFixups->FixupReturnType (pSourceMethod, pMethodBuilder, lParameterBuilder);
			}
#if	FALSE
			try
			{
				List<CustomAttributeBuilder^>^	lBuilders;
				CustomAttributeBuilder^			lBuilder;

				if	(lBuilders = CopyAttributes (pSourceMethod->ReturnParameterlParameterBuilder, CustomAttributeData::GetCustomAttributes (pSourceMethod->ReturnParameter)))
				{
					for each (lBuilder in lBuilders)
					{
						lParameterBuilder->SetCustomAttribute (lBuilder);
					}
				}
			}
			catch AnyExceptionDebug
#endif
		}
		catch AnyExceptionDebug
	}

	try
	{
		MethodImplAttributes	lImplAttributes = pSourceMethod->GetMethodImplementationFlags ();

		if	(mFixups)
		{
			mFixups->FixupMethodImpl (pSourceMethod, pMethodBuilder, lImplAttributes);
		}
		pMethodBuilder->SetImplementationFlags (lImplAttributes);
	}
	catch AnyExceptionDebug
}

void CopyAssembly::CopyMethodAttributes (MethodInfo^ pSourceMethod, MethodBuilder^ pMethodBuilder)
{
	if	(
			(pSourceMethod)
		&&	(pMethodBuilder)
		)
	{
		try
		{
			List<CustomAttributeBuilder^>^	lBuilders;
			CustomAttributeBuilder^			lBuilder;

			if	(lBuilders = CopyAttributes (pSourceMethod, pMethodBuilder, CustomAttributeData::GetCustomAttributes (pSourceMethod)))
			{
				for each (lBuilder in lBuilders)
				{
					pMethodBuilder->SetCustomAttribute (lBuilder);
				}
			}
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////

CustomAttributeBuilder^ CopyAssembly::CopyMarshalAttribute (MethodInfo^ pSourceMethod, MethodBuilder^ pMethodBuilder, ParameterInfo^ pSourceParameter, ParameterBuilder^ pParameterBuilder, array<Object^>^ pAttributes)
{
	CustomAttributeBuilder^	lRet = nullptr;

	try
	{
		if	(
				(pAttributes)
			&&	(pAttributes->Length > 0)
			)
		{
			MarshalAsAttribute^	lAttribute;

			try
			{
				lAttribute = safe_cast <MarshalAsAttribute^> (pAttributes[0]);
			}
			catch AnyExceptionSilent

			if	(
					(lAttribute)
				&&	(
						(!mFixups)
					||	(!mFixups->FixupMarshalAttribute (pSourceMethod, pMethodBuilder, pSourceParameter, pParameterBuilder, lAttribute, lRet))
					)
				)
			{
				Type^						lAttributeType = lAttribute->GetType();
				ConstructorInfo^			lConstructor = lAttributeType->GetConstructors (BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public) [0];
				Generic::List <Object^>^	lConstructorArgs = gcnew Generic::List <Object^>;
				array<FieldInfo^>^			lFields = lAttributeType->GetFields (BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public);
				Generic::List <FieldInfo^>^	lFieldInfos = gcnew Generic::List <FieldInfo^>;
				Generic::List <Object^>^	lFieldValues = gcnew Generic::List <Object^>;
				Object^						lFieldValue;
				int							lFieldNdx;

				lConstructorArgs->Add (lAttribute->Value);

				for	(lFieldNdx = 0; lFieldNdx < lFields->Length; lFieldNdx++)
				{
					if	(
							(
								(lAttribute->Value != UnmanagedType::SafeArray)
							||	(
									(String::Compare (lFields[lFieldNdx]->Name, "ArraySubType") != 0)
								&&	(String::Compare (lFields[lFieldNdx]->Name, "SizeConst") != 0)
								&&	(String::Compare (lFields[lFieldNdx]->Name, "SizeParamIndex") != 0)
								)
							)
						&&	(String::Compare (lFields[lFieldNdx]->Name, "IidParameterIndex") != 0)
						&&	(lFieldValue = lFields[lFieldNdx]->GetValue(lAttribute))
						)
					{
						lFieldInfos->Add (lFields[lFieldNdx]);
						lFieldValues->Add (lFieldValue);
					}
				}
#if	FALSE
				LogMessage (LogDebug, _T("  Attribute [%s] Type [%s] [%s] [%s]"), _B(lAttribute->ToString()), _B(lAttribute->Value.ToString()), _B(lAttribute->ArraySubType.ToString()), _B(lAttribute->SafeArraySubType.ToString()));
				if	(lAttribute->Value == UnmanagedType::SafeArray)
				{
					for	(lFieldNdx = 0; lFieldNdx < lFieldInfos->Length; lFieldNdx++)
					{
						LogMessage (LogDebug, _T("    [%s] [%s]"), _B(lFieldInfos[lFieldNdx]->ToString()), _B(lFieldValues[lFieldNdx]->ToString()));
					}
				}
#endif
				lRet = gcnew CustomAttributeBuilder (lConstructor, lConstructorArgs->ToArray(), lFieldInfos->ToArray(), lFieldValues->ToArray());
			}
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CopyAssembly::CopyProperties (Type^ pSourceType, TypeBuilder^ pTypeBuilder, DefinedMethods^ pDefinedMethods)
{
	try
	{
		array<PropertyInfo^>^	lSourceProperties;
		PropertyInfo^			lSourceProperty;

		if	(
				(lSourceProperties = pSourceType->GetProperties (BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
			&&	(lSourceProperties->Length > 0)
			)
		{
#ifdef	_DEBUG_TARGET_PROPERTY
			LogMessage (_DEBUG_TARGET_PROPERTY, _T("%s  Copy [%d] Properties"), _B(LogIndent()), lSourceProperties->Length);
#endif
			for each (lSourceProperty in lSourceProperties)
			{
				Reflection::PropertyAttributes	lPropertyAttributes = lSourceProperty->Attributes;
				String^							lPropertyName = lSourceProperty->Name;
				Type^							lPropertyType;
				PropertyBuilder^				lPropertyBuilder;
				MethodInfo^						lSetMethod = nullptr;
				MethodInfo^						lGetMethod = nullptr;
				array<ParameterInfo^>^			lGetParameters = nullptr;
				array<Type^>^					lParameterTypes = nullptr;
				int								lParameterNdx;
				MethodBuilder^					lMethodBuilder;

				if	(!ReferenceEquals (lSourceProperty->DeclaringType, pSourceType))
				{
#ifdef	_DEBUG_TARGET_PROPERTY
					LogMessage (_DEBUG_TARGET_PROPERTY, _T("%s    Property [%s] from [%s] skipped for [%s]"), _B(LogIndent()), _BM(lSourceProperty), _BMT(lSourceProperty), _BT(pSourceType));
#endif
					continue;
				}

				lGetMethod = lSourceProperty->GetGetMethod ();
				if	(lGetMethod)
				{
					lGetParameters = lGetMethod->GetParameters ();
				}
				if	(
						(lGetParameters)
					&&	(lGetParameters->Length > 0)
					)
				{
					lParameterTypes = gcnew array<Type^> (lGetParameters->Length);
					for	(lParameterNdx = 0; lParameterNdx < lGetParameters->Length; lParameterNdx++)
					{
						lParameterTypes [lParameterNdx] = GetTargetType (lGetParameters [lParameterNdx]->ParameterType, false);
					}
				}
				if	(lParameterTypes == nullptr)
				{
					lParameterTypes = gcnew array<Type^> (0);
				}
				lPropertyType = GetTargetType (lSourceProperty->PropertyType, false);

				if	(
						(mFixups)
					&&	(mFixups->FixupProperty (lSourceProperty, lPropertyName, lPropertyAttributes, lPropertyType))
					)
				{
					continue;
				}
#ifdef	_DEBUG_TARGET_PROPERTY
				LogMessage (_DEBUG_TARGET_PROPERTY, _T("%s    Property [%s] [%s] [%s] as [%s]"), _B(LogIndent()), _BM(lSourceProperty), _BMT(lSourceProperty), _B(PropertyAttrsStr(lPropertyAttributes)), _BT(lPropertyType));
#endif
				lPropertyBuilder = pTypeBuilder->DefineProperty (lPropertyName, lPropertyAttributes, lPropertyType, lParameterTypes);

#ifdef	_DEBUG_TARGET_PARAMETER
				for	(lParameterNdx = 0; lParameterNdx < lParameterTypes->Length; lParameterNdx++)
				{
					LogMessage (_DEBUG_TARGET_PARAMETER, _T("%s      Parameter {%s]"), _B(LogIndent()), _BT(lParameterTypes [lParameterNdx]));
				}
#endif

				if	(
						(lGetMethod = lSourceProperty->GetGetMethod ())
					&&	(pDefinedMethods->TryGetValue (GetMethodSignature (lGetMethod), lMethodBuilder))
					)
				{
#ifdef	_DEBUG_TARGET_PROPERTY
					LogMessage (_DEBUG_TARGET_PROPERTY, _T("%s      Get [%s]"), _B(LogIndent()), _BM(lMethodBuilder));
#endif
					lPropertyBuilder->SetGetMethod (lMethodBuilder);
				}

				if	(
						(lSetMethod = lSourceProperty->GetSetMethod ())
					&&	(pDefinedMethods->TryGetValue (GetMethodSignature (lSetMethod), lMethodBuilder))
					)
				{
#ifdef	_DEBUG_TARGET_PROPERTY
					LogMessage (_DEBUG_TARGET_PROPERTY, _T("%s      Set [%s]"), _B(LogIndent()), _BM(lMethodBuilder));
#endif
					lPropertyBuilder->SetSetMethod (lMethodBuilder);
				}

				try
				{
					List<CustomAttributeBuilder^>^	lBuilders;
					CustomAttributeBuilder^			lBuilder;

					if	(lBuilders = CopyAttributes (lSourceProperty, lPropertyBuilder, CustomAttributeData::GetCustomAttributes (lSourceProperty)))
					{
						for each (lBuilder in lBuilders)
						{
							lPropertyBuilder->SetCustomAttribute (lBuilder);
						}
					}
				}
				catch AnyExceptionDebug
			}
		}
	}
	catch AnyExceptionDebug
	{}
}

/////////////////////////////////////////////////////////////////////////////

void CopyAssembly::CopyEvents (Type^ pSourceType, TypeBuilder^ pTypeBuilder, DefinedMethods^ pDefinedMethods)
{
	try
	{
		array<EventInfo^>^	lSourceEvents;
		EventInfo^			lSourceEvent;

		if	(
				(lSourceEvents = pSourceType->GetEvents (BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
			&&	(lSourceEvents->Length > 0)
			)
		{
			for each (lSourceEvent in lSourceEvents)
			{
				Reflection::EventAttributes	lEventAttributes = lSourceEvent->Attributes;
				String^						lEventName = lSourceEvent->Name;
				Type^						lEventType;
				EventBuilder^				lEventBuilder;
				MethodInfo^					lAddMethod = nullptr;
				MethodInfo^					lRemoveMethod = nullptr;
				MethodInfo^					lRaiseMethod = nullptr;
				MethodBuilder^				lMethodBuilder;

				if	(!ReferenceEquals (lSourceEvent->DeclaringType, pSourceType))
				{
#ifdef	_DEBUG_TARGET_EVENT
					LogMessage (_DEBUG_TARGET_EVENT, _T("%s    Event [%s] from [%s] skipped for [%s]"), _B(LogIndent()), _B(lSourceEvent->Name), _BT(lSourceEvent->DeclaringType), _BT(pSourceType));
#endif
					continue;
				}
				if	(
						(mFixups)
					&&	(mFixups->FixupEvent (lSourceEvent, lEventName, lEventAttributes))
					)
				{
					continue;
				}

				lEventType = GetTargetType (lSourceEvent->EventHandlerType, false);
#ifdef	_DEBUG_TARGET_EVENT
				LogMessage (_DEBUG_TARGET_EVENT, _T("%s    Event [%s] [%s] [%s] as [%s]"), _B(LogIndent()), _B(lSourceEvent->Name), _B(EventAttrsStr(lEventAttributes)), _B(lSourceEvent->EventHandlerType->Name), _BT(lEventType));
#endif
				lEventBuilder = pTypeBuilder->DefineEvent (lEventName, lEventAttributes, lEventType);

				if	(
						(lAddMethod = lSourceEvent->GetAddMethod ())
					&&	(pDefinedMethods->TryGetValue (GetMethodSignature (lAddMethod), lMethodBuilder))
					)
				{
#ifdef	_DEBUG_TARGET_EVENT
					LogMessage (_DEBUG_TARGET_EVENT, _T("%s      Add [%s]"), _B(LogIndent()), _BM(lMethodBuilder));
#endif
					lEventBuilder->SetAddOnMethod (lMethodBuilder);
				}

				if	(
						(lRemoveMethod = lSourceEvent->GetRemoveMethod ())
					&&	(pDefinedMethods->TryGetValue (GetMethodSignature (lRemoveMethod), lMethodBuilder))
					)
				{
#ifdef	_DEBUG_TARGET_EVENT
					LogMessage (_DEBUG_TARGET_EVENT, _T("%s      Remove [%s]"), _B(LogIndent()), _BM(lMethodBuilder));
#endif
					lEventBuilder->SetRemoveOnMethod (lMethodBuilder);
				}

				if	(
						(lRaiseMethod = lSourceEvent->GetRaiseMethod ())
					&&	(pDefinedMethods->TryGetValue (lRaiseMethod->Name, lMethodBuilder))
					)
				{
#ifdef	_DEBUG_TARGET_EVENT
					LogMessage (_DEBUG_TARGET_EVENT, _T("%s      Raise [%s]"), _B(LogIndent()), _BM(lMethodBuilder));
#endif
					lEventBuilder->SetRaiseMethod (lMethodBuilder);
				}

				try
				{
					List<CustomAttributeBuilder^>^	lBuilders;
					CustomAttributeBuilder^			lBuilder;

					if	(lBuilders = CopyAttributes (lSourceEvent, lEventBuilder, CustomAttributeData::GetCustomAttributes (lSourceEvent)))
					{
						for each (lBuilder in lBuilders)
						{
							lEventBuilder->SetCustomAttribute (lBuilder);
						}
					}
				}
				catch AnyExceptionDebug
			}
		}
	}
	catch AnyExceptionDebug
	{}
}

/////////////////////////////////////////////////////////////////////////////

void CopyAssembly::CopyGenericParameters (Type^ pSourceType, TypeBuilder^ pTypeBuilder)
{
	try
	{
		array<Type^>^	lGenericArguments;
		array<String^>^	lGenericParameters;
		int				lNdx;

		if	(
				(lGenericArguments = pSourceType->GetGenericArguments ())
			&&	(lGenericArguments->Length > 0)
			&&	(lGenericParameters = gcnew array<String^> (lGenericArguments->Length))
			)
		{
			for	(lNdx = 0; lNdx < lGenericArguments->Length; lNdx++)
			{
				lGenericParameters[lNdx] = lGenericArguments[lNdx]->Name;
			}
			pTypeBuilder->DefineGenericParameters (lGenericParameters);
		}
	}
	catch AnyExceptionDebug
}

void CopyAssembly::CopyGenericParameters (MethodInfo^ pSourceMethod, MethodBuilder^ pMethodBuilder)
{
	try
	{
		array<Type^>^	lGenericArguments;
		array<String^>^	lGenericParameters;
		int				lNdx;

		if	(
				(lGenericArguments = pSourceMethod->GetGenericArguments ())
			&&	(lGenericArguments->Length > 0)
			&&	(lGenericParameters = gcnew array<String^> (lGenericArguments->Length))
			)
		{
			for	(lNdx = 0; lNdx < lGenericArguments->Length; lNdx++)
			{
				lGenericParameters[lNdx] = lGenericArguments[lNdx]->Name;
			}
			pMethodBuilder->DefineGenericParameters (lGenericParameters);
		}
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

List<CustomAttributeBuilder^>^ CopyAssembly::CopyAttributes (Object^ pSource, Object^ pTarget, CustomAttrDataList^ pAttributes)
{
	return CopyAttributes (pSource, pTarget, pAttributes, mFixups);
}

List<CustomAttributeBuilder^>^ CopyAssembly::CopyAttributes (Object^ pSource, Object^ pTarget, CustomAttrDataList^ pAttributes, CopyFixups^ pFixups)
{
	List<CustomAttributeBuilder^>^	lRet = gcnew List<CustomAttributeBuilder^>;
	List<CustomAttributeBuilder^>^	lBuilders = gcnew List<CustomAttributeBuilder^>;

	if	(pAttributes)
	{
		try
		{
			CustomAttributeData^			lAttribute = nullptr;
			int								lAttributeNdx = 0;
			CustomAttributeBuilder^			lCustomAttrBuilder = nullptr;

			for	(lAttributeNdx = 0; lAttributeNdx < pAttributes->Count; lAttributeNdx++)
			{
				array<PropertyInfo^>^	lNamedProperties = nullptr;
				array<Object^>^			lNamedValues = nullptr;
				array<Object^>^			lValues;
				int						lValueNdx;

				lAttribute = pAttributes [lAttributeNdx];

				if	(
						(lAttribute->NamedArguments)
					&&	(lAttribute->NamedArguments->Count > 0)
					)
				{
					lNamedProperties = gcnew array<PropertyInfo^> (lAttribute->NamedArguments->Count);
					lNamedValues = gcnew array<Object^> (lAttribute->NamedArguments->Count);
					for	(lValueNdx = 0; lValueNdx < lAttribute->NamedArguments->Count; lValueNdx++)
					{
						lNamedProperties [lValueNdx] = safe_cast <PropertyInfo^> (lAttribute->NamedArguments [lValueNdx].MemberInfo);
						lNamedValues [lValueNdx] = lAttribute->NamedArguments [lValueNdx].TypedValue.Value;
					}
				}

				lValues = gcnew array<Object^> (lAttribute->ConstructorArguments->Count);
				for	(lValueNdx = 0; lValueNdx < lAttribute->ConstructorArguments->Count; lValueNdx++)
				{
					lValues [lValueNdx] = lAttribute->ConstructorArguments [lValueNdx].Value;
				}

				if	(
						(pFixups)
					&&	(pFixups->FixupCustomAttribute (pSource, pTarget, lAttribute, lValues))
					)
				{
					continue;
				}

				try
				{
					if	(lNamedProperties)
					{
						lBuilders->Add (gcnew CustomAttributeBuilder (lAttribute->Constructor, lValues, lNamedProperties, lNamedValues));
					}
					else
					{
						lBuilders->Add (gcnew CustomAttributeBuilder (lAttribute->Constructor, lValues));
					}
				}
				catch AnyExceptionDebug
			}

			if	(pFixups)
			{
				pFixups->FixupCustomAttributes (pSource, pTarget, lBuilders);
			}
			if	(lBuilders->Count > 0)
			{
				lRet = lBuilders;
			}
		}
		catch AnyExceptionDebug
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

Type^ CopyAssembly::CopyEnum (Type^ pSourceType, String^ pTargetName, TypeAttributes pTargetAttrs)
{
	Type^	lTargetType = nullptr;

	try
	{
		EnumBuilder^		lEnumBuilder;
		Type ^				lUnderlyingType;
		array<FieldInfo^>^	lSourceFields;
		FieldInfo^			lSourceField;
		FieldBuilder^		lFieldBuilder;

		pTargetAttrs = (TypeAttributes) ((int)pTargetAttrs & (int)TypeAttributes::VisibilityMask);

		if	(
				(!mFixups)
			||	(!mFixups->FixupEnum (pSourceType, pTargetAttrs))
			)
		{
			lUnderlyingType = Enum::GetUnderlyingType(pSourceType);
#ifdef	_DEBUG_TARGET_TYPE
			LogMessage (_DEBUG_TARGET_TYPE, _T("%sCopy [%s] [%s] [%s] to [%s]"), _B(LogIndent()), _BT(pSourceType), _BT(lUnderlyingType), _B(TypeAttrsStr(pTargetAttrs)), _B(pTargetName));
#endif
			lEnumBuilder = mModuleBuilder->DefineEnum (pTargetName, pTargetAttrs, lUnderlyingType);

			if	(
					(lSourceFields = pSourceType->GetFields ())
				&&	(lSourceFields->Length > 0)
				)
			{
				for each (lSourceField in lSourceFields)
				{
					String^	lFieldName = lSourceField->Name;
					Object^	lFieldValue = nullptr;

					try
					{
						lFieldValue = lSourceField->GetRawConstantValue();
					}
					catch AnyExceptionSilent
					{}

					if	(lFieldValue)
					{
						if	(
								(mFixups)
							&&	(mFixups->FixupField (lSourceField, lFieldName, lEnumBuilder))
							)
						{
							continue;
						}
#ifdef	_DEBUG_TARGET_ENUM
						LogMessage (_DEBUG_TARGET_ENUM, _T("%s  Field [%s] (%s)"), _B(LogIndent()), _BM(lSourceField), _B(lFieldValue->ToString()));
#endif
						lFieldBuilder = lEnumBuilder->DefineLiteral (lFieldName, lFieldValue);
					}
					else
					if	(String::Compare (lSourceField->Name, COR_ENUM_FIELD_NAME, true) == 0)
					{
#ifdef	_DEBUG_TARGET_ENUM
						LogMessage (_DEBUG_TARGET_ENUM, _T("%s  Field [%s] [%s]"), _B(LogIndent()), _B(lEnumBuilder->UnderlyingField->Name), _B(FieldAttrsStr(lEnumBuilder->UnderlyingField->Attributes)));
#endif
					}
				}
			}

			lTargetType = lEnumBuilder;
		}
	}
	catch AnyExceptionDebug
	{}

	return lTargetType;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CopyAssembly::CopyMethodBodies ()
{
	try
	{
		KeyValuePair <ConstructorInfo^, ConstructorBuilder^>^	lCopiedConstructor;
		KeyValuePair <MethodInfo^, MethodBuilder^>^				lCopiedMethod;
		CopyILBinary^											lCopier = gcnew MakeILWrapper (mSourceModule, mModuleBuilder, nullptr, this);

		KeyValuePair <Type^, Type^>^							lCopiedType;
		Dictionary<String^,MakeILWrapper^>^						lWrappers = gcnew Dictionary<String^,MakeILWrapper^>;
		MakeILWrapper^											lWrapper;

		for each (lCopiedType in mCopiedTypes)
		{
			if	(IsCoClassWrapper (lCopiedType->Key))
			{
#ifdef	_LOG_WRAPPERS
				LogMessage (_LOG_WRAPPERS, _T("===> Type       [%s] wraps [%s]"), _BT(lCopiedType->Value), _BT(GetTargetType (GetCoClassInterface(lCopiedType->Key), false)));
#endif
				lWrappers->Add (lCopiedType->Key->FullName, gcnew MakeILWrapper (mSourceModule, mModuleBuilder, GetTargetType (GetCoClassInterface(lCopiedType->Key), false), this));
			}
		}

		for each (lCopiedConstructor in mCopiedConstructors)
		{
			CopyMethodBody (lCopiedConstructor->Key, lCopiedConstructor->Value, lCopier);
		}

		for each (lCopiedMethod in mCopiedMethods)
		{
			MethodInfo^		lSourceMethod = lCopiedMethod->Key;
			MethodBuilder^	lMethodBuilder = lCopiedMethod->Value;

			if	(
					(!lSourceMethod->IsAbstract)
				&&	(lWrappers->TryGetValue (lSourceMethod->DeclaringType->FullName, lWrapper))
				)
			{
				try
				{
					MethodInfo^	lWrappedMethod = FindMethodOverride (lSourceMethod, lMethodBuilder);
					if	(lWrappedMethod)
					{
#ifdef	_LOG_WRAPPERS
						LogMessage (_LOG_WRAPPERS, _T("===> Method     [%s.%s] wraps [%s.%s]"), _BMT(lMethodBuilder), _BM(lMethodBuilder), _BMT(lWrappedMethod), _BM(lWrappedMethod));
#endif
						lWrapper->MakeWrapperMethod (lSourceMethod, lMethodBuilder, lWrappedMethod);
					}
#ifdef	_LOG_WRAPPERS
					else
					{
						LogMessage (_LOG_WRAPPERS, _T("*==> Method     [%s.%s] wraps <no interface>"), _BMT(lMethodBuilder), _BM(lMethodBuilder));
					}
#endif
				}
				catch AnyExceptionDebug
			}
			else
			{
				CopyMethodBody (lSourceMethod, lMethodBuilder, lCopier);
				CopyMethodOverride (lSourceMethod, lMethodBuilder);
			}
		}
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////

void CopyAssembly::CopyMethodBody (MethodBase^ pSourceMethod, MethodBuilder^ pMethodBuilder)
{
	CopyMethodBody (pSourceMethod, pMethodBuilder, gcnew MakeILWrapper (mSourceModule, mModuleBuilder, nullptr, this));
}

void CopyAssembly::CopyMethodBody (MethodBase^ pSourceMethod, MethodBuilder^ pMethodBuilder, CopyILBinary^ pCopier)
{
	if	(
			(pSourceMethod)
		&&	(pMethodBuilder)
		&&	(pCopier)
		)
	{
		MethodBody^	lMethodBody;

		if	(
				(!pSourceMethod->IsAbstract)
			&&	(pSourceMethod->GetMethodBody())
			&&	(lMethodBody = pCopier->CopyMethodBody (pSourceMethod, pMethodBuilder->GetILGenerator()))
			)
		{
			pMethodBuilder->InitLocals = lMethodBody->InitLocals;
#ifdef	_DEBUG_TARGET_BODY
			LogMessage (_DEBUG_TARGET_BODY, _T("%s Body [%s.%s] from [%s.%s]"), _B(LogIndent()), _BMT(pMethodBuilder), _BM(pMethodBuilder), _BMT(pSourceMethod), _BM(pSourceMethod));
#endif
		}
	}
}

void CopyAssembly::CopyMethodOverride (MethodBase^ pSourceMethod, MethodBuilder^ pMethodBuilder)
{
	if	(
			(pSourceMethod)
		&&	(pMethodBuilder)
		&&	(
				(!pSourceMethod->IsAbstract)
			&&	(pSourceMethod->IsVirtual)
			&&	(
					(!pSourceMethod->GetMethodBody())
				||	(pSourceMethod->IsGenericMethod)
				||	(pSourceMethod->DeclaringType->IsGenericType)
				)
			&&	(String::Compare (pSourceMethod->Name, "Dispose", true) != 0)
			)
		)
	{
		try
		{
			TypeBuilder^	lTargetType;
			MethodInfo^		lTargetMethod;

			if	(
					(lTargetMethod = FindMethodOverride (pSourceMethod, pMethodBuilder))
				&&	(lTargetType = safe_cast <TypeBuilder^> (pMethodBuilder->DeclaringType))
				)
			{
#ifdef	_DEBUG_TARGET_BODY
				LogMessage (_DEBUG_TARGET_BODY, _T("%s Method [%s.%s] overrides [%s.%s]"), _B(LogIndent()), _BMT(pMethodBuilder), _BM(pMethodBuilder), _BMT(lTargetMethod), _BM(lTargetMethod));
#endif
				try
				{
					lTargetType->DefineMethodOverride (pMethodBuilder, lTargetMethod);
				}
				catch AnyExceptionDebug
			}
#ifdef	_DEBUG_TARGET_BODY
			else
			{
				LogMessage (_DEBUG_TARGET_BODY, _T("!!! [%s] [%s] override not found in [%s]"), _BM(pMethodBuilder), _BMT(pMethodBuilder), _BT(lOverrideType));
			}
#endif
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////

void CopyAssembly::CopyMethodBody (MethodBase^ pSourceMethod, ConstructorBuilder^ pConstructorBuilder)
{
	CopyMethodBody (pSourceMethod, pConstructorBuilder, gcnew CopyILBinary (mSourceModule, mModuleBuilder, this));
}

void CopyAssembly::CopyMethodBody (MethodBase^ pSourceMethod, ConstructorBuilder^ pConstructorBuilder, CopyILBinary^ pCopier)
{
	if	(
			(pSourceMethod)
		&&	(pConstructorBuilder)
		&&	(pCopier)
		)
	{
		MethodBody^	lMethodBody;

		if	(
				(!pSourceMethod->IsAbstract)
			&&	(pSourceMethod->GetMethodBody())
			&&	(lMethodBody = pCopier->CopyMethodBody (pSourceMethod, pConstructorBuilder->GetILGenerator()))
			)
		{
			pConstructorBuilder->InitLocals = lMethodBody->InitLocals;
#ifdef	_DEBUG_TARGET_BODY
			LogMessage (_DEBUG_TARGET_BODY, _T("%s Body [%s] [%s] from [%s] {%s]"), _B(LogIndent()), _BM(pConstructorBuilder), _BMT(pConstructorBuilder), _BM(pSourceMethod), _BMT(pSourceMethod));
#endif
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

MethodInfo^ CopyAssembly::FindMethodOverride (MethodBase^ pSourceMethod, MethodBuilder^ pMethodBuilder)
{
	MethodInfo^		lTargetMethod = nullptr;

	try
	{
		KeyValuePair <MethodInfo^, MethodBuilder^>^	lFindMethod;
		MethodInfo^									lOverrideMethod;
		Type^										lOverrideType;
		TypeBuilder^								lTargetType = nullptr;

		if	(
				(lOverrideMethod = IsMethodOverride (pSourceMethod))
			&&	(lOverrideType = GetTargetType (lOverrideMethod->DeclaringType, false))
			)
		{
#ifdef	_DEBUG_TARGET_BODY
			LogMessage (_DEBUG_TARGET_BODY, _T("%s Method [%s.%s] overrides [%s.%s] [%s]"), _B(LogIndent()), _BMT(pMethodBuilder), _BM(pMethodBuilder), _BT(lOverrideType), _BM(lOverrideMethod), _B(MethodAttrsStr(pMethodBuilder->Attributes)));
#endif
			try
			{
				lTargetType = safe_cast <TypeBuilder^> (pMethodBuilder->DeclaringType);
			}
			catch AnyExceptionSilent

			if	(lTargetType)
			{
#if	FALSE
				//
				//	Override always? - shouldn't cause problems because it's just making an implicit override explicit?
				//
				lTargetMethod = lOverrideMethod;
#endif
				for each (lFindMethod in mCopiedMethods)
				{
					if	(
							(ReferenceEquals (lFindMethod->Value->DeclaringType, lOverrideType))
						&&	(String::Compare (lFindMethod->Key->Name, pSourceMethod->Name) == 0)
						)
					{
						lTargetMethod = lFindMethod->Value;
						break;
					}
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lTargetMethod;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CopyAssembly::TranslateType (Type^ pSourceType, Type^& pTargetType)
{
	pTargetType = GetTargetType (pSourceType, false);

	if	(
			(ReferenceEquals (pTargetType, pSourceType))
		&&	(pSourceType->IsGenericType)
		&&	(!pSourceType->IsGenericTypeDefinition)
		)
	{
		try
		{
			Type^	lSourceType = pSourceType->GetGenericTypeDefinition ();
			Type^	lTargetType = GetTargetType (lSourceType, false);

			if	(lTargetType)
			{
				pTargetType = lTargetType;
			}
		}
		catch AnyExceptionSilent
	}
	return !ReferenceEquals (pTargetType, pSourceType);
}

bool CopyAssembly::TranslateMethod (MethodBase^ pSourceMethod, MethodInfo^& pTargetMethod)
{
	Type^			lSourceType = pSourceMethod->DeclaringType;
	Type^			lTargetType = nullptr;
	MethodInfo^		lTargetMethod = nullptr;

	if	(
			(lSourceType->IsGenericType)
		&&	(!lSourceType->IsGenericTypeDefinition)
		)
	{
		try
		{
			if	(lSourceType = lSourceType->GetGenericTypeDefinition())
			{
				pSourceMethod = lSourceType->GetMethod (pSourceMethod->Name, GetParameterTypes (pSourceMethod, false));
			}
		}
		catch AnyExceptionDebug
	}

	if	(
			(!String::IsNullOrEmpty (lSourceType->FullName))
		&&	(lSourceType->FullName->StartsWith ("System"))
		)
	{
		lTargetMethod = safe_cast <MethodInfo^> (pSourceMethod);
	}
	else
	if	(
			(!mTranslateMethods->TryGetValue (pSourceMethod, lTargetMethod))
		||	(!lTargetMethod)
		)
	{
		try
		{
			TranslateType (lSourceType, lTargetType);
			lTargetMethod = lTargetType->GetMethod (pSourceMethod->Name, BindingFlags::Instance|BindingFlags::Static|BindingFlags::Public|BindingFlags::NonPublic, nullptr, GetParameterTypes (pSourceMethod, true), nullptr);
		}
		catch AnyExceptionDebug
	}

	if	(lTargetMethod)
	{
		pTargetMethod = lTargetMethod;
		return true;
	}
#ifdef	_DEBUG_TARGET_CODE
	LogMessage (_DEBUG_TARGET_CODE, _T("!!! Method [%s.%s] to [%s.%s]"), _BT(lSourceType), _BM(pSourceMethod), _BT(lTargetType), _BM(pTargetMethod));
#endif
	return false;
}

bool CopyAssembly::TranslateConstructor (MethodBase^ pSourceConstructor, ConstructorInfo^& pTargetConstructor)
{
	Type^				lSourceType = pSourceConstructor->DeclaringType;
	Type^				lTargetType = nullptr;
	ConstructorInfo^	lTargetConstructor = nullptr;

	if	(
			(lSourceType->IsGenericType)
		&&	(!lSourceType->IsGenericTypeDefinition)
		)
	{
		try
		{
			if	(lSourceType = lSourceType->GetGenericTypeDefinition())
			{
				pSourceConstructor = lSourceType->GetConstructor (GetParameterTypes (pSourceConstructor, false));
			}
		}
		catch AnyExceptionDebug
	}

	if	(
			(!String::IsNullOrEmpty (lSourceType->FullName))
		&&	(lSourceType->FullName->StartsWith ("System"))
		)
	{
		lTargetConstructor = safe_cast <ConstructorInfo^> (pSourceConstructor);
	}
	else
	if	(
			(!mTranslateConstructors->TryGetValue (pSourceConstructor, lTargetConstructor))
		||	(!lTargetConstructor)
		)
	{
		try
		{
			TranslateType (lSourceType, lTargetType);
			lTargetConstructor = lTargetType->GetConstructor (GetParameterTypes (pSourceConstructor, true));
		}
		catch AnyExceptionDebug
	}

	if	(lTargetConstructor)
	{
		pTargetConstructor = lTargetConstructor;
		return true;
	}
#ifdef	_DEBUG_TARGET_CODE
	LogMessage (_DEBUG_TARGET_CODE, _T("!!! Method [%s.%s] to [%s.%s]"), _BT(lSourceType), _BM(pSourceConstructor), _BT(lTargetType), _BM(pTargetConstructor));
#endif
	return false;
}

bool CopyAssembly::TranslateField (FieldInfo^ pSourceField, FieldInfo^& pTargetField)
{
	Type^		lSourceType = pSourceField->DeclaringType;
	Type^		lTargetType = nullptr;
	FieldInfo^	lTargetField = nullptr;

	if	(
			(!mTranslateFields->TryGetValue (pSourceField, lTargetField))
		||	(!lTargetField)
		)
	{
		try
		{
			TranslateType (lSourceType, lTargetType);
			lTargetField = lTargetType->GetField (pSourceField->Name, BindingFlags::Instance|BindingFlags::Static|BindingFlags::Public|BindingFlags::NonPublic);
		}
		catch AnyExceptionSilent
	}

	if	(lTargetField)
	{
		pTargetField = lTargetField;
		return true;
	}
#ifdef	_DEBUG_TARGET_CODE
	LogMessage (LogDebug, _T("!!! Field [%s.%s] to [%s.%s]"), _BT(lSourceType), _BM(pSourceField), _BT(lTargetType), _BM(pSourceField));
#endif
	return false;
}

bool CopyAssembly::TranslateParameter (ParameterInfo^ pSourceParameter, Type^& pParameterType)
{
	TranslateType (pSourceParameter->ParameterType, pParameterType);
	return  mTranslateParameters->TryGetValue (pSourceParameter, pParameterType);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CopyAssembly::IsCoClassWrapper (Type^ pSourceType)
{
	bool	lRet = false;

	if	(mFixups)
	{
		lRet = mFixups->IsCoClassWrapper (pSourceType);
	}
	return	lRet;
}

Type^ CopyAssembly::GetCoClassInterface (Type^ pSourceType)
{
	Type^	lRet = nullptr;

	if	(mFixups)
	{
		lRet = mFixups->GetCoClassInterface (pSourceType);
	}
	return lRet;
}

Type^ CopyAssembly::GetCoClassWrapper (Type^ pSourceInterface)
{
	Type^	lRet = nullptr;

	if	(mFixups)
	{
		lRet = mFixups->GetCoClassWrapper (pSourceInterface);
	}
	return	lRet;
}

/////////////////////////////////////////////////////////////////////////////

void CopyAssembly::PutWrapperConstructor (Type^ pWrapperType, ConstructorBuilder^ pWrapperConstructor)
{
	mWrapperConstructors->Add (pWrapperType, pWrapperConstructor);
}

ConstructorBuilder^ CopyAssembly::GetWrapperConstructor (Type^ pWrapperType)
{
	ConstructorBuilder^	lWrapperConstructor = nullptr;

	mWrapperConstructors->TryGetValue (pWrapperType, lWrapperConstructor);
	return lWrapperConstructor;
}

void CopyAssembly::PutWrapperAccessor (Type^ pWrapperType, MethodBuilder^ pWrapperAccessor)
{
	mWrapperAccessors->Add (pWrapperType, pWrapperAccessor);
}

MethodBuilder^ CopyAssembly::GetWrapperAccessor (Type^ pWrapperType)
{
	MethodBuilder^	lWrapperAccessor = nullptr;

	mWrapperAccessors->TryGetValue (pWrapperType, lWrapperAccessor);
	return lWrapperAccessor;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

Type^ CopyAssembly::GetSourceType (String^ pSourceTypeName)
{
	Type^						lRet = nullptr;
	KeyValuePair<Type^, Type^>^	lTranslateType;

	for each (lTranslateType in mTranslateTypes)
	{
		if	(
				(!String::IsNullOrEmpty (lTranslateType->Key->FullName))
			&&	(String::Compare (lTranslateType->Key->FullName, pSourceTypeName, true) == 0)
			)
		{
			lRet = lTranslateType->Key;
			break;
		}
	}
	return lRet;
}

Type^ CopyAssembly::GetTargetType (Type^ pSourceType, bool pCreate)
{
	Type^	lSourceType = pSourceType;
	Type^	lTargetType = nullptr;

	if	(
			(lSourceType->IsByRef)
		||	(lSourceType->IsPointer)
		||	(lSourceType->IsArray)
		)
	{
		lSourceType = pSourceType->GetElementType();
	}

	if	(mCopiedTypes->TryGetValue (lSourceType, lTargetType))
	{
		if	(lTargetType == nullptr)
		{
#ifdef	_DEBUG_TARGET_TYPE
			LogMessage (_DEBUG_TARGET_TYPE, _T("%s==>"), _B(LogIndent()));
#endif

			lTargetType = CopyType (lSourceType);

#ifdef	_DEBUG_TARGET_TYPE
			LogMessage (_DEBUG_TARGET_TYPE, _T("%s<=="), _B(LogIndent()));
#endif
		}

		if	(
				(lTargetType)
			&&	(pCreate)
			&&	(
					(TypeBuilder::typeid->IsInstanceOfType (lTargetType))
				||	(EnumBuilder::typeid->IsInstanceOfType (lTargetType))
				)
			)
		{
			try
			{
				lTargetType = CreateType (lSourceType, lTargetType);
			}
			catch AnyExceptionSilent
		}
	}
	else
	{
		mTranslateTypes->TryGetValue (lSourceType, lTargetType);
	}

	if	(lTargetType)
	{
		if	(pSourceType->IsByRef)
		{
			lTargetType = lTargetType->MakeByRefType ();
		}
		else
		if	(pSourceType->IsPointer)
		{
			lTargetType = lTargetType->MakePointerType ();
		}
		else
		if	(pSourceType->IsArray)
		{
			lTargetType = lTargetType->MakeArrayType ();
		}
	}

	if	(lTargetType == nullptr)
	{
		lTargetType = pSourceType;
	}
	return lTargetType;
}

Type^ CopyAssembly::GetTargetType (String^ pSourceTypeName, bool pCreate)
{
	Type^						lRet = nullptr;
	KeyValuePair<Type^, Type^>^	lTranslateType;

	for each (lTranslateType in mTranslateTypes)
	{
		if	(
				(!String::IsNullOrEmpty (lTranslateType->Key->FullName))
			&&	(String::Compare (lTranslateType->Key->FullName, pSourceTypeName, true) == 0)
			)
		{
			lRet = GetTargetType (lTranslateType->Key, pCreate);
			break;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

Type^ CopyAssembly::GetTargetReturnType (MethodInfo^ pSourceMethod, bool pCreate)
{
	Type^	lReturnType;

	if	(
			(pSourceMethod)
		&&	(pSourceMethod->ReturnType)
		&&	(!ReferenceEquals (pSourceMethod->ReturnType, System::Void::typeid))
		&&	(lReturnType = GetTargetType (pSourceMethod->ReturnType, pCreate))
		)
	{
		if	(mFixups)
		{
			mFixups->FixupReturnType (pSourceMethod, lReturnType);
		}
	}
	return lReturnType;
}

Type^ CopyAssembly::GetTargetParameterType (MethodBase^ pSourceMethod, ParameterInfo^ pSourceParameter, bool pCreate)
{
	Type^	lParameterType;

	if	(
			(pSourceMethod)
		&&	(pSourceParameter)
		&&	(lParameterType = GetTargetType (pSourceParameter->ParameterType, pCreate))
		)
	{
		if	(mFixups)
		{
			mFixups->FixupParameter (pSourceMethod, pSourceParameter, lParameterType);
		}
	}
	return lParameterType;
}

Type^ CopyAssembly::GetTargetArgumentType (Type^ pSourceType, bool pCreate)
{
	Type^	lArgumentType;

	if	(
			(pSourceType)
		&&	(lArgumentType = GetTargetType (pSourceType, pCreate))
		)
	{
		if	(mFixups)
		{
			mFixups->FixupTypeArgument (pSourceType, lArgumentType);
		}
	}
	return lArgumentType;
}

/////////////////////////////////////////////////////////////////////////////

array<Type^>^ CopyAssembly::GetParameterTypes (MethodBase^ pMethod, bool pTargetTypes)
{
	array<Type^>^	lParameterTypes = nullptr;

	try
	{
		array<ParameterInfo^>^	lParameters = nullptr;
		int						lParameterNdx;

		lParameters = pMethod->GetParameters ();

		if	(lParameters)
		{
			lParameterTypes = gcnew array<Type^> (lParameters->Length);

			for	(lParameterNdx = 0; lParameterNdx < lParameters->Length; lParameterNdx++)
			{
				if	(pTargetTypes)
				{
					lParameterTypes [lParameterNdx] = GetTargetParameterType (pMethod, lParameters [lParameterNdx], false);
				}
				else
				{
					lParameterTypes [lParameterNdx] = lParameters [lParameterNdx]->ParameterType;
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lParameterTypes;
}

array<Type^>^ CopyAssembly::GetParameterTypes (ConstructorInfo^ pConstuctor, bool pTargetTypes)
{
	array<Type^>^	lParameterTypes = nullptr;

	try
	{
		array<ParameterInfo^>^	lParameters = nullptr;
		int						lParameterNdx;

		lParameters = pConstuctor->GetParameters ();

		if	(lParameters)
		{
			lParameterTypes = gcnew array<Type^> (lParameters->Length);

			for	(lParameterNdx = 0; lParameterNdx < lParameters->Length; lParameterNdx++)
			{
				if	(pTargetTypes)
				{
					lParameterTypes [lParameterNdx] = GetTargetParameterType (pConstuctor, lParameters [lParameterNdx], false);
				}
				else
				{
					lParameterTypes [lParameterNdx] = lParameters [lParameterNdx]->ParameterType;
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lParameterTypes;
}

/////////////////////////////////////////////////////////////////////////////

String^ CopyAssembly::GetMethodSignature (MethodBase^ pMethod)
{
	StringBuilder^	lSignature = gcnew StringBuilder;
	array<Type^>^	lParameterTypes;
	int				lParameterNdx;

	lSignature->Append (pMethod->DeclaringType->FullName);
	lSignature->Append (".");
	lSignature->Append (pMethod->Name);
	lSignature->Append ("(");

	if	(lParameterTypes = GetParameterTypes (pMethod, false))
	{
		for	(lParameterNdx = 0; lParameterNdx < lParameterTypes->Length; lParameterNdx++)
		{
			lSignature->Append (lParameterTypes [lParameterNdx]->FullName);
			if	(lParameterNdx < lParameterTypes->Length-1)
			{
				lSignature->Append (",");
			}
		}
	}
	lSignature->Append (")");

	return lSignature->ToString ();
}

/////////////////////////////////////////////////////////////////////////////

Type^ CopyAssembly::GetEnumeratedType (Type^ pType)
{
	Type^	lRet = nullptr;

	try
	{
		array<PropertyInfo^>^	lProperties;
		PropertyInfo^			lProperty;
		DispIdAttribute^		lDispId;

		if	(
				(pType)
			&&	(pType->GetInterface ("IEnumerable"))
			&&	(lProperties = pType->GetProperties (BindingFlags::Instance|BindingFlags::Public))
			)
		{
			for each (lProperty in lProperties)
			{
				if	(
						(lDispId = GetDispId (lProperty))
					&&	(lDispId->Value == DISPID_VALUE)
					)
				{
					lRet = lProperty->PropertyType;
					break;
				}
			}
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

TypeLibFuncFlags CopyAssembly::GetTypeLibFuncFlags (MethodInfo^ pMethod)
{
	TypeLibFuncFlags	lRet = (TypeLibFuncFlags)0;

	try
	{
		TypeLibFuncAttribute^	lTypeLibFuncAttribute = nullptr;

		lTypeLibFuncAttribute = safe_cast <TypeLibFuncAttribute^> (Attribute::GetCustomAttribute (pMethod, TypeLibFuncAttribute::typeid, false));
		if	(lTypeLibFuncAttribute)
		{
			lRet = lTypeLibFuncAttribute->Value;
		}
	}
	catch AnyExceptionSilent
	{}
	return lRet;
}

bool CopyAssembly::GetTypeLibFuncFlags (MethodInfo^ pMethod, TypeLibFuncFlags & pFuncFlags)
{
	bool	lRet = false;

	try
	{
		TypeLibFuncAttribute^	lTypeLibFuncAttribute = nullptr;

		lTypeLibFuncAttribute = safe_cast <TypeLibFuncAttribute^> (Attribute::GetCustomAttribute (pMethod, TypeLibFuncAttribute::typeid, false));
		if	(lTypeLibFuncAttribute)
		{
			pFuncFlags = lTypeLibFuncAttribute->Value;
			lRet = true;
		}
	}
	catch AnyExceptionSilent
	{}
	return lRet;
}

TypeLibFuncFlags CopyAssembly::GetTypeLibFuncFlags (PropertyInfo^ pProperty)
{
	TypeLibFuncFlags	lRet = (TypeLibFuncFlags)0;

	try
	{
		MethodInfo^	lGetMethod;

		if	(lGetMethod = pProperty->GetGetMethod ())
		{
			lRet = GetTypeLibFuncFlags (lGetMethod);
		}
	}
	catch AnyExceptionSilent
	{}
	return lRet;
}

bool CopyAssembly::GetTypeLibFuncFlags (PropertyInfo^ pProperty, TypeLibFuncFlags & pFuncFlags)
{
	bool	lRet = false;

	try
	{
		MethodInfo^	lGetMethod;

		if	(lGetMethod = pProperty->GetGetMethod ())
		{
			lRet = GetTypeLibFuncFlags (lGetMethod, pFuncFlags);
		}
	}
	catch AnyExceptionSilent
	{}
	return lRet;
}

TypeLibVarFlags CopyAssembly::GetTypeLibVarFlags (PropertyInfo^ pProperty)
{
	TypeLibVarFlags	lRet = (TypeLibVarFlags)0;

	try
	{
		TypeLibVarAttribute^	lTypeLibVarAttribute = nullptr;

		lTypeLibVarAttribute = safe_cast <TypeLibVarAttribute^> (Attribute::GetCustomAttribute (pProperty, TypeLibVarAttribute::typeid, false));
		if	(lTypeLibVarAttribute)
		{
			lRet = lTypeLibVarAttribute->Value;
		}
	}
	catch AnyExceptionSilent
	{}
	return lRet;
}

bool CopyAssembly::GetTypeLibVarFlags (PropertyInfo^ pProperty, TypeLibVarFlags & pVarFlags)
{
	bool	lRet = false;

	try
	{
		TypeLibVarAttribute^	lTypeLibVarAttribute = nullptr;

		lTypeLibVarAttribute = safe_cast <TypeLibVarAttribute^> (Attribute::GetCustomAttribute (pProperty, TypeLibVarAttribute::typeid, false));
		if	(lTypeLibVarAttribute)
		{
			pVarFlags = lTypeLibVarAttribute->Value;
			lRet = true;
		}
	}
	catch AnyExceptionSilent
	{}
	return lRet;
}

TypeLibTypeFlags CopyAssembly::GetTypeLibTypeFlags (Type^ pType)
{
	TypeLibTypeFlags	lRet = (TypeLibTypeFlags)0;

	try
	{
		TypeLibTypeAttribute^	lTypeLibTypeAttribute;

		lTypeLibTypeAttribute = safe_cast <TypeLibTypeAttribute^> (Attribute::GetCustomAttribute (pType, TypeLibTypeAttribute::typeid));
		if	(lTypeLibTypeAttribute)
		{
			lRet = lTypeLibTypeAttribute->Value;
		}
	}
	catch AnyExceptionSilent
	{}
	return lRet;
}

bool CopyAssembly::GetTypeLibTypeFlags (Type^ pType, TypeLibTypeFlags & pTypeFlags)
{
	bool	lRet = false;

	try
	{
		TypeLibTypeAttribute^	lTypeLibTypeAttribute;

		lTypeLibTypeAttribute = safe_cast <TypeLibTypeAttribute^> (Attribute::GetCustomAttribute (pType, TypeLibTypeAttribute::typeid));
		if	(lTypeLibTypeAttribute)
		{
			pTypeFlags = lTypeLibTypeAttribute->Value;
			lRet = true;
		}
	}
	catch AnyExceptionSilent
	{}
	return lRet;
}

TypeLibTypeFlags CopyAssembly::GetTypeLibTypeFlags (String^ pTypeName)
{
	TypeLibTypeFlags			lRet = (TypeLibTypeFlags)0;
	KeyValuePair<Type^, Type^>^	lTranslateType;

	for each (lTranslateType in mTranslateTypes)
	{
		if	(String::Compare (lTranslateType->Key->FullName, pTypeName, true) == 0)
		{
			lRet = GetTypeLibTypeFlags (lTranslateType->Key);
			break;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

DispIdAttribute^ CopyAssembly::GetDispId (MethodInfo^ pMethod)
{
	DispIdAttribute^	lRet = nullptr;

	try
	{
		if	(pMethod)
		{
			lRet = safe_cast <DispIdAttribute^> (Attribute::GetCustomAttribute (pMethod, DispIdAttribute::typeid));
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

DispIdAttribute^ CopyAssembly::GetDispId (PropertyInfo^ pProperty)
{
	DispIdAttribute^	lRet = nullptr;

	try
	{
		if	(pProperty)
		{
			lRet = safe_cast <DispIdAttribute^> (Attribute::GetCustomAttribute (pProperty, DispIdAttribute::typeid));
		}
	}
	catch AnyExceptionSilent

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

CoClassAttribute^ CopyAssembly::GetCoClass (Type^ pType)
{
	CoClassAttribute^	lRet = nullptr;

	try
	{
		array<Object^>^	lAttributes;

		if	(
				(pType)
			&&	(lAttributes = pType->GetCustomAttributes (CoClassAttribute::typeid, true))
			&&	(lAttributes->Length > 0)
			)
		{
			lRet = safe_cast <CoClassAttribute^> (lAttributes[0]);
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

String^ CopyAssembly::LogIndent ()
{
	return gcnew String(' ', mLogIndent*2);
}

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
