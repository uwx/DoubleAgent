#pragma once
#include "CopyAssembly.h"

namespace DoubleAgent {
/////////////////////////////////////////////////////////////////////////////

ref class FixupAssembly : public CopyAssembly
{
public:
	FixupAssembly () {}
	~FixupAssembly () {}

protected:
	virtual bool FixupType (Type^ pSourceType, TypeAttributes & pTypeAttributes) override;
	virtual bool FixupEnum (Type^ pSourceType, TypeAttributes & pTypeAttributes) override;
	virtual bool FixupMethod (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes) override;
	virtual bool FixupProperty (PropertyInfo^ pSourceProperty, Reflection::PropertyAttributes & pPropertyAttributes) override;
	virtual bool FixupEvent (EventInfo^ pSourceEvent,EventAttributes & pEventAttributes) override;
	virtual bool FixupCustomAttribute (Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues) override;
	virtual void FixupCustomAttributes (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) override;

	static TypeLibTypeFlags GetTypeLibTypeFlags (Type^ pType);
	TypeLibTypeFlags GetTypeLibTypeFlags (String^ pTypeName);
};

/////////////////////////////////////////////////////////////////////////////
};