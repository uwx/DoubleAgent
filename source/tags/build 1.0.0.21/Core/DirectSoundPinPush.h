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

class CDirectSoundPinPush : public CDirectShowPinOut, public CDirectShowSeeking, public CDirectShowUtils, private CInstanceGate
{
public:
	CDirectSoundPinPush (class CDirectShowFilter & pFilter, CDirectSoundConvertCache & pConvertCache, long pSoundNdx);
	virtual ~CDirectSoundPinPush ();
	DECLARE_DYNAMIC(CDirectSoundPinPush)

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
	protected:
	virtual LPUNKNOWN GetInterfaceHook(const void* iid);
	//}}AFX_VIRTUAL

// Implementation
protected:
	DECLARE_INTERFACE_MAP()

	HRESULT StreamCuedSound (INT_PTR pCueNdx);
	static DWORD WINAPI StreamProc (LPVOID pThreadParameter);

// Implementation
protected:
	IBaseFilterPtr				mAudioRender;
	CDirectShowPins &			mFilterPins;
	CDirectSoundConvertCache &	mConvertCache;
	CArrayEx <REFERENCE_TIME>	mCueTimes;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DIRECTSOUNDPINPUSH_H_INCLUDED_
