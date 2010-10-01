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
#include "StdAfx.h"
#include <math.h>
#include "LayoutTools.h"
#ifdef	_DEBUG
#include "DebugStr.h"
#endif

//////////////////////////////////////////////////////////////////////

HDWP CLayoutTools::BeginFormLayout (int pWndCount)
{
	mDwp = NULL;
	mRects = NULL;

	if	(mDwp = BeginDeferWindowPos (pWndCount))
	{
		mRects = new CAtlMap <HWND, CRect>;
	}
	return mDwp;
}

bool CLayoutTools::EndFormLayout ()
{
	bool	lRet = false;

	if	(
			(mDwp)
		&&	(EndDeferWindowPos (mDwp))
		)
	{
		lRet = true;
	}

	mDwp = NULL;
	mRects = NULL;
	return lRet;
}

HDWP CLayoutTools::IsFormLayoutStarted () const
{
	return mDwp;
}

//////////////////////////////////////////////////////////////////////

inline void CLayoutTools::MoveAlignWnd (HWND pParent, HWND pAlign, CRect & pRect)
{
	if	(mRects)
	{
		mRects->SetAt (pAlign, pRect);
	}
	if	(pParent)
	{
		::ScreenToClient (pParent, &pRect);

		if	(mRects)
		{
			CRect	lOldParentRect;
			CRect	lNewParentRect;

			if	(mRects->Lookup (pParent, lNewParentRect))
			{
				::GetWindowRect (pParent, &lOldParentRect);
			}
		}
	}
	if	(mDwp)
	{
		HDWP	lDwp;

		if	(lDwp = DeferWindowPos (mDwp, pAlign, NULL, pRect.left, pRect.top, pRect.Width(), pRect.Height(), SWP_NOZORDER|SWP_NOACTIVATE))
		{
			mDwp = lDwp;
		}
#ifdef	_DEBUG
		else
		if	(LogIsActive ())
		{
			LogWinErr (LogAlways, GetLastError(), _T("DeferWindowPos"));
		}
#endif
	}
	else
	{
		::MoveWindow (pAlign, pRect.left, pRect.top, pRect.Width(), pRect.Height(), TRUE);
	}
}

//////////////////////////////////////////////////////////////////////

void CLayoutTools::GetAlignWndRect (HWND pAlign, CRect & pRect)
{
	if	(
			(!mRects)
		||	(!mRects->Lookup (pAlign, pRect))
		)
	{
		::GetWindowRect (pAlign, &pRect);
	}
}

void CLayoutTools::GetAlignWndRect (HWND pParent, HWND pAlign, CRect & pRect)
{
	GetAlignWndRect (pAlign, pRect);
	if	(pParent)
	{
		::ScreenToClient (pParent, &pRect);
	}
}

void CLayoutTools::GetAlignWndClient (HWND pAlign, CRect & pRect)
{
	::GetClientRect (pAlign, &pRect);
	::ClientToScreen (pAlign, &pRect);
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CLayoutTools::AlignTo (HWND pAlign, HWND pAlignTo, const CRect * pMargin)
{
	if	(
			(pAlign)
		&&	(pAlignTo)
		)
	{
		CRect	lRect1;
		CRect	lRect2;

		GetAlignWndRect (pAlign, lRect1);
		if	(::IsChild (pAlignTo, pAlign))
		{
			::GetClientRect (pAlignTo, &lRect2);
			::ClientToScreen (pAlignTo, &lRect2);
		}
		else
		{
			GetAlignWndRect (pAlignTo, lRect2);
		}
		if	(pMargin)
		{
			lRect2.DeflateRect (*pMargin);
		}

		if	(!lRect2.EqualRect (&lRect1))
		{
			MoveAlignWnd (::GetParent (pAlign), pAlign, lRect2);
			return true;
		}
	}
	return false;
}

bool CLayoutTools::AlignPos (HWND pAlign, HWND pAlignTo)
{
	if	(
			(pAlign)
		&&	(pAlignTo)
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;

		GetAlignWndRect (pAlign, lRect1);
		if	(::IsChild (pAlignTo, pAlign))
		{
			::GetClientRect (pAlignTo, &lRect2);
			::ClientToScreen (pAlignTo, &lRect2);
		}
		else
		{
			GetAlignWndRect (pAlignTo, lRect2);
		}

		lRect3 = lRect1;
		lRect3.OffsetRect (lRect2.left - lRect1.left, lRect2.top - lRect1.top);

		if	(!lRect3.EqualRect (&lRect1))
		{
			MoveAlignWnd (::GetParent (pAlign), pAlign, lRect3);
			return true;
		}
	}
	return false;
}

bool CLayoutTools::AlignSize (HWND pAlign, HWND pAlignTo, CSize * pMult)
{
	if	(
			(pAlign)
		&&	(pAlignTo)
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;

		GetAlignWndRect (pAlign, lRect1);
		GetAlignWndRect (pAlignTo, lRect2);

		lRect3 = lRect1;
		if	(pMult)
		{
			lRect2.OffsetRect (-lRect2.TopLeft ());
			lRect2.right *= pMult->cx;
			lRect2.bottom *= pMult->cy;
		}
		lRect3 = CRect (lRect1.TopLeft (), lRect2.Size ());

		if	(!lRect3.EqualRect (&lRect1))
		{
			MoveAlignWnd (::GetParent (pAlign), pAlign, lRect3);
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////

bool CLayoutTools::AlignLeft (HWND pAlign, HWND pAlignTo, bool pStretch, int pOffset)
{
	if	(
			(pAlign)
		&&	(pAlignTo)
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;

		GetAlignWndRect (pAlign, lRect1);
		if	(::IsChild (pAlignTo, pAlign))
		{
			::GetClientRect (pAlignTo, &lRect2);
			::ClientToScreen (pAlignTo, &lRect2);
		}
		else
		{
			GetAlignWndRect (pAlignTo, lRect2);
		}

		lRect3 = lRect1;
		if	(pStretch)
		{
			lRect3.left = lRect2.left + pOffset;
		}
		else
		{
			lRect3.OffsetRect (lRect2.left - lRect1.left + pOffset, 0);
		}

		if	(!lRect3.EqualRect (&lRect1))
		{
			MoveAlignWnd (::GetParent (pAlign), pAlign, lRect3);
			return true;
		}
	}
	return false;
}

bool CLayoutTools::AlignRight (HWND pAlign, HWND pAlignTo, bool pStretch, int pOffset)
{
	if	(
			(pAlign)
		&&	(pAlignTo)
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;

		GetAlignWndRect (pAlign, lRect1);
		if	(::IsChild (pAlignTo, pAlign))
		{
			::GetClientRect (pAlignTo, &lRect2);
			::ClientToScreen (pAlignTo, &lRect2);
		}
		else
		{
			GetAlignWndRect (pAlignTo, lRect2);
		}

		lRect3 = lRect1;
		if	(pStretch)
		{
			lRect3.right = lRect2.right + pOffset;
		}
		else
		{
			lRect3.OffsetRect (lRect2.right - lRect1.right + pOffset, 0);
		}

		if	(!lRect3.EqualRect (&lRect1))
		{
			MoveAlignWnd (::GetParent (pAlign), pAlign, lRect3);
			return true;
		}
	}
	return false;
}

bool CLayoutTools::AlignTop (HWND pAlign, HWND pAlignTo, bool pStretch, int pOffset)
{
	if	(
			(pAlign)
		&&	(pAlignTo)
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;

		GetAlignWndRect (pAlign, lRect1);
		if	(::IsChild (pAlignTo, pAlign))
		{
			::GetClientRect (pAlignTo, &lRect2);
			::ClientToScreen (pAlignTo, &lRect2);
		}
		else
		{
			GetAlignWndRect (pAlignTo, lRect2);
		}

		lRect3 = lRect1;
		if	(pStretch)
		{
			lRect3.top = lRect2.top + pOffset;
		}
		else
		{
			lRect3.OffsetRect (0, lRect2.top - lRect1.top + pOffset);
		}

		if	(!lRect3.EqualRect (&lRect1))
		{
			MoveAlignWnd (::GetParent (pAlign), pAlign, lRect3);
			return true;
		}
	}
	return false;
}

bool CLayoutTools::AlignBottom (HWND pAlign, HWND pAlignTo, bool pStretch, int pOffset)
{
	if	(
			(pAlign)
		&&	(pAlignTo)
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;

		GetAlignWndRect (pAlign, lRect1);
		if	(::IsChild (pAlignTo, pAlign))
		{
			::GetClientRect (pAlignTo, &lRect2);
			::ClientToScreen (pAlignTo, &lRect2);
		}
		else
		{
			GetAlignWndRect (pAlignTo, lRect2);
		}

		lRect3 = lRect1;
		if	(pStretch)
		{
			lRect3.bottom = lRect2.bottom + pOffset;
		}
		else
		{
			lRect3.OffsetRect (0, lRect2.bottom - lRect1.bottom + pOffset);
		}

		if	(!lRect3.EqualRect (&lRect1))
		{
			MoveAlignWnd (::GetParent (pAlign), pAlign, lRect3);
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////

bool CLayoutTools::AlignCenter (HWND pAlign, HWND pAlignTo, const CRect * pMargin)
{
	if	(
			(pAlign)
		&&	(pAlignTo)
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;

		GetAlignWndRect (pAlign, lRect1);
		if	(::IsChild (pAlignTo, pAlign))
		{
			::GetClientRect (pAlignTo, &lRect2);
			::ClientToScreen (pAlignTo, &lRect2);
		}
		else
		{
			GetAlignWndRect (pAlignTo, lRect2);
		}
		if	(pMargin)
		{
			lRect2.DeflateRect (*pMargin);
		}

		lRect3 = lRect1;
		lRect3.OffsetRect (lRect2.CenterPoint ().x - lRect1.CenterPoint ().x, 0);

		if	(!lRect3.EqualRect (&lRect1))
		{
			MoveAlignWnd (::GetParent (pAlign), pAlign, lRect3);
			return true;
		}
	}
	return false;
}

bool CLayoutTools::AlignVCenter (HWND pAlign, HWND pAlignTo, const CRect * pMargin)
{
	if	(
			(pAlign)
		&&	(pAlignTo)
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;

		GetAlignWndRect (pAlign, lRect1);
		if	(::IsChild (pAlignTo, pAlign))
		{
			::GetClientRect (pAlignTo, &lRect2);
			::ClientToScreen (pAlignTo, &lRect2);
		}
		else
		{
			GetAlignWndRect (pAlignTo, lRect2);
		}
		if	(pMargin)
		{
			lRect2.DeflateRect (*pMargin);
		}

		lRect3 = lRect1;
		lRect3.OffsetRect (0, lRect2.CenterPoint ().y - lRect1.CenterPoint ().y);

		if	(!lRect3.EqualRect (&lRect1))
		{
			MoveAlignWnd (::GetParent (pAlign), pAlign, lRect3);
			return true;
		}
	}
	return false;
}

bool CLayoutTools::AlignWidth (HWND pAlign, HWND pAlignTo, float pMult)
{
	if	(
			(pAlign)
		&&	(pAlignTo)
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;

		GetAlignWndRect (pAlign, lRect1);
		if	(::IsChild (pAlignTo, pAlign))
		{
			::GetClientRect (pAlignTo, &lRect2);
			::ClientToScreen (pAlignTo, &lRect2);
		}
		else
		{
			GetAlignWndRect (pAlignTo, lRect2);
		}

		lRect3 = lRect1;
		if	(pMult > 0.0f)
		{
			lRect3.right = lRect3.left + (long) floor ((float) lRect2.Width () * pMult);
		}
		else
		{
			lRect3.right = lRect3.left + lRect2.Width ();
		}

		if	(!lRect3.EqualRect (&lRect1))
		{
			MoveAlignWnd (::GetParent (pAlign), pAlign, lRect3);
			return true;
		}
	}
	return false;
}

bool CLayoutTools::AlignHeight (HWND pAlign, HWND pAlignTo, float pMult)
{
	if	(
			(pAlign)
		&&	(pAlignTo)
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;

		GetAlignWndRect (pAlign, lRect1);
		if	(::IsChild (pAlignTo, pAlign))
		{
			::GetClientRect (pAlignTo, &lRect2);
			::ClientToScreen (pAlignTo, &lRect2);
		}
		else
		{
			GetAlignWndRect (pAlignTo, lRect2);
		}

		lRect3 = lRect1;
		if	(pMult > 0.0f)
		{
			lRect3.bottom = lRect3.top + (long) floor ((float) lRect2.Height () * pMult);
		}
		else
		{
			lRect3.bottom = lRect3.top + lRect2.Height ();
		}

		if	(!lRect3.EqualRect (&lRect1))
		{
			MoveAlignWnd (::GetParent (pAlign), pAlign, lRect3);
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////

bool CLayoutTools::AlignAfter (HWND pAlign, HWND pAlignTo, bool pStretch, int pGap)
{
	if	(
			(pAlign)
		&&	(pAlignTo)
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;

		GetAlignWndRect (pAlign, lRect1);
		GetAlignWndRect (pAlignTo, lRect2);

		lRect3 = lRect1;
		if	(pStretch)
		{
			lRect3.left = lRect2.right + pGap;
		}
		else
		{
			lRect3.OffsetRect (lRect2.right - lRect1.left + pGap, 0);
		}

		if	(!lRect3.EqualRect (&lRect1))
		{
			MoveAlignWnd (::GetParent (pAlign), pAlign, lRect3);
			return true;
		}
	}
	return false;
}

bool CLayoutTools::AlignBefore (HWND pAlign, HWND pAlignTo, bool pStretch, int pGap)
{
	if	(
			(pAlign)
		&&	(pAlignTo)
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;

		GetAlignWndRect (pAlign, lRect1);
		GetAlignWndRect (pAlignTo, lRect2);

		lRect3 = lRect1;
		if	(pStretch)
		{
			lRect3.right = lRect2.left - pGap;
		}
		else
		{
			lRect3.OffsetRect (lRect2.left - lRect1.right - pGap, 0);
		}

		if	(!lRect3.EqualRect (&lRect1))
		{
			MoveAlignWnd (::GetParent (pAlign), pAlign, lRect3);
			return true;
		}
	}
	return false;
}

bool CLayoutTools::AlignBelow (HWND pAlign, HWND pAlignTo, bool pStretch, int pGap)
{
	if	(
			(pAlign)
		&&	(pAlignTo)
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;

		GetAlignWndRect (pAlign, lRect1);
		GetAlignWndRect (pAlignTo, lRect2);

		lRect3 = lRect1;
		if	(pStretch)
		{
			lRect3.top = lRect2.bottom + pGap;
		}
		else
		{
			lRect3.OffsetRect (0, lRect2.bottom - lRect1.top + pGap);
		}

		if	(!lRect3.EqualRect (&lRect1))
		{
			MoveAlignWnd (::GetParent (pAlign), pAlign, lRect3);
			return true;
		}
	}
	return false;
}

bool CLayoutTools::AlignAbove (HWND pAlign, HWND pAlignTo, bool pStretch, int pGap)
{
	if	(
			(pAlign)
		&&	(pAlignTo)
		)
	{
		CRect	lRect1;
		CRect	lRect2;
		CRect	lRect3;

		GetAlignWndRect (pAlign, lRect1);
		GetAlignWndRect (pAlignTo, lRect2);

		lRect3 = lRect1;
		if	(pStretch)
		{
			lRect3.bottom = lRect2.top - pGap;
		}
		else
		{
			lRect3.OffsetRect (0, lRect2.top - lRect1.bottom - pGap);
		}

		if	(!lRect3.EqualRect (&lRect1))
		{
			MoveAlignWnd (::GetParent (pAlign), pAlign, lRect3);
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////

bool CLayoutTools::UpdateSize (HWND pWnd, const CSize & pSize)
{
	if	(pWnd)
	{
		CRect	lRect;

		GetAlignWndRect (pWnd, lRect);

		if	(lRect.Size() != pSize)
		{
			lRect.right = lRect.left + pSize.cx;
			lRect.bottom = lRect.top + pSize.cy;
			MoveAlignWnd (::GetParent (pWnd), pWnd, lRect);
			return true;
		}
	}
	return false;
}

bool CLayoutTools::UpdateWidth (HWND pWnd, int pWidth)
{
	if	(pWnd)
	{
		CRect	lRect;

		GetAlignWndRect (pWnd, lRect);

		if	(lRect.Width() != pWidth)
		{
			lRect.right = lRect.left + pWidth;
			MoveAlignWnd (::GetParent (pWnd), pWnd, lRect);
			return true;
		}
	}
	return false;
}

bool CLayoutTools::UpdateHeight (HWND pWnd, int pHeight)
{
	if	(pWnd)
	{
		CRect	lRect;

		GetAlignWndRect (pWnd, lRect);

		if	(lRect.Height() != pHeight)
		{
			lRect.bottom = lRect.top + pHeight;
			MoveAlignWnd (::GetParent (pWnd), pWnd, lRect);
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////

bool CLayoutTools::AdjustRect (HWND pWnd, const CPoint & pOffset)
{
	if	(pWnd)
	{
		CRect	lRect;

		GetAlignWndRect (pWnd, lRect);
		lRect.OffsetRect (pOffset);
		MoveAlignWnd (::GetParent (pWnd), pWnd, lRect);

		return true;
	}
	return false;
}

bool CLayoutTools::AdjustRect (HWND pWnd, const CRect & pMargin)
{
	if	(pWnd)
	{
		CRect	lRect;

		GetAlignWndRect (pWnd, lRect);
		lRect.InflateRect (pMargin);
		MoveAlignWnd (::GetParent (pWnd), pWnd, lRect);

		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////

CRect CLayoutTools::ChildWndRect (HWND pWnd, HWND pParentWnd)
{
	CRect	lRect;
	HWND	lParentWnd = pParentWnd ? pParentWnd : ::GetParent (pWnd);

	::GetWindowRect (pWnd, &lRect);
	::ScreenToClient (lParentWnd, &lRect);
	return lRect;
}

CPoint CLayoutTools::ChildWndPos (HWND pWnd, HWND pParentWnd)
{
	CRect	lRect;
	HWND	lParentWnd = pParentWnd ? pParentWnd : ::GetParent (pWnd);

	::GetWindowRect (pWnd, &lRect);
	::ScreenToClient (lParentWnd, &lRect);
	return lRect.TopLeft ();
}

CSize CLayoutTools::ChildWndSize (HWND pWnd)
{
	CRect	lRect;

	::GetWindowRect (pWnd, &lRect);
	return lRect.Size ();
}

CSize CLayoutTools::ChildWndGap (HWND pWnd1, HWND pWnd2)
{
	CRect	lRect1;
	CRect	lRect2;

	::GetWindowRect (pWnd1, &lRect1);
	::GetWindowRect (pWnd2, &lRect2);

	return CSize (max (lRect1.left-lRect2.right, lRect2.left-lRect1.right), max (lRect1.top-lRect2.bottom, lRect2.top-lRect1.bottom));
}

CPoint CLayoutTools::ChildWndMid (HWND pWnd1, HWND pWnd2, HWND pParentWnd)
{
	CPoint	lMidPoint;
	CRect	lRect1;
	CRect	lRect2;
	HWND	lParentWnd = pParentWnd ? pParentWnd : ::GetParent (pWnd1);

	::GetWindowRect (pWnd1, &lRect1);
	::GetWindowRect (pWnd2, &lRect2);

	lMidPoint.x = lRect1.right + (lRect2.left - lRect1.right) / 2;
	lMidPoint.y = lRect1.bottom + (lRect2.top - lRect1.bottom) / 2;

	::ScreenToClient (lParentWnd, &lMidPoint);
	return lMidPoint;
}

//////////////////////////////////////////////////////////////////////

CRect CLayoutTools::ChildUsedRect (HWND pWnd, bool pCheckVisible)
{
	CRect	lRect (-1,-1,-1,-1);
	CRect	lChildRect;
	HWND	lChildWnd;
	bool	lFirst = true;

	for	(lChildWnd = ::GetWindow (pWnd, GW_CHILD); lChildWnd; lChildWnd = ::GetWindow (lChildWnd, GW_HWNDNEXT))
	{
		if	(
				(!pCheckVisible)
			||	(GetWindowStyle (lChildWnd) & WS_VISIBLE)
			)
		{
			::GetWindowRect (lChildWnd, &lChildRect);
			::ScreenToClient (pWnd, &lChildRect);

			if	(lFirst)
			{
				lRect = lChildRect;
				lFirst = false;
			}
			else
			{
				lRect |= lChildRect;
			}
		}
	}

	return lRect;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CLayoutTools::OrderAfter (HWND pOrder, const HWND pOrderAfter)
{
	if	(
			(::IsWindow (pOrder))
		&&	(
				(pOrderAfter == HWND_BOTTOM)
			||	(pOrderAfter == HWND_TOP)
			||	(
					(::IsWindow (pOrderAfter))
				&&	(::GetParent (pOrder) == ::GetParent (pOrderAfter))
				)
			)
		&&	(::SetWindowPos (pOrder, pOrderAfter, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE))
		)
	{
		return true;
	}
	return false;
}

bool CLayoutTools::OrderBefore (HWND pOrder, const HWND pOrderBefore)
{
	if	(
			(::IsWindow (pOrder))
		&&	(::IsWindow (pOrderBefore))
		&&	(::GetParent (pOrder) == ::GetParent (pOrderBefore))
		&&	(::SetWindowPos (pOrder, ::GetWindow (pOrderBefore, GW_HWNDPREV), 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE))
		)
	{
		return true;
	}
	return false;
}
