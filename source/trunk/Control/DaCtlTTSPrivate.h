/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
#include "DaCmnTTSPrivate.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E584-A208-11DE-ABF2-002421116FB2}")) DaCtlTTSPrivate :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<DaCtlTTSPrivate, &__uuidof(DaCtlTTSPrivate)>,
	public IDispatchImpl<IDaCtlTTSPrivate, &__uuidof(IDaCtlTTSPrivate), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public IProvideClassInfoImpl<&__uuidof(DaCtlTTSPrivate), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public ISupportErrorInfo
{
public:
	DaCtlTTSPrivate();
	~DaCtlTTSPrivate();

#ifdef	_DEBUG_NOT
	STDMETHOD(GetClassInfo)(ITypeInfo** pptinfo)
	{
		return LogComErrAnon (LogAlways, IProvideClassInfoImpl<&__uuidof(DaCtlTTSPrivate), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>::GetClassInfo(pptinfo), _T("DaCtlTTSPrivate::GetClassInfo"));
	}
#endif

// Attributes
public:
#ifndef	_DACORE_LOCAL
	IDaSvrTTSPrivatePtr		mServerObject;
#endif
	tPtr <CDaCmnTTSPrivate>	mLocalObject;

// Operations
public:
	void FinalRelease ();
	void Terminate (bool pFinal);

	HRESULT SetOwner (DaControl * pOwner);
	DaControl * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLTTSPRIVATE)
	DECLARE_NOT_AGGREGATABLE(DaCtlTTSPrivate)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaCtlTTSPrivate)
		COM_INTERFACE_ENTRY(IDaCtlTTSPrivate)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlTTSPrivate)
		COM_INTERFACE_ENTRY2(IDaCtlTTSEngine, IDaCtlTTSPrivate)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaCtlTTSPrivate)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaCtlTTSEngine
	HRESULT STDMETHODCALLTYPE get_TTSModeID (BSTR * TTSModeID);
	HRESULT STDMETHODCALLTYPE get_DisplayName (BSTR * DisplayName);
	HRESULT STDMETHODCALLTYPE get_Manufacturer (BSTR * Manufacturer);
	HRESULT STDMETHODCALLTYPE GetVersion (short * MajorVersion,  short * MinorVersion);
	HRESULT STDMETHODCALLTYPE get_Gender (SpeechGenderType * Gender);
	HRESULT STDMETHODCALLTYPE get_LanguageID (long* LanguageID);
	HRESULT STDMETHODCALLTYPE get_LanguageName (VARIANT_BOOL EnglishName,  BSTR * LanguageName);

	// IDaCtlTTSPrivate
	HRESULT STDMETHODCALLTYPE get_InitDisplayName (BSTR * DisplayName);
    HRESULT STDMETHODCALLTYPE put_InitDisplayName (BSTR DisplayName);
	HRESULT STDMETHODCALLTYPE get_InitManufacturer (BSTR * Manufacturer);
    HRESULT STDMETHODCALLTYPE put_InitManufacturer (BSTR Manufacturer);
	HRESULT STDMETHODCALLTYPE get_InitVersion (BSTR * Version);
    HRESULT STDMETHODCALLTYPE put_InitVersion (BSTR Version);
	HRESULT STDMETHODCALLTYPE get_InitGender (SpeechGenderType * Gender);
    HRESULT STDMETHODCALLTYPE put_InitGender (SpeechGenderType Gender);
	HRESULT STDMETHODCALLTYPE get_InitLanguageID (long* LanguageID);
    HRESULT STDMETHODCALLTYPE put_InitLanguageID (long LanguageID);
    HRESULT STDMETHODCALLTYPE get_InitString (BSTR ValuePath, BSTR *StringValue);
    HRESULT STDMETHODCALLTYPE put_InitString (BSTR ValuePath, BSTR StringValue);
    HRESULT STDMETHODCALLTYPE get_InitAttribute (BSTR AttributeName, BSTR *AttributeValue);
    HRESULT STDMETHODCALLTYPE put_InitAttribute (BSTR AttributeName, BSTR AttributeValue);
	HRESULT STDMETHODCALLTYPE get_InitFilePath (BSTR FileId, BSTR *FilePath);
    HRESULT STDMETHODCALLTYPE put_InitFilePath (BSTR FileId, BSTR FilePath);

// Implementation
private:
	DaControl *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlTTSPrivate), DaCtlTTSPrivate)

/////////////////////////////////////////////////////////////////////////////
