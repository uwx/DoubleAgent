#include "stdafx.h"
#include "FixupAssembly.h"

namespace DoubleAgent {
namespace TlbToAsm {
/////////////////////////////////////////////////////////////////////////////

bool FixupAssembly::FixupType (Type^ pSourceType, String^& pTypeName, Type^& pBaseType, TypeAttributes & pTypeAttributes)
{
	bool	lRet = CopyFixups::FixupType (pSourceType, pTypeName, pBaseType, pTypeAttributes);

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

	if	(!lRet)
	{
		lRet = FixupWrapperType (pSourceType, pTypeName, pBaseType, pTypeAttributes);
	}

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool FixupAssembly::FixupTypeArgument (Type^ pSourceType, Type^& pTargetType)
{
	bool	lRet = CopyFixups::FixupTypeArgument (pSourceType, pTargetType);

	if	(!ReferenceEquals (pSourceType, pTargetType))
	{
		InterfaceTypeToClassType (pSourceType, pTargetType);
	}
	return lRet;
}

bool FixupAssembly::FixupInterface (Type^ pSourceType, Type^ pSourceInterface, TypeBuilder^ pTargetType, Type^& pTargetInterface)
{
	bool	lRet = CopyFixups::FixupInterface (pSourceType, pSourceInterface, pTargetType, pTargetInterface);

	if	(!lRet)
	{
		lRet = FixupWrapperInterface (pSourceType, pSourceInterface, pTargetType, pTargetInterface);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool FixupAssembly::FixupConstructor (ConstructorInfo^ pSourceConstructor, MethodAttributes & pConstructorAttributes)
{
	bool	lRet = CopyFixups::FixupConstructor (pSourceConstructor, pConstructorAttributes);

	if	(!lRet)
	{
		lRet = FixupWrapperConstructor (pSourceConstructor, pConstructorAttributes);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool FixupAssembly::FixupMethod (MethodInfo^ pSourceMethod, String^& pMethodName, MethodAttributes & pMethodAttributes)
{
	bool	lRet = CopyFixups::FixupMethod (pSourceMethod, pMethodName, pMethodAttributes);

	if	(
			(!lRet)
		&&	(SkipGeneratedMethod (pSourceMethod, pMethodAttributes))
		)
	{
		lRet = true;
	}
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
		FixMethodOverride (pSourceMethod, pMethodName);
		FixMethodName (pSourceMethod, pMethodName);
		FixupWrapperMethod (pSourceMethod, pMethodName, pMethodAttributes);
		HideNonBrowsableMethod (pSourceMethod, pMethodAttributes);
		SealAccessorOverride (pSourceMethod, pMethodAttributes);
		ProtectHiddenMethod (pSourceMethod, pMethodAttributes);
	}
	return lRet;
}

bool FixupAssembly::FixupMethodImpl (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, MethodImplAttributes & pMethodImplAttributes)
{
	bool	lRet = CopyFixups::FixupMethodImpl (pSourceMethod, pTargetMethod, pMethodImplAttributes);

	if	(!lRet)
	{
		FixupWrapperMethod (pSourceMethod, pTargetMethod, pMethodImplAttributes);
	}
	return lRet;
}

bool FixupAssembly::FixupReturnType (MethodInfo^ pSourceMethod, Type^& pReturnType)
{
	bool	lRet = CopyFixups::FixupReturnType (pSourceMethod, pReturnType);

	if	(!ReferenceEquals (pReturnType, pSourceMethod->ReturnType))
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

bool FixupAssembly::FixupParameter (MethodBase^ pSourceMethod, ParameterInfo^ pSourceParameter, Type^& pParameterType)
{
	bool	lRet = CopyFixups::FixupParameter (pSourceMethod, pSourceParameter, pParameterType);

	if	(!ReferenceEquals (pParameterType, pSourceParameter->ParameterType))
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
		&&	(!ReferenceEquals (pPropertyType, pSourceProperty->PropertyType))
		)
	{
		InterfaceTypeToClassType (pSourceProperty, pPropertyType);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool FixupAssembly::FixupField (FieldInfo^ pSourceField, String^& pFieldName, FieldAttributes & pFieldAttributes, Type^& pFieldType)
{
	bool	lRet = CopyFixups::FixupField (pSourceField, pFieldName, pFieldAttributes, pFieldType);

	if	(!lRet)
	{
		FixFieldName (pSourceField, pFieldName);
		FixEventFieldName (pSourceField, pFieldName);
	}
	if	(
			(!lRet)
		&&	(!ReferenceEquals (pFieldType, pSourceField->FieldType))
		)
	{
		InterfaceTypeToClassType (pSourceField, pFieldType);
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
		&&	(
				(TypeBuilder::typeid->IsInstanceOfType (pTarget))
			||	(MethodBuilder::typeid->IsInstanceOfType (pTarget))
			||	(PropertyBuilder::typeid->IsInstanceOfType (pTarget))
			||	(AssemblyBuilder::typeid->IsInstanceOfType (pTarget))
			)
		)
	{
		lRet = FixupWrapperAttribute (pSource, pTarget, pAttribute, pAttributeValues);
	}

	if	(
			(!lRet)
		&&	(TypeBuilder::typeid->IsInstanceOfType (pTarget))
		)
	{
		HideRestrictedCoClass (pSource, pTarget, pAttribute, pAttributeValues);
		HideInternalClass (pSource, pTarget, pAttribute, pAttributeValues);
		lRet = UnhideDelegate (pSource, pTarget, pAttribute, pAttributeValues);
	}

	if	(
			(!lRet)
		&&	(EnumBuilder::typeid->IsInstanceOfType (pTarget))
		)
	{
		lRet = RemoveEnumGuid (pSource, pTarget, pAttribute, pAttributeValues);
	}

	if	(
			(!lRet)
		&&	(MethodBuilder::typeid->IsInstanceOfType (pTarget))
		)
	{
		lRet = UnhideGetEnumerator (pSource, pTarget, pAttribute, pAttributeValues);
	}

	if	(
			(!lRet)
		&&	(TypeBuilder::typeid->IsInstanceOfType (pTarget))
		)
	{
		RenameAttributeTypes (pSource, pTarget, pAttribute, pAttributeValues);
	}

	if	(
			(!lRet)
		&&	(PropertyBuilder::typeid->IsInstanceOfType (pTarget))
		)
	{
		SetPropertyBindable (pSource, pTarget, pAttribute, pAttributeValues);
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
		HideInternalClass (pSource, pTarget, pCustomAttributes);
	}

	if	(TypeBuilder::typeid->IsInstanceOfType (pTarget))
	{
		SetActiveXControlAttributes (pSource, pTarget, pCustomAttributes);
	}

	if	(EnumBuilder::typeid->IsInstanceOfType (pTarget))
	{
		SetActiveXEnumAttributes (pSource, pTarget, pCustomAttributes);
	}

	if	(MethodBuilder::typeid->IsInstanceOfType (pTarget))
	{
		SetActiveXMethodVisibility (pSource, pTarget, pCustomAttributes);
	}

	if	(PropertyBuilder::typeid->IsInstanceOfType (pTarget))
	{
		SetPropertyBindable (pSource, pTarget, pCustomAttributes);
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
		MarkAssemblyVersion (pSource, pTarget, pCustomAttributes);
		AllowPartiallyTrustedCallers (pSource, pTarget, pCustomAttributes);
	}
}

/////////////////////////////////////////////////////////////////////////////

bool FixupAssembly::FixupMarshalAttribute (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, ParameterInfo^ pSourceParameter, ParameterBuilder^ pTargetParameter, MarshalAsAttribute^ pCustomAttribute, CustomAttributeBuilder^& pAttributeBuilder)
{
	bool lRet = CopyFixups::FixupMarshalAttribute (pSourceMethod, pTargetMethod, pSourceParameter, pTargetParameter, pCustomAttribute, pAttributeBuilder);

	if	(!lRet)
	{
		lRet = FixupWrapperMarshal (pSourceMethod, pTargetMethod, pSourceParameter, pTargetParameter, pCustomAttribute);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
