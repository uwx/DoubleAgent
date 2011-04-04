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
#include "AgentFileAcs.h"
#ifdef	_M_CEE
#include "HandleTemplates.h"
#include "AgtErr.h"
#else
#include <shlwapi.h>
#include "DaCore.h"
#include "GuidStr.h"
#include "StringArrayEx.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "ImageDebugger.h"
#include "DebugStr.h"
#endif
#endif
#include "AgentFileDefs.inl"

#ifndef	_M_CEE
IMPLEMENT_DLL_OBJECT(CAgentFileAcs)
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef	_M_CEE
namespace DoubleAgent {
namespace Character {
#endif
/////////////////////////////////////////////////////////////////////////////

CAgentFileAcs::CAgentFileAcs()
{
	Close ();
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CAgentFileAcs::CAgentFileAcs"), this);
#endif
}

CAgentFileAcs::~CAgentFileAcs ()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CAgentFileAcs::~CAgentFileAcs"), this);
#endif
	Close ();
}

#ifdef	_M_CEE
CAgentFileAcs^ CAgentFileAcs::CreateInstance ()
#else
CAgentFileAcs* CAgentFileAcs::CreateInstance ()
#endif
{
#ifdef	_M_CEE
	LogStart (true);
	return gcnew CAgentFileAcs;
#else
	return new CAgentFileAcs;
#endif	
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
bool CAgentFileAcs::IsAcsFile::get ()
{
	if	(IsOpen)
	{
		return (mSignature == sAcsFileSignature);
	}
	return true;
}
#else
bool CAgentFileAcs::get_IsAcsFile () const
{
	if	(IsOpen)
	{
		return (mSignature == sAcsFileSignature);
	}
	return true;
}
#endif	

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
HRESULT CAgentFileAcs::Open (const System::String^ pPath, UINT pLogLevel)
{
	HRESULT		lResult = S_OK;
	String^	lPath = ParseFilePath (pPath);

#ifdef	_DEBUG_LOAD
	pLogLevel = MinLogLevel (pLogLevel, _DEBUG_LOAD);
#endif
	if	(LogIsActive (pLogLevel))
	{
		LogMessage (pLogLevel, _T("Open [%s]"), _B(lPath));
	}

	Close ();
	mPath = lPath;
	lResult = LoadFile (lPath, pLogLevel);

	if	(FAILED (lResult))
	{
		mPath = nullptr;
	}
	return lResult;
}
#else
HRESULT CAgentFileAcs::Open (LPCTSTR pPath, UINT pLogLevel)
{
	HRESULT		lResult = S_OK;
	CAtlString	lPath = ParseFilePath (pPath);

#ifdef	_DEBUG_LOAD
	pLogLevel = MinLogLevel (pLogLevel, _DEBUG_LOAD);
#endif

	Close ();
	mPath = lPath;

	if	(LogIsActive (pLogLevel))
	{
		LogMessage (pLogLevel, _T("Open [%s]"), lPath);
	}

	lResult = LoadFile (lPath, pLogLevel);

	if	(FAILED (lResult))
	{
		mPath.Empty ();
	}
	return lResult;
}
#endif	

void CAgentFileAcs::Close ()
{
	FreeImageIndex ();
	FreeSoundIndex ();
	__super::Close ();
}

#ifdef	_M_CEE
HRESULT CAgentFileAcs::LoadFile (System::String^ pPath, UINT pLogLevel)
#else
HRESULT CAgentFileAcs::LoadFile (LPCTSTR pPath, UINT pLogLevel)
#endif
{
	HRESULT	lResult = __super::LoadFile (pPath, pLogLevel);

	if	(SUCCEEDED (lResult))
	{
		ReadImageIndex (pLogLevel);
		ReadSoundIndex (pLogLevel);

#ifdef	_DUMP_IMAGE
		if	(
				(LogIsActive (MaxLogLevel (pLogLevel, _DUMP_IMAGE)))
			&&	(ReadImageIndex (pLogLevel))
			)
		{
			DumpAcsImages (pLogLevel);
		}
#endif
#ifdef	_SAVE_IMAGE
		if	(
				(LogIsActive (MaxLogLevel (pLogLevel, _SAVE_IMAGE)))
			&&	(ReadImageIndex (pLogLevel))
			)
		{
			DumpAcsImages (pLogLevel);
		}
#endif
#ifdef	_DEBUG_BLOCKS
		if	(IsAcsFile ())
		{
			DumpBlocks (MaxLogLevel (pLogLevel,_DEBUG_BLOCKS));
		}
#endif
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
int CAgentFileAcs::ImageCount::get ()
{
	if	(
			(!mImageIndex)
		&&	(IsOpen)
		)
	{
		ReadImageIndex ();
	}
	if	(mImageIndex)
	{
		return mImageIndex->Length;
	}
	return 0;
}
#else
INT_PTR CAgentFileAcs::GetImageCount () const
{
	if	(
			(mImageIndex.GetCount() <= 0)
		&&	(IsOpen)
		)
	{
		const_cast <CAgentFileAcs*> (this)->ReadImageIndex ();
	}
	return mImageIndex.GetCount();
}
#endif

#ifdef	_M_CEE
CAgentFileImage^ CAgentFileAcs::GetImage (int pImageNdx, bool p32Bit, System::Drawing::Color pBkColor, UINT pLogLevel)
#else
CAgentFileImage* CAgentFileAcs::GetImage (INT_PTR pImageNdx, bool p32Bit, const COLORREF* pBkColor, UINT pLogLevel)
#endif
{
#ifdef	_M_CEE
	CAgentFileImage^	lImage;
	
	if	(
			(IsOpen)
		&&	(mImageIndex)
		&&	(pImageNdx >= 0)
		&&	(pImageNdx < mImageIndex->Length)
		)
	{
		lImage = ReadAcsImage (mImageIndex [pImageNdx]->Key, mImageIndex [pImageNdx]->Value, (UINT)pImageNdx, p32Bit, pBkColor, pLogLevel);
	}
	return lImage;
#else
	tPtr <CAgentFileImage>	lImage;

	if	(
			(IsOpen)
		&&	(pImageNdx >= 0)
		&&	(pImageNdx < (INT_PTR)mImageIndex.GetCount())
		)
	{
		lImage = ReadAcsImage (mImageIndex [pImageNdx].LowPart, mImageIndex [pImageNdx].HighPart, (UINT)pImageNdx, p32Bit, pBkColor, pLogLevel);
	}
	return lImage.Detach ();
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
int CAgentFileAcs::SoundCount::get ()
{
	if	(
			(!mSoundIndex)
		&&	(IsOpen)
		)
	{
		ReadSoundIndex ();
	}
	if	(mSoundIndex)
	{
		return mSoundIndex->Length;
	}
	return 0;
}
#else
INT_PTR CAgentFileAcs::GetSoundCount () const
{
	if	(
			(mSoundIndex.GetCount() <= 0)
		&&	(IsOpen)
		)
	{
		const_cast <CAgentFileAcs*> (this)->ReadSoundIndex ();
	}
	return mSoundIndex.GetCount();
}
#endif

#ifdef	_M_CEE
int CAgentFileAcs::GetSoundSize (int pSoundNdx)
{
	int	lRet = -1;

	if	(
			(IsOpen)
		&&	(mSoundIndex)
		&&	(pSoundNdx >= 0)
		&&	(pSoundNdx < mSoundIndex->Length)
		)
	{
		lRet = (int) mSoundIndex [pSoundNdx]->Value;
	}
	return lRet;
}
#else
long CAgentFileAcs::GetSoundSize (INT_PTR pSoundNdx)
{
	long	lRet = -1;

	if	(
			(IsOpen)
		&&	(pSoundNdx >= 0)
		&&	(pSoundNdx < (INT_PTR)mSoundIndex.GetCount())
		)
	{
		lRet = (long) mSoundIndex [pSoundNdx].HighPart;
	}
	return lRet;
}
#endif

#ifdef	_M_CEE
#else
LPCVOID CAgentFileAcs::GetSound (INT_PTR pSoundNdx)
{
	LPCVOID	lSound = NULL;

	if	(
			(IsOpen)
		&&	(pSoundNdx >= 0)
		&&	(pSoundNdx < (INT_PTR)mSoundIndex.GetCount())
		)
	{
		lSound = ReadAcsSound (mSoundIndex [pSoundNdx].LowPart, mSoundIndex [pSoundNdx].HighPart, (UINT)pSoundNdx+1);
	}
	return lSound;
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentFileAcs::ReadHeader (UINT pLogLevel)
{
	HRESULT	lResult = __super::ReadHeader (pLogLevel);

	if	(
			(SUCCEEDED (lResult))
		&&	(
				(mSignature != sAcsFileSignature)
			||	(!ReadAcsHeader (pLogLevel))
			)
		)
	{
		lResult = AGENTPROVERROR_MAGIC;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentFileAcs::ReadAcsHeader ()
{
	return ReadAcsHeader (LogVerbose+1);
}

bool CAgentFileAcs::ReadAcsHeader (UINT pLogLevel)
{
	bool	lRet = false;

#ifdef	_M_CEE
	array <BYTE>^	lBlockBuffer;
	DWORD			lBlockOffset;

	if	(IsOpen)
	{
		try
		{
			ULARGE_INTEGER	lBlockDef;
			DWORD			lBlockLength;

			mFileStream->Seek (sizeof(DWORD), SeekOrigin::Begin);
			lBlockDef.QuadPart = mFileReader->ReadUInt64 ();
			mFileStream->Seek (lBlockOffset=lBlockDef.LowPart, SeekOrigin::Begin);
			lBlockBuffer = mFileReader->ReadBytes (lBlockLength=lBlockDef.HighPart);

			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel, _T("  [%s] Read header of [%u] at [%8.8X (%u)]"), _B(mPath), lBlockLength, lBlockOffset, lBlockOffset);
			}
		}
		catch AnyExceptionDebug
	}

	if	(lBlockBuffer)
	{
#else
	LPCULARGE_INTEGER	lBlockDefs;
	DWORD				lBlockOffset;
	DWORD				lBlockLength;
	LPCBYTE				lBlock;

	if	(
			(IsOpen)
		&&	(lBlockDefs = (LPCULARGE_INTEGER)(((LPCBYTE)(LPVOID)mFileView)+sizeof(DWORD)))
		&&	(lBlockLength = lBlockDefs[0].HighPart)
		&&	(lBlock = ((LPCBYTE)(LPVOID)mFileView)+(lBlockOffset=lBlockDefs[0].LowPart))
		)
	{
		if	(LogIsActive (pLogLevel))
		{
			LogMessage (pLogLevel, _T("  [%s] Read header of [%u] at [%8.8X (%u)]"), mPath, lBlockLength, lBlockOffset, lBlockOffset);
		}
#endif		

		try
		{
#ifdef	_M_CEE
			pin_ptr <BYTE>	lBlock = &lBlockBuffer[0];
			LPCBYTE			lByte = lBlock;
#else
			LPCBYTE	lByte = lBlock;
#endif

			mVersionMinor = *(LPCWORD)lByte;
			lByte += sizeof (WORD);
			mVersionMajor = *(LPCWORD)lByte;
			lByte += sizeof (WORD);
			mFileNamesOffset = (*(LPCDWORD)lByte) - lBlockOffset;
			lByte += sizeof(DWORD);	// Names offset
			lByte += sizeof(DWORD);	// Names size

#ifdef	_M_CEE
			mHeader->mGuid = System::Guid (*(LPCDWORD)lByte, *(LPCWORD)(lByte+4), *(LPCWORD)(lByte+6), lByte[8], lByte[9], lByte[10], lByte[11], lByte[12], lByte[13], lByte[14], lByte[15]);
#else
			mHeader.mGuid = *(LPCGUID)lByte;
#endif
			lByte += sizeof (GUID);
#ifdef	_M_CEE
			mHeader->mImageSize.Width = *(LPCWORD)lByte;
#else
			mHeader.mImageSize.cx = *(LPCWORD)lByte;
#endif
			lByte += sizeof (WORD);
#ifdef	_M_CEE
			mHeader->mImageSize.Height = *(LPCWORD)lByte;
#else
			mHeader.mImageSize.cy = *(LPCWORD)lByte;
#endif			
			lByte += sizeof (WORD);
#ifdef	_M_CEE
			mHeader->mTransparency = *lByte;
#else
			mHeader.mTransparency = *lByte;
#endif			
			lByte++;
#ifdef	_M_CEE
			mHeader->mStyle = *(LPCDWORD)lByte;
#else
			mHeader.mStyle = *(LPCDWORD)lByte;
#endif			
			lByte += sizeof(DWORD);
			lByte += sizeof(DWORD); // Unknown - always 0x0000002

#ifdef	_M_CEE
			if	(mHeader->mStyle & (DWORD)AgentCharStyle::CharStyleTts)
#else
			if	(mHeader.mStyle & CharStyleTts)
#endif			
			{
				lByte = (LPCBYTE)ReadBufferTts (lByte, true, pLogLevel);
			}
#ifdef	_M_CEE
			if	(mHeader->mStyle & (DWORD)AgentCharStyle::CharStyleBalloon)
#else
			if	(mHeader.mStyle & CharStyleBalloon)
#endif			
			{
				lByte = (LPCBYTE)ReadBufferBalloon (lByte, true, pLogLevel);
			}
			lByte = (LPCBYTE)ReadBufferPalette (lByte, pLogLevel);
			lByte = (LPCBYTE)ReadBufferIcon (lByte, pLogLevel);

			mFileStatesOffset = (DWORD)(lByte - lBlock);
			lRet = true;
		}
		catch AnyExceptionDebug

#ifndef	_M_CEE
		if	(LogIsActive (pLogLevel))
		{
			Log (pLogLevel);
		}
#endif
	}

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentFileAcs::ReadNames (bool pFirstLetterCaps, UINT pLogLevel)
{
	bool	lRet = false;

#ifdef	_M_CEE
	array <BYTE>^	lBlockBuffer;

	FreeNames ();
	
	if	(IsOpen)
	{
		try
		{
			ULARGE_INTEGER	lBlockDef;
			DWORD			lBlockOffset;
			DWORD			lBlockLength;
			DWORD			lNamesOffset;
			pin_ptr <BYTE>	lBlock;

			mFileStream->Seek (sizeof(DWORD), SeekOrigin::Begin);
			lBlockDef.QuadPart = mFileReader->ReadUInt64 ();
			mFileStream->Seek (lBlockOffset=lBlockDef.LowPart, SeekOrigin::Begin);
			mFileStream->Seek (sizeof(DWORD), SeekOrigin::Current);
			lNamesOffset = mFileReader->ReadUInt32 () - lBlockOffset;
			mFileStream->Seek (lBlockOffset+lNamesOffset, SeekOrigin::Begin);
			lBlockBuffer = mFileReader->ReadBytes (lBlockLength=lBlockDef.HighPart-lNamesOffset);
			lBlock = &lBlockBuffer[0];

			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel|LogHighVolume, _T("  [%u] Names of [%u] at [%8.8X] in [%8.8X]"), *(LPCWORD)lBlock, lBlockLength-lNamesOffset, lNamesOffset, lBlockOffset);
			}

			ReadBufferNames (lBlock, true, pFirstLetterCaps, pLogLevel);

			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel, _T("  [%d] Names [%s]"), (mNames ? mNames->Count : 0), _B(mPath));
			}
			lRet = true;
		}
		catch AnyExceptionDebug
	}
#else	
	LPCULARGE_INTEGER	lBlockDefs;
	DWORD				lBlockOffset;
	DWORD				lBlockLength;
	LPCBYTE				lBlock;

	if	(LogIsActive (pLogLevel))
	{
		LogMessage (pLogLevel, _T("ReadNames [%s]"), mPath);
	}
	FreeNames ();

	if	(
			(IsOpen)
		&&	(lBlockDefs = (LPCULARGE_INTEGER)(((LPCBYTE)(LPVOID)mFileView)+sizeof(DWORD)))
		&&	(lBlockLength = lBlockDefs[0].HighPart)
		&&	(lBlock = ((LPCBYTE)(LPVOID)mFileView)+(lBlockOffset=lBlockDefs[0].LowPart))
		)
	{
		try
		{
			DWORD	lNamesOffset = ((LPCDWORD)lBlock) [1] - lBlockOffset;
			LPCBYTE	lByte = lBlock+lNamesOffset;

			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel|LogHighVolume, _T("  [%u] Names of [%u] at [%8.8X] in [%8.8X]"), *(LPCWORD)lByte, lBlockLength-lNamesOffset, lNamesOffset, lBlockOffset);
			}

			lByte = (LPCBYTE)ReadBufferNames (lByte, true, pFirstLetterCaps, pLogLevel);

#ifdef	_DEBUG_LOAD
			if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_LOAD)))
			{
				LogMessage (_DEBUG_LOAD, _T("    Left [%u]"), (long) lBlockLength - (long) (lByte-lBlock));
			}
#endif
			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel, _T("  [%d] Names [%s]"), mNames.GetCount(), mPath);
			}
			lRet = true;
		}
		catch AnyExceptionDebug
	}
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentFileAcs::ReadStates (UINT pLogLevel)
{
	bool	lRet = false;
#ifdef	_M_CEE
	array <BYTE>^	lBlockBuffer;

	FreeStates ();
	
	if	(IsOpen)
	{
		try
		{
			ULARGE_INTEGER	lBlockDef;
			DWORD			lBlockOffset;
			DWORD			lBlockLength;
			pin_ptr <BYTE>	lBlock;
			pin_ptr <BYTE>	lNamesStart;

			mFileStream->Seek (sizeof(DWORD), SeekOrigin::Begin);
			lBlockDef.QuadPart = mFileReader->ReadUInt64 ();
			mFileStream->Seek (lBlockOffset=lBlockDef.LowPart, SeekOrigin::Begin);
			lBlockBuffer = mFileReader->ReadBytes (lBlockLength=lBlockDef.HighPart);
			lBlock = &lBlockBuffer [mFileStatesOffset];
			lNamesStart = &lBlockBuffer [mFileNamesOffset];

			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel|LogHighVolume, _T("  [%u] States [%u] at [%8.8X]"), *(LPCWORD)lBlock, mFileNamesOffset-mFileStatesOffset, mFileStatesOffset);
			}

			ReadBufferStates (lBlock, true, lNamesStart, pLogLevel);

			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel, _T("  [%d] States [%s]"), mStates->Count, _B(mPath));
			}
			lRet = true;
		}
		catch AnyExceptionDebug
	}
#else	
	LPCULARGE_INTEGER	lBlockDefs;
	DWORD				lBlockOffset;
	DWORD				lBlockLength;
	LPCBYTE				lBlock;

	if	(LogIsActive (pLogLevel))
	{
		LogMessage (pLogLevel, _T("ReadStates [%s]"), mPath);
	}
	FreeStates ();

	if	(
			(IsOpen)
		&&	(lBlockDefs = (LPCULARGE_INTEGER)(((LPCBYTE)(LPVOID)mFileView)+sizeof(DWORD)))
		&&	(lBlockLength = lBlockDefs[0].HighPart)
		&&	(lBlock = ((LPCBYTE)(LPVOID)mFileView)+(lBlockOffset=lBlockDefs[0].LowPart))
		)
	{
		try
		{
			LPCBYTE	lByte = lBlock + mFileStatesOffset;
			LPCBYTE	lNamesStart = lBlock + mFileNamesOffset;

			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel|LogHighVolume, _T("  [%u] States [%u] at [%8.8X]"), *(LPCWORD)lByte, lNamesStart-lByte, lByte-lBlock);
			}
#ifdef	_DEBUG_LOAD
			if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_LOAD)))
			{
				LogDump (_DEBUG_LOAD|LogHighVolume, (LPBYTE)lByte, lNamesStart-lByte, _T("    "), true);
			}
#endif

			lByte = (LPCBYTE)ReadBufferStates (lByte, true, lNamesStart, pLogLevel);

			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel, _T("  [%d] States [%s]"), mStates.mGestures.GetCount(), mPath);
			}
			lRet = true;
		}
		catch AnyExceptionDebug
	}
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentFileAcs::ReadGestures (UINT pLogLevel)
{
	bool	lRet = false;
#ifdef	_M_CEE
	array <BYTE>^	lBlockBuffer;
	DWORD			lBlockOffset;
	DWORD			lBlockLength;

	FreeGestures ();
	
	if	(IsOpen)
	{
		try
		{
			ULARGE_INTEGER	lBlockDef;

			mFileStream->Seek (sizeof(DWORD)+sizeof(lBlockDef), SeekOrigin::Begin);
			lBlockDef.QuadPart = mFileReader->ReadUInt64 ();
			mFileStream->Seek (lBlockOffset=lBlockDef.LowPart, SeekOrigin::Begin);
			lBlockBuffer = mFileReader->ReadBytes (lBlockLength=lBlockDef.HighPart);
		}
		catch AnyExceptionDebug
	}		

	if	(lBlockBuffer)
	{		
#else	
	LPCULARGE_INTEGER	lBlockDefs;
	DWORD				lBlockOffset;
	DWORD				lBlockLength;
	LPCBYTE				lBlock;

	if	(LogIsActive (pLogLevel))
	{
		LogMessage (pLogLevel, _T("ReadGestures [%s]"), mPath);
	}
	FreeGestures ();

	if	(
			(IsOpen)
		&&	(lBlockDefs = (LPCULARGE_INTEGER)(((LPCBYTE)(LPVOID)mFileView)+sizeof(DWORD)))
		&&	(lBlockLength = lBlockDefs[1].HighPart)
		&&	(lBlock = ((LPCBYTE)(LPVOID)mFileView)+(lBlockOffset=lBlockDefs[1].LowPart))
		)
	{
#endif	
		try
		{
#ifdef	_M_CEE
			pin_ptr <BYTE>			lBlock = &lBlockBuffer[0];
			LPCBYTE					lByte = lBlock;
			String^				lGestureName;
			CAgentFileAnimation^	lAnimation;
#else
			LPCBYTE					lByte = lBlock;
			CAtlString				lGestureName;
#endif			
			DWORD					lCount = *(LPCDWORD)lByte;
			DWORD					lStrLen;
			LPCWSTR					lStr;
			LPCDWORD				lAnimationRef;

			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel|LogHighVolume, _T("  [%u] Gestures [%u] at [%8.8X (%u)]"), lCount, lBlockLength, lBlockOffset, lBlockOffset);
			}
#ifdef	_DEBUG_LOAD
			if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_LOAD)))
			{
				LogDump (_DEBUG_LOAD|LogHighVolume, (LPBYTE)lBlock, lBlockLength, _T("    "), true);
			}
#endif
			lByte += sizeof(DWORD);

			while	(lCount > 0)
			{
				lStrLen = *(LPCDWORD)lByte;
				lByte += sizeof(DWORD);
				lStr = (LPCWSTR) lByte;
				lByte += (lStrLen + 1) * sizeof (WCHAR);
#ifdef	_M_CEE
				lGestureName = gcnew String (lStr, 0, lStrLen);
#else
				lGestureName = CAtlString (lStr, lStrLen);
#endif				

				lAnimationRef = (LPCDWORD)lByte;
				lByte += sizeof(DWORD)*2;

#ifdef	_DEBUG_LOAD
				if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_LOAD)))
				{
					LogMessage (_DEBUG_LOAD, _T("  Gesture [%s] at [%8.8X (%u)] of [%8.8X (%u)]"), lGestureName, lAnimationRef[0], lAnimationRef[0], lAnimationRef[1], lAnimationRef[1]);
				}
#else
				if	(LogIsActive (pLogLevel))
				{
#ifdef	_M_CEE
					LogMessage (pLogLevel|LogHighVolume, _T("  Gesture [%s]"), _B(lGestureName));
#else
					LogMessage (pLogLevel|LogHighVolume, _T("  Gesture [%s]"), lGestureName);
#endif					
				}
#endif
#ifdef	_M_CEE
				lAnimation = ReadAcsAnimation (lAnimationRef[0], lAnimationRef[1], pLogLevel);
				lAnimation->mName = lGestureName;
				mGestures->Add (lAnimation);
#else
				mGestures.mAnimations.InsertAt (AddSortedString (mGestures.mNames, lGestureName), ReadAcsAnimation (lAnimationRef[0], lAnimationRef[1], pLogLevel));
#endif				
				lCount--;
			}

			if	(LogIsActive (pLogLevel))
			{
#ifdef	_M_CEE
				LogMessage (pLogLevel, _T("  [%d] Gestures [%s]"), mGestures->Count, _B(mPath));
#else
				LogMessage (pLogLevel, _T("  [%d] Gestures [%s]"), mGestures.mAnimations.GetCount(), mPath);
#endif				
			}
			lRet = true;
		}
		catch AnyExceptionDebug
	}

	return lRet;
}

#ifdef	_M_CEE
CAgentFileAnimation^ CAgentFileAcs::ReadAcsAnimation (DWORD pOffset, DWORD pSize, UINT pLogLevel)
#else
CAgentFileAnimation* CAgentFileAcs::ReadAcsAnimation (DWORD pOffset, DWORD pSize, UINT pLogLevel)
#endif
{
#ifdef	_M_CEE
	CAgentFileAnimation^		lRet = nullptr;
#else
	tPtr <CAgentFileAnimation>	lRet;
#endif	

	try
	{
#ifdef	_M_CEE
		array <BYTE>^	lBlockBuffer;
		pin_ptr <BYTE>	lBlock;

		if	(IsOpen)
		{
			mFileStream->Seek (pOffset, SeekOrigin::Begin);
			lBlockBuffer = mFileReader->ReadBytes (pSize);
			lBlock = &lBlockBuffer[0];
#else
		LPCBYTE	lBlock;

		if	(
				(IsOpen)
			&&	(lBlock = ((LPCBYTE)(LPVOID)mFileView)+pOffset)
			)
		{
#endif		
			LPCBYTE		lByte = lBlock;
			DWORD		lStrLen;
			LPCWSTR		lStr;
			BYTE		lReturnType;
			WORD		lFrameCount = 0;
			WORD		lFrameNdx;
#ifdef	_M_CEE
			String^	lName;
			String^	lReturnName;
#else
			CAtlString	lName;
			CAtlString	lReturnName;
#endif			

			lStrLen = *(LPCDWORD)lByte;
			lByte += sizeof(DWORD);
			lStr = (LPCWSTR) lByte;
			lByte += (lStrLen + 1) * sizeof (WCHAR);
#ifdef	_M_CEE
			lName = gcnew String (lStr, 0, lStrLen);
#else			
			lName = CAtlString (lStr, lStrLen);
#endif			

			lReturnType = *lByte;
			lByte++;
			lStrLen = *(LPCDWORD)lByte;
			lByte += sizeof(DWORD);
			if	(lStrLen > 0)
			{
				lStr = (LPCWSTR) lByte;
				lByte += (lStrLen + 1) * sizeof (WCHAR);
#ifdef	_M_CEE
				lReturnName = gcnew String (lStr, 0, lStrLen);
#else			
				lReturnName = CAtlString (lStr, lStrLen);
#endif
			}
			lFrameCount = *(LPCWORD)lByte;
			lByte += sizeof (WORD);

#ifdef	_DEBUG_ANIMATION
			if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_ANIMATION)))
			{
				CAtlString	lReturnString = (lReturnType == 1) ? _T("<exit branching>") : (lReturnType == 2) ? _T("<none>") : lReturnName;
				LogMessage (_DEBUG_ANIMATION, _T("    Animation [%s] Return [%u] [%s] Frames [%u]"), lName, lReturnType, lReturnString, lFrameCount);
			}
#else
			if	(LogIsActive (pLogLevel))
			{
#ifdef	_M_CEE
				LogMessage (pLogLevel|LogHighVolume, _T("    Animation [%s] Return [%u] [%s] Frames [%u]"), _B(lName), lReturnType, _B(lReturnName), lFrameCount);
#else
				LogMessage (pLogLevel|LogHighVolume, _T("    Animation [%s] Return [%u] [%s] Frames [%u]"), lName, lReturnType, lReturnName, lFrameCount);
#endif				
			}
#endif
#ifdef	_DUMP_ANIMATION
			if	(LogIsActive (MaxLogLevel (pLogLevel, _DUMP_ANIMATION)))
			{
				LogDump (_DUMP_ANIMATION, (LPBYTE)lBlock, pSize, _T("        "));
			}
#endif
			if	(
					(lFrameCount > 0)
#ifdef	_M_CEE
				&&	(lRet = gcnew CAgentFileAnimation)
#else
				&&	(lRet = new tS <CAgentFileAnimation>)
#endif				
				)
			{
#ifdef	_M_CEE
				lRet->mName = lName;
#else
				lRet->mName = lName.AllocSysString();
#endif				
				lRet->mReturnType = lReturnType;
#ifdef	_M_CEE
				lRet->mReturnName = lReturnName;
#else
				if	(!lReturnName.IsEmpty())
				{
					lRet->mReturnName = lReturnName.AllocSysString();
				}
#endif					

				if	(
						(lFrameCount > 0)
#ifdef	_M_CEE
					&&	(lRet->mFrames = gcnew CAgentFileFrames)
#else
					&&	(lRet->mFrames = new tS <CAgentFileFrame> [lFrameCount])
#endif					
					)
				{
#ifndef	_M_CEE
					lRet->mFrameCount = lFrameCount;
#endif
					for	(lFrameNdx = 0; lFrameNdx < lFrameCount; lFrameNdx++)
					{
#ifdef	_M_CEE
						CAgentFileFrame^	lFrame = gcnew CAgentFileFrame;
#else
						CAgentFileFrame*	lFrame = &lRet->mFrames [(int)lFrameNdx];
#endif						
						LPCBYTE				lFrameStart = lByte;
						WORD				lImageCount = *(LPCWORD)lByte;
						BYTE				lBranchCount;
						BYTE				lOverlayCount;
						INT_PTR				lNdx;

#ifdef	_M_CEE
						lRet->mFrames->Add (lFrame);
#endif
						lByte += sizeof(WORD);
#ifdef	_DEBUG_ANIMATION
						if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_ANIMATION)))
						{
							LogMessage (_DEBUG_ANIMATION, _T("      Frame [%hu] Images [%hu]"), lFrameNdx, lImageCount);
						}
#endif
						if	(
								(lImageCount > 0)
#ifdef	_M_CEE
							&&	(lFrame->mImages = gcnew CAgentFileFrameImages)
#else
							&&	(lFrame->mImages = new tS <CAgentFileFrameImage> [lImageCount])
#endif							
							)
						{
#ifndef	_M_CEE
							lFrame->mImageCount = lImageCount;
#endif
							for	(lNdx = 0; lNdx < (INT_PTR)lImageCount; lNdx++)
							{
#ifdef	_M_CEE
								CAgentFileFrameImage^	lFrameImage = gcnew CAgentFileFrameImage;
#else
								CAgentFileFrameImage*	lFrameImage = &lFrame->mImages [lNdx];
#endif
								lFrameImage->mImageNdx = *(LPCDWORD)lByte;
								lByte += sizeof(DWORD);
#ifdef	_M_CEE
								lFrameImage->mOffset.X = (long)(short)*(LPCWORD)lByte;
#else
								lFrameImage->mOffset.x = (long)(short)*(LPCWORD)lByte;
#endif								
								lByte += sizeof(WORD);
#ifdef	_M_CEE
								lFrameImage->mOffset.Y = (long)(short)*(LPCWORD)lByte;
#else
								lFrameImage->mOffset.y = (long)(short)*(LPCWORD)lByte;
#endif
								lByte += sizeof(WORD);
#ifdef	_M_CEE
								lFrame->mImages->Add (lFrameImage);
#endif
#ifdef	_DEBUG_ANIMATION
								if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_ANIMATION)))
								{
									LogMessage (_DEBUG_ANIMATION, _T("        Image [%4u] at [%d %d]"), lFrameImage->mImageNdx, lFrameImage->mOffset.x, lFrameImage->mOffset.y);
								}
#endif
							}
						}

						lFrame->mSoundNdx = *(LPCWORD)lByte;
						lByte += sizeof(WORD);
						lFrame->mDuration = *(LPCWORD)lByte;
						lByte += sizeof(WORD);
						lFrame->mExitFrame = *(LPCWORD)lByte;
						lByte += sizeof(WORD);
#ifdef	_DEBUG_ANIMATION
						if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_ANIMATION)))
						{
							LogMessage (_DEBUG_ANIMATION, _T("        Duration [%hu] ExitFrame [%hd] Sound [%2hd]"), lFrame->mDuration, lFrame->mExitFrame, lFrame->mSoundNdx);
						}
#endif
						lBranchCount = *lByte;
						lByte++;
						if	(lBranchCount > 0)
						{
#ifdef	_M_CEE
							lFrame->mBranching = gcnew array <CAgentFileFrameBranch> (lBranchCount);
#endif
							lFrame->mBranching [0].mFrameNdx = LOWORD(((LPCDWORD)lByte) [0]);
							lFrame->mBranching [0].mProbability = HIWORD(((LPCDWORD)lByte) [0]);
						}
						if	(lBranchCount > 1)
						{
							lFrame->mBranching [1].mFrameNdx = LOWORD(((LPCDWORD)lByte) [1]);
							lFrame->mBranching [1].mProbability = HIWORD(((LPCDWORD)lByte) [1]);
						}
						if	(lBranchCount > 2)
						{
							lFrame->mBranching [2].mFrameNdx = LOWORD(((LPCDWORD)lByte) [2]);
							lFrame->mBranching [2].mProbability = HIWORD(((LPCDWORD)lByte) [2]);
						}
						lByte += sizeof(WORD)*lBranchCount*2;

#ifdef	_DEBUG_ANIMATION
						if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_ANIMATION)))
						{
							for	(lNdx = 0; lNdx < min ((INT_PTR)lBranchCount, 3); lNdx++)
							{
								LogMessage (_DEBUG_ANIMATION, _T("        Branch [%d] [%u]"), lFrame->mBranching [lNdx].mFrameNdx, lFrame->mBranching [lNdx].mProbability);
							}
						}
#endif

						lOverlayCount = *lByte;
						lByte++;
						if	(
								(lOverlayCount > 0)
#ifdef	_M_CEE
							&&	(lFrame->mOverlays = gcnew CAgentFileFrameOverlays)
#else
							&&	(lFrame->mOverlays = new tS <CAgentFileFrameOverlay> [lOverlayCount])
#endif							
							)
						{
#ifndef	_M_CEE
							lFrame->mOverlayCount = lOverlayCount;
#endif
							for	(lNdx = 0; lNdx < (INT_PTR)lOverlayCount; lNdx++)
							{
#ifdef	_M_CEE
								CAgentFileFrameOverlay^	lOverlay = gcnew CAgentFileFrameOverlay;
#else
								CAgentFileFrameOverlay*	lOverlay = &lFrame->mOverlays [lNdx];
#endif								
								BYTE						lRgnFlag;

								lOverlay->mOverlayType = (AgentMouthOverlay)*lByte;
								lByte += sizeof(BYTE);
								lOverlay->mReplaceFlag = (*lByte) ? true : false;
								lByte += sizeof(BYTE);
								lOverlay->mImageNdx = *(LPCWORD)lByte;
								lByte += sizeof(WORD);
								lByte += sizeof(BYTE); // Unknown
								lRgnFlag = *lByte;
								lByte += sizeof(BYTE);
#ifdef	_M_CEE
								lOverlay->mOffset.X = (long)(short)*(LPCWORD)lByte;
#else
								lOverlay->mOffset.x = (long)(short)*(LPCWORD)lByte;
#endif
								lByte += sizeof(WORD);
#ifdef	_M_CEE
								lOverlay->mOffset.Y = (long)(short)*(LPCWORD)lByte;
#else
								lOverlay->mOffset.y = (long)(short)*(LPCWORD)lByte;
#endif
								lByte += sizeof(WORD);
#ifdef	_M_CEE
								lOverlay->mSomething.X = (long)(short)*(LPCWORD)lByte;
#else
								lOverlay->mSomething.x = (long)(short)*(LPCWORD)lByte;
#endif
								lByte += sizeof(WORD);
#ifdef	_M_CEE
								lOverlay->mSomething.Y = (long)(short)*(LPCWORD)lByte;
#else
								lOverlay->mSomething.y = (long)(short)*(LPCWORD)lByte;
#endif
								lByte += sizeof(WORD);
#ifdef	_M_CEE
								lFrame->mOverlays->Add (lOverlay);
#endif
#ifdef	_DEBUG_ANIMATION
								if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_ANIMATION)))
								{
									LogMessage (_DEBUG_ANIMATION, _T("        Overlay [%hu] Replace [%u] Image [%4u] at [%d %d] something [%d] [%d]"), lOverlay->OverlayNdx, lOverlay->ReplaceFlag, lOverlay->ImageNdx, lOverlay->Offset.x, lOverlay->Offset.y, lOverlay->mSomething.x, lOverlay->mSomething.y);
								}
#endif
							}
						}

#ifdef	_DUMP_ANIMATION
						if	(LogIsActive (MaxLogLevel (pLogLevel, _DUMP_ANIMATION)))
						{
							LogDump (_DUMP_ANIMATION, (LPBYTE)lFrameStart, min(lByte-lFrameStart, pSize-(lFrameStart-lBlock)), _T("          "));
						}
#endif
						if	(lByte > lBlock+pSize)
						{
#ifdef	_DEBUG
							LogMessage (LogDebug, _T("!!! Screwed !!!"));
#endif
							break;
						}
					}
#ifdef	_DEBUG
					if	(lByte < lBlock+pSize)
					{
						LogMessage (LogDebug, _T("!!! Extra !!! [%u]"), pSize-(lByte-lBlock));
					}
#endif
				}
			}
		}
	}
	catch AnyExceptionDebug

#ifdef	_M_CEE
	return lRet;
#else
	return lRet.Detach ();
#endif	
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAgentFileAcs::FreeImageIndex ()
{
#ifdef	_M_CEE
	mImageIndex = nullptr;
#else
	mImageIndex.RemoveAll ();
#endif	
}

bool CAgentFileAcs::ReadImageIndex ()
{
	return ReadImageIndex (LogVerbose+1);
}

bool CAgentFileAcs::ReadImageIndex (UINT pLogLevel)
{
	bool	lRet = false;

#ifdef	_M_CEE
	array <BYTE>^	lBlockBuffer;
	DWORD			lBlockOffset;
	DWORD			lBlockLength;
	pin_ptr <BYTE>	lBlock;

	if	(LogIsActive (pLogLevel))
	{
		LogMessage (pLogLevel, _T("ReadImageIndex [%s]"), _B(mPath));
	}
	FreeImageIndex ();

	if	(IsOpen)
	{
		try
		{
			ULARGE_INTEGER	lBlockDef;

			mFileStream->Seek (sizeof(DWORD)+(sizeof(lBlockDef)*2), SeekOrigin::Begin);
			lBlockDef.QuadPart = mFileReader->ReadUInt64 ();
			mFileStream->Seek (lBlockOffset=lBlockDef.LowPart, SeekOrigin::Begin);
			lBlockBuffer = mFileReader->ReadBytes (lBlockLength=lBlockDef.HighPart);
			lBlock = &lBlockBuffer[0];
		}
		catch AnyExceptionDebug
	}		

	if	(lBlock)
	{		
#else
	LPCULARGE_INTEGER	lBlockDefs;
	DWORD				lBlockOffset;
	DWORD				lBlockLength;
	LPCBYTE				lBlock;

	if	(LogIsActive (pLogLevel))
	{
		LogMessage (pLogLevel, _T("ReadImageIndex [%s]"), mPath);
	}
	FreeImageIndex ();

	if	(
			(IsOpen)
		&&	(lBlockDefs = (LPCULARGE_INTEGER)(((LPCBYTE)(LPVOID)mFileView)+sizeof(DWORD)))
		&&	(lBlockLength = lBlockDefs[2].HighPart)
		&&	(lBlock = ((LPCBYTE)(LPVOID)mFileView)+(lBlockOffset=lBlockDefs[2].LowPart))
		)
	{
#endif
		try
		{
			LPCBYTE			lByte = lBlock;
			DWORD			lEntry;
			DWORD			lCount = *(LPCDWORD)lByte;
			UINT			lFrameNum = 0;
			LPCDWORD		lIndex;
#ifdef	_M_CEE
			int				lArrayNdx = 0;
#else
			ULARGE_INTEGER	lIndexEntry;
#endif			

			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel|LogHighVolume, _T("  [%u] ImageIndex [%u] at [%8.8X (%u)]"), lCount, lBlockLength, lBlockOffset, lBlockOffset);
			}
#ifdef	_DEBUG_LOAD
			if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_LOAD)))
			{
				LogDump (_DEBUG_LOAD|LogHighVolume, (LPBYTE)lBlock, lBlockLength, _T("    "), true);
			}
#endif
			lByte += sizeof(DWORD);

#ifdef	_M_CEE
			if	(lCount > 0)
			{
				mImageIndex = gcnew array <KeyValuePair <UInt32, UInt32>^> (lCount);
			}
#endif
			for	(lEntry = 0; lEntry < lCount; lEntry++)
			{
				lIndex = (LPCDWORD) lByte;
				lByte += sizeof(DWORD) * 3;

#ifdef	_M_CEE
				mImageIndex [lArrayNdx++] = gcnew KeyValuePair <UInt32, UInt32> (lIndex [0], lIndex [1]);
#else
				lIndexEntry.LowPart = lIndex [0];
				lIndexEntry.HighPart = lIndex [1];
				mImageIndex.Add (lIndexEntry);
#endif				
#ifdef	_DEBUG_INDEX
				if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_INDEX)))
#else
				if	(LogIsActive (pLogLevel))
#endif
				{
					LogMessage (pLogLevel, _T("    Image [%4u] of [%6u] at [%8.8X] thru [%8.8X] chksum [%8.8X]"), lEntry, lIndex[1], lIndex[0], lIndex[0]+lIndex[1]-1, lIndex[2]);
				}
			}
			if	(LogIsActive (pLogLevel))
			{
#ifdef	_M_CEE
				LogMessage (pLogLevel|LogHighVolume, _T("  [%u] Images"), lCount);
#else
				LogMessage (pLogLevel|LogHighVolume, _T("  [%u] Images"), mImageIndex.GetCount());
#endif				
			}
			lRet = true;
		}
		catch AnyExceptionDebug
	}

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAgentFileAcs::FreeSoundIndex ()
{
#ifdef	_M_CEE
	mSoundIndex = nullptr;
#else
	mSoundIndex.RemoveAll ();
#endif	
}

bool CAgentFileAcs::ReadSoundIndex ()
{
	return ReadSoundIndex (LogVerbose+1);
}

bool CAgentFileAcs::ReadSoundIndex (UINT pLogLevel)
{
	bool	lRet = false;
#ifdef	_M_CEE
	array <BYTE>^	lBlockBuffer;
	DWORD			lBlockOffset;
	DWORD			lBlockLength;
	pin_ptr <BYTE>	lBlock;

	if	(LogIsActive (pLogLevel))
	{
		LogMessage (pLogLevel, _T("ReadSoundIndex [%s]"), _B(mPath));
	}
	FreeSoundIndex ();

	if	(IsOpen)
	{
		try
		{
			ULARGE_INTEGER	lBlockDef;

			mFileStream->Seek (sizeof(DWORD)+(sizeof(lBlockDef)*3), SeekOrigin::Begin);
			lBlockDef.QuadPart = mFileReader->ReadUInt64 ();
			mFileStream->Seek (lBlockOffset=lBlockDef.LowPart, SeekOrigin::Begin);
			lBlockBuffer = mFileReader->ReadBytes (lBlockLength=lBlockDef.HighPart);
			lBlock = &lBlockBuffer[0];
		}
		catch AnyExceptionDebug
	}		

	if	(lBlock)
	{		
#else
	LPCULARGE_INTEGER	lBlockDefs;
	DWORD				lBlockOffset;
	DWORD				lBlockLength;
	LPCBYTE				lBlock;

	if	(LogIsActive (pLogLevel))
	{
		LogMessage (pLogLevel, _T("ReadSoundIndex [%s]"), mPath);
	}
	FreeSoundIndex ();

	if	(
			(IsOpen)
		&&	(lBlockDefs = (LPCULARGE_INTEGER)(((LPCBYTE)(LPVOID)mFileView)+sizeof(DWORD)))
		&&	(lBlockLength = lBlockDefs[3].HighPart)
		&&	(lBlock = ((LPCBYTE)(LPVOID)mFileView)+(lBlockOffset=lBlockDefs[3].LowPart))
		)
	{
#endif
		try
		{
			LPCBYTE			lByte = lBlock;
			DWORD			lEntry;
			DWORD			lCount = *(LPCDWORD)lByte;
			LPCDWORD		lIndex;
#ifdef	_M_CEE
			int				lArrayNdx = 0;
#else
			ULARGE_INTEGER	lIndexEntry;
#endif			

			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel|LogHighVolume, _T("  [%u] SoundIndex [%u] at [%8.8X (%u)]"), lCount, lBlockLength, lBlockOffset, lBlockOffset);
			}
#ifdef	_DEBUG_LOAD
			if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_LOAD)))
			{
				LogDump (_DEBUG_LOAD|LogHighVolume, (LPBYTE)lBlock, lBlockLength, _T("    "), true);
			}
#endif
			lByte += sizeof(DWORD);

#ifdef	_M_CEE
			if	(lCount > 0)
			{
				mSoundIndex = gcnew array <KeyValuePair <UInt32, UInt32>^> (lCount);
			}
#endif
			for	(lEntry = 0; lEntry < lCount; lEntry++)
			{
				lIndex = (LPCDWORD) lByte;
				lByte += sizeof(DWORD) * 3;

#ifdef	_M_CEE
				mSoundIndex [lArrayNdx++] = gcnew KeyValuePair <UInt32, UInt32> (lIndex [0], lIndex [1]);
#else
				lIndexEntry.LowPart = lIndex [0];
				lIndexEntry.HighPart = lIndex [1];
				mSoundIndex.Add (lIndexEntry);
#endif				
#ifdef	_DEBUG_INDEX
				if	(LogIsActive (MaxLogLevel (pLogLevel, _DEBUG_INDEX)))
#else
				if	(LogIsActive (pLogLevel))
#endif
				{
					LogMessage (pLogLevel, _T("    Sound [%2u] of [%6u] at [%8.8X] thru [%8.8X] chksum [%8.8X]"), lEntry, lIndex[1], lIndex[0], lIndex[0]+lIndex[1]-1, lIndex[2]);
				}
			}

			if	(LogIsActive (pLogLevel))
			{
#ifdef	_M_CEE
				LogMessage (pLogLevel|LogHighVolume, _T("  [%u] Sounds"), lCount);
#else
				LogMessage (pLogLevel|LogHighVolume, _T("  [%u] Sounds"), mSoundIndex.GetCount());
#endif				
			}
			lRet = true;
		}
		catch AnyExceptionDebug
	}

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
CAgentFileImage^ CAgentFileAcs::ReadAcsImage (DWORD pOffset, DWORD pSize, UINT pImageNum, bool p32Bit, System::Drawing::Color pBkColor, UINT pLogLevel)
#else
CAgentFileImage* CAgentFileAcs::ReadAcsImage (DWORD pOffset, DWORD pSize, UINT pImageNum, bool p32Bit, const COLORREF* pBkColor, UINT pLogLevel)
#endif
{
#ifdef	_M_CEE
	CAgentFileImage^		lRet = nullptr;
	CAgentFileImage^		l32BitImage = nullptr;
#else
	tPtr <CAgentFileImage>	lRet;
	tPtr <CAgentFileImage>	l32BitImage;
#endif

	try
	{
#ifdef	_M_CEE
		array <BYTE>^	lBlockBuffer;
		pin_ptr <BYTE>	lBlock;

		if	(IsOpen)
		{
			mFileStream->Seek (pOffset, SeekOrigin::Begin);
			lBlockBuffer = mFileReader->ReadBytes (pSize);
			lBlock = &lBlockBuffer[0];
#else
		LPCBYTE	lBlock;

		if	(
				(IsOpen)
			&&	(lBlock = ((LPCBYTE)(LPVOID)mFileView)+pOffset)
			)
		{
#endif		
			LPCBYTE					lByte = lBlock;
			BYTE					lFirstByte;
			BYTE					lImageCompressed;
			DWORD					lByteCount;
#ifdef	_M_CEE
			System::Drawing::Size	lImageSize;
			pin_ptr <BYTE>			lImageBits;
			DWORD					lBitsSize;
#else
			CSize					lImageSize;
#endif			

			lFirstByte = *lByte;
			lByte++;
#ifdef	_M_CEE
			lImageSize.Width = (int)*(LPCWORD)lByte;
#else
			lImageSize.cx = (int)*(LPCWORD)lByte;
#endif			
			lByte += sizeof(WORD);
#ifdef	_M_CEE
			lImageSize.Height = (int)*(LPCWORD)lByte;
#else
			lImageSize.cy = (int)*(LPCWORD)lByte;
#endif			
			lByte += sizeof(WORD);
			lImageCompressed = *lByte;
			lByte++;
			lByteCount = *(LPCDWORD)lByte;
			lByte += sizeof(DWORD);

			if	(LogIsActive (pLogLevel))
			{
#ifdef	_M_CEE
				LogMessage (pLogLevel, _T("  Image [%4u] of [%5u] at [%8.8X] thru [%8.8X] Size [%d %d] @1 [%u] Compressed [%u] Bytes [%u]"), pImageNum, pSize, lBlock, lBlock+pSize-1, lImageSize.Width, lImageSize.Height, lFirstByte, lImageCompressed, lByteCount);
#else
				LogMessage (pLogLevel, _T("  Image [%4u] of [%5u] at [%8.8X] thru [%8.8X] Size [%d %d] @1 [%u] Compressed [%u] Bytes [%u]"), pImageNum, pSize, lBlock, lBlock+pSize-1, lImageSize.cx, lImageSize.cy, lFirstByte, lImageCompressed, lByteCount);
#endif				
			}

#ifdef	_DUMP_IMAGE
			if	(LogIsActive (MaxLogLevel (pLogLevel,_DUMP_IMAGE)))
			{
				LogDump (_DUMP_IMAGE, (LPBYTE)lBlock, lByteCount+10, _T("    "), true);
				if	(lByteCount < pSize)
				{
					LPCDWORD	lPart2 = (LPCDWORD)(lBlock+lByteCount+10);
					LogMessage (pLogLevel, _T("    Part 2 Compressed [%u] Decoded [%u]"), lPart2[0], lPart2[1]);
					LogDump (_DUMP_IMAGE, (LPBYTE)lBlock+lByteCount+10, pSize-lByteCount-10, _T("      "), true);
#if FALSE
					if	(lPart2 [0])
					{
						tArrayPtr <BYTE>	lPart2Decoded;

						if	(
								(lPart2Decoded = new BYTE [lPart2[1]])
							&&	(DecodeImage ((LPCBYTE)(lPart2+2), lPart2[0], (LPCBYTE)lPart2Decoded, lPart2[1], 0, 0))
							)
						{
							LogMessage (pLogLevel, _T("    Part 2 Decoded [%u]"), lPart2[1]);
							LogDump (_DUMP_IMAGE, (LPCBYTE)lPart2Decoded, lPart2[1], _T("      "), true);
						}
					}
#endif
				}
			}
#endif
			if	(
					(lFirstByte > 0)
#ifdef	_M_CEE
				&&	(lImageSize.Width > 0)
				&&	(lImageSize.Height > 0)
				&&	(lRet = gcnew CAgentFileImage)
#else
				&&	(lImageSize.cx > 0)
				&&	(lImageSize.cy > 0)
				&&	(lRet = new tS <CAgentFileImage>)
#endif
				)
			{
				lRet->mImageNum = pImageNum;
				lRet->mImageSize = lImageSize;

				if	(lImageCompressed)
				{
#ifdef	_M_CEE
					lBitsSize = (((lImageSize.Width + 3) / 4) * 4) * lImageSize.Height;
#else
					lRet->mBitsSize = (((lImageSize.cx + 3) / 4) * 4) * lImageSize.cy;
#endif
#ifdef	_M_CEE
					if	(lRet->mBits = gcnew array <BYTE> (lBitsSize))
#else
					if	(lRet->mBits = new BYTE [lRet->mBitsSize])
#endif					
					{
#ifdef	_M_CEE
						lImageBits = &lRet->mBits[0];
						if	(!DecodeImage (lByte, lByteCount, lImageBits, lBitsSize, lImageSize.Width, lImageSize.Height))
#else						
						if	(!DecodeImage (lByte, lByteCount, lRet->mBits, lRet->mBitsSize, lImageSize.cx, lImageSize.cy))
#endif						
						{
							LogMessage (LogNormal|LogTime, _T("*** [%s] Decode image [%u] failed ***"), mPath, pImageNum);
							//lRet = NULL;
						}
					}
					else
					{
#ifdef	_M_CEE
						lRet = nullptr;
#else
						lRet = NULL;
#endif						
					}
				}
				else
				{
#ifdef	_M_CEE
					if	(lRet->mBits = gcnew array <BYTE> (lByteCount))
#else
					if	(lRet->mBits = new BYTE [lByteCount])
#endif					
					{
#ifdef	_M_CEE
						lImageBits = &lRet->mBits[0];
						memcpy (lImageBits, lByte, lByteCount);
#else
						lRet->mBitsSize = lByteCount;
						memcpy ((LPBYTE)lRet->mBits, lByte, lByteCount);
#endif						
					}
					else
					{
#ifdef	_M_CEE
						lRet = nullptr;
#else
						lRet = NULL;
#endif						
					}
				}
			}

			if	(
					(p32Bit)
				&&	(lRet)
#ifdef	_M_CEE
				&&	(mHeader->Palette)
				&&	(l32BitImage = gcnew CAgentFileImage)
				&&	(lBitsSize = lRet->mImageSize.Width * lRet->mImageSize.Height * 4)
				&&	(l32BitImage->mBits = gcnew array <BYTE> (lBitsSize))
#else
				&&	(mHeader.Palette)
				&&	(l32BitImage = new tS <CAgentFileImage>)
				&&	(l32BitImage->mBitsSize = lRet->mImageSize.cx * lRet->mImageSize.cy * 4)
				&&	(l32BitImage->mBits = new BYTE [l32BitImage->mBitsSize])
#endif				
				)
			{
#ifdef	_M_CEE
				System::Drawing::Point	lPixel;
				int						lSrcNdx;
				int						lTrgNdx;
#else			
				CPoint					lPixel;
				INT_PTR					lSrcNdx;
				INT_PTR					lTrgNdx;
#endif
				int						lSrcScanBytes;
				int						lTrgScanBytes;

				l32BitImage->mImageNum = pImageNum;
				l32BitImage->mImageSize = lImageSize;
				l32BitImage->mIs32Bit = true;

#ifdef	_M_CEE
				lSrcScanBytes = ((lImageSize.Width + 3) / 4) * 4;
				lTrgScanBytes = lImageSize.Width * 4;
#else
				lSrcScanBytes = ((lImageSize.cx + 3) / 4) * 4;
				lTrgScanBytes = lImageSize.cx * 4;
#endif				

#ifdef	_M_CEE
				for	(lPixel.Y = 0; lPixel.Y < lImageSize.Height; lPixel.Y++)
#else
				for	(lPixel.y = 0; lPixel.y < lImageSize.cy; lPixel.y++)
#endif				
				{
#ifdef	_M_CEE
					lSrcNdx = lPixel.Y * lSrcScanBytes;
					lTrgNdx = lPixel.Y * lTrgScanBytes;
#else
					lSrcNdx = lPixel.y * lSrcScanBytes;
					lTrgNdx = lPixel.y * lTrgScanBytes;
#endif				

#ifdef	_M_CEE
					for	(lPixel.X = 0; lPixel.X < lImageSize.Width; lPixel.X++)
#else
					for	(lPixel.x = 0; lPixel.x < lImageSize.cx; lPixel.x++)
#endif				
					{
#ifdef	_M_CEE
						lImageBits = &l32BitImage->mBits[lTrgNdx];
						if	(lRet->mBits [lSrcNdx] == mHeader->mTransparency)
						{
							if	(
									(pBkColor != Color::Empty)
								&&	(pBkColor != Color::Transparent)
								)
							{
								*(LPCOLORREF)(LPBYTE)lImageBits = pBkColor.ToArgb() | 0xFF000000;
							}
							else
							{
								*(LPCOLORREF)(LPBYTE)lImageBits = 0;
							}
						}
						else
						{
							*(LPCOLORREF)(LPBYTE)lImageBits = mHeader->mPalette->Entries [(long)lRet->mBits [lSrcNdx]].ToArgb() | 0xFF000000;
						}
#else
						if	(lRet->mBits [lSrcNdx] == mHeader.mTransparency)
						{
							if	(pBkColor)
							{
								*(LPCOLORREF)(l32BitImage->mBits+lTrgNdx) = *pBkColor | 0xFF000000;
							}
							else
							{
								*(LPCOLORREF)(l32BitImage->mBits+lTrgNdx) = 0;
							}
						}
						else
						{
							*(LPCOLORREF)(l32BitImage->mBits+lTrgNdx) = mHeader.mPalette [(long)lRet->mBits [lSrcNdx]] | 0xFF000000;
						}
#endif							
						lSrcNdx++;
						lTrgNdx += 4;
					}
				}

#ifdef	_M_CEE
				lRet = l32BitImage;
#else
				lRet = l32BitImage.Detach();
#endif				
			}
		}
	}
	catch AnyExceptionDebug

#ifdef	_M_CEE
	return lRet;
#else
	return lRet.Detach ();
#endif	
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
array <BYTE>^ CAgentFileAcs::ReadAcsSound (DWORD pOffset, DWORD pSize, UINT pSoundNum, UINT pLogLevel)
#else
LPCVOID CAgentFileAcs::ReadAcsSound (DWORD pOffset, DWORD pSize, UINT pSoundNum, UINT pLogLevel)
#endif
{
#ifdef	_M_CEE
	array <BYTE>^	lRet = nullptr;
#else
	LPCVOID			lRet = NULL;
#endif

	try
	{
#ifdef	_M_CEE
		array <BYTE>^	lBlockBuffer;

		if	(IsOpen)
		{
			mFileStream->Seek (pOffset, SeekOrigin::Begin);
			lBlockBuffer = mFileReader->ReadBytes (pSize);

			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel, _T("  Sound [%4u] of [%5u] at [%8.8X] thru [%8.8X]"), pSoundNum, pSize, pOffset, pOffset+pSize-1);
			}
			lRet = lBlockBuffer;
		}			
#else
		LPCBYTE	lBlock;

		if	(
				(IsOpen)
			&&	(lBlock = ((LPCBYTE)(LPVOID)mFileView)+pOffset)
			)
		{
			if	(LogIsActive (pLogLevel))
			{
				LogMessage (pLogLevel, _T("  Sound [%4u] of [%5u] at [%8.8X] thru [%8.8X]"), pSoundNum, pSize, lBlock, lBlock+pSize-1);
			}
			lRet = (LPCVOID) lBlock;
		}
#endif		
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifndef	_M_CEE
/////////////////////////////////////////////////////////////////////////////

void CAgentFileAcs::DumpAcsImages (UINT pLogLevel)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			tPtr <CAgentFileImage>	lImage;
			INT_PTR					lImageNdx;

			LogMessage (pLogLevel, _T("[%s] Images [%d]"), mPath, mImageIndex.GetCount());

			for	(lImageNdx = 0; lImageNdx < (INT_PTR)mImageIndex.GetCount(); lImageNdx++)
			{
				if	(lImage = ReadAcsImage (mImageIndex [lImageNdx].LowPart, mImageIndex [lImageNdx].HighPart, (UINT)lImageNdx, false, NULL, pLogLevel))
				{
#ifdef	_SAVE_IMAGE
					SaveImage (lImage);
#endif
				}
				else
				{
					LogMessage (pLogLevel, _T("*** [%s] Load image [%d] failed ***"), mPath, lImageNdx);
				}
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

void CAgentFileAcs::DumpAcsImage (INT_PTR pImageNdx, UINT pLogLevel)
{
#ifdef	_DEBUG
	if	(
			(LogIsActive (pLogLevel))
		&&	(pImageNdx >= 0)
		&&	(pImageNdx < (INT_PTR)mImageIndex.GetCount())
		)
	{
		try
		{
			tPtr <CAgentFileImage>	lImage;

			if	(lImage = ReadAcsImage (mImageIndex [pImageNdx].LowPart, mImageIndex [pImageNdx].HighPart, (UINT)pImageNdx, false, NULL, pLogLevel))
			{
#ifdef	_SAVE_IMAGE
				SaveImage (lImage);
#endif
			}
			else
			{
				LogMessage (pLogLevel, _T("*** [%s] Load image [%d] failed ***"), mPath, pImageNdx);
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
