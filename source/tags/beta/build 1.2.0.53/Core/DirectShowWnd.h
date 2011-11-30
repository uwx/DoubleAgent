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
#include "DirectShowUtils.h"

/////////////////////////////////////////////////////////////////////////////

class CDirectShowWndBase :
	public CWindowImpl<CDirectShowWndBase, CWindow, CNullTraits>
{
public:
};

/////////////////////////////////////////////////////////////////////////////

class CDirectShowWnd :
	public CDirectShowWndBase,
	protected CDirectShowUtils
{
	DECLARE_DLL_OBJECT_EX(CDirectShowWnd, _DACORE_IMPEXP)
protected:
	CDirectShowWnd();
public:
	_DACORE_IMPEXP virtual ~CDirectShowWnd();
	_DACORE_IMPEXP static CDirectShowWnd * CreateInstance ();

// Attributes
public:
	bool								mAutoSize;
	bool								mAutoRewind;
	_DACORE_IMPEXP static const UINT	mEventMsg;

	_DACORE_IMPEXP virtual DWORD SetAlphaSmoothing (DWORD pAlphaSmoothing);
	_DACORE_IMPEXP virtual DWORD GetAlphaSmoothing () const;

// Operations
public:
	_DACORE_IMPEXP virtual bool Open (LPCTSTR pFileName);
	_DACORE_IMPEXP virtual void Close ();

	_DACORE_IMPEXP virtual HRESULT Start (DWORD pWaitForCompletion = 100);
	_DACORE_IMPEXP virtual HRESULT Stop (DWORD pWaitForCompletion = 100);
	_DACORE_IMPEXP virtual HRESULT Pause (DWORD pWaitForCompletion = 100);
	_DACORE_IMPEXP virtual HRESULT Resume (DWORD pWaitForCompletion = 100);
	_DACORE_IMPEXP virtual bool Rewind ();

	_DACORE_IMPEXP bool IsPlaying (bool pIncludePause = true) const;
	_DACORE_IMPEXP bool IsPaused () const;
	_DACORE_IMPEXP bool IsStopped () const;
	_DACORE_IMPEXP bool IsEndOfStream ();
	_DACORE_IMPEXP bool IsVideoVisible ();
	_DACORE_IMPEXP bool SetVideoVisible (bool pVisible);

	_DACORE_IMPEXP LONGLONG GetDuration ();
	_DACORE_IMPEXP LONGLONG GetPosition ();
	_DACORE_IMPEXP LONGLONG GetStop ();

	_DACORE_IMPEXP CSize GetVideoSize ();
	_DACORE_IMPEXP CRect GetVideoRect ();
	_DACORE_IMPEXP HRESULT SetVideoRect (const CRect& pVideoRect);
	_DACORE_IMPEXP HRESULT CenterVideo (const CSize* pVideoSize = NULL);
	_DACORE_IMPEXP HRESULT AutoSizeWindow ();
	_DACORE_IMPEXP HRESULT AutoSizeVideo (bool pKeepAspectRatio = false);

// Overrides

// Implementation
protected:
	_DACORE_IMPEXP LRESULT OnDestroy (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnDisplayChange (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnPaint (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnEraseBkgnd (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnPrintClient (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	_DACORE_IMPEXP LRESULT OnMediaEvent (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BEGIN_MSG_MAP(CDirectShowWnd)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_DISPLAYCHANGE, OnDisplayChange)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_PRINTCLIENT, OnPrintClient)
		MESSAGE_HANDLER(mEventMsg, OnMediaEvent)
	END_MSG_MAP()

protected:
	HRESULT _Open (LPCTSTR pFileName);
	void _Close ();
	_DACORE_IMPEXP virtual void Opening (LPCTSTR pFileName);
	_DACORE_IMPEXP virtual void Opened ();
	_DACORE_IMPEXP virtual void Closing ();
	_DACORE_IMPEXP virtual void Closed ();

protected:
	_DACORE_IMPEXP virtual HRESULT Initialize (LPCTSTR pFileName);
	_DACORE_IMPEXP virtual HRESULT PrepareGraph (LPCTSTR pFileName);
	_DACORE_IMPEXP virtual HRESULT GraphPrepared (LPCTSTR pFileName);

	_DACORE_IMPEXP HRESULT PrepareGraphWindowed (IBaseFilter ** pFilter);
	_DACORE_IMPEXP HRESULT PrepareGraphWindowless (IBaseFilter ** pFilter);
	_DACORE_IMPEXP virtual HRESULT InitVideoWindow ();

	_DACORE_IMPEXP virtual COLORREF GetEraseColor ();
	_DACORE_IMPEXP virtual bool EraseWindow (HDC pDC, COLORREF pBkColor);
	_DACORE_IMPEXP virtual bool PaintWindow (HDC pDC);

public:
	_DACORE_IMPEXP IGraphBuilder * GetGraphBuilder () const {return mGraphBuilder;}
	_DACORE_IMPEXP FILTER_STATE GetState (CAtlString* pStateStr);

	_DACORE_IMPEXP void LogState (UINT pLogLevel, LPCTSTR pFormat = NULL, ...);
	_DACORE_IMPEXP void LogStatus (UINT pLogLevel, LPCTSTR pFormat = NULL, ...);
	_DACORE_IMPEXP void LogFilters (UINT pLogLevel, bool pEnumPinTypes = false, LPCTSTR pFormat = NULL, ...);
	_DACORE_IMPEXP void LogFilterStates (UINT pLogLevel, bool pEnumPins = false, LPCTSTR pFormat = NULL, ...);

protected:
	IGraphBuilderPtr			mGraphBuilder;
	IBasicAudioPtr				mBasicAudio;
	IBasicVideoPtr				mBasicVideo;
	IMediaControlPtr			mMediaControl;
	IMediaSeekingPtr			mMediaSeeking;
	IMediaEventExPtr			mMediaEvent;
	IVideoWindowPtr				mVideoWindow;
	IVMRFilterConfigPtr			mVMRFilterConfig7;
	IVMRWindowlessControlPtr	mVMRWindowlessControl7;
	IVMRFilterConfig9Ptr		mVMRFilterConfig9;
	IVMRWindowlessControl9Ptr	mVMRWindowlessControl9;
	DWORD						mAlphaSmoothing;
	CFileHandle					mLogFile;
};

/////////////////////////////////////////////////////////////////////////////
