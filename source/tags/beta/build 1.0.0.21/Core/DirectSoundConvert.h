/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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
#ifndef DIRECTSOUNDCONVERT_H_INCLUDED_
#define DIRECTSOUNDCONVERT_H_INCLUDED_
#pragma once

#include <mmsystem.h>
#include "DirectShowUtils.h"

/////////////////////////////////////////////////////////////////////////////

class CDirectSoundConvert : public CDirectShowUtils
{
public:
	CDirectSoundConvert ();
	virtual ~CDirectSoundConvert ();

// Attributes
public:
	void SetInputFormat (const LPWAVEFORMATEX pFormat);
	bool GetInputFormat (LPWAVEFORMATEX & pFormat);
	bool GetOutputFormat (LPWAVEFORMATEX & pFormat);

	void SetInputBuffer (LPCVOID pBuffer, ULONG pBufferSize);
	bool GetInputBuffer (LPCVOID & pBuffer, ULONG & pBufferSize);
	bool GetOutputBuffer (LPVOID & pBuffer, ULONG & pBufferSize, ULONG & pDataLength);

// Operations
public:
	HRESULT DeriveOutputFormat ();
	HRESULT ConvertSound ();
	HRESULT StashSound ();

// Implementation
protected:
	bool IsValidOutputFormat (const LPWAVEFORMATEX pInputFormat, const LPWAVEFORMATEX pOutputFormat);
	bool SuggestOutputFormat (const LPWAVEFORMATEX pInputFormat, const LPWAVEFORMATEX pFilterFormat, LPWAVEFORMATEX * pOutputFormat, DWORD pFormatSize);
	HRESULT EnumAcmFormats (const LPWAVEFORMATEX pInputFormat, COwnPtrArray <WAVEFORMATEX> & pRankedFormats);

	static LPWAVEFORMATEX DuplicateFormat (const LPWAVEFORMATEX pFormat, DWORD pFormatSize = 0);
	static int RankFormatMatch (const LPWAVEFORMATEX pInputFormat, const LPWAVEFORMATEX pOutputFormat);

protected:
	mutable CCriticalSection	mLock;
	tPtr <WAVEFORMATEX>			mInputFormat;
	LPCVOID						mInputBuffer;
	ULONG						mInputSize;
	tPtr <WAVEFORMATEX>			mOutputFormat;
	CGlobalHandle				mOutputBuffer;
	ULONG						mOutputSize;
	ULONG						mOutputLength;
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

class CDirectSoundConvertCache
{
public:
	CDirectSoundConvertCache ();
	virtual ~CDirectSoundConvertCache ();

// Attributes
public:
	INT_PTR GetSize () const;

// Operations
public:
	CDirectSoundConvert * GetCachedConvert (UINT pSoundNdx);
	bool CacheSoundConvert (CDirectSoundConvert * pConvert, UINT pSoundNdx);
	bool RemovedCachedConvert (UINT pSoundNdx);

// Implementation
protected:
	mutable CCriticalSection				mLock;
	COwnPtrMap <UINT, CDirectSoundConvert>	mCache;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DIRECTSOUNDCONVERT_H_INCLUDED_
