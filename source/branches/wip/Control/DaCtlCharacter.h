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
#pragma once
#include "DaControlMod.h"
#include "DaControl.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E532-A208-11DE-ABF2-002421116FB2}")) DaCtlCharacter :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<DaCtlCharacter, &__uuidof(DaCtlCharacter)>,
	public ISupportErrorInfo,
	public IProvideClassInfoImpl<&__uuidof(DaCtlCharacter), &__uuidof(DaControlTypeLib), _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public IDispatchImpl<IDaCtlCharacter2, &__uuidof(IDaCtlCharacter2), &__uuidof(DaControlTypeLib), _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>
{
public:
	DaCtlCharacter();
	~DaCtlCharacter();

// Attributes
public:
	IDaSvrCharacter2Ptr	mServerObject;
	long				mServerCharID;
	IDispatchPtr		mBalloon;
	IDispatchPtr		mCommands;
	IDispatchPtr		mAnimationNames;

// Operations
public:
	void FinalRelease ();
	HRESULT Terminate (bool pFinal);

	void SetOwner (DaControl * pOwner);
	DaControl * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

	class DaCtlBalloon * GetBalloon (IDaCtlBalloonPtr & pInterface);
	class DaCtlCommands * GetCommands (IDaCtlCommandsPtr & pInterface);
	class DaCtlAnimationNames * GetAnimationNames (IDaCtlAnimationNamesPtr & pInterface);

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
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IDaCtlCharacter
	STDMETHOD(get_Balloon)( IDaCtlBalloon * * ppidBalloon);
	STDMETHOD(get_Commands)( IDaCtlCommands * * ppidCommands);
	STDMETHOD(get_Name)( BSTR * Name);
	STDMETHOD(get_Description)( BSTR * Description);
	STDMETHOD(get_Visible)( VARIANT_BOOL * Visible);
	STDMETHOD(put_Left)(short Left);
	STDMETHOD(get_Left)( short * Left);
	STDMETHOD(put_Top)(short Top);
	STDMETHOD(get_Top)( short * Top);
	STDMETHOD(put_Height)(short Height);
	STDMETHOD(get_Height)( short * Height);
	STDMETHOD(put_Width)(short Width);
	STDMETHOD(get_Width)( short * Width);
	STDMETHOD(get_Speed)( long * Speed);
	STDMETHOD(get_Pitch)( long * Pitch);
	STDMETHOD(put_IdleOn)(VARIANT_BOOL On);
	STDMETHOD(get_IdleOn)( VARIANT_BOOL * On);
	STDMETHOD(Activate)( VARIANT State,  VARIANT_BOOL * Success);
	STDMETHOD(Play)( BSTR Animation,  IDaCtlRequest * * Request);
	STDMETHOD(Get)( BSTR Type,  BSTR Name,  VARIANT Queue,  IDaCtlRequest * * Request);
	STDMETHOD(Stop)( VARIANT Request);
	STDMETHOD(Wait)( IDaCtlRequest * WaitForRequest,  IDaCtlRequest * * Request);
	STDMETHOD(Interrupt)( IDaCtlRequest * InterruptRequest,  IDaCtlRequest * * Request);
	STDMETHOD(Speak)( VARIANT Text,  VARIANT Url,  IDaCtlRequest * * Request);
	STDMETHOD(GestureAt)( short x,  short y,  IDaCtlRequest * * Request);
	STDMETHOD(MoveTo)( short x,  short y,  VARIANT Speed,  IDaCtlRequest * * Request);
	STDMETHOD(Hide)( VARIANT Fast,  IDaCtlRequest * * Request);
	STDMETHOD(Show)( VARIANT Fast,  IDaCtlRequest * * Request);
	STDMETHOD(StopAll)( VARIANT Types);
	STDMETHOD(get_MoveCause)( short * MoveCause);
	STDMETHOD(get_VisibilityCause)( short * VisibilityCause);
	STDMETHOD(get_HasOtherClients)( VARIANT_BOOL * HasOtherClients);
	STDMETHOD(put_SoundEffectsOn)(VARIANT_BOOL On);
	STDMETHOD(get_SoundEffectsOn)( VARIANT_BOOL * On);
	STDMETHOD(put_Name)( BSTR Name);
	STDMETHOD(put_Description)( BSTR Description);
	STDMETHOD(get_ExtraData)( BSTR * ExtraData);
	STDMETHOD(ShowPopupMenu)( short x,  short y,  VARIANT_BOOL * Showed);
	STDMETHOD(put_AutoPopupMenu)( VARIANT_BOOL On);
	STDMETHOD(get_AutoPopupMenu)( VARIANT_BOOL * On);
	STDMETHOD(put_HelpModeOn)( VARIANT_BOOL On);
	STDMETHOD(get_HelpModeOn)( VARIANT_BOOL * On);
	STDMETHOD(put_HelpContextID)( long ID);
	STDMETHOD(get_HelpContextID)( long * ID);
	STDMETHOD(get_Active)( short * State);
	STDMETHOD(Listen)( VARIANT_BOOL Listen,  VARIANT_BOOL * StartedListening);
	STDMETHOD(put_LanguageID)( long LanguageID);
	STDMETHOD(get_LanguageID)( long * LanguageID);
	STDMETHOD(get_SRModeID)( BSTR * EngineModeId);
	STDMETHOD(put_SRModeID)(BSTR EngineModeId);
	STDMETHOD(get_TTSModeID)(BSTR * EngineModeId);
	STDMETHOD(put_TTSModeID)(BSTR EngineModeId);
	STDMETHOD(get_HelpFile)(BSTR * File);
	STDMETHOD(put_HelpFile)(BSTR File);
	STDMETHOD(get_GUID)(BSTR * GUID);
	STDMETHOD(get_OriginalHeight)(short * Height);
	STDMETHOD(get_OriginalWidth)(short * Width);
	STDMETHOD(Think)(BSTR Text,  IDaCtlRequest * * Request);
	STDMETHOD(get_Version)(BSTR * Version);
	STDMETHOD(get_AnimationNames)(IDaCtlAnimationNames * * Names);
	STDMETHOD(get_SRStatus)(long * Status);

	// IDaCtlCharacter2
	STDMETHOD(get_Style)(long * Style);
	STDMETHOD(put_Style)(long Style);
	STDMETHOD(get_HasIcon)(VARIANT_BOOL * HasIcon);
	STDMETHOD(GenerateIcon)(long ClipLeft,  long ClipTop,  long ClipWidth,  long ClipHeight);
	STDMETHOD(get_IconShown)(VARIANT_BOOL * IconShown);
	STDMETHOD(put_IconShown)(VARIANT_BOOL IconShown);
	STDMETHOD(get_IconVisible)(VARIANT_BOOL * IconVisible);
	STDMETHOD(get_IconIdentity)(BSTR * IconIdentity);
	STDMETHOD(put_IconIdentity)(BSTR IconIdentity);
	STDMETHOD(get_IconTip)(BSTR * IconTip);
	STDMETHOD(put_IconTip)(BSTR IconTip);
	STDMETHOD(get_SpeechEngine)(VARIANT_BOOL GetDefault,  IDaCtlSpeechEngine * * SpeechEngine);
	STDMETHOD(FindSpeechEngines)(VARIANT LanguageID,  IDaCtlSpeechEngines * * SpeechEngines);
	STDMETHOD(get_RecognitionEngine)(VARIANT_BOOL GetDefault,  IDaCtlRecognitionEngine * * RecognitionEngine);
	STDMETHOD(FindRecognitionEngines)(VARIANT LanguageID,  IDaCtlRecognitionEngines * * RecognitionEngines);

// Implementation
private:
	DaControl *		mOwner;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlCharacter), DaCtlCharacter)

/////////////////////////////////////////////////////////////////////////////
