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
#ifndef	_HELPERTEMPLATES_H
#define	_HELPERTEMPLATES_H
////////////////////////////////////////////////////////////////////////
template <typename aStruct> struct __declspec(novtable) tS
:   public aStruct
{
    tS () {memset (this, 0, sizeof (aStruct));}
    tS (const aStruct& pSource) {operator= (pSource);}
	tS <aStruct>& operator= (const aStruct& pSource) {((aStruct&) (*this)) = pSource; return *this;}
    aStruct * Clear () {memset (this, 0, sizeof (aStruct)); return this;}
    ~tS () {}
};

template <typename aStruct, typename aSizeType> struct __declspec(novtable) tSS
:   public tS <aStruct>
{
    tSS () {(* (aSizeType *) this) = sizeof (aStruct);}
    tSS (const aStruct& pSource) {operator= (pSource);}
	tSS <aStruct, aSizeType>& operator= (const aStruct& pSource) {((aStruct&) (*this)) = pSource; return *this;}
    aStruct * Clear () {memset (this, 0, sizeof (aStruct)); (* (aSizeType *) this) = sizeof (aStruct); return this;}
    ~tSS () {}
};
////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////
#pragma warning (push)
#pragma warning (disable : 4284)
////////////////////////////////////////////////////////////////////////

template <typename aType> class tPtrFree {protected: static inline void _Init (aType *& pPtr) {}; static inline void _Free (aType * pPtr) {if (pPtr) delete pPtr;}};

template <typename aType, typename aFree = tPtrFree<aType> > class tPtr : protected aFree
{
public:
    tPtr () : mPtr (NULL) {}
    tPtr (aType * pPtr) {_Init (mPtr = pPtr);}
    tPtr (tPtr <aType>& pPtr) {_Init (mPtr = pPtr.Detach ());}
    ~tPtr () {_Free (mPtr); mPtr = NULL;}

    aType * Ptr() {return (mPtr);}
    const aType * Ptr() const {return (mPtr);}
    operator aType * () {return (mPtr);}
    operator const aType * () const {return (mPtr);}
    aType& operator* () {return (*mPtr);}
    const aType& operator* () const {return (*mPtr);}
    aType * operator-> () {return (mPtr);}
    const aType * operator-> () const {return (mPtr);}
    operator bool () const {return (mPtr != NULL);}
    bool operator! () const {return (mPtr == NULL);}

    aType * operator= (aType * pPtr) {if (pPtr != mPtr) _Free (mPtr); _Init (mPtr = pPtr); return (mPtr);}
    aType * operator= (tPtr <aType>& pPtr) {if (pPtr.mPtr != mPtr) _Free (mPtr); _Init (mPtr = pPtr.Detach ()); return (mPtr);}

    tPtr <aType, aFree>& Attach (aType * pPtr) {_Free (mPtr); _Init (mPtr = pPtr); return *this;}
    aType * Detach () {aType * lPtr = mPtr; mPtr = NULL; return lPtr;}
    aType ** Free () {_Free (mPtr); mPtr = NULL; return (&mPtr);}

    aType ** GetInside () {return (&mPtr);}
    aType * const * GetInside () const {return (&mPtr);}
protected:
    aType * mPtr;
};

////////////////////////////////////////////////////////////////////////

template <typename aType> class tArrayPtrFree {protected: static inline void _Init (aType *& pPtr) {}; static inline void _Free (aType * pPtr) {if (pPtr) delete [] pPtr;}};
#if (_MSC_VER >= 1400)
template <> void tArrayPtrFree<char>::_Free (char * pPtr) {if (pPtr) delete pPtr;};
template <> void tArrayPtrFree<unsigned char>::_Free (unsigned char * pPtr) {if (pPtr) delete pPtr;};
template <> void tArrayPtrFree<short>::_Free (short * pPtr) {if (pPtr) delete pPtr;};
template <> void tArrayPtrFree<unsigned short>::_Free (unsigned short * pPtr) {if (pPtr) delete pPtr;};
template <> void tArrayPtrFree<int>::_Free (int * pPtr) {if (pPtr) delete pPtr;};
template <> void tArrayPtrFree<unsigned int>::_Free (unsigned int * pPtr) {if (pPtr) delete pPtr;};
template <> void tArrayPtrFree<long>::_Free (long* pPtr) {if (pPtr) delete pPtr;};
template <> void tArrayPtrFree<unsigned long>::_Free (unsigned long* pPtr) {if (pPtr) delete pPtr;};
template <> void tArrayPtrFree<void*>::_Free (void** pPtr) {if (pPtr) delete pPtr;};
#endif

template <typename aType, typename aFree = tArrayPtrFree<aType> > class tArrayPtr : public tPtr <aType, aFree>
{
    public:

    tArrayPtr () {}
    tArrayPtr (aType * pPtr) : tPtr <aType, aFree> (pPtr) {}
    tArrayPtr (tArrayPtr <aType, aFree>& pPtr) : tPtr <aType, aFree> (pPtr) {}
    ~tArrayPtr () {_Free (mPtr); mPtr = NULL;}

    aType& operator[] (int pNdx) {return (mPtr [pNdx]);}
    const aType& operator[] (int pNdx) const {return (mPtr [pNdx]);}
    aType& operator[] (long pNdx) {return (mPtr [pNdx]);}
    const aType& operator[] (long pNdx) const {return (mPtr [pNdx]);}
#ifdef	_WIN64
    aType& operator[] (__int64 pNdx) {return (mPtr [pNdx]);}
    const aType& operator[] (__int64 pNdx) const {return (mPtr [pNdx]);}
#endif
    aType * operator= (aType * pPtr) {if (pPtr != mPtr) _Free (mPtr); _Init (mPtr = pPtr); return (mPtr);}
    aType * operator= (tArrayPtr <aType, aFree>& pPtr) {if (pPtr.mPtr != mPtr) _Free (mPtr); _Init (mPtr = pPtr.Detach ()); return (mPtr);}
};

////////////////////////////////////////////////////////////////////////

template <typename aType> class tApiPtrFree {protected: static inline void _Init (aType *& pPtr) {}; static inline void _Free (aType * pPtr) {if (pPtr) pPtr->aType::operator delete (pPtr);}};

template <typename aType, typename aFree = tApiPtrFree<aType> > class tApiPtr : public tPtr <aType, aFree>
{
    public:

    tApiPtr () {}
    tApiPtr (aType * pPtr) : tPtr <aType, aFree> (pPtr) {}
    tApiPtr (tApiPtr <aType, aFree>& pPtr) : tPtr <aType, aFree> (pPtr) {}
    ~tApiPtr () {_Free (mPtr); mPtr = NULL;}

    aType * operator= (aType * pPtr) {return tPtr <aType, aFree>::operator= (pPtr);}
    aType * operator= (tApiPtr <aType, aFree>& pPtr)  {return tPtr <aType, aFree>::operator= (pPtr);}
};

////////////////////////////////////////////////////////////////////////
#ifdef	_OLEAUTO_H_

class tBstrPtrFree {protected: static inline void _Init (OLECHAR *& pPtr) {}; static inline void _Free (OLECHAR * pPtr) {if (pPtr) ::SysFreeString (pPtr);}};

class tBstrPtr : public tPtr <OLECHAR, tBstrPtrFree>
{
    public:

    tBstrPtr () {}
    tBstrPtr (BSTR pPtr) : tPtr <OLECHAR, tBstrPtrFree> (pPtr) {}
    tBstrPtr (const tBstrPtr& pPtr) : tPtr <OLECHAR, tBstrPtrFree> (pPtr.Ptr()?::SysAllocStringLen(pPtr.Ptr(),::SysStringLen((BSTR)pPtr.Ptr())):NULL) {}
    ~tBstrPtr () {_Free (mPtr); mPtr = NULL;}

	operator BSTR () const {return mPtr;}
	bool IsEmpty () const {return (mPtr == NULL) || (::SysStringLen (mPtr) <= 0);}

    BSTR operator= (BSTR pPtr) {return tPtr <OLECHAR, tBstrPtrFree>::operator= (pPtr);}
    BSTR operator= (const tBstrPtr& pPtr) {return tPtr <OLECHAR, tBstrPtrFree>::operator= (pPtr.Ptr()?::SysAllocStringLen(pPtr.Ptr(),::SysStringLen((BSTR)pPtr.Ptr())):NULL);}
};

////////////////////////////////////////////////////////////////////////

class tSafeArrayPtrFree {protected: static inline void _Init (SAFEARRAY*& pPtr) {}; static inline void _Free (SAFEARRAY* pPtr) {if (pPtr) SafeArrayDestroy (pPtr);}};

class tSafeArrayPtr : public tPtr <SAFEARRAY, tSafeArrayPtrFree>
{
    public:

    tSafeArrayPtr () {}
    tSafeArrayPtr (SAFEARRAY* pPtr) : tPtr <SAFEARRAY, tSafeArrayPtrFree> (pPtr) {}
    tSafeArrayPtr (tSafeArrayPtr& pPtr) : tPtr <SAFEARRAY, tSafeArrayPtrFree> (pPtr) {}
    ~tSafeArrayPtr () {_Free (mPtr); mPtr = NULL;}

    SAFEARRAY* operator= (SAFEARRAY* pPtr) {return tPtr <SAFEARRAY, tSafeArrayPtrFree>::operator= (pPtr);}
    SAFEARRAY* operator= (tSafeArrayPtr& pPtr)  {return tPtr <SAFEARRAY, tSafeArrayPtrFree>::operator= (pPtr);}
};

#endif
////////////////////////////////////////////////////////////////////////
#pragma warning (pop)
////////////////////////////////////////////////////////////////////////

template <class aClass> inline void SafeFreeSafePtr (aClass& pSafePtr)
{
	aClass lSafePtr;
	try
	{
		lSafePtr.Attach (pSafePtr.Detach());
		lSafePtr = NULL;
	}
	catch AnyExceptionSilent
	lSafePtr.Detach ();
}

////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////

#ifdef	min
#undef	min
#endif
#ifdef	max
#undef	max
#endif

//----------------------------------------------------------------------

template <typename aType1, typename aType2> static inline aType1 min
(
    aType1 pInst1,
    aType2 pInst2
)
{
    return  (__min (pInst1, (aType1) pInst2));
}
template <typename aType1, typename aType2> static inline aType1 max
(
    aType1 pInst1,
    aType2 pInst2
)
{
    return  (__max (pInst1, (aType1) pInst2));
}

//----------------------------------------------------------------------

template short min (short, short);
template short max (short, short);
template unsigned short min (unsigned short, unsigned short);
template unsigned short max (unsigned short, unsigned short);
template int min (int, int);
template int max (int, int);
template unsigned int min (unsigned int, unsigned int);
template unsigned int max (unsigned int, unsigned int);
template long min (long, long);
template long max (long, long);
template unsigned long min (unsigned long, unsigned long);
template unsigned long max (unsigned long, unsigned long);
template __int64 min (__int64, __int64);
template __int64 max (__int64, __int64);
template unsigned __int64 min (unsigned __int64, unsigned __int64);
template unsigned __int64 max (unsigned __int64, unsigned __int64);
template double min (double, double);
template double max (double, double);
template float min (float, float);
template float max (float, float);

template int min (int, long);
template int max (int, long);
template long min (long, int);
template long max (long, int);
template unsigned int min (unsigned int, unsigned long);
template unsigned int max (unsigned int, unsigned long);
template unsigned long min (unsigned long, unsigned int);
template unsigned long max (unsigned long, unsigned int);
template double min (double, float);
template double max (double, float);
template float min (float, double);
template float max (float, double);

////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////

template <typename aType> static inline int compare
(
    aType pInst1,
    aType pInst2
)
{
    return (pInst1 > pInst2) ? 1 : (pInst1 < pInst2) ? -1 : 0;
}

////////////////////////////////////////////////////////////////////////

template <typename aType1, typename aType2> static inline void Swap (aType1& pElem1, aType2& pElem2)
{
	aType1 lTemp = pElem1;
	pElem1 = pElem2;
	pElem2 = lTemp;
}

template void Swap (int&, long&);
template void Swap (long&, int&);
template void Swap (unsigned int&, unsigned long&);
template void Swap (unsigned long&, unsigned int&);

////////////////////////////////////////////////////////////////////////

template <typename aType> static inline void MemAlign (aType& pAlign, UINT pAlignVal = 8)
{
	UINT	lOffset = ((ULONG_PTR) pAlign) % pAlignVal;
	if	(lOffset)
	{
		pAlign = (aType) (((ULONG_PTR) pAlign) + (8 - lOffset));
	}
}

////////////////////////////////////////////////////////////////////////
#ifdef	__ATLTYPES_H__
////////////////////////////////////////////////////////////////////////

template <typename aType> static inline CPoint operator* (const CPoint& pPoint, aType pScalar)
{
	return CPoint (pPoint.x*pScalar, pPoint.y*pScalar);
}

template <typename aType> static inline CPoint& operator*= (CPoint& pPoint, aType pScalar)
{
	pPoint.x *= pScalar;
	pPoint.y *= pScalar;
	return pPoint;
}

template <typename aType> static inline CPoint operator/ (const CPoint& pPoint, aType pScalar)
{
	return CPoint (MulDiv(pPoint.x,1,pScalar), MulDiv(pPoint.y,1,pScalar));
}

template <typename aType> static inline CPoint& operator/= (CPoint& pPoint, aType pScalar)
{
	pPoint.x = MulDiv(pPoint.x,1,pScalar);
	pPoint.y = MulDiv(pPoint.y,1,pScalar);
	return pPoint;
}
////////////////////////////////////////////////////////////////////////

template <typename aType> static inline CSize operator* (const CSize& pSize, aType pScalar)
{
	return CSize (pSize.cx*pScalar, pSize.cy*pScalar);
}

template <typename aType> static inline CSize& operator*= (CSize& pSize, aType pScalar)
{
	pSize.cx *= pScalar;
	pSize.cy *= pScalar;
	return pSize;
}

template <typename aType> static inline CSize operator/ (const CSize& pSize, aType pScalar)
{
	return CSize (MulDiv(pSize.cx,1,pScalar), MulDiv(pSize.cy,1,pScalar));
}

template <typename aType> static inline CSize& operator/= (CSize& pSize, aType pScalar)
{
	pSize.cx = MulDiv(pSize.cx,1,pScalar);
	pSize.cy = MulDiv(pSize.cy,1,pScalar);
	return pSize;
}

////////////////////////////////////////////////////////////////////////

template <typename aType> static inline CRect operator* (const CRect& pRect, aType pScalar)
{
	return CRect (pRect.left*pScalar, pRect.top*pScalar, pRect.right*pScalar, pRect.bottom*pScalar);
}

template <typename aType> static inline CRect& operator*= (CRect& pRect, aType pScalar)
{
	pRect.left *= pScalar;
	pRect.top *= pScalar;
	pRect.right *= pScalar;
	pRect.bottom *= pScalar;
	return pRect;
}

template <typename aType> static inline CRect operator/ (const CRect& pRect, aType pScalar)
{
	return CRect (MulDiv(pRect.left,1,pScalar), MulDiv(pRect.top,1,pScalar), MulDiv(pRect.right,1,pScalar), MulDiv(pRect.bottom,1,pScalar));
}

template <typename aType> static inline CRect& operator/= (CRect& pRect, aType pScalar)
{
	pRect.left = MulDiv(pRect.left,1,pScalar);
	pRect.top = MulDiv(pRect.top,1,pScalar);
	pRect.right = MulDiv(pRect.right,1,pScalar);
	pRect.bottom = MulDiv(pRect.bottom,1,pScalar);
	return pRect;
}

////////////////////////////////////////////////////////////////////////
#endif	// __AFXWIN_H__
#endif	// _HELPERTEMPLATES_H
////////////////////////////////////////////////////////////////////////
#pragma page()
////////////////////////////////////////////////////////////////////////
#ifdef	_INC_MATH
#ifndef	_HELPERTEMPLATES_H2
#define	_HELPERTEMPLATES_H2
////////////////////////////////////////////////////////////////////////
#include <float.h>
#pragma optimize("p",on)

static inline long dtol (double pDouble)
{
    return  ((long) _copysign (floor (fabs (pDouble) + 0.49), pDouble));
}
static inline long dtolDown (double pDouble)
{
    return  ((long) _copysign (floor (fabs (pDouble)), pDouble));
}
static inline long dtolUp (double pDouble)
{
    return  ((long) _copysign (ceil (fabs (pDouble)), pDouble));
}

static inline long dtolEven (double pDouble)
{
    return  ((((long) floor (pDouble)) + 1L) / 2L) * 2L;
}
static inline long dtolEvenDown (double pDouble)
{
    return  (((long) floor (pDouble)) / 2L) * 2L;
}
static inline long dtolEvenUp (double pDouble)
{
    return  ((((long) ceil (pDouble)) + 1L) / 2L) * 2L;
}

static inline long dtolOdd (double pDouble)
{
    return  (((((long) ceil (pDouble)) - 1L) / 2L) * 2L) + 1L;
}
static inline long dtolOddDown (double pDouble)
{
    return  (((((long) floor (pDouble)) - 1L) / 2L) * 2L) + 1L;
}
static inline long dtolOddUp (double pDouble)
{
    return  ((((long) ceil (pDouble)) / 2L) * 2L) + 1L;
}

inline double roundd (double pVal, UINT pPrecision = 1000000)
{
	return (double) ((double) dtol (pVal * (double) pPrecision) / (double) pPrecision);
}

inline float roundf (float pVal, UINT pPrecision = 1000)
{
	return (float) ((double) dtol (pVal * (double) pPrecision) / (double) pPrecision);
}

inline float copysignf (float pVal, float pSign)
{
	return (float) _copysign ((double) pVal, (double) pSign);
}

inline float chgsignf (float pVal)
{
	return (float) _chgsign ((double) pVal);
}

//////////////////////////////////////////////////////////////////////

template <typename aType> static inline aType DegreesToRadians (aType pDegrees, aType pOffset = (aType)0)
{
	return (pDegrees + pOffset) / (aType) 57.29577951472;
}

#if	(_MSC_VER >= 1400)
double DegreesToRadians (double, double);
float DegreesToRadians (float, double);
#else
template double DegreesToRadians<double> (double);
template float DegreesToRadians<float> (float);
#endif

template <typename aType> static inline aType RadiansToDegrees (aType pRadians, aType pOffset = (aType)0)
{
	return (pRadians * (aType) 57.29577951472) + pOffset;
}

#if	(_MSC_VER >= 1400)
double RadiansToDegrees (double, double);
float RadiansToDegrees (float, float);
#else
double DegreesToRadians<double> (double);
float DegreesToRadians<float> (float);
#endif

#ifndef	__cplusplus_cli
#define PI 3.1415926535f
#endif

////////////////////////////////////////////////////////////////////////
#endif	// _HELPERTEMPLATES_H2
#endif	// _INC_MATH
////////////////////////////////////////////////////////////////////////
