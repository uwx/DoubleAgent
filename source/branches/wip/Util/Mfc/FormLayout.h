/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2010 Cinnamon Software Inc.
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
#ifndef	FORMLAYOUT_H_INCLUDED_
#define FORMLAYOUT_H_INCLUDED_
#pragma once

//////////////////////////////////////////////////////////////////////

class CFormLayout
{
public:
	HDWP BeginFormLayout (int pWndCount = 10);
	bool EndFormLayout ();
	HDWP IsFormLayoutStarted () const;

	bool AlignTo (CWnd * pAlign, CWnd * pAlignTo, const CRect * pMargin = NULL);
	bool AlignPos (CWnd * pAlign, CWnd * pAlignTo);
	bool AlignSize (CWnd * pAlign, CWnd * pAlignTo, CSize * pMult = NULL);

	bool AlignLeft (CWnd * pAlign, CWnd * pAlignTo, bool pStretch = false, int pOffset = 0);
	bool AlignRight (CWnd * pAlign, CWnd * pAlignTo, bool pStretch = false, int pOffset = 0);
	bool AlignTop (CWnd * pAlign, CWnd * pAlignTo, bool pStretch = false, int pOffset = 0);
	bool AlignBottom (CWnd * pAlign, CWnd * pAlignTo, bool pStretch = false, int pOffset = 0);

	bool AlignCenter (CWnd * pAlign, CWnd * pAlignTo, const CRect * pMargin = NULL);
	bool AlignVCenter (CWnd * pAlign, CWnd * pAlignTo, const CRect * pMargin = NULL);
	bool AlignWidth (CWnd * pAlign, CWnd * pAlignTo, float pMult = 0.0f);
	bool AlignHeight (CWnd * pAlign, CWnd * pAlignTo, float pMult = 0.0f);

	bool AlignAfter (CWnd * pAlign, CWnd * pAlignTo, bool pStretch = false, int pGap = 0);
	bool AlignBefore (CWnd * pAlign, CWnd * pAlignTo, bool pStretch = false, int pGap = 0);
	bool AlignBelow (CWnd * pAlign, CWnd * pAlignTo, bool pStretch = false, int pGap = 0);
	bool AlignAbove (CWnd * pAlign, CWnd * pAlignTo, bool pStretch = false, int pGap = 0);

	bool UpdateSize (CWnd * pWnd, const CSize & pSize);
	bool UpdateWidth (CWnd * pWnd, int pWidth);
	bool UpdateHeight (CWnd * pWnd, int pHeight);

	bool AdjustRect (CWnd * pWnd, const CPoint & pOffset);
	bool AdjustRect (CWnd * pWnd, const CRect & pMargin);

	CRect ChildWndRect (CWnd & pWnd, CWnd * pParentWnd = NULL);
	CPoint ChildWndPos (CWnd & pWnd, CWnd * pParentWnd = NULL);
	CSize ChildWndSize (CWnd & pWnd);
	CSize ChildWndGap (CWnd & pWnd1, CWnd & pWnd2);
	CPoint ChildWndMid (CWnd & pWnd1, CWnd & pWnd2, CWnd * pParentWnd = NULL);

	CRect ChildUsedRect (CWnd & pWnd, bool pCheckVisible = true);

	bool OrderAfter (CWnd * pOrder, const CWnd * pOrderAfter);
	bool OrderBefore (CWnd * pOrder, const CWnd * pOrderBefore);

public:
	CFormLayout () : mDwp (NULL) {}

private:
	void MoveAlignWnd (CWnd * pParent, CWnd * pAlign, CRect & pRect);
	void GetAlignWndRect (CWnd * pAlign, CRect & pRect);
	void GetAlignWndRect (CWnd * pParent, CWnd * pAlign, CRect & pRect);
	void GetAlignWndClient (CWnd * pAlign, CRect & pRect);

	HDWP											mDwp;
	tPtr <CMap <HWND, HWND, CRect, const RECT &> >	mRects;
};

//////////////////////////////////////////////////////////////////////

#endif // FORMLAYOUT_H_INCLUDED_
