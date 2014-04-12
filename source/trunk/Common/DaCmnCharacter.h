/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software:
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
#pragma once
#include "AgentFile.h"
#include "AgentFileCache.h"
#include "AgentNotifyIcon.h"
#include "SapiVoiceCache.h"
#include "SapiInputCache.h"
#include "EventNotify.h"
#include "ListeningState.h"

/////////////////////////////////////////////////////////////////////////////

class CDaCmnCharacter :
	public CAgentFileClient,
	public CSapiVoiceClient,
	public CSapiInputClient,
	public CEventNotifyClient<CDaCmnCharacter>,
	protected _IEventReflect
{
public:
	CDaCmnCharacter ();
	virtual ~CDaCmnCharacter ();

// Attributes
public:
	long GetCharID () const {return mCharID;}
	LANGID GetLangID () const {return mLangID;}
	CAgentFile* GetFile () const {return mFile;}
	BSTR GetName () const;

	bool IsDefault () const {return mIsDefault;}
	bool IsVisible (bool pOrIsShowing = true) const;
	bool IsShowing () const;
	bool IsHiding () const;
	bool IsInputActive () const;
	bool IsClientActive () const;
	bool IsSpeaking () const;
	bool IsListening () const;
	bool IsHearing () const;
	bool IsIdleEnabled () const;
	bool IsSoundEnabled (bool pIgnoreGlobalConfig = false) const;
	bool IsAutoPopupMenu () const;
	bool IsIconShown () const;
	bool IsIconVisible () const;

	virtual DWORD GetStyle () const;
	long GetActiveClient () const;
	virtual short GetActiveState () const;
	virtual bool IsValid (const CAgentFile* pFile) const;

// Operations
public:
	void Initialize (long pCharID, CEventNotify* pNotify, _IListeningAnchor* pListeningAnchor);
	void Terminate (bool pFinal, bool pAbandonned = false);
	void Unrealize (bool pForce);

	long Show (bool pFast, bool pImmediate = false);
	long Hide (bool pFast, bool pImmediate = false);
	bool SetActiveClient (bool pActive, bool pInputActive);
	virtual HRESULT SetActiveState (short pActiveState);
	INT_PTR GetClientCount (int pSkipCharID = 0) const;

	HRESULT OpenFile (CAgentFile* pFile, bool pIsDefault = false);
	HRESULT Realize (class CAgentCharacterWnd* pCharacterWnd, DWORD pInitialStyle);
	HRESULT RealizePopup (CWindow* pParentWnd, DWORD pInitialStyle, DWORD pExStyle = 0);
	HRESULT SetLangID (LANGID pLangID);
	virtual HRESULT SetStyle (DWORD pRemoveStyle, DWORD pAddStyle);
	HRESULT StartListening (bool pManual);
	HRESULT StopListening (bool pManual, long pCause);

	static HRESULT GetLoadPath (VARIANT pProvider, CAtlString& pFilePath, LPCTSTR pSearchPath = NULL, bool* pIsDefault = NULL);
	static HRESULT GetAgentFile (VARIANT pProvider, tPtr <CAgentFile>& pAgentFile, LPCTSTR pSearchPath = NULL);
	static HRESULT GetAgentFile (LPCTSTR pFilePath, tPtr <CAgentFile>& pAgentFile);

// Overrides
public:
	virtual void _OnCharacterNameChanged (long pCharID);
	virtual void _OnCharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID);
	virtual bool _OnDownloadComplete (CFileDownload* pDownload);
	virtual class CFileDownload* _FindSoundDownload (LPCTSTR pSoundUrl);
	virtual bool _OnContextMenu (long pCharID, HWND pOwner, const CPoint& pPosition);
	virtual bool _OnDefaultCommand (long pCharID, HWND pOwner, const CPoint& pPosition);
	virtual void _OnOptionsChanged ();
	virtual void _OnDefaultCharacterChanged (REFGUID pCharGuid, LPCTSTR pFilePath);

// Interfaces
public:
	// ICDaCharacter2
	HRESULT SetPosition (long Left, long Top);
	HRESULT GetPosition (long *Left, long *Top);
	HRESULT SetSize (long Width, long Height);
	HRESULT GetSize (long *Width, long *Height);
	HRESULT GetVersion(short *MajorVersion, short *MinorVersion);
	HRESULT GetOriginalSize (long *Width, long *Height);
	HRESULT Prepare (long Type, BSTR Name, long Queue, long *RequestID);
	HRESULT Play (BSTR Animation, long *RequestID);
	HRESULT Stop (long RequestID);
	HRESULT StopAll (long Types);
	HRESULT Wait (long WaitForRequestID, long *RequestID);
	HRESULT Interrupt (long InterruptRequestID, long *RequestID);
	HRESULT Show (long Fast, long *RequestID);
	HRESULT Hide (long Fast, long *RequestID);
	HRESULT Speak (BSTR Text, class CAgentTextObject* pTextObject, BSTR Url, long *RequestID);
	HRESULT MoveTo (short X, short Y, long Speed, long *RequestID);
	HRESULT GestureAt (short X, short Y, long *RequestID);
	HRESULT Listen (long Listen);
	HRESULT Think (BSTR Text, class CAgentTextObject* pTextObject, long *RequestID);
	HRESULT ShowPopupMenu (short X, short Y);
	HRESULT GetTTSSpeed (long *Speed);
	HRESULT GetTTSPitch (short *Pitch);

	HRESULT get_Style (long *Style);
	HRESULT put_Style (long Style);
	HRESULT get_HasIcon (VARIANT_BOOL *HasIcon);
	HRESULT GenerateIcon (long ClipLeft = 0, long ClipTop = 0, long ClipWidth = -1, long ClipHeight = -1);
	HRESULT get_IconVisible (VARIANT_BOOL *IconVisible);
	HRESULT get_IconIdentity (BSTR *IconIdentity);
	HRESULT put_IconIdentity (BSTR IconIdentity);
	HRESULT GetIconIdentity (GUID *IconIdentity);
	HRESULT SetIconIdentity (const GUID *IconIdentity);
	HRESULT get_IconTip (BSTR *IconTip);
	HRESULT put_IconTip (BSTR IconTip);

	HRESULT get_CharacterID (long *CharacterID);
	HRESULT get_UniqueID (BSTR *CharGUID);
	HRESULT get_Name (BSTR *Name);
	HRESULT put_Name (BSTR Name);
	HRESULT get_Description (BSTR *Description);
	HRESULT put_Description (BSTR Description);
	HRESULT get_ExtraData (BSTR *ExtraData);
	HRESULT get_FileName (BSTR *FileName);
	HRESULT get_FilePath (BSTR *FilePath);
	HRESULT get_Animations (SAFEARRAY**Animations);
	HRESULT get_States (SAFEARRAY**States);
	HRESULT get_LanguageID (long *LanguageID);
	HRESULT put_LanguageID (long LanguageID);
	HRESULT get_TTSModeID (BSTR *TTSModeID);
	HRESULT put_TTSModeID (BSTR TTSModeID);
	HRESULT get_SRModeID (BSTR *SRModeID);
	HRESULT put_SRModeID (BSTR SRModeID);
	HRESULT get_Left (short *Left);
	HRESULT put_Left (short Left);
	HRESULT get_Top (short *Top);
	HRESULT put_Top (short Top);
	HRESULT get_Width (short *Width);
	HRESULT put_Width (short Width);
	HRESULT get_Height (short *Height);
	HRESULT put_Height (short Height);
	HRESULT get_OriginalWidth (short *OriginalWidth);
	HRESULT get_OriginalHeight (short *OriginalHeight);
	HRESULT get_Visible (VARIANT_BOOL *Visible);
	HRESULT get_ActiveState (ActiveStateType *ActiveState);
	HRESULT put_ActiveState (ActiveStateType ActiveState);
	HRESULT get_IdleState (VARIANT_BOOL *IdleState);
	HRESULT put_IdleState (VARIANT_BOOL IdleState);
	HRESULT get_OtherClientCount (long *OtherClientCount);
	HRESULT get_MoveCause (MoveCauseType *MoveCause);
	HRESULT get_VisibilityCause (VisibilityCauseType *VisibilityCause);
	HRESULT get_ListeningStatus (ListeningStatusType *ListeningStatus);

// Implementation
public:
	virtual class CDaCmnCommands* GetCommands (bool pCreateObject) = 0;
	virtual class CDaCmnBalloon* GetBalloon (bool pCreateObject) = 0;
	virtual class CAgentCharacterWnd* GetCharacterWnd (bool pMustExist = true) const;
	virtual class CAgentPopupWnd* GetPopupWnd (bool pMustExist = true) const;
	virtual class CAgentBalloonWnd* GetBalloonWnd (bool pCreateObject);
	virtual class CAgentListeningWnd* GetListeningWnd (bool pCreateObject);

	virtual bool DoMenuCommand (USHORT pCommandId);
	virtual bool DoMenuActivate ();
	virtual bool NotifyVoiceCommand (USHORT pCommandId, interface ISpRecoResult* pRecoResult, bool pGlobalCommand);
	HRESULT StopAll (long pStopTypes, HRESULT pReqStatus);

	class CSapiVoice* GetSapiVoice (bool pCreateObject, LPCTSTR pVoiceName = NULL);
	void ReleaseSapiVoice (bool pAbandonned = false);
	class CSapi5Input* GetSapiInput (bool pCreateObject, LPCTSTR pEngineName = NULL);
	void ReleaseSapiInput (bool pAbandonned = false);

	bool ShowListeningState (bool pShow);
	bool ShowHearingState (bool pShow);
	bool ShowIcon (bool pShow);

protected:
	void PropagateLangID ();
	HRESULT DoPrepare (long pType, LPCTSTR pName, bool pQueue, long& pReqID);
	bool DoContextMenu (HWND pOwner, const CPoint& pPosition);
	bool DoDefaultCommand (HWND pOwner, const CPoint& pPosition);

	LPVOID FindOtherRequest (long pReqID, CDaCmnCharacter*& pOtherCharacter);
	void TransferListeningState (CDaCmnCharacter* pOtherCharacter);

public:
	_IListeningAnchor*							mListeningAnchor;
protected:
	long										mCharID;
	LANGID										mLangID;
	CAgentFile*									mFile;
	class CSapiVoice*							mSapiVoice;
	class CSapi5Input*							mSapiInput;
	tPtr <class CSapi5Voice>					mPrivateSapiVoice;
	tPtr <class CListeningState>				mListeningState;
	CAtlOwnPtrMap <long, class CQueuedPrepare>	mPrepares;
	bool										mIdleEnabled;
	bool										mSoundEnabled;
	bool										mAutoPopupMenu;
	CAgentIconData								mIconData;
private:
	bool										mIsDefault;
	class CAgentCharacterWnd*					mWnd;
	IUnknownPtr									mWndRefHolder;
};

/////////////////////////////////////////////////////////////////////////////
