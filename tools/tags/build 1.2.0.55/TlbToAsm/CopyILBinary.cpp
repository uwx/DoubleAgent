#include "stdafx.h"
#include <corhdr.h>
#include "CopyILBinary.h"
#include "LogAssembly.h"

#ifdef	_DEBUG
//#define	_DEBUG_CODE			LogDebugFast
//#define	_DEBUG_TOKENS		LogDebugFast
//#define	_DEBUG_OPCODES		LogDebugFast
//#define	_DEBUG_EXCEPTIONS	LogDebugFast
#endif

/////////////////////////////////////////////////////////////////////////////

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace System::Reflection;
using namespace System::Reflection::Emit;

namespace DoubleAgent {
namespace TlbToAsm {
/////////////////////////////////////////////////////////////////////////////

CopyILBinary::CopyILBinary (Module^ pSourceModule, ModuleBuilder^ pTargetModule, TranslateILBinary^ pTranslator)
:	LogILBinary (pSourceModule, LogNormal, 0),
	mTargetModule (pTargetModule),
	mTranslator (pTranslator)
{}

MethodBody^ CopyILBinary::CopyMethodBody (MethodBase^ pSourceMethod, Emit::ILGenerator^ pGenerator)
{
	MethodBody^ lRet = nullptr;

	try
	{
		MethodCopyData^	lCopyData = gcnew MethodCopyData;

		if	(GetMethodBody (pSourceMethod, lCopyData))
		{
			lCopyData->mLabelsAt = gcnew Dictionary <int, Emit::Label>;
			lCopyData->mGenerator = pGenerator;

			if	(pSourceMethod->IsGenericMethodDefinition)
			{
				lCopyData->mGenericMethodArguments = pSourceMethod->GetGenericArguments ();
			}
			if	(pSourceMethod->DeclaringType->IsGenericTypeDefinition)
			{
				lCopyData->mGenericTypeArguments = pSourceMethod->DeclaringType->GetGenericArguments ();
			}

#ifdef	_DEBUG_CODE
			LogMessage (_DEBUG_CODE, _T("Copy Method Body [%s.%s]"), _BMT(pSourceMethod), _BM(pSourceMethod));
#endif

			ProcessMethodBody (lCopyData);

#ifdef	_DEBUG_CODE
			LogMessage (_DEBUG_CODE, _T("Copy Method Body Done"));
#endif
			lRet = lCopyData->mMethodBody;
		}
	}
	catch AnyException

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

void CopyILBinary::DefineBodyLabel (Object^ pData, int pLabelTarget)
{
	MethodCopyData^		lData = safe_cast <MethodCopyData^> (pData);

	if	(!lData->mLabelsAt->ContainsKey (pLabelTarget))
	{
		lData->mLabelsAt->Add (pLabelTarget, lData->mGenerator->DefineLabel ());
	}
}

void CopyILBinary::PutLocalVariables (Object^ pData)
{
	try
	{
		MethodCopyData^		lData = safe_cast <MethodCopyData^> (pData);
		LocalVariableInfo^	lLocalVariable;

		if	(lData->mMethodBody->LocalVariables)
		{
			for each (lLocalVariable in lData->mMethodBody->LocalVariables)
			{
				Type^	lLocalType;
				bool	lTranslated;

				lTranslated = mTranslator->TranslateType (lLocalVariable->LocalType, lLocalType);
				lData->mGenerator->DeclareLocal (lLocalType, lLocalVariable->IsPinned);

#ifdef	_DEBUG_TOKENS
				if	(lTranslated)
				{
					LogMessage (_DEBUG_TOKENS, _T("    Local Type [%s] to [%s] Pinned [%u]"), _BT(lLocalVariable->LocalType), _BT(lLocalType), lLocalVariable->IsPinned);
				}
				else
				{
					LogMessage (_DEBUG_TOKENS, _T("    Local Type [%s] Pinned [%u]"), _BT(lLocalType), lLocalVariable->IsPinned);
				}
#endif
			}
		}
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////

bool CopyILBinary::PutBodyException (Object^ pData, System::Reflection::Emit::OpCode & pOpCode, LPBYTE pOperand, int pOffset, LPBYTE pBinary)
{
	MethodCopyData^	lData = safe_cast <MethodCopyData^> (pData);

	if	(lData->mLabelsAt->ContainsKey (pOffset))
	{
		lData->mGenerator->MarkLabel (lData->mLabelsAt [pOffset]);
	}

	if	(lData->mCatchStartAt->ContainsKey (pOffset))
	{
		ExceptionHandlingClause^	lExceptionClause = lData->mCatchStartAt [pOffset];

		if	((int)lExceptionClause->Flags & (int)ExceptionHandlingClauseOptions::Finally)
		{
#ifdef	_DEBUG_EXCEPTIONS
			LogMessage (_DEBUG_EXCEPTIONS, _T("  %4.4X: Finally"), pOffset);
#endif
			lData->mGenerator->BeginFinallyBlock ();
		}
		else
		if	((int)lExceptionClause->Flags & (int)ExceptionHandlingClauseOptions::Filter)
		{
#ifdef	_DEBUG_EXCEPTIONS
			LogMessage (_DEBUG_EXCEPTIONS, _T("  %4.4X: Filter"), pOffset);
#endif
			lData->mGenerator->BeginExceptFilterBlock ();
		}
		else
		if	((int)lExceptionClause->Flags & (int)ExceptionHandlingClauseOptions::Fault)
		{
#ifdef	_DEBUG_EXCEPTIONS
			LogMessage (_DEBUG_EXCEPTIONS, _T("  %4.4X: Catch (...)"), pOffset);
#endif
			lData->mGenerator->BeginFaultBlock ();
		}
		else
		{
#ifdef	_DEBUG_EXCEPTIONS
			LogMessage (_DEBUG_EXCEPTIONS, _T("  %4.4X: Catch [%s]"), pOffset, _BT(lExceptionClause->CatchType));
#endif
			lData->mGenerator->BeginCatchBlock (lExceptionClause->CatchType);
		}
	}
	else
	if	(lData->mTryEndAt->ContainsKey (pOffset))
	{
		for	(int lNdx = 0; lNdx < lData->mTryEndAt [pOffset]; lNdx++)
		{
#ifdef	_DEBUG_EXCEPTIONS
			LogMessage (_DEBUG_EXCEPTIONS, _T("  %4.4X: Try End"), pOffset);
#endif
			lData->mGenerator->EndExceptionBlock ();
		}
	}
	else
	if	(lData->mCatchEndAt->ContainsKey (pOffset))
	{
#ifdef	_DEBUG_EXCEPTIONS
		LogMessage (_DEBUG_EXCEPTIONS, _T("  %4.4X: Catch End"), pOffset);
#endif
		lData->mGenerator->EndExceptionBlock ();
	}

	if	(lData->mTryStartAt->ContainsKey (pOffset))
	{
		for	(int lNdx = 0; lNdx < lData->mTryStartAt [pOffset]; lNdx++)
		{
#ifdef	_DEBUG_EXCEPTIONS
			LogMessage (_DEBUG_EXCEPTIONS, _T("  %4.4X: Try"), pOffset);
#endif
			lData->mGenerator->BeginExceptionBlock ();
		}
	}

	if	(
			(
				(pOpCode == Emit::OpCodes::Leave)
			||	(pOpCode == Emit::OpCodes::Leave_S)
			)
		&&	(
				(lData->mTryEndAt->ContainsKey (pOffset+pOpCode.Size+OperandSize(pOpCode)))
			||	(lData->mCatchEndAt->ContainsKey (pOffset+pOpCode.Size+OperandSize(pOpCode)))
			)
		)
	{
		return false;
	}
	else
	if	(
			(pOpCode == Emit::OpCodes::Endfinally)
		&&	(lData->mCatchEndAt->ContainsKey (pOffset+pOpCode.Size+OperandSize(pOpCode)))
		)
	{
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////

bool CopyILBinary::PutBodyOpCode (Object^ pData, OpCode & pOpCode, LPBYTE pOperand, int pOffset, LPBYTE pBinary)
{
	MethodCopyData^	lData = safe_cast <MethodCopyData^> (pData);

#ifdef	_DEBUG_OPCODES
	LogOpCode (pOpCode, pOperand, pOffset, pBinary);
#endif

	switch (pOpCode.OperandType)
	{
		case OperandType::InlineType:
		case OperandType::InlineTok:
		{
			try
			{
				lData->mGenerator->Emit (pOpCode, GetTokenType (pOpCode, *(PDWORD)pOperand, lData->mGenericTypeArguments, lData->mGenericMethodArguments));
			}
			catch AnyExceptionDebug
		}	break;
		case OperandType::InlineField:
		{
			try
			{
				lData->mGenerator->Emit (pOpCode, GetTokenField (pOpCode, *(PDWORD)pOperand, lData->mGenericTypeArguments, lData->mGenericMethodArguments));
			}
			catch AnyExceptionDebug
		}	break;
		case OperandType::InlineMethod:
		{
			try
			{
				ConstructorInfo^	lConstructor;
				MethodInfo^			lMethod;

				if	(lConstructor = GetTokenConstructor (pOpCode, *(PDWORD)pOperand, lData->mGenericTypeArguments, lData->mGenericMethodArguments))
				{
					lData->mGenerator->Emit (pOpCode, lConstructor);
				}
				else
				if	(lMethod = GetTokenMethodInfo (pOpCode, *(PDWORD)pOperand, lData->mGenericTypeArguments, lData->mGenericMethodArguments))
				{
					lData->mGenerator->Emit (pOpCode, lMethod);
				}
				else
				{
					lData->mGenerator->Emit (pOpCode);
				}
			}
			catch AnyExceptionDebug
		}	break;
		case OperandType::InlineString:
		{
			try
			{
				lData->mGenerator->Emit (pOpCode, GetTokenString (*(PDWORD)pOperand));
			}
			catch AnyExceptionDebug
		}	break;
		case OperandType::InlineI:
		{
			lData->mGenerator->Emit (pOpCode, *(Int32*)pOperand);
		}	break;
		case OperandType::InlineI8:
		{
			lData->mGenerator->Emit (pOpCode, *(Int64*)pOperand);
		}	break;
		case OperandType::InlineR:
		{
			lData->mGenerator->Emit (pOpCode, *(Double*)pOperand);
		}	break;
		case OperandType::ShortInlineI:
		{
			lData->mGenerator->Emit (pOpCode, *(Int16*)pOperand);
		}	break;
		case OperandType::ShortInlineR:
		{
			lData->mGenerator->Emit (pOpCode, *(Single*)pOperand);
		}	break;
		case OperandType::InlineVar:
		{
			lData->mGenerator->Emit (pOpCode, *(Int16*)pOperand);
		}	break;
		case OperandType::ShortInlineVar:
		{
			lData->mGenerator->Emit (pOpCode, *(SByte*)pOperand);
		}	break;
		case OperandType::InlineBrTarget:
		case OperandType::ShortInlineBrTarget:
		{
			int	lTarget = pOffset + pOpCode.Size + OperandSize(pOpCode);

			if	(pOpCode.OperandType == OperandType::ShortInlineBrTarget)
			{
				lTarget += *(SByte*)pOperand;
			}
			else
			{
				lTarget += *(Int32*)pOperand;
			}
			lData->mGenerator->Emit (pOpCode, lData->mLabelsAt [lTarget]);
		}	break;
		default:
		{
			lData->mGenerator->Emit (pOpCode);
		}	break;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////

void CopyILBinary::LogOpCode (System::Reflection::Emit::OpCode & pOpCode, LPBYTE pOperand, int pOffset, LPBYTE pBinary)
{
#ifdef	_DEBUG_OPCODES
	try
	{
		TCHAR	lOpCodeStr [8];
		String^	lOperandVal = "";

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
				TCHAR lVal [100];
				_stprintf (lVal, _T(" Token (%8.8X)"), *(PDWORD)pOperand);
				lOperandVal = gcnew String (lVal);
			}
			catch AnyExceptionSilent
		}
		else
		if	(OperandIsValue(pOpCode))
		{
			try
			{
				TCHAR lVal [100];
				_stprintf (lVal, _T(" (%d)"), (Int32)pOpCode.Value);
				lOperandVal = gcnew String (lVal);
			}
			catch AnyExceptionSilent
		}

		LogMessage (_DEBUG_OPCODES, _T("  %4.4X: OpCode [%s] Size [%d] [%s %s%s] [%s] [%s]"), pOffset, lOpCodeStr, pOpCode.Size+OperandSize(pOpCode), _B(pOpCode.Name), _B(OperandTypeName(pOpCode)), _B(lOperandVal),  _B(OpCodeTypeName(pOpCode)),  _B(OpCodeFlowName(pOpCode)));
	}
	catch AnyExceptionSilent
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

Type^ CopyILBinary::GetTokenType (OpCode & pOpCode, DWORD pToken, array<Type^>^ pGenericTypeArguments, array<Type^>^ pGenericMethodArguments)
{
	Type^	lTargetType = nullptr;
	Type^	lSourceType;

	try
	{
		if	(lSourceType = ParseILBinary::GetTokenType (pOpCode, pToken, pGenericTypeArguments, pGenericMethodArguments))
		{
			if	(
					(TypeFromToken (pToken) == mdtTypeSpec)
				||	(lSourceType->Namespace->StartsWith ("System"))
				)
			{
				lTargetType = lSourceType;
#ifdef	_DEBUG_TOKENS
				LogMessage (_DEBUG_TOKENS, _T("    Type       [%8.8X] [%s]"), pToken, _BT(lSourceType));
#endif
			}
			else
			if	(
					(mTranslator->TranslateType (lSourceType, lTargetType))
				&&	(lTargetType)
				)
			{
#ifdef	_DEBUG_TOKENS
				LogMessage (_DEBUG_TOKENS, _T("    Type       [%8.8X] [%s] to [%s]"), pToken, _BT(lSourceType), _BT(lTargetType));
#endif
			}
#ifdef	_DEBUG_TOKENS
			else
			if	(ReferenceEquals (lSourceType->Module, mSourceModule))
			{
				LogMessage (_DEBUG_TOKENS, _T("!!! Missing Type [%8.8X] [%s]"), pToken, _BT(lSourceType));
			}
#endif
		}
#ifdef	_DEBUG_TOKENS
		else
		{
			LogMessage (_DEBUG_TOKENS, _T("!!! Unknown Type [%8.8X] in [%s]"), pToken, _B(mSourceModule->Name));
		}
#endif
	}
	catch AnyExceptionSilent

	return lTargetType;
}

/////////////////////////////////////////////////////////////////////////////

MethodInfo^ CopyILBinary::GetTokenMethodInfo (OpCode & pOpCode, DWORD pToken, array<Type^>^ pGenericTypeArguments, array<Type^>^ pGenericMethodArguments)
{
	MethodInfo^	lTargetMethod = nullptr;
	MethodBase^	lSourceMethod;

	try
	{
		if	(lSourceMethod = ParseILBinary::GetTokenMethod (pOpCode, pToken, pGenericTypeArguments, pGenericMethodArguments))
		{
			if	(
					(!lSourceMethod->IsConstructor)
				&&	(mTranslator->TranslateMethod (lSourceMethod, lTargetMethod))
				&&	(lTargetMethod)
				)
			{
#ifdef	_DEBUG_TOKENS
				LogMessage (_DEBUG_TOKENS, _T("    Method     [%8.8X] [%s.%s] as [%s.%s]"), pToken, _BMT(lSourceMethod), _BM(lSourceMethod), _BMT(lTargetMethod), _BM(lTargetMethod));
#endif
			}
#ifdef	_DEBUG_TOKENS
			else
			if	(!lSourceMethod->IsConstructor)
			{
				LogMessage (_DEBUG_TOKENS, _T("!!! Missing Method [%8.8X] [%s.%s]"), pToken, _BMT(lSourceMethod), _BM(lSourceMethod));
			}
#endif
		}
#ifdef	_DEBUG_TOKENS
		else
		{
			LogMessage (_DEBUG_TOKENS, _T("!!! Unknown Method [%8.8X] in [%s]"), pToken, _B(mSourceModule->Name));
		}
#endif
	}
	catch AnyExceptionDebug

	return lTargetMethod;
}

/////////////////////////////////////////////////////////////////////////////

ConstructorInfo^ CopyILBinary::GetTokenConstructor (OpCode & pOpCode, DWORD pToken, array<Type^>^ pGenericTypeArguments, array<Type^>^ pGenericMethodArguments)
{
	ConstructorInfo^	lTargetConstructor = nullptr;
	MethodBase^			lSourceMethod = nullptr;

	try
	{
		if	(lSourceMethod = ParseILBinary::GetTokenMethod (pOpCode, pToken, pGenericTypeArguments, pGenericMethodArguments))
		{
			if	(
					(lSourceMethod->IsConstructor)
				&&	(mTranslator->TranslateConstructor (lSourceMethod, lTargetConstructor))
				&&	(lTargetConstructor)
				)
			{
#ifdef	_DEBUG_TOKENS
				LogMessage (_DEBUG_TOKENS, _T("    Method     [%8.8X] [%s.%s] as [%s.%s]"), pToken, _BMT(lSourceMethod), _BM(lSourceMethod), _BMT(lTargetConstructor), _BM(lTargetConstructor));
#endif
			}
#ifdef	_DEBUG_TOKENS
			else
			if	(lSourceMethod->IsConstructor)
			{
				LogMessage (_DEBUG_TOKENS, _T("!!! Missing Method [%8.8X] [%s.%s] (Constructor)"), pToken, _BMT(lSourceMethod), _BM(lSourceMethod));
			}
#endif
		}
#ifdef	_DEBUG_TOKENS
		else
		{
			LogMessage (_DEBUG_TOKENS, _T("!!! Unknown Method [%8.8X] in [%s]"), pToken, _B(mSourceModule->Name));
		}
#endif
	}
	catch AnyExceptionDebug

	return lTargetConstructor;
}

/////////////////////////////////////////////////////////////////////////////

FieldInfo^ CopyILBinary::GetTokenField (OpCode & pOpCode, DWORD pToken, array<Type^>^ pGenericTypeArguments, array<Type^>^ pGenericMethodArguments)
{
	FieldInfo^	lTargetField = nullptr;
	FieldInfo^	lSourceField = nullptr;

	try
	{
		if	(
				(lSourceField = ParseILBinary::GetTokenField (pOpCode, pToken, pGenericTypeArguments, pGenericMethodArguments))
			&&	(mTranslator->TranslateField (lSourceField, lTargetField))
			&&	(lTargetField)
			)
		{
#ifdef	_DEBUG_TOKENS
			LogMessage (LogDebug, _T("    Field      [%8.8X] [%s.%s] as [%s.%s]"), pToken, _BMT(lSourceField), _BM(lSourceField), _BMT(lTargetField), _BM(lTargetField));
#endif
		}
#ifdef	_DEBUG_TOKENS
		else
		if	(lSourceField == nullptr)
		{
			LogMessage (_DEBUG_TOKENS, _T("!!! Unknown Field [%8.8X] in [%s]"), pToken, _B(mSourceModule->Name));
		}
		else
		{
			LogMessage (_DEBUG_TOKENS, _T("!!! Missing Field [%8.8X] [%s.%s]"), pToken, _BMT(lSourceField), _BM(lSourceField));
		}
#endif
		if	(!lTargetField)
		{
			lTargetField = System::Reflection::Missing::typeid->GetField ("Value");
#ifdef	_DEBUG_TOKENS
			LogMessage (LogDebug, _T("    Field      [%8.8X] as [%s.%s]"), pToken, _BMT(lTargetField), _BM(lTargetField));
#endif
		}
	}
	catch AnyExceptionDebug

	return lTargetField;
}

/////////////////////////////////////////////////////////////////////////////
} // namespace TlbToAsm
} // namespace DoubleAgent
