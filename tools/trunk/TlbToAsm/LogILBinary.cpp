#include "stdafx.h"
#include "LogILBinary.h"
#include "LogAssembly.h"

/////////////////////////////////////////////////////////////////////////////

using namespace System::Reflection;
using namespace System::Reflection::Emit;
using namespace System::Collections::Generic;

namespace DoubleAgent {
namespace TlbToAsm {
/////////////////////////////////////////////////////////////////////////////

LogILBinary::LogILBinary (Module^ pSourceModule, UINT pLogLevel, int pLogIndent)
:	ParseILBinary (pSourceModule),
	mLogLevel (pLogLevel),
	mLogIndent (pLogIndent)
{}

void LogILBinary::LogMethodBody (MethodBase^ pSourceMethod)
{
	try
	{
		MethodParseData^	lParseData = gcnew MethodParseData;
		
		if	(GetMethodBody (pSourceMethod, lParseData))
		{
			LogMessage (mLogLevel, _T("%s.method %s %s (%s)"), _B(LogIndent()), _B(LogAssembly::MethodAttrsStr(pSourceMethod->Attributes)), _BM(pSourceMethod), _BMT(pSourceMethod));
			LogMessage (mLogLevel, _T("%s{"), _B(LogIndent()));
			mLogIndent++;
			LogMessage (mLogLevel, _T("%s// Code size %d (0x%X)"), _B(LogIndent()), lParseData->mBinary->Length, lParseData->mBinary->Length);
			
			ProcessMethodBody (lParseData);
			
			mLogIndent--;
			LogMessage (mLogLevel, _T("%s}"), _B(LogIndent()));
		}
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////

void LogILBinary::PutLocalVariables (Object^ pData)
{
	try
	{
		MethodParseData^	lData = safe_cast <MethodParseData^> (pData);
		LocalVariableInfo^	lLocalVariable;

		if	(
				(lData->mMethodBody->LocalVariables)
			&&	(lData->mMethodBody->LocalVariables->Count > 0)
			)
		{
			String^	lInitStr = "";
			int		lNdx;
			
			if	(lData->mMethodBody->InitLocals)
			{
				lInitStr = " init";
			}
			LogMessage (mLogLevel, _T("%s.locals%s"), _B(LogIndent()), _B(lInitStr));

			for	(lNdx = 0; lNdx < lData->mMethodBody->LocalVariables->Count; lNdx++)
			{
				lLocalVariable = lData->mMethodBody->LocalVariables [lNdx];
				LogMessage (mLogLevel, _T("%s  %s V_%d"), _B(LogIndent()), _BT(lLocalVariable->LocalType), lNdx);
			}
		}
	}
	catch AnyExceptionDebug
}

bool LogILBinary::PutBodyException (Object^ pData, System::Reflection::Emit::OpCode & pOpCode, LPBYTE pOperand, int pOffset, LPBYTE pBinary)
{
	MethodParseData^	lData = safe_cast <MethodParseData^> (pData);

	if	(lData->mTryEndAt->ContainsKey (pOffset))
	{
		for	(int lNdx = 0; lNdx < lData->mTryEndAt [pOffset]; lNdx++)
		{
			mLogIndent--;
			LogMessage (mLogLevel, _T("%s}  // end .try"), _B(LogIndent()));
		}
	}
	else
	if	(lData->mCatchEndAt->ContainsKey (pOffset))
	{
		mLogIndent--;
		LogMessage (mLogLevel, _T("%s}  // end handler"), _B(LogIndent()));
	}

	if	(lData->mCatchStartAt->ContainsKey (pOffset))
	{
		ExceptionHandlingClause^	lExceptionClause = lData->mCatchStartAt [pOffset];

		if	((int)lExceptionClause->Flags & (int)ExceptionHandlingClauseOptions::Finally)
		{
			LogMessage (mLogLevel, _T("%sfinally"), _B(LogIndent()));
		}
		else
		if	((int)lExceptionClause->Flags & (int)ExceptionHandlingClauseOptions::Filter)
		{
			LogMessage (mLogLevel, _T("%sfilter"), _B(LogIndent()));
		}
		else
		if	((int)lExceptionClause->Flags & (int)ExceptionHandlingClauseOptions::Fault)
		{
			LogMessage (mLogLevel, _T("%scatch (...)"), _B(LogIndent()));
		}
		else
		{
			LogMessage (mLogLevel, _T("%scatch (%s)"), _B(LogIndent()), _BT(lExceptionClause->CatchType));
		}
		LogMessage (mLogLevel, _T("%s{"), _B(LogIndent()));
		mLogIndent++;
	}

	if	(lData->mTryStartAt->ContainsKey (pOffset))
	{
		for	(int lNdx = 0; lNdx < lData->mTryStartAt [pOffset]; lNdx++)
		{
			LogMessage (mLogLevel, _T("%s.try"), _B(LogIndent()));
			LogMessage (mLogLevel, _T("%s{"), _B(LogIndent()));
			mLogIndent++;
		}
	}
	return true;
}

bool LogILBinary::PutBodyOpCode (Object^ pData, System::Reflection::Emit::OpCode & pOpCode, LPBYTE pOperand, int pOffset, LPBYTE pBinary)
{
	try
	{
		TCHAR	lOpBytesStr [100];
		TCHAR	lOperandStr [100];
		
		memset (lOpBytesStr, 0, sizeof(lOpBytesStr));
		memset (lOperandStr, 0, sizeof(lOperandStr));

		if	(false)
		{
			TCHAR	lOpCodeStr [8];

			if	(pBinary[pOffset] == 0xFE)
			{
				_stprintf (lOpCodeStr, _T("%2.2X%2.2X"), pBinary[pOffset], pBinary[pOffset+1]);
			}
			else
			{
				_stprintf (lOpCodeStr, _T("%2.2X  "), pBinary[pOffset]);
			}

			if	(OperandIsToken(pOpCode))
			{
				try
				{
					_stprintf (lOperandStr, _T("(%2.2X)%6.6X"), HIBYTE(HIWORD(*(PDWORD)pOperand)), (*(PDWORD)pOperand)&0x00FFFFFF);
				}
				catch AnyExceptionSilent
			}
			else
			{
				try
				{
					if	(OperandSize (pOpCode) == 8)
					{
						_stprintf (lOperandStr, _T("%16.16I64X"), *(PULONGLONG)pOperand);
					}
					else
					if	(OperandSize (pOpCode) == 4)
					{
						_stprintf (lOperandStr, _T("%8.8X"), *(PDWORD)pOperand);
					}
					else
					if	(OperandSize (pOpCode) == 2)
					{
						_stprintf (lOperandStr, _T("%4.4X"), *(PWORD)pOperand);
					}
					else
					{
						_stprintf (lOperandStr, _T("%2.2X"), *pOperand);
					}
				}
				catch AnyExceptionSilent
			}

			_stprintf (lOpBytesStr, _T(" /* %s | %-16s */"), lOpCodeStr, lOperandStr);
			memset (lOperandStr, 0, sizeof(lOperandStr));
		}

		switch (pOpCode.OperandType)
		{
			case OperandType::InlineType:
			{
				Type^ lType = GetTokenType (*(PDWORD)pOperand);
				_stprintf (lOperandStr, _T("%s"), _BT(lType));
			}	break;
			case OperandType::InlineField:
			{
				FieldInfo^ lField = GetTokenField (*(PDWORD)pOperand);
				_stprintf (lOperandStr, _T("%s.%s"), _BFT(lField), _BF(lField));
			}	break;
			case OperandType::InlineMethod:
			{
				MethodBase^ lMethod = GetTokenMethod (*(PDWORD)pOperand);
				_stprintf (lOperandStr, _T("%s.%s"), _BMT(lMethod), _BM(lMethod));
			}	break;
			case OperandType::InlineI:
			{
				_stprintf (lOperandStr, _T("%d (0x%X)"), *(PDWORD)pOperand, *(PDWORD)pOperand);
			}	break;
			case OperandType::InlineI8:
			{
				_stprintf (lOperandStr, _T("%I64d (0x%I64X)"), *(PLONGLONG)pOperand, *(PLONGLONG)pOperand);
			}	break;
			case OperandType::InlineR:
			{
				_stprintf (lOperandStr, _T("%f (0x%I64X)"), *(double*)pOperand, *(PLONGLONG)pOperand);
			}	break;
			case OperandType::ShortInlineI:
			{
				_stprintf (lOperandStr, _T("%d (0x%X)"), *(PWORD)pOperand, *(PWORD)pOperand);
			}	break;
			case OperandType::ShortInlineR:
			{
				_stprintf (lOperandStr, _T("%f (0x%X)"), *(float*)pOperand, *(PDWORD)pOperand);
			}	break;
			case OperandType::InlineVar:
			{
				_stprintf (lOperandStr, _T("%d"), *(PWORD)pOperand);
			}	break;
			case OperandType::ShortInlineVar:
			{
				_stprintf (lOperandStr, _T("%d"), *pOperand);
			}	break;
			case OperandType::InlineBrTarget:
			{
				int	lTarget = pOffset + pOpCode.Size + OperandSize(pOpCode) + *(SByte*)pOperand;
				_stprintf (lOperandStr, _T("IL_%4.4X"), *(PDWORD)lTarget);
			}	break;
			case OperandType::ShortInlineBrTarget:
			{
				int	lTarget = pOffset + pOpCode.Size + OperandSize(pOpCode) + *(Int32*)pOperand;
				_stprintf (lOperandStr, _T("IL_%4.4X"), *(PDWORD)lTarget);
			}	break;
			default:
			{
				//_stprintf (lOperandStr, _T("{%s}"), _B(OperandTypeName(pOpCode)));
			}
		}
		
		LogMessage (mLogLevel|LogHighVolume, _T("%sIL_%4.4X:%s %-10s %s"), _B(LogIndent()), pOffset, lOpBytesStr, _B(pOpCode.Name), lOperandStr);
	}
	catch AnyExceptionSilent
	
	return true;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

String^ LogILBinary::OpCodeTypeName (System::Reflection::Emit::OpCode pOpCode)
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

String^ LogILBinary::OpCodeFlowName (System::Reflection::Emit::OpCode pOpCode)
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

String^ LogILBinary::OperandTypeName (Emit::OpCode pOpCode)
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

#if	 FALSE
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
			LogMessage (_DEBUG_CODE, _T("Copy Method Body [%s.%s]"), _BMT(pSourceMethod), _BM(pSourceMethod));
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
							LogMessage (_DEBUG_TOKENS, _T("    Local Type [%s] to [%s]"), _BT(lLocalVariable->LocalType), _BT(lLocalType));
#endif
						}
#ifdef	_DEBUG_TOKENS
						else
						{
							LogMessage (_DEBUG_TOKENS, _T("    Local Type [%s]"), _BT(lLocalType));
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
							LogMessage (_DEBUG_CODE, _T("  %4.4X: Catch [%s]"), lOffset, _BT(lExceptionClause->CatchType));
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
							try
							{
								pGenerator->Emit (lOpCode, GetTokenType (*(PDWORD)lOperand));
							}
							catch AnyExceptionDebug
						}	break;
						case OperandType::InlineField:
						{
							try
							{
								pGenerator->Emit (lOpCode, GetTokenField (*(PDWORD)lOperand));
							}
							catch AnyExceptionDebug
						}	break;
						case OperandType::InlineMethod:
						{
							try
							{
								ConstructorInfo^ lConstructor = GetTokenConstructor (*(PDWORD)lOperand);
								if	(lConstructor)
								{
									pGenerator->Emit (lOpCode, lConstructor);
								}
								else
								{
									pGenerator->Emit (lOpCode, GetTokenMethod (*(PDWORD)lOperand));
								}
							}
							catch AnyExceptionDebug
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
#endif	

/////////////////////////////////////////////////////////////////////////////

String^ LogILBinary::LogIndent ()
{
	return gcnew String(' ', mLogIndent*2);
}

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
