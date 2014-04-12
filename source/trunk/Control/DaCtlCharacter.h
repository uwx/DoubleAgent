/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
#pragma once
#include "DaControlMod.h"
#include "DaControl.h"
#include "DaCmnCharacter.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E532-A208-11DE-ABF2-002421116FB2}")) DaCtlCharacter :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<DaCtlCharacter, &__uuidof(DaCtlCharacter)>,
	public IDispatchImpl<IDaCtlCharacter2, &__uuidof(IDaCtlCharacter2), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public IProvideClassInfoImpl<&__uuidof(DaCtlCharacter), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public ISupportErrorInfo
{
public:
	DaCtlCharacter();
	~DaCtlCharacter();

// Attributes
public:
#ifndef	_DACORE_LOCAL
	IDaSvrCharacter2Ptr		mServerObject;
	long					mServerCharID;
#endif
	tPtr <CDaCmnCharacter>	mLocalObject;

	long GetCharID () const;
	bool IsContained () const;
	bool IsSuspended () const;

// Operations
public:
	void FinalRelease ();
	HRESULT Terminate (bool pFinal);

	HRESULT SetOwner (DaControl * pOwner);
	DaControl * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

	HRESULT SetContained (bool pContained, DWORD pInitialStyle);
	bool SetSuspended (bool pSuspended);

	class DaCtlBalloon * GetBalloon ();
	class DaCtlCommands * GetCommands ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLCHARACTER)
	DECLARE_NOT_AGGREGATABLE(DaCtlCharacter)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaCtlCharacter)
		COM_INTERFACE_ENTRY(IDaCtlCharacter2)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlCharacter2)
		COM_INTERFACE_ENTRY2(IDaCtlCharacter, IDaCtlCharacter2)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCtlCharacter), IDaCtlCharacter2)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCtlCharacterEx), IDaCtlCharacter2)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaCtlCharacter)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaCtlCharacter
	HRESULT STDMETHODCALLTYPE get_Balloon (IDaCtlBalloon2 ** Balloon);
	HRESULT STDMETHODCALLTYPE get_Commands (IDaCtlCommands2 ** Commands);
	HRESULT STDMETHODCALLTYPE get_Name (BSTR * Name);
	HRESULT STDMETHODCALLTYPE get_Description (BSTR * Description);
	HRESULT STDMETHODCALLTYPE get_Visible (VARIANT_BOOL * Visible);
	HRESULT STDMETHODCALLTYPE put_Left (short Left);
	HRESULT STDMETHODCALLTYPE get_Left (short * Left);
	HRESULT STDMETHODCALLTYPE put_Top (short Top);
	HRESULT STDMETHODCALLTYPE get_Top (short * Top);
	HRESULT STDMETHODCALLTYPE put_Height (short Height);
	HRESULT STDMETHODCALLTYPE get_Height (short * Height);
	HRESULT STDMETHODCALLTYPE put_Width (short Width);
	HRESULT STDMETHODCALLTYPE get_Width (short * Width);
	HRESULT STDMETHODCALLTYPE get_Speed (long* Speed);
	HRESULT STDMETHODCALLTYPE get_Pitch (long* Pitch);
	HRESULT STDMETHODCALLTYPE put_IdleOn (VARIANT_BOOL On);
	HRESULT STDMETHODCALLTYPE get_IdleOn (VARIANT_BOOL * On);
	HRESULT STDMETHODCALLTYPE Activate (VARIANT State,  VARIANT_BOOL * Success);
	HRESULT STDMETHODCALLTYPE Play (BSTR Animation,  IDaCtlRequest ** Request);
	HRESULT STDMETHODCALLTYPE Get (BSTR Type,  BSTR Name,  VARIANT Queue,  IDaCtlRequest ** Request);
	HRESULT STDMETHODCALLTYPE Stop (VARIANT Request);
	HRESULT STDMETHODCALLTYPE Wait (IDaCtlRequest * WaitForRequest,  IDaCtlRequest ** Request);
	HRESULT STDMETHODCALLTYPE Interrupt (IDaCtlRequest * InterruptRequest,  IDaCtlRequest ** Request);
	HRESULT STDMETHODCALLTYPE Speak (VARIANT Text,  VARIANT Url,  IDaCtlRequest ** Request);
	HRESULT STDMETHODCALLTYPE GestureAt (short X,  short Y,  IDaCtlRequest ** Request);
	HRESULT STDMETHODCALLTYPE MoveTo (short X,  short Y,  VARIANT Speed,  IDaCtlRequest ** Request);
	HRESULT STDMETHODCALLTYPE Hide (VARIANT Fast,  IDaCtlRequest ** Request);
	HRESULT STDMETHODCALLTYPE Show (VARIANT Fast,  IDaCtlRequest ** Request);
	HRESULT STDMETHODCALLTYPE StopAll (VARIANT Types);
	HRESULT STDMETHODCALLTYPE get_MoveCause (MoveCauseType * MoveCause);
	HRESULT STDMETHODCALLTYPE get_VisibilityCause (VisibilityCauseType * VisibilityCause);
	HRESULT STDMETHODCALLTYPE get_HasOtherClients (VARIANT_BOOL * HasOtherClients);
	HRESULT STDMETHODCALLTYPE put_SoundEffectsOn (VARIANT_BOOL On);
	HRESULT STDMETHODCALLTYPE get_SoundEffectsOn (VARIANT_BOOL * On);
	HRESULT STDMETHODCALLTYPE put_Name (BSTR Name);
	HRESULT STDMETHODCALLTYPE put_Description (BSTR Description);
	HRESULT STDMETHODCALLTYPE get_ExtraData (BSTR * ExtraData);
	HRESULT STDMETHODCALLTYPE ShowPopupMenu (short X,  short Y,  VARIANT_BOOL * Showed);
	HRESULT STDMETHODCALLTYPE put_AutoPopupMenu (VARIANT_BOOL Enabled);
	HRESULT STDMETHODCALLTYPE get_AutoPopupMenu (VARIANT_BOOL * Enabled);
	HRESULT STDMETHODCALLTYPE put_HelpModeOn (VARIANT_BOOL On);
	HRESULT STDMETHODCALLTYPE get_HelpModeOn (VARIANT_BOOL * On);
	HRESULT STDMETHODCALLTYPE put_HelpContextID (long ID);
	HRESULT STDMETHODCALLTYPE get_HelpContextID (long* ID);
	HRESULT STDMETHODCALLTYPE get_Active (short * State);
	HRESULT STDMETHODCALLTYPE Listen (VARIANT_BOOL Listen,  VARIANT_BOOL * StartedListening);
	HRESULT STDMETHODCALLTYPE put_LanguageID (long LanguageID);
	HRESULT STDMETHODCALLTYPE get_LanguageID (long* LanguageID);
	HRESULT STDMETHODCALLTYPE get_SRModeID (BSTR * EngineModeId);
	HRESULT STDMETHODCALLTYPE put_SRModeID (BSTR EngineModeId);
	HRESULT STDMETHODCALLTYPE get_TTSModeID (BSTR * EngineModeId);
	HRESULT STDMETHODCALLTYPE put_TTSModeID (BSTR EngineModeId);
	HRESULT STDMETHODCALLTYPE get_HelpFile (BSTR * File);
	HRESULT STDMETHODCALLTYPE put_HelpFile (BSTR File);
	HRESULT STDMETHODCALLTYPE get_GUID (BSTR * CharGUID);
	HRESULT STDMETHODCALLTYPE get_OriginalHeight (short * Height);
	HRESULT STDMETHODCALLTYPE get_OriginalWidth (short * Width);
	HRESULT STDMETHODCALLTYPE Think (BSTR Text, IDaCtlRequest ** Request);
	HRESULT STDMETHODCALLTYPE get_Version (BSTR * Version);
	HRESULT STDMETHODCALLTYPE get_AnimationNames (IDaCtlAnimationNames ** Names);
	HRESULT STDMETHODCALLTYPE get_SRStatus (long* Status);

	// IDaCtlCharacter2
	HRESULT STDMETHODCALLTYPE get_Style (long* Style);
	HRESULT STDMETHODCALLTYPE put_Style (long Style);
	HRESULT STDMETHODCALLTYPE get_HasIcon (VARIANT_BOOL * HasIcon);
	HRESULT STDMETHODCALLTYPE GenerateIcon (long ClipLeft,  long ClipTop,  long ClipWidth,  long ClipHeight);
	HRESULT STDMETHODCALLTYPE get_IconShown (VARIANT_BOOL * IconShown);
	HRESULT STDMETHODCALLTYPE put_IconShown (VARIANT_BOOL IconShown);
	HRESULT STDMETHODCALLTYPE get_IconVisible (VARIANT_BOOL * IconVisible);
	HRESULT STDMETHODCALLTYPE get_IconIdentity (BSTR * IconIdentity);
	HRESULT STDMETHODCALLTYPE put_IconIdentity (BSTR IconIdentity);
	HRESULT STDMETHODCALLTYPE get_IconTip (BSTR * IconTip);
	HRESULT STDMETHODCALLTYPE put_IconTip (BSTR IconTip);

	HRESULT STDMETHODCALLTYPE SpeakFormatted (IDaCtlFormattedText * FormattedText, IDaCtlRequest ** Request);
	HRESULT STDMETHODCALLTYPE ThinkFormatted (IDaCtlFormattedText * FormattedText, IDaCtlRequest ** Request);
	HRESULT STDMETHODCALLTYPE NewFormattedText (IDaCtlFormattedText **FormattedText);
	HRESULT STDMETHODCALLTYPE get_TTSEngine (VARIANT GetDefault,  IDaCtlTTSEngine ** TTSEngine);
	HRESULT STDMETHODCALLTYPE FindTTSEngines (VARIANT LanguageID,  IDaCtlTTSEngines ** TTSEngines);
	HRESULT STDMETHODCALLTYPE get_SREngine (VARIANT GetDefault,  IDaCtlSREngine ** SREngine);
	HRESULT STDMETHODCALLTYPE FindSREngines (VARIANT LanguageID,  IDaCtlSREngines ** SREngines);

	HRESULT STDMETHODCALLTYPE get_UniqueID (BSTR * CharGUID);
	HRESULT STDMETHODCALLTYPE get_CharacterID (BSTR *CharacterID);
	HRESULT STDMETHODCALLTYPE get_Connected (VARIANT_BOOL *Connected);
	HRESULT STDMETHODCALLTYPE get_FileName (BSTR *FileName);
	HRESULT STDMETHODCALLTYPE get_FilePath (BSTR *FilePath);
	HRESULT STDMETHODCALLTYPE put_ActiveState (ActiveStateType ActiveState);
	HRESULT STDMETHODCALLTYPE get_ActiveState (ActiveStateType *ActiveState);
	HRESULT STDMETHODCALLTYPE get_IdleState (VARIANT_BOOL *IdleState);
	HRESULT STDMETHODCALLTYPE put_IdleEnabled (VARIANT_BOOL Enabled);
	HRESULT STDMETHODCALLTYPE get_IdleEnabled (VARIANT_BOOL *Enabled);
	HRESULT STDMETHODCALLTYPE put_SoundEffectsEnabled (VARIANT_BOOL Enabled);
	HRESULT STDMETHODCALLTYPE get_SoundEffectsEnabled (VARIANT_BOOL *Enabled);
	HRESULT STDMETHODCALLTYPE Prepare (PrepareResourceType ResourceType, BSTR Name, VARIANT Queue, IDaCtlRequest **Request);
	HRESULT STDMETHODCALLTYPE get_ListeningStatus (ListeningStatusType *ListeningStatus);
	HRESULT STDMETHODCALLTYPE get_Smoothed (VARIANT_BOOL *Smoothed);
	HRESULT STDMETHODCALLTYPE get_SmoothEdges (VARIANT_BOOL *SmoothEdges);
	HRESULT STDMETHODCALLTYPE get_Animations (SAFEARRAY**Animations);
	HRESULT STDMETHODCALLTYPE get_States (SAFEARRAY**States);
	HRESULT STDMETHODCALLTYPE put_SuspendPause (VARIANT_BOOL SuspendPause);
	HRESULT STDMETHODCALLTYPE get_SuspendPause (VARIANT_BOOL *SuspendPause);
	HRESULT STDMETHODCALLTYPE put_SuspendStop (VARIANT_BOOL Enabled);
	HRESULT STDMETHODCALLTYPE get_SuspendStop (VARIANT_BOOL *Enabled);
	HRESULT STDMETHODCALLTYPE put_SuspendHide (VARIANT_BOOL Enabled);
	HRESULT STDMETHODCALLTYPE get_SuspendHide (VARIANT_BOOL *Enabled);
	HRESULT STDMETHODCALLTYPE SetSize (short Width, short Height);

// Implementation
public:
	IDispatchPtr	mBalloon;
	IDispatchPtr	mCommands;
private:
	DaControl *		mOwner;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlCharacter), DaCtlCharacter)

/////////////////////////////////////////////////////////////////////////////
