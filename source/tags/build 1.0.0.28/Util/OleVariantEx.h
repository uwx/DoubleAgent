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
#ifndef	_OLEVARIANTEX_H
#define	_OLEVARIANTEX_H
////////////////////////////////////////////////////////////////////////

inline COleVariant DispatchVariant (IDispatch * pDispatch)
{
	VARIANT lVariant;
	VariantInit (&lVariant);
	V_VT (&lVariant) = VT_DISPATCH;
	V_DISPATCH (&lVariant) = pDispatch;
	if	(pDispatch)
	{
		pDispatch->AddRef ();
	}
	return COleVariant (lVariant);
}

inline CString VariantString (const VARIANT & pVariant)
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
		COleVariant	lVariant (pVariant);

		VariantChangeType (&lVariant, &lVariant, 0, VT_BSTR);

		if	(V_VT (&lVariant) == VT_BSTR)
		{
			return CString (V_BSTR (&lVariant));
		}
	}
	return CString();
}

inline CString VariantToString (COleVariant & pVariant)
{
	if	(V_VT (&pVariant) == VT_LPSTR)
	{
		V_BSTR (&pVariant) = CString ((LPCSTR) pVariant.byref).AllocSysString ();
		V_VT (&pVariant) = VT_BSTR;
	}
	else
	if	(V_VT (&pVariant) == VT_LPWSTR)
	{
		V_BSTR (&pVariant) = CString ((LPCWSTR) pVariant.byref).AllocSysString ();
		V_VT (&pVariant) = VT_BSTR;
	}

	if	(V_VT (&pVariant) == VT_BSTR)
	{
		return CString (V_BSTR (&pVariant));
	}
	else
	{
		VariantChangeType (&pVariant, &pVariant, 0, VT_BSTR);

		if	(V_VT (&pVariant) == VT_BSTR)
		{
			return CString (V_BSTR (&pVariant));
		}
	}
	return CString();
}

////////////////////////////////////////////////////////////////////////

#define EmptyParm COleVariant (DISP_E_PARAMNOTFOUND, VT_ERROR)
#define IsEmptyParm(v) ((V_VT (v) == VT_EMPTY) || (V_VT (v) == VT_ERROR))

////////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG
////////////////////////////////////////////////////////////////////////

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
		case DBTYPE_DBTIMESTAMP:			lRet = _T("DBTYPE_DBTIMESTAMP"); break;
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

////////////////////////////////////////////////////////////////////////
#endif	// _DEBUG
////////////////////////////////////////////////////////////////////////
#endif	// _OLEVARIANTEX_H
