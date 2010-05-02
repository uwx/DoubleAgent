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
#include "AgentFile.h"
#include "DaGlobalConfig.h"

/////////////////////////////////////////////////////////////////////////////

class CDaCmnBalloon
{
public:
	CDaCmnBalloon ();
	virtual ~CDaCmnBalloon ();

// Operations
public:
	void Initialize (long pCharID, CAgentFile * pFile, class CAgentPopupWnd * pOwnerWnd);
	bool SetLangID (LANGID pLangID);
	bool SetBalloonWndOptions (LANGID pLangID = 0);

// Interfaces
public:
	// IDaSvrBalloon2
	HRESULT get_Enabled (VARIANT_BOOL *Enabled);
	HRESULT get_Style (long *Style);
	HRESULT put_Style (long Style);
	HRESULT get_Visible (VARIANT_BOOL *Visible);
	HRESULT put_Visible (VARIANT_BOOL Visible);
	HRESULT get_NumberOfLines (long *NumberOfLines);
	HRESULT put_NumberOfLines (long NumberOfLines);
	HRESULT get_CharsPerLine (long *CharsPerLine);
	HRESULT put_CharsPerLine (long CharsPerLine);
	HRESULT get_TextColor (long *TextColor);
	HRESULT put_TextColor (long TextColor);
	HRESULT get_BackColor (long *BackColor);
	HRESULT put_BackColor (long BackColor);
	HRESULT get_BorderColor (long *BorderColor);
	HRESULT put_BorderColor (long BorderColor);
	HRESULT get_FontName (BSTR *FontName);
	HRESULT put_FontName (BSTR FontName);
	HRESULT get_FontSize (long *FontSize);
	HRESULT put_FontSize (long FontSize);
	HRESULT get_FontBold (VARIANT_BOOL *FontBold);
	HRESULT put_FontBold (VARIANT_BOOL FontBold);
	HRESULT get_FontItalic (VARIANT_BOOL *FontItalic);
	HRESULT put_FontItalic (VARIANT_BOOL FontItalic);
	HRESULT get_FontStrikethru (VARIANT_BOOL *FontStrikethru);
	HRESULT put_FontStrikethru (VARIANT_BOOL FontStrikethru);
	HRESULT get_FontUnderline (VARIANT_BOOL *FontUnderline);
	HRESULT put_FontUnderline (VARIANT_BOOL FontUnderline);
	HRESULT get_FontCharSet (short *FontCharSet);
	HRESULT put_FontCharSet (short FontCharSet);

// Implementation
protected:
	class CAgentBalloonWnd * GetBalloonWnd (long pCharID = 0);
	class CAgentPopupWnd * GetOwnerWnd ();
	CAgentFileBalloon * GetCustomConfig (bool pCreate = false);

protected:
	long						mCharID;
	LANGID						mLangID;
	CAgentFile *				mFile;
	class CAgentPopupWnd *		mOwnerWnd;
	CDaBalloonConfig			mGlobalConfig;
	tPtr <CAgentFileBalloon>	mCustomConfig;
	tPtr <ULARGE_INTEGER>		mCustomStyle;
};

/////////////////////////////////////////////////////////////////////////////
