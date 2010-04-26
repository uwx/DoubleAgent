/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "DaCore.h"
#include "AgentBalloonShape.h"
#include "ImageAlpha.h"
#ifdef	_DEBUG
#include "ImageBuffer.h"
#include "ImageDebugger.h"
#endif

#ifdef	_DEBUG
//#define	_DEBUG_LAYOUT		LogDebug
//#define	_TRACE_LAYOUT		LogDebug
//#define	_DEBUG_DRAW			LogDebug
//#define	_DEBUG_DRAW_PARTS	LogDebug
#endif

#ifndef	PI
#define	PI 3.141592
#endif

/////////////////////////////////////////////////////////////////////////////

CAgentBalloonShape::CAgentBalloonShape()
{
	InitLayout ();
}

CAgentBalloonShape::~CAgentBalloonShape()
{
}

/////////////////////////////////////////////////////////////////////////////

void CAgentBalloonShape::InitLayout ()
{
	DWORD	lDialogUnits = MAKELONG (12,6); //GetDialogBaseUnits ();

	mRounding.cx = MulDiv (LOWORD (lDialogUnits), 2, 1);
	mRounding.cy = MulDiv (HIWORD (lDialogUnits), 1, 1);
	mRounding.cx = mRounding.cy = max (mRounding.cx, mRounding.cy);
	mRounding.cx = mRounding.cy = ((mRounding.cx + 1) / 2) + 1;

	mCalloutSize.cx = MulDiv (LOWORD (lDialogUnits), 1, 2);
	mCalloutSize.cy = MulDiv (LOWORD (lDialogUnits), 2, 3);

	mBalloonRect.SetRectEmpty ();
}

CRect CAgentBalloonShape::RecalcLayout (const CRect & pTextRect, const CRect & pRefRect, const CRect & pBounds)
{
	CRect	lWinRect;

	CalcLayout (pTextRect, pRefRect, pBounds, mBalloonRect, mCalloutBeg, mCalloutEnd);

	lWinRect.left = min (mBalloonRect.left, mCalloutBeg.x);
	lWinRect.right = max (mBalloonRect.right, mCalloutBeg.x+1);
	lWinRect.top = min (mBalloonRect.top, mCalloutBeg.y);
	lWinRect.bottom = max (mBalloonRect.bottom, mCalloutBeg.y+1);

	mBalloonRect.OffsetRect (-lWinRect.left, -lWinRect.top);
	mCalloutBeg.x -= lWinRect.left;
	mCalloutBeg.y -= lWinRect.top;
	mCalloutEnd.x -= lWinRect.left;
	mCalloutEnd.y -= lWinRect.top;
	mTextRect = pTextRect;
	mTextRect.OffsetRect (mBalloonRect.left + mRounding.cx - mTextRect.left, mBalloonRect.top + mRounding.cy - mTextRect.top);

	return lWinRect;
}

/////////////////////////////////////////////////////////////////////////////

void CAgentBalloonShape::CalcLayout (const CRect & pTextRect, const CRect & pRefRect, const CRect & pBounds, CRect & pBalloonRect, CPoint & pCalloutBeg, CPoint & pCalloutEnd)
{
	_complex	lRefSize;
	_complex	lRefCenter;
	_complex	lBalloonSize;
	_complex	lBalloonCenter;
	_complex	lBegPos;
	_complex	lEndPos;

	pBalloonRect = pTextRect;
	pBalloonRect.InflateRect (mRounding.cx, mRounding.cy);
	pBalloonRect.InflateRect (mRounding.cx, mRounding.cy);
	ValidateBalloonRect (pBalloonRect, pRefRect, pBounds);
	pBalloonRect.DeflateRect (mRounding.cx, mRounding.cy);

#ifdef	_TRACE_LAYOUT
	InitTrace (pBalloonRect, pRefRect, pTextRect);
	TraceRect (pBalloonRect, RGB(0xFF,0x80,0x80));
	TraceRect (pRefRect, RGB(0x00,0xFF,0x00));
	TraceRect (pTextRect, RGB(0x00,0xFF,0xFF));
#endif

	lRefSize.x = (double)pRefRect.Width();
	lRefSize.y = (double)pRefRect.Height();
	lRefCenter.x = (double)pRefRect.left + lRefSize.x/2.0 - 0.5;
	lRefCenter.y = (double)pRefRect.top + lRefSize.y/2.0 - 0.5;
	lBalloonSize.x = (double)pBalloonRect.Width();
	lBalloonSize.y = (double)pBalloonRect.Height();
	lBalloonCenter.x = (double)pBalloonRect.left + lBalloonSize.x/2.0 - 0.5;
	lBalloonCenter.y = (double)pBalloonRect.top + lBalloonSize.y/2.0 - 0.5;

#ifdef	_TRACE_LAYOUT
	LogMessage (_TRACE_LAYOUT, _T("Text [%d %d %d %d] Balloon [%d %d %d %d] Ref [%d %d %d %d] Bounds [%d %d %d %d]"), pTextRect.left, pTextRect.top, pTextRect.right, pTextRect.bottom, pBalloonRect.left, pBalloonRect.top, pBalloonRect.right, pBalloonRect.bottom, pRefRect.left, pRefRect.top, pRefRect.right, pRefRect.bottom, pBounds.left, pBounds.top, pBounds.right, pBounds.bottom);
	LogMessage (_TRACE_LAYOUT, _T("  Balloon [%d %d] center [%d %d] Ref [%d %d] center [%d %d]"), dtol(lBalloonSize.x), dtol(lBalloonSize.y), dtol(lBalloonCenter.x), dtol(lBalloonCenter.y), dtol(lRefSize.x), dtol(lRefSize.y), dtol(lRefCenter.x), dtol(lRefCenter.y));
	TracePointFill (lBalloonCenter, RGB(0xFF,0x00,0x00));
	TracePointFill (lRefCenter, RGB(0x00,0xFF,0x00));
#endif

	CalcRectIntersect (lRefCenter, lBalloonCenter, lBalloonSize, lEndPos, 0.5);
	CalcRectIntersect (lEndPos, lRefCenter, lRefSize, lBegPos);

#ifdef	_TRACE_LAYOUT
	TracePointFill (lRefCenter, RGB(0x00,0xFF,0x00));
	TracePointFill (lBegPos, RGB(0x80,0xFF,0x00));
	TracePointFill (lEndPos, RGB(0xFF,0x80,0x00));
#endif

#ifdef	_TRACE_LAYOUT
	double lCalloutLgth = _hypot (lEndPos.x - lBegPos.x, lEndPos.y - lBegPos.y);
	LogMessage (_TRACE_LAYOUT, _T("  Beg [%.1f %.1f] End [%.1f %.1f] Length [%d]"), lBegPos.x, lBegPos.y, lEndPos.x, lEndPos.y, dtol(lCalloutLgth));
#endif

	pBalloonRect = CRect (CPoint (dtolUp(lBalloonCenter.x-lBalloonSize.x/2.0), dtolUp(lBalloonCenter.y-lBalloonSize.y/2.0)), pBalloonRect.Size());
	pCalloutBeg.x = dtol(lBegPos.x);
	pCalloutBeg.y = dtol(lBegPos.y);
	pCalloutEnd.x = dtol(lEndPos.x);
	pCalloutEnd.y = dtol(lEndPos.y);
	FixupNearPoint (pCalloutBeg, pRefRect);
	FixupNearPoint (pCalloutEnd, pBalloonRect);

	if	(
			(pCalloutEnd.x <= pBalloonRect.left + mRounding.cx)
		||	(pCalloutEnd.x > pBalloonRect.right - mRounding.cx)
		)
	{
		pCalloutEnd.y = min (max (pCalloutEnd.y, pBalloonRect.top + mRounding.cy + mCalloutSize.cx*2), pBalloonRect.bottom - mRounding.cy - mCalloutSize.cx*2);
	}
	else
	{
		pCalloutEnd.x = min (max (pCalloutEnd.x, pBalloonRect.left + mRounding.cx + mCalloutSize.cx*2), pBalloonRect.right - mRounding.cx - mCalloutSize.cx*2);
	}

#ifdef	_TRACE_LAYOUT
	LogMessage (_TRACE_LAYOUT, _T("  Text [%d %d %d %d] Balloon [%d %d %d %d] Ref [%d %d %d %d] Bounds [%d %d %d %d]"), pTextRect.left, pTextRect.top, pTextRect.right, pTextRect.bottom, pBalloonRect.left, pBalloonRect.top, pBalloonRect.right, pBalloonRect.bottom, pRefRect.left, pRefRect.top, pRefRect.right, pRefRect.bottom, pBounds.left, pBounds.top, pBounds.right, pBounds.bottom);
	LogMessage (_TRACE_LAYOUT, _T("  Callout Beg [%d %d] End [%d %d]"), pCalloutBeg.x, pCalloutBeg.y, pCalloutEnd.x, pCalloutEnd.y);
	TraceRect (pBalloonRect, RGB(0xFF,0xFF,0xFF));
	TracePointFrame (pCalloutBeg, RGB(0xFF,0xFF,0xFF));
	TracePointFrame (pCalloutEnd, RGB(0xFF,0xFF,0xFF));
	ShowTrace ();
#endif
}

/////////////////////////////////////////////////////////////////////////////
//
//	Calculate where a line from RefCenter to RectCenter intersects
//	a rectangle of RectSize centered on RectCenter
//
/////////////////////////////////////////////////////////////////////////////

void CAgentBalloonShape::CalcRectIntersect (const _complex & pRefPoint, const _complex & pRectCenter, const _complex & pRectSize, _complex & pRectIntersect, double pMinAngle)
{
	_complex	lRefPoint = {pRefPoint.x, pRefPoint.y};
	_complex	lRectHalf = {pRectSize.x / 2.0, pRectSize.y / 2.0};
	double		lAngle = atan2 (pRectCenter.y - lRefPoint.y, pRectCenter.x - lRefPoint.x);

#ifdef	_TRACE_LAYOUT
	LogMessage (_TRACE_LAYOUT, _T("  Point [%d %d] to center [%d %d] = Angle [%f]"), (int)lRefPoint.x, (int)lRefPoint.y, (int)pRectCenter.x, (int)pRectCenter.y, lAngle);
#endif
	if	(pMinAngle > 0.0)
	{
		double	lLength = _hypot (pRectCenter.y - lRefPoint.y, pRectCenter.x - lRefPoint.x);

		if	(fabs (lAngle) <= pMinAngle)
		{
			lAngle = 0.0;
		}
		else
		if	(fabs (_copysign (PI/2.0, lAngle) - lAngle) <= pMinAngle)
		{
			lAngle = _copysign (PI/2.0, lAngle);
		}
		if	(fabs (_copysign (PI, lAngle) - lAngle) <= pMinAngle)
		{
			lAngle = _copysign (PI, lAngle);
		}

		lRefPoint.x = pRectCenter.x - cos (lAngle) * lLength;
		lRefPoint.y = pRectCenter.y - sin (lAngle) * lLength;
#ifdef	_TRACE_LAYOUT
		LogMessage (_TRACE_LAYOUT, _T("  Point [%d %d] to center [%d %d] = Angle [%f] Min [%f]"), (int)lRefPoint.x, (int)lRefPoint.y, (int)pRectCenter.x, (int)pRectCenter.y, lAngle, pMinAngle);
#endif
	}

	if	(lRefPoint.x == pRectCenter.x)
	{
		pRectIntersect.x = 0.0;
		pRectIntersect.y = _copysign (lRectHalf.y, lRefPoint.y - pRectCenter.y);
	}
	else
	if	(lRefPoint.y == pRectCenter.y)
	{
		pRectIntersect.x = _copysign (lRectHalf.x, lRefPoint.x - pRectCenter.x);
		pRectIntersect.y = 0.0;
	}
	else
	{
		pRectIntersect.x = _copysign (lRectHalf.y / tan (lAngle), lRefPoint.x - pRectCenter.x);
		pRectIntersect.y = _copysign (tan (lAngle) * lRectHalf.x, lRefPoint.y - pRectCenter.y);
	}

	if	(fabs (pRectIntersect.x) > lRectHalf.x)
	{
		pRectIntersect.x = _copysign (lRectHalf.x, pRectIntersect.x);
	}
	if	(fabs (pRectIntersect.y) > lRectHalf.y)
	{
		pRectIntersect.y = _copysign (lRectHalf.y, pRectIntersect.y);
	}

	pRectIntersect.x += pRectCenter.x;
	pRectIntersect.y += pRectCenter.y;
}

/////////////////////////////////////////////////////////////////////////////
//
//	Ensure that the balloon rectangle is within the boundaries and
//	the proper distance from the reference rectangle
//
/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonShape::ValidateBalloonRect (CRect & pBalloonRect, const CRect & pRefRect, const CRect & pBounds)
{
	bool	lRet = false;
	CRect	lRefRect (pRefRect);
	CRect	lBounds (pBounds);
	CRect	lIntersect;
	CPoint	lCenter = pBalloonRect.CenterPoint();
	CPoint	lRefCenter = pRefRect.CenterPoint();

	lRefRect.InflateRect (mCalloutSize.cy, mCalloutSize.cy);
	lRefRect.InflateRect (pBalloonRect.Width(), pBalloonRect.Height());
	lBounds.IntersectRect (lBounds, lRefRect);
	lRefRect.DeflateRect (pBalloonRect.Width(), pBalloonRect.Height());

	if	(pBalloonRect.right > lBounds.right)
	{
		pBalloonRect.OffsetRect (lBounds.right - pBalloonRect.right, 0);
		lRet = true;
	}
	if	(pBalloonRect.bottom > lBounds.bottom)
	{
		pBalloonRect.OffsetRect (0, lBounds.bottom - pBalloonRect.bottom);
		lRet = true;
	}
	if	(pBalloonRect.left < lBounds.left)
	{
		pBalloonRect.OffsetRect (lBounds.left - pBalloonRect.left, 0);
		lRet = true;
	}
	if	(pBalloonRect.top < lBounds.top)
	{
		pBalloonRect.OffsetRect (0, lBounds.top - pBalloonRect.top);
		lRet = true;
	}

	if	(lIntersect.IntersectRect (&pBalloonRect, &lRefRect))
	{
		bool	lOffsetVertical = (labs (lCenter.x - lRefCenter.x) <= labs (lCenter.y - lRefCenter.y));
		bool	lDidVertical = lOffsetVertical;
		bool	lMoved = false;

		while	(!lMoved)
		{
			if	(
					(lOffsetVertical)
				&&	(lCenter.y >= lRefCenter.y)
				&&	(lRefRect.bottom >= lBounds.top)
				&&	(pBalloonRect.bottom + lRefRect.bottom - pBalloonRect.top <= lBounds.bottom)
				)
			{
				pBalloonRect.OffsetRect (0, lRefRect.bottom - pBalloonRect.top);
				lMoved = true;
			}
			else
			if	(
					(lOffsetVertical)
				&&	(lCenter.y <= lRefCenter.y)
				&&	(lRefRect.top <= lBounds.bottom)
				&&	(pBalloonRect.top + lRefRect.top - pBalloonRect.bottom >= lBounds.top)
				)
			{
				pBalloonRect.OffsetRect (0, lRefRect.top - pBalloonRect.bottom);
				lMoved = true;
			}
			else
			if	(
					(!lOffsetVertical)
				&&	(lCenter.x >= lRefCenter.x)
				&&	(lRefRect.right >= pBalloonRect.left)
				&&	(pBalloonRect.right + lRefRect.right - pBalloonRect.left <= lBounds.right)
				)
			{
				pBalloonRect.OffsetRect (lRefRect.right - pBalloonRect.left, 0);
				lMoved = true;
			}
			else
			if	(
					(!lOffsetVertical)
				&&	(lCenter.x <= lRefCenter.x)
				&&	(lRefRect.left <= lBounds.right)
				&&	(pBalloonRect.left + lRefRect.left - pBalloonRect.right >= lBounds.left)
				)
			{
				pBalloonRect.OffsetRect (lRefRect.left - pBalloonRect.right, 0);
				lMoved = true;
			}

			if	(lMoved)
			{
				break;
			}
			else
			if	(lDidVertical == lOffsetVertical)
			{
				lOffsetVertical = !lOffsetVertical;
			}
			else
			if	(
					(lRefCenter.x != lCenter.x)
				||	(lRefCenter.y != lCenter.y)
				)
			{
				lRefCenter = lCenter;
				lOffsetVertical = !lOffsetVertical;
			}
			else
			{
				break;
			}
		}
		if	(lMoved)
		{
			lRet = true;
		}
	}
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
//
//	Account for rounding errors by moving a point near a rectangle boundary
//	onto the boundary
//
/////////////////////////////////////////////////////////////////////////////

void CAgentBalloonShape::FixupNearPoint (CPoint & pPoint, const CRect & pRefRect, long pNearness)
{
	if	(labs (pPoint.x - pRefRect.left) <= pNearness)
	{
		pPoint.x = pRefRect.left;
	}
	else
	if	(labs (pPoint.x - (pRefRect.right - 1)) <= pNearness)
	{
		pPoint.x = pRefRect.right - 1;
	}

	if	(labs (pPoint.y - pRefRect.top) <= pNearness)
	{
		pPoint.y = pRefRect.top;
	}
	else
	if	(labs (pPoint.y - (pRefRect.bottom - 1)) <= pNearness)
	{
		pPoint.y = pRefRect.bottom - 1;
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAgentBalloonSpeak::CAgentBalloonSpeak()
{
	InitLayout ();
}

CAgentBalloonSpeak::~CAgentBalloonSpeak()
{
}

/////////////////////////////////////////////////////////////////////////////

void CAgentBalloonSpeak::InitLayout ()
{
	CAgentBalloonShape::InitLayout ();

#ifdef	_TRACE_LAYOUT
	mCalloutSize.cx = MulDiv (mCalloutSize.cx, 5, 4);
	mCalloutSize.cy = MulDiv (mCalloutSize.cy, 4, 1);
#endif
#ifdef	_DEBUG_LAYOUT
	LogMessage (_DEBUG_LAYOUT, _T("%s Rounding [%d %d] Callout [%d %d]"), ObjClassName(this), mRounding.cx, mRounding.cy, mCalloutSize.cx, mCalloutSize.cy);
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRGN CAgentBalloonSpeak::GetBalloonRgn (UINT pScale)
{
	int			lScale = max ((int)pScale, 1);
	CRgnHandle	lRgn1;
	CRgnHandle	lRgn2;
	CPoint		lPoints [3];

	if	(!mBalloonRect.IsRectEmpty())
	{
#ifdef	_DEBUG_DRAW
		CRect	lWinRect;

		lWinRect.left = min (mBalloonRect.left, mCalloutBeg.x);
		lWinRect.right = max (mBalloonRect.right, mCalloutBeg.x+1);
		lWinRect.top = min (mBalloonRect.top, mCalloutBeg.y);
		lWinRect.bottom = max (mBalloonRect.bottom, mCalloutBeg.y+1);
		lRgn1.CreateRectRgnIndirect (lWinRect);
#else
		GetCalloutPoints (lPoints, pScale);
		lRgn1 = ::CreateRoundRectRgn (mBalloonRect.left*lScale, mBalloonRect.top*lScale, ((mBalloonRect.right-1)*lScale)+1, ((mBalloonRect.bottom-1)*lScale)+1, mRounding.cx*lScale, mRounding.cy*lScale);
		lRgn2 = ::CreatePolygonRgn (lPoints, 3, WINDING);
		CombineRgn (lRgn1, lRgn1, lRgn2, RGN_OR);
		lRgn2.Close ();
#endif
	}
	return lRgn1.Detach();
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonSpeak::Draw (HDC pDC, COLORREF pBkColor, COLORREF pBrColor, UINT pScale)
{
	if	(!mBalloonRect.IsRectEmpty())
	{
		int				lScale = max ((int)pScale, 1);
		CBrushHandle	lFillBrush;
		CBrushHandle	lFrameBrush;

		lFillBrush = CImageAlpha::GetAlphaBrush (pBkColor, 255);
		lFrameBrush = CImageAlpha::GetAlphaBrush (pBrColor, 255);

#ifdef	_DEBUG_DRAW
		{
			CRect lClipRect;
			CBrush lBrush;
			lBrush.CreateSolidBrush (RGB (0xFF, 0x80, 0xFF));
			::GetClipBox (pDC, &lClipRect);
			::FillRect (pDC, &lClipRect, lBrush);
			lBrush.DeleteObject ();
		}
#endif

#ifdef	_DEBUG_DRAW_PARTS
		CPen	lPen;
		HGDIOBJ	lOldBrush = NULL;
		HGDIOBJ	lOldPen = NULL;
		CPoint	lPoints [3];

		lPen.CreatePen (PS_SOLID|PS_INSIDEFRAME, 1, pBrColor);
		lOldBrush = ::SelectObject (pDC, lFillBrush.GetSafeHandle());
		lOldPen = ::SelectObject (pDC, lPen.GetSafeHandle());

		::SetBkMode (pDC, OPAQUE);
		::SetBkColor (pDC, pBkColor);
		::SetTextColor (pDC, pBrColor);
		::RoundRect (pDC, mBalloonRect.left*lScale, mBalloonRect.top*lScale, mBalloonRect.right*lScale, mBalloonRect.bottom*lScale, mRounding.cx*lScale, mRounding.cy*lScale);

		GetCalloutPoints (lPoints, pScale);
		::SetPolyFillMode (pDC, WINDING);
		::Polygon (pDC, lPoints, 3);

		::SelectObject (pDC, lOldBrush);
		::SelectObject (pDC, lOldPen);
#else
		CRgnHandle	lRgn = GetBalloonRgn (pScale);

		::FillRgn (pDC, lRgn, lFillBrush);
		::FrameRgn (pDC, lRgn, lFrameBrush, lScale, lScale);
#endif


#ifdef	_DEBUG_DRAW
		{
			CBrush lBrush;
			lBrush.CreateSolidBrush (RGB (0xFF,0x00,0x00));
			::FillRect (pDC, CRect (mCalloutEnd.x*lScale-1, mCalloutEnd.y*lScale-1, mCalloutEnd.x*lScale+2, mCalloutEnd.y*lScale+2), lBrush);
		}
#endif
		return true;
	}
	return false;
}

void CAgentBalloonSpeak::GetCalloutPoints (CPoint * pPoints, UINT pScale)
{
	int		lScale = max ((int)pScale, 1);
	CRect	lBalloonRect (mBalloonRect.left*lScale, mBalloonRect.top*lScale, ((mBalloonRect.right-1)*lScale)+1, ((mBalloonRect.bottom-1)*lScale)+1);
	double	lCalloutAngle;
	double	lCalloutWidth;
	double	lCalloutExtension;
	INT_PTR	lPointNdx;
	INT_PTR	lExtendCount;
	CPoint	lExtendPoint;

	lCalloutAngle = atan2 ((double)(mCalloutEnd.y-mCalloutBeg.y), (double)(mCalloutEnd.x-mCalloutBeg.x));
	lCalloutWidth = (double)(mCalloutSize.cx*lScale);
	lCalloutExtension = lCalloutWidth * 2.0;

	pPoints [0].x = mCalloutBeg.x*lScale;
	pPoints [0].y = mCalloutBeg.y*lScale;
	pPoints [1].x = mCalloutEnd.x*lScale + (long)(cos (lCalloutAngle + PI/2.0) * lCalloutWidth);
	pPoints [1].y = mCalloutEnd.y*lScale + (long)(sin (lCalloutAngle + PI/2.0) * lCalloutWidth);
	pPoints [2].x = mCalloutEnd.x*lScale + (long)(cos (lCalloutAngle - PI/2.0) * lCalloutWidth);
	pPoints [2].y = mCalloutEnd.y*lScale + (long)(sin (lCalloutAngle - PI/2.0) * lCalloutWidth);

	for	(lPointNdx = 1; lPointNdx <= 2; lPointNdx++)
	{
		if	(!PtInRect (lBalloonRect, pPoints [lPointNdx]))
		{
			lExtendPoint = pPoints [lPointNdx];
			lCalloutAngle = atan2 ((double)(lExtendPoint.y-mCalloutBeg.y*lScale), (double)(lExtendPoint.x-mCalloutBeg.x*lScale));

			for	(lExtendCount = 1; lExtendCount <= 10; lExtendCount++)
			{
				pPoints [lPointNdx].x = lExtendPoint.x + (long)(cos (lCalloutAngle) * lCalloutExtension * (double)lExtendCount);
				pPoints [lPointNdx].y = lExtendPoint.y + (long)(sin (lCalloutAngle) * lCalloutExtension * (double)lExtendCount);

				if	(PtInRect (lBalloonRect, pPoints [lPointNdx]))
				{
					break;
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAgentBalloonThink::CAgentBalloonThink()
{
	InitLayout ();
}

CAgentBalloonThink::~CAgentBalloonThink()
{
}

/////////////////////////////////////////////////////////////////////////////

void CAgentBalloonThink::InitLayout ()
{
	CAgentBalloonShape::InitLayout ();

	mCalloutSize.cx = MulDiv (mCalloutSize.cx, 5, 4);
	mCalloutSize.cy = MulDiv (mCalloutSize.cy, 4, 1);
#ifdef	_DEBUG_LAYOUT
	LogMessage (_DEBUG_LAYOUT, _T("%s Rounding [%d %d] Callout [%d %d]"), ObjClassName(this), mRounding.cx, mRounding.cy, mCalloutSize.cx, mCalloutSize.cy);
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

HRGN CAgentBalloonThink::GetBalloonRgn (UINT pScale)
{
	int			lScale = max ((int)pScale, 1);
	CRgnHandle	lRgn1;
	CRgnHandle	lRgn2;

	if	(!mBalloonRect.IsRectEmpty())
	{
#ifdef	_DEBUG_DRAW
		CRect	lWinRect;

		lWinRect.left = min (mBalloonRect.left, mCalloutBeg.x);
		lWinRect.right = max (mBalloonRect.right, mCalloutBeg.x+1);
		lWinRect.top = min (mBalloonRect.top, mCalloutBeg.y);
		lWinRect.bottom = max (mBalloonRect.bottom, mCalloutBeg.y+1);
		lRgn1.CreateRectRgnIndirect (lWinRect);
#else
		CRect	lEllipses [3];
		INT_PTR	lEllipseNdx;

		GetCalloutEllipses (lEllipses, pScale);
		lRgn1 = ::CreateRoundRectRgn (mBalloonRect.left*lScale, mBalloonRect.top*lScale, ((mBalloonRect.right-1)*lScale)+1, ((mBalloonRect.bottom-1)*lScale)+1, mRounding.cx*lScale, mRounding.cy*lScale);

		for	(lEllipseNdx = 0; lEllipseNdx < 3; lEllipseNdx++)
		{
			lRgn2 = ::CreateEllipticRgn (lEllipses [lEllipseNdx].left, lEllipses [lEllipseNdx].top, lEllipses [lEllipseNdx].right+2-lScale, lEllipses [lEllipseNdx].bottom+2-lScale);
			::CombineRgn (lRgn1, lRgn1, lRgn2, RGN_OR);
			lRgn2.Close ();
		}
#endif
	}
	return lRgn1.Detach();
}

/////////////////////////////////////////////////////////////////////////////

bool CAgentBalloonThink::Draw (HDC pDC, COLORREF pBkColor, COLORREF pBrColor, UINT pScale)
{
	if	(!mBalloonRect.IsRectEmpty())
	{
		int				lScale = max ((int)pScale, 1);
		CBrushHandle	lFillBrush;
		CBrushHandle	lFrameBrush;

		lFillBrush = CImageAlpha::GetAlphaBrush (pBkColor, 255);
		lFrameBrush = CImageAlpha::GetAlphaBrush (pBrColor, 255);

#ifdef	_DEBUG_DRAW
		{
			CRect lClipRect;
			CBrush lBrush;
			lBrush.CreateSolidBrush (RGB (0xFF, 0x80, 0xFF));
			::GetClipBox (pDC, &lClipRect);
			::FillRect (pDC, &lClipRect, lBrush);
			lBrush.DeleteObject ();
		}
#endif

#ifdef	_DEBUG_DRAW_PARTS
		CPen	lPen;
		HGDIOBJ	lOldBrush = NULL;
		HGDIOBJ	lOldPen = NULL;
		CRect	lEllipses [3];
		INT_PTR	lNdx;

		lPen.CreatePen (PS_SOLID|PS_INSIDEFRAME, 1, pBrColor);
		lOldBrush = ::SelectObject (pDC, lFillBrush.GetSafeHandle());
		lOldPen = ::SelectObject (pDC, lPen.GetSafeHandle());

		::SetBkMode (pDC, OPAQUE);
		::SetBkColor (pDC, pBkColor);
		::SetTextColor (pDC, pBrColor);
		::RoundRect (pDC, mBalloonRect.left*lScale, mBalloonRect.top*lScale, mBalloonRect.right*lScale, mBalloonRect.bottom*lScale, mRounding.cx*lScale, mRounding.cy*lScale);

		GetCalloutEllipses (lEllipses, pScale);
		for	(lNdx = 0; lNdx < 3; lNdx++)
		{
			::Ellipse (pDC, lEllipses [lNdx].left, lEllipses [lNdx].top, lEllipses [lNdx].right, lEllipses [lNdx].bottom);
		}

		::SelectObject (pDC, lOldBrush);
		::SelectObject (pDC, lOldPen);
#else
		CRgnHandle	lRgn = GetBalloonRgn (pScale);

		::FillRgn (pDC, lRgn, lFillBrush);
		::FrameRgn (pDC, lRgn, lFrameBrush, lScale, lScale);
#endif


#ifdef	_DEBUG_DRAW
		{
			CBrush lBrush;
			lBrush.CreateSolidBrush (RGB (0xFF,0x00,0x00));
			::FillRect (pDC, CRect (mCalloutEnd.x*lScale-1, mCalloutEnd.y*lScale-1, mCalloutEnd.x*lScale+2, mCalloutEnd.y*lScale+2), lBrush);
		}
#endif
		return true;
	}
	return false;
}

void CAgentBalloonThink::GetCalloutEllipses (CRect * pEllipses, UINT pScale)
{
	int			lScale = max ((int)pScale, 1);
	double		lCalloutAngle = atan2 ((double)(mCalloutEnd.y-mCalloutBeg.y), (double)(mCalloutEnd.x-mCalloutBeg.x));
	double		lCalloutWidth = (double)(mCalloutSize.cx*lScale);
	double		lCalloutLength = _hypot ((double)(mCalloutEnd.y*lScale-mCalloutBeg.y*lScale), (double)(mCalloutEnd.x*lScale-mCalloutBeg.x*lScale));
	double		lEllipseRadius [3] = {lCalloutWidth * 0.6, lCalloutWidth * 0.75, lCalloutWidth};
	double		lEllipseOffset [3] = {lCalloutLength * 0.12, lCalloutLength * 0.4, lCalloutLength * 0.75};
	_complex	lEllipseCenter;
	INT_PTR		lEllipseNdx;

	for	(lEllipseNdx = 0; lEllipseNdx < 3; lEllipseNdx++)
	{
		lEllipseCenter.x = (cos (lCalloutAngle) * lEllipseOffset [lEllipseNdx]);
		lEllipseCenter.y = (sin (lCalloutAngle) * lEllipseOffset [lEllipseNdx]);

		pEllipses [lEllipseNdx].left = dtol (lEllipseCenter.x - lEllipseRadius [lEllipseNdx]);
		pEllipses [lEllipseNdx].top = dtol (lEllipseCenter.y - lEllipseRadius [lEllipseNdx]);
		pEllipses [lEllipseNdx].right = pEllipses [lEllipseNdx].left + dtolOdd (lEllipseRadius [lEllipseNdx] * 2.0) + 1L;
		pEllipses [lEllipseNdx].bottom = pEllipses [lEllipseNdx].top + dtolOdd (lEllipseRadius [lEllipseNdx] * 2.0) + 1L;

		pEllipses [lEllipseNdx].OffsetRect (mCalloutBeg.x*lScale, mCalloutBeg.y*lScale);
	}
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
/////////////////////////////////////////////////////////////////////////////

void CAgentBalloonShape::InitTrace (const CRect & pRect)
{
#ifdef	_TRACE_LAYOUT
	if	(mTraceBuffer = new CImageBuffer)
	{
		mTraceBuffer->CreateBuffer (pRect.Size()+CSize(4,4));
		OffsetWindowOrgEx (*mTraceBuffer->mDC, pRect.left-2, pRect.top-2, NULL);
	}
#endif
}

void CAgentBalloonShape::InitTrace (const CRect & pRect1, const CRect & pRect2)
{
#ifdef	_TRACE_LAYOUT
	CRect	lBufferRect;

	lBufferRect.UnionRect (pRect1, pRect2);
	InitTrace (lBufferRect);
#endif
}

void CAgentBalloonShape::InitTrace (const CRect & pRect1, const CRect & pRect2, const CRect & pRect3)
{
#ifdef	_TRACE_LAYOUT
	CRect	lBufferRect;

	lBufferRect.UnionRect (pRect1, pRect2);
	lBufferRect.UnionRect (lBufferRect, pRect3);
	InitTrace (lBufferRect);
#endif
}

void CAgentBalloonShape::ShowTrace ()
{
#ifdef	_TRACE_LAYOUT
	if	(mTraceBuffer)
	{
		mTraceBuffer->EndBuffer ();

		if	(
				(mTraceDebugger)
			||	(mTraceDebugger = new CImageDebugger)
			)
		{
			mTraceDebugger->ShowBitmap (*mTraceBuffer->mImage, 1, _T("Calc"));
		}
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CAgentBalloonShape::TraceRect (const CRect & pRect, COLORREF pColor) const
{
#ifdef	_TRACE_LAYOUT
	if	(mTraceBuffer)
	{
		CBrushHandle lBrush = ::CreateSolidBrush (pColor);
		FrameRect (*mTraceBuffer->mDC, pRect, lBrush);
	}
#endif
}

void CAgentBalloonShape::TracePointFill (const CPoint & pPoint, COLORREF pColor, UINT pSize) const
{
#ifdef	_TRACE_LAYOUT
	if	(mTraceBuffer)
	{
		FillSolidRect (*mTraceBuffer->mDC, CRect (pPoint.x-pSize, pPoint.y-pSize, pPoint.x+pSize+1, pPoint.y+pSize+1), pColor);
	}
#endif
}

void CAgentBalloonShape::TracePointFill (const _complex & pPoint, COLORREF pColor, UINT pSize) const
{
#ifdef	_TRACE_LAYOUT
	TracePointFill (CPoint (dtol(pPoint.x), dtol(pPoint.y)), pColor, pSize);
#endif
}

void CAgentBalloonShape::TracePointFrame (const CPoint & pPoint, COLORREF pColor, UINT pSize) const
{
#ifdef	_TRACE_LAYOUT
	if	(mTraceBuffer)
	{
		CBrushHandle lBrush = ::CreateSolidBrush (pColor);
		FrameRect (*mTraceBuffer->mDC, CRect (pPoint.x-pSize, pPoint.y-pSize, pPoint.x+pSize+1, pPoint.y+pSize+1), lBrush);
	}
#endif
}

void CAgentBalloonShape::TracePointFrame (const _complex & pPoint, COLORREF pColor, UINT pSize) const
{
#ifdef	_TRACE_LAYOUT
	TracePointFrame (CPoint (dtol(pPoint.x), dtol(pPoint.y)), pColor, pSize);
#endif
}

/////////////////////////////////////////////////////////////////////////////
#endif	// _DEBUG
/////////////////////////////////////////////////////////////////////////////
