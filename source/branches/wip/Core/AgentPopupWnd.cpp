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
#include "EventNotify.h"
#include "AgentPopupWnd.h"
#include "AgentBalloonWnd.h"
#include "AgentListeningWnd.h"
#include "AgentStreamInfo.h"
#include "DirectShowSource.h"
#include "DirectShowRender.h"
#include "DirectSoundLipSync.h"
#include "DaGlobalConfig.h"
#include "QueuedPrepare.h"
#include "FileDownload.h"
#include "VfwErr.h"
#include "StringArrayEx.h"
#include "MallocPtr.h"
#include "Sapi5Voice.h"
#include "Sapi5Err.h"
#include "ThreadSecurity.h"
#include "ImageTools.h"
#ifndef	_WIN64
#include "Sapi4Voice.h"
#include "Sapi4Err.h"
#endif
#include "DebugStr.h"
#include "DebugWin.h"
#ifdef	_DEBUG
#include "ImageDebugger.h"
#include "DebugProcess.h"
#endif
#ifdef	_DEBUG_NOT
#include "DebugTime.h"
#endif

#ifdef	_DEBUG
//#define	_DEBUG_MOUSE		LogNormal
#define	_DEBUG_NOTIFY_LEVEL		(GetProfileDebugInt(_T("DebugNotifyLevel"),LogVerbose,true)&0xFFFF|LogTime)
#define	_DEBUG_ACTIVE			(GetProfileDebugInt(_T("DebugActive"),LogDetails,true)&0xFFFF|LogTime)
#define	_DEBUG_SPEECH			(GetProfileDebugInt(_T("DebugSpeech"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_DEBUG_SPEECH_EVENTS	(GetProfileDebugInt(_T("DebugSpeechEvents"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Popup"),LogDetails,true)&0xFFFF|LogTime)
#define	_LOG_QUEUE_OPS			(GetProfileDebugInt(_T("LogQueueOps"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_LOG_POPUP_OPS			MinLogLevel(GetProfileDebugInt(_T("LogPopupOps"),LogVerbose,true)&0xFFFF|LogTimeMs,_LOG_QUEUE_OPS)
//#define	_TRACE_RESOURCES		(GetProfileDebugInt(_T("TraceResources"),LogVerbose,true)&0xFFFF|LogHighVolume)
#endif

#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE		LogDetails
#endif
#ifndef	_LOG_QUEUE_OPS
#define	_LOG_QUEUE_OPS		LogDetails
#endif
#ifndef	_LOG_POPUP_OPS
#define	_LOG_POPUP_OPS		LogDetails
#endif

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DLL_OBJECT(CAgentPopupWnd)

CAgentPopupWnd::CAgentPopupWnd ()
:	mIsDragging (false),
	mWasDragged (false)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CAgentPopupWnd::CAgentPopupWnd (%d)"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
	ResetBkColor ();
}

CAgentPopupWnd::~CAgentPopupWnd ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CAgentPopupWnd::~CAgentPopupWnd (%d) [%p] [%d]"), this, max(m_dwRef,-1), _AtlModule.GetLockCount(), m_hWnd, ::IsWindow(m_hWnd));
	}
#endif

	mNotifyIcon.Remove ();
	Detach (-1, NULL);

#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CAgentPopupWnd::~CAgentPopupWnd (%d) Done"), this, max(m_dwRef,-1), _AtlModule.GetLockCount());
	}
#endif
}

CAgentPopupWnd * CAgentPopupWnd::CreateInstance ()
{
	CComObject<CAgentPopupWnd> *	lInstance = NULL;
	LogComErr (LogIfActive|LogTime, CComObject<CAgentPopupWnd>::CreateInstance (&lInstance));
	return lInstance;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentPopupWnd::FinalRelease ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CAgentPopupWnd::FinalRelease [%u] [%u]"), this, max(m_dwRef,-1), IsInNotify(), IsQueueBusy());
	}
#endif
	Close ();
}

void CAgentPopupWnd::OnFinalMessage (HWND)
{
	if	(
			(HasFinalReleased ())
		&&	(CanFinalRelease ())
		)
	{
#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CAgentPopupWnd::OnFinalMessage [%u] [%u]"), this, max(m_dwRef,-1), IsInNotify(), IsQueueBusy());
		}
#endif
		m_dwRef = 1;
		Release ();
	}
}

bool CAgentPopupWnd::CanFinalRelease ()
{
	if	(
			(IsInNotify() == 0)
		&&	(IsQueueBusy() == 0)
		)
	{
		return true;
	}
	else
	if	(IsWindow ())
	{
		PostMessage (NULL, 0, 0); // To ensure OnFinalMessage gets called
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentPopupWnd::_PreNotify ()
{
#ifdef	_DEBUG_NOTIFY_LEVEL
	LogMessage (_DEBUG_NOTIFY_LEVEL, _T("[%p(%d)] CAgentPopupWnd::_PreNotify [%u]"), this, max(m_dwRef,-1), IsInNotify());
#endif
	if	(m_dwRef > 0)
	{
		return CAgentCharacterWnd::_PreNotify ();
	}
	return false;
}

bool CAgentPopupWnd::_PostNotify ()
{
	CAgentCharacterWnd::_PostNotify ();
#ifdef	_DEBUG_NOTIFY_LEVEL
	LogMessage (_DEBUG_NOTIFY_LEVEL, _T("[%p(%d)] CAgentPopupWnd::_PostNotify [%u] HasFinalRelased [%u] CanFinalRelease [%u]"), this, max(m_dwRef,-1), IsInNotify(), HasFinalReleased(), CanFinalRelease());
#endif

	if	(
			(HasFinalReleased ())
		&&	(CanFinalRelease ())
		)
	{
#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CAgentPopupWnd PostNotify -> DestroyWindow"), this, max(m_dwRef,-1));
		}
#endif
		if	(IsWindow ())
		{
			DestroyWindow ();
		}
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////

int CAgentPopupWnd::_PreDoQueue ()
{
	return CAgentCharacterWnd::_PreDoQueue ();
}

int CAgentPopupWnd::_PostDoQueue ()
{
	int	lRet = CAgentCharacterWnd::_PostDoQueue ();

	if	(
			(HasFinalReleased ())
		&&	(CanFinalRelease ())
		)
	{
#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CAgentPopupWnd PostDoQueue -> DestroyWindow"), this, max(m_dwRef,-1));
		}
#endif
		if	(IsWindow ())
		{
			DestroyWindow ();
		}
		lRet = -1;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CAgentPopupWnd::GetWindow (HWND *phwnd)
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

HRESULT STDMETHODCALLTYPE CAgentPopupWnd::ContextSensitiveHelp (BOOL fEnterMode)
{
	return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentPopupWnd::Create (CWindow * pParentWnd, CRect * pInitialRect, DWORD pExStyle)
{
	bool				lRet = false;
	HWND				lParentWnd = (pParentWnd) ? pParentWnd->m_hWnd : NULL;
	CRect				lInitialRect (0,0,0,0);
	DWORD				lStyle = WS_POPUP;
	CAgentFile *		lAgentFile;
	CAgentFileName *	lAgentFileName;

	CThreadSecurity::AllowUiPiMessage (CAgentNotifyIcon::mTaskbarCreatedMsg);

	if	(pInitialRect)
	{
		lInitialRect.CopyRect (pInitialRect);
		mAutoSize = false;
#ifdef	_DEBUG_NOT
		lStyle |= WS_DLGFRAME;
#endif
	}
	else
	{
		mAutoSize = true;
#ifdef	_DEBUG_NOT
		lStyle |= WS_CAPTION | WS_THICKFRAME;
#endif
	}
#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p(%d)] CAgentPopupWnd::Create [%p]"), this, max(m_dwRef,-1), m_hWnd);
	}
#endif

	if	(CAgentWnd::Create (lParentWnd, lInitialRect, NULL, lStyle, pExStyle|WS_EX_LAYERED))
	{
		ModifyStyle (WS_CAPTION|WS_THICKFRAME|WS_SYSMENU, 0, SWP_FRAMECHANGED);

		if	(
				(lAgentFile = GetAgentFile ())
			&&	(lAgentFileName = lAgentFile->FindName ())
			)
		{
			SetWindowText (CAtlString ((BSTR)lAgentFileName->mName));
		}
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CAgentPopupWnd::Create [%p] Parent [%p] [%p] Owner [%p]"), this, max(m_dwRef,-1), m_hWnd, lParentWnd, ::GetParent(m_hWnd), ::GetWindow(m_hWnd, GW_OWNER));
		}
#endif
		lRet = true;
	}
	else
	if	(LogIsActive ())
	{
		LogWinErrAnon (LogAlways, GetLastError(), _T("CAgentPopupWnd::Create"));
	}

#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p(%d)] CAgentPopupWnd::Create [%p] Done"), this, max(m_dwRef,-1), m_hWnd);
	}
#endif
	return lRet;
}

LRESULT CAgentPopupWnd::OnDestroy (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT	lResult = 0;
	mNotifyIcon.Remove ();
	bHandled = CAgentCharacterWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult);
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentPopupWnd::Opened ()
{
	mAutoSize = false;
	CAgentCharacterWnd::Opened ();
	UpdateNotifyIcon ();
}

void CAgentPopupWnd::Closing ()
{
	CAgentCharacterWnd::Closing ();
	mNotifyIcon.Remove ();
}

DWORD CAgentPopupWnd::GetAlphaSmoothing () const
{
	DWORD	lRet = CAgentCharacterWnd::GetAlphaSmoothing ();

	if	(!IsEqualGUID (mVideoRenderType, MEDIASUBTYPE_RGB8))
	{
		lRet |= 0x80000000;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentPopupWnd::Attach (long pCharID, CEventNotify * pNotify, const CAgentIconData * pIconData, bool pSetActiveCharID)
{
	bool	lRet = false;
	long	lPrevCharID = mCharID;

#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p(%d)(%d)] CAgentPopupWnd::Attach [%d] [%u]"), this, mCharID, max(m_dwRef,-1), pCharID, pSetActiveCharID);
	}
#endif
	lRet = CAgentCharacterWnd::Attach (pCharID, pNotify, pIconData, pSetActiveCharID);

	if	(
			(pSetActiveCharID)
		&&	(mNotifyIcon.Attach (pCharID, pIconData))
		)
	{
		lRet = true;
	}

	if	(
			(pSetActiveCharID)
		&&	(lPrevCharID != pCharID)
		)
	{
		UpdateNotifyIcon ();

		if	(mNotify.GetCount() > 0)
		{
//
//	Do the activation notifications twice - first for inactive, then for active.  It's important that the
//	inactive notifications are sent first so an application that is going inactive has a chance to clean
//	up before the next application becomes active.
//
			INT_PTR			lNotifyNdx;
			CEventNotify *	lNotify;
			long			lInputInactiveCharID = 0;
			long			lInputActiveCharID = 0;

			if	(lPrevCharID > 0)
			{
				if	(
						(GetLastActive() == m_hWnd)
					&&	(IsCharShown ())
					)
				{
					lInputInactiveCharID = lPrevCharID;
				}
#ifdef	_DEBUG_ACTIVE
				if	(LogIsActive (_DEBUG_ACTIVE))
				{
					LogMessage (_DEBUG_ACTIVE, _T("[%p(%d)(%d)] SetInactive [%d] InputActive [%d] (Attach)"), this, mCharID, max(m_dwRef,-1), lPrevCharID, lInputInactiveCharID);
				}
#endif
				for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
				{
					lNotify->ActiveCharacterNotify (0, 0, lPrevCharID, lInputInactiveCharID);
				}
			}

			if	(
					(GetLastActive() == m_hWnd)
				&&	(IsCharShown ())
				)
			{
				lInputActiveCharID = mCharID;
			}
#ifdef	_DEBUG_ACTIVE
			if	(LogIsActive (_DEBUG_ACTIVE))
			{
				LogMessage (_DEBUG_ACTIVE, _T("[%p(%d)(%d)] SetActive [%d] InputActive [%d] (Attach)"), this, mCharID, max(m_dwRef,-1), pCharID, lInputActiveCharID);
			}
#endif
			for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
			{
				lNotify->ActiveCharacterNotify (mCharID, lInputActiveCharID, 0, 0);
			}
			for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
			{
				if	(lNotify->ActiveCharacterChanged (mCharID, lInputActiveCharID, lPrevCharID, lInputInactiveCharID))
				{
					break;
				}
			}
		}
	}

#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p(%d)(%d)] CAgentPopupWnd::Attach [%d] [%u] Done"), this, mCharID, max(m_dwRef,-1), pCharID, pSetActiveCharID);
	}
#endif
	return lRet;
}

bool CAgentPopupWnd::Detach (long pCharID, CEventNotify * pNotify)
{
	bool	lRet = false;
	long	lPrevCharID = mCharID;

	try
	{
#ifdef	_TRACE_RESOURCES
		if	(LogIsActive (_TRACE_RESOURCES))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p(%d)(%d)] CAgentPopupWnd::Detach [%d]"), this, mCharID, max(m_dwRef,-1), pCharID);
		}
#endif
		lRet = CAgentCharacterWnd::Detach (pCharID, pNotify);

		if	(
				(lPrevCharID == pCharID)
			&&	(mNotify.GetCount() > 0)
			)
		{
			INT_PTR			lNotifyNdx;
			CEventNotify *	lNotify;
			long			lInputActiveCharID = 0;

			if	(
					(GetLastActive() == m_hWnd)
				&&	(IsCharShown ())
				)
			{
				lInputActiveCharID = pCharID;
			}

#ifdef	_DEBUG_ACTIVE
			if	(LogIsActive (_DEBUG_ACTIVE))
			{
				LogMessage (_DEBUG_ACTIVE, _T("[%p(%d)(%d)] SetInactive [%d] InputActive [%d] (Detach)"), this, mCharID, max(m_dwRef,-1), pCharID, lInputActiveCharID);
			}
#endif
			for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
			{
				lNotify->ActiveCharacterNotify (-1, -1, pCharID, lInputActiveCharID);
			}
			for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
			{
				if	(lNotify->ActiveCharacterChanged (-1, -1, pCharID, lInputActiveCharID))
				{
					break;
				}
			}
		}

		if	(mNotifyIcon.Detach (pCharID))
		{
			lRet = true;
		}

#ifdef	_TRACE_RESOURCES
		if	(LogIsActive (_TRACE_RESOURCES))
		{
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p(%d)(%d)] CAgentPopupWnd::Detach [%d] Done"), this, mCharID, max(m_dwRef,-1), pCharID);
		}
#endif
	}
	catch AnyExceptionSilent

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentPopupWnd::IsLastActive (bool pLastActive)
{
	CAgentCharacterWnd::IsLastActive (pLastActive);

	UpdateNotifyIcon ();
	if	(pLastActive)
	{
		BringWindowToTop ();
	}
}

bool CAgentPopupWnd::IsDragging () const
{
	if	(
			(IsWindow ())
		&&	(::GetCapture() == m_hWnd)
		&&	(mIsDragging)
		)
	{
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentPopupWnd::ShowPopup (long pForCharID, VisibilityCauseType pVisiblityCause, bool pAlwaysNotify)
{
	bool			lRet = false;
	bool			lIsPlaying = IsPlaying ();
	CAgentFile *	lAgentFile;

	if	(pForCharID <= 0)
	{
		pForCharID = mCharID;
	}

#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p(%d)(%d)] CAgentPopupWnd::ShowPopup [%p] for [%d]"), this, mCharID, max(m_dwRef,-1), m_hWnd, pForCharID);
	}
#endif
#ifdef	_LOG_POPUP_OPS
	if	(LogIsActive (_LOG_POPUP_OPS))
	{
		CRect lWinRect;
		GetWindowRect (&lWinRect);
		LogMessage (_LOG_POPUP_OPS, _T("[%p(%d)(%d)] ShowPopup for [%d] visible [%d %d] cause [%d] at [%d %d %d %d (%d %d)]"), this, mCharID, max(m_dwRef,-1), pForCharID, IsWindowVisible(), IsCharShown(), pVisiblityCause, lWinRect.left, lWinRect.top, lWinRect.right, lWinRect.bottom, lWinRect.Width(), lWinRect.Height());
	}
#endif

	if	(
			(IsWindow ())
		&&	(!IsWindowVisible ())
		)
	{
		if	(lIsPlaying)
		{
			Pause ();
		}

		ModifyStyleEx (0, WS_EX_LAYERED);
		::SetLayeredWindowAttributes (m_hWnd, 0, 255, LWA_ALPHA);

		if	(lAgentFile = GetAgentFile())
		{
			CRect					lWindowRect;
			CMemDCHandle			lDC;
			tS <CAgentFileImage>	lImage;
			UINT					lImageFormatSize;
			tPtr <BITMAPINFO>		lImageFormat;
			LPVOID					lBitmapBits;
			ATL::CImage				lBitmap;
			HGDIOBJ					lOldBitmap;

			ModifyStyleEx (WS_EX_LAYERED, 0);
			ModifyStyleEx (0, WS_EX_LAYERED);

			GetWindowRect (&lWindowRect);
			lImage.mImageSize = lWindowRect.Size ();

			if	(
					(
						(GetAlphaSmoothing())
					||	(mBkColor)
					)
				&&	(lDC.Attach (CreateCompatibleDC (NULL)))
				&&	(lImageFormatSize = lAgentFile->GetImageFormat (NULL, &lImage, (GetAlphaSmoothing()?true:false)))
				&&	(lImageFormat = (LPBITMAPINFO)(new BYTE [lImageFormatSize]))
				&&	(lAgentFile->GetImageFormat (lImageFormat, &lImage, (GetAlphaSmoothing()?true:false)))
				)
			{
				BYTE			lTransparency = lAgentFile->GetTransparency();
				BLENDFUNCTION	lBlend = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};

				if	(!GetAlphaSmoothing())
				{
					SetPaletteBkColor (lImageFormat, lTransparency, *mBkColor);
				}

				lBitmap.Attach (CreateDIBSection (NULL, lImageFormat, DIB_RGB_COLORS, NULL, NULL, 0));
				if	(lBitmapBits = GetImageBits (lBitmap))
				{
					if	(GetAlphaSmoothing())
					{
						memset (lBitmapBits, 0, lImageFormat->bmiHeader.biSizeImage);
					}
					else
					{
						memset (lBitmapBits, lTransparency, lImageFormat->bmiHeader.biSizeImage);
					}
					GdiFlush ();
					lOldBitmap = ::SelectObject (lDC, lBitmap);

					SetLastError (0);
					if	(
							(GetAlphaSmoothing())
						?	(!::UpdateLayeredWindow (m_hWnd, NULL, &lWindowRect.TopLeft(), &lWindowRect.Size(), lDC, &CPoint(0,0), 0, &lBlend, ULW_ALPHA))
						:	(!::UpdateLayeredWindow (m_hWnd, NULL, &lWindowRect.TopLeft(), &lWindowRect.Size(), lDC, &CPoint(0,0), *mBkColor, NULL, ULW_COLORKEY))
						)
					{
						LogWinErr (LogVerbose|LogTime, GetLastError(), _T("UpdateLayeredWindow"));
					}
					::SelectObject (lDC, lOldBitmap);

					if	(!GetAlphaSmoothing())
					{
						::SetLayeredWindowAttributes (m_hWnd, (*mBkColor), 255, LWA_COLORKEY);
					}
				}
			}
		}
	}

	if	(IsWindow ())
	{
		if	(IsWindowVisible())
		{
			::SetWindowPos (m_hWnd, HWND_TOP, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOOWNERZORDER);
		}
		else
		{
			::SetWindowPos (m_hWnd, HWND_TOP, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_SHOWWINDOW|SWP_NOOWNERZORDER);
		}

		SetLastError(0);
		if	(
				(GetExStyle () & WS_EX_TOPMOST)
			&&	(!::SetWindowPos (m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOOWNERZORDER))
			)
		{
			LogWinErr (LogDebug, GetLastError());
		}

		if	(
				(!(GetExStyle () & WS_EX_LAYERED))
			||	(LogIsActive (LogDetails))
			)
		{
			CAtlString	lTitle;
			lTitle.Format (_T("[%p(%d)(%d)] CAgentPopupWnd::ShowPopup"), this, mCharID, max(m_dwRef,-1));
			LogWindow (LogIfActive|LogTime, m_hWnd, lTitle);
		}
	}

	if	(
			(IsWindow ())
		&&	(IsWindowVisible())
		)
	{
		UpdateNotifyIcon ();

		if	(lIsPlaying)
		{
			Resume ();
		}
		if	(
				(mListeningWnd)
			&&	(mListeningWnd->IsWindow ())
			)
		{
			mListeningWnd->PositionTipWnd ();
		}
		lRet = true;
	}

	if	(
			(lRet)
		||	(pAlwaysNotify)
		)
	{
		NotifyShown (pForCharID, pVisiblityCause);
	}

	if	(
			(IsWindow ())
		&&	(IsWindowVisible ())
		)
	{
#ifdef	_DEBUG_NOT
		LogWindow (LogDebug, m_hWnd);
#endif
		SetLastActive ();
	}

#ifdef	_LOG_POPUP_OPS
	if	(
			(!lRet)
		&&	(LogIsActive (_LOG_POPUP_OPS))
		)
	{
		LogMessage (_LOG_POPUP_OPS, _T("[%p(%d)(%d)] ShowPopup false"), this, mCharID, max(m_dwRef,-1));
	}
#endif
#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p(%d)(%d)] CAgentPopupWnd::ShowPopup [%p] [%d] Done"), this, mCharID, max(m_dwRef,-1), m_hWnd, pForCharID);
	}
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentPopupWnd::HidePopup (long pForCharID, VisibilityCauseType pVisiblityCause, bool pAlwaysNotify)
{
	bool	lRet = false;

	if	(pForCharID <= 0)
	{
		pForCharID = mCharID;
	}

#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p(%d)(%d)] CAgentPopupWnd::HidePopup [%p] for [%d]"), this, mCharID, max(m_dwRef,-1), m_hWnd, pForCharID);
	}
#endif
#ifdef	_LOG_POPUP_OPS
	if	(LogIsActive (_LOG_POPUP_OPS))
	{
		LogMessage (_LOG_POPUP_OPS, _T("[%p(%d)(%d)] HidePopup for [%d] visible [%d %d] cause [%d]"), this, mCharID, max(m_dwRef,-1), pForCharID, IsWindowVisible(), IsCharShown(), pVisiblityCause);
	}
#endif

	StopIdle (_T("HidePopup"));

	if	(
			(mBalloonWnd)
		&&	(mBalloonWnd->IsWindow ())
		)
	{
		mBalloonWnd->HideBalloon ();
	}

	if	(
			(IsWindow ())
		&&	(IsWindowVisible())
		)
	{
		ShowWindow (SW_HIDE);
		lRet =  true;
	}

	UpdateNotifyIcon ();

	if	(
			(mListeningWnd)
		&&	(mListeningWnd->IsWindow ())
		)
	{
		mListeningWnd->RefreshTipWnd ();
		if	(mListeningWnd->GetCharID() == mCharID)
		{
			mListeningWnd->ShowTipWnd ();
		}
		mListeningWnd->PositionTipWnd ();
	}

	if	(
			(lRet)
		||	(pAlwaysNotify)
		)
	{
#ifdef	_DEBUG_ACTIVE
		if	(
				(lRet)
			&&	(LogIsActive (_DEBUG_ACTIVE))
			&&	(GetLastActive() == m_hWnd)
			)
		{
			LogMessage (_DEBUG_ACTIVE, _T("[%p(%d)(%d)] SetNotInputActive (Hide)"), this, mCharID, max(m_dwRef,-1));
		}
#endif
		NotifyHidden (pForCharID, pVisiblityCause);
	}

	if	(GetLastActive() == m_hWnd)
	{
		mLastActive = NULL;
	}
#ifdef	_LOG_POPUP_OPS
	if	(
			(!lRet)
		&&	(LogIsActive (_LOG_POPUP_OPS))
		)
	{
		LogMessage (_LOG_POPUP_OPS, _T("[%p(%d)(%d)] HidePopup false"), this, mCharID, max(m_dwRef,-1));
	}
#endif
#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p(%d)(%d)] CAgentPopupWnd::HidePopup [%p] [%d] Done"), this, mCharID, max(m_dwRef,-1), m_hWnd, pForCharID);
	}
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentPopupWnd::MovePopup (const CPoint & pPosition, long pForCharID, MoveCauseType pMoveCause, bool pAlwaysNotify)
{
	bool	lRet = false;

	if	(pForCharID <= 0)
	{
		pForCharID = mCharID;
	}

	if	(IsWindow ())
	{
		CRect	lWinRect;

		GetWindowRect (&lWinRect);
		if	(
				(lWinRect.left != pPosition.x)
			||	(lWinRect.top != pPosition.y)
			)
		{
			lWinRect.OffsetRect (pPosition.x - lWinRect.left, pPosition.y - lWinRect.top);
			MoveWindow (&lWinRect);
			lRet = true;
		}

		if	(
				(
					(lRet)
				||	(pAlwaysNotify)
				)
			&&	(pMoveCause != MoveCause_NeverMoved)
			&&	(PreNotify ())
			)
		{
			try
			{
				INT_PTR				lNotifyNdx;
				CEventNotify *		lNotify;
				long				lNotifyCharID;
				MoveCauseType		lMoveCause;

				for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
				{
					lNotifyCharID = lNotify->GetNotifyClient (mCharID);
					lMoveCause = ((pMoveCause==MoveCause_ProgramMoved) && (lNotifyCharID!=pForCharID)) ? MoveCause_OtherProgramMoved : pMoveCause;

					lNotify->_PutMoveCause (lNotifyCharID, lMoveCause);
					lNotify->Move (lNotifyCharID, pPosition.x, pPosition.y, lMoveCause);
				}
			}
			catch AnyExceptionDebug
			PostNotify ();
		}
	}
	return lRet;
}

bool CAgentPopupWnd::SizePopup (const CSize & pSize, long pForCharID, bool pAlwaysNotify)
{
	bool	lRet = false;

	if	(pForCharID <= 0)
	{
		pForCharID = mCharID;
	}

	if	(
			(IsWindow ())
		&&	(pSize.cx > 0)
		&&	(pSize.cy > 0)
		)
	{
		CRect	lWinRect;

		GetWindowRect (&lWinRect);
		if	(
				(pSize.cx != lWinRect.Width())
			||	(pSize.cy != lWinRect.Height())
			)
		{
			lWinRect.right = lWinRect.left + pSize.cx;
			lWinRect.bottom = lWinRect.top + pSize.cy;
			MoveWindow (&lWinRect);
			AutoSizeVideo (false);
			if	(IsWindowVisible ())
			{
				RedrawWindow ();
			}
			lRet = true;
		}

		if	(
				(
					(lRet)
				||	(pAlwaysNotify)
				)
			&&	(PreNotify ())
			)
		{
			try
			{
				INT_PTR			lNotifyNdx;
				CEventNotify *	lNotify;

				for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
				{
					lNotify->Size (lNotify->GetNotifyClient (mCharID), pSize.cx, pSize.cy);
				}
			}
			catch AnyExceptionDebug
			PostNotify ();
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentPopupWnd::ShowQueued (CQueuedShow * pQueuedShow)
{
	bool	lRet = false;

	if	(!IsWindowVisible ())
	{
		CAgentStreamInfo *	lStreamInfo;
		long				lSequenceDuration = 0;

		if	(
				(!pQueuedShow->mFast)
			||	(
					(lStreamInfo = GetAgentStreamInfo())
				&&	(lStreamInfo->GetSequenceDuration (&lSequenceDuration) == S_OK)
				&&	(lSequenceDuration <= 0)
				)
			)
		{
			Stop ();
		}
		lRet = ShowPopup (pQueuedShow->mCharID, (pQueuedShow->mVisibilityCause > 0) ? (VisibilityCauseType)(pQueuedShow->mVisibilityCause) : VisibilityCause_ProgramShowed);
	}
	if	(IsWindowVisible ())
	{
		mCharShown = true;
	}
	return lRet;
}

bool CAgentPopupWnd::HideQueued (CQueuedHide * pQueuedHide)
{
	bool	lRet = false;

	if	(IsWindowVisible ())
	{
		lRet = HidePopup (pQueuedHide->mCharID, (pQueuedHide->mVisibilityCause > 0) ? (VisibilityCauseType)(pQueuedHide->mVisibilityCause) : VisibilityCause_ProgramHid);
	}
	mCharShown = false;
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

long CAgentPopupWnd::QueueMove (long pCharID, const CPoint & pPosition, DWORD pSpeed)
{
	long			lReqID = 0;
	CQueuedMove *	lQueuedMove = NULL;

	if	(
			(IsWindow ())
		&&	(lQueuedMove = new CQueuedMove (pCharID, lReqID=NextReqID()))
		)
	{
		lQueuedMove->mPosition = pPosition;
		lQueuedMove->mTimeAllowed = pSpeed;
		mQueue.AddTail (lQueuedMove);
	}
	else
	{
		lReqID = 0;
	}

#ifdef	_LOG_QUEUE_OPS
	if	(LogIsActive (_LOG_QUEUE_OPS))
	{
		LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)(%d)] QueueMove [%p(%d)] to [%d %d] speed [%u]"), this, max(m_dwRef,-1), pCharID, lQueuedMove, lReqID, lQueuedMove->mPosition.x, lQueuedMove->mPosition.y, lQueuedMove->mTimeAllowed);
	}
#endif
	return lReqID;
}

bool CAgentPopupWnd::DoQueuedMoveCycle ()
{
	bool			lRet = false;
	CQueuedMove *	lQueuedMove;

	if	(
			(lQueuedMove = (CQueuedMove *) mQueue.GetNextAction (QueueActionMove))
		&&	(lQueuedMove->IsCycling ())
		&&	(PreDoQueue () > 0)
		)
	{
		lQueuedMove->Cycle (mQueue, this);
		PostDoQueue ();
	}
	return lRet;
}


/////////////////////////////////////////////////////////////////////////////

bool CAgentPopupWnd::CanDoAnimationQueue ()
{
	if	(
			(!IsQueuePaused ())
		&&	(!mQueue.IsEmpty ())
		&&	(DoQueuedMoveCycle ())
		)
	{
		return false;
	}
	return CAgentCharacterWnd::CanDoAnimationQueue ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentPopupWnd::OnMouseActivate (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT lResult = 0;

	if	(!(bHandled = CAgentCharacterWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult)))
	{
		bHandled = TRUE;
		lResult = DefWindowProc ();
	}
	SetActiveWindow ();
	return lResult;
}

LRESULT CAgentPopupWnd::OnActivate (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT lResult = 0;

	if	(!(bHandled = CAgentCharacterWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult)))
	{
		bHandled = TRUE;
		lResult = DefWindowProc ();
	}
	if	(wParam)
	{
#ifdef	_DEBUG_ACTIVE
		if	(LogIsActive (_DEBUG_ACTIVE))
		{
			LogMessage (_DEBUG_ACTIVE, _T("[%p(%d)(%d)] CAgentPopupWnd OnActivate Activate [%p] Last [%p]"), this, mCharID, max(m_dwRef,-1), m_hWnd, mLastActive);
		}
#endif
		SetLastActive ();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentPopupWnd::OnNcHitTest (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT	lResult = 0;

	if	(!(bHandled = CAgentCharacterWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult)))
	{
		bHandled = TRUE;
		lResult = DefWindowProc ();
	}
	if	(lResult == HTCLIENT)
	{
		lResult = HTCAPTION;
	}
	return lResult;
}

LRESULT CAgentPopupWnd::OnNcLButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT	lResult = 0;
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnNcLButtonDown [%d %d]"), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
#endif
	mWasDragged = false;
	if	(!(bHandled = CAgentCharacterWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult)))
	{
		bHandled = TRUE;
		lResult = DefWindowProc ();
	}

#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnNcLButtonDown [%d %d] WasDragged [%u]"), GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam), mWasDragged);
#endif
	if	(!mWasDragged)
	{
		NotifyClick (MK_LBUTTON, CPoint (GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
	}
	mLastButtonMsg = WM_LBUTTONDOWN;
	return lResult;
}

LRESULT CAgentPopupWnd::OnNcLButtonUp (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT	lResult = 0;
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnNcLButtonUp [%d %d]"), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
#endif
	if	(!(bHandled = CAgentCharacterWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult)))
	{
		bHandled = TRUE;
		lResult = DefWindowProc ();
	}
	mLastButtonMsg = WM_LBUTTONUP;
	return lResult;
}

LRESULT CAgentPopupWnd::OnNcLButtonDblClk (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnNcLButtonDblClk [%d %d]"), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
#endif
	return OnLButtonDblClk (uMsg, wParam, lParam, bHandled);
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentPopupWnd::OnNcRButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnNcRButtonDown [%d %d]"), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
#endif
	return OnRButtonDown (uMsg, wParam, lParam, bHandled);
}

LRESULT CAgentPopupWnd::OnNcRButtonUp (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT	lResult = 0;
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnNcRButtonUp [%d %d]"), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
#endif
	if	(!(bHandled = CAgentCharacterWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult)))
	{
		bHandled = TRUE;
		lResult = DefWindowProc ();
	}
	if	(mLastButtonMsg != WM_RBUTTONDBLCLK)
	{
		NotifyClick (MK_RBUTTON, CPoint (GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		PostMessage (WM_CONTEXTMENU, (WPARAM)m_hWnd, lParam);
	}
	mLastButtonMsg = WM_RBUTTONUP;
	return lResult;
}

LRESULT CAgentPopupWnd::OnNcRButtonDblClk (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnNcRButtonDblClk [%d %d]"), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
#endif
	return OnRButtonDblClk (uMsg, wParam, lParam, bHandled);
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentPopupWnd::OnNcMButtonDown (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnNcMButtonDown [%d %d]"), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
#endif
	return OnMButtonDown (uMsg, wParam, lParam, bHandled);
}

LRESULT CAgentPopupWnd::OnNcMButtonUp (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnNcMButtonUp [%d %d]"), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
#endif
	return OnMButtonUp (uMsg, wParam, lParam, bHandled);
}

LRESULT CAgentPopupWnd::OnNcMButtonDblClk (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnNcMButtonDblClk [%d %d]"), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
#endif
	return OnMButtonDblClk (uMsg, wParam, lParam, bHandled);
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentPopupWnd::OnEnterSizeMove (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT	lResult = 0;
	CRect	lWinRect;

	GetWindowRect (&lWinRect);
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnEnterSizeMove [%d %d]"), lWinRect.left, lWinRect.top);
#endif
	mSizeMoveStart = new CPoint (lWinRect.left, lWinRect.top);
	mIsDragging = false;

	bHandled = CAgentCharacterWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult);
	return lResult;
}

LRESULT CAgentPopupWnd::OnExitSizeMove (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT	lResult = 0;
	CRect	lWinRect;

	if	(
			(mBalloonWnd)
		&&	(mBalloonWnd->IsWindow ())
		)
	{
		mBalloonWnd->MoveBalloon ();
	}
	if	(
			(mListeningWnd)
		&&	(mListeningWnd->IsWindow ())
		)
	{
		mListeningWnd->PositionTipWnd ();
	}

	GetWindowRect (&lWinRect);
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnExitSizeMove [%d %d] Dragging [%u]"), lWinRect.left, lWinRect.top, mIsDragging);
#endif
	if	(PreNotify ())
	{
		try
		{
			INT_PTR			lNotifyNdx;
			CEventNotify *	lNotify;
			long			lNotifyCharID;

			if	(mIsDragging)
			{
				if	(lNotify = GetNotifyClientNotify (mCharID))
				{
					lNotify->DragComplete (mCharID, NotifyKeyState(), lWinRect.left, lWinRect.top);
				}
			}
			if	(
					(mSizeMoveStart)
				&&	(
						(lWinRect.left != mSizeMoveStart->x)
					||	(lWinRect.top != mSizeMoveStart->y)
					)
				)
			{
				for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
				{
					lNotifyCharID = lNotify->GetNotifyClient (mCharID);
					lNotify->_PutMoveCause (lNotifyCharID, MoveCause_UserMoved);
					lNotify->Move (lNotifyCharID, lWinRect.left, lWinRect.top, MoveCause_UserMoved);
				}
			}
		}
		catch AnyExceptionDebug
		PostNotify ();
	}
	mSizeMoveStart = NULL;
	mWasDragged = mIsDragging;
	mIsDragging = false;

	bHandled = CAgentCharacterWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult);
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentPopupWnd::OnMoving (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT	lResult = 0;
	LPCRECT	lRect = (LPCRECT)lParam;

#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnMoving [%d %d]"), lRect->left, lRect->top);
#endif
	if	(
			(mSizeMoveStart)
		&&	(!mIsDragging)
		&&	(
				(lRect->left != mSizeMoveStart->x)
			||	(lRect->top != mSizeMoveStart->y)
			)
		)
	{
		mIsDragging = true;

		if	(PreNotify ())
		{
			try
			{
				CEventNotify *	lNotify;

				if	(lNotify = GetNotifyClientNotify (mCharID))
				{
					lNotify->DragStart (mCharID, NotifyKeyState(), mSizeMoveStart->x, mSizeMoveStart->y);
				}
			}
			catch AnyExceptionDebug
			PostNotify ();
		}
	}

	bHandled = CAgentCharacterWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult);
	return lResult;
}

LRESULT CAgentPopupWnd::OnMove (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT	lResult = 0;

	if	(!(bHandled = CAgentCharacterWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult)))
	{
		bHandled = TRUE;
		lResult = DefWindowProc ();
	}

	if	(::GetCapture() != m_hWnd)
	{
		if	(
				(mBalloonWnd)
			&&	(mBalloonWnd->IsWindow ())
			)
		{
			mBalloonWnd->MoveBalloon ();
		}
		if	(
				(mListeningWnd)
			&&	(mListeningWnd->IsWindow ())
			)
		{
			mListeningWnd->PositionTipWnd ();
		}
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentPopupWnd::IsNotifyIconValid () const
{
	return mNotifyIcon.SafeIsValid ();
}

bool CAgentPopupWnd::IsIconVisible () const
{
	return mNotifyIcon.SafeIsVisible ();
}

bool CAgentPopupWnd::UpdateNotifyIcon (const CAgentIconData * pIconData)
{
	if	(mNotifyIcon.GetCharID() == GetCharID())
	{
		if	(pIconData)
		{
			mNotifyIcon.Attach (GetCharID(), pIconData);
		}
		return mNotifyIcon.ShowState (m_hWnd, GetAgentFile());
	}
	return false;
}

bool CAgentPopupWnd::SetNotifyIconTip (const CAgentIconData * pIconData, CAgentFile * pAgentFile, LANGID pLangID)
{
	return mNotifyIcon.SetIconTip (pIconData, pAgentFile, pLangID);
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentPopupWnd::OnNotifyIcon (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	UINT	lButtonMsg = mNotifyIcon.OnNotifyIcon (m_hWnd, wParam, lParam);

	if	(lButtonMsg)
	{
		CPoint	lCursorPos;

		GetCursorPos (&lCursorPos);

		switch (lButtonMsg)
		{
			case WM_LBUTTONUP:
			{
				if	(mLastButtonMsg != OCM__BASE+WM_LBUTTONDBLCLK)
				{
					NotifyClick (MK_LBUTTON|0x1000, lCursorPos);
				}
			}	break;
			case WM_LBUTTONDBLCLK:
			{
				NotifyDblClick (MK_LBUTTON|0x1000, lCursorPos);
				OnIconDblClick (lCursorPos);
			}	break;
			case WM_RBUTTONUP:
			{
				if	(mLastButtonMsg != OCM__BASE+WM_RBUTTONDBLCLK)
				{
					NotifyClick (MK_RBUTTON|0x1000, lCursorPos);
					PostMessage (WM_CONTEXTMENU, (WPARAM)m_hWnd, MAKELPARAM(lCursorPos.x, lCursorPos.y));
				}
			}	break;
			case WM_RBUTTONDBLCLK:
			{
				NotifyDblClick (MK_RBUTTON|0x1000, lCursorPos);
			}	break;
			case WM_MBUTTONUP:
			{
				if	(mLastButtonMsg != OCM__BASE+WM_MBUTTONDBLCLK)
				{
					NotifyClick (MK_MBUTTON|0x1000, lCursorPos);
				}
			}	break;
			case WM_MBUTTONDBLCLK:
			{
				NotifyDblClick (MK_MBUTTON|0x1000, lCursorPos);
			}	break;
		}
		mLastButtonMsg = OCM__BASE+lButtonMsg;
	}
	return 0;
}

void CAgentPopupWnd::OnIconDblClick (const CPoint & pPoint)
{
//
//	MS Agent leaves default command processing to the calling application
//
#ifndef	_STRICT_COMPATIBILITY
	if	(PreNotify ())
	{
		try
		{
			CEventNotify *	lNotify;

			if	(lNotify = GetNotifyClientNotify (mCharID))
			{
				lNotify->_DefaultCommand (mCharID, m_hWnd, pPoint);
			}
		}
		catch AnyExceptionDebug
		PostNotify ();
	}
#endif
}

LRESULT CAgentPopupWnd::OnTaskbarCreated (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
	LRESULT	lResult = 0;

	mNotifyIcon.OnTaskbarCreated (m_hWnd, wParam, lParam);
	UpdateNotifyIcon ();
	bHandled = CAgentCharacterWnd::ProcessWindowMessage (m_hWnd, uMsg, wParam, lParam, lResult);
	return lResult;
}