#pragma once
#include "CopyAssembly.h"

#ifndef _LOG_FIXES
#define	_LOG_FIXES	LogNormal
#endif

namespace DoubleAgent {
namespace TlbToAsm {
/////////////////////////////////////////////////////////////////////////////

ref class FixupAssembly : public CopyFixups
{
public:
	FixupAssembly (CopyAssembly^ pCopy) : CopyFixups (pCopy) {}
	~FixupAssembly () {}

// Attributes
public:

// Operations
public:
	virtual bool FixupType (Type^ pSourceType, String^& pTypeName, TypeAttributes & pTypeAttributes) override;
	virtual bool FixupTypeArgument (Type^ pSourceType, Type^& pTargetType) override;
	virtual bool FixupTypeTarget (Type^ pSourceType, TypeBuilder^ pTargetType) override;
	virtual bool FixupMethod (MethodInfo^ pSourceMethod, String^& pMethodName, MethodAttributes & pMethodAttributes) override;
	virtual bool FixupReturnType (MethodInfo^ pSourceMethod, Type^& pReturnType) override;
	virtual bool FixupParameter (MethodBase^ pSourceMethod, ParameterInfo^ pSourceParameter, String^& pParameterName, ParameterAttributes & pParameterAttributes) override;
	virtual bool FixupParameter (MethodBase^ pSourceMethod, ParameterInfo^ pSourceParameter, Type^& pParameterType) override;
	virtual bool FixupProperty (PropertyInfo^ pSourceProperty, String^& pPropertyName, Reflection::PropertyAttributes & pPropertyAttributes, Type^& pPropertyType) override;
	virtual bool FixupField (FieldInfo^ pSourceField, String^& pFieldName, FieldAttributes & pFieldAttributes) override;
	virtual bool FixupField (FieldInfo^ pSourceField, String^& pFieldName, EnumBuilder^ pEnumBuilder) override;
	virtual bool FixupEvent (EventInfo^ pSourceEvent, String^& pEventName, EventAttributes & pEventAttributes) override;
	virtual bool FixupCustomAttribute (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues) override;
	virtual void FixupCustomAttributes (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) override;

// Implemetation
protected:
	virtual String^ RenameClass (Type^ pSourceType, String^ pTypeName) {return nullptr;}
	virtual String^ RenameControlClass (Type^ pSourceType, String^ pTypeName) {return nullptr;}
	virtual String^ RenameServerClass (Type^ pSourceType, String^ pTypeName) {return nullptr;}
	virtual String^ RenameInterface (Type^ pSourceType, String^ pTypeName) {return nullptr;}
	virtual String^ RenameControlInterface (Type^ pSourceType, String^ pTypeName) {return nullptr;}
	virtual String^ RenameServerInterface (Type^ pSourceType, String^ pTypeName) {return nullptr;}
	virtual String^ RenameNativeInterface (Type^ pSourceType, String^ pTypeName) {return nullptr;}

	virtual void InterfaceTypeToClassType (Type^ pSourceType, Type^& pTargetType);
	virtual void InterfaceTypeToClassType (MethodInfo^ pSourceMethod, Type^& pReturnType);
	virtual void InterfaceTypeToClassType (MethodBase^ pSourceMethod, ParameterInfo^ pSourceParameter, Type^& pParameterType);
	virtual void InterfaceTypeToClassType (PropertyInfo^ pSourceProperty, Type^& pPropertyType);

	virtual bool SkipObsoleteProperty (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes) {return false;}
	virtual bool SkipObsoleteProperty (PropertyInfo^ pSourceProperty, Reflection::PropertyAttributes & pPropertyAttributes) {return false;}
	virtual bool SkipObsoleteEvent (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes) {return false;}
	virtual bool SkipObsoleteEvent (EventInfo^ pSourceEvent, EventAttributes & pEventAttributes) {return false;}

	virtual void HideNonBrowsableMethod (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes);
	virtual void SealAccessorOverride (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes);
	virtual void ProtectHiddenMethod (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes);

	virtual void FixMethodOverride (MethodBase^ pSourceMethod, String^& pMethodName);
	virtual void FixMethodName (MethodBase^ pSourceMethod, String^& pMethodName) {}
	virtual void FixParameterName (MethodBase^ pSourceMethod, ParameterInfo^ pSourceParameter, String^& pParameterName);
	virtual void FixFieldName (FieldInfo^ pSourceField, String^& pFieldName);
	virtual void FixEnumFieldName (FieldInfo^ pSourceField, String^& pFieldName, EnumBuilder^ pEnumBuilder);
	virtual void FixPropertyName (PropertyInfo^ pSourceProperty, String^& pPropertyName) {}
	virtual void FixEventName (EventInfo^ pSourceEvent, String^& pEventName) {}
	virtual bool FixEnumerableTarget (Type^ pSourceType, TypeBuilder^ pTargetType) {return false;}

	virtual void AllowPartiallyTrustedCallers (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes);
	virtual void RenameAttributeTypes (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues);
	virtual bool UnhideTypeWrapper (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues);
	virtual bool UnhideGetEnumerator (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues);
	virtual void SetPropertyBindable (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues);
	virtual void SetPropertyBindable (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes);

	virtual void SetActiveXPropertyVisibility (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) {}
	virtual void SetActiveXPropertyCategory (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) {}
	virtual void SetActiveXEventCategory (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) {}

	virtual bool RemoveInterfaceCoClass (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues);
	virtual void HideRestrictedCoClassInterface (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes);
	virtual void HideInternalClass (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) {}
	virtual bool HideNonCreatableCoClass (Type^ pSourceType, TypeAttributes & pTypeAttributes);

	virtual void SetDebuggerNonUserType (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes);
	virtual void SetDebuggerHiddenMethod (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes);
	virtual void SetDebuggerHiddenProperty (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes);
};

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
