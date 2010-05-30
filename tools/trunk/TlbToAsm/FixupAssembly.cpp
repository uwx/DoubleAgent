#include "stdafx.h"
#include "FixupAssembly.h"

namespace DoubleAgent {
namespace TlbToAsm {
/////////////////////////////////////////////////////////////////////////////

bool FixupAssembly::FixupType (Type^ pSourceType, String^& pTypeName, TypeAttributes & pTypeAttributes)
{
	bool	lRet = CopyFixups::FixupType (pSourceType, pTypeName, pTypeAttributes);

	if	(
			(!lRet)
		&&	(HideNonCreatableCoClass (pSourceType, pTypeAttributes))
		)
	{
		lRet = true;
	}

//
//	Rename types to conform to Namespace naming conventions
//
	if	(!lRet)
	{
		array <String^>^		lTypeNames = pTypeName->Split ('.');
		interior_ptr <String^>	lTypeName = &lTypeNames [lTypeNames->Length-1];
		String^					lNewName = nullptr;

		if	(
				(pSourceType->IsClass)
			&&	(lNewName = RenameClass (pSourceType, *lTypeName))
			)
		{
			(*lTypeName) = lNewName;
		}
		else
		if	(
				(pSourceType->IsInterface)
			&&	(lNewName = RenameInterface (pSourceType, *lTypeName))
			)
		{
			(*lTypeName) = lNewName;
		}

		if	(lNewName)
		{
			pTypeName = String::Join(".", lTypeNames);
		}
	}

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool FixupAssembly::FixupMethod (MethodInfo^ pSourceMethod, String^& pMethodName, MethodAttributes & pMethodAttributes)
{
	bool	lRet = CopyFixups::FixupMethod (pSourceMethod, pMethodName, pMethodAttributes);

	if	(
			(!lRet)
		&&	(SkipObsoleteProperty (pSourceMethod, pMethodAttributes))
		)
	{
		lRet = true;
	}
	if	(
			(!lRet)
		&&	(SkipObsoleteEvent (pSourceMethod, pMethodAttributes))
		)
	{
		lRet = true;
	}
	if	(!lRet)
	{
		FixMethodName (pSourceMethod, pMethodName);
		HideNonBrowsableMethod (pSourceMethod, pMethodAttributes);
		SealAccessorOverride (pSourceMethod, pMethodAttributes);
		ProtectHiddenMethod (pSourceMethod, pMethodAttributes);
	}
	return lRet;
}

bool FixupAssembly::FixupReturnType (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, Type^& pReturnType)
{
	bool	lRet = CopyFixups::FixupReturnType (pSourceMethod, pTargetMethod, pReturnType);

	if	(!Object::ReferenceEquals (pReturnType, pSourceMethod->ReturnType))
	{
		InterfaceTypeToClassType (pSourceMethod, pReturnType);
	}
	return lRet;
}

bool FixupAssembly::FixupParameter (MethodBase^ pSourceMethod, ParameterInfo^ pSourceParameter, String^& pParameterName, ParameterAttributes & pParameterAttributes)
{
	bool	lRet = CopyFixups::FixupParameter (pSourceMethod, pSourceParameter, pParameterName, pParameterAttributes);

	FixParameterName (pSourceMethod, pSourceParameter, pParameterName);
	return lRet;
}

bool FixupAssembly::FixupParameter (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, ParameterInfo^ pSourceParameter, Type^& pParameterType)
{
	bool	lRet = CopyFixups::FixupParameter (pSourceMethod, pTargetMethod, pSourceParameter, pParameterType);

	if	(!Object::ReferenceEquals (pParameterType, pSourceParameter->ParameterType))
	{
		InterfaceTypeToClassType (pSourceMethod, pSourceParameter, pParameterType);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool FixupAssembly::FixupProperty (PropertyInfo^ pSourceProperty, String^& pPropertyName, Reflection::PropertyAttributes & pPropertyAttributes, Type^& pPropertyType)
{
	bool	lRet = CopyFixups::FixupProperty (pSourceProperty, pPropertyName, pPropertyAttributes, pPropertyType);

	if	(
			(!lRet)
		&&	(SkipObsoleteProperty (pSourceProperty, pPropertyAttributes))
		)
	{
		lRet = true;
	}
	if	(!lRet)
	{
		FixPropertyName (pSourceProperty, pPropertyName);
	}
	if	(
			(!lRet)
		&&	(!Object::ReferenceEquals (pPropertyType, pSourceProperty->PropertyType))
		)
	{
		InterfaceTypeToClassType (pSourceProperty, pPropertyType);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool FixupAssembly::FixupField (FieldInfo^ pSourceField, String^& pFieldName, FieldAttributes & pFieldAttributes)
{
	bool	lRet = CopyFixups::FixupField (pSourceField, pFieldName, pFieldAttributes);

	if	(!lRet)
	{
		FixFieldName (pSourceField, pFieldName);
	}
	return lRet;
}

bool FixupAssembly::FixupField (FieldInfo^ pSourceField, String^& pFieldName, EnumBuilder^ pEnumBuilder)
{
	bool	lRet = CopyFixups::FixupField (pSourceField, pFieldName, pEnumBuilder);

	if	(!lRet)
	{
		FixEnumFieldName (pSourceField, pFieldName, pEnumBuilder);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool FixupAssembly::FixupEvent (EventInfo^ pSourceEvent, String^& pEventName, EventAttributes & pEventAttributes)
{
	bool	lRet = CopyFixups::FixupEvent (pSourceEvent, pEventName, pEventAttributes);

	if	(
			(!lRet)
		&&	(SkipObsoleteEvent (pSourceEvent, pEventAttributes))
		)
	{
		lRet = true;
	}
	if	(!lRet)
	{
		FixEventName (pSourceEvent, pEventName);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool FixupAssembly::FixupCustomAttribute (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues)
{
	bool	lRet = CopyFixups::FixupCustomAttribute (pSource, pTarget, pAttribute, pAttributeValues);

	if	(
			(!lRet)
		&&	(TypeBuilder::typeid->IsInstanceOfType (pTarget))
		&&	(UnhideTypeWrapper (pSource, pTarget, pAttribute, pAttributeValues))
		)
	{
		lRet = true;
	}

	if	(
			(!lRet)
		&&	(MethodBuilder::typeid->IsInstanceOfType (pTarget))
		&&	(UnhideGetEnumerator (pSource, pTarget, pAttribute, pAttributeValues))
		)
	{
		lRet = true;
	}

	if	(
			(!lRet)
		&&	(TypeBuilder::typeid->IsInstanceOfType (pTarget))
		&&	(RemoveInterfaceCoClass (pSource, pTarget, pAttribute, pAttributeValues))
		)
	{
		lRet = true;
	}

	if	(
			(!lRet)
		&&	(TypeBuilder::typeid->IsInstanceOfType (pTarget))
		)
	{
		RenameAttributeTypes (pSource, pTarget, pAttribute, pAttributeValues);
	}

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

void FixupAssembly::FixupCustomAttributes (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes)
{
	CopyFixups::FixupCustomAttributes (pSource, pTarget, pCustomAttributes);

	if	(TypeBuilder::typeid->IsInstanceOfType (pTarget))
	{
		HideRestrictedCoClassInterface (pSource, pTarget, pCustomAttributes);
	}

	if	(TypeBuilder::typeid->IsInstanceOfType (pTarget))
	{
		HideInternalClass (pSource, pTarget, pCustomAttributes);
	}

	if	(PropertyBuilder::typeid->IsInstanceOfType (pTarget))
	{
		SetActiveXPropertyVisibility (pSource, pTarget, pCustomAttributes);
	}

	if	(PropertyBuilder::typeid->IsInstanceOfType (pTarget))
	{
		SetActiveXPropertyCategory (pSource, pTarget, pCustomAttributes);
	}

	if	(EventBuilder::typeid->IsInstanceOfType (pTarget))
	{
		SetActiveXEventCategory (pSource, pTarget, pCustomAttributes);
	}

	if	(TypeBuilder::typeid->IsInstanceOfType (pTarget))
	{
		SetDebuggerNonUserType (pSource, pTarget, pCustomAttributes);
	}

	if	(MethodBuilder::typeid->IsInstanceOfType (pTarget))
	{
		SetDebuggerHiddenMethod (pSource, pTarget, pCustomAttributes);
	}

	if	(PropertyBuilder::typeid->IsInstanceOfType (pTarget))
	{
		SetDebuggerHiddenProperty (pSource, pTarget, pCustomAttributes);
	}

	if	(AssemblyBuilder::typeid->IsInstanceOfType (pTarget))
	{
		AllowPartiallyTrustedCallers (pSource, pTarget, pCustomAttributes);
	}
}

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
