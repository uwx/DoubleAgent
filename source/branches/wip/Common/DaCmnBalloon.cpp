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
#include "StdAfx.h"
#include <AgtErr.h>
#include "DaCmnBalloon.h"
#include "AgentBalloonWnd.h"
#include "AgentPopupWnd.h"
#include "GuidStr.h"

/////////////////////////////////////////////////////////////////////////////

CDaCmnBalloon::CDaCmnBalloon ()
:	mCharID (0),
	mLangID (GetUserDefaultUILanguage ()),
	mFile (NULL),
	mOwnerWnd (NULL)
{
}

CDaCmnBalloon::~CDaCmnBalloon ()
{
}

void CDaCmnBalloon::Initialize (long pCharID, CAgentFile * pFile, CAgentPopupWnd * pOwnerWnd)
{
	mCharID = pCharID;
	mFile = pFile;
	mOwnerWnd = pOwnerWnd;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAgentBalloonWnd * CDaCmnBalloon::GetBalloonWnd (long pCharID)
{
	CAgentBalloonWnd *	lBalloonWnd = NULL;
	CAgentPopupWnd *	lOwner;

	if	(
			(lOwner = GetOwnerWnd ())
		&&	(lBalloonWnd = lOwner->GetBalloonWnd())
		&&	(lBalloonWnd->IsWindow ())
		&&	(
				(pCharID <= 0)
			||	(lBalloonWnd->GetCharID () == pCharID)
			)
		)
	{
		return lBalloonWnd;
	}
	return NULL;
}

CAgentPopupWnd * CDaCmnBalloon::GetOwnerWnd ()
{
	if	(
			(mOwnerWnd)
		&&	(mOwnerWnd->IsWindow ())
		)
	{
		return mOwnerWnd;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

CAgentFileBalloon * CDaCmnBalloon::GetCustomConfig (bool pCreate)
{
	if	(
			(!mCustomConfig)
		&&	(pCreate)
		&&	(mCustomConfig = new CAgentFileBalloon)
		)
	{
		(*mCustomConfig) = const_cast <CAgentFileBalloon &> (mFile->GetBalloon());
	}
	return mCustomConfig;
}

/////////////////////////////////////////////////////////////////////////////

bool CDaCmnBalloon::SetLangID (LANGID pLangID)
{
	if	(pLangID == LANG_USER_DEFAULT)
	{
#ifdef	_DEBUG
		pLangID = LANGIDFROMLCID (GetThreadLocale ());
#else
		pLangID = GetUserDefaultUILanguage();
#endif
	}
	else
	if	(pLangID == LANG_SYSTEM_DEFAULT)
	{
		pLangID = GetSystemDefaultUILanguage();
	}

	if	(mLangID != pLangID)
	{
		CAgentFileBalloon *	lCustomConfig;
		tS <LOGFONT>		lOldFont;
		tS <LOGFONT>		lNewFont;

		if	(
				(mFile)
			&&	(lCustomConfig = GetCustomConfig (true))
			)
		{
			if	(
					(CAgentBalloonWnd::CopyBalloonFont (*lCustomConfig, lOldFont))
				&&	(CAgentBalloonWnd::SetFontLangID (lOldFont, mLangID))
				&&	(CAgentBalloonWnd::CopyBalloonFont (*lCustomConfig, lNewFont))
				&&	(CAgentBalloonWnd::SetFontLangID (lNewFont, pLangID))
				&&	(lNewFont.lfCharSet == lOldFont.lfCharSet)
				&&	(CAgentBalloonWnd::GetActualFont (lNewFont, lNewFont, false))
				)
			{
				CAgentBalloonWnd::CopyBalloonFont (lNewFont, *lCustomConfig);
			}
			else
			{
				lOldFont.Clear ();
				lNewFont.Clear ();

				if	(
						(CAgentBalloonWnd::CopyBalloonFont (mFile->GetBalloon(), lOldFont))
					&&	(CAgentBalloonWnd::SetFontLangID (lOldFont, pLangID))
					&&	(CAgentBalloonWnd::GetActualFont (lOldFont, lNewFont, false))
					)
				{
					CAgentBalloonWnd::CopyBalloonFont (lNewFont, *lCustomConfig);
				}
			}
		}

		mLangID = pLangID;
		return true;
	}
	return false;
}

bool CDaCmnBalloon::SetBalloonWndOptions (LANGID pLangID)
{
	bool				lRet = false;
	CAgentBalloonWnd *	lBalloonWnd = NULL;
	long				lBalloonStyle;

	if	(lBalloonWnd = GetBalloonWnd (mCharID))
	{
		get_Style (&lBalloonStyle);
		if	(!pLangID)
		{
			pLangID = mLangID;
		}
		if	(mCustomConfig)
		{
			lRet = lBalloonWnd->SetOptions ((DWORD)lBalloonStyle, *mCustomConfig, pLangID);
		}
		else
		{
			lRet = lBalloonWnd->SetOptions ((DWORD)lBalloonStyle, mFile->GetBalloon(), pLangID);
		}
	}		
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnBalloon::get_Enabled (VARIANT_BOOL *Enabled)
{
	HRESULT	lResult = S_OK;

	if	(
			(mFile->GetStyle() & CharStyleBalloon)
		&&	(mGlobalConfig.LoadConfig().mEnabled)
		)
	{
		if	(Enabled)
		{
			(*Enabled) = VARIANT_TRUE;
		}
	}
	else
	{
		if	(Enabled)
		{
			(*Enabled) = VARIANT_FALSE;
		}
		lResult = S_FALSE;
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnBalloon::get_Style (long *Style)
{
	HRESULT	lResult = S_OK;

	if	(!Style)
	{
		lResult = E_POINTER;
	}
	else
	{
		DWORD	lCharStyle;

		if	(mCustomStyle)
		{
			(*Style) = mCustomStyle->LowPart & ~BalloonStyle_Enabled;
			lCharStyle = mCustomStyle->HighPart;
		}
		else
		{
			(*Style) = BalloonStyle_AutoHide | BalloonStyle_AutoPace;
			lCharStyle = mFile->GetStyle();
		}

		if	(lCharStyle & CharStyleBalloon)
		{
			if	(mGlobalConfig.LoadConfig().mEnabled)
			{
				(*Style) |= BalloonStyle_Enabled;
			}
			if	(lCharStyle & CharStyleSizeToText)
			{
				(*Style) |= BalloonStyle_SizeToText;
			}
			if	(lCharStyle & CharStyleNoAutoHide)
			{
				(*Style) &= ~BalloonStyle_AutoHide;
			}
			if	(lCharStyle & CharStyleNoAutoPace)
			{
				(*Style) &= ~BalloonStyle_AutoPace;
			}
		}
	}
	return lResult;
}

HRESULT CDaCmnBalloon::put_Style (long Style)
{
	HRESULT	lResult = S_OK;
	DWORD	lCharStyle = mFile->GetStyle();

	if	(
			((lCharStyle & CharStyleBalloon) == 0)
		||	(!mGlobalConfig.LoadConfig().mEnabled)
		)
	{
		lResult = S_FALSE;
	}

	if	(Style & BalloonStyle_Enabled)
	{
		lCharStyle |= CharStyleBalloon;
	}
	else
	{
		lCharStyle &= ~CharStyleBalloon;
	}
	if	(Style & BalloonStyle_SizeToText)
	{
		lCharStyle |= CharStyleSizeToText;
	}
	else
	{
		lCharStyle &= ~CharStyleSizeToText;

		if	(
				(
					(!mCustomStyle)
				||	(mCustomStyle->HighPart & CharStyleSizeToText)
				)
			&&	(mCustomConfig)
			)
		{
			mCustomConfig->mLines = mFile->GetBalloon().mLines;
			mCustomConfig->mPerLine = mFile->GetBalloon().mPerLine;
		}
	}
	if	(Style & BalloonStyle_AutoHide)
	{
		lCharStyle &= ~CharStyleNoAutoHide;
	}
	else
	{
		lCharStyle |= CharStyleNoAutoHide;
	}
	if	(Style & BalloonStyle_AutoPace)
	{
		lCharStyle &= ~CharStyleNoAutoPace;
	}
	else
	{
		lCharStyle |= CharStyleNoAutoPace;
	}

	if	(mCustomStyle = new ULARGE_INTEGER)
	{
		mCustomStyle->LowPart = Style;
		mCustomStyle->HighPart = lCharStyle;
	}

	if	(SUCCEEDED (lResult))
	{
		SetBalloonWndOptions ();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnBalloon::get_Visible (VARIANT_BOOL *Visible)
{
	HRESULT				lResult = S_OK;
	CAgentBalloonWnd *	lBalloonWnd;

	if	(
			(lBalloonWnd = GetBalloonWnd())
		&&	(lBalloonWnd->IsWindowVisible())
		)
	{
		if	(Visible)
		{
			(*Visible) = VARIANT_TRUE;
		}
	}
	else
	{
		if	(Visible)
		{
			(*Visible) = VARIANT_FALSE;
		}
		lResult = S_FALSE;
	}
	return lResult;
}

HRESULT CDaCmnBalloon::put_Visible (VARIANT_BOOL Visible)
{
	HRESULT				lResult = S_OK;
	CAgentBalloonWnd *	lBalloonWnd;

	if	(lBalloonWnd = GetBalloonWnd ())
	{
		if	(Visible)
		{
			if	(!lBalloonWnd->ShowBalloonNow ())
			{
				lResult = S_FALSE;
			}
		}
		else
		{
			if	(!lBalloonWnd->HideBalloon ())
			{
				lResult = S_FALSE;
			}
		}
	}
	else
	{
		lResult = AGENTERR_NOBALLOON;
	}

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnBalloon::get_NumberOfLines (long *NumberOfLines)
{
	HRESULT	lResult = S_OK;

	if	(!NumberOfLines)
	{
		lResult = E_POINTER;
	}
	else
	if	(mCustomConfig)
	{
		(*NumberOfLines) = (long)(short)mCustomConfig->mLines;
	}
	else
	{
		(*NumberOfLines) = (long)(short)mFile->GetBalloon().mLines;
	}
	return lResult;
}

HRESULT CDaCmnBalloon::put_NumberOfLines (long NumberOfLines)
{
	HRESULT	lResult = S_OK;
	long	lStyle;

	get_Style (&lStyle);

	if	((mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	if	(lStyle & BalloonStyle_SizeToText)
	{
		lResult = E_UNEXPECTED;
	}
	else
	{
		if	(
				(NumberOfLines < (long)CAgentBalloonWnd::mMinLines)
			||	(NumberOfLines > (long)CAgentBalloonWnd::mMaxLines)
			)
		{
			lResult = E_INVALIDARG;
		}
		else
		if	(GetCustomConfig (true))
		{
			mCustomConfig->mLines = LOBYTE(NumberOfLines);
		}
	}
	if	(SUCCEEDED (lResult))
	{
		SetBalloonWndOptions ();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnBalloon::get_CharsPerLine (long *CharsPerLine)
{
	HRESULT	lResult = S_OK;

	if	(!CharsPerLine)
	{
		lResult = E_POINTER;
	}
	else
	if	(mCustomConfig)
	{
		(*CharsPerLine) = (long)(short)mCustomConfig->mPerLine;
	}
	else
	{
		(*CharsPerLine) = (long)(short)mFile->GetBalloon().mPerLine;
	}
	return lResult;
}

HRESULT CDaCmnBalloon::put_CharsPerLine (long CharsPerLine)
{
	HRESULT	lResult = S_OK;
	long	lStyle;

	get_Style (&lStyle);

	if	((mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	if	(lStyle & BalloonStyle_SizeToText)
	{
		lResult = E_UNEXPECTED;
	}
	else
	{
		if	(
				(CharsPerLine < (long)CAgentBalloonWnd::mMinPerLine)
			||	(CharsPerLine > (long)CAgentBalloonWnd::mMaxPerLine)
			)
		{
			lResult = E_INVALIDARG;
		}
		else
		if	(GetCustomConfig (true))
		{
			mCustomConfig->mPerLine = LOBYTE(CharsPerLine);
		}
	}
	if	(SUCCEEDED (lResult))
	{
		SetBalloonWndOptions ();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnBalloon::get_TextColor (long *TextColor)
{
	HRESULT	lResult = S_OK;

	if	(!TextColor)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mFgColor)
	{
		(*TextColor) = *mGlobalConfig.mFgColor;
	}
	else
	if	(mCustomConfig)
	{
		(*TextColor) = mCustomConfig->mFgColor;
	}
	else
	{
		(*TextColor) = mFile->GetBalloon().mFgColor;
	}
	return lResult;
}

HRESULT CDaCmnBalloon::put_TextColor (long TextColor)
{
	HRESULT	lResult = S_OK;

	if	((mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	{
		if	(GetCustomConfig (true))
		{
			mCustomConfig->mFgColor = TextColor;
		}
	}
	if	(SUCCEEDED (lResult))
	{
		SetBalloonWndOptions ();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnBalloon::get_BackColor (long *BackColor)
{
	HRESULT	lResult = S_OK;

	if	(!BackColor)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mBkColor)
	{
		(*BackColor) = *mGlobalConfig.mBkColor;
	}
	else
	if	(mCustomConfig)
	{
		(*BackColor) = mCustomConfig->mBkColor;
	}
	else
	{
		(*BackColor) = mFile->GetBalloon().mBkColor;
	}
	return lResult;
}

HRESULT CDaCmnBalloon::put_BackColor (long BackColor)
{
	HRESULT	lResult = S_OK;

	if	((mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	{
		if	(GetCustomConfig (true))
		{
			mCustomConfig->mBkColor = BackColor;
		}
	}
	if	(SUCCEEDED (lResult))
	{
		SetBalloonWndOptions ();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnBalloon::get_BorderColor (long *BorderColor)
{
	HRESULT	lResult = S_OK;

	if	(!BorderColor)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mBrColor)
	{
		(*BorderColor) = *mGlobalConfig.mBrColor;
	}
	else
	if	(mCustomConfig)
	{
		(*BorderColor) = mCustomConfig->mBrColor;
	}
	else
	{
		(*BorderColor) = mFile->GetBalloon().mBrColor;
	}
	return lResult;
}

HRESULT CDaCmnBalloon::put_BorderColor (long BorderColor)
{
	HRESULT	lResult = S_OK;

	if	((mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	{
		if	(GetCustomConfig (true))
		{
			mCustomConfig->mBrColor = BorderColor;
		}
	}
	if	(SUCCEEDED (lResult))
	{
		SetBalloonWndOptions ();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnBalloon::get_FontName (BSTR *FontName)
{
	HRESULT	lResult = S_OK;

	if	(!FontName)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mFont)
	{
		(*FontName) = _bstr_t (mGlobalConfig.mFont->lfFaceName).Detach ();
	}
	else
	if	(mCustomConfig)
	{
		(*FontName) = _bstr_t (mCustomConfig->mFont.lfFaceName).Detach ();
	}
	else
	{
		(*FontName) = _bstr_t (mFile->GetBalloon().mFont.lfFaceName).Detach ();
	}
	return lResult;
}

HRESULT CDaCmnBalloon::put_FontName (BSTR FontName)
{
	HRESULT	lResult = S_OK;

	if	((mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	if	(
			(!FontName)
		||	(!FontName [0])
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		if	(GetCustomConfig (true))
		{
			_tcsncpy (mCustomConfig->mFont.lfFaceName, CAtlString(FontName), (sizeof(mCustomConfig->mFont.lfFaceName)/sizeof(WCHAR))-1);
		}
	}
	if	(SUCCEEDED (lResult))
	{
		SetBalloonWndOptions ();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnBalloon::get_FontSize (long *FontSize)
{
	HRESULT	lResult = S_OK;

	if	(!FontSize)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mFont)
	{
		(*FontSize) = mGlobalConfig.mFont->lfHeight;
	}
	else
	if	(mCustomConfig)
	{
		(*FontSize) = mCustomConfig->mFont.lfHeight;
	}
	else
	{
		(*FontSize) = mFile->GetBalloon().mFont.lfHeight;
	}
	return lResult;
}

HRESULT CDaCmnBalloon::put_FontSize (long FontSize)
{
	HRESULT	lResult = S_OK;

	if	((mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	if	(FontSize == 0)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		if	(GetCustomConfig (true))
		{
			mCustomConfig->mFont.lfHeight = FontSize;
		}
	}
	if	(SUCCEEDED (lResult))
	{
		SetBalloonWndOptions ();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnBalloon::get_FontBold (VARIANT_BOOL *FontBold)
{
	HRESULT	lResult = S_OK;

	if	(!FontBold)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mFont)
	{
		(*FontBold) = (mGlobalConfig.mFont->lfWeight >= FW_BOLD) ? VARIANT_TRUE : VARIANT_FALSE;
	}
	else
	if	(mCustomConfig)
	{
		(*FontBold) = (mCustomConfig->mFont.lfWeight >= FW_BOLD) ? VARIANT_TRUE : VARIANT_FALSE;
	}
	else
	{
		(*FontBold) = (mFile->GetBalloon().mFont.lfWeight >= FW_BOLD) ? VARIANT_TRUE : VARIANT_FALSE;
	}
	return lResult;
}

HRESULT CDaCmnBalloon::put_FontBold (VARIANT_BOOL FontBold)
{
	HRESULT	lResult = S_OK;

	if	((mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	{
		if	(GetCustomConfig (true))
		{
			mCustomConfig->mFont.lfWeight = FontBold ? FW_BOLD : FW_NORMAL;
		}
	}
	if	(SUCCEEDED (lResult))
	{
		SetBalloonWndOptions ();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnBalloon::get_FontItalic (VARIANT_BOOL *FontItalic)
{
	HRESULT	lResult = S_OK;

	if	(!FontItalic)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mFont)
	{
		(*FontItalic) = mGlobalConfig.mFont->lfItalic ? VARIANT_TRUE : VARIANT_FALSE;
	}
	else
	if	(mCustomConfig)
	{
		(*FontItalic) = mCustomConfig->mFont.lfItalic ? VARIANT_TRUE : VARIANT_FALSE;
	}
	else
	{
		(*FontItalic) = mFile->GetBalloon().mFont.lfItalic ? VARIANT_TRUE : VARIANT_FALSE;
	}
	return lResult;
}

HRESULT CDaCmnBalloon::put_FontItalic (VARIANT_BOOL FontItalic)
{
	HRESULT	lResult = S_OK;

	if	((mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	{
		if	(GetCustomConfig (true))
		{
			mCustomConfig->mFont.lfItalic = FontItalic ? TRUE : FALSE;
		}
	}
	if	(SUCCEEDED (lResult))
	{
		SetBalloonWndOptions ();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnBalloon::get_FontStrikethru (VARIANT_BOOL *FontStrikethru)
{
	HRESULT	lResult = S_OK;

	if	(!FontStrikethru)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mFont)
	{
		(*FontStrikethru) = mGlobalConfig.mFont->lfStrikeOut ? VARIANT_TRUE : VARIANT_FALSE;
	}
	else
	if	(mCustomConfig)
	{
		(*FontStrikethru) = mCustomConfig->mFont.lfStrikeOut ? VARIANT_TRUE : VARIANT_FALSE;
	}
	else
	{
		(*FontStrikethru) = mFile->GetBalloon().mFont.lfStrikeOut ? VARIANT_TRUE : VARIANT_FALSE;
	}
	return lResult;
}

HRESULT CDaCmnBalloon::put_FontStrikethru (VARIANT_BOOL FontStrikethru)
{
	HRESULT	lResult = S_OK;

	if	((mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	{
		if	(GetCustomConfig (true))
		{
			mCustomConfig->mFont.lfStrikeOut = FontStrikethru ? TRUE : FALSE;
		}
	}
	if	(SUCCEEDED (lResult))
	{
		SetBalloonWndOptions ();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnBalloon::get_FontUnderline (VARIANT_BOOL *FontUnderline)
{
	HRESULT	lResult = S_OK;

	if	(!FontUnderline)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mFont)
	{
		(*FontUnderline) = mGlobalConfig.mFont->lfUnderline ? VARIANT_TRUE : VARIANT_FALSE;
	}
	else
	if	(mCustomConfig)
	{
		(*FontUnderline) = mCustomConfig->mFont.lfUnderline ? VARIANT_TRUE : VARIANT_FALSE;
	}
	else
	{
		(*FontUnderline) = mFile->GetBalloon().mFont.lfUnderline ? VARIANT_TRUE : VARIANT_FALSE;
	}
	return lResult;
}

HRESULT CDaCmnBalloon::put_FontUnderline (VARIANT_BOOL FontUnderline)
{
	HRESULT	lResult = S_OK;

	if	((mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	{
		if	(GetCustomConfig (true))
		{
			mCustomConfig->mFont.lfUnderline = FontUnderline ? TRUE : FALSE;
		}
	}
	if	(SUCCEEDED (lResult))
	{
		SetBalloonWndOptions ();
	}
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////

HRESULT CDaCmnBalloon::get_FontCharSet (short *FontCharSet)
{
	HRESULT	lResult = S_OK;

	if	(!FontCharSet)
	{
		lResult = E_POINTER;
	}
	else
	if	(mGlobalConfig.LoadConfig().mFont)
	{
		(*FontCharSet) = mGlobalConfig.mFont->lfCharSet;
	}
	else
	if	(mCustomConfig)
	{
		(*FontCharSet) = mCustomConfig->mFont.lfCharSet;
	}
	else
	{
		(*FontCharSet) = mFile->GetBalloon().mFont.lfCharSet;
	}
	return lResult;
}

HRESULT CDaCmnBalloon::put_FontCharSet (short FontCharSet)
{
	HRESULT	lResult = S_OK;

	if	((mFile->GetStyle() & CharStyleBalloon) == 0)
	{
		lResult = AGENTERR_NOBALLOON;
	}
	else
	if	(
			(FontCharSet < 0)
		||	(FontCharSet > 255)
		)
	{
		lResult = E_INVALIDARG;
	}
	else
	{
		if	(GetCustomConfig (true))
		{
			mCustomConfig->mFont.lfCharSet = LOBYTE(FontCharSet);
		}
	}
	if	(SUCCEEDED (lResult))
	{
		SetBalloonWndOptions ();
	}
	return lResult;
}
