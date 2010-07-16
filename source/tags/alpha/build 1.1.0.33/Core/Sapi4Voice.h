/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "SapiVoice.h"
#include "MallocPtr.h"
#pragma warning (push)
#pragma warning (disable: 4005)
#include <speech.h>
#pragma warning (pop)

//////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF (ITTSCentral, IID_ITTSCentral);

//////////////////////////////////////////////////////////////////////
#pragma warning (push)
#pragma warning (disable: 4251 4275)
/////////////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP CSapi4Voice : public CSapiVoice
{
	DECLARE_DLL_OBJECT(CSapi4Voice)
protected:
	CSapi4Voice ();
public:
	virtual ~CSapi4Voice ();
	static CSapi4Voice * CreateInstance ();

// Attributes
	ULONG GetDefaultRate () const;
	USHORT GetDefaultVolume () const;
	USHORT GetDefaultPitch () const;

// Operations
	HRESULT PrepareToSpeak (bool pHighPriority = false);
	HRESULT Speak (LPCTSTR pMessage, bool pAsync = true);
	HRESULT Stop ();

	HRESULT GetEngineId (GUID & pEngineId);
	HRESULT GetModeId (GUID & pModeId);
	HRESULT SetModeId (const GUID & pModeId);

	friend int VoiceMouthOverlay (LPVOID pMouth);
	friend CAtlString VoiceMouthStr (LPVOID pMouth);

// Overrides
protected:
	virtual UINT _IsValid () const;
	virtual bool _IsPrepared () const;
	virtual bool _IsSpeaking () const;
public:
	virtual tBstrPtr GetUniqueId ();
	virtual HRESULT GetUniqueId (tBstrPtr & pUniqueId);
	virtual tBstrPtr GetDisplayName ();
	virtual HRESULT GetDisplayName (tBstrPtr & pDisplayName);
	virtual ULONG GetRate ();
	virtual HRESULT GetRate (ULONG & pRate);
	virtual HRESULT SetRate (ULONG pRate);
	virtual USHORT GetVolume ();
	virtual HRESULT GetVolume (USHORT & pVolume);
	virtual HRESULT SetVolume (USHORT pVolume);
	virtual USHORT GetPitch ();
	virtual HRESULT GetPitch (USHORT & pPitch);
	virtual HRESULT SetPitch (USHORT pPitch);

// Implementation
private:
	class ATL_NO_VTABLE CTTSNotifyBase : public  CComObjectRoot, public ITTSNotifySink2
	{
	public:
		CTTSNotifyBase () {}
		~CTTSNotifyBase () {}

		BEGIN_COM_MAP(CTTSNotifyBase)
			COM_INTERFACE_ENTRY_IID(IID_ITTSNotifySink, ITTSNotifySink2)
			COM_INTERFACE_ENTRY_IID(IID_ITTSNotifySink2, ITTSNotifySink2)
		END_COM_MAP()
	};

	class CTTSNotifySink : public CComObjectNoLock<CTTSNotifyBase>
	{
	public:
		CTTSNotifySink (CSapi4Voice & pOwner);
		~CTTSNotifySink ();

	public:
		bool SafeIsConnected () const;
		HRESULT Connect ();
		HRESULT Disconnect ();

	public:
		// ITTSNotifySink2
		HRESULT STDMETHODCALLTYPE AttribChanged (DWORD dwAttribute);
		HRESULT STDMETHODCALLTYPE AudioStart (QWORD qTimeStamp);
		HRESULT STDMETHODCALLTYPE AudioStop (QWORD qTimeStamp);
		HRESULT STDMETHODCALLTYPE Visual (QWORD qTimeStamp, WCHAR cIPAPhoneme, WCHAR cEnginePhoneme, DWORD dwHints, PTTSMOUTH pTTSMouth);
		HRESULT STDMETHODCALLTYPE Error (LPUNKNOWN pError);
		HRESULT STDMETHODCALLTYPE Warning (LPUNKNOWN pWarning);
		HRESULT STDMETHODCALLTYPE VisualFuture (DWORD dwMilliseconds, QWORD qTimeStamp, WCHAR cIPAPhoneme, WCHAR cEnginePhoneme, DWORD dwHints, PTTSMOUTH pTTSMouth);

	protected:
		CSapi4Voice &	mOwner;
		DWORD			mRegisteredKey;
	};
	friend class CTTSNotifySink;

	class ATL_NO_VTABLE CTTSBufNotifyBase : public  CComObjectRoot, public ITTSBufNotifySink
	{
	public:
		CTTSBufNotifyBase () {}
		~CTTSBufNotifyBase () {}

		BEGIN_COM_MAP(CTTSBufNotifyBase)
			COM_INTERFACE_ENTRY_IID(IID_ITTSBufNotifySink, ITTSBufNotifySink)
		END_COM_MAP()
	};

	class CTTSBufNotifySink : public CComObjectNoLock<CTTSBufNotifyBase>
	{
	public:
		CTTSBufNotifySink (CSapi4Voice & pOwner);
		~CTTSBufNotifySink ();

	public:
		// ITTSBufNotifySink
		HRESULT STDMETHODCALLTYPE TextDataDone (QWORD qTimeStamp, DWORD dwFlags);
		HRESULT STDMETHODCALLTYPE TextDataStarted (QWORD qTimeStamp);
		HRESULT STDMETHODCALLTYPE BookMark (QWORD qTimeStamp, DWORD dwMarkNum);
		HRESULT STDMETHODCALLTYPE WordPosition (QWORD qTimeStamp, DWORD dwByteOffset);

	protected:
		CSapi4Voice &	mOwner;
	};
	friend class CTTSBufNotifySink;

protected:
	bool CheckIsQueueing () const;
	bool CheckIsSpeaking () const;
	bool CheckIsResetting () const;
	void SetIsQueueing (bool pIsQueueing);
	void SetIsSpeaking (bool pIsSpeaking);
	void SetIsResetting (bool pIsResetting);

protected:
	ITTSCentralPtr				mEngine;
	ULONG						mDefaultRate;
	USHORT						mDefaultVolume;
	USHORT						mDefaultPitch;
	tPtr <DWORD>				mIsQueueing;
	tPtr <DWORD>				mIsSpeaking;
	tPtr <DWORD>				mResetPending;
	tMallocPtr <WCHAR>			mLastText;
	tPtr <CTTSNotifySink>		mNotifySink;
	tPtr <CTTSBufNotifySink>	mBufNotifySink;
};

#pragma warning (pop)
//////////////////////////////////////////////////////////////////////
