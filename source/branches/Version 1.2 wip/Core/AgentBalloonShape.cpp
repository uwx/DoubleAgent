/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
#include "AgentBalloonShape.h"
#ifdef	__cplusplus_cli
using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;
using namespace System::Drawing::Drawing2D;
#else
#include "ImageAlpha.h"
#include "ImageBuffer.h"
#include "ImageTools.h"
#include "DebugStr.h"
#ifdef	_DEBUG
#include "ImageDebugger.h"
#endif

#ifndef	PI
#define	PI 3.141592
#endif
#endif

#ifdef	_DEBUG
//#define	_DEBUG_LAYOUT		LogDebug
//#define	_TRACE_LAYOUT		LogDebug
//#define	_DEBUG_DRAW			LogDebug
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
namespace DoubleAgent {
#endif
/////////////////////////////////////////////////////////////////////////////

CAgentBalloonShape::CAgentBalloonShape()
{
#ifdef	__cplusplus_cli
	mShadowOffset.X = 3;
	mShadowOffset.Y = 3;
#else
	mShadowOffset.x = mShadowOffset.y = 3;
#endif
	InitLayout ();
}

#ifndef	__cplusplus_cli
CAgentBalloonShape::~CAgentBalloonShape()
{
}
#endif

/////////////////////////////////////////////////////////////////////////////

void CAgentBalloonShape::InitLayout ()
{
#ifdef	__cplusplus_cli
	mRounding.Width = 7;
	mRounding.Height = 7;
	mCalloutSize.Width = 8;
	mCalloutSize.Height = 8;
#else
	DWORD	lDialogUnits = MAKELONG (12,6); //GetDialogBaseUnits ();

	mRounding.cx = MulDiv (LOWORD (lDialogUnits), 2, 1);
	mRounding.cy = MulDiv (HIWORD (lDialogUnits), 1, 1);
	mRounding.cx = mRounding.cy = max (mRounding.cx, mRounding.cy);
	mRounding.cx = mRounding.cy = ((mRounding.cx + 1) / 2) + 1;

	mCalloutSize.cx = MulDiv (LOWORD (lDialogUnits), 1, 3);
	mCalloutSize.cy = MulDiv (LOWORD (lDialogUnits), 2, 3);

	mBalloonRect.SetRectEmpty ();
#endif
}

#ifdef	__cplusplus_cli
System::Drawing::SizeF CAgentBalloonShape::Rounding::get()
{
	return mRounding;
}

System::Drawing::SizeF CAgentBalloonShape::LayoutMargin::get()
{
	return mCalloutSize;
}
#else
CSize CAgentBalloonShape::GetRounding () const
{
	return mRounding;
}

CSize CAgentBalloonShape::GetLayoutMargin () const
{
	return mCalloutSize;
}
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
System::Drawing::RectangleF CAgentBalloonShape::RecalcLayout (System::Drawing::RectangleF pTextRect, System::Drawing::RectangleF pRefRect, System::Drawing::RectangleF pBounds)
#else
CRect CAgentBalloonShape::RecalcLayout (const CRect& pTextRect, const CRect& pRefRect, const CRect& pBounds)
#endif
{
#ifdef	_TRACE_LAYOUT
	try
	{
#ifdef	__cplusplus_cli
		LogMessage (_TRACE_LAYOUT, _T("RecalcLayout Text [%.1f %.1f %.1f %.1f] Ref [%.1f %.1f %.1f %.1f] Bounds [%.1f %.1f %.1f %.1f]"), pTextRect.Left, pTextRect.Top, pTextRect.Right, pTextRect.Bottom, pRefRect.Left, pRefRect.Top, pRefRect.Right, pRefRect.Bottom, pBounds.Left, pBounds.Top, pBounds.Right, pBounds.Bottom);
#else
		LogMessage (_TRACE_LAYOUT, _T("RecalcLayout Text [%d %d %d %d] Ref [%d %d %d %d] Bounds [%d %d %d %d]"), pTextRect.left, pTextRect.top, pTextRect.right, pTextRect.bottom, pRefRect.left, pRefRect.top, pRefRect.right, pRefRect.bottom, pBounds.left, pBounds.top, pBounds.right, pBounds.bottom);
#endif	
	}
	catch AnyExceptionSilent
#endif	

	CalcLayout (pTextRect, pRefRect, pBounds, mBalloonRect, mCalloutBeg, mCalloutEnd);

#ifdef	__cplusplus_cli
	mBounds.X = min (mBalloonRect.Left, mCalloutBeg.X);
	mBounds.Y = min (mBalloonRect.Top, mCalloutBeg.Y);
	mBounds.Width = max (mBalloonRect.Width, mCalloutBeg.X-mBounds.X);
	mBounds.Height = max (mBalloonRect.Height, mCalloutBeg.Y-mBounds.Y);

	mBalloonRect.Offset (-mBounds.Left, -mBounds.Top);
	mCalloutBeg.X -= mBounds.Left;
	mCalloutBeg.Y -= mBounds.Top;
	mCalloutEnd.X -= mBounds.Left;
	mCalloutEnd.Y -= mBounds.Top;
	mTextRect = pTextRect;
	mTextRect.Offset (mBalloonRect.Left + mRounding.Width - mTextRect.Left, mBalloonRect.Top + mRounding.Height - mTextRect.Top);

	mBounds.Width += mShadowOffset.X*2;
	mBounds.Height += mShadowOffset.Y*2;
#else
	mBounds.left = min (mBalloonRect.left, mCalloutBeg.x);
	mBounds.top = min (mBalloonRect.top, mCalloutBeg.y);
	mBounds.right = max (mBalloonRect.right, mCalloutBeg.x+1);
	mBounds.bottom = max (mBalloonRect.bottom, mCalloutBeg.y+1);

	mBalloonRect.OffsetRect (-mBounds.left, -mBounds.top);
	mCalloutBeg.x -= mBounds.left;
	mCalloutBeg.y -= mBounds.top;
	mCalloutEnd.x -= mBounds.left;
	mCalloutEnd.y -= mBounds.top;
	mTextRect = pTextRect;
	mTextRect.OffsetRect (mBalloonRect.left + mRounding.cx - mTextRect.left, mBalloonRect.top + mRounding.cy - mTextRect.top);

	mBounds.right += mShadowOffset.x*2;
	mBounds.bottom += mShadowOffset.y*2;
#endif

#ifdef	_TRACE_LAYOUT
	try
	{
#ifdef	__cplusplus_cli
		LogMessage (_TRACE_LAYOUT, _T("RecalcLayout Text [%.1f %.1f %.1f %.1f] Balloon [%.1f %.1f %.1f %.1f] Bounds [%.1f %.1f %.1f %.1f] Callout Beg [%.1f %.1f] End [%.1f %.1f]"), mTextRect.Left, mTextRect.Top, mTextRect.Right, mTextRect.Bottom, mBalloonRect.Left, mBalloonRect.Top, mBalloonRect.Right, mBalloonRect.Bottom, mBounds.Left, mBounds.Top, mBounds.Right, mBounds.Bottom, mCalloutBeg.X, mCalloutBeg.Y, mCalloutEnd.X, mCalloutEnd.Y);
#else
		LogMessage (_TRACE_LAYOUT, _T("RecalcLayout Text [%d %d %d %d] Balloon [%d %d %d %d] Bounds [%d %d %d %d] Callout Beg [%d %d] End [%d %d]"), mTextRect.left, mTextRect.top, mTextRect.right, mTextRect.bottom, mBalloonRect.left, mBalloonRect.top, mBalloonRect.right, mBalloonRect.bottom, mBounds.left, mBounds.top, mBounds.right, mBounds.bottom, mCalloutBeg.x, mCalloutBeg.y, mCalloutEnd.x, mCalloutEnd.y);
#endif	
	}
	catch AnyExceptionSilent
#endif	

	return mBounds;
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
void CAgentBalloonShape::CalcLayout (System::Drawing::RectangleF pTextRect, System::Drawing::RectangleF pRefRect, System::Drawing::RectangleF pBounds, System::Drawing::RectangleF% pBalloonRect, System::Drawing::PointF% pCalloutBeg, System::Drawing::PointF% pCalloutEnd)
#else
void CAgentBalloonShape::CalcLayout (const CRect& pTextRect, const CRect& pRefRect, const CRect& pBounds, CRect& pBalloonRect, CPoint& pCalloutBeg, CPoint& pCalloutEnd)
#endif
{
#ifdef	__cplusplus_cli
	SizeF		lRefSize;
	PointF		lRefCenter;
	PointF		lRefCenterOffset;
	SizeF		lBalloonSize;
	PointF		lBalloonCenter;
	PointF		lBalloonCenterOffset;
#else
	_complex	lRefSize;
	_complex	lRefCenter;
	_complex	lRefCenterOffset;
	_complex	lBalloonSize;
	_complex	lBalloonCenter;
	_complex	lBalloonCenterOffset;
	_complex	lBegPos;
	_complex	lEndPos;
#endif

#ifdef	__cplusplus_cli
	pBalloonRect = pTextRect;
	pBalloonRect.Inflate (mRounding.Width, mRounding.Height);
	pBalloonRect.Inflate (mRounding.Width, mRounding.Height);
	ValidateBalloonRect (pBalloonRect, pRefRect, pBounds);
	pBalloonRect.Inflate (-mRounding.Width, -mRounding.Height);
#else
	pBalloonRect = pTextRect;
	pBalloonRect.InflateRect (mRounding.cx, mRounding.cy);
	pBalloonRect.InflateRect (mRounding.cx, mRounding.cy);
	ValidateBalloonRect (pBalloonRect, pRefRect, pBounds);
	pBalloonRect.DeflateRect (mRounding.cx, mRounding.cy);

#ifdef	_TRACE_LAYOUT
	try
	{
		InitTrace (pBalloonRect, pRefRect, pTextRect);
		TraceRect (pBalloonRect, RGB(0xFF,0x80,0x80));
		TraceRect (pRefRect, RGB(0x00,0xFF,0x00));
		TraceRect (pTextRect, RGB(0x00,0xFF,0xFF));
	}
	catch AnyExceptionSilent
#endif
#endif

#ifdef	__cplusplus_cli
	lRefSize.Width = pRefRect.Width;
	lRefSize.Height = pRefRect.Height;
	lRefCenterOffset.X = lRefSize.Width*0.5f;
	lRefCenterOffset.Y = lRefSize.Height*0.5f;
	lRefCenter.X = pRefRect.X + lRefCenterOffset.X;
	lRefCenter.Y = pRefRect.Y + lRefCenterOffset.Y;
	lBalloonSize.Width = pBalloonRect.Width;
	lBalloonSize.Height = pBalloonRect.Height;
	lBalloonCenterOffset.X = lBalloonSize.Width*0.5f;
	lBalloonCenterOffset.Y = lBalloonSize.Height*0.5f;
	lBalloonCenter.X = pBalloonRect.X + lBalloonCenterOffset.X;
	lBalloonCenter.Y = pBalloonRect.Y + lBalloonCenterOffset.Y;

#ifdef	_TRACE_LAYOUT
	try
	{
		LogMessage (_TRACE_LAYOUT, _T("Text [%.1f %.1f %.1f %.1f] Balloon [%.1f %.1f %.1f %.1f] Ref [%.1f %.1f %.1f %.1f] Bounds [%.1f %.1f %.1f %.1f]"), pTextRect.Left, pTextRect.Top, pTextRect.Right, pTextRect.Bottom, pBalloonRect.Left, pBalloonRect.Top, pBalloonRect.Right, pBalloonRect.Bottom, pRefRect.Left, pRefRect.Top, pRefRect.Right, pRefRect.Bottom, pBounds.Left, pBounds.Top, pBounds.Right, pBounds.Bottom);
		LogMessage (_TRACE_LAYOUT, _T("  Balloon [%.1f %.1f] center [%.1f %.1f] Ref [%.1f %.1f] center [%.1f %.1f]"), lBalloonSize.Width, lBalloonSize.Height, lBalloonCenter.X, lBalloonCenter.Y, lRefSize.Width, lRefSize.Height, lRefCenter.X, lRefCenter.Y);
	}
	catch AnyExceptionSilent
#endif
#else
	lRefSize.x = (double)pRefRect.Width();
	lRefSize.y = (double)pRefRect.Height();
	lRefCenter.x = (double)pRefRect.left + (lRefCenterOffset.x=lRefSize.x*0.5f) - 0.5f;
	lRefCenter.y = (double)pRefRect.top + (lRefCenterOffset.y=lRefSize.y*0.5f) - 0.5f;
	lBalloonSize.x = (double)pBalloonRect.Width();
	lBalloonSize.y = (double)pBalloonRect.Height();
	lBalloonCenter.x = (double)pBalloonRect.left + (lBalloonCenterOffset.x=lBalloonSize.x*0.5f) - 0.5f;
	lBalloonCenter.y = (double)pBalloonRect.top + (lBalloonCenterOffset.y=lBalloonSize.y*0.5f) - 0.5f;

#ifdef	_TRACE_LAYOUT
	try
	{
		LogMessage (_TRACE_LAYOUT, _T("Text [%d %d %d %d] Balloon [%d %d %d %d] Ref [%d %d %d %d] Bounds [%d %d %d %d]"), pTextRect.left, pTextRect.top, pTextRect.right, pTextRect.bottom, pBalloonRect.left, pBalloonRect.top, pBalloonRect.right, pBalloonRect.bottom, pRefRect.left, pRefRect.top, pRefRect.right, pRefRect.bottom, pBounds.left, pBounds.top, pBounds.right, pBounds.bottom);
		LogMessage (_TRACE_LAYOUT, _T("  Balloon [%d %d] center [%d %d] Ref [%d %d] center [%d %d]"), dtol(lBalloonSize.x), dtol(lBalloonSize.y), dtol(lBalloonCenter.x), dtol(lBalloonCenter.y), dtol(lRefSize.x), dtol(lRefSize.y), dtol(lRefCenter.x), dtol(lRefCenter.y));
		TracePointFill (lBalloonCenter, RGB(0xFF,0x00,0x00));
		TracePointFill (lRefCenter, RGB(0x00,0xFF,0x00));
	}
	catch AnyExceptionSilent
#endif
#endif

#ifdef	__cplusplus_cli
	CalcRectIntersect (lRefCenter, lBalloonCenter, lBalloonSize, pCalloutEnd, 0.5);
	CalcRectIntersect (pCalloutEnd, lRefCenter, lRefSize, pCalloutBeg);

#ifdef	_TRACE_LAYOUT
	try
	{
		Single lCalloutLgth = (Single)Math::Sqrt ((pCalloutEnd.X-pCalloutBeg.X)+(pCalloutEnd.X-pCalloutBeg.X) + (pCalloutEnd.Y-pCalloutBeg.Y)*(pCalloutEnd.Y-pCalloutBeg.Y));
		LogMessage (_TRACE_LAYOUT, _T("  Callout Beg [%.1f %.1f] End [%.1f %.1f] Length [%.1f]"), pCalloutBeg.X, pCalloutBeg.Y, pCalloutEnd.X, pCalloutEnd.Y, lCalloutLgth);
	}
	catch AnyExceptionSilent
#endif

	pBalloonRect = RectangleF (lBalloonCenter.X-lBalloonCenterOffset.X, lBalloonCenter.Y-lBalloonCenterOffset.Y, pBalloonRect.Width, pBalloonRect.Height);
	FixupNearPoint (pCalloutBeg, pRefRect);
	FixupNearPoint (pCalloutEnd, pBalloonRect);
#else
	CalcRectIntersect (lRefCenter, lBalloonCenter, lBalloonSize, lEndPos, 0.5);
	CalcRectIntersect (lEndPos, lRefCenter, lRefSize, lBegPos);

#ifdef	_TRACE_LAYOUT
	try
	{
		TracePointFill (lRefCenter, RGB(0x00,0xFF,0x00));
		TracePointFill (lBegPos, RGB(0x80,0xFF,0x00));
		TracePointFill (lEndPos, RGB(0xFF,0x80,0x00));
	}
	catch AnyExceptionSilent
#endif
#ifdef	_TRACE_LAYOUT
	try
	{
		double lCalloutLgth = _hypot (lEndPos.x - lBegPos.x, lEndPos.y - lBegPos.y);
		LogMessage (_TRACE_LAYOUT, _T("  Callout Beg [%.1f %.1f] End [%.1f %.1f] Length [%d]"), lBegPos.x, lBegPos.y, lEndPos.x, lEndPos.y, dtol(lCalloutLgth));
	}
	catch AnyExceptionSilent
#endif

	pBalloonRect = CRect (CPoint (dtolUp(lBalloonCenter.x-lBalloonCenterOffset.x), dtolUp(lBalloonCenter.y-lBalloonCenterOffset.y)), pBalloonRect.Size());
	pCalloutBeg.x = dtol(lBegPos.x);
	pCalloutBeg.y = dtol(lBegPos.y);
	pCalloutEnd.x = dtol(lEndPos.x);
	pCalloutEnd.y = dtol(lEndPos.y);
	FixupNearPoint (pCalloutBeg, pRefRect);
	FixupNearPoint (pCalloutEnd, pBalloonRect);
#endif

#ifdef	__cplusplus_cli
	if	(
			(pCalloutEnd.X <= pBalloonRect.Left + mRounding.Width)
		||	(pCalloutEnd.X > pBalloonRect.Right - mRounding.Width)
		)
	{
		if	(mRounding.Height*2 + mCalloutSize.Width*4 < pBalloonRect.Height)
		{
			pCalloutEnd.Y = Math::Min (Math::Max (pCalloutEnd.Y, pBalloonRect.Top + mRounding.Height + mCalloutSize.Width*2), pBalloonRect.Bottom - mRounding.Height - mCalloutSize.Width*2);
		}
		else
		{
			pCalloutEnd.Y = pBalloonRect.Top + pBalloonRect.Height/2;
		}
	}
	else
	{
		if	(mRounding.Width*2 + mCalloutSize.Width*4 < pBalloonRect.Width)
		{
			pCalloutEnd.X = Math::Min (Math::Max (pCalloutEnd.X, pBalloonRect.Left + mRounding.Width + mCalloutSize.Width*2), pBalloonRect.Right - mRounding.Width - mCalloutSize.Width*2);
		}
		else
		{
			pCalloutEnd.X = pBalloonRect.Left + pBalloonRect.Width/2;
		}
	}

#ifdef	_TRACE_LAYOUT
	try
	{
		LogMessage (_TRACE_LAYOUT, _T("  Text [%.1f %.1f %.1f %.1f] Balloon [%.1f %.1f %.1f %.1f] Ref [%.1f %.1f %.1f %.1f] Bounds [%.1f %.1f %.1f %.1f]"), pTextRect.Left, pTextRect.Top, pTextRect.Right, pTextRect.Bottom, pBalloonRect.Left, pBalloonRect.Top, pBalloonRect.Right, pBalloonRect.Bottom, pRefRect.Left, pRefRect.Top, pRefRect.Right, pRefRect.Bottom, pBounds.Left, pBounds.Top, pBounds.Right, pBounds.Bottom);
		LogMessage (_TRACE_LAYOUT, _T("  Callout Beg [%.1f %.1f] End [%.1f %.1f]"), pCalloutBeg.X, pCalloutBeg.Y, pCalloutEnd.X, pCalloutEnd.Y);
	}
	catch AnyExceptionSilent
#endif	
#else
	if	(
			(pCalloutEnd.x <= pBalloonRect.left + mRounding.cx)
		||	(pCalloutEnd.x > pBalloonRect.right - mRounding.cx)
		)
	{
		if	(mRounding.cy*2 + mCalloutSize.cx*4 < pBalloonRect.Height())
		{
			pCalloutEnd.y = min (max (pCalloutEnd.y, pBalloonRect.top + mRounding.cy + mCalloutSize.cx*2), pBalloonRect.bottom - mRounding.cy - mCalloutSize.cx*2);
		}
		else
		{
			pCalloutEnd.y = pBalloonRect.top + pBalloonRect.Height()/2;
		}
	}
	else
	{
		if	(mRounding.cx*2 + mCalloutSize.cx*4 < pBalloonRect.Width())
		{
			pCalloutEnd.x = min (max (pCalloutEnd.x, pBalloonRect.left + mRounding.cx + mCalloutSize.cx*2), pBalloonRect.right - mRounding.cx - mCalloutSize.cx*2);
		}
		else
		{
			pCalloutEnd.x = pBalloonRect.left + pBalloonRect.Width()/2;
		}
	}

#ifdef	_TRACE_LAYOUT
	try
	{
		LogMessage (_TRACE_LAYOUT, _T("  Text [%d %d %d %d] Balloon [%d %d %d %d] Ref [%d %d %d %d] Bounds [%d %d %d %d]"), pTextRect.left, pTextRect.top, pTextRect.right, pTextRect.bottom, pBalloonRect.left, pBalloonRect.top, pBalloonRect.right, pBalloonRect.bottom, pRefRect.left, pRefRect.top, pRefRect.right, pRefRect.bottom, pBounds.left, pBounds.top, pBounds.right, pBounds.bottom);
		LogMessage (_TRACE_LAYOUT, _T("  Callout Beg [%d %d] End [%d %d]"), pCalloutBeg.x, pCalloutBeg.y, pCalloutEnd.x, pCalloutEnd.y);
		TraceRect (pBalloonRect, RGB(0xFF,0xFF,0xFF));
		TracePointFrame (pCalloutBeg, RGB(0xFF,0xFF,0xFF));
		TracePointFrame (pCalloutEnd, RGB(0xFF,0xFF,0xFF));
		ShowTrace ();
	}
	catch AnyExceptionSilent
#endif
#endif
}

/////////////////////////////////////////////////////////////////////////////
//
//	Calculate where a line from RefCenter to RectCenter intersects
//	a rectangle of RectSize centered on RectCenter
//
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
void CAgentBalloonShape::CalcRectIntersect (System::Drawing::PointF pRefPoint, System::Drawing::PointF pRectCenter, System::Drawing::SizeF pRectSize, System::Drawing::PointF% pRectIntersect)
{
	CalcRectIntersect (pRefPoint, pRectCenter, pRectSize, pRectIntersect, 0.0);
}

void CAgentBalloonShape::CalcRectIntersect (System::Drawing::PointF pRefPoint, System::Drawing::PointF pRectCenter, System::Drawing::SizeF pRectSize, System::Drawing::PointF% pRectIntersect, System::Single pMinAngle)
#else
void CAgentBalloonShape::CalcRectIntersect (const _complex& pRefPoint, const _complex& pRectCenter, const _complex& pRectSize, _complex& pRectIntersect, double pMinAngle)
#endif
{
#ifdef	__cplusplus_cli
	PointF		lRefPoint (pRefPoint);
	PointF		lRectHalf (pRectSize.Width / 2.0f, pRectSize.Height / 2.0f);
	Single		lAngle = (Single)Math::Atan2 (pRectCenter.Y - pRefPoint.Y, pRectCenter.X - pRefPoint.X);
#else
	_complex	lRefPoint = {pRefPoint.x, pRefPoint.y};
	_complex	lRectHalf = {pRectSize.x / 2.0, pRectSize.y / 2.0};
	double		lAngle = atan2 (pRectCenter.y - lRefPoint.y, pRectCenter.x - lRefPoint.x);
#endif

#ifdef	_TRACE_LAYOUT
	try
	{
#ifdef	__cplusplus_cli
		LogMessage (_TRACE_LAYOUT, _T("  Point [%.1f %.1f] to center [%.1f %.1f] = Angle [%f]"), lRefPoint.X, lRefPoint.Y, pRectCenter.X, pRectCenter.Y, lAngle);
#else
		LogMessage (_TRACE_LAYOUT, _T("  Point [%d %d] to center [%d %d] = Angle [%f]"), (int)lRefPoint.x, (int)lRefPoint.y, (int)pRectCenter.x, (int)pRectCenter.y, lAngle);
#endif	
	}
	catch AnyExceptionSilent
#endif

	if	(pMinAngle > 0.0)
	{
#ifdef	__cplusplus_cli
		Single	lLength = (Single)Math::Sqrt ((pRectCenter.Y-lRefPoint.Y)*(pRectCenter.Y-lRefPoint.Y) + (pRectCenter.X-lRefPoint.X)*(pRectCenter.X-lRefPoint.X));

		if	(Math::Abs (lAngle) <= pMinAngle)
		{
			lAngle = 0.0;
		}
		else
		if	(Math::Abs (Math::PI/2.0*Math::Sign(lAngle) - lAngle) <= pMinAngle)
		{
			lAngle = (Single)(Math::PI/2.0*Math::Sign(lAngle));
		}
		if	(Math::Abs (Math::PI*Math::Sign(lAngle) - lAngle) <= pMinAngle)
		{
			lAngle = (Single)(Math::PI*Math::Sign(lAngle));
		}

		lRefPoint.X = pRectCenter.X - (Single)Math::Cos (lAngle) * lLength;
		lRefPoint.Y = pRectCenter.Y - (Single)Math::Sin (lAngle) * lLength;
#ifdef	_TRACE_LAYOUT
		try
		{
			LogMessage (_TRACE_LAYOUT, _T("  Point [%.1f %.1f] to center [%.1f %.1f] = Angle [%f] Min [%f]"), lRefPoint.X, lRefPoint.Y, pRectCenter.X, pRectCenter.Y, lAngle, pMinAngle);
		}
		catch AnyExceptionSilent
#endif
#else
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
		try
		{
			LogMessage (_TRACE_LAYOUT, _T("  Point [%d %d] to center [%d %d] = Angle [%f] Min [%f]"), (int)lRefPoint.x, (int)lRefPoint.y, (int)pRectCenter.x, (int)pRectCenter.y, lAngle, pMinAngle);
		}
		catch AnyExceptionSilent
#endif
#endif
	}

#ifdef	__cplusplus_cli
	if	(Math::Round (lRefPoint.X) == Math::Round (pRectCenter.X))
	{
		pRectIntersect.X = 0.0;
		pRectIntersect.Y = lRectHalf.Y * Math::Sign(lRefPoint.Y - pRectCenter.Y);
	}
	else
	if	(Math::Round (lRefPoint.Y) == Math::Round (pRectCenter.Y))
	{
		pRectIntersect.X = lRectHalf.X * Math::Sign(lRefPoint.X - pRectCenter.X);
		pRectIntersect.Y = 0.0;
	}
	else
	{
		pRectIntersect.X = (float)((lRectHalf.Y / Math::Tan (lAngle)) * Math::Sign(lRefPoint.X - pRectCenter.X));
		pRectIntersect.Y = (float)((Math::Tan (lAngle) * lRectHalf.X) * Math::Sign(lRefPoint.Y - pRectCenter.Y));
	}

	if	(Math::Abs (pRectIntersect.X) > lRectHalf.X)
	{
		pRectIntersect.X = lRectHalf.X * Math::Sign(pRectIntersect.X);
	}
	if	(Math::Abs (pRectIntersect.Y) > lRectHalf.Y)
	{
		pRectIntersect.Y = lRectHalf.Y * Math::Sign(pRectIntersect.Y);
	}

	pRectIntersect.X += pRectCenter.X;
	pRectIntersect.Y += pRectCenter.Y;
#else
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
#endif
}

/////////////////////////////////////////////////////////////////////////////
//
//	Ensure that the balloon rectangle is within the boundaries and
//	the proper distance from the reference rectangle
//
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
System::Boolean CAgentBalloonShape::ValidateBalloonRect (System::Drawing::RectangleF% pBalloonRect, System::Drawing::RectangleF pRefRect, System::Drawing::RectangleF pBounds)
#else
bool CAgentBalloonShape::ValidateBalloonRect (CRect& pBalloonRect, const CRect& pRefRect, const CRect& pBounds)
#endif
{
	bool	lRet = false;
#ifdef	__cplusplus_cli
	RectangleF	lRefRect (pRefRect);
	RectangleF	lBounds (pBounds);
	RectangleF	lIntersect;
	PointF		lCenter (pBalloonRect.Left + pBalloonRect.Width/2.0f, pBalloonRect.Top + pBalloonRect.Height/2.0f);
	PointF		lRefCenter (pRefRect.Left + pRefRect.Width/2.0f, pRefRect.Top + pRefRect.Height/2.0f);
#else
	CRect		lRefRect (pRefRect);
	CRect		lBounds (pBounds);
	CRect		lIntersect;
	CPoint		lCenter = pBalloonRect.CenterPoint();
	CPoint		lRefCenter = pRefRect.CenterPoint();
#endif	

#ifdef	_TRACE_LAYOUT
	try
	{
#ifdef	__cplusplus_cli
		LogMessage (_TRACE_LAYOUT, _T("  Validate [%.1f %.1f %.1f %.1f] Ref [%.1f %.1f %.1f %.1f] Bounds [%.1f %.1f %.1f %.1f]"), pBalloonRect.Left, pBalloonRect.Top, pBalloonRect.Right, pBalloonRect.Bottom, pRefRect.Left, pRefRect.Top, pRefRect.Right, pRefRect.Bottom, pBounds.Left, pBounds.Top, pBounds.Right, pBounds.Bottom);
#else
		LogMessage (_TRACE_LAYOUT, _T("  Validate [%d %d %d %d] Ref [%d %d %d %d] Bounds [%d %d %d %d]"), pBalloonRect.left, pBalloonRect.top, pBalloonRect.right, pBalloonRect.bottom, pRefRect.left, pRefRect.top, pRefRect.right, pRefRect.bottom, pBounds.left, pBounds.top, pBounds.right, pBounds.bottom);
#endif	
	}
	catch AnyExceptionSilent
#endif	

#ifdef	__cplusplus_cli
	lRefRect.Inflate (mCalloutSize.Width, mCalloutSize.Height);
	lRefRect.Inflate (pBalloonRect.Width, pBalloonRect.Height);
	lBounds.Intersect (lBounds, lRefRect);
	lRefRect.Inflate (-pBalloonRect.Width, -pBalloonRect.Height);
#else
	lRefRect.InflateRect (mCalloutSize.cy, mCalloutSize.cy);
	lRefRect.InflateRect (pBalloonRect.Width(), pBalloonRect.Height());
	lBounds.IntersectRect (lBounds, lRefRect);
	lRefRect.DeflateRect (pBalloonRect.Width(), pBalloonRect.Height());
#endif	

#ifdef	__cplusplus_cli
	if	(pBalloonRect.Right > lBounds.Right)
	{
		pBalloonRect.Offset (lBounds.Right - pBalloonRect.Right, 0);
		lRet = true;
	}
	if	(pBalloonRect.Bottom > lBounds.Bottom)
	{
		pBalloonRect.Offset (0, lBounds.Bottom - pBalloonRect.Bottom);
		lRet = true;
	}
	if	(pBalloonRect.Left < lBounds.Left)
	{
		pBalloonRect.Offset (lBounds.Left - pBalloonRect.Left, 0);
		lRet = true;
	}
	if	(pBalloonRect.Left < lBounds.Left)
	{
		pBalloonRect.Offset (0, lBounds.Left - pBalloonRect.Left);
		lRet = true;
	}
#else
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
#endif	

#ifdef	__cplusplus_cli
	lIntersect.Intersect (pBalloonRect, lRefRect);
	if	(!lIntersect.IsEmpty)
	{
		bool	lOffsetVertical = (Math::Abs (lCenter.X - lRefCenter.X) <= Math::Abs (lCenter.Y - lRefCenter.Y));
		bool	lDidVertical = lOffsetVertical;
		bool	lMoved = false;

		while	(!lMoved)
		{
			if	(
					(lOffsetVertical)
				&&	(lCenter.Y >= lRefCenter.Y)
				&&	(lRefRect.Bottom >= lBounds.Top)
				&&	(pBalloonRect.Bottom + lRefRect.Bottom - pBalloonRect.Top <= lBounds.Bottom)
				)
			{
				pBalloonRect.Offset (0, lRefRect.Bottom - pBalloonRect.Top);
				lMoved = true;
			}
			else
			if	(
					(lOffsetVertical)
				&&	(lCenter.Y <= lRefCenter.Y)
				&&	(lRefRect.Top <= lBounds.Bottom)
				&&	(pBalloonRect.Top + lRefRect.Top - pBalloonRect.Bottom >= lBounds.Top)
				)
			{
				pBalloonRect.Offset (0, lRefRect.Top - pBalloonRect.Bottom);
				lMoved = true;
			}
			else
			if	(
					(!lOffsetVertical)
				&&	(lCenter.X >= lRefCenter.X)
				&&	(lRefRect.Right >= pBalloonRect.Left)
				&&	(pBalloonRect.Right + lRefRect.Right - pBalloonRect.Left <= lBounds.Right)
				)
			{
				pBalloonRect.Offset (lRefRect.Right - pBalloonRect.Left, 0);
				lMoved = true;
			}
			else
			if	(
					(!lOffsetVertical)
				&&	(lCenter.X <= lRefCenter.X)
				&&	(lRefRect.Left <= lBounds.Right)
				&&	(pBalloonRect.Left + lRefRect.Left - pBalloonRect.Right >= lBounds.Left)
				)
			{
				pBalloonRect.Offset (lRefRect.Left - pBalloonRect.Right, 0);
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
					(lRefCenter.X != lCenter.X)
				||	(lRefCenter.Y != lCenter.Y)
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
#else
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
#endif

#ifdef	_TRACE_LAYOUT
	try
	{
		if	(lRet)
		{
#ifdef	__cplusplus_cli
			LogMessage (_TRACE_LAYOUT, _T("  Validate [%.1f %.1f %.1f %.1f]"), pBalloonRect.Left, pBalloonRect.Top, pBalloonRect.Right, pBalloonRect.Bottom);
#else
			LogMessage (_TRACE_LAYOUT, _T("  Validate [%d %d %d %d]"), pBalloonRect.left, pBalloonRect.top, pBalloonRect.right, pBalloonRect.bottom);
#endif	
		}
	}
	catch AnyExceptionSilent
#endif	
	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
//
//	Account for rounding errors by moving a point near a rectangle boundary
//	onto the boundary
//
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
void CAgentBalloonShape::FixupNearPoint (System::Drawing::PointF% pPoint, System::Drawing::RectangleF pRefRect)
{
	FixupNearPoint (pPoint, pRefRect, 2);
}

void CAgentBalloonShape::FixupNearPoint (System::Drawing::PointF% pPoint, System::Drawing::RectangleF pRefRect, long pNearness)
#else
void CAgentBalloonShape::FixupNearPoint (CPoint& pPoint, const CRect& pRefRect, long pNearness)
#endif
{
#ifdef	__cplusplus_cli
	if	(Math::Abs (pPoint.X - pRefRect.Left) <= (Single)pNearness)
	{
		pPoint.X = pRefRect.Left;
	}
	else
	if	(Math::Abs (pPoint.X - pRefRect.Right) <= (Single)pNearness)
	{
		pPoint.X = pRefRect.Right;
	}

	if	(Math::Abs (pPoint.Y - pRefRect.Top) <= (Single)pNearness)
	{
		pPoint.Y = pRefRect.Top;
	}
	else
	if	(Math::Abs (pPoint.Y - pRefRect.Bottom) <= (Single)pNearness)
	{
		pPoint.Y = pRefRect.Bottom;
	}
#else
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
#endif
}

/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
void CAgentBalloonShape::MakeRoundRect (System::Drawing::Drawing2D::GraphicsPath^ pShapePath)
#else
void CAgentBalloonShape::MakeRoundRect (Gdiplus::GraphicsPath& pShapePath)
#endif
{
#ifdef	__cplusplus_cli
	SizeF		lRounding (mRounding);
	RectangleF	lShapeRect (mBalloonRect.Left+1, mBalloonRect.Top+1, mBalloonRect.Width-2, mBalloonRect.Height-2);

	lRounding.Width = Math::Min (lRounding.Width, lShapeRect.Width/2.0f-2.0f);
	lRounding.Height = Math::Min (lRounding.Height, lShapeRect.Height/2.0f-2.0f);

	pShapePath->AddLine (lShapeRect.Left+lRounding.Width, lShapeRect.Top, lShapeRect.Right-+lRounding.Width, lShapeRect.Top);
	pShapePath->AddArc (lShapeRect.Right-lRounding.Width*2.0f, lShapeRect.Top, lRounding.Width*2.0f, lRounding.Height*2.0f, 270.0f, 90.0f);
	pShapePath->AddLine (lShapeRect.Right, lShapeRect.Top+lRounding.Height, lShapeRect.Right, lShapeRect.Bottom-lRounding.Height);
	pShapePath->AddArc (lShapeRect.Right-lRounding.Width*2.0f, lShapeRect.Bottom-lRounding.Height*2.0f, lRounding.Width*2.0f, lRounding.Height*2.0f, 0.0f, 90.0f);
	pShapePath->AddLine (lShapeRect.Right-lRounding.Width, lShapeRect.Bottom, lShapeRect.Left+lRounding.Width, lShapeRect.Bottom);
	pShapePath->AddArc (lShapeRect.Left, lShapeRect.Bottom-lRounding.Height*2.0f, lRounding.Width*2.0f, lRounding.Height*2.0f, 90.0f, 90.0f);
	pShapePath->AddLine (lShapeRect.Left, lShapeRect.Bottom-lRounding.Height, lShapeRect.Left, lShapeRect.Top+lRounding.Height);
	pShapePath->AddArc (lShapeRect.Left, lShapeRect.Top, lRounding.Width*2.0f, lRounding.Height*2.0f, 180.0f, 90.0f);
#else
	Gdiplus::SizeF	lRounding ((Gdiplus::REAL)mRounding.cx, (Gdiplus::REAL)mRounding.cy);
	Gdiplus::RectF	lShapeRect ((Gdiplus::REAL)mBalloonRect.left+1, (Gdiplus::REAL)mBalloonRect.top+1, (Gdiplus::REAL)mBalloonRect.Width()-2, (Gdiplus::REAL)mBalloonRect.Height()-2);

	lRounding.Width = min (lRounding.Width, lShapeRect.Width/2.0f-2.0f);
	lRounding.Height = min (lRounding.Height, lShapeRect.Height/2.0f-2.0f);

	pShapePath.AddLine (lShapeRect.GetLeft()+lRounding.Width, lShapeRect.GetTop(), lShapeRect.GetRight()-+lRounding.Width, lShapeRect.GetTop());
	pShapePath.AddArc (lShapeRect.GetRight()-lRounding.Width*2.0f, lShapeRect.GetTop(), lRounding.Width*2.0f, lRounding.Height*2.0f, 270.0f, 90.0f);
	pShapePath.AddLine (lShapeRect.GetRight(), lShapeRect.GetTop()+lRounding.Height, lShapeRect.GetRight(), lShapeRect.GetBottom()-lRounding.Height);
	pShapePath.AddArc (lShapeRect.GetRight()-lRounding.Width*2.0f, lShapeRect.GetBottom()-lRounding.Height*2.0f, lRounding.Width*2.0f, lRounding.Height*2.0f, 0.0f, 90.0f);
	pShapePath.AddLine (lShapeRect.GetRight()-lRounding.Width, lShapeRect.GetBottom(), lShapeRect.GetLeft()+lRounding.Width, lShapeRect.GetBottom());
	pShapePath.AddArc (lShapeRect.GetLeft(), lShapeRect.GetBottom()-lRounding.Height*2.0f, lRounding.Width*2.0f, lRounding.Height*2.0f, 90.0f, 90.0f);
	pShapePath.AddLine (lShapeRect.GetLeft(), lShapeRect.GetBottom()-lRounding.Height, lShapeRect.GetLeft(), lShapeRect.GetTop()+lRounding.Height);
	pShapePath.AddArc (lShapeRect.GetLeft(), lShapeRect.GetTop(), lRounding.Width*2.0f, lRounding.Height*2.0f, 180.0f, 90.0f);
#endif
}

#ifdef	__cplusplus_cli
void CAgentBalloonShape::DrawShadow (System::Drawing::Drawing2D::GraphicsPath^ pShapePath, System::Drawing::Graphics^ pGraphics)
#else
void CAgentBalloonShape::DrawShadow (Gdiplus::GraphicsPath& pShapePath, Gdiplus::Graphics& pGraphics)
#endif
{
#ifdef	__cplusplus_cli
	System::Drawing::Bitmap^	lShadow = gcnew System::Drawing::Bitmap ((int)Math::Ceiling(mBounds.Width+mShadowOffset.X), (int)Math::Ceiling(mBounds.Height+mShadowOffset.Y), PixelFormat::Format32bppArgb);
#else
	Gdiplus::Bitmap				lShadow (mBounds.Width()+mShadowOffset.x, mBounds.Height()+mShadowOffset.y, PixelFormat32bppPARGB);
	Gdiplus::Pen				lClipPen (Gdiplus::Color::Black, (Gdiplus::REAL)mShadowOffset.x+1);
	Gdiplus::GraphicsPath		lClipPath;
#endif	

#ifndef	__cplusplus_cli
	lClipPath.AddPath (&pShapePath, FALSE);
	lClipPath.Widen (&lClipPen);
#endif	

#ifdef	__cplusplus_cli
	{
		Graphics^	lGraphics = Graphics::FromImage (lShadow);
		SolidBrush^	lBrush = gcnew SolidBrush (Color::Black);

		lGraphics->Clear (Color::Transparent);
		lGraphics->CompositingMode = CompositingMode::SourceOver;
		lGraphics->CompositingQuality = CompositingQuality::HighQuality;
		lGraphics->SmoothingMode = SmoothingMode::HighQuality;
		lGraphics->PixelOffsetMode = PixelOffsetMode::HighQuality;

		lGraphics->FillPath (lBrush, pShapePath);
	}
#else
	{
		Gdiplus::Graphics	lGraphics (&lShadow);
		Gdiplus::SolidBrush	lBrush (Gdiplus::Color::Black);

		lGraphics.Clear (Gdiplus::Color (0,0,0,0));
		lGraphics.SetCompositingMode (Gdiplus::CompositingModeSourceOver);
		lGraphics.SetCompositingQuality (Gdiplus::CompositingQualityHighQuality);
		lGraphics.SetSmoothingMode (Gdiplus::SmoothingModeHighQuality);
		lGraphics.SetPixelOffsetMode (Gdiplus::PixelOffsetModeHighQuality);

		lGraphics.SetClip (&lClipPath, Gdiplus::CombineModeExclude);
		lGraphics.FillPath (&lBrush, &pShapePath);
	}
#endif

#ifdef	__cplusplus_cli
	try
	{
		Boolean		lSmeared = false;
		Bitmap^		lTarget = gcnew Bitmap (lShadow->Width, lShadow->Height, PixelFormat::Format32bppArgb);
		BitmapData^	lSrcData;
		BitmapData^	lTrgData;
		LPBYTE		lSrcBytes;
		LPBYTE		lTrgBytes;

		lSrcData = lShadow->LockBits (System::Drawing::Rectangle (0, 0, lShadow->Width, lShadow->Height), ImageLockMode::ReadWrite, PixelFormat::Format32bppArgb);
		lSrcBytes = (LPBYTE)(INT_PTR)lSrcData->Scan0;
		lTrgData = lTarget->LockBits (System::Drawing::Rectangle (0, 0, lTarget->Width, lTarget->Height), ImageLockMode::ReadWrite, PixelFormat::Format32bppArgb);
		lTrgBytes = (LPBYTE)(INT_PTR)lTrgData->Scan0;

		try
		{
			Point	lMaxPixel;
			Point	lTrgPixel;
			Point	lSrcPixel;
			long	lNdx;
			long	lTrgNdx;
			long	lSrcNdx;
			long	lAperture = 3;
			long	lPixel [4];
			long	lCount;

			lMaxPixel.X = lShadow->Width-1;
			lMaxPixel.Y = lShadow->Height-1;

			for	(lTrgPixel.Y = 0; lTrgPixel.Y <= lMaxPixel.Y; lTrgPixel.Y++)
			{
				for	(lTrgPixel.X = 0; lTrgPixel.X <= lMaxPixel.X; lTrgPixel.X++)
				{
					memset (lPixel, 0, sizeof (lPixel));
					lCount = 0;

					for	(lSrcPixel.Y = max (lTrgPixel.Y - lAperture, 0); lSrcPixel.Y <= min (lTrgPixel.Y + lAperture, lMaxPixel.Y); lSrcPixel.Y++)
					{
						for	(lSrcPixel.X = max (lTrgPixel.X - lAperture, 0); lSrcPixel.X <= min (lTrgPixel.X + lAperture, lMaxPixel.X); lSrcPixel.X++)
						{
							lSrcNdx = (lSrcPixel.Y * lSrcData->Stride) + (lSrcPixel.X * 4);

							for	(lNdx = 0; lNdx < 4; lNdx++)
							{
								lPixel [lNdx] += (ULONG) lSrcBytes [lSrcNdx+lNdx];
							}
							lCount++;
						}
					}

					for	(lNdx = 0; lNdx < 4; lNdx++)
					{
						lPixel [lNdx] /= lCount;
					}

					lTrgNdx = (lTrgPixel.Y * lTrgData->Stride) + (lTrgPixel.X * 4);

					for	(lNdx = 0; lNdx < 4; lNdx++)
					{
						lTrgBytes [lTrgNdx+lNdx] = (BYTE) min (lPixel [lNdx], 255);
					}
				}
			}
			lSmeared = true;
		}
		catch AnyExceptionSilent

		lShadow->UnlockBits (lSrcData);
		lTarget->UnlockBits (lTrgData);
		
		if	(lSmeared)
		{
			lShadow = lTarget;
		}
	}
	catch AnyExceptionDebug

	pGraphics->DrawImage (lShadow, mShadowOffset.X, mShadowOffset.Y);
#else	
#if	(GDIPVER < 0x0110)
	{
		HBITMAP		lBitmap;
		ATL::CImage	lImage;

		if	(lShadow.GetHBITMAP (Gdiplus::Color (0,0,0,0), &lBitmap) == Gdiplus::Ok)
		{
			lImage.Attach (lBitmap);
			if	(CImageTools::SmearImage (lImage, 3))
			{
				Gdiplus::Bitmap	lShadow (lImage.GetWidth(), lImage.GetHeight(), lImage.GetPitch(), PixelFormat32bppPARGB, (LPBYTE)lImage.GetBits());
				pGraphics.DrawImage (&lShadow, mShadowOffset.x, mShadowOffset.y);
			}
		}
	}
#else	// Requires GDI+ Version 1.1 which may not be installed on the target machine
	{
		Gdiplus::BlurParams	lBlurParams = {(Gdiplus::REAL)mShadowOffset.x+2, TRUE};
		Gdiplus::Blur		lBlur;

		lBlur.SetParameters (&lBlurParams);
		lShadow.ApplyEffect (&lBlur, NULL);
	}
#endif
	pGraphics.DrawImage (&lShadow, mShadowOffset.x, mShadowOffset.y);
#endif
}

/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
/////////////////////////////////////////////////////////////////////////////

System::Windows::Media::Drawing^ CAgentBalloonShape::MakeDrawing (System::Drawing::Color pBkColor, System::Drawing::Color pBrColor)
{
	return MakeDrawing (System::Windows::Media::Color::FromArgb (pBkColor.A, pBkColor.R, pBkColor.G, pBkColor.B), System::Windows::Media::Color::FromArgb (pBrColor.A, pBrColor.R, pBrColor.G, pBrColor.B));
}

System::Windows::Media::Drawing^ CAgentBalloonShape::MakeDrawing (System::Windows::Media::Color pBkColor, System::Windows::Media::Color pBrColor)
{
	return MakeDrawing (gcnew System::Windows::Media::SolidColorBrush (pBkColor), gcnew System::Windows::Media::SolidColorBrush (pBrColor));
}

/////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAgentBalloonSpeak::CAgentBalloonSpeak()
{
	InitLayout ();
}

#ifndef	__cplusplus_cli
CAgentBalloonSpeak::~CAgentBalloonSpeak()
{
}
#endif

/////////////////////////////////////////////////////////////////////////////

void CAgentBalloonSpeak::InitLayout ()
{
	CAgentBalloonShape::InitLayout ();
#ifdef	__cplusplus_cli
#ifdef	_DEBUG_LAYOUT
	LogMessage (_DEBUG_LAYOUT, _T("%s Rounding [%f %f] Callout [%f %f]"), _B(GetType()->Name), mRounding.Width, mRounding.Height, mCalloutSize.Width, mCalloutSize.Height);
#endif
#else
#ifdef	_TRACE_LAYOUT_NOT
	mCalloutSize.cx = MulDiv (mCalloutSize.cx, 5, 4);
	mCalloutSize.cy = MulDiv (mCalloutSize.cy, 4, 1);
#endif
#ifdef	_DEBUG_LAYOUT
	LogMessage (_DEBUG_LAYOUT, _T("%s Rounding [%d %d] Callout [%d %d]"), ObjTypeName(this), mRounding.cx, mRounding.cy, mCalloutSize.cx, mCalloutSize.cy);
#endif
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
GraphicsPath^ CAgentBalloonSpeak::GetBalloonRgn ()
#else
HRGN CAgentBalloonSpeak::GetBalloonRgn ()
#endif
{
#ifdef	__cplusplus_cli
//TODO
	return nullptr;
#else
	if	(mUseGdiplus)
	{
		return NULL;
	}
	else
	{
		CRgnHandle	lRgn1;
		CRgnHandle	lRgn2;

		if	(!mBalloonRect.IsRectEmpty())
		{
			Gdiplus::PointF	lCalloutPoints [3];
			CPoint			lPoints [3];
#ifdef	_DEBUG_DRAW
			CRect	lWinRect;

			lWinRect.left = min (mBalloonRect.left, mCalloutBeg.x);
			lWinRect.right = max (mBalloonRect.right, mCalloutBeg.x+1);
			lWinRect.top = min (mBalloonRect.top, mCalloutBeg.y);
			lWinRect.bottom = max (mBalloonRect.bottom, mCalloutBeg.y+1);
			lRgn1 = ::CreateRectRgnIndirect (lWinRect);
#else
			GetCalloutPoints (lCalloutPoints);
			lPoints [0].x = dtol (lCalloutPoints [0].X);
			lPoints [0].y = dtol (lCalloutPoints [0].Y);
			lPoints [1].x = dtol (lCalloutPoints [1].X);
			lPoints [1].y = dtol (lCalloutPoints [1].Y);
			lPoints [2].x = dtol (lCalloutPoints [2].X);
			lPoints [2].y = dtol (lCalloutPoints [2].Y);
			lRgn1 = ::CreateRoundRectRgn (mBalloonRect.left, mBalloonRect.top, ((mBalloonRect.right-1))+1, ((mBalloonRect.bottom-1))+1, mRounding.cx, mRounding.cy);
			lRgn2 = ::CreatePolygonRgn (lPoints, 3, WINDING);
			CombineRgn (lRgn1, lRgn1, lRgn2, RGN_OR);
			lRgn2.Close ();
#endif
		}
		return lRgn1.Detach();
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

#ifndef	__cplusplus_cli
bool CAgentBalloonSpeak::Draw (HDC pDC, COLORREF pBkColor, COLORREF pBrColor)
{
#ifdef	_DEBUG_DRAW
	LogMessage (_DEBUG_DRAW, _T("CAgentBalloonSpeak::Draw [%s] [%s] [%s] [%s] [%s] [%s]"), FormatRect(mBounds), FormatRect(mBalloonRect), FormatRect(mTextRect), FormatSize(mCalloutSize), FormatPoint(mCalloutBeg), FormatPoint(mCalloutEnd));
#endif
	if	(
			(!mBalloonRect.IsRectEmpty())
		&&	(mUseGdiplus)
		)
	{
		Gdiplus::Graphics	lGraphics (pDC);
		Gdiplus::Color		lBkColor (GetRValue(pBkColor), GetGValue(pBkColor), GetBValue(pBkColor));
		Gdiplus::Color		lBrColor (GetRValue(pBrColor), GetGValue(pBrColor), GetBValue(pBrColor));

		lGraphics.Clear (Gdiplus::Color (0,0,0,0));
		return Draw (lGraphics, lBkColor, lBrColor);
	}
	else
	if	(!mBalloonRect.IsRectEmpty())
	{
		CBrushHandle	lFillBrush = CImageAlpha::GetAlphaBrush (pBkColor, 255);
		CBrushHandle	lFrameBrush = CImageAlpha::GetAlphaBrush (pBrColor, 255);
		CRgnHandle		lRgn = GetBalloonRgn ();

#ifdef	_DEBUG_DRAW
		{
			CRect			lClipRect;
			CBrushHandle	lBrush;
			lBrush = ::CreateSolidBrush (RGB (0xFF, 0x80, 0xFF));
			::GetClipBox (pDC, &lClipRect);
			::FillRect (pDC, &lClipRect, lBrush);
		}
#endif
		::FillRgn (pDC, lRgn, lFillBrush);
		::FrameRgn (pDC, lRgn, lFrameBrush, 1, 1);


#ifdef	_DEBUG_DRAW
		{
			CBrushHandle lBrush;
			lBrush = ::CreateSolidBrush (RGB (0xFF,0x00,0x00));
			::FillRect (pDC, CRect (mCalloutEnd.x-1, mCalloutEnd.y-1, mCalloutEnd.x+2, mCalloutEnd.y+2), lBrush);
		}
#endif
		return true;
	}
	return false;
}
#endif

#ifdef	__cplusplus_cli
System::Boolean CAgentBalloonSpeak::Draw (System::Drawing::Graphics^ pGraphics, System::Drawing::Color pBkColor, System::Drawing::Color pBrColor)
#else
bool CAgentBalloonSpeak::Draw (Gdiplus::Graphics & pGraphics, Gdiplus::Color & pBkColor, Gdiplus::Color & pBrColor)
#endif
{
#ifdef	__cplusplus_cli
	if	(!mBalloonRect.IsEmpty)
	{
		GraphicsPath^	lShapePath = gcnew GraphicsPath;
		GraphicsPath^	lOutlinePath;
		array<PointF>^	lCalloutPoints = gcnew array<PointF> (3);
		SolidBrush^		lFillBrush = gcnew SolidBrush (pBkColor);
		SolidBrush^		lOutlineBrush = gcnew SolidBrush (pBrColor);
		Pen^			lOutlinePen = gcnew Pen (pBrColor, 1.9f);

		lShapePath->StartFigure ();
		MakeRoundRect (lShapePath);
		lShapePath->CloseFigure ();

		GetCalloutPoints (lCalloutPoints);
		lShapePath->AddLine (lCalloutPoints [1], lCalloutPoints [0]);
		lShapePath->AddLine (lCalloutPoints [0], lCalloutPoints [2]);
		lShapePath->FillMode = FillMode::Winding;
		lOutlinePath = safe_cast<GraphicsPath^> (lShapePath->Clone());
		lOutlinePath->Widen (lOutlinePen);

		pGraphics->CompositingMode = CompositingMode::SourceOver;
		pGraphics->CompositingQuality = CompositingQuality::HighQuality;
		pGraphics->SmoothingMode = SmoothingMode::HighQuality;
		pGraphics->PixelOffsetMode = PixelOffsetMode::HighQuality;

		DrawShadow (lShapePath, pGraphics);
		pGraphics->FillPath (lOutlineBrush, lOutlinePath);
		pGraphics->FillPath (lFillBrush, lShapePath);

		return true;
	}
#else
	if	(!mBalloonRect.IsRectEmpty())
	{
		Gdiplus::GraphicsPath	lShapePath;
		Gdiplus::PointF			lCalloutPoints [3];
		Gdiplus::SolidBrush		lBrush (pBkColor);
		Gdiplus::Pen			lPen (pBrColor, 1.0f);

		lShapePath.StartFigure ();
		MakeRoundRect (lShapePath);
		lShapePath.CloseFigure ();

		GetCalloutPoints (lCalloutPoints);
		lShapePath.AddLine (lCalloutPoints [1], lCalloutPoints [0]);
		lShapePath.AddLine (lCalloutPoints [0], lCalloutPoints [2]);
		lShapePath.SetFillMode (Gdiplus::FillModeWinding);
		lShapePath.Outline ();

		pGraphics.SetCompositingMode (Gdiplus::CompositingModeSourceOver);
		pGraphics.SetCompositingQuality (Gdiplus::CompositingQualityHighQuality);
		pGraphics.SetSmoothingMode (Gdiplus::SmoothingModeHighQuality);
		pGraphics.SetPixelOffsetMode (Gdiplus::PixelOffsetModeHighQuality);

		DrawShadow (lShapePath, pGraphics);
		pGraphics.FillPath (&lBrush, &lShapePath);
		pGraphics.DrawPath (&lPen, &lShapePath);

		return true;
	}
#endif
	return false;
}

#ifdef	__cplusplus_cli
void CAgentBalloonSpeak::GetCalloutPoints (array<System::Drawing::PointF>^ pPoints)
#else
void CAgentBalloonSpeak::GetCalloutPoints (Gdiplus::PointF* pPoints)
#endif
{
#ifdef	__cplusplus_cli
	RectangleF		lBalloonRect (mBalloonRect);
	Single			lCalloutAngle;
	Single			lCalloutExtension;
	int				lPointNdx;
	int				lExtendCount;
	PointF			lExtendPoint;
#else
	Gdiplus::RectF	lBalloonRect ((Gdiplus::REAL)mBalloonRect.left+1, (Gdiplus::REAL)mBalloonRect.top+1, (Gdiplus::REAL)mBalloonRect.Width()-2, (Gdiplus::REAL)mBalloonRect.Height()-2);
	double			lCalloutAngle;
	double			lCalloutWidth;
	double			lCalloutExtension;
	INT_PTR			lPointNdx;
	INT_PTR			lExtendCount;
	Gdiplus::PointF	lExtendPoint;
#endif	

#ifdef	__cplusplus_cli
	lCalloutAngle = (Single)Math::Atan2 (mCalloutEnd.Y-mCalloutBeg.Y, mCalloutEnd.X-mCalloutBeg.X);
	lCalloutExtension = mCalloutSize.Width * 2;

	pPoints [0].X = mCalloutBeg.X;
	pPoints [0].Y = mCalloutBeg.Y;
	pPoints [1].X = mCalloutEnd.X + (Single)(Math::Cos (lCalloutAngle + Math::PI/2.0) * mCalloutSize.Width);
	pPoints [1].Y = mCalloutEnd.Y + (Single)(Math::Sin (lCalloutAngle + Math::PI/2.0) * mCalloutSize.Width);
	pPoints [2].X = mCalloutEnd.X + (Single)(Math::Cos (lCalloutAngle - Math::PI/2.0) * mCalloutSize.Width);
	pPoints [2].Y = mCalloutEnd.Y + (Single)(Math::Sin (lCalloutAngle - Math::PI/2.0) * mCalloutSize.Width);
#else
	lCalloutAngle = atan2 ((double)(mCalloutEnd.y-mCalloutBeg.y), (double)(mCalloutEnd.x-mCalloutBeg.x));
	lCalloutWidth = (double)(mCalloutSize.cx);
	lCalloutExtension = lCalloutWidth * 2.0;

	pPoints [0].X = (Gdiplus::REAL)mCalloutBeg.x;
	pPoints [0].Y = (Gdiplus::REAL)mCalloutBeg.y;
	pPoints [1].X = (Gdiplus::REAL)mCalloutEnd.x + (Gdiplus::REAL)(cos (lCalloutAngle + PI/2.0) * lCalloutWidth);
	pPoints [1].Y = (Gdiplus::REAL)mCalloutEnd.y + (Gdiplus::REAL)(sin (lCalloutAngle + PI/2.0) * lCalloutWidth);
	pPoints [2].X = (Gdiplus::REAL)mCalloutEnd.x + (Gdiplus::REAL)(cos (lCalloutAngle - PI/2.0) * lCalloutWidth);
	pPoints [2].Y = (Gdiplus::REAL)mCalloutEnd.y + (Gdiplus::REAL)(sin (lCalloutAngle - PI/2.0) * lCalloutWidth);
#endif	

	for	(lPointNdx = 1; lPointNdx <= 2; lPointNdx++)
	{
		if	(!lBalloonRect.Contains (pPoints [lPointNdx]))
		{
			lExtendPoint = pPoints [lPointNdx];
#ifdef	__cplusplus_cli
			lCalloutAngle =(Single)Math::Atan2 (lExtendPoint.Y-mCalloutBeg.Y, lExtendPoint.X-mCalloutBeg.X);
#else
			lCalloutAngle = atan2 (lExtendPoint.Y-(double)mCalloutBeg.y, lExtendPoint.X-(double)mCalloutBeg.x);
#endif			

			for	(lExtendCount = 1; lExtendCount <= 10; lExtendCount++)
			{
#ifdef	__cplusplus_cli
				pPoints [lPointNdx].X = lExtendPoint.X + (Single)(Math::Cos (lCalloutAngle) * lCalloutExtension * (Single)lExtendCount);
				pPoints [lPointNdx].Y = lExtendPoint.Y + (Single)(Math::Sin (lCalloutAngle) * lCalloutExtension * (Single)lExtendCount);
#else
				pPoints [lPointNdx].X = lExtendPoint.X + (Gdiplus::REAL)(cos (lCalloutAngle) * lCalloutExtension * (double)lExtendCount);
				pPoints [lPointNdx].Y = lExtendPoint.Y + (Gdiplus::REAL)(sin (lCalloutAngle) * lCalloutExtension * (double)lExtendCount);
#endif			
				if	(lBalloonRect.Contains (pPoints [lPointNdx]))
				{
					break;
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
/////////////////////////////////////////////////////////////////////////////

System::Windows::Media::Drawing^ CAgentBalloonSpeak::MakeDrawing (System::Windows::Media::Brush^ pBkBrush, System::Windows::Media::Brush^ pBrBrush)
{
	System::Windows::Media::Drawing^	lDrawing = nullptr;

	if	(!mBalloonRect.IsEmpty)
	{
		System::Windows::Media::Pen^				lOutlinePen = gcnew System::Windows::Media::Pen (pBrBrush, 1);
		System::Windows::Media::DrawingGroup^		lDrawingGroup = gcnew System::Windows::Media::DrawingGroup ();
		System::Windows::Media::PathGeometry^		lPathGeometry;
		System::Windows::Media::RectangleGeometry^	lRectGeometry;
		System::Windows::Media::PathGeometry^		lCalloutGeometry;
		System::Windows::Media::PathFigure^			lCalloutFigure;
		array<PointF>^								lCalloutPoints = gcnew array<PointF> (3);

		lPathGeometry = gcnew System::Windows::Media::PathGeometry ();
		lPathGeometry->FillRule = System::Windows::Media::FillRule::Nonzero;

		lRectGeometry = gcnew System::Windows::Media::RectangleGeometry (System::Windows::Rect (mBalloonRect.Left+1, mBalloonRect.Top+1, mBalloonRect.Width-2, mBalloonRect.Height-2), mRounding.Width, mRounding.Height);
		lPathGeometry->AddGeometry (lRectGeometry->GetFlattenedPathGeometry ());

		lCalloutGeometry = gcnew System::Windows::Media::PathGeometry ();
		lCalloutGeometry->FillRule = System::Windows::Media::FillRule::Nonzero;
		GetCalloutPoints (lCalloutPoints);
		lCalloutFigure = gcnew System::Windows::Media::PathFigure ();
		lCalloutFigure->IsClosed = false;
		lCalloutFigure->IsFilled = true;
		lCalloutFigure->StartPoint = System::Windows::Point (lCalloutPoints[1].X, lCalloutPoints[1].Y);
		lCalloutFigure->Segments->Add (gcnew System::Windows::Media::LineSegment (System::Windows::Point (lCalloutPoints[0].X, lCalloutPoints[0].Y), true));
		lCalloutFigure->Segments->Add (gcnew System::Windows::Media::LineSegment (System::Windows::Point (lCalloutPoints[2].X, lCalloutPoints[2].Y), true));
		lCalloutGeometry->Figures->Add (lCalloutFigure);
		lPathGeometry->AddGeometry (lCalloutGeometry);

		lPathGeometry = lPathGeometry->GetOutlinedPathGeometry ();
		lDrawingGroup->Children->Add (gcnew System::Windows::Media::GeometryDrawing (pBkBrush, lOutlinePen, lPathGeometry));

// Shadow added by caller
		lDrawing = lDrawingGroup;
	}
	return lDrawing;
}

#endif
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

CAgentBalloonThink::CAgentBalloonThink()
{
	InitLayout ();
}

#ifndef	__cplusplus_cli
CAgentBalloonThink::~CAgentBalloonThink()
{
}
#endif

/////////////////////////////////////////////////////////////////////////////

void CAgentBalloonThink::InitLayout ()
{
	CAgentBalloonShape::InitLayout ();

#ifdef	__cplusplus_cli
#else
	mCalloutSize.cx = MulDiv (mCalloutSize.cx, 3, 2);
	mCalloutSize.cy = MulDiv (mCalloutSize.cy, 4, 1);
#ifdef	_DEBUG_LAYOUT
	LogMessage (_DEBUG_LAYOUT, _T("%s Rounding [%d %d] Callout [%d %d]"), ObjTypeName(this), mRounding.cx, mRounding.cy, mCalloutSize.cx, mCalloutSize.cy);
#endif
#endif
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
GraphicsPath^ CAgentBalloonThink::GetBalloonRgn ()
#else
HRGN CAgentBalloonThink::GetBalloonRgn ()
#endif
{
#ifdef	__cplusplus_cli
//TODO
	return nullptr;
#else
	if	(mUseGdiplus)
	{
		return NULL;
	}
	else
	{
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
			lRgn1 = ::CreateRectRgnIndirect (lWinRect);
#else
			Gdiplus::RectF	lEllipses [3];
			INT_PTR			lEllipseNdx;

			GetCalloutEllipses (lEllipses);
			lRgn1 = ::CreateRoundRectRgn (mBalloonRect.left, mBalloonRect.top, ((mBalloonRect.right-1))+1, ((mBalloonRect.bottom-1))+1, mRounding.cx, mRounding.cy);

			for	(lEllipseNdx = 0; lEllipseNdx < 3; lEllipseNdx++)
			{
				lRgn2 = ::CreateEllipticRgn ((int)lEllipses [lEllipseNdx].GetLeft(), (int)lEllipses [lEllipseNdx].GetTop(), (int)lEllipses [lEllipseNdx].GetRight()+1, (int)lEllipses [lEllipseNdx].GetBottom()+1);
				::CombineRgn (lRgn1, lRgn1, lRgn2, RGN_OR);
				lRgn2.Close ();
			}
#endif
		}
		return lRgn1.Detach();
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////

#ifndef	__cplusplus_cli
bool CAgentBalloonThink::Draw (HDC pDC, COLORREF pBkColor, COLORREF pBrColor)
{
#ifdef	_DEBUG_DRAW
	LogMessage (_DEBUG_DRAW, _T("CAgentBalloonThink::Draw [%s] [%s] [%s] [%s] [%s] [%s]"), FormatRect(mBounds), FormatRect(mBalloonRect), FormatRect(mTextRect), FormatSize(mCalloutSize), FormatPoint(mCalloutBeg), FormatPoint(mCalloutEnd));
#endif
	if	(
			(!mBalloonRect.IsRectEmpty())
		&&	(mUseGdiplus)
		)
	{
		Gdiplus::Graphics	lGraphics (pDC);
		Gdiplus::Color		lBkColor (GetRValue(pBkColor), GetGValue(pBkColor), GetBValue(pBkColor));
		Gdiplus::Color		lBrColor (GetRValue(pBrColor), GetGValue(pBrColor), GetBValue(pBrColor));

		lGraphics.Clear (Gdiplus::Color (0,0,0,0));
		return Draw (lGraphics, lBkColor, lBrColor);
	}
	else
	if	(!mBalloonRect.IsRectEmpty())
	{
		CBrushHandle	lFillBrush = CImageAlpha::GetAlphaBrush (pBkColor, 255);
		CBrushHandle	lFrameBrush = CImageAlpha::GetAlphaBrush (pBrColor, 255);
		CRgnHandle		lRgn = GetBalloonRgn ();

#ifdef	_DEBUG_DRAW
		{
			CRect			lClipRect;
			CBrushHandle	lBrush;
			lBrush = CreateSolidBrush (RGB (0xFF, 0x80, 0xFF));
			::GetClipBox (pDC, &lClipRect);
			::FillRect (pDC, &lClipRect, lBrush);
		}
#endif

		::FillRgn (pDC, lRgn, lFillBrush);
		::FrameRgn (pDC, lRgn, lFrameBrush, 1, 1);

#ifdef	_DEBUG_DRAW
		{
			CBrushHandle lBrush;
			lBrush = ::CreateSolidBrush (RGB (0xFF,0x00,0x00));
			::FillRect (pDC, CRect (mCalloutEnd.x-1, mCalloutEnd.y-1, mCalloutEnd.x+2, mCalloutEnd.y+2), lBrush);
		}
#endif
		return true;
	}
	return false;
}
#endif

#ifdef	__cplusplus_cli
System::Boolean CAgentBalloonThink::Draw (System::Drawing::Graphics^ pGraphics, System::Drawing::Color pBkColor, System::Drawing::Color pBrColor)
#else
bool CAgentBalloonThink::Draw (Gdiplus::Graphics & pGraphics, Gdiplus::Color & pBkColor, Gdiplus::Color & pBrColor)
#endif
{
#ifdef	__cplusplus_cli
	if	(!mBalloonRect.IsEmpty)
	{
		GraphicsPath^		lShapePath = gcnew GraphicsPath;
		GraphicsPath^		lOutlinePath;
		array<RectangleF>^	lEllipses = gcnew array<RectangleF> (3);
		SolidBrush^			lFillBrush = gcnew SolidBrush (pBkColor);
		SolidBrush^			lOutlineBrush = gcnew SolidBrush (pBrColor);
		Pen^				lOutlinePen = gcnew Pen (pBrColor, 1.9f);

		lShapePath->StartFigure ();
		MakeRoundRect (lShapePath);
		lShapePath->CloseFigure ();

		GetCalloutEllipses (lEllipses);
		for each (RectangleF lEllipse in lEllipses)
		{
			lShapePath->AddEllipse (lEllipse);
		}
		lShapePath->FillMode = FillMode::Winding;
		lOutlinePath = safe_cast<GraphicsPath^> (lShapePath->Clone());
		lOutlinePath->Widen (lOutlinePen);

		pGraphics->CompositingMode = CompositingMode::SourceOver;
		pGraphics->CompositingQuality = CompositingQuality::HighQuality;
		pGraphics->SmoothingMode = SmoothingMode::HighQuality;
		pGraphics->PixelOffsetMode = PixelOffsetMode::HighQuality;

		DrawShadow (lShapePath, pGraphics);
		pGraphics->FillPath (lOutlineBrush, lOutlinePath);
		pGraphics->FillPath (lFillBrush, lShapePath);

		return true;
	}
#else
	if	(!mBalloonRect.IsRectEmpty())
	{
		Gdiplus::GraphicsPath	lShapePath;
		Gdiplus::RectF			lEllipses [3];
		INT_PTR					lEllipseNdx;
		Gdiplus::SolidBrush		lBrush (pBkColor);
		Gdiplus::Pen			lPen (pBrColor, 1.0f);

		lShapePath.StartFigure ();
		MakeRoundRect (lShapePath);
		lShapePath.CloseFigure ();

		GetCalloutEllipses (lEllipses);
		for	(lEllipseNdx = 0; lEllipseNdx < 3; lEllipseNdx++)
		{
			lShapePath.AddEllipse (lEllipses [lEllipseNdx]);
		}

		pGraphics.SetCompositingMode (Gdiplus::CompositingModeSourceOver);
		pGraphics.SetCompositingQuality (Gdiplus::CompositingQualityHighQuality);
		pGraphics.SetSmoothingMode (Gdiplus::SmoothingModeHighQuality);
		pGraphics.SetPixelOffsetMode (Gdiplus::PixelOffsetModeHighQuality);

		DrawShadow (lShapePath, pGraphics);
		pGraphics.FillPath (&lBrush, &lShapePath);
		pGraphics.DrawPath (&lPen, &lShapePath);

		return true;
	}
#endif
	return false;
}

#ifdef	__cplusplus_cli
void CAgentBalloonThink::GetCalloutEllipses (array<System::Drawing::RectangleF>^ pEllipses)
#else
void CAgentBalloonThink::GetCalloutEllipses (Gdiplus::RectF* pEllipses)
#endif
{
#ifdef	__cplusplus_cli
	Single		lCalloutAngle = (Single)Math::Atan2 ((mCalloutEnd.Y-mCalloutBeg.Y), (mCalloutEnd.X-mCalloutBeg.X));
	Single		lCalloutWidth = mCalloutSize.Width;
	Single		lCalloutLength = (Single)Math::Sqrt ((mCalloutEnd.Y-mCalloutBeg.Y)*(mCalloutEnd.Y-mCalloutBeg.Y) + (mCalloutEnd.X-mCalloutBeg.X)*(mCalloutEnd.X-mCalloutBeg.X));
	Single		lEllipseRadius [3];
	Single		lEllipseOffset [3];
	PointF		lEllipseCenter;
	int			lEllipseNdx;
#else
	double		lCalloutAngle = atan2 ((double)(mCalloutEnd.y-mCalloutBeg.y), (double)(mCalloutEnd.x-mCalloutBeg.x));
	double		lCalloutWidth = (double)(mCalloutSize.cx);
	double		lCalloutLength = _hypot ((double)(mCalloutEnd.y-mCalloutBeg.y), (double)(mCalloutEnd.x-mCalloutBeg.x));
	double		lEllipseRadius [3];
	double		lEllipseOffset [3];
	_complex	lEllipseCenter;
	INT_PTR		lEllipseNdx;
#endif	

#ifdef	__cplusplus_cli
	lEllipseRadius [0] = lCalloutWidth * 0.5f;
	lEllipseRadius [1] = lCalloutWidth * 0.7f;
	lEllipseRadius [2] = lCalloutWidth * 0.9f;
#else
	if	(mUseGdiplus)
	{
		lEllipseRadius [0] = lCalloutWidth * 0.5;
		lEllipseRadius [1] = lCalloutWidth * 0.7;
		lEllipseRadius [2] = lCalloutWidth * 0.9;
	}
	else
	{
		lEllipseRadius [0] = lCalloutWidth * 0.6;
		lEllipseRadius [1] = lCalloutWidth * 0.75;
		lEllipseRadius [2] = lCalloutWidth;
	}
#endif	
	lEllipseOffset [0] = lCalloutLength * 0.12f;
	lEllipseOffset [1] = lCalloutLength * 0.4f;
	lEllipseOffset [2] = lCalloutLength * 0.75f;

	for	(lEllipseNdx = 0; lEllipseNdx < 3; lEllipseNdx++)
	{
#ifdef	__cplusplus_cli
		lEllipseCenter.X = (float)(Math::Cos (lCalloutAngle) * lEllipseOffset [lEllipseNdx]);
		lEllipseCenter.Y = (float)(Math::Sin (lCalloutAngle) * lEllipseOffset [lEllipseNdx]);

		pEllipses [lEllipseNdx].X = (float)(lEllipseCenter.X - lEllipseRadius [lEllipseNdx]);
		pEllipses [lEllipseNdx].Y = (float)(lEllipseCenter.Y - lEllipseRadius [lEllipseNdx]);
		pEllipses [lEllipseNdx].Width = (float)(lEllipseRadius [lEllipseNdx] * 2.0);
		pEllipses [lEllipseNdx].Height = pEllipses [lEllipseNdx].Width;

		pEllipses [lEllipseNdx].X += mCalloutBeg.X;
		pEllipses [lEllipseNdx].Y += mCalloutBeg.Y;
#else
		lEllipseCenter.x = cos (lCalloutAngle) * lEllipseOffset [lEllipseNdx];
		lEllipseCenter.y = sin (lCalloutAngle) * lEllipseOffset [lEllipseNdx];

		if	(mUseGdiplus)
		{
			pEllipses [lEllipseNdx].X = (Gdiplus::REAL)(lEllipseCenter.x - lEllipseRadius [lEllipseNdx]);
			pEllipses [lEllipseNdx].Y = (Gdiplus::REAL)(lEllipseCenter.y - lEllipseRadius [lEllipseNdx]);
			pEllipses [lEllipseNdx].Width = pEllipses [lEllipseNdx].Height = (Gdiplus::REAL)(lEllipseRadius [lEllipseNdx] * 2.0);
		}
		else
		{
			CRect	lEllipseRect;

			lEllipseRect.left = dtol (lEllipseCenter.x - lEllipseRadius [lEllipseNdx]);
			lEllipseRect.top = dtol (lEllipseCenter.y - lEllipseRadius [lEllipseNdx]);
			lEllipseRect.right = lEllipseRect.left + dtolOdd (lEllipseRadius [lEllipseNdx] * 2.0) + 1L;
			lEllipseRect.bottom = lEllipseRect.top + dtolOdd (lEllipseRadius [lEllipseNdx] * 2.0) + 1L;

			pEllipses [lEllipseNdx].X = (Gdiplus::REAL)lEllipseRect.left;
			pEllipses [lEllipseNdx].Y = (Gdiplus::REAL)lEllipseRect.top;
			pEllipses [lEllipseNdx].Width = (Gdiplus::REAL)lEllipseRect.Width();
			pEllipses [lEllipseNdx].Height = (Gdiplus::REAL)lEllipseRect.Height();
		}

		pEllipses [lEllipseNdx].X += mCalloutBeg.x;
		pEllipses [lEllipseNdx].Y += mCalloutBeg.y;
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
/////////////////////////////////////////////////////////////////////////////

System::Windows::Media::Drawing^ CAgentBalloonThink::MakeDrawing (System::Windows::Media::Brush^ pBkBrush, System::Windows::Media::Brush^ pBrBrush)
{
	System::Windows::Media::Drawing^	lDrawing = nullptr;

	if	(!mBalloonRect.IsEmpty)
	{
		System::Windows::Media::Pen^				lOutlinePen = gcnew System::Windows::Media::Pen (pBrBrush, 1);
		System::Windows::Media::DrawingGroup^		lDrawingGroup = gcnew System::Windows::Media::DrawingGroup ();
		System::Windows::Media::PathGeometry^		lPathGeometry;
		System::Windows::Media::RectangleGeometry^	lRectGeometry;
		System::Windows::Media::PathGeometry^		lCalloutGeometry;
		array<RectangleF>^							lEllipses = gcnew array<RectangleF> (3);

		lPathGeometry = gcnew System::Windows::Media::PathGeometry ();
		lPathGeometry->FillRule = System::Windows::Media::FillRule::Nonzero;

		lRectGeometry = gcnew System::Windows::Media::RectangleGeometry (System::Windows::Rect (mBalloonRect.Left+1, mBalloonRect.Top+1, mBalloonRect.Width-2, mBalloonRect.Height-2), mRounding.Width, mRounding.Height);
		lPathGeometry->AddGeometry (lRectGeometry->GetFlattenedPathGeometry ());

		lCalloutGeometry = gcnew System::Windows::Media::PathGeometry ();
		lCalloutGeometry->FillRule = System::Windows::Media::FillRule::Nonzero;
		GetCalloutEllipses (lEllipses);
		for each (RectangleF lEllipse in lEllipses)
		{
			lCalloutGeometry->AddGeometry (gcnew System::Windows::Media::EllipseGeometry (System::Windows::Rect (lEllipse.Left, lEllipse.Top, lEllipse.Width, lEllipse.Height)));
		}
		lPathGeometry->AddGeometry (lCalloutGeometry);

		lPathGeometry = lPathGeometry->GetOutlinedPathGeometry ();
		lDrawingGroup->Children->Add (gcnew System::Windows::Media::GeometryDrawing (pBkBrush, lOutlinePen, lPathGeometry));

// Shadow added by caller
		lDrawing = lDrawingGroup;
	}
	return lDrawing;
}

#endif
/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
#ifndef	__cplusplus_cli
/////////////////////////////////////////////////////////////////////////////

void CAgentBalloonShape::InitTrace (const CRect& pRect)
{
#ifdef	_TRACE_LAYOUT_NOT
	try
	{
		if	(mTraceBuffer = new CImageBuffer)
		{
			mTraceBuffer->CreateBuffer (pRect.Size()+CSize(4,4));
			SetWindowOrgEx (*mTraceBuffer->mDC, pRect.left-2, pRect.top-2, NULL);
		}
	}
	catch AnyExceptionSilent
#endif
}

void CAgentBalloonShape::InitTrace (const CRect& pRect1, const CRect& pRect2)
{
#ifdef	_TRACE_LAYOUT
	try
	{
		CRect	lBufferRect;

		lBufferRect.UnionRect (pRect1, pRect2);
		InitTrace (lBufferRect);
	}
	catch AnyExceptionSilent
#endif
}

void CAgentBalloonShape::InitTrace (const CRect& pRect1, const CRect& pRect2, const CRect& pRect3)
{
#ifdef	_TRACE_LAYOUT
	try
	{
		CRect	lBufferRect;

		lBufferRect.UnionRect (pRect1, pRect2);
		lBufferRect.UnionRect (lBufferRect, pRect3);
		InitTrace (lBufferRect);
	}
	catch AnyExceptionSilent
#endif
}

void CAgentBalloonShape::ShowTrace ()
{
#ifdef	_TRACE_LAYOUT
	try
	{
		if	(mTraceBuffer)
		{
			mTraceBuffer->EndBuffer ();

			if	(
					(mTraceBuffer->mImage)
				&&	(
						(mTraceDebugger)
					||	(mTraceDebugger = new CImageDebugger)
					)
				)
			{
				mTraceDebugger->ShowBitmap (mTraceBuffer->mImage->Detach(), 1, _T("Calc"), true, false);
			}

			mTraceBuffer = NULL;
		}
	}
	catch AnyExceptionSilent
#endif
}

/////////////////////////////////////////////////////////////////////////////

void CAgentBalloonShape::TraceRect (const CRect& pRect, COLORREF pColor) const
{
#ifdef	_TRACE_LAYOUT
	try
	{
		if	(mTraceBuffer)
		{
			CBrushHandle lBrush = ::CreateSolidBrush (pColor);
			FrameRect (*mTraceBuffer->mDC, pRect, lBrush);
		}
	}
	catch AnyExceptionSilent
#endif
}

void CAgentBalloonShape::TracePointFill (const CPoint& pPoint, COLORREF pColor, UINT pSize) const
{
#ifdef	_TRACE_LAYOUT
	try
	{
		if	(mTraceBuffer)
		{
			FillSolidRect (*mTraceBuffer->mDC, CRect (pPoint.x-pSize, pPoint.y-pSize, pPoint.x+pSize+1, pPoint.y+pSize+1), pColor);
		}
	}
	catch AnyExceptionSilent
#endif
}

void CAgentBalloonShape::TracePointFill (const _complex& pPoint, COLORREF pColor, UINT pSize) const
{
#ifdef	_TRACE_LAYOUT
	try
	{
		TracePointFill (CPoint (dtol(pPoint.x), dtol(pPoint.y)), pColor, pSize);
	}
	catch AnyExceptionSilent
#endif
}

void CAgentBalloonShape::TracePointFrame (const CPoint& pPoint, COLORREF pColor, UINT pSize) const
{
#ifdef	_TRACE_LAYOUT
	try
	{
		if	(mTraceBuffer)
		{
			CBrushHandle lBrush = ::CreateSolidBrush (pColor);
			FrameRect (*mTraceBuffer->mDC, CRect (pPoint.x-pSize, pPoint.y-pSize, pPoint.x+pSize+1, pPoint.y+pSize+1), lBrush);
		}
	}
	catch AnyExceptionSilent
#endif
}

void CAgentBalloonShape::TracePointFrame (const _complex& pPoint, COLORREF pColor, UINT pSize) const
{
#ifdef	_TRACE_LAYOUT
	try
	{
		TracePointFrame (CPoint (dtol(pPoint.x), dtol(pPoint.y)), pColor, pSize);
	}
	catch AnyExceptionSilent
#endif
}

/////////////////////////////////////////////////////////////////////////////
#endif	// __cplusplus_cli
#endif	// _DEBUG
/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
} // namespace DoubleAgent
#endif
/////////////////////////////////////////////////////////////////////////////
