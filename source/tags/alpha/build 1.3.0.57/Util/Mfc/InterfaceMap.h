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
#ifndef	_INTERFACEMAP_H
#define _INTERFACEMAP_H
/////////////////////////////////////////////////////////////////////////////

#define	DECLARE_DISPATCH_IID() \
	virtual BOOL GetDispatchIID(IID* pIID);

#define	DECLARE_SUPPORTERRORINFO() \
	BEGIN_INTERFACE_PART(SupportErrorInfo, ISupportErrorInfo) \
		HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo (REFIID riid); \
	END_INTERFACE_PART(SupportErrorInfo)

#define	DECLARE_PROVIDECLASSINFO() \
	BEGIN_INTERFACE_PART(ProvideClassInfo, IProvideClassInfo) \
        HRESULT STDMETHODCALLTYPE GetClassInfo (ITypeInfo **ppTI); \
	END_INTERFACE_PART(ProvideClassInfo)

#define	DECLARE_PROVIDECLASSINFO2() \
	BEGIN_INTERFACE_PART(ProvideClassInfo, IProvideClassInfo2) \
        HRESULT STDMETHODCALLTYPE GetClassInfo (ITypeInfo **ppTI); \
		HRESULT STDMETHODCALLTYPE GetGUID (DWORD dwGuidKind, GUID *pGUID); \
	END_INTERFACE_PART(ProvideClassInfo)

/////////////////////////////////////////////////////////////////////////////
#endif // _INTERFACEMAP_H