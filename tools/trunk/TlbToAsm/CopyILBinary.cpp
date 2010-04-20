#include "stdafx.h"
#include <corhdr.h>
#include "CopyILBinary.h"

#ifdef	_DEBUG
//#define	_DEBUG_CODE		LogDebugFast
//#define	_DEBUG_TOKENS	LogDebugFast
#endif

/////////////////////////////////////////////////////////////////////////////

using namespace System::Runtime::InteropServices;
using namespace System::Reflection;
using namespace System::Reflection::Emit;
using namespace System::Collections::Generic;

namespace DoubleAgent {
/////////////////////////////////////////////////////////////////////////////

CopyILBinary::CopyILBinary (Module^ pSourceModule, ModuleBuilder^ pTargetModule, TranslateILBinary^ pTranslator)
:	mSourceModule (pSourceModule),
	mTargetModule (pTargetModule),
	mTranslator (pTranslator)
{}

MethodBody^ CopyILBinary::CopyMethodBody (MethodBase^ pSourceMethod, Emit::ILGenerator^ pGenerator)
{
	MethodBody^ lMethodBody = nullptr;

	try
	{
		lMethodBody = pSourceMethod->GetMethodBody ();
		
		if	(lMethodBody)
		{
			array <unsigned char>^						lBody = lMethodBody->GetILAsByteArray ();
			GCHandle									lMemHandle = GCHandle::Alloc (lBody, GCHandleType::Pinned);
			IntPtr										lMemAddress = lMemHandle.AddrOfPinnedObject ();
			LPBYTE										lBinary = (LPBYTE)lMemAddress.ToPointer();
			Dictionary <int, Emit::OpCode>^				lOpCodesAt = gcnew Dictionary <int, Emit::OpCode>;
			Dictionary <int, Emit::Label>^				lLabelsAt = gcnew Dictionary <int, Emit::Label>;
			Dictionary <int, int>^						lTryStartAt = gcnew Dictionary <int, int>;
			Dictionary <int, int>^						lTryEndAt = gcnew Dictionary <int, int>;
			Dictionary <int, ExceptionHandlingClause^>^	lCatchStartAt = gcnew Dictionary <int, ExceptionHandlingClause^>;
			Dictionary <int, ExceptionHandlingClause^>^	lCatchEndAt = gcnew Dictionary <int, ExceptionHandlingClause^>;

#ifdef	_DEBUG_CODE
			LogMessage (_DEBUG_CODE, _T("Copy Method Body [%s.%s]"), _B(pSourceMethod->DeclaringType->FullName), _B(pSourceMethod->Name));
#endif
			try
			{
				LocalVariableInfo^	lLocalVariable;
				
				if	(lMethodBody->LocalVariables)
				{
					for each (lLocalVariable in lMethodBody->LocalVariables)
					{
						Type^	lLocalType;
						
						if	(mTranslator->TranslateType (lLocalVariable->LocalType, lLocalType))
						{
#ifdef	_DEBUG_TOKENS
							LogMessage (_DEBUG_TOKENS, _T("    Local Type [%s] to [%s]"), _B(lLocalVariable->LocalType->FullName), _B(lLocalType->FullName));
#endif
						}
#ifdef	_DEBUG_TOKENS
						else
						{
							LogMessage (_DEBUG_TOKENS, _T("    Local Type [%s]"), _B(lLocalType->FullName));
						}
#endif
						pGenerator->DeclareLocal (lLocalType, lLocalVariable->IsPinned);	
					}
				}
			}
			catch AnyExceptionDebug

			try
			{
				ExceptionHandlingClause^	lExceptionClause;
				
				if	(lMethodBody->ExceptionHandlingClauses)
				{
					for each (lExceptionClause in lMethodBody->ExceptionHandlingClauses)
					{
						if	(lTryStartAt->ContainsKey (lExceptionClause->TryOffset))
						{
							lTryStartAt [lExceptionClause->TryOffset] = lTryStartAt [lExceptionClause->TryOffset] + 1;
						}
						else
						{
							lTryStartAt [lExceptionClause->TryOffset] = 1;
						}
						if	(lTryEndAt->ContainsKey (lExceptionClause->TryOffset+lExceptionClause->TryLength))
						{
							lTryEndAt [lExceptionClause->TryOffset+lExceptionClause->TryLength] = lTryStartAt [lExceptionClause->TryOffset+lExceptionClause->TryLength] + 1;
						}
						else
						{
							lTryEndAt [lExceptionClause->TryOffset+lExceptionClause->TryLength] = 1;
						}
						lCatchStartAt [lExceptionClause->HandlerOffset] = lExceptionClause;
						lCatchEndAt [lExceptionClause->HandlerOffset+lExceptionClause->HandlerLength] = lExceptionClause;
					}
				}
			}
			catch AnyExceptionDebug

			try
			{
				int				lOffset;
				LPBYTE			lOpVal;
				LPBYTE			lOpEnd = lBinary + lBody->Length;
				LPBYTE			lOperand;
				Emit::OpCode	lOpCode;

				for (lOpVal = lBinary, lOffset = 0; lOpVal < lOpEnd; lOpVal += lOpCode.Size+OperandSize(lOpCode), lOffset += lOpCode.Size+OperandSize(lOpCode))
				{
					lOpCode = GetOpCode (lOpVal);
					lOpCodesAt->Add (lOffset, lOpCode);
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
						if	(!lLabelsAt->ContainsKey (lTarget))
						{
							lLabelsAt->Add (lTarget, pGenerator->DefineLabel ());
						}
					}
				}
			}
			catch AnyExceptionDebug
			
			try
			{
				KeyValuePair <int, Emit::OpCode>^	lOpCodeAt;

				for each (lOpCodeAt in lOpCodesAt)
				{
					Emit::OpCode &	lOpCode = lOpCodeAt->Value;
					int				lOffset = lOpCodeAt->Key;
					LPBYTE			lOperand = lBinary + lOffset + lOpCode.Size;
					
#ifdef	_DEBUG_CODE
					try
					{
						TCHAR	lOpCodeStr [3];
						String^	lOperandVal = "";

						if	(lBinary[lOffset] == 0xFE)
						{
							_stprintf (lOpCodeStr, _T("%2.2X%2.2X"), lBinary[lOffset], lBinary[lOffset+1]);
						}
						else
						{
							_stprintf (lOpCodeStr, _T("%2.2X  "), lBinary[lOffset]);
						}

						if	(OperandIsToken(lOpCode))
						{
							try
							{
								TCHAR lVal [100];
								_stprintf (lVal, _T(" Token (%8.8X)"), *(LPDWORD)lOperand);
								lOperandVal = gcnew String (lVal);
							}
							catch AnyExceptionSilent
						}
						else
						if	(OperandIsValue(lOpCode))
						{
							try
							{
								TCHAR lVal [100];
								_stprintf (lVal, _T(" (%d)"), (Int32)lOpCode.Value);
								lOperandVal = gcnew String (lVal);
							}
							catch AnyExceptionSilent
						}

						LogMessage (_DEBUG_CODE, _T("  %4.4X: OpCode [%s] Size [%d] [%s %s%s] [%s] [%s]"), lOffset, lOpCodeStr, lOpCode.Size+OperandSize(lOpCode), _B(lOpCode.Name), _B(OperandTypeName(lOpCode)), _B(lOperandVal),  _B(OpCodeTypeName(lOpCode)),  _B(OpCodeFlowName(lOpCode)));

						if	(lOpCode.Size+OperandSize(lOpCode) != OpCodeSize(lBinary+lOffset))
						{
							LogMessage (LogDebug, _T("!!!! OpCodeSize [%d] [%d]"), lOpCode.Size+OperandSize(lOpCode), OpCodeSize(lBinary+lOffset));
						}
						if	(OperandIsToken(lOpCode) != OpHasToken(lBinary+lOffset))
						{
							LogMessage (LogDebug, _T("!!!! OpHasToken [%d] [%d]"), OperandIsToken(lOpCode), OpHasToken(lBinary+lOffset));
						}
					}
					catch AnyExceptionSilent
#endif
					if	(lLabelsAt->ContainsKey (lOffset))
					{
						pGenerator->MarkLabel (lLabelsAt [lOffset]);
					}

					if	(lCatchStartAt->ContainsKey (lOffset))
					{
						ExceptionHandlingClause^	lExceptionClause = lCatchStartAt [lOffset];
						
						if	((int)lExceptionClause->Flags & (int)ExceptionHandlingClauseOptions::Finally)
						{
#ifdef	_DEBUG_CODE
							LogMessage (_DEBUG_CODE, _T("  %4.4X: Finally"), lOffset);
#endif						
							pGenerator->BeginFinallyBlock ();
						}
						else
						if	((int)lExceptionClause->Flags & (int)ExceptionHandlingClauseOptions::Filter)
						{
#ifdef	_DEBUG_CODE
							LogMessage (_DEBUG_CODE, _T("  %4.4X: Filter"), lOffset);
#endif						
							pGenerator->BeginExceptFilterBlock ();
						}
						else
						if	((int)lExceptionClause->Flags & (int)ExceptionHandlingClauseOptions::Fault)
						{
#ifdef	_DEBUG_CODE
							LogMessage (_DEBUG_CODE, _T("  %4.4X: Catch (...)"), lOffset);
#endif						
							pGenerator->BeginFaultBlock ();
						}
						else
						{
#ifdef	_DEBUG_CODE
							LogMessage (_DEBUG_CODE, _T("  %4.4X: Catch [%s]"), lOffset, _B(lExceptionClause->CatchType->FullName));
#endif						
							pGenerator->BeginCatchBlock (lExceptionClause->CatchType);
						}
					}
					else
					if	(lTryEndAt->ContainsKey (lOffset))
					{
						for	(int lNdx = 0; lNdx < lTryEndAt [lOffset]; lNdx++)
						{
#ifdef	_DEBUG_CODE
							LogMessage (_DEBUG_CODE, _T("  %4.4X: Try End"), lOffset);
#endif						
							pGenerator->EndExceptionBlock ();
						}
					}
					else
					if	(lCatchEndAt->ContainsKey (lOffset))
					{
#ifdef	_DEBUG_CODE
						LogMessage (_DEBUG_CODE, _T("  %4.4X: Catch End"), lOffset);
#endif						
						pGenerator->EndExceptionBlock ();
					}

					if	(lTryStartAt->ContainsKey (lOffset))
					{
						for	(int lNdx = 0; lNdx < lTryStartAt [lOffset]; lNdx++)
						{
#ifdef	_DEBUG_CODE
							LogMessage (_DEBUG_CODE, _T("  %4.4X: Try"), lOffset);
#endif						
							pGenerator->BeginExceptionBlock ();
						}
					}

					if	(
							(lOpCode == Emit::OpCodes::Leave)
						&&	(
								(lTryEndAt->ContainsKey (lOffset+lOpCode.Size+OperandSize(lOpCode)))
							||	(lCatchEndAt->ContainsKey (lOffset+lOpCode.Size+OperandSize(lOpCode)))
							)
						)
					{
						continue;
					}
					else
					if	(
							(lOpCode == Emit::OpCodes::Endfinally)
						&&	(lCatchEndAt->ContainsKey (lOffset+lOpCode.Size+OperandSize(lOpCode)))
						)
					{
						continue;
					}

					switch (lOpCode.OperandType)
					{
						case OperandType::InlineType:
						{
							pGenerator->Emit (lOpCode, GetTokenType (*(LPDWORD)lOperand));
						}	break;
						case OperandType::InlineField:
						{
							pGenerator->Emit (lOpCode, GetTokenField (*(LPDWORD)lOperand));
						}	break;
						case OperandType::InlineMethod:
						{
							ConstructorInfo^ lConstructor = GetTokenConstructor (*(LPDWORD)lOperand);
							if	(lConstructor)
							{
								pGenerator->Emit (lOpCode, lConstructor);
							}
							else
							{
								pGenerator->Emit (lOpCode, GetTokenMethod (*(LPDWORD)lOperand));
							}
						}	break;
						case OperandType::InlineI:
						{
							pGenerator->Emit (lOpCode, *(Int32*)lOperand);
						}	break;
						case OperandType::InlineI8:
						{
							pGenerator->Emit (lOpCode, *(Int64*)lOperand);
						}	break;
						case OperandType::InlineR:
						{
							pGenerator->Emit (lOpCode, *(Double*)lOperand);
						}	break;
						case OperandType::ShortInlineI:
						{
							pGenerator->Emit (lOpCode, *(Int16*)lOperand);
						}	break;
						case OperandType::ShortInlineR:
						{
							pGenerator->Emit (lOpCode, *(Single*)lOperand);
						}	break;
						case OperandType::InlineVar:
						{
							pGenerator->Emit (lOpCode, *(Int16*)lOperand);
						}	break;
						case OperandType::ShortInlineVar:
						{
							pGenerator->Emit (lOpCode, *(SByte*)lOperand);
						}	break;
						case OperandType::InlineBrTarget:
						case OperandType::ShortInlineBrTarget:
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
							pGenerator->Emit (lOpCode, lLabelsAt [lTarget]);
						}	break;
						default:
						{
							pGenerator->Emit (lOpCode);
						}	break;
					}
				}
			}
			catch AnyExceptionDebug

#ifdef	_DEBUG_CODE
			LogMessage (LogDebug, _T("Copy Method Body Done"));
#endif			
		}
	}
	catch AnyExceptionDebug

	return lMethodBody;
}

/////////////////////////////////////////////////////////////////////////////

Type^ CopyILBinary::GetTokenType (DWORD pToken)
{
	Type^	lTargetType = nullptr;

	if	(
			(TypeFromToken (pToken) == mdtTypeRef)
		||	(TypeFromToken (pToken) == mdtTypeDef)
		)
	{
		try
		{
			Type^	lSourceType = mSourceModule->ResolveType (pToken);

			if	(
					(lSourceType)
				&&	(mTranslator->TranslateType (lSourceType, lTargetType))
				)
			{
#ifdef	_DEBUG_TOKENS
				LogMessage (_DEBUG_TOKENS, _T("    Type   [%8.8X] [%s] to [%s]"), pToken, _B(lSourceType->FullName), _B(lTargetType->FullName));
#endif
			}
#ifdef	_DEBUG_TOKENS
			else
			if	(lSourceType == nullptr)
			{
				LogMessage (_DEBUG_TOKENS, _T("!!! Unknown Type [%8.8X]"), pToken);
			}
#endif
		}
		catch AnyExceptionSilent
	}
	
	return lTargetType;
}

/////////////////////////////////////////////////////////////////////////////

MethodInfo^ CopyILBinary::GetTokenMethod (DWORD pToken)
{
	MethodInfo^	lTargetMethod = nullptr;

	if	(
			(TypeFromToken (pToken) == mdtMethodDef)
		||	(TypeFromToken (pToken) == mdtMemberRef)
		)
	{
		try
		{
			MethodBase^	lSourceMethod = mSourceModule->ResolveMethod (pToken);

			if	(
					(lSourceMethod)
				&&	(String::Compare (lSourceMethod->Name, COR_CTOR_METHOD_NAME) != 0)
				&&	(mTranslator->TranslateMethod (lSourceMethod, lTargetMethod))
				)
			{
#ifdef	_DEBUG_TOKENS
				LogMessage (_DEBUG_TOKENS, _T("    Method [%8.8X] [%s.%s] as [%s.%s]"), pToken, _B(lSourceMethod->DeclaringType->FullName), _B(lSourceMethod->Name), _B(lTargetMethod->DeclaringType->FullName), _B(lTargetMethod->Name));
#endif
			}
#ifdef	_DEBUG_TOKENS
			else
			if	(lSourceMethod == nullptr)
			{
				LogMessage (_DEBUG_TOKENS, _T("!!! Unknown Method [%8.8X]"), pToken);
			}
#endif
		}
		catch AnyExceptionSilent
	}	
	return lTargetMethod;
}

/////////////////////////////////////////////////////////////////////////////

ConstructorInfo^ CopyILBinary::GetTokenConstructor (DWORD pToken)
{
	ConstructorInfo^	lTargetConstructor = nullptr;

	if	(
			(TypeFromToken (pToken) == mdtMethodDef)
		||	(TypeFromToken (pToken) == mdtMemberRef)
		)
	{
		try
		{
			MethodBase^	lSourceMethod = mSourceModule->ResolveMethod (pToken);

			if	(
					(lSourceMethod)
				&&	(String::Compare (lSourceMethod->Name, COR_CTOR_METHOD_NAME) == 0)
				&&	(mTranslator->TranslateConstructor (lSourceMethod, lTargetConstructor))
				)
			{
#ifdef	_DEBUG_TOKENS
				LogMessage (_DEBUG_TOKENS, _T("    Method [%8.8X] [%s.%s] as [%s.%s]"), pToken, _B(lSourceMethod->DeclaringType->FullName), _B(lSourceMethod->Name), _B(lTargetConstructor->DeclaringType->FullName), _B(lTargetConstructor->Name));
#endif
			}
#ifdef	_DEBUG_TOKENS
			else
			if	(lSourceMethod == nullptr)
			{
				LogMessage (_DEBUG_TOKENS, _T("!!! Unknown Method [%8.8X]"), pToken);
			}
#endif
		}
		catch AnyExceptionSilent
	}
	return lTargetConstructor;
}

/////////////////////////////////////////////////////////////////////////////

FieldInfo^ CopyILBinary::GetTokenField (DWORD pToken)
{
	FieldInfo^	lTargetField = nullptr;
	
	if	(TypeFromToken (pToken) == mdtFieldDef)
	{
		try
		{
			FieldInfo^	lSourceField = mSourceModule->ResolveField (pToken);

			if	(
					(lSourceField)
				&&	(mTranslator->TranslateField (lSourceField, lTargetField))
				)
			{
#ifdef	_DEBUG_TOKENS
				LogMessage (LogDebug, _T("    Field  [%8.8X] [%s.%s] as [%s.%s]"), pToken, _B(lSourceField->DeclaringType->FullName), _B(lSourceField->Name), _B(lTargetField->DeclaringType->FullName), _B(lTargetField->Name));
#endif
			}
#ifdef	_DEBUG_TOKENS
			else
			if	(lSourceField == nullptr)
			{
				LogMessage (_DEBUG_TOKENS, _T("!!! Unknown Field [%8.8X]"), pToken);
			}
#endif
		}
		catch AnyExceptionSilent
	}
	return lTargetField;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

System::Reflection::Emit::OpCode CopyILBinary::GetOpCode (LPBYTE pOpCode)
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

int CopyILBinary::OperandSize (Emit::OpCode pOpCode)
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

bool CopyILBinary::OperandIsToken (System::Reflection::Emit::OpCode pOpCode)
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

bool CopyILBinary::OperandIsValue (System::Reflection::Emit::OpCode pOpCode)
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

#ifdef	_DEBUG
String^ CopyILBinary::OpCodeTypeName (System::Reflection::Emit::OpCode pOpCode)
{
	switch (pOpCode.OpCodeType)
	{
		case Emit::OpCodeType::Annotation:	return "Annotation"; break;
		case Emit::OpCodeType::Macro:		return "Macro"; break;
		case Emit::OpCodeType::Nternal:		return "Nternal"; break;
		case Emit::OpCodeType::Objmodel:	return "Objmodel"; break;
		case Emit::OpCodeType::Prefix:		return "Prefix"; break;
		case Emit::OpCodeType::Primitive:	return "Primitive"; break;
	}
	return "<unknown>";
}
#endif

#ifdef	_DEBUG
String^ CopyILBinary::OpCodeFlowName (System::Reflection::Emit::OpCode pOpCode)
{
	switch (pOpCode.FlowControl)
	{
		case Emit::FlowControl::Branch:			return "Branch"; break;
		case Emit::FlowControl::Break:			return "Break"; break;
		case Emit::FlowControl::Call:			return "Call"; break;
		case Emit::FlowControl::Cond_Branch:	return "Cond_Branch"; break;
		case Emit::FlowControl::Meta:			return "Meta"; break;
		case Emit::FlowControl::Next:			return "Next"; break;
		case Emit::FlowControl::Phi:			return "Phi"; break;
		case Emit::FlowControl::Return:			return "Return"; break;
		case Emit::FlowControl::Throw:			return "Throw"; break;
	}
	return "<unknown>";
}
#endif

#ifdef	_DEBUG
String^ CopyILBinary::OperandTypeName (Emit::OpCode pOpCode)
{
	switch (pOpCode.OperandType)
	{
		case Emit::OperandType::InlineBrTarget:			return "InlineBrTarget"; break;
		case Emit::OperandType::InlineField:			return "InlineField"; break;
		case Emit::OperandType::InlineI:				return "InlineI"; break;
		case Emit::OperandType::InlineI8:				return "InlineI8"; break;
		case Emit::OperandType::InlineMethod:			return "InlineMethod"; break;
		case Emit::OperandType::InlineNone:				return "InlineNone"; break;
		case Emit::OperandType::InlinePhi:				return "InlinePhi"; break;
		case Emit::OperandType::InlineR:				return "InlineR"; break;
		case Emit::OperandType::InlineSig:				return "InlineSig"; break;
		case Emit::OperandType::InlineString:			return "InlineString"; break;
		case Emit::OperandType::InlineSwitch:			return "InlineSwitch"; break;
		case Emit::OperandType::InlineTok:				return "InlineTok"; break;
		case Emit::OperandType::InlineType:				return "InlineType"; break;
		case Emit::OperandType::InlineVar:				return "InlineVar"; break;
		case Emit::OperandType::ShortInlineBrTarget:	return "ShortInlineBrTarget"; break;
		case Emit::OperandType::ShortInlineI:			return "ShortInlineI"; break;
		case Emit::OperandType::ShortInlineR:			return "ShortInlineR"; break;
		case Emit::OperandType::ShortInlineVar:			return "ShortInlineVar"; break;
	}
	return "<unknown>";
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_DEBUG
bool CopyILBinary::OpHasToken (LPBYTE pOpCode)
{
	if	(
			(pOpCode[0] == 0x28)
		||	(pOpCode[0] == 0x29)
		||	(pOpCode[0] == 0x27)
		||	(pOpCode[0] == 0x8C)
		||	(pOpCode[0] == 0x6F)
		||	(pOpCode[0] == 0x74)
		||	(pOpCode[0] == 0x70)
		||	(pOpCode[0] == 0x75)
		||	(pOpCode[0] == 0xA3)
		||	(pOpCode[0] == 0x8F)
		||	(pOpCode[0] == 0x7B)
		||	(pOpCode[0] == 0x7C)
		||	(pOpCode[0] == 0x71)
		||	(pOpCode[0] == 0x7E)
		||	(pOpCode[0] == 0x7F)
		||	(pOpCode[0] == 0x72)
		||	(pOpCode[0] == 0xD0)
		||	(pOpCode[0] == 0xC6)
		||	(pOpCode[0] == 0x8D)
		||	(pOpCode[0] == 0x73)
		||	(pOpCode[0] == 0xC2)
		||	(pOpCode[0] == 0xA4)
		||	(pOpCode[0] == 0x7D)
		||	(pOpCode[0] == 0x81)
		||	(pOpCode[0] == 0x80)
		||	(pOpCode[0] == 0x79)
		||	(pOpCode[0] == 0xA5)
		||	(
				(pOpCode[0] == 0xFE)
			&&	(
					(pOpCode[1] == 0x16)
				||	(pOpCode[1] == 0x06)
				||	(pOpCode[1] == 0x15)
				||	(pOpCode[1] == 0x07)
				||	(pOpCode[1] == 0x1C)
				)
			)
		)
	{
		return true;
	}
	return false;
}
#endif

#ifdef	_DEBUG
int CopyILBinary::OpCodeSize (LPBYTE pOpCode)
{
	if	(pOpCode[0] == 0xFE)
	{
		if	(
				(pOpCode[1] == 0x19)
			||	(pOpCode[1] == 0x12)
			||	(pOpCode[1] == 0x01)
			||	(pOpCode[1] == 0x02)
			||	(pOpCode[1] == 0x03)
			||	(pOpCode[1] == 0x04)
			||	(pOpCode[1] == 0x05)
			||	(pOpCode[1] == 0x07)
			||	(pOpCode[1] == 0x11)
			||	(pOpCode[1] == 0x18)
			||	(pOpCode[1] == 0x0F)
			)
		{
			return 3;
		}
		else
		if	(
				(pOpCode[1] == 0x1E)
			||	(pOpCode[1] == 0x14)
			||	(pOpCode[1] == 0x13)
			||	(pOpCode[1] == 0x00)
			||	(pOpCode[1] == 0x1D)
			||	(pOpCode[1] == 0x1A)
			)
		{
			return 2;
		}
		else
		if	(OpHasToken (pOpCode))
		{
			return 6;
		}
		return 4;
	}
	else
	if	(OpHasToken (pOpCode))
	{
		return 5;
	}
	else
	if	(
			(
				(pOpCode[0] >= 0x2B)
			&&	(pOpCode[0] <= 0x37)
			)
		||	(pOpCode[0] == 0x0E)
		||	(pOpCode[0] == 0x0F)
		||	(pOpCode[0] == 0x1F)
		||	(pOpCode[0] == 0x11)
		||	(pOpCode[0] == 0x12)
		||	(pOpCode[0] == 0xDE)
		||	(pOpCode[0] == 0x10)
		||	(pOpCode[0] == 0x13)
		)
	{
		return 2;
	}
	else
	if	(
			(
				(pOpCode[0] >= 0x38)
			&&	(pOpCode[0] <= 0x44)
			)
		||	(pOpCode[0] == 0x20)
		||	(pOpCode[0] == 0xDD)
		)
	{
		return sizeof(DWORD)+1;
	}
	else
	if	(pOpCode[0] == 0x21)
	{
		return sizeof(DWORDLONG)+1;
	}
	else
	if	(pOpCode[0] == 0x22)
	{
		return sizeof(FLOAT)+1;
	}
	else
	if	(pOpCode[0] == 0x23)
	{
		return sizeof(DOUBLE)+1;
	}
	else
	if	(pOpCode[0] == 0x45)
	{
		return ((*(LPDWORD)(pOpCode+1))*sizeof(DWORD))+sizeof(DWORD)+1;
	}
	return 1;
}
#endif

/////////////////////////////////////////////////////////////////////////////
};