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
#include "StdAfx.h"
#include <search.h>
#include "StringArrayEx.h"

#ifdef	__AFX_H__
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

//////////////////////////////////////////////////////////////////////

#ifdef	__AFX_H__
IMPLEMENT_DYNAMIC (CStringArrayEx, CStringArray)
#endif

//////////////////////////////////////////////////////////////////////

static int __cdecl CompareStringCase (const void* pElem1, const void* pElem2)
{
	const CString *	lElem1 = (const CString *) pElem1;
	const CString *	lElem2 = (const CString *) pElem2;

	return _tcscmp (*lElem1, *lElem2);
}

static int __cdecl CompareStringNoCase (const void* pElem1, const void* pElem2)
{
	const CString *	lElem1 = (const CString *) pElem1;
	const CString *	lElem2 = (const CString *) pElem2;

	return _tcsicmp (*lElem1, *lElem2);
}

//////////////////////////////////////////////////////////////////////

static int __cdecl CollateStringCase (const void* pElem1, const void* pElem2)
{
	const CString *	lElem1 = (const CString *) pElem1;
	const CString *	lElem2 = (const CString *) pElem2;

	return _tcscoll (*lElem1, *lElem2);
}

static int __cdecl CollateStringNoCase (const void* pElem1, const void* pElem2)
{
	const CString *	lElem1 = (const CString *) pElem1;
	const CString *	lElem2 = (const CString *) pElem2;

	return _tcsicoll (*lElem1, *lElem2);
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

INT_PTR AddUniqueString (CStringArray& pStrings, const CString& pString, bool pIgnoreCase)
{
	if	(FindString (pStrings, pString) < 0)
	{
		return pStrings.Add (pString);
	}
	return -1;
}

INT_PTR FindString (const CStringArray& pStrings, const CString& pFind, bool pIgnoreCase)
{
	const CString *	lBase = pStrings.GetData();
	UINT			lCount = (UINT)pStrings.GetCount();
	const CString *	lFound = (const CString *) _lfind (&pFind, lBase, &lCount, sizeof (const CString *), pIgnoreCase ? CompareStringNoCase : CompareStringCase);
	return (lFound) ? (lFound - lBase) : -1;
}

//////////////////////////////////////////////////////////////////////

INT_PTR RemoveString (CStringArray& pStrings, const CString& pFind, bool pAll, bool pIgnoreCase)
{
	INT_PTR	lRet = FindString (pStrings, pFind, pIgnoreCase);
	if	(lRet >= 0)
	{
		pStrings.RemoveAt (lRet);
		if	(pAll)
		{
			INT_PTR	lNdx;
			while ((lNdx = FindString (pStrings, pFind, pIgnoreCase)) >= 0)
			{
				pStrings.RemoveAt (lNdx);
			}
		}
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////

void SortStrings (CStringArray& pStrings, bool pIgnoreCase)
{
	qsort (pStrings.GetData(), pStrings.GetCount(), sizeof (const CString *), pIgnoreCase ? CollateStringNoCase : CollateStringCase);
}

INT_PTR AddSortedString (CStringArray& pStrings, const CString& pString, bool pAllowDuplicates, bool pIgnoreCase)
{
	CString *	lArray = pStrings.GetData();
	INT_PTR		lCount = pStrings.GetCount();

	if	(
			(lArray)
		&&	(lCount)
		)
	{
		INT_PTR lBase = 0;
		INT_PTR lOffset = lCount/2;
		INT_PTR lNdx;
		int		lCompare;

		for (lNdx = lOffset; (lOffset >= 0) && (lNdx < lCount); lNdx = lBase + lOffset)
		{
			lCompare =	(pIgnoreCase) ? lArray [lNdx].CollateNoCase (pString) : lArray [lNdx].Collate (pString);
			if	(lCompare < 0)
			{
				if	(lOffset > 0)
				{
					lOffset = (lCount-lNdx)/2;
				}
				else
				{
					lNdx++;
					break;
				}
				lBase = lNdx;
			}
			else
			{
				if	(lCompare == 0)
				{
					if	(!pAllowDuplicates)
					{
						return -1;
					}

					do
					{
						lNdx++;
					}
					while	(
								(lNdx < lCount)
							&&	(
									(pIgnoreCase)
								?	(lArray [lNdx].CollateNoCase (pString) == 0)
								:	(lArray [lNdx].Collate (pString) == 0)
								)
							);
					break;
				}

				if	(lOffset > 0)
				{
					lOffset = min ((lOffset+1)/2, lOffset-1);
				}
				else
				{
					break;
				}
			}
		}

		pStrings.InsertAt (lNdx, pString);
		return lNdx;
	}
	else
	{
		return pStrings.Add (pString);
	}
}

INT_PTR AddSortedStringReverse (CStringArray& pStrings, const CString& pString, bool pAllowDuplicates, bool pIgnoreCase)
{
	CString	lString (pString);
	lString.MakeReverse ();
	return AddSortedString (pStrings, lString, pAllowDuplicates, pIgnoreCase);
}

//////////////////////////////////////////////////////////////////////

INT_PTR FindSortedString (const CStringArray& pStrings, const CString& pFind, bool pIgnoreCase)
{
	const CString *	lBase = pStrings.GetData();
	const CString *	lFound = (const CString *) bsearch (&pFind, lBase, pStrings.GetCount(), sizeof (const CString *), pIgnoreCase ? CollateStringNoCase : CollateStringCase);
	return (lFound) ? (lFound - lBase) : -1;
}

INT_PTR FindNextSortedString (const CStringArray& pStrings, const CString& pFind, INT_PTR pStartAfter, bool pIgnoreCase)
{
	INT_PTR	lRet = -1;

	if	(pStartAfter < (INT_PTR)pStrings.GetCount()-1)
	{
		INT_PTR			lStartAt = max (pStartAfter+1, 0);
		INT_PTR			lSize = pStrings.GetCount() - lStartAt;
		const CString *	lBase = pStrings.GetData() + lStartAt;
		const CString *	lFound;

		if	(
				(pStartAfter >= 0)
			&&	(
					(pIgnoreCase)
				?	(CollateStringNoCase (&pFind, lBase) == 0)
				:	(CollateStringCase (&pFind, lBase) == 0)
				)
			)
		{
			lFound = lBase;
		}
		else
		{
			lFound = (const CString *) bsearch (&pFind, lBase, lSize, sizeof (const CString *), pIgnoreCase ? CollateStringNoCase : CollateStringCase);
		}

		if	(lFound)
		{
			lRet = (lFound - lBase) + lStartAt;

			while	(
						(lRet > lStartAt)
					&&	(
							(pIgnoreCase)
						?	(CollateStringNoCase (&pFind, lFound-1) == 0)
						:	(CollateStringCase (&pFind, lFound-1) == 0)
						)
					)
			{
				lRet--;
				lFound--;
			}
		}
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

INT_PTR MakeStringArray (LPCTSTR pString, CStringArray& pStrings, LPCTSTR pDelimiters, bool pIncludeEmpty, bool pTrim, int pStringLength)
{
	if	(
			(pDelimiters)
		&&	(*pDelimiters == 0)
		)
	{
		pDelimiters = NULL;
	}

	if	(
			(pString)
		&&	(!pDelimiters)
		&&	(pStringLength == 0)
		)
	{
		LPCTSTR	lString;
		int		lStrLen;
		for	(lString = pString, lStrLen = (int)_tcslen (lString); (*lString); lString += lStrLen+1, lStrLen = (int)_tcslen (lString))
		{
			pStringLength += lStrLen+1;
		}
	}

	INT_PTR	lRet = 0;
	CString	lString (pString, (pStringLength >= 0) ? (int)pStringLength : pString ? (int)_tcslen (pString) : 0);
	int		lCharPos;

	while	(!lString.IsEmpty ())
	{
		if	(pDelimiters)
		{
			lCharPos = lString.FindOneOf (pDelimiters);
		}
		else
		{
			lCharPos = lString.Find ((TCHAR) 0);
			if	(
					(lCharPos == 0)
				&&	(!pIncludeEmpty)
				)
			{
				lCharPos = -1;
				lString.Empty ();
			}
		}
		if	(lCharPos >= 0)
		{
			if	(
					(lCharPos > 0)
				||	(pIncludeEmpty)
				)
			{
				CString lPart (lString.Left (lCharPos));
				if	(pTrim)
				{
					lPart.TrimLeft ();
					lPart.TrimRight ();
				}
				if	(
						(pIncludeEmpty)
					||	(!lPart.IsEmpty ())
					)
				{
					pStrings.Add (lPart);
					lRet++;
				}
			}
			lString.Delete (0, min (lCharPos+1, lString.GetLength ()));
		}
		else
		if	(!lString.IsEmpty ())
		{
			CString lPart ((LPCTSTR) lString);
			if	(pTrim)
			{
				lPart.TrimLeft ();
				lPart.TrimRight ();
			}
			if	(
					(pIncludeEmpty)
				||	(!lPart.IsEmpty ())
				)
			{
				pStrings.Add (lPart);
				lRet++;
			}
			lString.Empty ();
		}
	}

	return lRet;
}

CString JoinStringArray (const CStringArray& pStrings, LPCTSTR pDelimiter, bool pIncludeEmpty, INT_PTR pFirstNdx, INT_PTR pLastNdx)
{
	CString	lRet;
	INT_PTR	lNdx;
	bool	lFirst = true;

	pFirstNdx = max (pFirstNdx, 0);
	pLastNdx = min (pLastNdx, pStrings.GetCount()-1);

	for	(lNdx = pFirstNdx; lNdx <= pLastNdx; lNdx++)
	{
		const CString&	lString = const_cast <CStringArray&> (pStrings).operator[] (lNdx);

		if	(
				(pIncludeEmpty)
			||	(!lString.IsEmpty ())
			)
		{
			if	(pDelimiter)
			{
				if	(!lFirst)
				{
					lRet += pDelimiter;
				}

				lRet += lString;
			}
			else
			{
				if	(lFirst)
				{
					memcpy (lRet.GetBufferSetLength (lString.GetLength ()+1), (LPCTSTR) lString, (lString.GetLength ()+1) * sizeof (TCHAR));
				}
				else
				{
					int lLength = lRet.GetLength ();
					memcpy (lRet.GetBufferSetLength (lRet.GetLength ()+lString.GetLength ()+1) + lLength, (LPCTSTR) lString, (lString.GetLength ()+1) * sizeof (TCHAR));
					lLength = lRet.GetLength ();
				}
			}

			lFirst = false;
		}
	}

	if	(
			(!pDelimiter)
		&&	(!lRet.IsEmpty ())
		)
	{
		if	(pIncludeEmpty)
		{
			lRet.GetBufferSetLength (lRet.GetLength ()+1);
		}
		else
		{
			lRet.GetBuffer (lRet.GetLength ()+1);
		}
	}

	return CString (lRet);
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

INT_PTR ReplaceAll (CStringArray& pStrings, TCHAR pReplace, TCHAR pReplaceWith)
{
	INT_PTR	lRet = 0;
	INT_PTR	lNdx = 0;

	for	(lNdx = 0; lNdx < (INT_PTR)pStrings.GetCount(); lNdx++)
	{
		lRet += pStrings [lNdx].Replace (pReplace, pReplaceWith);
	}
	return lRet;
}

INT_PTR ReplaceAll (CStringArray& pStrings, LPCTSTR pReplace, LPCTSTR pReplaceWith)
{
	INT_PTR	lRet = 0;
	INT_PTR	lNdx = 0;

	for	(lNdx = 0; lNdx < (INT_PTR)pStrings.GetCount(); lNdx++)
	{
		lRet += pStrings [lNdx].Replace (pReplace, pReplaceWith);
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////

bool CompareStringArrays (const CStringArray& pStrings1, const CStringArray& pStrings2, bool pIgnoreCase)
{
	if	(pStrings1.GetCount() != pStrings2.GetCount())
	{
		return false;
	}

	for	(INT_PTR lNdx = 0; lNdx < (INT_PTR)pStrings1.GetCount(); lNdx++)
	{
		if	(
				(pIgnoreCase)
			?	(pStrings1 [lNdx].CompareNoCase (pStrings2 [lNdx]) != 0)
			:	(pStrings1 [lNdx].Compare (pStrings2 [lNdx]) != 0)
			)
		{
			return false;
		}
	}

	return true;
}

