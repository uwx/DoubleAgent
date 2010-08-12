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
#include "DaControlMod.h"
#include "DaCtlCharacter.h"
#include "DaCmnBalloon.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E533-A208-11DE-ABF2-002421116FB2}")) DaCtlBalloon :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<DaCtlBalloon, &__uuidof(DaCtlBalloon)>,
	public IDispatchImpl<IDaCtlBalloon2, &__uuidof(IDaCtlBalloon2), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public IProvideClassInfoImpl<&__uuidof(DaCtlBalloon), &__uuidof(DoubleAgentCtl_TypeLib), DoubleAgentCtl_MajorVer, DoubleAgentCtl_MinorVer>,
	public ISupportErrorInfo
{
public:
	DaCtlBalloon();
	~DaCtlBalloon();

// Attributes
public:
	IDaSvrBalloon2Ptr		mServerObject;
	tPtr <CDaCmnBalloon>	mLocalObject;

// Operations
public:
	void FinalRelease ();
	void Terminate (bool pFinal);

	HRESULT SetOwner (DaCtlCharacter * pOwner);
	DaCtlCharacter * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DACTLBALLOON)
	DECLARE_NOT_AGGREGATABLE(DaCtlBalloon)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaCtlBalloon)
		COM_INTERFACE_ENTRY(IDaCtlBalloon2)
		COM_INTERFACE_ENTRY2(IDispatch, IDaCtlBalloon2)
		COM_INTERFACE_ENTRY2(IDaCtlBalloon, IDaCtlBalloon2)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCtlBalloon), IDaCtlBalloon2)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentCtlBalloonEx), IDaCtlBalloon2)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaCtlBalloon)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid);

	// IDaCtlBalloon2 Methods
	HRESULT STDMETHODCALLTYPE get_Enabled (VARIANT_BOOL * Enabled);
	HRESULT STDMETHODCALLTYPE get_NumberOfLines (long * NumberOfLines);
	HRESULT STDMETHODCALLTYPE get_CharsPerLine (long * CharsPerLine);
	HRESULT STDMETHODCALLTYPE get_FontName (BSTR * FontName);
	HRESULT STDMETHODCALLTYPE get_FontSize (long * FontSize);
	HRESULT STDMETHODCALLTYPE get_FontBold (VARIANT_BOOL * FontBold);
	HRESULT STDMETHODCALLTYPE get_FontItalic (VARIANT_BOOL * FontItalic);
	HRESULT STDMETHODCALLTYPE get_FontStrikethru (VARIANT_BOOL * FontStrikethru);
	HRESULT STDMETHODCALLTYPE get_FontUnderline (VARIANT_BOOL * FontUnderline);
	HRESULT STDMETHODCALLTYPE get_ForeColor (long * ForeColor);
	HRESULT STDMETHODCALLTYPE get_BackColor (OLE_COLOR * BackColor);
	HRESULT STDMETHODCALLTYPE get_BorderColor (OLE_COLOR * BorderColor);
	HRESULT STDMETHODCALLTYPE put_Visible (VARIANT_BOOL Visible);
	HRESULT STDMETHODCALLTYPE get_Visible (VARIANT_BOOL * Visible);
	HRESULT STDMETHODCALLTYPE put_FontName (BSTR FontName);
	HRESULT STDMETHODCALLTYPE put_FontSize (long FontSize);
	HRESULT STDMETHODCALLTYPE put_FontCharSet (short FontCharSet);
	HRESULT STDMETHODCALLTYPE get_FontCharSet (short * FontCharSet);
	HRESULT STDMETHODCALLTYPE put_Style (long Style);
	HRESULT STDMETHODCALLTYPE get_Style (long * Style);

	HRESULT STDMETHODCALLTYPE get__NumberOfLines (long * NumberOfLines);
	HRESULT STDMETHODCALLTYPE get__CharsPerLine (long * CharsPerLine);
	HRESULT STDMETHODCALLTYPE get__BackColor (long *BackColor);
	HRESULT STDMETHODCALLTYPE get__BorderColor (long *BorderColor);
	HRESULT STDMETHODCALLTYPE get__FontBold (VARIANT_BOOL *FontBold);
	HRESULT STDMETHODCALLTYPE get__FontItalic (VARIANT_BOOL *FontItalic);
	HRESULT STDMETHODCALLTYPE get__FontStrikethru (VARIANT_BOOL *FontStrikethru);
	HRESULT STDMETHODCALLTYPE get__FontUnderline (VARIANT_BOOL *FontUnderline);
	HRESULT STDMETHODCALLTYPE put_NumberOfLines (long NumberOfLines);
	HRESULT STDMETHODCALLTYPE put_CharsPerLine (long CharsPerLine);
	HRESULT STDMETHODCALLTYPE get_TextColor (OLE_COLOR *TextColor);
	HRESULT STDMETHODCALLTYPE put_TextColor (OLE_COLOR TextColor);
	HRESULT STDMETHODCALLTYPE put_BackColor (OLE_COLOR BackColor);
	HRESULT STDMETHODCALLTYPE put_BorderColor (OLE_COLOR BorderColor);
	HRESULT STDMETHODCALLTYPE put_FontBold (VARIANT_BOOL FontBold);
	HRESULT STDMETHODCALLTYPE put_FontItalic (VARIANT_BOOL FontItalic);
	HRESULT STDMETHODCALLTYPE put_FontStrikethru (VARIANT_BOOL FontStrikethru);
	HRESULT STDMETHODCALLTYPE put_FontUnderline (VARIANT_BOOL FontUnderline);
	HRESULT STDMETHODCALLTYPE get_SizeToText (VARIANT_BOOL *SizeToText);
	HRESULT STDMETHODCALLTYPE put_SizeToText (VARIANT_BOOL SizeToText);
	HRESULT STDMETHODCALLTYPE get_AutoHide (VARIANT_BOOL *AutoHide);
	HRESULT STDMETHODCALLTYPE put_AutoHide (VARIANT_BOOL AutoHide);
	HRESULT STDMETHODCALLTYPE get_AutoPace (VARIANT_BOOL *AutoPace);
	HRESULT STDMETHODCALLTYPE put_AutoPace (VARIANT_BOOL AutoPace);
	HRESULT STDMETHODCALLTYPE get_ShowPartialLines (VARIANT_BOOL *ShowPartialLines);
	HRESULT STDMETHODCALLTYPE put_ShowPartialLines (VARIANT_BOOL ShowPartialLines);

// Implementation
private:
	DaCtlCharacter *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlBalloon), DaCtlBalloon)

/////////////////////////////////////////////////////////////////////////////
