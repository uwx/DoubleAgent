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
#ifndef DIRECTSHOWWND_H_INCLUDED_
#define DIRECTSHOWWND_H_INCLUDED_
#pragma once

#include "DirectShowUtils.h"

/////////////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable: 4251 4275)

class _DACORE_IMPEXP CDirectShowWnd : public CWnd, protected CDirectShowUtils
{
protected:
	CDirectShowWnd();
public:
	virtual ~CDirectShowWnd();
	DECLARE_DYNCREATE(CDirectShowWnd)

// Attributes
public:
	bool				mAutoSize;
	bool				mAutoRewind;
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
	//{{AFX_VIRTUAL(CDirectShowWnd)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CDirectShowWnd)
	afx_msg void OnDestroy();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg LRESULT OnDisplayChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC *pDC);
	afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMediaEvent(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

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
	virtual bool EraseWindow (CDC * pDC, COLORREF pBkColor);
	virtual bool PaintWindow (CDC * pDC);

public:
	FILTER_STATE GetState (CString * pStateStr);
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

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DIRECTSHOWWND_H_INCLUDED_
