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
#ifndef DIRECTSOUNDLIPSYNC_H_INCLUDED_
#define DIRECTSOUNDLIPSYNC_H_INCLUDED_
#pragma once

#include "DirectShowUtils.h"
#include "AgentStreamUtils.h"

/////////////////////////////////////////////////////////////////////////////

class CDirectSoundLipSync : public CComObjectRootEx<CComMultiThreadModel>, ISampleGrabberCB, public CAgentStreamUtils, public CDirectShowUtils
{
public:
	CDirectSoundLipSync ();
	virtual ~CDirectSoundLipSync ();

// Attributes
public:
	REFERENCE_TIME GetDuration () const {return mDuration;}

// Operations
public:
	HRESULT Connect (IGraphBuilder * pGraphBuilder, LPCTSTR pWaveFileName, CAgentStreamInfo * pStreamInfo);
	HRESULT Disconnect (IGraphBuilder * pGraphBuilder = NULL);

	HRESULT Start ();
	HRESULT Stop ();

// Overrides
	//{{AFX_VIRTUAL(CDirectSoundLipSync)
	//}}AFX_VIRTUAL

// Interfaces
public:
	BEGIN_COM_MAP(CDirectSoundLipSync)
		COM_INTERFACE_ENTRY(ISampleGrabberCB)
	END_COM_MAP()

public:
	// ISampleGrabberCB
	HRESULT STDMETHODCALLTYPE SampleCB (double SampleTime, IMediaSample *pSample);
	HRESULT STDMETHODCALLTYPE BufferCB (double SampleTime, BYTE *pBuffer, long BufferLen);

// Implementation
protected:
	void FinalRelease ();

protected:
	mutable CMutex				mStateLock;
	class CDirectShowSource *	mAnimationSource;
	CString						mFilterName;
	IBaseFilterPtr				mGrabberFilter;
	ISampleGrabberPtr			mGrabber;
	IBaseFilterPtr				mFileSource;
	IBaseFilterPtr				mWaveParser;
	IBaseFilterPtr				mAudioRender;
	IFilterChainPtr				mFilterChain;
	tMediaTypePtr				mMediaType;
	REFERENCE_TIME				mDuration;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DIRECTSOUNDLIPSYNC_H_INCLUDED_
