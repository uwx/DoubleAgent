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
//#define	_DEBUG_TARGET_PARAMETER	LogNormal|LogHighVolume
//#define	_DEBUG_TARGET_PROPERTY	LogNormal|LogHighVolume
//#define	_DEBUG_TARGET_EVENT		LogNormal|LogHighVolume
//#define	_DEBUG_CREATE_TYPE		LogNormal
//#define	_DEBUG_RESOLVE_TYPE		LogNormal
#define	_DEBUG_TARGET_CODE		LogDebug
#endif

namespace DoubleAgent {
/////////////////////////////////////////////////////////////////////////////

Assembly^ CopyAssembly::DoCopy (Assembly^ pSourceAssembly, String^ pAssemblyName, String^ pModuleName, bool pSave)
{
	Assembly^	lTargetAssembly = nullptr;

	try
	{
		AssemblyName^	lAssemblyName = gcnew AssemblyName;
		String^			lFilePath = IO::Path::GetDirectoryName (pAssemblyName);
		String^			lFileName = IO::Path::GetFileName (pAssemblyName);

		lAssemblyName->Name = IO::Path::GetFileNameWithoutExtension (pAssemblyName);
		lAssemblyName->Version = safe_cast <System::Version^> (pSourceAssembly->GetName()->Version->Clone());
		lAssemblyName->ProcessorArchitecture = pSourceAssembly->GetName()->ProcessorArchitecture;

		mSourceAssembly = pSourceAssembly;
		mSourceModule = mSourceAssembly->GetModules() [0];
		mModuleName = pModuleName;

		mAssemblyBuilder = AppDomain::CurrentDomain->DefineDynamicAssembly (lAssemblyName, AssemblyBuilderAccess::Save, lFilePath);
		mModuleBuilder = mAssemblyBuilder->DefineDynamicModule (mModuleName, lFileName, true);

		CopyTypes ();
		CopyMethodBodies ();
		CreateTypes ();

		try
		{
			List<CustomAttributeBuilder^>^	lBuilders;
			CustomAttributeBuilder^			lBuilder;

			if	(lBuilders = CopyAttributes (mAssemblyBuilder, CustomAttributeData::GetCustomAttributes (pSourceAssembly)))
			{
				FixupCustomAttributes (pSourceAssembly, mAssemblyBuilder, lBuilders);
				for each (lBuilder in lBuilders)
				{
					mAssemblyBuilder->SetCustomAttribute (lBuilder);
				}
			}
		}
		catch AnyExceptionDebug

//TODO		mAssemblyBuilder->DefineVersionInfoResource
//TODO		mAssemblyBuilder->-- Define references / manifest
		if	(pSave)
		{
			LogMessage (LogNormal, _T("Save %s"), _B(pAssemblyName)); 
			mAssemblyBuilder->Save (lFileName);
		}
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
		for each (lSourceType in lSourceTypes)
		{
			mCopiedTypes->Add (lSourceType, nullptr);
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
		array<String^>^	lTypeName = pSourceType->FullName->Split ('.');
		String^			lTargetName = String::Format ("{0}.{1}", mModuleName, lTypeName [lTypeName->Length-1]);

		if	(pSourceType->IsEnum)
		{
#ifdef	_DEBUG_SOURCE_TYPE
			LogMessage (_DEBUG_SOURCE_TYPE, _T("%sSource Enum [%s]"), _B(LogIndent()), _B(pSourceType->FullName));
#endif
			lTargetType = CopyEnum (pSourceType, lTargetName, lTypeAttrs);
		}
		else
		if	(pSourceType->IsInterface)
		{
#ifdef	_DEBUG_SOURCE_TYPE
			LogMessage (_DEBUG_SOURCE_TYPE, _T("%sSource Interface [%s]"), _B(LogIndent()), _B(pSourceType->FullName));
#endif
			lTargetType = CopyType (pSourceType, lTargetName, lTypeAttrs);
		}
		else
		if	(pSourceType->IsClass)
		{
#ifdef	_DEBUG_SOURCE_TYPE
			LogMessage (_DEBUG_SOURCE_TYPE, _T("%sSource Class [%s]"), _B(LogIndent()), _B(pSourceType->FullName));
#endif
			lTargetType = CopyType (pSourceType, lTargetName, lTypeAttrs);
		}

		if	(lTargetType)
		{
			mCopiedTypes [pSourceType] = lTargetType;
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
	ResolveEventHandler^			lHandler = gcnew ResolveEventHandler (this, &CopyAssembly::ResolveType);

	mLogIndent = 0;
	AppDomain::CurrentDomain->TypeResolve::add (lHandler);

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

					if	(lBuilders = CopyAttributes (lTypeBuilder, CustomAttributeData::GetCustomAttributes (lCopiedType->Key)))
					{
						FixupCustomAttributes (lCopiedType->Key, lTypeBuilder, lBuilders);
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

					if	(lBuilders = CopyAttributes (lEnumBuilder, CustomAttributeData::GetCustomAttributes (lCopiedType->Key)))
					{
						FixupCustomAttributes (lCopiedType->Key, lEnumBuilder, lBuilders);
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

	AppDomain::CurrentDomain->TypeResolve::remove (lHandler);
}

Type^ CopyAssembly::CreateType (Type^ pSourceType, Type^ pTargetType)
{
	Type^	lTargetType = nullptr;

	if	(pTargetType == nullptr)
	{
#ifdef	_DEBUG_CREATE_TYPE
		LogMessage (_DEBUG_CREATE_TYPE, _T("%s  Skip   Type [%s]"), _B(LogIndent()), _B(pSourceType->FullName));
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
				LogMessage (_DEBUG_CREATE_TYPE, _T("%s  Create Type [%s]"), _B(LogIndent()), _B(lTypeBuilder->FullName));
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
				LogMessage (_DEBUG_CREATE_TYPE, _T("%s  Create Enum [%s]"), _B(LogIndent()), _B(lEnumBuilder->FullName));
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
#ifdef	_DEBUG_CREATE_TYPE
				LogMessage (_DEBUG_CREATE_TYPE, _T("%s    Created   [%s] for [%s]"), _B(LogIndent()), _B(lTargetType->FullName), _B(pSourceType->FullName));
#endif
			}
		}
		catch AnyExceptionDebug
	}

	return lTargetType;
}

Assembly^ CopyAssembly::ResolveType (Object^ pSender, ResolveEventArgs^ pEventArgs)
{
	Assembly^	lRet = nullptr;

	mLogIndent++;

	try
	{
		KeyValuePair <Type^, Type^>^	lCopiedType;
		Type^							lTargetType = nullptr;

#ifdef	_DEBUG_RESOLVE_TYPE
		LogMessage (_DEBUG_RESOLVE_TYPE, _T("%s  Resolve   [%s]"), _B(LogIndent()), _B(pEventArgs->Name));
#endif
		for each (lCopiedType in mCopiedTypes)
		{
			if	(
					(lCopiedType->Value)
				&&	(lCopiedType->Value->FullName == pEventArgs->Name)
				)
			{
#ifdef	_DEBUG_RESOLVE_TYPE
				LogMessage (_DEBUG_RESOLVE_TYPE, _T("%s  Resolving [%s] from [%s]"), _B(LogIndent()), _B(lCopiedType->Value->FullName), _B(lCopiedType->Key->FullName));
#endif
				lTargetType = CreateType (lCopiedType->Key, lCopiedType->Value);

				if	(lTargetType)
				{
#ifdef	_DEBUG_RESOLVE_TYPE
					LogMessage (_DEBUG_RESOLVE_TYPE, _T("%s  Resolved [%s]"), _B(LogIndent()), _B(lTargetType->Name));
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

		if	(!FixupType (pSourceType, pTargetAttrs))
		{
#ifdef	_DEBUG_TARGET_TYPE
			LogMessage (_DEBUG_TARGET_TYPE, _T("%sCopy [%s] [%s] to [%s] [%s]"), _B(LogIndent()), _B(pSourceType->FullName), _B(TypeProps(pSourceType)), _B(pTargetName), _B(TypeAttrsStr(pTargetAttrs)));
#endif
			lTypeBuilder = mModuleBuilder->DefineType (pTargetName, pTargetAttrs);

			mCopiedTypes [pSourceType] = lTypeBuilder;

			if	(pSourceType->BaseType)
			{
				if	(String::Compare (pSourceType->BaseType->FullName, "System.__ComObject", true) == 0)
				{
#ifdef	_DEBUG_TARGET_TYPE
					LogMessage (_DEBUG_TARGET_TYPE, _T("%s  Skip Base [%s]"), _B(LogIndent()), _B(pSourceType->BaseType->FullName));
#endif
				}
				else
				{
					Type^	lTargetParent = GetTargetType (pSourceType->BaseType, false);
#ifdef	_DEBUG_TARGET_TYPE
					LogMessage (_DEBUG_TARGET_TYPE, _T("%s  Copy Base [%s] as [%s]"), _B(LogIndent()), _B(pSourceType->BaseType->FullName), _B(lTargetParent->FullName));
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
					LogMessage (_DEBUG_TARGET_INTERFACE, _T("%s    Skip Inherited [%s]"), _B(LogIndent()), _B(lSourceInterface->FullName));
#endif
				}
				else
				{
					lTargetInterface = GetTargetType (lSourceInterface, false);
#ifdef	_DEBUG_TARGET_INTERFACE
					LogMessage (_DEBUG_TARGET_INTERFACE, _T("%s    Interface [%s] as [%s]"), _B(LogIndent()), _B(lSourceInterface->FullName), _B(lTargetInterface->FullName));
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
				FieldAttributes	lFieldAttributes = lSourceField->Attributes;
				Type^			lFieldType;
				FieldBuilder^	lFieldBuilder;

				if	(!Object::ReferenceEquals (lSourceField->DeclaringType, pSourceType))
				{
#ifdef	_DEBUG_TARGET_FIELD
					LogMessage (_DEBUG_TARGET_FIELD, _T("%s    Field [%s] from [%s] skipped for [%s]"), _B(LogIndent()), _B(lSourceField->Name), _B(lSourceField->DeclaringType->FullName), _B(pSourceType->FullName));
#endif
					continue;
				}

				lFieldType = GetTargetType (lSourceField->FieldType, false);
#ifdef	_DEBUG_TARGET_FIELD
				LogMessage (_DEBUG_TARGET_FIELD, _T("%s    Field [%s] [%s] as [%s]"), _B(LogIndent()), _B(lSourceField->Name), _B(lSourceField->FieldType->FullName), _B(lFieldType->FullName));
#endif
				lFieldBuilder = pTypeBuilder->DefineField (lSourceField->Name, lFieldType, lFieldAttributes);
				mCopiedFields [lSourceField] = lFieldBuilder;
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
				if	(lParameterTypes)
				{
					for	(lParameterNdx = 0; lParameterNdx < lParameters->Length; lParameterNdx++)
					{
						LogMessage (_DEBUG_TARGET_PARAMETER, _T("%s        Parameter [%s] [%s] as [%s]"), _B(LogIndent()), _B(lParameters [lParameterNdx]->Name), _B(lParameters [lParameterNdx]->ParameterType->FullName), _B(lParameterTypes [lParameterNdx]->FullName));
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
						ParameterBuilder^	lParameterBuilder;

						lParameterBuilder = lConstructorBuilder->DefineParameter (lParameterNdx+1, lParameters [lParameterNdx]->Attributes, lParameters [lParameterNdx]->Name);
//TODO - default values
					}
				}

				lConstructorBuilder->SetImplementationFlags (lSourceConstructor->GetMethodImplementationFlags ());
				mCopiedConstructors [lSourceConstructor] = lConstructorBuilder;
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
		array<MethodInfo^>^	lSourceMethods;
		MethodInfo^			lSourceMethod;

		if	(
				(lSourceMethods = pSourceType->GetMethods (BindingFlags::Instance|BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::NonPublic))
			&&	(lSourceMethods->Length > 0)
			)
		{
#ifdef	_DEBUG_TARGET_METHOD
			LogMessage (_DEBUG_TARGET_METHOD, _T("%s  Copy [%d] Methods"), _B(LogIndent()), lSourceMethods->Length);
#endif
			for each (lSourceMethod in lSourceMethods)
			{
				MethodBuilder^		lMethodBuilder;
				MethodAttributes	lMethodAttrs = lSourceMethod->Attributes;

				if	(!Object::ReferenceEquals (lSourceMethod->DeclaringType, pSourceType))
				{
#ifdef	_DEBUG_TARGET_METHOD
					LogMessage (_DEBUG_TARGET_METHOD, _T("%s    Method [%s] from [%s] skipped for [%s]"), _B(LogIndent()), _B(lSourceMethod->Name), _B(lSourceMethod->DeclaringType->FullName), _B(pSourceType->FullName));
#endif
					continue;
				}
				if	(FixupMethod (lSourceMethod, lMethodAttrs))
				{
					continue;
				}

#ifdef	_DEBUG_TARGET_METHOD
				LogMessage (_DEBUG_TARGET_METHOD, _T("%s    Method [%s] [%s] [%s] [%s]"), _B(LogIndent()), _B(lSourceMethod->Name), _B(MethodAttrsStr(lMethodAttrs)), _B(MethodCallType(lSourceMethod->CallingConvention)), _B(MethodImplementation(lSourceMethod->GetMethodImplementationFlags())));
#endif
				lMethodBuilder = pTypeBuilder->DefineMethod (lSourceMethod->Name, lMethodAttrs, lSourceMethod->CallingConvention);

				if	(
						(lSourceMethod->ReturnType)
					&&	(String::Compare (lSourceMethod->ReturnType->FullName, gcnew String("System.Void"), true) != 0)
					)
				{
#ifdef	_DEBUG_TARGET_PARAMETER
					LogMessage (_DEBUG_TARGET_PARAMETER, _T("%s      Copy ReturnType [%s] as [%s]"), _B(LogIndent()), _B(lSourceMethod->ReturnType->FullName), _B(GetTargetType (lSourceMethod->ReturnType)->FullName));
#endif
					lMethodBuilder->SetReturnType (GetTargetType (lSourceMethod->ReturnType, false));
				}

				try
				{
					array<ParameterInfo^>^	lParameters = lSourceMethod->GetParameters ();

					if	(
							(lParameters)
						&&	(lParameters->Length > 0)
						)
					{
						array<Type^>^		lParameterTypes = GetParameterTypes (lSourceMethod);
						ParameterBuilder^	lParameterBuilder;
						int					lParameterNdx;

#ifdef	_DEBUG_TARGET_PARAMETER
						LogMessage (_DEBUG_TARGET_PARAMETER, _T("%s      Copy [%d] Parameters"), _B(LogIndent()), lParameters->Length);
#endif
						lMethodBuilder->SetParameters (lParameterTypes);

						for	(lParameterNdx = 0; lParameterNdx < lParameters->Length; lParameterNdx++)
						{
							lParameterBuilder = lMethodBuilder->DefineParameter (lParameterNdx+1, lParameters [lParameterNdx]->Attributes, lParameters [lParameterNdx]->Name);
//TODO - default values
						}
					}
				}
				catch AnyExceptionDebug

				lMethodBuilder->SetImplementationFlags (lSourceMethod->GetMethodImplementationFlags ());

				try
				{
					List<CustomAttributeBuilder^>^	lBuilders;
					CustomAttributeBuilder^			lBuilder;

					if	(lBuilders = CopyAttributes (lMethodBuilder, CustomAttributeData::GetCustomAttributes (lSourceMethod)))
					{
						FixupCustomAttributes (lSourceMethod, lMethodBuilder, lBuilders);
 						for each (lBuilder in lBuilders)
						{
							lMethodBuilder->SetCustomAttribute (lBuilder);
						}
					}
				}
				catch AnyExceptionDebug

				lRet->Add (lMethodBuilder->Name, lMethodBuilder);
				mCopiedMethods [lSourceMethod] = lMethodBuilder;
			}
		}
	}
	catch AnyExceptionDebug
	{}

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
					LogMessage (_DEBUG_TARGET_PROPERTY, _T("%s    Property [%s] from [%s] skipped for [%s]"), _B(LogIndent()), _B(lSourceProperty->Name), _B(lSourceProperty->DeclaringType->FullName), _B(pSourceType->FullName));
#endif
					continue;
				}
				if	(FixupProperty (lSourceProperty, lPropertyAttributes))
				{
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
#ifdef	_DEBUG_TARGET_PROPERTY
				LogMessage (_DEBUG_TARGET_PROPERTY, _T("%s    Property [%s] [%s] [%s] as [%s]"), _B(LogIndent()), _B(lSourceProperty->Name), _B(lSourceProperty->PropertyType->FullName), _B(PropertyAttrsStr(lPropertyAttributes)), _B(lPropertyType->FullName));
#endif
				lPropertyBuilder = pTypeBuilder->DefineProperty (lSourceProperty->Name, lPropertyAttributes, lPropertyType, lParameterTypes);

#ifdef	_DEBUG_TARGET_PARAMETER
				for	(lParameterNdx = 0; lParameterNdx < lParameterTypes->Length; lParameterNdx++)
				{
					LogMessage (_DEBUG_TARGET_PARAMETER, _T("%s      Parameter {%s]"), _B(LogIndent()), _B(lParameterTypes [lParameterNdx]->FullName));
				}
#endif

				if	(
						(lGetMethod = lSourceProperty->GetGetMethod ())
					&&	(pDefinedMethods->TryGetValue (lGetMethod->Name, lMethodBuilder))
					)
				{
#ifdef	_DEBUG_TARGET_PROPERTY
					LogMessage (_DEBUG_TARGET_PROPERTY, _T("%s      Get [%s]"), _B(LogIndent()), _B(lMethodBuilder->Name));
#endif
					lPropertyBuilder->SetGetMethod (lMethodBuilder);
				}

				if	(
						(lSetMethod = lSourceProperty->GetSetMethod ())
					&&	(pDefinedMethods->TryGetValue (lSetMethod->Name, lMethodBuilder))
					)
				{
#ifdef	_DEBUG_TARGET_PROPERTY
					LogMessage (_DEBUG_TARGET_PROPERTY, _T("%s      Set [%s]"), _B(LogIndent()), _B(lMethodBuilder->Name));
#endif
					lPropertyBuilder->SetSetMethod (lMethodBuilder);
				}

				try
				{
					List<CustomAttributeBuilder^>^	lBuilders;
					CustomAttributeBuilder^			lBuilder;

					if	(lBuilders = CopyAttributes (lPropertyBuilder, CustomAttributeData::GetCustomAttributes (lSourceProperty)))
					{
						FixupCustomAttributes (lSourceProperty, lPropertyBuilder, lBuilders);
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
				Type^						lEventType;
				EventBuilder^				lEventBuilder;
				MethodInfo^					lAddMethod = nullptr;
				MethodInfo^					lRemoveMethod = nullptr;
				MethodInfo^					lRaiseMethod = nullptr;
				MethodBuilder^				lMethodBuilder;

				if	(!Object::ReferenceEquals (lSourceEvent->DeclaringType, pSourceType))
				{
#ifdef	_DEBUG_TARGET_EVENT
					LogMessage (_DEBUG_TARGET_EVENT, _T("%s    Event [%s] from [%s] skipped for [%s]"), _B(LogIndent()), _B(lSourceEvent->Name), _B(lSourceEvent->DeclaringType->FullName), _B(pSourceType->FullName));
#endif
					continue;
				}
				if	(FixupEvent (lSourceEvent, lEventAttributes))
				{
					continue;
				}

				lEventType = GetTargetType (lSourceEvent->EventHandlerType, false);
#ifdef	_DEBUG_TARGET_EVENT
				LogMessage (_DEBUG_TARGET_EVENT, _T("%s    Event [%s] [%s] [%s] as [%s]"), _B(LogIndent()), _B(lSourceEvent->Name), _B(EventAttrsStr(lEventAttributes)), _B(lSourceEvent->EventHandlerType->Name), _B(lEventType->Name));
#endif
				lEventBuilder = pTypeBuilder->DefineEvent (lSourceEvent->Name, lEventAttributes, lEventType);

				if	(
						(lAddMethod = lSourceEvent->GetAddMethod ())
					&&	(pDefinedMethods->TryGetValue (lAddMethod->Name, lMethodBuilder))
					)
				{
#ifdef	_DEBUG_TARGET_EVENT
					LogMessage (_DEBUG_TARGET_EVENT, _T("%s      Add [%s]"), _B(LogIndent()), _B(lMethodBuilder->Name));
#endif
					lEventBuilder->SetAddOnMethod (lMethodBuilder);
				}

				if	(
						(lRemoveMethod = lSourceEvent->GetRemoveMethod ())
					&&	(pDefinedMethods->TryGetValue (lRemoveMethod->Name, lMethodBuilder))
					)
				{
#ifdef	_DEBUG_TARGET_EVENT
					LogMessage (_DEBUG_TARGET_EVENT, _T("%s      Remove [%s]"), _B(LogIndent()), _B(lMethodBuilder->Name));
#endif
					lEventBuilder->SetRemoveOnMethod (lMethodBuilder);
				}

				if	(
						(lRaiseMethod = lSourceEvent->GetRaiseMethod ())
					&&	(pDefinedMethods->TryGetValue (lRaiseMethod->Name, lMethodBuilder))
					)
				{
#ifdef	_DEBUG_TARGET_EVENT
					LogMessage (_DEBUG_TARGET_EVENT, _T("%s      Raise [%s]"), _B(LogIndent()), _B(lMethodBuilder->Name));
#endif
					lEventBuilder->SetRaiseMethod (lMethodBuilder);
				}

				try
				{
					List<CustomAttributeBuilder^>^	lBuilders;
					CustomAttributeBuilder^			lBuilder;

					if	(lBuilders = CopyAttributes (lEventBuilder, CustomAttributeData::GetCustomAttributes (lSourceEvent)))
					{
						FixupCustomAttributes (lSourceEvent, lEventBuilder, lBuilders);
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

List<CustomAttributeBuilder^>^ CopyAssembly::CopyAttributes (Object^ pTarget, CustomAttrDataList^ pAttributes)
{
	List<CustomAttributeBuilder^>^	lRet = nullptr;

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
				array<Object^>^	lValues;
				int				lValueNdx;

				lAttribute = pAttributes [lAttributeNdx];

				if	(
						(lAttribute->NamedArguments)
					&&	(lAttribute->NamedArguments->Count > 0)
					)
				{
					lValues = gcnew array<Object^> (lAttribute->NamedArguments->Count);
					for	(lValueNdx = 0; lValueNdx < lAttribute->NamedArguments->Count; lValueNdx++)
					{
						lValues [lValueNdx] = lAttribute->NamedArguments [lValueNdx].TypedValue;
					}
				}
				else
				{
					lValues = gcnew array<Object^> (lAttribute->ConstructorArguments->Count);
					for	(lValueNdx = 0; lValueNdx < lAttribute->ConstructorArguments->Count; lValueNdx++)
					{
						lValues [lValueNdx] = lAttribute->ConstructorArguments [lValueNdx].Value;
					}
				}

				if	(FixupCustomAttribute (pTarget, lAttribute, lValues))
				{
					continue;
				}

				lBuilders->Add (gcnew CustomAttributeBuilder (lAttribute->Constructor, lValues));
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
		array<FieldInfo^>^	lSourceFields;
		FieldInfo^			lSourceField;
		FieldBuilder^		lFieldBuilder;

		if	(!FixupEnum (pSourceType, pTargetAttrs))
		{
#ifdef	_DEBUG_TARGET_TYPE
			LogMessage (_DEBUG_TARGET_TYPE, _T("%sCopy [%s] to [%s] (%s)"), _B(LogIndent()), _B(pSourceType->FullName), _B(pTargetName), _B(TypeAttrsStr(pTargetAttrs)));
#endif
			lEnumBuilder = mModuleBuilder->DefineEnum (pTargetName, pTargetAttrs, Enum::GetUnderlyingType(pSourceType));

			if	(
					(lSourceFields = pSourceType->GetFields ())
				&&	(lSourceFields->Length > 0)
				)
			{
				for each (lSourceField in lSourceFields)
				{
					Object^	lValue = nullptr;
					try
					{
						lValue = lSourceField->GetRawConstantValue();
					}
					catch AnyExceptionSilent
					{}

					if	(lValue)
					{
#ifdef	_DEBUG_TARGET_ENUM
						LogMessage (_DEBUG_TARGET_ENUM, _T("%s  Field [%s] (%s)"), _B(LogIndent()), _B(lSourceField->Name), _B(lValue->ToString()));
#endif
						lFieldBuilder = lEnumBuilder->DefineLiteral (lSourceField->Name, lValue);
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

bool CopyAssembly::FixupEnum (Type^ pSourceType, TypeAttributes & pTypeAttributes)
{
	pTypeAttributes = (TypeAttributes) ((int)pTypeAttributes & (int)TypeAttributes::VisibilityMask);
	return false;
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
				)
			{
				lMethodBody = lCopier->CopyMethodBody (lCopiedConstructor->Key, lCopiedConstructor->Value->GetILGenerator());
				if	(lMethodBody)
				{
					lCopiedConstructor->Value->InitLocals = lMethodBody->InitLocals;
				}
			}
		}

		for each (lCopiedMethod in mCopiedMethods)
		{
			if	(
					(!lCopiedMethod->Key->IsAbstract)
				&&	(lCopiedMethod->Key->GetMethodBody())
				)
			{
				lMethodBody = lCopier->CopyMethodBody (lCopiedMethod->Key, lCopiedMethod->Value->GetILGenerator());
				if	(lMethodBody)
				{
					lCopiedMethod->Value->InitLocals = lMethodBody->InitLocals;
				}
			}
		}
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////

bool CopyAssembly::TranslateType (Type^ pSourceType, Type^& pTargetType)
{
	pTargetType = GetTargetType (pSourceType, false);
	return !Object::ReferenceEquals (pTargetType, pSourceType);
}

bool CopyAssembly::TranslateMethod (MethodBase^ pSourceMethod, MethodInfo^& pTargetMethod)
{
	Type^				lSourceType = pSourceMethod->DeclaringType;
	Type^				lTargetType = nullptr;
	MethodInfo^			lTargetMethod = nullptr;
	MethodBuilder^		lCopiedMethod;

	if	(
			(TranslateType (lSourceType, lTargetType))
		&&	(mCopiedMethods->TryGetValue (pSourceMethod, lCopiedMethod))
		)
	{
		lTargetMethod = lCopiedMethod;
	}
	else
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
	LogMessage (_DEBUG_TARGET_CODE, _T("!!! Method [%s.%s] to [%s.%s]"), _B(lSourceType->FullName), _B(pSourceMethod->Name), _B(lTargetType->FullName), _B(pTargetMethod->Name));
#endif
	return false;
}

bool CopyAssembly::TranslateConstructor (MethodBase^ pSourceConstructor, ConstructorInfo^& pTargetConstructor)
{
	Type^				lSourceType = pSourceConstructor->DeclaringType;
	Type^				lTargetType = nullptr;
	ConstructorInfo^	lTargetConstructor = nullptr;
	ConstructorBuilder^	lCopiedConstructor;

	if	(
			(TranslateType (lSourceType, lTargetType))
		&&	(mCopiedConstructors->TryGetValue (pSourceConstructor, lCopiedConstructor))
		)
	{
		lTargetConstructor = lCopiedConstructor;
	}
	else
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
	LogMessage (_DEBUG_TARGET_CODE, _T("!!! Method [%s.%s] to [%s.%s]"), _B(lSourceType->FullName), _B(pSourceConstructor->Name), _B(lTargetType->FullName), _B(pTargetConstructor->Name));
#endif
	return false;
}

bool CopyAssembly::TranslateField (FieldInfo^ pSourceField, FieldInfo^& pTargetField)
{
	Type^			lSourceType = pSourceField->DeclaringType;
	Type^			lTargetType = nullptr;
	FieldInfo^		lTargetField = nullptr;
	FieldBuilder^	lCopiedField = nullptr;

	if	(
			(TranslateType (lSourceType, lTargetType))
		&&	(mCopiedFields->TryGetValue (pSourceField, lCopiedField))
		)
	{
		lTargetField = lCopiedField;
	}
	else
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
	LogMessage (LogDebug, _T("!!! Field [%s.%s] to [%s.%s]"), _B(lSourceType->FullName), _B(pSourceField->Name), _B(lTargetType->FullName), _B(pSourceField->Name));
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
	KeyValuePair<Type^, Type^>^	lPair;

	for each (lPair in mCopiedTypes)
	{
		if	(String::Compare (lPair->Key->FullName, pSourceTypeName, true) == 0)
		{
			lRet = GetTargetType (lPair->Key, pCreate);
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

String^ CopyAssembly::LogIndent ()
{
	return gcnew String(' ', mLogIndent*2);
}

/////////////////////////////////////////////////////////////////////////////
};