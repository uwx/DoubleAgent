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
#ifndef DABALLOONOBJ_H_INCLUDED_
#define DABALLOONOBJ_H_INCLUDED_
#pragma once

#include "OleObjectFactoryExEx.h"

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E533-A208-11DE-ABF2-002421116FB2}")) CDaBalloonObj : public CCmdTarget
{
	DECLARE_DYNAMIC(CDaBalloonObj)
	DECLARE_OLECREATE_EX(CDaBalloonObj)
	DECLARE_OLETYPELIB(CDaBalloonObj)
public:
	CDaBalloonObj (class CDaCharacterObj & pOwner);
	~CDaBalloonObj ();
	void Terminate (bool pFinal);

// Attributes
	IDaSvrBalloonPtr	mServerObject;

	class CDaCharacterObj * SafeGetOwner () const;
	int SafeGetOwnerUsed () const;

// Overrides
	//{{AFX_VIRTUAL(CDaBalloonObj)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_DISPATCH(CDaBalloonObj)
	afx_msg BOOL DspGetEnabled();
	afx_msg void DspSetEnabled(BOOL bNewValue);
	afx_msg long DspGetNumberOfLines();
	afx_msg void DspSetNumberOfLines(long nNewValue);
	afx_msg long DspGetCharsPerLine();
	afx_msg void DspSetCharsPerLine(long nNewValue);
	afx_msg BSTR DspGetFontName();
	afx_msg void DspSetFontName(LPCTSTR lpszNewValue);
	afx_msg long DspGetFontSize();
	afx_msg void DspSetFontSize(long nNewValue);
	afx_msg long DspGetForeColor();
	afx_msg void DspSetForeColor(long nNewValue);
	afx_msg long DspGetBackColor();
	afx_msg void DspSetBackColor(long nNewValue);
	afx_msg long DspGetBorderColor();
	afx_msg void DspSetBorderColor(long nNewValue);
	afx_msg BOOL DspGetFontBold();
	afx_msg void DspSetFontBold(BOOL bNewValue);
	afx_msg BOOL DspGetFontItalic();
	afx_msg void DspSetFontItalic(BOOL bNewValue);
	afx_msg BOOL DspGetFontStrikethru();
	afx_msg void DspSetFontStrikethru(BOOL bNewValue);
	afx_msg BOOL DspGetFontUnderline();
	afx_msg void DspSetFontUnderline(BOOL bNewValue);
	afx_msg short DspGetFontCharSet();
	afx_msg void DspSetFontCharSet(short nNewValue);
	afx_msg long DspGetStyle();
	afx_msg void DspSetStyle(long nNewValue);
	afx_msg BOOL DspGetVisible();
	afx_msg void DspSetVisible(BOOL bNewValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_DISPATCH_IID()

	BEGIN_INTERFACE_PART(Balloon, IDaCtlBalloon)
		HRESULT STDMETHODCALLTYPE GetTypeInfoCount (unsigned int*);
		HRESULT STDMETHODCALLTYPE GetTypeInfo (unsigned int, LCID, ITypeInfo**);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames (REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		HRESULT STDMETHODCALLTYPE Invoke (DISPID, REFIID, LCID, unsigned short, DISPPARAMS*, VARIANT*, EXCEPINFO*, unsigned int*);

		HRESULT STDMETHODCALLTYPE get_Enabled (VARIANT_BOOL *Enabled);
		HRESULT STDMETHODCALLTYPE get_NumberOfLines (long *Lines);
		HRESULT STDMETHODCALLTYPE get_CharsPerLine (long *CharsPerLine);
		HRESULT STDMETHODCALLTYPE get_FontName (BSTR *FontName);
		HRESULT STDMETHODCALLTYPE get_FontSize (long *FontSize);
		HRESULT STDMETHODCALLTYPE get_FontBold (VARIANT_BOOL *FontBold);
		HRESULT STDMETHODCALLTYPE get_FontItalic (VARIANT_BOOL *FontItalic);
		HRESULT STDMETHODCALLTYPE get_FontStrikethru (VARIANT_BOOL *FontStrikethru);
		HRESULT STDMETHODCALLTYPE get_FontUnderline (VARIANT_BOOL *FontUnderline);
		HRESULT STDMETHODCALLTYPE get_ForeColor (long *ForeColor);
		HRESULT STDMETHODCALLTYPE get_BackColor (long *BackColor);
		HRESULT STDMETHODCALLTYPE get_BorderColor (long *BorderColor);
		HRESULT STDMETHODCALLTYPE put_Visible (VARIANT_BOOL Visible);
		HRESULT STDMETHODCALLTYPE get_Visible (VARIANT_BOOL *Visible);
		HRESULT STDMETHODCALLTYPE put_FontName (BSTR FontName);
		HRESULT STDMETHODCALLTYPE put_FontSize (long FontSize);
		HRESULT STDMETHODCALLTYPE put_FontCharSet (short FontCharSet);
		HRESULT STDMETHODCALLTYPE get_FontCharSet (short *FontCharSet);
		HRESULT STDMETHODCALLTYPE put_Style (long Style);
		HRESULT STDMETHODCALLTYPE get_Style (long *Style);
	END_INTERFACE_PART(Balloon)

	DECLARE_SUPPORTERRORINFO()
	DECLARE_PROVIDECLASSINFO()
	DECLARE_INTERFACE_MAP()

private:
	class CDaCharacterObj *	mOwner;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // DABALLOONOBJ_H_INCLUDED_
