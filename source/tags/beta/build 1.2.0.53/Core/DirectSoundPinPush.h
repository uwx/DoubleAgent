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
#include "DirectShowPins.h"
#include "DirectShowSeeking.h"
#include "DirectSoundConvert.h"
#include "InstanceGate.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CDirectSoundPinPush : public CDirectShowPinOut, public CDirectShowSeeking<CDirectSoundPinPush>, public CDirectShowUtils, private CInstanceGate
{
public:
	CDirectSoundPinPush ();
	virtual ~CDirectSoundPinPush ();

// Attributes
public:
	const long	mSoundNdx;

// Operations
public:
	CDirectSoundPinPush& Initialize (class CDirectShowFilter& pFilter, CDirectSoundConvertCache& pConvertCache, long pSoundNdx);

	HRESULT ConvertSound (LPCVOID pSound, long pSoundSize);
	HRESULT ConnectFilters ();
	HRESULT DisconnectFilters (bool pCacheUnusedFilters = true);
	HRESULT CueSound (REFERENCE_TIME pStartTime);

// Overrides
public:
	virtual LONGLONG GetDuration ();
	virtual HRESULT BeginOutputStream (REFERENCE_TIME pStartTime, REFERENCE_TIME pEndTime, double pRate = 1.0);

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
	CAtlTypeArray <REFERENCE_TIME>	mCueTimes;
	INT_PTR							mCueAsyncStart;
};

/////////////////////////////////////////////////////////////////////////////
