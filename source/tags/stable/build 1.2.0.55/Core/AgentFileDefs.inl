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
#ifdef	_DEBUG
#ifndef	__cplusplus_cli
//#define	_DEBUG_INSTANCE		LogNormal
//#define	_DEBUG_DUMP			LogDebugFast
//#define	_DEBUG_FIND_NAME	LogDebugFast
//#define	_DEBUG_LOAD			LogDebugFast
//#define	_DEBUG_BLOCKS		LogDebugFast
//#define	_DEBUG_INDEX		LogDebugFast
//
//#define	_DEBUG_ANIMATION	LogDebugFast
//#define	_SAVE_PALETTE		LogDebugFast
//#define	_SAVE_ICON			LogDebugFast
//#define	_SAVE_IMAGE			LogDebugFast
//
//#define	_DUMP_ANIMATION		LogDebugFast
//#define	_DUMP_PALETTE		LogDebugFast
//#define	_DUMP_ICON			LogDebugFast
//#define	_DUMP_IMAGE			LogDebugFast
//#define	_SHOW_IMAGE			LogDebugFast
//#define	_TRACE_RESOURCES	(GetProfileDebugInt(_T("TraceResources"),LogVerbose,true)&0xFFFF|LogTime|LogHighVolume)
#endif
#endif

#ifndef	_LOG_LOAD_ERRS
#define	_LOG_LOAD_ERRS		LogDetails
#endif

//////////////////////////////////////////////////////////////////////

typedef const BYTE* LPCBYTE;
typedef const WORD* LPCWORD;
typedef const DWORD* LPCDWORD;
typedef const GUID* LPCGUID;
typedef const ULARGE_INTEGER* LPCULARGE_INTEGER;

static LPCTSTR		sAcdFileExt = _T(".acd");
static LPCTSTR		sAcsFileExt = _T(".acs");
static LPCTSTR		sAcfFileExt = _T(".acf");
static const DWORD	sAcsFileSignature = 0xABCDABC3;
static const DWORD	sAcfFileSignature = 0xABCDABC4;
static const DWORD	sAcsFileSignatureV15 = 0xABCDABC1;
static const DWORD	sAcsFileSignatureV15_0 = 0xE011CFD0;
static const DWORD	sAcsFileSignatureV15_1 = 0xE11AB1A1;

/////////////////////////////////////////////////////////////////////////////
#ifdef	__cplusplus_cli
using namespace System;
using namespace System::IO;
using namespace System::Globalization;
#pragma warning (disable : 4638)
#else
#define _B(x) x
#pragma warning (disable : 4482)
#endif
/////////////////////////////////////////////////////////////////////////////
