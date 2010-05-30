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
#pragma once
#include "DaServerApp.h"
#include "AgentFileCache.h"
#include "DaCmnBalloon.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E512-A208-11DE-ABF2-002421116FB2}")) DaSvrBalloon :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<DaSvrBalloon, &__uuidof(DaSvrBalloon)>,
	public IDispatchImpl<IDaSvrBalloon2, &__uuidof(IDaSvrBalloon2), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MINOR>,
	public IProvideClassInfoImpl<&__uuidof(DaSvrBalloon), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MAJOR>,
	public ISupportErrorInfo,
	public CAgentFileClient,
	public CDaCmnBalloon,
	public CSvrObjLifetime
{
public:
	DaSvrBalloon ();
	virtual ~DaSvrBalloon ();

// Attributes
public:
	long GetCharID () const;
	LANGID GetLangID () const;

// Operations
public:
	static DaSvrBalloon * CreateInstance (long pCharID, CAgentFile * pFile, class CAgentPopupWnd * pOwner, LPCTSTR pClientMutexName = NULL);
	void Terminate (bool pFinal, bool pAbandonned = false);
	void FinalRelease ();

// Overrides
public:
	virtual void OnClientEnded ();

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DASVRBALLOON)
	DECLARE_NOT_AGGREGATABLE(DaSvrBalloon)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaSvrBalloon)
		COM_INTERFACE_ENTRY(IDaSvrBalloon2)
		COM_INTERFACE_ENTRY2(IDispatch, IDaSvrBalloon2)
		COM_INTERFACE_ENTRY2(IDaSvrBalloon, IDaSvrBalloon2)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentBalloon), IDaSvrBalloon2)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentBalloonEx), IDaSvrBalloon2)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()

	BEGIN_CATEGORY_MAP(DaSvrBalloon)
	   IMPLEMENTED_CATEGORY(__uuidof(DaServer))
	   IMPLEMENTED_CATEGORY(CATID_Programmable)
	END_CATEGORY_MAP()

// Interfaces
public:
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// IDaSvrBalloon2
	HRESULT STDMETHODCALLTYPE GetEnabled (long *Enabled);
	HRESULT STDMETHODCALLTYPE GetNumLines (long *Lines);
	HRESULT STDMETHODCALLTYPE GetNumCharsPerLine (long *CharsPerLine);
	HRESULT STDMETHODCALLTYPE GetFontName (BSTR *FontName);
	HRESULT STDMETHODCALLTYPE GetFontSize (long *FontSize);
	HRESULT STDMETHODCALLTYPE GetFontBold (long *FontBold);
	HRESULT STDMETHODCALLTYPE GetFontItalic (long *FontItalic);
	HRESULT STDMETHODCALLTYPE GetFontStrikethru (long *FontStrikethru);
	HRESULT STDMETHODCALLTYPE GetFontUnderline (long *FontUnderline);
	HRESULT STDMETHODCALLTYPE GetForeColor (long *ForeColor);
	HRESULT STDMETHODCALLTYPE GetBackColor (long *BackColor);
	HRESULT STDMETHODCALLTYPE GetBorderColor (long *BorderColor);
	HRESULT STDMETHODCALLTYPE SetVisible (long Visible);
	HRESULT STDMETHODCALLTYPE GetVisible (long *Visible);
	HRESULT STDMETHODCALLTYPE SetFontName (BSTR FontName);
	HRESULT STDMETHODCALLTYPE SetFontSize (long FontSize);
	HRESULT STDMETHODCALLTYPE SetFontCharSet (short FontCharSet);
	HRESULT STDMETHODCALLTYPE GetFontCharSet (short *FontCharSet);

	HRESULT STDMETHODCALLTYPE SetStyle (long Style);
	HRESULT STDMETHODCALLTYPE GetStyle (long *Style);
	HRESULT STDMETHODCALLTYPE SetNumLines (long Lines);
	HRESULT STDMETHODCALLTYPE SetNumCharsPerLine (long CharsPerLine);

	HRESULT STDMETHODCALLTYPE get_Enabled (VARIANT_BOOL *Enabled);
	HRESULT STDMETHODCALLTYPE get_Style (long *Style);
	HRESULT STDMETHODCALLTYPE put_Style (long Style);
	HRESULT STDMETHODCALLTYPE get_Visible (VARIANT_BOOL *Visible);
	HRESULT STDMETHODCALLTYPE put_Visible (VARIANT_BOOL Visible);
	HRESULT STDMETHODCALLTYPE get_NumberOfLines (long *NumberOfLines);
	HRESULT STDMETHODCALLTYPE put_NumberOfLines (long NumberOfLines);
	HRESULT STDMETHODCALLTYPE get_CharsPerLine (long *CharsPerLine);
	HRESULT STDMETHODCALLTYPE put_CharsPerLine (long CharsPerLine);
	HRESULT STDMETHODCALLTYPE get_TextColor (long *TextColor);
	HRESULT STDMETHODCALLTYPE put_TextColor (long TextColor);
	HRESULT STDMETHODCALLTYPE get_BackColor (long *BackColor);
	HRESULT STDMETHODCALLTYPE put_BackColor (long BackColor);
	HRESULT STDMETHODCALLTYPE get_BorderColor (long *BorderColor);
	HRESULT STDMETHODCALLTYPE put_BorderColor (long BorderColor);
	HRESULT STDMETHODCALLTYPE get_FontName (BSTR *FontName);
	HRESULT STDMETHODCALLTYPE put_FontName (BSTR FontName);
	HRESULT STDMETHODCALLTYPE get_FontSize (long *FontSize);
	HRESULT STDMETHODCALLTYPE put_FontSize (long FontSize);
	HRESULT STDMETHODCALLTYPE get_FontBold (VARIANT_BOOL *FontBold);
	HRESULT STDMETHODCALLTYPE put_FontBold (VARIANT_BOOL FontBold);
	HRESULT STDMETHODCALLTYPE get_FontItalic (VARIANT_BOOL *FontItalic);
	HRESULT STDMETHODCALLTYPE put_FontItalic (VARIANT_BOOL FontItalic);
	HRESULT STDMETHODCALLTYPE get_FontStrikethru (VARIANT_BOOL *FontStrikethru);
	HRESULT STDMETHODCALLTYPE put_FontStrikethru (VARIANT_BOOL FontStrikethru);
	HRESULT STDMETHODCALLTYPE get_FontUnderline (VARIANT_BOOL *FontUnderline);
	HRESULT STDMETHODCALLTYPE put_FontUnderline (VARIANT_BOOL FontUnderline);
	HRESULT STDMETHODCALLTYPE get_FontCharSet (short *FontCharSet);
	HRESULT STDMETHODCALLTYPE put_FontCharSet (short FontCharSet);

// Implementation
protected:
	IUnknownPtr					mOwnerRefHolder;
	CDaBalloonConfig			mGlobalConfig;
	tPtr <CAgentFileBalloon>	mCustomConfig;
	tPtr <ULARGE_INTEGER>		mCustomStyle;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaSvrBalloon), DaSvrBalloon)

/////////////////////////////////////////////////////////////////////////////
