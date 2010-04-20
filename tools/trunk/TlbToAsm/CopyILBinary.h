#pragma once

namespace DoubleAgent {
/////////////////////////////////////////////////////////////////////////////

interface class TranslateILBinary
{
	virtual bool TranslateType (System::Type^ pSourceType, System::Type^& pTargetType) = 0;
	virtual bool TranslateMethod (System::Reflection::MethodBase^ pSourceMethod, System::Reflection::MethodInfo^& pTargetMethod) = 0;
	virtual bool TranslateConstructor (System::Reflection::MethodBase^ pSourceConstructor, System::Reflection::ConstructorInfo^& pTargetConstructor) = 0;
	virtual bool TranslateField (System::Reflection::FieldInfo^ pSourceField, System::Reflection::FieldInfo^& pTargetField) = 0;
};

/////////////////////////////////////////////////////////////////////////////

ref class CopyILBinary
{
public:
	CopyILBinary (System::Reflection::Module^ pSourceModule, System::Reflection::Emit::ModuleBuilder^ pTargetModule, TranslateILBinary^ pTranslator);
	~CopyILBinary () {}

public:
	System::Reflection::MethodBody^ CopyMethodBody (System::Reflection::MethodBase^ pSourceMethod, System::Reflection::Emit::ILGenerator^ pGenerator);

protected:
	System::Type^ GetTokenType (DWORD pToken);
	System::Reflection::MethodInfo^ GetTokenMethod (DWORD pToken);
	System::Reflection::ConstructorInfo^ GetTokenConstructor (DWORD pToken);
	System::Reflection::FieldInfo^ GetTokenField (DWORD pToken);

private:
	static System::Reflection::Emit::OpCode GetOpCode (LPBYTE pOpCode);
	static int OperandSize (System::Reflection::Emit::OpCode pOpCode);
	static bool OperandIsToken (System::Reflection::Emit::OpCode pOpCode);
	static bool OperandIsValue (System::Reflection::Emit::OpCode pOpCode);
#ifdef	_DEBUG
	static String^ OpCodeTypeName (System::Reflection::Emit::OpCode pOpCode);
	static String^ OpCodeFlowName (System::Reflection::Emit::OpCode pOpCode);
	static String^ OperandTypeName (System::Reflection::Emit::OpCode pOpCode);
	static bool OpHasToken (LPBYTE pOpCode);
	static int OpCodeSize (LPBYTE pOpCode);
#endif

protected:
	System::Reflection::Module^					mSourceModule;
	System::Reflection::Emit::ModuleBuilder^	mTargetModule;
	TranslateILBinary^							mTranslator;
};

/////////////////////////////////////////////////////////////////////////////
};