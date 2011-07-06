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
#include "AgentFileAcf.h"
#ifdef	__cplusplus_cli
#include "AgtErr.h"
#else
#include <shlwapi.h>
#include <wininet.h>
#include "DaCore.h"
#include "FileDownload.h"
#include "GuidStr.h"
#include "StringArrayEx.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "DebugStr.h"
#endif
#endif
#include "AgentFileDefs.inl"

#ifdef __cplusplus_cli
using namespace System::Collections::Generic;
using namespace System::Runtime::Serialization;
#else
IMPLEMENT_DLL_OBJECT(CAgentFileAcf)
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
namespace DoubleAgent {
namespace Character {
#endif
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
[Serializable] private ref class AcfFileAnimation : public FileAnimation
{
public:	
	[NonSerialized]	System::String^	mFileName;
	[NonSerialized]	UInt32			mFileCheckSum;

internal:
	AcfFileAnimation (CharacterFile^ pOwner, FileGestures^ pContainer);
public:	
	virtual ~AcfFileAnimation ();
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

CAgentFileAcf::CAgentFileAcf()
{
	Close ();
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CAgentFileAcf::CAgentFileAcf"), this);
#endif
}

CAgentFileAcf::~CAgentFileAcf ()
{
#ifdef	_DEBUG_INSTANCE
	LogMessage (_DEBUG_INSTANCE, _T("[%p] CAgentFileAcf::~CAgentFileAcf"), this);
#endif
	Close ();
}

#ifdef	__cplusplus_cli
CAgentFileAcf^ CAgentFileAcf::CreateInstance ()
#else
CAgentFileAcf* CAgentFileAcf::CreateInstance ()
#endif
{
#ifdef	__cplusplus_cli
	LogStart (false);
	return gcnew CAgentFileAcf;
#else
	return new CAgentFileAcf;
#endif
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
bool CAgentFileAcf::IsAcfFile::get ()
{
	if	(IsOpen)
	{
		return (mSignature == sAcfFileSignature);
	}
	return true;
}
#else
bool CAgentFileAcf::get_IsAcfFile () const
{
	if	(IsOpen)
	{
		return (mSignature == sAcfFileSignature);
	}
	return false;
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
bool CAgentFileAcf::Open (const System::String^ pPath)
{
	bool			lRet = false;
	String^			lPath = ParseFilePath (pPath);
	System::Uri^	lUri = gcnew System::Uri (lPath);

	Close ();

#ifdef	_DEBUG_LOAD
	UINT	lLogLevel = mLogLevel;
	mLogLevel = MinLogLevel (mLogLevel, _DEBUG_LOAD);
#endif
	if	(LogIsActive (mLogLevel))
	{
		LogMessage (mLogLevel, _T("Open [%s]"), _B(lPath));
	}

	if	(lUri->Scheme == lUri->UriSchemeHttp)
	{
		throw gcnew NotSupportedException ("The HTTP scheme is not yet supported.");
	}
	else
	{
		mPath = lPath;
		lRet = LoadFile (lPath);
	}
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
HRESULT CAgentFileAcf::Open (LPCTSTR pPath)
{
	HRESULT		lResult = S_OK;
	CAtlString	lPath = ParseFilePath (pPath);

	Close ();

#ifdef	_DEBUG_LOAD
	UINT lLogLevel = mLogLevel;
	mLogLevel = MinLogLevel (mLogLevel, _DEBUG_LOAD);
#endif

	mPath = lPath;
	if	(LogIsActive (mLogLevel))
	{
		LogMessage (mLogLevel, _T("Open [%s]"), lPath);
	}

	if	(PathIsURL (lPath))
	{
		tPtr <CFileDownload>	lOldDownload = mFileDownload.Detach();

		mFileDownload = CFileDownload::CreateInstance (lPath);
		if	(lOldDownload)
		{
			mFileDownload->SetBindFlags (lOldDownload->GetBindFlags());
		}
		SafeFreeSafePtr (lOldDownload);

		if	(SUCCEEDED (lResult = mFileDownload->Download ()))
		{
			lResult = LoadFile (CAtlString ((BSTR) mFileDownload->GetCacheName()));
		}
	}
	else
	{
		lResult = LoadFile (lPath);
	}
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

#ifdef	__cplusplus_cli
bool CAgentFileAcf::Save (const System::String^ pPath, CAgentFile^ pSource)
{
	throw gcnew NotSupportedException ("Saving ACF files is not yet supported.");
}
#endif

void CAgentFileAcf::Close ()
{
#ifdef	__cplusplus_cli
	mAcaImages = gcnew System::Collections::Generic::List <CAgentFileImage^> ();
	mAcaSounds = gcnew System::Collections::Generic::List <array <BYTE>^> ();
#else
	mAcaImages.DeleteAll ();
	mAcaSounds.DeleteAll ();
#endif	
	__super::Close ();
	mSignature = sAcfFileSignature;
}

/////////////////////////////////////////////////////////////////////////////
#ifndef	__cplusplus_cli
/////////////////////////////////////////////////////////////////////////////

void CAgentFileAcf::SetDownloadMode (bool pRefresh, bool pReload, bool pSecure)
{
	if	(!mFileDownload)
	{
		mFileDownload = CFileDownload::CreateInstance (NULL);
	}
	if	(pReload)
	{
		mFileDownload->SetReloadMode (pReload);
	}
	else
	{
		mFileDownload->SetResynchronizeMode (pRefresh);
	}
	mFileDownload->SetSecurityMode (pSecure);
}

HRESULT CAgentFileAcf::LoadAcf (CFileDownload* pDownload)
{
	HRESULT	lResult = E_FAIL;

#ifdef	_DEBUG_LOAD
	UINT lLogLevel = mLogLevel;
	mLogLevel = MinLogLevel (mLogLevel, _DEBUG_LOAD);
#endif
	Close ();

	if	(
			(pDownload)
		&&	(pDownload->IsDownloadComplete () == S_OK)
		)
	{
		lResult = LoadFile (CAtlString ((BSTR) pDownload->GetCacheName()));
		if	(SUCCEEDED (lResult))
		{
			mPath = ParseFilePath (CAtlString ((BSTR) pDownload->GetURL ()));
		}
	}
#ifdef	_DEBUG_LOAD
	mLogLevel = lLogLevel;
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#endif	// __cplusplus_cli
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
bool CAgentFileAcf::IsAnimationLoaded (int pAnimationNdx)
#else
bool CAgentFileAcf::IsAnimationLoaded (INT_PTR pAnimationNdx)
#endif
{
	bool				lRet = false;
#ifdef	__cplusplus_cli
	AcfFileAnimation^	lAnimation;

	if	(
			(IsOpen)
		&&	(lAnimation = GetAcfAnimation (pAnimationNdx))
		)
	{
		lRet = lAnimation->IsLoaded;
	}
#else
	CAgentFileAnimation*	lAnimation = NULL;

	if	(
			(IsOpen)
		&&	(pAnimationNdx >= 0)
		&&	(pAnimationNdx < (INT_PTR)mGestures.mAnimations.GetCount())
		&&	(lAnimation = mGestures.mAnimations [pAnimationNdx])
		&&	(lAnimation->mFileCheckSum == (DWORD)-1)
		)
	{
		lRet = true;
	}
#endif	
	return lRet;
}

#ifdef	__cplusplus_cli
bool CAgentFileAcf::LoadAnimationAca (int pAnimationNdx)
#else
HRESULT CAgentFileAcf::LoadAnimationAca (INT_PTR pAnimationNdx, CFileDownload* pDownload)
#endif
{
#ifdef	__cplusplus_cli
	bool				lRet = false;
	AcfFileAnimation^	lAnimation;
	
	if	(
			(IsOpen)
		&&	(lAnimation = GetAcfAnimation (pAnimationNdx))
		)
	{
		if	(lAnimation->IsLoaded)
		{
			lRet = true;
		}
		else
		if	(ReadAcaFile (lAnimation))
		{
			lRet = true;
		}
	}
	return lRet;
#else	
	HRESULT					lResult = S_OK;
	CAgentFileAnimation*	lAnimation = NULL;

	if	(!pDownload)
	{
		lResult = E_POINTER;
	}
	else
	if	(!IsOpen)
	{
		lResult = AGENTPROVERROR_CHARACTERINVALID;
	}
	else
	if	(
			(pAnimationNdx >= 0)
		&&	(pAnimationNdx < (INT_PTR)mGestures.mAnimations.GetCount())
		&&	(lAnimation = mGestures.mAnimations [pAnimationNdx])
		)
	{
		if	(GetAcaPath (lAnimation).CompareNoCase (CAtlString ((BSTR) pDownload->GetURL ())) != 0)
		{
			lResult = AGENTERR_INVALIDANIMATION;
		}
		else
		if	(lAnimation->mFileCheckSum == (DWORD)-1)
		{
			lResult = S_FALSE;
		}
		else
		{
			lResult = ReadAcaFile (lAnimation, CAtlString ((BSTR) pDownload->GetCacheName()));
			lAnimation->mFileCheckSum = (DWORD)-1;
		}
	}
	else
	{
		lResult = AGENTERR_ANIMATIONNOTFOUND;
	}
	return lResult;
#endif	
}

#ifdef	__cplusplus_cli
System::String^ CAgentFileAcf::GetAnimationAcaPath (int pAnimationNdx)
{
	String^				lPath;
	AcfFileAnimation^	lAnimation;

	if	(
			(IsOpen)
		&&	(lAnimation = GetAcfAnimation (pAnimationNdx))
		)
	{
		lPath = GetAcaPath (lAnimation);
	}
	return lPath;
}
#else
tBstrPtr CAgentFileAcf::GetAnimationAcaPath (INT_PTR pAnimationNdx)
{
	CAtlString				lPath;
	CAgentFileAnimation*	lAnimation = NULL;

	if	(
			(IsOpen)
		&&	(pAnimationNdx >= 0)
		&&	(pAnimationNdx < (INT_PTR)mGestures.mAnimations.GetCount())
		&&	(lAnimation = mGestures.mAnimations [pAnimationNdx])
		)
	{
		lPath = GetAcaPath (lAnimation);
	}
	return lPath.AllocSysString();
}
#endif	

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
bool CAgentFileAcf::IsAnimationLoaded (System::String^ pAnimationName)
{
	if	(mGestures->Contains (pAnimationName))
	{
		return IsAnimationLoaded (mGestures->IndexOf (mGestures [pAnimationName]));
	}
	return false;
}
#else
bool CAgentFileAcf::IsAnimationLoaded (LPCTSTR pAnimationName)
{
	return IsAnimationLoaded (FindAnimation (pAnimationName));
}
#endif

#ifdef	__cplusplus_cli
bool CAgentFileAcf::LoadAnimationAca (System::String^ pAnimationName)
{
	if	(mGestures->Contains (pAnimationName))
	{
		return LoadAnimationAca (mGestures->IndexOf (mGestures [pAnimationName]));
	}
	return false;
}
#else
HRESULT CAgentFileAcf::LoadAnimationAca (LPCTSTR pAnimationName, CFileDownload* pDownload)
{
	return LoadAnimationAca (FindAnimation (pAnimationName), pDownload);
}
#endif

#ifdef	__cplusplus_cli
System::String^ CAgentFileAcf::GetAnimationAcaPath (System::String^ pAnimationName)
{
	if	(mGestures->Contains (pAnimationName))
	{
		return GetAnimationAcaPath (mGestures->IndexOf (mGestures [pAnimationName]));
	}
	return nullptr;
}
#else
tBstrPtr CAgentFileAcf::GetAnimationAcaPath (LPCTSTR pAnimationName)
{
	return GetAnimationAcaPath (FindAnimation (pAnimationName));
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
AcfFileAnimation^ CAgentFileAcf::GetAcfAnimation (int pAnimationNdx)
{
	AcfFileAnimation^	lAnimation;

	if	(
			(IsOpen)
		&&	(pAnimationNdx >= 0)
		&&	(pAnimationNdx < mGestures->Count)
		&&	(AcfFileAnimation::typeid->IsAssignableFrom (mGestures [pAnimationNdx]->GetType()))
		)
	{
		lAnimation = safe_cast<AcfFileAnimation^> (mGestures [pAnimationNdx]);
	}
	return lAnimation;
}
#else
const CAgentFileAnimation* CAgentFileAcf::GetGesture (INT_PTR pGestureNdx)
{
	CAgentFileAnimation*	lRet = const_cast <CAgentFileAnimation*> (__super::GetGesture (pGestureNdx));

	if	(lRet)
	{
		ReadAcaFile (lRet);
	}
	return lRet;
}

const CAgentFileAnimation* CAgentFileAcf::GetAnimation (INT_PTR pAnimationNdx)
{
	CAgentFileAnimation*	lRet = const_cast <CAgentFileAnimation*> (__super::GetAnimation (pAnimationNdx));

	if	(lRet)
	{
		ReadAcaFile (lRet);
	}
	return lRet;
}
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
int CAgentFileAcf::ImageCount::get ()
{
	return mAcaImages->Count;
}
#else
INT_PTR CAgentFileAcf::GetImageCount () const
{
	return mAcaImages.GetCount();
}
#endif

#ifdef	__cplusplus_cli
CAgentFileImage^ CAgentFileAcf::GetImage (int pImageNdx, bool p32Bit, System::Drawing::Color pBkColor)
{
	CAgentFileImage^	lImage;
	CAgentFileImage^	lAcaImage;

	if	(
			(pImageNdx >= 0)
		&&	(pImageNdx < mAcaImages->Count)
		&&	(lAcaImage = mAcaImages [pImageNdx])
		)
	{
		if	(p32Bit)
		{
			lImage = Get32BitImage (lAcaImage, pBkColor);
		}
		else
		if	(lImage = gcnew CAgentFileImage)
		{
			lImage->mImageNum = lAcaImage->mImageNum;
			lImage->mImageSize = lAcaImage->mImageSize;
			if	(
					(lAcaImage->mBits)
				&&	(lImage->mBits = gcnew array<BYTE> (lAcaImage->mBits->Length))
				)
			{
				Array::Copy (lAcaImage->mBits, lImage->mBits, lAcaImage->mBits->Length);
				
				if	(p32Bit)
				{
				}
			}
		}
	}
	return lImage;
}
#else
CAgentFileImage* CAgentFileAcf::GetImage (INT_PTR pImageNdx, bool p32Bit, const COLORREF* pBkColor)
{
	tPtr <CAgentFileImage>	lImage;
	const CAgentFileImage*	lAcaImage;

	if	(
			(pImageNdx >= 0)
		&&	(pImageNdx < (INT_PTR)mAcaImages.GetCount())
		&&	(lAcaImage = mAcaImages [pImageNdx])
		)
	{
		if	(p32Bit)
		{
			lImage = Get32BitImage (lAcaImage, pBkColor);
		}
		else
		if	(
				(lImage = new CAgentFileImage)
			&&	(lImage->mBits = new BYTE [lAcaImage->mBitsSize])
			)
		{
			lImage->mImageNum = lAcaImage->mImageNum;
			lImage->mImageSize = lAcaImage->mImageSize;
			lImage->mBitsSize = lAcaImage->mBitsSize;
			memcpy (lImage->mBits.Ptr(), lAcaImage->mBits.Ptr(), lImage->mBitsSize);
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
int CAgentFileAcf::SoundCount::get ()
{
	return mAcaSounds->Count;
}
#else
INT_PTR CAgentFileAcf::GetSoundCount () const
{
	return mAcaSounds.GetCount();
}
#endif

#ifdef	__cplusplus_cli
int CAgentFileAcf::GetSoundSize (int pSoundNdx)
{
	long	lRet = -1;

	if	(
			(pSoundNdx >= 0)
		&&	(pSoundNdx < mAcaSounds->Count)
		)
	{
		lRet = mAcaSounds[pSoundNdx]->Length;
	}
	return lRet;
}
#else
long CAgentFileAcf::GetSoundSize (INT_PTR pSoundNdx)
{
	long			lRet = -1;
	CAtlByteArray *	lAcaSound;

	if	(lAcaSound = mAcaSounds (pSoundNdx))
	{
		lRet = (long)lAcaSound->GetCount();
	}
	return lRet;
}
#endif

#ifdef	__cplusplus_cli
array <BYTE>^ CAgentFileAcf::GetSound (int pSoundNdx)
{
	if	(
			(pSoundNdx >= 0)
		&&	(pSoundNdx < mAcaSounds->Count)
		)
	{
		return mAcaSounds [pSoundNdx];
	}
	return nullptr;
}
#else
LPCVOID CAgentFileAcf::GetSound (INT_PTR pSoundNdx)
{
	LPCVOID			lSound = NULL;
	CAtlByteArray *	lAcaSound;

	if	(lAcaSound = mAcaSounds (pSoundNdx))
	{
		lSound = lAcaSound->GetData();
	}
	return lSound;
}
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
bool CAgentFileAcf::ReadHeader ()
#else
HRESULT CAgentFileAcf::ReadHeader ()
#endif
{
#ifdef	__cplusplus_cli
	bool	lRet = __super::ReadHeader ();
#else
	HRESULT	lResult = __super::ReadHeader ();
#endif

	if	(
#ifdef	__cplusplus_cli
			(lRet)
#else
			(SUCCEEDED (lResult))
#endif
		&&	(
				(mSignature != sAcfFileSignature)
			||	(!ReadAcfHeader ())
			)
		)
	{
#ifdef	__cplusplus_cli
		throw gcnew Exception ("The specified file is not a Microsoft Agent 2.x character file.");
#else
		lResult = AGENTPROVERROR_MAGIC;
#endif
	}

#ifdef	__cplusplus_cli
	return lRet;
#else
	return lResult;
#endif
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentFileAcf::ReadAcfHeader ()
{
	bool				lRet = false;

#ifdef	__cplusplus_cli
	UInt32				lUncompressedSize;
	UInt32				lCompressedSize;
	array <BYTE>^		lHeaderBuffer;
	pin_ptr <BYTE>		lHeaderData;
	array <BYTE>^		lCompressedBuffer;
	pin_ptr <BYTE>		lCompressedData;
#else
	DWORD				lUncompressedSize;
	DWORD				lCompressedSize;
	LPCBYTE				lHeaderData = NULL;
	tArrayPtr <BYTE>	lUncompressedData;
#endif	

	FreeNames ();
	FreeGestures ();
	FreeStates ();

	if	(
			(IsOpen)
		&&	(IsAcfFile)
		)
	{
#ifdef	__cplusplus_cli
		mFileStream->Seek (sizeof(DWORD), SeekOrigin::Begin);
		lUncompressedSize = mFileReader->ReadUInt32 ();
		lCompressedSize = mFileReader->ReadUInt32 ();
#else
		lUncompressedSize = ((LPCDWORD)(LPVOID)mFileView) [1];
		lCompressedSize = ((LPCDWORD)(LPVOID)mFileView) [2];
#endif		

		if	(LogIsActive (mLogLevel))
		{
			LogMessage (mLogLevel, _T("  [%s] UncompressedSize [%u] CompressedSize [%u]"), _B(mPath), lUncompressedSize, lCompressedSize);
		}

		if	(lCompressedSize == 0)
		{
#ifdef	__cplusplus_cli
			lHeaderBuffer = mFileReader->ReadBytes (lUncompressedSize);
			lHeaderData = &lHeaderBuffer[0];
#else
			lHeaderData = ((LPCBYTE)(LPVOID)mFileView) + sizeof(DWORD)*3;
#endif			
		}
		else
		{
#ifdef	__cplusplus_cli
			lCompressedBuffer = mFileReader->ReadBytes (lCompressedSize);
			lCompressedData = &lCompressedBuffer[0]; 
			lHeaderBuffer = gcnew array <BYTE> (lUncompressedSize);
			lHeaderData = &lHeaderBuffer[0];
			
			if	(DecodeData (lCompressedData, lCompressedSize, lHeaderData, lUncompressedSize) != lUncompressedSize)
			{
				lHeaderData = nullptr;
			}
#else
			if	(
					(lUncompressedData = new BYTE [lUncompressedSize])
				&&	(DecodeData (((LPCBYTE)(LPVOID)mFileView) + sizeof(DWORD)*3, lCompressedSize, (LPBYTE)lUncompressedData, lUncompressedSize) == lUncompressedSize)
				)
			{
				lHeaderData = lUncompressedData;
			}
#endif			
		}

		if	(
				(lHeaderData)
			&&	(ReadBufferHeader (lHeaderData, lUncompressedSize))
			)
		{
			lRet = true;
		}

#ifndef	__cplusplus_cli
		if	(LogIsActive (mLogLevel))
		{
			Log (mLogLevel);
			LogNames (mLogLevel);
			LogStates (mLogLevel);
			LogGestures (mLogLevel);
		}
#endif		
	}
	return lRet;
}

LPCVOID CAgentFileAcf::ReadBufferHeader (LPCVOID pBuffer, DWORD pBufferSize)
{
	LPCBYTE	lByte = (LPCBYTE)pBuffer;

	try
	{
		WORD						lAnimationCount;
#ifdef	__cplusplus_cli
		AcfFileAnimation^			lAnimation;
#else
		tPtr <CAgentFileAnimation>	lAnimation;
#endif		

		mVersionMinor = *(LPCWORD)lByte;
		lByte += sizeof (WORD);
		mVersionMajor = *(LPCWORD)lByte;
		lByte += sizeof (WORD);
		lAnimationCount = *(LPWORD)lByte;
		lByte += sizeof (WORD);

		while (lAnimationCount > 0)
		{
#ifdef	__cplusplus_cli
			lAnimation = gcnew AcfFileAnimation (this, mGestures);
#else
			lAnimation = new CAgentFileAnimation;
#endif			

			lByte = (LPCBYTE)ReadBufferString (lByte, false, lAnimation->mName);
			lByte = (LPCBYTE)ReadBufferString (lByte, false, lAnimation->mFileName);
			lByte = (LPCBYTE)ReadBufferString (lByte, false, lAnimation->mReturnName);
			lAnimation->mFileCheckSum = *(LPDWORD)lByte;
			lByte += sizeof(DWORD);

#ifdef	__cplusplus_cli
			mGestures->Add (lAnimation);
#else			
			mGestures.mAnimations.InsertAt (AddSortedString (mGestures.mNames, CAtlString ((BSTR)lAnimation->mName)), lAnimation);
			lAnimation.Detach ();
#endif			
			lAnimationCount--;
		}

#ifdef	__cplusplus_cli
		mHeader->mGuid = System::Guid (*(LPCDWORD)lByte, *(LPCWORD)(lByte+4), *(LPCWORD)(lByte+6), lByte[8], lByte[9], lByte[10], lByte[11], lByte[12], lByte[13], lByte[14], lByte[15]);
#else
		mHeader.mGuid = *(LPCGUID)lByte;
#endif		
		lByte += sizeof (GUID);
		lByte = (LPCBYTE)ReadBufferNames (lByte, 0, false, true);

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
		mHeader->mStyle = *(LPCDWORD)lByte;
#else
		mHeader.mStyle = *(LPCDWORD)lByte;
#endif
		lByte += sizeof(DWORD);
		lByte += sizeof(DWORD); // Unknown - always 0x0000002

#ifdef	__cplusplus_cli
		if	(mHeader->mStyle & (DWORD)CharacterStyle::Tts)
#else
		if	(mHeader.mStyle & CharStyleTts)
#endif		
		{
			lByte = (LPCBYTE)ReadBufferTts (lByte, false);
		}
#ifdef	__cplusplus_cli
		if	(mHeader->mStyle & (DWORD)CharacterStyle::Balloon)
#else
		if	(mHeader.mStyle & CharStyleBalloon)
#endif		
		{
			lByte = (LPCBYTE)ReadBufferBalloon (lByte, false);
		}
		lByte = (LPCBYTE)ReadBufferPalette (lByte);
		lByte = (LPCBYTE)ReadBufferIcon (lByte);
		lByte = (LPCBYTE)ReadBufferStates (lByte, pBufferSize-(DWORD)(lByte-(LPBYTE)pBuffer), false);

		return lByte;
	}
	catch AnyExceptionDebug
	
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
System::String^ CAgentFileAcf::GetAcaPath (AcfFileAnimation^ pAnimation)
{
	if	(pAnimation)
	{
		return ParseRelativePath (pAnimation->mFileName);
	}
	return nullptr;
}
#else
CAtlString CAgentFileAcf::GetAcaPath (CAgentFileAnimation* pAnimation)
{
	if	(pAnimation)
	{
		return CAtlString (ParseRelativePath (CAtlString(pAnimation->mFileName)));
	}
	return CAtlString();
}
#endif

#ifdef	__cplusplus_cli
bool CAgentFileAcf::ReadAcaFile (ref class AcfFileAnimation^ pAnimation)
{
	bool	lRet = false;
	
	if	(pAnimation)
	{
		if	(
				(!pAnimation->IsLoaded)
			&&	(!String::IsNullOrEmpty (pAnimation->mFileName))
			)
		{
			lRet = ReadAcaFile (pAnimation, GetAcaPath (pAnimation));
		}
		pAnimation->IsLoaded = true;
	}
	return lRet;
}
#else
HRESULT CAgentFileAcf::ReadAcaFile (CAgentFileAnimation* pAnimation)
{
	HRESULT	lResult = S_FALSE;

#ifdef	_DEBUG_LOAD
	UINT lLogLevel = mLogLevel;
	mLogLevel = MinLogLevel (mLogLevel, _DEBUG_LOAD);
#endif

	if	(
			(pAnimation)
		&&	(pAnimation->mFileCheckSum != (DWORD)-1)
		&&	(pAnimation->mFileName.Ptr ())
		)
	{
		try
		{
			CAtlString	lPath = GetAcaPath (pAnimation);

			if	(PathIsURL (mPath))
			{
				tPtr <CFileDownload>	lOldDownload = mFileDownload.Detach();

				mFileDownload = CFileDownload::CreateInstance (lPath);
				if	(lOldDownload)
				{
					mFileDownload->SetBindFlags (lOldDownload->GetBindFlags());
				}
				SafeFreeSafePtr (lOldDownload);

				if	(SUCCEEDED (lResult = mFileDownload->Download ()))
				{
					lResult = ReadAcaFile (pAnimation, CAtlString ((BSTR) mFileDownload->GetCacheName()));
				}
			}
			else
			{
				lResult = ReadAcaFile (pAnimation, lPath);
			}
		}
		catch AnyExceptionDebug
	}

	if	(pAnimation)
	{
		pAnimation->mFileCheckSum = (DWORD)-1;
	}
#ifdef	_DEBUG_LOAD
	mLogLevel = lLogLevel;
#endif
	return lResult;
}
#endif

#ifdef	__cplusplus_cli
bool CAgentFileAcf::ReadAcaFile (ref class AcfFileAnimation^ pAnimation, System::String^ pPath)
#else
HRESULT CAgentFileAcf::ReadAcaFile (CAgentFileAnimation* pAnimation, LPCTSTR pPath)
#endif
{
#ifdef	__cplusplus_cli
	bool	lRet = false;
#else
	HRESULT	lResult = S_FALSE;
#endif

#ifdef	_DEBUG_LOAD
	UINT lLogLevel = mLogLevel;
	mLogLevel = MinLogLevel (mLogLevel, _DEBUG_LOAD);
#endif
	
	try
	{
#ifdef	__cplusplus_cli
		System::IO::FileStream^		lFileStream;
		System::IO::BinaryReader^	lFileReader;
		UInt32						lFileSize;
#else
		CFileHandle					lFileHandle;
		CGenericHandle				lFileMapping;
		CMappedHandle				lFileView;
		DWORD						lFileSize;
#endif		

#ifdef	__cplusplus_cli
		try
		{
			lFileStream = gcnew FileStream (pPath, FileMode::Open, FileAccess::Read);
		}
		catch (Exception^ pException)
		{
			__LogCliException (LogIfActive);
		}
#else
		lFileHandle = CreateFile (pPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
#endif		

#ifdef	__cplusplus_cli
		if	(
				(lFileStream)
			&&	(lFileSize = (UInt32)lFileStream->Length)
			&&	(lFileReader = gcnew BinaryReader (lFileStream))
			)
#else
		if	(
				(lFileHandle.SafeIsValid ())
			&&	(lFileSize = GetFileSize (lFileHandle, NULL))
			&&	((lFileMapping = CreateFileMapping (lFileHandle, NULL, PAGE_READONLY, 0, 0, NULL)).SafeIsValid())
			&&	((lFileView = MapViewOfFile (lFileMapping, FILE_MAP_READ, 0, 0, lFileSize)).SafeIsValid())
			)
#endif			
		{
			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel, _T(""));
				LogMessage (mLogLevel, _T("Opened [%s] [%s] [%s] [%u]"), _B(pAnimation->mName), _B(pAnimation->mFileName), _B(pPath), lFileSize);
			}

			try
			{
#ifdef	__cplusplus_cli
				array <BYTE>^		lFileBuffer;
				pin_ptr <BYTE>		lFileData;
				UInt32				lUncompressedSize;
				UInt32				lCompressedSize;
				array <BYTE>^		lUncompressedBuffer;
				pin_ptr <BYTE>		lUncompressedData;
#else
				LPCBYTE				lFileData;
				DWORD				lUncompressedSize;
				DWORD				lCompressedSize;
				tArrayPtr <BYTE>	lUncompressedData;
#endif				
				LPCBYTE				lAnimationData = NULL;
				LPCBYTE				lByte;
				DWORD				lVersion;
				DWORD				lChksum;

#ifdef	__cplusplus_cli
				lFileBuffer = lFileReader->ReadBytes (lFileSize);
				lFileData = &lFileBuffer[0];
				lByte = lFileData;
#else
				lByte = lFileData = (LPCBYTE)(LPVOID)lFileView;
#endif				

				lVersion = MAKELONG (((LPCWORD)lByte) [0], ((LPCWORD)lByte) [1]);
				lByte += sizeof(WORD) * 2;
				lChksum = *(LPDWORD)lByte;
				lByte += sizeof(DWORD);

				if	(
						(HIWORD (lVersion) >= 2)
					&&	(lChksum == pAnimation->mFileCheckSum)
					)
				{
#ifdef	__cplusplus_cli
					pAnimation->IsLoaded = true;
#endif
					if	(*lByte)
					{
						lByte++;
						lUncompressedSize = *(LPDWORD)lByte;
						lByte += sizeof(DWORD);
						lCompressedSize = *(LPDWORD)lByte;
						lByte += sizeof(DWORD);

						if	(
								(lCompressedSize <= lFileSize - (lByte - (LPBYTE)lFileData))
#ifdef	__cplusplus_cli
							&&	(lUncompressedBuffer = gcnew array <BYTE> (lUncompressedSize))
							&&	(lUncompressedData = &lUncompressedBuffer[0])
#else
							&&	(lUncompressedData = new BYTE [lUncompressedSize])
#endif							
							&&	(DecodeData (lByte, lCompressedSize, (LPBYTE)lUncompressedData, lUncompressedSize) == lUncompressedSize)
							)
						{
							lAnimationData = lUncompressedData;
						}
					}
					else
					{
						lByte++;
						lAnimationData = lByte;
						lUncompressedSize = lFileSize - (DWORD)(lByte - (LPBYTE)lFileData);
					}
				}

				if	(lByte = lAnimationData)
				{
#ifdef	__cplusplus_cli
					Int32	lFirstSound = (Int32)mAcaSounds->Count;
					Int32	lFirstImage = (Int32)mAcaImages->Count;
#else
					long	lFirstSound = (long)mAcaSounds.GetCount();
					long	lFirstImage = (long)mAcaImages.GetCount();
#endif					
					
					if	(
							(lByte = (LPCBYTE)ReadBufferSounds (pAnimation, lByte, lUncompressedSize-(DWORD)(lByte-lAnimationData)))
						&&	(lByte = (LPCBYTE)ReadBufferImages (pAnimation, lByte, lUncompressedSize-(DWORD)(lByte-lAnimationData)))
						)
					{
						pAnimation->mReturnType = *lByte;
						lByte += sizeof(BYTE);
						
						lByte = (LPCBYTE)ReadBufferFrames (pAnimation, lByte, lUncompressedSize-(DWORD)(lByte-lAnimationData), lFirstImage, lFirstSound);
					}

#ifdef	_DEBUG_LOAD
					if	((DWORD)(lByte-lAnimationData) < lUncompressedSize)
					{
						LogMessage (_DEBUG_LOAD, _T("Leftover [%u]"), lUncompressedSize-(DWORD)(lByte-lAnimationData));
						LogDump (_DEBUG_LOAD, (LPBYTE)lByte, lUncompressedSize-(DWORD)(lByte-lAnimationData));
					}
#endif
				}
			}
			catch AnyExceptionDebug
		}
#ifndef	__cplusplus_cli
		else
		{
			lResult = HRESULT_FROM_WIN32 (GetLastError());
			if	(SUCCEEDED (lResult))
			{
				lResult = E_FAIL;
			}
			if	(LogIsActive ())
			{
				LogWinErr (LogNormal|LogTime, GetLastError(), _T("Open [%s]"), pPath);
			}
		}
#endif
		
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

#ifdef	_DEBUG_LOAD
	mLogLevel = lLogLevel;
#endif
#ifdef	__cplusplus_cli
	return lRet;
#else
	return lResult;
#endif	
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
LPCVOID CAgentFileAcf::ReadBufferFrames (CAgentFileAnimation^ pAnimation, LPCVOID pBuffer, DWORD pBufferSize, Int32 pFirstImage, Int32 pFirstSound)
#else
LPCVOID CAgentFileAcf::ReadBufferFrames (CAgentFileAnimation* pAnimation, LPCVOID pBuffer, DWORD pBufferSize, long pFirstImage, long pFirstSound)
#endif
{
	try
	{
		LPCBYTE	lByte = (LPCBYTE)pBuffer;
		WORD	lFrameCount;
		WORD	lFrameNum;

		//LogDump (LogDebugFast, (LPBYTE)pBuffer, pBufferSize);

		lFrameCount = *(LPCWORD)lByte;
		lByte += sizeof(WORD);
#ifdef	_DEBUG_ANIMATION
		if	(LogIsActive (MaxLogLevel (mLogLevel, _DEBUG_ANIMATION)))
		{
			LogMessage (_DEBUG_ANIMATION, _T("[%ls] Aca Frames [%hu]"), (BSTR)pAnimation->mName, lFrameCount);
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
				BYTE				lBranchCount;
				BYTE				lOverlayCount;
				BYTE				lOverlayReplace;
				INT_PTR				lNdx;

				//LogDump (LogDebugFast, (LPBYTE)lByte, sizeof(WORD)*7);

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
				DWORD lSomething = *(LPCDWORD)lByte;
				lByte += sizeof(DWORD);
				lFrame->mExitFrame = *(LPCWORD)lByte;
				lByte += sizeof(WORD);

#ifdef	_DEBUG_ANIMATION
				if	(LogIsActive (MaxLogLevel (mLogLevel, _DEBUG_ANIMATION)))
				{
					LogMessage (_DEBUG_ANIMATION, _T("  Frame [%u] Image [%hu] Duration [%hu] ExitFrame [%hd] Sound [%2hd] Something [%8.8X]"), lFrameNum, lFrame->mImages [0].mImageNdx, lFrame->mDuration, lFrame->mExitFrame, lFrame->mSoundNdx, lSomething);
				}
#endif
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
				lByte += sizeof(WORD)*lBranchCount*2;

#ifdef	_DEBUG_ANIMATION
				if	(LogIsActive (MaxLogLevel (mLogLevel, _DEBUG_ANIMATION)))
				{
					for	(lNdx = 0; lNdx < min ((INT_PTR)lBranchCount, 3); lNdx++)
					{
						LogMessage (_DEBUG_ANIMATION, _T("    Branch [%d] [%u]"), lFrame->mBranching [lNdx].mFrameNdx, lFrame->mBranching [lNdx].mProbability);
					}
				}
#endif
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
					lOverlayReplace = *lByte;
					lByte += sizeof(BYTE);
#ifdef	_DEBUG_ANIMATION
					if	(LogIsActive (MaxLogLevel (mLogLevel, _DEBUG_ANIMATION)))
					{
						LogMessage (_DEBUG_ANIMATION, _T("  Overlays [%hu] Replace [%2.2X]"), lOverlayCount, lOverlayReplace);
					}
#endif
					if	(lOverlayReplace)
					{
						LogMessage (LogDebug, _T("  Unknown [%u]"), *(LPCDWORD)lByte);
						lByte += sizeof (DWORD);
					}

					for	(lNdx = 0; lNdx < (INT_PTR)lOverlayCount; lNdx++)
					{
#ifdef	__cplusplus_cli
						CAgentFileFrameOverlay^	lOverlay = gcnew CAgentFileFrameOverlay (this, lFrame);
						CAgentFileImage^		lOverlayImage = gcnew CAgentFileImage;
						pin_ptr <BYTE>			lImageBits;
#else
						CAgentFileFrameOverlay*	lOverlay = &lFrame->mOverlays [lNdx];
						tPtr <CAgentFileImage>	lOverlayImage = new CAgentFileImage;
#endif						
						DWORD					lOverlaySize;

						lOverlay->mOverlayType = (AgentMouthOverlay)*lByte;
						lByte += sizeof(BYTE);
						lOverlaySize = *(LPCDWORD)lByte;
						lByte += sizeof(DWORD);
						lByte += sizeof(BYTE); // Unknown
						lOverlay->mReplaceFlag = (*lByte) ? true : false;
						lByte += sizeof(BYTE);
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
							lOverlayImage->mImageNum = (UInt32)mAcaImages->Count;
							lOverlay->mImageNdx = lOverlayImage->mImageNum;
							mAcaImages->Add (lOverlayImage);
#else							
							memcpy (lOverlayImage->mBits.Ptr(), lByte, lOverlaySize);
							lOverlayImage->mBitsSize = lOverlaySize;
							lOverlayImage->mImageNum = (long)mAcaImages.GetCount();
							lOverlay->mImageNdx = lOverlayImage->mImageNum;
							mAcaImages.Add (lOverlayImage.Detach ());
#endif							
						}
						lByte += lOverlaySize;
//#ifdef	__cplusplus_cli
//						LogMessage (LogDebugFast, _T("[%s] [%s] Frame [%u] Overlay [%u] [%u] [%u] - Image [%d] Offset [%d %d] Size [%d %d]"), _B(mPath), _B(pAnimation->mName), lFrameNum, lNdx, lOverlay->mOverlayType, lOverlaySize, lOverlay->mImageNdx, lOverlay->mOffset.X, lOverlay->mOffset.Y, lOverlayImage->mImageSize.Width, lOverlayImage->mImageSize.Height);
//#endif							

						if	(lOverlay->mReplaceFlag)
						{
							DWORD lRgnSize = *(LPCDWORD)lByte;
							lByte += sizeof(DWORD);
#ifdef	_DEBUG_LOAD_NOT
							LogMessage (_DEBUG_LOAD, _T("    Region [%hu]"), lRgnSize);
							LogDump (_DEBUG_LOAD, (LPBYTE)lByte, lRgnSize);
#endif
							lByte += lRgnSize;
						}
#ifdef	_DEBUG_ANIMATION
						if	(LogIsActive (MaxLogLevel (mLogLevel, _DEBUG_ANIMATION)))
						{
							LogMessage (_DEBUG_ANIMATION, _T("    Overlay [%hu] Replace [%u] Image [%u] [%d] [%d] of [%4u] at [%d %d]"), lOverlay->OverlayType, lOverlay->ReplaceFlag, lOverlay->ImageNdx, mAcaImages[(long)lOverlay->ImageNdx]->ImageSize.cx, mAcaImages[(long)lOverlay->ImageNdx]->ImageSize.cy, mAcaImages[(long)lOverlay->ImageNdx]->BitsSize, lOverlay->Offset.x, lOverlay->Offset.y);
						}
#endif
					}
				}

				if	(lByte > (LPCBYTE)pBuffer + pBufferSize)
				{
#ifdef	_DEBUG
					LogMessage (LogDebug, _T("!!! Screwed !!!"));
#endif
					break;
				}
			}
		}

		return lByte;
	}
	catch AnyExceptionDebug

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
LPCVOID CAgentFileAcf::ReadBufferImages (CAgentFileAnimation^ pAnimation, LPCVOID pBuffer, DWORD pBufferSize)
#else
LPCVOID CAgentFileAcf::ReadBufferImages (CAgentFileAnimation* pAnimation, LPCVOID pBuffer, DWORD pBufferSize)
#endif
{
	try
	{
		LPCBYTE					lByte = (LPCBYTE)pBuffer;
		DWORD					lByteCount;
		WORD					lImageCount;
		WORD					lImageNum;
#ifdef	__cplusplus_cli
		CAgentFileImage^		lImage;
		pin_ptr <BYTE>			lImageBits;
#else
		tPtr <CAgentFileImage>	lImage;
#endif

		lImageCount = *(LPCWORD)lByte;
		lByte += sizeof(WORD);

#ifdef	_DEBUG_LOAD
		if	(LogIsActive (mLogLevel))
		{
			LogMessage (mLogLevel|LogHighVolume, _T("[%ls] Aca Images [%hu] Starting [%u]"), (BSTR)pAnimation->mName, lImageCount, mAcaImages.GetCount());
		}
#endif
		for	(lImageNum = 0; lImageNum < lImageCount; lImageNum++)
		{
			lByteCount = *(LPCDWORD)lByte;
			lByte += sizeof(DWORD);
			lByte += sizeof(BYTE); // Unknown

#ifdef	_DEBUG_LOAD
			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel|LogHighVolume, _T("  Image [%hu (%hu)] of [%hu]"), lImageNum, mAcaImages.GetCount(), lByteCount);
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
				lImage->mImageNum = (UInt32)mAcaImages->Count;
				mAcaImages->Add (lImage);
#else
				lImage->mBitsSize = lByteCount;
				memcpy (lImage->mBits, lByte, lByteCount);

				lImage->mImageSize = mHeader.ImageSize;
				lImage->mImageNum = (long)mAcaImages.GetCount();
				mAcaImages.Add (lImage.Detach ());
#endif				

#ifdef	_SAVE_IMAGE
				if	(LogIsActive (_SAVE_IMAGE))
				{
					SaveImage (lImage);
				}
#endif
#ifdef	_DUMP_IMAGE
				if	(LogIsActive (_DUMP_IMAGE))
				{
					DumpAcaImage ((INT_PTR)mAcaImages.GetCount()-1, _DUMP_IMAGE);
				}
#endif
			}
			else
			{
				break;
			}

			lByte += lByteCount;
			lByteCount = *(LPCDWORD)lByte;
			lByte += sizeof(DWORD);
#ifdef	_DEBUG_LOAD_NOT
			LogMessage (_DEBUG_LOAD, _T("  Region [%hu]"), lByteCount);
			LogDump (_DEBUG_LOAD, (LPBYTE)lByte, lByteCount);
#endif

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
LPCVOID CAgentFileAcf::ReadBufferSounds (CAgentFileAnimation^ pAnimation, LPCVOID pBuffer, DWORD pBufferSize)
#else
LPCVOID CAgentFileAcf::ReadBufferSounds (CAgentFileAnimation* pAnimation, LPCVOID pBuffer, DWORD pBufferSize)
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
		lByte += sizeof(WORD);

#ifdef	_DEBUG_LOAD
		if	(LogIsActive (mLogLevel))
		{
			LogMessage (mLogLevel|LogHighVolume, _T("[%ls] Aca Sounds [%hu] Starting [%u]"), (BSTR)pAnimation->mName, lSoundCount, mAcaSounds.GetCount());
		}
#endif
		for	(lSoundNum = 0; lSoundNum < lSoundCount; lSoundNum++)
		{
			lByteCount = *(LPCDWORD)lByte;
			lByte += sizeof(DWORD);

#ifdef	_DEBUG_LOAD
			if	(LogIsActive (mLogLevel))
			{
				LogMessage (mLogLevel|LogHighVolume, _T("  Sound [%hu (%hu)] of [%hu]"), lSoundNum, mAcaSounds.GetCount(), lByteCount);
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
				mAcaSounds->Add (lSound);
#else
				lSound->SetCount (lByteCount);
				memcpy (lSound->GetData(), lByte, lByteCount);
				mAcaSounds.Add (lSound.Detach());
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
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
/////////////////////////////////////////////////////////////////////////////

AcfFileAnimation::AcfFileAnimation (CharacterFile^ pOwner, FileGestures^ pContainer)
:	FileAnimation (pOwner, pContainer)
{
}

AcfFileAnimation::~AcfFileAnimation ()
{
}

void AcfFileAnimation::Empty ()
{
	__super::Empty ();
	mFileCheckSum = 0;
	mFileName = nullptr;
}

/////////////////////////////////////////////////////////////////////////////

UInt16 AcfFileAnimation::FrameCount::get()
{
	EnsureIsLoaded ();	
	return __super::get();
}

FileFrames^ AcfFileAnimation::Frames::get()
{
	EnsureIsLoaded ();	
	return __super::get();
}

Boolean AcfFileAnimation::CopyTo (FileAnimation^ pTarget, Boolean pDeepCopy)
{
	Boolean				lRet;
	AcfFileAnimation^	lTarget;

	EnsureIsLoaded ();	
	lRet = __super::CopyTo (pTarget, pDeepCopy);

	if	(
			(lRet)
		&&	(AcfFileAnimation::typeid->IsAssignableFrom (pTarget->GetType()))
		&&	(lTarget = safe_cast <AcfFileAnimation^> (pTarget))
		)
	{
		lTarget->mFileCheckSum = mFileCheckSum;
		lTarget->mFileName = mFileName ? gcnew String (mFileName) : nullptr;
	}
	return lRet;
}

System::String^ AcfFileAnimation::ToString()
{
	EnsureIsLoaded ();	
    return String::Format ("{0} [{1}]", __super::ToString(), mFileName);
}

/////////////////////////////////////////////////////////////////////////////

Boolean AcfFileAnimation::IsLoaded::get()
{
	return mFileCheckSum == (UInt32)-1; 
}

void AcfFileAnimation::IsLoaded::set (Boolean pValue)
{
	mFileCheckSum = pValue ? (UInt32)-1 : 0;
}

void AcfFileAnimation::EnsureIsLoaded ()
{
	if	(!IsLoaded)
	{
		try
		{
			CAgentFileAcf^	lOwner;
			
			if	(
					(
						(lOwner = safe_cast <CAgentFileAcf^> (mOwner))
					||	(
							(mContainer)
						&&	(lOwner = safe_cast <CAgentFileAcf^> (mContainer->mOwner))
						)
					)
				&&	(lOwner->mGestures)
				&&	(lOwner->mGestures->Contains (this))
				)
			{
				lOwner->ReadAcaFile (this);
			}
		}
		catch AnyExceptionDebug
	}
}

/////////////////////////////////////////////////////////////////////////////
#endif	__cplusplus_cli
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifndef	__cplusplus_cli
/////////////////////////////////////////////////////////////////////////////

void CAgentFileAcf::DumpAcaImages (UINT pLogLevel)
{
#ifdef	_DEBUG
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			INT_PTR	lImageNdx;

			LogMessage (pLogLevel, _T("[%s] Images [%d]"), mPath, mAcaImages.GetCount());

			for	(lImageNdx = 0; lImageNdx < (INT_PTR)mAcaImages.GetCount(); lImageNdx++)
			{
				DumpAcaImage (lImageNdx, pLogLevel);
			}
		}
		catch AnyExceptionSilent
	}
#endif
}

void CAgentFileAcf::DumpAcaImage (INT_PTR pImageNdx, UINT pLogLevel)
{
#ifdef	_DEBUG
	if	(
			(LogIsActive (pLogLevel))
		&&	(pImageNdx >= 0)
		&&	(pImageNdx < (INT_PTR)mAcaImages.GetCount())
		)
	{
		try
		{
			CAgentFileImage*	lImage;

			if	(lImage = mAcaImages [pImageNdx])
			{
				LogMessage (pLogLevel, _T("  [%.3d] Image [%.3u] [%d x %d] Bits [%u]"), pImageNdx, lImage->mImageNum, lImage->mImageSize.cx, lImage->mImageSize.cy, lImage->mBitsSize);
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
