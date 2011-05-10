/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include <math.h>
#include "AgentFileBinary.h"
#include "AgentFileAcs.h"
#ifdef	__cplusplus_cli
#include "AgtErr.h"
#else
#include <shlwapi.h>
#include "DaCore.h"
#include "GuidStr.h"
#include "ImageTools.h"
#include "StringArrayEx.h"
#include "AgentFileAcf.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "ImageDebugger.h"
#include "DebugStr.h"
#include "DebugProcess.h"
#endif
#endif
#include "AgentFileDefs.inl"

#ifndef	__cplusplus_cli
IMPLEMENT_DLL_OBJECT(CAgentFileBinary)
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
namespace DoubleAgent {
namespace Character {
#endif
/////////////////////////////////////////////////////////////////////////////

CAgentFileBinary::CAgentFileBinary()
{
}

CAgentFileBinary::~CAgentFileBinary ()
{
}

#ifdef	__cplusplus_cli
CAgentFileBinary^ CAgentFileBinary::CreateInstance (const System::String^ pPath)
#else
CAgentFileBinary* CAgentFileBinary::CreateInstance (LPCTSTR pPath)
#endif
{
#ifdef	__cplusplus_cli
	return gcnew CAgentFileAcs;
#else
	if	(
			(pPath)
		&&	(_tcsicmp (PathFindExtension (pPath), sAcfFileExt) == 0)
		)
	{
		return CAgentFileAcf::CreateInstance ();
	}
	else
	{
		return CAgentFileAcs::CreateInstance ();
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
bool CAgentFileBinary::IsOpen::get ()
{
	if	(
			(mFileStream)
		&&	(
				(mFileReader)
			||	(mFileWriter)
			)
		)
	{
		return true;
	}
	return false;
}
#else
bool CAgentFileBinary::get_IsOpen () const
{
	return mFileView.SafeIsValid ();
}
#endif

#ifdef	__cplusplus_cli
bool CAgentFileBinary::IsReadOnly::get ()
{
	if	(
			(mFileStream)
		&&	(mFileWriter)
		)
	{
		return false;
	}
	return true;
}
#else
bool CAgentFileBinary::get_IsReadOnly () const
{
	return true;
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifndef	__cplusplus_cli
/////////////////////////////////////////////////////////////////////////////

INT_PTR CAgentFileBinary::FindGesture (LPCTSTR pGestureName)
{
	if	(
			(mGestures.mAnimations.GetCount() <= 0)
		&&	(IsAcsFile)
		)
	{
		ReadGestures ();
	}
	return __super::FindGesture (pGestureName);
}

const CAgentFileAnimation* CAgentFileBinary::GetGesture (INT_PTR pGestureNdx)
{
	if	(
			(mGestures.mAnimations.GetCount() <= 0)
		&&	(IsAcsFile)
		)
	{
		ReadGestures ();
	}
	return __super::GetGesture (pGestureNdx);
}

/////////////////////////////////////////////////////////////////////////////

INT_PTR CAgentFileBinary::FindAnimation (LPCTSTR pAnimationName)
{
	if	(
			(mGestures.mAnimations.GetCount() <= 0)
		&&	(IsAcsFile)
		)
	{
		ReadGestures ();
	}
	return __super::FindAnimation (pAnimationName);
}

const CAgentFileAnimation* CAgentFileBinary::GetAnimation (INT_PTR pAnimationNdx)
{
	if	(
			(mGestures.mAnimations.GetCount() <= 0)
		&&	(IsAcsFile)
		)
	{
		ReadGestures ();
	}
	return __super::GetAnimation (pAnimationNdx);
}

/////////////////////////////////////////////////////////////////////////////
#endif	// __cplusplus_cli
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAgentFileBinary::Close ()
{
	CloseFile ();
	__super::Close ();
}

void CAgentFileBinary::CloseFile ()
{
#ifdef	__cplusplus_cli
	mFileSize = 0;

	if	(mFileReader)
	{
		try
		{
			mFileReader->Close ();
		}
		catch AnyExceptionDebug
		try
		{
			mFileReader->~BinaryReader ();
		}
		catch AnyExceptionDebug
	}
	mFileReader = nullptr;

	if	(mFileWriter)
	{
		try
		{
			mFileWriter->Close ();
		}
		catch AnyExceptionDebug
		try
		{
			mFileWriter->~BinaryWriter ();
		}
		catch AnyExceptionDebug
	}
	mFileWriter = nullptr;

	if	(mFileStream)
	{
		try
		{
			mFileStream->Close ();
		}
		catch AnyExceptionDebug
		try
		{
			mFileStream->~FileStream ();
		}
		catch AnyExceptionDebug
	}
	mFileStream = nullptr;
#else
	mFileSize = 0;
	mFileView.Close ();
	mFileMapping.Close ();
	mFileHandle.Close ();
#endif
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
bool CAgentFileBinary::LoadFile (System::String^ pPath)
{
	bool	lRet = false;

#ifdef	_DEBUG_LOAD
	UINT lLogLevel = mLogLevel;
	mLogLevel = MinLogLevel (mLogLevel, _DEBUG_LOAD);
#endif
	if	(LogIsActive (mLogLevel))
	{
		LogMessage (mLogLevel, _T("Load [%s]"), _B(pPath));
	}

	CloseFile ();

	try
	{
		mFileStream = gcnew FileStream (pPath, FileMode::Open, FileAccess::Read);
	}
	catch (Exception^ pException)
	{
		__LogCliException (LogIfActive);
		throw pException;
	}

	if	(
			(mFileStream)
		&&	(mFileSize = (UInt32)mFileStream->Length)
		&&	(mFileReader = gcnew BinaryReader (mFileStream))
		)
	{
		lRet = true;
	}

	if	(lRet)
	{
		try
		{
			lRet = ReadHeader ();

			if	(lRet)
			{
				ReadNames (true);
				ReadStates ();
				ReadGestures ();
			}
		}
		catch (Exception^ pException)
		{
			try
			{
				Close ();
			}
			catch AnyExceptionDebug

			throw pException;
		}

		if	(!lRet)
		{
			Close ();
		}
	}

	if	(LogIsActive())
	{
		LogMessage (LogNormal|LogTime, _T("Load [%s] [%u]"), _B(pPath), lRet);
	}
#ifdef	_DEBUG_LOAD
	mLogLevel = lLogLevel;
#endif
	return lRet;
}
#else
HRESULT CAgentFileBinary::LoadFile (LPCTSTR pPath)
{
	HRESULT	lResult = E_FAIL;

#ifdef	_DEBUG_LOAD
	UINT lLogLevel = mLogLevel;
	mLogLevel = MinLogLevel (mLogLevel, _DEBUG_LOAD);
#endif
	if	(LogIsActive (mLogLevel))
	{
		LogMessage (mLogLevel, _T("Load [%s]"), pPath);
	}

	mFileHandle = CreateFile (pPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if	(
			(mFileHandle.SafeIsValid ())
		&&	(mFileSize = GetFileSize (mFileHandle, NULL))
		&&	((mFileMapping = CreateFileMapping (mFileHandle, NULL, PAGE_READONLY, 0, 0, NULL)).SafeIsValid())
		&&	((mFileView = MapViewOfFile (mFileMapping, FILE_MAP_READ, 0, 0, mFileSize)).SafeIsValid())
		)
	{
		lResult = S_OK;
	}
	else
	{
		lResult = HRESULT_FROM_WIN32 (GetLastError());
		if	(SUCCEEDED (lResult))
		{
			lResult = E_FAIL;
		}
	}

	if	(SUCCEEDED (lResult))
	{
		lResult = ReadHeader ();

		if	(SUCCEEDED (lResult))
		{
			ReadNames (true);
			ReadStates ();
			ReadGestures ();
#ifdef	_DEBUG_DUMP
			if	(LogIsActive (MaxLogLevel (mLogLevel, _DEBUG_DUMP)))
			{
				LogDump (_DEBUG_DUMP, mFileView, mFileSize, 0, true);
			}
#endif
		}
		else
		{
			Close ();
		}
	}
	if	(LogIsActive())
	{
		LogComErr (((lResult==AGENTPROVERROR_MAGIC)?LogDetails:LogNormal)|LogTime, lResult, _T("Load [%s]"), pPath);
	}
#ifdef	_DEBUG_LOAD
	mLogLevel = lLogLevel;
#endif
	return lResult;
}
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
bool CAgentFileBinary::ReadHeader ()
#else
HRESULT CAgentFileBinary::ReadHeader ()
#endif
{
	mSignature = 0;

#ifdef	__cplusplus_cli
	if	(
			(mFileStream)
		&&	(mFileReader)
		)
	{
		mFileStream->Seek (0, SeekOrigin::Begin);
		mSignature = mFileReader->ReadUInt32 ();

		if	(
				(mSignature == sAcsFileSignature)
			||	(mSignature == sAcfFileSignature)
			)
		{
			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel, _T("  [%s] Signature [%8.8X]"), _B(mPath), mSignature);
			}
		}
		else
		{
			throw gcnew Exception ("The specified file is not a Microsoft Agent 2.x character file.");
		}
	}
	return true;
#else
	LPCDWORD	lSignature;

	if	(
			(mFileView.SafeIsValid ())
		&&	(lSignature = (LPCDWORD)(LPVOID)mFileView)
		)
	{
		if	(
				(*lSignature == sAcsFileSignature)
			||	(*lSignature == sAcfFileSignature)
			)
		{
			mSignature = *lSignature;
			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel, _T("  [%s] Signature [%8.8X]"), mPath, *lSignature);
			}
		}
		else
		{
#ifdef	_LOG_LOAD_ERRS
			if	(LogIsActive ())
			{
				LogMessage (_LOG_LOAD_ERRS, _T("  [%s] Invalid Signature [%8.8X] != [%8.8X]"), mPath, *lSignature, sAcsFileSignature);
			}
#endif
			return AGENTPROVERROR_MAGIC;
		}
	}
	else
	{
#ifdef	_LOG_LOAD_ERRS
		if	(LogIsActive ())
		{
			LogMessage (_LOG_LOAD_ERRS, _T("  [%s] Failed to get signature"), mPath);
		}
#endif
		return AGENTPROVERROR_MAGIC;
	}

#ifdef	_DEBUG_LOAD
	LPCULARGE_INTEGER	lBlockDefs;

	if	(
			(LogIsActive (MinLogLevel (mLogLevel, _DEBUG_LOAD)))
		&&	(mFileView.SafeIsValid ())
		&&	(lBlockDefs = (LPCULARGE_INTEGER)(((LPCBYTE)(LPVOID)mFileView)+sizeof(DWORD)))
		)
	{
		LogMessage (mLogLevel, _T("  [%s] Blocks"), mPath);
		LogMessage (mLogLevel, _T("    At [%8.8X (%u)] of [%8.8X (%u)]"), lBlockDefs[0].LowPart, lBlockDefs[0].LowPart, lBlockDefs[0].HighPart, lBlockDefs[0].HighPart);
		LogMessage (mLogLevel, _T("    At [%8.8X (%u)] of [%8.8X (%u)]"), lBlockDefs[1].LowPart, lBlockDefs[1].LowPart, lBlockDefs[1].HighPart, lBlockDefs[1].HighPart);
		LogMessage (mLogLevel, _T("    At [%8.8X (%u)] of [%8.8X (%u)]"), lBlockDefs[2].LowPart, lBlockDefs[2].LowPart, lBlockDefs[2].HighPart, lBlockDefs[2].HighPart);
		LogMessage (mLogLevel, _T("    At [%8.8X (%u)] of [%8.8X (%u)]"), lBlockDefs[3].LowPart, lBlockDefs[3].LowPart, lBlockDefs[3].HighPart, lBlockDefs[3].HighPart);
	}
#endif

	return S_OK;
#endif
}

/////////////////////////////////////////////////////////////////////////////

LPCVOID CAgentFileBinary::ReadBufferTts (LPCVOID pBuffer, bool pNullTerminated)
{
	LPCBYTE	lByte = (LPCBYTE)pBuffer;

	try
	{
#ifdef	__cplusplus_cli
		String^		lUnknownStr;
#else
		CAtlString	lUnknownStr;
#endif

#ifdef	__cplusplus_cli
		mTts = gcnew CAgentFileTts;
		mTts->mEngine = System::Guid (*(LPCDWORD)lByte, *(LPCWORD)(lByte+4), *(LPCWORD)(lByte+6), lByte[8], lByte[9], lByte[10], lByte[11], lByte[12], lByte[13], lByte[14], lByte[15]);
#else
		mTts.mEngine = *(LPCGUID)lByte;
#endif
		lByte += sizeof (GUID);
#ifdef	__cplusplus_cli
		mTts->mMode = System::Guid (*(LPCDWORD)lByte, *(LPCWORD)(lByte+4), *(LPCWORD)(lByte+6), lByte[8], lByte[9], lByte[10], lByte[11], lByte[12], lByte[13], lByte[14], lByte[15]);
#else
		mTts.mMode = *(LPCGUID)lByte;
#endif
		lByte += sizeof (GUID);

#ifdef	__cplusplus_cli
		//mTts->mSpeed = *(long*)lByte;
#else
		mTts.mSpeed = *(long*)lByte;
#endif
		lByte += sizeof (long);
#ifdef	__cplusplus_cli
		//mTts->mPitch = *(short*)lByte;
#else
		mTts.mPitch = *(short*)lByte;
#endif
		lByte += sizeof (short);

		if	(*lByte)
		{
			lByte += sizeof (BYTE);	// TRUE
#ifdef	__cplusplus_cli
			mTts->mLanguage = *(LPCWORD)lByte;
#else
			mTts.mLanguage = *(LPCWORD)lByte;
#endif
			lByte += sizeof (WORD);
			lByte = (LPCBYTE)ReadBufferString (lByte, pNullTerminated, lUnknownStr);
#ifdef	__cplusplus_cli
			mTts->mGender = *(LPCWORD)lByte;
#else
			mTts.mGender = *(LPCWORD)lByte;
#endif
			lByte += sizeof (WORD);
#ifdef	__cplusplus_cli
			//mTts->mAge = *(LPCWORD)lByte;
#else
			mTts.mAge = *(LPCWORD)lByte;
#endif
			lByte += sizeof (WORD);
#ifdef	__cplusplus_cli
			lByte = (LPCBYTE)ReadBufferString (lByte, pNullTerminated, lUnknownStr); // Style
#else
			lByte = (LPCBYTE)ReadBufferString (lByte, pNullTerminated, mTts.mStyle);
#endif
		}
		else
		{
			lByte += sizeof (BYTE);	// FALSE
		}

#ifdef	__cplusplus_cli
		if	(mTts->mMode.Equals (Guid::Empty))
		{
			mTts->mModeId = nullptr;
		}
		else
		{
			mTts->mModeId = mTts->mMode.ToString ();
		}
#else
		if	(IsEqualGUID (mTts.mMode, GUID_NULL))
		{
			mTts.mModeId = NULL;
		}
		else
		{
			mTts.mModeId = ((CString)CGuidStr (mTts.mMode)).AllocSysString();
		}
#endif

		if	(LogIsActive(mLogLevel))
		{
			LogMessage (mLogLevel, _T("  [%s] Read Tts          of [%u] at [%8.8X (%u)]"), _B(mPath), lByte-(LPBYTE)pBuffer, pBuffer, pBuffer);
		}
	}
	catch AnyExceptionDebug

	return lByte;
}

#ifdef	__cplusplus_cli
LPVOID CAgentFileBinary::WriteBufferTts (LPVOID pBuffer, CAgentFileTts^ pTts, bool pNullTerminated)
{
	LPBYTE	lByte = (LPBYTE)pBuffer;

	try
	{
		const DWORD	lStrPad = (pNullTerminated) ? 1 : 0;

		if	(pBuffer)
		{
			array <BYTE>^	lBytes = pTts->mEngine.ToByteArray ();
			pin_ptr <BYTE>	lPinned = &lBytes [0];
			memcpy (lByte, lPinned, sizeof(GUID));
		}
		lByte += sizeof (GUID);
		if	(pBuffer)
		{
			array <BYTE>^	lBytes = pTts->mMode.ToByteArray ();
			pin_ptr <BYTE>	lPinned = &lBytes [0];
			memcpy (lByte, lPinned, sizeof(GUID));
		}
		lByte += sizeof (GUID);

		if	(pBuffer)
		{
			*(long*) lByte = 0; // Speed;
		}
		lByte += sizeof (long);
		if	(pBuffer)
		{
			*(short*) lByte = 0; // Pitch;
		}
		lByte += sizeof (short);

		if	(pTts->mMode.Equals (Guid::Empty))
		{
			if	(pBuffer)
			{
				*lByte = FALSE;
			}
			lByte += sizeof (BYTE);
		}
		else
		{
			if	(pBuffer)
			{
				*lByte = TRUE;
			}
			lByte += sizeof (BYTE);

			if	(pBuffer)
			{
				*(LPWORD)lByte = pTts->mLanguage;
			}
			lByte += sizeof (WORD);

			if	(pBuffer)
			{
				*(LPDWORD)lByte = 0; // Empty unknown string
			}
			lByte += sizeof (DWORD);

			if	(pBuffer)
			{
				*(LPWORD)lByte = pTts->mGender;
			}
			lByte += sizeof (WORD);

			if	(pBuffer)
			{
				*(LPWORD)lByte = 0; // Age
			}
			lByte += sizeof (WORD);

			if	(pBuffer)
			{
				*(LPDWORD)lByte = 0; // Empty style
			}
			lByte += sizeof (DWORD);
		}

		if	(LogIsActive(mLogLevel))
		{
			LogMessage (mLogLevel, _T("  [%s] Write Tts         of [%u] at [%8.8X (%u)]"), _B(mPath), lByte-(LPBYTE)pBuffer, pBuffer, pBuffer);
		}
	}
	catch AnyExceptionDebug

	return lByte;
}
#endif

/////////////////////////////////////////////////////////////////////////////

LPCVOID CAgentFileBinary::ReadBufferBalloon (LPCVOID pBuffer, bool pNullTerminated)
{
	LPCBYTE	lByte = (LPCBYTE)pBuffer;

	try
	{
#ifdef	__cplusplus_cli
		String^		lFontName = nullptr;
		long		lFontHeight = 0;
		int			lFontStyle = (int)System::Drawing::FontStyle::Regular;
#else
		CAtlString	lFontName;
#endif

#ifdef	__cplusplus_cli
		mBalloon = gcnew CAgentFileBalloon;
		mBalloon->mLines = *lByte;
#else
		mBalloon.mLines = *lByte;
#endif
		lByte++;
#ifdef	__cplusplus_cli
		mBalloon->mPerLine = *lByte;
#else
		mBalloon.mPerLine = *lByte;
#endif
		lByte++;
#ifdef	__cplusplus_cli
//		mBalloon->mFgColor = System::Drawing::ColorTranslator::FromWin32 (*(LPCOLORREF)lByte);
		mBalloon->mFgColor = System::Drawing::Color::FromArgb (255, (int)lByte[2], (int)lByte[1], (int)lByte[0]); // BGRA
#else
		mBalloon.mFgColor = *(LPCDWORD)lByte;
#endif
		lByte += sizeof(DWORD);
#ifdef	__cplusplus_cli
//		mBalloon->mBkColor = System::Drawing::ColorTranslator::FromWin32 (*(LPCOLORREF)lByte);
		mBalloon->mBkColor = System::Drawing::Color::FromArgb (255, (int)lByte[2], (int)lByte[1], (int)lByte[0]); // BGRA
#else
		mBalloon.mBkColor = *(LPCDWORD)lByte;
#endif
		lByte += sizeof(DWORD);
#ifdef	__cplusplus_cli
//		mBalloon->mBrColor = System::Drawing::ColorTranslator::FromWin32 (*(LPCOLORREF)lByte);
		mBalloon->mBrColor = System::Drawing::Color::FromArgb (255, (int)lByte[2], (int)lByte[1], (int)lByte[0]); // BGRA
#else
		mBalloon.mBrColor = *(LPCDWORD)lByte;
#endif
		lByte += sizeof(DWORD);

		lByte = (LPCBYTE)ReadBufferString (lByte, pNullTerminated, lFontName);
#ifndef	__cplusplus_cli
		_tcsncpy (mBalloon.mFont.lfFaceName, lFontName, (sizeof(mBalloon.mFont.lfFaceName)/sizeof(WCHAR))-1);
#endif
#ifdef	__cplusplus_cli
		lFontHeight = labs (*(long*) lByte);
#else
		mBalloon.mFont.lfHeight = *(long*) lByte;
#endif
		lByte += sizeof (long);
#ifdef	__cplusplus_cli
		if	((*(LPCWORD)lByte) >= FW_BOLD)
		{
			lFontStyle |= (int)System::Drawing::FontStyle::Bold;
		}
#else
		mBalloon.mFont.lfWeight = ((*(LPCWORD)lByte) >= FW_BOLD) ? FW_BOLD : FW_NORMAL;
#endif
		lByte += sizeof (WORD);
#ifdef	__cplusplus_cli
		if	(*lByte)
		{
			lFontStyle |= (int)System::Drawing::FontStyle::Strikeout;
		}
#else
		mBalloon.mFont.lfStrikeOut = *lByte; // Unsure, and where is underline?
#endif
		lByte += sizeof (WORD);
#ifdef	__cplusplus_cli
		if	(*lByte)
		{
			lFontStyle |= (int)System::Drawing::FontStyle::Italic;
		}
#else
		mBalloon.mFont.lfItalic = *lByte;
#endif
		lByte += sizeof (WORD);

#ifdef	__cplusplus_cli
		try
		{
			mBalloon->mFont = gcnew System::Drawing::Font (lFontName, (float)lFontHeight, (FontStyle)lFontStyle, GraphicsUnit::Pixel);
		}
		catch AnyExceptionDebug
#else
		mBalloon.mFont.lfCharSet = DEFAULT_CHARSET;
#endif

		if	(LogIsActive(mLogLevel))
		{
			LogMessage (mLogLevel, _T("  [%s] Read Balloon      of [%u] at [%8.8X (%u)]"), _B(mPath), lByte-(LPBYTE)pBuffer, pBuffer, pBuffer);
		}
	}
	catch AnyExceptionDebug

	return lByte;
}

#ifdef	__cplusplus_cli
LPVOID CAgentFileBinary::WriteBufferBalloon (LPVOID pBuffer, CAgentFileBalloon^ pBalloon, bool pNullTerminated)
{
	LPBYTE				lByte = (LPBYTE)pBuffer;
	String^				lFontName = String::Empty;
	long				lFontHeight = 0;
	WORD				lFontWeight = FW_NORMAL;
	WORD				lFontItalic = FALSE;

	if	(pBuffer)
	{
		*lByte = (BYTE)pBalloon->mLines;
	}
	lByte++;
	if	(pBuffer)
	{
		*lByte = (BYTE)pBalloon->mPerLine;
	}
	lByte++;
	if	(pBuffer)
	{
		COLORREF lColor = pBalloon->mFgColor.ToArgb();
		//*(COLORREF*)lByte = RGB (GetBValue (lColor), GetGValue(lColor), GetRValue(lColor)); // BGR;
		*(COLORREF*)lByte = lColor & 0x00FFFFFF;
	}
	lByte += sizeof(DWORD);
	if	(pBuffer)
	{
		COLORREF lColor = pBalloon->mBkColor.ToArgb();
		//*(COLORREF*)lByte = RGB (GetBValue (lColor), GetGValue(lColor), GetRValue(lColor)); // BGR;
		*(COLORREF*)lByte = lColor & 0x00FFFFFF;
	}
	lByte += sizeof(DWORD);
	if	(pBuffer)
	{
		COLORREF lColor = pBalloon->mBrColor.ToArgb();
		//*(COLORREF*)lByte = RGB (GetBValue (lColor), GetGValue(lColor), GetRValue(lColor)); // BGR;
		*(COLORREF*)lByte = lColor & 0x00FFFFFF;
	}
	lByte += sizeof(DWORD);

	if	(pBalloon->mFont)
	{
		lFontName = (String::IsNullOrEmpty (pBalloon->mFont->OriginalFontName)) ? pBalloon->mFont->Name : pBalloon->mFont->OriginalFontName;
		lFontHeight = (long)(ceil(pBalloon->Font->GetHeight(96.0f)));
		if	(pBalloon->mFont->Bold)
		{
			lFontWeight = FW_BOLD;
		}
		if	(pBalloon->mFont->Italic)
		{
			lFontItalic = TRUE;
		}
		
		//if	(pBalloon->Font->Unit != System::Drawing::GraphicsUnit::Pixel)
		//{
		//	try
		//	{
		//		System::Drawing::FontConverter::FontUnitConverter^	lConverter = gcnew System::Drawing::FontConverter::FontUnitConverter();
		//		
		//		bool lCanConvertTo = lConverter->CanConvertTo (System::Drawing::GraphicsUnit::typeid);
		//		bool lCanConvertFrom = lConverter->CanConvertFrom (System::Drawing::GraphicsUnit::typeid);
		//		bool lIsValid = lConverter->IsValid (pBalloon->Font->Unit);
		//	}
		//	catch AnyExceptionDebug
		//}	
	}

	if	(pBuffer)
	{
		lByte = (LPBYTE)WriteBufferString (lByte, pNullTerminated, lFontName);
	}
	else
	{
		lByte += (DWORD)WriteBufferString (NULL, pNullTerminated, lFontName);
	}

	if	(pBuffer)
	{
		*(long*)lByte = lFontHeight;
	}
	lByte += sizeof (long);
	if	(pBuffer)
	{
		*(LPWORD)lByte = lFontWeight;
	}
	lByte += sizeof (WORD);
	if	(pBuffer)
	{
		*(LPWORD)lByte = FALSE; // Strikeout
	}
	lByte += sizeof (WORD);
	if	(pBuffer)
	{
		*(LPWORD)lByte = lFontItalic;
	}
	lByte += sizeof (WORD);

	if	(LogIsActive(mLogLevel))
	{
		LogMessage (mLogLevel, _T("  [%s] Write Balloon     of [%u] at [%8.8X (%u)]"), _B(mPath), lByte-(LPBYTE)pBuffer, pBuffer, pBuffer);
	}
	return lByte;
}
#endif

/////////////////////////////////////////////////////////////////////////////

LPCVOID CAgentFileBinary::ReadBufferPalette (LPCVOID pBuffer)
{
	LPCBYTE	lByte = (LPCBYTE)pBuffer;
	DWORD	lPaletteSize;

	lPaletteSize = *(LPCDWORD)lByte;
	lByte += sizeof(DWORD);
	if	(lPaletteSize > 0)
	{
#ifdef	__cplusplus_cli
		System::Drawing::Bitmap^		lBitmap;
		array <System::Drawing::Color>^	lColors;
		int								lNdx;

		if	(
				(lBitmap = gcnew System::Drawing::Bitmap (1,1, System::Drawing::Imaging::PixelFormat::Format8bppIndexed))
			&&	(mHeader->mPalette = lBitmap->Palette)
			&&	(lColors = mHeader->mPalette->Entries)
			&&	(lColors->Length == 256)
			)
		{

			for (lNdx = 0; lNdx < 256; lNdx++)
			{
				//lColors [lNdx] = System::Drawing::ColorTranslator::FromWin32 (*(LPCOLORREF)lByte);
				lColors [lNdx] = System::Drawing::Color::FromArgb (255, (int)lByte[2], (int)lByte[1], (int)lByte[0]); // BGRA
				lByte += sizeof(COLORREF);
			}
		}
		else
		{
			mHeader->mPalette = nullptr;
		}
#else
		if	(mHeader.mPalette = new COLORREF [256])
		{
			memcpy ((LPCOLORREF)mHeader.mPalette, lByte, min (lPaletteSize, 256) * sizeof(COLORREF));
		}
		lByte += lPaletteSize * sizeof(COLORREF);
#endif
	}
	else
	{
#ifdef	__cplusplus_cli
		mHeader->mPalette = nullptr;
#else
		mHeader.mPalette = NULL;
#endif
	}

	if	(LogIsActive (mLogLevel))
	{
		LogMessage (mLogLevel, _T("  [%s] Read Palette      of [%u] at [%8.8X (%u)]"), _B(mPath), lByte-(LPCBYTE)pBuffer, pBuffer, pBuffer);
	}
#ifdef	_SAVE_PALETTE
	if	(
			(mHeader.Palette)
		&&	(LogIsActive (MaxLogLevel (mLogLevel, _SAVE_PALETTE)))
		)
	{
		DumpPalette (mHeader.Palette);
	}
#else
#ifdef	_DUMP_PALETTE
	if	(
			(mHeader.Palette)
		&&	(LogIsActive (MaxLogLevel (mLogLevel, _DUMP_PALETTE)))
		)
	{
		DumpPalette (mHeader.Palette);
	}
#endif
#endif
	return lByte;
}

#ifdef	__cplusplus_cli
LPVOID CAgentFileBinary::WriteBufferPalette (LPVOID pBuffer, CAgentFileHeader^ pHeader)
{
	LPBYTE	lByte = (LPBYTE)pBuffer;

	try
	{
		if	(
				(pHeader->mPalette)
			&&	(pHeader->mPalette->Entries->Length == 256)
			)
		{
			array <System::Drawing::Color>^	lColors = pHeader->mPalette->Entries;
			int								lNdx;

			if	(pBuffer)
			{
				*(LPDWORD)lByte = pHeader->mPalette->Entries->Length;
			}
			lByte += sizeof(DWORD);

			if	(pBuffer)
			{
				for (lNdx = 0; lNdx < pHeader->mPalette->Entries->Length; lNdx++)
				{
					COLORREF lColor = pHeader->mPalette->Entries[lNdx].ToArgb();
					//*(COLORREF*)lByte = RGB (GetBValue (lColor), GetGValue(lColor), GetRValue(lColor)) >> 8; // BGR;
					*(COLORREF*)lByte = lColor & 0x00FFFFFF;
					lByte += sizeof(DWORD);
				}
			}
			else
			{
				lByte += sizeof(DWORD) * pHeader->mPalette->Entries->Length;
			}
		}
		else
		{
			*(LPDWORD)lByte = 0;
			lByte += sizeof(DWORD);
		}

		if	(LogIsActive (mLogLevel))
		{
			LogMessage (mLogLevel, _T("  [%s] Write Palette     of [%u] at [%8.8X (%u)]"), _B(mPath), lByte-(LPCBYTE)pBuffer, pBuffer, pBuffer);
		}
	}
	catch AnyExceptionDebug

	return lByte;
}
#endif

/////////////////////////////////////////////////////////////////////////////

LPCVOID CAgentFileBinary::ReadBufferIcon (LPCVOID pBuffer)
{
	LPCBYTE	lByte = (LPCBYTE)pBuffer;

#ifdef	__cplusplus_cli
	if	(mHeader->mIcon)
	{
		DestroyIcon ((HICON)(INT_PTR)mHeader->mIcon->Handle);
	}
	mHeader->mIcon = nullptr;
#else
	if	(mHeader.mIcon)
	{
		DestroyIcon (mHeader.mIcon);
	}
	mHeader.mIcon = NULL;
#endif

	if	(*(lByte++))
	{
		DWORD			lBitsSize;
		LPBITMAPINFO	lMaskBitmapInfo;
		LPBITMAPINFO	lColorBitmapInfo;
		LPCBYTE			lMaskBits;
		LPCBYTE			lColorBits;
		UINT			lPaletteSize = 0;
		tS <ICONINFO>	lIconInfo;
		CMemDCHandle	lDC;

		lBitsSize = *(LPCDWORD)lByte;
		lByte += sizeof(DWORD);
		lMaskBitmapInfo = (LPBITMAPINFO)lByte;
		lMaskBits = lByte + lMaskBitmapInfo->bmiHeader.biSize;
		lPaletteSize = (lMaskBitmapInfo->bmiHeader.biClrUsed) ? lMaskBitmapInfo->bmiHeader.biClrUsed : (lMaskBitmapInfo->bmiHeader.biBitCount == 1) ? 2 : (lMaskBitmapInfo->bmiHeader.biBitCount == 4) ? 16 : (lMaskBitmapInfo->bmiHeader.biBitCount == 8) ? 256 : 0;
		//LogMessage (LogIfActive, _T("    At [%u] Mask  [%d] Header [%d] Bits [%d]"), lByte-sizeof(DWORD)-(LPBYTE)pBuffer, lBitsSize, lMaskBitmapInfo->bmiHeader.biSize + (sizeof(DWORD) * lPaletteSize), lBitsSize - lMaskBitmapInfo->bmiHeader.biSize - (sizeof(DWORD) * lPaletteSize));
		//LogMessage (LogIfActive, _T("      Size [%d] Width {%d] Height [%d] Bits [%d] Palette [%d (%d)]"), lMaskBitmapInfo->bmiHeader.biSizeImage, lMaskBitmapInfo->bmiHeader.biWidth, lMaskBitmapInfo->bmiHeader.biHeight, lMaskBitmapInfo->bmiHeader.biBitCount, lMaskBitmapInfo->bmiHeader.biClrUsed, lPaletteSize);
		lMaskBits += sizeof(DWORD) * lPaletteSize;
		lByte += lBitsSize;

		lBitsSize = *(LPCDWORD)lByte;
		lByte += sizeof(DWORD);
		lColorBitmapInfo = (LPBITMAPINFO)lByte;
		lColorBits = lByte + lColorBitmapInfo->bmiHeader.biSize;
		lPaletteSize = (lColorBitmapInfo->bmiHeader.biClrUsed) ? lColorBitmapInfo->bmiHeader.biClrUsed : (lColorBitmapInfo->bmiHeader.biBitCount == 1) ? 2 : (lColorBitmapInfo->bmiHeader.biBitCount == 4) ? 16 : (lColorBitmapInfo->bmiHeader.biBitCount == 8) ? 256 : 0;
		//LogMessage (LogIfActive, _T("    At [%u] Color [%d] Header [%d] Bits [%d]"), lByte-sizeof(DWORD)-(LPBYTE)pBuffer, lBitsSize, lColorBitmapInfo->bmiHeader.biSize + (sizeof(DWORD) * lPaletteSize), lBitsSize - lMaskBitmapInfo->bmiHeader.biSize - (sizeof(DWORD) * lPaletteSize));
		//LogMessage (LogIfActive, _T("      Size [%d] Width {%d] Height [%d] Bits [%d] Palette [%d (%d)]"), lColorBitmapInfo->bmiHeader.biSizeImage, lColorBitmapInfo->bmiHeader.biWidth, lColorBitmapInfo->bmiHeader.biHeight, lColorBitmapInfo->bmiHeader.biBitCount, lColorBitmapInfo->bmiHeader.biClrUsed, lPaletteSize);
		lColorBits += sizeof(DWORD) * lPaletteSize;
		lByte += lBitsSize;

#ifdef	_DUMP_ICON
		if	(LogIsActive (MaxLogLevel (mLogLevel, _DUMP_ICON)))
		{
			CImageDebugger::DumpBitmap (_DUMP_ICON, *lMaskBitmapInfo, (LPBYTE)lMaskBits, _T("Icon Mask"));
			CImageDebugger::DumpBitmap (_DUMP_ICON, *lColorBitmapInfo, (LPBYTE)lColorBits, _T("Icon Color"));
		}
#endif
#ifdef	_TRACE_RESOURCES
		if	(LogIsActive (_TRACE_RESOURCES))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentFile::CreateIcon [%p]"), this, mHeader.mIcon);
		}
#endif
#ifdef	__cplusplus_cli
		HICON	lIcon = NULL;
#endif

		if	(lDC.Attach (::CreateDC (_T("DISPLAY"), NULL, NULL, NULL)))
		{
			lIconInfo.hbmMask = ::CreateDIBitmap (lDC, &lMaskBitmapInfo->bmiHeader, CBM_INIT, lMaskBits, lMaskBitmapInfo, DIB_RGB_COLORS);
			lIconInfo.hbmColor = ::CreateDIBitmap (lDC, &lColorBitmapInfo->bmiHeader, CBM_INIT, lColorBits, lColorBitmapInfo, DIB_RGB_COLORS);

			lIconInfo.fIcon = TRUE;
#ifdef	__cplusplus_cli
			lIcon = ::CreateIconIndirect (&lIconInfo);
#else
			mHeader.mIcon = CreateIconIndirect (&lIconInfo);
#endif
			lDC.Close ();
		}
		if	(lIconInfo.hbmMask)
		{
			::DeleteObject (lIconInfo.hbmMask);
		}
		if	(lIconInfo.hbmColor)
		{
			::DeleteObject (lIconInfo.hbmColor);
		}

#ifdef	__cplusplus_cli
		if	(lIcon)
		{
			try
			{
				mHeader->mIcon = System::Drawing::Icon::FromHandle ((IntPtr)lIcon);
			}
			catch AnyExceptionDebug
		}
#endif
#ifdef	_TRACE_RESOURCES
		if	(LogIsActive (_TRACE_RESOURCES))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentFile::CreateIcon [%p] Done"), this, mHeader.mIcon);
		}
#endif
	}

	if	(LogIsActive (mLogLevel))
	{
		LogMessage (mLogLevel, _T("  [%s] Read Icon         of [%u] at [%8.8X (%u)]"), _B(mPath), lByte-(LPCBYTE)pBuffer, pBuffer, pBuffer);
	}

#ifdef	_M_CEE_NOT
if	(mHeader->mIcon)
{
	try
	{
		String^ lPath = "C:\\Users\\Don\\Desktop";
		System::IO::FileStream^	lStream;
		lPath = System::IO::Path::Combine (lPath, System::IO::Path::GetFileNameWithoutExtension (mPath));
		lPath = lPath + " - Icon.ico";
		lStream = gcnew FileStream (lPath, FileMode::Create, FileAccess::Write, FileShare::Read);
		mHeader->mIcon->Save (lStream);
		lStream->Close ();
	}
	catch AnyExceptionDebug
}
#endif

#ifdef	_SAVE_ICON
	if	(
			(mHeader.mIcon)
		&&	(LogIsActive (MaxLogLevel (mLogLevel, _SAVE_ICON)))
		)
	{
		CAtlString	lDumpName (mPath);

		PathStripPath (lDumpName.GetBuffer (lDumpName.GetLength ()));
		PathRemoveExtension (lDumpName.GetBuffer (lDumpName.GetLength ()));
		lDumpName.ReleaseBuffer ();
		CImageDebugger::SaveIcon (mHeader.mIcon, lDumpName);
	}
#endif
	return lByte;
}

#ifdef	__cplusplus_cli
LPVOID CAgentFileBinary::WriteBufferIcon (LPVOID pBuffer, CAgentFileHeader^ pHeader)
{
	LPBYTE	lByte = (LPBYTE)pBuffer;

	try
	{
		if	(
				(pHeader->mIcon)
			&&	((INT_PTR)pHeader->mIcon->Handle != 0)
			)
		{
			tS <ICONINFO>	lIconInfo;
			tS <BITMAP>		lMaskBitmap;
			tS <BITMAP>		lColorBitmap;
			array <BYTE>^	lBitmapInfoBuffer;
			pin_ptr <BYTE>	lBitmapInfoPinned;
			LPBITMAPINFO	lBitmapInfo;
			UINT			lPaletteSize = 0;
			CMemDCHandle	lDC;

			if	(pBuffer)
			{
				*lByte = TRUE;
			}
			lByte++;
			::GetIconInfo ((HICON)(INT_PTR)pHeader->mIcon->Handle, &lIconInfo);

			lDC.Attach (CreateDC (_T("DISPLAY"), NULL, NULL, NULL));

			try
			{
				if	(lIconInfo.hbmMask)
				{
					::GetObject (lIconInfo.hbmMask, sizeof(BITMAP), &lMaskBitmap);

					lPaletteSize = (lMaskBitmap.bmBitsPixel == 1) ? 2 : (lMaskBitmap.bmBitsPixel == 4) ? 16 : (lMaskBitmap.bmBitsPixel == 8) ? 256 : 0;
					lBitmapInfoBuffer = gcnew array <BYTE> (sizeof(BITMAPINFOHEADER) + (sizeof(DWORD) * lPaletteSize));
					lBitmapInfoPinned = &lBitmapInfoBuffer[0];
					lBitmapInfo = (LPBITMAPINFO)(LPBYTE)lBitmapInfoPinned;

					lBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
					lBitmapInfo->bmiHeader.biCompression = BI_RGB;
					lBitmapInfo->bmiHeader.biWidth = lMaskBitmap.bmWidth;
					lBitmapInfo->bmiHeader.biHeight = lMaskBitmap.bmHeight;
					lBitmapInfo->bmiHeader.biBitCount = lMaskBitmap.bmBitsPixel;
					lBitmapInfo->bmiHeader.biPlanes = lMaskBitmap.bmPlanes;
					lBitmapInfo->bmiHeader.biSizeImage = lMaskBitmap.bmHeight * lMaskBitmap.bmWidthBytes;
					lBitmapInfo->bmiHeader.biClrUsed = lPaletteSize;
					::GetDIBits (lDC, lIconInfo.hbmMask, 0, lBitmapInfo->bmiHeader.biHeight, NULL, lBitmapInfo, DIB_RGB_COLORS);
					lBitmapInfo->bmiHeader.biClrUsed = max (lBitmapInfo->bmiHeader.biClrUsed, lPaletteSize);

					//LogMessage (LogIfActive, _T("    At [%u] Mask  [%d] Header [%d] Bits [%d]"), lByte-(LPBYTE)pBuffer, sizeof(BITMAPINFOHEADER) + (sizeof(DWORD) * lBitmapInfo->bmiHeader.biClrUsed) + lBitmapInfo->bmiHeader.biSizeImage, sizeof(BITMAPINFOHEADER) + (sizeof(DWORD) * lBitmapInfo->bmiHeader.biClrUsed), lBitmapInfo->bmiHeader.biSizeImage);
					//LogMessage (LogIfActive, _T("      Size [%d] Width {%d] Height [%d] Bits [%d] Palette [%d]"), lBitmapInfo->bmiHeader.biSizeImage, lBitmapInfo->bmiHeader.biWidth, lBitmapInfo->bmiHeader.biHeight, lBitmapInfo->bmiHeader.biBitCount, lBitmapInfo->bmiHeader.biClrUsed);
					if	(pBuffer)
					{
						*(LPDWORD)lByte = sizeof(BITMAPINFOHEADER) + (sizeof(DWORD) * lBitmapInfo->bmiHeader.biClrUsed) + lBitmapInfo->bmiHeader.biSizeImage;
					}
					lByte += sizeof(DWORD);

					if	(pBuffer)
					{
						memcpy (lByte, &lBitmapInfo->bmiHeader, sizeof(BITMAPINFOHEADER) + (sizeof(DWORD) * lBitmapInfo->bmiHeader.biClrUsed));
					}
					lByte += sizeof(BITMAPINFOHEADER) + (sizeof(DWORD) * lBitmapInfo->bmiHeader.biClrUsed);

					if	(pBuffer)
					{
						array <BYTE>^	lMaskBytes = gcnew array <BYTE> (lBitmapInfo->bmiHeader.biSizeImage);
						pin_ptr <BYTE>	lPinned = &lMaskBytes[0];

						::GetDIBits (lDC, lIconInfo.hbmMask, 0, lBitmapInfo->bmiHeader.biHeight, lPinned, lBitmapInfo, DIB_RGB_COLORS);
						memcpy (lByte, lPinned, lBitmapInfo->bmiHeader.biSizeImage);
					}
					lByte += lBitmapInfo->bmiHeader.biSizeImage;
				}
				else
				{
					if	(pBuffer)
					{
						*(LPDWORD)lByte = 0;
					}
					lByte += sizeof(DWORD);
				}

				if	(lIconInfo.hbmColor)
				{
					::GetObject (lIconInfo.hbmColor, sizeof(BITMAP), &lColorBitmap);

					lPaletteSize = (lColorBitmap.bmBitsPixel == 1) ? 2 : (lColorBitmap.bmBitsPixel == 4) ? 16 : (lColorBitmap.bmBitsPixel == 8) ? 256 : 0;
					lBitmapInfoBuffer = gcnew array <BYTE> (sizeof(BITMAPINFOHEADER) + (sizeof(DWORD) * lPaletteSize));
					lBitmapInfoPinned = &lBitmapInfoBuffer[0];
					lBitmapInfo = (LPBITMAPINFO)(LPBYTE)lBitmapInfoPinned;

					lBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
					lBitmapInfo->bmiHeader.biCompression = BI_RGB;
					lBitmapInfo->bmiHeader.biWidth = lColorBitmap.bmWidth;
					lBitmapInfo->bmiHeader.biHeight = lColorBitmap.bmHeight;
					lBitmapInfo->bmiHeader.biBitCount = lColorBitmap.bmBitsPixel;
					lBitmapInfo->bmiHeader.biPlanes = lColorBitmap.bmPlanes;
					lBitmapInfo->bmiHeader.biSizeImage = lColorBitmap.bmHeight * lColorBitmap.bmWidthBytes;
					lBitmapInfo->bmiHeader.biClrUsed = lBitmapInfo->bmiHeader.biClrUsed;
					::GetDIBits (lDC, lIconInfo.hbmColor, 0, lBitmapInfo->bmiHeader.biHeight, NULL, lBitmapInfo, DIB_RGB_COLORS);
					lBitmapInfo->bmiHeader.biClrUsed = max (lBitmapInfo->bmiHeader.biClrUsed, lPaletteSize);

					//LogMessage (LogIfActive, _T("    At [%u] Color [%d] Header [%d] Bits [%d]"), lByte-(LPBYTE)pBuffer, sizeof(BITMAPINFOHEADER) + (sizeof(DWORD) * lBitmapInfo->bmiHeader.biClrUsed) + lBitmapInfo->bmiHeader.biSizeImage, sizeof(BITMAPINFOHEADER) + (sizeof(DWORD) * lBitmapInfo->bmiHeader.biClrUsed), lBitmapInfo->bmiHeader.biSizeImage);
					//LogMessage (LogIfActive, _T("      Size [%d] Width {%d] Height [%d] Bits [%d] Palette [%d]"), lBitmapInfo->bmiHeader.biSizeImage, lBitmapInfo->bmiHeader.biWidth, lBitmapInfo->bmiHeader.biHeight, lBitmapInfo->bmiHeader.biBitCount, lBitmapInfo->bmiHeader.biClrUsed);
					if	(pBuffer)
					{
						*(LPDWORD)lByte = sizeof(BITMAPINFOHEADER) + (sizeof(DWORD) * lBitmapInfo->bmiHeader.biClrUsed) + lBitmapInfo->bmiHeader.biSizeImage;
					}
					lByte += sizeof(DWORD);

					if	(pBuffer)
					{
						memcpy (lByte, &lBitmapInfo->bmiHeader, sizeof(BITMAPINFOHEADER) + (sizeof(DWORD) * lBitmapInfo->bmiHeader.biClrUsed));
					}
					lByte += sizeof(BITMAPINFOHEADER) + (sizeof(DWORD) * lBitmapInfo->bmiHeader.biClrUsed);

					if	(pBuffer)
					{
						array <BYTE>^	lColorBytes = gcnew array <BYTE> (lBitmapInfo->bmiHeader.biSizeImage);
						pin_ptr <BYTE>	lPinned = &lColorBytes[0];

						::GetDIBits (lDC, lIconInfo.hbmColor, 0, lBitmapInfo->bmiHeader.biHeight, lPinned, lBitmapInfo, DIB_RGB_COLORS);
						memcpy (lByte, lPinned, lBitmapInfo->bmiHeader.biSizeImage);
					}
					lByte += lBitmapInfo->bmiHeader.biSizeImage;
				}
				else
				{
					if	(pBuffer)
					{
						*(LPDWORD)lByte = 0;
					}
					lByte += sizeof(DWORD);
				}
			}
			catch AnyExceptionDebug

			if	(lIconInfo.hbmMask)
			{
				::DeleteObject (lIconInfo.hbmMask);
			}
			if	(lIconInfo.hbmColor)
			{
				::DeleteObject (lIconInfo.hbmColor);
			}
			lDC.Close ();
		}
		else
		{
			if	(pBuffer)
			{
				*lByte = FALSE;
			}
			lByte++;
		}

		if	(LogIsActive (mLogLevel))
		{
			LogMessage (mLogLevel, _T("  [%s] Write Icon        of [%u] at [%8.8X (%u)]"), _B(mPath), lByte-(LPCBYTE)pBuffer, pBuffer, pBuffer);
		}
	}
	catch AnyExceptionDebug

	return lByte;
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LPCVOID CAgentFileBinary::ReadBufferNames (LPCVOID pBuffer, DWORD pBufferSize, bool pNullTerminated, bool pFirstLetterCaps)
{
	LPCBYTE	lByte = (LPCBYTE)pBuffer;

	try
	{
		WORD					lNameCount;
#ifdef	__cplusplus_cli
		CAgentFileName^			lName;
#else
		tPtr <CAgentFileName>	lName;
#endif

		lNameCount = *(LPCWORD)lByte;
		lByte += sizeof (WORD);

		while	(
					(lNameCount > 0)
				&&	(
						(pBufferSize == 0)
					||	(lByte < ((LPCBYTE)pBuffer + pBufferSize))
					)
				)
		{
#ifdef	__cplusplus_cli
			if	(!mNames)
			{
				mNames = gcnew CAgentFileNames (this);
			}
			lName = gcnew CAgentFileName (this, mNames);
#else
			lName = new CAgentFileName;
#endif

			lName->mLanguage = *(LPCWORD)lByte;
			lByte += sizeof (WORD);

			lByte = (LPCBYTE)ReadBufferString (lByte, pNullTerminated, lName->mName);
			lByte = (LPCBYTE)ReadBufferString (lByte, pNullTerminated, lName->mDesc1);
			lByte = (LPCBYTE)ReadBufferString (lByte, pNullTerminated, lName->mDesc2);

			if	(pFirstLetterCaps)
			{
#ifdef	__cplusplus_cli
				if	(!lName->mName->IsNullOrEmpty (lName->mName))
				{
					lName->mName = String::Concat (lName->mName->Substring (0,1)->ToUpper() + lName->mName->Substring (1));
				}
#else
				CAtlString	lCharName ((BSTR) lName->mName);

				if	(
						(!lCharName.IsEmpty ())
					&&	(_istlower (lCharName [0]))
					)
				{
					lCharName.SetAt (0, _totupper (lCharName [0]));
					lName->mName = lCharName.AllocSysString();
				}
#endif
			}

			if	(LogIsActive(mLogLevel))
			{
				LogMessage (mLogLevel|LogHighVolume, _T("    NameCount [%hu] Language [%4.4hX] Name [%s]"), lNameCount, lName->mLanguage, _B(lName->mName));
			}

			lNameCount--;
#ifdef	__cplusplus_cli
			mNames->Add (lName);
#else
			mNames.Add (lName.Detach ());
#endif
		}

		if	(LogIsActive(mLogLevel))
		{
			LogMessage (mLogLevel, _T("  [%s] Read Names        of [%u (%u)] at [%8.8X (%u)]"), _B(mPath), lByte-(LPBYTE)pBuffer, pBufferSize, pBuffer, pBuffer);
		}
	}
	catch AnyExceptionDebug

	return lByte;
}

#ifdef	__cplusplus_cli
LPVOID CAgentFileBinary::WriteBufferNames (LPVOID pBuffer, CAgentFileNames^ pNames, bool pNullTerminated)
{
	LPBYTE	lByte = (LPBYTE)pBuffer;

	try
	{
		if	(pBuffer)
		{
			*(LPWORD)lByte = pNames->Count;
		}
		lByte += sizeof (WORD);

		for each (CAgentFileName^ lName in pNames)
		{
			if	(pBuffer)
			{
				*(LPWORD)lByte = lName->mLanguage;
			}
			lByte += sizeof (WORD);

			if	(pBuffer)
			{
				lByte = (LPBYTE)WriteBufferString (lByte, pNullTerminated, lName->mName);
				lByte = (LPBYTE)WriteBufferString (lByte, pNullTerminated, lName->mDesc1);
				lByte = (LPBYTE)WriteBufferString (lByte, pNullTerminated, lName->mDesc2);
			}
			else
			{
				lByte += (DWORD)WriteBufferString (NULL, pNullTerminated, lName->mName);
				lByte += (DWORD)WriteBufferString (NULL, pNullTerminated, lName->mDesc1);
				lByte += (DWORD)WriteBufferString (NULL, pNullTerminated, lName->mDesc2);
			}
		}

		if	(LogIsActive(mLogLevel))
		{
			LogMessage (mLogLevel, _T("  [%s] Write Names       of [%u] at [%8.8X (%u)]"), _B(mPath), lByte-(LPBYTE)pBuffer, pBuffer, pBuffer);
		}
	}
	catch AnyExceptionDebug

	return lByte;
}
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
CAgentFileName^ CAgentFileBinary::FindName (WORD pLangID)
#else
CAgentFileName* CAgentFileBinary::FindName (WORD pLangID)
#endif
{
#ifdef	__cplusplus_cli
	if	(
			(
				(!mNames)
			||	(mNames->Count <= 0)
			)
		&&	(IsOpen)
		)
	{
		ReadNames ();
	}
#else
	if	(
			(mNames.GetCount() <= 0)
		&&	(IsOpen)
		)
	{
		ReadNames ();
	}
#endif
	return __super::FindName (pLangID);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LPCVOID CAgentFileBinary::ReadBufferStates (LPCVOID pBuffer, DWORD pBufferSize, bool pNullTerminated)
{
	LPCBYTE	lByte = (LPCBYTE)pBuffer;

	try
	{
		WORD	lStateCount;

		lStateCount = *(LPCWORD)lByte;
		lByte += sizeof (WORD);

		while	(
					(lStateCount > 0)
				&&	(
						(pBufferSize == 0)
					||	(lByte < ((LPCBYTE)pBuffer + pBufferSize))
					)
				)
		{
#ifdef	__cplusplus_cli
			String^			lState;
			List <String^>^	lGestures;
			String^			lGesture;
#else
			CAtlString		lState;
			INT_PTR			lStateNdx;
			CAtlString		lGesture;
#endif

			lByte = (LPCBYTE)ReadBufferString (lByte, pNullTerminated, lState);
#ifdef	__cplusplus_cli
			lGestures = gcnew List<String^>;
#else
			lStateNdx = FindSortedString (mStates.mNames, lState);

			if	(lStateNdx < 0)
			{
				mStates.mGestures.InsertAt (lStateNdx = AddSortedString (mStates.mNames, lState), CAtlStringArray());
			}
#endif

#ifndef	__cplusplus_cli
			CAtlStringArray&	lGestures  = mStates.mGestures [lStateNdx];
#endif
			WORD				lGestureCount;

			lGestureCount = *(LPCWORD)lByte;
			lByte += sizeof (WORD);

			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel|LogHighVolume, _T("    State [%s] Gestures [%u]"), _B(lState), lGestureCount);
			}

			while	(
						(lGestureCount > 0)
					&&	(
							(pBufferSize == 0)
						||	(lByte < ((LPCBYTE)pBuffer + pBufferSize))
						)
					)
			{
				lByte = (LPCBYTE)ReadBufferString (lByte, pNullTerminated, lGesture);
#ifdef	__cplusplus_cli
				lGestures->Add (lGesture);
#else
				lGestures.Add (lGesture);
#endif
				if	(LogIsActive (mLogLevel))
				{
					LogMessage (mLogLevel|LogHighVolume, _T("      Gesture [%s]"), _B(lGesture));
				}
				lGestureCount--;
			}
			lStateCount--;

#ifdef	__cplusplus_cli
			mStates->Add (lState, lGestures->ToArray());
#endif
		}

		if	(LogIsActive(mLogLevel))
		{
			LogMessage (mLogLevel, _T("  [%s] Read States       of [%u (%u)] at [%8.8X (%u)]"), _B(mPath), lByte-(LPBYTE)pBuffer, pBufferSize, pBuffer, pBuffer);
		}
	}
	catch AnyExceptionDebug

	return lByte;
}

#ifdef	__cplusplus_cli
LPVOID CAgentFileBinary::WriteBufferStates (LPVOID pBuffer, CAgentFileStates^ pStates, bool pNullTerminated)
{
	LPBYTE	lByte = (LPBYTE)pBuffer;

	try
	{
		if	(pBuffer)
		{
			*(LPWORD)lByte = pStates->Count;
		}
		lByte += sizeof (WORD);

		for each (CAgentFileState^ lState in pStates)
		{
			if	(pBuffer)
			{
				lByte = (LPBYTE)WriteBufferString (lByte, pNullTerminated, lState->StateName);
			}
			else
			{
				lByte += (DWORD)WriteBufferString (NULL, pNullTerminated, lState->StateName);
			}

			if	(pBuffer)
			{
				*(LPWORD)lByte = lState->AnimationNames->Length;
			}
			lByte += sizeof (WORD);

			for each (String^ lGesture in lState->AnimationNames)
			{
				if	(pBuffer)
				{
					lByte = (LPBYTE)WriteBufferString (lByte, pNullTerminated, lGesture);
				}
				else
				{
					lByte += (DWORD)WriteBufferString (NULL, pNullTerminated, lGesture);
				}
			}
		}

		if	(LogIsActive(mLogLevel))
		{
			LogMessage (mLogLevel, _T("  [%s] Write States      of [%u] at [%8.8X (%u)]"), _B(mPath), lByte-(LPBYTE)pBuffer, pBuffer, pBuffer);
		}
	}
	catch AnyExceptionDebug

	return lByte;
}
#endif

#ifndef	__cplusplus_cli
INT_PTR CAgentFileBinary::FindState (LPCTSTR pStateName)
{
	if	(
			(IsOpen)
		&&	(mStates.mGestures.GetCount() <= 0)
		)
	{
		ReadStates ();
	}
	return __super::FindState (pStateName);
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
LPCVOID CAgentFileBinary::ReadBufferString (LPCVOID pBuffer, bool pNullTerminated, System::String^% pString)
{
	LPCBYTE	lByte = (LPCBYTE)pBuffer;
	DWORD	lStrLen;

	lStrLen = *(LPCDWORD)lByte;
	lByte += sizeof(DWORD);

	if	(lStrLen > 0)
	{
#ifdef	_DEBUG
		if	(
				(lStrLen > 2048)
			||	(IsBadStringPtr ((LPCWSTR)lByte, 2048))
			)
		{
			LogComErr (LogIfActive, HRESULT_FROM_WIN32 (ERROR_INVALID_DATA));
		}
#endif
		pString = gcnew String ((LPCWSTR)lByte, 0, lStrLen);
		lByte += lStrLen * sizeof(wchar_t);
		if	(pNullTerminated)
		{
			lByte += sizeof(wchar_t);
		}
	}
	else
	{
		pString = String::Empty;
	}
	return lByte;
}

LPVOID CAgentFileBinary::WriteBufferString (LPVOID pBuffer, bool pNullTerminated, System::String^ pString)
{
	LPBYTE				lByte = (LPBYTE)pBuffer;
	DWORD				lStrLen = 0;
	array<wchar_t>^		lStrChars;
	pin_ptr<wchar_t>	lStrPinned;

	if	(!String::IsNullOrEmpty (pString))
	{
		lStrLen = (DWORD)pString->Length;
	}
	if	(pBuffer)
	{
		*(LPDWORD)lByte = lStrLen;
	}
	lByte += sizeof(DWORD);

	if	(lStrLen > 0)
	{
		if	(pBuffer)
		{
			lStrChars = pString->ToCharArray();
			lStrPinned = &lStrChars[0];
			memcpy (lByte, lStrPinned, lStrLen * sizeof(wchar_t));
		}
		lByte += lStrLen * sizeof(wchar_t);
		if	(pNullTerminated)
		{
			if	(pBuffer)
			{
				*(wchar_t*)lByte = 0;
			}
			lByte += sizeof(wchar_t);
		}
	}
	return lByte;
}
#else
LPCVOID CAgentFileBinary::ReadBufferString (LPCVOID pBuffer, bool pNullTerminated, CAtlString& pString)
{
	LPCBYTE		lByte = (LPCBYTE)pBuffer;
	DWORD		lStrLen;

	lStrLen = *(LPCDWORD)lByte;
	lByte += sizeof(DWORD);

	if	(lStrLen > 0)
	{
#ifdef	_DEBUG
		if	(
				(lStrLen > 2048)
			||	(IsBadStringPtr ((LPCWSTR)lByte, 2048))
			)
		{
			AtlThrow (HRESULT_FROM_WIN32 (ERROR_INVALID_DATA));
		}
#endif
		pString = CAtlString ((LPCWSTR)lByte, lStrLen);
		lByte += lStrLen * sizeof(WCHAR);
		if	(pNullTerminated)
		{
			lByte += sizeof(WCHAR);
		}
	}
	else
	{
		pString.Empty ();
	}
	return lByte;
}

LPCVOID CAgentFileBinary::ReadBufferString (LPCVOID pBuffer, bool pNullTerminated, tBstrPtr& pString)
{
	LPCVOID		lRet;
	CAtlString	lString;

	lRet = ReadBufferString (pBuffer, pNullTerminated, lString);
	if	(lString.IsEmpty())
	{
		pString.Free();
	}
	else
	{
		pString = lString.AllocSysString ();
	}
	return lRet;
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentFileBinary::DecodeImage (LPCVOID pSrcBits, ULONG pSrcCount, LPBYTE pTrgBits, ULONG pTrgCount, long pTrgWidth, long pTrgHeight)
{
	bool	lRet;

#ifdef	_SHOW_IMAGE
	CImageDebugger		lDebugger;
	tArrayPtr <BYTE>	lDebugInfoBuffer;
	LPBITMAPINFO		lDebugInfo = NULL;

	if	(
			(pTrgWidth > 0)
		&&	(pTrgHeight > 0)
		&&	(lDebugInfoBuffer = new BYTE [GetImageFormat (NULL)])
		&&	(lDebugInfo = (LPBITMAPINFO) (LPBYTE) lDebugInfoBuffer)
		)
	{
		GetImageFormat (lDebugInfo);
		lDebugInfo->bmiHeader.biWidth = pTrgWidth;
		lDebugInfo->bmiHeader.biHeight = pTrgHeight;
		lDebugInfo->bmiHeader.biSizeImage = pTrgCount;
	}
	memset (pTrgBits, 0xFF, pTrgCount);
#endif

	if	(DecodeData (pSrcBits, pSrcCount, pTrgBits, pTrgCount) == pTrgCount)
	{
		lRet = true;
	}

#ifdef	_SHOW_IMAGE
	if	(
			(lRet)
		&&	(lDebugInfo)
		)
	{
		CBitmapHandle	lBitmap;
		LPVOID			lBits = NULL;

		if	(lBitmap.Attach (CreateDIBSection (NULL, lDebugInfo, DIB_RGB_COLORS, &lBits, NULL, 0)))
		{
			memcpy (lBits, pTrgBits, pTrgCount);
			GdiFlush ();

			lDebugger.ShowBitmap (lBitmap, (pTrgHeight<=32)?4:(pTrgHeight<=64)?2:1, _T("Decode"), false);
			switch	(lDebugger.DoModal ())
			{
				case IDCANCEL:	lDebugInfo = NULL; break;
				case IDCLOSE:	ExitProcess (IDCANCEL);
			}
		}
	}
#endif

	return lRet;
}

ULONG CAgentFileBinary::DecodeData (LPCVOID pSrcData, ULONG pSrcSize, LPVOID pTrgData, ULONG pTrgSize)
{
	LPCBYTE	lSrcPtr = (LPCBYTE)pSrcData;
	LPCBYTE	lSrcEnd = lSrcPtr + pSrcSize;
	LPBYTE	lTrgPtr = (LPBYTE)pTrgData;
	LPBYTE	lTrgEnd = lTrgPtr + pTrgSize;
	DWORD	lSrcQuad;
	BYTE	lTrgByte;
	DWORD	lBitCount = 0;
	DWORD	lSrcOffset;
	DWORD	lRunLgth;
	DWORD	lRunCount;

	if	(
			(pSrcSize <= 7)
		||	(*lSrcPtr != 0)
		)
	{
		return 0;
	}

	for	(lBitCount = 1; (*(lSrcEnd-lBitCount) == 0xFF); lBitCount++)
	{
		if	(lBitCount > 6)
		{
			break;
		}
	}
	if	(lBitCount < 6)
	{
		return 0;
	}

	lBitCount = 0;
	lSrcPtr += 5;

	while	(
				(lSrcPtr < lSrcEnd)
			&&	(lTrgPtr < lTrgEnd)
			)
	{
		lSrcQuad = *(LPCDWORD)(lSrcPtr - sizeof(DWORD));

		if	(lSrcQuad & (1 << LOWORD(lBitCount)))
		{
			lSrcOffset = 1;

			if	(lSrcQuad & (1 << LOWORD(lBitCount+1)))
			{
				if	(lSrcQuad & (1 << LOWORD(lBitCount+2)))
				{
					if	(lSrcQuad & (1 << LOWORD(lBitCount+3)))
					{
						lSrcQuad >>= LOWORD(lBitCount+4);
						lSrcQuad &= 0x000FFFFF;
						if	(lSrcQuad == 0x000FFFFF)
						{
							break;
						}
						lSrcQuad += 4673;
						lBitCount += 24;

						lSrcOffset = 2;
					}
					else
					{
						lSrcQuad >>= LOWORD(lBitCount+4);
						lSrcQuad &= 0x00000FFF;
						lSrcQuad += 577;
						lBitCount += 16;
					}
				}
				else
				{
					lSrcQuad >>= LOWORD(lBitCount+3);
					lSrcQuad &= 0x000001FF;
					lSrcQuad += 65;
					lBitCount += 12;
				}
			}
			else
			{
				lSrcQuad >>= LOWORD(lBitCount+2);
				lSrcQuad &= 0x0000003F;
				lSrcQuad += 1;
				lBitCount += 8;
			}

			lSrcPtr += (lBitCount/8);
			lBitCount &= 7;
			lRunLgth = *(LPCDWORD)(lSrcPtr - sizeof(DWORD));
			lRunCount = 0;
			while	(lRunLgth & (1 << LOWORD(lBitCount+lRunCount)))
			{
				lRunCount++;
				if	(lRunCount > 11)
				{
					break;
				}
			}

			lRunLgth >>= LOWORD(lBitCount+lRunCount+1);
			lRunLgth &= (1 << lRunCount) - 1;
			lRunLgth += 1 << lRunCount;
			lRunLgth += lSrcOffset;
			lBitCount += lRunCount*2+1;

			if	(lTrgPtr + lRunLgth > lTrgEnd)
			{
				break;
			}
			if	(lTrgPtr - lSrcQuad < pTrgData)
			{
				break;
			}
			while	((long)lRunLgth > 0)
			{
				lTrgByte = *(lTrgPtr - lSrcQuad);
				*(lTrgPtr++) = lTrgByte;
				lRunLgth--;
			}
		}
		else
		{
			lSrcQuad >>= LOWORD(lBitCount+1);
			lBitCount += 9;

			lTrgByte = LOBYTE(lSrcQuad);
			*(lTrgPtr++) = lTrgByte;
		}

		lSrcPtr += lBitCount/8;
		lBitCount &= 7;
	}

#ifdef	_DEBUG
	if	(lTrgPtr - (LPBYTE)pTrgData != pTrgSize)
	{
		LogMessage (LogDebug, _T("Decoded [%u] of [%u]"), lTrgPtr - (LPBYTE)pTrgData, pTrgSize);
	}
#endif
	return (ULONG)(lTrgPtr - (LPBYTE)pTrgData);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifndef	__cplusplus_cli
/////////////////////////////////////////////////////////////////////////////

void CAgentFileBinary::DumpBlocks (UINT pLogLevel, UINT pMaxBlockSize)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			DWORD				lFileSize;
			CGenericHandle		lFileMapping;
			CMappedHandle		lFileView;
			LPCULARGE_INTEGER	lBlockDefs;
			DWORD				lBlockOffset;
			DWORD				lBlockLength;
			LPCBYTE				lBlock;

			lFileSize = GetFileSize (mFileHandle, NULL);
			lFileMapping = CreateFileMapping (mFileHandle, NULL, PAGE_READONLY, 0, 0, NULL);
			lFileView = MapViewOfFile (lFileMapping, FILE_MAP_READ, 0, 0, lFileSize);

			try
			{
				if	(
						(lFileView.SafeIsValid ())
					&&	(lBlockDefs = (LPCULARGE_INTEGER)(((LPCBYTE)(LPVOID)lFileView)+sizeof(DWORD)))
					&&	(lBlockLength = lBlockDefs[0].HighPart)
					&&	(lBlock = ((LPCBYTE)(LPVOID)lFileView)+(lBlockOffset=lBlockDefs[0].LowPart))
					)
				{
					LPCBYTE	lByte = lBlock;
					LPCBYTE	lPrevByte = lBlock;
					DWORD	lStrLen;
					DWORD	lPaletteSize;
					DWORD	lIconSize;
					WORD	lStateCount;
					LPCBYTE	lNamesStart = ((LPCDWORD)lBlock) [1] - lBlockOffset + lBlock;
					WORD	lNameCount;
					WORD	lNameNdx;

					LogMessage (pLogLevel, _T("Blocks [%u] at [%8.8X] (%u) of (%u)"), lBlockLength, lBlockOffset, lBlockOffset, lFileSize);

					lByte += 41;
					LogMessage (pLogLevel|LogNoPrefix|LogHighVolume, _T("  Header Block [%u] at [%8.8X] [%8.8X]"), lByte-lPrevByte, lPrevByte-lBlock, lBlockOffset);
					LogDump (pLogLevel|LogNoPrefix|LogHighVolume, (LPBYTE)lPrevByte, min(lByte-lPrevByte,pMaxBlockSize), _T("    "), true);
					lPrevByte = lByte;

					if	(mHeader.mStyle & CharStyleTts)
					{
						lByte += sizeof (GUID) + sizeof (GUID) + sizeof (long) + sizeof (short) + sizeof (BYTE) + sizeof (WORD);
						lStrLen = *(LPCDWORD)lByte;
						if	(lStrLen > 0)
						{
							lByte += (lStrLen + 1) * sizeof(WCHAR);
						}
						lByte += sizeof(DWORD) + sizeof (WORD) + sizeof (WORD);
						lStrLen = *(LPCDWORD)lByte;
						if	(lStrLen > 0)
						{
							lByte += (lStrLen + 1) * sizeof(WCHAR);
						}
						lByte += sizeof(DWORD);

						LogMessage (pLogLevel|LogNoPrefix|LogHighVolume, _T("  Tts Block [%u] at [%8.8X]"), lByte-lPrevByte, lPrevByte-lBlock);
						LogDump (pLogLevel|LogNoPrefix|LogHighVolume, (LPBYTE)lPrevByte, min(lByte-lPrevByte,pMaxBlockSize), _T("    "), true);
						lPrevByte = lByte;
					}

					if	(mHeader.mStyle & CharStyleBalloon)
					{
						lByte += sizeof (BYTE) + sizeof (BYTE) + sizeof(DWORD) + sizeof(DWORD) + sizeof(DWORD);
						lStrLen = *(LPCDWORD)lByte;
						if	(lStrLen > 0)
						{
							lByte += (lStrLen + 1) * sizeof(WCHAR);
						}
						lByte += sizeof(DWORD) + sizeof (long) + sizeof (WORD) + sizeof(DWORD);

						LogMessage (pLogLevel|LogNoPrefix|LogHighVolume, _T("  Balloon Block [%u] at [%8.8X]"), lByte-lPrevByte, lPrevByte-lBlock);
						LogDump (pLogLevel|LogNoPrefix|LogHighVolume, (LPBYTE)lPrevByte, min(lByte-lPrevByte,pMaxBlockSize), _T("    "), true);
						lPrevByte = lByte;
					}

					lPaletteSize = *(LPCDWORD)lByte;
					lByte += sizeof(DWORD);
					lPrevByte = lByte;
					lByte += lPaletteSize * sizeof(DWORD);

					LogMessage (pLogLevel|LogNoPrefix|LogHighVolume, _T("  Palette [%u] at [%8.8X]"), lPaletteSize, lPrevByte-lBlock);
					LogDump (pLogLevel|LogNoPrefix|LogHighVolume, (LPBYTE)lPrevByte, lPaletteSize*sizeof(DWORD), _T("    "), true);
					lPrevByte = lByte;

					if	(*(lByte++))
					{
						lIconSize = *(LPCDWORD)lByte;
						lByte += sizeof(DWORD);
						lPrevByte = lByte;
						lByte += lIconSize;
						lIconSize += *(LPCDWORD)lByte;
						lIconSize += sizeof(DWORD);
						lByte += *(LPCDWORD)lByte;
						lByte += sizeof(DWORD);

						LogMessage (pLogLevel|LogNoPrefix|LogHighVolume, _T("  Icon [%u] at [%8.8X]"), lIconSize, lPrevByte-lBlock);
						LogDump (pLogLevel|LogNoPrefix|LogHighVolume, (LPBYTE)lPrevByte, lIconSize, _T("    "), true);
					}
					lPrevByte = lByte;

					lStateCount = *(LPCWORD)lByte;
					lByte += sizeof (WORD);

					LogMessage (pLogLevel|LogNoPrefix|LogHighVolume, _T("  [%u] States [%u] at [%8.8X]"), lStateCount, lNamesStart-lPrevByte, lPrevByte-lBlock);
					LogDump (pLogLevel|LogNoPrefix|LogHighVolume, (LPBYTE)lPrevByte, min(lNamesStart-lPrevByte,pMaxBlockSize), _T("    "), true);
					lPrevByte = lNamesStart;

					lByte = lNamesStart;
					lNameCount = *(LPCWORD)lByte;
					lByte += sizeof (WORD);

					for (lNameNdx = 0; lNameNdx < lNameCount; lNameNdx++)
					{
						lByte += sizeof (WORD); // Language

						lStrLen = *(LPCDWORD)lByte;
						lByte += sizeof(DWORD);
						if	(lStrLen > 0)
						{
							lByte += (lStrLen + 1) * sizeof(WCHAR);
						}
						lStrLen = *(LPCDWORD)lByte;
						lByte += sizeof(DWORD);
						if	(lStrLen > 0)
						{
							lByte += (lStrLen + 1) * sizeof(WCHAR);
						}
						lStrLen = *(LPCDWORD)lByte;
						lByte += sizeof(DWORD);
						if	(lStrLen > 0)
						{
							lByte += (lStrLen + 1) * sizeof(WCHAR);
						}
					}

					LogMessage (pLogLevel|LogNoPrefix|LogHighVolume, _T("  [%u] Names [%u] at [%8.8X] [%8.8X]"), lNameCount, lByte-lNamesStart, lNamesStart-lBlock, lNamesStart+lBlockOffset-lBlock);
					LogDump (pLogLevel|LogNoPrefix|LogHighVolume, (LPBYTE)lNamesStart, min(lByte-lNamesStart,pMaxBlockSize), _T("    "), true);
					lPrevByte = lByte;
				}
			}
			catch AnyExceptionDebug

			try
			{
				if	(
						(lFileView.SafeIsValid ())
					&&	(lBlockDefs = (LPCULARGE_INTEGER)(((LPCBYTE)(LPVOID)lFileView)+sizeof(DWORD)))
					&&	(lBlockLength = lBlockDefs[1].HighPart)
					&&	(lBlock = ((LPCBYTE)(LPVOID)lFileView)+(lBlockOffset=lBlockDefs[1].LowPart))
					)
				{
					LogMessage (pLogLevel, _T("  [%u] Gestures [%u] at [%8.8X] (%u)"), *(LPCDWORD)lBlock, lBlockLength, lBlockOffset, lBlockOffset);
					LogDump (pLogLevel|LogNoPrefix|LogHighVolume, (LPBYTE)lBlock, min(lBlockLength,pMaxBlockSize), _T("    "), true);
				}
			}
			catch AnyExceptionDebug

			try
			{
				if	(
						(lFileView.SafeIsValid ())
					&&	(lBlockDefs = (LPCULARGE_INTEGER)(((LPCBYTE)(LPVOID)lFileView)+sizeof(DWORD)))
					&&	(lBlockLength = lBlockDefs[2].HighPart)
					&&	(lBlock = ((LPCBYTE)(LPVOID)lFileView)+(lBlockOffset=lBlockDefs[2].LowPart))
					)
				{
					LogMessage (pLogLevel, _T("  [%u] Images [%u] at [%8.8X] (%u)"), *(LPCDWORD)lBlock, lBlockLength, lBlockOffset, lBlockOffset);
					LogDump (pLogLevel|LogNoPrefix|LogHighVolume, (LPBYTE)lBlock, min(lBlockLength,pMaxBlockSize), _T("    "), true);
				}
			}
			catch AnyExceptionDebug

			try
			{
				if	(
						(lFileView.SafeIsValid ())
					&&	(lBlockDefs = (LPCULARGE_INTEGER)(((LPCBYTE)(LPVOID)lFileView)+sizeof(DWORD)))
					&&	(lBlockLength = lBlockDefs[3].HighPart)
					&&	(lBlock = ((LPCBYTE)(LPVOID)lFileView)+(lBlockOffset=lBlockDefs[3].LowPart))
					)
				{
					LogMessage (pLogLevel, _T("  [%u] Sounds [%u] at [%8.8X] (%u)"), *(LPCDWORD)lBlock, lBlockLength, lBlockOffset, lBlockOffset);
					LogDump (pLogLevel|LogNoPrefix|LogHighVolume, (LPBYTE)lBlock, min(lBlockLength,pMaxBlockSize), _T("    "), true);
				}
			}
			catch AnyExceptionDebug

			lFileView.Close ();
			lFileMapping.Close ();
		}
		catch AnyExceptionDebug
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAgentFileBinary::DumpPalette (LPVOID pPalette)
{
#ifdef	_DEBUG
	tArrayPtr <BYTE>	lInfoBuff;
	BITMAPINFO *		lInfo;
	ATL::CImage			lDumpBmp;
	LPBYTE				lDumpBits;

	lInfoBuff = new BYTE [sizeof (BITMAPINFOHEADER) + (sizeof(COLORREF) * 256)];
	lInfo = (BITMAPINFO *) (LPBYTE) lInfoBuff;

	lInfo->bmiHeader.biSize = sizeof (lInfo->bmiHeader);
	lInfo->bmiHeader.biWidth = 256;
	lInfo->bmiHeader.biHeight = -256;
	lInfo->bmiHeader.biBitCount = 8;
	lInfo->bmiHeader.biPlanes = 1;
	lInfo->bmiHeader.biCompression = BI_RGB;
	lInfo->bmiHeader.biSizeImage = lInfo->bmiHeader.biWidth * abs (lInfo->bmiHeader.biHeight);
	lInfo->bmiHeader.biClrUsed = 256;
	memcpy (lInfo->bmiColors, pPalette, sizeof(DWORD) * 256);

	lDumpBmp.Attach (CreateDIBSection (NULL, lInfo, DIB_RGB_COLORS, NULL, NULL, 0));
	if	(lDumpBits = ::GetImageBits (lDumpBmp))
	{
		for	(long lNdx = 0; lNdx < (long) lInfo->bmiHeader.biSizeImage; lNdx++)
		{
			lDumpBits [lNdx] = (BYTE) ((lNdx >> 4) & 0x000F) | ((lNdx >> 8) & 0x00F0);
		}
		GdiFlush ();

#ifdef	_SAVE_IMAGE
		CImageDebugger::SaveBitmap (lDumpBmp, _T("Palette"));
#else
#ifdef	_SAVE_PALETTE
		lDumpName += _T(" - Palette");
		CImageDebugger::SaveBitmap (lDumpBmp, lDumpName);
#endif
#endif
#ifdef	_DUMP_PALETTE
		CImageDebugger::DumpBitmap (_DUMP_PALETTE, *lInfo, lDumpBits, _T("Palette"));
#endif
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAgentFileBinary::SaveImage (CAgentFileImage* pImage)
{
#ifdef	_DEBUG
	if	(pImage)
	{
		try
		{
			ATL::CImage			lBitmap;
			tArrayPtr <BYTE>	lInfoBuffer;
			LPBITMAPINFO		lInfo;
			LPVOID				lBits = NULL;

			if	(
					(lInfoBuffer = new BYTE [GetImageFormat (NULL)])
				&&	(lInfo = (LPBITMAPINFO) (LPBYTE) lInfoBuffer)
				)
			{
				GetImageFormat (lInfo, pImage, false);

				lBitmap.Attach (CreateDIBSection (NULL, lInfo, DIB_RGB_COLORS, NULL, NULL, 0));
				if	(lBits = ::GetImageBits (lBitmap))
				{
					CAtlString	lDumpName;

					memcpy (lBits, pImage->Bits, pImage->BitsSize);
					GdiFlush ();
					lDumpName.Format (_T("Image %.4u"), pImage->ImageNum);
					CImageDebugger::SaveBitmap (lBitmap, lDumpName);
				}
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
#endif	// __cplusplus_cli
/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
} // namespace Character
} // namespace DoubleAgent
#endif
/////////////////////////////////////////////////////////////////////////////
