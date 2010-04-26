#pragma once
#include "ParseILBinary.h"

namespace DoubleAgent {
namespace TlbToAsm {
/////////////////////////////////////////////////////////////////////////////

ref class LogILBinary : public ParseILBinary
{
public:
	LogILBinary (System::Reflection::Module^ pSourceModule, UINT pLogLevel, int pLogIndent);
	~LogILBinary () {}

public:
	UINT	mLogLevel;
	int		mLogIndent;

public:
	void LogMethodBody (System::Reflection::MethodBase^ pSourceMethod);

protected:
	virtual void PutLocalVariables (Object^ pData) override;
	virtual bool PutBodyException (Object^ pData, System::Reflection::Emit::OpCode & pOpCode, LPBYTE pOperand, int pOffset, LPBYTE pBinary) override;
	virtual bool PutBodyOpCode (Object^ pData, System::Reflection::Emit::OpCode & pOpCode, LPBYTE pOperand, int pOffset, LPBYTE pBinary) override;

protected:
	static String^ OpCodeTypeName (System::Reflection::Emit::OpCode pOpCode);
	static String^ OpCodeFlowName (System::Reflection::Emit::OpCode pOpCode);
	static String^ OperandTypeName (System::Reflection::Emit::OpCode pOpCode);
	
private:	
	String^ LogIndent ();
};

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
