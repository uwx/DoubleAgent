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
#include "StdAfx.h"
#include "DaCore.h"
#include <AgtErr.h>
#include "AgentPreviewWnd.h"
#include "AgentStreamInfo.h"
#include "DirectShowRender.h"
#include "VfwErr.h"
#include "MallocPtr.h"
#include "ImageBuffer.h"
#include "ImageTools.h"

#ifdef	_DEBUG
//#define	_DEBUG_INTERFACE	LogNormal
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Preview"),LogVerbose,true)&0xFFFF|LogTime)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF|LogTime)
#endif

#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE		LogVerbose
#endif
#ifndef	_LOG_RESULTS
#define	_LOG_RESULTS		LogDetails
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DLL_OBJECT(CAgentPreviewWnd)

CAgentPreviewWnd::CAgentPreviewWnd ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CAgentPreviewWnd::CAgentPreviewWnd (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
	mAlphaSmoothing = 0;
	SetBkColor (GetSysColor (COLOR_WINDOW));
}

CAgentPreviewWnd::~CAgentPreviewWnd ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CAgentPreviewWnd::~CAgentPreviewWnd (%d) [%p] [%d]"), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), m_hWnd, ::IsWindow(m_hWnd));
	}
#endif
}

void CAgentPreviewWnd::FinalRelease ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CAgentPreviewWnd::FinalRelease"), this, max(m_dwRef,-1));
	}
#endif
}

CAgentPreviewWnd* CAgentPreviewWnd::CreateInstance ()
{
	CComObject<CAgentPreviewWnd>*	lInstance = NULL;
	LogComErr (LogIfActive|LogTime, CComObject<CAgentPreviewWnd>::CreateInstance (&lInstance));
	return lInstance;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::GetWindow (HWND *phwnd)
{
	HRESULT	lResult = S_OK;
	if	(!phwnd)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*phwnd) = m_hWnd;
	}
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::ContextSensitiveHelp (BOOL fEnterMode)
{
	return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentPreviewWnd::Create (HWND pParentWnd, CRect* pInitialRect)
{
	bool	lRet = false;
	CRect	lInitialRect (0,0,0,0);

	if	(pInitialRect)
	{
		lInitialRect.CopyRect (pInitialRect);
		mAutoSize = false;
	}
	else
	{
		mAutoSize = true;
	}

	if	(
			(::IsWindow (pParentWnd))
		&&	(CDirectShowWnd::Create (pParentWnd, lInitialRect, NULL, WS_CHILD|WS_CLIPSIBLINGS))
		)
	{
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CAgentPreviewWnd::Create [%p] Parent [%p] [%p] Owner [%p]"), this, max(m_dwRef,-1), m_hWnd, pParentWnd, ::GetParent(m_hWnd), ::GetWindow(m_hWnd, GW_OWNER));
		}
#endif
		lRet = true;
	}
	return lRet;
}

DWORD CAgentPreviewWnd::GetAlphaSmoothing () const
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentPreviewWnd::Opened ()
{
	CAgentFile*	lAgentFile;

	CAgentWnd::Opened ();

	if	(
			(!mAutoSize)
		&&	(lAgentFile = GetAgentFile ())
		)
	{
		CSize	lImageSize = lAgentFile->Header.ImageSize;
		CRect	lClientRect;

		GetClientRect (&lClientRect);

		if	(
				(lImageSize.cx > lClientRect.Width())
			||	(lImageSize.cy > lClientRect.Height())
			)
		{
			AutoSizeVideo (true);
		}
		else
		{
			SetVideoRect (CRect (CPoint (0,0), lImageSize));
		}
		CenterVideo ();
	}
}

bool CAgentPreviewWnd::DoAnimationQueue (bool& pNextActivateImmediate, DWORD& pNextQueueTime)
{
	bool	lRet = CAgentWnd::DoAnimationQueue (pNextActivateImmediate, pNextQueueTime);

	if	(lRet)
	{
		UpdateActiveMedia ();
	}
	return lRet;
}

bool CAgentPreviewWnd::DoIdle ()
{
	bool	lRet = CAgentWnd::DoIdle ();

	if	(lRet)
	{
		UpdateActiveMedia ();
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentPreviewWnd::OnMouseActivate (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return MA_ACTIVATEANDEAT;
}

LRESULT CAgentPreviewWnd::OnNcHitTest (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return HTTRANSPARENT;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentPreviewWnd::InternalRenderFrame (HDC pDC, const POINT* pPosition)
{
	HRESULT	lResult = E_FAIL;

	try
	{
		CSize	lImageSize;
		CRect	lTargetRect;

		if	(mRenderFilter)
		{
			mRenderFilter->GetImageSize (&lImageSize.cx, &lImageSize.cy);
			if	(pPosition)
			{
				lTargetRect = CRect (*pPosition, lImageSize);
			}
			else
			{
				lTargetRect = CRect (CPoint (0,0), lImageSize);
			}
			if	(mRenderFilter->DrawSampleImage (pDC, &lTargetRect))
			{
				lResult = S_OK;
			}
			else
			{
				lResult = S_FALSE;
			}
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentPreviewWnd::InternalRenderAnimationFrame (CAgentFile* pAgentFile, LPCTSTR pAnimationName, long pFrameNum, HDC pDC, const POINT* pPosition)
{
	HRESULT	lResult = E_FAIL;

	try
	{
		if	(pAgentFile)
		{
			const CAgentFileAnimation*	lAnimation;

			if	(
					(lAnimation = pAgentFile->GetAnimation (pAnimationName))
				&&	(pFrameNum >= 0)
				&&	(pFrameNum < (long)(short)lAnimation->FrameCount)
				&&	(lAnimation->Frames)
				)
			{
				CPoint					lRenderPos (0,0);
				const CAgentFileFrame&	lFrame = lAnimation->Frames [pFrameNum];
				UINT					lImageFormatSize;
				tArrayPtr <BYTE>		lImageFormat;
				LPBITMAPINFO			lBitmapInfo;
				CImageBuffer			lImageBuffer;

				if	(pPosition)
				{
					lRenderPos.x = pPosition->x;
					lRenderPos.y = pPosition->y;
				}
				if	(
						(lImageFormatSize = pAgentFile->GetImageFormat (NULL, NULL, false))
					&&	(lImageFormat = new BYTE [lImageFormatSize])
					&&	(lBitmapInfo = (LPBITMAPINFO) lImageFormat.Ptr())
					&&	(pAgentFile->GetImageFormat (lBitmapInfo, NULL, false))
					)
				{
					if	(mBkColor)
					{
						SetPaletteBkColor (lBitmapInfo, pAgentFile->Header.Transparency, *mBkColor);
					}
					if	(lImageBuffer.mImage = new ATL::CImage)
					{
						lImageBuffer.mImage->Attach (CreateDIBSection (NULL, lBitmapInfo, DIB_RGB_COLORS, NULL, NULL, 0));
					}
					if	(
							(lImageBuffer.GetImage ())
						&&	(lImageBuffer.mImage->IsDIBSection ())
						)
					{
						pAgentFile->GetFrameBits (GetImageBits (*lImageBuffer.mImage), &lFrame);
						GdiFlush ();

						if	(lImageBuffer.StartBuffer ())
						{
							::BitBlt (pDC, lRenderPos.x, lRenderPos.y, lBitmapInfo->bmiHeader.biWidth, lBitmapInfo->bmiHeader.biHeight, *lImageBuffer.mDC, 0, 0, SRCCOPY);
							lImageBuffer.EndBuffer ();
							lResult = S_OK;
						}
					}
				}
			}
			else
			{
				lResult = AGENTERR_ANIMATIONNOTFOUND;
			}
		}
	}
	catch AnyExceptionDebug

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::PlayAnimation (BSTR pAnimationName)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CAgentPreviewWnd::PlayAnimation [%ls]"), this, max(m_dwRef,-1), pAnimationName);
#endif
	HRESULT	lResult = S_OK;

	if	(!m_hWnd)
	{
		lResult = E_UNEXPECTED;
	}
	else
	if	(!CAgentWnd::ShowAnimation (CAtlString (pAnimationName)))
	{
		lResult = AGENTERR_ANIMATIONNOTFOUND;
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CAgentPreviewWnd::PlayAnimation"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::PlayState (BSTR pStateName)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CAgentPreviewWnd::PlayState"), this, max(m_dwRef,-1), pStateName);
#endif
	HRESULT	lResult = S_OK;

	if	(!m_hWnd)
	{
		lResult = E_UNEXPECTED;
	}
	else
	if	(!CAgentWnd::ShowState (CAtlString (pStateName)))
	{
		lResult = AGENTERR_STATENOTFOUND;
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CAgentPreviewWnd::PlayState"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::GetIsPlaying (boolean *pIsPlaying)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CAgentPreviewWnd::GetIsPlaying"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = S_FALSE;

	if	(pIsPlaying)
	{
		(*pIsPlaying) = FALSE;
	}
	if	(!m_hWnd)
	{
		lResult = E_UNEXPECTED;
	}
	else
	if	(!IsAnimationComplete ())
	{
		lResult = S_OK;
		if	(pIsPlaying)
		{
			(*pIsPlaying) = TRUE;
		}
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CAgentPreviewWnd::GetIsPlaying"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::StopPlaying ()
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CAgentPreviewWnd::StopPlaying"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = S_OK;

	if	(!m_hWnd)
	{
		lResult = E_UNEXPECTED;
	}
	else
	{
		lResult = CAgentWnd::Stop ();
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CAgentPreviewWnd::StopPlaying"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::GetSoundsEnabled (boolean *pEnabled)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CAgentPreviewWnd::GetSoundsEnabled"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = S_FALSE;

	if	(pEnabled)
	{
		(*pEnabled) = FALSE;
	}
	if	(!m_hWnd)
	{
		lResult = E_UNEXPECTED;
	}
	else
	if	(IsSoundEnabled())
	{
		lResult = S_OK;
		if	(pEnabled)
		{
			(*pEnabled) = TRUE;
		}
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CAgentPreviewWnd::GetSoundsEnabled"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::EnableSounds (boolean pEnabled)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CAgentPreviewWnd::EnableSounds"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = S_OK;

	if	(!m_hWnd)
	{
		lResult = E_UNEXPECTED;
	}
	else
	{
		CAgentWnd::EnableSound (pEnabled!=FALSE);
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CAgentPreviewWnd::EnableSounds"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::GetIdleEnabled (boolean *pEnabled)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CAgentPreviewWnd::GetIdleEnabled"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = S_FALSE;

	if	(pEnabled)
	{
		(*pEnabled) = FALSE;
	}
	if	(!m_hWnd)
	{
		lResult = E_UNEXPECTED;
	}
	else
	if	(IsIdleEnabled())
	{
		lResult = S_OK;
		if	(pEnabled)
		{
			(*pEnabled) = TRUE;
		}
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CAgentPreviewWnd::GetIdleEnabled"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::EnableIdle (boolean pEnabled)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CAgentPreviewWnd::EnableIdle"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = S_OK;

	if	(!m_hWnd)
	{
		lResult = E_UNEXPECTED;
	}
	else
	{
		CAgentWnd::EnableIdle (pEnabled!=FALSE);
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CAgentPreviewWnd::EnableIdle"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::SetBkColor (COLORREF pBkColor)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CAgentPreviewWnd::SetBkColor"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = S_OK;

	CAgentWnd::SetBkColor (pBkColor);

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CAgentPreviewWnd::SetBkColor"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::GetFrameSize (SIZE* pFrameSize)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CAgentPreviewWnd::GetFrameSize"), this, max(m_dwRef,-1));
#endif
	HRESULT			lResult = S_OK;
	CAgentFile*	lAgentFile;

	if	(!pFrameSize)
	{
		lResult = E_POINTER;
	}
	else
	if	(lAgentFile = GetAgentFile ())
	{
		(*pFrameSize) = lAgentFile->Header.ImageSize;
	}
	else
	{
		lResult = E_UNEXPECTED;
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CAgentPreviewWnd::GetFrameSize"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::GetCharacterFrameSize (BSTR pCharacterPath, SIZE* pFrameSize)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CAgentPreviewWnd::GetCharacterFrameSize"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = S_OK;

	if	(!pFrameSize)
	{
		lResult = E_POINTER;
	}
	else
	{
		pFrameSize->cx = 0;
		pFrameSize->cy = 0;

		try
		{
			CAtlString			lCharacterPath (pCharacterPath);
			CAgentFile*		lAgentFile;
			tPtr <CAgentFile>	lLocalFile;

			if	(
					(lAgentFile = _AtlModule.FindCachedFile (lCharacterPath))
				||	(
						(lLocalFile = CAgentFile::CreateInstance (pCharacterPath))
					&&	(SUCCEEDED (lResult = lLocalFile->Open (pCharacterPath)))
					&&	(lAgentFile = lLocalFile)
					)
				)
			{
				(*pFrameSize) = lAgentFile->Header.ImageSize;
			}

			SafeFreeSafePtr (lLocalFile);
		}
		catch AnyExceptionSilent
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CAgentPreviewWnd::GetCharacterFrameSize"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::GetFrameFormat (BYTE**pFrameFormat)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CAgentPreviewWnd::GetFrameFormat"), this, max(m_dwRef,-1));
#endif
	HRESULT			lResult = S_OK;
	CAgentFile*	lAgentFile;

	if	(!pFrameFormat)
	{
		lResult = E_POINTER;
	}
	else
	if	(lAgentFile = GetAgentFile())
	{
		UINT				lFormatSize;
		tMallocPtr <BYTE>	lFormat;

		(*pFrameFormat) = NULL;

		if	(
				(lFormatSize = lAgentFile->GetImageFormat (NULL))
			&&	(lFormat.Alloc (lFormatSize))
			)
		{
			lAgentFile->GetImageFormat ((LPBITMAPINFO)lFormat.Ptr());
			(*pFrameFormat) = lFormat.Detach();
		}
		else
		{
			lResult = E_OUTOFMEMORY;
		}
	}
	else
	{
		lResult = E_UNEXPECTED;
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CAgentPreviewWnd::GetFrameFormat"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::GetCharacterFrameFormat (BSTR pCharacterPath, BYTE**pFrameFormat)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CAgentPreviewWnd::GetCharacterFrameFormat"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = S_OK;

	if	(!pFrameFormat)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pFrameFormat) = NULL;

		try
		{
			CAtlString			lCharacterPath (pCharacterPath);
			CAgentFile*		lAgentFile;
			tPtr <CAgentFile>	lLocalFile;
			UINT				lFormatSize;
			tMallocPtr <BYTE>	lFormat;

			if	(
					(lAgentFile = _AtlModule.FindCachedFile (lCharacterPath))
				||	(
						(lLocalFile = CAgentFile::CreateInstance (pCharacterPath))
					&&	(SUCCEEDED (lResult = lLocalFile->Open (pCharacterPath)))
					&&	(lAgentFile = lLocalFile)
					)
				)
			{
				if	(
						(lFormatSize = lAgentFile->GetImageFormat (NULL))
					&&	(lFormat.Alloc (lFormatSize))
					)
				{
					lAgentFile->GetImageFormat ((LPBITMAPINFO)lFormat.Ptr());
					(*pFrameFormat) = lFormat.Detach();
				}
				else
				{
					lResult = E_OUTOFMEMORY;
				}
			}

			SafeFreeSafePtr (lLocalFile);
		}
		catch AnyExceptionSilent
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CAgentPreviewWnd::GetCharacterFrameFormat"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::OnAppActive (boolean pActive)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CAgentPreviewWnd::OnAppActive [%u]"), this, max(m_dwRef,-1), pActive);
#endif
	MakeActiveMedia (pActive!=FALSE);
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::RenderFrame (HDC pDC, POINT *pPosition)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CAgentPreviewWnd::RenderFrame"), this, max(m_dwRef,-1));
#endif

	HRESULT	lResult = InternalRenderFrame (pDC, pPosition);

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CAgentPreviewWnd::RenderFrame"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::RenderAnimationFrame (BSTR pAnimationName, USHORT pFrameNum, HDC pDC, POINT *pPosition)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CAgentPreviewWnd::RenderAnimationFrame [%ls] [%hu]"), this, max(m_dwRef,-1), pAnimationName, pFrameNum);
#endif

	HRESULT	lResult = InternalRenderAnimationFrame (GetAgentFile(), CAtlString (pAnimationName), (long)pFrameNum, pDC, pPosition);

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CAgentPreviewWnd::RenderAnimationFrame"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::RenderCharacterFrame (BSTR pCharacterPath, BSTR pAnimationName, USHORT pFrameNum, HDC pDC, POINT *pPosition)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CAgentPreviewWnd::RenderCharacterFrame [%ls] [%ls] [%hu]"), this, max(m_dwRef,-1), pCharacterPath, pAnimationName, pFrameNum);
#endif
	HRESULT	lResult = E_FAIL;

	try
	{
		CAtlString			lCharacterPath (pCharacterPath);
		CAgentFile*		lAgentFile;
		tPtr <CAgentFile>	lLocalFile;

		if	(
				(lAgentFile = _AtlModule.FindCachedFile (lCharacterPath))
			||	(
					(lLocalFile = CAgentFile::CreateInstance (pCharacterPath))
				&&	(SUCCEEDED (lResult = lLocalFile->Open (pCharacterPath)))
				&&	(lAgentFile = lLocalFile)
				)
			)
		{
			lResult = InternalRenderAnimationFrame (lAgentFile, CAtlString (pAnimationName), (long)pFrameNum, pDC, pPosition);
		}

		SafeFreeSafePtr (lLocalFile);
	}
	catch AnyExceptionDebug

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon ((lResult == AGENTERR_ANIMATIONNOTFOUND) ? MaxLogLevel(_LOG_RESULTS,LogVerbose) : _LOG_RESULTS, lResult, _T("[%p(%d)] CAgentPreviewWnd::RenderCharacterFrame [%ls] [%ls] [%hu]"), this, max(m_dwRef,-1), pCharacterPath, pAnimationName, pFrameNum);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::OpenFile (BSTR pCharacterPath, HWND pParentWnd)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CAgentPreviewWnd::OpenFile [%ls] [%p]"), this, max(m_dwRef,-1), pCharacterPath, pParentWnd);
#endif
	HRESULT				lResult = S_OK;
	tPtr <CAgentFile>	lAgentFile;
	CRect				lParentRect;
	CRect*				lInitialRect = NULL;

	if	(lAgentFile = CAgentFile::CreateInstance (pCharacterPath))
	{
		if	(SUCCEEDED (lResult = lAgentFile->Open (pCharacterPath)))
		{
			if	(m_hWnd)
			{
				Close ();
				DestroyWindow ();
			}
			if	(
					(::IsWindow (pParentWnd))
				&&	(::GetClientRect (pParentWnd, &lParentRect))
				)
			{
				lInitialRect = &lParentRect;
			}
			if	(
					(Create (pParentWnd, lInitialRect))
				&&	(Open (lAgentFile))
				)
			{
				EnableIdle (true);
			}
			else
			{
				DestroyWindow ();
				lResult = E_FAIL;
			}
		}
	}
	else
	{
		lResult = E_OUTOFMEMORY;
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CAgentPreviewWnd::OpenFile"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::GetCharacterPath (BSTR *pCharacterPath)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CAgentPreviewWnd::GetCharacterPath"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = S_OK;

	if	(
			(!m_hWnd)
		||	(!GetAgentFile ())
		)
	{
		lResult = E_UNEXPECTED;
	}
	else
	if	(!pCharacterPath)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pCharacterPath) = (BSTR)GetAgentFile()->Path.Detach();
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CAgentPreviewWnd::GetCharacterPath"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::GetCharacterName (BSTR *pCharacterName, USHORT pLangID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CAgentPreviewWnd::GetCharacterName"), this, max(m_dwRef,-1));
#endif
	HRESULT				lResult = S_OK;
	CAgentFileName*	lFileName;

	if	(
			(!m_hWnd)
		||	(!GetAgentFile ())
		)
	{
		lResult = E_UNEXPECTED;
	}
	else
	if	(!pCharacterName)
	{
		lResult = E_POINTER;
	}
	else
	if	(lFileName = GetAgentFile()->FindName (pLangID))
	{
		(*pCharacterName) = tBstrPtr (lFileName->Name).Detach();
	}
	else
	{
		(*pCharacterName) = NULL;
		lResult = S_FALSE;
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CAgentPreviewWnd::GetCharacterName"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::GetCharacterDescription (BSTR *pCharacterDescription, USHORT pLangID)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CAgentPreviewWnd::GetCharacterDescription"), this, max(m_dwRef,-1));
#endif
	HRESULT				lResult = S_OK;
	CAgentFileName*	lFileName;

	if	(
			(!m_hWnd)
		||	(!GetAgentFile ())
		)
	{
		lResult = E_UNEXPECTED;
	}
	else
	if	(!pCharacterDescription)
	{
		lResult = E_POINTER;
	}
	else
	if	(lFileName = GetAgentFile()->FindName (pLangID))
	{
		(*pCharacterDescription) = tBstrPtr (lFileName->Desc1).Detach();
	}
	else
	{
		(*pCharacterDescription) = NULL;
		lResult = S_FALSE;
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CAgentPreviewWnd::GetCharacterDescription"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::GetCharacterGuid (GUID *pGuid)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] CAgentPreviewWnd::GetCharacterGuid"), this, max(m_dwRef,-1));
#endif
	HRESULT	lResult = S_OK;

	if	(
			(!m_hWnd)
		||	(!GetAgentFile ())
		)
	{
		lResult = E_UNEXPECTED;
	}
	else
	if	(!pGuid)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*pGuid) = GetAgentFile()->Header.Guid;
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] CAgentPreviewWnd::GetCharacterGuid"), this, max(m_dwRef,-1));
	}
#endif
	return lResult;
}
