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
	public IDispatchImpl<IDaCtlBalloon2, &__uuidof(IDaCtlBalloon2), &__uuidof(DaControlTypeLib), _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
	public IProvideClassInfoImpl<&__uuidof(DaCtlBalloon), &__uuidof(DaControlTypeLib), _CONTROL_VER_MAJOR, _CONTROL_VER_MINOR>,
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
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IDaCtlBalloon2 Methods
	STDMETHOD(get_Enabled)(VARIANT_BOOL * Enabled);
	STDMETHOD(get_NumberOfLines)(long * NumberOfLines);
	STDMETHOD(get_CharsPerLine)(long * CharsPerLine);
	STDMETHOD(get_FontName)(BSTR * FontName);
	STDMETHOD(get_FontSize)(long * FontSize);
	STDMETHOD(get_FontBold)(VARIANT_BOOL * FontBold);
	STDMETHOD(get_FontItalic)(VARIANT_BOOL * FontItalic);
	STDMETHOD(get_FontStrikethru)(VARIANT_BOOL * FontStrikethru);
	STDMETHOD(get_FontUnderline)(VARIANT_BOOL * FontUnderline);
	STDMETHOD(get_ForeColor)(long * ForeColor);
	STDMETHOD(get_BackColor)(long * BackColor);
	STDMETHOD(get_BorderColor)(long * BorderColor);
	STDMETHOD(put_Visible)(VARIANT_BOOL Visible);
	STDMETHOD(get_Visible)(VARIANT_BOOL * Visible);
	STDMETHOD(put_FontName)(BSTR FontName);
	STDMETHOD(put_FontSize)(long FontSize);
	STDMETHOD(put_FontCharSet)(short FontCharSet);
	STDMETHOD(get_FontCharSet)(short * FontCharSet);
	STDMETHOD(put_Style)(long Style);
	STDMETHOD(get_Style)(long * Style);

	STDMETHOD(get__NumberOfLines)(long * NumberOfLines);
	STDMETHOD(get__CharsPerLine)(long * CharsPerLine);
	STDMETHOD(get__BackColor)(long *BackColor);
	STDMETHOD(get__BorderColor)(long *BorderColor);
	STDMETHOD(get__FontBold)(VARIANT_BOOL *FontBold);
	STDMETHOD(get__FontItalic)(VARIANT_BOOL *FontItalic);
	STDMETHOD(get__FontStrikethru)(VARIANT_BOOL *FontStrikethru);
	STDMETHOD(get__FontUnderline)(VARIANT_BOOL *FontUnderline);
	STDMETHOD(put_NumberOfLines)(long NumberOfLines);
	STDMETHOD(put_CharsPerLine)(long CharsPerLine);
	STDMETHOD(get_TextColor)(long *TextColor);
	STDMETHOD(put_TextColor)(long TextColor);
	STDMETHOD(put_BackColor)(long BackColor);
	STDMETHOD(put_BorderColor)(long BorderColor);
	STDMETHOD(put_FontBold)(VARIANT_BOOL FontBold);
	STDMETHOD(put_FontItalic)(VARIANT_BOOL FontItalic);
	STDMETHOD(put_FontStrikethru)(VARIANT_BOOL FontStrikethru);
	STDMETHOD(put_FontUnderline)(VARIANT_BOOL FontUnderline);

// Implementation
private:
	DaCtlCharacter *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaCtlBalloon), DaCtlBalloon)

/////////////////////////////////////////////////////////////////////////////
