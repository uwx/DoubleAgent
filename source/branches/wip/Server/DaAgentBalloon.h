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
#ifndef DAAGENTBALLOON_H_INCLUDED_
#define DAAGENTBALLOON_H_INCLUDED_
#pragma once

#include "AgentFile.h"
#include "AgentFileCache.h"
#include "DaAgentNotify.h"
#include "DaGlobalConfig.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E512-A208-11DE-ABF2-002421116FB2}")) CDaAgentBalloon : public CCmdTarget
{
public:
	CDaAgentBalloon (long pCharID, class CAgentPopupWnd * pOwner, CAgentFile * pFile, IDaNotify & pNotify);
	virtual ~CDaAgentBalloon ();
	void Terminate (bool pFinal, bool pAbandonned = false);
	DECLARE_DYNAMIC (CDaAgentBalloon)
	DECLARE_OLETYPELIB(CDaAgentBalloon)

// Attributes
public:
	long GetCharID () const;
	LANGID GetLangID () const;

// Operations
public:
	bool SetLangID (LANGID pLangID);

// Overrides
	//{{AFX_VIRTUAL(CDaAgentBalloon)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual LPUNKNOWN GetInterfaceHook(const void* iid);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_DISPATCH(CDaAgentBalloon)
	afx_msg void DspGetEnabled(long FAR* Enabled);
	afx_msg void DspGetNumLines(long FAR* Lines);
	afx_msg void DspGetNumCharsPerLine(long FAR* CharsPerLine);
	afx_msg void DspGetFontName(BSTR FAR* FontName);
	afx_msg void DspGetFontSize(long FAR* FontSize);
	afx_msg void DspGetFontBold(long FAR* FontBold);
	afx_msg void DspGetFontItalic(long FAR* FontItalic);
	afx_msg void DspGetFontStrikethru(long FAR* FontStrikethru);
	afx_msg void DspGetFontUnderline(long FAR* FontUnderline);
	afx_msg void DspGetForeColor(long FAR* ForeColor);
	afx_msg void DspGetBackColor(long FAR* BackColor);
	afx_msg void DspGetBorderColor(long FAR* BorderColor);
	afx_msg void DspSetVisible(long Visible);
	afx_msg void DspGetVisible(long FAR* Visible);
	afx_msg void DspSetFontName(LPCTSTR FontName);
	afx_msg void DspSetFontSize(long FontSize);
	afx_msg void DspSetFontCharSet(short FontCharSet);
	afx_msg void DspGetFontCharSet(short FAR* FontCharSet);
	afx_msg void DspSetStyle(long Style);
	afx_msg void DspGetStyle(long FAR* Style);
	afx_msg void DspSetNumLines(long Lines);
	afx_msg void DspSetNumCharsPerLine(long CharsPerLine);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_DISPATCH_IID()

	BEGIN_INTERFACE_PART(Balloon, IDaSvrBalloon)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

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
	END_INTERFACE_PART(Balloon)

	DECLARE_SUPPORTERRORINFO()
	DECLARE_PROVIDECLASSINFO()
	DECLARE_INTERFACE_MAP()

protected:
	class CAgentBalloonWnd * GetBalloonWnd (long pCharID = 0);
	class CAgentPopupWnd * GetOwnerWnd ();
	CAgentFileBalloon * GetCustomConfig (bool pCreate = false);

protected:
	long						mCharID;
	LANGID						mLangID;
	class CAgentPopupWnd *		mOwner;
	IDispatchPtr				mOwnerRefHolder;
	CAgentFile *				mFile;
	IDaNotify &					mNotify;
	CDaBalloonConfig			mGlobalConfig;
	tPtr <CAgentFileBalloon>	mCustomConfig;
	tPtr <ULARGE_INTEGER>		mCustomStyle;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DAAGENTBALLOON_H_INCLUDED_
