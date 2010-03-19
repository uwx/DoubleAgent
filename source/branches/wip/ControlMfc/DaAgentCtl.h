/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent ActiveX Control.

    The Double Agent ActiveX Control is free software:
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
#ifndef DAAGENTCTL_H_INCLUDED_
#define DAAGENTCTL_H_INCLUDED_
#pragma once

#include <ObjSafe.h>
#include "OleObjectFactoryExEx.h"
#include "DaRequestObj.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E530-A208-11DE-ABF2-002421116FB2}")) CDaAgentCtl : public COleControl
{
	DECLARE_DYNCREATE(CDaAgentCtl)
	DECLARE_OLECREATE_EX(CDaAgentCtl)
	DECLARE_OLETYPELIB(CDaAgentCtl)
	DECLARE_OLECTLTYPE(CDaAgentCtl)
protected:
	CDaAgentCtl ();
public:
	~CDaAgentCtl ();

// Attributes
public:
	IDaServer2Ptr	mServer;
	IDispatchPtr	mCharacters;
	IDispatchPtr	mAudioOutput;
	IDispatchPtr	mSpeechInput;
	IDispatchPtr	mCommandsWindow;
	IDispatchPtr	mPropertySheet;
	IDispatchPtr	mCharacterFiles;
	bool			mRaiseRequestErrors;

// Operations
public:
	void Terminate (bool pFinal);

	IDaCtlRequest * PutRequest (DaRequestCategory pCategory, long pReqID, HRESULT pResult);
	void CompleteRequests (bool pIdleTime = false);
	void TerminateRequests (bool pFinal);

	HRESULT ConnectServer ();
	HRESULT DisconnectServer (bool pForce);
	void DisconnectNotify (bool pForce);

// Overrides
	//{{AFX_VIRTUAL(CDaAgentCtl)
	protected:
	virtual LPUNKNOWN GetInterfaceHook(const void* iid);
	virtual void OnFinalRelease();
	virtual void OnSetClientSite();
	virtual BOOL OnSetExtent(LPSIZEL lpSizeL);
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CDaAgentCtl)
	afx_msg LRESULT OnCompleteRequests (WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_DISPATCH(CDaAgentCtl)
	afx_msg LPDISPATCH DspGetCharacters();
	afx_msg void DspSetCharacters(LPDISPATCH newValue);
	afx_msg LPDISPATCH DspGetAudioOutput();
	afx_msg void DspSetAudioOutput(LPDISPATCH newValue);
	afx_msg LPDISPATCH DspGetSpeechInput();
	afx_msg void DspSetSpeechInput(LPDISPATCH newValue);
	afx_msg LPDISPATCH DspGetPropertySheet();
	afx_msg void DspSetPropertySheet(LPDISPATCH newValue);
	afx_msg LPDISPATCH DspGetCommandsWindow();
	afx_msg void DspSetCommandsWindow(LPDISPATCH newValue);
	afx_msg BOOL DspGetConnected();
	afx_msg void DspSetConnected(BOOL bNewValue);
	afx_msg BOOL DspGetSuspended();
	afx_msg void DspSetSuspended(BOOL bNewValue);
	afx_msg BOOL DspGetRaiseRequestErrors();
	afx_msg void DspSetRaiseRequestErrors(BOOL bNewValue);
	afx_msg void DspShowDefaultCharacterProperties(const VARIANT & x, const VARIANT & y);
	afx_msg LPDISPATCH DspGetCharacterFiles();
	afx_msg void DspSetCharacterFiles(LPDISPATCH CharacterFiles);
	afx_msg BOOL DspGetIconsShown();
	afx_msg void DspSetIconsShown(BOOL IconsShown);
	afx_msg LPDISPATCH DspGetSpeechEngines ();
	afx_msg void DspSetSpeechEngines (LPDISPATCH SpeechEngines);
	afx_msg LPDISPATCH DspFindSpeechEngines (VARIANT LanguageID, VARIANT Gender);
	afx_msg LPDISPATCH DspGetCharacterSpeechEngine (VARIANT LoadKey);
	afx_msg LPDISPATCH DspFindCharacterSpeechEngines (VARIANT LoadKey, VARIANT LanguageID);
	afx_msg LPDISPATCH DspGetRecognitionEngines ();
	afx_msg void DspSetRecognitionEngines (LPDISPATCH RecognitionEngines);
	afx_msg LPDISPATCH DspFindRecognitionEngines (VARIANT LanguageID);
	afx_msg LPDISPATCH DspGetCharacterRecognitionEngine (VARIANT LoadKey);
	afx_msg LPDISPATCH DspFindCharacterRecognitionEngines (VARIANT LoadKey, VARIANT LanguageID);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	//{{AFX_EVENT(CDaAgentCtl)
	void FireActivateInput(LPCTSTR CharacterID)
		{FireEvent(DISPID_AgentEvents_ActivateInput,EVENT_PARAM(VTS_BSTR), CharacterID);}
	void FireDeactivateInput(LPCTSTR CharacterID)
		{FireEvent(DISPID_AgentEvents_DeactivateInput,EVENT_PARAM(VTS_BSTR), CharacterID);}
	void FireClick(LPCTSTR CharacterID, short Button, short Shift, short x, short y)
		{FireEvent(DISPID_AgentEvents_Click,EVENT_PARAM(VTS_BSTR  VTS_I2  VTS_I2  VTS_I2  VTS_I2), CharacterID, Button, Shift, x, y);}
	void FireDblClick(LPCTSTR CharacterID, short Button, short Shift, short x, short y)
		{FireEvent(DISPID_AgentEvents_DblClick,EVENT_PARAM(VTS_BSTR  VTS_I2  VTS_I2  VTS_I2  VTS_I2), CharacterID, Button, Shift, x, y);}
	void FireDragStart(LPCTSTR CharacterID, short Button, short Shift, short x, short y)
		{FireEvent(DISPID_AgentEvents_DragStart,EVENT_PARAM(VTS_BSTR  VTS_I2  VTS_I2  VTS_I2  VTS_I2), CharacterID, Button, Shift, x, y);}
	void FireDragComplete(LPCTSTR CharacterID, short Button, short Shift, short x, short y)
		{FireEvent(DISPID_AgentEvents_DragComplete,EVENT_PARAM(VTS_BSTR  VTS_I2  VTS_I2  VTS_I2  VTS_I2), CharacterID, Button, Shift, x, y);}
	void FireShow(LPCTSTR CharacterID, short Cause)
		{FireEvent(DISPID_AgentEvents_Show,EVENT_PARAM(VTS_BSTR  VTS_I2), CharacterID, Cause);}
	void FireHide(LPCTSTR CharacterID, short Cause)
		{FireEvent(DISPID_AgentEvents_Hide,EVENT_PARAM(VTS_BSTR  VTS_I2), CharacterID, Cause);}
	void FireRequestStart(LPDISPATCH Request)
		{FireEvent(DISPID_AgentEvents_RequestStart,EVENT_PARAM(VTS_DISPATCH), Request);}
	void FireRequestComplete(LPDISPATCH Request)
		{FireEvent(DISPID_AgentEvents_RequestComplete,EVENT_PARAM(VTS_DISPATCH), Request);}
	void FireRestart()
		{FireEvent(DISPID_AgentEvents_Restart,EVENT_PARAM(VTS_NONE));}
	void FireShutdown()
		{FireEvent(DISPID_AgentEvents_Shutdown,EVENT_PARAM(VTS_NONE));}
	void FireBookmark(long BookmarkID)
		{FireEvent(DISPID_AgentEvents_Bookmark,EVENT_PARAM(VTS_I4), BookmarkID);}
	void FireCommand(LPDISPATCH UserInput)
		{FireEvent(DISPID_AgentEvents_Command,EVENT_PARAM(VTS_DISPATCH), UserInput);}
	void FireIdleStart(LPCTSTR CharacterID)
		{FireEvent(DISPID_AgentEvents_IdleStart,EVENT_PARAM(VTS_BSTR), CharacterID);}
	void FireIdleComplete(LPCTSTR CharacterID)
		{FireEvent(DISPID_AgentEvents_IdleComplete,EVENT_PARAM(VTS_BSTR), CharacterID);}
	void FireMove(LPCTSTR CharacterID, short x, short y, short Cause)
		{FireEvent(DISPID_AgentEvents_Move,EVENT_PARAM(VTS_BSTR  VTS_I2  VTS_I2  VTS_I2), CharacterID, x, y, Cause);}
	void FireSize(LPCTSTR CharacterID, short Width, short Height)
		{FireEvent(DISPID_AgentEvents_Size,EVENT_PARAM(VTS_BSTR  VTS_I2  VTS_I2), CharacterID, Width, Height);}
	void FireBalloonShow(LPCTSTR CharacterID)
		{FireEvent(DISPID_AgentEvents_BalloonShow,EVENT_PARAM(VTS_BSTR), CharacterID);}
	void FireBalloonHide(LPCTSTR CharacterID)
		{FireEvent(DISPID_AgentEvents_BalloonHide,EVENT_PARAM(VTS_BSTR), CharacterID);}
	void FireHelpComplete(LPCTSTR CharacterID, LPCTSTR Name, short Cause)
		{FireEvent(DISPID_AgentEvents_HelpComplete,EVENT_PARAM(VTS_BSTR  VTS_BSTR  VTS_I2), CharacterID, Name, Cause);}
	void FireListenStart(LPCTSTR CharacterID)
		{FireEvent(DISPID_AgentEvents_ListenStart,EVENT_PARAM(VTS_BSTR), CharacterID);}
	void FireListenComplete(LPCTSTR CharacterID, short Cause)
		{FireEvent(DISPID_AgentEvents_ListenComplete,EVENT_PARAM(VTS_BSTR  VTS_I2), CharacterID, Cause);}
	void FireDefaultCharacterChange(LPCTSTR GUID)
		{FireEvent(DISPID_AgentEvents_DefaultCharacterChange,EVENT_PARAM(VTS_BSTR), GUID);}
	void FireAgentPropertyChange()
		{FireEvent(DISPID_AgentEvents_AgentPropertyChange,EVENT_PARAM(VTS_NONE));}
	void FireActiveClientChange(LPCTSTR CharacterID, BOOL Active)
		{FireEvent(DISPID_AgentEvents_ActiveClientChange,EVENT_PARAM(VTS_BSTR  VTS_BOOL), CharacterID, Active);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

	BEGIN_INTERFACE_PART(AgentCtl, IDaControl2)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

		HRESULT STDMETHODCALLTYPE get_Characters (IDaCtlCharacters **Characters);
		HRESULT STDMETHODCALLTYPE get_AudioOutput (IDaCtlAudioOutput **AudioOutput);
		HRESULT STDMETHODCALLTYPE get_SpeechInput (IDaCtlSpeechInput **SpeechInput);
		HRESULT STDMETHODCALLTYPE get_PropertySheet (IDaCtlPropertySheet **PropSheet);
		HRESULT STDMETHODCALLTYPE get_CommandsWindow (IDaCtlCommandsWindow **CommandsWindow);
		HRESULT STDMETHODCALLTYPE get_Connected (VARIANT_BOOL *Connected);
		HRESULT STDMETHODCALLTYPE put_Connected (VARIANT_BOOL Connected);
		HRESULT STDMETHODCALLTYPE get_Suspended (VARIANT_BOOL *Suspended);
		HRESULT STDMETHODCALLTYPE ShowDefaultCharacterProperties (VARIANT x, VARIANT y);
		HRESULT STDMETHODCALLTYPE get_RaiseRequestErrors (VARIANT_BOOL *RaiseErrors);
		HRESULT STDMETHODCALLTYPE put_RaiseRequestErrors (VARIANT_BOOL RaiseErrors);

		HRESULT STDMETHODCALLTYPE get_CharacterFiles (IDaCtlCharacterFiles **CharacterFiles);
		HRESULT STDMETHODCALLTYPE get_IconsShown (VARIANT_BOOL *IconsShown);
		HRESULT STDMETHODCALLTYPE put_IconsShown (VARIANT_BOOL IconsShown);
		HRESULT STDMETHODCALLTYPE get_SpeechEngines (IDaCtlSpeechEngines **SpeechEngines);
		HRESULT STDMETHODCALLTYPE FindSpeechEngines (VARIANT LanguageID, VARIANT Gender, IDaCtlSpeechEngines **SpeechEngines);
		HRESULT STDMETHODCALLTYPE GetCharacterSpeechEngine (VARIANT LoadKey, IDaCtlSpeechEngine **SpeechEngine);
		HRESULT STDMETHODCALLTYPE FindCharacterSpeechEngines (VARIANT LoadKey, VARIANT LanguageID, IDaCtlSpeechEngines **SpeechEngines);
		HRESULT STDMETHODCALLTYPE get_RecognitionEngines (IDaCtlRecognitionEngines **RecognitionEngines);
		HRESULT STDMETHODCALLTYPE FindRecognitionEngines (VARIANT LanguageID, IDaCtlRecognitionEngines **RecognitionEngines);
		HRESULT STDMETHODCALLTYPE GetCharacterRecognitionEngine (VARIANT LoadKey, IDaCtlRecognitionEngine **RecognitionEngine);
		HRESULT STDMETHODCALLTYPE FindCharacterRecognitionEngines (VARIANT LoadKey, VARIANT LanguageID, IDaCtlRecognitionEngines **RecognitionEngines);
	END_INTERFACE_PART(AgentCtl)

	BEGIN_INTERFACE_PART(ObjectSafety, IObjectSafety)
		HRESULT STDMETHODCALLTYPE GetInterfaceSafetyOptions (REFIID riid, DWORD *pdwSupportedOptions, DWORD *pdwEnabledOptions);
		HRESULT STDMETHODCALLTYPE SetInterfaceSafetyOptions (REFIID riid, DWORD dwOptionSetMask, DWORD dwEnabledOptions);
	END_INTERFACE_PART(ObjectSafety)

	DECLARE_SUPPORTERRORINFO()
	DECLARE_INTERFACE_MAP()
	DECLARE_CONNECTION_MAP()

protected:
	class CServerNotifySink : public CCmdTarget
	{
	public:
		CServerNotifySink (CDaAgentCtl & pOwner);
		~CServerNotifySink ();

		BEGIN_INTERFACE_PART(NotifySink, IDaSvrNotifySink)
			HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
			HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
			HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
			HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

			HRESULT STDMETHODCALLTYPE Command (long dwCommandID, IUnknown *punkUserInput);
			HRESULT STDMETHODCALLTYPE ActivateInputState (long dwCharID, long bActivated);
			HRESULT STDMETHODCALLTYPE Restart (void);
			HRESULT STDMETHODCALLTYPE Shutdown (void);
			HRESULT STDMETHODCALLTYPE VisibleState (long dwCharID, long bVisible, long dwCause);
			HRESULT STDMETHODCALLTYPE Click (long dwCharID, short fwKeys, long x, long y);
			HRESULT STDMETHODCALLTYPE DblClick (long dwCharID, short fwKeys, long x, long y);
			HRESULT STDMETHODCALLTYPE DragStart (long dwCharID, short fwKeys, long x, long y);
			HRESULT STDMETHODCALLTYPE DragComplete (long dwCharID, short fwKeys, long x, long y);
			HRESULT STDMETHODCALLTYPE RequestStart (long dwRequestID);
			HRESULT STDMETHODCALLTYPE RequestComplete (long dwRequestID, long hrStatus);
			HRESULT STDMETHODCALLTYPE BookMark (long dwBookMarkID);
			HRESULT STDMETHODCALLTYPE Idle (long dwCharID, long bStart);
			HRESULT STDMETHODCALLTYPE Move (long dwCharID, long x, long y, long dwCause);
			HRESULT STDMETHODCALLTYPE Size (long dwCharID, long lWidth, long lHeight);
			HRESULT STDMETHODCALLTYPE BalloonVisibleState (long dwCharID, long bVisible);
			HRESULT STDMETHODCALLTYPE HelpComplete (long dwCharID, long dwCommandID, long dwCause);
			HRESULT STDMETHODCALLTYPE ListeningState (long dwCharID, long bListening, long dwCause);
			HRESULT STDMETHODCALLTYPE DefaultCharacterChange (BSTR bszGUID);
			HRESULT STDMETHODCALLTYPE AgentPropertyChange(void);
			HRESULT STDMETHODCALLTYPE ActiveClientChange (long dwCharID, long lStatus);
		END_INTERFACE_PART(NotifySink)

		DECLARE_INTERFACE_MAP()

	public:
		CDaAgentCtl &	mOwner;
		long			mServerNotifyId;
	};
	friend class CServerNotifySink;

protected:
	CString GetControlCharacterID (long pServerCharID);
	CString GetActiveCharacterID ();
	class CDaCharacterObj * GetActiveCharacter ();

public:
	void RequestCreated (CDaRequestObj * pRequest);
	void RequestDeleted (CDaRequestObj * pRequest);

protected:
	tPtr <CServerNotifySink>							mServerNotifySink;
	CMap <long, long, CDaRequestObj *, CDaRequestObj *>	mActiveRequests;
	CPtrTypeArray <CDaRequestObj>						mCompletedRequests;
private:
	bool												mFinalReleased;
	static UINT											mCompleteRequestsMsg;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DAAGENTCTL_H_INCLUDED_
