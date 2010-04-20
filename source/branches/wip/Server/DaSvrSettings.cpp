/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent Server.

    The Double Agent Server is free software:
    you can redistribute it and/or modify it under the terms of the
    GNU Lesser Public License as published by the Free Software Foundation,
    either version 3 of the License, or (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser Public License for more details.

    You should have received a copy of the GNU Lesser Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "DaSvrSettings.h"
#include "DaGlobalConfig.h"
#include "DaSvrCharacter.h"
#include "MallocPtr.h"
#ifdef	_DEBUG
#include "Registry.h"
#include "GuidStr.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_INTERFACE	(GetProfileDebugInt(_T("DebugInterface_Settings"),LogVerbose,true)&0xFFFF|LogHighVolume)
#define	_LOG_INSTANCE		(GetProfileDebugInt(_T("LogInstance_Settings"),LogDetails,true)&0xFFFF)
#define	_LOG_RESULTS		(GetProfileDebugInt(_T("LogResults"),LogNormal,true)&0xFFFF)
#endif

/////////////////////////////////////////////////////////////////////////////

DaSvrSettings::DaSvrSettings ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSettings::DaSvrSettings (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
}

DaSvrSettings::~DaSvrSettings ()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSettings::~DaSvrSettings (%d)"), this, m_dwRef, _AtlModule.GetLockCount());
	}
#endif
	try
	{
		_AtlModule.OnDeleteSvrSettings (this);
	}
	catch AnyExceptionSilent

	Terminate (true);
}

/////////////////////////////////////////////////////////////////////////////

DaSvrSettings * DaSvrSettings::CreateInstance (LPCTSTR pClientMutexName)
{
	CComObject<DaSvrSettings> *	lInstance = NULL;

	if	(SUCCEEDED (LogComErr (LogIfActive, CComObject<DaSvrSettings>::CreateInstance (&lInstance))))
	{
		lInstance->ManageObjectLifetime (lInstance, pClientMutexName);
	}
	return lInstance;
}

void DaSvrSettings::Terminate (bool pFinal, bool pAbandonned)
{
	if	(this)
	{
		if	(
				(pFinal)
			&&	(m_dwRef > 0)
			)
		{
			if	(!pAbandonned)
			{
				try
				{
					CoDisconnectObject (GetUnknown(), 0);
				}
				catch AnyExceptionDebug
			}
			m_dwRef = 0;
		}

		if	(pFinal)
		{
			UnmanageObjectLifetime (this);
		}
	}
}

void DaSvrSettings::FinalRelease()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSettings::FinalRelease"), this, m_dwRef);
	}
#endif
	Terminate (false);
}

void DaSvrSettings::OnClientEnded()
{
#ifdef	_LOG_INSTANCE
	if	(LogIsActive())
	{
		LogMessage (_LOG_INSTANCE, _T("[%p(%d)] DaSvrSettings::OnClientEnded"), this, m_dwRef);
	}
#endif
	Terminate (true, true);
	try
	{
		delete this;
	}
	catch AnyExceptionDebug
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

AudioStatusType DaSvrSettings::GetAudioStatus ()
{
	AudioStatusType	lStatus = AudioStatus_Available;

	try
	{
		INT_PTR			lFileNdx;
		CAgentFile *	lFile;

		for	(lFileNdx = 0; lFile = _AtlModule.GetCachedFile (lFileNdx); lFileNdx++)
		{
			CAtlPtrTypeArray <CAgentFileClient>	lFileClients;
			INT_PTR								lClientNdx;
			DaSvrCharacter *					lCharacter;

			if	(_AtlModule.GetFileClients (lFile, lFileClients))
			{
				for	(lClientNdx = lFileClients.GetUpperBound(); lClientNdx >= 0; lClientNdx--)
				{
					if	(lCharacter = dynamic_cast <DaSvrCharacter *> (lFileClients [lClientNdx]))
					{
						if	(lCharacter->IsSpeaking ())
						{
							lStatus = AudioStatus_CharacterSpeaking;
						}
						else
						if	(lCharacter->IsHearing ())
						{
							lStatus = AudioStatus_UserSpeaking;
						}
						else
						if	(lCharacter->IsListening ())
						{
							lStatus = AudioStatus_CharacterListening;
						}
					}
				}
				if	(lStatus != AudioStatus_Available)
				{
					break;
				}
			}
			if	(lStatus != AudioStatus_Available)
			{
				break;
			}
		}

		if	(
				(lStatus == AudioStatus_Available)
			&&	(!CDaSettingsConfig().LoadConfig().mTtsEnabled)
			)
		{
			lStatus = AudioStatus_Disabled;
		}
	}
	catch AnyExceptionDebug

	return lStatus;
}

/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP DaSvrSettings::InterfaceSupportsErrorInfo(REFIID riid)
{
	if	(InlineIsEqualGUID (__uuidof(IDaSvrSettings), riid))
	{
		return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrSettings::get_SoundEffectsEnabled (VARIANT_BOOL *SoundEffectsEnabled)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSettings::get_SoundEffectsEnabled"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaSettingsConfig().LoadConfig().mEffectsEnabled ? S_OK : S_FALSE;

	if	(SoundEffectsEnabled)
	{
		(*SoundEffectsEnabled) = (lResult == S_OK) ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrSettings));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSettings::get_SoundEffectsEnabled"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrSettings::get_BalloonEnabled (VARIANT_BOOL *BalloonEnabled)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSettings::get_BalloonEnabled"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaBalloonConfig().LoadConfig().mEnabled ? S_OK : S_FALSE;

	if	(BalloonEnabled)
	{
		(*BalloonEnabled) = (lResult == S_OK) ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrSettings));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSettings::get_BalloonEnabled"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSettings::get_BalloonFont (IFontDisp **BalloonFont)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSettings::get_BalloonFont"), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(!BalloonFont)
	{
		lResult = E_POINTER;
	}
	else
	{
		CDaBalloonConfig		lBalloonConfig;
		tSS <FONTDESC, UINT>	lFontDesc;
		tMallocPtr <OLECHAR>	lFontName;
		
		(*BalloonFont) = NULL;
		
		if	(lBalloonConfig.LoadConfig ().mFont)
		{
			CMemDCHandle	lDC;
			CPoint			lFontSize (0, lBalloonConfig.mFont->lfHeight);
			CPoint			lOrigin (0, 0);

			lDC.Attach (CreateCompatibleDC (NULL));
			LPtoDP (lDC, &lFontSize, 1);
			LPtoDP (lDC, &lOrigin, 1);
			lFontSize.y = abs(lOrigin.y - lFontSize.y);
			lFontDesc.cySize.int64 = MulDiv (lFontSize.y, 720, GetDeviceCaps (lDC, LOGPIXELSY)) * 1000;
	
			lFontDesc.lpstrName = lFontName = AtlAllocTaskOleString (lBalloonConfig.mFont->lfFaceName);
			lFontDesc.sWeight = (short)lBalloonConfig.mFont->lfWeight;
			lFontDesc.sCharset = lBalloonConfig.mFont->lfCharSet;
			lFontDesc.fItalic = lBalloonConfig.mFont->lfItalic;
			lFontDesc.fUnderline = lBalloonConfig.mFont->lfUnderline;
			lFontDesc.fStrikethrough = lBalloonConfig.mFont->lfStrikeOut;
			
			lResult = OleCreateFontIndirect (&lFontDesc, __uuidof(IFontDisp), (void**)BalloonFont);
		}		
	}

	PutServerError (lResult, __uuidof(IDaSvrSettings));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSettings::get_BalloonFont"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrSettings::get_TTSEnabled (VARIANT_BOOL *TTSEnabled)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSettings::get_TTSEnabled"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaSettingsConfig().LoadConfig().mTtsEnabled ? S_OK : S_FALSE;

	if	(TTSEnabled)
	{
		(*TTSEnabled) = (lResult == S_OK) ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrSettings));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSettings::get_TTSEnabled"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSettings::get_TTSSpeed (short *TTSSpeed)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSettings::get_TTSSpeed"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!TTSSpeed)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*TTSSpeed) = (short)CDaSettingsConfig().LoadConfig().mTtsSpeed;
	}

	PutServerError (lResult, __uuidof(IDaSvrSettings));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSettings::get_TTSSpeed"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrSettings::get_SREnabled (VARIANT_BOOL *SREnabled)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechInput::get_SREnabled"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaSettingsConfig().LoadConfig().mSrEnabled ? S_OK : S_FALSE;

	if	(SREnabled)
	{
		(*SREnabled) = (lResult == S_OK) ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrSettings));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechInput::get_SREnabled"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSettings::get_SRHotKey (BSTR *SRHotKey)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechInput::get_SRHotKey"), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(SRHotKey)
	{
		CDaSettingsConfig	lSettingsConfig;
		CString				lKeyName;
		CString				lModName;
		UINT				lHotKeyCode;
		long				lHotKeyScan;

		lSettingsConfig.LoadConfig ();
		if	(
				(lHotKeyCode = LOBYTE (lSettingsConfig.mSrHotKey))
			&&	(lHotKeyScan = MapVirtualKey (lHotKeyCode, MAPVK_VK_TO_VSC))
			&&	(GetKeyNameText (lHotKeyScan<<16, lKeyName.GetBuffer(MAX_PATH), MAX_PATH) > 0)
			)
		{
			lKeyName.ReleaseBuffer ();

			if	(
					(HIBYTE (lSettingsConfig.mSrHotKey) & HOTKEYF_ALT)
				&&	(lHotKeyScan = MapVirtualKey (VK_MENU, MAPVK_VK_TO_VSC))
				&&	(GetKeyNameText (lHotKeyScan<<16, lModName.GetBuffer(MAX_PATH), MAX_PATH) > 0)
				)
			{
				lModName.ReleaseBuffer ();
				lKeyName = lModName + _T(" + ") + lKeyName;
			}
			if	(
					(HIBYTE (lSettingsConfig.mSrHotKey) & HOTKEYF_SHIFT)
				&&	(lHotKeyScan = MapVirtualKey (VK_SHIFT, MAPVK_VK_TO_VSC))
				&&	(GetKeyNameText (lHotKeyScan<<16, lModName.GetBuffer(MAX_PATH), MAX_PATH) > 0)
				)
			{
				lModName.ReleaseBuffer ();
				lKeyName = lModName + _T(" + ") + lKeyName;
			}
			if	(
					(HIBYTE (lSettingsConfig.mSrHotKey) & HOTKEYF_CONTROL)
				&&	(lHotKeyScan = MapVirtualKey (VK_CONTROL, MAPVK_VK_TO_VSC))
				&&	(GetKeyNameText (lHotKeyScan<<16, lModName.GetBuffer(MAX_PATH), MAX_PATH) > 0)
				)
			{
				lModName.ReleaseBuffer ();
				lKeyName = lModName + _T(" + ") + lKeyName;
			}
		}
		else
		{
			lKeyName.Empty ();
		}
		(*SRHotKey) = lKeyName.AllocSysString();
	}

	PutServerError (lResult, __uuidof(IDaSvrSettings));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechInput::get_SRHotKey"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSettings::get_SRHotKeyTime (short *SRHotKeyTime)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSettings::get_SRHotKeyTime"), this, m_dwRef);
#endif
	HRESULT	lResult = S_OK;

	if	(!SRHotKeyTime)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*SRHotKeyTime) = (short)(CDaSettingsConfig().LoadConfig().mSrHotKeyDelay / 1000);
	}

	PutServerError (lResult, __uuidof(IDaSvrSettings));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSettings::get_SRHotKeyTime"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSettings::get_SRListeningTip (VARIANT_BOOL *SRListeningTip)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechInput::get_SRListeningTip"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaSettingsConfig().mSrListeningTip ? S_OK : S_FALSE;

	if	(SRListeningTip)
	{
		(*SRListeningTip) = (lResult == S_OK) ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechInput::get_SRListeningTip"), this, m_dwRef);
	}
#endif
	return lResult;
}

HRESULT STDMETHODCALLTYPE DaSvrSettings::get_SRListeningPrompt (VARIANT_BOOL *SRListeningPrompt)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSpeechInput::get_SRListeningPrompt"), this, m_dwRef);
#endif
	HRESULT	lResult = CDaSettingsConfig().mSrListeningPrompt ? S_OK : S_FALSE;

	if	(SRListeningPrompt)
	{
		(*SRListeningPrompt) = (lResult == S_OK) ? VARIANT_TRUE : VARIANT_FALSE;
	}

	PutServerError (lResult, __uuidof(IDaSvrSpeechInput));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSpeechInput::get_SRListeningPrompt"), this, m_dwRef);
	}
#endif
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DaSvrSettings::get_AudioStatus (AudioStatusType *AudioStatus)
{
#ifdef	_DEBUG_INTERFACE
	LogMessage (_DEBUG_INTERFACE, _T("[%p(%d)] DaSvrSettings::get_AudioStatus"), this, m_dwRef);
#endif
	HRESULT	lResult = S_FALSE;

	if	(!AudioStatus)
	{
		lResult = E_POINTER;
	}
	else
	{
		(*AudioStatus) = GetAudioStatus ();
	}

	PutServerError (lResult, __uuidof(IDaSvrSettings));
#ifdef	_LOG_RESULTS
	if	(LogIsActive (_LOG_RESULTS))
	{
		LogComErrAnon (_LOG_RESULTS, lResult, _T("[%p(%d)] DaSvrSettings::get_AudioStatus"), this, m_dwRef);
	}
#endif
	return lResult;
}
