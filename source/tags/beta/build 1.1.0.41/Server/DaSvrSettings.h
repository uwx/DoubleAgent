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
#pragma once
#include "DaServerApp.h"
#include "DaCmnSettings.h"
#include "ServerLifetime.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E528-A208-11DE-ABF2-002421116FB2}")) DaSvrSettings :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<DaSvrSettings, &__uuidof(DaSvrSettings)>,
	public IDispatchImpl<IDaSvrSettings, &__uuidof(IDaSvrSettings), &__uuidof(DoubleAgentSvr_TypeLib), DoubleAgentSvr_MajorVer, DoubleAgentSvr_MinorVer>,
	public IProvideClassInfoImpl<&__uuidof(DaSvrSettings), &__uuidof(DoubleAgentSvr_TypeLib), DoubleAgentSvr_MajorVer, DoubleAgentSvr_MinorVer>,
	public ISupportErrorInfo,
	public CDaCmnSettings,
	public CSvrObjLifetime
{
public:
	DaSvrSettings ();
	virtual ~DaSvrSettings ();

// Attributes
public:

// Operations
public:
	static DaSvrSettings * CreateInstance (LPCTSTR pClientMutexName = NULL);
	void Terminate (bool pFinal, bool pAbandonned = false);
	void FinalRelease ();

// Overrides
public:
	virtual void OnClientEnded ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DASVRSETTINGS)
	DECLARE_NOT_AGGREGATABLE(DaSvrSettings)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaSvrSettings)
		COM_INTERFACE_ENTRY(IDaSvrSettings)
		COM_INTERFACE_ENTRY2(IDispatch, IDaSvrSettings)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaSvrSettings)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaSvrSettings
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
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaSvrSettings), DaSvrSettings)

/////////////////////////////////////////////////////////////////////////////
