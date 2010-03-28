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
#ifndef DIRECTSOUNDPINPUSH_H_INCLUDED_
#define DIRECTSOUNDPINPUSH_H_INCLUDED_
#pragma once

#include "DirectShowPins.h"
#include "DirectShowSeeking.h"
#include "DirectSoundConvert.h"
#include "InstanceGate.h"

/////////////////////////////////////////////////////////////////////////////

class CDirectSoundPinPush : public CDirectShowPinOut, public CDirectShowSeeking<CDirectSoundPinPush>, public CDirectShowUtils, private CInstanceGate
{
public:
	CDirectSoundPinPush ();
	virtual ~CDirectSoundPinPush ();
	CDirectSoundPinPush & Initialize (class CDirectShowFilter & pFilter, CDirectSoundConvertCache & pConvertCache, long pSoundNdx);

// Attributes
public:
	const long	mSoundNdx;

// Operations
public:
	HRESULT ConvertSound (LPCVOID pSound, long pSoundSize);
	HRESULT ConnectFilters ();
	HRESULT DisconnectFilters (bool pCacheUnusedFilters = true);
	HRESULT CueSound (REFERENCE_TIME pStartTime);

// Overrides
	//{{AFX_VIRTUAL(CDirectSoundPinPush)
	public:
	virtual LONGLONG GetDuration ();
	virtual HRESULT BeginOutputStream (REFERENCE_TIME pStartTime, REFERENCE_TIME pEndTime, double pRate = 1.0);
	//}}AFX_VIRTUAL

// Interfaces
public:
	BEGIN_COM_MAP(CDirectSoundPinPush)
		COM_INTERFACE_ENTRY(IMediaSeeking)
		COM_INTERFACE_ENTRY_CHAIN(CDirectShowPinOut)
	END_COM_MAP()

// Implementation
protected:
	HRESULT StreamCuedSound (INT_PTR pCueNdx, bool pSynchronous);
	static DWORD WINAPI StreamProc (LPVOID pThreadParameter);

// Implementation
protected:
	IBaseFilterPtr					mAudioRender;
	CDirectShowPins *				mFilterPins;
	CDirectSoundConvertCache *		mConvertCache;
	CTypeArray <REFERENCE_TIME>		mCueTimes;
	int								mCueAsyncStart;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DIRECTSOUNDPINPUSH_H_INCLUDED_
