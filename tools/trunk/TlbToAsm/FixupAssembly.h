#pragma once
#include "CopyAssembly.h"

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
	virtual bool FixupType (Type^ pSourceType, String^& pTypeName, Type^& pBaseType, TypeAttributes & pTypeAttributes) override;
	virtual bool FixupTypeArgument (Type^ pSourceType, Type^& pTargetType) override;
	virtual bool FixupInterface (Type^ pSourceType, Type^ pSourceInterface, TypeBuilder^ pTargetType, Type^& pTargetInterface) override;
	virtual bool FixupConstructor (ConstructorInfo^ pSourceConstructor, MethodAttributes & pConstructorAttributes) override;
	virtual bool FixupMethod (MethodInfo^ pSourceMethod, String^& pMethodName, MethodAttributes & pMethodAttributes) override;
	virtual bool FixupMethodImpl (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, MethodImplAttributes & pMethodImplAttributes) override;
	virtual bool FixupReturnType (MethodInfo^ pSourceMethod, Type^& pReturnType) override;
	virtual bool FixupParameter (MethodBase^ pSourceMethod, ParameterInfo^ pSourceParameter, String^& pParameterName, ParameterAttributes & pParameterAttributes) override;
	virtual bool FixupParameter (MethodBase^ pSourceMethod, ParameterInfo^ pSourceParameter, Type^& pParameterType) override;
	virtual bool FixupProperty (PropertyInfo^ pSourceProperty, String^& pPropertyName, Reflection::PropertyAttributes & pPropertyAttributes, Type^& pPropertyType) override;
	virtual bool FixupField (FieldInfo^ pSourceField, String^& pFieldName, FieldAttributes & pFieldAttributes, Type^& pFieldType) override;
	virtual bool FixupField (FieldInfo^ pSourceField, String^& pFieldName, EnumBuilder^ pEnumBuilder) override;
	virtual bool FixupEvent (EventInfo^ pSourceEvent, String^& pEventName, EventAttributes & pEventAttributes) override;
	virtual bool FixupCustomAttribute (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues) override;
	virtual void FixupCustomAttributes (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) override;
	virtual bool FixupMarshalAttribute (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, ParameterInfo^ pSourceParameter, ParameterBuilder^ pTargetParameter, MarshalAsAttribute^ pCustomAttribute, CustomAttributeBuilder^& pAttributeBuilder) override;

	virtual bool IsCoClassWrapper (Type^ pSourceType) override;
	virtual Type^ GetCoClassInterface (Type^ pSourceType) override;
	virtual Type^ GetCoClassWrapper (Type^ pSourceInterface) override;

// Implemetation
protected:
	virtual String^ RenameClass (Type^ pSourceType, String^ pTypeName) {return nullptr;}
	virtual String^ RenameControlClass (Type^ pSourceType, String^ pTypeName) {return nullptr;}
	virtual String^ RenameServerClass (Type^ pSourceType, String^ pTypeName) {return nullptr;}
	virtual String^ RenameInterface (Type^ pSourceType, String^ pTypeName) {return nullptr;}
	virtual String^ RenameControlInterface (Type^ pSourceType, String^ pTypeName) {return nullptr;}
	virtual String^ RenameServerInterface (Type^ pSourceType, String^ pTypeName) {return nullptr;}
	virtual String^ RenameNativeInterface (Type^ pSourceType, String^ pTypeName) {return nullptr;}

	virtual bool FixupWrapperType (Type^ pSourceType, String^& pTypeName, Type^& pBaseType, TypeAttributes & pTypeAttributes);
	virtual bool FixupWrapperInterface (Type^ pSourceType, Type^ pSourceInterface, TypeBuilder^ pTargetType, Type^& pTargetInterface);
	virtual bool FixupWrapperConstructor (ConstructorInfo^ pSourceConstructor, MethodAttributes & pConstructorAttributes);
	virtual void FixupWrapperMethod (MethodInfo^ pSourceMethod, String^& pMethodName, MethodAttributes & pMethodAttributes);
	virtual void FixupWrapperMethod (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, MethodImplAttributes & pMethodImplAttributes);
	virtual bool FixupWrapperAttribute (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues);
	virtual bool FixupWrapperMarshal (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, ParameterInfo^ pSourceParameter, ParameterBuilder^ pTargetParameter, MarshalAsAttribute^ pCustomAttribute);

	virtual void InterfaceTypeToClassType (Type^ pSourceType, Type^& pTargetType);
	virtual void InterfaceTypeToClassType (MethodInfo^ pSourceMethod, Type^& pReturnType);
	virtual void InterfaceTypeToClassType (MethodBase^ pSourceMethod, ParameterInfo^ pSourceParameter, Type^& pParameterType);
	virtual void InterfaceTypeToClassType (PropertyInfo^ pSourceProperty, Type^& pPropertyType);
	virtual void InterfaceTypeToClassType (FieldInfo^ pSourceField, Type^& pFieldType);

	virtual bool SkipGeneratedMethod (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes) {return false;}
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
	virtual void FixEventFieldName (FieldInfo^ pSourceField, String^& pFieldName) {}

	virtual void AllowPartiallyTrustedCallers (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes);
	virtual void RenameAttributeTypes (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues);
	virtual bool UnhideGetEnumerator (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues);
	virtual bool UnhideDelegate (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues);
	virtual void SetPropertyBindable (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues);
	virtual void SetPropertyBindable (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes);

	virtual void SetActiveXMethodVisibility (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) {}
	virtual void SetActiveXPropertyVisibility (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) {}
	virtual void SetActiveXPropertyCategory (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) {}
	virtual void SetActiveXEventCategory (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) {}

	virtual void HideRestrictedCoClassInterface (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes);
	virtual void HideInternalClass (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) {}

	virtual void SetDebuggerNonUserType (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes);
	virtual void SetDebuggerHiddenMethod (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes);
	virtual void SetDebuggerHiddenProperty (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes);
};

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
