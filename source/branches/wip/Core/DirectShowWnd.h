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
#pragma once
#include "DirectShowUtils.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable: 4251 4275)
/////////////////////////////////////////////////////////////////////////////

class CDirectShowWndBase :
	public CWindowImpl<CDirectShowWndBase, CWindow, CNullTraits>
{
public:
};

/////////////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP CDirectShowWnd :
	public CDirectShowWndBase,
	protected CDirectShowUtils
{
	DECLARE_DLL_OBJECT(CDirectShowWnd)
protected:
	CDirectShowWnd();
public:
	virtual ~CDirectShowWnd();
	static CDirectShowWnd * CreateInstance ();

// Attributes
public:
	bool				mAutoSize;
	bool				mAutoRewind;
	DWORD				mAlphaSmoothing;
	static const UINT	mEventMsg;

// Operations
public:
	virtual bool Open (LPCTSTR pFileName);
	virtual void Close ();

	virtual HRESULT Start (DWORD pWaitForCompletion = 100);
	virtual HRESULT Stop (DWORD pWaitForCompletion = 100);
	virtual HRESULT Pause (DWORD pWaitForCompletion = 100);
	virtual HRESULT Resume (DWORD pWaitForCompletion = 100);
	virtual bool Rewind ();

	bool IsPlaying (bool pIncludePause = true) const;
	bool IsPaused () const;
	bool IsStopped () const;
	bool IsEndOfStream ();
	bool IsVideoVisible ();
	bool SetVideoVisible (bool pVisible);

	LONGLONG GetDuration ();
	LONGLONG GetPosition ();
	LONGLONG GetStop ();

// Overrides

// Implementation
protected:
	LRESULT OnDestroy (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnDisplayChange (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnPaint (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnEraseBkgnd (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnPrintClient (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);
	LRESULT OnMediaEvent (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled);

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
	virtual void Opening (LPCTSTR pFileName);
	virtual void Opened ();
	virtual void Closing ();
	virtual void Closed ();

protected:
	virtual HRESULT Initialize (LPCTSTR pFileName);
	virtual HRESULT PrepareGraph (LPCTSTR pFileName);
	virtual HRESULT GraphPrepared (LPCTSTR pFileName);

	HRESULT PrepareGraphWindowed (IBaseFilter ** pFilter);
	HRESULT PrepareGraphWindowless (IBaseFilter ** pFilter);
	virtual HRESULT InitVideoWindow ();

	HRESULT AutoSizeWindow ();
	HRESULT AutoSizeVideo (bool pKeepAspectRatio = false);
	CSize GetVideoSize ();
	CRect GetVideoRect ();

	virtual COLORREF GetEraseColor ();
	virtual bool EraseWindow (HDC pDC, COLORREF pBkColor);
	virtual bool PaintWindow (HDC pDC);

public:
	FILTER_STATE GetState (CAtlString * pStateStr);
	void LogState (UINT pLogLevel, LPCTSTR pFormat = NULL, ...);
	void LogStatus (UINT pLogLevel, LPCTSTR pFormat = NULL, ...);
	void LogFilters (UINT pLogLevel, bool pEnumPinTypes = false, LPCTSTR pFormat = NULL, ...);
	void LogFilterStates (UINT pLogLevel, bool pEnumPins = false, LPCTSTR pFormat = NULL, ...);

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
	CFileHandle					mLogFile;
};

#pragma warning(pop)
/////////////////////////////////////////////////////////////////////////////
