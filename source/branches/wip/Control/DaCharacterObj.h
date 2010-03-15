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
#ifndef DACHARACTEROBJ_H_INCLUDED_
#define DACHARACTEROBJ_H_INCLUDED_
#pragma once

#include "OleObjectFactoryExEx.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E532-A208-11DE-ABF2-002421116FB2}")) CDaCharacterObj : public CCmdTarget
{
	DECLARE_DYNAMIC(CDaCharacterObj)
	DECLARE_OLECREATE_EX(CDaCharacterObj)
	DECLARE_OLETYPELIB(CDaCharacterObj)
public:
	CDaCharacterObj (CDaAgentCtl & pOwner);
	~CDaCharacterObj ();
	HRESULT Terminate (bool pFinal);

// Attributes
	IDaSvrCharacter2Ptr	mServerObject;
	long				mServerCharID;
	IDispatchPtr		mBalloon;
	IDispatchPtr		mCommands;
	IDispatchPtr		mAnimationNames;

	CDaAgentCtl * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

// Operations
	class CDaBalloonObj * GetBalloon (IDaCtlBalloonPtr & pInterface);
	class CDaCommandsObj * GetCommands (IDaCtlCommandsPtr & pInterface);
	class CDaAnimationNamesObj * GetAnimationNames (IDaCtlAnimationNamesPtr & pInterface);

// Overrides
	//{{AFX_VIRTUAL(CDaCharacterObj)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_DISPATCH(CDaCharacterObj)
	afx_msg BOOL DspGetVisible();
	afx_msg void DspSetVisible(BOOL bNewValue);
	afx_msg short DspGetLeft();
	afx_msg void DspSetLeft(short nNewValue);
	afx_msg short DspGetTop();
	afx_msg void DspSetTop(short nNewValue);
	afx_msg short DspGetHeight();
	afx_msg void DspSetHeight(short nNewValue);
	afx_msg short DspGetWidth();
	afx_msg void DspSetWidth(short nNewValue);
	afx_msg long DspGetSpeed();
	afx_msg void DspSetSpeed(long nNewValue);
	afx_msg long DspGetPitch();
	afx_msg void DspSetPitch(long nNewValue);
	afx_msg BOOL DspGetIdleOn();
	afx_msg void DspSetIdleOn(BOOL bNewValue);
	afx_msg LPDISPATCH DspGetBalloon();
	afx_msg void DspSetBalloon(LPDISPATCH newValue);
	afx_msg LPDISPATCH DspGetCommands();
	afx_msg void DspSetCommands(LPDISPATCH newValue);
	afx_msg BSTR DspGetName();
	afx_msg void DspSetName(LPCTSTR lpszNewValue);
	afx_msg BSTR DspGetDescription();
	afx_msg void DspSetDescription(LPCTSTR lpszNewValue);
	afx_msg short DspGetMoveCause();
	afx_msg void DspSetMoveCause(short nNewValue);
	afx_msg short DspGetVisibilityCause();
	afx_msg void DspSetVisibilityCause(short nNewValue);
	afx_msg BOOL DspGetHasOtherClients();
	afx_msg void DspSetHasOtherClients(BOOL bNewValue);
	afx_msg BOOL DspGetSoundEffectsOn();
	afx_msg void DspSetSoundEffectsOn(BOOL bNewValue);
	afx_msg BSTR DspGetExtraData();
	afx_msg void DspSetExtraData(LPCTSTR lpszNewValue);
	afx_msg LPDISPATCH DspPlay(LPCTSTR Animation);
	afx_msg void DspStop(const VARIANT & Request);
	afx_msg LPDISPATCH DspSpeak(const VARIANT & Text, const VARIANT & Url);
	afx_msg LPDISPATCH DspGestureAt(short x, short y);
	afx_msg LPDISPATCH DspMoveTo(short x, short y, const VARIANT & Speed);
	afx_msg LPDISPATCH DspHide(const VARIANT & Fast);
	afx_msg LPDISPATCH DspShow(const VARIANT & Fast);
	afx_msg void DspStopAll(const VARIANT & Types);
	afx_msg LPDISPATCH DspInterrupt(LPDISPATCH InterruptRequest);
	afx_msg LPDISPATCH DspWait(LPDISPATCH WaitForRequest);
	afx_msg BOOL DspActivate(const VARIANT & State);
	afx_msg LPDISPATCH DspGet(LPCTSTR Type, LPCTSTR Name, const VARIANT & Queue);
	afx_msg BOOL DspGetAutoPopupMenu();
	afx_msg void DspSetAutoPopupMenu(BOOL bNewValue);
	afx_msg BOOL DspGetHelpModeOn();
	afx_msg void DspSetHelpModeOn(BOOL bNewValue);
	afx_msg long DspGetHelpContextID();
	afx_msg void DspSetHelpContextID(long nNewValue);
	afx_msg short DspGetActive();
	afx_msg void DspSetActive(short nNewValue);
	afx_msg long DspGetLanguageID();
	afx_msg void DspSetLanguageID(long nNewValue);
	afx_msg BSTR DspGetSRModeID();
	afx_msg void DspSetSRModeID(LPCTSTR lpszNewValue);
	afx_msg BSTR DspGetTTSModeID();
	afx_msg void DspSetTTSModeID(LPCTSTR lpszNewValue);
	afx_msg BSTR DspGetHelpFile();
	afx_msg void DspSetHelpFile(LPCTSTR lpszNewValue);
	afx_msg BSTR DspGetGUID();
	afx_msg void DspSetGUID(LPCTSTR lpszNewValue);
	afx_msg short DspGetOriginalHeight();
	afx_msg void DspSetOriginalHeight(short nNewValue);
	afx_msg short DspGetOriginalWidth();
	afx_msg void DspSetOriginalWidth(short nNewValue);
	afx_msg BSTR DspGetVersion();
	afx_msg void DspSetVersion(LPCTSTR lpszNewValue);
	afx_msg LPDISPATCH DspGetAnimationNames();
	afx_msg void DspSetAnimationNames(LPDISPATCH newValue);
	afx_msg long DspGetSRStatus();
	afx_msg void DspSetSRStatus(long nNewValue);
	afx_msg BOOL DspShowPopupMenu(short x, short y);
	afx_msg BOOL DspListen(BOOL Listen);
	afx_msg LPDISPATCH DspThink(LPCTSTR Text);
	afx_msg BOOL DspGetHasIcon();
	afx_msg void DspSetHasIcon(BOOL HasIcon);
	afx_msg void DspGenerateIcon(long ClipLeft = 0, long ClipTop = 0, long ClipWidth = -1, long ClipHeight = -1);
	afx_msg BOOL DspGetIsIconShown();
	afx_msg void DspSetIsIconShown(BOOL IsIconShown);
	afx_msg BOOL DspGetIsIconVisible();
	afx_msg void DspSetIsIconVisible(BOOL IsIconVisible);
	afx_msg BSTR DspGetIconIdentity();
	afx_msg void DspSetIconIdentity(LPCTSTR IconIdentity);
	afx_msg BSTR DspGetIconTip();
	afx_msg void DspSetIconTip(LPCTSTR IconTip);
	afx_msg LPDISPATCH DspGetSpeechEngine (BOOL GetDefault = FALSE);
	afx_msg void DspSetSpeechEngine (BOOL GetDefault, LPDISPATCH SpeechEngine);
	afx_msg LPDISPATCH DspFindSpeechEngines (VARIANT LanguageID);
	afx_msg LPDISPATCH DspGetRecognitionEngine (BOOL GetDefault = FALSE);
	afx_msg void DspSetRecognitionEngine (BOOL GetDefault, LPDISPATCH RecognitionEngine);
	afx_msg LPDISPATCH DspFindRecognitionEngines (VARIANT LanguageID);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_DISPATCH_IID()

	BEGIN_INTERFACE_PART(Character, IDaCtlCharacter2)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

		HRESULT STDMETHODCALLTYPE get_Balloon (IDaCtlBalloon **ppidBalloon);
		HRESULT STDMETHODCALLTYPE get_Commands (IDaCtlCommands **ppidCommands);
		HRESULT STDMETHODCALLTYPE get_Name (BSTR *Name);
		HRESULT STDMETHODCALLTYPE get_Description (BSTR *Description);
		HRESULT STDMETHODCALLTYPE get_Visible (VARIANT_BOOL *Visible);
		HRESULT STDMETHODCALLTYPE put_Left (short Left);
		HRESULT STDMETHODCALLTYPE get_Left (short *Left);
		HRESULT STDMETHODCALLTYPE put_Top (short Top);
		HRESULT STDMETHODCALLTYPE get_Top (short *Top);
		HRESULT STDMETHODCALLTYPE put_Height (short Height);
		HRESULT STDMETHODCALLTYPE get_Height (short *Height);
		HRESULT STDMETHODCALLTYPE put_Width (short Width);
		HRESULT STDMETHODCALLTYPE get_Width (short *Width);
		HRESULT STDMETHODCALLTYPE get_Speed (long *Speed);
		HRESULT STDMETHODCALLTYPE get_Pitch (long *Pitch);
		HRESULT STDMETHODCALLTYPE put_IdleOn (VARIANT_BOOL On);
		HRESULT STDMETHODCALLTYPE get_IdleOn (VARIANT_BOOL *On);
		HRESULT STDMETHODCALLTYPE Activate (VARIANT State, VARIANT_BOOL *Success);
		HRESULT STDMETHODCALLTYPE Play (BSTR Animation, IDaCtlRequest **Request);
		HRESULT STDMETHODCALLTYPE Get (BSTR Type, BSTR Name, VARIANT Queue, IDaCtlRequest **Request);
		HRESULT STDMETHODCALLTYPE Stop (VARIANT Request);
		HRESULT STDMETHODCALLTYPE Wait (IDaCtlRequest *WaitForRequest, IDaCtlRequest **Request);
		HRESULT STDMETHODCALLTYPE Interrupt (IDaCtlRequest *InterruptRequest, IDaCtlRequest **Request);
		HRESULT STDMETHODCALLTYPE Speak (VARIANT Text, VARIANT Url, IDaCtlRequest **Request);
		HRESULT STDMETHODCALLTYPE GestureAt (short x, short y, IDaCtlRequest **Request);
		HRESULT STDMETHODCALLTYPE MoveTo (short x, short y, VARIANT Speed, IDaCtlRequest **Request);
		HRESULT STDMETHODCALLTYPE Hide (VARIANT Fast, IDaCtlRequest **Request);
		HRESULT STDMETHODCALLTYPE Show (VARIANT Fast, IDaCtlRequest **Request);
		HRESULT STDMETHODCALLTYPE StopAll (VARIANT Types);
		HRESULT STDMETHODCALLTYPE get_MoveCause (short *MoveCause);
		HRESULT STDMETHODCALLTYPE get_VisibilityCause (short *VisibilityCause);
		HRESULT STDMETHODCALLTYPE get_HasOtherClients (VARIANT_BOOL *HasOtherClients);
		HRESULT STDMETHODCALLTYPE put_SoundEffectsOn (VARIANT_BOOL On);
		HRESULT STDMETHODCALLTYPE get_SoundEffectsOn (VARIANT_BOOL *On);
		HRESULT STDMETHODCALLTYPE put_Name (BSTR Name);
		HRESULT STDMETHODCALLTYPE put_Description (BSTR Description);
		HRESULT STDMETHODCALLTYPE get_ExtraData (BSTR *ExtraData);

		HRESULT STDMETHODCALLTYPE ShowPopupMenu (short x, short y, VARIANT_BOOL *Showed);
		HRESULT STDMETHODCALLTYPE put_AutoPopupMenu (VARIANT_BOOL On);
		HRESULT STDMETHODCALLTYPE get_AutoPopupMenu (VARIANT_BOOL *On);
		HRESULT STDMETHODCALLTYPE put_HelpModeOn (VARIANT_BOOL On);
		HRESULT STDMETHODCALLTYPE get_HelpModeOn (VARIANT_BOOL *On);
		HRESULT STDMETHODCALLTYPE put_HelpContextID (long ID);
		HRESULT STDMETHODCALLTYPE get_HelpContextID (long *ID);
		HRESULT STDMETHODCALLTYPE get_Active (short *State);
		HRESULT STDMETHODCALLTYPE Listen (VARIANT_BOOL Listen, VARIANT_BOOL *StartedListening);
		HRESULT STDMETHODCALLTYPE put_LanguageID (long LanguageID);
		HRESULT STDMETHODCALLTYPE get_LanguageID (long *LanguageID);
		HRESULT STDMETHODCALLTYPE get_SRModeID (BSTR *EngineModeId);
		HRESULT STDMETHODCALLTYPE put_SRModeID (BSTR EngineModeId);
		HRESULT STDMETHODCALLTYPE get_TTSModeID (BSTR *EngineModeId);
		HRESULT STDMETHODCALLTYPE put_TTSModeID (BSTR EngineModeId);
		HRESULT STDMETHODCALLTYPE get_HelpFile (BSTR *File);
		HRESULT STDMETHODCALLTYPE put_HelpFile (BSTR File);
		HRESULT STDMETHODCALLTYPE get_GUID (BSTR *GUID);
		HRESULT STDMETHODCALLTYPE get_OriginalHeight (short *Height);
		HRESULT STDMETHODCALLTYPE get_OriginalWidth (short *Width);
		HRESULT STDMETHODCALLTYPE Think (BSTR Text, IDaCtlRequest **Request);
		HRESULT STDMETHODCALLTYPE get_Version (BSTR *Version);
		HRESULT STDMETHODCALLTYPE get_AnimationNames (IDaCtlAnimationNames **Names);
		HRESULT STDMETHODCALLTYPE get_SRStatus (long *Status);

		HRESULT STDMETHODCALLTYPE get_HasIcon (VARIANT_BOOL *HasIcon);
		HRESULT STDMETHODCALLTYPE GenerateIcon (long ClipLeft = 0, long ClipTop = 0, long ClipWidth = -1, long ClipHeight = -1);
		HRESULT STDMETHODCALLTYPE get_IsIconShown (VARIANT_BOOL *IsIconShown);
		HRESULT STDMETHODCALLTYPE put_IsIconShown (VARIANT_BOOL IsIconShown);
		HRESULT STDMETHODCALLTYPE get_IsIconVisible (VARIANT_BOOL *IsIconVisible);
		HRESULT STDMETHODCALLTYPE get_IconIdentity (BSTR *IconIdentity);
		HRESULT STDMETHODCALLTYPE put_IconIdentity (BSTR IconIdentity);
		HRESULT STDMETHODCALLTYPE get_IconTip (BSTR *IconTip);
		HRESULT STDMETHODCALLTYPE put_IconTip (BSTR IconTip);
		HRESULT STDMETHODCALLTYPE get_SpeechEngine (VARIANT_BOOL GetDefault, IDaCtlSpeechEngine **SpeechEngine);
		HRESULT STDMETHODCALLTYPE FindSpeechEngines (VARIANT LanguageID, IDaCtlSpeechEngines **SpeechEngines);
		HRESULT STDMETHODCALLTYPE get_RecognitionEngine (VARIANT_BOOL GetDefault, IDaCtlRecognitionEngine **RecognitionEngine);
		HRESULT STDMETHODCALLTYPE FindRecognitionEngines (VARIANT LanguageID, IDaCtlRecognitionEngines **RecognitionEngines);
	END_INTERFACE_PART(Character)

	DECLARE_SUPPORTERRORINFO()
	DECLARE_PROVIDECLASSINFO()
	DECLARE_INTERFACE_MAP()

private:
	CDaAgentCtl *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DACHARACTEROBJ_H_INCLUDED_
