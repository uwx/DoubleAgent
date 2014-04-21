/////////////////////////////////////////////////////////////////////////////
//	Copyright 2009-2014 Cinnamon Software Inc.
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
#if !defined(AFX_BITMAPBUFFER_H__4C0291CE_CE7B_47C3_B023_0AE850050B2E__INCLUDED_)
#define AFX_BITMAPBUFFER_H__4C0291CE_CE7B_47C3_B023_0AE850050B2E__INCLUDED_
#pragma once

//////////////////////////////////////////////////////////////////////

class CBitmapBuffer
{
public:
	CBitmapBuffer (LPCTSTR pDebugName = NULL);
	virtual ~CBitmapBuffer ();

//	Attributes
public:
	CDC		mDC;
	CBitmap	mBitmap;
	HGDIOBJ	mOldBitmap;
	LPBYTE	mBitmapBits;
	HGDIOBJ	mOldFont;

//	Operations
public:
	bool StartBuffer (CDC* pCompatibleDC = NULL);
	void EndBuffer (bool pReleaseBuffer = false, bool pReleaseDC = true);
	bool PauseBuffer ();
	bool ResumeBuffer ();

	bool CreateScaledBuffer (const CSize& pSize, bool pForBlending = false, UINT pScale = 3);
	bool UnscaleBuffer (UINT pScale = 3, UINT pBlend = 1);

	virtual bool CreateBuffer (const CSize& pBufferSize, bool pForBlending = false, bool pForceNewBuffer = false, bool pTopDown = false);
	virtual bool DrawBuffer (HDC pTargetDC, const CRect& pRect, bool pReleaseBuffer = false, bool pReleaseDC = true);

	CSize GetBitmapSize () const;
	static bool SaveBitmap (HBITMAP pBitmap, LPCTSTR pFileName);

// Implementation
protected:
	bool NewBufferRequired (CBitmap& pBitmap, const CSize& pSize, bool pForBlending, int pSizeFactor = 3);

#ifdef	_DEBUG
public:
	CString												mDebugName;
#endif
#ifdef	_DEBUG_BITMAP_BUFFERS
public:
	static void DumpUsage (UINT pLogLevel, LPCTSTR pTitle = NULL);
	static void DumpInstances (UINT pLogLevel, LPCTSTR pTitle = NULL);
protected:
	virtual UINT IsInUse () const;
	virtual UINT IsDcInUse () const;
	virtual UINT IsBitmapUnUse (DWORD& pBitmapBytes) const;
	virtual void LogBitmapUnUse (UINT pLogLevel) const;

private:
	static CTypedPtrArray <CPtrArray, CBitmapBuffer *>	mInstances;
protected:
	ULONGLONG											mCreated;
	ULONGLONG											mDeleted;
	ULONGLONG											mUsed;
	LONGLONG											mStartTime;
	double												mInUseTime;
	static UINT											mTotalInstances;
	static UINT											mMaxInstances;
	static UINT											mNowInUse;
	static UINT											mTotalInUse;
	static UINT											mMaxInUse;
	static ULONGLONG									mTotalCreated;
	static ULONGLONG									mTotalDeleted;
	static double										mTotalCreateTime;
	static double										mTotalDeleteTime;
	static double										mTotalInUseTime;
	static CCriticalSection								mUsageCriticalSection;
#endif
};

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

class CBitmapBufferScaled : public CBitmapBuffer
{
public:
	CBitmapBufferScaled (UINT pScale = 3, UINT pBlend = 1, LPCTSTR pDebugName = NULL);
	virtual ~CBitmapBufferScaled ();

//	Attributes
public:
	UINT	mScale;
	UINT	mBlend;

//	Operations
public:
	virtual bool CreateBuffer (const CSize& pBufferSize, bool pForBlending = false, bool pForceNewBuffer = false, bool pTopDown = false);
	virtual bool DrawBuffer (HDC pTargetDC, const CRect& pRect, bool pReleaseBuffer = false, bool pReleaseDC = true);
	virtual void EraseBuffer (COLORREF pColor);
	virtual bool UnscaleBuffer ();

// Implementation
private:
	CBitmap	mScaledBitmap;
	LPBYTE	mScaledBits;

#ifdef	_DEBUG_BITMAP_BUFFERS
protected:
	virtual UINT IsBitmapUnUse (DWORD& pBitmapBytes) const;
	virtual void LogBitmapUnUse (UINT pLogLevel) const;
#endif
};

//////////////////////////////////////////////////////////////////////

#ifdef	_DEBUG
#define	BufferDebugName(s)	_T(#s)
#else
#define	BufferDebugName(s)	NULL
#endif

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_BITMAPBUFFER_H__4C0291CE_CE7B_47C3_B023_0AE850050B2E__INCLUDED_)
