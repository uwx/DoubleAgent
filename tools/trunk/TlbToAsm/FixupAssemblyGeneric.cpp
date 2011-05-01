#include "stdafx.h"
#include <corhdr.h>
#include "FixupAssembly.h"

namespace DoubleAgent {
namespace TlbToAsm {

/////////////////////////////////////////////////////////////////////////////
//
//	Runtime Callable Wrappers are turned into real wrappers
//
bool FixupAssembly::CreateClassWrappers ()
{
//	return (CopyAssembly::AssemblyRuntimeVersion () < 4);
	return true;
}

bool FixupAssembly::IsCoClassWrapper (Type^ pSourceType)
{
	bool	lRet = false;

	if	(
			(CreateClassWrappers())
		&&	(pSourceType)
		&&	(pSourceType->IsClass)
		&&	(pSourceType->IsImport)
		&&	(pSourceType->IsCOMObject)
		)
	{
		lRet = true;
	}
	return lRet;
}

Type^ FixupAssembly::GetCoClassInterface (Type^ pSourceType)
{
	Type^	lRet = nullptr;

	if	(
			(CreateClassWrappers())
		&&	(IsCoClassWrapper (pSourceType))
		)
	{
		array<Type^>^	lInterfaces = pSourceType->GetInterfaces ();
		Type^			lInterface;

		for each (lInterface in lInterfaces)
		{
			if	(!mCopy->IsInterfaceInherited (pSourceType, lInterface))
			{
				lRet = lInterface;
				break;
			}
		}
	}
	return lRet;
}

Type^ FixupAssembly::GetCoClassWrapper (Type^ pSourceInterface)
{
	Type^	lRet = nullptr;

	if	(
			(CreateClassWrappers())
		&&	(pSourceInterface)
		&&	(pSourceInterface->IsInterface)
		)
	{
		Type^	lSourceInterface = pSourceInterface;
		Type^	lClassType;

		if	(lSourceInterface->IsArray)
		{
			lSourceInterface = lSourceInterface->GetElementType ();
		}
		lClassType = mCopy->GetSourceType (lSourceInterface->FullName + "Class");

		if	(
				(lClassType)
			&&	(IsCoClassWrapper (lClassType))
			)
		{
			if	(pSourceInterface->IsArray)
			{
				lClassType = lClassType->MakeArrayType ();
			}
			lRet = lClassType;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
//
//	Fixup wrapper type definition - skip inheritance and import
//
bool FixupAssembly::FixupWrapperType (Type^ pSourceType, String^& pTypeName, Type^& pBaseType, TypeAttributes & pTypeAttributes)
{
	if	(
			(CreateClassWrappers())
		&&	(IsCoClassWrapper (pSourceType))
		)
	{
		TypeAttributes	lTypeAttributes = (TypeAttributes) ((int)pTypeAttributes & ~(int)TypeAttributes::Import);
#ifdef	_LOG_WRAPPERS
		LogMessage (_LOG_WRAPPERS, _T("===> Type       [%s] from [%s] to [%s]"), _BT(pSourceType), _B(mCopy->TypeAttrsStr(pTypeAttributes)), _B(mCopy->TypeAttrsStr(lTypeAttributes)));
#endif
		pBaseType = nullptr;
		pTypeAttributes = lTypeAttributes;
	}
	return false;
}

bool FixupAssembly::FixupWrapperInterface (Type^ pSourceType, Type^ pSourceInterface, TypeBuilder^ pTargetType, Type^& pTargetInterface)
{
	bool	lRet = false;

	if	(
			(CreateClassWrappers())
		&&	(IsCoClassWrapper (pSourceType))
		)
	{
		try
		{
			Type^	lEnumerable = pSourceInterface->GetInterface ("IEnumerable");
			Type^	lEnumerableType;

			if	(lEnumerable)
			{
				if	(
						(lEnumerableType = mCopy->GetEnumerableType (pSourceType))
					&&	(lEnumerableType->IsInterface)
					&&	(lEnumerableType = mCopy->GetCoClassWrapper (lEnumerableType))
					&&	(lEnumerableType = mCopy->GetTargetType (lEnumerableType, false))
					)
				{
					pTargetInterface = System::Collections::Generic::IEnumerable::typeid->MakeGenericType (lEnumerableType);
				}
				else
				{
					pTargetInterface = lEnumerable;
				}
#ifdef	_LOG_WRAPPERS
				LogMessage (_LOG_WRAPPERS, _T("===> Type       [%s] Interface [%s] to [%s]"), _BT(pSourceType), _BT(pSourceInterface), _BT(pTargetInterface));
#endif
			}
			else
			{
#ifdef	_LOG_WRAPPERS
				LogMessage (_LOG_WRAPPERS, _T("===> Skip       [%s] Interface [%s]"), _BT(pSourceType), _BT(pSourceInterface));
#endif
				lRet = true;
			}
		}
		catch AnyExceptionDebug
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
//
//	Skip wrapper constuctor (during copy - it will be synthesized later)
//
bool FixupAssembly::FixupWrapperConstructor (ConstructorInfo^ pSourceConstructor, MethodAttributes & pConstructorAttributes)
{
	if	(
			(CreateClassWrappers())
		&&	(IsCoClassWrapper (pSourceConstructor->DeclaringType))
		)
	{
#ifdef	_LOG_WRAPPERS
		LogMessage (_LOG_WRAPPERS, _T("===> Skip       [%s.%s]"), _BMT(pSourceConstructor), _BM(pSourceConstructor));
#endif
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
//
//	Hide the IEnumerable GetEnumerator if using IEnumerable<t> (to avoid ambiguity)
//
void FixupAssembly::FixupWrapperMethod (MethodInfo^ pSourceMethod, String^& pMethodName, MethodAttributes & pMethodAttributes)
{
	if	(
			(CreateClassWrappers())
		&&	(pSourceMethod->Name == "GetEnumerator")
		&&	(IsCoClassWrapper (pSourceMethod->DeclaringType))
		)
	{
		Type^	lEnumerableType;

		if	(
				(pSourceMethod->DeclaringType->GetInterface ("IEnumerable"))
			&&	(lEnumerableType = mCopy->GetEnumerableType (pSourceMethod->DeclaringType))
			&&	(lEnumerableType->IsInterface)
			)
		{
			MethodAttributes	lAttributes = (MethodAttributes) ((int)pMethodAttributes & ~(int)MethodAttributes::MemberAccessMask | (int)MethodAttributes::Assembly);
#ifdef	_LOG_WRAPPERS
			LogMessage (_LOG_WRAPPERS, _T("===> Method     [%s.%s] from [%s] to [%s]"), _BMT(pSourceMethod), _BM(pSourceMethod), _B(mCopy->MethodAttrsStr(pMethodAttributes)), _B(mCopy->MethodAttrsStr(lAttributes)));
#endif
			pMethodAttributes = lAttributes;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
//	Change wrapper methods to real methods
//
void FixupAssembly::FixupWrapperMethod (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, MethodImplAttributes & pMethodImplAttributes)
{
	if	(
			(CreateClassWrappers())
		&&	(IsCoClassWrapper (pSourceMethod->DeclaringType))
		)
	{
		MethodImplAttributes	lAttributes = (MethodImplAttributes) ((int)MethodImplAttributes::IL | (int)MethodImplAttributes::Managed);
#ifdef	_LOG_WRAPPERS
		LogMessage (_LOG_WRAPPERS, _T("===> Method     [%s.%s] from [%s] to [%s]"), _BMT(pSourceMethod), _BM(pSourceMethod), _B(mCopy->MethodImplementation(pMethodImplAttributes)), _B(mCopy->MethodImplementation(lAttributes)));
#endif
		pMethodImplAttributes = lAttributes;
	}
}

/////////////////////////////////////////////////////////////////////////////
//
//	Skip custom attributes for wrappers
//
bool FixupAssembly::FixupWrapperAttribute (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues)
{
	bool	lRet = false;
	
	if	(CreateClassWrappers())
	{
		Type^			lSourceType;
		MethodInfo^		lSourceMethod;
		PropertyInfo^	lSourceProperty;
		Assembly^		lSourceAssembly;

		if	(Type::typeid->IsInstanceOfType (pSource))
		{
			lSourceType = safe_cast <Type^> (pSource);
		}
		else
		if	(MethodInfo::typeid->IsInstanceOfType (pSource))
		{
			lSourceMethod = safe_cast <MethodInfo^> (pSource);
		}
		else
		if	(PropertyInfo::typeid->IsInstanceOfType (pSource))
		{
			lSourceProperty = safe_cast <PropertyInfo^> (pSource);
		}
		else
		if	(Assembly::typeid->IsInstanceOfType (pSource))
		{
			lSourceAssembly = safe_cast <Assembly^> (pSource);
		}

		if	(
				(lSourceType)
			&&	(lSourceType->IsInterface)
			&&	(lSourceType->IsImport)
			&&	(CoClassAttribute::typeid->IsAssignableFrom (pAttribute->Constructor->DeclaringType))
			&&	(IsCoClassWrapper (pAttribute->Constructor->DeclaringType))
			)
		{
#ifdef	_LOG_WRAPPERS
			LogMessage (_LOG_WRAPPERS, _T("===> Type       [%s] skip [%s]"), _BT(lSourceType), _B(pAttribute->ToString()));
#endif
			lRet = true;
		}
		else
		if	(
				(lSourceType)
			&&	(IsCoClassWrapper (lSourceType))
			&&	(!TypeLibTypeAttribute::typeid->IsAssignableFrom (pAttribute->Constructor->DeclaringType))
			&&	(!DefaultMemberAttribute::typeid->IsAssignableFrom (pAttribute->Constructor->DeclaringType))
			)
		{
#ifdef	_LOG_WRAPPERS
			LogMessage (_LOG_WRAPPERS, _T("===> Type       [%s] skip [%s]"), _BT(lSourceType), _B(pAttribute->ToString()));
#endif
			lRet = true;
		}
		else
		if	(
				(lSourceMethod)
			&&	(IsCoClassWrapper (lSourceMethod->DeclaringType))
			&&	(!TypeLibFuncAttribute::typeid->IsAssignableFrom (pAttribute->Constructor->DeclaringType))
			)
		{
#ifdef	_LOG_WRAPPERS
			LogMessage (_LOG_WRAPPERS, _T("===> Method     [%s.%s] skip [%s]"), _BMT(lSourceMethod), _BM(lSourceMethod), _B(pAttribute->ToString()));
#endif
			lRet = true;
		}
		else
		if	(
				(lSourceProperty)
			&&	(IsCoClassWrapper (lSourceProperty->DeclaringType))
			&&	(!TypeLibVarAttribute::typeid->IsAssignableFrom (pAttribute->Constructor->DeclaringType))
			)
		{
#ifdef	_LOG_WRAPPERS
			LogMessage (_LOG_WRAPPERS, _T("===> Property   [%s.%s] skip [%s]"), _BMT(lSourceProperty), _BM(lSourceProperty), _B(pAttribute->ToString()));
#endif
			lRet = true;
		}
		else
		if	(
				(lSourceAssembly)
			&&	(CreateClassWrappers ())
			&&	(
					(System::Runtime::InteropServices::ImportedFromTypeLibAttribute::typeid->IsAssignableFrom (pAttribute->Constructor->DeclaringType))
				||	(System::Runtime::InteropServices::GuidAttribute::typeid->IsAssignableFrom (pAttribute->Constructor->DeclaringType))
// Keep this one so it can be used to generate the PrimaryInteropAssemblyAttribute
//				||	(System::Runtime::InteropServices::TypeLibVersionAttribute::typeid->IsAssignableFrom (pAttribute->Constructor->DeclaringType))
				||	(System::Windows::Forms::AxHost::TypeLibraryTimeStampAttribute::typeid->IsAssignableFrom (pAttribute->Constructor->DeclaringType))
				)
			)
		{
#ifdef	_LOG_WRAPPERS
			LogMessage (_LOG_WRAPPERS, _T("===> Assembly   skip [%s]"), _B(pAttribute->ToString()));
#endif
			lRet = true;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
//
//	Skip marshalling attributes for wrappers
//

bool FixupAssembly::FixupWrapperMarshal (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, ParameterInfo^ pSourceParameter, ParameterBuilder^ pTargetParameter, MarshalAsAttribute^ pCustomAttribute)
{
	if	(
			(CreateClassWrappers())
		&&	(IsCoClassWrapper (pSourceMethod->DeclaringType))
		)
	{
#ifdef	_LOG_WRAPPERS
		LogMessage (_LOG_WRAPPERS, _T("===> Parameter  [%s.%s.%s] skip [%s]"), _BMT(pSourceMethod), _BM(pSourceMethod), _BM(pSourceParameter), _B(pCustomAttribute->ToString()));
#endif
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
//
//	Change type reference from interface to class
//
void FixupAssembly::InterfaceTypeToClassType (Type^ pSourceType, Type^& pTargetType)
{
	if	(CreateClassWrappers())
	{
		try
		{
			Type^	lClassType;

			if	(
					(lClassType = GetCoClassWrapper (pSourceType))
				&&	(lClassType = mCopy->GetTargetType (lClassType, false))
				)
			{
#ifdef	_LOG_WRAPPERS
				LogMessage (_LOG_WRAPPERS, _T("===> Type       [%s] as [%s]"), _BT(pSourceType), _BT(lClassType));
#endif
				pTargetType = lClassType;
			}
		}
		catch AnyExceptionSilent
	}
}

void FixupAssembly::InterfaceTypeToClassType (MethodInfo^ pSourceMethod, Type^& pReturnType)
{
	if	(CreateClassWrappers())
	{
		try
		{
			if	(IsCoClassWrapper (pSourceMethod->DeclaringType))
			{
				Type^	lClassType;

				if	(
						(lClassType = GetCoClassWrapper (pSourceMethod->ReturnType))
					&&	(lClassType = mCopy->GetTargetType (lClassType, false))
					)
				{
#ifdef	_LOG_WRAPPERS
					LogMessage (_LOG_WRAPPERS, _T("===> Return     [%s] as [%s] for [%s.%s]"), _BT(pSourceMethod->ReturnType), _BT(lClassType), _BMT(pSourceMethod), _BM(pSourceMethod));
#endif
					pReturnType = lClassType;
				}
			}
			else
			if	(pSourceMethod->DeclaringType->IsSubclassOf (System::Windows::Forms::AxHost::typeid))
			{
				CoClassAttribute^	lCoClass;
				Type^				lClassType;

				if	(
						(lCoClass = mCopy->GetCoClass (pSourceMethod->ReturnType))
					&&	(lClassType = mCopy->GetTargetType (lCoClass->CoClass, false))
					)
				{
#ifdef	_LOG_WRAPPERS
					LogMessage (_LOG_WRAPPERS, _T("===> Return     [%s] as [%s] for [%s.%s]"), _BT(pSourceMethod->ReturnType), _BT(lClassType), _BMT(pSourceMethod), _BM(pSourceMethod));
#endif
					pReturnType = lClassType;
				}
			}
		}
		catch AnyExceptionSilent
	}
}

void FixupAssembly::InterfaceTypeToClassType (MethodBase^ pSourceMethod, ParameterInfo^ pSourceParameter, Type^& pParameterType)
{
	if	(CreateClassWrappers())
	{
		try
		{
			if	(IsCoClassWrapper (pSourceMethod->DeclaringType))
			{
				Type^	lClassType;

				if	(
						(lClassType = GetCoClassWrapper (pSourceParameter->ParameterType))
					&&	(lClassType = mCopy->GetTargetType (lClassType, false))
					)
				{
#ifdef	_LOG_WRAPPERS
					LogMessage (_LOG_WRAPPERS, _T("===> Parameter  [%s] as [%s] for [%s] in [%s.%s]"), _BT(pSourceParameter->ParameterType), _BT(lClassType), _BM(pSourceParameter), _BMT(pSourceMethod), _BM(pSourceMethod));
#endif
					pParameterType = lClassType;
				}
			}
			else
			if	(
					(pSourceMethod->DeclaringType->IsSubclassOf (System::Windows::Forms::AxHost::typeid))
				||	(pSourceMethod->DeclaringType->Name->EndsWith ("EventHandler"))
				||	(pSourceMethod->DeclaringType->Name->EndsWith ("Event"))
				)
			{
				CoClassAttribute^	lCoClass;
				Type^				lClassType;

				if	(
						(lCoClass = mCopy->GetCoClass (pSourceParameter->ParameterType))
					&&	(lClassType = mCopy->GetTargetType (lCoClass->CoClass, false))
					)
				{
#ifdef	_LOG_WRAPPERS
					LogMessage (_LOG_WRAPPERS, _T("===> Parameter  [%s] as [%s] for [%s] in [%s.%s]"), _BT(pSourceParameter->ParameterType), _BT(lClassType), _BM(pSourceParameter), _BMT(pSourceMethod), _BM(pSourceMethod));
#endif
					pParameterType = lClassType;
				}
			}
		}
		catch AnyExceptionSilent
	}
}

void FixupAssembly::InterfaceTypeToClassType (PropertyInfo^ pSourceProperty, Type^& pPropertyType)
{
	if	(CreateClassWrappers())
	{
		try
		{
			if	(IsCoClassWrapper (pSourceProperty->DeclaringType))
			{
				Type^	lClassType;

				if	(
						(lClassType = GetCoClassWrapper (pSourceProperty->PropertyType))
					&&	(lClassType = mCopy->GetTargetType (lClassType, false))
					)
				{
#ifdef	_LOG_WRAPPERS
					LogMessage (_LOG_WRAPPERS, _T("===> Property   [%s] as [%s] for [%s.%s]"), _BT(pSourceProperty->PropertyType), _BT(lClassType), _BMT(pSourceProperty), _BM(pSourceProperty));
#endif
					pPropertyType = lClassType;
				}
			}
			else
			if	(pSourceProperty->DeclaringType->IsSubclassOf (System::Windows::Forms::AxHost::typeid))
			{
				CoClassAttribute^	lCoClass;
				Type^				lClassType;

				if	(
						(lCoClass = mCopy->GetCoClass (pSourceProperty->PropertyType))
					&&	(lClassType = mCopy->GetTargetType (lCoClass->CoClass, false))
					)
				{
#ifdef	_LOG_WRAPPERS
					LogMessage (_LOG_WRAPPERS, _T("===> Property   [%s] as [%s] for [%s.%s]"), _BT(pSourceProperty->PropertyType), _BT(lClassType), _BMT(pSourceProperty), _BM(pSourceProperty));
#endif
					pPropertyType = lClassType;
				}
			}
		}
		catch AnyExceptionSilent
	}
}

void FixupAssembly::InterfaceTypeToClassType (FieldInfo^ pSourceField, Type^& pFieldType)
{
	if	(CreateClassWrappers())
	{
		try
		{
			if	(pSourceField->DeclaringType->Name->EndsWith ("Event"))
			{
				CoClassAttribute^	lCoClass;
				Type^				lClassType;

				if	(
						(lCoClass = mCopy->GetCoClass (pSourceField->FieldType))
					&&	(lClassType = mCopy->GetTargetType (lCoClass->CoClass, false))
					)
				{
#ifdef	_LOG_WRAPPERS
					LogMessage (_LOG_WRAPPERS, _T("===> Field      [%s] as [%s] for [%s.%s]"), _BT(pFieldType), _BT(lClassType), _BMT(pSourceField), _BM(pSourceField));
#endif
					pFieldType = lClassType;
				}
			}
		}
		catch AnyExceptionSilent
	}
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
		LogMessage (_LOG_FIXES, _T("---> Method     [%s.%s] from [%s] to [%s] for [%s]"), _BMT(pSourceMethod), _BM(pSourceMethod), _B(mCopy->MethodAttrsStr(pSourceMethod->Attributes)), _B(mCopy->MethodAttrsStr(pMethodAttributes)), _B(mCopy->TypeLibFuncFlagsStr(lTypeLibFuncFlags)));
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
		LogMessage (_LOG_FIXES, _T("---> Method     [%s.%s] from [%s] to [%s]"), _BMT(pSourceMethod), _BM(pSourceMethod), _B(mCopy->MethodAttrsStr(pSourceMethod->Attributes)), _B(mCopy->MethodAttrsStr(pMethodAttributes)));
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
		LogMessage (_LOG_FIXES, _T("---> Method     [%s.%s] from [%s] to [%s]"), _BMT(pSourceMethod), _BM(pSourceMethod), _B(mCopy->MethodAttrsStr(pSourceMethod->Attributes)), _B(mCopy->MethodAttrsStr(pMethodAttributes)));
#endif
	}
#endif
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
			LogMessage (_LOG_FIXES, _T("---> Method     [%s.%s] as [%s] for override [%s.%s]"), _BMT(pSourceMethod), _BM(pSourceMethod), _B(pMethodName), _BMT(lOverrideMethod), _BM(lOverrideMethod));
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
#if	TRUE
	if	(
			(pParameterName)
		&&	(
				(
					(pParameterName->Length == 1)
				&&	(!Char::IsUpper (pParameterName, 0))
				&&	(pParameterName != "e")
				)
			||	(
					(pParameterName->Length > 1)
				&&	(
						(pParameterName[0] != 'p')
					||	(!Char::IsUpper (pParameterName, 1))
					)
				&&	(!pParameterName->StartsWith ("punk"))
				&&	(!pParameterName->StartsWith ("celt"))
				&&	(!pParameterName->StartsWith ("rgvar"))
				&&	(!pParameterName->StartsWith ("ppenum"))
				)
			)
		)
	{
		String^	lParameterName;

		if	(pParameterName->Length > 1)
		{
			lParameterName = String::Concat (pParameterName->Substring (0,1)->ToUpper(), pParameterName->Substring (1));
		}
		else
		{
			lParameterName = pParameterName->ToUpper();
		}

		if	(String::Compare (lParameterName, pParameterName) != 0)
		{
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("---> Parameter  [%s] to [%s] in [%s.%s]"), _B(pParameterName), _B(lParameterName), _BMT(pSourceMethod), _BM(pSourceMethod));
#endif
			pParameterName = lParameterName;
		}
	}
#endif	
}

/////////////////////////////////////////////////////////////////////////////
//
//	Remove hungarian notation field prefixes and capitalize first letter
//
void FixupAssembly::FixFieldName (FieldInfo^ pSourceField, String^& pFieldName)
{
#if	TRUE
	if	(pFieldName->StartsWith ("m_dw"))
	{
		pFieldName = pFieldName->Substring (4);
#ifdef	_LOG_FIXES
		LogMessage (_LOG_FIXES, _T("---> Field      [%s.%s] as [%s]"), _BMT(pSourceField), _B(pSourceField->Name), _B(pFieldName));
#endif
	}
	else
	if	(pFieldName->StartsWith ("m_a"))
	{
		pFieldName = pFieldName->Substring (3);
#ifdef	_LOG_FIXES
		LogMessage (_LOG_FIXES, _T("---> Field      [%s.%s] as [%s]"), _BMT(pSourceField), _B(pSourceField->Name), _B(pFieldName));
#endif
	}
	else
	if	(pFieldName->StartsWith ("m_"))
	{
		pFieldName = pFieldName->Substring (2);
#ifdef	_LOG_FIXES
		LogMessage (_LOG_FIXES, _T("---> Field      [%s.%s] as [%s]"), _BMT(pSourceField), _B(pSourceField->Name), _B(pFieldName));
#endif
	}

	if	(
			(
				(
					(pFieldName->Length == 1)
				&&	(!Char::IsUpper (pFieldName, 0))
				)
			||	(
					(pFieldName->Length >= 1)
				&&	(
						(pFieldName[0] != 'm')
					||	(!Char::IsUpper (pFieldName, 1))
					)
				)
			)
		)
	{
		String^	lFieldName;

		if	(pFieldName->Length > 1)
		{
			lFieldName = String::Concat (pFieldName->Substring (0,1)->ToUpper(), pFieldName->Substring (1));
		}
		else
		{
			lFieldName = pFieldName->ToUpper();
		}

		if	(String::Compare (lFieldName, pFieldName) != 0)
		{
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("---> Field      [%s.%s] as [%s]"), _BMT(pSourceField), _B(pFieldName), _B(lFieldName));
#endif
			pFieldName = lFieldName;
		}
	}
#endif	
}

/////////////////////////////////////////////////////////////////////////////
//
//	Remove prefix on Enum field names when the prefix matches the Enum name
//
void FixupAssembly::FixEnumFieldName (FieldInfo^ pSourceField, String^& pFieldName, EnumBuilder^ pEnumBuilder)
{
#if	TRUE
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
		LogMessage (_LOG_FIXES, _T("---> Field      [%s.%s] as [%s]"), _BT(pEnumBuilder), _BM(pSourceField), _B(pFieldName));
#endif
	}
#endif	
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
//
//	Add the AssemblyVersion attribute to the target assembly
//
void FixupAssembly::MarkAssemblyVersion (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes)
{
#if	TRUE
	Assembly^			lSourceAssembly = nullptr;
	AssemblyBuilder^	lTargetAssembly = nullptr;

	try
	{
		lSourceAssembly = safe_cast <Assembly^> (pSource);
		lTargetAssembly = safe_cast <AssemblyBuilder^> (pTarget);
	}
	catch AnyExceptionSilent

	if	(
			(lSourceAssembly)
		&&	(lTargetAssembly)
		)
	{
		try
		{
			System::Version^	lAssemblyVersion = lSourceAssembly->GetName()->Version;
			array <Type^>^		lAttrArgTypes = gcnew array <Type^> (1);
			array <Object^>^	lAttrArgValues = gcnew array <Object^> (1);

			lAttrArgTypes[0] = System::String::typeid;
			lAttrArgValues[0] = lAssemblyVersion->ToString();
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("---> Assembly   [%s] Version [%s]"), _B(lTargetAssembly->FullName), _B(lAssemblyVersion->ToString()));
#endif
			pCustomAttributes->Add (gcnew CustomAttributeBuilder (AssemblyVersionAttribute::typeid->GetConstructor(lAttrArgTypes), lAttrArgValues));
		}
		catch AnyExceptionSilent		
	}
#endif	
}

/////////////////////////////////////////////////////////////////////////////
//
//	Add the AllowPartiallyTrustedCallers attribute to the target assembly
//
void FixupAssembly::AllowPartiallyTrustedCallers (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes)
{
#if	TRUE
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
			LogMessage (_LOG_FIXES, _T("---> Assembly   [%s] AllowPartiallyTrustedCallers"), _B(lTargetAssembly->FullName));
#endif
			pCustomAttributes->Add (gcnew CustomAttributeBuilder (Security::AllowPartiallyTrustedCallersAttribute::typeid->GetConstructor(gcnew array <Type^> (0)), gcnew array <Object^> (0)));
		}
		catch AnyExceptionDebug
	}
#endif
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
					LogMessage (_LOG_FIXES, _T("---> Change     [%s] to [%s] in [%s] for [%s]"), _B(pAttributeValues[lValueNdx]->ToString()), _BT(lValueType), _BT(pAttribute->Constructor->DeclaringType), _BT(lTypeBuilder));
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
					LogMessage (_LOG_FIXES, _T("---> Change     [%s] to [%s] in [%s] for [%s]"), _B(pAttributeValues[lValueNdx]->ToString()), _BT(lValueType), _BT(pAttribute->Constructor->DeclaringType), _BT(lTypeBuilder));
#endif
					pAttributeValues[lValueNdx] = lValueType->FullName;
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
//	This logic unhides the GetEnumerator method, but if it's only marked as Restricted and not Hidden in the ODL, that's enough.
//	Right now, it just removes the TypeLibFuncFlags attribute, but it really should modify it to keep Restricted and remove Hidden.
//
bool FixupAssembly::UnhideGetEnumerator (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues)
{
	bool			lRet = false;
#if	TRUE
	MethodBuilder^	lTargetMethod;

	try
	{
		lTargetMethod = safe_cast <MethodBuilder^> (pTarget);
	}
	catch AnyExceptionSilent

	if	(
			(lTargetMethod)
		&&	(System::Runtime::InteropServices::TypeLibFuncAttribute::typeid->IsAssignableFrom (pAttribute->Constructor->DeclaringType))
		&&	(pAttribute->ConstructorArguments->Count > 0)
		&&	(String::Compare (lTargetMethod->Name, "GetEnumerator", true) == 0)
		)
	{
#ifdef	_LOG_FIXES
		int	lTypeLibFuncFlags;
		try
		{
			lTypeLibFuncFlags = (Int16) pAttribute->ConstructorArguments [0].Value;
		}
		catch AnyExceptionSilent
		try
		{
			lTypeLibFuncFlags = (Int32) pAttribute->ConstructorArguments [0].Value;
		}
		catch AnyExceptionSilent
		LogMessage (_LOG_FIXES, _T("---> Skip       [%s] for [%s.%s]"), _B(mCopy->TypeLibFuncFlagsStr((TypeLibFuncFlags)lTypeLibFuncFlags)), _BMT(lTargetMethod), _BM(lTargetMethod));
#endif
		lRet = true;
	}
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
//
//	Remove spurious attributes from MulticastDelegate classes (event handlers)
//
bool FixupAssembly::UnhideDelegate (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues)
{
	bool	lRet = false;
#if	TRUE
	Type^	lSourceType;

	try
	{
		lSourceType = safe_cast <Type^> (pSource);
	}
	catch AnyExceptionSilent

	if	(
			(lSourceType)
		&&	(System::MulticastDelegate::typeid->IsAssignableFrom (lSourceType->BaseType))
		&&	(
				(TypeLibTypeAttribute::typeid->IsAssignableFrom (pAttribute->Constructor->DeclaringType))
			||	(ComVisibleAttribute::typeid->IsAssignableFrom (pAttribute->Constructor->DeclaringType))
			)
		)
	{
#ifdef	_LOG_FIXES
		LogMessage (_LOG_FIXES, _T("---> Skip       [%s] for [%s]"), _B(pAttribute->ToString()), _BT(lSourceType));
#endif
		lRet = true;
	}
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
//
//	Remove GUID attributes from enumerations - they confuse some development tools
//
bool FixupAssembly::RemoveEnumGuid (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues)
{
	bool	lRet = false;
#if	TRUE
	Type^	lSourceType;

	try
	{
		lSourceType = safe_cast <Type^> (pSource);
	}
	catch AnyExceptionSilent

	if	(
			(lSourceType)
		&&	(lSourceType->IsEnum)
		&&	(GuidAttribute::typeid->IsAssignableFrom (pAttribute->Constructor->DeclaringType))
		)
	{
#ifdef	_LOG_FIXES
		LogMessage (_LOG_FIXES, _T("---> Skip       [%s] for [%s]"), _B(pAttribute->ToString()), _BT(lSourceType));
#endif
		lRet = true;
	}
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
//
//	Mark properties as bindable
//
void FixupAssembly::SetPropertyBindable (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues)
{
#if	TRUE
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
		&&	(lSourceProperty->DeclaringType->IsImport)
		&&	(lSourceProperty->DeclaringType->IsClass)
		&&	(BindableAttribute::typeid->IsAssignableFrom (pAttribute->Constructor->DeclaringType))
		&&	(pAttributeValues->Length == 1)
		&&	(mCopy->GetTypeLibFuncFlags (lSourceProperty, lFuncFlags))
		)
	{
		System::Boolean^	lBindable = gcnew System::Boolean (((int)lFuncFlags & (int)TypeLibFuncFlags::FDisplayBind) ? true : false);

		if	(lBindable != pAttributeValues[0])
		{
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("---> Property   [%s.%s] [%s] from [%s] to [%s]"), _BMT(lPropertyBuilder), _BM(lPropertyBuilder), _BT(pAttribute->Constructor->DeclaringType), _B(pAttributeValues[0]->ToString()), _B(lBindable->ToString()));
#endif
			pAttributeValues[0] = lBindable;
		}
	}
#endif	
}

/////////////////////////////////////////////////////////////////////////////

void FixupAssembly::SetPropertyBindable (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes)
{
#if	TRUE
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
		&&	(lSourceProperty->DeclaringType->IsImport)
		&&	(mCopy->GetTypeLibFuncFlags (lSourceProperty, lFuncFlags))
		&&	((int)lFuncFlags & (int)TypeLibFuncFlags::FBindable)
		)
	{
		int	lNdx;

		for	(lNdx = pCustomAttributes->Count-1; lNdx >= 0; lNdx--)
		{
			if	(pCustomAttributes [lNdx]->ToString()->Contains (BindableAttribute::typeid->Name))
			{
				break;
			}
		}

		if	(lNdx < 0)
		{
			array <Type^>^		lAttrArgTypes = gcnew array <Type^> (1);
			array <Object^>^	lAttrArgValues = gcnew array <Object^> (1);

			lAttrArgTypes[0] = System::Boolean::typeid;
			lAttrArgValues[0] = gcnew System::Boolean (((int)lFuncFlags & (int)TypeLibFuncFlags::FDisplayBind) ? true : false);
#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("---> Property   [%s.%s] [%s] [%s]"), _BMT(lPropertyBuilder), _BM(lPropertyBuilder), _BT(BindableAttribute::typeid), _B(lAttrArgValues[0]->ToString()));
#endif
			pCustomAttributes->Add (gcnew CustomAttributeBuilder (BindableAttribute::typeid->GetConstructor(lAttrArgTypes), lAttrArgValues));
		}
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
//
//	Hide restricted (deprecated) CoClass
//
void FixupAssembly::HideRestrictedCoClass (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues)
{
#if	TRUE 
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
		&&	(lSourceType->IsImport)
		&&	(TypeLibTypeAttribute::typeid->IsAssignableFrom (pAttribute->Constructor->DeclaringType))
		)
	{
		try
		{
			Int16	lTypeFlags = Int16::Parse (pAttributeValues[0]->ToString());

			if	(
					(lTypeFlags && (int)TypeLibTypeFlags::FRestricted)
				&&	(!(lTypeFlags && (int)TypeLibTypeFlags::FHidden))
				)
			{
#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("---> Hide       [%s] Restricted"), _BT(lTargetType));
#endif
				lTypeFlags |= (int)TypeLibTypeFlags::FHidden;
				pAttributeValues[0] = gcnew Int16 (lTypeFlags);
			}
		}
		catch AnyExceptionSilent
		{}
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
//
//	Hide interfaces for restricted (deprecated) CoClass
//
void FixupAssembly::HideRestrictedCoClassInterface (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes)
{
#if	TRUE 
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
		&&	(!lSourceType->IsClass)
		&&	(lSourceType->IsImport)
		)
	{
		try
		{
			CoClassAttribute^	lCoClassAttribute;
			array <Type^>^		lAttrArgTypes = gcnew array <Type^> (1);
			array <Object^>^	lAttrArgValues = gcnew array <Object^> (1);

			lCoClassAttribute = safe_cast <CoClassAttribute^> (Attribute::GetCustomAttribute (lSourceType, CoClassAttribute::typeid));
			if	(lCoClassAttribute)
			{
				if	((int)mCopy->GetTypeLibTypeFlags (lCoClassAttribute->CoClass) & (int)TypeLibTypeFlags::FRestricted)
				{
#ifdef	_LOG_FIXES
					LogMessage (_LOG_FIXES, _T("---> Hide       [%s] for Restricted [%s]"), _BT(lTargetType), _BT(lCoClassAttribute->CoClass));
#endif
					lAttrArgTypes [0] = Int16::typeid;
					lAttrArgValues [0] = (Int16) ((int)TypeLibTypeFlags::FHidden | (int)TypeLibTypeFlags::FRestricted);
					pCustomAttributes->Add (gcnew CustomAttributeBuilder (TypeLibTypeAttribute::typeid->GetConstructor(lAttrArgTypes), lAttrArgValues));
				}
			}
		}
		catch AnyExceptionSilent
		{}
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
//
//	Hide the TLBIMP classes for .NET 4.0 or higher
//
void FixupAssembly::HideInternalClass (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues)
{
#if	TRUE 
	if	(!CreateClassWrappers())
	{
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
			&&	(lSourceType->IsImport)
			&&	(TypeLibTypeAttribute::typeid->IsAssignableFrom (pAttribute->Constructor->DeclaringType))
			)
		{
			try
			{
				Int16	lTypeFlags = Int16::Parse (pAttributeValues[0]->ToString());

				if	(!(lTypeFlags && (int)TypeLibTypeFlags::FHidden))
				{
#ifdef	_LOG_FIXES
					LogMessage (_LOG_FIXES, _T("---> Hide       [%s] Framework 4.0"), _BT(lTargetType));
#endif
					lTypeFlags |= (int)TypeLibTypeFlags::FHidden;
					pAttributeValues[0] = gcnew Int16 (lTypeFlags);
				}
			}
			catch AnyExceptionSilent
			{}
		}
	}
#endif	
}

void FixupAssembly::HideInternalClass (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes)
{
#if	TRUE
	if	(!CreateClassWrappers())
	{
		Type^				lSourceType = nullptr;
		TypeBuilder^		lTargetType = nullptr;
		TypeLibTypeFlags	lTypeFlags;

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
			&&	(lSourceType->IsImport)
			&&	(!mCopy->GetTypeLibTypeFlags (lSourceType, lTypeFlags))
			)
		{
			try
			{
				array <Type^>^		lAttrArgTypes = gcnew array <Type^> (1);
				array <Object^>^	lAttrArgValues = gcnew array <Object^> (1);

#ifdef	_LOG_FIXES
				LogMessage (_LOG_FIXES, _T("---> Hide       [%s] Framework 4.0"), _BT(lTargetType));
#endif
				lAttrArgTypes [0] = Int16::typeid;
				lAttrArgValues [0] = (Int16) ((int)TypeLibTypeFlags::FHidden | (int)TypeLibTypeFlags::FRestricted);
				pCustomAttributes->Add (gcnew CustomAttributeBuilder (TypeLibTypeAttribute::typeid->GetConstructor(lAttrArgTypes), lAttrArgValues));
			}
			catch AnyExceptionSilent
			{}
		}
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
			array <Type^>^		lAttrArgTypes = gcnew array <Type^> (0);
			array <Object^>^	lAttrArgValues = gcnew array <Object^> (0);

#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("---> NonUser    [%s]"), _BT(lTargetType));
#endif
			pCustomAttributes->Add (gcnew CustomAttributeBuilder (System::Diagnostics::DebuggerNonUserCodeAttribute::typeid->GetConstructor(lAttrArgTypes), lAttrArgValues));
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
			array <Type^>^		lAttrArgTypes = gcnew array <Type^> (1);
			array <Object^>^	lAttrArgValues = gcnew array <Object^> (1);

#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("---> NoDebug    [%s.%s]"), _BMT(lTargetMethod), _BM(lTargetMethod));
#endif
			lAttrArgTypes [0] = System::Diagnostics::DebuggerBrowsableState::typeid;
			lAttrArgValues [0] = System::Diagnostics::DebuggerBrowsableState::Never;

			pCustomAttributes->Add (gcnew CustomAttributeBuilder (System::Diagnostics::DebuggerBrowsableAttribute::typeid->GetConstructor(lAttrArgTypes), lAttrArgValues));
			pCustomAttributes->Add (gcnew CustomAttributeBuilder (System::Diagnostics::DebuggerHiddenAttribute::typeid->GetConstructor(gcnew array <Type^> (0)), gcnew array <Object^> (0)));
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
			array <Type^>^		lAttrArgTypes = gcnew array <Type^> (1);
			array <Object^>^	lAttrArgValues = gcnew array <Object^> (1);

#ifdef	_LOG_FIXES
			LogMessage (_LOG_FIXES, _T("---> NoDebug    [%s.%s]"), _BMT(lTargetProperty), _BM(lTargetProperty));
#endif
			lAttrArgTypes [0] = System::Diagnostics::DebuggerBrowsableState::typeid;
			lAttrArgValues [0] = System::Diagnostics::DebuggerBrowsableState::Never;

			pCustomAttributes->Add (gcnew CustomAttributeBuilder (System::Diagnostics::DebuggerBrowsableAttribute::typeid->GetConstructor(lAttrArgTypes), lAttrArgValues));
			pCustomAttributes->Add (gcnew CustomAttributeBuilder (System::Diagnostics::DebuggerHiddenAttribute::typeid->GetConstructor(gcnew array <Type^> (0)), gcnew array <Object^> (0)));
		}
		catch AnyExceptionSilent
		{}
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
