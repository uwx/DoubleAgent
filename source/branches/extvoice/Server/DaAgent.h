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
#ifndef	DAAGENT_H_INCLUDED_
#define DAAGENT_H_INCLUDED_
#pragma once

#include "DaAgentNotify.h"
#include "DaServerLifetime.h"
#include "OleObjectFactoryExEx.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E500-A208-11DE-ABF2-002421116FB2}")) CDaAgent : public CCmdTarget, public CDaObjectLifetime, protected IDaInternalNotify
{
	DECLARE_DYNCREATE(CDaAgent)
	DECLARE_OLECREATE_EX(CDaAgent)
	DECLARE_OLETYPELIB(CDaAgent)
protected:
	CDaAgent();
	virtual ~CDaAgent();
	void Terminate (bool pFinal, bool pAbandonned = false);

// Attributes
public:
	CDaAgentNotify	mNotify;

// Operations
public:

// Overrides
	//{{AFX_VIRTUAL(CDaAgent)
	public:
	virtual void OnFinalRelease();
	virtual void OnClientEnded ();
	virtual bool _OnDownloadComplete (CFileDownload * pDownload);
	protected:
	virtual LPUNKNOWN GetInterfaceHook(const void* iid);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CDaAgent)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_DISPATCH(CDaAgent)
	afx_msg void DspLoad(const VARIANT & LoadKey, long * CharID, long * ReqID);
	afx_msg void DspUnload(long CharID);
	afx_msg void DspRegister(LPUNKNOWN NotifySink, long * SinkID);
	afx_msg void DspUnregister(long SinkID);
	afx_msg void DspGetCharacter(long CharID, LPDISPATCH * Character);
	afx_msg void DspGetSuspended(long * Suspended);
	afx_msg void DspGetCharacterEx(long CharID, LPDISPATCH * Character);
	afx_msg void DspGetVersion(short * Major, short * Minor);
	afx_msg void DspShowDefaultCharacterProperties(short x, short y, long UseDefaultPosition);
	afx_msg LPDISPATCH DspGetCharacter2(long CharacterID);
	afx_msg LPDISPATCH DspGetSpeechEngines();
	afx_msg LPDISPATCH DspFindSpeechEngines(long LanguageID, short Gender);
	afx_msg LPDISPATCH DspGetCharacterSpeechEngine(VARIANT LoadKey);
	afx_msg LPDISPATCH DspFindCharacterSpeechEngines(VARIANT LoadKey, long LanguageID = 0);
	afx_msg LPDISPATCH DspGetRecognitionEngines();
	afx_msg LPDISPATCH DspFindRecognitionEngines(long LanguageID);
	afx_msg LPDISPATCH DspGetCharacterRecognitionEngine(VARIANT LoadKey);
	afx_msg LPDISPATCH DspFindCharacterRecognitionEngines(VARIANT LoadKey, long LanguageID = 0);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_DISPATCH_IID()

	BEGIN_INTERFACE_PART(Server2, IDaServer2)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

		HRESULT STDMETHODCALLTYPE Load (VARIANT vLoadKey, long * pdwCharID, long * pdwReqID);
		HRESULT STDMETHODCALLTYPE Unload (long dwCharID);
		HRESULT STDMETHODCALLTYPE Register (IUnknown * punkNotifySink, long * pdwSinkID);
		HRESULT STDMETHODCALLTYPE Unregister (long dwSinkID);
		HRESULT STDMETHODCALLTYPE GetCharacter (long dwCharID, IDispatch ** ppunkCharacter);
		HRESULT STDMETHODCALLTYPE GetSuspended (long * pbSuspended);

		HRESULT STDMETHODCALLTYPE GetCharacterEx (long dwCharID, IDaSvrCharacter **ppCharacterEx);
		HRESULT STDMETHODCALLTYPE GetVersion (short *psMajor, short *psMinor);
		HRESULT STDMETHODCALLTYPE ShowDefaultCharacterProperties (short x, short y, long bUseDefaultPosition);

		HRESULT STDMETHODCALLTYPE GetCharacter2 (long CharacterID, IDaSvrCharacter2 **Character2);
		HRESULT STDMETHODCALLTYPE GetSpeechEngines (IDaSvrSpeechEngines **SpeechEngines);
		HRESULT STDMETHODCALLTYPE FindSpeechEngines (long LanguageID, short Gender, IDaSvrSpeechEngines **SpeechEngines);
		HRESULT STDMETHODCALLTYPE GetCharacterSpeechEngine (VARIANT LoadKey, IDaSvrSpeechEngine **SpeechEngine);
		HRESULT STDMETHODCALLTYPE FindCharacterSpeechEngines (VARIANT LoadKey, long LanguageID, IDaSvrSpeechEngines **SpeechEngines);
		HRESULT STDMETHODCALLTYPE GetRecognitionEngines (IDaSvrRecognitionEngines **RecognitionEngines);
		HRESULT STDMETHODCALLTYPE FindRecognitionEngines (long LanguageID, IDaSvrRecognitionEngines **RecognitionEngines);
		HRESULT STDMETHODCALLTYPE GetCharacterRecognitionEngine (VARIANT LoadKey, IDaSvrRecognitionEngine **RecognitionEngine);
		HRESULT STDMETHODCALLTYPE FindCharacterRecognitionEngines (VARIANT LoadKey, long LanguageID, IDaSvrRecognitionEngines **RecognitionEngines);
	END_INTERFACE_PART(Server2)

	BEGIN_INTERFACE_PART(StdMarshalInfo, IStdMarshalInfo)
		HRESULT STDMETHODCALLTYPE GetClassForHandler (DWORD dwDestContext, void *pvDestContext, CLSID *pClsid);
	END_INTERFACE_PART(StdMarshalInfo)

	DECLARE_PROVIDECLASSINFO2()
	DECLARE_SUPPORTERRORINFO()
	DECLARE_INTERFACE_MAP()

public:
	static COleObjectFactoryEx & GetClassFactory() {return factory;}

	bool PreNotify ();
	bool PostNotify ();
	UINT IsInNotify () const;

protected:
	void UnloadAllCharacters (bool pAbandonned = false);
	HRESULT GetLoadPath (VARIANT pLoadKey, CString & pFilePath);
	HRESULT LoadCharacter (LPCTSTR pFilePath, long & pCharID, long & pReqID);
	HRESULT UnloadCharacter (long pCharID);

protected:
	COwnPtrMap <long, CFileDownload>	mCharactersLoading;
private:
	UINT								mUsingHandler;
	UINT								mInNotify;
	CArrayEx <long>						mInNotifyUnregister;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DAAGENT_H_INCLUDED_
