#pragma once

namespace DoubleAgent {
namespace TlbToAsm {
/////////////////////////////////////////////////////////////////////////////

ref class ParseILBinary
{
public:
	ParseILBinary (System::Reflection::Module^ pSourceModule);
	~ParseILBinary () {}

public:
	void ParseMethodBody (System::Reflection::MethodBase^ pSourceMethod);

protected:
	ref struct MethodParseData
	{
		System::Reflection::MethodBase^																	mMethod;
		System::Reflection::MethodBody^																	mMethodBody;
		array <unsigned char>^																			mBinary;
		System::Collections::Generic::Dictionary <int, System::Reflection::Emit::OpCode>^				mOpCodesAt;
		System::Collections::Generic::Dictionary <int, int>^											mTryStartAt;
		System::Collections::Generic::Dictionary <int, int>^											mTryEndAt;
		System::Collections::Generic::Dictionary <int, System::Reflection::ExceptionHandlingClause^>^	mCatchStartAt;
		System::Collections::Generic::Dictionary <int, System::Reflection::ExceptionHandlingClause^>^	mCatchEndAt;
	};

	virtual bool GetMethodBody (System::Reflection::MethodBase^ pSourceMethod, MethodParseData^ pData);
	virtual System::Reflection::MethodInfo^ GetBodyMethod (Object^ pData);
	virtual System::Reflection::ConstructorInfo^ GetBodyConstructor (Object^ pData);

	virtual void ProcessMethodBody (Object^ pData);

	virtual void ParseBodyBinary (Object^ pData);
	virtual void DefineBodyLabel (Object^ pData, int pLabelTarget);
	virtual void GetExceptionClauses (Object^ pData);

	virtual void PutLocalVariables (Object^ pData);
	virtual void PutBodyBinary (Object^ pData);
	virtual bool PutBodyException (Object^ pData, System::Reflection::Emit::OpCode & pOpCode, LPBYTE pOperand, int pOffset, LPBYTE pBinary);
	virtual bool PutBodyOpCode (Object^ pData, System::Reflection::Emit::OpCode & pOpCode, LPBYTE pOperand, int pOffset, LPBYTE pBinary);

protected:
	virtual System::Type^ GetTokenType (System::Reflection::Emit::OpCode & pOpCode, DWORD pToken);
	virtual System::Type^ GetTokenType (System::Reflection::Emit::OpCode & pOpCode, DWORD pToken, array<System::Type^>^ pGenericTypeArguments, array<System::Type^>^ pGenericMethodArguments);
	virtual System::Reflection::MethodBase^ GetTokenMethod (System::Reflection::Emit::OpCode & pOpCode, DWORD pToken);
	virtual System::Reflection::MethodBase^ GetTokenMethod (System::Reflection::Emit::OpCode & pOpCode, DWORD pToken, array<System::Type^>^ pGenericTypeArguments, array<System::Type^>^ pGenericMethodArguments);
	virtual System::Reflection::FieldInfo^ GetTokenField (System::Reflection::Emit::OpCode & pOpCode, DWORD pToken);
	virtual System::Reflection::FieldInfo^ GetTokenField (System::Reflection::Emit::OpCode & pOpCode, DWORD pToken, array<System::Type^>^ pGenericTypeArguments, array<System::Type^>^ pGenericMethodArguments);
	virtual System::String^ GetTokenString (DWORD pToken);

protected:
	static System::Reflection::Emit::OpCode GetOpCode (LPBYTE pOpCode);
	static int OperandSize (System::Reflection::Emit::OpCode pOpCode);
	static bool OperandIsToken (System::Reflection::Emit::OpCode pOpCode);
	static bool OperandIsValue (System::Reflection::Emit::OpCode pOpCode);

protected:
	System::Reflection::Module^	mSourceModule;
};

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
