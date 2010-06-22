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
#include "DaCmnSettings.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E55C-A208-11DE-ABF2-002421116FB2}")) DaCtlSettings :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<DaCtlSettings, &__uuidof(DaCtlSettings)>,
	public IDispatchImpl<IDaCtlSettings, &__uuidof(IDaCtlSettings), &__uuidof(DaControlTypeLib), _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public IProvideClassInfoImpl<&__uuidof(DaCtlSettings), &__uuidof(DaControlTypeLib), _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public ISupportErrorInfo
{
public:
	DaCtlSettings ();
	~DaCtlSettings ();

// Attributes
public:
	IDaSvrSettingsPtr		mServerObject;
	tPtr <CDaCmnSettings>	mLocalObject;

// Operations
public:
	void FinalRelease ();
	void Terminate (bool pFinal);
	void Disconnect (bool pFinal);

	HRESULT SetOwner (DaControl * pOwner);
	DaControl * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLSETTINGS)
	DECLARE_NOT_AGGREGATABLE(DaCtlSettings)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaCtlSettings)
		COM_INTERFACE_ENTRY(IDaCtlSettings)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlSettings)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaCtlSettings)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaCtlSettings
	HRESULT STDMETHODCALLTYPE get_SoundEffectsEnabled (VARIANT_BOOL *SoundEffectsEnabled);
	HRESULT STDMETHODCALLTYPE get_BalloonEnabled (VARIANT_BOOL *BalloonEnabled);
	HRESULT STDMETHODCALLTYPE get_BalloonFont (IFontDisp **BalloonFont);
	HRESULT STDMETHODCALLTYPE get_TTSEnabled (VARIANT_BOOL *TTSEnabled);
	HRESULT STDMETHODCALLTYPE get_TTSSpeed (short *TTSSpeed);
	HRESULT STDMETHODCALLTYPE get_SREnabled (VARIANT_BOOL *SREnabled);
	HRESULT STDMETHODCALLTYPE get_SRHotKey (BSTR *SRHotKey);
	HRESULT STDMETHODCALLTYPE get_SRHotKeyTime (short *SRHotKeyTime);
	HRESULT STDMETHODCALLTYPE get_SRListeningTip (VARIANT_BOOL *SRListeningTip);
	HRESULT STDMETHODCALLTYPE get_SRListeningPrompt (VARIANT_BOOL *SRListeningPrompt);
	HRESULT STDMETHODCALLTYPE get_AudioStatus (AudioStatusType *AudioStatus);

// Implementation
private:
	DaControl *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlSettings), DaCtlSettings)

/////////////////////////////////////////////////////////////////////////////
