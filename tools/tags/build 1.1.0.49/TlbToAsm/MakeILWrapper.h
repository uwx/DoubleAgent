#pragma once
#include "CopyILBinary.h"
#include "CopyAssembly.h"

namespace DoubleAgent {
namespace TlbToAsm {
/////////////////////////////////////////////////////////////////////////////

ref class MakeILWrapper : public CopyILBinary
{
public:
	MakeILWrapper (System::Reflection::Module^ pSourceModule, System::Reflection::Emit::ModuleBuilder^ pTargetModule, System::Type^ pWrappedType, CopyAssembly^ pCopy);
	~MakeILWrapper () {}

public:
	bool MakeWrapperMethod (System::Reflection::MethodInfo^ pSourceMethod, System::Reflection::Emit::MethodBuilder^ pTargetMethod, System::Reflection::MethodInfo^ pWrappedMethod);

protected:
	bool GenerateMethod (System::Reflection::MethodInfo^ pSourceMethod, System::Reflection::Emit::MethodBuilder^ pTargetMethod, System::Reflection::MethodInfo^ pWrappedMethod);
	void MakeEnumerator (System::Reflection::MethodInfo^ pSourceMethod, System::Reflection::Emit::MethodBuilder^ pTargetMethod);
	System::Reflection::ConstructorInfo^ MakeEnumeratorClass (System::Type^ pEnumeratedType, System::Type^ pEnumeratedWrapper);

	void MakeBasics (System::Type^ pSourceType, System::Type^ pTargetType);
	void GenerateConstructor (System::Type^ pSourceType, System::Reflection::Emit::TypeBuilder^ pTargetType);
	void GenerateAccessor (System::Reflection::Emit::TypeBuilder^ pTargetType);

	System::Reflection::ConstructorInfo^ GetWrapperConstructor (System::Type^ pWrapperType, System::Type^ pInterfaceType);
	System::Reflection::MethodInfo^ GetWrapperAccessor (System::Type^ pWrapperType, System::Type^ pInterfaceType);

	bool NeedsCoClass (System::Type^ pSourceType);
	System::Reflection::ConstructorInfo^ MakeCoClass (System::Type^ pSourceType, System::Type^ pTargetType);

protected:
	virtual bool PutBodyOpCode (Object^ pData, System::Reflection::Emit::OpCode & pOpCode, LPBYTE pOperand, int pOffset, LPBYTE pBinary) override;

	bool WrapperToInterface (System::Reflection::MethodInfo^ pMethod, int pParameterNdx, bool pIsEvent, System::Reflection::Emit::ILGenerator^ pGenerator, System::Reflection::Emit::OpCode & pOpCode);
	bool WrapperToInterface (System::Reflection::MethodInfo^ pMethod, System::Reflection::ParameterInfo^ pParameter, bool pIsEvent, System::Reflection::Emit::ILGenerator^ pGenerator, System::Reflection::Emit::OpCode & pOpCode);
	bool InterfaceToWrapper (System::Reflection::MethodInfo^ pMethod, int pParameterNdx, bool pIsEvent, System::Reflection::Emit::ILGenerator^ pGenerator, System::Reflection::Emit::OpCode & pOpCode);
	bool InterfaceToWrapper (System::Reflection::MethodInfo^ pMethod, System::Reflection::ParameterInfo^ pParameter, bool pIsEvent, System::Reflection::Emit::ILGenerator^ pGenerator, System::Reflection::Emit::OpCode & pOpCode);

protected:
	CopyAssembly^							mCopy;
	System::Type^							mInterfaceType;
	System::Reflection::Emit::FieldBuilder^	mInterfaceField;
};

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
