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

public:
	virtual bool FixupType (Type^ pSourceType, String^& pTypeName, TypeAttributes & pTypeAttributes) override;
	virtual bool FixupMethod (MethodInfo^ pSourceMethod, String^& pMethodName, MethodAttributes & pMethodAttributes) override;
	virtual bool FixupReturnType (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, Type^& pReturnType) override;
	virtual bool FixupParameter (MethodBase^ pSourceMethod, ParameterInfo^ pSourceParameter, String^& pParameterName, ParameterAttributes & pParameterAttributes) override;
	virtual bool FixupParameter (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, ParameterInfo^ pSourceParameter, Type^& pParameterType) override;
	virtual bool FixupProperty (PropertyInfo^ pSourceProperty, String^& pPropertyName, Reflection::PropertyAttributes & pPropertyAttributes, Type^& pPropertyType) override;
	virtual bool FixupField (FieldInfo^ pSourceField, String^& pFieldName, FieldAttributes & pFieldAttributes) override;
	virtual bool FixupField (FieldInfo^ pSourceField, String^& pFieldName, EnumBuilder^ pEnumBuilder) override;
	virtual bool FixupEvent (EventInfo^ pSourceEvent, String^& pEventName, EventAttributes & pEventAttributes) override;
	virtual bool FixupCustomAttribute (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues) override;
	virtual void FixupCustomAttributes (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) override;

protected:
	String^ RenameClass (Type^ pSourceType, String^ pTypeName);
	String^ RenameControlClass (Type^ pSourceType, String^ pTypeName);
	String^ RenameServerClass (Type^ pSourceType, String^ pTypeName);
	String^ RenameInterface (Type^ pSourceType, String^ pTypeName);
	String^ RenameControlInterface (Type^ pSourceType, String^ pTypeName);
	String^ RenameServerInterface (Type^ pSourceType, String^ pTypeName);
	String^ RenameNativeInterface (Type^ pSourceType, String^ pTypeName);

	void InterfaceTypeToClassType (MethodInfo^ pSourceMethod, Type^& pReturnType);
	void InterfaceTypeToClassType (MethodInfo^ pSourceMethod, ParameterInfo^ pSourceParameter, Type^& pParameterType);
	void InterfaceTypeToClassType (PropertyInfo^ pSourceProperty, Type^& pPropertyType);

	bool SkipObsoleteProperty (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes);
	bool SkipObsoleteProperty (PropertyInfo^ pSourceProperty, Reflection::PropertyAttributes & pPropertyAttributes);
	bool SkipObsoleteEvent (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes);
	bool SkipObsoleteEvent (EventInfo^ pSourceEvent, EventAttributes & pEventAttributes);

	void HideNonBrowsableMethod (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes);
	void SealAccessorOverride (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes);
	void ProtectHiddenMethod (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes);

	void FixMethodName (MethodBase^ pSourceMethod, String^& pMethodName);
	void FixParameterName (MethodBase^ pSourceMethod, ParameterInfo^ pSourceParameter, String^& pParameterName);
	void FixFieldName (FieldInfo^ pSourceField, String^& pFieldName);
	void FixEnumFieldName (FieldInfo^ pSourceField, String^& pFieldName, EnumBuilder^ pEnumBuilder);
	void FixPropertyName (PropertyInfo^ pSourceProperty, String^& pPropertyName);
	void FixEventName (EventInfo^ pSourceEvent, String^& pEventName);

	void AllowPartiallyTrustedCallers (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes);
	void RenameAttributeTypes (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues);
	bool UnhideTypeWrapper (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues);
	bool UnhideGetEnumerator (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues);

	void SetActiveXPropertyVisibility (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes);
	void SetActiveXPropertyCategory (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes);
	void SetActiveXEventCategory (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes);

	bool RemoveInterfaceCoClass (Object^ pSource, Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues);
	void HideRestrictedCoClassInterface (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes);
	void HideInternalClass (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes);
	bool HideNonCreatableCoClass (Type^ pSourceType, TypeAttributes & pTypeAttributes);

	void SetDebuggerNonUserType (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes);
	void SetDebuggerHiddenMethod (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes);
	void SetDebuggerHiddenProperty (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes);
};

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
