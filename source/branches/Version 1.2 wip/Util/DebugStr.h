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
#ifndef	_DEBUGSTR_H_
#define _DEBUGSTR_H_
//////////////////////////////////////////////////////////////////////

#ifndef	_DEBUG_T
#ifdef	_DEBUG
#define	_DEBUG_T(x)	_T(x)
#else
#define	_DEBUG_T(x)	_T("")
#endif
#endif

static inline CString DebugStr (LPCTSTR pString)
{
	CString	lString (pString);
	lString.Replace (_T("\r"), _T("\\r"));
	lString.Replace (_T("\n"), _T("\\n"));
	lString.Replace (_T("\t"), _T("\\t"));
	lString.Replace (_T("\f"), _T("\\f"));
	return lString;
}

////////////////////////////////////////////////////////////////////////

#ifdef _CPPRTTI
#include <typeinfo.h>
template <typename TYPE> static CString ObjTypeName (TYPE const * pObject)
{
	CString	lTypeName;
	if	(pObject)
	{
		try
		{
			lTypeName = CString (typeid(*pObject).name());
			if	(lTypeName.Left (5) == _T("class"))
			{
				lTypeName = lTypeName.Mid (5);
			}
			lTypeName.TrimLeft ();
			lTypeName.TrimRight ();
		}
		catch (...)
		{
			try
			{
				lTypeName = CString (typeid(pObject).name());
			}
			catch (...)
			{
				lTypeName = _T("<invalid>");
			}
		}
	}
	else
	{
		lTypeName = _T("<null>");
	}
	return lTypeName;
}
#endif

#ifdef	__AFX_H__
static CString ObjClassName (CObject const * pObject)
{
	CString	lClassName;
	if	(pObject)
	{
		try
		{
			lClassName = pObject->GetRuntimeClass()->m_lpszClassName;
		}
		catch (...)
		{
			lClassName = _T("<invalid>");
		}
	}
	else
	{
		lClassName = _T("<null>");
	}
	return lClassName;
}
#endif

static CString WndClassName (HWND pWnd)
{
	CString	lClassName;
	if	(pWnd)
	{
		if	(IsWindow (pWnd))
		{
			GetClassName (pWnd, lClassName.GetBuffer(MAX_PATH), MAX_PATH);
			lClassName.ReleaseBuffer ();
		}
		else
		{
			lClassName = _T("<not a window>");
		}
	}
	else
	{
		lClassName = _T("<null>");
	}
	return lClassName;
}

#ifdef	__AFXWIN_H__
static CString WndClassName (CWnd const * pWnd)
{
	return ObjClassName (pWnd);
}
#endif	//	__AFXWIN_H__

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

__if_not_exists(VarTypeStr)
{
	inline CString VarTypeStr (VARTYPE pVarType)
	{
		CString	lRet;

		switch (pVarType & VT_TYPEMASK)
		{
			case VT_EMPTY:				lRet = _T("VT_EMPTY"); break;
			case VT_NULL:				lRet = _T("VT_NULL"); break;
			case VT_I2:					lRet = _T("VT_I2"); break;
			case VT_I4:					lRet = _T("VT_I4"); break;
			case VT_R4:					lRet = _T("VT_R4"); break;
			case VT_R8:					lRet = _T("VT_R8"); break;
			case VT_CY:					lRet = _T("VT_CY"); break;
			case VT_DATE:				lRet = _T("VT_DATE"); break;
			case VT_BSTR:				lRet = _T("VT_BSTR"); break;
			case VT_DISPATCH:			lRet = _T("VT_DISPATCH"); break;
			case VT_ERROR:				lRet = _T("VT_ERROR"); break;
			case VT_BOOL:				lRet = _T("VT_BOOL"); break;
			case VT_VARIANT:			lRet = _T("VT_VARIANT"); break;
			case VT_UNKNOWN:			lRet = _T("VT_UNKNOWN"); break;
			case VT_DECIMAL:			lRet = _T("VT_DECIMAL"); break;
			case VT_I1:					lRet = _T("VT_I1"); break;
			case VT_UI1:				lRet = _T("VT_UI1"); break;
			case VT_UI2:				lRet = _T("VVT_UI2T_BSTR"); break;
			case VT_UI4:				lRet = _T("VT_UI4"); break;
			case VT_I8:					lRet = _T("VT_I8"); break;
			case VT_UI8:				lRet = _T("VT_UI8"); break;
			case VT_INT:				lRet = _T("VT_INT"); break;
			case VT_UINT:				lRet = _T("VT_UINT"); break;
			case VT_VOID:				lRet = _T("VT_VOID"); break;
			case VT_HRESULT:			lRet = _T("VT_HRESULT"); break;
			case VT_PTR:				lRet = _T("VT_PTR"); break;
			case VT_CARRAY:				lRet = _T("VT_CARRAY"); break;
			case VT_USERDEFINED:		lRet = _T("VVT_USERDEFINEDT_BSTR"); break;
			case VT_LPSTR:				lRet = _T("VT_LPSTR"); break;
			case VT_LPWSTR:				lRet = _T("VT_LPWSTR"); break;
			case VT_RECORD:				lRet = _T("VT_RECORD"); break;
			case VT_INT_PTR:			lRet = _T("VT_INT_PTR"); break;
			case VT_UINT_PTR:			lRet = _T("VT_UINT_PTR"); break;
			case VT_FILETIME:			lRet = _T("VT_FILETIME"); break;
			case VT_BLOB:				lRet = _T("VT_BLOB"); break;
			case VT_STREAM:				lRet = _T("VT_STREAM"); break;
			case VT_STORAGE:			lRet = _T("VT_STORAGE"); break;
			case VT_STREAMED_OBJECT:	lRet = _T("VT_STREAMED_OBJECT"); break;
			case VT_STORED_OBJECT:		lRet = _T("VT_STORED_OBJECT"); break;
			case VT_BLOB_OBJECT:		lRet = _T("VT_BLOB_OBJECT"); break;
			case VT_CF:					lRet = _T("VT_CF"); break;
			case VT_CLSID:				lRet = _T("VT_CLSID"); break;
	#ifdef	DBTYPE
			case DBTYPE_BYTES:			lRet = _T("DBTYPE_BYTES"); break;
			case DBTYPE_STR:			lRet = _T("DBTYPE_STR"); break;
			case DBTYPE_WSTR:			lRet = _T("DBTYPE_WSTR"); break;
			case DBTYPE_NUMERIC:		lRet = _T("DBTYPE_NUMERIC"); break;
			case DBTYPE_UDT:			lRet = _T("DBTYPE_UDT"); break;
			case DBTYPE_DBDATE:			lRet = _T("DBTYPE_DBDATE"); break;
			case DBTYPE_DBTIME:			lRet = _T("DBTYPE_DBTIME"); break;
			case DBTYPE_DBTIMESTAMP:	lRet = _T("DBTYPE_DBTIMESTAMP"); break;
	#endif
			default:					lRet.Format (_T("%4.4X"), pVarType & VT_TYPEMASK); break;
		}

		if	(pVarType & VT_VECTOR)
		{
			lRet += _T(" VT_VECTOR");
		}
		if	(pVarType & VT_ARRAY)
		{
			lRet += _T(" VT_ARRAY");
		}
		if	(pVarType & VT_BYREF)
		{
			lRet += _T(" VT_BYREF");
		}
		if	(pVarType & VT_RESERVED)
		{
			lRet += _T(" VT_RESERVED");
		}

		return lRet;
	}
}

__if_not_exists(VariantString)
{
	inline CString VariantString (const VARIANT& pVariant)
	{
		if	(V_VT (&pVariant) == VT_BSTR)
		{
			return CString (V_BSTR (&pVariant));
		}
		else
		if	(V_VT (&pVariant) == VT_LPSTR)
		{
			return CString ((LPCSTR) pVariant.byref);
		}
		else
		if	(V_VT (&pVariant) == VT_LPWSTR)
		{
			return CString ((LPCWSTR) pVariant.byref);
		}
		else
		{
			_variant_t	lVariant (pVariant);

			try
			{
				lVariant.ChangeType (VT_BSTR);
			}
			catch AnyExceptionSilent

			if	(V_VT (&lVariant) == VT_BSTR)
			{
				return CString (V_BSTR (&lVariant));
			}
		}
		return CString();
	}
}

inline CString DebugVariant (VARIANT& pVariant)
{
	return _T("[") + VariantString (pVariant) + _T("] (") + VarTypeStr (V_VT(&pVariant)) + _T(")");
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

#ifndef	_DEBUG_FORMAT_ALIGNED
#define	_DEBUG_FORMAT_ALIGNED	0
#endif
#ifndef	_DEBUG_FORMAT_SIGNED
#define	_DEBUG_FORMAT_SIGNED	false
#endif

//////////////////////////////////////////////////////////////////////

static inline CString FormatPoint (const POINT& pPoint, USHORT pAligned = _DEBUG_FORMAT_ALIGNED, bool pSigned = _DEBUG_FORMAT_SIGNED)
{
	CString	lRet;
	if	(pAligned)
	{
		if	(pSigned)
		{
			pAligned++;
			lRet.Format (_T("%+*.1d %+*.1d"), pAligned, pPoint.x, pAligned, pPoint.y);
		}
		else
		{
			lRet.Format (_T("%*.1d %*.1d"), pAligned, pPoint.x, pAligned, pPoint.y);
		}
	}
	else
	{
		if	(pSigned)
		{
			lRet.Format (_T("%+d %+d"), pPoint.x, pPoint.y);
		}
		else
		{
			lRet.Format (_T("%d %d"), pPoint.x, pPoint.y);
		}
	}
	return CString ((LPCTSTR)lRet);
}

inline CString FormatSize (const SIZE& pSize, USHORT pAligned = _DEBUG_FORMAT_ALIGNED, bool pSigned = _DEBUG_FORMAT_SIGNED)
{
	CString	lRet;
	if	(pAligned)
	{
		if	(pSigned)
		{
			pAligned++;
			lRet.Format (_T("%+*.1d %+*.1d"), pAligned, pSize.cx, pAligned, pSize.cy);
		}
		else
		{
			lRet.Format (_T("%*.1d %*.1d"), pAligned, pSize.cx, pAligned, pSize.cy);
		}
	}
	else
	{
		if	(pSigned)
		{
			lRet.Format (_T("%+d %+d"), pSize.cx, pSize.cy);
		}
		else
		{
			lRet.Format (_T("%d %d"), pSize.cx, pSize.cy);
		}
	}
	return CString ((LPCTSTR)lRet);
}

static inline CString FormatRect (const RECT& pRect, USHORT pAligned = _DEBUG_FORMAT_ALIGNED, bool pSigned = _DEBUG_FORMAT_SIGNED)
{
	CString	lRet;
	if	(pAligned)
	{
		if	(pSigned)
		{
			pAligned++;
			lRet.Format (_T("%+*.1d %+*.1d %+*.1d %+*.1d (%+*.1d %+*.1d)"), pAligned, pRect.left, pAligned, pRect.top, pAligned, pRect.right, pAligned, pRect.bottom, pAligned, pRect.right-pRect.left, pAligned, pRect.bottom-pRect.top);
		}
		else
		{
			lRet.Format (_T("%*.1d %*.1d %*.1d %*.1d (%*.1d %*.1d)"), pAligned, pRect.left, pAligned, pRect.top, pAligned, pRect.right, pAligned, pRect.bottom, pAligned, pRect.right-pRect.left, pAligned, pRect.bottom-pRect.top);
		}
	}
	else
	{
		if	(pSigned)
		{
			lRet.Format (_T("%+d %+d %+d %+d (%+d %+d)"), pRect.left, pRect.top, pRect.right, pRect.bottom, pRect.right-pRect.left, pRect.bottom-pRect.top);
		}
		else
		{
			lRet.Format (_T("%d %d %d %d (%d %d)"), pRect.left, pRect.top, pRect.right, pRect.bottom, pRect.right-pRect.left, pRect.bottom-pRect.top);
		}
	}
	return CString ((LPCTSTR)lRet);
}

static inline CString FormatMargin (const RECT& pRect, USHORT pAligned = _DEBUG_FORMAT_ALIGNED, bool pSigned = _DEBUG_FORMAT_SIGNED)
{
	CString	lRet;
	if	(pAligned)
	{
		if	(pSigned)
		{
			pAligned++;
			lRet.Format (_T("%+*.1d %+*.1d %+*.1d %+*.1d"), pAligned, pRect.left, pAligned, pRect.top, pAligned, pRect.right, pAligned, pRect.bottom);
		}
		else
		{
			lRet.Format (_T("%*.1d %*.1d %*.1d %*.1d"), pAligned, pRect.left, pAligned, pRect.top, pAligned, pRect.right, pAligned, pRect.bottom);
		}
	}
	else
	{
		if	(pSigned)
		{
			lRet.Format (_T("%+d %+d %+d %+d"), pRect.left, pRect.top, pRect.right, pRect.bottom);
		}
		else
		{
			lRet.Format (_T("%d %d %d %d"), pRect.left, pRect.top, pRect.right, pRect.bottom);
		}
	}
	return CString ((LPCTSTR)lRet);
}

////////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

static CString DrawTextFlags (UINT pFlags, bool pIncludeDefaults = true)
{
	CString	lRet;

	if	(
			(pIncludeDefaults)
		&&	((pFlags & (DT_CENTER|DT_RIGHT)) == 0)
		)
	{
		lRet += _T("LEFT ");
	}
	if	(pFlags & DT_CENTER)
	{
		lRet += _T("CENTER ");
	}
	if	(pFlags & DT_RIGHT)
	{
		lRet += _T("RIGHT ");
	}

	if	(
			(pIncludeDefaults)
		&&	((pFlags & (DT_VCENTER|DT_BOTTOM)) == 0)
		)
	{
		lRet += _T("TOP ");
	}
	if	(pFlags & DT_VCENTER)
	{
		lRet += _T("VCENTER ");
	}
	if	(pFlags & DT_BOTTOM)
	{
		lRet += _T("BOTTOM ");
	}

	if	(pFlags & DT_WORDBREAK)
	{
		lRet += _T("WORDBREAK ");
	}
	if	(pFlags & DT_SINGLELINE)
	{
		lRet += _T("SINGLELINE ");
	}
	if	(pFlags & DT_EXPANDTABS)
	{
		lRet += _T("EXPANDTABS ");
	}
	if	(pFlags & DT_TABSTOP)
	{
		lRet += _T("TABSTOP ");
	}
	if	(pFlags & DT_NOCLIP)
	{
		lRet += _T("NOCLIP ");
	}
	if	(pFlags & DT_EXTERNALLEADING)
	{
		lRet += _T("EXTERNALLEADING ");
	}
	if	(pFlags & DT_CALCRECT)
	{
		lRet += _T("CALCRECT ");
	}
	if	(pFlags & DT_NOPREFIX)
	{
		lRet += _T("NOPREFIX ");
	}
	if	(pFlags & DT_INTERNAL)
	{
		lRet += _T("INTERNAL ");
	}
#ifdef	DT_EDITCONTROL
	if	(pFlags & DT_EDITCONTROL)
	{
		lRet += _T("EDITCONTROL ");
	}
	if	(pFlags & DT_PATH_ELLIPSIS)
	{
		lRet += _T("PATH_ELLIPSIS ");
	}
	if	(pFlags & DT_END_ELLIPSIS)
	{
		lRet += _T("END_ELLIPSIS ");
	}
	if	(pFlags & DT_MODIFYSTRING)
	{
		lRet += _T("MODIFYSTRING ");
	}
	if	(pFlags & DT_RTLREADING)
	{
		lRet += _T("RTLREADING ");
	}
	if	(pFlags & DT_WORD_ELLIPSIS)
	{
		lRet += _T("WORD_ELLIPSIS ");
	}
#endif
#ifdef	DT_NOFULLWIDTHCHARBREAK
	if	(pFlags & DT_NOFULLWIDTHCHARBREAK)
	{
		lRet += _T("NOFULLWIDTHCHARBREAK ");
	}
#endif
#ifdef	DT_HIDEPREFIX
	if	(pFlags & DT_HIDEPREFIX)
	{
		lRet += _T("HIDEPREFIX ");
	}
	if	(pFlags & DT_PREFIXONLY)
	{
		lRet += _T("PREFIXONLY ");
	}
#endif

	lRet.TrimRight ();
	return lRet;
}

////////////////////////////////////////////////////////////////////////

static CString ItemDrawType (UINT pCtlType)
{
	CString	lRet;

	switch (pCtlType)
	{
		case ODT_MENU:		lRet = _T("MENU"); break;
		case ODT_LISTBOX:	lRet = _T("LISTBOX"); break;
		case ODT_COMBOBOX:	lRet = _T("COMBOBOX"); break;
		case ODT_BUTTON:	lRet = _T("BUTTON"); break;
		case ODT_STATIC:	lRet = _T("STATIC"); break;
		default:			lRet.Format (_T("-%u-"), pCtlType);
	}
	return lRet;
}

static CString ItemDrawType (LPDRAWITEMSTRUCT pDIS)
{
	if	(pDIS)
	{
		return ItemDrawType (pDIS->CtlType);
	}
	return CString();
}

static CString ItemDrawAction (UINT pAction)
{
	CString	lRet;

	if	(pAction & ODA_DRAWENTIRE)
	{
		lRet += _T("DRAWENTIRE ");
	}
	if	(pAction & ODA_SELECT)
	{
		lRet += _T("SELECT ");
	}
	if	(pAction & ODA_FOCUS)
	{
		lRet += _T("FOCUS ");
	}

	lRet.TrimRight ();
	return lRet;
}

static CString ItemDrawAction (LPDRAWITEMSTRUCT pDIS)
{
	if	(pDIS)
	{
		return ItemDrawAction (pDIS->itemAction);
	}
	return CString();
}

static CString ItemDrawState (UINT pState)
{
	CString	lRet;

	if	(pState & ODS_SELECTED)
	{
		lRet += _T("SELECTED ");
	}
	if	(pState & ODS_GRAYED)
	{
		lRet += _T("GRAYED ");
	}
	if	(pState & ODS_DISABLED)
	{
		lRet += _T("DISABLED ");
	}
	if	(pState & ODS_CHECKED)
	{
		lRet += _T("CHECKED ");
	}
	if	(pState & ODS_FOCUS)
	{
		lRet += _T("FOCUS ");
	}
	if	(pState & ODS_DEFAULT)
	{
		lRet += _T("DEFAULT ");
	}
#ifdef	ODS_HOTLIGHT
	if	(pState & ODS_HOTLIGHT)
	{
		lRet += _T("HOTLIGHT ");
	}
#endif
#ifdef	ODS_INACTIVE
	if	(pState & ODS_INACTIVE)
	{
		lRet += _T("INACTIVE ");
	}
#endif
#ifdef	ODS_NOACCEL
	if	(pState & ODS_NOACCEL)
	{
		lRet += _T("NOACCEL ");
	}
#endif
#ifdef	ODS_NOFOCUSRECT
	if	(pState & ODS_NOFOCUSRECT)
	{
		lRet += _T("NOFOCUSRECT ");
	}
#endif
	if	(pState & ODS_COMBOBOXEDIT)
	{
		lRet += _T("COMBOBOXEDIT ");
	}

	lRet.TrimRight ();
	return lRet;
}

static CString ItemDrawState (LPDRAWITEMSTRUCT pDIS)
{
	if	(pDIS)
	{
		return ItemDrawState (pDIS->itemState);
	}
	return CString();
}

////////////////////////////////////////////////////////////////////////

static CString CustomDrawStage (DWORD pStage)
{
	CString	lRet;

	switch (pStage & ~CDDS_ITEM & ~CDDS_SUBITEM)
	{
		case CDDS_PREPAINT:		lRet = (pStage & CDDS_SUBITEM) ?  _T("SUBITEMPREPAINT") : (pStage & CDDS_ITEM) ? _T("ITEMPREPAINT") : _T("PREPAINT"); break;
		case CDDS_POSTPAINT:	lRet = (pStage & CDDS_SUBITEM) ?  _T("SUBITEMPOSTPAINT") : (pStage & CDDS_ITEM) ? _T("ITEMPOSTPAINT") : _T("POSTPAINT"); break;
		case CDDS_PREERASE:		lRet = (pStage & CDDS_SUBITEM) ?  _T("SUBITEMPREERASE") : (pStage & CDDS_ITEM) ? _T("ITEMPREERASE") : _T("PREERASE"); break;
		case CDDS_POSTERASE:	lRet = (pStage & CDDS_SUBITEM) ?  _T("SUBITEMPOSTERASE") : (pStage & CDDS_ITEM) ? _T("ITEMPOSTERASE") : _T("POSTERASE"); break;
		default:					lRet.Format (_T("-%8.8X-"), pStage);
	}
	return lRet;
}

static CString CustomDrawStage (LPNMCUSTOMDRAW pCustomDraw)
{
	if	(pCustomDraw)
	{
		return CustomDrawStage (pCustomDraw->dwDrawStage);
	}
	return CString();
}

static CString CustomDrawState (UINT pState)
{
	CString	lRet;

	if	(pState & CDIS_SELECTED)
	{
		lRet += _T("SELECTED ");
	}
	if	(pState & CDIS_GRAYED)
	{
		lRet += _T("GRAYED ");
	}
	if	(pState & CDIS_DISABLED)
	{
		lRet += _T("DISABLED ");
	}
	if	(pState & CDIS_CHECKED)
	{
		lRet += _T("CHECKED ");
	}
	if	(pState & CDIS_FOCUS)
	{
		lRet += _T("FOCUS ");
	}
	if	(pState & CDIS_DEFAULT)
	{
		lRet += _T("DEFAULT ");
	}
	if	(pState & CDIS_HOT)
	{
		lRet += _T("HOT ");
	}
	if	(pState & CDIS_MARKED)
	{
		lRet += _T("MARKED ");
	}
	if	(pState & CDIS_INDETERMINATE)
	{
		lRet += _T("INDETERMINATE ");
	}
#ifdef	CDIS_SHOWKEYBOARDCUES
	if	(pState & CDIS_SHOWKEYBOARDCUES)
	{
		lRet += _T("SHOWKEYBOARDCUES ");
	}
#endif
#ifdef	CDIS_NEARHOT
	if	(pState & CDIS_NEARHOT)
	{
		lRet += _T("NEARHOT ");
	}
#endif
#ifdef	CDIS_OTHERSIDEHOT
	if	(pState & CDIS_OTHERSIDEHOT)
	{
		lRet += _T("OTHERSIDEHOT ");
	}
#endif
#ifdef	CDIS_DROPHILITED
	if	(pState & CDIS_DROPHILITED)
	{
		lRet += _T("DROPHILITED ");
	}
#endif

	lRet.TrimRight ();
	return lRet;
}

static CString CustomDrawState (LPNMCUSTOMDRAW pCustomDraw)
{
	if	(pCustomDraw)
	{
		return CustomDrawState (pCustomDraw->uItemState);
	}
	return CString();
}

static CString CustomDrawResult (LRESULT pResult)
{
	CString	lRet;

	if	(pResult & CDRF_NEWFONT)
	{
		lRet += _T("NEWFONT ");
	}
	if	(pResult & CDRF_SKIPDEFAULT)
	{
		lRet += _T("SKIPDEFAULT ");
	}
#ifdef	CDRF_DOERASE
	if	(pResult & CDRF_DOERASE)
	{
		lRet += _T("DOERASE ");
	}
#endif
#ifdef	CDRF_SKIPPOSTPAINT
	if	(pResult & CDRF_SKIPPOSTPAINT)
	{
		lRet += _T("SKIPPOSTPAINT ");
	}
#endif
	if	(pResult & CDRF_NOTIFYPOSTPAINT)
	{
		lRet += _T("NOTIFYPOSTPAINT ");
	}
	if	(pResult & CDRF_NOTIFYITEMDRAW)
	{
		lRet += _T("NOTIFYITEMDRAW ");
	}
	if	(pResult & CDRF_NOTIFYSUBITEMDRAW)
	{
		lRet += _T("NOTIFYSUBITEMDRAW ");
	}
	if	(pResult & CDRF_NOTIFYPOSTERASE)
	{
		lRet += _T("NOTIFYPOSTERASE ");
	}
	if	(lRet.IsEmpty ())
	{
		lRet = _T("DODEFAULT");
	}

	lRet.TrimRight ();
	return lRet;
}

////////////////////////////////////////////////////////////////////////

static inline CString WinStyleStr (DWORD pStyle)
{
	CString	lStyleStr;

	if	(pStyle & WS_VISIBLE)
	{
		lStyleStr += _T("VISIBLE ");
	}
	if	(pStyle & WS_POPUP)
	{
		lStyleStr += _T("POPUP ");
	}
	if	(pStyle & WS_CHILD)
	{
		lStyleStr += _T("CHILD ");
	}
	if	(pStyle & WS_DISABLED)
	{
		lStyleStr += _T("DISABLED ");
	}
	if	(pStyle & WS_MINIMIZE)
	{
		lStyleStr += _T("MINIMIZE ");
	}
	if	(pStyle & WS_MAXIMIZE)
	{
		lStyleStr += _T("MAXIMIZE ");
	}
	if	(pStyle & WS_CLIPSIBLINGS)
	{
		lStyleStr += _T("CLIPSIBLINGS ");
	}
	if	(pStyle & WS_CLIPCHILDREN)
	{
		lStyleStr += _T("CLIPCHILDREN ");
	}
	if	(pStyle & WS_THICKFRAME)
	{
		lStyleStr += _T("THICKFRAME ");
	}
	else
	if	(
			((pStyle & WS_CAPTION) != WS_CAPTION)
		&&	(pStyle & WS_BORDER)
		)
	{
		lStyleStr += _T("BORDER ");
	}

	if	((pStyle & WS_CAPTION) == WS_CAPTION)
	{
		lStyleStr += _T("CAPTION ");
	}
	else
	if	(pStyle & WS_DLGFRAME)
	{
		lStyleStr += _T("DLGFRAME ");
	}

	if	(pStyle & WS_SYSMENU)
	{
		lStyleStr += _T("SYSMENU ");
	}
	if	(pStyle & WS_VSCROLL)
	{
		lStyleStr += _T("VSCROLL ");
	}
	if	(pStyle & WS_HSCROLL)
	{
		lStyleStr += _T("HSCROLL ");
	}
	if	(pStyle & WS_CHILD)
	{
		if	(pStyle & WS_TABSTOP)
		{
			lStyleStr += _T("TABSTOP ");
		}
		if	(pStyle & WS_GROUP)
		{
			lStyleStr += _T("GROUP ");
		}
	}
	else
	{
		if	(pStyle & WS_MINIMIZEBOX)
		{
			lStyleStr += _T("MINIMIZEBOX ");
		}
		if	(pStyle & WS_MAXIMIZEBOX)
		{
			lStyleStr += _T("MAXIMIZEBOX ");
		}
	}

	lStyleStr.TrimRight ();
	return lStyleStr;
}

static inline CString WinExStyleStr (DWORD pExStyle)
{
	CString	lExStyleStr;

	if	(pExStyle & WS_EX_DLGMODALFRAME)
	{
		lExStyleStr += _T("DLGMODALFRAME ");
	}
	if	(pExStyle & WS_EX_NOPARENTNOTIFY)
	{
		lExStyleStr += _T("NOPARENTNOTIFY ");
	}
	if	(pExStyle & WS_EX_TOPMOST)
	{
		lExStyleStr += _T("TOPMOST ");
	}
	if	(pExStyle & WS_EX_ACCEPTFILES)
	{
		lExStyleStr += _T("ACCEPTFILES ");
	}
	if	(pExStyle & WS_EX_TRANSPARENT)
	{
		lExStyleStr += _T("TRANSPARENT ");
	}
	if	(pExStyle & WS_EX_MDICHILD)
	{
		lExStyleStr += _T("MDICHILD ");
	}
	if	(pExStyle & WS_EX_APPWINDOW)
	{
		lExStyleStr += _T("APPWINDOW ");
	}
	if	(pExStyle & WS_EX_TOOLWINDOW)
	{
		lExStyleStr += _T("TOOLWINDOW ");
	}
#ifdef	WS_EX_LAYERED
	if	(pExStyle & WS_EX_LAYERED)
	{
		lExStyleStr += _T("LAYERED ");
	}
#endif
#ifdef	WS_EX_COMPOSITED
	if	(pExStyle & WS_EX_COMPOSITED)
	{
		lExStyleStr += _T("COMPOSITED ");
	}
#endif
	if	(pExStyle & WS_EX_NOACTIVATE)
	{
		lExStyleStr += _T("NOACTIVATE ");
	}
	if	(pExStyle & WS_EX_WINDOWEDGE)
	{
		lExStyleStr += _T("WINDOWEDGE ");
	}
	if	(pExStyle & WS_EX_CLIENTEDGE)
	{
		lExStyleStr += _T("CLIENTEDGE ");
	}
	if	(pExStyle & WS_EX_STATICEDGE)
	{
		lExStyleStr += _T("STATICEDGE ");
	}
	if	(pExStyle & WS_EX_CONTEXTHELP)
	{
		lExStyleStr += _T("CONTEXTHELP ");
	}
	if	(pExStyle & WS_EX_CONTROLPARENT)
	{
		lExStyleStr += _T("CONTROLPARENT ");
	}
	if	(pExStyle & WS_EX_RIGHT)
	{
		lExStyleStr += _T("RIGHT ");
	}
	if	(pExStyle & WS_EX_RTLREADING)
	{
		lExStyleStr += _T("RTLREADING ");
	}
	if	(pExStyle & WS_EX_LEFTSCROLLBAR)
	{
		lExStyleStr += _T("LEFTSCROLLBAR ");
	}
	if	(pExStyle & WS_EX_LAYOUTRTL)
	{
		lExStyleStr += _T("LAYOUTRTL ");
	}
	if	(pExStyle & WS_EX_NOINHERITLAYOUT)
	{
		lExStyleStr += _T("NOINHERITLAYOUT ");
	}

	lExStyleStr.TrimRight ();
	return lExStyleStr;
}

//////////////////////////////////////////////////////////////////////

static inline CString WinPosFlagsStr (UINT pFlags)
{
	CString	lFlagsStr;

	if	(pFlags & SWP_NOMOVE)
	{
		lFlagsStr += _T("NOMOVE ");
	}
	if	(pFlags & SWP_NOSIZE)
	{
		lFlagsStr += _T("NOSIZE ");
	}
	if	(pFlags & SWP_NOZORDER)
	{
		lFlagsStr += _T("NOZORDER ");
	}
	if	(pFlags & SWP_SHOWWINDOW)
	{
		lFlagsStr += _T("SHOWWINDOW ");
	}
	if	(pFlags & SWP_HIDEWINDOW)
	{
		lFlagsStr += _T("HIDEWINDOW ");
	}
	if	(pFlags & SWP_FRAMECHANGED)
	{
		lFlagsStr += _T("FRAMECHANGED ");
	}
	if	(pFlags & SWP_NOACTIVATE)
	{
		lFlagsStr += _T("NOACTIVATE ");
	}
	if	(pFlags & SWP_NOOWNERZORDER)
	{
		lFlagsStr += _T("NOOWNERZORDER ");
	}
	if	(pFlags & SWP_NOSENDCHANGING)
	{
		lFlagsStr += _T("NOSENDCHANGING ");
	}
	if	(pFlags & SWP_DEFERERASE)
	{
		lFlagsStr += _T("DEFERERASE ");
	}
	if	(pFlags & SWP_ASYNCWINDOWPOS)
	{
		lFlagsStr += _T("ASYNCWINDOWPOS ");
	}
	if	(pFlags & SWP_NOREDRAW)
	{
		lFlagsStr += _T("NOREDRAW ");
	}
	if	(pFlags & SWP_NOCOPYBITS)
	{
		lFlagsStr += _T("NOCOPYBITS ");
	}

	lFlagsStr.TrimRight ();
	return lFlagsStr;
}

static inline CString WinPosAfterStr (HWND pAfter)
{
	CString	lAfterStr;

	if	(pAfter == HWND_TOP)
	{
		lAfterStr = _T("HWND_TOP");
	}
	else
	if	(pAfter == HWND_BOTTOM)
	{
		lAfterStr = _T("HWND_BOTTOM");
	}
	else
	if	(pAfter == HWND_TOPMOST)
	{
		lAfterStr = _T("HWND_TOPMOST");
	}
	else
	if	(pAfter == HWND_NOTOPMOST)
	{
		lAfterStr = _T("HWND_NOTOPMOST");
	}
	else
	{
		lAfterStr.Format (_T("%8.8X"), pAfter);
	}

	return lAfterStr;
}

static inline CString WindowPosStr (WINDOWPOS & pWindowPos)
{
	CString	lStr = WinPosFlagsStr (pWindowPos.flags);

	if	((pWindowPos.flags & (SWP_NOMOVE|SWP_NOSIZE)) == 0)
	{
		lStr.Format (_T("%s Move/Size [%d %d %d %d (%d %d)]"), CString((LPCTSTR)lStr), pWindowPos.x, pWindowPos.y, pWindowPos.x+pWindowPos.cx, pWindowPos.y+pWindowPos.cy, pWindowPos.cx, pWindowPos.cy);
	}
	else
	if	((pWindowPos.flags & SWP_NOMOVE) == 0)
	{
		lStr.Format (_T("%s Move [%d %d]"), CString((LPCTSTR)lStr), pWindowPos.x, pWindowPos.y);
	}
	else
	if	((pWindowPos.flags & SWP_NOSIZE) == 0)
	{
		lStr.Format (_T("%s Size [%d %d]"), CString((LPCTSTR)lStr), pWindowPos.cx, pWindowPos.cy);
	}

	if	((pWindowPos.flags & SWP_NOACTIVATE) == 0)
	{
		lStr.Format (_T("%s After [%s]"), CString((LPCTSTR)lStr), WinPosAfterStr (pWindowPos.hwndInsertAfter));
	}

	return lStr;
}

//////////////////////////////////////////////////////////////////////
#endif // _DEBUGSTR_H_
