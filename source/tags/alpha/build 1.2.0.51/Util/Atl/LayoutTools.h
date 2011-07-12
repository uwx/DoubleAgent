/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2011 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is a utility used by Double Agent but not specific to
	Double Agent.  However, it is included as part of the Double Agent
	source code under the following conditions:

    This is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this file.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#ifndef	LAYOUTTOOLS_H_INCLUDED_
#define LAYOUTTOOLS_H_INCLUDED_
#pragma once

#include "AtlCollEx.h"

//////////////////////////////////////////////////////////////////////

class CLayoutTools
{
public:
	HDWP BeginFormLayout (int pWndCount = 10);
	bool EndFormLayout ();
	HDWP IsFormLayoutStarted () const;

	bool AlignTo (HWND pAlign, HWND pAlignTo, const CRect* pMargin = NULL);
	bool AlignPos (HWND pAlign, HWND pAlignTo);
	bool AlignSize (HWND pAlign, HWND pAlignTo, CSize* pMult = NULL);

	bool AlignLeft (HWND pAlign, HWND pAlignTo, bool pStretch = false, int pOffset = 0);
	bool AlignRight (HWND pAlign, HWND pAlignTo, bool pStretch = false, int pOffset = 0);
	bool AlignTop (HWND pAlign, HWND pAlignTo, bool pStretch = false, int pOffset = 0);
	bool AlignBottom (HWND pAlign, HWND pAlignTo, bool pStretch = false, int pOffset = 0);

	bool AlignCenter (HWND pAlign, HWND pAlignTo, const CRect* pMargin = NULL);
	bool AlignVCenter (HWND pAlign, HWND pAlignTo, const CRect* pMargin = NULL);
	bool AlignWidth (HWND pAlign, HWND pAlignTo, float pMult = 0.0f);
	bool AlignHeight (HWND pAlign, HWND pAlignTo, float pMult = 0.0f);

	bool AlignAfter (HWND pAlign, HWND pAlignTo, bool pStretch = false, int pGap = 0);
	bool AlignBefore (HWND pAlign, HWND pAlignTo, bool pStretch = false, int pGap = 0);
	bool AlignBelow (HWND pAlign, HWND pAlignTo, bool pStretch = false, int pGap = 0);
	bool AlignAbove (HWND pAlign, HWND pAlignTo, bool pStretch = false, int pGap = 0);

	bool UpdateSize (HWND pWnd, const CSize& pSize);
	bool UpdateWidth (HWND pWnd, int pWidth);
	bool UpdateHeight (HWND pWnd, int pHeight);

	bool AdjustRect (HWND pWnd, const CPoint& pOffset);
	bool AdjustRect (HWND pWnd, const CRect& pMargin);

	CRect ChildWndRect (HWND pWnd, HWND pParentWnd = NULL);
	CPoint ChildWndPos (HWND pWnd, HWND pParentWnd = NULL);
	CSize ChildWndSize (HWND pWnd);
	CSize ChildWndGap (HWND pWnd1, HWND pWnd2);
	CPoint ChildWndMid (HWND pWnd1, HWND pWnd2, HWND pParentWnd = NULL);

	CRect ChildUsedRect (HWND pWnd, bool pCheckVisible = true);

	bool OrderAfter (HWND pOrder, const HWND pOrderAfter);
	bool OrderBefore (HWND pOrder, const HWND pOrderBefore);

public:
	CLayoutTools () : mDwp (NULL) {}

private:
	void MoveAlignWnd (HWND pParent, HWND pAlign, CRect& pRect);
	void GetAlignWndRect (HWND pAlign, CRect& pRect);
	void GetAlignWndRect (HWND pParent, HWND pAlign, CRect& pRect);
	void GetAlignWndClient (HWND pAlign, CRect& pRect);

	HDWP							mDwp;
	tPtr <CAtlMap <HWND, CRect> >	mRects;
};

//////////////////////////////////////////////////////////////////////

#endif // LAYOUTTOOLS_H_INCLUDED_
