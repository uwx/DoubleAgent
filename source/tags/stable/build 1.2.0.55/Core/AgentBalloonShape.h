/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
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

#ifndef	__cplusplus_cli
#include "UseGdiplus.h"
#include <math.h>
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
namespace DoubleAgent {
#endif
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
private ref class CAgentBalloonShape abstract
#else
class CAgentBalloonShape
#endif
{
protected:
	CAgentBalloonShape ();
public:
#ifndef	__cplusplus_cli
	virtual ~CAgentBalloonShape ();
#endif	

// Attributes
public:
#ifdef	__cplusplus_cli
	System::Drawing::RectangleF		mTextRect;
	System::Drawing::RectangleF		mBalloonRect;
	System::Drawing::PointF			mShadowOffset;
#else
	CRect							mTextRect;
	CRect							mBalloonRect;
	CPoint							mShadowOffset;
	tPtr <CUseGdiplus>				mUseGdiplus;
#endif

// Attributes
#ifdef	__cplusplus_cli
	property System::Drawing::SizeF Rounding {System::Drawing::SizeF get();}
	property System::Drawing::SizeF LayoutMargin {System::Drawing::SizeF get();}
#else
	CSize GetRounding () const;
	CSize GetLayoutMargin () const;
#endif

// Operations
public:
	virtual void InitLayout ();
#ifdef	__cplusplus_cli
	virtual System::Drawing::RectangleF RecalcLayout (System::Drawing::RectangleF pTextRect, System::Drawing::RectangleF pRefRect, System::Drawing::RectangleF pBounds);
	virtual System::Drawing::Drawing2D::GraphicsPath^ GetBalloonRgn () = 0;
	virtual System::Boolean Draw (System::Drawing::Graphics^ pGraphics, System::Drawing::Color pBkColor, System::Drawing::Color pBrColor) = 0;

	virtual System::Windows::Media::Drawing^ MakeDrawing (System::Drawing::Color pBkColor, System::Drawing::Color pBrColor);
	virtual System::Windows::Media::Drawing^ MakeDrawing (System::Windows::Media::Color pBkColor, System::Windows::Media::Color pBrColor);
	virtual System::Windows::Media::Drawing^ MakeDrawing (System::Windows::Media::Brush^ pBkBrush, System::Windows::Media::Brush^ pBrBrush) = 0;
#else
	virtual CRect RecalcLayout (const CRect& pTextRect, const CRect& pRefRect, const CRect& pBounds);
	virtual HRGN GetBalloonRgn () = 0;
	virtual bool Draw (HDC pDC, COLORREF pBkColor, COLORREF pBrColor) = 0;
	virtual bool Draw (Gdiplus::Graphics & pGraphics, Gdiplus::Color & pBkColor, Gdiplus::Color & pBrColor) = 0;
#endif

// Overrides

// Implementation
protected:
#ifdef	__cplusplus_cli
	void CalcLayout (System::Drawing::RectangleF pTextRect, System::Drawing::RectangleF pRefRect, System::Drawing::RectangleF pBounds, System::Drawing::RectangleF% pBalloonRect, System::Drawing::PointF% pCalloutBeg, System::Drawing::PointF% pCalloutEnd);
	void CalcRectIntersect (System::Drawing::PointF pRefPoint, System::Drawing::PointF pRectCenter, System::Drawing::SizeF pRectSize, System::Drawing::PointF% pRectIntersect);
	void CalcRectIntersect (System::Drawing::PointF pRefPoint, System::Drawing::PointF pRectCenter, System::Drawing::SizeF pRectSize, System::Drawing::PointF% pRectIntersect, System::Single pMinAngle);
	System::Boolean ValidateBalloonRect (System::Drawing::RectangleF% pBalloonRect, System::Drawing::RectangleF pRefRect, System::Drawing::RectangleF pBounds);
	void FixupNearPoint (System::Drawing::PointF% pPoint, System::Drawing::RectangleF pRefRect);
	void FixupNearPoint (System::Drawing::PointF% pPoint, System::Drawing::RectangleF pRefRect, long pNearness);
	void MakeRoundRect (System::Drawing::Drawing2D::GraphicsPath^ pShapePath);
	void DrawShadow (System::Drawing::Drawing2D::GraphicsPath^ pShapePath, System::Drawing::Graphics^ pGraphics);
#else
	void CalcLayout (const CRect& pTextRect, const CRect& pRefRect, const CRect& pBounds, CRect& pBalloonRect, CPoint& pCalloutBeg, CPoint& pCalloutEnd);
	void CalcRectIntersect (const _complex& pRefPoint, const _complex& pRectCenter, const _complex& pRectSize, _complex& pRectIntersect, double pMinAngle = 0.0);
	bool ValidateBalloonRect (CRect& pBalloonRect, const CRect& pRefRect, const CRect& pBounds);
	void FixupNearPoint (CPoint& pPoint, const CRect& pRefRect, long pNearness = 2);
	void MakeRoundRect (Gdiplus::GraphicsPath& pShapePath);
	void DrawShadow (Gdiplus::GraphicsPath& pShapePath, Gdiplus::Graphics& pGraphics);
#endif

protected:
#ifdef	__cplusplus_cli
	System::Drawing::RectangleF	mBounds;
	System::Drawing::SizeF		mRounding;
	System::Drawing::SizeF		mCalloutSize;
	System::Drawing::PointF		mCalloutBeg;
	System::Drawing::PointF		mCalloutEnd;
#else
	CRect						mBounds;
	CSize						mRounding;
	CSize						mCalloutSize;
	CPoint						mCalloutBeg;
	CPoint						mCalloutEnd;
#endif

#ifndef	__cplusplus_cli
#ifdef	_DEBUG
protected:
	void InitTrace (const CRect& pRect);
	void InitTrace (const CRect& pRect1, const CRect& pRect2);
	void InitTrace (const CRect& pRect1, const CRect& pRect2, const CRect& pRect3);
	void ShowTrace ();
	void TraceRect (const CRect& pRect, COLORREF pColor) const;
	void TracePointFill (const CPoint& pPoint, COLORREF pColor, UINT pSize = 1) const;
	void TracePointFill (const _complex& pPoint, COLORREF pColor, UINT pSize = 1) const;
	void TracePointFrame (const CPoint& pPoint, COLORREF pColor, UINT pSize = 3) const;
	void TracePointFrame (const _complex& pPoint, COLORREF pColor, UINT pSize = 3) const;

protected:
	mutable tPtr <class CImageBuffer>	mTraceBuffer;
	mutable tPtr <class CImageDebugger>	mTraceDebugger;
#endif
#endif
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
private ref class CAgentBalloonSpeak : public CAgentBalloonShape
#else
class CAgentBalloonSpeak : public CAgentBalloonShape
#endif
{
public:
	CAgentBalloonSpeak ();
#ifndef	__cplusplus_cli
	virtual ~CAgentBalloonSpeak ();
#endif	

// Attributes
public:

// Operations
public:

// Overrides
public:
#ifdef	__cplusplus_cli
	virtual void InitLayout () override;
	virtual System::Drawing::Drawing2D::GraphicsPath^ GetBalloonRgn () override;
	virtual System::Boolean Draw (System::Drawing::Graphics^ pGraphics, System::Drawing::Color pBkColor, System::Drawing::Color pBrColor) override;
	virtual System::Windows::Media::Drawing^ MakeDrawing (System::Windows::Media::Brush^ pBkBrush, System::Windows::Media::Brush^ pBrBrush) override;
#else
	virtual void InitLayout ();
	virtual HRGN GetBalloonRgn ();
	virtual bool Draw (HDC pDC, COLORREF pBkColor, COLORREF pBrColor);
	virtual bool Draw (Gdiplus::Graphics & pGraphics, Gdiplus::Color & pBkColor, Gdiplus::Color & pBrColor);
#endif

// Implementation
protected:
#ifdef	__cplusplus_cli
	void GetCalloutPoints (array<System::Drawing::PointF>^ pPoints);
#else
	void GetCalloutPoints (Gdiplus::PointF* pPoints);
#endif
};

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

#ifdef	__cplusplus_cli
private ref class CAgentBalloonThink : public CAgentBalloonShape
#else
class CAgentBalloonThink : public CAgentBalloonShape
#endif
{
public:
	CAgentBalloonThink ();
#ifndef	__cplusplus_cli
	virtual ~CAgentBalloonThink ();
#endif	

// Attributes
public:

// Operations
public:

// Overrides
public:
#ifdef	__cplusplus_cli
	virtual void InitLayout () override;
	virtual System::Drawing::Drawing2D::GraphicsPath^ GetBalloonRgn () override;
	virtual System::Boolean Draw (System::Drawing::Graphics^ pGraphics, System::Drawing::Color pBkColor, System::Drawing::Color pBrColor) override;
	virtual System::Windows::Media::Drawing^ MakeDrawing (System::Windows::Media::Brush^ pBkBrush, System::Windows::Media::Brush^ pBrBrush) override;
#else
	virtual void InitLayout ();
	virtual HRGN GetBalloonRgn ();
	virtual bool Draw (HDC pDC, COLORREF pBkColor, COLORREF pBrColor);
	virtual bool Draw (Gdiplus::Graphics & pGraphics, Gdiplus::Color & pBkColor, Gdiplus::Color & pBrColor);
#endif

// Implementation
protected:
#ifdef	__cplusplus_cli
	void GetCalloutEllipses (array<System::Drawing::RectangleF>^ pEllipses);
#else
	void GetCalloutEllipses (Gdiplus::RectF* pEllipses);
#endif
};

/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
} // namespace DoubleAgent
#endif
/////////////////////////////////////////////////////////////////////////////
