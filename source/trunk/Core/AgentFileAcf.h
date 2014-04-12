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
#pragma once
#include "AgentFileBinary.h"
#ifndef	__cplusplus_cli
#include "FileDownload.h"
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
namespace DoubleAgent {
namespace Character {
#else
#pragma warning (push)
#pragma warning (disable:4251)
#endif
/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
#define CAgentFileAcf AcfFile
public ref class AcfFile : public CAgentFileBinary
#else
class CAgentFileAcf : public CAgentFileBinary
#endif
{
public:
	CAgentFileAcf ();
	_DACORE_IMPEXP virtual ~CAgentFileAcf ();
#ifdef	__cplusplus_cli
	static CAgentFileAcf^ CreateInstance ();
#else
	DECLARE_DLL_OBJECT_EX(CAgentFileAcf, _DACORE_IMPEXP)
	_DACORE_IMPEXP static CAgentFileAcf* CreateInstance ();
#endif

// Overrides
public:
#ifdef	__cplusplus_cli
	virtual property bool IsAcfFile {virtual bool get() override;}
	virtual bool Open (const System::String^ pPath) override;
	virtual bool Save (const System::String^ pPath, CAgentFile^ pSource) override;
	virtual void Close () override;
#else
	_DACORE_IMPEXP virtual bool get_IsAcfFile () const;
	_DACORE_IMPEXP virtual HRESULT Open (LPCTSTR pPath);
	_DACORE_IMPEXP virtual void Close ();
#endif

#ifndef	__cplusplus_cli
	_DACORE_IMPEXP virtual const CAgentFileAnimation* GetGesture (INT_PTR pGestureNdx);
	_DACORE_IMPEXP virtual const CAgentFileAnimation* GetAnimation (INT_PTR pAnimationNdx);
#endif	

#ifdef	__cplusplus_cli
	virtual property int ImageCount {virtual int get() override;}
	virtual CAgentFileImage^ GetImage (int pImageNdx, bool p32Bit, System::Drawing::Color pBkColor) override;
#else
	_DACORE_IMPEXP virtual INT_PTR GetImageCount () const;
	_DACORE_IMPEXP virtual CAgentFileImage* GetImage (INT_PTR pImageNdx, bool p32Bit = false, const COLORREF* pBkColor = NULL);
#endif

#ifdef	__cplusplus_cli
	virtual property int SoundCount {virtual int get() override;}
	virtual int GetSoundSize (int pSoundNdx) override;
	virtual array <BYTE>^ GetSound (int pSoundNdx) override;
#else
	_DACORE_IMPEXP virtual INT_PTR GetSoundCount () const;
	_DACORE_IMPEXP virtual long GetSoundSize (INT_PTR pSoundNdx);
	_DACORE_IMPEXP virtual LPCVOID GetSound (INT_PTR pSoundNdx);
#endif

// Operations
public:
#ifndef	__cplusplus_cli
	_DACORE_IMPEXP void SetDownloadMode (bool pRefresh = true, bool pReload = false, bool pSecure = true);
	_DACORE_IMPEXP HRESULT LoadAcf (CFileDownload* pDownload);
#endif

#ifdef	__cplusplus_cli
	bool IsAnimationLoaded (int pAnimationNdx);
	bool IsAnimationLoaded (System::String^ pAnimationName);
	bool LoadAnimationAca (int pAnimationNdx);
	bool LoadAnimationAca (System::String^ pAnimationName);
	System::String^ GetAnimationAcaPath (int pAnimationNdx);
	System::String^ GetAnimationAcaPath (System::String^ pAnimationName);
#else
	_DACORE_IMPEXP bool IsAnimationLoaded (INT_PTR pAnimationNdx);
	_DACORE_IMPEXP bool IsAnimationLoaded (LPCTSTR pAnimationName);
	_DACORE_IMPEXP HRESULT LoadAnimationAca (INT_PTR pAnimationNdx, CFileDownload* pDownload);
	_DACORE_IMPEXP HRESULT LoadAnimationAca (LPCTSTR pAnimationName, CFileDownload* pDownload);
	_DACORE_IMPEXP tBstrPtr GetAnimationAcaPath (INT_PTR pAnimationNdx);
	_DACORE_IMPEXP tBstrPtr GetAnimationAcaPath (LPCTSTR pAnimationName);
#endif

protected:
#ifdef	__cplusplus_cli
	virtual bool ReadHeader () override;
#else
	virtual HRESULT ReadHeader ();
#endif

// Implementation
protected:
	bool ReadAcfHeader ();
	LPCVOID ReadBufferHeader (LPCVOID pBuffer, DWORD pBufferSize);

#ifdef	__cplusplus_cli
internal:	
	ref class AcfFileAnimation^ GetAcfAnimation (int pAnimationNdx);
	System::String^ GetAcaPath (ref class AcfFileAnimation^ pAnimation);
	bool ReadAcaFile (ref class AcfFileAnimation^ pAnimation);
	bool ReadAcaFile (ref class AcfFileAnimation^ pAnimation, System::String^ pPath);
protected:	
	LPCVOID ReadBufferFrames (CAgentFileAnimation^ pAnimation, LPCVOID pBuffer, DWORD pBufferSize, Int32 pFirstImage, Int32 pFirstSound);
	LPCVOID ReadBufferImages (CAgentFileAnimation^ pAnimation, LPCVOID pBuffer, DWORD pBufferSize);
	LPCVOID ReadBufferSounds (CAgentFileAnimation^ pAnimation, LPCVOID pBuffer, DWORD pBufferSize);
#else
	CAtlString GetAcaPath (CAgentFileAnimation* pAnimation);
	HRESULT ReadAcaFile (CAgentFileAnimation* pAnimation);
	HRESULT ReadAcaFile (CAgentFileAnimation* pAnimation, LPCTSTR pPath);
	LPCVOID ReadBufferFrames (CAgentFileAnimation* pAnimation, LPCVOID pBuffer, DWORD pBufferSize, long pFirstImage, long pFirstSound);
	LPCVOID ReadBufferImages (CAgentFileAnimation* pAnimation, LPCVOID pBuffer, DWORD pBufferSize);
	LPCVOID ReadBufferSounds (CAgentFileAnimation* pAnimation, LPCVOID pBuffer, DWORD pBufferSize);
#endif

protected:
#ifndef	__cplusplus_cli
	void DumpAcaImages (UINT pLogLevel);
	void DumpAcaImage (INT_PTR pImageNdx, UINT pLogLevel);
#endif

protected:
#ifdef	__cplusplus_cli
	System::Collections::Generic::List <CAgentFileImage^>^	mAcaImages;
	System::Collections::Generic::List <array <BYTE>^>^		mAcaSounds;
#else
	tPtr <CFileDownload>									mFileDownload;
	CAtlOwnPtrArray <CAgentFileImage>						mAcaImages;
	CAtlOwnPtrArray <CAtlByteArray>							mAcaSounds;
#endif
};

/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
} // namespace Character
} // namespace DoubleAgent
#else
#pragma warning (pop)
#endif
