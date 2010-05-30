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
#include <ObjSafe.h>
#include <atlctl.h>
#include "DaControlMod.h"
#include "DaCtlEvents.h"
#include "DaCtlRequest.h"
#include "ListeningState.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E530-A208-11DE-ABF2-002421116FB2}")) DaControl :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IProvideClassInfo2Impl<&__uuidof(DaControl), &__uuidof(_DaCtlEvents), &__uuidof(DaControlTypeLib), _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public ISupportErrorInfo,
	public IObjectSafetyImpl<DaControl, INTERFACESAFE_FOR_UNTRUSTED_CALLER|INTERFACESAFE_FOR_UNTRUSTED_DATA>,
	public CStockPropImpl<DaControl, IDaControl2, &__uuidof(IDaControl2), &__uuidof(DaControlTypeLib), _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public ISpecifyPropertyPagesImpl<DaControl>,
	public IPerPropertyBrowsingImpl<DaControl>,
	public IPersistStreamInitImpl<DaControl>,
	public IPersistPropertyBagImpl<DaControl>,
	public IOleControlImpl<DaControl>,
	public IOleObjectImpl<DaControl>,
	public IOleInPlaceActiveObjectImpl<DaControl>,
	public IViewObjectExImpl<DaControl>,
	public IOleInPlaceObjectWindowlessImpl<DaControl>,
	public IObjectWithSiteImpl<DaControl>,
	public IConnectionPointContainerImpl<DaControl>,
	public IPropertyNotifySinkCP<DaControl>,
	public CProxy_DaCtlEvents<DaControl>,
	public CProxy_AgentEvents<DaControl>,
//	public IDataObjectImpl<DaControl>,
	public CComCoClass<DaControl, &__uuidof(DaControl)>,
	public CComControl<DaControl>,
	public CInstanceAnchor,	// For local characters only
	public CListeningAnchor	// For local characters only
{
public:
	DaControl ();
	~DaControl ();

// Attributes
public:
	IDaServer2Ptr			mServer;
	IDispatchPtr			mCharacters;
	IDispatchPtr			mSettings;
	IDispatchPtr			mAudioOutput;
	IDispatchPtr			mSpeechInput;
	IDispatchPtr			mCommandsWindow;
	IDispatchPtr			mPropertySheet;
	IDispatchPtr			mCharacterFiles;
	IDispatchPtr			mTTSEngines;
	IDispatchPtr			mSREngines;
	bool					mRaiseRequestErrors;
	bool					mAutoConnect;
	CEventNotifyReflect		mLocalEventNotify;
	DWORD					mLocalCharacterStyle;

	// IOleControl
	OLE_COLOR				m_clrBackColor;
	OLE_COLOR				m_clrBorderColor;
	LONG					m_nBorderStyle;
	BOOL					m_bBorderVisible;
	LONG					m_nBorderWidth;
	CComPtr <IPictureDisp>	m_pMouseIcon;
	LONG					m_nMousePointer;

// Operations
public:
	static HRESULT WINAPI UpdateRegistryOverride (BOOL bRegister);
	void FinalRelease ();
	void Terminate (bool pFinal);

	IDaCtlRequest * PutRequest (DaRequestCategory pCategory, long pReqID, HRESULT pResult);
	void CompleteRequests (bool pIdleTime = false);
	void TerminateRequests (bool pFinal);

	HRESULT ConnectServer ();
	HRESULT DisconnectServer (bool pForce);
	void DisconnectNotify (bool pForce);

// Overrides
public:
	virtual bool AddListeningTimer (UINT_PTR pTimerId, DWORD pInterval, _ITimerNotifySink * pNotifySink);
	virtual bool DelListeningTimer (UINT_PTR pTimerId);

// Declarations
public:
#ifdef	_DEBUG
	DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE|OLEMISC_CANTLINKINSIDE|OLEMISC_INSIDEOUT|OLEMISC_SETCLIENTSITEFIRST|OLEMISC_INSERTNOTREPLACE|OLEMISC_NOUIACTIVATE|OLEMISC_STATIC)
#else
	DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE|OLEMISC_CANTLINKINSIDE|OLEMISC_INSIDEOUT|OLEMISC_SETCLIENTSITEFIRST|OLEMISC_INSERTNOTREPLACE|OLEMISC_NOUIACTIVATE|OLEMISC_ONLYICONIC|OLEMISC_INVISIBLEATRUNTIME)
#endif
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND|VIEWSTATUS_OPAQUE)
	DECLARE_REGISTRY_RESOURCEID(IDR_DACONTROL)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaControl)
		COM_INTERFACE_ENTRY(IDaControl2)
		COM_INTERFACE_ENTRY2(IDispatch, IDaControl2)
		COM_INTERFACE_ENTRY2(IDaControl, IDaControl2)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCtl), IDaControl)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCtlEx), IDaControl)
		COM_INTERFACE_ENTRY(IViewObjectEx)
		COM_INTERFACE_ENTRY(IViewObject2)
		COM_INTERFACE_ENTRY(IViewObject)
		COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
		COM_INTERFACE_ENTRY(IOleInPlaceObject)
		COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObject)
		COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
		COM_INTERFACE_ENTRY(IOleControl)
		COM_INTERFACE_ENTRY(IOleObject)
		COM_INTERFACE_ENTRY(IPersistStreamInit)
		COM_INTERFACE_ENTRY(IPersistPropertyBag)
		COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
		COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
		COM_INTERFACE_ENTRY(IPerPropertyBrowsing)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
//		COM_INTERFACE_ENTRY(IDataObject)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo2)
		COM_INTERFACE_ENTRY(IObjectWithSite)
		COM_INTERFACE_ENTRY(IObjectSafety)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaControl)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	   IMPLEMENTED_CATEGORY(CATID_Control)
	   IMPLEMENTED_CATEGORY(CATID_Insertable)
	   IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
	   IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
	END_CATEGORY_MAP()

	BEGIN_PROP_MAP(DaControl)
		PROP_DATA_ENTRY("_ExtentX", m_sizeExtent.cx, VT_UI4)
		PROP_DATA_ENTRY("_ExtentY", m_sizeExtent.cy, VT_UI4)
		PROP_ENTRY_TYPE("AutoSize", DISPID_AUTOSIZE, CLSID_NULL, VT_BOOL)
		PROP_ENTRY_TYPE("BackColor", DISPID_BACKCOLOR, CLSID_StockColorPage, VT_UI4)
		PROP_ENTRY_TYPE("BorderColor", DISPID_BORDERCOLOR, CLSID_StockColorPage, VT_UI4)
		PROP_ENTRY_TYPE("BorderStyle", DISPID_BORDERSTYLE, CLSID_NULL, VT_I4)
		PROP_ENTRY_TYPE("BorderVisible", DISPID_BORDERVISIBLE, CLSID_NULL, VT_BOOL)
		PROP_ENTRY_TYPE("BorderWidth", DISPID_BORDERWIDTH, CLSID_NULL, VT_I4)
		PROP_ENTRY_TYPE("MouseIcon", DISPID_MOUSEICON, CLSID_StockPicturePage, VT_DISPATCH)
		PROP_ENTRY_TYPE("MousePointer", DISPID_MOUSEPOINTER, CLSID_NULL, VT_UI4)
		PROP_ENTRY_TYPE("RaiseRequestErrors", DISPID_IAgentCtlEx_RaiseRequestErrors, CLSID_NULL, VT_BOOL)
		PROP_ENTRY_TYPE("AutoConnect", DISPID_IDaControl2_AutoConnect, CLSID_NULL, VT_BOOL)
		PROP_PAGE(CLSID_StockColorPage)
		PROP_PAGE(CLSID_StockPicturePage)
	END_PROP_MAP()

	BEGIN_PROP_VAL_MAP(DaControl)
		PROP_VAL_INT(DISPID_BORDERSTYLE, 0, "0 - None")
		PROP_VAL_INT(DISPID_BORDERSTYLE, 1, "1 - FixedSingle")
		PROP_VAL_INT(DISPID_MOUSEPOINTER, 0, "0 - Default")
		PROP_VAL_INT(DISPID_MOUSEPOINTER, 1, "1 - Arrow")
		PROP_VAL_INT(DISPID_MOUSEPOINTER, 2, "2 - Cross")
		PROP_VAL_INT(DISPID_MOUSEPOINTER, 11, "11 - WaitCursor")
		PROP_VAL_INT(DISPID_MOUSEPOINTER, 12, "12 - No")
		PROP_VAL_INT(DISPID_MOUSEPOINTER, 14, "14 - Help")
	END_PROP_VAL_MAP()

	BEGIN_CONNECTION_POINT_MAP(DaControl)
		CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
		CONNECTION_POINT_ENTRY(__uuidof(_DaCtlEvents))
		CONNECTION_POINT_ENTRY(__uuidof(_AgentEvents))
	END_CONNECTION_POINT_MAP()

	BEGIN_MSG_MAP(DaControl)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_HOTKEY, OnHotKey)
		MESSAGE_HANDLER(mCompleteRequestsMsg, OnCompleteRequests)
		CHAIN_MSG_MAP(CComControl<DaControl>)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

// Message Handlers
public:
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnHotKey(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCompleteRequests(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

// Interfaces
public:
	// ISupportErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IOleControl
	HRESULT OnDrawAdvanced(ATL_DRAWINFO& di);

#ifdef	_DEBUG_NOT
	HRESULT IPersistPropertyBag_Load(LPPROPERTYBAG pPropBag, LPERRORLOG pErrorLog, const ATL_PROPMAP_ENTRY* pMap)
	{
		LogMessage (LogNormal, _T("--- IPersistPropertyBag_Load ---"));
		HRESULT lResult = IPersistPropertyBagImpl<DaControl>::IPersistPropertyBag_Load (pPropBag, pErrorLog, pMap);
		LogComErrAnon (LogAlways, lResult, _T("--- IPersistPropertyBag_Load ---"));
		return lResult;
	}

	HRESULT IPersistPropertyBag_Save(LPPROPERTYBAG pPropBag, BOOL fClearDirty, BOOL fSaveAllProperties, const ATL_PROPMAP_ENTRY* pMap)
	{
		LogMessage (LogNormal, _T("--- IPersistPropertyBag_Save [%d %d] ---"), fClearDirty, fSaveAllProperties);
		HRESULT lResult = IPersistPropertyBagImpl<DaControl>::IPersistPropertyBag_Save (pPropBag, fClearDirty, fSaveAllProperties, pMap);
		LogComErrAnon (LogAlways, lResult, _T("--- IPersistPropertyBag_Save ---"));
		return lResult;
	}

	HRESULT IPersistStreamInit_Load(LPSTREAM pStm, const ATL_PROPMAP_ENTRY* pMap)
	{
		LogMessage (LogNormal, _T("--- IPersistStreamInit_Load ---"));
		HRESULT lResult = IPersistStreamInitImpl<DaControl>::IPersistStreamInit_Load (pStm, pMap);
		LogComErrAnon (LogAlways, lResult, _T("--- IPersistStreamInit_Load ---"));
		return lResult;
	}

	HRESULT IPersistStreamInit_Save(LPSTREAM pStm, BOOL fClearDirty, const ATL_PROPMAP_ENTRY* pMap)
	{
		LogMessage (LogNormal, _T("--- IPersistStreamInit_Save [%d] ---"), fClearDirty);
		HRESULT lResult = IPersistStreamInitImpl<DaControl>::IPersistStreamInit_Save (pStm, fClearDirty, pMap);
		LogComErrAnon (LogAlways, lResult, _T("--- IPersistStreamInit_Save ---"));
		return lResult;
	}
#endif

	void OnAutoSizeChanged();
	void OnBackColorChanged();
	void OnBorderColorChanged();
	void OnBorderStyleChanged();
	void OnBorderVisibleChanged();
	void OnBorderWidthChanged();
	void OnMouseIconChanged();
	void OnMousePointerChanged();

	// IDaControl
	HRESULT STDMETHODCALLTYPE get_Characters (IDaCtlCharacters2 ** Characters);
	HRESULT STDMETHODCALLTYPE get_Settings (IDaCtlSettings ** Settings);
	HRESULT STDMETHODCALLTYPE get_AudioOutput (IDaCtlAudioOutput ** AudioOutput);
	HRESULT STDMETHODCALLTYPE get_SpeechInput (IDaCtlSpeechInput ** SpeechInput);
	HRESULT STDMETHODCALLTYPE get_PropertySheet (IDaCtlPropertySheet2 ** PropSheet);
	HRESULT STDMETHODCALLTYPE get_CommandsWindow (IDaCtlCommandsWindow ** CommandsWindow);
	HRESULT STDMETHODCALLTYPE get_Connected (VARIANT_BOOL * Connected);
	HRESULT STDMETHODCALLTYPE put_Connected (VARIANT_BOOL Connected);
	HRESULT STDMETHODCALLTYPE get_Suspended (VARIANT_BOOL * Suspended);
	HRESULT STDMETHODCALLTYPE ShowDefaultCharacterProperties (VARIANT x,  VARIANT y);
	HRESULT STDMETHODCALLTYPE get_RaiseRequestErrors (VARIANT_BOOL * RaiseErrors);
	HRESULT STDMETHODCALLTYPE put_RaiseRequestErrors (VARIANT_BOOL RaiseErrors);

	// IDaControl2
	HRESULT STDMETHODCALLTYPE get_CharacterFiles (IDaCtlCharacterFiles ** CharacterFiles);
	HRESULT STDMETHODCALLTYPE get_CharacterStyle (long * CharacterStyle);
	HRESULT STDMETHODCALLTYPE put_CharacterStyle (long CharacterStyle);
	HRESULT STDMETHODCALLTYPE get_TTSEngines (IDaCtlTTSEngines ** TTSEngines);
	HRESULT STDMETHODCALLTYPE FindTTSEngines (VARIANT LanguageID,  VARIANT Gender,  IDaCtlTTSEngines ** TTSEngines);
	HRESULT STDMETHODCALLTYPE GetCharacterTTSEngine (VARIANT LoadKey,  IDaCtlTTSEngine ** TTSEngine);
	HRESULT STDMETHODCALLTYPE FindCharacterTTSEngines (VARIANT LoadKey,  VARIANT LanguageID,  IDaCtlTTSEngines ** TTSEngines);
	HRESULT STDMETHODCALLTYPE get_SREngines (IDaCtlSREngines ** SREngines);
	HRESULT STDMETHODCALLTYPE FindSREngines (VARIANT LanguageID,  IDaCtlSREngines ** SREngines);
	HRESULT STDMETHODCALLTYPE GetCharacterSREngine (VARIANT LoadKey,  IDaCtlSREngine ** SREngine);
	HRESULT STDMETHODCALLTYPE FindCharacterSREngines (VARIANT LoadKey,  VARIANT LanguageID,  IDaCtlSREngines ** SREngines);
	HRESULT STDMETHODCALLTYPE get_AutoConnect (VARIANT_BOOL * AutoConnect);
	HRESULT STDMETHODCALLTYPE put_AutoConnect (VARIANT_BOOL AutoConnect);

// Events
public:
	void FireActivateInput(LPCTSTR CharacterID);
	void FireDeactivateInput(LPCTSTR CharacterID);
	void FireClick(LPCTSTR CharacterID, short Button, short Shift, short x, short y);
	void FireDblClick(LPCTSTR CharacterID, short Button, short Shift, short x, short y);
	void FireDragStart(LPCTSTR CharacterID, short Button, short Shift, short x, short y);
	void FireDragComplete(LPCTSTR CharacterID, short Button, short Shift, short x, short y);
	void FireShow(LPCTSTR CharacterID, VisibilityCauseType Cause);
	void FireHide(LPCTSTR CharacterID, VisibilityCauseType Cause);
	void FireRequestStart(LPDISPATCH Request);
	void FireRequestComplete(LPDISPATCH Request);
	void FireBookmark(long BookmarkID);
	void FireCommand(LPDISPATCH UserInput);
	void FireIdleStart(LPCTSTR CharacterID);
	void FireIdleComplete(LPCTSTR CharacterID);
	void FireMove(LPCTSTR CharacterID, short x, short y, MoveCauseType Cause);
	void FireSize(LPCTSTR CharacterID, short Width, short Height);
	void FireBalloonShow(LPCTSTR CharacterID);
	void FireBalloonHide(LPCTSTR CharacterID);
	void FireListenStart(LPCTSTR CharacterID);
	void FireListenComplete(LPCTSTR CharacterID, ListenCompleteType Cause);
	void FireDefaultCharacterChange(LPCTSTR CharGUID);
	void FireAgentPropertyChange();
	void FireActiveClientChange(LPCTSTR CharacterID, BOOL Active);

// Implementation
public:
	CAtlString GetControlCharacterID (long pServerCharID);
	CAtlString GetActiveCharacterID ();
	class DaCtlCharacter * GetActiveCharacter ();

	void CharacterLoaded (int pCharacterCount, class DaCtlCharacter * pCharacter);
	void CharacterUnloaded (int pCharacterCount, class DaCtlCharacter * pCharacter);

	void RequestCreated (DaCtlRequest * pRequest);
	void RequestDeleted (DaCtlRequest * pRequest);

protected:
	HWND GetMsgPostingWnd ();

protected:
	friend class CServerNotifySink;
	tPtr <CComObject <CServerNotifySink> >	mServerNotifySink;
	CAtlMap <long, DaCtlRequest *>			mActiveRequests;
	CAtlPtrTypeArray <DaCtlRequest>			mCompletedRequests;
	tPtr <CMsgPostingWnd <DaControl> >		mMsgPostingWnd;
private:
	CIconHandle								mIcon;
	bool									mFinalReleased;
	static UINT								mCompleteRequestsMsg;
};

/////////////////////////////////////////////////////////////////////////////

#define	UpdateRegistry	UpdateRegistryOverride
OBJECT_ENTRY_AUTO(__uuidof(DaControl), DaControl)
#undef	UpdateRegistry

/////////////////////////////////////////////////////////////////////////////
