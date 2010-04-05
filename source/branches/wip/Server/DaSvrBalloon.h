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
#include "AgentFile.h"
#include "AgentFileCache.h"
#include "ServerNotify.h"
#include "DaGlobalConfig.h"

/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE __declspec(uuid("{1147E512-A208-11DE-ABF2-002421116FB2}")) DaSvrBalloon :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<DaSvrBalloon, &__uuidof(DaSvrBalloon)>,
	public IDispatchImpl<IDaSvrBalloon, &__uuidof(IDaSvrBalloon), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MINOR>,
	public IProvideClassInfoImpl<&__uuidof(DaSvrBalloon), &__uuidof(DaServerTypeLib), _SERVER_VER_MAJOR, _SERVER_VER_MAJOR>,
	public ISupportErrorInfo,
	public CAgentFileClient
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
	static DaSvrBalloon * CreateInstance (long pCharID, class CAgentPopupWnd * pOwner, CAgentFile * pFile, _IServerNotify * pNotify);
	void Terminate (bool pFinal, bool pAbandonned = false);
	void FinalRelease ();

	bool SetLangID (LANGID pLangID);

// Overrides

// Declarations
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_DASVRBALLOON)
	DECLARE_NOT_AGGREGATABLE(DaSvrBalloon)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(DaSvrBalloon)
		COM_INTERFACE_ENTRY(IDaSvrBalloon)
		COM_INTERFACE_ENTRY2(IDispatch, IDaSvrBalloon)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentBalloon), IDaSvrBalloon)
		COM_INTERFACE_ENTRY_IID(__uuidof(IAgentBalloonEx), IDaSvrBalloon)
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

	// IDaSvrBalloon
	HRESULT STDMETHODCALLTYPE GetEnabled (long *pbEnabled);
	HRESULT STDMETHODCALLTYPE GetNumLines (long *plLines);
	HRESULT STDMETHODCALLTYPE GetNumCharsPerLine (long *plCharsPerLine);
	HRESULT STDMETHODCALLTYPE GetFontName (BSTR *pbszFontName);
	HRESULT STDMETHODCALLTYPE GetFontSize (long *plFontSize);
	HRESULT STDMETHODCALLTYPE GetFontBold (long *pbFontBold);
	HRESULT STDMETHODCALLTYPE GetFontItalic (long *pbFontItalic);
	HRESULT STDMETHODCALLTYPE GetFontStrikethru (long *pbFontStrikethru);
	HRESULT STDMETHODCALLTYPE GetFontUnderline (long *pbFontUnderline);
	HRESULT STDMETHODCALLTYPE GetForeColor (long *plFGColor);
	HRESULT STDMETHODCALLTYPE GetBackColor (long *plBGColor);
	HRESULT STDMETHODCALLTYPE GetBorderColor (long *plBorderColor);
	HRESULT STDMETHODCALLTYPE SetVisible (long bVisible);
	HRESULT STDMETHODCALLTYPE GetVisible (long *pbVisible);
	HRESULT STDMETHODCALLTYPE SetFontName (BSTR bszFontName);
	HRESULT STDMETHODCALLTYPE SetFontSize (long lFontSize);
	HRESULT STDMETHODCALLTYPE SetFontCharSet (short sFontCharSet);
	HRESULT STDMETHODCALLTYPE GetFontCharSet (short *psFontCharSet);

	HRESULT STDMETHODCALLTYPE SetStyle (long lStyle);
	HRESULT STDMETHODCALLTYPE GetStyle (long *plStyle);
	HRESULT STDMETHODCALLTYPE SetNumLines (long lLines);
	HRESULT STDMETHODCALLTYPE SetNumCharsPerLine (long lCharsPerLine);

// Implementation
protected:
	class CAgentBalloonWnd * GetBalloonWnd (long pCharID = 0);
	class CAgentPopupWnd * GetOwnerWnd ();
	CAgentFileBalloon * GetCustomConfig (bool pCreate = false);

protected:
	long						mCharID;
	LANGID						mLangID;
	class CAgentPopupWnd *		mOwner;
	IUnknownPtr					mOwnerRefHolder;
	CAgentFile *				mFile;
	_IServerNotify *			mNotify;
	CDaBalloonConfig			mGlobalConfig;
	tPtr <CAgentFileBalloon>	mCustomConfig;
	tPtr <ULARGE_INTEGER>		mCustomStyle;
};

/////////////////////////////////////////////////////////////////////////////

OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(__uuidof(DaSvrBalloon), DaSvrBalloon)

/////////////////////////////////////////////////////////////////////////////
