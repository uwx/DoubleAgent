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
#include <AfxPriv.h>
#include "DaCore.h"
#include "..\Server\DaAgentNotify.h"
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
#ifndef	_WIN64
#include "Sapi4Voice.h"
#include "Sapi4Err.h"
#endif
#ifdef	_DEBUG
#include "BitmapDebugger.h"
#include "DebugWin.h"
#include "DebugProcess.h"
#endif
#ifdef	_DEBUG_NOT
#include "DebugTime.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_DEBUG
//#define	_DEBUG_MOUSE		LogNormal
//#define	_DEBUG_ACTIVATE		LogNormal
#define	_DEBUG_SPEECH			(GetProfileDebugInt(_T("DebugSpeech"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_DEBUG_SPEECH_EVENTS	(GetProfileDebugInt(_T("DebugSpeechEvents"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_LOG_INSTANCE			(GetProfileDebugInt(_T("LogInstance_Popup"),LogDetails,true)&0xFFFF)
#define	_LOG_POPUP_OPS			(GetProfileDebugInt(_T("LogPopupOps"),LogVerbose,true)&0xFFFF|LogTimeMs)
#define	_LOG_QUEUE_OPS			(GetProfileDebugInt(_T("LogQueueOps"),LogVerbose,true)&0xFFFF|LogTimeMs|LogHighVolume)
#define	_TRACE_RESOURCES		(GetProfileDebugInt(_T("TraceResources"),LogVerbose,true)&0xFFFF|LogHighVolume)
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

const UINT	CAgentPopupWnd::mNotifyIconMsg = RegisterWindowMessage (_T("1147E500-A208-11DE-ABF2-002421116FB2"));
const UINT	CAgentPopupWnd::mTaskbarCreatedMsg = RegisterWindowMessage (_T("TaskbarCreated"));
HWND		CAgentPopupWnd::mLastActive = NULL;
UINT		CAgentPopupWnd::mVoiceStartMsg = RegisterWindowMessage (_T("A444DB92-39D0-4677-8D6D-1C4032BC9DED"));
UINT		CAgentPopupWnd::mVoiceEndMsg = RegisterWindowMessage (_T("AD44294A-BC10-43e5-94A7-C9C392863A79"));
UINT		CAgentPopupWnd::mVoiceBookMarkMsg = RegisterWindowMessage (_T("8FC08C6D-E6EB-4d53-B115-8378AB001571"));
UINT		CAgentPopupWnd::mVoiceVisualMsg = RegisterWindowMessage (_T("242D8583-6BAC-44d5-8CF8-F6DD020F701C"));

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CAgentPopupWnd, CAgentWnd)

BEGIN_MESSAGE_MAP(CAgentPopupWnd, CAgentWnd)
	//{{AFX_MSG_MAP(CAgentPopupWnd)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_NCRBUTTONDOWN()
	ON_WM_NCRBUTTONUP()
	ON_WM_NCRBUTTONDBLCLK()
	ON_WM_NCMBUTTONDOWN()
	ON_WM_NCMBUTTONUP()
	ON_WM_NCMBUTTONDBLCLK()
	ON_MESSAGE(WM_ENTERSIZEMOVE, OnEnterSizeMove)
	ON_MESSAGE(WM_EXITSIZEMOVE, OnExitSizeMove)
	ON_WM_MOVING()
	ON_WM_MOVE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_ACTIVATE()
	ON_WM_CONTEXTMENU()
	ON_MESSAGE(WM_DISPLAYCHANGE, OnDisplayChange)
	ON_MESSAGE(WM_INPUTLANGCHANGE, OnInputLangChange)
	ON_REGISTERED_MESSAGE(mNotifyIconMsg, OnNotifyIcon)
	ON_REGISTERED_MESSAGE(mTaskbarCreatedMsg, OnTaskbarCreated)
	ON_REGISTERED_MESSAGE(mVoiceStartMsg, OnVoiceStartMsg)
	ON_REGISTERED_MESSAGE(mVoiceEndMsg, OnVoiceEndMsg)
	ON_REGISTERED_MESSAGE(mVoiceBookMarkMsg, OnVoiceBookMarkMsg)
	ON_REGISTERED_MESSAGE(mVoiceVisualMsg, OnVoiceVisualMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CAgentPopupWnd::CAgentPopupWnd ()
:	mCharID (0),
	mIsDragging (false),
	mWasDragged (false),
	mLastButtonMsg (0),
	mInNotify (0)
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CAgentPopupWnd::CAgentPopupWnd (%d)"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount);
	}
#endif
	AfxOleLockApp ();
	EnableAutomation ();
	m_dwRef = 0;

#ifdef	_DEBUG
	if	(GetProfileDebugInt(_T("DebugDisableSmoothing")) <= 0)
	{
		mAlphaBlended = true;
	}
#endif
	SetBkColor (0x00040404);
}

CAgentPopupWnd::~CAgentPopupWnd ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CAgentPopupWnd::~CAgentPopupWnd (%d) [%p] [%d]"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, m_hWnd, ::IsWindow(m_hWnd));
	}
#endif
	Detach (-1, NULL);
	HideNotifyIcon ();
	SafeFreeSafePtr (mBalloonWnd);
	SafeFreeSafePtr (mListeningWnd);

	AfxOleUnlockApp ();
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CAgentPopupWnd::~CAgentPopupWnd (%d) Done [%d]"), this, m_dwRef, AfxGetModuleState()->m_nObjectCount, AfxOleCanExitApp());
	}
#endif
}

void CAgentPopupWnd::OnFinalRelease ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive (_LOG_INSTANCE))
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CAgentPopupWnd::OnFinalRelease [%u] [%u]"), this, m_dwRef, IsInNotify(), IsQueueBusy());
	}
#endif
	if	(
			(IsInNotify() == 0)
		&&	(IsQueueBusy() == 0)
		)
	{
		Close ();
		CCmdTarget::OnFinalRelease ();
	}
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

	CThreadSecurity::AllowUiPiMessage (mTaskbarCreatedMsg);

	if	(pInitialRect)
	{
		lInitialRect.CopyRect (pInitialRect);
		mAutoSize = false;
#ifdef	_DEBUG_NOT
		lStyle |= WS_DLGFRAME;
#endif
	}
#ifdef	_DEBUG_NOT
	else
	{
		lStyle |= WS_CAPTION | WS_THICKFRAME;
	}
#endif
#ifdef	_TRACE_RESOURCES
	CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentPopupWnd::Create [%p]"), this, m_hWnd);
#endif
	if	(CDirectShowWnd::CreateEx (WS_EX_TOPMOST|WS_EX_LAYERED, AfxRegisterWndClass(CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW|CS_NOCLOSE), NULL, lStyle, lInitialRect, CWnd::FromHandle(pParentWnd), 0))
	{
		if	(
				(lAgentFile = GetAgentFile ())
			&&	(lAgentFileName = lAgentFile->FindName ())
			)
		{
			SetWindowText (CString ((BSTR)lAgentFileName->mName));
		}
#ifdef	_LOG_INSTANCE
		if	(LogIsActive())
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CAgentPopupWnd::Create [%p] Parent [%p] [%p] Owner [%p]"), this, m_dwRef, m_hWnd, pParentWnd, ::GetParent(m_hWnd), ::GetWindow(m_hWnd, GW_OWNER));
		}
#endif
		lRet = true;
	}
#ifdef	_TRACE_RESOURCES
	CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentPopupWnd::Create [%p] Done"), this, m_hWnd);
#endif
	return lRet;
}

void CAgentPopupWnd::OnDestroy()
{
	HideNotifyIcon ();
	CAgentWnd::OnDestroy();
}

/////////////////////////////////////////////////////////////////////////////

void CAgentPopupWnd::Opened ()
{
	CAgentWnd::Opened ();
#ifdef	_STRICT_COMPATIBILITY
	ShowNotifyIcon ();
#endif
}

void CAgentPopupWnd::Closing ()
{
	CAgentWnd::Closing ();
	HideNotifyIcon ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentPopupWnd::Attach (long pCharID, IDaNotify * pNotify, bool pSetActiveCharID)
{
	bool	lRet = false;
	long	lPrevCharID = mCharID;

#ifdef	_TRACE_RESOURCES
	CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentPopupWnd::Attach [%d] [%u]"), this, pCharID, pSetActiveCharID);
#endif
	if	(
			(pNotify)
		&&	(mNotify.AddUnique (pNotify) >= 0)
		)
	{
		lRet = true;
	}
	if	(
			(mBalloonWnd)
		&&	(mBalloonWnd->Attach (pCharID, pNotify, pSetActiveCharID))
		)
	{
		lRet = true;
	}
	if	(
			(pSetActiveCharID)
		&&	(mListeningWnd)
		&&	(mListeningWnd->Attach (pCharID, NULL))
		)
	{
		lRet = true;
	}

	if	(
			(pSetActiveCharID)
		&&	(mCharID != pCharID)
		)
	{
		mCharID = pCharID;
		lRet = true;

		if	(mNotify.GetSize() > 0)
		{
//
//	Do the activation notifications twice - first for inactive, then for active.  It's important that the
//	inactive notifications are sent first so an application that is going inactive has a chance to clean
//	up before the next application becomes active.
//
			int			lNotifyNdx;
			IDaNotify *	lNotify;
			long		lInputInactiveCharID = 0;
			long		lInputActiveCharID = 0;

			if	(lPrevCharID > 0)
			{
				if	(
						(GetLastActive() == m_hWnd)
					&&	(IsWindowVisible ())
					)
				{
					lInputInactiveCharID = lPrevCharID;
				}
#ifdef	_DEBUG_ACTIVATE
				LogMessage (_DEBUG_ACTIVATE, _T("[%p(%d)] SetInactive [%d] InputActive [%d] (Attach)"), this, m_dwRef, lPrevCharID, lInputInactiveCharID);
#endif
				for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
				{
					lNotify->_ActiveCharacterNotify (0, 0, lPrevCharID, lInputInactiveCharID);
				}
			}

			if	(
					(GetLastActive() == m_hWnd)
				&&	(IsWindowVisible ())
				)
			{
				lInputActiveCharID = mCharID;
			}
#ifdef	_DEBUG_ACTIVATE
			LogMessage (_DEBUG_ACTIVATE, _T("[%p(%d)] SetActive [%d] InputActive [%d] (Attach)"), this, m_dwRef, mCharID, lInputActiveCharID);
#endif
			for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
			{
				lNotify->_ActiveCharacterNotify (mCharID, lInputActiveCharID, 0, 0);
			}
			for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
			{
				if	(lNotify->_ActiveCharacterChanged (mCharID, lInputActiveCharID, lPrevCharID, lInputInactiveCharID))
				{
					break;
				}
			}
		}
	}
#ifdef	_TRACE_RESOURCES
	CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentPopupWnd::Attach [%d] [%u] Done"), this, pCharID, pSetActiveCharID);
#endif
	return lRet;
}

bool CAgentPopupWnd::Detach (long pCharID, IDaNotify * pNotify)
{
	bool	lRet = false;

	try
	{
#ifdef	_TRACE_RESOURCES
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentPopupWnd::Detach [%d]"), this, pCharID);
#endif
		ClearQueuedActions (pCharID, 0, _T("Detach"));

		if	(pCharID == mCharID)
		{
			mCharID = 0;

			if	(mNotify.GetSize() > 0)
			{
				int			lNotifyNdx;
				IDaNotify *	lNotify;
				long		lInputActiveCharID = 0;

				if	(
						(GetLastActive() == m_hWnd)
					&&	(IsWindowVisible ())
					)
				{
					lInputActiveCharID = pCharID;
				}

#ifdef	_DEBUG_ACTIVATE
				LogMessage (_DEBUG_ACTIVATE, _T("[%p(%d)] SetInactive [%d] InputActive [%d] (Detach)"), this, m_dwRef, pCharID, lInputActiveCharID);
#endif
				for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
				{
					lNotify->_ActiveCharacterNotify (-1, -1, pCharID, lInputActiveCharID);
				}
				for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
				{
					if	(lNotify->_ActiveCharacterChanged (-1, -1, pCharID, lInputActiveCharID))
					{
						break;
					}
				}
			}
		}

		if	(
				(pNotify)
			&&	(mNotify.Remove (pNotify) >= 0)
			)
		{
			lRet = true;
		}
		if	(
				(mBalloonWnd)
			&&	(mBalloonWnd->Detach (pCharID, pNotify))
			)
		{
			lRet = true;
		}
		if	(
				(mListeningWnd)
			&&	(mListeningWnd->Detach (pCharID))
			)
		{
			lRet = true;
		}

		if	(
				(pCharID < 0)
			&&	(!pNotify)
			)
		{
			INT_PTR		lNotifyNdx;
			IDaNotify *	lNotify;

			for	(lNotifyNdx = mNotify.GetUpperBound(); lNotify = mNotify (lNotifyNdx); lNotifyNdx--)
			{
				if	(Detach (-1, lNotify))
				{
					lRet = true;
				}
			}
			mNotify.RemoveAll ();
		}
#ifdef	_TRACE_RESOURCES
		CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentPopupWnd::Detach [%d] Done"), this, pCharID);
#endif
	}
	catch AnyExceptionSilent

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentPopupWnd::PreNotify ()
{
	if	(
			(this)
		&&	(m_dwRef > 0)
		&&	(mNotify.GetSize() > 0)
		)
	{
		mInNotify++;
		return true;
	}
	return false;
}

bool CAgentPopupWnd::PostNotify ()
{
	if	(this)
	{
		if	(mInNotify > 0)
		{
			mInNotify--;
		}
		if	(
				(mInNotify == 0)
			&&	(IsQueueBusy () == 0)
			&&	(m_dwRef == 0)
			)
		{
#ifdef	_LOG_INSTANCE
			if	(LogIsActive (_LOG_INSTANCE))
			{
				LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CAgentPopupWnd PostNotify -> OnFinalRelease"), this, m_dwRef);
			}
#endif
			OnFinalRelease ();
			return false;
		}
		return true;
	}
	return false;
}

UINT CAgentPopupWnd::IsInNotify () const
{
	if	(this)
	{
		return mInNotify;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

int CAgentPopupWnd::_PreDoQueue ()
{
	return CAgentWnd::_PreDoQueue ();
}

int CAgentPopupWnd::_PostDoQueue ()
{
	int	lRet = CAgentWnd::_PostDoQueue ();

	if	(
			(mInNotify == 0)
		&&	(lRet <= 0)
		&&	(m_dwRef == 0)
		)
	{
#ifdef	_LOG_INSTANCE
		if	(LogIsActive (_LOG_INSTANCE))
		{
			LogMessage (_LOG_INSTANCE, _T("[%p(%d)] CAgentPopupWnd PostDoQueue -> OnFinalRelease"), this, m_dwRef);
		}
#endif
		OnFinalRelease ();
		lRet = -1;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CAgentPopupWnd::GetCharID () const
{
	return mCharID;
}

bool CAgentPopupWnd::IsDragging () const
{
	if	(
			(IsWindow (m_hWnd))
		&&	(::GetCapture() == m_hWnd)
		&&	(mIsDragging)
		)
	{
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

CAgentBalloonWnd * CAgentPopupWnd::GetBalloonWnd (bool pCreate)
{
	if	(
			(!mBalloonWnd)
		&&	(pCreate)
		&&	(mBalloonWnd = (CAgentBalloonWnd *) CAgentBalloonWnd::CreateObject())
		)
	{
		INT_PTR	lNotifyNdx;

		for	(lNotifyNdx = 0; lNotifyNdx <= mNotify.GetUpperBound(); lNotifyNdx++)
		{
			mBalloonWnd->Attach (mCharID, mNotify [lNotifyNdx], true);
		}
	}

	if	(
			(mBalloonWnd)
		&&	(pCreate)
		)
	{
		if	(mBalloonWnd->GetSafeHwnd())
		{
			mBalloonWnd->CommitOptions ();
		}
		else
		{
			mBalloonWnd->Create (this);
		}
	}
	return mBalloonWnd;
}

CAgentListeningWnd * CAgentPopupWnd::GetListeningWnd (bool pCreate)
{
	if	(
			(!mListeningWnd)
		&&	(pCreate)
		&&	(mListeningWnd = (CAgentListeningWnd *) CAgentListeningWnd::CreateObject())
		)
	{
		mListeningWnd->Create (this);
	}
	return mListeningWnd;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentPopupWnd::SetLastActive (HWND pLastActive)
{
	if	(
			(pLastActive != mLastActive)
		&&	(DYNAMIC_DOWNCAST (CAgentPopupWnd, CWnd::FromHandlePermanent (pLastActive)))
		)
	{
		CAgentPopupWnd *	lLastActive = DYNAMIC_DOWNCAST (CAgentPopupWnd, CWnd::FromHandlePermanent (mLastActive));
		long				lLastActiveCharID = -1;
		int					lNotifyNdx;
		IDaNotify *			lNotify;

		if	(lLastActive->GetSafeHwnd())
		{
			lLastActiveCharID = lLastActive->mCharID;
			lLastActive->MakeActiveMedia (false);
		}
		mLastActive = m_hWnd;
		MakeActiveMedia (true);

		if	(
				(lLastActive->GetSafeHwnd())
			&&	(lLastActive->PreNotify ())
			)
		{
			try
			{
#ifdef	_DEBUG_ACTIVATE
				LogMessage (_DEBUG_ACTIVATE, _T("[%p(%d)] SetNotInputActive [%d] (Activate)"), this, m_dwRef, lLastActiveCharID);
#endif
				for	(lNotifyNdx = 0; lNotify = lLastActive->mNotify (lNotifyNdx); lNotifyNdx++)
				{
					lNotify->_ActiveCharacterNotify (-1, -1, lLastActiveCharID, lLastActiveCharID);
				}
			}
			catch AnyExceptionDebug
			lLastActive->PostNotify ();
		}

		if	(PreNotify ())
		{
			try
			{
#ifdef	_DEBUG_ACTIVATE
				LogMessage (_DEBUG_ACTIVATE, _T("[%p(%d)] SetInputActive [%d] (Activate)"), this, m_dwRef, mCharID);
#endif
				for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
				{
					lNotify->_ActiveCharacterNotify (mCharID, mCharID, -1, -1);
				}
				for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
				{
					if	(lNotify->_ActiveCharacterChanged (mCharID, mCharID, lLastActiveCharID, lLastActiveCharID))
					{
						break;
					}
				}
			}
			catch AnyExceptionDebug
			PostNotify ();
		}
		return true;
	}
	return false;
}

HWND CAgentPopupWnd::GetLastActive ()
{
	return mLastActive;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentPopupWnd::ShowPopup (long pForCharID, long pVisiblityCause, bool pAlwaysNotify)
{
	bool			lRet = false;
	bool			lIsPlaying = IsPlaying ();
	CAgentFile *	lAgentFile;

	if	(pForCharID <= 0)
	{
		pForCharID = mCharID;
	}
#ifdef	_TRACE_RESOURCES
	CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentPopupWnd::ShowPopup [%p] [%d]"), this, m_hWnd, pForCharID);
#endif
#ifdef	_LOG_POPUP_OPS
	if	(LogIsActive (_LOG_POPUP_OPS))
	{
		CRect lWinRect;
		GetWindowRect (&lWinRect);
		LogMessage (_LOG_POPUP_OPS, _T("[%p(%d)] [%d] ShowPopup for [%d] visible [%d] cause [%d] at [%d %d %d %d (%d %d)]"), this, m_dwRef, mCharID, pForCharID, ::IsWindowVisible(m_hWnd), pVisiblityCause, lWinRect.left, lWinRect.top, lWinRect.right, lWinRect.bottom, lWinRect.Width(), lWinRect.Height());
	}
#endif
	if	(
			(IsWindow (m_hWnd))
		&&	(!IsWindowVisible())
		)
	{
		if	(lIsPlaying)
		{
			Pause ();
		}

		ModifyStyleEx (0, WS_EX_LAYERED);
		SetLayeredWindowAttributes (0, 255, LWA_ALPHA);

		if	(lAgentFile = GetAgentFile())
		{
			CRect					lWindowRect;
			CDC						lDC;
			tS <CAgentFileImage>	lImage;
			UINT					lImageFormatSize;
			tPtr <BITMAPINFO>		lImageFormat;
			LPVOID					lImageBits;
			CBitmap					lBitmap;
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
				&&	(lDC.CreateCompatibleDC (NULL))
				&&	(lImageFormatSize = lAgentFile->GetImageFormat (NULL, &lImage, l32BitSamples))
				&&	(lImageFormat = (LPBITMAPINFO)(new BYTE [lImageFormatSize]))
				&&	(lAgentFile->GetImageFormat (lImageFormat, &lImage, l32BitSamples))
				)
			{
				BYTE			lTransparency = lAgentFile->GetTransparency();
				BLENDFUNCTION	lBlend = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};

				if	(!l32BitSamples)
				{
					((LPRGBQUAD)(&lImageFormat->bmiColors [lTransparency]))->rgbRed = GetRValue(*mBkColor);
					((LPRGBQUAD)(&lImageFormat->bmiColors [lTransparency]))->rgbGreen = GetGValue(*mBkColor);
					((LPRGBQUAD)(&lImageFormat->bmiColors [lTransparency]))->rgbBlue = GetBValue(*mBkColor);
				}

				if	(lBitmap.Attach (CreateDIBSection (NULL, lImageFormat, DIB_RGB_COLORS, &lImageBits, NULL, 0)))
				{
					if	(l32BitSamples)
					{
						memset (lImageBits, 0, lImageFormat->bmiHeader.biSizeImage);
					}
					else
					{
						memset (lImageBits, lTransparency, lImageFormat->bmiHeader.biSizeImage);
					}
					GdiFlush ();
					lOldBitmap = ::SelectObject (lDC, lBitmap);

					SetLastError (0);
					if	(
							(l32BitSamples)
						?	(!UpdateLayeredWindow (NULL, &lWindowRect.TopLeft(), &lWindowRect.Size(), &lDC, &CPoint(0,0), 0, &lBlend, ULW_ALPHA))
						:	(!UpdateLayeredWindow (NULL, &lWindowRect.TopLeft(), &lWindowRect.Size(), &lDC, &CPoint(0,0), *mBkColor, NULL, ULW_COLORKEY))
						)
					{
						LogWinErr (LogVerbose, GetLastError(), _T("UpdateLayeredWindow"));
					}
					::SelectObject (lDC, lOldBitmap);

					if	(!l32BitSamples)
					{
						SetLayeredWindowAttributes ((*mBkColor), 255, LWA_COLORKEY);
					}
				}
			}
		}

#ifndef	_STRICT_COMPATIBILITY
		if	(IsWindowVisible())
		{
			ShowNotifyIcon ();
		}
#endif
	}

	if	(IsWindow (m_hWnd))
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
			(IsWindow (m_hWnd))
		&&	(IsWindowVisible())
		)
	{
		if	(lIsPlaying)
		{
			Resume ();
		}
		if	(IsWindow (mListeningWnd->GetSafeHwnd()))
		{
			mListeningWnd->PositionTipWnd ();
		}
		lRet = true;
	}

	if	(
			(
				(lRet)
			||	(pAlwaysNotify)
			)
		&&	(pVisiblityCause != NeverShown)
		&&	(PreNotify ())
		)
	{
		try
		{
			int			lNotifyNdx;
			IDaNotify *	lNotify;
			long		lNotifyCharID;
			long		lVisibilityCause;

			for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
			{
				lNotifyCharID = lNotify->_GetNotifyClient (mCharID);
				lVisibilityCause = ((pVisiblityCause==ProgramShowed) && (lNotifyCharID!=pForCharID)) ? OtherProgramShowed : pVisiblityCause;

				lNotify->_PutVisibilityCause (lNotifyCharID, lVisibilityCause);
				lNotify->VisibleState (lNotifyCharID, TRUE, lVisibilityCause);
			}
		}
		catch AnyExceptionDebug
		PostNotify ();
	}

	if	(
			(IsWindow (m_hWnd))
		&&	(IsWindowVisible())
		)
	{
#ifdef	_DEBUG_NOT
		LogWindow (LogDebug, m_hWnd);
#endif
		SetLastActive (m_hWnd);
	}

#ifdef	_LOG_POPUP_OPS
	if	(
			(!lRet)
		&&	(LogIsActive (_LOG_POPUP_OPS))
		)
	{
		LogMessage (_LOG_POPUP_OPS, _T("[%p(%d)] [%d] ShowPopup false"), this, m_dwRef, mCharID);
	}
#endif
#ifdef	_TRACE_RESOURCES
	CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentPopupWnd::ShowPopup [%p] [%d] Done"), this, m_hWnd, pForCharID);
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentPopupWnd::HidePopup (long pForCharID, long pVisiblityCause, bool pAlwaysNotify)
{
	bool	lRet = false;

	if	(pForCharID <= 0)
	{
		pForCharID = mCharID;
	}
#ifdef	_TRACE_RESOURCES
	CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentPopupWnd::HidePopup [%p] [%d]"), this, m_hWnd, pForCharID);
#endif
#ifdef	_LOG_POPUP_OPS
	if	(LogIsActive (_LOG_POPUP_OPS))
	{
		LogMessage (_LOG_POPUP_OPS, _T("[%p(%d)] [%d] HidePopup for [%d] visible [%d] cause [%d]"), this, m_dwRef, mCharID, pForCharID, ::IsWindowVisible(m_hWnd), pVisiblityCause);
	}
#endif
	StopIdle (_T("HidePopup"));

	if	(IsWindow (mBalloonWnd->GetSafeHwnd()))
	{
		mBalloonWnd->HideBalloon ();
	}

	if	(
			(IsWindow (m_hWnd))
		&&	(IsWindowVisible())
		)
	{
		ShowWindow (SW_HIDE);
		lRet =  true;
	}

	if	(IsWindow (mListeningWnd->GetSafeHwnd()))
	{
		mListeningWnd->RefreshTipWnd ();
		if	(mListeningWnd->GetCharID() == mCharID)
		{
			mListeningWnd->ShowTipWnd ();
		}
		mListeningWnd->PositionTipWnd ();
	}

	if	(
			(
				(lRet)
			||	(pAlwaysNotify)
			)
		&&	(pVisiblityCause != NeverShown)
		&&	(PreNotify ())
		)
	{
		try
		{
			int			lNotifyNdx;
			IDaNotify *	lNotify;
			long		lNotifyCharID;
			long		lVisibilityCause;

			for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
			{
				lNotifyCharID = lNotify->_GetNotifyClient (mCharID);
				lVisibilityCause = ((pVisiblityCause==ProgramHid) && (lNotifyCharID!=pForCharID)) ? OtherProgramHid : pVisiblityCause;

				lNotify->_PutVisibilityCause (lNotifyCharID, lVisibilityCause);
				lNotify->VisibleState (lNotifyCharID, FALSE, lVisibilityCause);
			}

			if	(
					(lRet)
				&&	(GetLastActive() == m_hWnd)
				)
			{
#ifdef	_DEBUG_ACTIVATE
				LogMessage (_DEBUG_ACTIVATE, _T("[%p(%d)] SetNotInputActive [%d] (Hide)"), this, m_dwRef, mCharID);
#endif
				for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
				{
					lNotify->_ActiveCharacterNotify (-1, -1, mCharID, mCharID);
				}
				for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
				{
					if	(lNotify->_ActiveCharacterChanged (-1, -1, mCharID, mCharID))
					{
						break;
					}
				}
			}
		}
		catch AnyExceptionDebug
		PostNotify ();
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
		LogMessage (_LOG_POPUP_OPS, _T("[%p(%d)] [%d] HidePopup false"), this, m_dwRef, mCharID);
	}
#endif
#ifdef	_TRACE_RESOURCES
	CDebugProcess().LogGuiResourcesInline (_TRACE_RESOURCES, _T("[%p] CAgentPopupWnd::HidePopup [%p] [%d] Done"), this, m_hWnd, pForCharID);
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentPopupWnd::MovePopup (const CPoint & pPosition, long pForCharID, long pMoveCause, bool pAlwaysNotify)
{
	bool	lRet = false;

	if	(pForCharID <= 0)
	{
		pForCharID = mCharID;
	}

	if	(IsWindow (m_hWnd))
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
			&&	(pMoveCause != NeverMoved)
			&&	(PreNotify ())
			)
		{
			try
			{
				int			lNotifyNdx;
				IDaNotify *	lNotify;
				long		lNotifyCharID;
				long		lMoveCause;

				for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
				{
					lNotifyCharID = lNotify->_GetNotifyClient (mCharID);
					lMoveCause = ((pMoveCause==ProgramMoved) && (lNotifyCharID!=pForCharID)) ? OtherProgramMoved : pMoveCause;

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
			(IsWindow (m_hWnd))
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
				int			lNotifyNdx;
				IDaNotify *	lNotify;

				for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
				{
					lNotify->Size (lNotify->_GetNotifyClient (mCharID), pSize.cx, pSize.cy);
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

long CAgentPopupWnd::QueueShow (long pCharID, bool pFast)
{
	long			lReqID = 0;
	CQueuedShow *	lQueuedShow = NULL;

	if	(
			(IsWindow (m_hWnd))
		&&	(lQueuedShow = new CQueuedShow (pCharID, lReqID=NextReqID()))
		)
	{
		lQueuedShow->mFast = pFast;
		if	(pFast)
		{
			mQueue.AddHead (lQueuedShow);
		}
		else
		{
			mQueue.AddTail (lQueuedShow);
		}
	}
	else
	{
		lReqID = 0;
	}

#ifdef	_LOG_POPUP_OPS
	if	(LogIsActive (_LOG_POPUP_OPS))
	{
		LogMessage (_LOG_POPUP_OPS, _T("[%p(%d)] [%d] QueueShow [%p] [%d] [%d]"), this, m_dwRef, mCharID, lQueuedShow, pCharID, lReqID);
	}
#endif
	return lReqID;
}

bool CAgentPopupWnd::DoQueuedShow ()
{
	tPtr <CQueuedShow>	lQueuedShow;

	if	(lQueuedShow = (CQueuedShow *) mQueue.GetNextAction (QueueActionShow))
	{
		BOOL						lVisible = IsWindowVisible ();
		COwnPtrList <CQueuedAction>	lQueue;
		CAgentStreamInfo *			lStreamInfo;
		long						lSequenceDuration = 0;

		if	(!lQueuedShow->mStarted)
		{
			lQueuedShow->NotifyStarted (mNotify);
		}
		if	(mQueue.GetNextAction (QueueActionShow) == lQueuedShow)
		{
			if	(!lQueuedShow->mAnimationShown)
			{
				if	(
						(!lVisible)
					&&	(
							(!lQueuedShow->mFast)
						||	(
								(lStreamInfo = GetAgentStreamInfo())
							&&	(lStreamInfo->GetSequenceDuration (&lSequenceDuration) == S_OK)
							&&	(lSequenceDuration <= 0)
							)
						)
					)
				{
					Stop ();
				}

				ShowPopup (lQueuedShow->mCharID, ProgramShowed);
			}

			if	(mQueue.GetNextAction (QueueActionShow) == lQueuedShow)
			{
				mQueue.RemoveHead (); // Wait until window visible so (IsWindowVisible || IsShowingQueued) works

				if	(lQueuedShow->mFast)
				{
					lQueuedShow->mAnimationShown = true;
				}
				if	(
						(!lVisible)
					&&	(!lQueuedShow->mAnimationShown)
					)
				{
					ClearAnimations ();
					mQueue.PushQueue (lQueue);
					if	(ShowState (_T("SHOWING")))
					{
						lQueuedShow->mAnimationShown;
						mQueue.AddTail (lQueuedShow.Detach());
#ifdef	_LOG_QUEUE_OPS
						if	(LogIsActive (_LOG_QUEUE_OPS))
						{
							LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d] Requeue show to end of queue"), this, m_dwRef, mCharID);
						}
#endif
					}
					mQueue.PopQueue (lQueue);
				}

				if	(lQueuedShow)
				{
					lQueuedShow->NotifyComplete (mNotify);
				}
			}
			else
			{
				lQueuedShow.Detach (); // Was deleted during ShowPopup
			}
		}
		else
		{
			lQueuedShow.Detach (); // Was deleted during NotifyStarted
		}
		return true;
	}
	return false;
}

void CAgentPopupWnd::AbortQueuedShow (CQueuedAction * pQueuedAction, HRESULT pReqStatus, LPCTSTR pReason)
{
	CQueuedShow *	lQueuedShow;
	CQueuedState *	lQueuedState;

	if	(
			(lQueuedShow = (CQueuedShow *) pQueuedAction)
		&&	(!lQueuedShow->mFast)
		&&	(lQueuedState = (CQueuedState *) mQueue.GetNextAction (QueueActionState))
		&&	(lQueuedState->mStateName.CompareNoCase (_T("SHOWING")) == 0)
		)
	{
		RemoveQueuedAction (lQueuedState, pReqStatus, pReason);
	}
}

/////////////////////////////////////////////////////////////////////////////

CQueuedAction * CAgentPopupWnd::IsShowQueued (long pCharID)
{
	return mQueue.GetCharAction (QueueActionShow, pCharID);
}

long CAgentPopupWnd::IsShowingQueued ()
{
	if	(
			(GetSafeHwnd ())
		&&	(IsShowQueued ())
		&&	(IsShowQueued () == mQueue.FindNextAction ())
		)
	{
		return mCharID;
	}
	return 0;
}

bool CAgentPopupWnd::RemoveQueuedShow (long pCharID, HRESULT pReqStatus, LPCTSTR pReason, bool pExcludeActive)
{
	bool			lRet = false;
	CQueuedShow *	lQueuedShow;

	while	(
				(lQueuedShow = (CQueuedShow *) mQueue.GetCharAction (QueueActionShow, pCharID, true))
			&&	(
					(!pExcludeActive)
				||	(!lQueuedShow->mStarted)
				)
			&&	(RemoveQueuedAction (lQueuedShow, pReqStatus, pReason))
			)
	{
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CAgentPopupWnd::QueueHide (long pCharID, bool pFast)
{
	long			lReqID = 0;
	CQueuedHide *	lQueuedHide = NULL;

	if	(
			(IsWindow (m_hWnd))
		&&	(lQueuedHide = new CQueuedHide (pCharID, lReqID=NextReqID()))
		)
	{
		lQueuedHide->mFast = pFast;
		mQueue.AddTail (lQueuedHide);
	}
	else
	{
		lReqID = 0;
	}

#ifdef	_LOG_POPUP_OPS
	if	(LogIsActive (_LOG_POPUP_OPS))
	{
		LogMessage (_LOG_POPUP_OPS, _T("[%p(%d)] [%d] QueueHide [%p] [%d] [%d]"), this, m_dwRef, mCharID, lQueuedHide, pCharID, lReqID);
	}
#endif
	return lReqID;
}

bool CAgentPopupWnd::DoQueuedHide ()
{
	tPtr <CQueuedHide>	lQueuedHide;

	if	(lQueuedHide = (CQueuedHide *) mQueue.GetNextAction (QueueActionHide))
	{
		if	(!lQueuedHide->mStarted)
		{
			if	(IsWindow (mBalloonWnd->GetSafeHwnd()))
			{
				mBalloonWnd->HideBalloon ();
			}
			if	(
					(IsWindow (mListeningWnd->GetSafeHwnd ()))
				&&	(mListeningWnd->GetCharID() == mCharID)
				)
			{
				mListeningWnd->HideTipWnd ();
			}

			lQueuedHide->NotifyStarted (mNotify);
		}
		if	(mQueue.GetNextAction (QueueActionHide) == lQueuedHide)
		{
			if	(lQueuedHide->mFast)
			{
				lQueuedHide->mAnimationShown = true;
			}
			if	(!lQueuedHide->mAnimationShown)
			{
				lQueuedHide->mAnimationShown = true;

				if	(IsWindowVisible ())
				{
					COwnPtrList <CQueuedAction>	lQueue;

					mQueue.PushQueue (lQueue);
					if	(ShowState (_T("HIDING")))
					{
						lQueue.RemoveHead ();
						mQueue.AddTail (lQueuedHide.Detach());
#ifdef	_LOG_QUEUE_OPS
						if	(LogIsActive (_LOG_QUEUE_OPS))
						{
							LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d] Requeue hide to end of queue"), this, m_dwRef, mCharID);
						}
#endif
					}
					mQueue.PopQueue (lQueue);
				}
			}
			if	(lQueuedHide)
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d] Queued hide"), this, m_dwRef, mCharID);
				}
#endif
				HidePopup (lQueuedHide->mCharID, ProgramHid);

				if	(mQueue.GetNextAction (QueueActionHide) == lQueuedHide)
				{
					mQueue.RemoveHead ();
					lQueuedHide->NotifyComplete (mNotify);
				}
				else
				{
					lQueuedHide.Detach (); // Was deleted during HidePopup
				}
			}
		}
		else
		{
			lQueuedHide.Detach (); // Was deleted during NotifyStarted
		}
		return true;
	}
	return false;
}

void CAgentPopupWnd::AbortQueuedHide (CQueuedAction * pQueuedAction, HRESULT pReqStatus, LPCTSTR pReason)
{
	CQueuedHide *	lQueuedHide;
	CQueuedState *	lQueuedState;

	if	(
			(lQueuedHide = (CQueuedHide *) pQueuedAction)
		&&	(lQueuedHide->mAnimationShown)
		&&	(lQueuedState = (CQueuedState *) mQueue.GetNextAction (QueueActionState))
		&&	(lQueuedState->mStateName.CompareNoCase (_T("HIDING")) == 0)
		)
	{
		RemoveQueuedAction (lQueuedState, pReqStatus, pReason);
	}
}

/////////////////////////////////////////////////////////////////////////////

CQueuedAction * CAgentPopupWnd::IsHideQueued (long pCharID)
{
	return mQueue.GetCharAction (QueueActionHide, pCharID);
}

long CAgentPopupWnd::IsHidingQueued ()
{
	if	(
			(GetSafeHwnd ())
		&&	(IsHideQueued ())
		&&	(IsHideQueued () == mQueue.FindNextAction ())
		)
	{
		return mCharID;
	}
	return 0;
}

bool CAgentPopupWnd::RemoveQueuedHide (long pCharID, HRESULT pReqStatus, LPCTSTR pReason, bool pExcludeActive)
{
	bool			lRet = false;
	CQueuedHide *	lQueuedHide;

	while	(
				(lQueuedHide = (CQueuedHide *) mQueue.GetCharAction (QueueActionHide, pCharID, true))
			&&	(
					(!pExcludeActive)
				||	(!lQueuedHide->mStarted)
				)
			&&	(RemoveQueuedAction (lQueuedHide, pReqStatus, pReason))
			)
	{
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CAgentPopupWnd::QueueMove (long pCharID, const CPoint & pPosition, DWORD pSpeed)
{
	long			lReqID = 0;
	CQueuedMove *	lQueuedMove = NULL;

	if	(
			(IsWindow (m_hWnd))
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
		LogMessage (_LOG_POPUP_OPS, _T("[%p(%d)] [%d] QueueMove [%p] [%d] [%d] to [%d %d] speed [%u]"), this, m_dwRef, mCharID, lQueuedMove, pCharID, lReqID, lQueuedMove->mPosition.x, lQueuedMove->mPosition.y, lQueuedMove->mTimeAllowed);
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
		long	lElapsed;

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
					COwnPtrList <CQueuedAction>	lQueue;

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
							LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d] Requeue move to end of queue"), this, m_dwRef, mCharID);
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
					lQueuedMove->mMoveStarted = true;
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
					if	(lQueuedMove->mTimeAllowed > 0)
					{
						lElapsed = ElapsedTicks (lQueuedMove->mTimeStarted);

						if	(mQueueTime != mQueueTimeMin)
						{
							ActivateQueue (false, mQueueTimeMin);
						}
						if	(lElapsed < (long)lQueuedMove->mTimeAllowed - (long)mQueueTime)
						{
							lOffset.x = MulDiv (lOffset.x, (long)mQueueTime, (long)lQueuedMove->mTimeAllowed - lElapsed);
							lOffset.y = MulDiv (lOffset.y, (long)mQueueTime, (long)lQueuedMove->mTimeAllowed - lElapsed);
							mQueue.AddHead (lQueuedMove.Detach());
						}
						else
						if	(mQueueTime != mQueueTimeDefault)
						{
							ActivateQueue (false, mQueueTimeDefault);
						}
#ifdef	_LOG_QUEUE_OPS
						if	(LogIsActive (_LOG_QUEUE_OPS))
						{
							CQueuedMove * lMove = lQueuedMove.Ptr() ? lQueuedMove.Ptr() : (CQueuedMove *) mQueue.GetHead();
							LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d] Queued move [%d %d] to [%d %d] by [%d %d] [%f %f] Elapsed [%d of %u] Remaining [%d %d] [%d]"), this, m_dwRef, mCharID, lWinRect.left, lWinRect.top, lMove->mPosition.x, lMove->mPosition.y, lOffset.x, lOffset.y, (float)lOffset.x/(float)max(lMove->mPosition.x-lWinRect.left,1), (float)lOffset.y/(float)max(lMove->mPosition.y-lWinRect.top,1), lElapsed, lMove->mTimeAllowed, lMove->mPosition.x-lWinRect.left-lOffset.x, lMove->mPosition.y-lWinRect.top-lOffset.y, (long)lMove->mTimeAllowed-lElapsed);
						}
#endif
					}

					lWinRect.OffsetRect (lOffset);

					if	(lQueuedMove)
					{
						if	(!lQueuedMove->mEndAnimationShown)
						{
							MovePopup (lWinRect.TopLeft(), lQueuedMove->mCharID, ProgramMoved, true);
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
							if	((GetStyle () & WS_VISIBLE) == 0)
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
					else
					{
						MoveWindow (lWinRect);
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

long CAgentPopupWnd::QueueThink (long pCharID, LPCTSTR pText)
{
	long			lReqID = 0;
	CQueuedThink *	lQueuedThink = NULL;

	if	(
			(IsWindow (m_hWnd))
		&&	(lQueuedThink = new CQueuedThink (pCharID, lReqID=NextReqID()))
		)
	{
		lQueuedThink->mText = pText;
		mQueue.AddTail (lQueuedThink);
	}
	else
	{
		lReqID = 0;
	}
#ifdef	_LOG_QUEUE_OPS
	if	(LogIsActive (_LOG_QUEUE_OPS))
	{
		LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d] QueueThink [%p] [%d] [%d] [%s]"), this, m_dwRef, mCharID, lQueuedThink, pCharID, lReqID, (lQueuedThink ? (LPCTSTR)DebugStr(lQueuedThink->mText) : NULL));
	}
#endif
	return lReqID;
}

bool CAgentPopupWnd::DoQueuedThink ()
{
	tPtr <CQueuedThink>	lQueuedThink;

	if	(lQueuedThink = (CQueuedThink *) mQueue.GetNextAction (QueueActionThink))
	{
#ifdef	_LOG_QUEUE_OPS
		if	(LogIsActive (_LOG_QUEUE_OPS))
		{
			LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d] QueuedThink [%p] [%d] [%d] [%s]"), this, m_dwRef, mCharID, lQueuedThink.Ptr(), lQueuedThink->mCharID, lQueuedThink->mReqID, DebugStr(lQueuedThink->mText));
		}
#endif
		if	(!lQueuedThink->mStarted)
		{
			if	(
					(mBalloonWnd->GetSafeHwnd())
				&&	(mBalloonWnd->IsBusy (true))
				)
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d] QueuedThink [%p] [%d] [%d] is still busy"), this, m_dwRef, mCharID, lQueuedThink.Ptr(), lQueuedThink->mCharID, lQueuedThink->mReqID);
				}
#endif
				lQueuedThink.Detach();
			}
			else
			{
				lQueuedThink->NotifyStarted (mNotify);

				if	(mQueue.GetNextAction (QueueActionThink) == lQueuedThink)
				{
					if	((GetStyle () & WS_VISIBLE) == 0)
					{
						mQueue.RemoveHead ();
						lQueuedThink->NotifyComplete (mNotify, AGENTERR_CHARACTERNOTVISIBLE);
					}
					else
					if	(
							(GetBalloonWnd (true))
						&&	(mBalloonWnd->GetSafeHwnd())
						)
					{
//
//	MS Agent doesn't do this
//
//						ShowGesture (_T("Think"));
#ifdef	_LOG_QUEUE_OPS
						if	(LogIsActive (_LOG_QUEUE_OPS))
						{
							LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d] Show QueuedThink [%p] [%d] [%d] [%s]"), this, m_dwRef, mCharID, lQueuedThink.Ptr(), lQueuedThink->mCharID, lQueuedThink->mReqID, DebugStr(lQueuedThink->mText));
						}
#endif
						if	(mBalloonWnd->ShowBalloonThought (lQueuedThink->mText))
						{
							mBalloonWnd->ShowBalloonAuto ();
						}
						lQueuedThink.Detach ();
					}
					else
					{
						mQueue.RemoveHead ();
						lQueuedThink->NotifyComplete (mNotify, AGENTERR_NOBALLOON);
					}
				}
				else
				{
					lQueuedThink.Detach (); // Was deleted during NotifyStarted
				}
			}
		}
		else
		{
			if	(
					(mBalloonWnd->GetSafeHwnd())
				&&	(mBalloonWnd->IsBusy (false))
				)
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d] QueuedThink [%p] [%d] [%d] is busy"), this, m_dwRef, mCharID, lQueuedThink.Ptr(), lQueuedThink->mCharID, lQueuedThink->mReqID);
				}
#endif
				lQueuedThink.Detach();
			}
			else
			{
				mQueue.RemoveHead ();

				if	(mBalloonWnd->GetSafeHwnd())
				{
					mBalloonWnd->AbortSpeechText ();
				}
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d] Done QueuedThink [%p] [%d] [%d]"), this, m_dwRef, mCharID, lQueuedThink.Ptr(), lQueuedThink->mCharID, lQueuedThink->mReqID);
				}
#endif
				lQueuedThink->NotifyComplete (mNotify);
			}
		}
		return true;
	}
	return false;
}

void CAgentPopupWnd::AbortQueuedThink (CQueuedAction * pQueuedAction, HRESULT pReqStatus, LPCTSTR pReason)
{
	CQueuedThink *	lQueuedThink;

	if	(
			(lQueuedThink = (CQueuedThink *) pQueuedAction)
		&&	(lQueuedThink->mStarted)
		&&	(mBalloonWnd->GetSafeHwnd())
		)
	{
#ifdef	_STRICT_COMPATIBILITY
		mBalloonWnd->HideBalloon (true);
#else
		mBalloonWnd->AbortSpeechText ();
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////

CQueuedAction * CAgentPopupWnd::IsThinkQueued (long pCharID)
{
	return mQueue.GetCharAction (QueueActionThink, pCharID);
}

bool CAgentPopupWnd::RemoveQueuedThink (long pCharID, HRESULT pReqStatus, LPCTSTR pReason, bool pExcludeActive)
{
	bool			lRet = false;
	CQueuedThink *	lQueuedThink;

	while	(
				(lQueuedThink = (CQueuedThink *) mQueue.GetCharAction (QueueActionThink, pCharID, true))
			&&	(
					(!pExcludeActive)
				||	(!lQueuedThink->mStarted)
				)
			&&	(RemoveQueuedAction (lQueuedThink, pReqStatus, pReason))
			)
	{
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CAgentPopupWnd::QueueSpeak (long pCharID, LPCTSTR pText, LPCTSTR pSoundUrl, CSapiVoice * pVoice, bool pShowBalloon)
{
	long			lReqID = 0;
	CQueuedSpeak *	lQueuedSpeak = NULL;
	CString			lText;
	CStringArray	lTextParts;
#ifdef	DebugTimeStart
	DebugTimeStart
#endif

	if	(
			(IsWindow (m_hWnd))
		&&	(lQueuedSpeak = new CQueuedSpeak (pVoice, pShowBalloon, pCharID, lReqID=NextReqID()))
		)
	{
		lQueuedSpeak->mSoundUrl = pSoundUrl;

		if	(MakeStringArray (pText, lTextParts, _T("|")) > 1)
		{
			lText = lTextParts [((rand() / lTextParts.GetSize()) * lTextParts.GetSize()) % lTextParts.GetSize()];
		}
		else
		{
			lText = pText;
		}

		if	(!lQueuedSpeak->mSoundUrl.IsEmpty ())
		{
			lTextParts.RemoveAll ();
			lQueuedSpeak->mText.SplitText (lText, lTextParts);
			lQueuedSpeak->mText = lTextParts;
		}
		else
		{
			if	(lText.CompareNoCase (_T("\\lst\\")) == 0)
			{
				lQueuedSpeak->mText = mLastSpeech;
			}
			else
			{
				lQueuedSpeak->mText = lText;
			}
		}
		if	(lQueuedSpeak->mText.GetFullText().IsEmpty ())
		{
			lQueuedSpeak->mShowBalloon = false;
		}

#ifdef	_DEBUG_SPEECH
		if	(
				(lQueuedSpeak->mSoundUrl.IsEmpty ())
			&&	(LogIsActive (_DEBUG_SPEECH))
			)
		{
			LogMessage (_DEBUG_SPEECH, _T("[%p(%d)] [%d] CAgentPopupWnd Queue   [%s]"), this, m_dwRef, mCharID, DebugStr(pText));
			LogMessage (_DEBUG_SPEECH, _T("[%p(%d)] [%d]                Speech  [%s]"), this, m_dwRef, mCharID, DebugStr(lQueuedSpeak->mText.GetSpeechText()));
			LogMessage (_DEBUG_SPEECH, _T("[%p(%d)] [%d]                Text    [%s]"), this, m_dwRef, mCharID, DebugStr(lQueuedSpeak->mText.GetFullText()));
			LogMessage (_DEBUG_SPEECH, _T("[%p(%d)] [%d]                Voice   [%u] Busy [%u] Balloon [%u] Busy [%u]"), this, m_dwRef, mCharID, lQueuedSpeak->mVoice->SafeIsValid(), lQueuedSpeak->mVoice->SafeIsSpeaking (), (mBalloonWnd->GetSafeHwnd()!= NULL), ((mBalloonWnd->GetSafeHwnd()!= NULL) && mBalloonWnd->IsBusy (false)));
			LogMessage (_DEBUG_SPEECH, _T("[%p(%d)] [%d]                Queue   [%u] Busy [%u %u]"), this, m_dwRef, mCharID, mQueue.GetCount(), IsQueueBusy(), !IsAnimationComplete());
		}
#endif
		mQueue.AddTail (lQueuedSpeak);
	}
	else
	{
		lReqID = 0;
	}
#ifdef	_LOG_QUEUE_OPS
	if	(LogIsActive (_LOG_QUEUE_OPS))
	{
		LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d] QueueSpeak [%p] [%d] [%d] [%s] [%s]"), this, m_dwRef, mCharID, lQueuedSpeak, pCharID, lReqID, DebugStr(pText), DebugStr(pSoundUrl));
	}
#endif

#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogHighVolume|LogTimeMs, _T("%f CAgentPopupWnd::QueueSpeak"), DebugTimeElapsed);
#endif
	return lReqID;
}

bool CAgentPopupWnd::DoQueuedSpeak ()
{
	tPtr <CQueuedSpeak>	lQueuedSpeak;
	HRESULT				lResult = S_OK;

	if	(lQueuedSpeak = (CQueuedSpeak *) mQueue.GetNextAction (QueueActionSpeak))
	{
#ifdef	DebugTimeStart
		DebugTimeStart
#endif
#ifdef	_LOG_QUEUE_OPS
		if	(LogIsActive (_LOG_QUEUE_OPS))
		{
			LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d] QueuedSpeak [%p] [%d] [%d] [%s] [%s]"), this, m_dwRef, mCharID, lQueuedSpeak.Ptr(), lQueuedSpeak->mCharID, lQueuedSpeak->mReqID, DebugStr(lQueuedSpeak->mText.GetFullText()), DebugStr(lQueuedSpeak->mSoundUrl));
		}
#endif
		if	(!lQueuedSpeak->mStarted)
		{
			lResult = SpeechIsReady (lQueuedSpeak);

			if	(lResult == S_FALSE)
			{
				lQueuedSpeak.Detach();
			}
		}

		if	(
				(lResult == S_OK)
			&&	(lQueuedSpeak->mStarted)
			)
		{
			if	(SpeechIsBusy (lQueuedSpeak))
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d] QueuedSpeak [%p] [%d] [%d] is busy"), this, m_dwRef, mCharID, lQueuedSpeak.Ptr(), lQueuedSpeak->mCharID, lQueuedSpeak->mReqID);
				}
#endif
				lQueuedSpeak.Detach();
			}
		}
		else
		if	(
				(lResult == S_OK)
			&&	(!lQueuedSpeak->mStarted)
			)
		{
			if	(SpeechIsBusy (lQueuedSpeak))
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d] QueuedSpeak [%p] [%d] [%d] still busy"), this, m_dwRef, mCharID, lQueuedSpeak.Ptr(), lQueuedSpeak->mCharID, lQueuedSpeak->mReqID);
				}
#endif
				lQueuedSpeak.Detach();
			}
			else
			if	(
					(!lQueuedSpeak->mAnimated)
				&&	(ShowSpeechAnimation (lQueuedSpeak))
				)
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d] QueuedSpeak [%p] [%d] [%d] animated"), this, m_dwRef, mCharID, lQueuedSpeak.Ptr(), lQueuedSpeak->mCharID, lQueuedSpeak->mReqID);
				}
#endif
				lQueuedSpeak.Detach();
			}
			else
			if	(
					(lQueuedSpeak->mAnimated)
				&&	(FAILED (lResult = PrepareSpeech (lQueuedSpeak)))
				)
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d] QueuedSpeak [%p] [%d] [%d] failed"), this, m_dwRef, mCharID, lQueuedSpeak.Ptr(), lQueuedSpeak->mCharID, lQueuedSpeak->mReqID);
				}
#endif
			}
			else
			{
				lQueuedSpeak->NotifyStarted (mNotify);

				if	(mQueue.GetNextAction (QueueActionSpeak) == lQueuedSpeak)
				{
					if	((GetStyle () & WS_VISIBLE) == 0)
					{
						lResult = AGENTERR_CHARACTERNOTVISIBLE;
					}
					else
					{
#ifdef	_LOG_QUEUE_OPS
						if	(LogIsActive (_LOG_QUEUE_OPS))
						{
							LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d] Show QueuedSpeak [%p] [%d] [%d] [%s] [%s]"), this, m_dwRef, mCharID, lQueuedSpeak.Ptr(), lQueuedSpeak->mCharID, lQueuedSpeak->mReqID, DebugStr(lQueuedSpeak->mText.GetFullText()), DebugStr(lQueuedSpeak->mSoundUrl));
						}
#endif
						mLastSpeech = lQueuedSpeak->mText;
						if	(SUCCEEDED (lResult = StartSpeech (lQueuedSpeak)))
						{
							lQueuedSpeak.Detach();
						}
					}
				}
				else
				{
					lQueuedSpeak.Detach (); // Was deleted during NotifyStarted
				}
			}
		}

		if	(
				(lQueuedSpeak)
			&&	(mQueue.GetNextAction (QueueActionSpeak) == lQueuedSpeak)
			)
		{
			mQueue.RemoveHead ();

			StopMouthAnimation ();

			if	(lQueuedSpeak->mVoice)
			{
				lQueuedSpeak->mVoice->RemoveEventSink (this);
			}
			if	(
					(lQueuedSpeak->mShowBalloon)
				&&	(mBalloonWnd->GetSafeHwnd())
				)
			{
				mBalloonWnd->AbortSpeechText ();
			}

#ifdef	_LOG_QUEUE_OPS
			if	(LogIsActive (_LOG_QUEUE_OPS))
			{
				LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d] Done QueuedSpeak [%p] [%d] [%d] [%s] [%s] [%8.8X]"), this, m_dwRef, mCharID, lQueuedSpeak.Ptr(), lQueuedSpeak->mCharID, lQueuedSpeak->mReqID, DebugStr(lQueuedSpeak->mText.GetFullText()), DebugStr(lQueuedSpeak->mSoundUrl), lResult);
			}
#endif
			lQueuedSpeak->NotifyComplete (mNotify, lResult);
		}
#ifdef	DebugTimeStart
		DebugTimeStop
		LogMessage (LogIfActive|LogHighVolume|LogTimeMs, _T("%f CAgentPopupWnd::DoQueuedSpeak"), DebugTimeElapsed);
#endif
		return true;
	}
	return false;
}

void CAgentPopupWnd::AbortQueuedSpeak (CQueuedAction * pQueuedAction, HRESULT pReqStatus, LPCTSTR pReason)
{
	CQueuedSpeak *	lQueuedSpeak;

	if	(lQueuedSpeak = (CQueuedSpeak *) pQueuedAction)
	{
#ifdef	_DEBUG_SPEECH
		if	(LogIsActive (_DEBUG_SPEECH))
		{
			LogMessage (_DEBUG_SPEECH, _T("[%p(%d)] AbortQueuedSpeak [%d] [%d] Started [%u] Animated [%u] Belloon [%u]"), this, m_dwRef, lQueuedSpeak->mCharID, lQueuedSpeak->mReqID, lQueuedSpeak->mStarted, lQueuedSpeak->mAnimated, lQueuedSpeak->mShowBalloon);
		}
#endif
		if	(lQueuedSpeak->mStarted)
		{
			CDirectSoundLipSync *	lLipSync;

			if	(lQueuedSpeak->mVoice->SafeIsValid())
			{
				lQueuedSpeak->mVoice->Stop ();
				lQueuedSpeak->mVoice->ClearEventSinks ();
			}

			if	(
					(!lQueuedSpeak->mSoundUrl.IsEmpty ())
				&&	(lLipSync = DYNAMIC_DOWNCAST (CDirectSoundLipSync, lQueuedSpeak->mSoundFilter.Ptr()))
				)
			{
				lLipSync->Stop ();
				lLipSync->Disconnect ();
			}

			StopMouthAnimation ();

			if	(
					(lQueuedSpeak->mShowBalloon)
				&&	(mBalloonWnd->GetSafeHwnd())
				)
			{
#ifdef	_STRICT_COMPATIBILITY
				mBalloonWnd->HideBalloon (true);
#else
				mBalloonWnd->AbortSpeechText ();
#endif
			}
		}
		else
		if	(lQueuedSpeak->mAnimated)
		{
			if	(!ShowGesture (NULL))
			{
				Stop ();
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentPopupWnd::SpeechIsBusy (CQueuedSpeak * pQueuedSpeak)
{
	bool	lRet = false;
#ifdef	DebugTimeStart
	DebugTimeStart
#endif

	if	(pQueuedSpeak)
	{
		if	(pQueuedSpeak->mStarted)
		{
			if	(
					(
						(pQueuedSpeak->mSoundUrl.IsEmpty ())
					&&	(pQueuedSpeak->mVoice->SafeIsValid ())
					&&	(pQueuedSpeak->mVoice->SafeIsSpeaking ())
					)
				||	(
						(pQueuedSpeak->mShowBalloon)
					&&	(mBalloonWnd->GetSafeHwnd())
					&&	(mBalloonWnd->IsBusy (false))
					)
				)
			{
				lRet = true;
			}
		}
		else
		{
			if	(
					(
						(pQueuedSpeak->mVoice->SafeIsValid ())
					&&	(pQueuedSpeak->mVoice->SafeIsSpeaking ())
					)
				||	(
						(!pQueuedSpeak->mSoundUrl.IsEmpty ())
					&&	(PathIsURL (pQueuedSpeak->mSoundUrl))
					)
				||	(
						(pQueuedSpeak->mShowBalloon)
					&&	(mBalloonWnd->GetSafeHwnd())
#ifdef	_STRICT_COMPATIBILITY
					&&	(mBalloonWnd->IsBusy (false))
#else
					&&	(mBalloonWnd->IsBusy (mBalloonWnd->IsAutoSize()))
#endif
					)
				)
			{
				lRet = true;
			}
		}
	}

#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogHighVolume|LogTimeMs, _T("%f   CAgentPopupWnd::SpeechIsBusy [%u]"), DebugTimeElapsed, lRet);
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CAgentPopupWnd::SpeechIsReady (CQueuedSpeak * pQueuedSpeak)
{
	HRESULT	lResult = S_OK;
#ifdef	DebugTimeStart
	DebugTimeStart
#endif

	if	(pQueuedSpeak)
	{
		if	(!pQueuedSpeak->mSoundUrl.IsEmpty ())
		{
			if	(PathIsURL (pQueuedSpeak->mSoundUrl))
			{
				int				lNotifyNdx;
				IDaNotify *		lNotify;
				CFileDownload *	lSoundDownload = NULL;

				for	(lNotifyNdx = 0; lNotifyNdx <= mNotify.GetUpperBound (); lNotifyNdx++)
				{
					if	(
							(lNotify = mNotify [lNotifyNdx])
						&&	(lSoundDownload = lNotify->_FindSoundDownload (pQueuedSpeak->mSoundUrl))
						)
					{
						break;
					}
				}

				if	(
						(lSoundDownload)
					&&	(lSoundDownload->IsDownloadComplete () == S_OK)
					)
				{
					pQueuedSpeak->mSoundUrl = lSoundDownload->GetCacheName ();
				}
				else
				if	(
						(lSoundDownload)
					&&	(
							(lSoundDownload->IsDownloadStarted ())
						||	(lSoundDownload->IsDownloadStarting ())
						)
					&&	(!lSoundDownload->IsDownloadCancelling ())
					)
				{
#ifdef	_LOG_QUEUE_OPS
					if	(LogIsActive (_LOG_QUEUE_OPS))
					{
						LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d] QueuedSpeak [%p] [%d] [%d] waiting for [%s]"), this, m_dwRef, mCharID, pQueuedSpeak, pQueuedSpeak->mCharID, pQueuedSpeak->mReqID, DebugStr(pQueuedSpeak->mSoundUrl));
					}
#endif
					lResult = S_FALSE;
				}
				else
				{
#ifdef	_LOG_QUEUE_OPS
					if	(LogIsActive (_LOG_QUEUE_OPS))
					{
						LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d] QueuedSpeak [%p] [%d] [%d] failed for [%s]"), this, m_dwRef, mCharID, pQueuedSpeak, pQueuedSpeak->mCharID, pQueuedSpeak->mReqID, DebugStr(pQueuedSpeak->mSoundUrl));
					}
#endif
					lResult = AGENTPROVERROR_WAVEINVALID;
				}
			}
		}
		else
		if	(!pQueuedSpeak->mVoice->SafeIsValid())
		{
			lResult = AGENTWARNING_TTSENGINENOTFOUND;
		}
	}

#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogHighVolume|LogTimeMs, _T("%f   CAgentPopupWnd::SpeechIsReady [%8.8X]"), DebugTimeElapsed, lResult);
#endif
	return lResult;
}

HRESULT CAgentPopupWnd::PrepareSpeech (CQueuedSpeak * pQueuedSpeak)
{
	HRESULT	lResult = S_FALSE;
#ifdef	DebugTimeStart
	DebugTimeStart
#endif

	if	(pQueuedSpeak)
	{
		if	(!pQueuedSpeak->mSoundUrl.IsEmpty ())
		{
			if	(!pQueuedSpeak->mSoundFilter)
			{
				tPtr <CDirectSoundLipSync>	lLipSync;

				if	(lLipSync = (CDirectSoundLipSync*)CDirectSoundLipSync::CreateObject ())
				{
					lResult = lLipSync->Connect (mGraphBuilder, pQueuedSpeak->mSoundUrl, GetAgentStreamInfo());
					if	(SUCCEEDED (lResult))
					{
						pQueuedSpeak->mSoundFilter = lLipSync.Detach ();
					}
				}
				else
				{
					lResult = E_OUTOFMEMORY;
				}
			}
		}
		else
		if	(pQueuedSpeak->mVoice->SafeIsValid())
		{
#ifndef	_WIN64
			CSapi4Voice *	lSapi4Voice;
			CAgentFile *	lAgentFile;

			if	(lSapi4Voice = DYNAMIC_DOWNCAST (CSapi4Voice, pQueuedSpeak->mVoice))
			{
				if	(
						(lAgentFile = GetAgentFile())
					&&	(lAgentFile->GetTts().mPitch > 0)
					)
				{
					LogSapi4Err (LogNormal, lSapi4Voice->SetPitch (lAgentFile->GetTts().mPitch));
				}
				if	(
						(lAgentFile)
					&&	(lAgentFile->GetTts().mSpeed > 0)
					)
				{
					LogSapi4Err (LogNormal, lSapi4Voice->SetRate (CDaAudioOutputConfig().ApplyVoiceRate (lAgentFile->GetTts().mSpeed, 4)));
				}
				else
				{
					LogSapi4Err (LogNormal, lSapi4Voice->SetRate (CDaAudioOutputConfig().ApplyVoiceRate (lSapi4Voice->GetDefaultRate(), 4)));
				}
			}
			else
#endif
			{
				LogSapi5Err (LogNormal, pQueuedSpeak->mVoice->SetRate (CDaAudioOutputConfig().CalcVoiceRate()));
			}
			lResult = S_OK;
		}
	}

#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogHighVolume|LogTimeMs, _T("%f   CAgentPopupWnd::PrepareSpeech"), DebugTimeElapsed);
#endif
	return lResult;
}

HRESULT CAgentPopupWnd::StartSpeech (CQueuedSpeak * pQueuedSpeak)
{
	HRESULT	lResult = S_FALSE;
#ifdef	DebugTimeStart
	DebugTimeStart
#endif

	if	(pQueuedSpeak)
	{
		bool					lBalloonShown = false;
		CDirectSoundLipSync *	lLipSync;

		if	(!pQueuedSpeak->mSoundUrl.IsEmpty ())
		{
			if	(lLipSync = DYNAMIC_DOWNCAST (CDirectSoundLipSync, pQueuedSpeak->mSoundFilter.Ptr()))
			{
				if	(StartMouthAnimation ((long)(lLipSync->GetDuration() / MsPer100Ns)))
				{
					PlayMouthAnimation (-1, false);
				}
			}
			if	(
					(mQueue.GetNextAction (QueueActionSpeak) == pQueuedSpeak)
				&&	(pQueuedSpeak->mShowBalloon)
				&&	(GetBalloonWnd (true))
				&&	(mBalloonWnd->GetSafeHwnd())
				)
			{
				lBalloonShown = mBalloonWnd->ShowBalloonSpeech (pQueuedSpeak->mText, true);
			}

			if	(mQueue.GetNextAction (QueueActionSpeak) == pQueuedSpeak)
			{
#ifdef	_DEBUG_SPEECH
				if	(LogIsActive (_DEBUG_SPEECH))
				{
					LogMessage (_DEBUG_SPEECH, _T("[%p(%d)] [%d] CAgentPopupWnd Speak [%s] [%s]"), this, m_dwRef, mCharID, DebugStr(pQueuedSpeak->mText.GetFullText()), DebugStr(pQueuedSpeak->mSoundUrl));
				}
#endif
				lResult = PlayFromTo (0, GetDurationMs(), true);
			}
		}
		else
		if	(pQueuedSpeak->mVoice->SafeIsValid())
		{
			pQueuedSpeak->mVoice->ClearEventSinks ();
			pQueuedSpeak->mVoice->AddEventSink (this);
			pQueuedSpeak->mVoice->SetEventCharID (pQueuedSpeak->mCharID);

			if	(
					(mQueue.GetNextAction (QueueActionSpeak) == pQueuedSpeak)
				&&	(pQueuedSpeak->mShowBalloon)
				&&	(GetBalloonWnd (true))
				&&	(mBalloonWnd->GetSafeHwnd())
				)
			{
				pQueuedSpeak->mVoice->AddEventSink (mBalloonWnd);
				lBalloonShown = mBalloonWnd->ShowBalloonSpeech (pQueuedSpeak->mText);
			}

			if	(mQueue.GetNextAction (QueueActionSpeak) == pQueuedSpeak)
			{
#ifdef	_DEBUG_SPEECH
				if	(LogIsActive (_DEBUG_SPEECH))
				{
					LogMessage (_DEBUG_SPEECH, _T("[%p(%d)] [%d] CAgentPopupWnd Speak   [%s]"), this, m_dwRef, mCharID, DebugStr(pQueuedSpeak->mText.GetSpeechText()));
					LogMessage (_DEBUG_SPEECH, _T("[%p(%d)] [%d]                Text    [%s]"), this, m_dwRef, mCharID, DebugStr(pQueuedSpeak->mText.GetFullText()));
					LogMessage (_DEBUG_SPEECH, _T("[%p(%d)] [%d]                Voice   [%u] Rate [%u]"), this, m_dwRef, mCharID, pQueuedSpeak->mVoice->SafeIsValid (), pQueuedSpeak->mVoice->GetRate());
					if	(
							(pQueuedSpeak->mShowBalloon)
						&&	(mBalloonWnd->GetSafeHwnd())
						)
					{
						LogMessage (_DEBUG_SPEECH, _T("[%p(%d)] [%d]                Balloon [%u] AutoSize [%u] AutoPace [%u] AutoHide [%u]"), this, m_dwRef, mCharID, mBalloonWnd->IsWindowVisible(), mBalloonWnd->IsAutoSize(), mBalloonWnd->IsAutoPace(), mBalloonWnd->IsAutoHide());
					}
				}
#endif
				lResult = LogComErr (LogNormal, pQueuedSpeak->mVoice->Speak (pQueuedSpeak->mText.GetSpeechText()));
			}
		}
		else
		{
			lResult = E_FAIL;
		}

		if	(FAILED (lResult))
		{
			if	(mBalloonWnd->GetSafeHwnd())
			{
				mBalloonWnd->AbortSpeechText ();
				mBalloonWnd->HideBalloon (true);
			}
		}
		else
		if	(lBalloonShown)
		{
			mBalloonWnd->ShowBalloonAuto ();
		}
	}

#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogHighVolume|LogTimeMs, _T("%f   CAgentPopupWnd::StartSpeech"), DebugTimeElapsed);
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentPopupWnd::ShowSpeechAnimation (CQueuedSpeak * pQueuedSpeak)
{
	bool	lRet = false;
#ifdef	DebugTimeStart
	DebugTimeStart
#endif

	if	(
			(pQueuedSpeak)
		&&	(!pQueuedSpeak->mAnimated)
		)
	{
		CAgentStreamInfo *		lStreamInfo;
		long					lAnimationNdx = -1;
		long					lSpeakingFrameNdx = -1;
		const CStringArray *	lGestures;
		LPCTSTR					lGesture;

		mQueue.RemoveHead ();
		pQueuedSpeak->mAnimated = true;

		if	(
				(lStreamInfo = GetAgentStreamInfo())
			&&	(lStreamInfo->GetAnimationIndex (&lAnimationNdx) == S_OK)
			&&	(lAnimationNdx >= 0)
			&&	(lStreamInfo->FindAnimationSpeakingFrame (&lSpeakingFrameNdx, lAnimationNdx) == S_OK)
			&&	(lSpeakingFrameNdx >= 0)
			)
		{
			mQueue.AddHead (pQueuedSpeak);
#ifdef	_DEBUG_SPEECH
			if	(LogIsActive (_DEBUG_SPEECH))
			{
				LogMessage (_DEBUG_SPEECH, _T("[%p(%d)] [%d]   Speech Animation [%d] [%ls] Frame [%d]"), this, m_dwRef, mCharID, lAnimationNdx, (BSTR)(GetAgentFile()->GetAnimation (lAnimationNdx)->mName), lSpeakingFrameNdx);
			}
#endif
		}
		else
		if	(
				(lGestures = GetFileStates() (_T("SPEAKING")))
			&&	(lGestures->GetSize() > 0)
			)
		{
			COwnPtrList <CQueuedAction>	lQueue;

			mQueue.PushQueue (lQueue);
			ShowState (_T("SPEAKING"));
			mQueue.AddTail (pQueuedSpeak);
			mQueue.PopQueue (lQueue);
#ifdef	_DEBUG_SPEECH
			if	(LogIsActive (_DEBUG_SPEECH))
			{
				LogMessage (_DEBUG_SPEECH, _T("[%p(%d)] [%d]   Showed SPEAKING state"), this, m_dwRef, mCharID);
			}
#endif
			lRet = true;
		}
		else
		if	(
				(ShowGesture (lGesture = _T("RestPose")))
			||	(ShowGesture (lGesture = _T("Explain")))
			)
		{
			mQueue.AddHead (pQueuedSpeak);
#ifdef	_DEBUG_SPEECH
			if	(LogIsActive (_DEBUG_SPEECH))
			{
				LogMessage (_DEBUG_SPEECH, _T("[%p(%d)] [%d]   Showed %s for speech"), this, m_dwRef, mCharID, lGesture);
			}
#endif
			lRet = true;
		}
		else
		{
			mQueue.AddHead (pQueuedSpeak);
		}
	}

#ifdef	DebugTimeStart
	DebugTimeStop
	LogMessage (LogIfActive|LogHighVolume|LogTimeMs, _T("%f   CAgentPopupWnd::ShowSpeechAnimation"), DebugTimeElapsed);
#endif
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentPopupWnd::StartMouthAnimation (long pSpeakingDuration)
{
	bool				lRet = false;
	CAgentFile *		lAgentFile;
	CAgentStreamInfo *	lStreamInfo;
	long				lAnimationNdx = -1;
	long				lSpeakingFrameNdx = -1;

	if	(
			(lAgentFile = GetAgentFile())
		&&	(lStreamInfo = GetAgentStreamInfo())
		)
	{
#ifdef	_DEBUG_SPEECH
		if	(
				(LogIsActive (_DEBUG_SPEECH))
			&&	(lStreamInfo->GetAnimationIndex (&lAnimationNdx) == S_OK)
			&&	(lAnimationNdx >= 0)
			)
		{
			LogMessage (_DEBUG_SPEECH, _T("[%p(%d)] [%d]   Last Animation [%d] [%ls]"), this, m_dwRef, mCharID, lAnimationNdx, (BSTR)(lAgentFile->GetAnimation (lAnimationNdx)->mName));
		}
#endif
		if	(
				(lStreamInfo->GetAnimationIndex (&lAnimationNdx) == S_OK)
			&&	(lAnimationNdx >= 0)
			&&	(lStreamInfo->FindAnimationSpeakingFrame (&lSpeakingFrameNdx, lAnimationNdx) == S_OK)
			&&	(lSpeakingFrameNdx >= 0)
			)
		{
			lStreamInfo->ClearAnimationSequences ();

			if	(
					(lStreamInfo->NewAnimationSequence () == S_OK)
				&&	(lStreamInfo->SequenceAnimationFrame (lAnimationNdx, lSpeakingFrameNdx) == S_OK)
				)
			{
				lStreamInfo->SetSpeakingDuration (max (pSpeakingDuration, 60000));
				AnimationSequenceChanged ();
#ifdef	_DEBUG_SPEECH
				if	(LogIsActive (_DEBUG_SPEECH))
				{
					LogMessage (_DEBUG_SPEECH, _T("[%p(%d)] [%d]   Speech MouthAnimation [%d] [%ls] Frame [%d] started [%d]"), this, m_dwRef, mCharID, lAnimationNdx, (BSTR)(lAgentFile->GetAnimation (lAnimationNdx)->mName), lSpeakingFrameNdx, pSpeakingDuration);
				}
#endif
				PlayMouthAnimation (-1, true);
				lRet = true;
			}
		}
	}
	return lRet;
}

bool CAgentPopupWnd::StopMouthAnimation ()
{
	bool				lRet;
	CAgentStreamInfo *	lStreamInfo;

	lRet = PlayMouthAnimation (-1, true);

	if	(
			(lStreamInfo = GetAgentStreamInfo())
		&&	(lStreamInfo->SetSpeakingDuration (0))
		)
	{
		AnimationSequenceChanged ();
#ifdef	_DEBUG_SPEECH
		if	(LogIsActive (_DEBUG_SPEECH))
		{
			LogMessage (_DEBUG_SPEECH, _T("[%p(%d)] [%d]   Speech MouthAnimation stopped"), this, m_dwRef, mCharID);
		}
#endif
		lRet = true;
	}
	return lRet;
}

bool CAgentPopupWnd::PlayMouthAnimation (short pMouthOverlayNdx, bool pPlayAlways)
{
	bool				lRet = false;
	CAgentStreamInfo *	lStreamInfo;

	if	(lStreamInfo = GetAgentStreamInfo())
	{
		if	(
				(
					(
						(pMouthOverlayNdx < 0)
					?	(lStreamInfo->ResetMouthOverlays ())
					:	(lStreamInfo->SetMouthOverlay (pMouthOverlayNdx))
					)
				||	(pPlayAlways)
				)
			&&	(
					(pPlayAlways)
				||	(IsAnimationComplete ())
				)
			)
		{
			long	lStopPosition = GetDurationMs();
			long	lStartPosition = max (lStopPosition - 100000, 0);

#ifdef	_DEBUG_SPEECH
			if	(LogIsActive (_DEBUG_SPEECH))
			{
				LogMessage (_DEBUG_SPEECH, _T("[%p(%d)] [%d]   Speech MouthAnimation [%d] from [%d] to [%d]"), this, m_dwRef, mCharID, pMouthOverlayNdx, lStartPosition, lStopPosition);
			}
#endif
			if	(SUCCEEDED (PlayFromTo (lStartPosition, lStopPosition, (pMouthOverlayNdx < 0))))
			{
				lRet = true;
			}
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

CQueuedAction * CAgentPopupWnd::IsSpeakQueued (long pCharID)
{
	return mQueue.GetCharAction (QueueActionSpeak, pCharID);
}

bool CAgentPopupWnd::RemoveQueuedSpeak (long pCharID, HRESULT pReqStatus, LPCTSTR pReason, bool pExcludeActive)
{
	bool			lRet = false;
	CQueuedSpeak *	lQueuedSpeak;

#ifdef	_DEBUG_SPEECH
	if	(LogIsActive (_DEBUG_SPEECH))
	{
		LogMessage (_DEBUG_SPEECH, _T("[%p(%d)] RemoveQueuedSpeak for [%d] [%8.8X] [%s]"), this, m_dwRef, pCharID, pReqStatus, pReason);
	}
#endif
	while	(
				(lQueuedSpeak = (CQueuedSpeak *) mQueue.GetCharAction (QueueActionSpeak, pCharID, true))
			&&	(
					(!pExcludeActive)
				||	(!lQueuedSpeak->mStarted)
				)
			&&	(RemoveQueuedAction (lQueuedSpeak, pReqStatus, pReason))
			)
	{
		lRet = true;
	}
	return lRet;
}

bool CAgentPopupWnd::UpdateQueuedSpeak (long pCharID, CSapiVoice * pVoice)
{
	bool			lRet = false;
	POSITION		lPos;
	CQueuedAction *	lQueuedAction;
	CQueuedSpeak *	lQueuedSpeak;

	if	(pVoice)
	{
		for	(lPos = mQueue.GetHeadPosition(); lPos;)
		{
			if	(
					(lQueuedAction = mQueue.GetNext (lPos))
				&&	(lQueuedAction->mAction == QueueActionSpeak)
				&&	(lQueuedAction->mCharID == pCharID)
				)
			{
				lQueuedSpeak = (CQueuedSpeak *) lQueuedAction;
				if	(
						(!lQueuedSpeak->mStarted)
					&&	(lQueuedSpeak->SetVoice (pVoice))
					)
				{
					lRet = true;
				}
			}
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentPopupWnd::KeepBalloonVisible (CAgentBalloonWnd * pBalloon)
{
	bool				lRet = false;
	CQueuedAction *		lQueuedAction;
	CQueuedThink *		lQueuedThink;
	CQueuedSpeak *		lQueuedSpeak;
	POSITION			lPos;

	if	(
			(!mQueue.IsEmpty ())
		&&	(lPos = mQueue.GetHeadPosition ())
		&&	(lQueuedAction = mQueue.GetNext (lPos))
		)
	{
		if	(
				(lQueuedAction->mAction == QueueActionThink)
			&&	(lQueuedThink = (CQueuedThink *) lQueuedAction)
			&&	(
					(!lQueuedThink->mStarted)
				||	(
						(lQueuedThink->mStarted)
					&&	(lPos)
					&&	(lQueuedAction = mQueue.GetNext (lPos))
					&&	(lQueuedAction->mAction == QueueActionThink)
					&&	(lQueuedThink = (CQueuedThink *) lQueuedAction)
					&&	(!lQueuedThink->mStarted)
					)
				)
			)
		{
			lRet = true;
		}
		else
		if	(
				(lQueuedAction->mAction == QueueActionSpeak)
			&&	(lQueuedSpeak = (CQueuedSpeak *) lQueuedAction)
			&&	(lQueuedSpeak->mShowBalloon)
			&&	(
					(!lQueuedSpeak->mStarted)
				||	(
						(lQueuedSpeak->mStarted)
					&&	(lPos)
					&&	(lQueuedAction = mQueue.GetNext (lPos))
					&&	(lQueuedAction->mAction == QueueActionSpeak)
					&&	(lQueuedSpeak = (CQueuedSpeak *) lQueuedAction)
					&&	(lQueuedSpeak->mShowBalloon)
					&&	(!lQueuedSpeak->mStarted)
					)
				)
			)
		{
			lRet = true;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAgentPopupWnd::OnVoiceStart (long pCharID)
{
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH_EVENTS))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p(%d)] [%d] CAgentPopupWnd   OnVoiceStart"), this, m_dwRef, mCharID);
	}
#endif
	SendMessage (mVoiceStartMsg, pCharID);
}

void CAgentPopupWnd::OnVoiceEnd (long pCharID)
{
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH_EVENTS))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p(%d)] [%d] CAgentPopupWnd   OnVoiceEnd"), this, m_dwRef, mCharID);
	}
#endif
	SendMessage (mVoiceEndMsg, pCharID);
}

void CAgentPopupWnd::OnVoiceBookMark (long pCharID, long pBookMarkId)
{
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH_EVENTS))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p(%d)] [%d] CAgentPopupWnd   OnVoiceBookMark [%d] [%d]"), this, m_dwRef, mCharID, pCharID, pBookMarkId);
	}
#endif
	SendMessage (mVoiceBookMarkMsg, pCharID, pBookMarkId);
}

void CAgentPopupWnd::OnVoiceVisual (long pCharID, int pMouthOverlay)
{
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p(%d)] [%d] CAgentPopupWnd     OnVoiceVisual [%s]"), this, m_dwRef, mCharID, MouthOverlayStr(pMouthOverlay));
	}
#endif
	SendMessage (mVoiceVisualMsg, pCharID, pMouthOverlay);
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentPopupWnd::OnVoiceStartMsg (WPARAM wParam, LPARAM lParam)
{
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH_EVENTS))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p(%d)] [%d] CAgentPopupWnd   OnVoiceStartMsg"), this, m_dwRef, mCharID);
	}
#endif
	if	(StartMouthAnimation ())
	{
		PlayMouthAnimation (-1, false);
	}
	return 0;
}

LRESULT CAgentPopupWnd::OnVoiceEndMsg (WPARAM wParam, LPARAM lParam)
{
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH_EVENTS))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p(%d)] [%d] CAgentPopupWnd   OnVoiceEndMsg"), this, m_dwRef, mCharID);
	}
#endif
	StopMouthAnimation ();
	return 0;
}

LRESULT CAgentPopupWnd::OnVoiceBookMarkMsg (WPARAM wParam, LPARAM lParam)
{
	long	lCharID = (long)wParam;
	long	lBookMarkId = (long)lParam;
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH_EVENTS))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p(%d)] [%d] CAgentPopupWnd   OnVoiceBookMarkMsg [%d] [%d]"), this, m_dwRef, mCharID, lCharID, lBookMarkId);
	}
#endif
	if	(PreNotify ())
	{
		try
		{
			int			lNotifyNdx;
			IDaNotify *	lNotify;

			for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
			{
				if	(lNotify->_GetNotifyClient (mCharID) == lCharID)
				{
					lNotify->BookMark (lBookMarkId);
				}
			}
		}
		catch AnyExceptionDebug
		PostNotify ();
	}
	return 0;
}

LRESULT CAgentPopupWnd::OnVoiceVisualMsg (WPARAM wParam, LPARAM lParam)
{
	int	lMouthOverlay = (int)lParam;
#ifdef	_DEBUG_SPEECH_EVENTS
	if	(LogIsActive (_DEBUG_SPEECH))
	{
		LogMessage (_DEBUG_SPEECH_EVENTS, _T("[%p(%d)] [%d] CAgentPopupWnd     OnVoiceVisualMsg [%s]"), this, m_dwRef, mCharID, MouthOverlayStr(lMouthOverlay));
	}
#endif
	PlayMouthAnimation (lMouthOverlay, true);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

long CAgentPopupWnd::QueueWait (long pCharID, long pOtherCharID, long pOtherReqID)
{
	long			lReqID = 0;
	CQueuedWait *	lQueuedWait = NULL;

	if	(
			(IsWindow (m_hWnd))
		&&	(lQueuedWait = new CQueuedWait (pOtherCharID, pOtherReqID, pCharID, lReqID=NextReqID()))
		)
	{
		mQueue.AddTail (lQueuedWait);
#ifdef	_LOG_QUEUE_OPS
		if	(LogIsActive (_LOG_QUEUE_OPS))
		{
			LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d] QueueWait [%p] [%d] [%d] Other [%d] [%d]"), this, m_dwRef, mCharID, lQueuedWait, pCharID, lReqID, pOtherCharID, pOtherReqID);
		}
#endif
	}
	else
	{
		lReqID = 0;
	}
	return lReqID;
}

bool CAgentPopupWnd::DoQueuedWait ()
{
	tPtr <CQueuedWait>	lQueuedWait;

	if	(lQueuedWait = (CQueuedWait *) mQueue.GetNextAction (QueueActionWait))
	{
		CQueuedAction *		lOtherRequest;
		CAgentPopupWnd *	lRequestOwner = NULL;

#ifdef	_LOG_QUEUE_OPS
		if	(LogIsActive (_LOG_QUEUE_OPS))
		{
			LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d] QueuedWait [%p] [%d] [%d] Other [%d] [%d]"), this, m_dwRef, mCharID, lQueuedWait.Ptr(), lQueuedWait->mCharID, lQueuedWait->mReqID, lQueuedWait->mOtherCharID, lQueuedWait->mOtherReqID);
		}
#endif

		if	(!lQueuedWait->mStarted)
		{
			lQueuedWait->NotifyStarted (mNotify);
		}
		if	(mQueue.GetNextAction (QueueActionWait) == lQueuedWait)
		{
			mQueue.RemoveHead ();

			if	(
					(lOtherRequest = FindOtherRequest (lQueuedWait->mOtherReqID, lRequestOwner))
				&&	(lRequestOwner)
				)
			{
#ifdef	_LOG_QUEUE_OPS
					if	(LogIsActive (_LOG_QUEUE_OPS))
					{
						LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d]   Waiting for [%p] [%d] [%d]"), this, m_dwRef, mCharID, lOtherRequest, lQueuedWait->mOtherCharID, lQueuedWait->mOtherReqID);
					}
#endif
				lQueuedWait->mStarted = true;
				mQueue.AddHead (lQueuedWait.Detach ());
			}
			else
			if	(lQueuedWait->mStarted)
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d]   Wait done for [%p] [%d] [%d]"), this, m_dwRef, mCharID, lOtherRequest, lQueuedWait->mOtherCharID, lQueuedWait->mOtherReqID);
				}
#endif
				if	(mNotify.GetSize() > 0)
				{
					lQueuedWait->NotifyComplete (mNotify);
				}
			}
			else
			{
				if	(mNotify.GetSize() > 0)
				{
					lQueuedWait->NotifyComplete (mNotify, AGENTREQERR_REMOVED);
				}
			}
		}
		else
		{
			lQueuedWait.Detach (); // Was deleted during NotifyStarted
		}
		return true;
	}
	return false;
}

void CAgentPopupWnd::AbortQueuedWait (CQueuedAction * pQueuedAction, HRESULT pReqStatus, LPCTSTR pReason)
{
}

/////////////////////////////////////////////////////////////////////////////

CQueuedAction * CAgentPopupWnd::IsWaitQueued (long pCharID)
{
	return mQueue.GetCharAction (QueueActionWait, pCharID);
}

bool CAgentPopupWnd::RemoveQueuedWait (long pCharID, HRESULT pReqStatus, LPCTSTR pReason, bool pExcludeActive)
{
	bool			lRet = false;
	CQueuedWait *	lQueuedWait;

	while	(
				(lQueuedWait = (CQueuedWait *) mQueue.GetCharAction (QueueActionWait, pCharID, true))
			&&	(
					(!pExcludeActive)
				||	(!lQueuedWait->mStarted)
				)
			&&	(RemoveQueuedAction (lQueuedWait, pReqStatus, pReason))
			)
	{
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

long CAgentPopupWnd::QueueInterrupt (long pCharID, long pOtherCharID, long pOtherReqID)
{
	long				lReqID = 0;
	CQueuedInterrupt *	lQueuedInterrupt = NULL;

	if	(
			(IsWindow (m_hWnd))
		&&	(lQueuedInterrupt = new CQueuedInterrupt (pOtherCharID, pOtherReqID, pCharID, lReqID=NextReqID()))
		)
	{
		mQueue.AddTail (lQueuedInterrupt);
#ifdef	_LOG_QUEUE_OPS
		if	(LogIsActive (_LOG_QUEUE_OPS))
		{
			LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d] QueueInterrupt [%p] [%d] [%d] Other [%d] [%d]"), this, m_dwRef, mCharID, lQueuedInterrupt, pCharID, lReqID, pOtherCharID, pOtherReqID);
		}
#endif
	}
	else
	{
		lReqID = 0;
	}
	return lReqID;
}

bool CAgentPopupWnd::DoQueuedInterrupt ()
{
	tPtr <CQueuedInterrupt>	lQueuedInterrupt;

	if	(lQueuedInterrupt = (CQueuedInterrupt *) mQueue.GetNextAction (QueueActionInterrupt))
	{
		CQueuedAction *		lOtherRequest;
		CAgentPopupWnd *	lRequestOwner = NULL;
		HRESULT				lStatus;

#ifdef	_LOG_QUEUE_OPS
		if	(LogIsActive (_LOG_QUEUE_OPS))
		{
			LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d] QueuedInterrupt [%p] [%d] [%d] Other [%d] [%d]"), this, m_dwRef, mCharID, lQueuedInterrupt.Ptr(), lQueuedInterrupt->mCharID, lQueuedInterrupt->mReqID, lQueuedInterrupt->mOtherCharID, lQueuedInterrupt->mOtherReqID);
		}
#endif
		if	(!lQueuedInterrupt->mStarted)
		{
			lQueuedInterrupt->NotifyStarted (mNotify);
		}
		if	(mQueue.GetNextAction (QueueActionInterrupt) == lQueuedInterrupt)
		{
			mQueue.RemoveHead ();

			if	(
					(lOtherRequest = FindOtherRequest (lQueuedInterrupt->mOtherReqID, lRequestOwner))
				&&	(lRequestOwner)
				&&	(lRequestOwner->RemoveQueuedAction (lOtherRequest, AGENTREQERR_INTERRUPTEDCODE, _T("Interrupt")))
				)
			{
#ifdef	_LOG_QUEUE_OPS
				if	(LogIsActive (_LOG_QUEUE_OPS))
				{
					LogMessage (_LOG_QUEUE_OPS, _T("[%p(%d)] [%d]   Interrupted [%p] [%d] [%d]"), this, m_dwRef, mCharID, lOtherRequest, lQueuedInterrupt->mOtherCharID, lQueuedInterrupt->mOtherReqID);
				}
#endif
				lStatus = S_OK;
			}
			else
			{
				lStatus = AGENTREQERR_REMOVED;
			}

			if	(mNotify.GetSize() > 0)
			{
				lQueuedInterrupt->NotifyComplete (mNotify, lStatus);
			}
		}
		else
		{
			lQueuedInterrupt.Detach (); // Was deleted during NotifyStarted
		}
		return true;
	}
	return false;
}

void CAgentPopupWnd::AbortQueuedInterrupt (CQueuedAction * pQueuedAction, HRESULT pReqStatus, LPCTSTR pReason)
{
}

/////////////////////////////////////////////////////////////////////////////

CQueuedAction * CAgentPopupWnd::IsInterruptQueued (long pCharID)
{
	return mQueue.GetCharAction (QueueActionInterrupt, pCharID);
}

bool CAgentPopupWnd::RemoveQueuedInterrupt (long pCharID, HRESULT pReqStatus, LPCTSTR pReason, bool pExcludeActive)
{
	bool				lRet = false;
	CQueuedInterrupt *	lQueuedInterrupt;

	while	(
				(lQueuedInterrupt = (CQueuedInterrupt *) mQueue.GetCharAction (QueueActionInterrupt, pCharID, true))
			&&	(
					(!pExcludeActive)
				||	(!lQueuedInterrupt->mStarted)
				)
			&&	(RemoveQueuedAction (lQueuedInterrupt, pReqStatus, pReason))
			)
	{
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

CQueuedAction * CAgentPopupWnd::FindOtherRequest (long pReqID, CAgentPopupWnd *& pRequestOwner)
{
	CQueuedAction *		lRet = NULL;
	int					lNotifyNdx;
	IDaNotify *			lNotify;
	CAgentPopupWnd *	lRequestOwner;

	pRequestOwner = NULL;

	for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
	{
		if	(
				(lRequestOwner = DYNAMIC_DOWNCAST (CAgentPopupWnd, lNotify->_GetRequestOwner (pReqID)))
			&&	(lRet = lRequestOwner->FindQueuedAction (pReqID))
			)
		{
			pRequestOwner = lRequestOwner;
			break;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentPopupWnd::DoQueuedPrepare ()
{
	tPtr <CQueuedPrepare>	lQueuedPrepare;

	if	(lQueuedPrepare = (CQueuedPrepare *) mQueue.GetNextAction (QueueActionPrepare))
	{
		if	(!lQueuedPrepare->IsStarted ())
		{
			lQueuedPrepare->NotifyStarted (mNotify);
		}
		if	(mQueue.GetNextAction (QueueActionPrepare) == lQueuedPrepare)
		{
			if	(lQueuedPrepare->IsSoundDownload ())
			{
				lQueuedPrepare->FinishDownloads ();
			}
			else
			{
				lQueuedPrepare->PutAnimations (GetAgentFile ());
			}

			if	(lQueuedPrepare->IsComplete ())
			{
				mQueue.RemoveHead ();

				if	(mNotify.GetSize() > 0)
				{
					HRESULT			lPrepareResult = S_OK;
					CFileDownload *	lFileDownload;

					if	(lFileDownload = lQueuedPrepare->GetDownload ())
					{
						lPrepareResult = lFileDownload->IsDownloadComplete ();
					}
					lQueuedPrepare->NotifyComplete (mNotify, lPrepareResult);
				}
			}
			else
			{
				lQueuedPrepare->StartDownloads ();
				lQueuedPrepare.Detach ();
			}
		}
		else
		{
			lQueuedPrepare.Detach (); // Was deleted during NotifyStarted
		}

		return true;
	}
	return false;
}

void CAgentPopupWnd::AbortQueuedPrepare (CQueuedAction * pQueuedAction, HRESULT pReqStatus, LPCTSTR pReason)
{
	CQueuedPrepare *	lQueuedPrepare;

	if	(lQueuedPrepare = (CQueuedPrepare *) pQueuedAction)
	{
		lQueuedPrepare->CancelDownloads ();
	}
}

/////////////////////////////////////////////////////////////////////////////

CQueuedAction * CAgentPopupWnd::IsPrepareQueued (long pCharID)
{
	return mQueue.GetCharAction (QueueActionPrepare, pCharID);
}

bool CAgentPopupWnd::RemoveQueuedPrepare (long pCharID, HRESULT pReqStatus, LPCTSTR pReason, bool pExcludeActive)
{
	bool				lRet = false;
	CQueuedPrepare *	lQueuedPrepare;

	while	(
				(lQueuedPrepare = (CQueuedPrepare *) mQueue.GetCharAction (QueueActionPrepare, pCharID, true))
			&&	(
					(!pExcludeActive)
				||	(!lQueuedPrepare->IsStarted ())
				)
			&&	(RemoveQueuedAction (lQueuedPrepare, pReqStatus, pReason))
			)
	{
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

int CAgentPopupWnd::IsIdle () const
{
	int	lRet = CAgentWnd::IsIdle ();

	if	(
			(lRet > 0)
		&&	(mBalloonWnd->GetSafeHwnd ())
		&&	(mBalloonWnd->IsBusy (true))
		)
	{
		lRet = 0;
	}
	return lRet;
}

bool CAgentPopupWnd::StopIdle (LPCTSTR pReason)
{
	bool	lRet;
	bool	lIdleStarted = mIdleStarted;

	lRet = CAgentWnd::StopIdle (pReason);

	if	(
			(lRet)
		&&	(lIdleStarted)
		&&	(PreNotify ())
		)
	{
		try
		{
			int			lNotifyNdx;
			IDaNotify *	lNotify;

			for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
			{
				lNotify->Idle (lNotify->_GetNotifyClient (mCharID), FALSE);
			}
		}
		catch AnyExceptionDebug
		PostNotify ();
	}
	return lRet;
}

bool CAgentPopupWnd::DoIdle ()
{
	bool	lRet;
	bool	lIdleStarted = mIdleStarted;

	lRet = CAgentWnd::DoIdle ();

	if	(
			(lRet)
		&&	(!lIdleStarted)
		&&	(mIdleStarted)
		&&	(PreNotify ())
		)
	{
		try
		{
			int			lNotifyNdx;
			IDaNotify *	lNotify;

			for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
			{
				lNotify->Idle (lNotify->_GetNotifyClient (mCharID), TRUE);
			}
		}
		catch AnyExceptionDebug
		PostNotify ();
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentPopupWnd::AbortQueuedAction (CQueuedAction * pQueuedAction, HRESULT pReqStatus, LPCTSTR pReason)
{
	CQueuedAction *	lQueuedAction;

	CAgentWnd::AbortQueuedAction (pQueuedAction, pReqStatus, pReason);

	if	(lQueuedAction = (CQueuedAction *) pQueuedAction)
	{
		if	(lQueuedAction->mAction == QueueActionShow)
		{
			AbortQueuedShow (pQueuedAction, pReqStatus, pReason);
		}
		else
		if	(lQueuedAction->mAction == QueueActionHide)
		{
			AbortQueuedHide (pQueuedAction, pReqStatus, pReason);
		}
		else
		if	(lQueuedAction->mAction == QueueActionMove)
		{
			AbortQueuedMove (pQueuedAction, pReqStatus, pReason);
		}
		else
		if	(lQueuedAction->mAction == QueueActionThink)
		{
			AbortQueuedThink (pQueuedAction, pReqStatus, pReason);
		}
		else
		if	(lQueuedAction->mAction == QueueActionSpeak)
		{
			AbortQueuedSpeak (pQueuedAction, pReqStatus, pReason);
		}
		else
		if	(lQueuedAction->mAction == QueueActionWait)
		{
			AbortQueuedWait (pQueuedAction, pReqStatus, pReason);
		}
		else
		if	(lQueuedAction->mAction == QueueActionInterrupt)
		{
			AbortQueuedInterrupt (pQueuedAction, pReqStatus, pReason);
		}
		else
		if	(lQueuedAction->mAction == QueueActionPrepare)
		{
			AbortQueuedPrepare (pQueuedAction, pReqStatus, pReason);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAgentPopupWnd::OnTimer (UINT_PTR nIDEvent)
{
	if	(!mQueue.IsEmpty ())
	{
		StopIdle (_T("Queue"));
	}

	CAgentWnd::OnTimer (nIDEvent);

	if	(
			(mQueueTimer)
		&&	(nIDEvent == mQueueTimer)
		)
	{
		if	(
				(!mQueue.IsEmpty ())
			&&	(IsAnimationComplete (true))
			)
		{
			if	(PreDoQueue () > 0)
			{
				bool			lActivateQueue = false;
				CQueuedAction *	lNextAction = mQueue.GetNextAction ();

				try
				{
					if	(
							(DoQueuedShow ())
						||	(DoQueuedHide ())
						||	(DoQueuedMove ())
						||	(DoQueuedThink ())
						||	(DoQueuedSpeak ())
						||	(DoQueuedWait ())
						||	(DoQueuedInterrupt ())
						||	(DoQueuedPrepare ())
						)
					{
						lActivateQueue = true;
					}
				}
				catch AnyExceptionSilent

				if	(
						(PostDoQueue () >= 0)
					&&	(lActivateQueue)
					)
				{
					ActivateQueue ((mQueue.GetNextAction () != lNextAction));
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

short CAgentPopupWnd::NotifyKeyState () const
{
	short	lRet = 0;

	if	(GetKeyState (VK_SHIFT) < 0)
	{
		lRet |= MK_SHIFT;
	}
	if	(GetKeyState (VK_CONTROL) < 0)
	{
		lRet |= MK_CONTROL;
	}
	if	(GetKeyState (VK_MENU) < 0)
	{
		lRet |= MK_ALT;
	}
	return lRet;
}

void CAgentPopupWnd::NotifyClick (short pButton, const CPoint & pPoint)
{
	if	(PreNotify ())
	{
		try
		{
			int			lNotifyNdx;
			IDaNotify *	lNotify;

			for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
			{
				if	(lNotify->_GetNotifyClient (mCharID) == mCharID)
				{
					lNotify->Click (mCharID, pButton|NotifyKeyState(), pPoint.x, pPoint.y);
				}
			}
		}
		catch AnyExceptionDebug
		PostNotify ();
	}
}

void CAgentPopupWnd::NotifyDblClick (short pButton, const CPoint & pPoint)
{
	if	(PreNotify ())
	{
		try
		{
			int			lNotifyNdx;
			IDaNotify *	lNotify;

			for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
			{
				if	(lNotify->_GetNotifyClient (mCharID) == mCharID)
				{
					lNotify->DblClick (mCharID, pButton|NotifyKeyState(), pPoint.x, pPoint.y);
				}
			}
		}
		catch AnyExceptionDebug
		PostNotify ();
	}
}

/////////////////////////////////////////////////////////////////////////////

int CAgentPopupWnd::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	int lRet = CAgentWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
	SetActiveWindow ();
	return lRet;
}

void CAgentPopupWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CAgentWnd::OnActivate(nState, pWndOther, bMinimized);
	if	(nState)
	{
#ifdef	_DEBUG_ACTIVATE
		LogMessage (_DEBUG_ACTIVATE, _T("[%p(%d)] CAgentPopupWnd OnActivate [%d] Activate [%p] Last [%p]"), this, m_dwRef, mCharID, m_hWnd, mLastActive);
#endif
		SetLastActive (m_hWnd);
	}
}

/////////////////////////////////////////////////////////////////////////////

_MFC_NCHITTEST_RESULT CAgentPopupWnd::OnNcHitTest(CPoint point)
{
	_MFC_NCHITTEST_RESULT	lRet = CAgentWnd::OnNcHitTest(point);

	if	(lRet == HTCLIENT)
	{
		lRet = HTCAPTION;
	}
	return lRet;
}

void CAgentPopupWnd::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnNcLButtonDown [%d %d]"), point.x, point.y);
#endif
	mWasDragged = false;
	CAgentWnd::OnNcLButtonDown(nHitTest, point);

#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnNcLButtonDown [%d %d] WasDragged [%u]"), point.x, point.y, mWasDragged);
#endif
	if	(!mWasDragged)
	{
		NotifyClick (MK_LBUTTON, point);
	}
	mLastButtonMsg = WM_NCLBUTTONDOWN;
}

void CAgentPopupWnd::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnNcLButtonUp [%d %d]"), point.x, point.y);
#endif
	CAgentWnd::OnNcLButtonUp(nHitTest, point);
	mLastButtonMsg = WM_NCLBUTTONUP;
}

void CAgentPopupWnd::OnNcLButtonDblClk(UINT nHitTest, CPoint point)
{
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnNcLButtonDblClk [%d %d]"), point.x, point.y);
#endif
	CAgentWnd::OnNcLButtonDblClk(nHitTest, point);
	NotifyDblClick (MK_LBUTTON, point);
	mLastButtonMsg = WM_NCLBUTTONDBLCLK;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentPopupWnd::OnNcRButtonDown(UINT nHitTest, CPoint point)
{
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnNcRButtonDown [%d %d]"), point.x, point.y);
#endif
	CAgentWnd::OnNcRButtonDown(nHitTest, point);
	mLastButtonMsg = WM_NCRBUTTONDOWN;
}

void CAgentPopupWnd::OnNcRButtonUp(UINT nHitTest, CPoint point)
{
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnNcRButtonUp [%d %d]"), point.x, point.y);
#endif
	CAgentWnd::OnNcRButtonUp(nHitTest, point);
	if	(mLastButtonMsg != WM_NCRBUTTONDBLCLK)
	{
		NotifyClick (MK_RBUTTON, point);
		PostMessage (WM_CONTEXTMENU, (WPARAM)m_hWnd, MAKELPARAM(point.x, point.y));
	}
	mLastButtonMsg = WM_NCRBUTTONUP;
}

void CAgentPopupWnd::OnNcRButtonDblClk(UINT nHitTest, CPoint point)
{
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnNcRButtonDblClk [%d %d]"), point.x, point.y);
#endif
	CAgentWnd::OnNcRButtonDblClk(nHitTest, point);
	NotifyDblClick (MK_RBUTTON, point);
	mLastButtonMsg = WM_NCRBUTTONDBLCLK;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentPopupWnd::OnNcMButtonDown(UINT nHitTest, CPoint point)
{
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnNcMButtonDown [%d %d]"), point.x, point.y);
#endif
	CAgentWnd::OnNcMButtonDown(nHitTest, point);
	mLastButtonMsg = WM_NCMBUTTONDOWN;
}

void CAgentPopupWnd::OnNcMButtonUp(UINT nHitTest, CPoint point)
{
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnNcMButtonUp [%d %d]"), point.x, point.y);
#endif
	CAgentWnd::OnNcMButtonUp(nHitTest, point);
	if	(mLastButtonMsg != WM_NCMBUTTONDBLCLK)
	{
		NotifyClick (MK_MBUTTON, point);
	}
	mLastButtonMsg = WM_NCMBUTTONUP;
}

void CAgentPopupWnd::OnNcMButtonDblClk(UINT nHitTest, CPoint point)
{
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnNcMButtonDblClk [%d %d]"), point.x, point.y);
#endif
	CAgentWnd::OnNcMButtonDblClk(nHitTest, point);
	NotifyDblClick (MK_MBUTTON, point);
	mLastButtonMsg = WM_NCMBUTTONDBLCLK;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentPopupWnd::OnEnterSizeMove(WPARAM wParam, LPARAM lParam)
{
	CRect	lWinRect;

	GetWindowRect (&lWinRect);
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnEnterSizeMove [%d %d]"), lWinRect.left, lWinRect.top);
#endif
	mSizeMoveStart = new CPoint (lWinRect.left, lWinRect.top);
	mIsDragging = false;

	return Default();
}

LRESULT CAgentPopupWnd::OnExitSizeMove(WPARAM wParam, LPARAM lParam)
{
	CRect	lWinRect;

	if	(IsWindow (mBalloonWnd->GetSafeHwnd()))
	{
		mBalloonWnd->MoveBalloon ();
	}
	if	(IsWindow (mListeningWnd->GetSafeHwnd()))
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
			int			lNotifyNdx;
			IDaNotify *	lNotify;
			long		lNotifyCharID;

			if	(mIsDragging)
			{
				for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
				{
					if	(lNotify->_GetNotifyClient (mCharID) == mCharID)
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
					lNotifyCharID = lNotify->_GetNotifyClient (mCharID);
					lNotify->_PutMoveCause (lNotifyCharID, UserMoved);
					lNotify->Move (lNotifyCharID, lWinRect.left, lWinRect.top, UserMoved);
				}
			}
		}
		catch AnyExceptionDebug
		PostNotify ();
	}
	mSizeMoveStart = NULL;
	mWasDragged = mIsDragging;
	mIsDragging = false;

	return Default();
}

/////////////////////////////////////////////////////////////////////////////

void CAgentPopupWnd::OnMoving(UINT nSide, LPRECT lpRect)
{
#ifdef	_DEBUG_MOUSE
	LogMessage (_DEBUG_MOUSE, _T("OnMoving [%d %d]"), lpRect->left, lpRect->top);
#endif
	if	(
			(mSizeMoveStart)
		&&	(!mIsDragging)
		&&	(
				(lpRect->left != mSizeMoveStart->x)
			||	(lpRect->top != mSizeMoveStart->y)
			)
		)
	{
		mIsDragging = true;

		if	(PreNotify ())
		{
			try
			{
				int			lNotifyNdx;
				IDaNotify *	lNotify;

				for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
				{
					if	(lNotify->_GetNotifyClient (mCharID) == mCharID)
					{
						lNotify->DragStart (mCharID, NotifyKeyState(), mSizeMoveStart->x, mSizeMoveStart->y);
					}
				}
			}
			catch AnyExceptionDebug
			PostNotify ();
		}
	}
	CAgentWnd::OnMoving(nSide, lpRect);
}

void CAgentPopupWnd::OnMove(int x, int y)
{
	CAgentWnd::OnMove(x, y);

	if	(::GetCapture() != m_hWnd)
	{
		if	(IsWindow (mBalloonWnd->GetSafeHwnd()))
		{
			mBalloonWnd->MoveBalloon ();
		}
		if	(IsWindow (mListeningWnd->GetSafeHwnd()))
		{
			mListeningWnd->PositionTipWnd ();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

void CAgentPopupWnd::OnContextMenu(CWnd *pWnd, CPoint pos)
{
	if	(
			(pWnd->GetSafeHwnd () == m_hWnd)
		&&	(PreNotify ())
		)
	{
		try
		{
			int			lNotifyNdx;
			IDaNotify *	lNotify;

			for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
			{
				if	(
						(lNotify->_GetNotifyClient (mCharID) == mCharID)
					&&	(lNotify->_DoContextMenu (mCharID, m_hWnd, pos))
					)
				{
					break;
				}
			}
		}
		catch AnyExceptionDebug
		PostNotify ();
	}
}

LRESULT CAgentPopupWnd::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{
	LogMessage (LogNormal, _T("CAgentPopupWnd::OnDisplayChange"));
	return Default ();
}

LRESULT CAgentPopupWnd::OnInputLangChange(WPARAM wParam, LPARAM lParam)
{
	LogMessage (LogNormal, _T("CAgentPopupWnd::OnInputLangChange [%4.4hX]"), LOWORD(GetKeyboardLayout(GetCurrentThreadId())));
	return Default ();
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

bool CAgentPopupWnd::ShowNotifyIcon ()
{
	bool	lRet = false;

	if	(
			(IsWindow (m_hWnd))
		&&	(!mNotifyIcon.hWnd)
		)
	{
		CAgentFile *	lAgentFile;

		mNotifyIcon.uVersion = NOTIFYICON_VERSION;
		Shell_NotifyIcon (NIM_SETVERSION, &mNotifyIcon);

		mNotifyIcon.Clear();
		mNotifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_STATE;
		mNotifyIcon.hWnd = m_hWnd;
		mNotifyIcon.uCallbackMessage = mNotifyIconMsg;
		mNotifyIcon.dwState = NIS_SHAREDICON;
		mNotifyIcon.guidItem = __uuidof(CDaAgent);

		if	(!mNotifyIconName.IsEmpty ())
		{
			_tcsncpy (mNotifyIcon.szTip, mNotifyIconName, sizeof(mNotifyIcon.szTip)/sizeof(TCHAR));
			mNotifyIcon.uFlags |= NIF_TIP;
		}

		if	(
				(lAgentFile = GetAgentFile())
			&&	(mNotifyIcon.hIcon = lAgentFile->GetIcon())
			&&	(Shell_NotifyIcon (NIM_ADD, &mNotifyIcon))
			)
		{
			lRet = true;
		}
		else
		{
			mNotifyIcon.Clear();
		}
	}
	else
	{
		mNotifyIcon.Clear();
	}
	return lRet;
}

bool CAgentPopupWnd::HideNotifyIcon ()
{
	bool	lRet = false;

	if	(
			(mNotifyIcon.hWnd)
		&&	(Shell_NotifyIcon (NIM_DELETE, &mNotifyIcon))
		)
	{
		lRet = true;
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentPopupWnd::SetNotifyIconName (LANGID pLangID)
{
	bool				lRet = false;
	CAgentFile *		lAgentFile;
	CAgentFileName *	lAgentFileName = NULL;

	if	(lAgentFile = GetAgentFile ())
	{
		lAgentFileName = lAgentFile->FindName (pLangID);
		if	(lAgentFileName)
		{
			lRet = SetNotifyIconName (CString ((BSTR)lAgentFileName->mName), pLangID);
		}
	}
	return lRet;
}

bool CAgentPopupWnd::SetNotifyIconName (LPCTSTR pName, LANGID pLangID)
{
	bool	lRet = false;

	mNotifyIconName = pName;
#ifdef	_DEBUG
	mNotifyIconName.Format (_T("%s [%d]"), CString((LPCTSTR)mNotifyIconName), mCharID);
#endif

	if	(
			(IsWindow (m_hWnd))
		&&	(mNotifyIcon.hWnd)
		)
	{
		_tcsncpy (mNotifyIcon.szTip, mNotifyIconName, sizeof(mNotifyIcon.szTip)/sizeof(TCHAR));
		mNotifyIcon.uFlags = NIF_TIP;
		Shell_NotifyIcon (NIM_MODIFY, &mNotifyIcon);
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CAgentPopupWnd::OnNotifyIcon(WPARAM wParam, LPARAM lParam)
{
	if	(wParam == mNotifyIcon.uID)
	{
		CPoint	lCursorPos;

		GetCursorPos (&lCursorPos);

		switch (lParam)
		{
			case NIN_KEYSELECT:
			case NIN_SELECT:
			{
				PostMessage (WM_CONTEXTMENU, (WPARAM)m_hWnd, MAKELPARAM(lCursorPos.x, lCursorPos.y));
			}	break;
			case WM_LBUTTONUP:
			{
				if	(mLastButtonMsg != WM_LBUTTONDBLCLK)
				{
					NotifyClick (MK_LBUTTON|0x1000, lCursorPos);
				}
				mLastButtonMsg = WM_LBUTTONUP;
			}	break;
			case WM_LBUTTONDBLCLK:
			{
				NotifyDblClick (MK_LBUTTON|0x1000, lCursorPos);
				OnIconDblClick (lCursorPos);
				mLastButtonMsg = WM_LBUTTONDBLCLK;
			}	break;
			case WM_RBUTTONUP:
			{
				if	(mLastButtonMsg != WM_RBUTTONDBLCLK)
				{
					NotifyClick (MK_RBUTTON|0x1000, lCursorPos);
					PostMessage (WM_CONTEXTMENU, (WPARAM)m_hWnd, MAKELPARAM(lCursorPos.x, lCursorPos.y));
				}
				mLastButtonMsg = WM_RBUTTONUP;
			}	break;
			case WM_RBUTTONDBLCLK:
			{
				NotifyDblClick (MK_RBUTTON|0x1000, lCursorPos);
				mLastButtonMsg = WM_RBUTTONDBLCLK;
			}	break;
			case WM_MBUTTONUP:
			{
				if	(mLastButtonMsg != WM_MBUTTONDBLCLK)
				{
					NotifyClick (MK_MBUTTON|0x1000, lCursorPos);
				}
				mLastButtonMsg = WM_MBUTTONUP;
			}	break;
			case WM_MBUTTONDBLCLK:
			{
				NotifyDblClick (MK_MBUTTON|0x1000, lCursorPos);
				mLastButtonMsg = WM_MBUTTONDBLCLK;
			}	break;
		}
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
			int			lNotifyNdx;
			IDaNotify *	lNotify;

			for	(lNotifyNdx = 0; lNotify = mNotify (lNotifyNdx); lNotifyNdx++)
			{
				if	(
						(lNotify->_GetNotifyClient (mCharID) == mCharID)
					&&	(lNotify->_DoDefaultCommand (mCharID, m_hWnd, pPoint))
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

LRESULT CAgentPopupWnd::OnTaskbarCreated(WPARAM wParam, LPARAM lParam)
{
//
//	In case the desktop is recreated for some reason we want to recreate the notification icon.
//
	if	(mNotifyIcon.hWnd)
	{
		try
		{
			HideNotifyIcon ();
			ShowNotifyIcon ();
		}
		catch AnyExceptionDebug
	}
	return Default();
}