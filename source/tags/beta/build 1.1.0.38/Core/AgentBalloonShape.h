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
#pragma once

#include "UseGdiplus.h"

/////////////////////////////////////////////////////////////////////////////

class CAgentBalloonShape
{
protected:
	CAgentBalloonShape ();
public:
	virtual ~CAgentBalloonShape ();

// Attributes
public:
	CRect				mTextRect;
	CRect				mBalloonRect;
	CPoint				mShadowOffset;
	tPtr <CUseGdiplus>	mUseGdiplus;

// Operations
public:
	virtual void InitLayout ();
	virtual CRect RecalcLayout (const CRect & pTextRect, const CRect & pRefRect, const CRect & pBounds);

	virtual HRGN GetBalloonRgn () = 0;
	virtual bool Draw (HDC pDC, COLORREF pBkColor, COLORREF pBrColor) = 0;

// Overrides

// Implementation
protected:
	void CalcLayout (const CRect & pTextRect, const CRect & pRefRect, const CRect & pBounds, CRect & pBalloonRect, CPoint & pCalloutBeg, CPoint & pCalloutEnd);
	void CalcRectIntersect (const _complex & pRefPoint, const _complex & pRectCenter, const _complex & pRectSize, _complex & pRectIntersect, double pMinAngle = 0.0);
	bool ValidateBalloonRect (CRect & pBalloonRect, const CRect & pRefRect, const CRect & pBounds);
	void FixupNearPoint (CPoint & pPoint, const CRect & pRefRect, long pNearness = 2);
	void MakeRoundRect (Gdiplus::GraphicsPath & pShapePath);
	void DrawShadow (Gdiplus::GraphicsPath & pShapePath, Gdiplus::Graphics & pGraphics);

protected:
	CRect	mBounds;
	CSize	mRounding;
	CSize	mCalloutSize;
	CPoint	mCalloutBeg;
	CPoint	mCalloutEnd;

#ifdef	_DEBUG
protected:
	void InitTrace (const CRect & pRect);
	void InitTrace (const CRect & pRect1, const CRect & pRect2);
	void InitTrace (const CRect & pRect1, const CRect & pRect2, const CRect & pRect3);
	void ShowTrace ();
	void TraceRect (const CRect & pRect, COLORREF pColor) const;
	void TracePointFill (const CPoint & pPoint, COLORREF pColor, UINT pSize = 1) const;
	void TracePointFill (const _complex & pPoint, COLORREF pColor, UINT pSize = 1) const;
	void TracePointFrame (const CPoint & pPoint, COLORREF pColor, UINT pSize = 3) const;
	void TracePointFrame (const _complex & pPoint, COLORREF pColor, UINT pSize = 3) const;

protected:
	mutable tPtr <class CImageBuffer>	mTraceBuffer;
	mutable tPtr <class CImageDebugger>	mTraceDebugger;
#endif
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

class CAgentBalloonSpeak : public CAgentBalloonShape
{
public:
	CAgentBalloonSpeak ();
	virtual ~CAgentBalloonSpeak ();

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual void InitLayout ();
	virtual HRGN GetBalloonRgn ();
	virtual bool Draw (HDC pDC, COLORREF pBkColor, COLORREF pBrColor);

// Implementation
protected:
	void GetCalloutPoints (Gdiplus::PointF * pPoints);
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

class CAgentBalloonThink : public CAgentBalloonShape
{
public:
	CAgentBalloonThink ();
	virtual ~CAgentBalloonThink ();

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual void InitLayout ();
	virtual HRGN GetBalloonRgn ();
	virtual bool Draw (HDC pDC, COLORREF pBkColor, COLORREF pBrColor);

// Implementation
protected:
	void GetCalloutEllipses (Gdiplus::RectF * pEllipses);
};

/////////////////////////////////////////////////////////////////////////////
