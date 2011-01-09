/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
#include "DaCmnFormattedText.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E570-A208-11DE-ABF2-002421116FB2}")) DaSvrFormattedText :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<DaSvrFormattedText, &__uuidof(DaSvrFormattedText)>,
	public IDispatchImpl<IDaSvrFormattedText, &__uuidof(IDaSvrFormattedText), &__uuidof(DoubleAgentSvr_TypeLib), DoubleAgentSvr_MajorVer, DoubleAgentSvr_MinorVer>,
	public IProvideClassInfoImpl<&__uuidof(DaSvrFormattedText), &__uuidof(DoubleAgentSvr_TypeLib), DoubleAgentSvr_MajorVer, DoubleAgentSvr_MinorVer>,
	public ISupportErrorInfo,
	public CDaCmnFormattedText,
	public CSvrObjLifetime
{
public:
	DaSvrFormattedText ();
	virtual ~DaSvrFormattedText ();

// Attributes
public:

// Operations
public:
	void Terminate (bool pFinal, bool pAbandonned = false);
	void FinalRelease ();

// Overrides
public:
	virtual void OnClientEnded ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DASVRFORMATTEDTEXT)
	DECLARE_NOT_AGGREGATABLE(DaSvrFormattedText)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaSvrFormattedText)
		COM_INTERFACE_ENTRY(IDaSvrFormattedText)
		COM_INTERFACE_ENTRY2(IDispatch, IDaSvrFormattedText)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaSvrFormattedText)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaSvrFormattedText
	HRESULT STDMETHODCALLTYPE get_RawText (BSTR *RawText);
	HRESULT STDMETHODCALLTYPE put_RawText (BSTR RawText);
	HRESULT STDMETHODCALLTYPE get_ParsedText(BSTR *ParsedText);
	HRESULT STDMETHODCALLTYPE get_SpeechText(BSTR *SpeechText);
	HRESULT STDMETHODCALLTYPE get_WordCount (long *WordCount);
	HRESULT STDMETHODCALLTYPE get_DisplayWord (long WordIndex, BSTR *DisplayWord);
	HRESULT STDMETHODCALLTYPE get_WordIndex (long *WordIndex);
	HRESULT STDMETHODCALLTYPE get_DisplayText (BSTR *DisplayText);
	HRESULT STDMETHODCALLTYPE Parse (BSTR RawText);
	HRESULT STDMETHODCALLTYPE Append (IDaSvrFormattedText *FormattedText);
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaSvrFormattedText), DaSvrFormattedText)

/////////////////////////////////////////////////////////////////////////////
