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
#include "DaControlMod.h"
#include "DaControl.h"
#include "DaCmnFormattedText.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E580-A208-11DE-ABF2-002421116FB2}")) DaCtlFormattedText :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<DaCtlFormattedText, &__uuidof(DaCtlFormattedText)>,
	public IDispatchImpl<IDaCtlFormattedText, &__uuidof(IDaCtlFormattedText), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public IProvideClassInfoImpl<&__uuidof(DaCtlFormattedText), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public ISupportErrorInfo
{
public:
	DaCtlFormattedText();
	~DaCtlFormattedText();

// Attributes
public:
#ifndef	_DACORE_LOCAL
	IDaSvrFormattedTextPtr		mServerObject;
#endif
	tPtr <CDaCmnFormattedText>	mLocalObject;

// Operations
public:
	void FinalRelease ();
	void Terminate (bool pFinal);

	HRESULT SetOwner (DaControl * pOwner);
	DaControl * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLFORMATTEDTEXT)
	DECLARE_NOT_AGGREGATABLE(DaCtlFormattedText)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaCtlFormattedText)
		COM_INTERFACE_ENTRY(IDaCtlFormattedText)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlFormattedText)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaCtlFormattedText)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaCtlFormattedText
	HRESULT STDMETHODCALLTYPE get_RawText (BSTR *RawText);
	HRESULT STDMETHODCALLTYPE put_RawText (BSTR RawText);
	HRESULT STDMETHODCALLTYPE get_ParsedText(BSTR *ParsedText);
	HRESULT STDMETHODCALLTYPE get_SpeechText(BSTR *SpeechText);
	HRESULT STDMETHODCALLTYPE get_WordCount (long *WordCount);
	HRESULT STDMETHODCALLTYPE get_DisplayWord (long WordIndex, BSTR *DisplayWord);
	HRESULT STDMETHODCALLTYPE get_WordIndex (long *WordIndex);
	HRESULT STDMETHODCALLTYPE get_DisplayText (BSTR *DisplayText);
	HRESULT STDMETHODCALLTYPE Parse (BSTR RawText);
	HRESULT STDMETHODCALLTYPE Append (IDaCtlFormattedText *FormattedText);

// Implementation
private:
	DaControl *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlFormattedText), DaCtlFormattedText)

/////////////////////////////////////////////////////////////////////////////
