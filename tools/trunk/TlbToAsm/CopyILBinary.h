#pragma once
#include "LogILBinary.h"

namespace DoubleAgent {
namespace TlbToAsm {
/////////////////////////////////////////////////////////////////////////////

interface class TranslateILBinary
{
	virtual bool TranslateType (System::Type^ pSourceType, System::Type^& pTargetType) = 0;
	virtual bool TranslateMethod (System::Reflection::MethodBase^ pSourceMethod, System::Reflection::MethodInfo^& pTargetMethod) = 0;
	virtual bool TranslateConstructor (System::Reflection::MethodBase^ pSourceConstructor, System::Reflection::ConstructorInfo^& pTargetConstructor) = 0;
	virtual bool TranslateField (System::Reflection::FieldInfo^ pSourceField, System::Reflection::FieldInfo^& pTargetField) = 0;
};

/////////////////////////////////////////////////////////////////////////////

ref class CopyILBinary : public LogILBinary
{
public:
	CopyILBinary (System::Reflection::Module^ pSourceModule, System::Reflection::Emit::ModuleBuilder^ pTargetModule, TranslateILBinary^ pTranslator);
	~CopyILBinary () {}

public:
	System::Reflection::MethodBody^ CopyMethodBody (System::Reflection::MethodBase^ pSourceMethod, System::Reflection::Emit::ILGenerator^ pGenerator);

protected:
	ref struct MethodCopyData : public MethodParseData
	{
		System::Collections::Generic::Dictionary <int, System::Reflection::Emit::Label>^	mLabelsAt;
		System::Reflection::Emit::ILGenerator^												mGenerator;
	};

	virtual void DefineBodyLabel (Object^ pData, int pLabelTarget) override;
	virtual void PutLocalVariables (Object^ pData) override;
	virtual bool PutBodyException (Object^ pData, System::Reflection::Emit::OpCode & pOpCode, LPBYTE pOperand, int pOffset, LPBYTE pBinary) override;
	virtual bool PutBodyOpCode (Object^ pData, System::Reflection::Emit::OpCode & pOpCode, LPBYTE pOperand, int pOffset, LPBYTE pBinary) override;

	void LogOpCode (System::Reflection::Emit::OpCode & pOpCode, LPBYTE pOperand, int pOffset, LPBYTE pBinary);

protected:
	virtual System::Type^ GetTokenType (DWORD pToken) override;
	System::Reflection::MethodInfo^ GetTokenMethodInfo (DWORD pToken);
	System::Reflection::ConstructorInfo^ GetTokenConstructor (DWORD pToken);
	virtual System::Reflection::FieldInfo^ GetTokenField (DWORD pToken) override;

protected:
	System::Reflection::Emit::ModuleBuilder^	mTargetModule;
	TranslateILBinary^							mTranslator;
};

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
