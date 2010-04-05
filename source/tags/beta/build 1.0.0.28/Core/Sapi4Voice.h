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
#ifndef _SAPI4VOICE_H_INCLUDED
#define _SAPI4VOICE_H_INCLUDED
#pragma once

#include "SapiVoice.h"
#include "MallocPtr.h"
#include <speech.h>

//////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF (ITTSCentral, IID_ITTSCentral);

//////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable: 4251 4275)

class _DACORE_IMPEXP CSapi4Voice : public CSapiVoice
{
protected:
	CSapi4Voice ();
public:
	virtual ~CSapi4Voice ();
	DECLARE_DYNCREATE (CSapi4Voice)

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
	friend CString VoiceMouthStr (LPVOID pMouth);

// Overrides
	//{{AFX_VIRTUAL(CSapi4Voice)
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
	//}}AFX_VIRTUAL

// Implementation
private:
	class CTTSNotifySink : public CCmdTarget
	{
	public:
		CTTSNotifySink (CSapi4Voice & pOwner);
		~CTTSNotifySink ();

	public:
		bool SafeIsConnected () const;
		HRESULT Connect ();
		HRESULT Disconnect ();

	public:
		BEGIN_INTERFACE_PART(NotifySink, ITTSNotifySink2)
			HRESULT STDMETHODCALLTYPE AttribChanged (DWORD dwAttribute);
			HRESULT STDMETHODCALLTYPE AudioStart (QWORD qTimeStamp);
			HRESULT STDMETHODCALLTYPE AudioStop (QWORD qTimeStamp);
			HRESULT STDMETHODCALLTYPE Visual (QWORD qTimeStamp, WCHAR cIPAPhoneme, WCHAR cEnginePhoneme, DWORD dwHints, PTTSMOUTH pTTSMouth);
			HRESULT STDMETHODCALLTYPE Error (LPUNKNOWN pError);
			HRESULT STDMETHODCALLTYPE Warning (LPUNKNOWN pWarning);
			HRESULT STDMETHODCALLTYPE VisualFuture (DWORD dwMilliseconds, QWORD qTimeStamp, WCHAR cIPAPhoneme, WCHAR cEnginePhoneme, DWORD dwHints, PTTSMOUTH pTTSMouth);
		END_INTERFACE_PART(NotifySink)

		BEGIN_INTERFACE_PART(BufNotifySink, ITTSBufNotifySink)
			HRESULT STDMETHODCALLTYPE TextDataDone (QWORD qTimeStamp, DWORD dwFlags);
			HRESULT STDMETHODCALLTYPE TextDataStarted (QWORD qTimeStamp);
			HRESULT STDMETHODCALLTYPE BookMark (QWORD qTimeStamp, DWORD dwMarkNum);
			HRESULT STDMETHODCALLTYPE WordPosition (QWORD qTimeStamp, DWORD dwByteOffset);
		END_INTERFACE_PART(BufNotifySink)

		DECLARE_INTERFACE_MAP()

	protected:
		CSapi4Voice &	mOwner;
		DWORD			mRegisteredKey;
	};
	friend class CTTSNotifySink;

protected:
	bool CheckIsQueueing () const;
	bool CheckIsSpeaking () const;
	bool CheckIsResetting () const;
	void SetIsQueueing (bool pIsQueueing);
	void SetIsSpeaking (bool pIsSpeaking);
	void SetIsResetting (bool pIsResetting);

protected:
	ITTSCentralPtr			mEngine;
	ULONG					mDefaultRate;
	USHORT					mDefaultVolume;
	USHORT					mDefaultPitch;
	tPtr <DWORD>			mIsQueueing;
	tPtr <DWORD>			mIsSpeaking;
	tPtr <DWORD>			mResetPending;
	tMallocPtr <WCHAR>		mLastText;
	tPtr <CTTSNotifySink>	mNotifySink;
};

#pragma warning(pop)
//////////////////////////////////////////////////////////////////////

#endif // _SAPI4VOICE_H_INCLUDED
