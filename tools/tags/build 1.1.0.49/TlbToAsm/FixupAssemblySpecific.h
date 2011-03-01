#pragma once
#include "FixupAssembly.h"

namespace DoubleAgent {
namespace TlbToAsm {
/////////////////////////////////////////////////////////////////////////////

ref class FixupAssemblySpecific : public FixupAssembly
{
public:
	FixupAssemblySpecific (CopyAssembly^ pCopy) : FixupAssembly (pCopy) {}
	~FixupAssemblySpecific () {}

// Attributes
public:

// Overrides
protected:
	virtual String^ RenameClass (Type^ pSourceType, String^ pTypeName) override;
	virtual String^ RenameControlClass (Type^ pSourceType, String^ pTypeName) override;
	virtual String^ RenameServerClass (Type^ pSourceType, String^ pTypeName) override;
	virtual String^ RenameInterface (Type^ pSourceType, String^ pTypeName) override;
	virtual String^ RenameControlInterface (Type^ pSourceType, String^ pTypeName) override;
	virtual String^ RenameServerInterface (Type^ pSourceType, String^ pTypeName) override;
	virtual String^ RenameNativeInterface (Type^ pSourceType, String^ pTypeName) override;

	virtual bool SkipGeneratedMethod (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes) override;
	virtual bool SkipObsoleteProperty (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes) override;
	virtual bool SkipObsoleteProperty (PropertyInfo^ pSourceProperty, Reflection::PropertyAttributes & pPropertyAttributes) override;
	virtual bool SkipObsoleteEvent (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes) override;
	virtual bool SkipObsoleteEvent (EventInfo^ pSourceEvent, EventAttributes & pEventAttributes) override;

	virtual void FixMethodName (MethodBase^ pSourceMethod, String^& pMethodName) override;
	virtual void FixPropertyName (PropertyInfo^ pSourceProperty, String^& pPropertyName) override;
	virtual void FixEventName (EventInfo^ pSourceEvent, String^& pEventName) override;
	virtual void FixEventFieldName (FieldInfo^ pSourceField, String^& pFieldName) override;

	virtual void SetActiveXControlAttributes (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) override;
	virtual void SetActiveXMethodVisibility (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) override;
	virtual void SetActiveXPropertyVisibility (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) override;
	virtual void SetActiveXPropertyCategory (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) override;
	virtual void SetActiveXEventCategory (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) override;

	virtual void HideInternalClass (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) override;
};

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
