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
#ifndef	IMAGEBUFFER_H_INCLUDED_
#define IMAGEBUFFER_H_INCLUDED_
#pragma once

#include <atltypes.h>
#include <atlimage.h>
#include "HelperTemplates.h"

//////////////////////////////////////////////////////////////////////

class CImageBuffer
{
public:
	CImageBuffer ();
	virtual ~CImageBuffer ();

//	Attributes
public:
	tPtr <ATL::CImage>		mImage;
	tPtr <ATL::CImageDC>	mDC;
	HGDIOBJ					mOldFont;

//	Operations
public:
	bool StartBuffer ();
	void EndBuffer (bool pReleaseBuffer = false);
	bool PauseBuffer ();
	bool ResumeBuffer ();

	bool CreateScaledBuffer (const CSize & pSize, bool pForBlending = false, UINT pScale = 3);
	bool UnscaleBuffer (UINT pScale = 3, UINT pBlend = 1);

	virtual bool CreateBuffer (const CSize & pBufferSize, bool pForBlending = false, bool pForceNewBuffer = false, bool pTopDown = false);
	virtual bool DrawBuffer (HDC pTargetDC, const CRect & pRect, bool pReleaseBuffer = false, bool pReleaseDC = true);

	HDC GetDC () const;
	HBITMAP GetImage () const;
	CSize GetImageSize () const;

// Implementation
protected:
	bool NewBufferRequired (ATL::CImage * pImage, const CSize & pSize, bool pForBlending, int pSizeFactor = 3);
};

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

class CImageBufferScaled : public CImageBuffer
{
public:
	CImageBufferScaled (UINT pScale = 3, UINT pBlend = 1);
	virtual ~CImageBufferScaled ();

//	Attributes
public:
	UINT	mScale;
	UINT	mBlend;

//	Operations
public:
	virtual bool CreateBuffer (const CSize & pBufferSize, bool pForBlending = false, bool pForceNewBuffer = false, bool pTopDown = false);
	virtual bool DrawBuffer (HDC pTargetDC, const CRect & pRect, bool pReleaseBuffer = false, bool pReleaseDC = true);
	virtual void EraseBuffer (COLORREF pColor);
	virtual bool UnscaleBuffer ();

// Implementation
private:
	tPtr <ATL::CImage>	mScaledImage;
};

//////////////////////////////////////////////////////////////////////

#endif // IMAGEBUFFER_H_INCLUDED_
