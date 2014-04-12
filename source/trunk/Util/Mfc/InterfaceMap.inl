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
#ifndef	_INTERFACEMAP_INL
#define	_INTERFACEMAP_INL
/////////////////////////////////////////////////////////////////////////////
#ifndef	_LOG_INSTANCE
#define	_LOG_INSTANCE		LogDetails
#endif
#ifndef	_LOG_RESULTS
#define	_LOG_RESULTS		LogDetails
#endif
/////////////////////////////////////////////////////////////////////////////

class CInterfaceBase : public CCmdTarget
{
public:
	AFX_MODULE_STATE * ModuleState () {return m_pModuleState;}
};

#define	METHOD_PROLOGUE_INHERITED(theClass, localClass, base) \
	METHOD_PROLOGUE_(theClass, localClass) \
	AFX_MANAGE_STATE(((CInterfaceBase&)pThis->base).ModuleState())

/////////////////////////////////////////////////////////////////////////////
#ifndef	_DEBUG_COM_QI
#define	_DEBUG_COM_QI	LogAlways
#endif
/////////////////////////////////////////////////////////////////////////////

#ifdef	_DEBUG_COM
#define	IMPLEMENT_IUNKNOWN(theClass, localClass) \
	ULONG STDMETHODCALLTYPE theClass::X##localClass::AddRef () \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		ULONG lRet = pThis->ExternalAddRef (); \
		LogMessage (_DEBUG_COM, _T("[%p(%d)] ") _T(#theClass) _T("::X") _T(#localClass) _T("::AddRef"), pThis, lRet); \
		return lRet; \
	} \
	ULONG STDMETHODCALLTYPE theClass::X##localClass::Release () \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		ULONG lRet = pThis->ExternalRelease (); \
		LogMessage (_DEBUG_COM, _T("[%p(%d)] ") _T(#theClass) _T("::X") _T(#localClass) _T("::Release"), pThis, lRet); \
		return lRet; \
	} \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::QueryInterface(REFIID iid, LPVOID* ppvObj) \
	{ \
		METHOD_PROLOGUE_EX(theClass, localClass) \
		HRESULT	lResult = pThis->ExternalQueryInterface(&iid, ppvObj); \
		LogComErrAnon (MinLogLevel(_DEBUG_COM,_DEBUG_COM_QI), lResult, _T("[%p(%d)] ") _T(#theClass) _T("::X") _T(#localClass) _T("::QueryInterface [%s]"), pThis, pThis->m_dwRef, CGuidStr::GuidName(iid)); \
		return lResult; \
	}
#else	// _DEBUG_COM
#define	IMPLEMENT_IUNKNOWN(theClass, localClass) \
	ULONG STDMETHODCALLTYPE theClass::X##localClass::AddRef () \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		return pThis->ExternalAddRef (); \
	} \
	ULONG STDMETHODCALLTYPE theClass::X##localClass::Release () \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		return pThis->ExternalRelease (); \
	} \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::QueryInterface(REFIID iid, LPVOID* ppvObj) \
	{ \
		METHOD_PROLOGUE_EX(theClass, localClass) \
		return pThis->ExternalQueryInterface(&iid, ppvObj); \
	}
#endif	// _DEBUG_COM

/////////////////////////////////////////////////////////////////////////////

#ifdef	_DEBUG_COM
#define	IMPLEMENT_IUNKNOWN_OWNED(theClass, localClass) \
	ULONG STDMETHODCALLTYPE theClass::X##localClass::AddRef () \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		ULONG lRet = pThis->ExternalAddRef (); \
		LogMessage (_DEBUG_COM, _T("[%p(%d)] ") _T(#theClass) _T("::X") _T(#localClass) _T("::AddRef"), pThis, lRet); \
		return lRet; \
	} \
	ULONG STDMETHODCALLTYPE theClass::X##localClass::Release () \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		ULONG lRet = pThis->ExternalRelease (); \
		LogMessage (_DEBUG_COM, _T("[%p(%d)] ") _T(#theClass) _T("::X") _T(#localClass) _T("::Release"), pThis, lRet); \
		return lRet; \
	} \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::QueryInterface(REFIID iid, LPVOID* ppvObj) \
	{ \
		METHOD_PROLOGUE_EX(theClass, localClass) \
		HRESULT	lResult = pThis->InternalQueryInterface(&iid, ppvObj); \
		if	(FAILED (lResult)) lResult = pThis->ExternalQueryInterface(&iid, ppvObj); \
		LogComErrAnon (MinLogLevel(_DEBUG_COM,_DEBUG_COM_QI), lResult, _T("[%p(%d)] ") _T(#theClass) _T("::X") _T(#localClass) _T("::QueryInterface [%s]"), pThis, pThis->m_dwRef, CGuidStr::GuidName(iid)); \
		return lResult; \
	}
#else	// _DEBUG_COM
#define	IMPLEMENT_IUNKNOWN_OWNED(theClass, localClass) \
	ULONG STDMETHODCALLTYPE theClass::X##localClass::AddRef () \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		return pThis->ExternalAddRef (); \
	} \
	ULONG STDMETHODCALLTYPE theClass::X##localClass::Release () \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		return pThis->ExternalRelease (); \
	} \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::QueryInterface(REFIID iid, LPVOID* ppvObj) \
	{ \
		METHOD_PROLOGUE_EX(theClass, localClass) \
		HRESULT	lResult = pThis->InternalQueryInterface(&iid, ppvObj); \
		if	(FAILED (lResult)) lResult = pThis->ExternalQueryInterface(&iid, ppvObj); \
		return lResult; \
	}
#endif	// _DEBUG_COM

/////////////////////////////////////////////////////////////////////////////

#ifdef	_DEBUG_COM
#define	IMPLEMENT_IUNKNOWN_INHERITED(theClass, localClass, base) \
	ULONG STDMETHODCALLTYPE theClass::X##localClass::AddRef () \
	{ \
		METHOD_PROLOGUE_INHERITED(theClass, localClass, base) \
		ULONG lRet = pThis->base.ExternalAddRef (); \
		LogMessage (_DEBUG_COM, _T("[%p(%d)] %s::") _T(#theClass) _T("::X") _T(#localClass) _T("::AddRef"), &pThis->base, lRet, ObjClassName(&pThis->base)); \
		return lRet; \
	} \
	ULONG STDMETHODCALLTYPE theClass::X##localClass::Release () \
	{ \
		METHOD_PROLOGUE_INHERITED(theClass, localClass, base) \
		ULONG lRet = pThis->base.ExternalRelease (); \
		LogMessage (_DEBUG_COM, _T("[%p(%d)] %s::") _T(#theClass) _T("::X") _T(#localClass) _T("::Release"), &pThis->base, lRet, ObjClassName(&pThis->base)); \
		return lRet; \
	} \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::QueryInterface(REFIID iid, LPVOID* ppvObj) \
	{ \
		METHOD_PROLOGUE_INHERITED(theClass, localClass, base) \
		HRESULT lResult = pThis->base.ExternalQueryInterface(&iid, ppvObj); \
		LogComErrAnon (MinLogLevel(_DEBUG_COM,_DEBUG_COM_QI), lResult, _T("[%p(%d)] %s::") _T(#theClass) _T("::X") _T(#localClass) _T("::QueryInterface [%s]"), &pThis->base, pThis->base.m_dwRef, ObjClassName(&pThis->base), CGuidStr::GuidName(iid)); \
		return lResult; \
	}
#else	// _DEBUG_COM
#define	IMPLEMENT_IUNKNOWN_INHERITED(theClass, localClass, base) \
	ULONG STDMETHODCALLTYPE theClass::X##localClass::AddRef () \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		return pThis->base.ExternalAddRef (); \
	} \
	ULONG STDMETHODCALLTYPE theClass::X##localClass::Release () \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		return pThis->base.ExternalRelease (); \
	} \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::QueryInterface(REFIID iid, LPVOID* ppvObj) \
	{ \
		METHOD_PROLOGUE_INHERITED(theClass, localClass, base) \
		HRESULT	lResult = pThis->base.ExternalQueryInterface(&iid, ppvObj); \
		return lResult; \
	}
#endif	// _DEBUG_COM

/////////////////////////////////////////////////////////////////////////////

#ifdef	_DEBUG_COM
#define	IMPLEMENT_IDISPATCH(theClass, localClass) \
	IMPLEMENT_IUNKNOWN(theClass, localClass) \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::GetTypeInfoCount(UINT*pctinfo) \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		HRESULT	lResult = reinterpret_cast <LPDISPATCH> (&pThis->m_xDispatch)->GetTypeInfoCount (pctinfo); \
		LogComErrAnon (MinLogLevel(_DEBUG_COM,LogAlways), lResult, _T("[%p(%d)] ") _T(#theClass) _T("::X") _T(#localClass) _T("::GetTypeInfoCount [%u]"), pThis, pThis->m_dwRef, (pctinfo?*pctinfo:0)); \
		return lResult; \
	} \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo) \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		HRESULT	lResult = reinterpret_cast <LPDISPATCH> (&pThis->m_xDispatch)->GetTypeInfo (iTInfo, lcid, ppTInfo); \
		LogComErrAnon (MinLogLevel(_DEBUG_COM,LogAlways), lResult, _T("[%p(%d)] ") _T(#theClass) _T("::X") _T(#localClass) _T("::GetTypeInfo [%u] [%8.8X] [%8.8X]"), pThis, pThis->m_dwRef, iTInfo, lcid, (ppTInfo ? (*ppTInfo) : NULL)); \
		return lResult; \
	} \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId) \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		HRESULT	lResult = reinterpret_cast <LPDISPATCH> (&pThis->m_xDispatch)->GetIDsOfNames (riid, rgszNames, cNames, lcid, rgDispId); \
		LogComErrAnon (MinLogLevel(_DEBUG_COM,LogAlways), lResult, _T("[%p(%d)] ") _T(#theClass) _T("::X") _T(#localClass) _T("::GetIDsOfNames [%u] [%8.8X] [%s]"), pThis, pThis->m_dwRef, cNames, lcid, CGuidStr::GuidName(riid)); \
		return lResult; \
	} \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT*puArgErr) \
	{ \
		METHOD_PROLOGUE_EX(theClass, localClass) \
		HRESULT	lResult = reinterpret_cast <LPDISPATCH> (&pThis->m_xDispatch)->Invoke (dispIdMember, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr); \
		LogComErrAnon (MinLogLevel(_DEBUG_COM,LogAlways), lResult, _T("[%p(%d)] ") _T(#theClass) _T("::X") _T(#localClass) _T("::Invoke [%d] [%8.8X] [%s]"), pThis, pThis->m_dwRef, dispIdMember, lcid, CGuidStr::GuidName(riid)); \
		return lResult; \
	}
#else	// _DEBUG_COM
#define	IMPLEMENT_IDISPATCH(theClass, localClass) \
	IMPLEMENT_IUNKNOWN(theClass, localClass) \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::GetTypeInfoCount(UINT*pctinfo) \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		return reinterpret_cast <LPDISPATCH> (&pThis->m_xDispatch)->GetTypeInfoCount (pctinfo); \
	} \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo) \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		return reinterpret_cast <LPDISPATCH> (&pThis->m_xDispatch)->GetTypeInfo (iTInfo, lcid, ppTInfo); \
	} \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId) \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		return reinterpret_cast <LPDISPATCH> (&pThis->m_xDispatch)->GetIDsOfNames (riid, rgszNames, cNames, lcid, rgDispId); \
	} \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT*puArgErr) \
	{ \
		METHOD_PROLOGUE_EX(theClass, localClass) \
		return reinterpret_cast <LPDISPATCH> (&pThis->m_xDispatch)->Invoke (dispIdMember, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr); \
	}
#endif	// _DEBUG_COM

/////////////////////////////////////////////////////////////////////////////

#ifdef	_DEBUG_COM
#define	IMPLEMENT_IDISPATCH_NOMAP(theClass, localClass) \
	IMPLEMENT_IUNKNOWN(theClass, localClass) \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::GetTypeInfoCount(UINT*pctinfo) \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		HRESULT	lResult = reinterpret_cast <LPDISPATCH> (&pThis->m_xDispatch)->GetTypeInfoCount (pctinfo); \
		LogComErrAnon (MinLogLevel(_DEBUG_COM,LogAlways), lResult, _T("[%p(%d)] ") _T(#theClass) _T("::X") _T(#localClass) _T("::GetTypeInfoCount [%u]"), pThis, pThis->m_dwRef, (pctinfo?*pctinfo:0)); \
		return lResult; \
	} \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo) \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		HRESULT	lResult = reinterpret_cast <LPDISPATCH> (&pThis->m_xDispatch)->GetTypeInfo (iTInfo, lcid, ppTInfo); \
		LogComErrAnon (MinLogLevel(_DEBUG_COM,LogAlways), lResult, _T("[%p(%d)] ") _T(#theClass) _T("::X") _T(#localClass) _T("::GetTypeInfo [%u] [%8.8X] [%8.8X]"), pThis, pThis->m_dwRef, iTInfo, lcid, (ppTInfo ? (*ppTInfo) : NULL)); \
		return lResult; \
	} \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId) \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		HRESULT	lResult = reinterpret_cast <LPDISPATCH> (&pThis->m_xDispatch)->GetIDsOfNames (riid, rgszNames, cNames, lcid, rgDispId); \
		LogComErrAnon (MinLogLevel(_DEBUG_COM,LogAlways), lResult, _T("[%p(%d)] ") _T(#theClass) _T("::X") _T(#localClass) _T("::GetIDsOfNames [%u] [%8.8X] [%s]"), pThis, pThis->m_dwRef, cNames, lcid, CGuidStr::GuidName(riid)); \
		return lResult; \
	} \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT*puArgErr) \
	{ \
		METHOD_PROLOGUE_EX(theClass, localClass) \
		ITypeInfo * lTypeInfo = NULL; \
		HRESULT lResult = GetTypeInfo (0, lcid, &lTypeInfo); \
		if	(SUCCEEDED (lResult)) \
		{ \
			lResult = lTypeInfo->Invoke (&pThis->m_x##localClass, dispIdMember, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr); \
		} \
		if	(lTypeInfo) \
		{ \
			lTypeInfo->Release (); \
		} \
		LogComErrAnon (MinLogLevel(_DEBUG_COM,LogAlways), lResult, _T("[%p(%d)] ") _T(#theClass) _T("::X") _T(#localClass) _T("::Invoke [%d] [%8.8X] [%s]"), pThis, pThis->m_dwRef, dispIdMember, lcid, CGuidStr::GuidName(riid)); \
		return lResult; \
	}
#else	// _DEBUG_COM
#define	IMPLEMENT_IDISPATCH_NOMAP(theClass, localClass) \
	IMPLEMENT_IUNKNOWN(theClass, localClass) \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::GetTypeInfoCount(UINT*pctinfo) \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		return reinterpret_cast <LPDISPATCH> (&pThis->m_xDispatch)->GetTypeInfoCount (pctinfo); \
	} \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo) \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		return reinterpret_cast <LPDISPATCH> (&pThis->m_xDispatch)->GetTypeInfo (iTInfo, lcid, ppTInfo); \
	} \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId) \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		return reinterpret_cast <LPDISPATCH> (&pThis->m_xDispatch)->GetIDsOfNames (riid, rgszNames, cNames, lcid, rgDispId); \
	} \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT*puArgErr) \
	{ \
		METHOD_PROLOGUE_EX(theClass, localClass) \
		ITypeInfo * lTypeInfo = NULL; \
		HRESULT lResult = GetTypeInfo (0, lcid, &lTypeInfo); \
		if	(SUCCEEDED (lResult)) \
		{ \
			lResult = lTypeInfo->Invoke (&pThis->m_x##localClass, dispIdMember, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr); \
		} \
		if	(lTypeInfo) \
		{ \
			lTypeInfo->Release (); \
		} \
		return lResult; \
	}
#endif	// _DEBUG_COM

/////////////////////////////////////////////////////////////////////////////

#ifdef	_DEBUG_COM
#define	IMPLEMENT_IDISPATCH_AGGREGATE(theClass, localClass, aggregate) \
	IMPLEMENT_IUNKNOWN(theClass, localClass) \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::GetTypeInfoCount(UINT*pctinfo) \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		HRESULT	lResult = pThis->aggregate->GetTypeInfoCount (pctinfo); \
		LogComErrAnon (MinLogLevel(_DEBUG_COM,LogAlways), lResult, _T("[%p(%d)] ") _T(#theClass) _T("::X") _T(#localClass) _T("::GetTypeInfoCount [%u]"), pThis, pThis->m_dwRef, (pctinfo?*pctinfo:0)); \
		return lResult; \
	} \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo) \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		HRESULT	lResult = pThis->aggregate->GetTypeInfo (iTInfo, lcid, ppTInfo); \
		LogComErrAnon (MinLogLevel(_DEBUG_COM,LogAlways), lResult, _T("[%p(%d)] ") _T(#theClass) _T("::X") _T(#localClass) _T("::GetTypeInfo [%u] [%8.8X] [%8.8X]"), pThis, pThis->m_dwRef, iTInfo, lcid, (ppTInfo ? (*ppTInfo) : NULL)); \
		return lResult; \
	} \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId) \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		HRESULT	lResult = pThis->aggregate->GetIDsOfNames (riid, rgszNames, cNames, lcid, rgDispId); \
		LogComErrAnon (MinLogLevel(_DEBUG_COM,LogAlways), lResult, _T("[%p(%d)] ") _T(#theClass) _T("::X") _T(#localClass) _T("::GetIDsOfNames [%u] [%8.8X] [%s]"), pThis, pThis->m_dwRef, cNames, lcid, CGuidStr::GuidName(riid)); \
		return lResult; \
	} \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT*puArgErr) \
	{ \
		METHOD_PROLOGUE_EX(theClass, localClass) \
		ITypeInfo * lTypeInfo = NULL; \
		HRESULT lResult = GetTypeInfo (0, lcid, &lTypeInfo); \
		if	(SUCCEEDED (lResult)) \
		{ \
			lResult = lTypeInfo->Invoke (pThis->aggregate, dispIdMember, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr); \
		} \
		if	(lTypeInfo) \
		{ \
			lTypeInfo->Release (); \
		} \
		LogComErrAnon (MinLogLevel(_DEBUG_COM,LogAlways), lResult, _T("[%p(%d)] ") _T(#theClass) _T("::X") _T(#localClass) _T("::Invoke [%d] [%8.8X] [%s]"), pThis, pThis->m_dwRef, dispIdMember, lcid, CGuidStr::GuidName(riid)); \
		return lResult; \
	}
#else	// _DEBUG_COM
#define	IMPLEMENT_IDISPATCH_AGGREGATE(theClass, localClass, aggregate) \
	IMPLEMENT_IUNKNOWN(theClass, localClass) \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::GetTypeInfoCount(UINT*pctinfo) \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		return pThis->aggregate->GetTypeInfoCount (pctinfo); \
	} \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo) \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		return pThis->aggregate->GetTypeInfo (iTInfo, lcid, ppTInfo); \
	} \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId) \
	{ \
		METHOD_PROLOGUE_EX_(theClass, localClass) \
		return pThis->aggregate->GetIDsOfNames (riid, rgszNames, cNames, lcid, rgDispId); \
	} \
	HRESULT STDMETHODCALLTYPE theClass::X##localClass::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT*puArgErr) \
	{ \
		METHOD_PROLOGUE_EX(theClass, localClass) \
		ITypeInfo * lTypeInfo = NULL; \
		HRESULT lResult = GetTypeInfo (0, lcid, &lTypeInfo); \
		if	(SUCCEEDED (lResult)) \
		{ \
			lResult = lTypeInfo->Invoke (pThis->aggregate, dispIdMember, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr); \
		} \
		if	(lTypeInfo) \
		{ \
			lTypeInfo->Release (); \
		} \
		return lResult; \
	}
#endif	// _DEBUG_COM

/////////////////////////////////////////////////////////////////////////////

#define	IMPLEMENT_DISPATCH_IID(theClass, interfaceId) \
	BOOL theClass::GetDispatchIID(IID* pIID) {*pIID = interfaceId; return TRUE;}

/////////////////////////////////////////////////////////////////////////////

#ifdef	_DEBUG_COM
#define	BEGIN_SUPPORTERRORINFO(theClass) \
	IMPLEMENT_IUNKNOWN(theClass,SupportErrorInfo) \
	HRESULT STDMETHODCALLTYPE theClass::XSupportErrorInfo::InterfaceSupportsErrorInfo (REFIID riid) \
	{ \
		METHOD_PROLOGUE(theClass, SupportErrorInfo) \
		LogMessage (_DEBUG_COM, _T("[%p(%d)] ") _T(#theClass) _T("::XSupportErrorInfo::InterfaceSupportsErrorInfo [%s]"), pThis, pThis->m_dwRef, CGuidStr::GuidName(riid));
#else	// _DEBUG_COM
#define	BEGIN_SUPPORTERRORINFO(theClass) \
	IMPLEMENT_IUNKNOWN(theClass,SupportErrorInfo) \
	HRESULT STDMETHODCALLTYPE theClass::XSupportErrorInfo::InterfaceSupportsErrorInfo (REFIID riid) \
	{ \
		METHOD_PROLOGUE(theClass, SupportErrorInfo)
#endif	// _DEBUG_COM

#define IMPLEMENT_SUPPORTERRORINFO(theClass, interfaceId) \
		if	(IsEqualIID (riid, interfaceId)) return S_OK;

#define	END_SUPPORTERRORINFO(theClass) \
		return S_FALSE;\
	}

/////////////////////////////////////////////////////////////////////////////

#ifdef	_DEBUG_COM
#define IMPLEMENT_PROVIDECLASSINFO(theClass, classId) \
	IMPLEMENT_IUNKNOWN(theClass,ProvideClassInfo) \
	HRESULT STDMETHODCALLTYPE theClass::XProvideClassInfo::GetClassInfo (ITypeInfo **ppTI) \
	{ \
		METHOD_PROLOGUE(theClass, ProvideClassInfo) \
		HRESULT lResult = pThis->GetTypeInfoOfGuid(GetUserDefaultLCID(), classId, ppTI); \
		LogComErrAnon (MinLogLevel(_DEBUG_COM,LogAlways), lResult, _T("[%p(%d)] ") _T(#theClass) _T("::XProvideClassInfo::GetClassInfo [%s]"), pThis, pThis->m_dwRef, CGuidStr::GuidName(classId)); \
		return lResult; \
	}
#else	// _DEBUG_COM
#define IMPLEMENT_PROVIDECLASSINFO(theClass, classId) \
	IMPLEMENT_IUNKNOWN(theClass,ProvideClassInfo) \
	HRESULT STDMETHODCALLTYPE theClass::XProvideClassInfo::GetClassInfo (ITypeInfo **ppTI) \
	{ \
		METHOD_PROLOGUE(theClass, ProvideClassInfo) \
		return pThis->GetTypeInfoOfGuid(GetUserDefaultLCID(), classId, ppTI); \
	}
#endif	// _DEBUG_COM

/////////////////////////////////////////////////////////////////////////////

#define	BEGIN_PROVIDECLASSINFO2(theClass, classId) \
	IMPLEMENT_PROVIDECLASSINFO(theClass, classId) \
	HRESULT STDMETHODCALLTYPE theClass::XProvideClassInfo::GetGUID (DWORD dwGuidKind, GUID *pGUID) \
	{ \
		METHOD_PROLOGUE(theClass, ProvideClassInfo) \
		if	(!pGUID) \
		{ \
			return E_POINTER; \
		} \

#define IMPLEMENT_PROVIDECLASSINFO2(theClass, interfaceId) \
		if	(dwGuidKind == GUIDKIND_DEFAULT_SOURCE_DISP_IID) \
		{ \
			*pGUID = interfaceId; \
			return NOERROR; \
		}

#define	END_PROVIDECLASSINFO2(theClass) \
		*pGUID = GUID_NULL; \
		return E_INVALIDARG; \
	}

/////////////////////////////////////////////////////////////////////////////
#endif // _INTERFACEMAP_INL
