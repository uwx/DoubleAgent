#include "stdafx.h"
#include <corhdr.h>
#include "FixupAssembly.h"

#define	_LOG_FIXES	LogNormal

namespace DoubleAgent {
namespace TlbToAsm {
/////////////////////////////////////////////////////////////////////////////

bool FixupAssembly::FixupType (Type^ pSourceType, String^& pTypeName, TypeAttributes & pTypeAttributes)
{
	bool	lRet = CopyAssembly::FixupType (pSourceType, pTypeName, pTypeAttributes);

//
//	Rename types to conform to Namespace naming conventions
//
	if	(true)
	{
		array <String^>^		lTypeNames = pTypeName->Split ('.');
		interior_ptr <String^>	lTypeName = &lTypeNames [lTypeNames->Length-1];
		bool					lRenamed = false;

		if	((*lTypeName)->StartsWith ("DaSvr"))
		{
			(*lTypeName) = (*lTypeName)->Substring (5);
			lRenamed = true;
		}
		else
		if	((*lTypeName)->StartsWith ("DaCtl"))
		{
			(*lTypeName) = (*lTypeName)->Substring (5);
			lRenamed = true;
		}
		else
		if	((*lTypeName)->StartsWith ("Da"))
		{
			(*lTypeName) = (*lTypeName)->Substring (2);
			lRenamed = true;
		}

		if	(lRenamed)
		{
			if	(pSourceType->IsInterface)
			{
				(*lTypeName) = gcnew String ("I") + (*lTypeName);
			}
			else
			if	(
					(pSourceType->IsClass)
				&&	(!pSourceType->IsAbstract)
				&&	((*lTypeName)->EndsWith ("Class"))
				)
			{
				(*lTypeName) = (*lTypeName)->Substring (0, (*lTypeName)->Length-5);
			}
		}
		else
		if	(
				(
					(pSourceType->IsClass)
				||	(!pSourceType->IsImport)
				)
			&&	((*lTypeName)->StartsWith ("_DaSvrEvents_"))
			)
		{
			if	(
					(pSourceType->IsInterface)
				&&	((*lTypeName)->EndsWith ("_Event"))
				)
			{
				(*lTypeName) = "IEvents";
			}
			else
			if	(
					(pSourceType->IsClass)
				&&	((*lTypeName)->EndsWith ("EventHandler"))
				)
			{
				(*lTypeName) = (*lTypeName)->Replace ("EventHandler", "Handler");
				lRenamed = true;
			}

			if	(pSourceType->IsInterface)
			{
				(*lTypeName) = (*lTypeName)->Replace ("_DaSvrEvents_", "IEvents_");
			}
			else
			{
				(*lTypeName) = (*lTypeName)->Replace ("_DaSvrEvents_", "Event_");
			}
			lRenamed = true;
		}
		else
		if	(
				(
					(pSourceType->IsClass)
				||	(!pSourceType->IsImport)
				)
			&&	((*lTypeName)->StartsWith ("_DaCtlEvents_"))
			)
		{
			if	(
					(pSourceType->IsInterface)
				&&	((*lTypeName)->EndsWith ("_Event"))
				)
			{
				(*lTypeName) = "IEvents";
			}
			else
			if	(
					(pSourceType->IsClass)
				&&	((*lTypeName)->EndsWith ("EventHandler"))
				)
			{
				(*lTypeName) = (*lTypeName)->Replace ("EventHandler", "Handler");
				lRenamed = true;
			}

			if	(pSourceType->IsInterface)
			{
				(*lTypeName) = (*lTypeName)->Replace ("_DaCtlEvents_", "IEvents_");
			}
			else
			{
				(*lTypeName) = (*lTypeName)->Replace ("_DaCtlEvents_", "Event_");
			}
			lRenamed = true;
		}
		else
		if	(
				(pSourceType->IsInterface)
			&&	(pSourceType->IsImport)
			)
		{
			if	(
					((*lTypeName)->StartsWith ("IDaSvr"))
				||	((*lTypeName)->StartsWith ("_DaSvr"))
				||	((*lTypeName)->StartsWith ("IDaCtl"))
				||	((*lTypeName)->StartsWith ("_DaCtl"))
				||	((*lTypeName)->StartsWith ("IDa"))
				)
			{
				(*lTypeName) = gcnew String ("Native.") + (*lTypeName);
				lRenamed = true;
			}
		}

		if	(lRenamed)
		{
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("--> Rename [%s] to [%s]"), _B(pTypeName), _B(String::Join(".", lTypeNames)));
#endif
			pTypeName = String::Join(".", lTypeNames);
		}
	}

#if	FALSE
//
//	Skip NonCreatable classes?
//
	if	(
			(pSourceType->IsClass)
		&&	(pSourceType->IsCOMObject)
		&&	(((int)GetTypeLibTypeFlags (pSourceType) & (int)TypeLibTypeFlags::FCanCreate) == 0)
		)
	{
#ifdef	_LOG_FIXES
		LogMessage (_LOG_FIXES, _T("--> Skip   [%s] NonCreatable Class"), _BT(pSourceType));
#endif
		lRet = true;
	}
#endif

#if	FALSE
//
//	Hide NonCreatable classes?
//
	if	(
			(pSourceType->IsClass)
		&&	(pSourceType->IsCOMObject)
		&&	(((int)GetTypeLibTypeFlags (pSourceType) & (int)TypeLibTypeFlags::FCanCreate) == 0)
		)
	{
		pTypeAttributes = (TypeAttributes) ((int)pTypeAttributes & ~(int)TypeAttributes::VisibilityMask | (int)TypeAttributes::NotPublic);
#ifdef	_LOG_FIXES
		LogMessage (_LOG_FIXES, _T("--> Class  [%s] from [%s] to [%s]"), _BT(pSourceType), _B(TypeAttrsStr(pSourceType->Attributes)), _B(TypeAttrsStr(pTypeAttributes)));
#endif
	}
#endif

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool FixupAssembly::FixupMethod (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes)
{
	bool				lRet = CopyAssembly::FixupMethod (pSourceMethod, pMethodAttributes);
	TypeLibFuncFlags	lTypeLibFuncFlags = GetTypeLibFuncFlags (pSourceMethod);

//
//	Hide NonBrowsable methods (mostly deprecated methods) or make them Protected (applies to _NewEnum)
//
	if	(
			((int)lTypeLibFuncFlags & (int)TypeLibFuncFlags::FRestricted)
		&&	((int)lTypeLibFuncFlags & (int)TypeLibFuncFlags::FNonBrowsable)
		)
	{
		if	((int)lTypeLibFuncFlags & (int)TypeLibFuncFlags::FHidden)
		{
			pMethodAttributes = (MethodAttributes) ((int)pMethodAttributes & ~(int)MethodAttributes::MemberAccessMask | (int)MethodAttributes::Private);
		}
		else
		{
			pMethodAttributes = (MethodAttributes) ((int)pMethodAttributes & ~(int)MethodAttributes::MemberAccessMask | (int)MethodAttributes::Family);
		}
#ifdef	_LOG_FIXES
		LogMessage (_LOG_FIXES, _T("--> Method [%s] in [%s] from [%s] to [%s]"), _BM(pSourceMethod), _BMT(pSourceMethod), _B(MethodAttrsStr(pSourceMethod->Attributes)), _B(MethodAttrsStr(pMethodAttributes)));
#endif
	}

#if	TRUE
//
//	Seal accessor method overrides on classes
//
	if	(
			(!lRet)
		&&	(pSourceMethod->IsSpecialName)
		&&	(pSourceMethod->DeclaringType->IsClass)
		&&	(pSourceMethod->DeclaringType->IsCOMObject)
		)
	{
		pMethodAttributes = (MethodAttributes) ((int)pMethodAttributes | (int)MethodAttributes::Final);
#ifdef	_LOG_FIXES
		LogMessage (_LOG_FIXES, _T("--> Method [%s] in [%s] from [%s] to [%s]"), _BM(pSourceMethod), _BMT(pSourceMethod), _B(MethodAttrsStr(pSourceMethod->Attributes)), _B(MethodAttrsStr(pMethodAttributes)));
#endif
	}
#endif

#if	FALSE
//
//	Protect hidden methods?
//
	if	(
			(!lRet)
		&&	((int)lTypeLibFuncFlags & (int)TypeLibFuncFlags::FHidden)
		)
	{
		pMethodAttributes = (MethodAttributes) ((int)pMethodAttributes & ~(int)MethodAttributes::MemberAccessMask | (int)MethodAttributes::Assembly);
	}
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool FixupAssembly::FixupReturnType (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, Type^& pReturnType)
{
	bool	lRet = CopyAssembly::FixupReturnType (pSourceMethod, pTargetMethod, pReturnType);

#if	TRUE
//
//	Change type reference from interface to class
//
	if	(!Object::ReferenceEquals (pReturnType, pSourceMethod->ReturnType))
	{
		try
		{
			Type^	lClassType = GetTargetType (pSourceMethod->ReturnType->FullName + "Class", false);

			if	(lClassType)
			{
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("--> Return [%s] as [%s] for [%s] in [%s]"), _BT(pReturnType), _BT(lClassType), _BM(pSourceMethod), _BMT(pSourceMethod));
#endif
				pReturnType = lClassType;
				lRet = true;
			}
		}
		catch AnyExceptionSilent
	}
#endif	
	return lRet;
}

bool FixupAssembly::FixupParameter (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, ParameterInfo^ pSourceParameter, Type^& pParameterType)
{
	bool	lRet = CopyAssembly::FixupParameter (pSourceMethod, pTargetMethod, pSourceParameter, pParameterType);

#if	TRUE
//
//	Change type reference from interface to class
//
	if	(!Object::ReferenceEquals (pParameterType, pSourceParameter->ParameterType))
	{
		try
		{
			Type^	lClassType = GetTargetType (pSourceParameter->ParameterType->FullName + "Class", false);

			if	(lClassType)
			{
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("--> Param  [%s] as [%s] for [%s] in [%s] [%s]"), _BT(pParameterType), _BT(lClassType), _BP(pSourceParameter), _BM(pSourceMethod), _BMT(pSourceMethod));
#endif
				pParameterType = lClassType;
				lRet = true;
			}
		}
		catch AnyExceptionSilent
	}
#endif	
	return lRet;
}

bool FixupAssembly::FixupProperty (PropertyInfo^ pSourceProperty, Type^& pPropertyType)
{
	bool	lRet = CopyAssembly::FixupProperty (pSourceProperty, pPropertyType);
	
#if	TRUE	
//
//	Change type reference from interface to class
//
	if	(!Object::ReferenceEquals (pPropertyType, pSourceProperty->PropertyType))
	{
		try
		{
			Type^	lClassType = GetTargetType (pSourceProperty->PropertyType->FullName + "Class", false);

			if	(lClassType)
			{
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("--> Define [%s] as [%s] for [%s] in [%s]"), _BT(pPropertyType), _BT(lClassType), _BP(pSourceProperty), _BPT(pSourceProperty));
#endif
				pPropertyType = lClassType;
				lRet = true;
			}
		}
		catch AnyExceptionSilent
	}
#endif	
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool FixupAssembly::FixupField (FieldInfo^ pSourceField, String^& pFieldName, EnumBuilder^ pEnumBuilder)
{
	bool	lRet = CopyAssembly::FixupField (pSourceField, pFieldName, pEnumBuilder);
	String^	lNamePrefix;

	try
	{	
		lNamePrefix = pFieldName->Substring (0, pFieldName->IndexOf ('_'));
	}
	catch AnyExceptionSilent
	
	if	(
			(!String::IsNullOrEmpty (lNamePrefix))
		&&	(
				(String::Compare (pEnumBuilder->Name, lNamePrefix, true) == 0)
			||	(pEnumBuilder->Name->StartsWith (lNamePrefix))
			)
		)
	{
		pFieldName = pFieldName->Substring (lNamePrefix->Length+1);
#ifdef	_LOG_FIXES
		LogMessage (_LOG_FIXES, _T("--> Field  [%s] as [%s] in [%s]"), _B(pSourceField->Name), _B(pFieldName), _B(pEnumBuilder->FullName));
#endif
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool FixupAssembly::FixupCustomAttribute (Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues)
{
	bool	lRet = CopyAssembly::FixupCustomAttribute (pTarget, pAttribute, pAttributeValues);

	try
	{
		TypeBuilder^	lTypeBuilder;
		String^			lAttributeString;

		try
		{
			lTypeBuilder = safe_cast <TypeBuilder^> (pTarget);
			lAttributeString = pAttribute->ToString();
		}
		catch AnyExceptionSilent

//
//	Remove hidden attribute for class/interface wrappers (inherited from the native interface during import)
//
		if	(
				(!lRet)
			&&	(lTypeBuilder)
			&&	(!lTypeBuilder->IsImport)
			&&	(lAttributeString->IndexOf (INTEROP_TYPELIBTYPE_TYPE) >= 0)
			&&	(pAttributeValues->Length == 1)
			)
		{
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("--> Unhide [%s] (remove [%s])"), _BT(lTypeBuilder), _B(lAttributeString));
#endif
			lRet = true;
		}

#if	FALSE
		if	(
				(!lRet)
			&&	(lTypeBuilder)
			&&	(!lTypeBuilder->IsClass)
			&&	(lAttributeString->IndexOf (INTEROP_COCLASS_TYPE) >= 0)
			&&	(pAttributeValues->Length == 1)
			&&	(String::Compare (pAttributeValues[0]->GetType()->ToString(), "System.RuntimeType", true) == 0)
			)
		{
//
//	Just remove this attribute to keep interfaces from showing up as classes in the object browser.
//	We want them to show up as interfaces, and the CoClassAttribute causes interfaces to show up as classes in VB.
//
			Type^	lValueType;

			if	(
					(lValueType = GetTargetType (pAttributeValues[0]->ToString(), false))
				&&	(lValueType->IsClass)
				&&	(lValueType->IsCOMObject)
				&&	(((int)GetTypeLibTypeFlags (lValueType) & (int)TypeLibTypeFlags::FCanCreate) == 0)
				&&	(false)
				)
			{
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("--> Skip   [%s] in [%s] for [%s]"), _B(lAttributeString), _BT(lTypeBuilder), _B(pAttributeValues[0]->ToString()));
#endif
				lRet = true;
			}
		}
#endif

		if	(
				(!lRet)
			&&	(lTypeBuilder)
			&&	(
					(lAttributeString->IndexOf (INTEROP_COCLASS_TYPE) >= 0)
				||	(lAttributeString->IndexOf (INTEROP_COMSOURCEINTERFACES_TYPE) >= 0)
				||	(lAttributeString->IndexOf (INTEROP_COMEVENTINTERFACE_TYPE) >= 0)
				)
			)
		{
			int		lValueNdx;
			Type^	lValueType;

//
//	Translate references to copied types
//
			for	(lValueNdx = 0; lValueNdx < pAttributeValues->Length; lValueNdx++)
			{
				if	(String::Compare (pAttributeValues[lValueNdx]->GetType()->ToString(), "System.RuntimeType", true) == 0)
				{
					lValueType = GetTargetType (pAttributeValues[lValueNdx]->ToString(), true);
					if	(lValueType)
					{
#ifdef	_LOG_FIXES
						LogMessage (_LOG_FIXES, _T("--> Change [%s] to [%s] in [%s] for [%s]"), _B(pAttributeValues[lValueNdx]->ToString()), _BT(lValueType), _B(lAttributeString), _BT(lTypeBuilder));
#endif
						pAttributeValues[lValueNdx] = lValueType;
					}
				}
				else
				if	(String::Compare (pAttributeValues[lValueNdx]->GetType()->ToString(), "System.String", true) == 0)
				{
					lValueType = GetTargetType (pAttributeValues[lValueNdx]->ToString(), false);
					if	(lValueType)
					{
#ifdef	_LOG_FIXES
						LogMessage (_LOG_FIXES, _T("--> Change [%s] to [%s] in [%s] for [%s]"), _B(pAttributeValues[lValueNdx]->ToString()), _BT(lValueType), _B(lAttributeString), _BT(lTypeBuilder));
#endif
						pAttributeValues[lValueNdx] = lValueType->FullName;
					}
				}
			}
		}
	}
	catch AnyExceptionDebug

#if	FALSE
//
//	This logic was to unhide the GetEnumerator method, but if it's only marked as Restricted and not Hidden in the ODL, that's enough.
//
	try
	{
		MethodBuilder^	lMethodBuilder;
		String^			lAttributeString;

		try
		{
			lMethodBuilder = safe_cast <MethodBuilder^> (pTarget);
			lAttributeString = pAttribute->ToString();
		}
		catch AnyExceptionSilent

		if	(
				(lMethodBuilder)
			&&	(lAttributeString->IndexOf (INTEROP_TYPELIBFUNC_TYPE) >= 0)
			&&	(pAttribute->ConstructorArguments->Count > 0)
			&&	(String::Compare (lMethodBuilder->Name, "GetEnumerator", true) == 0)
			)
		{
#ifdef	_LOG_FIXES
			int	lTypeLibFuncFlags = (Int16) pAttribute->ConstructorArguments [0].Value;
			LogMessage (_LOG_FIXES, _T("--> Skip   [%s] for [%s]"), _B(TypeLibFuncFlagsStr((TypeLibFuncFlags)lTypeLibFuncFlags)), _BM(lMethodBuilder));
#endif
			lRet = true;
		}
	}
	catch AnyExceptionDebug
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

void FixupAssembly::FixupCustomAttributes (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes)
{
	Type^				lSourceType = nullptr;
	TypeBuilder^		lTargetType = nullptr;
	MethodInfo^			lSourceMethod = nullptr;
	MethodBuilder^		lTargetMethod = nullptr;
	AssemblyBuilder^	lTargetAssembly = nullptr;

	try
	{
		lSourceType = safe_cast <Type^> (pSource);
		lTargetType = safe_cast <TypeBuilder^> (pTarget);
	}
	catch AnyExceptionSilent
	try
	{
		lSourceMethod = safe_cast <MethodInfo^> (pSource);
		lTargetMethod = safe_cast <MethodBuilder^> (pTarget);
	}
	catch AnyExceptionSilent
	try
	{
		lTargetAssembly = safe_cast <AssemblyBuilder^> (pTarget);
	}
	catch AnyExceptionSilent

	if	(
			(lSourceType)
		&&	(lTargetType)
		&&	(!lSourceType->IsClass)
		&&	(lSourceType->IsImport)
		)
	{
//
//	Hide deprecated CoClass interfaces
//
		try
		{
			CoClassAttribute^	lCoClassAttribute;
			array <Type^>^		lTypeLibTypeArgs = gcnew array <Type^> (1);
			array <Object^>^	lTypeLibTypeFlags = gcnew array <Object^> (1);

			lCoClassAttribute = safe_cast <CoClassAttribute^> (Attribute::GetCustomAttribute (lSourceType, CoClassAttribute::typeid));
			if	(lCoClassAttribute)
			{
				if	((int)GetTypeLibTypeFlags (lCoClassAttribute->CoClass) & (int)TypeLibTypeFlags::FRestricted)
				{
#ifdef	_LOG_FIXES
					LogMessage (_LOG_FIXES, _T("--> Hide   [%s] for Restricted [%s]"), _BT(lTargetType), _BT(lCoClassAttribute->CoClass));
#endif
					lTypeLibTypeArgs [0] = TypeLibTypeFlags::typeid;
					lTypeLibTypeFlags [0] = TypeLibTypeFlags::FHidden;
					pCustomAttributes->Add (gcnew CustomAttributeBuilder (TypeLibTypeAttribute::typeid->GetConstructor(lTypeLibTypeArgs), lTypeLibTypeFlags));
				}
			}
		}
		catch AnyExceptionSilent
		{}
	}

#if	TRUE
	if	(
			(lSourceType)
		&&	(lTargetType)
		&&	(lSourceType->IsClass)
		&&	(!lSourceType->IsAbstract)
		)
	{
		try
		{
			array <Type^>^		lDebuggerNonUserTypes = gcnew array <Type^> (0);
			array <Object^>^	lDebuggerNonUserArgs = gcnew array <Object^> (0);
			
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("--> NUser  [%s]"), _BT(lTargetType));
#endif
			pCustomAttributes->Add (gcnew CustomAttributeBuilder (System::Diagnostics::DebuggerNonUserCodeAttribute::typeid->GetConstructor(lDebuggerNonUserTypes), lDebuggerNonUserArgs));
		}
		catch AnyExceptionSilent
		{}
	}
#endif	

#if	TRUE	
	if	(
			(lSourceMethod)
		&&	(lTargetMethod)
		&&	((int)GetTypeLibFuncFlags (lSourceMethod) & (int)TypeLibFuncFlags::FNonBrowsable)
		)
	{
		try
		{
			array <Type^>^		lDebuggerBrowsableTypes = gcnew array <Type^> (1);
			array <Object^>^	lDebuggerBrowsableArgs = gcnew array <Object^> (1);
			array <Type^>^		lDebuggerHiddenTypes = gcnew array <Type^> (0);
			array <Object^>^	lDebuggerHiddenArgs = gcnew array <Object^> (0);
			
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("--> NDebug [%s] in [%s]"), _BM(lTargetMethod), _BMT(lTargetMethod));
#endif
			lDebuggerBrowsableTypes [0] = System::Diagnostics::DebuggerBrowsableState::typeid;
			lDebuggerBrowsableArgs [0] = System::Diagnostics::DebuggerBrowsableState::Never;

			pCustomAttributes->Add (gcnew CustomAttributeBuilder (System::Diagnostics::DebuggerBrowsableAttribute::typeid->GetConstructor(lDebuggerBrowsableTypes), lDebuggerBrowsableArgs));
			pCustomAttributes->Add (gcnew CustomAttributeBuilder (System::Diagnostics::DebuggerHiddenAttribute::typeid->GetConstructor(lDebuggerHiddenTypes), lDebuggerHiddenArgs));
		}
		catch AnyExceptionSilent
		{}
	}
#endif	

	if	(lTargetAssembly)
	{
		try
		{
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("--> [%s] AllowPartiallyTrustedCallers"), _B(lTargetAssembly->FullName));
#endif
			pCustomAttributes->Add (gcnew CustomAttributeBuilder (Security::AllowPartiallyTrustedCallersAttribute::typeid->GetConstructor(gcnew array <Type^> (0)), gcnew array <Object^> (0)));
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

TypeLibFuncFlags FixupAssembly::GetTypeLibFuncFlags (MethodInfo^ pMethod)
{
	TypeLibFuncFlags	lRet = (TypeLibFuncFlags)0;

	try
	{
		TypeLibFuncAttribute^	lTypeLibFuncAttribute = nullptr;

		lTypeLibFuncAttribute = safe_cast <TypeLibFuncAttribute^> (Attribute::GetCustomAttribute (pMethod, TypeLibFuncAttribute::typeid));
		if	(lTypeLibFuncAttribute)
		{
			lRet = lTypeLibFuncAttribute->Value;
		}
	}
	catch AnyExceptionSilent
	{}
	return lRet;
}

TypeLibTypeFlags FixupAssembly::GetTypeLibTypeFlags (Type^ pType)
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

TypeLibTypeFlags FixupAssembly::GetTypeLibTypeFlags (String^ pTypeName)
{
	TypeLibTypeFlags			lRet = (TypeLibTypeFlags)0;
	KeyValuePair<Type^, Type^>^	lPair;

	for each (lPair in mCopiedTypes)
	{
		if	(String::Compare (lPair->Key->FullName, pTypeName, true) == 0)
		{
			lRet = GetTypeLibTypeFlags (lPair->Key);
			break;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
