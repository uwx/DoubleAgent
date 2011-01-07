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
#include "DirectShowFilter.h"
#include "DirectShowSeeking.h"
#include "AgentStreamUtils.h"
#include "ImageBuffer.h"

/////////////////////////////////////////////////////////////////////////////

class CDirectShowRender : public CDirectShowFilter, public _IAgentStreamRender, public IDispatchImpl<IBasicVideo>, public CDirectShowSeeking<CDirectShowRender>
{
public:
	CDirectShowRender();
	virtual ~CDirectShowRender();

// Attributes
public:

// Operations
public:

// Overrides
protected:
	virtual const GUID & GetClassID ();
	virtual HRESULT SetFilterName (LPCWSTR pFilterName);
	virtual CAtlString GetFilterName ();
	virtual void GetSeekingTimes (REFERENCE_TIME & pCurrTime, REFERENCE_TIME & pStopTime);
	virtual bool GetUpstreamSeeking (IMediaSeeking ** pMediaSeeking);
	virtual void InitializePins ();
	virtual void OnJoinedFilterGraph ();
	virtual void OnLeftFilterGraph ();
	virtual HRESULT OnStateChanged (FILTER_STATE pOldState, FILTER_STATE pNewState);
	virtual void OnPinConnected (class CDirectShowPin * pPin);
	virtual void OnStartInputStream (REFERENCE_TIME pStartTime, REFERENCE_TIME pEndTime, double pRate);
	virtual void OnEndInputStream (INT_PTR pPendingSamples);
	virtual void OnClockPulse ();

// Interfaces
public:
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CDirectShowRender)
		COM_INTERFACE_ENTRY(_IAgentStreamRender)
		COM_INTERFACE_ENTRY(IMediaSeeking)
		COM_INTERFACE_ENTRY(IBasicVideo)
		COM_INTERFACE_ENTRY2(IDispatch, IBasicVideo)
		COM_INTERFACE_ENTRY_CHAIN(CDirectShowFilter)
	END_COM_MAP()

public:
	// _IAgentStreamRender
	HRESULT STDMETHODCALLTYPE GetRenderWnd (HWND *pRenderWnd);
	HRESULT STDMETHODCALLTYPE SetRenderWnd (HWND pRenderWnd);
	HRESULT STDMETHODCALLTYPE GetBkColor (COLORREF *pBkColor);
	HRESULT STDMETHODCALLTYPE SetBkColor (const COLORREF *pBkColor);
	HRESULT STDMETHODCALLTYPE GetSmoothing (DWORD *pSmoothing);
	HRESULT STDMETHODCALLTYPE SetSmoothing (DWORD pSmoothing);
	HRESULT STDMETHODCALLTYPE GetImageSize (long *pImageWidth, long *pImageHeight);
	HRESULT STDMETHODCALLTYPE DrawSampleImage (HDC pDC = 0, const RECT *pTargetRect = 0);

	// IBasicVideo
	HRESULT STDMETHODCALLTYPE get_AvgTimePerFrame (REFTIME *pAvgTimePerFrame);
	HRESULT STDMETHODCALLTYPE get_BitRate (long *pBitRate);
	HRESULT STDMETHODCALLTYPE get_BitErrorRate (long *pBitErrorRate);
	HRESULT STDMETHODCALLTYPE get_VideoWidth (long *pVideoWidth);
	HRESULT STDMETHODCALLTYPE get_VideoHeight (long *pVideoHeight);
	HRESULT STDMETHODCALLTYPE put_SourceLeft (long SourceLeft);
	HRESULT STDMETHODCALLTYPE get_SourceLeft (long *pSourceLeft);
	HRESULT STDMETHODCALLTYPE put_SourceWidth (long SourceWidth);
	HRESULT STDMETHODCALLTYPE get_SourceWidth (long *pSourceWidth);
	HRESULT STDMETHODCALLTYPE put_SourceTop (long SourceTop);
	HRESULT STDMETHODCALLTYPE get_SourceTop (long *pSourceTop);
	HRESULT STDMETHODCALLTYPE put_SourceHeight (long SourceHeight);
	HRESULT STDMETHODCALLTYPE get_SourceHeight (long *pSourceHeight);
	HRESULT STDMETHODCALLTYPE put_DestinationLeft (long DestinationLeft);
	HRESULT STDMETHODCALLTYPE get_DestinationLeft (long *pDestinationLeft);
	HRESULT STDMETHODCALLTYPE put_DestinationWidth (long DestinationWidth);
	HRESULT STDMETHODCALLTYPE get_DestinationWidth (long *pDestinationWidth);
	HRESULT STDMETHODCALLTYPE put_DestinationTop (long DestinationTop);
	HRESULT STDMETHODCALLTYPE get_DestinationTop (long *pDestinationTop);
	HRESULT STDMETHODCALLTYPE put_DestinationHeight (long DestinationHeight);
	HRESULT STDMETHODCALLTYPE get_DestinationHeight (long *pDestinationHeight);
	HRESULT STDMETHODCALLTYPE SetSourcePosition (long Left, long Top, long Width, long Height);
	HRESULT STDMETHODCALLTYPE GetSourcePosition (long *pLeft, long *pTop, long *pWidth, long *pHeight);
	HRESULT STDMETHODCALLTYPE SetDefaultSourcePosition (void);
	HRESULT STDMETHODCALLTYPE SetDestinationPosition (long Left, long Top, long Width, long Height);
	HRESULT STDMETHODCALLTYPE GetDestinationPosition (long *pLeft, long *pTop, long *pWidth, long *pHeight);
	HRESULT STDMETHODCALLTYPE SetDefaultDestinationPosition (void);
	HRESULT STDMETHODCALLTYPE GetVideoSize (long *pWidth, long *pHeight);
	HRESULT STDMETHODCALLTYPE GetVideoPaletteEntries (long StartIndex, long Entries, long *pRetrieved, long *pPalette);
	HRESULT STDMETHODCALLTYPE GetCurrentImage (long *pBufferSize,long *pDIBImage);
	HRESULT STDMETHODCALLTYPE IsUsingDefaultSource (void);
	HRESULT STDMETHODCALLTYPE IsUsingDefaultDestination (void);

// Implementation
protected:
	HRESULT FinalConstruct ();

	HRESULT GetNextSampleTime (REFERENCE_TIME pStreamTime, REFERENCE_TIME & pNextSampleTime);
	HRESULT GetInputSample (REFERENCE_TIME pStreamTime, IMediaSamplePtr & pSample, REFERENCE_TIME & pSampleTime, REFERENCE_TIME & pNextSampleTime);
	bool GetSampleImage (IMediaSample * pSample);

	CImageBuffer * ScaleImage (const CSize & pImageSize, const CRect & pTargetRect);
	CImageBuffer * SmoothImage (const CSize & pImageSize, const CRect & pTargetRect);

protected:
	CAtlString					mFilterName;
	tPtr <CDirectShowPinIn>		mInputPin;
	HWND						mRenderWnd;
	tPtr <COLORREF>				mBkColor;
	tPtr <class CUseGdiplus>	mUseGdiplus;
	DWORD						mSmoothing;
	CRect						mSourceRect;
	CRect						mRenderRect;
	CImageBuffer				mImageBuffer;
};

/////////////////////////////////////////////////////////////////////////////
