#include "stdafx.h"
#include <corhdr.h>
#include "CopyAssembly.h"

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
//#define	_DEBUG_RESOLVE_TYPE		LogNormal
#define	_DEBUG_TARGET_CODE		LogDebug
#endif

namespace DoubleAgent {
namespace TlbToAsm {
/////////////////////////////////////////////////////////////////////////////

CopyAssembly::CopyAssembly ()
{
	mSavedAssemblies = gcnew List<Assembly^>;
	mTranslateTypes = gcnew Dictionary <Type^, Type^>;
	mTranslateMethods = gcnew Dictionary <MethodBase^, MethodInfo^>;
	mTranslateConstructors = gcnew Dictionary <MethodBase^, ConstructorInfo^>;
	mTranslateFields = gcnew Dictionary <FieldInfo^, FieldInfo^>;
}

CopyAssembly::~CopyAssembly ()
{
}

/////////////////////////////////////////////////////////////////////////////

AssemblyBuilder^ CopyAssembly::DoCopy (Assembly^ pSourceAssembly, String^ pAssemblyName, String^ pModuleName, StrongNameKeyPair^ pStrongName)
{
	AssemblyBuilder^	lTargetAssembly = nullptr;

	try
	{
		AssemblyName^	lAssemblyName = gcnew AssemblyName;
		String^			lFilePath = IO::Path::GetDirectoryName (pAssemblyName);
		String^			lFileName = IO::Path::GetFileName (pAssemblyName);

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

		CopyTypes ();
		CopyMethodBodies ();
		CreateTypes ();

		try
		{
			List<CustomAttributeBuilder^>^	lBuilders;
			CustomAttributeBuilder^			lBuilder;

			if	(lBuilders = CopyAttributes (pSourceAssembly, mAssemblyBuilder, CustomAttributeData::GetCustomAttributes (pSourceAssembly)))
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
	array<Type^>^	lSourceTypes = nullptr;
	Type^			lSourceType = nullptr;

	mLogIndent = 0;
	mCopiedTypes = gcnew Dictionary <Type^, Type^>;
	mCopiedMethods = gcnew Dictionary <MethodBase^, MethodBuilder^>;
	mCopiedConstructors = gcnew Dictionary <MethodBase^, ConstructorBuilder^>;
	mCopiedFields = gcnew Dictionary <FieldInfo^, FieldBuilder^>;

	if	(lSourceTypes = mSourceAssembly->GetTypes())
	{
		GetInterfaceMappings (lSourceTypes);

		for each (lSourceType in lSourceTypes)
		{
			mCopiedTypes->Add (lSourceType, nullptr);
			mTranslateTypes->Add (lSourceType, nullptr);
		}
		for each (lSourceType in lSourceTypes)
		{
			Type^	lTargetType = nullptr;

			if	(
					(mCopiedTypes->TryGetValue (lSourceType, lTargetType))
				&&	(lTargetType)
				)
			{
				continue;
			}
			CopyType (lSourceType);
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
			mCopiedTypes [pSourceType] = lTargetType;
			mTranslateTypes [pSourceType] = lTargetType;
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
	ResolveEventHandler^			lTypeHandler = gcnew ResolveEventHandler (this, &CopyAssembly::ResolveType);
	ResolveEventHandler^			lAssemblyHandler = gcnew ResolveEventHandler (this, &CopyAssembly::ResolveAssembly);

	mLogIndent = 0;
	AppDomain::CurrentDomain->TypeResolve::add (lTypeHandler);
	AppDomain::CurrentDomain->AssemblyResolve::add (lAssemblyHandler);

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
			CreateType (lCopiedType->Key, mCopiedTypes [lCopiedType->Key]);
		}
		for each (lCopiedType in lCopiedTypes)
		{
			TypeBuilder^	lTypeBuilder = nullptr;
			EnumBuilder^	lEnumBuilder = nullptr;

			try
			{
				lTypeBuilder = safe_cast <TypeBuilder^> (lCopiedType->Value);
			}
			catch AnyExceptionSilent
			try
			{
				lEnumBuilder = safe_cast <EnumBuilder^> (lCopiedType->Value);
			}
			catch AnyExceptionSilent

			if	(lTypeBuilder)
			{
				try
				{
					List<CustomAttributeBuilder^>^	lBuilders;
					CustomAttributeBuilder^			lBuilder;

					if	(lBuilders = CopyAttributes (lCopiedType->Key, lTypeBuilder, CustomAttributeData::GetCustomAttributes (lCopiedType->Key)))
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

					if	(lBuilders = CopyAttributes (lCopiedType->Key, lEnumBuilder, CustomAttributeData::GetCustomAttributes (lCopiedType->Key)))
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
	catch AnyExceptionDebug

	AppDomain::CurrentDomain->TypeResolve::remove (lTypeHandler);
	AppDomain::CurrentDomain->AssemblyResolve::remove (lAssemblyHandler);
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

			if	(lTargetType)
			{
				mCopiedTypes [pSourceType] = lTargetType;
				mTranslateTypes [pSourceType] = lTargetType;
#ifdef	_DEBUG_CREATE_TYPE
				LogMessage (_DEBUG_CREATE_TYPE, _T("%s    Created   [%s] for [%s]"), _B(LogIndent()), _BT(lTargetType), _BT(pSourceType));
#endif
			}
		}
		catch AnyExceptionDebug
	}

	return lTargetType;
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

	try
	{
		Assembly^	lAssembly;

#ifdef	_DEBUG_RESOLVE_TYPE
		LogMessage (_DEBUG_RESOLVE_TYPE, _T("%s  ResolveAssembly [%s]"), _B(LogIndent()), _B(pEventArgs->Name));
#endif
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
		DefinedMethods^	lDefinedMethods;

		if	(
				(!mFixups)
			||	(!mFixups->FixupType (pSourceType, pTargetName, pTargetAttrs))
			)
		{
#ifdef	_DEBUG_TARGET_TYPE
			LogMessage (_DEBUG_TARGET_TYPE, _T("%sCopy [%s] [%s] to [%s] [%s]"), _B(LogIndent()), _BT(pSourceType), _B(TypeProps(pSourceType)), _B(pTargetName), _B(TypeAttrsStr(pTargetAttrs)));
#endif
			lTypeBuilder = mModuleBuilder->DefineType (pTargetName, pTargetAttrs);

			mCopiedTypes [pSourceType] = lTypeBuilder;
			mTranslateTypes [pSourceType] = lTypeBuilder;

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
					Type^	lTargetParent = GetTargetType (pSourceType->BaseType, false);
#ifdef	_DEBUG_TARGET_TYPE
					LogMessage (_DEBUG_TARGET_TYPE, _T("%s  Copy Base [%s] as [%s]"), _B(LogIndent()), _BT(pSourceType->BaseType), _BT(lTargetParent));
#endif
					lTypeBuilder->SetParent (lTargetParent);
				}
			}

			CopyInterfaces (pSourceType, lTypeBuilder);
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

void CopyAssembly::CopyInterfaces (Type^ pSourceType, TypeBuilder^ pTypeBuilder)
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
				if	(lSourceInterface->FullName->IndexOf (_T("UnsafeNativeMethods")) > 0)
				{
#ifdef	_DEBUG_TARGET_INTERFACE
					LogMessage (_DEBUG_TARGET_INTERFACE, _T("%s    Skip AxNative  [%s]"), _B(LogIndent()), _BT(lSourceInterface));
#endif
				}
				else
				{
					lTargetInterface = GetTargetType (lSourceInterface, false);
#ifdef	_DEBUG_TARGET_INTERFACE
					LogMessage (_DEBUG_TARGET_INTERFACE, _T("%s    Interface [%s] as [%s]"), _B(LogIndent()), _BT(lSourceInterface), _BT(lTargetInterface));
#endif
					pTypeBuilder->AddInterfaceImplementation (lTargetInterface);
				}
			}
		}
	}
	catch AnyExceptionDebug
	{}
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

				if	(!Object::ReferenceEquals (lSourceField->DeclaringType, pSourceType))
				{
#ifdef	_DEBUG_TARGET_FIELD
					LogMessage (_DEBUG_TARGET_FIELD, _T("%s    Field [%s] from [%s] skipped for [%s]"), _B(LogIndent()), _BM(lSourceField), _BMT(lSourceField), _BT(pSourceType));
#endif
					continue;
				}
				if	(
						(mFixups)
					&&	(mFixups->FixupField (lSourceField, lFieldName, lFieldAttributes))
					)
				{
					continue;
				}

				lFieldType = GetTargetType (lSourceField->FieldType, false);
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
				array<ParameterInfo^>^	lParameters = nullptr;
				array<Type^>^			lParameterTypes = GetParameterTypes (lSourceConstructor);
				int						lParameterNdx;

				try
				{
					lParameters = lSourceConstructor->GetParameters ();
				}
				catch AnyExceptionSilent
#ifdef	_DEBUG_TARGET_METHOD
				LogMessage (_DEBUG_TARGET_METHOD, _T("%s    Constructor [%s] [%s] [%s]"), _B(LogIndent()), _B(MethodAttrsStr(lSourceConstructor->Attributes)), _B(MethodCallType(lSourceConstructor->CallingConvention)), _B(MethodImplementation(lSourceConstructor->GetMethodImplementationFlags())));
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
					lConstructorBuilder = pTypeBuilder->DefineDefaultConstructor (lSourceConstructor->Attributes);
				}
				else
				{
					lConstructorBuilder = pTypeBuilder->DefineConstructor (lSourceConstructor->Attributes, lSourceConstructor->CallingConvention, lParameterTypes);

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
					}
				}

				lConstructorBuilder->SetImplementationFlags (lSourceConstructor->GetMethodImplementationFlags ());
				mCopiedConstructors [lSourceConstructor] = lConstructorBuilder;
				mTranslateConstructors [lSourceConstructor] = lConstructorBuilder;
			}
		}
	}
	catch AnyExceptionDebug
	{}
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

		if	(lSourceMethods = pSourceType->GetMethods (BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
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

				if	(!Object::ReferenceEquals (lSourceMethod->DeclaringType, pSourceType))
				{
#ifdef	_DEBUG_TARGET_METHOD
					LogMessage (_DEBUG_TARGET_METHOD, _T("%s    Method [%s] from [%s] skipped for [%s]"), _B(LogIndent()), _BM(lSourceMethod), _BMT(lSourceMethod), _BT(pSourceType));
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
				LogMessage (_DEBUG_TARGET_METHOD, _T("%s    Method [%3.3d] [%s] [%s] [%s] [%s]"), _B(LogIndent()), lMethodNdx, _BM(lSourceMethod), _B(MethodAttrsStr(lMethodAttrs)), _B(MethodCallType(lSourceMethod->CallingConvention)), _B(MethodImplementation(lSourceMethod->GetMethodImplementationFlags())));
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
				bool			lDefineReturnParameter = false;

				lSourceMethod = pSourceMethods [lMethodNdx];
				if	(!lTargetMethods->TryGetValue (lSourceMethod, lMethodBuilder))
				{
					continue;
				}

				try
				{
					if	(
							(lSourceMethod->ReturnType)
						&&	(String::Compare (lSourceMethod->ReturnType->FullName, gcnew String("System.Void"), true) != 0)
						)
					{
						Type^	lReturnType = GetTargetType (lSourceMethod->ReturnType, false);

						if	(mFixups)
						{
							mFixups->FixupReturnType (lSourceMethod, lMethodBuilder, lReturnType);
						}
#ifdef	_DEBUG_TARGET_PARAMETER
						LogMessage (_DEBUG_TARGET_PARAMETER, _T("%s      Copy ReturnType [%s] as [%s] for [%s.%s]"), _B(LogIndent()), _BT(lSourceMethod->ReturnType), _BT(lReturnType), _BMT(lSourceMethod), _BM(lSourceMethod));
#endif
#if	FALSE
						if	(
								(lSourceMethod->ReturnParameter)
							&&	(lSourceMethod->ReturnParameter->GetRequiredCustomModifiers())
							&&	(lSourceMethod->ReturnParameter->GetRequiredCustomModifiers()->Length > 0)
							)
						{
							LogMessage (LogDebug, _T("------ Return GetRequiredCustomModifiers [%d]"), lSourceMethod->ReturnParameter->GetRequiredCustomModifiers()->Length);
						}
						if	(
								(lSourceMethod->ReturnParameter)
							&&	(lSourceMethod->ReturnParameter->GetOptionalCustomModifiers())
							&&	(lSourceMethod->ReturnParameter->GetOptionalCustomModifiers()->Length > 0)
							)
						{
							LogMessage (LogDebug, _T("------ Return GetOptionalCustomModifiers [%d]"), lSourceMethod->ReturnParameter->GetOptionalCustomModifiers()->Length);
						}
#endif
						lMethodBuilder->SetReturnType (lReturnType);
						lDefineReturnParameter = true;
					}
				}
				catch AnyExceptionDebug

				try
				{
					array<ParameterInfo^>^	lParameters = lSourceMethod->GetParameters ();
					array<Type^>^			lParameterTypes;
					int						lParameterNdx;

					if	(
							(lParameters)
						&&	(lParameters->Length > 0)
						&&	(lParameterTypes = GetParameterTypes (lSourceMethod))
						)
					{
						for	(lParameterNdx = 0; lParameterNdx < lParameters->Length; lParameterNdx++)
						{
							Type^	lParameterType = lParameterTypes [lParameterNdx];

							if	(mFixups)
							{
								mFixups->FixupParameter (lSourceMethod, lMethodBuilder, lParameters [lParameterNdx], lParameterType);
								lParameterTypes [lParameterNdx] = lParameterType;
							}
						}
#ifdef	_DEBUG_TARGET_PARAMETER
						LogMessage (_DEBUG_TARGET_PARAMETER, _T("%s      Copy [%d] Parameters for [%s.%s]"), _B(LogIndent()), lParameters->Length, _BMT(lSourceMethod), _BM(lSourceMethod));
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
						lMethodBuilder->SetParameters (lParameterTypes);

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
								mFixups->FixupParameter (lSourceMethod, lParameter, lParameterName, lParameterAttributes);
							}
							lParameterBuilder = lMethodBuilder->DefineParameter (lParameterNdx+1, lParameterAttributes, lParameterName);

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

								if	(lAttributeBuilder = CopyMarshalAttribute (lParameter->GetCustomAttributes (MarshalAsAttribute::typeid, false)))
								{
									lParameterBuilder->SetCustomAttribute (lAttributeBuilder);
								}
							}
							catch AnyExceptionDebug

							if	(mFixups)
							{
								mFixups->FixupParameter (lSourceMethod, lMethodBuilder, lParameter, lParameterBuilder);
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
						}
					}
				}
				catch AnyExceptionDebug

				if	(lDefineReturnParameter)
				{
					try
					{
						ParameterBuilder^		lParameterBuilder = lMethodBuilder->DefineParameter (0, lSourceMethod->ReturnParameter->Attributes, String::Empty);
						CustomAttributeBuilder^	lAttributeBuilder;

						if	(
								(lSourceMethod->ReturnTypeCustomAttributes)
							&&	(lAttributeBuilder = CopyMarshalAttribute (lSourceMethod->ReturnTypeCustomAttributes->GetCustomAttributes (MarshalAsAttribute::typeid, false)))
							)
						{
							lParameterBuilder->SetCustomAttribute (lAttributeBuilder);
						}

						if	(mFixups)
						{
							mFixups->FixupReturnType (lSourceMethod, lMethodBuilder, lParameterBuilder);
						}
#if	FALSE
						try
						{
							List<CustomAttributeBuilder^>^	lBuilders;
							CustomAttributeBuilder^			lBuilder;

							if	(lBuilders = CopyAttributes (lSourceMethod->ReturnParameterlParameterBuilder, CustomAttributeData::GetCustomAttributes (lSourceMethod->ReturnParameter)))
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

				lMethodBuilder->SetImplementationFlags (lSourceMethod->GetMethodImplementationFlags ());

				try
				{
					List<CustomAttributeBuilder^>^	lBuilders;
					CustomAttributeBuilder^			lBuilder;

					if	(lBuilders = CopyAttributes (lSourceMethod, lMethodBuilder, CustomAttributeData::GetCustomAttributes (lSourceMethod)))
					{
						for each (lBuilder in lBuilders)
						{
							lMethodBuilder->SetCustomAttribute (lBuilder);
						}
					}
				}
				catch AnyExceptionDebug

				try
				{
					pDefinedMethods->Add (GetMethodSignature (lSourceMethod), lMethodBuilder);
				}
				catch AnyExceptionDebug

				mCopiedMethods [lSourceMethod] = lMethodBuilder;
				mTranslateMethods [lSourceMethod] = lMethodBuilder;
			}
		}
		catch AnyExceptionDebug
		{}
	}
}

CustomAttributeBuilder^ CopyAssembly::CopyMarshalAttribute (array<Object^>^ pAttributes)
{
	CustomAttributeBuilder^	lRet;

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

			if	(lAttribute)
			{
				Type^						lAttributeType = lAttribute->GetType();
				ConstructorInfo^			lConstructor = lAttributeType->GetConstructors (BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public) [0];
				array<Object^>^				lParameters = gcnew array<Object^> (1);
				array<FieldInfo^>^			lFields = lAttributeType->GetFields (BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public);
				Generic::List <FieldInfo^>^	lFieldInfos = gcnew Generic::List <FieldInfo^>;
				Generic::List <Object^>^	lFieldValues = gcnew Generic::List <Object^>;
				Object^						lFieldValue;
				int							lNdx;

				lParameters[0] = lAttribute->Value;

				for	(lNdx = 0; lNdx < lFields->Length; lNdx++)
				{
					if	(
							(
								(lAttribute->Value != UnmanagedType::SafeArray)
							||	(
									(String::Compare (lFields[lNdx]->Name, "ArraySubType") != 0)
								&&	(String::Compare (lFields[lNdx]->Name, "SizeConst") != 0)
								&&	(String::Compare (lFields[lNdx]->Name, "SizeParamIndex") != 0)
								)
							)
						&&	(String::Compare (lFields[lNdx]->Name, "IidParameterIndex") != 0)
						&&	(lFieldValue = lFields[lNdx]->GetValue(lAttribute))
						)
					{
						lFieldInfos->Add (lFields[lNdx]);
						lFieldValues->Add (lFieldValue);
					}
				}
#if	FALSE
				LogMessage (LogDebug, _T("  Attribute [%s] Type [%s] [%s] [%s]"), _B(lAttribute->ToString()), _B(lAttribute->Value.ToString()), _B(lAttribute->ArraySubType.ToString()), _B(lAttribute->SafeArraySubType.ToString()));
				if	(lAttribute->Value == UnmanagedType::SafeArray)
				{
					for	(lNdx = 0; lNdx < lFieldInfos->Length; lNdx++)
					{
						LogMessage (LogDebug, _T("    [%s] [%s]"), _B(lFieldInfos[lNdx]->ToString()), _B(lFieldValues[lNdx]->ToString()));
					}
				}
#endif
				lRet = gcnew CustomAttributeBuilder (lConstructor, lParameters, lFieldInfos->ToArray(), lFieldValues->ToArray());
			}
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

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

				if	(!Object::ReferenceEquals (lSourceProperty->DeclaringType, pSourceType))
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

				if	(!Object::ReferenceEquals (lSourceEvent->DeclaringType, pSourceType))
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
#pragma page()
/////////////////////////////////////////////////////////////////////////////

List<CustomAttributeBuilder^>^ CopyAssembly::CopyAttributes (Object^ pSource, Object^ pTarget, CustomAttrDataList^ pAttributes)
{
	List<CustomAttributeBuilder^>^	lRet = gcnew List<CustomAttributeBuilder^>;

	if	(
			(pAttributes)
		&&	(pAttributes->Count > 0)
		)
	{
		try
		{
			List<CustomAttributeBuilder^>^	lBuilders = gcnew List<CustomAttributeBuilder^>;
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
						(mFixups)
					&&	(mFixups->FixupCustomAttribute (pSource, pTarget, lAttribute, lValues))
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

			if	(lBuilders->Count > 0)
			{
				if	(mFixups)
				{
					mFixups->FixupCustomAttributes (pSource, pTarget, lBuilders);
				}
				if	(lBuilders->Count > 0)
				{
					lRet = lBuilders;
				}
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
		KeyValuePair <MethodBase^, ConstructorBuilder^>^	lCopiedConstructor;
		KeyValuePair <MethodBase^, MethodBuilder^>^			lCopiedMethod;
		CopyILBinary^										lCopier = gcnew CopyILBinary (mSourceModule, mModuleBuilder, this);
		MethodBody^											lMethodBody;

		for each (lCopiedConstructor in mCopiedConstructors)
		{
			if	(
					(!lCopiedConstructor->Key->IsAbstract)
				&&	(lCopiedConstructor->Key->GetMethodBody())
				&&	(lMethodBody = lCopier->CopyMethodBody (lCopiedConstructor->Key, lCopiedConstructor->Value->GetILGenerator()))
				)
			{
				lCopiedConstructor->Value->InitLocals = lMethodBody->InitLocals;
#ifdef	_DEBUG_TARGET_BODY
				LogMessage (_DEBUG_TARGET_BODY, _T("%s Body [%s] [%s] from [%s] {%s]"), _B(LogIndent()), _BM(lCopiedConstructor->Value), _BMT(lCopiedConstructor->Value), _BM(lCopiedConstructor->Key), _BMT(lCopiedConstructor->Key));
#endif
			}
		}

		for each (lCopiedMethod in mCopiedMethods)
		{
			if	(
					(!lCopiedMethod->Key->IsAbstract)
				&&	(lCopiedMethod->Key->GetMethodBody())
				&&	(lMethodBody = lCopier->CopyMethodBody (lCopiedMethod->Key, lCopiedMethod->Value->GetILGenerator()))
				)
			{
				lCopiedMethod->Value->InitLocals = lMethodBody->InitLocals;
#ifdef	_DEBUG_TARGET_BODY
				LogMessage (_DEBUG_TARGET_BODY, _T("%s Body [%s] [%s] from [%s] {%s]"), _B(LogIndent()), _BM(lCopiedMethod->Value), _BMT(lCopiedMethod->Value), _BM(lCopiedMethod->Key), _BMT(lCopiedMethod->Key));
#endif
			}
			else
			if	(!lCopiedMethod->Key->IsAbstract)
			{
#if	TRUE
				try
				{
					KeyValuePair <MethodBase^, MethodBuilder^>^	lFindMethod;
					Type^										lSourceOverride;
					Type^										lTargetOverride;

					if	(
							(lSourceOverride = IsMethodOverride (lCopiedMethod->Key))
						&&	(lTargetOverride = GetTargetType (lSourceOverride, false))
						)
					{
						TypeBuilder^	lTargetType = nullptr;
						MethodBuilder^	lTargetMethod = nullptr;

#ifdef	_DEBUG_TARGET_BODY
						LogMessage (_DEBUG_TARGET_BODY, _T("%s Method [%s] [%s] overrides [%s]"), _B(LogIndent()), _BM(lCopiedMethod->Value), _BMT(lCopiedMethod->Value), _BT(lTargetOverride));
#endif
						try
						{
							lTargetType = safe_cast <TypeBuilder^> (lCopiedMethod->Value->DeclaringType);
						}
						catch AnyExceptionSilent

						if	(lTargetType)
						{
							for each (lFindMethod in mCopiedMethods)
							{
								if	(
										(Object::ReferenceEquals (lFindMethod->Value->DeclaringType, lTargetOverride))
									&&	(String::Compare (lFindMethod->Key->Name, lCopiedMethod->Key->Name) == 0)
									)
								{
									lTargetMethod = lFindMethod->Value;
									break;
								}
							}
						}
						if	(
								(lTargetType)
							&&	(lTargetMethod)
							)
						{
#ifdef	_DEBUG_TARGET_BODY
							LogMessage (_DEBUG_TARGET_BODY, _T("%s Method [%s] [%s] overrides [%s] [%s] for [%s]"), _B(LogIndent()), _BM(lCopiedMethod->Value), _BMT(lCopiedMethod->Value), _BM(lTargetMethod), _BMT(lTargetMethod), _BT(lTargetType));
#endif
							try
							{
								lTargetType->DefineMethodOverride (lCopiedMethod->Value, lTargetMethod);
							}
							catch AnyExceptionDebug
						}
#ifdef	_DEBUG_TARGET_BODY
						else
						{
							LogMessage (_DEBUG_TARGET_BODY, _T("!!! [%s] [%s] override not found in [%s]"), _BM(lCopiedMethod->Value), _BMT(lCopiedMethod->Value), _BT(lTargetOverride));
						}
#endif
					}
				}
				catch AnyExceptionDebug
#endif
			}
		}
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CopyAssembly::TranslateType (Type^ pSourceType, Type^& pTargetType)
{
	pTargetType = GetTargetType (pSourceType, false);
	return !Object::ReferenceEquals (pTargetType, pSourceType);
}

bool CopyAssembly::TranslateMethod (MethodBase^ pSourceMethod, MethodInfo^& pTargetMethod)
{
	Type^			lSourceType = pSourceMethod->DeclaringType;
	Type^			lTargetType = nullptr;
	MethodInfo^		lTargetMethod = nullptr;

	if	(lSourceType->FullName->StartsWith ("System"))
	{
		lTargetMethod = safe_cast <MethodInfo^> (pSourceMethod);
	}
	else
	if	(
			(!TranslateType (lSourceType, lTargetType))
		||	(!mTranslateMethods->TryGetValue (pSourceMethod, lTargetMethod))
		||	(!lTargetMethod)
		)
	{
		try
		{
			lTargetMethod = lTargetType->GetMethod (pSourceMethod->Name, BindingFlags::Instance|BindingFlags::Static|BindingFlags::Public|BindingFlags::NonPublic, nullptr, GetParameterTypes (pSourceMethod), nullptr);
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

	if	(lSourceType->FullName->StartsWith ("System"))
	{
		lTargetConstructor = safe_cast <ConstructorInfo^> (pSourceConstructor);
	}
	else
	if	(
			(!TranslateType (lSourceType, lTargetType))
		||	(!mTranslateConstructors->TryGetValue (pSourceConstructor, lTargetConstructor))
		||	(!lTargetConstructor)
		)
	{
		try
		{
			lTargetConstructor = lTargetType->GetConstructor (GetParameterTypes (pSourceConstructor));
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
			(!TranslateType (lSourceType, lTargetType))
		||	(!mTranslateFields->TryGetValue (pSourceField, lTargetField))
		||	(!lTargetField)
		)
	{
		try
		{
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

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

Type^ CopyAssembly::GetTargetType (Type^ pSourceType, bool pCreate)
{
	Type^	lSourceType = pSourceType;
	Type^	lTargetType = nullptr;

	if	(
			(lSourceType->IsByRef)
		||	(lSourceType->IsPointer)
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
		if	(String::Compare (lTranslateType->Key->FullName, pSourceTypeName, true) == 0)
		{
			lRet = GetTargetType (lTranslateType->Key, pCreate);
			break;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

array<Type^>^ CopyAssembly::GetParameterTypes (MethodBase^ pMethod)
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
				lParameterTypes [lParameterNdx] = GetTargetType (lParameters [lParameterNdx]->ParameterType, false);
			}
		}
	}
	catch AnyExceptionDebug

	return lParameterTypes;
}

array<Type^>^ CopyAssembly::GetParameterTypes (ConstructorInfo^ pConstuctor)
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
				lParameterTypes [lParameterNdx] = GetTargetType (lParameters [lParameterNdx]->ParameterType, false);
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

	if	(lParameterTypes = GetParameterTypes (pMethod))
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

String^ CopyAssembly::LogIndent ()
{
	return gcnew String(' ', mLogIndent*2);
}

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
