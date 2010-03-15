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
#ifndef DACONTROL_H_INCLUDED_
#define DACONTROL_H_INCLUDED_
#pragma once

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "DaControlRes.h"
#include "DaControlOdl.h"
#include "DaServerOdl.h"
#include "DaError.h"
#include "InterfaceMap.h"

/////////////////////////////////////////////////////////////////////////////
#ifndef	_STRICT_COMPATIBILITY
#define	_STRICT_COMPATIBILITY
#endif
/////////////////////////////////////////////////////////////////////////////

class CDaControlApp : public COleControlModule
{
public:
	CDaControlApp ();
	virtual ~CDaControlApp ();
	DECLARE_DYNAMIC(CDaControlApp);

// Attributes
#ifdef	_DEBUG
	CPtrTypeArray <CCmdTarget>	mComObjects;
#endif

// Operations
public:
	void OnControlCreated (class CDaAgentCtl * pControl);
	void OnControlDeleted (class CDaAgentCtl * pControl);

	HRESULT PreServerCall (LPUNKNOWN pServerInterface);
	HRESULT PostServerCall (LPUNKNOWN pServerInterface);
	bool PreNotify ();
	void PostNotify ();

// Overrides
	//{{AFX_VIRTUAL(CDaControlApp)
	public:
	BOOL InitInstance();
	int ExitInstance();
	//}}AFX_VIRTUAL

protected:
	void DeleteAllControls ();
	void PendingMessageFilter ();
	void BusyMessageFilter ();
	void EndMessageFilter (bool pFinal);

private:
	COwnPtrArray <class CDaAgentCtl>	mControls;
	int									mServerCallLevel;
	int									mNotifyLevel;
	tPtr <class COleMessageFilterEx>	mMessageFilter;
};

/////////////////////////////////////////////////////////////////////////////

#define TheControlApp ((CDaControlApp *) AfxGetApp ())

extern const GUID gDaTypeLibId;
extern const WORD gDaTypeLibVerMajor;
extern const WORD gDaTypeLibVerMinor;

extern const GUID gDaMsTypeLibId;
extern const WORD gDaMsTypeLibVerMajor;
extern const WORD gDaMsTypeLibVerMinor;

/////////////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF (IDaControl, __uuidof(IDaControl));
_COM_SMARTPTR_TYPEDEF (IDaCtlCharacters, __uuidof(IDaCtlCharacters));
_COM_SMARTPTR_TYPEDEF (IDaCtlCharacter, __uuidof(IDaCtlCharacter));
_COM_SMARTPTR_TYPEDEF (IDaCtlBalloon, __uuidof(IDaCtlBalloon));
_COM_SMARTPTR_TYPEDEF (IDaCtlCommands, __uuidof(IDaCtlCommands));
_COM_SMARTPTR_TYPEDEF (IDaCtlCommand, __uuidof(IDaCtlCommand));
_COM_SMARTPTR_TYPEDEF (IDaCtlRequest, __uuidof(IDaCtlRequest));
_COM_SMARTPTR_TYPEDEF (IDaCtlAudioObject, __uuidof(IDaCtlAudioObject));
_COM_SMARTPTR_TYPEDEF (IDaCtlSpeechInput, __uuidof(IDaCtlSpeechInput));
_COM_SMARTPTR_TYPEDEF (IDaCtlPropertySheet, __uuidof(IDaCtlPropertySheet));
_COM_SMARTPTR_TYPEDEF (IDaCtlUserInput, __uuidof(IDaCtlUserInput));
_COM_SMARTPTR_TYPEDEF (IDaCtlCommandsWindow, __uuidof(IDaCtlCommandsWindow));
_COM_SMARTPTR_TYPEDEF (IDaCtlAnimationNames, __uuidof(IDaCtlAnimationNames));

_COM_SMARTPTR_TYPEDEF (IDaControl2, __uuidof(IDaControl2));
_COM_SMARTPTR_TYPEDEF (IDaCtlCharacter2, __uuidof(IDaCtlCharacter2));
_COM_SMARTPTR_TYPEDEF (IDaCtlCharacterFiles, __uuidof(IDaCtlCharacterFiles));
_COM_SMARTPTR_TYPEDEF (IDaCtlSpeechEngine, __uuidof(IDaCtlSpeechEngine));
_COM_SMARTPTR_TYPEDEF (IDaCtlSpeechEngines, __uuidof(IDaCtlSpeechEngines));
_COM_SMARTPTR_TYPEDEF (IDaCtlRecognitionEngine, __uuidof(IDaCtlRecognitionEngine));
_COM_SMARTPTR_TYPEDEF (IDaCtlRecognitionEngines, __uuidof(IDaCtlRecognitionEngines));

_COM_SMARTPTR_TYPEDEF (IDaServer, __uuidof(IDaServer));
_COM_SMARTPTR_TYPEDEF (IDaSvrCharacter, __uuidof(IDaSvrCharacter));
_COM_SMARTPTR_TYPEDEF (IDaSvrBalloon, __uuidof(IDaSvrBalloon));
_COM_SMARTPTR_TYPEDEF (IDaSvrCommand, __uuidof(IDaSvrCommand));
_COM_SMARTPTR_TYPEDEF (IDaSvrCommands, __uuidof(IDaSvrCommands));
_COM_SMARTPTR_TYPEDEF (IDaSvrCommands, __uuidof(IDaSvrCommands));
_COM_SMARTPTR_TYPEDEF (IDaSvrPropertySheet, __uuidof(IDaSvrPropertySheet));
_COM_SMARTPTR_TYPEDEF (IDaSvrAudioOutputProperties, __uuidof(IDaSvrAudioOutputProperties));
_COM_SMARTPTR_TYPEDEF (IDaSvrSpeechInputProperties, __uuidof(IDaSvrSpeechInputProperties));
_COM_SMARTPTR_TYPEDEF (IDaSvrCommandWindow, __uuidof(IDaSvrCommandWindow));
_COM_SMARTPTR_TYPEDEF (IDaSvrUserInput, __uuidof(IDaSvrUserInput));
_COM_SMARTPTR_TYPEDEF (IDaSvrNotifySink15, __uuidof(IDaSvrNotifySink15));
_COM_SMARTPTR_TYPEDEF (IDaSvrNotifySink, __uuidof(IDaSvrNotifySink));

_COM_SMARTPTR_TYPEDEF (IDaServer2, __uuidof(IDaServer2));
_COM_SMARTPTR_TYPEDEF (IDaSvrCharacter2, __uuidof(IDaSvrCharacter2));
_COM_SMARTPTR_TYPEDEF (IDaSvrCharacterFiles, __uuidof(IDaSvrCharacterFiles));
_COM_SMARTPTR_TYPEDEF (IDaSvrSpeechEngine, __uuidof(IDaSvrSpeechEngine));
_COM_SMARTPTR_TYPEDEF (IDaSvrSpeechEngines, __uuidof(IDaSvrSpeechEngines));
_COM_SMARTPTR_TYPEDEF (IDaSvrRecognitionEngine, __uuidof(IDaSvrRecognitionEngine));
_COM_SMARTPTR_TYPEDEF (IDaSvrRecognitionEngines, __uuidof(IDaSvrRecognitionEngines));

/////////////////////////////////////////////////////////////////////////////

#define IMPLEMENT_OLECREATE_UUID(class_name, external_name) \
	const TCHAR _szProgID_##class_name[] = _T(external_name); \
	const GUID class_name::guid = __uuidof(class_name); \
	class_name::class_name##Factory class_name::factory(__uuidof(class_name), RUNTIME_CLASS(class_name), FALSE, _szProgID_##class_name); \
	HRESULT class_name::GetClassID(LPCLSID pclsid) { *pclsid = guid; return NOERROR; }

#define IMPLEMENT_OLECREATE_UUID_TA(class_name, external_name, treatasclass) \
	const TCHAR _szProgID_##class_name[] = _T(external_name); \
	const GUID class_name::guid = __uuidof(class_name); \
	class_name::class_name##Factory class_name::factory(__uuidof(class_name), RUNTIME_CLASS(class_name), FALSE, _szProgID_##class_name); \
	HRESULT class_name::GetClassID(LPCLSID pclsid) { *pclsid = __uuidof(treatasclass); return NOERROR; }

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DACONTROL_H_INCLUDED_
