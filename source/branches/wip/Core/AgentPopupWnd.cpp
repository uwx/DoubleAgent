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
#include "Elapsed.h"
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
#ifdef	_DEBUG
#include "ImageDebugger.h"
#include "DebugWin.h"
#include "DebugProcess.h"
#endif
#ifdef	_DEBUG_NOT
#include "DebugTime.h"
#endif

#ifdef	_DEBUG
//#define	_DEBUG_MOUSE		LogNormal
//#define	_DEBUG_ACTIVATE		LogNormal
#define	_DEBUG_SPEECH			(GetProfileDebugInt(_T("DebugSpeech"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_DEBUG_SPEECH_EVENTS	(GetProfileDebugInt(_T("DebugSpeechEvents"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Popup"),LogDetails,true)&0xFFFF)
#define	_LOG_POPUP_OPS			(GetProfileDebugInt(_T("LogPopupOps"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_LOG_QUEUE_OPS			(GetProfileDebugInt(_T("LogQueueOps"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
//#define	_TRACE_RESOURCES		(GetProfileDebugInt(_T("TraceResources"),LogVerbose,true)&0xFFFF|LogHighVolume)
#endif

#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE		LogDetails
#endif
#ifndef	_LOG_POPUP_OPS
#define	_LOG_POPUP_OPS		LogDetails
#endif
#ifndef	_LOG_QUEUE_OPS
#define	_LOG_QUEUE_OPS		LogDetails
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
	SetBkColor (0x00040804);
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
	LogComErr (LogIfActive, CComObject<CAgentPopupWnd>::CreateInstance (&lInstance));
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

bool CAgentPopupWnd::Create (HWND pParentWnd, CRect * pInitialRect)
{
	bool				lRet = false;
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
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentPopupWnd::Create [%p]"), this, m_hWnd);
	}
#endif

	if	(CAgentWnd::Create (pParentWnd, lInitialRect, NULL, lStyle, WS_EX_TOPMOST|WS_EX_LAYERED))
	{
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
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CAgentPopupWnd::Create [%p] Parent [%p] [%p] Owner [%p]"), this, max(m_dwRef,-1), m_hWnd, pParentWnd, ::GetParent(m_hWnd), ::GetWindow(m_hWnd, GW_OWNER));
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
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentPopupWnd::Create [%p] Done"), this, m_hWnd);
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
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentPopupWnd::Attach [%d] [%u]"), this, pCharID, pSetActiveCharID);
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
#ifdef	_DEBUG_ACTIVATE
				LogMessage (_DEBUG_ACTIVATE, _T("[%p(%d)] SetInactive [%d] InputActive [%d] (Attach)"), this, max(m_dwRef,-1), lPrevCharID, lInputInactiveCharID);
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
#ifdef	_DEBUG_ACTIVATE
			LogMessage (_DEBUG_ACTIVATE, _T("[%p(%d)] SetActive [%d] InputActive [%d] (Attach)"), this, max(m_dwRef,-1), mCharID, lInputActiveCharID);
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
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentPopupWnd::Attach [%d] [%u] Done"), this, pCharID, pSetActiveCharID);
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
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentPopupWnd::Detach [%d]"), this, pCharID);
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

#ifdef	_DEBUG_ACTIVATE
			LogMessage (_DEBUG_ACTIVATE, _T("[%p(%d)] SetInactive [%d] InputActive [%d] (Detach)"), this, max(m_dwRef,-1), pCharID, lInputActiveCharID);
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
			CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentPopupWnd::Detach [%d] Done"), this, pCharID);
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

/////////////////////////////////////////////////////////////////////////////

bool CAgentPopupWnd::_PreNotify ()
{
	if	(m_dwRef > 0)
	{
		return CAgentCharacterWnd::_PreNotify ();
	}
	return false;
}

bool CAgentPopupWnd::_PostNotify ()
{
	CAgentCharacterWnd::_PostNotify ();
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
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentPopupWnd::ShowPopup [%p] [%d]"), this, m_hWnd, pForCharID);
	}
#endif
#ifdef	_LOG_POPUP_OPS
	if	(LogIsActive (_LOG_POPUP_OPS))
	{
		CRect lWinRect;
		GetWindowRect (&lWinRect);
		LogMessage (_LOG_POPUP_OPS, _T("[%p(%d)] [%d] ShowPopup for [%d] visible [%d %d] cause [%d] at [%d %d %d %d (%d %d)]"), this, max(m_dwRef,-1), mCharID, pForCharID, IsWindowVisible(), IsCharShown(), pVisiblityCause, lWinRect.left, lWinRect.top, lWinRect.right, lWinRect.bottom, lWinRect.Width(), lWinRect.Height());
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
			bool					l32BitSamples = false;

			if	(IsEqualGUID (mVideoRenderType, MEDIASUBTYPE_ARGB32))
			{
				l32BitSamples = true;
			}

			ModifyStyleEx (WS_EX_LAYERED, 0);
			ModifyStyleEx (0, WS_EX_LAYERED);

			GetWindowRect (&lWindowRect);
			lImage.mImageSize = lWindowRect.Size ();

			if	(
					(
						(l32BitSamples)
					||	(mBkColor)
					)
				&&	(lDC.Attach (CreateCompatibleDC (NULL)))
				&&	(lImageFormatSize = lAgentFile->GetImageFormat (NULL, &lImage, l32BitSamples))
				&&	(lImageFormat = (LPBITMAPINFO)(new BYTE [lImageFormatSize]))
				&&	(lAgentFile->GetImageFormat (lImageFormat, &lImage, l32BitSamples))
				)
			{
				BYTE			lTransparency = lAgentFile->GetTransparency();
				BLENDFUNCTION	lBlend = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};

				if	(!l32BitSamples)
				{
					SetPaletteBkColor (lImageFormat, lTransparency, *mBkColor);
				}

				lBitmap.Attach (CreateDIBSection (NULL, lImageFormat, DIB_RGB_COLORS, NULL, NULL, 0));
				if	(lBitmapBits = GetImageBits (lBitmap))
				{
					if	(l32BitSamples)
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
							(l32BitSamples)
						?	(!::UpdateLayeredWindow (m_hWnd, NULL, &lWindowRect.TopLeft(), &lWindowRect.Size(), lDC, &CPoint(0,0), 0, &lBlend, ULW_ALPHA))
						:	(!::UpdateLayeredWindow (m_hWnd, NULL, &lWindowRect.TopLeft(), &lWindowRect.Size(), lDC, &CPoint(0,0), *mBkColor, NULL, ULW_COLORKEY))
						)
					{
						LogWinErr (LogVerbose, GetLastError(), _T("UpdateLayeredWindow"));
					}
					::SelectObject (lDC, lOldBitmap);

					if	(!l32BitSamples)
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
			::SetWindowPos (m_hWnd, HWND_TOP, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
		}
		else
		{
			::SetWindowPos (m_hWnd, HWND_TOP, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_SHOWWINDOW);
		}

		SetLastError(0);
		if	(!::SetWindowPos (m_hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE))
		{
			LogWinErr (LogDebug, GetLastError());
		}

		if	(
				(!(GetExStyle () & WS_EX_TOPMOST))
			||	(!(GetExStyle () & WS_EX_LAYERED))
			||	(LogIsActive (LogDetails))
			)
		{
			LogMessage (LogIfActive, _T("[%p] CAgentPopupWnd::ShowPopup [%p] Parent [%p] Owner [%p] [%s %s %s %s]"), this, m_hWnd, ::GetParent(m_hWnd), ::GetWindow(m_hWnd, GW_OWNER), ((GetExStyle () & WS_EX_TOPMOST) ? _T("Topmost") : _T("NOT Topmost")), ((GetExStyle () & WS_EX_LAYERED) ? _T("Layered") : _T("NOT Layered")), ((GetExStyle () & WS_EX_TOOLWINDOW) ? _T("ToolWindow") : _T("NOT ToolWindow")), ((GetExStyle () & WS_EX_TRANSPARENT) ? _T("Transparent") : _T("NOT Transparent")));
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
		LogMessage (_LOG_POPUP_OPS, _T("[%p(%d)] [%d] ShowPopup false"), this, max(m_dwRef,-1), mCharID);
	}
#endif
#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentPopupWnd::ShowPopup [%p] [%d] Done"), this, m_hWnd, pForCharID);
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
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentPopupWnd::HidePopup [%p] [%d]"), this, m_hWnd, pForCharID);
	}
#endif
#ifdef	_LOG_POPUP_OPS
	if	(LogIsActive (_LOG_POPUP_OPS))
	{
		LogMessage (_LOG_POPUP_OPS, _T("[%p(%d)] [%d] HidePopup for [%d] visible [%d %d] cause [%d]"), this, max(m_dwRef,-1), mCharID, pForCharID, IsWindowVisible(), IsCharShown(), pVisiblityCause);
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
#ifdef	_DEBUG_ACTIVATE
		if	(
				(lRet)
			&&	(GetLastActive() == m_hWnd)
			)
		{
			LogMessage (_DEBUG_ACTIVATE, _T("[%p(%d)] SetNotInputActive [%d] (Hide)"), this, max(m_dwRef,-1), mCharID);
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
		LogMessage (_LOG_POPUP_OPS, _T("[%p(%d)] [%d] HidePopup false"), this, max(m_dwRef,-1), mCharID);
	}
#endif
#ifdef	_TRACE_RESOURCES
	if	(LogIsActive (_TRACE_RESOURCES))
	{
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentPopupWnd::HidePopup [%p] [%d] Done"), this, m_hWnd, pForCharID);
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

#ifdef	_LOG_POPUP_OPS
	if	(LogIsActive (_LOG_POPUP_OPS))
	{
		LogMessage (_LOG_POPUP_OPS, _T("[%p(%d)] [%d] QueueMove [%p] [%d] [%d] to [%d %d] speed [%u]"), this, max(m_dwRef,-1), mCharID, lQueuedMove, pCharID, lReqID, lQueuedMove->mPosition.x, lQueuedMove->mPosition.y, lQueuedMove->mTimeAllowed);
	}
#endif
	return lReqID;
}

bool CAgentPopupWnd::DoQueuedMove ()
{
	tPtr <CQueuedMove>	lQueuedMove;

	if	(lQueuedMove = (CQueuedMove *) mQueue.GetNextAction (QueueActionMove))
	{
		CPoint	lOffset;
		CRect	lWinRect;

		GetWindowRect (&lWinRect);
		lOffset.x = lQueuedMove->mPosition.x - lWinRect.left;
		lOffset.y = lQueuedMove->mPosition.y - lWinRect.top;

		if	(!lQueuedMove->mStarted)
		{
			lQueuedMove->NotifyStarted (mNotify);
		}
		if	(mQueue.GetNextAction (QueueActionMove) == lQueuedMove)
		{
			if	(
					(!lQueuedMove->mAnimationShown)
				&&	(
						(!IsWindowVisible ())
					||	(
							(lOffset.x == 0)
						&&	(lOffset.y == 0)
						)
					)
				)
			{
				lQueuedMove->mTimeAllowed = 0;
			}

			if	(!lQueuedMove->mAnimationShown)
			{
				lQueuedMove->mAnimationShown = true;

				if	(lQueuedMove->mTimeAllowed > 0)
				{
					CAtlOwnPtrList <CQueuedAction>	lQueue;

					if	(lOffset.x < 0)
					{
						if	(lOffset.y < lOffset.x)
						{
							lQueuedMove->mAnimationState = _T("MOVINGUP");
						}
						else
						if	(lOffset.y > -lOffset.x)
						{
							lQueuedMove->mAnimationState = _T("MOVINGDOWN");
						}
						else
						{
							lQueuedMove->mAnimationState = _T("MOVINGRIGHT");
						}
					}
					else
					{
						if	(lOffset.y > lOffset.x)
						{
							lQueuedMove->mAnimationState = _T("MOVINGDOWN");
						}
						else
						if	(lOffset.y < -lOffset.x)
						{
							lQueuedMove->mAnimationState = _T("MOVINGUP");
						}
						else
						{
							lQueuedMove->mAnimationState = _T("MOVINGLEFT");
						}
					}

					mQueue.PushQueue (lQueue);
					if	(ShowState (lQueuedMove->mAnimationState))
					{
						lQueue.RemoveHead ();
						mQueue.AddTail (lQueuedMove.Detach());
#ifdef	_LOG_QUEUE_OPS
						if	(LogIsActive (_LOG_QUEUE_OPS))
						{
							LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d] Requeue move to end of queue"), this, max(m_dwRef,-1), mCharID);
						}
#endif
					}
					mQueue.PopQueue (lQueue);
				}
			}

			if	(
					(lQueuedMove)
				&&	(mQueue.GetNextAction (QueueActionMove) == lQueuedMove)
				)
			{
				mQueue.RemoveHead ();

				if	(
						(lQueuedMove)
					&&	(!lQueuedMove->mMoveStarted)
					)
				{
					lQueuedMove->mMoveStarted = new CPoint (lWinRect.TopLeft());
					if	(lQueuedMove->mTimeAllowed > 0)
					{
						lQueuedMove->mTimeStarted = GetTickCount();
						lQueuedMove->mTimeAllowed = MulDiv (max (max (labs (lOffset.x), labs (lOffset.y)), 10), lQueuedMove->mTimeAllowed, 500);
					}
				}

				if	(
						(lQueuedMove)
					&&	(lQueuedMove->mMoveStarted)
					)
				{
					if	(DoQueuedMoveCycle (lQueuedMove))
					{
						mQueue.AddHead (lQueuedMove.Detach());
					}

					if	(lQueuedMove)
					{
						if	(!lQueuedMove->mEndAnimationShown)
						{
							MovePopup (lQueuedMove->mPosition, lQueuedMove->mCharID, MoveCause_ProgramMoved, true);
						}
						if	(
								(lQueuedMove->mTimeAllowed > 0)
							&&	(!lQueuedMove->mEndAnimationShown)
							&&	(ShowGesture (NULL))
							)
						{
							lQueuedMove->mEndAnimationShown = true;
							mQueue.AddHead (lQueuedMove.Detach());
						}
						if	(lQueuedMove)
						{
#ifdef	_STRICT_COMPATIBILITY
							if	(!IsCharShown ())
							{
								lQueuedMove->NotifyComplete (mNotify, AGENTERR_CHARACTERNOTVISIBLE);
							}
							else
#endif
							{
								lQueuedMove->NotifyComplete (mNotify);
							}
						}
					}
				}
			}
			else
			{
				lQueuedMove.Detach ();
			}
		}
		else
		{
			lQueuedMove.Detach (); // Was deleted during NotifyStarted
		}
		return true;
	}
	return false;
}

bool CAgentPopupWnd::DoQueuedMoveCycle (CQueuedMove * pQueuedMove)
{
	bool			lRet = false;
	CQueuedMove *	lQueuedMove;

	if	(
			(
				(lQueuedMove = pQueuedMove)
			||	(lQueuedMove = (CQueuedMove *) mQueue.GetNextAction (QueueActionMove))
			)
		&&	(lQueuedMove->mMoveStarted)
		&&	(lQueuedMove->mTimeAllowed > 0)
		)
	{
		if	(
				(pQueuedMove)
			||	(PreDoQueue () > 0)
			)
		{
			long	lElapsed = ElapsedTicks (lQueuedMove->mTimeStarted);
			CPoint	lOffset;
			CRect	lWinRect;

			GetWindowRect (&lWinRect);
			lOffset.x = lQueuedMove->mPosition.x - lWinRect.left;
			lOffset.y = lQueuedMove->mPosition.y - lWinRect.top;

			if	(mQueueTime != mQueueTimeMin)
			{
				ActivateQueue (false, mQueueTimeMin);
			}
			if	(lElapsed < (long)lQueuedMove->mTimeAllowed)
			{
				lOffset.x = lQueuedMove->mMoveStarted->x + MulDiv (lQueuedMove->mPosition.x - lQueuedMove->mMoveStarted->x, lElapsed, (long)lQueuedMove->mTimeAllowed) - lWinRect.left;
				lOffset.y = lQueuedMove->mMoveStarted->y + MulDiv (lQueuedMove->mPosition.y - lQueuedMove->mMoveStarted->y, lElapsed, (long)lQueuedMove->mTimeAllowed) - lWinRect.top;
				lRet = true;
			}
			else
			if	(mQueueTime != mQueueTimeDefault)
			{
				ActivateQueue (false, mQueueTimeDefault);
			}

			if	(lRet)
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d] Queued move [%d %d] to [%d %d] by [%d %d] Elapsed [%d of %u] Remaining [%d %d] [%d]"), this, max(m_dwRef,-1), mCharID, lWinRect.left, lWinRect.top, lQueuedMove->mPosition.x, lQueuedMove->mPosition.y, lOffset.x, lOffset.y, lElapsed, lQueuedMove->mTimeAllowed, lQueuedMove->mPosition.x-lWinRect.left-lOffset.x, lQueuedMove->mPosition.y-lWinRect.top-lOffset.y, (long)lQueuedMove->mTimeAllowed-lElapsed);
				}
#endif
				lWinRect.OffsetRect (lOffset);
				MoveWindow (lWinRect);
			}
			if	(!pQueuedMove)
			{
				PostDoQueue ();
			}
		}
	}
	return lRet;
}

void CAgentPopupWnd::AbortQueuedMove (CQueuedAction * pQueuedAction, HRESULT pReqStatus, LPCTSTR pReason)
{
	CQueuedMove *	lQueuedMove;
	CQueuedState *	lQueuedState;

	if	(lQueuedMove = (CQueuedMove *) pQueuedAction)
	{
		if	(
				(lQueuedMove->mAnimationShown)
			&&	(!lQueuedMove->mEndAnimationShown)
			)
		{
			if	(!ShowGesture (NULL))
			{
				Stop ();
			}
		}
		if	(
				(!lQueuedMove->mAnimationState.IsEmpty ())
			&&	(lQueuedState = (CQueuedState *) mQueue.GetNextAction (QueueActionState))
			&&	(lQueuedState->mStateName.CompareNoCase (lQueuedMove->mAnimationState) == 0)
			)
		{
			RemoveQueuedAction (lQueuedState, pReqStatus, pReason);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

CQueuedAction * CAgentPopupWnd::IsMoveQueued (long pCharID)
{
	return mQueue.GetCharAction (QueueActionMove, pCharID);
}

bool CAgentPopupWnd::RemoveQueuedMove (long pCharID, HRESULT pReqStatus, LPCTSTR pReason, bool pExcludeActive)
{
	bool			lRet = false;
	CQueuedMove *	lQueuedMove;

	while	(
				(lQueuedMove = (CQueuedMove *) mQueue.GetCharAction (QueueActionMove, pCharID, true))
			&&	(
					(!pExcludeActive)
				||	(!lQueuedMove->mStarted)
				)
			&&	(RemoveQueuedAction (lQueuedMove, pReqStatus, pReason))
			)
	{
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAgentPopupWnd::AbortQueuedAction (CQueuedAction * pQueuedAction, HRESULT pReqStatus, LPCTSTR pReason)
{
	CQueuedAction *	lQueuedAction;

	CAgentCharacterWnd::AbortQueuedAction (pQueuedAction, pReqStatus, pReason);

	if	(lQueuedAction = (CQueuedAction *) pQueuedAction)
	{
		if	(lQueuedAction->mAction == QueueActionMove)
		{
			AbortQueuedMove (pQueuedAction, pReqStatus, pReason);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentPopupWnd::CanDoAnimationQueue ()
{
	if	(
			(!mQueue.IsEmpty ())
		&&	(DoQueuedMoveCycle ())
		)
	{
		return false;
	}
	return CAgentCharacterWnd::CanDoAnimationQueue ();
}

bool CAgentPopupWnd::DoAnimationQueue (bool & pNextActivateImmediate, DWORD & pNextQueueTime)
{
	bool			lRet = CAgentCharacterWnd::DoAnimationQueue (pNextActivateImmediate, pNextQueueTime);
	CQueuedAction *	lNextAction = mQueue.GetNextAction ();

	if	(
			(!lRet)
		&&	(DoQueuedMove ())
		)
	{
		pNextActivateImmediate = (mQueue.GetNextAction () != lNextAction);
		pNextQueueTime = mQueueTime;
		lRet = true;
	}
	return lRet;
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
#ifdef	_DEBUG_ACTIVATE
		LogMessage (_DEBUG_ACTIVATE, _T("[%p(%d)] CAgentPopupWnd OnActivate [%d] Activate [%p] Last [%p]"), this, max(m_dwRef,-1), mCharID, m_hWnd, mLastActive);
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
				for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
				{
					if	(lNotify->GetNotifyClient (mCharID) == mCharID)
					{
						lNotify->DragComplete (mCharID, NotifyKeyState(), lWinRect.left, lWinRect.top);
					}
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
				INT_PTR			lNotifyNdx;
				CEventNotify *	lNotify;

				for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
				{
					if	(lNotify->GetNotifyClient (mCharID) == mCharID)
					{
						lNotify->DragStart (mCharID, NotifyKeyState(), mSizeMoveStart->x, mSizeMoveStart->y);
					}
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
			INT_PTR			lNotifyNdx;
			CEventNotify *	lNotify;

			for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
			{
				if	(
						(lNotify->GetNotifyClient (mCharID) == mCharID)
					&&	(lNotify->_DefaultCommand (mCharID, m_hWnd, pPoint))
					)
				{
					break;
				}
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