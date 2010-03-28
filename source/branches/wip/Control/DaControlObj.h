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
#include "DaControl.h"
#include "DaCtlEvents.h"
#include "DaCtlRequest.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E530-A208-11DE-ABF2-002421116FB2}")) CDaControlObj :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CStockPropImpl<CDaControlObj, IDaControl2, &__uuidof(IDaControl2), &LIBID_DoubleAgentCtl, _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public ISpecifyPropertyPagesImpl<CDaControlObj>,
	public IPerPropertyBrowsingImpl<CDaControlObj>,
	public IPersistStreamInitImpl<CDaControlObj>,
	public IPersistPropertyBagImpl<CDaControlObj>,
	public IOleControlImpl<CDaControlObj>,
	public IOleObjectImpl<CDaControlObj>,
	public IOleInPlaceActiveObjectImpl<CDaControlObj>,
	public IViewObjectExImpl<CDaControlObj>,
	public IOleInPlaceObjectWindowlessImpl<CDaControlObj>,
	public IConnectionPointContainerImpl<CDaControlObj>,
	public IPropertyNotifySinkCP<CDaControlObj>,
	public CProxy_DaCtlEvents<CDaControlObj>,
	public CProxy_AgentEvents<CDaControlObj>,
	public IObjectWithSiteImpl<CDaControlObj>,
//	public IDataObjectImpl<CDaControlObj>,
	public IProvideClassInfo2Impl<&__uuidof(DaControl), &__uuidof(_DaCtlEvents), &LIBID_DoubleAgentCtl, _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public IObjectSafetyImpl<CDaControlObj, INTERFACESAFE_FOR_UNTRUSTED_CALLER|INTERFACESAFE_FOR_UNTRUSTED_DATA>,
	public ISupportErrorInfo,
	public CComCoClass<CDaControlObj, &__uuidof(DaControl)>,
	public CComControl<CDaControlObj>
{
public:
	CDaControlObj ();
	~CDaControlObj ();

// Declarations
public:
//	DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE|OLEMISC_CANTLINKINSIDE|OLEMISC_INSIDEOUT|OLEMISC_SETCLIENTSITEFIRST|OLEMISC_INSERTNOTREPLACE/*|OLEMISC_ACTIVATEWHENVISIBLE*/|OLEMISC_NOUIACTIVATE|/*OLEMISC_STATIC|*/OLEMISC_ONLYICONIC)
	DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE|OLEMISC_CANTLINKINSIDE|OLEMISC_INSIDEOUT|OLEMISC_SETCLIENTSITEFIRST|OLEMISC_INSERTNOTREPLACE|OLEMISC_NOUIACTIVATE|OLEMISC_ONLYICONIC|OLEMISC_INVISIBLEATRUNTIME)
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND|VIEWSTATUS_OPAQUE)
	DECLARE_REGISTRY_RESOURCEID(IDR_DACONTROLOBJ)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CDaControlObj)
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

	BEGIN_CATEGORY_MAP(CDaControlObj)
	   IMPLEMENTED_CATEGORY(__uuidof(CDaAgent))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	   IMPLEMENTED_CATEGORY(CATID_Control)
	   IMPLEMENTED_CATEGORY(CATID_Insertable)
	   IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
	   IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
	END_CATEGORY_MAP()

	BEGIN_PROP_MAP(CDaControlObj)
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
		PROP_PAGE(CLSID_StockColorPage)
		PROP_PAGE(CLSID_StockPicturePage)
	END_PROP_MAP()
	
	BEGIN_PROP_VAL_MAP(CDaControlObj)
		PROP_VAL_INT(DISPID_BORDERSTYLE, 0, "0 - None")
		PROP_VAL_INT(DISPID_BORDERSTYLE, 1, "1 - FixedSingle")
		PROP_VAL_INT(DISPID_MOUSEPOINTER, 0, "0 - Default")
		PROP_VAL_INT(DISPID_MOUSEPOINTER, 1, "1 - Arrow")
		PROP_VAL_INT(DISPID_MOUSEPOINTER, 2, "2 - Cross")
		PROP_VAL_INT(DISPID_MOUSEPOINTER, 11, "11 - WaitCursor")
		PROP_VAL_INT(DISPID_MOUSEPOINTER, 12, "12 - No")
		PROP_VAL_INT(DISPID_MOUSEPOINTER, 14, "14 - Help")
	END_PROP_VAL_MAP()

	BEGIN_CONNECTION_POINT_MAP(CDaControlObj)
		CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
		CONNECTION_POINT_ENTRY(__uuidof(_DaCtlEvents))
		CONNECTION_POINT_ENTRY(__uuidof(_AgentEvents))
	END_CONNECTION_POINT_MAP()

	BEGIN_MSG_MAP(CDaControlObj)
		MESSAGE_HANDLER(mCompleteRequestsMsg, OnCompleteRequests)
		CHAIN_MSG_MAP(CComControl<CDaControlObj>)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()
	// Handler prototypes:
	//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	
// Message Handlers
public:
	LRESULT OnCompleteRequests(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

// Interfaces
public:
	// ISupportErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IOleControl
	HRESULT OnDrawAdvanced(ATL_DRAWINFO& di);

#ifdef	_DEBUG_NOT
	HRESULT IPersistPropertyBag_Load(LPPROPERTYBAG pPropBag, LPERRORLOG pErrorLog, const ATL_PROPMAP_ENTRY* pMap)
	{
		LogMessage (LogNormal, _T("--- IPersistPropertyBag_Load ---"));
		HRESULT lResult = IPersistPropertyBagImpl<CDaControlObj>::IPersistPropertyBag_Load (pPropBag, pErrorLog, pMap);
		LogComErrAnon (LogAlways, lResult, _T("--- IPersistPropertyBag_Load ---"));
		return lResult;
	}

	HRESULT IPersistPropertyBag_Save(LPPROPERTYBAG pPropBag, BOOL fClearDirty, BOOL fSaveAllProperties, const ATL_PROPMAP_ENTRY* pMap)
	{
		LogMessage (LogNormal, _T("--- IPersistPropertyBag_Save [%d %d] ---"), fClearDirty, fSaveAllProperties);
		HRESULT lResult = IPersistPropertyBagImpl<CDaControlObj>::IPersistPropertyBag_Save (pPropBag, fClearDirty, fSaveAllProperties, pMap);
		LogComErrAnon (LogAlways, lResult, _T("--- IPersistPropertyBag_Save ---"));
		return lResult;
	}

	HRESULT IPersistStreamInit_Load(LPSTREAM pStm, const ATL_PROPMAP_ENTRY* pMap)
	{
		LogMessage (LogNormal, _T("--- IPersistStreamInit_Load ---"));
		HRESULT lResult = IPersistStreamInitImpl<CDaControlObj>::IPersistStreamInit_Load (pStm, pMap);
		LogComErrAnon (LogAlways, lResult, _T("--- IPersistStreamInit_Load ---"));
		return lResult;
	}

	HRESULT IPersistStreamInit_Save(LPSTREAM pStm, BOOL fClearDirty, const ATL_PROPMAP_ENTRY* pMap)
	{
		LogMessage (LogNormal, _T("--- IPersistStreamInit_Save [%d] ---"), fClearDirty);
		HRESULT lResult = IPersistStreamInitImpl<CDaControlObj>::IPersistStreamInit_Save (pStm, fClearDirty, pMap);
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
	STDMETHOD(get_Characters)(IDaCtlCharacters * * Characters);
	STDMETHOD(get_AudioOutput)(IDaCtlAudioOutput * * AudioOutput);
	STDMETHOD(get_SpeechInput)(IDaCtlSpeechInput * * SpeechInput);
	STDMETHOD(get_PropertySheet)(IDaCtlPropertySheet * * PropSheet);
	STDMETHOD(get_CommandsWindow)(IDaCtlCommandsWindow * * CommandsWindow);
	STDMETHOD(get_Connected)(VARIANT_BOOL * Connected);
	STDMETHOD(put_Connected)(VARIANT_BOOL Connected);
	STDMETHOD(get_Suspended)(VARIANT_BOOL * Suspended);
	STDMETHOD(ShowDefaultCharacterProperties)(VARIANT x,  VARIANT y);
	STDMETHOD(get_RaiseRequestErrors)(VARIANT_BOOL * RaiseErrors);
	STDMETHOD(put_RaiseRequestErrors)(VARIANT_BOOL RaiseErrors);

	// IDaControl2
	STDMETHOD(get_CharacterFiles)(IDaCtlCharacterFiles * * CharacterFiles);
	STDMETHOD(get_CharacterStyle)(long * CharacterStyle);
	STDMETHOD(put_CharacterStyle)(long CharacterStyle);
	STDMETHOD(get_SpeechEngines)(IDaCtlSpeechEngines * * SpeechEngines);
	STDMETHOD(FindSpeechEngines)(VARIANT LanguageID,  VARIANT Gender,  IDaCtlSpeechEngines * * SpeechEngines);
	STDMETHOD(GetCharacterSpeechEngine)(VARIANT LoadKey,  IDaCtlSpeechEngine * * SpeechEngine);
	STDMETHOD(FindCharacterSpeechEngines)(VARIANT LoadKey,  VARIANT LanguageID,  IDaCtlSpeechEngines * * SpeechEngines);
	STDMETHOD(get_RecognitionEngines)(IDaCtlRecognitionEngines * * RecognitionEngines);
	STDMETHOD(FindRecognitionEngines)(VARIANT LanguageID,  IDaCtlRecognitionEngines * * RecognitionEngines);
	STDMETHOD(GetCharacterRecognitionEngine)(VARIANT LoadKey,  IDaCtlRecognitionEngine * * RecognitionEngine);
	STDMETHOD(FindCharacterRecognitionEngines)(VARIANT LoadKey,  VARIANT LanguageID,  IDaCtlRecognitionEngines * * RecognitionEngines);

// Events
public:
	void FireActivateInput(LPCTSTR CharacterID);
	void FireDeactivateInput(LPCTSTR CharacterID);
	void FireClick(LPCTSTR CharacterID, short Button, short Shift, short x, short y);
	void FireDblClick(LPCTSTR CharacterID, short Button, short Shift, short x, short y);
	void FireDragStart(LPCTSTR CharacterID, short Button, short Shift, short x, short y);
	void FireDragComplete(LPCTSTR CharacterID, short Button, short Shift, short x, short y);
	void FireShow(LPCTSTR CharacterID, short Cause);
	void FireHide(LPCTSTR CharacterID, short Cause);
	void FireRequestStart(LPDISPATCH Request);
	void FireRequestComplete(LPDISPATCH Request);
	void FireRestart();
	void FireShutdown();
	void FireBookmark(long BookmarkID);
	void FireCommand(LPDISPATCH UserInput);
	void FireIdleStart(LPCTSTR CharacterID);
	void FireIdleComplete(LPCTSTR CharacterID);
	void FireMove(LPCTSTR CharacterID, short x, short y, short Cause);
	void FireSize(LPCTSTR CharacterID, short Width, short Height);
	void FireBalloonShow(LPCTSTR CharacterID);
	void FireBalloonHide(LPCTSTR CharacterID);
	void FireHelpComplete(LPCTSTR CharacterID, LPCTSTR Name, short Cause);
	void FireListenStart(LPCTSTR CharacterID);
	void FireListenComplete(LPCTSTR CharacterID, short Cause);
	void FireDefaultCharacterChange(LPCTSTR GUID);
	void FireAgentPropertyChange();
	void FireActiveClientChange(LPCTSTR CharacterID, BOOL Active);

// Attributes
public:
	IDaServer2Ptr			mServer;
	IDispatchPtr			mCharacters;
	IDispatchPtr			mAudioOutput;
	IDispatchPtr			mSpeechInput;
	IDispatchPtr			mCommandsWindow;
	IDispatchPtr			mPropertySheet;
	IDispatchPtr			mCharacterFiles;
	bool					mRaiseRequestErrors;

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

// Implementation
protected:
	CString GetControlCharacterID (long pServerCharID);
	CString GetActiveCharacterID ();
	class CDaCtlCharacter * GetActiveCharacter ();

public:
	void RequestCreated (CDaCtlRequest * pRequest);
	void RequestDeleted (CDaCtlRequest * pRequest);

protected:
	class ATL_NO_VTABLE CServerNotifySink :
		public CComObjectRootEx<CComSingleThreadModel>,
		public IDispatchImpl<IDaSvrNotifySink, &__uuidof(IDaSvrNotifySink), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MINOR>
	{
	public:
		CServerNotifySink ();
		~CServerNotifySink ();
		
		HRESULT Initialize (CDaControlObj * pOwner);
		HRESULT Terminate ();

		BEGIN_COM_MAP(CServerNotifySink)
			COM_INTERFACE_ENTRY(IDaSvrNotifySink)
			COM_INTERFACE_ENTRY2(IDispatch, IDaSvrNotifySink)
		END_COM_MAP()

		STDMETHOD(Command)(long dwCommandID, IUnknown *punkUserInput);
		STDMETHOD(ActivateInputState)(long dwCharID, long bActivated);
		STDMETHOD(Restart)(void);
		STDMETHOD(Shutdown)(void);
		STDMETHOD(VisibleState)(long dwCharID, long bVisible, long dwCause);
		STDMETHOD(Click)(long dwCharID, short fwKeys, long x, long y);
		STDMETHOD(DblClick)(long dwCharID, short fwKeys, long x, long y);
		STDMETHOD(DragStart)(long dwCharID, short fwKeys, long x, long y);
		STDMETHOD(DragComplete)(long dwCharID, short fwKeys, long x, long y);
		STDMETHOD(RequestStart)(long dwRequestID);
		STDMETHOD(RequestComplete)(long dwRequestID, long hrStatus);
		STDMETHOD(BookMark)(long dwBookMarkID);
		STDMETHOD(Idle)(long dwCharID, long bStart);
		STDMETHOD(Move)(long dwCharID, long x, long y, long dwCause);
		STDMETHOD(Size)(long dwCharID, long lWidth, long lHeight);
		STDMETHOD(BalloonVisibleState)(long dwCharID, long bVisible);
		STDMETHOD(HelpComplete)(long dwCharID, long dwCommandID, long dwCause);
		STDMETHOD(ListeningState)(long dwCharID, long bListening, long dwCause);
		STDMETHOD(DefaultCharacterChange)(BSTR bszGUID);
		STDMETHOD(AgentPropertyChange)();
		STDMETHOD(ActiveClientChange)(long dwCharID, long lStatus);

	public:
		CDaControlObj *	mOwner;
		long			mServerNotifyId;
	};

protected:
	tPtr <CComObject <CServerNotifySink> >	mServerNotifySink;
	CAtlMap <long, CDaCtlRequest *>			mActiveRequests;
	CPtrTypeArray <CDaCtlRequest>			mCompletedRequests;
	tPtr <CMsgPostingWnd <CDaControlObj> >	mMsgPostingWnd;
private:
	CIconHandle								mIcon;
	bool									mFinalReleased;
	static UINT								mCompleteRequestsMsg;
};

/////////////////////////////////////////////////////////////////////////////

#define	UpdateRegistry	UpdateRegistryOverride
OBJECT_ENTRY_AUTO(__uuidof(DaControl), CDaControlObj)
#undef	UpdateRegistry

/////////////////////////////////////////////////////////////////////////////
