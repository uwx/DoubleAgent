#ifndef DIRECTSHOWAUDIOPIN_H_INCLUDED_
#define DIRECTSHOWAUDIOPIN_H_INCLUDED_
#pragma once

#include "DirectShowPins.h"

/////////////////////////////////////////////////////////////////////////////

class CDirectShowAudioPin : public CDirectShowPinPull, public CDirectShowUtils
{
public:
	CDirectShowAudioPin (class CDirectShowFilter & pFilter, long pSoundNdx);
	virtual ~CDirectShowAudioPin ();
	DECLARE_DYNAMIC(CDirectShowAudioPin)

// Attributes
public:
	long		mSoundNdx;
	LONGLONG	mSoundDuration;
	 
// Operations
public:
	HRESULT ConnectFilters (LPCVOID pSound, long pSoundSize);
	HRESULT CueSound (REFERENCE_TIME pStartTime);

// Overrides
	//{{AFX_VIRTUAL(CDirectShowAudioPin)
	public:
	virtual HRESULT BeginOutput ();
	virtual HRESULT EndOutput ();
	virtual HRESULT BeginOutputStream (REFERENCE_TIME pStartTime, REFERENCE_TIME pEndTime, double pRate = 1.0);
	//}}AFX_VIRTUAL

// Implementation
protected:
	tPtr <class CDirectSoundDelay>	mDelayFilter;
	IBaseFilterPtr					mWaveParser;
	IBaseFilterPtr					mAudioRender;
	IFilterChainPtr					mFilterChain;
	CDirectShowPins &				mFilterPins;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DIRECTSHOWAUDIOPIN_H_INCLUDED_
