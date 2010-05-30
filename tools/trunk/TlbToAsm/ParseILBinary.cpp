#include "stdafx.h"
#include <corhdr.h>
#include "ParseILBinary.h"

/////////////////////////////////////////////////////////////////////////////

using namespace System::Reflection;
using namespace System::Reflection::Emit;
using namespace System::Collections::Generic;

namespace DoubleAgent {
namespace TlbToAsm {
/////////////////////////////////////////////////////////////////////////////

ParseILBinary::ParseILBinary (Module^ pSourceModule)
:	mSourceModule (pSourceModule)
{}

void ParseILBinary::ParseMethodBody (MethodBase^ pSourceMethod)
{
	try
	{
		MethodParseData^	lParseData = gcnew MethodParseData;

		if	(GetMethodBody (pSourceMethod, lParseData))
		{
			ProcessMethodBody (lParseData);
		}
	}
	catch AnyExceptionDebug
}

bool ParseILBinary::GetMethodBody (MethodBase^ pSourceMethod, MethodParseData^ pData)
{
	bool	lRet = false;

	try
	{
		if	(
				(pData->mMethodBody = pSourceMethod->GetMethodBody ())
			&&	(pData->mBinary = pData->mMethodBody->GetILAsByteArray ())
			&&	(pData->mBinary->Length > 0)
			)
		{
			lRet = true;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

void ParseILBinary::ProcessMethodBody (Object^ pData)
{
	ParseBodyBinary (pData);
	GetExceptionClauses (pData);
	PutLocalVariables (pData);
	PutBodyBinary (pData);
}

/////////////////////////////////////////////////////////////////////////////

void ParseILBinary::ParseBodyBinary (Object^ pData)
{
	try
	{
		MethodParseData^		lData = safe_cast <MethodParseData^> (pData);
		int						lOffset;
		pin_ptr <unsigned char>	lBinary = &lData->mBinary[0];
		LPBYTE					lOpVal;
		LPBYTE					lOpEnd = (LPBYTE)lBinary + lData->mBinary->Length;
		LPBYTE					lOperand;
		Emit::OpCode			lOpCode;

		lData->mOpCodesAt = gcnew Dictionary <int, Emit::OpCode>;

		for (lOpVal = (LPBYTE)lBinary, lOffset = 0; lOpVal < lOpEnd; lOpVal += lOpCode.Size+OperandSize(lOpCode), lOffset += lOpCode.Size+OperandSize(lOpCode))
		{
			lOpCode = GetOpCode (lOpVal);
			lData->mOpCodesAt->Add (lOffset, lOpCode);
			lOperand = lOpVal + lOpCode.Size;

			if	(
					(lOpCode.OperandType == OperandType::InlineBrTarget)
				||	(lOpCode.OperandType == OperandType::ShortInlineBrTarget)
				)
			{
				int	lTarget = lOffset + lOpCode.Size + OperandSize(lOpCode);

				if	(lOpCode.OperandType == OperandType::ShortInlineBrTarget)
				{
					lTarget += *(SByte*)lOperand;
				}
				else
				{
					lTarget += *(Int32*)lOperand;
				}
				DefineBodyLabel (pData, lTarget);
			}
		}
	}
	catch AnyExceptionDebug
}

void ParseILBinary::DefineBodyLabel (Object^ pData, int pLabelTarget)
{
}

/////////////////////////////////////////////////////////////////////////////

void ParseILBinary::GetExceptionClauses (Object^ pData)
{
	try
	{
		MethodParseData^			lData = safe_cast <MethodParseData^> (pData);
		ExceptionHandlingClause^	lExceptionClause;

		lData->mTryStartAt = gcnew Dictionary <int, int>;
		lData->mTryEndAt = gcnew Dictionary <int, int>;
		lData->mCatchStartAt = gcnew Dictionary <int, ExceptionHandlingClause^>;
		lData->mCatchEndAt = gcnew Dictionary <int, ExceptionHandlingClause^>;

		if	(lData->mMethodBody->ExceptionHandlingClauses)
		{
			for each (lExceptionClause in lData->mMethodBody->ExceptionHandlingClauses)
			{
				if	(lData->mTryStartAt->ContainsKey (lExceptionClause->TryOffset))
				{
					lData->mTryStartAt [lExceptionClause->TryOffset] = lData->mTryStartAt [lExceptionClause->TryOffset] + 1;
				}
				else
				{
					lData->mTryStartAt [lExceptionClause->TryOffset] = 1;
				}
				if	(lData->mTryEndAt->ContainsKey (lExceptionClause->TryOffset+lExceptionClause->TryLength))
				{
					lData->mTryEndAt [lExceptionClause->TryOffset+lExceptionClause->TryLength] = lData->mTryStartAt [lExceptionClause->TryOffset+lExceptionClause->TryLength] + 1;
				}
				else
				{
					lData->mTryEndAt [lExceptionClause->TryOffset+lExceptionClause->TryLength] = 1;
				}
				lData->mCatchStartAt [lExceptionClause->HandlerOffset] = lExceptionClause;
				lData->mCatchEndAt [lExceptionClause->HandlerOffset+lExceptionClause->HandlerLength] = lExceptionClause;
			}
		}
	}
	catch AnyExceptionDebug
}

void ParseILBinary::PutLocalVariables (Object^ pData)
{
}

/////////////////////////////////////////////////////////////////////////////

void ParseILBinary::PutBodyBinary (Object^ pData)
{
	try
	{
		MethodParseData^					lData = safe_cast <MethodParseData^> (pData);
		pin_ptr <unsigned char>				lBinary = &lData->mBinary[0];
		KeyValuePair <int, Emit::OpCode>^	lOpCodeAt;

		for each (lOpCodeAt in lData->mOpCodesAt)
		{
			if	(PutBodyException (pData, lOpCodeAt->Value, lBinary+lOpCodeAt->Key+lOpCodeAt->Value.Size, lOpCodeAt->Key, lBinary))
			{
				PutBodyOpCode (pData, lOpCodeAt->Value, lBinary+lOpCodeAt->Key+lOpCodeAt->Value.Size, lOpCodeAt->Key, lBinary);
			}
		}
	}
	catch AnyExceptionDebug
}

bool ParseILBinary::PutBodyException (Object^ pData, System::Reflection::Emit::OpCode & pOpCode, LPBYTE pOperand, int pOffset, LPBYTE pBinary)
{
	return true;
}

bool ParseILBinary::PutBodyOpCode (Object^ pData, System::Reflection::Emit::OpCode & pOpCode, LPBYTE pOperand, int pOffset, LPBYTE pBinary)
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

Type^ ParseILBinary::GetTokenType (DWORD pToken)
{
	Type^	lTokenType = nullptr;

	if	(
			(TypeFromToken (pToken) == mdtTypeRef)
		||	(TypeFromToken (pToken) == mdtTypeDef)
		)
	{
		try
		{
			try
			{
				lTokenType = mSourceModule->ResolveType (pToken);
			}
			catch AnyExceptionSilent

			if	(!lTokenType)
			{
				try
				{
					lTokenType = GetType()->Module->ResolveType (pToken);
				}
				catch AnyExceptionSilent
			}
		}
		catch AnyExceptionSilent
	}
	return lTokenType;
}

/////////////////////////////////////////////////////////////////////////////

MethodBase^ ParseILBinary::GetTokenMethod (DWORD pToken)
{
	MethodBase^	lTokenMethod = nullptr;

	if	(
			(TypeFromToken (pToken) == mdtMethodDef)
		||	(TypeFromToken (pToken) == mdtMemberRef)
		)
	{
		try
		{
			try
			{
				lTokenMethod = mSourceModule->ResolveMethod (pToken);
			}
			catch AnyExceptionSilent

			if	(!lTokenMethod)
			{
				try
				{
					lTokenMethod = GetType()->Module->ResolveMethod (pToken);
				}
				catch AnyExceptionSilent
			}
		}
		catch AnyExceptionSilent
	}
	return lTokenMethod;
}

/////////////////////////////////////////////////////////////////////////////

FieldInfo^ ParseILBinary::GetTokenField (DWORD pToken)
{
	FieldInfo^	lTokenField = nullptr;

	if	(TypeFromToken (pToken) == mdtFieldDef)
	{
		try
		{
			try
			{
				lTokenField = mSourceModule->ResolveField (pToken);
			}
			catch AnyExceptionSilent

			if	(!lTokenField)
			{
				try
				{
					lTokenField = GetType()->Module->ResolveField (pToken);
				}
				catch AnyExceptionSilent
			}
		}
		catch AnyExceptionSilent
	}
	return lTokenField;
}

/////////////////////////////////////////////////////////////////////////////

System::String^ ParseILBinary::GetTokenString (DWORD pToken)
{
	String^	lTokenString = nullptr;

	if	(TypeFromToken (pToken) == mdtString)
	{
		try
		{
			try
			{
				lTokenString = mSourceModule->ResolveString (pToken);
			}
			catch AnyExceptionSilent

			if	(!lTokenString)
			{
				try
				{
					lTokenString = GetType()->Module->ResolveString (pToken);
				}
				catch AnyExceptionSilent
			}
		}
		catch AnyExceptionSilent
	}
	return lTokenString;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

System::Reflection::Emit::OpCode ParseILBinary::GetOpCode (LPBYTE pOpCode)
{
	switch (pOpCode[0])
	{
		case 0x00: return Emit::OpCodes::Nop;
		case 0x01: return Emit::OpCodes::Break;
		case 0x02: return Emit::OpCodes::Ldarg_0;
		case 0x03: return Emit::OpCodes::Ldarg_1;
		case 0x04: return Emit::OpCodes::Ldarg_2;
		case 0x05: return Emit::OpCodes::Ldarg_3;
		case 0x06: return Emit::OpCodes::Ldloc_0;
		case 0x07: return Emit::OpCodes::Ldloc_1;
		case 0x08: return Emit::OpCodes::Ldloc_2;
		case 0x09: return Emit::OpCodes::Ldloc_3;
		case 0x0A: return Emit::OpCodes::Stloc_0;
		case 0x0B: return Emit::OpCodes::Stloc_1;
		case 0x0C: return Emit::OpCodes::Stloc_2;
		case 0x0D: return Emit::OpCodes::Stloc_3;
		case 0x0E: return Emit::OpCodes::Ldarg_S;
		case 0x0F: return Emit::OpCodes::Ldarga_S;

		case 0x10: return Emit::OpCodes::Starg_S;
		case 0x11: return Emit::OpCodes::Ldloc_S;
		case 0x12: return Emit::OpCodes::Ldloca_S;
		case 0x13: return Emit::OpCodes::Stloc_S;
		case 0x14: return Emit::OpCodes::Ldnull;
		case 0x15: return Emit::OpCodes::Ldc_I4_M1;
		case 0x16: return Emit::OpCodes::Ldc_I4_0;
		case 0x17: return Emit::OpCodes::Ldc_I4_1;
		case 0x18: return Emit::OpCodes::Ldc_I4_2;
		case 0x19: return Emit::OpCodes::Ldc_I4_3;
		case 0x1A: return Emit::OpCodes::Ldc_I4_4;
		case 0x1B: return Emit::OpCodes::Ldc_I4_5;
		case 0x1C: return Emit::OpCodes::Ldc_I4_6;
		case 0x1D: return Emit::OpCodes::Ldc_I4_7;
		case 0x1E: return Emit::OpCodes::Ldc_I4_8;
		case 0x1F: return Emit::OpCodes::Ldc_I4_S;

		case 0x20: return Emit::OpCodes::Ldc_I4;
		case 0x21: return Emit::OpCodes::Ldc_I8;
		case 0x22: return Emit::OpCodes::Ldc_R4;
		case 0x23: return Emit::OpCodes::Ldc_R8;
		case 0x25: return Emit::OpCodes::Dup;
		case 0x26: return Emit::OpCodes::Pop;
		case 0x27: return Emit::OpCodes::Jmp;
		case 0x28: return Emit::OpCodes::Call;
		case 0x29: return Emit::OpCodes::Calli;
		case 0x2A: return Emit::OpCodes::Ret;
		case 0x2B: return Emit::OpCodes::Br_S;
		case 0x2C: return Emit::OpCodes::Brfalse_S;
		case 0x2D: return Emit::OpCodes::Brtrue_S;
		case 0x2E: return Emit::OpCodes::Beq_S;
		case 0x2F: return Emit::OpCodes::Bge_S;

		case 0x30: return Emit::OpCodes::Bgt_S;
		case 0x31: return Emit::OpCodes::Ble_S;
		case 0x32: return Emit::OpCodes::Blt_S;
		case 0x33: return Emit::OpCodes::Bne_Un_S;
		case 0x34: return Emit::OpCodes::Bge_Un_S;
		case 0x35: return Emit::OpCodes::Bgt_Un_S;
		case 0x36: return Emit::OpCodes::Ble_Un_S;
		case 0x37: return Emit::OpCodes::Blt_Un_S;
		case 0x38: return Emit::OpCodes::Br;
		case 0x39: return Emit::OpCodes::Brfalse;
		case 0x3A: return Emit::OpCodes::Brtrue;
		case 0x3B: return Emit::OpCodes::Beq;
		case 0x3C: return Emit::OpCodes::Bge;
		case 0x3D: return Emit::OpCodes::Bgt;
		case 0x3E: return Emit::OpCodes::Ble;
		case 0x3F: return Emit::OpCodes::Blt;

		case 0x40: return Emit::OpCodes::Bne_Un;
		case 0x41: return Emit::OpCodes::Bge_Un;
		case 0x42: return Emit::OpCodes::Bgt_Un;
		case 0x43: return Emit::OpCodes::Ble_Un;
		case 0x44: return Emit::OpCodes::Blt_Un;
		case 0x45: return Emit::OpCodes::Switch;
		case 0x46: return Emit::OpCodes::Ldind_I1;
		case 0x48: return Emit::OpCodes::Ldind_I2;
		case 0x47: return Emit::OpCodes::Ldind_U1;
		case 0x49: return Emit::OpCodes::Ldind_U2;
		case 0x4A: return Emit::OpCodes::Ldind_I4;
		case 0x4B: return Emit::OpCodes::Ldind_U4;
		case 0x4C: return Emit::OpCodes::Ldind_I8;
		case 0x4D: return Emit::OpCodes::Ldind_I;
		case 0x4E: return Emit::OpCodes::Ldind_R4;
		case 0x4F: return Emit::OpCodes::Ldind_R8;

		case 0x50: return Emit::OpCodes::Ldind_Ref;
		case 0x51: return Emit::OpCodes::Stind_Ref;
		case 0x52: return Emit::OpCodes::Stind_I1;
		case 0x53: return Emit::OpCodes::Stind_I2;
		case 0x54: return Emit::OpCodes::Stind_I4;
		case 0x55: return Emit::OpCodes::Stind_I8;
		case 0x56: return Emit::OpCodes::Stind_R4;
		case 0x57: return Emit::OpCodes::Stind_R8;
		case 0x58: return Emit::OpCodes::Add;
		case 0x59: return Emit::OpCodes::Sub;
		case 0x5A: return Emit::OpCodes::Mul;
		case 0x5B: return Emit::OpCodes::Div;
		case 0x5C: return Emit::OpCodes::Div_Un;
		case 0x5D: return Emit::OpCodes::Rem;
		case 0x5E: return Emit::OpCodes::Rem_Un;
		case 0x5F: return Emit::OpCodes::And;

		case 0x60: return Emit::OpCodes::Or;
		case 0x61: return Emit::OpCodes::Xor;
		case 0x62: return Emit::OpCodes::Shl;
		case 0x63: return Emit::OpCodes::Shr;
		case 0x64: return Emit::OpCodes::Shr_Un;
		case 0x65: return Emit::OpCodes::Neg;
		case 0x66: return Emit::OpCodes::Not;
		case 0x67: return Emit::OpCodes::Conv_I1;
		case 0x68: return Emit::OpCodes::Conv_I2;
		case 0x69: return Emit::OpCodes::Conv_I4;
		case 0x6A: return Emit::OpCodes::Conv_I8;
		case 0x6B: return Emit::OpCodes::Conv_R4;
		case 0x6C: return Emit::OpCodes::Conv_R8;
		case 0x6D: return Emit::OpCodes::Conv_U4;
		case 0x6E: return Emit::OpCodes::Conv_U8;
		case 0x6F: return Emit::OpCodes::Callvirt;

		case 0x70: return Emit::OpCodes::Cpobj;
		case 0x72: return Emit::OpCodes::Ldstr;
		case 0x73: return Emit::OpCodes::Newobj;
		case 0x74: return Emit::OpCodes::Castclass;
		case 0x75: return Emit::OpCodes::Isinst;
		case 0x76: return Emit::OpCodes::Conv_R_Un;
		case 0x79: return Emit::OpCodes::Unbox;
		case 0x7A: return Emit::OpCodes::Throw;
		case 0x7B: return Emit::OpCodes::Ldfld;
		case 0x7C: return Emit::OpCodes::Ldflda;
		case 0x7D: return Emit::OpCodes::Stfld;
		case 0x7E: return Emit::OpCodes::Ldsfld;
		case 0x7F: return Emit::OpCodes::Ldsflda;

		case 0x80: return Emit::OpCodes::Stsfld;
		case 0x81: return Emit::OpCodes::Stobj;
		case 0x82: return Emit::OpCodes::Conv_Ovf_I1_Un;
		case 0x83: return Emit::OpCodes::Conv_Ovf_I2_Un;
		case 0x84: return Emit::OpCodes::Conv_Ovf_I4_Un;
		case 0x85: return Emit::OpCodes::Conv_Ovf_I8_Un;
		case 0x86: return Emit::OpCodes::Conv_Ovf_U1_Un;
		case 0x87: return Emit::OpCodes::Conv_Ovf_U2_Un;
		case 0x88: return Emit::OpCodes::Conv_Ovf_U4_Un;
		case 0x89: return Emit::OpCodes::Conv_Ovf_U8_Un;
		case 0x8A: return Emit::OpCodes::Conv_Ovf_I_Un;
		case 0x8B: return Emit::OpCodes::Conv_Ovf_U_Un;
		case 0x8C: return Emit::OpCodes::Box;
		case 0x8D: return Emit::OpCodes::Newarr;
		case 0x8E: return Emit::OpCodes::Ldlen;
		case 0x8F: return Emit::OpCodes::Ldelema;

		case 0x90: return Emit::OpCodes::Ldelem_I1;
		case 0x91: return Emit::OpCodes::Ldelem_U1;
		case 0x92: return Emit::OpCodes::Ldelem_I2;
		case 0x93: return Emit::OpCodes::Ldelem_U2;
		case 0x94: return Emit::OpCodes::Ldelem_I4;
		case 0x95: return Emit::OpCodes::Ldelem_U4;
		case 0x96: return Emit::OpCodes::Ldelem_I8;
		case 0x97: return Emit::OpCodes::Ldelem_I;
		case 0x98: return Emit::OpCodes::Ldelem_R4;
		case 0x99: return Emit::OpCodes::Ldelem_R8;
		case 0x9A: return Emit::OpCodes::Ldelem_Ref;
		case 0x9B: return Emit::OpCodes::Stelem_I;
		case 0x9C: return Emit::OpCodes::Stelem_I1;
		case 0x9D: return Emit::OpCodes::Stelem_I2;
		case 0x9E: return Emit::OpCodes::Stelem_I4;
		case 0x9F: return Emit::OpCodes::Stelem_I8;

		case 0xA0: return Emit::OpCodes::Stelem_R4;
		case 0xA1: return Emit::OpCodes::Stelem_R8;
		case 0xA2: return Emit::OpCodes::Stelem_Ref;
		case 0xA3: return Emit::OpCodes::Ldelem;
		case 0xA5: return Emit::OpCodes::Unbox_Any;
		case 0xAA: return Emit::OpCodes::Stelem;

		case 0xB3: return Emit::OpCodes::Conv_Ovf_I1;
		case 0xB4: return Emit::OpCodes::Conv_Ovf_U1;
		case 0xB5: return Emit::OpCodes::Conv_Ovf_I2;
		case 0xB6: return Emit::OpCodes::Conv_Ovf_U2;
		case 0xB7: return Emit::OpCodes::Conv_Ovf_I4;
		case 0xB8: return Emit::OpCodes::Conv_Ovf_U4;
		case 0xB9: return Emit::OpCodes::Conv_Ovf_I8;
		case 0xBA: return Emit::OpCodes::Conv_Ovf_U8;

		case 0xC2: return Emit::OpCodes::Refanyval;
		case 0xC3: return Emit::OpCodes::Ckfinite;
		case 0xC6: return Emit::OpCodes::Mkrefany;

		case 0xD0: return Emit::OpCodes::Ldtoken;
		case 0xD2: return Emit::OpCodes::Conv_U1;
		case 0xD1: return Emit::OpCodes::Conv_U2;
		case 0xD3: return Emit::OpCodes::Conv_I;
		case 0xD4: return Emit::OpCodes::Conv_Ovf_I;
		case 0xD5: return Emit::OpCodes::Conv_Ovf_U;
		case 0xD6: return Emit::OpCodes::Add_Ovf;
		case 0xD7: return Emit::OpCodes::Add_Ovf_Un;
		case 0xD8: return Emit::OpCodes::Mul_Ovf;
		case 0xD9: return Emit::OpCodes::Mul_Ovf_Un;
		case 0xDA: return Emit::OpCodes::Sub_Ovf;
		case 0xDB: return Emit::OpCodes::Sub_Ovf_Un;
		case 0xDC: return Emit::OpCodes::Endfinally;
		case 0xDD: return Emit::OpCodes::Leave;
		case 0xDE: return Emit::OpCodes::Leave_S;
		case 0xDF: return Emit::OpCodes::Stind_I;

		case 0xE0: return Emit::OpCodes::Conv_U;

		case 0xFE:
		{
			switch (pOpCode[1])
			{
				case 0x00: return Emit::OpCodes::Arglist;
				case 0x01: return Emit::OpCodes::Ceq;
				case 0x02: return Emit::OpCodes::Cgt;
				case 0x03: return Emit::OpCodes::Cgt_Un;
				case 0x04: return Emit::OpCodes::Clt;
				case 0x05: return Emit::OpCodes::Clt_Un;
				case 0x06: return Emit::OpCodes::Ldftn;
				case 0x07: return Emit::OpCodes::Ldvirtftn;
				case 0x09: return Emit::OpCodes::Ldarg;
				case 0x0A: return Emit::OpCodes::Ldarga;
				case 0x0B: return Emit::OpCodes::Starg;
				case 0x0C: return Emit::OpCodes::Ldloc;
				case 0x0D: return Emit::OpCodes::Ldloca;
				case 0x0E: return Emit::OpCodes::Stloc;
				case 0x0F: return Emit::OpCodes::Localloc;

				case 0x11: return Emit::OpCodes::Endfilter;
				case 0x12: return Emit::OpCodes::Unaligned;
				case 0x13: return Emit::OpCodes::Volatile;
				case 0x14: return Emit::OpCodes::Tailcall;
				case 0x15: return Emit::OpCodes::Initobj;
				case 0x16: return Emit::OpCodes::Constrained;
				case 0x17: return Emit::OpCodes::Cpblk;
				case 0x18: return Emit::OpCodes::Initblk;
				case 0x19: return Emit::OpCodes::Prefix1;
				case 0x1A: return Emit::OpCodes::Rethrow;
				case 0x1C: return Emit::OpCodes::Sizeof;
				case 0x1D: return Emit::OpCodes::Refanytype;
				case 0x1E: return Emit::OpCodes::Readonly;
			}
		}
	}
	return Emit::OpCodes::Nop;
}

/////////////////////////////////////////////////////////////////////////////

int ParseILBinary::OperandSize (Emit::OpCode pOpCode)
{
	switch (pOpCode.OperandType)
	{
		case Emit::OperandType::ShortInlineBrTarget:	return 1; break;
		case Emit::OperandType::ShortInlineI:			return 1; break;
		case Emit::OperandType::ShortInlineVar:			return 1; break;
		case Emit::OperandType::InlineVar:				return 2; break;
		case Emit::OperandType::InlineBrTarget:			return 4; break;
		case Emit::OperandType::InlineField:			return 4; break;
		case Emit::OperandType::InlineI:				return 4; break;
		case Emit::OperandType::InlineMethod:			return 4; break;
		case Emit::OperandType::InlineSig:				return 4; break;
		case Emit::OperandType::InlineString:			return 4; break;
		case Emit::OperandType::InlineSwitch:			return 4; break;
		case Emit::OperandType::InlineTok:				return 4; break;
		case Emit::OperandType::InlineType:				return 4; break;
		case Emit::OperandType::ShortInlineR:			return 4; break;
		case Emit::OperandType::InlineI8:				return 8; break;
		case Emit::OperandType::InlineR:				return 8; break;
		default:										return 0;
	}
}

bool ParseILBinary::OperandIsToken (System::Reflection::Emit::OpCode pOpCode)
{
	if	(
			(pOpCode.OperandType == Emit::OperandType::InlineField)
		||	(pOpCode.OperandType == Emit::OperandType::InlineMethod)
		||	(pOpCode.OperandType == Emit::OperandType::InlineType)
		||	(pOpCode.OperandType == Emit::OperandType::InlineString)
		||	(pOpCode.OperandType == Emit::OperandType::InlineSig)
		||	(pOpCode.OperandType == Emit::OperandType::InlineTok)
		)
	{
		return true;
	}
	return false;
}

bool ParseILBinary::OperandIsValue (System::Reflection::Emit::OpCode pOpCode)
{
	if	(
			(pOpCode.OperandType == Emit::OperandType::InlineI)
		||	(pOpCode.OperandType == Emit::OperandType::InlineI8)
		||	(pOpCode.OperandType == Emit::OperandType::InlineR)
		||	(pOpCode.OperandType == Emit::OperandType::InlineString)
		||	(pOpCode.OperandType == Emit::OperandType::ShortInlineI)
		||	(pOpCode.OperandType == Emit::OperandType::ShortInlineR)
		)
	{
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
