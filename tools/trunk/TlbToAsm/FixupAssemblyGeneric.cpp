#include "stdafx.h"
#include <corhdr.h>
#include "FixupAssembly.h"

namespace DoubleAgent {
namespace TlbToAsm {

/////////////////////////////////////////////////////////////////////////////
//
//	Hide or skip NonCreatable classes?
//	Nope, we still need them 'cause we're replacing interface references with class references.
//
bool FixupAssembly::HideNonCreatableCoClass (Type^ pSourceType, TypeAttributes & pTypeAttributes)
{
	bool	lRet = false;
#if	FALSE
	if	(
			(pSourceType->IsClass)
		&&	(pSourceType->IsCOMObject)
		&&	(((int)mCopy->GetTypeLibTypeFlags (pSourceType) & (int)TypeLibTypeFlags::FCanCreate) == 0)
		)
	{
#if	FALSE
		pTypeAttributes = (TypeAttributes) ((int)pTypeAttributes & ~(int)TypeAttributes::VisibilityMask | (int)TypeAttributes::NotPublic);
#ifdef	_LOG_FIXES
		LogMessage (_LOG_FIXES, _T("--> Class      [%s] from [%s] to [%s]"), _BT(pSourceType), _B(mCopy->TypeAttrsStr(pSourceType->Attributes)), _B(mCopy->TypeAttrsStr(pTypeAttributes)));
#endif
#endif

#if	FALSE
#ifdef	_LOG_FIXES
		LogMessage (_LOG_FIXES, _T("--> Skip       [%s] NonCreatable Class"), _BT(pSourceType));
#endif
		lRet = true;
#endif
	}
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
//
//	Hide NonBrowsable methods (mostly deprecated methods) or make them Protected (applies to _NewEnum)
//
void FixupAssembly::HideNonBrowsableMethod (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes)
{
#if	TRUE
	TypeLibFuncFlags	lTypeLibFuncFlags = mCopy->GetTypeLibFuncFlags (pSourceMethod);

	if	(
			(pSourceMethod->DeclaringType->IsImport)
		&&	(!pSourceMethod->DeclaringType->IsInterface)
		&&	((int)lTypeLibFuncFlags & (int)TypeLibFuncFlags::FRestricted)
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
		LogMessage (_LOG_FIXES, _T("--> Method     [%s] in [%s] from [%s] to [%s] for [%s]"), _BM(pSourceMethod), _BMT(pSourceMethod), _B(mCopy->MethodAttrsStr(pSourceMethod->Attributes)), _B(mCopy->MethodAttrsStr(pMethodAttributes)), _B(mCopy->TypeLibFuncFlagsStr(lTypeLibFuncFlags)));
#endif
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
//
//	Seal accessor method overrides on classes
//
void FixupAssembly::SealAccessorOverride (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes)
{
#if	TRUE
	if	(
			(pSourceMethod->IsSpecialName)
		&&	(pSourceMethod->DeclaringType->IsClass)
		&&	(pSourceMethod->DeclaringType->IsCOMObject)
		)
	{
		pMethodAttributes = (MethodAttributes) ((int)pMethodAttributes | (int)MethodAttributes::Final);
#ifdef	_LOG_FIXES
		LogMessage (_LOG_FIXES, _T("--> Method     [%s] in [%s] from [%s] to [%s]"), _BM(pSourceMethod), _BMT(pSourceMethod), _B(mCopy->MethodAttrsStr(pSourceMethod->Attributes)), _B(mCopy->MethodAttrsStr(pMethodAttributes)));
#endif
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
//
//	Make hidden methods protected - not for now because it causes compiler errors
//
void FixupAssembly::ProtectHiddenMethod (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes)
{
#if	FALSE
	TypeLibFuncFlags	lTypeLibFuncFlags = mCopy->GetTypeLibFuncFlags (pSourceMethod);

	if	((int)lTypeLibFuncFlags & (int)TypeLibFuncFlags::FHidden)
	{
		pMethodAttributes = (MethodAttributes) ((int)pMethodAttributes & ~(int)MethodAttributes::MemberAccessMask | (int)MethodAttributes::Assembly);
#ifdef	_LOG_FIXES
		LogMessage (_LOG_FIXES, _T("--> Method     [%s] in [%s] from [%s] to [%s]"), _BM(pSourceMethod), _BMT(pSourceMethod), _B(mCopy->MethodAttrsStr(pSourceMethod->Attributes)), _B(mCopy->MethodAttrsStr(pMethodAttributes)));
#endif
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
//
//	Change type reference from interface to class
//
void FixupAssembly::InterfaceTypeToClassType (Type^ pSourceType, Type^& pTargetType)
{
	try
	{
		Type^	lSourceType = pSourceType;
		Type^	lClassType;
		
		if	(pSourceType->IsArray)
		{
			lSourceType = lSourceType->GetElementType ();
		}
		lClassType = mCopy->GetTargetType (lSourceType->FullName + "Class", false);

		if	(lClassType)
		{
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("--> Type       [%s] as [%s]"), _BT(pSourceType), _BT(lClassType));
#endif
			if	(pSourceType->IsArray)
			{
				pTargetType = lClassType->MakeArrayType ();
			}
			else
			{
				pTargetType = lClassType;
			}
		}
	}
	catch AnyExceptionSilent
}

void FixupAssembly::InterfaceTypeToClassType (MethodInfo^ pSourceMethod, Type^& pReturnType)
{
	try
	{
		Type^	lReturnType = pSourceMethod->ReturnType;
		Type^	lClassType;
		
		if	(pSourceMethod->ReturnType->IsArray)
		{
			lReturnType = lReturnType->GetElementType ();
		}
		lClassType = mCopy->GetTargetType (lReturnType->FullName + "Class", false);

		if	(lClassType)
		{
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("--> Return     [%s] as [%s] for [%s] in [%s]"), _BT(pReturnType), _BT(lClassType), _BM(pSourceMethod), _BMT(pSourceMethod));
#endif
			if	(pSourceMethod->ReturnType->IsArray)
			{
				pReturnType = lClassType->MakeArrayType ();
			}
			else
			{
				pReturnType = lClassType;
			}
		}
	}
	catch AnyExceptionSilent
}

void FixupAssembly::InterfaceTypeToClassType (MethodBase^ pSourceMethod, ParameterInfo^ pSourceParameter, Type^& pParameterType)
{
	try
	{
		Type^	lParameterType = pSourceParameter->ParameterType;
		Type^	lClassType;
		
		if	(pSourceParameter->ParameterType->IsArray)
		{
			lParameterType = lParameterType->GetElementType ();
		}
		lClassType = mCopy->GetTargetType (lParameterType->FullName + "Class", false);

		if	(lClassType)
		{
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("--> Param      [%s] as [%s] for [%s] in [%s] [%s]"), _BT(pParameterType), _BT(lClassType), _BM(pSourceParameter), _BM(pSourceMethod), _BMT(pSourceMethod));
#endif
			if	(pSourceParameter->ParameterType->IsArray)
			{
				pParameterType = lClassType->MakeArrayType ();
			}
			else
			{
				pParameterType = lClassType;
			}
		}
	}
	catch AnyExceptionSilent
}

void FixupAssembly::InterfaceTypeToClassType (PropertyInfo^ pSourceProperty, Type^& pPropertyType)
{
	try
	{
		Type^	lPropertyType = pSourceProperty->PropertyType;
		Type^	lClassType;
		
		if	(pSourceProperty->PropertyType->IsArray)
		{
			lPropertyType = lPropertyType->GetElementType ();
		}
		lClassType = mCopy->GetTargetType (lPropertyType->FullName + "Class", false);

		if	(lClassType)
		{
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("--> Define     [%s] as [%s] for [%s] in [%s]"), _BT(pPropertyType), _BT(lClassType), _BM(pSourceProperty), _BMT(pSourceProperty));
#endif
			if	(pSourceProperty->PropertyType->IsArray)
			{
				pPropertyType = lClassType->MakeArrayType ();
			}
			else
			{
				pPropertyType = lClassType;
			}
		}
	}
	catch AnyExceptionSilent
}

/////////////////////////////////////////////////////////////////////////////
//
//	Rename covariant to overridden method name (supported in C# but not C++)
//
void FixupAssembly::FixMethodOverride (MethodBase^ pSourceMethod, String^& pMethodName)
{
#if	TRUE
	try
	{
		MethodInfo^	lOverrideMethod;
		if	(
				(lOverrideMethod = mCopy->IsMethodOverride (pSourceMethod))
			&&	(String::Compare (pSourceMethod->Name, lOverrideMethod->Name) != 0)
			)
		{
			pMethodName = lOverrideMethod->Name;
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("--> Method     [%s] as [%s] in [%s] for override [%s.%s]"), _BM(pSourceMethod), _B(pMethodName), _BMT(pSourceMethod), _BMT(lOverrideMethod), _BM(lOverrideMethod));
#endif
		}
	}
	catch AnyExceptionSilent
#endif
}

/////////////////////////////////////////////////////////////////////////////
//
//	Capitalize first letter
//
void FixupAssembly::FixParameterName (MethodBase^ pSourceMethod, ParameterInfo^ pSourceParameter, String^& pParameterName)
{
	if	(
			(pParameterName)
		&&	(pParameterName->Length > 1)
		&&	(
				(pParameterName[0] != 'p')
			||	(!Char::IsUpper (pParameterName, 1))
			)
		)
	{
		String^	lParameterName;

#if	FALSE
		if	(
				(pParameterName->Length > 2)
			&&	(pParameterName[0] == 'p')
			)
		{
			lParameterName = String::Concat (pParameterName->Substring (0,1) ,pParameterName->Substring (1,1)->ToUpper(),  pParameterName->Substring (2));
		}
		else
#endif
		{
			lParameterName = String::Concat (pParameterName->Substring (0,1)->ToUpper(), pParameterName->Substring (1));
		}

		if	(String::Compare (lParameterName, pParameterName) != 0)
		{
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("--> Param      [%s] to [%s] in [%s] [%s]"), _B(pParameterName), _B(lParameterName), _BM(pSourceMethod), _BMT(pSourceMethod));
#endif
			pParameterName = lParameterName;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
//	Remove hungarian notation field prefixes and capitalize first letter
//
void FixupAssembly::FixFieldName (FieldInfo^ pSourceField, String^& pFieldName)
{
	if	(pFieldName->StartsWith ("m_dw"))
	{
		pFieldName = pFieldName->Substring (4);
#ifdef	_LOG_FIXES
		LogMessage (_LOG_FIXES, _T("--> Field      [%s] as [%s] in [%s]"), _B(pSourceField->Name), _B(pFieldName), _BMT(pSourceField));
#endif
	}
	else
	if	(pFieldName->StartsWith ("m_a"))
	{
		pFieldName = pFieldName->Substring (3);
#ifdef	_LOG_FIXES
		LogMessage (_LOG_FIXES, _T("--> Field      [%s] as [%s] in [%s]"), _B(pSourceField->Name), _B(pFieldName), _BMT(pSourceField));
#endif
	}
	else
	if	(pFieldName->StartsWith ("m_"))
	{
		pFieldName = pFieldName->Substring (2);
#ifdef	_LOG_FIXES
		LogMessage (_LOG_FIXES, _T("--> Field      [%s] as [%s] in [%s]"), _B(pSourceField->Name), _B(pFieldName), _BMT(pSourceField));
#endif
	}

	if	(
			(pFieldName->Length > 1)
		&&	(
				(pFieldName[0] != 'm')
			||	(!Char::IsUpper (pFieldName, 1))
			)
		)
	{
		String^	lFieldName;

#if	FALSE
		if	(
				(pFieldName->Length > 2)
			&&	(pFieldName[0] == 'm')
			)
		{
			lFieldName = String::Concat (pFieldName->Substring (0,1), pFieldName->Substring (1,1)->ToUpper(), pFieldName->Substring (2));
		}
		else
#endif
		{
			lFieldName = String::Concat (pFieldName->Substring (0,1)->ToUpper(), pFieldName->Substring (1));
		}

		if	(String::Compare (lFieldName, pFieldName) != 0)
		{
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("--> Field      [%s] as [%s] in [%s]"), _B(pFieldName), _B(lFieldName), _BMT(pSourceField));
#endif
			pFieldName = lFieldName;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
//	Remove prefix on Enum field names when the prefix matches the Enum name
//
void FixupAssembly::FixEnumFieldName (FieldInfo^ pSourceField, String^& pFieldName, EnumBuilder^ pEnumBuilder)
{
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
		LogMessage (_LOG_FIXES, _T("--> Field      [%s] as [%s] in [%s]"), _BM(pSourceField), _B(pFieldName), _BT(pEnumBuilder));
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
//
//	Add the AllowPartiallyTrustedCallers attribute to the target assembly
//
void FixupAssembly::AllowPartiallyTrustedCallers (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes)
{
	AssemblyBuilder^	lTargetAssembly = nullptr;

	try
	{
		lTargetAssembly = safe_cast <AssemblyBuilder^> (pTarget);
	}
	catch AnyExceptionSilent

	if	(lTargetAssembly)
	{
		try
		{
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("--> Assembly   [%s] AllowPartiallyTrustedCallers"), _B(lTargetAssembly->FullName));
#endif
			pCustomAttributes->Add (gcnew CustomAttributeBuilder (Security::AllowPartiallyTrustedCallersAttribute::typeid->GetConstructor(gcnew array <Type^> (0)), gcnew array <Object^> (0)));
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////
//
//	Translate references to copied types
//
void FixupAssembly::RenameAttributeTypes (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues)
{
	TypeBuilder^	lTypeBuilder;

	try
	{
		lTypeBuilder = safe_cast <TypeBuilder^> (pTarget);
	}
	catch AnyExceptionSilent

	if	(
			(lTypeBuilder)
		&&	(
				(System::Runtime::InteropServices::CoClassAttribute::typeid->IsAssignableFrom (pAttribute->Constructor->DeclaringType))
			||	(System::Runtime::InteropServices::ComSourceInterfacesAttribute::typeid->IsAssignableFrom (pAttribute->Constructor->DeclaringType))
			||	(System::Runtime::InteropServices::ComEventInterfaceAttribute::typeid->IsAssignableFrom (pAttribute->Constructor->DeclaringType))
			)
		)
	{
		int		lValueNdx;
		Type^	lValueType;

		for	(lValueNdx = 0; lValueNdx < pAttributeValues->Length; lValueNdx++)
		{
			if	(String::Compare (pAttributeValues[lValueNdx]->GetType()->ToString(), "System.RuntimeType", true) == 0)
			{
				lValueType = mCopy->GetTargetType (pAttributeValues[lValueNdx]->ToString(), true);
				if	(lValueType)
				{
#ifdef	_LOG_FIXES
					LogMessage (_LOG_FIXES, _T("--> Change     [%s] to [%s] in [%s] for [%s]"), _B(pAttributeValues[lValueNdx]->ToString()), _BT(lValueType), _BT(pAttribute->Constructor->DeclaringType), _BT(lTypeBuilder));
#endif
					pAttributeValues[lValueNdx] = lValueType;
				}
			}
			else
			if	(String::Compare (pAttributeValues[lValueNdx]->GetType()->ToString(), "System.String", true) == 0)
			{
				lValueType = mCopy->GetTargetType (pAttributeValues[lValueNdx]->ToString(), false);
				if	(lValueType)
				{
#ifdef	_LOG_FIXES
					LogMessage (_LOG_FIXES, _T("--> Change     [%s] to [%s] in [%s] for [%s]"), _B(pAttributeValues[lValueNdx]->ToString()), _BT(lValueType), _BT(pAttribute->Constructor->DeclaringType), _BT(lTypeBuilder));
#endif
					pAttributeValues[lValueNdx] = lValueType->FullName;
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
//	Remove hidden attribute for class/interface wrappers (inherited from the native interface during import)
//
bool FixupAssembly::UnhideTypeWrapper (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues)
{
	bool			lRet = false;
	TypeBuilder^	lTypeBuilder;

	try
	{
		lTypeBuilder = safe_cast <TypeBuilder^> (pTarget);
	}
	catch AnyExceptionSilent

#if	TRUE
	if	(
			(lTypeBuilder)
		&&	(!lTypeBuilder->IsImport)
		&&	(System::Runtime::InteropServices::TypeLibTypeAttribute::typeid->IsAssignableFrom (pAttribute->Constructor->DeclaringType))
		&&	(pAttributeValues->Length == 1)
		)
	{
#ifdef	_LOG_FIXES
		LogMessage (_LOG_FIXES, _T("--> Unhide     [%s] (remove [%s])"), _BT(lTypeBuilder), _BT(pAttribute->Constructor->DeclaringType));
#endif
		lRet = true;
	}
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
//
//	This logic unhides the GetEnumerator method, but if it's only marked as Restricted and not Hidden in the ODL, that's enough.
//	Right now, it just removes the TypeLibFuncFlags attribute, but it really should modify it to keep Restricted and remove Hidden.
//
bool FixupAssembly::UnhideGetEnumerator (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues)
{
	bool			lRet = false;
	MethodBuilder^	lMethodBuilder;

	try
	{
		lMethodBuilder = safe_cast <MethodBuilder^> (pTarget);
	}
	catch AnyExceptionSilent

	if	(
			(lMethodBuilder)
		&&	(System::Runtime::InteropServices::TypeLibFuncAttribute::typeid->IsAssignableFrom (pAttribute->Constructor->DeclaringType))
		&&	(pAttribute->ConstructorArguments->Count > 0)
		&&	(String::Compare (lMethodBuilder->Name, "GetEnumerator", true) == 0)
		)
	{
#ifdef	_LOG_FIXES
		int	lTypeLibFuncFlags = (Int16) pAttribute->ConstructorArguments [0].Value;
		LogMessage (_LOG_FIXES, _T("--> Skip       [%s] for [%s]"), _B(mCopy->TypeLibFuncFlagsStr((TypeLibFuncFlags)lTypeLibFuncFlags)), _BM(lMethodBuilder));
#endif
		lRet = true;
	}

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

void FixupAssembly::SetPropertyBindable (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues)
{
	PropertyInfo^		lSourceProperty;
	PropertyBuilder^	lPropertyBuilder;
	TypeLibFuncFlags	lFuncFlags;

	try
	{
		lSourceProperty = safe_cast <PropertyInfo^> (pSource);
		lPropertyBuilder = safe_cast <PropertyBuilder^> (pTarget);
	}
	catch AnyExceptionSilent
	
	if	(
			(lSourceProperty)
		&&	(lPropertyBuilder)
		&&	(lSourceProperty->DeclaringType->IsClass)
		&&	(System::ComponentModel::BindableAttribute::typeid->IsAssignableFrom (pAttribute->Constructor->DeclaringType))
		&&	(pAttributeValues->Length == 1)
		&&	(mCopy->GetTypeLibFuncFlags (lSourceProperty, lFuncFlags))
		)
	{
		System::Boolean^	lBindable = gcnew System::Boolean (((int)lFuncFlags & (int)TypeLibFuncFlags::FDisplayBind) ? true : false);
		
		if	(lBindable != pAttributeValues[0])
		{
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("--> Property   [%s.%s] [%s] from [%s] to [%s]"), _BMT(lPropertyBuilder), _BM(lPropertyBuilder), _BT(pAttribute->Constructor->DeclaringType), _B(pAttributeValues[0]->ToString()), _B(lBindable->ToString()));
#endif
			pAttributeValues[0] = lBindable;			
		}
	}
}

void FixupAssembly::SetPropertyBindable (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes)
{
	PropertyInfo^		lSourceProperty;
	PropertyBuilder^	lPropertyBuilder;
	TypeLibFuncFlags	lFuncFlags;

	try
	{
		lSourceProperty = safe_cast <PropertyInfo^> (pSource);
		lPropertyBuilder = safe_cast <PropertyBuilder^> (pTarget);
	}
	catch AnyExceptionSilent
	
	if	(
			(lSourceProperty)
		&&	(lPropertyBuilder)
		&&	(lSourceProperty->DeclaringType->IsClass)
		&&	(mCopy->GetTypeLibFuncFlags (lSourceProperty, lFuncFlags))
		&&	((int)lFuncFlags & (int)TypeLibFuncFlags::FBindable)
		)
	{
		int	lNdx;
		
		for	(lNdx = pCustomAttributes->Count-1; lNdx >= 0; lNdx--)
		{
			if	(pCustomAttributes [lNdx]->ToString()->Contains (System::ComponentModel::BindableAttribute::typeid->Name))
			{
				break;
			}
		}
		
		if	(lNdx < 0)
		{
			array <Type^>^		lAttributeTypes = gcnew array <Type^> (1);
			array <Object^>^	lAttributeArgs = gcnew array <Object^> (1);

			lAttributeTypes[0] = System::Boolean::typeid;
			lAttributeArgs[0] = gcnew System::Boolean (((int)lFuncFlags & (int)TypeLibFuncFlags::FDisplayBind) ? true : false);
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("--> Property   [%s.%s] [%s] [%s]"), _BMT(lPropertyBuilder), _BM(lPropertyBuilder), _BT(System::ComponentModel::BindableAttribute::typeid), _B(lAttributeArgs[0]->ToString()));
#endif
			pCustomAttributes->Add (gcnew CustomAttributeBuilder (System::ComponentModel::BindableAttribute::typeid->GetConstructor(lAttributeTypes), lAttributeArgs));
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
//
//	Remove the CoClassAttribute to keep interfaces from showing up as classes in the object browser.
//	We want them to show up as interfaces, and the CoClassAttribute causes interfaces to show up as classes in VB.
//	For now, removing the CoClassAttribute causes runtime errors - gotta look into this.
//
bool FixupAssembly::RemoveInterfaceCoClass (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues)
{
	bool			lRet = false;
#if	FALSE
	TypeBuilder^	lTypeBuilder;

	try
	{
		lTypeBuilder = safe_cast <TypeBuilder^> (pTarget);
	}
	catch AnyExceptionSilent

	if	(
			(lTypeBuilder)
		&&	(!lTypeBuilder->IsClass)
		&&	(System::Runtime::InteropServices::CoClassAttribute::typeid->IsAssignableFrom (pAttribute->Constructor->DeclaringType))
		&&	(pAttributeValues->Length == 1)
		&&	(String::Compare (pAttributeValues[0]->GetType()->ToString(), "System.RuntimeType", true) == 0)
		)
	{
		Type^	lValueType;

		if	(
				(lValueType = mCopy->GetTargetType (pAttributeValues[0]->ToString(), false))
			&&	(lValueType->IsClass)
			&&	(lValueType->IsCOMObject)
			&&	(((int)mCopy->GetTypeLibTypeFlags (lValueType) & (int)TypeLibTypeFlags::FCanCreate) == 0)
			)
		{
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("--> Skip       [%s] in [%s] for [%s]"), _BT(pAttribute->Constructor->DeclaringType), _BT(lTypeBuilder), _B(pAttributeValues[0]->ToString()));
#endif
			lRet = true;
		}
	}
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
//
//	Hide interfaces for restricted (deprecated) CoClass
//
void FixupAssembly::HideRestrictedCoClassInterface (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes)
{
	Type^			lSourceType = nullptr;
	TypeBuilder^	lTargetType = nullptr;

	try
	{
		lSourceType = safe_cast <Type^> (pSource);
		lTargetType = safe_cast <TypeBuilder^> (pTarget);
	}
	catch AnyExceptionSilent

#if	TRUE
	if	(
			(lSourceType)
		&&	(lTargetType)
		&&	(!lSourceType->IsClass)
		&&	(lSourceType->IsImport)
		)
	{
		try
		{
			CoClassAttribute^	lCoClassAttribute;
			array <Type^>^		lTypeLibTypeArgs = gcnew array <Type^> (1);
			array <Object^>^	lTypeLibTypeFlags = gcnew array <Object^> (1);

			lCoClassAttribute = safe_cast <CoClassAttribute^> (Attribute::GetCustomAttribute (lSourceType, CoClassAttribute::typeid));
			if	(lCoClassAttribute)
			{
				if	((int)mCopy->GetTypeLibTypeFlags (lCoClassAttribute->CoClass) & (int)TypeLibTypeFlags::FRestricted)
				{
#ifdef	_LOG_FIXES
					LogMessage (_LOG_FIXES, _T("--> Hide       [%s] for Restricted [%s]"), _BT(lTargetType), _BT(lCoClassAttribute->CoClass));
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
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
//
//	Add DebuggerNonUserCodeAttribute to the target type
//
void FixupAssembly::SetDebuggerNonUserType (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes)
{
#ifndef	_DEBUG
	Type^			lSourceType = nullptr;
	TypeBuilder^	lTargetType = nullptr;

	try
	{
		lSourceType = safe_cast <Type^> (pSource);
		lTargetType = safe_cast <TypeBuilder^> (pTarget);
	}
	catch AnyExceptionSilent

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
			LogMessage (_LOG_FIXES, _T("--> NonUser    [%s]"), _BT(lTargetType));
#endif
			pCustomAttributes->Add (gcnew CustomAttributeBuilder (System::Diagnostics::DebuggerNonUserCodeAttribute::typeid->GetConstructor(lDebuggerNonUserTypes), lDebuggerNonUserArgs));
		}
		catch AnyExceptionSilent
		{}
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
//
//	Add DebuggerHiddenAttribute and DebuggerBrowsableAttribute to the target method
//
void FixupAssembly::SetDebuggerHiddenMethod (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes)
{
#ifndef	_DEBUG
	MethodInfo^		lSourceMethod = nullptr;
	MethodBuilder^	lTargetMethod = nullptr;

	try
	{
		lSourceMethod = safe_cast <MethodInfo^> (pSource);
		lTargetMethod = safe_cast <MethodBuilder^> (pTarget);
	}
	catch AnyExceptionSilent

	if	(
			(lSourceMethod)
		&&	(lTargetMethod)
		&&	((int)mCopy->GetTypeLibFuncFlags (lSourceMethod) & (int)TypeLibFuncFlags::FNonBrowsable)
		)
	{
		try
		{
			array <Type^>^		lDebuggerBrowsableTypes = gcnew array <Type^> (1);
			array <Object^>^	lDebuggerBrowsableArgs = gcnew array <Object^> (1);
			array <Type^>^		lDebuggerHiddenTypes = gcnew array <Type^> (0);
			array <Object^>^	lDebuggerHiddenArgs = gcnew array <Object^> (0);

#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("--> NoDebug    [%s] in [%s]"), _BM(lTargetMethod), _BMT(lTargetMethod));
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
}

/////////////////////////////////////////////////////////////////////////////
//
//	Add DebuggerHiddenAttribute and DebuggerBrowsableAttribute to the target property
//
void FixupAssembly::SetDebuggerHiddenProperty (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes)
{
#ifndef	_DEBUG
	PropertyInfo^		lSourceProperty = nullptr;
	PropertyBuilder^	lTargetProperty = nullptr;
	MethodInfo^			lSourceMethod = nullptr;

	try
	{
		lSourceProperty = safe_cast <PropertyInfo^> (pSource);
		lTargetProperty = safe_cast <PropertyBuilder^> (pTarget);
	}
	catch AnyExceptionSilent

	if	(
			(lSourceProperty)
		&&	(lTargetProperty)
		&&	(lSourceMethod = lSourceProperty->GetGetMethod ())
		&&	((int)mCopy->GetTypeLibFuncFlags (lSourceMethod) & (int)TypeLibFuncFlags::FNonBrowsable)
		)
	{
		try
		{
			array <Type^>^		lDebuggerBrowsableTypes = gcnew array <Type^> (1);
			array <Object^>^	lDebuggerBrowsableArgs = gcnew array <Object^> (1);
			array <Type^>^		lDebuggerHiddenTypes = gcnew array <Type^> (0);
			array <Object^>^	lDebuggerHiddenArgs = gcnew array <Object^> (0);

#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("--> NoDebug    [%s] in [%s]"), _BM(lTargetProperty), _BMT(lTargetProperty));
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
}

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
