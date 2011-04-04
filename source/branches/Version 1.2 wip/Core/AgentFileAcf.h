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
#pragma once
#include "AgentFileBinary.h"
#include "FileDownload.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning (push)
#pragma warning (disable:4251)
/////////////////////////////////////////////////////////////////////////////

class CAgentFileAcf : public CAgentFileBinary
{
	DECLARE_DLL_OBJECT_EX(CAgentFileAcf, _DACORE_IMPEXP)
public:
	CAgentFileAcf ();
	_DACORE_IMPEXP virtual ~CAgentFileAcf ();
	_DACORE_IMPEXP static CAgentFileAcf* CreateInstance ();

// Overrides
public:
	_DACORE_IMPEXP virtual bool get_IsAcfFile () const;
	_DACORE_IMPEXP virtual HRESULT Open (LPCTSTR pPath, UINT pLogLevel = 15);
	_DACORE_IMPEXP virtual void Close ();

	_DACORE_IMPEXP virtual const CAgentFileAnimation* GetGesture (INT_PTR pGestureNdx);
	_DACORE_IMPEXP virtual const CAgentFileAnimation* GetAnimation (INT_PTR pAnimationNdx);

	_DACORE_IMPEXP virtual INT_PTR GetImageCount () const;
	_DACORE_IMPEXP virtual CAgentFileImage* GetImage (INT_PTR pImageNdx, bool p32Bit = false, UINT pLogLevel = 15);

	_DACORE_IMPEXP virtual INT_PTR GetSoundCount () const;
	_DACORE_IMPEXP virtual long GetSoundSize (INT_PTR pSoundNdx);
	_DACORE_IMPEXP virtual LPCVOID GetSound (INT_PTR pSoundNdx);

// Operations
public:
	_DACORE_IMPEXP void SetDownloadMode (bool pRefresh = true, bool pReload = false, bool pSecure = true);
	_DACORE_IMPEXP HRESULT LoadAcf (CFileDownload* pDownload, UINT pLogLevel = 15);

	_DACORE_IMPEXP bool IsAnimationLoaded (INT_PTR pAnimationNdx);
	_DACORE_IMPEXP bool IsAnimationLoaded (LPCTSTR pAnimationName);
	_DACORE_IMPEXP HRESULT LoadAnimationAca (INT_PTR pAnimationNdx, CFileDownload* pDownload);
	_DACORE_IMPEXP HRESULT LoadAnimationAca (LPCTSTR pAnimationName, CFileDownload* pDownload);
	_DACORE_IMPEXP tBstrPtr GetAnimationAcaPath (INT_PTR pAnimationNdx);
	_DACORE_IMPEXP tBstrPtr GetAnimationAcaPath (LPCTSTR pAnimationName);

protected:
	virtual HRESULT ReadHeader (UINT pLogLevel);

// Implementation
protected:
	bool ReadAcfHeader (UINT pLogLevel = 15);

	CAtlString GetAcaPath (CAgentFileAnimation* pAnimation);
	HRESULT ReadAcaFile (CAgentFileAnimation* pAnimation, bool p32Bit = false, UINT pLogLevel = 15);
	HRESULT ReadAcaFile (CAgentFileAnimation* pAnimation, LPCTSTR pPath, bool p32Bit = false, UINT pLogLevel = 15);
	HRESULT ReadAcaFrames (CAgentFileAnimation* pAnimation, LPCVOID& pBuffer, DWORD& pBufferSize, WORD pImageStart, bool p32Bit = false, UINT pLogLevel = 15);
	HRESULT ReadAcaImages (CAgentFileAnimation* pAnimation, LPCVOID& pBuffer, DWORD& pBufferSize, bool p32Bit = false, UINT pLogLevel = 15);
	HRESULT ReadAcaSounds (CAgentFileAnimation* pAnimation, LPCVOID& pBuffer, DWORD& pBufferSize, bool p32Bit = false, UINT pLogLevel = 15);

protected:
	void DumpAcaImages (UINT pLogLevel);
	void DumpAcaImage (INT_PTR pImageNdx, UINT pLogLevel);

protected:
	tPtr <CFileDownload>				mFileDownload;
	CAtlOwnPtrArray <CAgentFileImage>	mAcaImages;
	CAtlOwnPtrArray <CAtlByteArray>		mAcaSounds;
};

/////////////////////////////////////////////////////////////////////////////
#pragma warning (pop)
