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
	CopyAssembly^	mInternalCopy;

// Overrides
public:
	virtual void PreCopyTypes () override;

protected:
	virtual String^ RenameClass (Type^ pSourceType, String^ pTypeName) override;
	virtual String^ RenameControlClass (Type^ pSourceType, String^ pTypeName) override;
	virtual String^ RenameServerClass (Type^ pSourceType, String^ pTypeName) override;
	virtual String^ RenameInterface (Type^ pSourceType, String^ pTypeName) override;
	virtual String^ RenameControlInterface (Type^ pSourceType, String^ pTypeName) override;
	virtual String^ RenameServerInterface (Type^ pSourceType, String^ pTypeName) override;
	virtual String^ RenameNativeInterface (Type^ pSourceType, String^ pTypeName) override;

	virtual bool SkipObsoleteProperty (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes) override;
	virtual bool SkipObsoleteProperty (PropertyInfo^ pSourceProperty, Reflection::PropertyAttributes & pPropertyAttributes) override;
	virtual bool SkipObsoleteEvent (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes) override;
	virtual bool SkipObsoleteEvent (EventInfo^ pSourceEvent, EventAttributes & pEventAttributes) override;

	virtual void FixMethodName (MethodBase^ pSourceMethod, String^& pMethodName) override;
	virtual void FixPropertyName (PropertyInfo^ pSourceProperty, String^& pPropertyName) override;
	virtual void FixEventName (EventInfo^ pSourceEvent, String^& pEventName) override;
	virtual bool FixEnumerableTarget (Type^ pSourceType, TypeBuilder^ pTargetType) override;

	virtual void SetActiveXPropertyVisibility (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) override;
	virtual void SetActiveXPropertyCategory (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) override;
	virtual void SetActiveXEventCategory (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) override;

	virtual void HideInternalClass (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) override;

protected:
	Type^ GetEnumeratorType (Type^ pSourceType);
	Type^ AddInternalType (Type^ pType);
	Type^ AddInternalType (Type^ pType, Type^ pGenericArgument);

protected:
	Dictionary <Type^, Type^>^	mEnumerableTypes;
	Dictionary <Type^, Type^>^	mEnumeratorTypes;
};

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
