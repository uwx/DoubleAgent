#pragma once
#include "CopyAssembly.h"

namespace DoubleAgent {
namespace TlbToAsm {
/////////////////////////////////////////////////////////////////////////////

ref class FixupAssembly : public CopyAssembly
{
public:
	FixupAssembly () {}
	~FixupAssembly () {}

protected:
	virtual bool FixupType (Type^ pSourceType, String^& pTypeName, TypeAttributes & pTypeAttributes) override;
	virtual bool FixupMethod (MethodInfo^ pSourceMethod, MethodAttributes & pMethodAttributes) override;
	virtual bool FixupReturnType (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, Type^& pReturnType) override;
	virtual bool FixupParameter (MethodInfo^ pSourceMethod, MethodBuilder^ pTargetMethod, ParameterInfo^ pSourceParameter, Type^& pParameterType) override;
	virtual bool FixupProperty (PropertyInfo^ pSourceProperty, Type^& pPropertyType) override;
	virtual bool FixupCustomAttribute (Object^ pTarget, CustomAttributeData^ pAttribute, array<Object^>^ pAttributeValues) override;
	virtual void FixupCustomAttributes (Object^ pSource, Object^ pTarget, List<CustomAttributeBuilder^>^ pCustomAttributes) override;

	static TypeLibTypeFlags GetTypeLibTypeFlags (Type^ pType);
	TypeLibTypeFlags GetTypeLibTypeFlags (String^ pTypeName);
};

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
