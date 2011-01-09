/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
_COM_SMARTPTR_TYPEDEF (IAudio, IID_IAudio);
_COM_SMARTPTR_TYPEDEF (IAudioDest, IID_IAudioDest);

//////////////////////////////////////////////////////////////////////
#pragma warning (push)
#pragma warning (disable: 4251)
/////////////////////////////////////////////////////////////////////////////

class CSapi4Voice : public CSapiVoice
{
	DECLARE_DLL_OBJECT_EX(CSapi4Voice, _DACORE_IMPEXP )
protected:
	CSapi4Voice ();
public:
	_DACORE_IMPEXP virtual ~CSapi4Voice ();
	_DACORE_IMPEXP static CSapi4Voice * CreateInstance ();

// Attributes
	_DACORE_IMPEXP ULONG GetDefaultRate () const;
	_DACORE_IMPEXP USHORT GetDefaultVolume () const;
	_DACORE_IMPEXP USHORT GetDefaultPitch () const;

// Operations
	_DACORE_IMPEXP HRESULT PrepareToSpeak (bool pHighPriority = false);
	_DACORE_IMPEXP HRESULT Speak (LPCTSTR pMessage, bool pAsync = true);
	_DACORE_IMPEXP HRESULT Stop ();
	_DACORE_IMPEXP HRESULT Pause ();
	_DACORE_IMPEXP HRESULT Resume ();

	_DACORE_IMPEXP HRESULT GetEngineId (GUID & pEngineId);
	_DACORE_IMPEXP HRESULT GetModeId (GUID & pModeId);
	_DACORE_IMPEXP HRESULT SetModeId (const GUID & pModeId);

	_DACORE_IMPEXP friend int VoiceMouthOverlay (LPVOID pMouth);
	_DACORE_IMPEXP friend CAtlString VoiceMouthStr (LPVOID pMouth);

// Overrides
protected:
	_DACORE_IMPEXP virtual UINT _IsValid () const;
	_DACORE_IMPEXP virtual bool _IsPrepared () const;
	_DACORE_IMPEXP virtual bool _IsSpeaking () const;
	_DACORE_IMPEXP virtual bool _IsPaused () const;
public:
	_DACORE_IMPEXP virtual tBstrPtr GetUniqueId ();
	_DACORE_IMPEXP virtual HRESULT GetUniqueId (tBstrPtr & pUniqueId);
	_DACORE_IMPEXP virtual tBstrPtr GetDisplayName ();
	_DACORE_IMPEXP virtual HRESULT GetDisplayName (tBstrPtr & pDisplayName);
	_DACORE_IMPEXP virtual ULONG GetRate ();
	_DACORE_IMPEXP virtual HRESULT GetRate (ULONG & pRate);
	_DACORE_IMPEXP virtual HRESULT SetRate (ULONG pRate);
	_DACORE_IMPEXP virtual USHORT GetVolume ();
	_DACORE_IMPEXP virtual HRESULT GetVolume (USHORT & pVolume);
	_DACORE_IMPEXP virtual HRESULT SetVolume (USHORT pVolume);
	_DACORE_IMPEXP virtual USHORT GetPitch ();
	_DACORE_IMPEXP virtual HRESULT GetPitch (USHORT & pPitch);
	_DACORE_IMPEXP virtual HRESULT SetPitch (USHORT pPitch);

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

	void LogTtsEngine (UINT pLogLevel, ITTSCentral * pTtsEngine, LPCTSTR pFormat = NULL, ...);
	void LogTtsAudio (UINT pLogLevel, IAudioDest * pTtsAudio, LPCTSTR pFormat = NULL, ...);

protected:
	ITTSCentralPtr				mEngine;
	IAudioDestPtr				mAudioDest;
	ULONG						mDefaultRate;
	USHORT						mDefaultVolume;
	USHORT						mDefaultPitch;
	bool						mPaused;
	tPtr <DWORD>				mIsQueueing;
	tPtr <DWORD>				mIsSpeaking;
	tPtr <DWORD>				mResetPending;
	tMallocPtr <WCHAR>			mLastText;
	tPtr <CTTSNotifySink>		mNotifySink;
	tPtr <CTTSBufNotifySink>	mBufNotifySink;
};

#pragma warning (pop)
//////////////////////////////////////////////////////////////////////
