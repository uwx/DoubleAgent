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
#ifndef DAAGENTCHARACTER_H_INCLUDED_
#define DAAGENTCHARACTER_H_INCLUDED_
#pragma once

#include "AgentFile.h"
#include "AgentFileCache.h"
#include "DaInternalNotify.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E50D-A208-11DE-ABF2-002421116FB2}")) CDaAgentCharacter : public CCmdTarget, protected IDaInternalNotify
{
public:
	CDaAgentCharacter (long pCharID, CAgentFile * pFile, CAgentFileCache & pUsedFileCache, IDaNotify & pNotify);
	virtual ~CDaAgentCharacter ();
	void Terminate (bool pFinal, bool pAbandonned = false);
	DECLARE_DYNAMIC(CDaAgentCharacter)
	DECLARE_OLETYPELIB(CDaAgentCharacter)

// Attributes
public:
	long GetCharID () const {return mCharID;}
	LANGID GetLangID () const {return mLangID;}
	CAgentFile * GetFile () const {return mFile;}
	BSTR GetName () const;

	bool IsVisible (bool pOrIsShowing = true) const;
	bool IsShowing () const;
	bool IsHiding () const;
	bool IsInputActive () const;
	bool IsClientActive () const;
	bool IsSpeaking () const;
	bool IsListening () const;
	bool IsHearing () const;
	long GetActiveClient () const;

// Operations
public:
	long Show (bool pFast, bool pImmediate = false);
	long Hide (bool pFast, bool pImmediate = false);
	bool SetClientActive (bool pActive, bool pInputActive);
	int GetClientCount (int pSkipCharID = 0) const;

	HRESULT SetLangID (LANGID pLangID);
	HRESULT StartListening (bool pManual);
	HRESULT StopListening (bool pManual, long pCause);

	static HRESULT GetDefaultSpeechEngine (CAgentFile * pFile, IDaSvrSpeechEngine ** pSpeechEngine);
	static HRESULT FindSpeechEngines (CAgentFile * pFile, LANGID pLangId, short pGender, IDaSvrSpeechEngines ** pSpeechEngines);
	static HRESULT GetDefaultRecognitionEngine (CAgentFile * pFile, IDaSvrRecognitionEngine ** pRecognitionEngine);
	static HRESULT FindRecognitionEngines (CAgentFile * pFile, LANGID pLangId, IDaSvrRecognitionEngines ** pRecognitionEngines);

// Overrides
	//{{AFX_VIRTUAL(CDaAgentCharacter)
	public:
	virtual void OnFinalRelease();
	virtual void _OnCharacterNameChanged (long pCharID);
	virtual void _OnCharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID);
	virtual bool _OnDownloadComplete (CFileDownload * pDownload);
	virtual class CFileDownload * _FindSoundDownload (LPCTSTR pSoundUrl);
	virtual bool _OnContextMenu (long pCharID, HWND pOwner, const CPoint & pPosition);
	virtual bool _OnDefaultCommand (long pCharID, HWND pOwner, const CPoint & pPosition);
	virtual void _OnOptionsChanged ();
	virtual void _OnDefaultCharacterChanged ();
	protected:
	virtual LPUNKNOWN GetInterfaceHook(const void* iid);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_DISPATCH(CDaAgentCharacter)
	afx_msg void DspGetVisible(long * Visible);
	afx_msg void DspSetPosition(long Left, long Top);
	afx_msg void DspGetPosition(long * Left, long * Top);
	afx_msg void DspSetSize(long Width, long Height);
	afx_msg void DspGetSize(long * Width, long * Height);
	afx_msg void DspGetName(BSTR * Name);
	afx_msg void DspGetDescription(BSTR * Description);
	afx_msg void DspGetTTSSpeed(short * Speed);
	afx_msg void DspGetTTSPitch(short * Pitch);
	afx_msg void DspActivate(short State);
	afx_msg void DspSetIdleOn(long On);
	afx_msg void DspGetIdleOn(long * On);
	afx_msg void DspPrepare(long Type, LPCTSTR Name, long Queue, long * ReqID);
	afx_msg void DspPlay(LPCTSTR Animation, long * ReqID);
	afx_msg void DspStop(long ReqID);
	afx_msg void DspStopAll(long Types);
	afx_msg void DspWait(long WaitForReqID, long * ReqID);
	afx_msg void DspInterrupt(long InterruptReqID, long * ReqID);
	afx_msg void DspShow(long Fast, long * ReqID);
	afx_msg void DspHide(long Fast, long * ReqID);
	afx_msg void DspSpeak(LPCTSTR Text, LPCTSTR Url, long * ReqID);
	afx_msg void DspMoveTo(short x, short y, long Speed, long * ReqID);
	afx_msg void DspGestureAt(short x, short y, long * ReqID);
	afx_msg void DspGetMoveCause(long * Cause);
	afx_msg void DspGetVisibilityCause(long * Cause);
	afx_msg void DspHasOtherClients(long * NumOtherClients);
	afx_msg void DspSetSoundEffectsOn(long On);
	afx_msg void DspGetSoundEffectsOn(long * On);
	afx_msg void DspSetName(LPCTSTR Name);
	afx_msg void DspSetDescription(LPCTSTR Description);
	afx_msg void DspGetExtraData(BSTR * ExtraData);
	afx_msg void DspShowPopupMenu(short x, short y);
	afx_msg void DspSetAutoPopupMenu(long AutoPopupMenu);
	afx_msg void DspGetAutoPopupMenu(long * AutoPopupMenu);
	afx_msg void DspGetHelpFileName(BSTR * Name);
	afx_msg void DspSetHelpFileName(LPCTSTR Name);
	afx_msg void DspSetHelpModeOn(long HelpModeOn);
	afx_msg void DspGetHelpModeOn(long * HelpModeOn);
	afx_msg void DspSetHelpContextID(long ID);
	afx_msg void DspGetHelpContextID(long * ID);
	afx_msg void DspGetActive(short * State);
	afx_msg void DspListen(long Listen);
	afx_msg void DspSetLanguageID(long langid);
	afx_msg void DspGetLanguageID(long * langid);
	afx_msg void DspGetTTSModeID(BSTR * ModeID);
	afx_msg void DspSetTTSModeID(LPCTSTR ModeID);
	afx_msg void DspGetSRModeID(BSTR * ModeID);
	afx_msg void DspSetSRModeID(LPCTSTR ModeID);
	afx_msg void DspGetGUID(BSTR * ID);
	afx_msg void DspGetOriginalSize(long * Width, long * Height);
	afx_msg void DspThink(LPCTSTR Text, long * ReqID);
	afx_msg void DspGetVersion(short * Major, short * Minor);
	afx_msg void DspGetAnimationNames(LPUNKNOWN * Enum);
	afx_msg void DspGetSRStatus(long * Status);
	afx_msg LPDISPATCH DspGetSpeechEngine (BOOL GetDefault = FALSE);
	afx_msg LPDISPATCH DspFindSpeechEngines (long LanguageID = 0);
	afx_msg LPDISPATCH DspGetRecognitionEngine (BOOL GetDefault = FALSE);
	afx_msg LPDISPATCH DspFindRecognitionEngines (long LanguageID = 0);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_DISPATCH_IID()

	BEGIN_INTERFACE_PART(Character2, IDaSvrCharacter2)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

		HRESULT STDMETHODCALLTYPE GetVisible (long *pbVisible);
		HRESULT STDMETHODCALLTYPE SetPosition (long lLeft, long lTop);
		HRESULT STDMETHODCALLTYPE GetPosition (long *plLeft, long *plTop);
		HRESULT STDMETHODCALLTYPE SetSize (long lWidth, long lHeight);
		HRESULT STDMETHODCALLTYPE GetSize (long *plWidth, long *plHeight);
		HRESULT STDMETHODCALLTYPE GetName (BSTR *pbszName);
		HRESULT STDMETHODCALLTYPE GetDescription (BSTR *pbszDescription);
		HRESULT STDMETHODCALLTYPE GetTTSSpeed (long *pdwSpeed);
		HRESULT STDMETHODCALLTYPE GetTTSPitch (short *pwPitch);
		HRESULT STDMETHODCALLTYPE Activate (short sState);
		HRESULT STDMETHODCALLTYPE SetIdleOn (long bOn);
		HRESULT STDMETHODCALLTYPE GetIdleOn (long *pbOn);
		HRESULT STDMETHODCALLTYPE Prepare (long dwType, BSTR bszName, long bQueue, long *pdwReqID);
		HRESULT STDMETHODCALLTYPE Play (BSTR bszAnimation, long *pdwReqID);
		HRESULT STDMETHODCALLTYPE Stop (long dwReqID);
		HRESULT STDMETHODCALLTYPE StopAll (long lTypes);
		HRESULT STDMETHODCALLTYPE Wait (long dwReqID, long *pdwReqID);
		HRESULT STDMETHODCALLTYPE Interrupt (long dwReqID, long *pdwReqID);
		HRESULT STDMETHODCALLTYPE Show (long bFast, long *pdwReqID);
		HRESULT STDMETHODCALLTYPE Hide (long bFast, long *pdwReqID);
		HRESULT STDMETHODCALLTYPE Speak (BSTR bszText, BSTR bszUrl, long *pdwReqID);
		HRESULT STDMETHODCALLTYPE MoveTo (short x, short y, long lSpeed, long *pdwReqID);
		HRESULT STDMETHODCALLTYPE GestureAt (short x, short y, long *pdwReqID);
		HRESULT STDMETHODCALLTYPE GetMoveCause (long *pdwCause);
		HRESULT STDMETHODCALLTYPE GetVisibilityCause (long *pdwCause);
		HRESULT STDMETHODCALLTYPE HasOtherClients (long *plNumOtherClients);
		HRESULT STDMETHODCALLTYPE SetSoundEffectsOn (long bOn);
		HRESULT STDMETHODCALLTYPE GetSoundEffectsOn (long *pbOn);
		HRESULT STDMETHODCALLTYPE SetName (BSTR bszName);
		HRESULT STDMETHODCALLTYPE SetDescription (BSTR bszDescription);
		HRESULT STDMETHODCALLTYPE GetExtraData (BSTR *pbszExtraData);

		HRESULT STDMETHODCALLTYPE ShowPopupMenu (short x, short y);
		HRESULT STDMETHODCALLTYPE SetAutoPopupMenu (long bAutoPopupMenu);
		HRESULT STDMETHODCALLTYPE GetAutoPopupMenu (long *pbAutoPopupMenu);
		HRESULT STDMETHODCALLTYPE GetHelpFileName (BSTR *pbszName);
		HRESULT STDMETHODCALLTYPE SetHelpFileName (BSTR bszName);
		HRESULT STDMETHODCALLTYPE SetHelpModeOn (long bHelpModeOn);
		HRESULT STDMETHODCALLTYPE GetHelpModeOn (long *pbHelpModeOn);
		HRESULT STDMETHODCALLTYPE SetHelpContextID (long ulID);
		HRESULT STDMETHODCALLTYPE GetHelpContextID (long *pulID);
		HRESULT STDMETHODCALLTYPE GetActive (short *psState);
		HRESULT STDMETHODCALLTYPE Listen (long bListen);
		HRESULT STDMETHODCALLTYPE SetLanguageID (long langid);
		HRESULT STDMETHODCALLTYPE GetLanguageID (long *plangid);
		HRESULT STDMETHODCALLTYPE GetTTSModeID (BSTR *pbszModeID);
		HRESULT STDMETHODCALLTYPE SetTTSModeID (BSTR bszModeID);
		HRESULT STDMETHODCALLTYPE GetSRModeID (BSTR *pbszModeID);
		HRESULT STDMETHODCALLTYPE SetSRModeID (BSTR bszModeID);
		HRESULT STDMETHODCALLTYPE GetGUID (BSTR *pbszID);
		HRESULT STDMETHODCALLTYPE GetOriginalSize (long *plWidth, long *plHeight);
		HRESULT STDMETHODCALLTYPE Think (BSTR bszText, long *pdwReqID);
		HRESULT STDMETHODCALLTYPE GetVersion(short *psMajor, short *psMinor);
		HRESULT STDMETHODCALLTYPE GetAnimationNames (IUnknown **punkEnum);
		HRESULT STDMETHODCALLTYPE GetSRStatus (long *plStatus);

		HRESULT STDMETHODCALLTYPE GetSpeechEngine (boolean GetDefault, IDaSvrSpeechEngine **SpeechEngine);
		HRESULT STDMETHODCALLTYPE FindSpeechEngines (long LanguageID, IDaSvrSpeechEngines **SpeechEngines);
		HRESULT STDMETHODCALLTYPE GetRecognitionEngine (boolean GetDefault, IDaSvrRecognitionEngine **RecognitionEngine);
		HRESULT STDMETHODCALLTYPE FindRecognitionEngines (long LanguageID, IDaSvrRecognitionEngines **RecognitionEngines);
	END_INTERFACE_PART(Character2)

	BEGIN_INTERFACE_PART(StdMarshalInfo, IStdMarshalInfo)
		HRESULT STDMETHODCALLTYPE GetClassForHandler (DWORD dwDestContext, void *pvDestContext, CLSID *pClsid);
	END_INTERFACE_PART(StdMarshalInfo)

	DECLARE_SUPPORTERRORINFO()
	DECLARE_PROVIDECLASSINFO()
	DECLARE_INTERFACE_MAP()

public:
	class CDaAgentCommands * GetCommandsObj (bool pCreateObject);
	class CDaAgentBalloon * GetBalloonObj (bool pCreateObject);
	class CAgentPopupWnd * GetAgentWnd ();
	class CAgentBalloonWnd * GetBalloonWnd (bool pCreateObject);
	class CAgentListeningWnd * GetListeningWnd (bool pCreateObject);

	void OpenFile ();
	bool DoMenuCommand (USHORT pCommandId);
	HRESULT StopAll (long pStopTypes, HRESULT pReqStatus);

	class CSapiVoice * GetSapiVoice (bool pCreateObject, LPCTSTR pVoiceName = NULL);
	void ReleaseSapiVoice ();
	class CSapi5Input * GetSapiInput (bool pCreateObject, LPCTSTR pEngineName = NULL);
	void ReleaseSapiInput ();

	bool ShowListeningState (bool pShow);
	bool ShowHearingState (bool pShow);

public:
	bool PreNotify ();
	bool PostNotify ();
	UINT IsInNotify () const;

	IDaNotify &	mNotify;

protected:
	IDaSvrBalloon * GetBalloonInterface (bool pCreateObject);
	void PropagateLangID ();
	HRESULT DoPrepare (long pType, LPCTSTR pName, bool pQueue, long & pReqID);
	bool DoContextMenu (HWND pOwner, const CPoint & pPosition);
	bool DoDefaultCommand (HWND pOwner, const CPoint & pPosition);

	LPVOID FindOtherRequest (long pReqID, CDaAgentCharacter *& pOtherCharacter);
	void TransferListeningState (CDaAgentCharacter * pOtherCharacter);
	long GetListeningStatus ();

protected:
	long									mCharID;
	LANGID									mLangID;
	CAgentFile *							mFile;
	CAgentFileCache &						mUsedFileCache;
	class CAgentPopupWnd *					mWnd;
	IDispatchPtr							mWndRefHolder;
	LPDISPATCH								mAgentBalloon;
	LPDISPATCH								mAgentCommands;
	class CSapiVoice *						mSapiVoice;
	class CSapi5Input *						mSapiInput;
	tPtr <class CListeningState>			mListeningState;
	COwnPtrMap <long, class CQueuedPrepare>	mPrepares;
	bool									mIdleOn;
	bool									mAutoPopupMenu;
private:
	UINT									mInNotify;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DAAGENTCHARACTER_H_INCLUDED_
