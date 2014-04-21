/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
#include "AgentFileV15.h"
#include "AgentFileBinary.h"
#ifdef	__cplusplus_cli
#include "AgtErr.h"
#else
#include <shlwapi.h>
#include "DaCore.h"
#include "GuidStr.h"
#include "StringArrayEx.h"
#endif
#include "AgentFileDefs.inl"

#ifndef	__cplusplus_cli
IMPLEMENT_DLL_OBJECT(CAgentFileV15)
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
#pragma managed(push,off)
/////////////////////////////////////////////////////////////////////////////

class CAgentFileV15Reader
{
public:
	CAgentFileV15Reader ();
	~CAgentFileV15Reader ();
	
	HRESULT Open (LPCTSTR pPath, UINT pLogLevel = LogVerbose+1);
	void Close ();
	
	HRESULT ReadHeaderStream (tArrayPtr <BYTE> & pHeader, DWORD & pHeaderSize, UINT pLogLevel = LogVerbose+1);
	HRESULT ReadAnimationStream (LPCWSTR pAnimationName, tArrayPtr <BYTE> & pAnimation, DWORD & pAnimationSize, UINT pLogLevel = LogVerbose+1);
	
protected:
	IStoragePtr	mStorage;	
};

/////////////////////////////////////////////////////////////////////////////

CAgentFileV15Reader::CAgentFileV15Reader ()
{
}
CAgentFileV15Reader::~CAgentFileV15Reader ()
{
	Close ();
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentFileV15Reader::Open (LPCWSTR pPath, UINT pLogLevel)
{
	return LogComErrAnon (pLogLevel, StgOpenStorageEx (pPath, STGM_TRANSACTED|STGM_READ|STGM_SHARE_DENY_NONE, STGFMT_DOCFILE, 0, NULL, 0, __uuidof(IStorage), (void**)&mStorage), _T("StgOpenStorageEx [%s]"), pPath);
}

void CAgentFileV15Reader::Close ()
{
	try
	{
		mStorage = NULL;
	}
	catch (...)
	{}
	mStorage.Detach ();
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentFileV15Reader::ReadHeaderStream (tArrayPtr <BYTE> & pHeader, DWORD & pHeaderSize, UINT pLogLevel)
{
	HRESULT			lResult = E_UNEXPECTED;
	IStreamPtr		lStream;
	tS <STATSTG>	lStatStg;
	
	pHeader = NULL;
	pHeaderSize = 0;
	
	if	(mStorage != NULL)
	{
		if	(
				(SUCCEEDED (lResult = LogComErrAnon (pLogLevel, mStorage->OpenStream (L"char.acf", NULL, STGM_DIRECT|STGM_SHARE_EXCLUSIVE|STGM_READ, NULL, &lStream), _T("Open char.acf"))))
			&&	(SUCCEEDED (lResult = LogComErr (pLogLevel, lStream->Stat (&lStatStg, STATFLAG_NONAME))))
			)
		{
			pHeaderSize = lStatStg.cbSize.LowPart;
			
			if	(pHeader = new BYTE [pHeaderSize])
			{
				if	(
						(SUCCEEDED (lResult = LogComErr (pLogLevel, lStream->Read ((LPBYTE)pHeader, pHeaderSize, &lStatStg.cbSize.LowPart))))
					&&	(lStatStg.cbSize.LowPart != pHeaderSize)
					)
				{
					lResult = E_FAIL;
				}
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
		}				
	}
	return lResult;
}

HRESULT CAgentFileV15Reader::ReadAnimationStream (LPCWSTR pAnimationName, tArrayPtr <BYTE> & pAnimation, DWORD & pAnimationSize, UINT pLogLevel)
{
	HRESULT			lResult = E_UNEXPECTED;
	IStreamPtr		lStream;
	tS <STATSTG>	lStatStg;
	
	pAnimation = NULL;
	pAnimationSize = 0;
	
	if	(mStorage != NULL)
	{
		if	(
				(SUCCEEDED (lResult = LogComErrAnon (pLogLevel, mStorage->OpenStream (pAnimationName, NULL, STGM_DIRECT|STGM_SHARE_EXCLUSIVE|STGM_READ, NULL, &lStream), _T("Open char.acf"))))
			&&	(SUCCEEDED (lResult = LogComErr (pLogLevel, lStream->Stat (&lStatStg, STATFLAG_NONAME))))
			)
		{
			pAnimationSize = lStatStg.cbSize.LowPart;
			
			if	(pAnimation = new BYTE [pAnimationSize])
			{
				if	(
						(SUCCEEDED (lResult = LogComErr (pLogLevel, lStream->Read ((LPBYTE)pAnimation, pAnimationSize, &lStatStg.cbSize.LowPart))))
					&&	(lStatStg.cbSize.LowPart != pAnimationSize)
					)
				{
					lResult = E_FAIL;
				}
			}
			else
			{
				lResult = E_OUTOFMEMORY;
			}
		}				
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma managed(pop)
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
namespace DoubleAgent {
namespace Character {
#endif
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
[Serializable] private ref class V15FileAnimation : public FileAnimation
{
public:	
	[NonSerialized]	System::String^	mFileName;
	[NonSerialized]	UInt32			mFileCheckSum;

internal:
	V15FileAnimation (CharacterFile^ pOwner, FileGestures^ pContainer);
public:	
	virtual ~V15FileAnimation ();
internal:
	virtual void Empty () override;

public:	
	virtual property UInt16 FrameCount {virtual UInt16 get() override;}
	virtual property FileFrames^ Frames {virtual FileFrames^ get() override;}
	virtual Boolean CopyTo (FileAnimation^ pTarget, Boolean pDeepCopy) override;
    virtual System::String^ ToString() override;

internal:
	property Boolean IsLoaded {Boolean get(); void set (Boolean pValue);}
	void EnsureIsLoaded ();
};
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAgentFileV15::CAgentFileV15()
:	mFileReader (NULL)
{
	Close ();
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CAgentFileV15::CAgentFileV15"), this);
#endif
}

CAgentFileV15::~CAgentFileV15 ()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CAgentFileV15::~CAgentFileV15"), this);
#endif
	Close ();
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
CAgentFileV15^ CAgentFileV15::CreateInstance ()
#else
CAgentFileV15* CAgentFileV15::CreateInstance ()
#endif
{
#ifdef	__cplusplus_cli
	LogStart (false);
	return gcnew CAgentFileV15;
#else
	return new CAgentFileV15;
#endif
}

#ifdef	__cplusplus_cli
bool CAgentFileV15::CheckFileSignature (const System::String^ pPath)
#else
bool CAgentFileV15::CheckFileSignature (LPCTSTR pPath)
#endif	
{
	bool	lRet = false;
	
	try
	{
#ifdef	__cplusplus_cli
		String^						lPath = ParseFilePath (pPath);
		System::IO::FileStream^		lFileStream;
		System::IO::BinaryReader^	lFileReader;
#else
		CAtlString					lPath = ParseFilePath (pPath);
		CFileHandle					lFileHandle;
		CGenericHandle				lFileMapping;
		CMappedHandle				lFileView;
#endif

#ifdef	__cplusplus_cli
		try
		{
			lFileStream = gcnew FileStream (lPath, FileMode::Open, FileAccess::Read);
		}
		catch AnyExceptionSilent
		
		if	(
				(lFileStream)
			&&	(lFileStream->Length >= sizeof(DWORD)*2)
			&&	(lFileReader = gcnew BinaryReader (lFileStream))
			)
#else
		lFileHandle = CreateFile (lPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

		if	(
				(lFileHandle.SafeIsValid ())
			&&	(GetFileSize (lFileHandle, NULL) >= sizeof(DWORD)*2)
			&&	((lFileMapping = CreateFileMapping (lFileHandle, NULL, PAGE_READONLY, 0, 0, NULL)).SafeIsValid())
			&&	((lFileView = MapViewOfFile (lFileMapping, FILE_MAP_READ, 0, 0, sizeof(DWORD)*2)).SafeIsValid())
			)
#endif
		{
			DWORD	lSignature_0 = 0;
			DWORD	lSignature_1 = 0;
			
#ifdef	__cplusplus_cli
			lSignature_0 = lFileReader->ReadUInt32 ();
			lSignature_1 = lFileReader->ReadUInt32 ();
#else
			lSignature_0 = ((const DWORD *)(LPVOID)lFileView)[0];
			lSignature_1 = ((const DWORD *)(LPVOID)lFileView)[1];
#endif
			if	(
					(lSignature_0 == sAcsFileSignatureV15_0)
				&&	(lSignature_1 == sAcsFileSignatureV15_1)
				)
			{
				lRet = true;
			}
		}
		
#ifdef	__cplusplus_cli
		if	(lFileReader)
		{
			try
			{
				lFileReader->Close ();
			}
			catch AnyExceptionDebug
			try
			{
				lFileReader->~BinaryReader ();
			}
			catch AnyExceptionDebug
		}
		if	(lFileStream)
		{
			try
			{
				lFileStream->Close ();
			}
			catch AnyExceptionDebug
			try
			{
				lFileStream->~FileStream ();
			}
			catch AnyExceptionDebug
		}
#else
		lFileView.Close ();
		lFileMapping.Close ();
		lFileHandle.Close ();
#endif
	}
	catch AnyExceptionDebug
	
	return lRet;	
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
bool CAgentFileV15::IsOpen::get ()
{
	return (mFileReader != NULL);
}
#else
bool CAgentFileV15::get_IsOpen () const
{
	return (mFileReader != NULL);
}
#endif

#ifdef	__cplusplus_cli
bool CAgentFileV15::IsAcsFile::get ()
{
	if	(IsOpen)
	{
		return (mSignature == sAcsFileSignatureV15);
	}
	return true;
}
#else
bool CAgentFileV15::get_IsAcsFile () const
{
	if	(IsOpen)
	{
		return (mSignature == sAcsFileSignatureV15);
	}
	return true;
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
bool CAgentFileV15::Open (const System::String^ pPath)
{
	bool	lRet = false;
	String^	lPath = ParseFilePath (pPath);

	Close ();

#ifdef	_DEBUG_LOAD
	UINT	lLogLevel = mLogLevel;
	mLogLevel = MinLogLevel (mLogLevel, _DEBUG_LOAD);
#endif
/**/mLogLevel = LogDebugFast;
	if	(LogIsActive (mLogLevel))
	{
		LogMessage (mLogLevel, _T("Open [%s]"), _B(lPath));
	}

	mPath = lPath;

	try
	{
		array<wchar_t>^		lPathChars = lPath->ToCharArray();
		pin_ptr<wchar_t>	lPathPinned = &lPathChars[0];

		mFileReader = new CAgentFileV15Reader ();
		if	(SUCCEEDED (mFileReader->Open (lPathPinned, MinLogLevel(mLogLevel,LogIfActive))))
		{
			lRet = ReadHeaderStream ();
		}
	}
	catch AnyExceptionDebug

	if	(!lRet)
	{
		mPath = nullptr;
		Close ();
	}

#ifdef	_DEBUG_LOAD
	mLogLevel = lLogLevel;
#endif
	return lRet;
}
#else
HRESULT CAgentFileV15::Open (LPCTSTR pPath)
{
	HRESULT		lResult = S_OK;
	CAtlString	lPath = ParseFilePath (pPath);

	Close ();

#ifdef	_DEBUG_LOAD
	UINT lLogLevel = mLogLevel;
	mLogLevel = MinLogLevel (mLogLevel, _DEBUG_LOAD);
#endif

	mPath = lPath;
	mFileReader = new CAgentFileV15Reader ();
	if	(SUCCEEDED (mFileReader->Open (mPath, MinLogLevel(mLogLevel,LogIfActive))))
	{
		lResult = ReadHeaderStream ();
	}
	if	(FAILED (lResult))
	{
		mPath.Empty();
		Close ();
	}

#ifdef	_DEBUG_LOAD
	mLogLevel = lLogLevel;
#endif
	return lResult;
}
#endif

#ifdef	__cplusplus_cli
bool CAgentFileV15::Save (const System::String^ pPath, CAgentFile^ pSource)
{
	throw gcnew NotSupportedException ("Saving ACS Version 1.5 files is not supported.");
}
#endif

void CAgentFileV15::Close ()
{
#ifdef	__cplusplus_cli
	mStreamImages = gcnew System::Collections::Generic::List <CAgentFileImage^> ();
	mStreamSounds = gcnew System::Collections::Generic::List <array <BYTE>^> ();
#else
	mStreamImages.DeleteAll ();
	mStreamSounds.DeleteAll ();
#endif	

#ifdef	__cplusplus_cli
	if	(mFileReader)
	{
		delete mFileReader;
	}
#endif
	mFileReader = NULL;
	__super::Close ();
	mSignature = sAcsFileSignatureV15;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
bool CAgentFileV15::ReadHeaderStream ()
#else
HRESULT CAgentFileV15::ReadHeaderStream ()
#endif
{
#ifdef	__cplusplus_cli
	bool				lRet = false;
#endif	
	HRESULT				lResult;
	tArrayPtr <BYTE>	lHeader;
	DWORD				lHeaderSize;

	if	(SUCCEEDED (lResult = mFileReader->ReadHeaderStream (lHeader, lHeaderSize, MinLogLevel(mLogLevel,LogIfActive))))
	{
		LPCBYTE		lByte = (LPCBYTE)lHeader;
		LPCDWORD	lDword = (LPCDWORD)lByte;
		
		mSignature = *lDword;

		if	(mSignature == sAcsFileSignatureV15)
		{
			lResult = S_FALSE;
#ifdef	_DEBUG_LOAD
			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel, _T("  [%s] Signature [%8.8X]"), _B(mPath), mSignature);
			}
#endif
			try
			{
				DWORD				lEncodedSize = lDword[2];
				LPCBYTE				lEncoded = lByte + sizeof(DWORD)*3;
				DWORD				lDecodedSize = lDword[1];
				tArrayPtr <BYTE>	lDecoded = new BYTE [lDecodedSize];
				
				if	(CAgentFileBinary::DecodeData (lEncoded, lEncodedSize, lDecoded, lDecodedSize) == lDecodedSize)
				{
					lByte = (LPCBYTE)lDecoded;
					lDword = (LPCDWORD)lByte;
					mVersionMajor = HIWORD(*lDword);
					mVersionMinor = LOWORD(*lDword);
					lByte += sizeof(DWORD);
#ifdef	_DEBUG_LOAD
					if	(LogIsActive (mLogLevel))
					{
						LogMessage (mLogLevel, _T("  [%s] Version [%hu %hu]"), _B(mPath), mVersionMajor, mVersionMinor);
					}
#endif
					if	(
							(lByte = (LPCBYTE)ReadBufferAnimations (lByte, lDecodedSize-(DWORD)(lByte-(LPCBYTE)lDecoded)))
						&&	(lByte = (LPCBYTE)ReadBufferIdentity (lByte, lDecodedSize-(DWORD)(lByte-(LPCBYTE)lDecoded)))
						&&	(lByte = (LPCBYTE)ReadBufferHeader (lByte, lDecodedSize-(DWORD)(lByte-(LPCBYTE)lDecoded)))
						&&	(lByte = (LPCBYTE)ReadBufferPalette (lByte, lDecodedSize-(DWORD)(lByte-(LPCBYTE)lDecoded)))
						&&	(lByte = (LPCBYTE)ReadBufferStates (lByte, lDecodedSize-(DWORD)(lByte-(LPCBYTE)lDecoded)))
						)
					{
#ifdef	__cplusplus_cli
						lRet = true;
#else
						lResult = S_OK;
#endif
					}
				}
			}
			catch AnyExceptionDebug
		}
		else
		{
#ifdef	__cplusplus_cli
			throw gcnew Exception (String::Format ("{0} is not a Microsoft Agent 2.x character file.", mPath));
#else
#ifdef	_LOG_LOAD_ERRS
			if	(LogIsActive ())
			{
				LogMessage (_LOG_LOAD_ERRS, _T("  [%s] Invalid Signature [%8.8X] != [%8.8X]"), mPath, mSignature, sAcsFileSignatureV15);
			}
#endif
			lResult = AGENTPROVERROR_MAGIC;
#endif
		}
	}
	
#ifdef	__cplusplus_cli
	return lRet;
#else
	return lResult;
#endif	
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
bool CAgentFileV15::ReadAnimationStream (V15FileAnimation^ pAnimation)
#else
HRESULT CAgentFileV15::ReadAnimationStream (CAgentFileAnimation* pAnimation)
#endif
{
#ifdef	__cplusplus_cli
	bool	lRet = false;
#endif	
	HRESULT	lResult = S_FALSE;
	
	if	(pAnimation)
	{
		if	(
#ifdef	__cplusplus_cli
				(!pAnimation->IsLoaded)
			&&	(!String::IsNullOrEmpty (pAnimation->mFileName))
#else			
				(pAnimation->mFileCheckSum != (DWORD)-1)
			&&	(pAnimation->mFileName.Ptr ())
#endif
			)			
		{
			try
			{
#ifdef	__cplusplus_cli
				array<wchar_t>^		lStreamChars = pAnimation->mFileName->ToCharArray();
				pin_ptr<wchar_t>	lStreamName = &lStreamChars[0];
#else
				LPCWSTR				lStreamName = pAnimation->mFileName;			
#endif		
				tArrayPtr <BYTE>	lStreamData;
				DWORD				lStreamSize;

				if	(SUCCEEDED (lResult = mFileReader->ReadAnimationStream (lStreamName, lStreamData, lStreamSize, MinLogLevel(mLogLevel,LogIfActive))))
				{
					LPCBYTE				lByte = (LPCBYTE)lStreamData + sizeof(DWORD)*2+1;
					LPCDWORD			lDword = (LPCDWORD)lByte;
					DWORD				lEncodedSize = lDword[1];
					LPCBYTE				lEncoded = lByte + sizeof(DWORD)*2;;
					DWORD				lDecodedSize = lDword[0];
					tArrayPtr <BYTE>	lDecoded = new BYTE [lDecodedSize];

					lResult = S_FALSE;

					if	(
							(((LPCDWORD)(LPCBYTE)lStreamData)[0] == MAKELONG (mVersionMinor, mVersionMajor))
						&&	(((LPCDWORD)(LPCBYTE)lStreamData)[1] == pAnimation->mFileCheckSum)
						&&	(CAgentFileBinary::DecodeData (lEncoded, lEncodedSize, lDecoded, lDecodedSize) == lDecodedSize)
						)
					{
#ifdef	__cplusplus_cli
						Int32	lFirstSound = (Int32)mStreamSounds->Count;
						Int32	lFirstImage = (Int32)mStreamImages->Count;
#else
						long	lFirstSound = (long)mStreamSounds.GetCount();
						long	lFirstImage = (long)mStreamImages.GetCount();
#endif					
#ifdef	__cplusplus_cli
						pAnimation->IsLoaded = true;
#endif						
						lByte = (LPCBYTE)lDecoded;

						if	(
								(lByte = (LPCBYTE)ReadBufferSounds (pAnimation, lByte, lDecodedSize-(DWORD)(lByte-(LPCBYTE)lDecoded)))
							&&	(lByte = (LPCBYTE)ReadBufferImages (pAnimation, lByte, lDecodedSize-(DWORD)(lByte-(LPCBYTE)lDecoded)))
							&&	(lByte = (LPCBYTE)ReadBufferFrames (pAnimation, lByte, lDecodedSize-(DWORD)(lByte-(LPCBYTE)lDecoded), lFirstImage, lFirstSound))
							)
						{
#ifdef	__cplusplus_cli
							lRet = true;
#else
							lResult = S_OK;
#endif
						}
					}
					else
					if	(LogIsActive ())
					{
						LogMessage (LogIfActive, _T("[%s] [%s] [%s] Decode failed"), _B(mPath), _B(pAnimation->mName), _B(pAnimation->mFileName));
					}
				}
			}
			catch AnyExceptionDebug
		}

#ifdef	__cplusplus_cli
		pAnimation->IsLoaded = true;
#else
		pAnimation->mFileCheckSum = (DWORD)-1;
#endif		
	}
#ifdef	__cplusplus_cli
	return lRet;
#else
	return lResult;
#endif	
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
V15FileAnimation^ CAgentFileV15::GetV15Animation (int pAnimationNdx)
{
	V15FileAnimation^	lAnimation;

	if	(
			(IsOpen)
		&&	(pAnimationNdx >= 0)
		&&	(pAnimationNdx < mGestures->Count)
		&&	(V15FileAnimation::typeid->IsAssignableFrom (mGestures [pAnimationNdx]->GetType()))
		)
	{
		lAnimation = safe_cast<V15FileAnimation^> (mGestures [pAnimationNdx]);
	}
	return lAnimation;
}
#else
const CAgentFileAnimation* CAgentFileV15::GetGesture (INT_PTR pGestureNdx)
{
	CAgentFileAnimation*	lRet = const_cast <CAgentFileAnimation*> (__super::GetGesture (pGestureNdx));

	if	(lRet)
	{
		ReadAnimationStream (lRet);
	}
	return lRet;
}

const CAgentFileAnimation* CAgentFileV15::GetAnimation (INT_PTR pAnimationNdx)
{
	CAgentFileAnimation*	lRet = const_cast <CAgentFileAnimation*> (__super::GetAnimation (pAnimationNdx));

	if	(lRet)
	{
		ReadAnimationStream (lRet);
	}
	return lRet;
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LPCVOID CAgentFileV15::ReadBufferHeader (LPCVOID pBuffer, DWORD pBufferSize)
{
	try
	{
		LPCBYTE		lByte = (LPCBYTE)pBuffer;
#ifdef	__cplusplus_cli
		String^		lFontName = String::Empty;
#else		
		CAtlString	lFontName;
#endif		

#ifdef	__cplusplus_cli
		mHeader->mImageSize.Width = *(LPCWORD)lByte;
#else
		mHeader.mImageSize.cx = *(LPCWORD)lByte;
#endif		
		lByte += sizeof (WORD);
#ifdef	__cplusplus_cli
		mHeader->mImageSize.Height = *(LPCWORD)lByte;
#else
		mHeader.mImageSize.cy = *(LPCWORD)lByte;
#endif		
		lByte += sizeof (WORD);
#ifdef	__cplusplus_cli
		mHeader->mTransparency = *lByte;
#else
		mHeader.mTransparency = *lByte;
#endif
		lByte++;
#ifdef	__cplusplus_cli
		mHeader->mStyle = *(LPCWORD)lByte;
#else
		mHeader.mStyle = *(LPCWORD)lByte;
#endif
		lByte += sizeof(WORD);
		lByte++;

#ifdef	__cplusplus_cli
		if	(mHeader->mStyle & (DWORD)CharacterStyle::Tts)
#else
		if	(mHeader.mStyle & CharStyleTts)
#endif		
		{
			lByte = (LPCBYTE)ReadBufferTts (lByte, (DWORD)(lByte-(LPCBYTE)pBuffer));
		}
#ifdef	__cplusplus_cli
		if	(mHeader->mStyle & (DWORD)CharacterStyle::Balloon)
#else
		if	(mHeader.mStyle & CharStyleBalloon)
#endif		
		{
			lByte = (LPCBYTE)ReadBufferBalloon (lByte, (DWORD)(lByte-(LPCBYTE)pBuffer));
		}

		lByte += sizeof(WORD); // Unknown - always zero

		return lByte;
	}
	catch AnyExceptionDebug
	
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

LPCVOID CAgentFileV15::ReadBufferTts (LPCVOID pBuffer, DWORD pBufferSize)
{
	try
	{
		LPCBYTE	lByte = (LPCBYTE)pBuffer;

#ifdef	__cplusplus_cli
		mTts = gcnew CAgentFileTts (this);
		mTts->mEngine = System::Guid (*(LPCDWORD)lByte, *(LPCWORD)(lByte+4), *(LPCWORD)(lByte+6), lByte[8], lByte[9], lByte[10], lByte[11], lByte[12], lByte[13], lByte[14], lByte[15]);
#else
		mTts.mEngine = *(LPCGUID)lByte;
#endif
		lByte += sizeof(GUID);
#ifdef	__cplusplus_cli
		mTts->mMode = System::Guid (*(LPCDWORD)lByte, *(LPCWORD)(lByte+4), *(LPCWORD)(lByte+6), lByte[8], lByte[9], lByte[10], lByte[11], lByte[12], lByte[13], lByte[14], lByte[15]);
#else
		mTts.mMode = *(LPCGUID)lByte;
#endif
		lByte += sizeof(GUID);

#ifdef	__cplusplus_cli_NOT
		LogMessage (mLogLevel, _T("  TtsSomething [%2.2X (%hu)] Speed [%d] Pitch [%d]"), lByte[0], lByte[0], *(long*)(lByte+1), *(short*)(lByte+5));
		LogDump (mLogLevel|LogHighVolume, lByte-sizeof(GUID)-sizeof(GUID), sizeof(GUID), _T("      "));
		LogDump (mLogLevel|LogHighVolume, lByte-sizeof(GUID), sizeof(GUID), _T("      "));
		LogDump (mLogLevel|LogHighVolume, lByte+1, sizeof(long)+sizeof(short), _T("      "));
#endif
		lByte++;

#ifdef	__cplusplus_cli
		//mTts->mSpeed = *(long*)lByte;
#else
		mTts.mSpeed = *(long*)lByte;
#endif
		lByte += sizeof (long);
#ifdef	__cplusplus_cli
		//mTts->mPitch = *(short*)lByte;
		mTts->mGender = (*(short*)lByte >= 200) ? 1 : 2;
#else
		mTts.mPitch = *(short*)lByte;
		mTts.mGender = (*(short*)lByte >= 200) ? 1 : 2;
#endif
		lByte += sizeof (short);

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
#ifdef	_DEBUG_LOAD
		if	(LogIsActive(mLogLevel))
		{
			LogMessage (mLogLevel, _T("  [%s] Read Tts          of [%u] at [%8.8X (%u)]"), _B(mPath), lByte-(LPBYTE)pBuffer, pBuffer, pBuffer);
		}
#endif		
		return lByte;
	}
	catch AnyExceptionDebug
	
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

LPCVOID CAgentFileV15::ReadBufferBalloon (LPCVOID pBuffer, DWORD pBufferSize)
{
	try
	{
		LPCBYTE		lByte = (LPCBYTE)pBuffer;
		
#ifdef	__cplusplus_cli
		String^		lFontName = nullptr;
		long		lFontHeight = 0;
		int			lFontStyle = (int)System::Drawing::FontStyle::Regular;
#else
		CAtlString	lFontName;
#endif

#ifdef	__cplusplus_cli
		mBalloon = gcnew CAgentFileBalloon (this);
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
		mBalloon->mFgColor = System::Drawing::ColorTranslator::FromWin32 (*(LPCOLORREF)lByte);
#else
		mBalloon.mFgColor = *(LPCDWORD)lByte;
#endif
		lByte += sizeof(DWORD);
#ifdef	__cplusplus_cli
		mBalloon->mBkColor = System::Drawing::ColorTranslator::FromWin32 (*(LPCOLORREF)lByte);
#else
		mBalloon.mBkColor = *(LPCDWORD)lByte;
#endif
		lByte += sizeof(DWORD);
#ifdef	__cplusplus_cli
		mBalloon->mBrColor = System::Drawing::ColorTranslator::FromWin32 (*(LPCOLORREF)lByte);
#else
		mBalloon.mBrColor = *(LPCDWORD)lByte;
#endif
		lByte += sizeof(DWORD);

		lByte = (LPCBYTE)CAgentFileBinary::ReadBufferString (lByte, false, lFontName);
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
		mBalloon.mFont.lfStrikeOut = *lByte;
#endif
		lByte++;
#ifdef	__cplusplus_cli
		if	(*lByte)
		{
			lFontStyle |= (int)System::Drawing::FontStyle::Italic;
		}
#else
		mBalloon.mFont.lfItalic = *lByte;
#endif
		lByte++;

#ifdef	__cplusplus_cli
		try
		{
			mBalloon->mFont = gcnew System::Drawing::Font (lFontName, (float)lFontHeight, (FontStyle)lFontStyle, GraphicsUnit::Pixel);
		}
		catch AnyExceptionDebug
#else
		mBalloon.mFont.lfCharSet = DEFAULT_CHARSET;
#endif
#ifdef	_DEBUG_LOAD
		if	(LogIsActive(mLogLevel))
		{
			LogMessage (mLogLevel, _T("  [%s] Read Balloon      of [%u] at [%8.8X (%u)]"), _B(mPath), lByte-(LPBYTE)pBuffer, pBuffer, pBuffer);
		}
#endif
		return lByte;
	}
	catch AnyExceptionDebug
	
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

LPCVOID CAgentFileV15::ReadBufferIdentity (LPCVOID pBuffer, DWORD pBufferSize)
{
	try
	{
		LPCBYTE					lByte = (LPCBYTE)pBuffer;
#ifdef	__cplusplus_cli
		CAgentFileName^			lName;
#else
		tPtr <CAgentFileName>	lName;
#endif

#ifdef	__cplusplus_cli
		mHeader->mGuid = System::Guid (*(LPCDWORD)lByte, *(LPCWORD)(lByte+4), *(LPCWORD)(lByte+6), lByte[8], lByte[9], lByte[10], lByte[11], lByte[12], lByte[13], lByte[14], lByte[15]);
#else
		mHeader.mGuid = *(LPCGUID)lByte;
#endif		
		lByte += sizeof(GUID);

#ifdef	__cplusplus_cli
		mNames = gcnew CAgentFileNames (this);
		lName = gcnew CAgentFileName (this, mNames);
#else
		lName = new CAgentFileName;
#endif
		lName->mLanguage = MAKELANGID (LANG_ENGLISH, SUBLANG_DEFAULT);

		lByte = (LPCBYTE)CAgentFileBinary::ReadBufferString (lByte, false, lName->mName);
		lByte = (LPCBYTE)CAgentFileBinary::ReadBufferString (lByte, false, lName->mDesc1);
		lByte = (LPCBYTE)CAgentFileBinary::ReadBufferString (lByte, false, lName->mDesc2);

#ifdef	__cplusplus_cli
		if	(!lName->mName->IsNullOrEmpty (lName->mName))
		{
			lName->mName = String::Concat (lName->mName->Substring (0,1)->ToUpper() + lName->mName->Substring (1));
		}
		mNames->Add (lName);
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
		mNames.Add (lName.Detach ());
#endif
		return lByte;
	}
	catch AnyExceptionDebug
	
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

LPCVOID CAgentFileV15::ReadBufferPalette (LPCVOID pBuffer, DWORD pBufferSize)
{
	try
	{
		LPCBYTE	lByte = (LPCBYTE)pBuffer;
		WORD	lPaletteSize;

		lPaletteSize = *(LPCWORD)(lByte);
		lByte += sizeof(DWORD);
		
		if	(lPaletteSize == 1)
		{
			lByte--;
			lPaletteSize = 256;
		}

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

				for (lNdx = 0; lNdx < (int)lPaletteSize; lNdx++)
				{
					lColors [lNdx] = System::Drawing::Color::FromArgb (255, (int)lByte[2], (int)lByte[1], (int)lByte[0]); // BGRA
					lByte += sizeof(COLORREF);
				}
				for (; lNdx < 256; lNdx++)
				{
					lColors [lNdx] = System::Drawing::Color::Black;
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

#ifdef	_DEBUG_LOAD
		if	(LogIsActive (mLogLevel))
		{
			LogMessage (mLogLevel, _T("  [%s] Read Palette      of [%u] at [%8.8X (%u)]"), _B(mPath), lByte-(LPCBYTE)pBuffer, pBuffer, pBuffer);
		}
#endif
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
	catch AnyExceptionDebug
	
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

LPCVOID CAgentFileV15::ReadBufferAnimations (LPCVOID pBuffer, DWORD pBufferSize)
{
	try
	{
		LPCBYTE						lByte = (LPCBYTE)pBuffer;
		WORD						lAnimationCount;
#ifdef	__cplusplus_cli
		V15FileAnimation^			lAnimation;
#else
		tPtr <CAgentFileAnimation>	lAnimation;
#endif		

		lAnimationCount = (int)(*(LPCWORD)lByte);
		lByte += sizeof(WORD);

		while (lAnimationCount > 0)
		{
#ifdef	__cplusplus_cli
			lAnimation = gcnew V15FileAnimation (this, mGestures);
#else
			lAnimation = new CAgentFileAnimation;
#endif
			lByte = (LPCBYTE)CAgentFileBinary::ReadBufferString (lByte, false, lAnimation->mName);
			lByte = (LPCBYTE)CAgentFileBinary::ReadBufferString (lByte, false, lAnimation->mFileName);
			lByte = (LPCBYTE)CAgentFileBinary::ReadBufferString (lByte, false, lAnimation->mReturnName);
			lAnimation->mFileCheckSum = *(LPDWORD)lByte;
			lByte += sizeof(DWORD);
			
#ifdef	__cplusplus_cli
			lAnimation->mReturnType = String::IsNullOrEmpty (lAnimation->mReturnName) ? 2 : 0;
#else
			lAnimation->mReturnType = lAnimation->mReturnName.IsEmpty () ? 2 : 0;
#endif

#ifdef	__cplusplus_cli
			mGestures->Add (lAnimation);
#else			
			mGestures.mAnimations.InsertAt (AddSortedString (mGestures.mNames, CAtlString ((BSTR)lAnimation->mName)), lAnimation);
			lAnimation.Detach ();
#endif			
			lAnimationCount--;
		}

#if	FALSE		
		try
		{
#ifdef	__cplusplus_cli
			for each (CAgentFileAnimation^ lAnimation in mGestures)
#else
			CAgentFileAnimation *	lAnimation;
			for (int lNdx = 0; lAnimation = mGestures.mAnimations (lNdx); lNdx++)
#endif
			{
#ifdef	__cplusplus_cli
				if	(String::IsNullOrEmpty (lAnimation->mReturnName))
#else
				if	(lAnimation->mReturnName.IsEmpty())
#endif								
				{
#ifdef	__cplusplus_cli
					String^		lReturnName = String::Format ("{0}Return", lAnimation->mName);
#else
					CAtlString	lReturnName ((BSTR)lAnimation->mName);
					
					lReturnName += _T("Return");
#endif
#ifdef	__cplusplus_cli
					if	(mGestures->Contains (lReturnName))
#else
					if	(FindAnimation (lReturnName) >= 0)
#endif					
					{
#ifdef	__cplusplus_cli
						lAnimation->mReturnName = lReturnName;
#else
						lAnimation->mReturnName = lReturnName.AllocSysString();
#endif
						lAnimation->mReturnType = 0;
					}					
				}
			}
		}
		catch AnyExceptionDebug
#endif
		return lByte;
	}
	catch AnyExceptionDebug
	
	return NULL;
}

LPCVOID CAgentFileV15::ReadBufferStates (LPCVOID pBuffer, DWORD pBufferSize)
{
	try
	{
		LPCBYTE	lByte = (LPCBYTE)pBuffer;
		WORD	lStateCount;

		lStateCount = *(LPCWORD)lByte;
		lByte += sizeof(WORD);

		while	(
					(lStateCount > 0)
				&&	(lByte < ((LPCBYTE)pBuffer + pBufferSize))
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

			lByte = (LPCBYTE)CAgentFileBinary::ReadBufferString (lByte, false, lState);
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

#ifdef	_DEBUG_LOAD
			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel|LogHighVolume, _T("    State [%s] Gestures [%u]"), _B(lState), lGestureCount);
			}
#endif
			while	(
						(lGestureCount > 0)
					&&	(lByte < ((LPCBYTE)pBuffer + pBufferSize))
					)
			{
				lByte = (LPCBYTE)CAgentFileBinary::ReadBufferString (lByte, false, lGesture);
#ifdef	__cplusplus_cli
				lGestures->Add (lGesture);
#else
				lGestures.Add (lGesture);
#endif
#ifdef	_DEBUG_LOAD
				if	(LogIsActive (mLogLevel))
				{
					LogMessage (mLogLevel|LogHighVolume, _T("      Gesture [%s]"), _B(lGesture));
				}
#endif				
				lGestureCount--;
			}

#ifdef	__cplusplus_cli
			mStates->Add (lState, lGestures->ToArray());
#endif
			lStateCount--;
		}

#ifdef	_DEBUG_LOAD
		if	(LogIsActive(mLogLevel))
		{
			LogMessage (mLogLevel, _T("  [%s] Read States       of [%u (%u)] at [%8.8X (%u)]"), _B(mPath), lByte-(LPBYTE)pBuffer, pBufferSize, pBuffer, pBuffer);
		}
#endif		
		return lByte;
	}
	catch AnyExceptionDebug
	
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
LPCVOID CAgentFileV15::ReadBufferSounds (CAgentFileAnimation^ pAnimation, LPCVOID pBuffer, DWORD pBufferSize)
#else
LPCVOID CAgentFileV15::ReadBufferSounds (CAgentFileAnimation* pAnimation, LPCVOID pBuffer, DWORD pBufferSize)
#endif
{
	try
	{
		LPCBYTE					lByte = (LPCBYTE)pBuffer;
		WORD					lSoundCount;
		WORD					lSoundNum;
		DWORD					lByteCount;
#ifdef	__cplusplus_cli
		array <BYTE>^			lSound;
		pin_ptr <BYTE>			lSoundBytes;
#else		
		tPtr <CAtlByteArray>	lSound;
#endif		
				
		lSoundCount = *(LPCWORD)lByte;
		lByte += sizeof (WORD);

#ifdef	__cplusplus_cli_NOT
		LogMessage (mLogLevel|LogHighVolume, _T("[%s] Sounds [%hu] Starting [%u]"), _B(pAnimation->mName), lSoundCount, mStreamSounds->Count);
#endif
#ifdef	_DEBUG_LOAD
		if	(LogIsActive (mLogLevel))
		{
			LogMessage (mLogLevel|LogHighVolume, _T("[%s] Sounds [%hu] Starting [%u]"), _B(pAnimation->mName), lSoundCount, mStreamSounds.GetCount());
		}
#endif
		for	(lSoundNum = 0; lSoundNum < lSoundCount; lSoundNum++)
		{
			lByteCount = *(LPCDWORD)lByte;
			lByte += sizeof(DWORD);

#ifdef	__cplusplus_cli_NOT
			LogMessage (mLogLevel|LogHighVolume, _T("  Sound [%hu (%hu)] of [%hu]"), lSoundNum, mStreamSounds->Count, lByteCount);
#endif
#ifdef	_DEBUG_LOAD
			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel|LogHighVolume, _T("  Sound [%hu (%hu)] of [%hu]"), lSoundNum, mStreamSounds.GetCount(), lByteCount);
			}
#endif
#ifdef	__cplusplus_cli
			if	(lSound = gcnew array <BYTE> (lByteCount))
#else
			if	(lSound = new CAtlByteArray)
#endif			
			{
#ifdef	__cplusplus_cli
				lSoundBytes = &lSound[0];
				memcpy (lSoundBytes, lByte, lByteCount);
				mStreamSounds->Add (lSound);
#else
				lSound->SetCount (lByteCount);
				memcpy (lSound->GetData(), lByte, lByteCount);
				mStreamSounds.Add (lSound.Detach());
#endif				
			}
			lByte += lByteCount;

			if	(lByte > (LPCBYTE)pBuffer + pBufferSize)
			{
#ifdef	_DEBUG
				LogMessage (LogDebug, _T("!!! Screwed !!!"));
#endif
				break;
			}
		}
		return lByte;
 	}
	catch AnyExceptionDebug
	
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
LPCVOID CAgentFileV15::ReadBufferImages (CAgentFileAnimation^ pAnimation, LPCVOID pBuffer, DWORD pBufferSize)
#else
LPCVOID CAgentFileV15::ReadBufferImages (CAgentFileAnimation* pAnimation, LPCVOID pBuffer, DWORD pBufferSize)
#endif
{
	try
	{
		LPCBYTE					lByte = (LPCBYTE)pBuffer;
		WORD					lImageCount;
		WORD					lImageNum;
		DWORD					lByteCount;
#ifdef	__cplusplus_cli
		CAgentFileImage^		lImage;
		pin_ptr <BYTE>			lImageBits;
#else		
		tPtr <CAgentFileImage>	lImage;
#endif		
				
		lImageCount = *(LPCWORD)lByte;
		lByte += sizeof (WORD);

#ifdef	__cplusplus_cli_NOT
		LogMessage (mLogLevel|LogHighVolume, _T("[%s] Images [%hu] Starting [%u]"), _B(pAnimation->mName), lImageCount, mStreamImages->Count);
#endif
#ifdef	_DEBUG_LOAD
		if	(LogIsActive (mLogLevel))
		{
			LogMessage (mLogLevel|LogHighVolume, _T("[%s] Images [%hu] Starting [%u]"), _B(pAnimation->mName), lImageCount, mStreamImages.GetCount());
		}
#endif
		for	(lImageNum = 0; lImageNum < lImageCount; lImageNum++)
		{
			lByteCount = *(LPCDWORD)lByte;
			lByte += sizeof(DWORD);

#ifdef	__cplusplus_cli
			if	(!lByte)
			{
				LogMessage (mLogLevel|LogHighVolume, _T("  Image [%hu] ImageSomething [%2.2X] [%u]"), lImageNum, *lByte, *lByte);
			}
#endif
			lByte++;

#ifdef	_DEBUG_LOAD
			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel|LogHighVolume, _T("  Image [%hu (%hu)] of [%hu]"), lImageNum, mStreamImages.GetCount(), lByteCount);
			}
#endif
#ifdef	__cplusplus_cli
			if	(
					(lImage = gcnew CAgentFileImage)
				&&	(lImage->mBits = gcnew array <BYTE> (lByteCount))
				)
#else
			if	(
					(lImage = new CAgentFileImage)
				&&	(lImage->mBits = new BYTE [lByteCount])
				)
#endif			
			{
#ifdef	__cplusplus_cli
				lImageBits = &lImage->mBits[0];
				memcpy (lImageBits, lByte, lByteCount);

				lImage->mImageSize = mHeader->ImageSize;
				lImage->mImageNum = (UInt32)mStreamImages->Count;
				mStreamImages->Add (lImage);
#else
				lImage->mBitsSize = lByteCount;
				memcpy (lImage->mBits, lByte, lByteCount);

				lImage->mImageSize = mHeader.ImageSize;
				lImage->mImageNum = (long)mStreamImages.GetCount();
				mStreamImages.Add (lImage.Detach ());
#endif			
			}
			lByte += lByteCount;

			lByteCount = *(LPCDWORD)lByte;
			lByte += sizeof(DWORD);
			lByte += lByteCount;

			if	(lByte > (LPCBYTE)pBuffer + pBufferSize)
			{
#ifdef	_DEBUG
				LogMessage (LogDebug, _T("!!! Screwed !!!"));
#endif
				break;
			}
		}
		return lByte;
 	}
	catch AnyExceptionDebug
	
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
LPCVOID CAgentFileV15::ReadBufferFrames (CAgentFileAnimation^ pAnimation, LPCVOID pBuffer, DWORD pBufferSize, Int32 pFirstImage, Int32 pFirstSound)
#else
LPCVOID CAgentFileV15::ReadBufferFrames (CAgentFileAnimation* pAnimation, LPCVOID pBuffer, DWORD pBufferSize, long pFirstImage, long pFirstSound)
#endif
{
	try
	{
		LPCBYTE	lByte = (LPCBYTE)pBuffer;
		WORD	lFrameCount;
		WORD	lFrameNum;

		lFrameCount = *(LPCWORD)lByte;
		lByte += sizeof(WORD);

#ifdef	__cplusplus_cli_NOT
		LogMessage (LogDebugFast, _T("[%s] [%s] Frames [%hu]"), _B(mPath), _B(pAnimation->mName), lFrameCount);
#endif
#ifdef	_DEBUG_ANIMATION
		if	(LogIsActive (MaxLogLevel (mLogLevel, _DEBUG_ANIMATION)))
		{
			LogMessage (_DEBUG_ANIMATION, _T("[%ls] Frames [%hu]"), (BSTR)pAnimation->mName, lFrameCount);
		}
#endif

		if	(
				(lFrameCount > 0)
#ifdef	__cplusplus_cli
			&&	(pAnimation->mFrames = gcnew CAgentFileFrames (this, pAnimation))
#else
			&&	(pAnimation->mFrames = new CAgentFileFrame [lFrameCount])
#endif
			)		
		{
#ifndef	__cplusplus_cli
			pAnimation->mFrameCount = lFrameCount;
#endif
			for	(lFrameNum = 0; lFrameNum < lFrameCount; lFrameNum++)
			{
#ifdef	__cplusplus_cli
				CAgentFileFrame^	lFrame = gcnew CAgentFileFrame (this, pAnimation->Frames);
#else				
				CAgentFileFrame*	lFrame = &pAnimation->mFrames [(short)(long)lFrameNum];
#endif				
				WORD				lBranchCount;
				WORD				lOverlayCount;
				WORD				lOverlayNum = 0;

#ifdef	__cplusplus_cli
				pAnimation->mFrames->Add (lFrame);
				lFrame->mImages = gcnew CAgentFileFrameImages (this, lFrame);
				lFrame->mImages->Add (gcnew CAgentFileFrameImage (this, lFrame));
				lFrame->mImages[0]->mImageNdx = (Int32)(*(LPCWORD)lByte);
				if	(lFrame->mImages[0]->mImageNdx >= 0)
				{
					lFrame->mImages[0]->mImageNdx += (Int32)pFirstImage;
				}
#else				
				lFrame->mImageCount = 1;
				lFrame->mImages = new CAgentFileFrameImage [1];
				lFrame->mImages[0].mImageNdx = (long)(*(LPCWORD)lByte);
				if	(lFrame->mImages[0].mImageNdx >= 0)
				{
					lFrame->mImages[0].mImageNdx += (long)pFirstImage;
				}
#endif
				lByte += sizeof(WORD);

				lFrame->mSoundNdx = (short)*(LPCWORD)lByte;
				if	(lFrame->mSoundNdx >= 0)
				{
					lFrame->mSoundNdx += (short)pFirstSound;
				}
				lByte += sizeof(WORD);
				lFrame->mDuration = *(LPCWORD)lByte;
				lByte += sizeof(WORD);

#ifdef	__cplusplus_cli_NOT
				{
					LPCWORD lWord = (LPCWORD)lByte;
					if	(lWord[0] || lWord[1])
					{
						LogMessage (LogDebug, _T("[%s] [%s] Frame [%d] Image [%d] Sound [%d] Duration [%d] FrameSomething [%4.4X (%d)] FrameSomething [%4.4X (%d)]"), _B(mPath), _B(pAnimation->mName), pAnimation->mFrames->Count, lFrame->mImages[0]->mImageNdx, lFrame->mSoundNdx, lFrame->mDuration, lWord[0], lWord[0], lWord[1], lWord[1]);
					}
				}
#endif
				lByte += sizeof(WORD)*2;

				lBranchCount = *lByte;
				lByte++;
#ifdef	__cplusplus_cli
				if	(lBranchCount > 0)
				{
					lFrame->mBranching = gcnew array <CAgentFileFrameBranch> (lBranchCount);
				}
#endif					
				if	(lBranchCount > 0)
				{
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
#ifdef	__cplusplus_cli_NOT
				if	(lBranchCount > 0)
				{
					LPCWORD lWord = (LPCWORD)lByte;
					String^	lWords = String::Format (" [{0:X4} ({0:D} {1:D} {2:D})] [{3:X4} ({3:D} {4:D} {5:D})]", lWord[0], LOBYTE(lWord[0]), HIBYTE(lWord[0]), lWord[1], LOBYTE(lWord[1]), HIBYTE(lWord[1]));
					if	(lBranchCount>1)
					{
						lWord+=2;
						lWords += String::Format (" [{0:X4} ({0:D} {1:D} {2:D})] [{3:X4} ({3:D} {4:D} {5:D})]", lWord[0], LOBYTE(lWord[0]), HIBYTE(lWord[0]), lWord[1], LOBYTE(lWord[1]), HIBYTE(lWord[1]));
					}
					if	(lBranchCount>2)
					{
						lWord+=2;
						lWords += String::Format (" [{0:X4} ({0:D} {1:D} {2:D})] [{3:X4} ({3:D} {4:D} {5:D})]", lWord[0], LOBYTE(lWord[0]), HIBYTE(lWord[0]), lWord[1], LOBYTE(lWord[1]), HIBYTE(lWord[1]));
					}
					LogMessage (LogDebug, _T("[%s] [%s] Frame [%d] Branching [%hu]%s"), _B(mPath), _B(pAnimation->mName), pAnimation->mFrames->Count, lByte[-2], _B(lWords));
					LogDumpWords (LogDebugFast, lByte, lBranchCount*4, _T("  "), true);
				}
#endif
				lByte += sizeof(WORD)*lBranchCount*2;

				lOverlayCount = *lByte;
				lByte++;

				if	(
						(lOverlayCount > 0)
#ifdef	__cplusplus_cli
					&&	(lFrame->mOverlays = gcnew CAgentFileFrameOverlays (this, lFrame))
#else
					&&	(lFrame->mOverlays = new CAgentFileFrameOverlay [lOverlayCount])
					&&	(lFrame->mOverlayCount = lOverlayCount)
#endif						
					)
				{
#ifdef	__cplusplus_cli_NOT
					LogMessage (LogDebug, _T("[%s] [%s] Frame [%d] Overlays [%u]"), _B(mPath), _B(pAnimation->mName), pAnimation->mFrames->Count, lOverlayCount);
#endif
					while	(lOverlayCount > 0)
					{
						BYTE	lOverlayType;
						DWORD	lOverlaySize;
						BYTE	lOverlayFlagSomething;
						
						lOverlayType = *lByte;
						lByte++;
						lOverlaySize = *(LPCDWORD)lByte;
						lByte+= sizeof(DWORD);
						
						if	(lOverlaySize)
						{
#ifdef	__cplusplus_cli
							CAgentFileFrameOverlay^	lOverlay = gcnew CAgentFileFrameOverlay (this, lFrame);
							CAgentFileImage^		lOverlayImage = gcnew CAgentFileImage;
							pin_ptr <BYTE>			lImageBits;
#else
							CAgentFileFrameOverlay*	lOverlay = &lFrame->mOverlays [lOverlayNum];
							tPtr <CAgentFileImage>	lOverlayImage = new CAgentFileImage;
#endif						
							lOverlay->mOverlayType = (AgentMouthOverlay)lOverlayType;
							lOverlayFlagSomething = *lByte;
							lByte++;
#ifdef	__cplusplus_cli
							lOverlay->mOffset.X = (long)(short)*(LPCWORD)lByte;
#else
							lOverlay->mOffset.x = (long)(short)*(LPCWORD)lByte;
#endif						
							lByte += sizeof(WORD);
#ifdef	__cplusplus_cli
							lOverlay->mOffset.Y = (long)(short)*(LPCWORD)lByte;
#else
							lOverlay->mOffset.y = (long)(short)*(LPCWORD)lByte;
#endif						
							lByte += sizeof(WORD);
#ifdef	__cplusplus_cli
							lOverlayImage->mImageSize.Width = (long)(short)*(LPCWORD)lByte;
#else
							lOverlayImage->mImageSize.cx = (long)(short)*(LPCWORD)lByte;
#endif						
							lByte += sizeof(WORD);
#ifdef	__cplusplus_cli
							lOverlayImage->mImageSize.Height = (long)(short)*(LPCWORD)lByte;
#else
							lOverlayImage->mImageSize.cy = (long)(short)*(LPCWORD)lByte;
#endif						
							lByte += sizeof(WORD);
#ifdef	__cplusplus_cli
							lFrame->mOverlays->Add (lOverlay);
#endif

#ifdef	__cplusplus_cli
							if	(lOverlayImage->mBits = gcnew array <BYTE> (lOverlaySize))
#else						
							if	(lOverlayImage->mBits = new BYTE [lOverlaySize])
#endif						
							{
#ifdef	__cplusplus_cli
								lImageBits = &lOverlayImage->mBits[0];
								memcpy (lImageBits, lByte, lOverlaySize);
								lOverlayImage->mImageNum = (UInt32)mStreamImages->Count;
								lOverlay->mImageNdx = lOverlayImage->mImageNum;
								mStreamImages->Add (lOverlayImage);
#else							
								memcpy (lOverlayImage->mBits.Ptr(), lByte, lOverlaySize);
								lOverlayImage->mBitsSize = lOverlaySize;
								lOverlayImage->mImageNum = (long)mStreamImages.GetCount();
								lOverlay->mImageNdx = lOverlayImage->mImageNum;
								mStreamImages.Add (lOverlayImage.Detach ());
#endif							
							}
							lByte += lOverlaySize;
#ifdef	__cplusplus_cli
							if	(lOverlayFlagSomething)
							{
								LogMessage (LogDebugFast, _T("[%s] [%s] Frame [%d] Overlay [%u] [%u] [%u] - Image [%d] Offset [%d %d] Size [%d %d] Flag? [%u]"), _B(mPath), _B(pAnimation->mName), pAnimation->mFrames->Count, lOverlayNum, lOverlay->mOverlayType, lOverlaySize, lOverlay->mImageNdx, lOverlay->mOffset.X, lOverlay->mOffset.Y, lOverlayImage->mImageSize.Width, lOverlayImage->mImageSize.Height, lOverlayFlagSomething);
							}
#endif							
						}
						else
						{
#ifdef	__cplusplus_cli_NOT
							LogMessage (LogDebugFast, _T("[%s] [%s] Frame [%d] Overlay [%u] [%u] [%u]"), _B(mPath), _B(pAnimation->mName), pAnimation->mFrames->Count, lOverlayNum, lOverlayType, lOverlaySize);
#endif							
						}

						lOverlayCount--;
						lOverlayNum++;
					}
				}				
			}
		}
		return lByte;
 	}
	catch AnyExceptionDebug
	
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
int CAgentFileV15::ImageCount::get ()
{
	return mStreamImages->Count;
}
#else
INT_PTR CAgentFileV15::GetImageCount () const
{
	return mStreamImages.GetCount();
}
#endif

#ifdef	__cplusplus_cli
CAgentFileImage^ CAgentFileV15::GetImage (int pImageNdx, bool p32Bit, System::Drawing::Color pBkColor)
{
	CAgentFileImage^	lImage;
	CAgentFileImage^	lStreamImage;

	if	(
			(pImageNdx >= 0)
		&&	(pImageNdx < mStreamImages->Count)
		&&	(lStreamImage = mStreamImages [pImageNdx])
		)
	{
		if	(p32Bit)
		{
			lImage = Get32BitImage (lStreamImage, pBkColor);
		}
		else
		if	(lImage = gcnew CAgentFileImage)
		{
			lImage->mImageNum = lStreamImage->mImageNum;
			lImage->mImageSize = lStreamImage->mImageSize;
			if	(
					(lStreamImage->mBits)
				&&	(lImage->mBits = gcnew array<BYTE> (lStreamImage->mBits->Length))
				)
			{
				Array::Copy (lStreamImage->mBits, lImage->mBits, lStreamImage->mBits->Length);
			}
		}
	}
	return lImage;
}
#else
CAgentFileImage* CAgentFileV15::GetImage (INT_PTR pImageNdx, bool p32Bit, const COLORREF* pBkColor)
{
	tPtr <CAgentFileImage>	lImage;
	const CAgentFileImage*	lStreamImage;

	if	(
			(pImageNdx >= 0)
		&&	(pImageNdx < (INT_PTR)mStreamImages.GetCount())
		&&	(lStreamImage = mStreamImages [pImageNdx])
		)
	{
		if	(p32Bit)
		{
			lImage = Get32BitImage (lStreamImage, pBkColor);
		}
		else
		if	(
				(lImage = new CAgentFileImage)
			&&	(lImage->mBits = new BYTE [lStreamImage->mBitsSize])
			)
		{
			lImage->mImageNum = lStreamImage->mImageNum;
			lImage->mImageSize = lStreamImage->mImageSize;
			lImage->mBitsSize = lStreamImage->mBitsSize;
			memcpy (lImage->mBits.Ptr(), lStreamImage->mBits.Ptr(), lImage->mBitsSize);
		}
		else
		{
			lImage = NULL;
		}
	}
	return lImage.Detach ();
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
int CAgentFileV15::SoundCount::get ()
{
	return mStreamSounds->Count;
}
#else
INT_PTR CAgentFileV15::GetSoundCount () const
{
	return mStreamSounds.GetCount();
}
#endif

#ifdef	__cplusplus_cli
int CAgentFileV15::GetSoundSize (int pSoundNdx)
{
	long	lRet = -1;

	if	(
			(pSoundNdx >= 0)
		&&	(pSoundNdx < mStreamSounds->Count)
		)
	{
		lRet = mStreamSounds[pSoundNdx]->Length;
	}
	return lRet;
}
#else
long CAgentFileV15::GetSoundSize (INT_PTR pSoundNdx)
{
	long			lRet = -1;
	CAtlByteArray *	lStreamSound;

	if	(lStreamSound = mStreamSounds (pSoundNdx))
	{
		lRet = (long)lStreamSound->GetCount();
	}
	return lRet;
}
#endif

#ifdef	__cplusplus_cli
array <BYTE>^ CAgentFileV15::GetSound (int pSoundNdx)
{
	if	(
			(pSoundNdx >= 0)
		&&	(pSoundNdx < mStreamSounds->Count)
		)
	{
		return mStreamSounds [pSoundNdx];
	}
	return nullptr;
}
#else
LPCVOID CAgentFileV15::GetSound (INT_PTR pSoundNdx)
{
	LPCVOID			lSound = NULL;
	CAtlByteArray *	lStreamSound;

	if	(lStreamSound = mStreamSounds (pSoundNdx))
	{
		lSound = lStreamSound->GetData();
	}
	return lSound;
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
/////////////////////////////////////////////////////////////////////////////

V15FileAnimation::V15FileAnimation (CharacterFile^ pOwner, FileGestures^ pContainer)
:	FileAnimation (pOwner, pContainer)
{
}

V15FileAnimation::~V15FileAnimation ()
{
}

void V15FileAnimation::Empty ()
{
	__super::Empty ();
	mFileCheckSum = 0;
	mFileName = nullptr;
}

/////////////////////////////////////////////////////////////////////////////

UInt16 V15FileAnimation::FrameCount::get()
{
	EnsureIsLoaded ();	
	return __super::get();
}

FileFrames^ V15FileAnimation::Frames::get()
{
	EnsureIsLoaded ();	
	return __super::get();
}

Boolean V15FileAnimation::CopyTo (FileAnimation^ pTarget, Boolean pDeepCopy)
{
	Boolean				lRet;
	V15FileAnimation^	lTarget;

	EnsureIsLoaded ();	
	lRet = __super::CopyTo (pTarget, pDeepCopy);

	if	(
			(lRet)
		&&	(V15FileAnimation::typeid->IsAssignableFrom (pTarget->GetType()))
		&&	(lTarget = safe_cast <V15FileAnimation^> (pTarget))
		)
	{
		lTarget->mFileCheckSum = mFileCheckSum;
		lTarget->mFileName = mFileName ? gcnew String (mFileName) : nullptr;
	}
	return lRet;
}

System::String^ V15FileAnimation::ToString()
{
	EnsureIsLoaded ();	
    return String::Format ("{0} stream \"{1}\"", __super::ToString(), mFileName);
}

/////////////////////////////////////////////////////////////////////////////

Boolean V15FileAnimation::IsLoaded::get()
{
	return mFileCheckSum == (UInt32)-1; 
}

void V15FileAnimation::IsLoaded::set (Boolean pValue)
{
	mFileCheckSum = pValue ? (UInt32)-1 : 0;
}

void V15FileAnimation::EnsureIsLoaded ()
{
	if	(!IsLoaded)
	{
		try
		{
			CAgentFileV15^	lOwner;
			
			if	(
					(
						(lOwner = safe_cast <CAgentFileV15^> (mOwner))
					||	(
							(mContainer)
						&&	(lOwner = safe_cast <CAgentFileV15^> (mContainer->mOwner))
						)
					)
				&&	(lOwner->mGestures)
				&&	(lOwner->mGestures->Contains (this))
				)
			{
				lOwner->ReadAnimationStream (this);
			}
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////
#endif	__cplusplus_cli
/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
} // namespace Character
} // namespace DoubleAgent
#endif
/////////////////////////////////////////////////////////////////////////////
