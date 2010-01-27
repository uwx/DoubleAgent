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
#include "StdAfx.h"
#include "DaCore.h"
#include <AfxPriv.h>
#include "AgtErr.h"
#include "AgentPreviewWnd.h"
#include "AgentStreamInfo.h"
#include "DirectShowRender.h"
#include "VfwErr.h"
#include "MallocPtr.h"
#include "BitmapBuffer.h"
#ifdef	_DEBUG
#include "GuidStr.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_COM			LogNormal
//#define	_DEBUG_INTERFACE	LogNormal
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Preview"),LogVerbose,true)&0xFFFF)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE		LogVerbose
#endif
#ifndef	_LOG_RESULTS
#define	_LOG_RESULTS		LogDetails
#endif

/////////////////////////////////////////////////////////////////////////////

// {1147E561-A208-11DE-ABF2-002421116FB2}
IMPLEMENT_DYNCREATE(CAgentPreviewWnd, CAgentWnd)
IMPLEMENT_OLECREATE_EX(CAgentPreviewWnd, _PREVIEW_PROGID, 0x1147E561, 0xA208, 0x11DE, 0xAB, 0xF2, 0x00, 0x24, 0x21, 0x11, 0x6F, 0xB2)

BOOL CAgentPreviewWnd::CAgentPreviewWndFactory::UpdateRegistry (BOOL bRegister)
{
	if	(COleObjectFactoryExEx::UpdateRegistry (bRegister, _T(_PREVIEW_PROGID_NAME)))
	{
		if	(bRegister)
		{
			RegisterApartmentThreaded ();
			RegisterDefCategory ();
		}
		return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CAgentPreviewWnd, CAgentWnd)
	//{{AFX_MSG_MAP(CAgentPreviewWnd)
	ON_WM_MOUSEACTIVATE()
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

#include "InterfaceMap.inl"

BEGIN_INTERFACE_MAP(CAgentPreviewWnd, CAgentWnd)
	INTERFACE_PART(CAgentPreviewWnd, __uuidof(IUnknown), InnerUnknown)
	INTERFACE_PART(CAgentPreviewWnd, __uuidof(IOleWindow), OleWindow)
	INTERFACE_PART(CAgentPreviewWnd, __uuidof(IDaPreview), DaPreview)
END_INTERFACE_MAP()

IMPLEMENT_IUNKNOWN(CAgentPreviewWnd, OleWindow)
IMPLEMENT_IUNKNOWN(CAgentPreviewWnd, DaPreview)

/////////////////////////////////////////////////////////////////////////////

CAgentPreviewWnd::CAgentPreviewWnd ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CAgentPreviewWnd::CAgentPreviewWnd (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp();
	EnableAggregation ();

	mAlphaBlended = false;
	SetBkColor (GetSysColor (COLOR_WINDOW));
}

CAgentPreviewWnd::~CAgentPreviewWnd ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CAgentPreviewWnd::~CAgentPreviewWnd (%d) [%p] [%d]"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, m_hWnd, ::IsWindow(m_hWnd));
	}
#endif
	AfxOleUnlockApp();
}

void CAgentPreviewWnd::OnFinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CAgentPreviewWnd::OnFinalRelease"), this, m_dwRef);
	}
#endif
	CCmdTarget::OnFinalRelease();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentPreviewWnd::Create (HWND pParentWnd, CRect * pInitialRect)
{
	bool	lRet = false;
	CRect	lInitialRect (0,0,0,0);

	if	(pInitialRect)
	{
		lInitialRect.CopyRect (pInitialRect);
		mAutoSize = false;
	}

	if	(
			(IsWindow (pParentWnd))
		&&	(CDirectShowWnd::Create (NULL, NULL, WS_CHILD|WS_CLIPSIBLINGS, lInitialRect, CWnd::FromHandle(pParentWnd), 0))
		)
	{
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%u)] CAgentPreviewWnd::Create [%p] Parent [%p] [%p] Owner [%p]"), this, m_dwRef, m_hWnd, pParentWnd, ::GetParent(m_hWnd), ::GetWindow(m_hWnd, GW_OWNER));
		}
#endif
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentPreviewWnd::DoAnimationQueue ()
{
	bool	lRet = CAgentWnd::DoAnimationQueue ();

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

int CAgentPreviewWnd::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return MA_ACTIVATEANDEAT;
}

_MFC_NCHITTEST_RESULT CAgentPreviewWnd::OnNcHitTest(CPoint point)
{
	return HTTRANSPARENT;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentPreviewWnd::RenderFrame (HDC pDC, const POINT * pPosition)
{
	HRESULT	lResult = E_FAIL;

	try
	{
		CDirectShowRender *	lRenderFilter;
		CRect				lTargetRect;

		if	(lRenderFilter = GetRenderFilter ())
		{
			if	(pPosition)
			{
				lTargetRect = CRect (*pPosition, lRenderFilter->GetImageSize());
			}
			else
			{
				lTargetRect = CRect (CPoint (0,0), lRenderFilter->GetImageSize());
			}
			if	(lRenderFilter->DrawSampleImage (pDC, &lTargetRect))
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

HRESULT CAgentPreviewWnd::RenderAnimationFrame (CAgentFile * pAgentFile, LPCTSTR pAnimationName, long pFrameNum, HDC pDC, const POINT * pPosition)
{
	HRESULT	lResult = E_FAIL;

	try
	{
		if	(pAgentFile)
		{
			const CAgentFileAnimation *	lAnimation;

			if	(
					(lAnimation = pAgentFile->GetAnimation (pAnimationName))
				&&	(pFrameNum >= 0)
				&&	(pFrameNum < (long)(short)lAnimation->mFrameCount)
				&&	(lAnimation->mFrames)
				)
			{
				CPoint					lRenderPos (0,0);
				BYTE					lTransparency = pAgentFile->GetTransparency();
				const CAgentFileFrame &	lFrame = lAnimation->mFrames [pFrameNum];
				UINT					lImageFormatSize;
				tArrayPtr <BYTE>		lImageFormat;
				LPBITMAPINFO			lBitmapInfo;
				CBitmapBuffer			lBitmapBuffer;

				if	(pPosition)
				{
					lRenderPos.x = pPosition->x;
					lRenderPos.y = pPosition->y;
				}
				if	(pAgentFile->GetImageCount () <= 0)
				{
					pAgentFile->ReadImageIndex ();
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
						((LPRGBQUAD)lBitmapInfo->bmiColors) [lTransparency].rgbRed = GetRValue(*mBkColor);
						((LPRGBQUAD)lBitmapInfo->bmiColors) [lTransparency].rgbGreen = GetGValue(*mBkColor);
						((LPRGBQUAD)lBitmapInfo->bmiColors) [lTransparency].rgbBlue = GetBValue(*mBkColor);
					}
					if	(lBitmapBuffer.mBitmap.Attach (CreateDIBSection (NULL, lBitmapInfo, DIB_RGB_COLORS, (LPVOID *) &lBitmapBuffer.mBitmapBits, NULL, 0)))
					{
						pAgentFile->GetFrameBits (lBitmapBuffer.mBitmapBits, lFrame);
						GdiFlush ();

						if	(lBitmapBuffer.StartBuffer ())
						{
							::BitBlt (pDC, lRenderPos.x, lRenderPos.y, lBitmapInfo->bmiHeader.biWidth, lBitmapInfo->bmiHeader.biHeight, lBitmapBuffer.mDC, 0, 0, SRCCOPY);
							lBitmapBuffer.EndBuffer ();
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

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::XOleWindow::GetWindow (HWND *phwnd)
{
	METHOD_PROLOGUE(CAgentPreviewWnd, OleWindow)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CAgentPreviewWnd::XOleWindow::GetWindow"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!phwnd)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*phwnd) = pThis->m_hWnd;
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CAgentPreviewWnd::XOleWindow::GetWindow"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::XOleWindow::ContextSensitiveHelp (BOOL fEnterMode)
{
	METHOD_PROLOGUE(CAgentPreviewWnd, OleWindow)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CAgentPreviewWnd::XOleWindow::ContextSensitiveHelp"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = E_NOTIMPL;
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CAgentPreviewWnd::XOleWindow::ContextSensitiveHelp"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::XDaPreview::PlayAnimation (BSTR pAnimationName)
{
	METHOD_PROLOGUE(CAgentPreviewWnd, DaPreview)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::PlayAnimation [%ls]"), pThis, pThis->m_dwRef, pAnimationName);
#endif
	HRESULT	lResult = S_OK;

	if	(!pThis->GetSafeHwnd ())
	{
		lResult = E_UNEXPECTED;
	}
	else
	if	(!pThis->ShowAnimation (CString (pAnimationName)))
	{
		lResult = AGENTERR_ANIMATIONNOTFOUND;
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::PlayAnimation"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::XDaPreview::PlayState (BSTR pStateName)
{
	METHOD_PROLOGUE(CAgentPreviewWnd, DaPreview)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::PlayState"), pThis, pThis->m_dwRef, pStateName);
#endif
	HRESULT	lResult = S_OK;

	if	(!pThis->GetSafeHwnd ())
	{
		lResult = E_UNEXPECTED;
	}
	else
	if	(!pThis->ShowState (CString (pStateName)))
	{
		lResult = AGENTERR_STATENOTFOUND;
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::PlayState"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::XDaPreview::GetIsPlaying (boolean *pIsPlaying)
{
	METHOD_PROLOGUE(CAgentPreviewWnd, DaPreview)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::GetIsPlaying"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(pIsPlaying)
	{
		(*pIsPlaying) = FALSE;
	}
	if	(!pThis->GetSafeHwnd ())
	{
		lResult = E_UNEXPECTED;
	}
	else
	if	(!pThis->IsAnimationComplete ())
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::GetIsPlaying"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::XDaPreview::Stop ()
{
	METHOD_PROLOGUE(CAgentPreviewWnd, DaPreview)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::Stop"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!pThis->GetSafeHwnd ())
	{
		lResult = E_UNEXPECTED;
	}
	else
	{
		lResult = pThis->Stop ();
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::Stop"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::XDaPreview::GetSoundsEnabled (boolean *pEnabled)
{
	METHOD_PROLOGUE(CAgentPreviewWnd, DaPreview)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::GetSoundsEnabled"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(pEnabled)
	{
		(*pEnabled) = FALSE;
	}
	if	(!pThis->GetSafeHwnd ())
	{
		lResult = E_UNEXPECTED;
	}
	else
	if	(pThis->IsSoundEnabled())
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::GetSoundsEnabled"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::XDaPreview::EnableSounds (boolean pEnabled)
{
	METHOD_PROLOGUE(CAgentPreviewWnd, DaPreview)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::EnableSounds"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!pThis->GetSafeHwnd ())
	{
		lResult = E_UNEXPECTED;
	}
	else
	{
		pThis->EnableSound (pEnabled!=FALSE);
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::EnableSounds"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::XDaPreview::GetIdleEnabled (boolean *pEnabled)
{
	METHOD_PROLOGUE(CAgentPreviewWnd, DaPreview)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::GetIdleEnabled"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(pEnabled)
	{
		(*pEnabled) = FALSE;
	}
	if	(!pThis->GetSafeHwnd ())
	{
		lResult = E_UNEXPECTED;
	}
	else
	if	(pThis->IsIdleEnabled())
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::GetIdleEnabled"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::XDaPreview::EnableIdle (boolean pEnabled)
{
	METHOD_PROLOGUE(CAgentPreviewWnd, DaPreview)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::EnableIdle"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!pThis->GetSafeHwnd ())
	{
		lResult = E_UNEXPECTED;
	}
	else
	{
		pThis->EnableIdle (pEnabled!=FALSE);
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::EnableIdle"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::XDaPreview::SetBkColor (COLORREF pBkColor)
{
	METHOD_PROLOGUE(CAgentPreviewWnd, DaPreview)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::SetBkColor"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	pThis->SetBkColor (pBkColor);

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::SetBkColor"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::XDaPreview::GetFrameSize (SIZE * pFrameSize)
{
	METHOD_PROLOGUE(CAgentPreviewWnd, DaPreview)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::GetFrameSize"), pThis, pThis->m_dwRef);
#endif
	HRESULT			lResult = S_OK;
	CAgentFile *	lAgentFile;

	if	(!pFrameSize)
	{
		lResult = E_POINTER;
	}
	else
	if	(lAgentFile = pThis->GetAgentFile ())
	{
		(*pFrameSize) = lAgentFile->GetImageSize();
	}
	else
	{
		lResult = E_UNEXPECTED;
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::GetFrameSize"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::XDaPreview::GetCharacterFrameSize (BSTR pCharacterPath, SIZE * pFrameSize)
{
	METHOD_PROLOGUE(CAgentPreviewWnd, DaPreview)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::GetCharacterFrameSize"), pThis, pThis->m_dwRef);
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
			CString				lCharacterPath (pCharacterPath);
			CAgentFile *		lAgentFile;
			tPtr <CAgentFile>	lLocalFile;

			if	(
					(lAgentFile = TheCoreApp->FindCachedFile (lCharacterPath))
				||	(
						(lLocalFile = (CAgentFile *)CAgentFile::CreateObject())
					&&	(SUCCEEDED (lResult = lLocalFile->Open (pCharacterPath)))
					&&	(lAgentFile = lLocalFile)
					)
				)
			{
				(*pFrameSize) = lAgentFile->GetImageSize();
			}

			SafeFreeSafePtr (lLocalFile);
		}
		catch AnyExceptionSilent
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::GetCharacterFrameSize"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::XDaPreview::GetFrameFormat (BYTE **pFrameFormat)
{
	METHOD_PROLOGUE(CAgentPreviewWnd, DaPreview)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::GetFrameFormat"), pThis, pThis->m_dwRef);
#endif
	HRESULT			lResult = S_OK;
	CAgentFile *	lAgentFile;

	if	(!pFrameFormat)
	{
		lResult = E_POINTER;
	}
	else
	if	(lAgentFile = pThis->GetAgentFile())
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::GetFrameFormat"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::XDaPreview::GetCharacterFrameFormat (BSTR pCharacterPath, BYTE **pFrameFormat)
{
	METHOD_PROLOGUE(CAgentPreviewWnd, DaPreview)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::GetCharacterFrameFormat"), pThis, pThis->m_dwRef);
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
			CString				lCharacterPath (pCharacterPath);
			CAgentFile *		lAgentFile;
			tPtr <CAgentFile>	lLocalFile;
			UINT				lFormatSize;
			tMallocPtr <BYTE>	lFormat;

			if	(
					(lAgentFile = TheCoreApp->FindCachedFile (lCharacterPath))
				||	(
						(lLocalFile = (CAgentFile *)CAgentFile::CreateObject())
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::GetCharacterFrameFormat"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::XDaPreview::OnActivateApp (boolean pActivated)
{
	METHOD_PROLOGUE(CAgentPreviewWnd, DaPreview)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::OnActivateApp [%u]"), pThis, pThis->m_dwRef, pActivated);
#endif
	pThis->MakeActiveMedia (pActivated!=FALSE);
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::XDaPreview::RenderFrame (HDC pDC, POINT *pPosition)
{
	METHOD_PROLOGUE(CAgentPreviewWnd, DaPreview)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::RenderFrame"), pThis, pThis->m_dwRef);
#endif

	HRESULT	lResult = pThis->RenderFrame (pDC, pPosition);

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::RenderFrame"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::XDaPreview::RenderAnimationFrame (BSTR pAnimationName, USHORT pFrameNum, HDC pDC, POINT *pPosition)
{
	METHOD_PROLOGUE(CAgentPreviewWnd, DaPreview)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::RenderAnimationFrame [%ls] [%hu]"), pThis, pThis->m_dwRef, pAnimationName, pFrameNum);
#endif

	HRESULT	lResult = pThis->RenderAnimationFrame (pThis->GetAgentFile(), CString (pAnimationName), (long)pFrameNum, pDC, pPosition);

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::RenderAnimationFrame"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::XDaPreview::RenderCharacterFrame (BSTR pCharacterPath, BSTR pAnimationName, USHORT pFrameNum, HDC pDC, POINT *pPosition)
{
	METHOD_PROLOGUE(CAgentPreviewWnd, DaPreview)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::RenderCharacterFrame [%ls] [%ls] [%hu]"), pThis, pThis->m_dwRef, pCharacterPath, pAnimationName, pFrameNum);
#endif
	HRESULT	lResult = E_FAIL;

	try
	{
		CString				lCharacterPath (pCharacterPath);
		CAgentFile *		lAgentFile;
		tPtr <CAgentFile>	lLocalFile;

		if	(
				(lAgentFile = TheCoreApp->FindCachedFile (lCharacterPath))
			||	(
					(lLocalFile = (CAgentFile *)CAgentFile::CreateObject())
				&&	(SUCCEEDED (lResult = lLocalFile->Open (pCharacterPath)))
				&&	(lAgentFile = lLocalFile)
				)
			)
		{
			lResult = pThis->RenderAnimationFrame (lAgentFile, CString (pAnimationName), (long)pFrameNum, pDC, pPosition);
		}

		SafeFreeSafePtr (lLocalFile);
	}
	catch AnyExceptionDebug

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon ((lResult == AGENTERR_ANIMATIONNOTFOUND) ? MaxLogLevel(_LOG_RESULTS,LogVerbose) : _LOG_RESULTS, lResult, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::RenderCharacterFrame [%ls] [%ls] [%hu]"), pThis, pThis->m_dwRef, pCharacterPath, pAnimationName, pFrameNum);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::XDaPreview::Open (BSTR pCharacterPath, HWND pParentWnd)
{
	METHOD_PROLOGUE(CAgentPreviewWnd, DaPreview)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::Open [%ls] [%p]"), pThis, pThis->m_dwRef, pCharacterPath, pParentWnd);
#endif
	HRESULT				lResult = S_OK;
	tPtr <CAgentFile>	lAgentFile;

	if	(lAgentFile = (CAgentFile *)CAgentFile::CreateObject())
	{
		if	(SUCCEEDED (lResult = lAgentFile->Open (pCharacterPath)))
		{
			if	(pThis->GetSafeHwnd())
			{
				pThis->Close ();
				pThis->DestroyWindow ();
			}
			if	(
					(pThis->Create (pParentWnd))
				&&	(pThis->Open (lAgentFile))
				)
			{
				pThis->EnableIdle (true);
			}
			else
			{
				pThis->DestroyWindow ();
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
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::Open"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::XDaPreview::GetCharacterPath (BSTR *pCharacterPath)
{
	METHOD_PROLOGUE(CAgentPreviewWnd, DaPreview)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::GetCharacterPath"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(
			(!pThis->GetSafeHwnd ())
		||	(!pThis->GetAgentFile ())
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
		(*pCharacterPath) = pThis->GetAgentFile()->GetPath().Detach();
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::GetCharacterPath"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::XDaPreview::GetCharacterName (BSTR *pCharacterName, USHORT pLangID)
{
	METHOD_PROLOGUE(CAgentPreviewWnd, DaPreview)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::GetCharacterName"), pThis, pThis->m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	CAgentFileName *	lFileName;

	if	(
			(!pThis->GetSafeHwnd ())
		||	(!pThis->GetAgentFile ())
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
	if	(lFileName = pThis->GetAgentFile()->FindName (pLangID))
	{
		(*pCharacterName) = tBstrPtr (lFileName->mName).Detach();
	}
	else
	{
		(*pCharacterName) = NULL;
		lResult = S_FALSE;
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::GetCharacterName"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::XDaPreview::GetCharacterDescription (BSTR *pCharacterDescription, USHORT pLangID)
{
	METHOD_PROLOGUE(CAgentPreviewWnd, DaPreview)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::GetCharacterDescription"), pThis, pThis->m_dwRef);
#endif
	HRESULT				lResult = S_OK;
	CAgentFileName *	lFileName;

	if	(
			(!pThis->GetSafeHwnd ())
		||	(!pThis->GetAgentFile ())
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
	if	(lFileName = pThis->GetAgentFile()->FindName (pLangID))
	{
		(*pCharacterDescription) = tBstrPtr (lFileName->mDesc1).Detach();
	}
	else
	{
		(*pCharacterDescription) = NULL;
		lResult = S_FALSE;
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::GetCharacterDescription"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE CAgentPreviewWnd::XDaPreview::GetCharacterGuid (GUID *pGuid)
{
	METHOD_PROLOGUE(CAgentPreviewWnd, DaPreview)
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::GetCharacterGuid"), pThis, pThis->m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(
			(!pThis->GetSafeHwnd ())
		||	(!pThis->GetAgentFile ())
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
		(*pGuid) = pThis->GetAgentFile()->GetGuid();
	}

#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%u)] CAgentPreviewWnd::XDaPreview::GetCharacterGuid"), pThis, pThis->m_dwRef);
	}
#endif
	return lResult;
}
