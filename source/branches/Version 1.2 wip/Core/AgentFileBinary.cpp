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
#include "AgentFileBinary.h"
#include "AgentFileAcs.h"
#ifdef	_M_CEE
#include "HandleTemplates.h"
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

#ifndef	_M_CEE
IMPLEMENT_DLL_OBJECT(CAgentFileBinary)
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifdef	_M_CEE
namespace DoubleAgent {
namespace Character {
#endif
/////////////////////////////////////////////////////////////////////////////

CAgentFileBinary::CAgentFileBinary()
:	mFileNamesOffset (0),
	mFileStatesOffset (0)
{
}

CAgentFileBinary::~CAgentFileBinary ()
{
}

#ifdef	_M_CEE
CAgentFileBinary^ CAgentFileBinary::CreateInstance (const System::String^ pPath)
#else
CAgentFileBinary* CAgentFileBinary::CreateInstance (LPCTSTR pPath)
#endif
{
#ifdef	_M_CEE
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

#ifdef	_M_CEE
bool CAgentFileBinary::IsOpen::get ()
{
	if	(
			(mFileStream)
		&&	(mFileReader)
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

#ifdef	_M_CEE
bool CAgentFileBinary::IsReadOnly::get ()
{
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
#ifndef	_M_CEE
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
#endif	// _M_CEE
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAgentFileBinary::Close ()
{
#ifdef	_M_CEE
	mFileStream = nullptr;
	mFileReader = nullptr;
	mFileSize = 0;
#else
	mFileView.Close ();
	mFileMapping.Close ();
	mFileHandle.Close ();
	mFileSize = 0;
#endif	
	__super::Close ();
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
HRESULT CAgentFileBinary::LoadFile (System::String^ pPath, UINT pLogLevel)
{
	HRESULT	lResult = E_FAIL;

#ifdef	_DEBUG_LOAD
	pLogLevel = MinLogLevel (pLogLevel, _DEBUG_LOAD);
#endif
	if	(LogIsActive (pLogLevel))
	{
		LogMessage (pLogLevel, _T("Load [%s]"), _B(pPath));
	}

	mFileStream = nullptr;
	mFileReader = nullptr;
	mFileSize = 0;
	
	try
	{
		mFileStream = gcnew FileStream (pPath, FileMode::Open, FileAccess::Read);
	}
	catch (FileNotFoundException^)
	{
		lResult = HRESULT_FROM_WIN32 (ERROR_FILE_NOT_FOUND);
	}
	catch AnyExceptionDebug
	
	if	(
			(mFileStream)
		&&	(mFileSize = mFileStream->Length)
		&&	(mFileReader = gcnew BinaryReader (mFileStream))
		)
	{
		lResult = S_OK;
	}

	if	(SUCCEEDED (lResult))
	{
		lResult = ReadHeader (pLogLevel);

		if	(SUCCEEDED (lResult))
		{
			ReadNames (true, pLogLevel);
			ReadStates (pLogLevel);
			ReadGestures (pLogLevel);
		}
		else
		{
			Close ();
		}
	}

	if	(LogIsActive())
	{
		LogComErr (((lResult==AGENTPROVERROR_MAGIC)?LogDetails:LogNormal)|LogTime, lResult, _T("Load [%s]"), _B(pPath));
	}
	return lResult;
}
#else
HRESULT CAgentFileBinary::LoadFile (LPCTSTR pPath, UINT pLogLevel)
{
	HRESULT	lResult = E_FAIL;

#ifdef	_DEBUG_LOAD
	pLogLevel = MinLogLevel (pLogLevel, _DEBUG_LOAD);
#endif
	if	(LogIsActive (pLogLevel))
	{
		LogMessage (pLogLevel, _T("Load [%s]"), pPath);
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
		lResult = ReadHeader (pLogLevel);

		if	(SUCCEEDED (lResult))
		{
			ReadNames (true, pLogLevel);
			ReadStates (pLogLevel);
			ReadGestures (pLogLevel);
#ifdef	_DEBUG_DUMP
			if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_DUMP)))
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
	return lResult;
}
#endif

/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentFileBinary::ReadHeader ()
{
	return ReadHeader (LogVerbose+1);
}

HRESULT CAgentFileBinary::ReadHeader (UINT pLogLevel)
{
	mSignature = 0;

#ifdef	_M_CEE
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
			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel, _T("  [%s] Signature [%8.8X]"), _B(mPath), mSignature);
			}
		}
		else
		{
			return AGENTPROVERROR_MAGIC;
		}
	}
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
			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel, _T("  [%s] Signature [%8.8X]"), mPath, *lSignature);
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
			(LogIsActive (MinLogLevel (pLogLevel, _DEBUG_LOAD)))
		&&	(mFileView.SafeIsValid ())
		&&	(lBlockDefs = (LPCULARGE_INTEGER)(((LPCBYTE)(LPVOID)mFileView)+sizeof(DWORD)))
		)
	{
		LogMessage (pLogLevel, _T("  [%s] Blocks"), mPath);
		LogMessage (pLogLevel, _T("    At [%8.8X (%u)] of [%8.8X (%u)]"), lBlockDefs[0].LowPart, lBlockDefs[0].LowPart, lBlockDefs[0].HighPart, lBlockDefs[0].HighPart);
		LogMessage (pLogLevel, _T("    At [%8.8X (%u)] of [%8.8X (%u)]"), lBlockDefs[1].LowPart, lBlockDefs[1].LowPart, lBlockDefs[1].HighPart, lBlockDefs[1].HighPart);
		LogMessage (pLogLevel, _T("    At [%8.8X (%u)] of [%8.8X (%u)]"), lBlockDefs[2].LowPart, lBlockDefs[2].LowPart, lBlockDefs[2].HighPart, lBlockDefs[2].HighPart);
		LogMessage (pLogLevel, _T("    At [%8.8X (%u)] of [%8.8X (%u)]"), lBlockDefs[3].LowPart, lBlockDefs[3].LowPart, lBlockDefs[3].HighPart, lBlockDefs[3].HighPart);
	}
#endif

#endif
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

LPCVOID CAgentFileBinary::ReadBufferTts (LPCVOID pBuffer, bool pNullTerminated, UINT pLogLevel)
{
	LPCBYTE		lByte = (LPCBYTE)pBuffer;
	const DWORD	lStrPad = (pNullTerminated) ? 1 : 0;
	DWORD		lStrLen;
	LPCWSTR		lStr;

#ifdef	_M_CEE
	mTts = gcnew CAgentFileTts;
	mTts->mEngine = System::Guid (*(LPCDWORD)lByte, *(LPCWORD)(lByte+4), *(LPCWORD)(lByte+6), lByte[8], lByte[9], lByte[10], lByte[11], lByte[12], lByte[13], lByte[14], lByte[15]);
#else
	mTts.mEngine = *(LPCGUID)lByte;
#endif	
	lByte += sizeof (GUID);
#ifdef	_M_CEE
	mTts->mMode = System::Guid (*(LPCDWORD)lByte, *(LPCWORD)(lByte+4), *(LPCWORD)(lByte+6), lByte[8], lByte[9], lByte[10], lByte[11], lByte[12], lByte[13], lByte[14], lByte[15]);
#else
	mTts.mMode = *(LPCGUID)lByte;
#endif	
	lByte += sizeof (GUID);

#ifdef	_M_CEE
	//mTts->mSpeed = *(long*) lByte;
#else
	mTts.mSpeed = *(long*) lByte;
#endif	
	lByte += sizeof (long);
#ifdef	_M_CEE
	//mTts->mPitch = *(short*) lByte;
#else
	mTts.mPitch = *(short*) lByte;
#endif	
	lByte += sizeof (short);

	if	(*lByte)
	{
		lByte += sizeof (BYTE);	// TRUE
#ifdef	_M_CEE
		mTts->mLanguage = *(LPCWORD)lByte;
#else
		mTts.mLanguage = *(LPCWORD)lByte;
#endif		
		lByte += sizeof (WORD);

		lStrLen = *(LPCDWORD)lByte;
		lByte += sizeof(DWORD);
		lStr = (LPCWSTR) lByte;

#ifndef	_M_CEE
		if	(
				(lStrLen > 2048)
			||	(IsBadStringPtr (lStr, 2048))
			)
		{
			AtlThrow (HRESULT_FROM_WIN32 (ERROR_INVALID_DATA));
		}
#ifdef	_DEBUG_NOT
		CAtlString	lUnknownStr (lStr, lStrLen);
		if	(!lUnknownStr.IsEmpty ())
		{
			LogMessage (LogDebug, _T("Unknown TTS string [%s]"), lUnknownStr);
		}
#endif
#endif
		if	(lStrLen > 0)
		{
			lByte += (lStrLen + lStrPad) * sizeof (WCHAR);
		}

#ifdef	_M_CEE
		mTts->mGender = *(LPCWORD)lByte;
#else
		mTts.mGender = *(LPCWORD)lByte;
#endif		
		lByte += sizeof (WORD);
#ifdef	_M_CEE
		//mTts->mAge = *(LPCWORD)lByte;
#else
		mTts.mAge = *(LPCWORD)lByte;
#endif		
		lByte += sizeof (WORD);

		lStrLen = *(LPCDWORD)lByte;
		lByte += sizeof(DWORD);
		lStr = (LPCWSTR) lByte;
#ifdef	_M_CEE
		//mTts->mStyle = gcnew String (lStr, 0, lStrLen);
#else
		if	(
				(lStrLen > MAX_PATH)
			||	(IsBadStringPtr (lStr, MAX_PATH))
			)
		{
			AtlThrow (HRESULT_FROM_WIN32 (ERROR_INVALID_DATA));
		}
		mTts.mStyle = CAtlString (lStr, lStrLen).AllocSysString();
#endif		
		if	(lStrLen > 0)
		{
			lByte += (lStrLen + lStrPad) * sizeof (WCHAR);
		}
	}
	else
	{
		lByte += sizeof (BYTE);	// FALSE
	}

#ifdef	_M_CEE
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
	return lByte;
}

LPCVOID CAgentFileBinary::ReadBufferBalloon (LPCVOID pBuffer, bool pNullTerminated, UINT pLogLevel)
{
	LPCBYTE		lByte = (LPCBYTE)pBuffer;
	const DWORD	lStrPad = (pNullTerminated) ? 1 : 0;
	DWORD		lStrLen;
	LPCWSTR		lStr;
#ifdef	_M_CEE
	String^	lFontName = nullptr;
	long		lFontHeight = 0;
	int			lFontStyle = (int)System::Drawing::FontStyle::Regular;
#endif	

#ifdef	_M_CEE
	mBalloon = gcnew CAgentFileBalloon;
	mBalloon->mLines = *lByte;
#else
	mBalloon.mLines = *lByte;
#endif	
	lByte++;
#ifdef	_M_CEE
	mBalloon->mPerLine = *lByte;
#else
	mBalloon.mPerLine = *lByte;
#endif
	lByte++;
#ifdef	_M_CEE
//	mBalloon->mFgColor = System::Drawing::ColorTranslator::FromWin32 (*(LPCOLORREF)lByte);
	mBalloon->mFgColor = System::Drawing::Color::FromArgb (255, (int)lByte[2], (int)lByte[1], (int)lByte[0]); // BGRA
#else
	mBalloon.mFgColor = *(LPCDWORD)lByte;
#endif
	lByte += sizeof(DWORD);
#ifdef	_M_CEE
//	mBalloon->mBkColor = System::Drawing::ColorTranslator::FromWin32 (*(LPCOLORREF)lByte);
	mBalloon->mBkColor = System::Drawing::Color::FromArgb (255, (int)lByte[2], (int)lByte[1], (int)lByte[0]); // BGRA
#else
	mBalloon.mBkColor = *(LPCDWORD)lByte;
#endif
	lByte += sizeof(DWORD);
#ifdef	_M_CEE
//	mBalloon->mBrColor = System::Drawing::ColorTranslator::FromWin32 (*(LPCOLORREF)lByte);
	mBalloon->mBrColor = System::Drawing::Color::FromArgb (255, (int)lByte[2], (int)lByte[1], (int)lByte[0]); // BGRA
#else
	mBalloon.mBrColor = *(LPCDWORD)lByte;
#endif
	lByte += sizeof(DWORD);

	lStrLen = *(LPCDWORD)lByte;
	lByte += sizeof(DWORD);
	lStr = (LPCWSTR) lByte;
#ifdef	_M_CEE
	lFontName = gcnew String (lStr, 0, lStrLen);
#else
	if	(
			(lStrLen > MAX_PATH)
		||	(IsBadStringPtr (lStr, MAX_PATH))
		)
	{
		AtlThrow (HRESULT_FROM_WIN32 (ERROR_INVALID_DATA));
	}
	_tcsncpy (mBalloon.mFont.lfFaceName, lStr, (sizeof(mBalloon.mFont.lfFaceName)/sizeof(WCHAR))-1);
#endif
	if	(lStrLen > 0)
	{
		lByte += (lStrLen + lStrPad) * sizeof (WCHAR);
	}
#ifdef	_M_CEE
	lFontHeight = labs (*(long*) lByte);
#else
	mBalloon.mFont.lfHeight = *(long*) lByte;
#endif
	lByte += sizeof (long);
#ifdef	_M_CEE
	if	((*(LPCWORD)lByte) >= FW_BOLD)
	{
		lFontStyle |= (int)System::Drawing::FontStyle::Bold;
	}
#else
	mBalloon.mFont.lfWeight = ((*(LPCWORD)lByte) >= FW_BOLD) ? FW_BOLD : FW_NORMAL;
#endif
	lByte += sizeof (WORD);
#ifdef	_M_CEE
	if	(*lByte)
	{
		lFontStyle |= (int)System::Drawing::FontStyle::Strikeout;
	}
#else
	mBalloon.mFont.lfStrikeOut = *lByte; // Unsure, and where is underline?
#endif
	lByte += sizeof (WORD);
#ifdef	_M_CEE
	if	(*lByte)
	{
		lFontStyle |= (int)System::Drawing::FontStyle::Italic;
	}
#else
	mBalloon.mFont.lfItalic = *lByte;
#endif
	lByte += sizeof (WORD);

#ifdef	_M_CEE
	try
	{
		mBalloon->mFont = gcnew System::Drawing::Font (lFontName, (float)lFontHeight, (FontStyle)lFontStyle, GraphicsUnit::Pixel); 
	}
	catch AnyExceptionDebug
#else
	mBalloon.mFont.lfCharSet = DEFAULT_CHARSET;
#endif
	return lByte;
}

LPCVOID CAgentFileBinary::ReadBufferPalette (LPCVOID pBuffer, UINT pLogLevel)
{
	LPCBYTE	lByte = (LPCBYTE)pBuffer;
	DWORD	lPaletteSize;

	lPaletteSize = *(LPCDWORD)lByte;
	lByte += sizeof(DWORD);
	if	(lPaletteSize > 0)
	{
#ifdef	_M_CEE
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
#ifdef	_M_CEE
		mHeader->mPalette = nullptr;
#else
		mHeader.mPalette = NULL;
#endif
	}

#ifdef	_SAVE_PALETTE
	if	(
			(mHeader.Palette)
		&&	(LogIsActive (MaxLogLevel (pLogLevel, _SAVE_PALETTE)))
		)
	{
		DumpPalette (mHeader.Palette);
	}
#else
#ifdef	_DUMP_PALETTE
	if	(
			(mHeader.Palette)
		&&	(LogIsActive (MaxLogLevel (pLogLevel, _DUMP_PALETTE)))
		)
	{
		DumpPalette (mHeader.Palette);
	}
#endif
#endif
	return lByte;
}

LPCVOID CAgentFileBinary::ReadBufferIcon (LPCVOID pBuffer, UINT pLogLevel)
{
	LPCBYTE	lByte = (LPCBYTE)pBuffer;

#ifdef	_M_CEE
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
		DWORD			lIconSize = *(LPCDWORD)lByte;
		LPBITMAPINFO	lMaskBitmapInfo;
		LPBITMAPINFO	lColorBitmapInfo;
		LPCBYTE			lMaskBytes;
		LPCBYTE			lColorBytes;
		tS <ICONINFO>	lIconInfo;
		CMemDCHandle	lDC;

		lByte += sizeof(DWORD);
		lMaskBitmapInfo = (LPBITMAPINFO)lByte;
		lMaskBytes = lByte + lMaskBitmapInfo->bmiHeader.biSize;
		lMaskBytes += 2 * sizeof(DWORD);
		lByte += lIconSize;

		lIconSize = *(LPCDWORD)lByte;
		lByte += sizeof(DWORD);
		lColorBitmapInfo = (LPBITMAPINFO)lByte;
		lColorBytes = lByte + lColorBitmapInfo->bmiHeader.biSize;
		lColorBytes += 16 * sizeof(DWORD);
		lByte += lIconSize;

#ifdef	_DUMP_ICON
		if	(LogIsActive (MaxLogLevel (pLogLevel, _DUMP_ICON)))
		{
			CImageDebugger::DumpBitmap (_DUMP_ICON, *lMaskBitmapInfo, (LPBYTE)lMaskBytes, _T("Icon Mask"));
			CImageDebugger::DumpBitmap (_DUMP_ICON, *lColorBitmapInfo, (LPBYTE)lColorBytes, _T("Icon Color"));
		}
#endif
#ifdef	_TRACE_RESOURCES
		if	(LogIsActive (_TRACE_RESOURCES))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentFile::CreateIcon [%p]"), this, mHeader.mIcon);
		}
#endif
#ifdef	_M_CEE
		HICON	lIcon = NULL;
#endif		

		if	(lDC.Attach (CreateDC (_T("DISPLAY"), NULL, NULL, NULL)))
		{
			lIconInfo.hbmMask = CreateDIBitmap (lDC, &lMaskBitmapInfo->bmiHeader, CBM_INIT, lMaskBytes, lMaskBitmapInfo, DIB_RGB_COLORS);
			lIconInfo.hbmColor = CreateDIBitmap (lDC, &lColorBitmapInfo->bmiHeader, CBM_INIT, lColorBytes, lColorBitmapInfo, DIB_RGB_COLORS);

			lIconInfo.fIcon = TRUE;
#ifdef	_M_CEE
			lIcon = CreateIconIndirect (&lIconInfo);
#else
			mHeader.mIcon = CreateIconIndirect (&lIconInfo);
#endif			
			lDC.Close ();
		}
		if	(lIconInfo.hbmMask)
		{
			DeleteObject (lIconInfo.hbmMask);
		}
		if	(lIconInfo.hbmColor)
		{
			DeleteObject (lIconInfo.hbmColor);
		}

#ifdef	_M_CEE
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

#ifdef	_SAVE_ICON
	if	(
			(mHeader.mIcon)
		&&	(LogIsActive (MaxLogLevel (pLogLevel, _SAVE_ICON)))
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

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LPCVOID CAgentFileBinary::ReadBufferNames (LPCVOID pBuffer, bool pNullTerminated, bool pFirstLetterCaps, UINT pLogLevel)
{
	LPCBYTE	lByte = (LPCBYTE)pBuffer;

	try
	{
		WORD					lNameCount;
		const DWORD				lStrPad = (pNullTerminated) ? 1 : 0;
		DWORD					lStrLen;
		LPCWSTR					lStr;
#ifdef	_M_CEE
		CAgentFileName^		lName;
#else		
		tPtr <CAgentFileName>	lName;
#endif		

		lNameCount = *(LPCWORD)lByte;
		lByte += sizeof (WORD);

		while (lNameCount > 0)
		{
#ifdef	_M_CEE
			lName = gcnew CAgentFileName;
#else
			lName = new CAgentFileName;
#endif			

			lName->mLanguage = *(LPCWORD)lByte;
			lByte += sizeof (WORD);

			lStrLen = *(LPCDWORD)lByte;
			lByte += sizeof(DWORD);
			if	(lStrLen > 0)
			{
				lStr = (LPCWSTR) lByte;
				lByte += (lStrLen + lStrPad) * sizeof (WCHAR);
#ifdef	_M_CEE
				lName->mName = gcnew String (lStr, 0, lStrLen);
#else
				lName->mName = CAtlString (lStr, lStrLen).AllocSysString();
#endif				
			}
#ifdef	_DEBUG_LOAD
			if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_LOAD)))
			{
				LogMessage (_DEBUG_LOAD|LogHighVolume, _T("    StrLen [%u] Str [%ls]"), lStrLen, (lStrLen) ? CAtlString (lStr, lStrLen) : NULL);
			}
#endif

			lStrLen = *(LPCDWORD)lByte;
			lByte += sizeof(DWORD);
			if	(lStrLen > 0)
			{
				lStr = (LPCWSTR) lByte;
				lByte += (lStrLen + lStrPad) * sizeof (WCHAR);
#ifdef	_M_CEE
				lName->mDesc1 = gcnew String (lStr, 0, lStrLen);
#else
				lName->mDesc1 = CAtlString (lStr, lStrLen).AllocSysString();
#endif				
			}
#ifdef	_DEBUG_LOAD
			if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_LOAD)))
			{
				LogMessage (_DEBUG_LOAD|LogHighVolume, _T("    StrLen [%u] Str [%ls]"), lStrLen, (lStrLen) ? CAtlString (lStr, lStrLen) : NULL);
			}
#endif

			lStrLen = *(LPCDWORD)lByte;
			lByte += sizeof(DWORD);
			if	(lStrLen > 0)
			{
				lStr = (LPCWSTR) lByte;
				lByte += (lStrLen + lStrPad) * sizeof (WCHAR);
#ifdef	_M_CEE
				lName->mDesc2 = gcnew String (lStr, 0, lStrLen);
#else
				lName->mDesc2 = CAtlString (lStr, lStrLen).AllocSysString();
#endif
			}
#ifdef	_DEBUG_LOAD
			if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_LOAD)))
			{
				LogMessage (_DEBUG_LOAD|LogHighVolume, _T("    StrLen [%u] Str [%ls]"), lStrLen, (lStrLen) ? CAtlString (lStr, lStrLen) : NULL);
			}
#endif

			if	(pFirstLetterCaps)
			{
#ifdef	_M_CEE
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

			if	(LogIsActive(pLogLevel))
			{
#ifdef	_M_CEE
				LogMessage (pLogLevel|LogHighVolume, _T("  NameCount [%hu] Language [%4.4hX] Name [%s]"), lNameCount, lName->mLanguage, _B(lName->mName));
#else
				LogMessage (pLogLevel|LogHighVolume, _T("  NameCount [%hu] Language [%4.4hX] Name [%ls]"), lNameCount, lName->mLanguage, (BSTR) lName->mName);
#endif				
			}

			lNameCount--;
#ifdef	_M_CEE
			if	(!mNames)
			{
				mNames = gcnew CAgentFileNames;
			}
			mNames->Add (lName);
#else
			mNames.Add (lName.Detach ());
#endif			
		}
	}
	catch AnyExceptionDebug

	return lByte;
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
CAgentFileName^ CAgentFileBinary::FindName (WORD pLangID)
#else
CAgentFileName* CAgentFileBinary::FindName (WORD pLangID)
#endif
{
#ifdef	_M_CEE
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

LPCVOID CAgentFileBinary::ReadBufferStates (LPCVOID pBuffer, bool pNullTerminated, LPCVOID pBufferEnd, UINT pLogLevel)
{
	LPCBYTE	lByte = (LPCBYTE)pBuffer;

	try
	{
		WORD		lStateCount;
		const DWORD	lStrPad = (pNullTerminated) ? 1 : 0;
		DWORD		lStrLen;
		LPCWSTR		lStr;

		lStateCount = *(LPCWORD)lByte;
		lByte += sizeof (WORD);

		while	(
					(lStateCount > 0)
				&&	(
						(pBufferEnd == NULL)
					||	(lByte < pBufferEnd)
					)
				)
		{																					 
			lStrLen = *(LPCDWORD)lByte;
			lByte += sizeof(DWORD);
			lStr = (LPCWSTR) lByte;
			lByte += (lStrLen + lStrPad) * sizeof (WCHAR);

#ifdef	_M_CEE
			String^		lState = gcnew String (lStr, 0, lStrLen);
			List <String^>^	lGestures = gcnew List<String^>;
			String^		lGesture;
#else
			CAtlString		lState (lStr, lStrLen);
			INT_PTR			lStateNdx = FindSortedString (mStates.mNames, lState);

			if	(lStateNdx < 0)
			{
				mStates.mGestures.InsertAt (lStateNdx = AddSortedString (mStates.mNames, lState), CAtlStringArray());
			}
#endif			

#ifndef	_M_CEE
			CAtlStringArray&	lGestures  = mStates.mGestures [lStateNdx];
#endif
			WORD				lGestureCount;

			lGestureCount = *(LPCWORD)lByte;
			lByte += sizeof (WORD);

			if	(LogIsActive (pLogLevel))
			{
#ifdef	_M_CEE
				LogMessage (pLogLevel|LogHighVolume, _T("  State [%s] Gestures [%u]"), _B(lState), lGestureCount);
#else
				LogMessage (pLogLevel|LogHighVolume, _T("  State [%s] Gestures [%u]"), lState, lGestureCount);
#endif
			}

			while	(
						(lGestureCount > 0)
					&&	(
							(pBufferEnd == NULL)
						||	(lByte < pBufferEnd)
						)
					)
			{
				lStrLen = *(LPCDWORD)lByte;
				lByte += sizeof(DWORD);
				lStr = (LPCWSTR) lByte;
				lByte += (lStrLen + lStrPad) * sizeof (WCHAR);
#ifdef	_M_CEE
				lGesture = gcnew String (lStr, 0, lStrLen);
				lGestures->Add (lGesture);
#else
				CAtlString	lGesture (lStr, lStrLen);
				lGestures.Add (lGesture);
#endif
				if	(LogIsActive (pLogLevel))
				{
#ifdef	_M_CEE
					LogMessage (pLogLevel|LogHighVolume, _T("    Gesture [%s]"), _B(lGesture));
#else
					LogMessage (pLogLevel|LogHighVolume, _T("    Gesture [%s]"), lGesture);
#endif
				}
				lGestureCount--;
			}
			lStateCount--;

#ifdef	_M_CEE
			mStates->Add (lState, lGestures->ToArray());
#endif			
		}
	}
	catch AnyExceptionDebug

	return lByte;
}

#ifndef	_M_CEE
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
#ifndef	_M_CEE
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
							lByte += (lStrLen + 1) * sizeof (WCHAR);
						}
						lByte += sizeof(DWORD) + sizeof (WORD) + sizeof (WORD);
						lStrLen = *(LPCDWORD)lByte;
						if	(lStrLen > 0)
						{
							lByte += (lStrLen + 1) * sizeof (WCHAR);
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
							lByte += (lStrLen + 1) * sizeof (WCHAR);
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
							lByte += (lStrLen + 1) * sizeof (WCHAR);
						}
						lStrLen = *(LPCDWORD)lByte;
						lByte += sizeof(DWORD);
						if	(lStrLen > 0)
						{
							lByte += (lStrLen + 1) * sizeof (WCHAR);
						}
						lStrLen = *(LPCDWORD)lByte;
						lByte += sizeof(DWORD);
						if	(lStrLen > 0)
						{
							lByte += (lStrLen + 1) * sizeof (WCHAR);
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
#endif	// _M_CEE
/////////////////////////////////////////////////////////////////////////////
#ifdef	_M_CEE
} // namespace Character
} // namespace DoubleAgent
#endif
/////////////////////////////////////////////////////////////////////////////
