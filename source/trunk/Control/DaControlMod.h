/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
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
#include "DaControlRes.h"
#include "DaGuid.h"
#include "DaControlOdl.h"
#ifndef	_DACORE_LOCAL
#include "DaServerOdl.h"
#endif
#include "DaError.h"
#include "DaVersion.h"
#include "AgentAnchor.h"
#include "ListeningState.h"
#include "ThemedModule.h"
#ifdef	_DACORE_LOCAL
#include "DaCoreAnchor.h"
#endif

#pragma warning (push)
#pragma warning (disable: 4250 4584)
/////////////////////////////////////////////////////////////////////////////

class CDaControlModule :
	public CAtlDllModuleT <CDaControlModule>,
	public CGlobalAnchor,		// For local characters only
	public CEventGlobal,		// For local characters only
	public CListeningGlobal,	// For local characters only
#ifdef	_DACORE_LOCAL
	public CDaCoreAnchor,
#endif
	public CThemedModule,
	public _IEventNotify
{
public:
	CDaControlModule ();
	virtual ~CDaControlModule ();

	DECLARE_LIBID(__uuidof(DoubleAgentCtl_TypeLib))
	DECLARE_REGISTRY_RESOURCEID(IDR_DACONTROLMOD)

	HRESULT RegisterServer (BOOL bRegTypeLib = FALSE, const CLSID* pCLSID = NULL);
	HRESULT UnregisterServer (BOOL bUnRegTypeLib, const CLSID* pCLSID = NULL);

// Attributes
#ifdef	_DEBUG
	CAtlPtrTypeArray <IUnknown>	mComObjects;
#endif

// Operations
public:
	void OnControlCreated (class DaControl * pControl);
	void OnControlDeleted (class DaControl * pControl);

#ifndef	_DACORE_LOCAL
	HRESULT PreServerCall (LPUNKNOWN pServerInterface);
	HRESULT PostServerCall (LPUNKNOWN pServerInterface);
#endif
	bool PreNotify ();
	void PostNotify ();

	bool VerifyAppActive () const;

// Overrides
public:
	virtual bool IsAppActive () const;
	virtual void _CharacterLoaded (long pCharID);
	virtual void _CharacterUnloaded (long pCharID);
	virtual void _CharacterNameChanged (long pCharID);
	virtual void _CharacterActivated (long pActiveCharID, long pInputActiveCharID, long pInactiveCharID, long pInputInactiveCharID);
	virtual void _AppActivated (bool pActive);
	virtual void _OptionsChanged ();

// Implementation
protected:
	void Terminate ();
	void _Terminate ();
	void DeleteAllControls ();
	bool NotifyMessageFilter ();
	bool FinalMessageFilter ();
	void EndMessageFilter (bool pFinal);

private:
	bool								mAppActive;
	CAtlOwnPtrArray <class DaControl>	mControls;
#ifndef	_DACORE_LOCAL
	int									mServerCallLevel;
#endif
	int									mNotifyLevel;
	tPtr <class CComMessageFilter>		mMessageFilter;
};

extern class CDaControlModule _AtlModule;

#import "mscorlib.tlb" rename("ReportEvent","CorReportEvent") no_smart_pointers raw_interfaces_only raw_method_prefix("cor_") raw_property_prefixes("corget_","corput_","corputref_")

/////////////////////////////////////////////////////////////////////////////
#pragma warning (pop)

_COM_SMARTPTR_TYPEDEF (IDaControl, __uuidof(IDaControl));
_COM_SMARTPTR_TYPEDEF (IDaControl2, __uuidof(IDaControl2));
_COM_SMARTPTR_TYPEDEF (IDaCtlCharacters, __uuidof(IDaCtlCharacters));
_COM_SMARTPTR_TYPEDEF (IDaCtlCharacters2, __uuidof(IDaCtlCharacters2));
_COM_SMARTPTR_TYPEDEF (IDaCtlCharacter, __uuidof(IDaCtlCharacter));
_COM_SMARTPTR_TYPEDEF (IDaCtlCharacter2, __uuidof(IDaCtlCharacter2));
_COM_SMARTPTR_TYPEDEF (IDaCtlBalloon, __uuidof(IDaCtlBalloon));
_COM_SMARTPTR_TYPEDEF (IDaCtlBalloon2, __uuidof(IDaCtlBalloon2));
_COM_SMARTPTR_TYPEDEF (IDaCtlCommands, __uuidof(IDaCtlCommands));
_COM_SMARTPTR_TYPEDEF (IDaCtlCommands2, __uuidof(IDaCtlCommands2));
_COM_SMARTPTR_TYPEDEF (IDaCtlCommand, __uuidof(IDaCtlCommand));
_COM_SMARTPTR_TYPEDEF (IDaCtlCommand2, __uuidof(IDaCtlCommand2));
_COM_SMARTPTR_TYPEDEF (IDaCtlRequest, __uuidof(IDaCtlRequest));
_COM_SMARTPTR_TYPEDEF (IDaCtlSettings, __uuidof(IDaCtlSettings));
_COM_SMARTPTR_TYPEDEF (IDaCtlAudioOutput, __uuidof(IDaCtlAudioOutput));
_COM_SMARTPTR_TYPEDEF (IDaCtlSpeechInput, __uuidof(IDaCtlSpeechInput));
_COM_SMARTPTR_TYPEDEF (IDaCtlPropertySheet, __uuidof(IDaCtlPropertySheet));
_COM_SMARTPTR_TYPEDEF (IDaCtlPropertySheet2, __uuidof(IDaCtlPropertySheet2));
_COM_SMARTPTR_TYPEDEF (IDaCtlUserInput, __uuidof(IDaCtlUserInput));
_COM_SMARTPTR_TYPEDEF (IDaCtlCommandsWindow, __uuidof(IDaCtlCommandsWindow));
_COM_SMARTPTR_TYPEDEF (IDaCtlAnimationNames, __uuidof(IDaCtlAnimationNames));

_COM_SMARTPTR_TYPEDEF (IDaCtlCharacterFiles, __uuidof(IDaCtlCharacterFiles));
_COM_SMARTPTR_TYPEDEF (IDaCtlFormattedText, __uuidof(IDaCtlFormattedText));
_COM_SMARTPTR_TYPEDEF (IDaCtlTTSEngine, __uuidof(IDaCtlTTSEngine));
_COM_SMARTPTR_TYPEDEF (IDaCtlTTSEngines, __uuidof(IDaCtlTTSEngines));
_COM_SMARTPTR_TYPEDEF (IDaCtlSREngine, __uuidof(IDaCtlSREngine));
_COM_SMARTPTR_TYPEDEF (IDaCtlSREngines, __uuidof(IDaCtlSREngines));

_COM_SMARTPTR_TYPEDEF (IDaServer, __uuidof(IDaServer));
_COM_SMARTPTR_TYPEDEF (IDaServer2, __uuidof(IDaServer2));
_COM_SMARTPTR_TYPEDEF (IDaSvrCharacter, __uuidof(IDaSvrCharacter));
_COM_SMARTPTR_TYPEDEF (IDaSvrCharacter2, __uuidof(IDaSvrCharacter2));
_COM_SMARTPTR_TYPEDEF (IDaSvrBalloon, __uuidof(IDaSvrBalloon));
_COM_SMARTPTR_TYPEDEF (IDaSvrBalloon2, __uuidof(IDaSvrBalloon2));
_COM_SMARTPTR_TYPEDEF (IDaSvrCommand, __uuidof(IDaSvrCommand));
_COM_SMARTPTR_TYPEDEF (IDaSvrCommand2, __uuidof(IDaSvrCommand2));
_COM_SMARTPTR_TYPEDEF (IDaSvrCommands, __uuidof(IDaSvrCommands));
_COM_SMARTPTR_TYPEDEF (IDaSvrCommands2, __uuidof(IDaSvrCommands2));
_COM_SMARTPTR_TYPEDEF (IDaSvrPropertySheet, __uuidof(IDaSvrPropertySheet));
_COM_SMARTPTR_TYPEDEF (IDaSvrPropertySheet2, __uuidof(IDaSvrPropertySheet2));
_COM_SMARTPTR_TYPEDEF (IDaSvrAudioOutput, __uuidof(IDaSvrAudioOutput));
_COM_SMARTPTR_TYPEDEF (IDaSvrSpeechInput, __uuidof(IDaSvrSpeechInput));
_COM_SMARTPTR_TYPEDEF (IDaSvrCommandsWindow, __uuidof(IDaSvrCommandsWindow));
_COM_SMARTPTR_TYPEDEF (IDaSvrCommandsWindow2, __uuidof(IDaSvrCommandsWindow2));
_COM_SMARTPTR_TYPEDEF (IDaSvrUserInput, __uuidof(IDaSvrUserInput));
_COM_SMARTPTR_TYPEDEF (IDaSvrUserInput2, __uuidof(IDaSvrUserInput2));
_COM_SMARTPTR_TYPEDEF (IDaSvrNotifySink, __uuidof(IDaSvrNotifySink));
_COM_SMARTPTR_TYPEDEF (IDaSvrNotifySink2, __uuidof(IDaSvrNotifySink2));

_COM_SMARTPTR_TYPEDEF (IDaSvrSettings, __uuidof(IDaSvrSettings));
_COM_SMARTPTR_TYPEDEF (IDaSvrCharacterFiles, __uuidof(IDaSvrCharacterFiles));
_COM_SMARTPTR_TYPEDEF (IDaSvrFormattedText, __uuidof(IDaSvrFormattedText));
_COM_SMARTPTR_TYPEDEF (IDaSvrTTSEngine, __uuidof(IDaSvrTTSEngine));
_COM_SMARTPTR_TYPEDEF (IDaSvrTTSEngines, __uuidof(IDaSvrTTSEngines));
_COM_SMARTPTR_TYPEDEF (IDaSvrSREngine, __uuidof(IDaSvrSREngine));
_COM_SMARTPTR_TYPEDEF (IDaSvrSREngines, __uuidof(IDaSvrSREngines));

/////////////////////////////////////////////////////////////////////////////

#ifndef	_DEBUG
#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE	LogVerbose
#endif
#ifndef	_LOG_RESULTS
#define	_LOG_RESULTS	LogNormal
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
