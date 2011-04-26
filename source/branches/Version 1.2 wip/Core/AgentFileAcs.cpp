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
	LogStart (false);
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
bool CAgentFileAcs::Open (const System::String^ pPath)
{
	bool	lRet = false;
	String^	lPath = ParseFilePath (pPath);

#ifdef	_DEBUG_LOAD
	UINT	lLogLevel = mLogLevel;
	mLogLevel = MinLogLevel (mLogLevel, _DEBUG_LOAD);
#endif
	if	(LogIsActive (mLogLevel))
	{
		LogMessage (mLogLevel, _T("Open [%s]"), _B(lPath));
	}

	Close ();
	mPath = lPath;
	lRet = LoadFile (lPath);

	if	(!lRet)
	{
		mPath = nullptr;
	}

#ifdef	_DEBUG_LOAD
	mLogLevel = lLogLevel;
#endif
	return lRet;
}
#else
HRESULT CAgentFileAcs::Open (LPCTSTR pPath)
{
	HRESULT		lResult = S_OK;
	CAtlString	lPath = ParseFilePath (pPath);

#ifdef	_DEBUG_LOAD
	UINT lLogLevel = mLogLevel;
	mLogLevel = MinLogLevel (mLogLevel, _DEBUG_LOAD);
#endif

	Close ();
	mPath = lPath;

	if	(LogIsActive (mLogLevel))
	{
		LogMessage (mLogLevel, _T("Open [%s]"), lPath);
	}

	lResult = LoadFile (lPath);

	if	(FAILED (lResult))
	{
		mPath.Empty ();
	}
#ifdef	_DEBUG_LOAD
	mLogLevel = lLogLevel;
#endif	
	return lResult;
}
#endif

#ifdef	_M_CEE
bool CAgentFileAcs::Save (const System::String^ pPath, CAgentFile^ pSource)
{
	bool	lRet = false;
	String^	lOldPath = mPath;
	String^	lNewPath = nullptr;

	if	(!String::IsNullOrEmpty (const_cast <String^> (pPath)))
	{
		lNewPath = ParseFilePath (pPath);
	}
	if	(LogIsActive (mLogLevel))
	{
		if	(pSource == this)
		{
			LogMessage (mLogLevel, _T("Save [%s] as [%s]"), _B(mPath), _B(lNewPath));
		}
		else
		if	(!String::IsNullOrEmpty (lNewPath))
		{
			LogMessage (mLogLevel, _T("Save [%s] as [%s]"), _B(pSource->Path), _B(lNewPath));
		}
		else
		{
			LogMessage (mLogLevel, _T("Save [%s]"), _B(mPath));
		}
	}

	if	(
			(
				(IsOpen)
			&&	(!IsReadOnly)
			)
		||	(!String::IsNullOrEmpty (lNewPath))
		||	(
				(pSource != this)
			&&	(pSource->IsOpen)
			)
		)
	{
		try
		{
			if	(!String::IsNullOrEmpty (lNewPath))
			{
				mPath = lNewPath;
			}

			CloseFile ();

			try
			{
				mFileStream = gcnew FileStream (mPath, FileMode::Create, FileAccess::Write, FileShare::Read);
				mFileWriter = gcnew BinaryWriter (mFileStream);
			}
			catch AnyExceptionDebug

			if	(
					(mFileStream)
				&&	(mFileWriter)
				)
			{
				FreeUnusedImages ();
				FreeUnusedSounds ();

				mFileSize = sizeof(DWORD)+(sizeof(ULARGE_INTEGER)*4);
				mFileSize += WriteImages (mFileSize, pSource);
				mFileSize += WriteSounds (mFileSize, pSource);
				mFileSize += WriteGestures (mFileSize, pSource);
				mFileSize += WriteImageIndex (mFileSize, pSource);
				mFileSize += WriteSoundIndex (mFileSize, pSource);
				mFileSize += WriteAcsHeader (mFileSize, pSource);
				mFileWriter->Flush ();
				lRet = true;
			}
			if	(lRet)
			{
				IsDirty = false;
			}
			else
			{
				mPath = lOldPath;
			}

			try
			{
				CloseFile ();

				if	(!String::IsNullOrEmpty (mPath))
				{
					mFileStream = gcnew FileStream (mPath, FileMode::Open, FileAccess::ReadWrite, FileShare::Read);
				}
				if	(mFileStream)
				{
					mFileSize = (UInt32)mFileStream->Length;
					mFileReader = gcnew BinaryReader (mFileStream);
				}
			}
			catch AnyExceptionDebug
		}
		catch AnyExceptionDebug
	}
	return lRet;
}
#endif

void CAgentFileAcs::Close ()
{
	mFileNamesOffset = 0;
	mFileNamesSize = 0;
	mFileStatesOffset = 0;
	mFileStatesSize = 0;
	FreeImageIndex ();
	FreeSoundIndex ();
	__super::Close ();
	mSignature = sAcsFileSignature;
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
bool CAgentFileAcs::LoadFile (System::String^ pPath)
#else
HRESULT CAgentFileAcs::LoadFile (LPCTSTR pPath)
#endif
{
#ifdef	_M_CEE
	bool	lRet = __super::LoadFile (pPath);
#else
	HRESULT	lResult = __super::LoadFile (pPath);
#endif

#ifdef	_M_CEE
	if	(lRet)
#else
	if	(SUCCEEDED (lResult))
#endif
	{
		ReadImageIndex ();
		ReadSoundIndex ();

#ifdef	_M_CEE
#ifdef	_DEBUG_NOT
		if	(
				(mImageIndex)
			&&	(mImageIndex->Length > 0)
			)
		{
			CAgentFileImage^		lImage;
			
			for (int lNdx = 0; lNdx < mImageIndex->Length; lNdx++)
			{
				lImage = GetImage (lNdx, false, Color::Transparent, LogDebugFast);
			}
		}
#endif
#else
#ifdef	_DEBUG_NOT
		if	(mImageIndex.GetCount() > 0)
		{
			tPtr <CAgentFileImage>	lImage;
			
			for (int lNdx = 0; lNdx < (int)mImageIndex.GetCount(); lNdx++)
			{
				lImage = GetImage (lNdx, false, NULL, LogDebugFast);
			}
		}
#endif
#endif

#ifdef	_DUMP_IMAGE
		if	(LogIsActive (MaxLogLevel (mLogLevel, _DUMP_IMAGE)))
		{
			DumpAcsImages (mLogLevel);
		}
#endif
#ifdef	_SAVE_IMAGE
		if	(LogIsActive (MaxLogLevel (mLogLevel, _SAVE_IMAGE)))
		{
			DumpAcsImages (mLogLevel);
		}
#endif
#ifdef	_DEBUG_BLOCKS
		if	(IsAcsFile ())
		{
			DumpBlocks (MaxLogLevel (mLogLevel,_DEBUG_BLOCKS));
		}
#endif
	}
#ifdef	_M_CEE
	return lRet;
#else
	return lResult;
#endif
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
CAgentFileImage^ CAgentFileAcs::GetImage (int pImageNdx, bool p32Bit, System::Drawing::Color pBkColor)
#else
CAgentFileImage* CAgentFileAcs::GetImage (INT_PTR pImageNdx, bool p32Bit, const COLORREF* pBkColor)
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
		lImage = ReadAcsImage (mImageIndex [pImageNdx].Key, mImageIndex [pImageNdx].Value, (UINT)pImageNdx, p32Bit, pBkColor);
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
		lImage = ReadAcsImage (mImageIndex [pImageNdx].LowPart, mImageIndex [pImageNdx].HighPart, (UINT)pImageNdx, p32Bit, pBkColor);
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
		lRet = (int) mSoundIndex [pSoundNdx].Value;
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
array <BYTE>^ CAgentFileAcs::GetSound (int pSoundNdx)
{
	array <BYTE>^	lSound = nullptr;

	if	(
			(IsOpen)
		&&	(mSoundIndex)
		&&	(pSoundNdx >= 0)
		&&	(pSoundNdx < mSoundIndex->Length)
		)
	{
		lSound = ReadAcsSound (mSoundIndex [pSoundNdx].Key, mSoundIndex [pSoundNdx].Value, (UINT)pSoundNdx);
	}
	return lSound;
}
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

#ifdef	_M_CEE
bool CAgentFileAcs::ReadHeader ()
#else
HRESULT CAgentFileAcs::ReadHeader ()
#endif
{
#ifdef	_M_CEE
	bool	lRet = __super::ReadHeader ();
#else
	HRESULT	lResult = __super::ReadHeader ();
#endif

	if	(
#ifdef	_M_CEE
			(lRet)
#else
			(SUCCEEDED (lResult))
#endif
		&&	(
				(mSignature != sAcsFileSignature)
			||	(!ReadAcsHeader ())
			)
		)
	{
#ifdef	_M_CEE
		throw gcnew Exception ("The specified file is not a Microsoft Agent 2.x character file.");
#else
		lResult = AGENTPROVERROR_MAGIC;
#endif
	}

#ifdef	_M_CEE
	return lRet;
#else
	return lResult;
#endif
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentFileAcs::ReadAcsHeader ()
{
	bool	lRet = false;

#ifdef	_M_CEE
	array <BYTE>^	lBlockBuffer = nullptr;
	DWORD			lBlockOffset = 0;
	DWORD			lBlockLength = 0;

	if	(IsOpen)
	{
		try
		{
			ULARGE_INTEGER	lBlockDef;

			mFileStream->Seek (sizeof(DWORD), SeekOrigin::Begin);
			lBlockDef.QuadPart = mFileReader->ReadUInt64 ();
			if	(lBlockDef.HighPart)
			{
				mFileStream->Seek (lBlockOffset=lBlockDef.LowPart, SeekOrigin::Begin);
				lBlockBuffer = mFileReader->ReadBytes (lBlockLength=lBlockDef.HighPart);
			}
			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel, _T("  [%s] Read Header       of [%u] at [%8.8X (%u)]"), _B(mPath), lBlockLength, lBlockOffset, lBlockOffset);
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
		if	(LogIsActive (mLogLevel))
		{
			LogMessage (mLogLevel, _T("  [%s] Read Header       of [%u] at [%8.8X (%u)]"), mPath, lBlockLength, lBlockOffset, lBlockOffset);
		}
#endif

		try
		{
#ifdef	_M_CEE
			pin_ptr <BYTE>	lBlock = &lBlockBuffer[0];
			LPCBYTE			lByte = lBlock;
#else
			LPCBYTE			lByte = lBlock;
#endif
			lByte = (LPCBYTE)ReadBufferHeader (lByte);

#ifdef	_M_CEE
			if	(mHeader->mStyle & (DWORD)AgentCharStyle::CharStyleTts)
#else
			if	(mHeader.mStyle & CharStyleTts)
#endif
			{
				lByte = (LPCBYTE)ReadBufferTts (lByte, true);
			}
#ifdef	_M_CEE
			if	(mHeader->mStyle & (DWORD)AgentCharStyle::CharStyleBalloon)
#else
			if	(mHeader.mStyle & CharStyleBalloon)
#endif
			{
				lByte = (LPCBYTE)ReadBufferBalloon (lByte, true);
			}
			lByte = (LPCBYTE)ReadBufferPalette (lByte);
			lByte = (LPCBYTE)ReadBufferIcon (lByte);

			mFileNamesOffset -= lBlockOffset;
			mFileStatesOffset = (DWORD)(lByte - (LPBYTE)lBlock);
			mFileStatesSize = mFileNamesOffset - mFileStatesOffset;
			lRet = true;

			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel, _T("  [%s] Read Header       of [%u] at [%8.8X (%u)] States Size [%d] Offset [%d] Names Size [%d] Offset [%d (%d)]"), _B(mPath), lBlockLength, lBlockOffset, lBlockOffset, mFileStatesSize, mFileStatesOffset, mFileNamesSize, mFileNamesOffset, mFileNamesOffset+lBlockOffset);
			}
		}
		catch AnyExceptionDebug

#ifndef	_M_CEE
		if	(LogIsActive (mLogLevel))
		{
			Log (mLogLevel);
		}
#endif
	}

	return lRet;
}

LPCVOID CAgentFileAcs::ReadBufferHeader (LPCVOID pBuffer)
{
	LPCBYTE	lByte = (LPCBYTE)pBuffer;

	try
	{
		mVersionMinor = *(LPCWORD)lByte;
		lByte += sizeof (WORD);
		mVersionMajor = *(LPCWORD)lByte;
		lByte += sizeof (WORD);
		mFileNamesOffset = (*(LPCDWORD)lByte);
		lByte += sizeof(DWORD);
		mFileNamesSize = (*(LPCDWORD)lByte);
		lByte += sizeof(DWORD);

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
	}
	catch AnyExceptionDebug

	return lByte;
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
DWORD CAgentFileAcs::WriteAcsHeader (DWORD pFileOffset, CAgentFile^ pSource)
{
	DWORD	lBlockLength = 0;

	if	(
			(IsOpen)
		&&	(!IsReadOnly)
		)
	{
		array <BYTE>^	lBlockBuffer;

		lBlockLength += (DWORD)WriteBufferHeader (NULL, pSource->Header);
		if	(pSource->Header->mStyle & (DWORD)AgentCharStyle::CharStyleTts)
		{
			lBlockLength += (DWORD)WriteBufferTts (NULL, pSource->Tts, true);
		}
		if	(pSource->Header->mStyle & (DWORD)AgentCharStyle::CharStyleBalloon)
		{
			lBlockLength += (DWORD)WriteBufferBalloon (NULL, pSource->Balloon, true);
		}
		lBlockLength += (DWORD)WriteBufferPalette (NULL, pSource->Header);
		lBlockLength += (DWORD)WriteBufferIcon (NULL, pSource->Header);

		mFileStatesOffset = lBlockLength;
		mFileStatesSize = (DWORD)WriteBufferStates (NULL, pSource->States, true);
		lBlockLength += mFileStatesSize;

		mFileNamesOffset = lBlockLength + pFileOffset;
		mFileNamesSize = (DWORD)WriteBufferNames (NULL, pSource->Names, true);
		lBlockLength += mFileNamesSize;

		lBlockBuffer = gcnew array <BYTE> (lBlockLength);

		try
		{
			pin_ptr <BYTE>	lBlock = &lBlockBuffer[0];
			LPBYTE			lByte = lBlock;

			lByte = (LPBYTE)WriteBufferHeader (lByte, pSource->Header);
			if	(pSource->Header->mStyle & (DWORD)AgentCharStyle::CharStyleTts)
			{
				lByte = (LPBYTE)WriteBufferTts (lByte, pSource->Tts, true);
			}
			if	(pSource->Header->mStyle & (DWORD)AgentCharStyle::CharStyleBalloon)
			{
				lByte = (LPBYTE)WriteBufferBalloon (lByte, pSource->Balloon, true);
			}
			lByte = (LPBYTE)WriteBufferPalette (lByte, pSource->Header);
			lByte = (LPBYTE)WriteBufferIcon (lByte, pSource->Header);

			lByte = lBlock + mFileStatesOffset;
			lByte = (LPBYTE)WriteBufferStates (lByte, pSource->States, true);

			lByte = lBlock + mFileNamesOffset - pFileOffset;
			lByte = (LPBYTE)WriteBufferNames (lByte, pSource->Names, true);
		}
		catch AnyExceptionDebug

		try
		{
			ULARGE_INTEGER	lBlockDef;

			mFileStream->Seek (0, SeekOrigin::Begin);
			mFileWriter->Write ((UInt32)mSignature);

			lBlockDef.LowPart = pFileOffset;
			lBlockDef.HighPart = lBlockLength;
			mFileStream->Seek (sizeof(DWORD), SeekOrigin::Begin);
			mFileWriter->Write (lBlockDef.QuadPart);
			mFileStream->Seek (pFileOffset, SeekOrigin::Begin);
			mFileWriter->Write (lBlockBuffer);

			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel, _T("  [%s] Write Header      of [%u] at [%8.8X (%u)] States Size [%d] Offset [%d] Names Size [%d] Offset [%d (%d)]"), _B(mPath), lBlockLength, pFileOffset, pFileOffset, mFileStatesSize, mFileStatesOffset, mFileNamesSize, mFileNamesOffset-pFileOffset, mFileNamesOffset);
			}
		}
		catch AnyExceptionDebug
	}
	return lBlockLength;
}

LPVOID CAgentFileAcs::WriteBufferHeader (LPVOID pBuffer, CAgentFileHeader^ pHeader)
{
	LPBYTE	lByte = (LPBYTE)pBuffer;

	try
	{
		if	(pBuffer)
		{
			*(LPWORD)lByte = mVersionMinor;
		}
		lByte += sizeof (WORD);
		if	(pBuffer)
		{
			*(LPWORD)lByte = mVersionMajor;
		}
		lByte += sizeof (WORD);
		if	(pBuffer)
		{
			(*(LPDWORD)lByte) = mFileNamesOffset;
		}
		lByte += sizeof(DWORD);
		if	(pBuffer)
		{
			(*(LPDWORD)lByte) = mFileNamesSize;
		}
		lByte += sizeof(DWORD);

		if	(pBuffer)
		{
			array <BYTE>^	lBytes = pHeader->mGuid.ToByteArray ();
			pin_ptr <BYTE>	lPinned = &lBytes [0];
			memcpy (lByte, lPinned, sizeof(GUID));
		}
		lByte += sizeof (GUID);
		if	(pBuffer)
		{
			*(LPWORD)lByte = pHeader->mImageSize.Width;
		}
		lByte += sizeof (WORD);
		if	(pBuffer)
		{
			*(LPWORD)lByte = pHeader->mImageSize.Height;
		}
		lByte += sizeof (WORD);
		if	(pBuffer)
		{
			*lByte = pHeader->mTransparency;
		}
		lByte++;
		if	(pBuffer)
		{
			*(LPDWORD)lByte = pHeader->mStyle;
		}
		lByte += sizeof(DWORD);

		if	(pBuffer)
		{
			*(LPDWORD)lByte = 2; // Unknown
		}
		lByte += sizeof(DWORD);
	}
	catch AnyExceptionDebug

	return lByte;
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentFileAcs::ReadNames (bool pFirstLetterCaps)
{
	bool	lRet = false;

#ifdef	_M_CEE
	array <BYTE>^	lBlockBuffer = nullptr;

	FreeNames ();

	if	(IsOpen)
	{
		try
		{
			ULARGE_INTEGER	lBlockDef;
			DWORD			lBlockOffset = 0;
			DWORD			lBlockLength = 0;
			pin_ptr <BYTE>	lBlock;

			mFileStream->Seek (sizeof(DWORD), SeekOrigin::Begin);
			lBlockDef.QuadPart = mFileReader->ReadUInt64 ();
			mFileStream->Seek ((lBlockOffset=lBlockDef.LowPart)+mFileNamesOffset, SeekOrigin::Begin);
			lBlockBuffer = mFileReader->ReadBytes (lBlockLength=lBlockDef.HighPart-mFileNamesOffset);
			lBlock = &lBlockBuffer[0];

#ifdef	_DEBUG
			System::Diagnostics::Debug::Assert (mFileNamesSize == lBlockLength);
#endif
			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel, _T("  [%s] Read %3.3u Names    of [%d] at [%8.8X (%u)] in [%8.8X]"), _B(mPath), *(LPCWORD)lBlock, mFileNamesSize, mFileNamesOffset, mFileNamesOffset, lBlockOffset);
			}

			ReadBufferNames (lBlock, mFileNamesSize, true, pFirstLetterCaps);

			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel, _T("  [%s] Read %3.3d Names"), _B(mPath), (mNames ? mNames->Count : 0));
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

	if	(LogIsActive (mLogLevel))
	{
		LogMessage (mLogLevel, _T("ReadNames [%s]"), mPath);
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
			LPCBYTE	lByte = lBlock+mFileNamesOffset;

			ATLASSERT (mFileNamesOffset == ((LPCDWORD)lBlock) [1] - lBlockOffset);
			ATLASSERT (mFileNamesSize == lBlockLength-mFileNamesOffset);
			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel|LogHighVolume, _T("  [%s] Read %3.3u Names    of [%u] at [%8.8X] in [%8.8X]"), mPath, *(LPCWORD)lByte, mFileNamesSize, mFileNamesOffset, lBlockOffset);
			}

			lByte = (LPCBYTE)ReadBufferNames (lByte, mFileNamesSize, true, pFirstLetterCaps);

#ifdef	_DEBUG_LOAD
			if	(LogIsActive (MaxLogLevel (mLogLevel, _DEBUG_LOAD)))
			{
				LogMessage (_DEBUG_LOAD, _T("    Left [%u]"), (long) lBlockLength - (long) (lByte-lBlock));
			}
#endif
			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel, _T("  [%s] Read %3.3d Names"), mPath, mNames.GetCount());
			}
			lRet = true;
		}
		catch AnyExceptionDebug
	}
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentFileAcs::ReadStates ()
{
	bool	lRet = false;
#ifdef	_M_CEE
	array <BYTE>^	lBlockBuffer = nullptr;

	FreeStates ();

	if	(IsOpen)
	{
		try
		{
			ULARGE_INTEGER	lBlockDef;
			DWORD			lBlockOffset = 0;
			DWORD			lBlockLength = 0;
			pin_ptr <BYTE>	lBlock;

			mFileStream->Seek (sizeof(DWORD), SeekOrigin::Begin);
			lBlockDef.QuadPart = mFileReader->ReadUInt64 ();
LogMessage (LogIfActive, _T("Block [%u] States [%d] [%d]"), lBlockDef.HighPart, mFileStatesSize, mFileNamesOffset-mFileStatesOffset);			
			mFileStream->Seek ((lBlockOffset=lBlockDef.LowPart)+mFileStatesOffset, SeekOrigin::Begin);
			lBlockBuffer = mFileReader->ReadBytes (lBlockLength=mFileNamesOffset-mFileStatesOffset);
			lBlock = &lBlockBuffer [0];

#ifdef	_DEBUG
			System::Diagnostics::Debug::Assert (mFileStatesSize == lBlockLength);
#endif
			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel|LogHighVolume, _T("  [%s] Read %3.3u States   at [%8.8X (%d)] in [%8.8X]"), _B(mPath), *(LPCWORD)lBlock, mFileStatesSize, mFileStatesOffset, mFileStatesOffset, lBlockOffset);
			}

			ReadBufferStates (lBlock, mFileStatesSize, true);

			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel, _T("  [%s] Read %3.3d States"), _B(mPath), mStates->Count);
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

	if	(LogIsActive (mLogLevel))
	{
		LogMessage (mLogLevel, _T("ReadStates [%s]"), mPath);
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

			ATLASSERT (mFileStatesSize == mFileNamesOffset-mFileStatesOffset);
			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel|LogHighVolume, _T("  [%s] Read %3.3u States   of [%u] at [%8.8X]"), mPath, *(LPCWORD)lByte, mFileStatesSize, lByte-lBlock);
			}
#ifdef	_DEBUG_LOAD
			if	(LogIsActive (MaxLogLevel (mLogLevel, _DEBUG_LOAD)))
			{
				LogDump (_DEBUG_LOAD|LogHighVolume, (LPBYTE)lByte, mFileStatesSize, _T("    "), true);
			}
#endif

			lByte = (LPCBYTE)ReadBufferStates (lByte, mFileStatesSize, true);

			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel, _T("  [%s] Read %3.3d States"), mPath, mStates.mGestures.GetCount());
			}
			lRet = true;
		}
		catch AnyExceptionDebug
	}
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentFileAcs::ReadGestures ()
{
	bool	lRet = false;
#ifdef	_M_CEE
	array <BYTE>^	lBlockBuffer = nullptr;
	DWORD			lBlockOffset = 0;
	DWORD			lBlockLength = 0;

	FreeGestures ();

	if	(IsOpen)
	{
		try
		{
			ULARGE_INTEGER	lBlockDef;

			mFileStream->Seek (sizeof(DWORD)+sizeof(lBlockDef), SeekOrigin::Begin);
			lBlockDef.QuadPart = mFileReader->ReadUInt64 ();
			if	(lBlockDef.HighPart)
			{
				mFileStream->Seek (lBlockOffset=lBlockDef.LowPart, SeekOrigin::Begin);
				lBlockBuffer = mFileReader->ReadBytes (lBlockLength=lBlockDef.HighPart);
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

	if	(LogIsActive (mLogLevel))
	{
		LogMessage (mLogLevel, _T("ReadGestures [%s]"), mPath);
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
			String^					lGestureName;
			CAgentFileAnimation^	lAnimation;
#else
			LPCBYTE					lByte = lBlock;
			CAtlString				lGestureName;
#endif
			DWORD					lCount = *(LPCDWORD)lByte;
			LPCDWORD				lAnimationRef;

			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel|LogHighVolume, _T("  [%s] Read %3.3u Gestures at [%8.8X (%u)]"), _B(mPath), lCount, lBlockLength, lBlockOffset, lBlockOffset);
			}
#ifdef	_DEBUG_LOAD
			if	(LogIsActive (MaxLogLevel (mLogLevel, _DEBUG_LOAD)))
			{
				LogDump (_DEBUG_LOAD|LogHighVolume, (LPBYTE)lBlock, lBlockLength, _T("    "), true);
			}
#endif
			lByte += sizeof(DWORD);

			while	(lCount > 0)
			{
				lByte = (LPCBYTE)ReadBufferString (lByte, true, lGestureName);
				lAnimationRef = (LPCDWORD)lByte;
				lByte += sizeof(DWORD)*2;

#ifdef	_DEBUG_LOAD
				if	(LogIsActive (MaxLogLevel (mLogLevel, _DEBUG_LOAD)))
				{
					LogMessage (_DEBUG_LOAD, _T("  Gesture [%s] at [%8.8X (%u)] of [%8.8X (%u)]"), lGestureName, lAnimationRef[0], lAnimationRef[0], lAnimationRef[1], lAnimationRef[1]);
				}
#else
				if	(LogIsActive (mLogLevel))
				{
					LogMessage (mLogLevel|LogHighVolume, _T("    Gesture [%s] of [%u] at [%8.8X]"), _B(lGestureName), lAnimationRef[1], lAnimationRef[0]);
				}
#endif
#ifdef	_M_CEE
				lAnimation = ReadAcsAnimation (lAnimationRef[0], lAnimationRef[1]);
				lAnimation->mName = lGestureName;
				mGestures->Add (lAnimation);
#else
				mGestures.mAnimations.InsertAt (AddSortedString (mGestures.mNames, lGestureName), ReadAcsAnimation (lAnimationRef[0], lAnimationRef[1]));
#endif
				lCount--;
			}

			if	(LogIsActive (mLogLevel))
			{
#ifdef	_M_CEE
				LogMessage (mLogLevel, _T("  [%s] Read %3.3d Gestures"), _B(mPath), mGestures->Count);
#else				
				LogMessage (mLogLevel, _T("  [%s] Read %3.3d Gestures"), mPath, mGestures.mAnimations.GetCount());
#endif				
			}
			lRet = true;
		}
		catch AnyExceptionDebug
	}

	return lRet;
}

#ifdef	_M_CEE
CAgentFileAnimation^ CAgentFileAcs::ReadAcsAnimation (DWORD pOffset, DWORD pSize)
#else
CAgentFileAnimation* CAgentFileAcs::ReadAcsAnimation (DWORD pOffset, DWORD pSize)
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
			BYTE		lReturnType;
			WORD		lFrameCount = 0;
			WORD		lFrameNdx;
#ifdef	_M_CEE
			String^		lName;
			String^		lReturnName;
#else
			CAtlString	lName;
			CAtlString	lReturnName;
#endif

			lByte = (LPCBYTE)ReadBufferString (lByte, true, lName);
			lReturnType = *lByte;
			lByte++;
			lByte = (LPCBYTE)ReadBufferString (lByte, true, lReturnName);
			lFrameCount = *(LPCWORD)lByte;
			lByte += sizeof (WORD);

#ifdef	_DEBUG_ANIMATION
			if	(LogIsActive (MaxLogLevel (mLogLevel, _DEBUG_ANIMATION)))
			{
				CAtlString	lReturnString = (lReturnType == 1) ? _T("<exit branching>") : (lReturnType == 2) ? _T("<none>") : lReturnName;
				LogMessage (_DEBUG_ANIMATION, _T("    Animation [%s] Return [%u] [%s] Frames [%u]"), lName, lReturnType, lReturnString, lFrameCount);
			}
#else
			if	(LogIsActive (mLogLevel))
			{
#ifdef	_M_CEE
				LogMessage (mLogLevel|LogHighVolume, _T("    Animation [%s] Return [%u] [%s] Frames [%u]"), _B(lName), lReturnType, _B(lReturnName), lFrameCount);
#else
				LogMessage (mLogLevel|LogHighVolume, _T("    Animation [%s] Return [%u] [%s] Frames [%u]"), lName, lReturnType, lReturnName, lFrameCount);
#endif
			}
#endif
#ifdef	_DUMP_ANIMATION
			if	(LogIsActive (MaxLogLevel (mLogLevel, _DUMP_ANIMATION)))
			{
				LogDump (_DUMP_ANIMATION, (LPBYTE)lBlock, pSize, _T("        "));
			}
#endif
			if	(
					(lFrameCount > 0)
#ifdef	_M_CEE
				&&	(lRet = gcnew CAgentFileAnimation (nullptr, mGestures))
#else
				&&	(lRet = new CAgentFileAnimation)
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
					&&	(lRet->mFrames = gcnew CAgentFileFrames (nullptr, lRet))
#else
					&&	(lRet->mFrames = new CAgentFileFrame [lFrameCount])
#endif
					)
				{
#ifndef	_M_CEE
					lRet->mFrameCount = lFrameCount;
#endif
					for	(lFrameNdx = 0; lFrameNdx < lFrameCount; lFrameNdx++)
					{
#ifdef	_M_CEE
						CAgentFileFrame^	lFrame = gcnew CAgentFileFrame (nullptr, lRet->Frames);
#else
						CAgentFileFrame*	lFrame = &lRet->mFrames [(int)lFrameNdx];
#endif
						LPCBYTE				lFrameStart = lByte;
						WORD				lImageCount;
						BYTE				lBranchCount;
						BYTE				lOverlayCount;
						INT_PTR				lNdx;

#ifdef	_M_CEE
						lRet->mFrames->Add (lFrame);
#endif
						lImageCount = *(LPCWORD)lByte;
						lByte += sizeof(WORD);
#ifdef	_DEBUG_ANIMATION
						if	(LogIsActive (MaxLogLevel (mLogLevel, _DEBUG_ANIMATION)))
						{
							LogMessage (_DEBUG_ANIMATION, _T("      Frame [%hu] Images [%hu]"), lFrameNdx, lImageCount);
						}
#endif
						if	(
								(lImageCount > 0)
#ifdef	_M_CEE
							&&	(lFrame->mImages = gcnew CAgentFileFrameImages (nullptr, lFrame))
#else
							&&	(lFrame->mImages = new CAgentFileFrameImage [lImageCount])
#endif
							)
						{
#ifndef	_M_CEE
							lFrame->mImageCount = lImageCount;
#endif
							for	(lNdx = 0; lNdx < (INT_PTR)lImageCount; lNdx++)
							{
#ifdef	_M_CEE
								CAgentFileFrameImage^	lFrameImage = gcnew CAgentFileFrameImage (nullptr, lFrame);
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
								if	(LogIsActive (MaxLogLevel (mLogLevel, _DEBUG_ANIMATION)))
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
						if	(LogIsActive (MaxLogLevel (mLogLevel, _DEBUG_ANIMATION)))
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
						if	(LogIsActive (MaxLogLevel (mLogLevel, _DEBUG_ANIMATION)))
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
							&&	(lFrame->mOverlays = gcnew CAgentFileFrameOverlays (nullptr, lFrame))
#else
							&&	(lFrame->mOverlays = new CAgentFileFrameOverlay [lOverlayCount])
#endif
							)
						{
#ifndef	_M_CEE
							lFrame->mOverlayCount = lOverlayCount;
#endif
							for	(lNdx = 0; lNdx < (INT_PTR)lOverlayCount; lNdx++)
							{
#ifdef	_M_CEE
								CAgentFileFrameOverlay^	lOverlay = gcnew CAgentFileFrameOverlay (nullptr, lFrame);
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
								if	(LogIsActive (MaxLogLevel (mLogLevel, _DEBUG_ANIMATION)))
								{
									LogMessage (_DEBUG_ANIMATION, _T("        Overlay [%hu] Replace [%u] Image [%4u] at [%d %d] something [%d] [%d]"), lOverlay->OverlayNdx, lOverlay->ReplaceFlag, lOverlay->ImageNdx, lOverlay->Offset.x, lOverlay->Offset.y, lOverlay->mSomething.x, lOverlay->mSomething.y);
								}
#endif
							}
						}

#ifdef	_DUMP_ANIMATION
						if	(LogIsActive (MaxLogLevel (mLogLevel, _DUMP_ANIMATION)))
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

#ifdef	_M_CEE
DWORD CAgentFileAcs::WriteGestures (DWORD pFileOffset, CAgentFile^ pSource)
{
	DWORD	lBlockLength = 0;

	if	(
			(IsOpen)
		&&	(!IsReadOnly)
		)
	{
		array <BYTE>^							lBlockBuffer;
		array <KeyValuePair <UInt32, UInt32> >^	lGestureIndex;

		lBlockLength = sizeof(DWORD);

		if	(
				(pSource->Gestures)
			&&	(pSource->Gestures->Count > 0)
			)
		{
			UInt32	lAnimationSize;
			int		lIndexNdx = 0;

			lGestureIndex = gcnew array <KeyValuePair <UInt32, UInt32> > (pSource->Gestures->Count);

			for each (CAgentFileAnimation^ lAnimation in pSource->Gestures)
			{
				lBlockLength += (DWORD)WriteBufferString (NULL, true, lAnimation->Name);
				lBlockLength += sizeof(DWORD) * 2;
			}
			for each (CAgentFileAnimation^ lAnimation in pSource->Gestures)
			{
				lAnimationSize = (UInt32)WriteAcsAnimation (NULL, lAnimation);
				lGestureIndex [lIndexNdx++] = KeyValuePair <UInt32, UInt32> (pFileOffset+lBlockLength, lAnimationSize);
				lBlockLength += lAnimationSize;
			}
		}

		lBlockBuffer = gcnew array <BYTE> (lBlockLength);

		try
		{
			pin_ptr <BYTE>	lBlock = &lBlockBuffer[0];
			LPBYTE			lByte = lBlock;
			int				lIndexNdx = 0;

			if	(
					(pSource->Gestures)
				&&	(pSource->Gestures->Count > 0)
				)
			{
				*(LPDWORD)lByte = pSource->Gestures->Count;
				lByte += sizeof(DWORD);

				for each (CAgentFileAnimation^ lAnimation in pSource->Gestures)
				{
					lByte = (LPBYTE)WriteBufferString (lByte, true, lAnimation->Name);

					*(LPDWORD)lByte = lGestureIndex[lIndexNdx].Key;
					lByte += sizeof(DWORD);
					*(LPDWORD)lByte = lGestureIndex[lIndexNdx].Value;
					lByte += sizeof(DWORD);
					lIndexNdx++;
				}

				for each (CAgentFileAnimation^ lAnimation in pSource->Gestures)
				{
					lByte = (LPBYTE)WriteAcsAnimation (lByte, lAnimation);
				}
			}
			else
			{
				*(LPDWORD)lByte = 0;
				lByte += sizeof(DWORD);
			}
		}
		catch AnyExceptionDebug

		try
		{
			ULARGE_INTEGER	lBlockDef;

			lBlockDef.LowPart = pFileOffset;
			lBlockDef.HighPart = lBlockLength;
			mFileStream->Seek (sizeof(DWORD)+sizeof(lBlockDef), SeekOrigin::Begin);
			mFileWriter->Write (lBlockDef.QuadPart);
			mFileStream->Seek (pFileOffset, SeekOrigin::Begin);
			mFileWriter->Write (lBlockBuffer);

			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel, _T("  [%s] Write Gestures    of [%u] at [%8.8X (%u)]"), _B(mPath), lBlockLength, pFileOffset, pFileOffset);
			}
		}
		catch AnyExceptionDebug
	}
	return lBlockLength;
}

LPVOID CAgentFileAcs::WriteAcsAnimation (LPVOID pBuffer, CAgentFileAnimation^ pAnimation)
{
	LPBYTE	lByte = (LPBYTE)pBuffer;

	try
	{
		if	(pBuffer)
		{
			lByte = (LPBYTE)WriteBufferString (lByte, true, pAnimation->Name);
		}
		else
		{
			lByte += (DWORD)WriteBufferString (NULL, true, pAnimation->Name);
		}
		if	(pBuffer)
		{
			*lByte = pAnimation->ReturnType;
		}
		lByte++;
		if	(pBuffer)
		{
			lByte = (LPBYTE)WriteBufferString (lByte, true, pAnimation->ReturnName);
		}
		else
		{
			lByte += (DWORD)WriteBufferString (NULL, true, pAnimation->ReturnName);
		}
		if	(pBuffer)
		{
			*(LPWORD)lByte = pAnimation->FrameCount;
		}
		lByte += sizeof (WORD);

		if	(pAnimation->FrameCount > 0)
		{
			for each (CAgentFileFrame^ lFrame in pAnimation->Frames)
			{
				if	(pBuffer)
				{
					*(LPWORD)lByte = (WORD)lFrame->ImageCount;
				}
				lByte += sizeof(WORD);

				if	(lFrame->ImageCount > 0)
				{
					for each (CAgentFileFrameImage^ lFrameImage in lFrame->Images)
					{
						if	(pBuffer)
						{
							*(LPDWORD)lByte = (DWORD)lFrameImage->ImageNdx;
						}
						lByte += sizeof(DWORD);
						if	(pBuffer)
						{
							*(LPWORD)lByte = (WORD)lFrameImage->Offset.X;
						}
						lByte += sizeof(WORD);
						if	(pBuffer)
						{
							*(LPWORD)lByte = (WORD)lFrameImage->Offset.Y;
						}
						lByte += sizeof(WORD);
					}
				}

				if	(pBuffer)
				{
					*(LPWORD)lByte = (WORD)lFrame->SoundNdx;
				}
				lByte += sizeof(WORD);
				if	(pBuffer)
				{
					*(LPWORD)lByte = (WORD)lFrame->Duration;
				}
				lByte += sizeof(WORD);
				if	(pBuffer)
				{
					*(LPWORD)lByte = (WORD)lFrame->ExitFrame;
				}
				lByte += sizeof(WORD);

				if	(lFrame->Branching)
				{
					if	(pBuffer)
					{
						*lByte = (BYTE)lFrame->Branching->Length;
					}
					lByte++;
					if	(lFrame->Branching->Length > 0)
					{
						if	(pBuffer)
						{
							*(LPDWORD)lByte = MAKELONG (lFrame->Branching [0].mFrameNdx, lFrame->Branching [0].mProbability);
						}
						lByte += sizeof(DWORD);
					}
					if	(lFrame->Branching->Length > 1)
					{
						if	(pBuffer)
						{
							*(LPDWORD)lByte = MAKELONG (lFrame->Branching [1].mFrameNdx, lFrame->Branching [1].mProbability);
						}
						lByte += sizeof(DWORD);
					}
					if	(lFrame->Branching->Length > 2)
					{
						if	(pBuffer)
						{
							*(LPDWORD)lByte = MAKELONG (lFrame->Branching [2].mFrameNdx, lFrame->Branching [2].mProbability);
						}
						lByte += sizeof(DWORD);
					}
				}
				else
				{
					if	(pBuffer)
					{
						*lByte = 0;
					}
					lByte++;
				}

				if	(pBuffer)
				{
					*lByte = (BYTE)lFrame->OverlayCount;
				}
				lByte++;

				if	(lFrame->OverlayCount > 0)
				{
					for each (CAgentFileFrameOverlay^ lOverlay in lFrame->Overlays)
					{
						if	(pBuffer)
						{
							*lByte = (BYTE)lOverlay->OverlayType;
						}
						lByte++;
						if	(pBuffer)
						{
							*lByte = lOverlay->ReplaceFlag ? TRUE : FALSE;
						}
						lByte++;
						if	(pBuffer)
						{
							*(LPWORD)lByte = (WORD)lOverlay->ImageNdx;
						}
						lByte += sizeof(WORD);
						if	(pBuffer)
						{
							*lByte = 0; // Unknown
						}
						lByte++;
						if	(pBuffer)
						{
							*lByte = 0; // RgnFlag
						}
						lByte++;
						if	(pBuffer)
						{
							*(LPWORD)lByte = (WORD)lOverlay->Offset.X;
						}
						lByte += sizeof(WORD);
						if	(pBuffer)
						{
							*(LPWORD)lByte = (WORD)lOverlay->Offset.Y;
						}
						lByte += sizeof(WORD);
						if	(pBuffer)
						{
							*(LPWORD)lByte = 0; // Something.X;
						}
						lByte += sizeof(WORD);
						if	(pBuffer)
						{
							*(LPWORD)lByte = 0; // Something.Y;
						}
						lByte += sizeof(WORD);
					}
				}
			}
		}
		if	(LogIsActive (mLogLevel))
		{
			LogMessage (mLogLevel, _T("  [%s] Write Animation   of [%u] at [%8.8X] [%s]"), _B(mPath), lByte-(LPBYTE)pBuffer, pBuffer, _B(pAnimation->Name));
		}
	}
	catch AnyExceptionDebug

	return lByte;
}
#endif

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
	bool	lRet = false;

#ifdef	_M_CEE
	array <BYTE>^	lBlockBuffer = nullptr;
	DWORD			lBlockOffset = 0;
	DWORD			lBlockLength = 0;
	pin_ptr <BYTE>	lBlock;

	if	(LogIsActive (mLogLevel))
	{
		LogMessage (mLogLevel, _T("ReadImageIndex [%s]"), _B(mPath));
	}
	FreeImageIndex ();

	if	(IsOpen)
	{
		try
		{
			ULARGE_INTEGER	lBlockDef;

			mFileStream->Seek (sizeof(DWORD)+(sizeof(lBlockDef)*2), SeekOrigin::Begin);
			lBlockDef.QuadPart = mFileReader->ReadUInt64 ();
			if	(lBlockDef.HighPart)
			{
				mFileStream->Seek (lBlockOffset=lBlockDef.LowPart, SeekOrigin::Begin);
				lBlockBuffer = mFileReader->ReadBytes (lBlockLength=lBlockDef.HighPart);
				lBlock = &lBlockBuffer[0];
			}
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

	if	(LogIsActive (mLogLevel))
	{
		LogMessage (mLogLevel, _T("ReadImageIndex [%s]"), mPath);
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

			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel|LogHighVolume, _T("  [%u] ImageIndex [%u] at [%8.8X (%u)]"), lCount, lBlockLength, lBlockOffset, lBlockOffset);
			}
#ifdef	_DEBUG_LOAD
			if	(LogIsActive (MaxLogLevel (mLogLevel, _DEBUG_LOAD)))
			{
				LogDump (_DEBUG_LOAD|LogHighVolume, (LPBYTE)lBlock, lBlockLength, _T("    "), true);
			}
#endif
			lByte += sizeof(DWORD);

#ifdef	_M_CEE
			if	(lCount > 0)
			{
				mImageIndex = gcnew array <KeyValuePair <UInt32, UInt32> > (lCount);
			}
#endif
			for	(lEntry = 0; lEntry < lCount; lEntry++)
			{
				lIndex = (LPCDWORD) lByte;
				lByte += sizeof(DWORD) * 3;

#ifdef	_M_CEE
				mImageIndex [lArrayNdx++] = KeyValuePair <UInt32, UInt32> (lIndex [0], lIndex [1]);
#else
				lIndexEntry.LowPart = lIndex [0];
				lIndexEntry.HighPart = lIndex [1];
				mImageIndex.Add (lIndexEntry);
#endif
#ifdef	_DEBUG_INDEX
				if	(LogIsActive (MaxLogLevel (mLogLevel, _DEBUG_INDEX)))
#else
				if	(LogIsActive (mLogLevel))
#endif
				{
					LogMessage (mLogLevel, _T("    Image [%4u] of [%6u] at [%8.8X] thru [%8.8X] chksum [%8.8X]"), lEntry, lIndex[1], lIndex[0], lIndex[0]+lIndex[1]-1, lIndex[2]);
				}
			}

			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel, _T("  [%s] Read [%u] ImageIndex   of [%u] at [%8.8X (%u)]"), _B(mPath), lCount, lByte-(LPCBYTE)lBlock, (LPCBYTE)lBlock, (LPCBYTE)lBlock);
			}
			lRet = true;
		}
		catch AnyExceptionDebug
	}

	return lRet;
}

#ifdef	_M_CEE
DWORD CAgentFileAcs::WriteImageIndex (DWORD pFileOffset, CAgentFile^ pSource)
{
	DWORD	lBlockLength = 0;

	if	(
			(IsOpen)
		&&	(!IsReadOnly)
		)
	{
		array <BYTE>^	lBlockBuffer;
#if	FALSE		
		CAgentFileAcs^	lSourceAcs = safe_cast <CAgentFileAcs^> (pSource);
		array <BYTE>^	lSourceBlockBuffer;
		pin_ptr <BYTE>	lSourceBlockPinned;
		LPCDWORD		lSourceImageIndex = NULL;

		if	(lSourceAcs)
		{
			try
			{
				ULARGE_INTEGER	lBlockDef;
				DWORD			lBlockOffset = 0;

				lSourceAcs->mFileStream->Seek (sizeof(DWORD)+(sizeof(lBlockDef)*2), SeekOrigin::Begin);
				lBlockDef.QuadPart = lSourceAcs->mFileReader->ReadUInt64 ();
				if	(lBlockDef.HighPart)
				{
					lSourceAcs->mFileStream->Seek (lBlockOffset=lBlockDef.LowPart, SeekOrigin::Begin);
					lSourceBlockBuffer = lSourceAcs->mFileReader->ReadBytes (lBlockLength=lBlockDef.HighPart);
					lSourceBlockPinned = &lSourceBlockBuffer[0];
					lSourceImageIndex = (LPCDWORD)(LPBYTE)lSourceBlockPinned;
					lSourceImageIndex++;
				}
			}
			catch AnyExceptionDebug
		}
#endif
		lBlockLength = sizeof(DWORD);
		if	(
				(mImageIndex)
			&&	(mImageIndex->Length > 0)
			)
		{
			lBlockLength += sizeof(DWORD) * mImageIndex->Length * 3;
		}
		lBlockBuffer = gcnew array <BYTE> (lBlockLength);

		try
		{
			pin_ptr <BYTE>	lBlock = &lBlockBuffer[0];
			LPBYTE			lByte = lBlock;
			LPDWORD			lIndex;
			int				lImageNdx;

			if	(
					(mImageIndex)
				&&	(mImageIndex->Length > 0)
				)
			{
				*(LPDWORD)lByte = mImageIndex->Length;
				lByte += sizeof(DWORD);

				for (lImageNdx = 0; lImageNdx < mImageIndex->Length; lImageNdx++)
				{
					lIndex = (LPDWORD)lByte;
					lIndex[0] = mImageIndex [lImageNdx].Key;
					lIndex[1] = mImageIndex [lImageNdx].Value;
#if	FALSE					
					if	(lSourceImageIndex)
					{
						lIndex[2] = lSourceImageIndex[(lImageNdx*3)+2]; // chksum
					}
					else
#endif					
					{
						lIndex[2] = 0; // chksum
					}
					lByte += sizeof(DWORD) * 3;

					if	(LogIsActive (mLogLevel))
					{
						LogMessage (mLogLevel, _T("    Image [%4u] of [%6u] at [%8.8X] thru [%8.8X] chksum [%8.8X]"), lImageNdx, lIndex[1], lIndex[0], lIndex[0]+lIndex[1]-1, lIndex[2]);
					}
				}
			}
			else
			{
				*(LPDWORD)lByte = 0;
			}
		}
		catch AnyExceptionDebug

		try
		{
			ULARGE_INTEGER	lBlockDef;

			lBlockDef.LowPart = pFileOffset;
			lBlockDef.HighPart = lBlockLength;
			mFileStream->Seek (sizeof(DWORD)+(sizeof(lBlockDef)*2), SeekOrigin::Begin);
			mFileWriter->Write (lBlockDef.QuadPart);
			mFileStream->Seek (pFileOffset, SeekOrigin::Begin);
			mFileWriter->Write (lBlockBuffer);

			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel, _T("  [%s] Write ImageIndex  of [%u] at [%8.8X (%u)]"), _B(mPath), lBlockLength, pFileOffset, pFileOffset);
			}
		}
		catch AnyExceptionDebug
	}
	return lBlockLength;
}
#endif

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
	bool	lRet = false;
#ifdef	_M_CEE
	array <BYTE>^	lBlockBuffer = nullptr;
	DWORD			lBlockOffset = 0;
	DWORD			lBlockLength = 0;
	pin_ptr <BYTE>	lBlock;

	if	(LogIsActive (mLogLevel))
	{
		LogMessage (mLogLevel, _T("ReadSoundIndex [%s]"), _B(mPath));
	}
	FreeSoundIndex ();

	if	(IsOpen)
	{
		try
		{
			ULARGE_INTEGER	lBlockDef;

			mFileStream->Seek (sizeof(DWORD)+(sizeof(lBlockDef)*3), SeekOrigin::Begin);
			lBlockDef.QuadPart = mFileReader->ReadUInt64();
			if	(lBlockDef.HighPart)
			{
				mFileStream->Seek (lBlockOffset=lBlockDef.LowPart, SeekOrigin::Begin);
				lBlockBuffer = mFileReader->ReadBytes (lBlockLength=lBlockDef.HighPart);
				lBlock = &lBlockBuffer[0];
			}
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

	if	(LogIsActive (mLogLevel))
	{
		LogMessage (mLogLevel, _T("ReadSoundIndex [%s]"), mPath);
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

			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel|LogHighVolume, _T("  [%u] SoundIndex [%u] at [%8.8X (%u)]"), lCount, lBlockLength, lBlockOffset, lBlockOffset);
			}
#ifdef	_DEBUG_LOAD
			if	(LogIsActive (MaxLogLevel (mLogLevel, _DEBUG_LOAD)))
			{
				LogDump (_DEBUG_LOAD|LogHighVolume, (LPBYTE)lBlock, lBlockLength, _T("    "), true);
			}
#endif
			lByte += sizeof(DWORD);

#ifdef	_M_CEE
			if	(lCount > 0)
			{
				mSoundIndex = gcnew array <KeyValuePair <UInt32, UInt32> > (lCount);
			}
#endif
			for	(lEntry = 0; lEntry < lCount; lEntry++)
			{
				lIndex = (LPCDWORD) lByte;
				lByte += sizeof(DWORD) * 3;

#ifdef	_M_CEE
				mSoundIndex [lArrayNdx++] = KeyValuePair <UInt32, UInt32> (lIndex [0], lIndex [1]);
#else
				lIndexEntry.LowPart = lIndex [0];
				lIndexEntry.HighPart = lIndex [1];
				mSoundIndex.Add (lIndexEntry);
#endif
#ifdef	_DEBUG_INDEX
				if	(LogIsActive (MaxLogLevel (mLogLevel, _DEBUG_INDEX)))
#else
				if	(LogIsActive (mLogLevel))
#endif
				{
					LogMessage (mLogLevel, _T("    Sound [%2u] of [%6u] at [%8.8X] thru [%8.8X] chksum [%8.8X]"), lEntry, lIndex[1], lIndex[0], lIndex[0]+lIndex[1]-1, lIndex[2]);
				}
			}

			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel, _T("  [%s] Read [%u] SoundIndex   of [%u] at [%8.8X (%u)]"), _B(mPath), lCount, lByte-(LPCBYTE)lBlock, (LPCBYTE)lBlock, (LPCBYTE)lBlock);
			}
			lRet = true;
		}
		catch AnyExceptionDebug
	}

	return lRet;
}

#ifdef	_M_CEE
DWORD CAgentFileAcs::WriteSoundIndex (DWORD pFileOffset, CAgentFile^ pSource)
{
	DWORD	lBlockLength = 0;

	if	(
			(IsOpen)
		&&	(!IsReadOnly)
		)
	{
		array <BYTE>^	lBlockBuffer;
#if	FALSE		
		CAgentFileAcs^	lSourceAcs = safe_cast <CAgentFileAcs^> (pSource);
		array <BYTE>^	lSourceBlockBuffer;
		pin_ptr <BYTE>	lSourceBlockPinned;
		LPCDWORD		lSourceSoundIndex = NULL;

		if	(lSourceAcs)
		{
			try
			{
				ULARGE_INTEGER	lBlockDef;
				DWORD			lBlockOffset = 0;

				lSourceAcs->mFileStream->Seek (sizeof(DWORD)+(sizeof(lBlockDef)*3), SeekOrigin::Begin);
				lBlockDef.QuadPart = lSourceAcs->mFileReader->ReadUInt64 ();
				if	(lBlockDef.HighPart)
				{
					lSourceAcs->mFileStream->Seek (lBlockOffset=lBlockDef.LowPart, SeekOrigin::Begin);
					lSourceBlockBuffer = lSourceAcs->mFileReader->ReadBytes (lBlockLength=lBlockDef.HighPart);
					lSourceBlockPinned = &lSourceBlockBuffer[0];
					lSourceSoundIndex = (LPCDWORD)(LPBYTE)lSourceBlockPinned;
					lSourceSoundIndex++;
				}
			}
			catch AnyExceptionDebug
		}
#endif
		lBlockLength = sizeof(DWORD);
		if	(
				(mSoundIndex)
			&&	(mSoundIndex->Length > 0)
			)
		{
			lBlockLength += sizeof(DWORD) * mSoundIndex->Length * 3;
		}
		lBlockBuffer = gcnew array <BYTE> (lBlockLength);

		try
		{
			pin_ptr <BYTE>	lBlock = &lBlockBuffer[0];
			LPBYTE			lByte = lBlock;
			LPDWORD			lIndex;
			int				lSoundNdx;

			if	(
					(mSoundIndex)
				&&	(mSoundIndex->Length > 0)
				)
			{
				*(LPDWORD)lByte = mSoundIndex->Length;
				lByte += sizeof(DWORD);

				for (lSoundNdx = 0; lSoundNdx < mSoundIndex->Length; lSoundNdx++)
				{
					lIndex = (LPDWORD)lByte;
					lIndex[0] = mSoundIndex [lSoundNdx].Key;
					lIndex[1] = mSoundIndex [lSoundNdx].Value;
#if	FALSE					
					if	(lSourceSoundIndex)
					{
						lIndex[2] = lSourceSoundIndex[(lSoundNdx*3)+2]; // chksum
					}
					else
#endif					
					{
						lIndex[2] = 0; // chksum
					}
					lByte += sizeof(DWORD) * 3;

					if	(LogIsActive (mLogLevel))
					{
						LogMessage (mLogLevel, _T("    Sound [%2u] of [%6u] at [%8.8X] thru [%8.8X] chksum [%8.8X]"), lSoundNdx, lIndex[1], lIndex[0], lIndex[0]+lIndex[1]-1, lIndex[2]);
					}
				}
			}
			else
			{
				*(LPDWORD)lByte = 0;
			}

		}
		catch AnyExceptionDebug

		try
		{
			ULARGE_INTEGER	lBlockDef;

			lBlockDef.LowPart = pFileOffset;
			lBlockDef.HighPart = lBlockLength;
			mFileStream->Seek (sizeof(DWORD)+(sizeof(lBlockDef)*3), SeekOrigin::Begin);
			mFileWriter->Write (lBlockDef.QuadPart);
			mFileStream->Seek (pFileOffset, SeekOrigin::Begin);
			mFileWriter->Write (lBlockBuffer);

			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel, _T("  [%s] Write SoundIndex  of [%u] at [%8.8X (%u)]"), _B(mPath), lBlockLength, pFileOffset, pFileOffset);
			}
		}
		catch AnyExceptionDebug
	}
	return lBlockLength;
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
CAgentFileImage^ CAgentFileAcs::ReadAcsImage (DWORD pOffset, DWORD pSize, UINT pImageNum, bool p32Bit, System::Drawing::Color pBkColor)
#else
CAgentFileImage* CAgentFileAcs::ReadAcsImage (DWORD pOffset, DWORD pSize, UINT pImageNum, bool p32Bit, const COLORREF* pBkColor)
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

			if	(LogIsActive (mLogLevel))
			{
#ifdef	_M_CEE
				LogMessage (mLogLevel, _T("[%s]    Read Image [%4u] of [%6u] at [%8.8X] thru [%8.8X] Size [%d %d] @1 [%u] Compressed [%u] Bytes [%u]"), _B(mPath), pImageNum, pSize, lBlock, lBlock+pSize-1, lImageSize.Width, lImageSize.Height, lFirstByte, lImageCompressed, lByteCount);
#else
				LogMessage (mLogLevel, _T("[%s]    Read Image [%4u] of [%6u] at [%8.8X] thru [%8.8X] Size [%d %d] @1 [%u] Compressed [%u] Bytes [%u]"), mPath, pImageNum, pSize, lBlock, lBlock+pSize-1, lImageSize.cx, lImageSize.cy, lFirstByte, lImageCompressed, lByteCount);
#endif
			}

#ifdef	_DUMP_IMAGE
			if	(LogIsActive (MaxLogLevel (mLogLevel,_DUMP_IMAGE)))
			{
				LogDump (_DUMP_IMAGE, (LPBYTE)lBlock, lByteCount+10, _T("    "), true);
				if	(lByteCount < pSize)
				{
					LPCDWORD	lPart2 = (LPCDWORD)(lBlock+lByteCount+10);
					LogMessage (mLogLevel, _T("    Part 2 Compressed [%u] Decoded [%u]"), lPart2[0], lPart2[1]);
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
							LogMessage (mLogLevel, _T("    Part 2 Decoded [%u]"), lPart2[1]);
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
				&&	(lRet = new CAgentFileImage)
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

#ifdef	_DEBUG_NOT
				lByte += lByteCount;
				if	((lByte-(LPCBYTE)lBlock) < (INT_PTR)pSize)
				{
					LogMessage (LogDebugFast, _T("--- Leftover [%u] ---"), pSize - (lByte-(LPCBYTE)lBlock));
#ifdef	_M_CEE
					LogDumpWords (LogDebugFast, lByte, min (pSize - (lByte-(LPCBYTE)lBlock), 256));
					LogMessage (LogDebug, _T("------"));
#endif					
				}
#endif						
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
				&&	(l32BitImage = new CAgentFileImage)
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
DWORD CAgentFileAcs::WriteImages (DWORD pFileOffset, CAgentFile^ pSource)
{
	DWORD	lBlockLength = 0;

	if	(
			(IsOpen)
		&&	(!IsReadOnly)
		&&	(pSource->ImageCount > 0)
		)
	{
		array <BYTE>^	lBlockBuffer;
#if	FALSE		
		CAgentFileAcs^	lSourceAcs = safe_cast <CAgentFileAcs^> (pSource);

		if	(lSourceAcs)
		{
			DWORD									lBlockOffset = 0;
			List <KeyValuePair <UInt32, UInt32> >^	lImageIndex = gcnew List <KeyValuePair <UInt32, UInt32> >;

			for each (KeyValuePair <UInt32, UInt32> lEntry in lSourceAcs->mImageIndex)
			{
				if	(lBlockOffset == 0)
				{
					lBlockOffset = lEntry.Key;
				}
				else
				{
					lBlockOffset = min (lBlockOffset, lEntry.Key);
				}
				lBlockLength += lEntry.Value;
			}

			for each (KeyValuePair <UInt32, UInt32> lEntry in lSourceAcs->mImageIndex)
			{
				lImageIndex->Add (KeyValuePair <UInt32, UInt32> (lEntry.Key + pFileOffset - lBlockOffset, lEntry.Value));
			}
			mImageIndex = lImageIndex->ToArray();

			lSourceAcs->mFileStream->Seek (lBlockOffset, SeekOrigin::Begin);
			lBlockBuffer = lSourceAcs->mFileReader->ReadBytes (lBlockLength);
		}
		else
#endif		
		{
			int										lImageNdx;
			List <KeyValuePair <UInt32, UInt32> >^	lImageIndex = gcnew List <KeyValuePair <UInt32, UInt32> >;
			CAgentFileImage^						lImage;

			for (lImageNdx = 0; lImageNdx < pSource->ImageCount; lImageNdx++)
			{
				if	(lImage = pSource->GetImage (lImageNdx))
				{
					lBlockLength += 10 + (sizeof(DWORD)*2);
					lBlockLength += lImage->BitsSize;
				}
				else
				{
					lBlockLength += sizeof(DWORD);
				}
			}
			lBlockBuffer = gcnew array <BYTE> (lBlockLength);

			try
			{
				pin_ptr <BYTE>	lBlock = &lBlockBuffer[0];
				LPBYTE			lByte = lBlock;
				LPBYTE			lImageStart;

				for (lImageNdx = 0; lImageNdx < pSource->ImageCount; lImageNdx++)
				{
					if	(lImage = pSource->GetImage (lImageNdx))
					{
						lImageStart = lByte;
						lByte = (LPBYTE)WriteAcsImage (lImageStart, lImage);

						*(LPDWORD)lByte = 0; // Delimiter - always 0
						lByte += sizeof(DWORD);
						*(LPDWORD)lByte = 0; // Size of image region
						lByte += sizeof(DWORD);

						lImageIndex->Add (KeyValuePair <UInt32, UInt32> ((lImageStart-lBlock) + pFileOffset, lByte-lImageStart));
					}
					else
					{
						*(LPDWORD)lByte = 0;
						lByte += sizeof(DWORD);
					}
				}

				mImageIndex = lImageIndex->ToArray();

				if	(LogIsActive (mLogLevel))
				{
					LogMessage (mLogLevel, _T("  [%s] Write Images      of [%u] at [%8.8X (%u)]"), _B(mPath), lByte-(LPBYTE)lBlock, pFileOffset, pFileOffset);
				}
			}
			catch AnyExceptionDebug
		}

		try
		{
			mFileStream->Seek (pFileOffset, SeekOrigin::Begin);
			mFileWriter->Write (lBlockBuffer);

			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel, _T("  [%s] Write Images      of [%u] at [%8.8X (%u)]"), _B(mPath), lBlockLength, pFileOffset, pFileOffset);
			}
		}
		catch AnyExceptionDebug
	}
	return lBlockLength;
}

LPVOID CAgentFileAcs::WriteAcsImage (LPVOID pBuffer, CAgentFileImage^ pImage)
{
	LPBYTE	lByte = (LPBYTE)pBuffer;

	try
	{
		if	(pImage)
		{
			*lByte = TRUE;
		}
		else
		{
			*lByte = FALSE;
		}
		lByte++;
		if	(pImage)
		{
			*(LPWORD)lByte = (WORD)pImage->ImageSize.Width;
		}
		else
		{
			*(LPWORD)lByte = (WORD)mHeader->ImageSize.Width;
		}
		lByte += sizeof(WORD);
		if	(pImage)
		{
			*(LPWORD)lByte = (WORD)pImage->ImageSize.Height;
		}
		else
		{
			*(LPWORD)lByte = (WORD)mHeader->ImageSize.Height;
		}
		lByte += sizeof(WORD);
		*lByte = FALSE; // Uncompressed
		lByte++;
		if	(
				(pImage)
			&&	(pImage->mBits)
			)
		{
			*(LPDWORD)lByte = pImage->mBits->Length;
		}
		else
		{
			*(LPDWORD)lByte = 0;
		}
		lByte += sizeof(DWORD);

		if	(
				(pImage)
			&&	(pImage->mBits)
			&&	(pImage->mBits->Length > 0)
			)
		{
			pin_ptr <BYTE>	lImageBits = &pImage->mBits[0];

			memcpy (lByte, lImageBits, pImage->mBits->Length);
			lByte += pImage->mBits->Length;
		}

		if	(LogIsActive (mLogLevel))
		{
			if	(pImage)
			{
				LogMessage (mLogLevel, _T("    Write Image [%4u] of [%6u] at [%8.8X] thru [%8.8X] Size [%d %d] Bytes [%u]"), pImage->ImageNum, lByte-(LPBYTE)pBuffer, pBuffer, lByte-1, pImage->ImageSize.Width, pImage->ImageSize.Height, pImage->BitsSize);
			}
			else
			{
				LogMessage (mLogLevel, _T("    NO Image of [%6u] at [%8.8X] thru [%8.8X]"), lByte-(LPBYTE)pBuffer, pBuffer, lByte-1);
			}
		}
	}
	catch AnyExceptionDebug

	return lByte;
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
array <BYTE>^ CAgentFileAcs::ReadAcsSound (DWORD pOffset, DWORD pSize, UINT pSoundNum)
#else
LPCVOID CAgentFileAcs::ReadAcsSound (DWORD pOffset, DWORD pSize, UINT pSoundNum)
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

			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel, _T("    Sound [%4u] of [%5u] at [%8.8X] thru [%8.8X]"), pSoundNum, pSize, pOffset, pOffset+pSize-1);
			}
			lRet = lBlockBuffer;

#ifdef	_DEBUG_NOT
			{
				pin_ptr <BYTE> lPinned = &lRet[0];			
				LogMessage (LogDebug, _T("--- Sound [%u] of [%u] ---"), pSoundNum, pSize);
				LogDump (LogDebugFast, (LPBYTE)lPinned, min (pSize, 256));
				LogMessage (LogDebug, _T("------"));
			}
#endif			
		}
#else
		LPCBYTE	lBlock;

		if	(
				(IsOpen)
			&&	(lBlock = ((LPCBYTE)(LPVOID)mFileView)+pOffset)
			)
		{
			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel, _T("    Sound [%4u] of [%5u] at [%8.8X] thru [%8.8X]"), pSoundNum, pSize, lBlock, lBlock+pSize-1);
			}
			lRet = (LPCVOID) lBlock;
		}
#endif
	}
	catch AnyExceptionDebug

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
DWORD CAgentFileAcs::WriteSounds (DWORD pFileOffset, CAgentFile^ pSource)
{
	DWORD	lBlockLength = 0;

	if	(
			(IsOpen)
		&&	(!IsReadOnly)
		&&	(pSource->SoundCount > 0)
		)
	{
		array <BYTE>^	lBlockBuffer;
#if	FALSE		
		CAgentFileAcs^	lSourceAcs = safe_cast <CAgentFileAcs^> (pSource);

		if	(lSourceAcs)
		{
			DWORD									lBlockOffset = 0;
			List <KeyValuePair <UInt32, UInt32> >^	lSoundIndex = gcnew List <KeyValuePair <UInt32, UInt32> >;

			for each (KeyValuePair <UInt32, UInt32> lEntry in lSourceAcs->mSoundIndex)
			{
				if	(lBlockOffset == 0)
				{
					lBlockOffset = lEntry.Key;
				}
				else
				{
					lBlockOffset = min (lBlockOffset, lEntry.Key);
				}
				lBlockLength += lEntry.Value;
			}

			for each (KeyValuePair <UInt32, UInt32> lEntry in lSourceAcs->mSoundIndex)
			{
				lSoundIndex->Add (KeyValuePair <UInt32, UInt32> (lEntry.Key + pFileOffset - lBlockOffset, lEntry.Value));
			}
			mSoundIndex = lSoundIndex->ToArray();

			lSourceAcs->mFileStream->Seek (lBlockOffset, SeekOrigin::Begin);
			lBlockBuffer = lSourceAcs->mFileReader->ReadBytes (lBlockLength);
		}
		else
#endif		
		{
			int										lSoundNdx;
			List <KeyValuePair <UInt32, UInt32> >^	lSoundIndex = gcnew List <KeyValuePair <UInt32, UInt32> >;

			for (lSoundNdx = 0; lSoundNdx < pSource->SoundCount; lSoundNdx++)
			{
				lBlockLength += pSource->GetSoundSize (lSoundNdx);
			}
			lBlockBuffer = gcnew array <BYTE> (lBlockLength);

			try
			{
				pin_ptr <BYTE>	lBlock = &lBlockBuffer[0];
				LPBYTE			lByte = lBlock;
				LPBYTE			lSoundStart;
				array <BYTE>^	lSound;

				for (lSoundNdx = 0; lSoundNdx < pSource->SoundCount; lSoundNdx++)
				{
					if	(lSound = pSource->GetSound (lSoundNdx))
					{
#ifdef	_DEBUG_NOT
						{
							pin_ptr <BYTE> lPinned = &lSound[0];			
							LogMessage (LogDebug, _T("--- Sound [%u] of [%u] ---"), lSoundNdx, lSound->Length);
							LogDump (LogDebugFast, (LPBYTE)lPinned, min (lSound->Length, 256));
							LogMessage (LogDebug, _T("------"));
						}
#endif			
						lSoundStart = lByte;
						lByte = (LPBYTE)WriteAcsSound (lSoundStart, lSound);
						lSoundIndex->Add (KeyValuePair <UInt32, UInt32> ((lSoundStart-lBlock) + pFileOffset, lByte-lSoundStart));
					}
				}

				if	(LogIsActive (mLogLevel))
				{
					LogMessage (mLogLevel, _T("  [%s] Write Sounds      of [%u] at [%8.8X (%u)]"), _B(mPath), lByte-(LPBYTE)lBlock, pFileOffset, pFileOffset);
				}

				mSoundIndex = lSoundIndex->ToArray();
			}
			catch AnyExceptionDebug
		}

		try
		{
			mFileStream->Seek (pFileOffset, SeekOrigin::Begin);
			mFileWriter->Write (lBlockBuffer);

			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel, _T("  [%s] Write Sounds      of [%u] at [%8.8X (%u)]"), _B(mPath), lBlockLength, pFileOffset, pFileOffset);
			}
		}
		catch AnyExceptionDebug
	}
	return lBlockLength;
}

LPVOID CAgentFileAcs::WriteAcsSound (LPVOID pBuffer, array <BYTE>^ pSound)
{
	LPBYTE	lByte = (LPBYTE)pBuffer;

	try
	{
		if	(
				(pSound)
			&&	(pSound->Length)
			)
		{
			if	(pBuffer)
			{
				pin_ptr <BYTE>	lSound = &pSound[0];
				memcpy (lByte, (LPBYTE)lSound, pSound->Length);
			}
			lByte += pSound->Length;
		}
		if	(LogIsActive (mLogLevel))
		{
			if	(pSound)
			{
				LogMessage (mLogLevel, _T("    Write Sound of [%5u] at [%8.8X] thru [%8.8X]"), pSound->Length, (LPBYTE)pBuffer, lByte);
			}
			else
			{
				LogMessage (mLogLevel, _T("    NO Sound at [%8.8X] thru [%8.8X]"), (LPBYTE)pBuffer, lByte);
			}
		}
	}
	catch AnyExceptionDebug

	return lByte;
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifdef	_M_CEE
/////////////////////////////////////////////////////////////////////////////

void CAgentFileAcs::FreeUnusedImages ()
{
#if	FALSE
	if	(
			(mImageIndex)
		&&	(mImageIndex->Length > 0)
		)
	{
		array <int>^							lImageUsage = gcnew array <int> (mImageIndex->Length);
		int										lImageNdx;
		List <KeyValuePair <UInt32, UInt32> >^	lImageList = gcnew List <KeyValuePair <UInt32, UInt32> >;
		int										lUnusedCount = 0;
		
		for (lImageNdx = 0; lImageNdx < lImageUsage->Length; lImageNdx++)
		{
			lImageUsage[lImageNdx] = 0;
		}

		for each (CAgentFileAnimation^ lAnimation in Gestures)
		{
			if	(lAnimation->Frames)
			{
				for each (CAgentFileFrame^ lFrame in lAnimation->Frames)
				{
					if	(lFrame->Images)
					{
						for each (CAgentFileFrameImage^ lImage in lFrame->Images)
						{
							if	(
									(lImage->mImageNdx >= 0)
								&&	(lImage->mImageNdx < lImageUsage->Length)
								)
							{
								lImageUsage[lImage->mImageNdx]++;
							}
						}
					}
					if	(lFrame->Overlays)
					{
						for each (CAgentFileFrameOverlay^ lOverlay in lFrame->Overlays)
						{
							if	(
									(lOverlay->mImageNdx >= 0)
								&&	(lOverlay->mImageNdx < lImageUsage->Length)
								)
							{
								lImageUsage[lOverlay->mImageNdx]++;
							}
						}
					}
				}
			}
		}

		for (lImageNdx = 0; lImageNdx < mImageIndex->Length; lImageNdx++)
		{
			if	(lImageUsage[lImageNdx] > 0)
			{
				lImageList->Add (mImageIndex[lImageNdx]);
				lImageUsage[lImageNdx] = lUnusedCount;
			}
			else
			{
				lUnusedCount++;
				if	(LogIsActive (mLogLevel))
				{
					LogMessage (mLogLevel, _T("  [%s] Image [%d] unused"), _B(mPath), lImageNdx); 
				}
			}
		}
		
		if	(lUnusedCount > 0)
		{
			mImageIndex = lImageList->ToArray();

			for each (CAgentFileAnimation^ lAnimation in Gestures)
			{
				if	(lAnimation->Frames)
				{
					for each (CAgentFileFrame^ lFrame in lAnimation->Frames)
					{
						if	(lFrame->Images)
						{
							for each (CAgentFileFrameImage^ lImage in lFrame->Images)
							{
								if	(
										(lImage->ImageNdx >= 0)
									&&	(lImage->ImageNdx < lImageUsage->Length)
									)
								{
									lImage->mImageNdx -= lImageUsage[lImage->mImageNdx];
								}
							}
						}
						if	(lFrame->Overlays)
						{
							for each (CAgentFileFrameOverlay^ lOverlay in lFrame->Overlays)
							{
								if	(
										(lOverlay->mImageNdx >= 0)
									&&	(lOverlay->mImageNdx < lImageUsage->Length)
									)
								{
									lOverlay->mImageNdx -= lImageUsage[lOverlay->mImageNdx];
								}
							}
						}
					}
				}
			}
		}
	}
#endif	
}

void CAgentFileAcs::FreeUnusedSounds ()	
{
#if	FALSE
	if	(
			(mSoundIndex)
		&&	(mSoundIndex->Length > 0)
		)
	{
		array <int>^							lSoundUsage = gcnew array <int> (mSoundIndex->Length);
		int										lSoundNdx;
		List <KeyValuePair <UInt32, UInt32> >^	lSoundList = gcnew List <KeyValuePair <UInt32, UInt32> >;
		int										lUnusedCount = 0;
		
		for (lSoundNdx = 0; lSoundNdx < lSoundUsage->Length; lSoundNdx++)
		{
			lSoundUsage[lSoundNdx] = 0;
		}

		for each (CAgentFileAnimation^ lAnimation in Gestures)
		{
			if	(lAnimation->Frames)
			{
				for each (CAgentFileFrame^ lFrame in lAnimation->Frames)
				{
					if	(
							(lFrame->mSoundNdx >= 0)
						&&	(lFrame->mSoundNdx < lSoundUsage->Length)
						)
					{
						lSoundUsage[lFrame->mSoundNdx]++;
					}
				}
			}
		}

		for (lSoundNdx = 0; lSoundNdx < mSoundIndex->Length; lSoundNdx++)
		{
			if	(lSoundUsage[lSoundNdx] > 0)
			{
				lSoundList->Add (mSoundIndex[lSoundNdx]);
				lSoundUsage[lSoundNdx] = lUnusedCount;
			}
			else
			{
				lUnusedCount++;
				if	(LogIsActive (mLogLevel))
				{
					LogMessage (mLogLevel, _T("  [%s] Sound [%d] unused"), _B(mPath), lSoundNdx); 
				}
			}
		}
		
		if	(lUnusedCount > 0)
		{
			mSoundIndex = lSoundList->ToArray();

			for each (CAgentFileAnimation^ lAnimation in Gestures)
			{
				if	(lAnimation->Frames)
				{
					for each (CAgentFileFrame^ lFrame in lAnimation->Frames)
					{
						if	(
								(lFrame->mSoundNdx >= 0)
							&&	(lFrame->mSoundNdx < lSoundUsage->Length)
							)
						{
							lFrame->mSoundNdx -= lSoundUsage[lFrame->mSoundNdx];
						}
					}
				}
			}
		}
	}
#endif	
}

/////////////////////////////////////////////////////////////////////////////
#endif	// _M_CEE
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
				if	(lImage = ReadAcsImage (mImageIndex [lImageNdx].LowPart, mImageIndex [lImageNdx].HighPart, (UINT)lImageNdx, false, NULL))
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

			if	(lImage = ReadAcsImage (mImageIndex [pImageNdx].LowPart, mImageIndex [pImageNdx].HighPart, (UINT)pImageNdx, false, NULL))
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
