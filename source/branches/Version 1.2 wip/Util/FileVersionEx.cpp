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
#include "stdafx.h"
#include <shellapi.h>
#include <ddeml.h>
#include "FileVersionEx.h"
#include "Localize.h"
#include "Log.h"

#pragma comment(lib, "version.lib")

#ifdef	__AFX_H__
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#endif

//////////////////////////////////////////////////////////////////////

CFileVersionEx::CFileVersionEx (void* pVersionInfo, LPCTSTR pFileName)
:	CFileVersion ((void*) NULL),
	mFileName (pFileName)
{
	GetVersionInfo (pVersionInfo);
}

CFileVersionEx::CFileVersionEx (LPCTSTR pFileName)
:	CFileVersion ((LPCTSTR) NULL),
	mFileName (pFileName)
{
	GetFileInfo (pFileName);
}

CFileVersionEx::CFileVersionEx (HMODULE pModule, LPCTSTR pFileName)
:	CFileVersion ((HMODULE) NULL),
	mFileName (pFileName)
{
	GetModuleInfo (pModule);
}

CFileVersionEx::CFileVersionEx (const CFileVersionEx& pSource)
:	CFileVersion (pSource),
	mFileName (pSource.mFileName)
{
	int	lNdx;

	for	(lNdx = 0; lNdx < (int)pSource.mLocales.GetCount(); lNdx++)
	{
		if	(pSource.mLocales [lNdx])
		{
			mLocales.Add (new CVersionLocale (*(pSource.mLocales [lNdx])));
		}
	}

	for	(lNdx = 0; lNdx < (int)pSource.mStrings.GetCount(); lNdx++)
	{
		if	(pSource.mStrings [lNdx])
		{
			mStrings.Add (new CVersionStrings (*(pSource.mStrings [lNdx])));
		}
	}
}

CFileVersionEx::~CFileVersionEx ()
{
}

//////////////////////////////////////////////////////////////////////

void CFileVersionEx::GetVersionInfo (void* pVersionInfo)
{
	if	(pVersionInfo)
	{
		VS_VERSION_INFO_W * lVersionInfoW = (VS_VERSION_INFO_W *) pVersionInfo;
		VS_VERSION_INFO_A * lVersionInfoA = (VS_VERSION_INFO_A *) pVersionInfo;

		if	(
				(lVersionInfoW->Value.dwSignature == 0xFEEF04BD)
			&&	(lVersionInfoW->szKey [sizeof (lVersionInfoW->szKey) / sizeof (WCHAR)] == 0)
			&&	(wcscmp (lVersionInfoW->szKey, L"VS_VERSION_INFO") == 0)
			)
		{
			GetVersionInfoW (pVersionInfo);
		}
		else
		if	(
				(lVersionInfoA->Value.dwSignature == 0xFEEF04BD)
			&&	(lVersionInfoA->szKey [sizeof (lVersionInfoA->szKey) - 1] == 0)
			&&	(strcmp (lVersionInfoA->szKey, "VS_VERSION_INFO") == 0)
			)
		{
			GetVersionInfoA (pVersionInfo);
		}
	}
}

void CFileVersionEx::GetVersionInfoW (void* pVersionInfo)
{
	VS_VERSION_INFO_W * lVersionInfo = (VS_VERSION_INFO_W *) pVersionInfo;
	int 				lChildLength;
	char *				lChildren;
	StringFileInfoW *	lStringInfo;
	VarFileInfoW *		lVarInfo;

	memcpy (&mFileInfo, &lVersionInfo->Value, sizeof (VS_FIXEDFILEINFO));

	lChildLength = lVersionInfo->wLength - lVersionInfo->wValueLength - (sizeof (WORD) * 4) - sizeof (lVersionInfo->szKey);
	lChildren = (lVersionInfo->wValueLength) ? (char *) &lVersionInfo->Children : (char *) &lVersionInfo->Value;

//	  OutputDebugString (CString ("<%ls>\n").fSPrintf (0, lVersionInfo->szKey));

	while (lChildLength > 0)
	{
		if	(
				(lStringInfo = (StringFileInfoW *) lChildren)
			&&	(wcscmp (lStringInfo->szKey, L"StringFileInfo") == 0)
			)
		{
			CVersionStrings *	lStrings;
			INT_PTR				lStringNdx;
			int 				lInfoLength;
			StringTableW *		lTable;
			int 				lTableLength;
			StringW *			lString;
			int 				lStringLength;
			int 				lKeyLength;
			WCHAR * 			lStringVal;

			lInfoLength = lStringInfo->wLength	- (sizeof (WORD) * 3) - sizeof (lStringInfo->szKey);
			lTable = &lStringInfo->Children;

			while (lInfoLength > 0)
			{
				lStrings = new CVersionStrings (CString ((BSTR) lTable->szKey));
				mStrings.Add (lStrings);

				lTableLength = lTable->wLength	- (sizeof (WORD) * 3) - sizeof (lTable->szKey);
				lString = &lTable->Children;

				while (lTableLength > 0)
				{
					lStringNdx = lStrings->Add (CVersionString (CString ((BSTR) lString->szKey)));
					if	(lString->wValueLength)
					{
						lKeyLength = (int)wcslen (lString->szKey);
						lKeyLength = ((lKeyLength + 1) / 2) * 2;
						lStringVal = lString->Value;
						lStringVal += lKeyLength;

						if	(iswprint (*lStringVal))
						{
							CVersionString& lString = (*lStrings) [lStringNdx];
							lString.mValue = CString ((BSTR) lStringVal);
							lString.mValue.TrimLeft ();
							lString.mValue.TrimRight ();
						}
					}

					lStringLength = ((lString->wLength + 3) / 4) * 4;
					lTableLength -= lStringLength;
					lString = (StringW *) (((char *) lString) + lStringLength);

					if	(lStringLength == 0)
					{
						LogMessage (LogIfActive, _T("Invalid or corrupt version info in [%s]"), mFileName);
						break;
					}
				}

				lInfoLength -= lTable->wLength;
				lTable = (StringTableW *) (((char *) lTable) + lTable->wLength);

				if	(lTable->wLength <= 0)
				{
					break;
				}
			}

			lChildLength -= lStringInfo->wLength;
			lChildren += lStringInfo->wLength;
		}
		else
		if	(
				(lVarInfo = (VarFileInfoW *) lChildren)
			&&	(wcscmp (lVarInfo->szKey, L"VarFileInfo") == 0)
			)
		{
			VarW *	lVar = (VarW *) &lVarInfo->Children;
			int 	lCount = lVar->wValueLength / sizeof (DWORD);
			DWORD* lList = (DWORD*) &lVar->Value;

			//OutputDebugString (CString (" <%ls>\n").fSPrintf (0, lVarInfo->szKey));

			if	(lCount > 0)
			{
				int lNdx;
				for (lNdx = 0; lNdx < lCount; lNdx++)
				{
					//OutputDebugString (CString ("  Language %hu %4.4hX CodePage %hu %4.4hX\n").fSPrintf (0, LOWORD (lList [lNdx]), LOWORD (lList [lNdx]), HIWORD (lList [lNdx]), HIWORD (lList [lNdx])));
					mLocales.Add (new CVersionLocale (lList [lNdx]));
				}
			}

			lChildLength -= lVarInfo->wLength;
			lChildren += lVarInfo->wLength;
		}
		else
		{
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////

void CFileVersionEx::GetVersionInfoA (void* pVersionInfo)
{
	VS_VERSION_INFO_A * lVersionInfo = (VS_VERSION_INFO_A *) pVersionInfo;
	int 				lChildLength;
	char *				lChildren;
	StringFileInfoA *	lStringInfo;
	VarFileInfoA *		lVarInfo;

	memcpy (&mFileInfo, &lVersionInfo->Value, sizeof (VS_FIXEDFILEINFO));

	lChildLength = lVersionInfo->wLength - lVersionInfo->wValueLength - (sizeof (WORD) * 2) - sizeof (lVersionInfo->szKey);
	lChildren = (lVersionInfo->wValueLength) ? (char *) &lVersionInfo->Children : (char *) &lVersionInfo->Value;

	while (lChildLength > 0)
	{
		if	(
				(lStringInfo = (StringFileInfoA *) lChildren)
			&&	(strcmp (lStringInfo->szKey, "StringFileInfo") == 0)
			)
		{
			CVersionStrings *	lStrings;
			INT_PTR				lStringNdx;
			int 				lInfoLength;
			StringTableA *		lTable;
			int 				lTableLength;
			StringA *			lString;
			int 				lStringLength;
			int 				lKeyLength;
			char *				lStringVal;

			lInfoLength = lStringInfo->wLength	- (sizeof (WORD) * 4) - sizeof (lStringInfo->szKey);
			lInfoLength = ((lInfoLength + 3) / 4) * 4;
			lTable = &lStringInfo->Children;

			while (lInfoLength > 0)
			{
				lStrings = new CVersionStrings (CString (lTable->szKey));
				mStrings.Add (lStrings);

				lTableLength = lTable->wLength - (sizeof (WORD) * 4) - sizeof (lTable->szKey);
				lTableLength = ((lTableLength + 3) / 4) * 4;
				lString = &lTable->Children;

				while (lTableLength > 0)
				{
					lStringNdx = lStrings->Add (CVersionString (CString (lString->szKey)));
					if	(lString->wValueLength)
					{
						lKeyLength = (int)strlen (lString->szKey) + 1;
						lKeyLength = ((lKeyLength + 3) / 4) * 4;
						lStringVal = lString->szKey;
						lStringVal += lKeyLength;

						if	(isprint (*lStringVal))
						{
							CVersionString& lString = (*lStrings) [lStringNdx];
							lString.mValue = lStringVal;
							lString.mValue.TrimLeft ();
							lString.mValue.TrimRight ();
						}
					}

					lStringLength = ((lString->wLength + 3) / 4) * 4;
					lTableLength -= lStringLength;
					lString = (StringA *) (((char *) lString) + lStringLength);
				}

				lInfoLength -= lTable->wLength;
				lTable = (StringTableA *) (((char *) lTable) + lTable->wLength);

				if	(lTable->wLength <= 0)
				{
					break;
				}
			}

			lChildLength -= lStringInfo->wLength;
			lChildren += lStringInfo->wLength;
		}
		else
		if	(
				(lVarInfo = (VarFileInfoA *) lChildren)
			&&	(strcmp (lVarInfo->szKey, "VarFileInfo") == 0)
			)
		{
			VarA *	lVar = (VarA *) &lVarInfo->Children;
			int 	lCount = lVar->wValueLength / sizeof (DWORD);
			DWORD* lList = (DWORD*) &lVar->Value;

			if	(lCount > 0)
			{
				int lNdx;
				for (lNdx = 0; lNdx < lCount; lNdx++)
				{
					mLocales.Add (new CVersionLocale (lList [lNdx]));
				}
			}
			lChildLength -= lVarInfo->wLength;
			lChildren += lVarInfo->wLength;
		}
		else
		{
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CString CFileVersionEx::FileVersionName (LPCTSTR pFormatString) const
{
	CString				lRet;
	CVersionStrings *	lStrings;
	CVersionString*	lString;
	ULARGE_INTEGER		lVersion;

	lVersion.HighPart = mFileInfo.dwFileVersionMS;
	lVersion.LowPart = mFileInfo.dwFileVersionLS;

	if	(
			(pFormatString)
		&&	(mStrings.GetCount() > 0)
		&&	(lStrings = mStrings [0])
		&&	(lString = lStrings->Find (mFileVersionName))
		)
	{
		lRet = FormatVersion (lVersion.QuadPart, lString->mValue, pFormatString);
	}
	else
	{
		lRet = FormatVersion (lVersion.QuadPart);
	}
	return lRet;
}

CString CFileVersionEx::ProductVersionName (LPCTSTR pFormatString) const
{
	CString				lRet;
	CVersionStrings *	lStrings;
	CVersionString*	lString;
	ULARGE_INTEGER		lVersion;

	lVersion.HighPart = mFileInfo.dwProductVersionMS;
	lVersion.LowPart = mFileInfo.dwProductVersionLS;

	if	(
			(pFormatString)
		&&	(mStrings.GetCount() > 0)
		&&	(lStrings = mStrings [0])
		&&	(lString = lStrings->Find (mProductVersionName))
		)
	{
		lRet = FormatVersion (lVersion.QuadPart, lString->mValue, pFormatString);
	}
	else
	{
		lRet = FormatVersion (lVersion.QuadPart);
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////

CString CFileVersionEx::AppVersionName () const
{
	CString								lVersion;
	CFileVersionEx::CVersionString*	lVersionString;
	CString								lBuild;

	lVersion = ProductVersionName (NULL);
	MakeValidVersion (lVersion, true);

	if	(
			(!lVersion.IsEmpty ())
		&&	(lVersion.Find (_T('.')) < 0)
		)
	{
		lVersion += _T(".0");
	}

	if	(lVersionString = mStrings.Find (mSpecialBuildName))
	{
		lBuild = lVersionString->mValue;
		lBuild.TrimLeft ();
		lBuild.TrimRight ();
		lBuild.TrimLeft (_T("("));
		lBuild.TrimRight (_T(")"));
	}

	if	(!lBuild.IsEmpty ())
	{
		lVersion += _T(" (") + lBuild + _T(")");
	}

	return lVersion;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

DWORD CFileVersionEx::FileType () const
{
	return mFileInfo.dwFileType & 0x0000000F;
}

DWORD CFileVersionEx::FileFlags () const
{
	DWORD lRet = mFileInfo.dwFileFlags;
	if	(mFileInfo.dwFileFlagsMask)
	{
		lRet &= mFileInfo.dwFileFlagsMask;
	}
	return lRet;
}

DWORD CFileVersionEx::OsType () const
{
	return (mFileInfo.dwFileOS & 0xFFFF0000);
}

DWORD CFileVersionEx::OsSubType () const
{
	return (mFileInfo.dwFileOS & 0x0000FFFF);
}

DWORD CFileVersionEx::DriverType () const
{
	if	(mFileInfo.dwFileType == VFT_DRV)
	{
		return mFileInfo.dwFileSubtype;
	}
	return 0;
}

DWORD CFileVersionEx::FontType () const
{
	if	((mFileInfo.dwFileType & 0x0000000F) == VFT_FONT)
	{
		return mFileInfo.dwFileSubtype;
	}
	return 0;
}

DWORD CFileVersionEx::DeviceId () const
{
	if	((mFileInfo.dwFileType & 0x0000000F) == VFT_VXD)
	{
		return mFileInfo.dwFileSubtype;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

const CString	CFileVersionEx::mFileTypeNameUnknown (_T("Unknown"));
const CString	CFileVersionEx::mFileTypeNameExe (_T("Application"));
const CString	CFileVersionEx::mFileTypeNameDll (_T("Application Extension"));
const CString	CFileVersionEx::mFileTypeNameDrv (_T("Device driver"));
const CString	CFileVersionEx::mFileTypeNameFon (_T("Font"));
const CString	CFileVersionEx::mFileTypeNameVxd (_T("Virtual device driver"));
const CString	CFileVersionEx::mFileTypeNameLib (_T("Library"));
const CString	CFileVersionEx::mFileTypeNameOcx (_T("ActiveX Control"));

CString CFileVersionEx::FileTypeName (DWORD pFileType)
{
	static bool lTypesInitialized = false;

	if	(!lTypesInitialized)
	{
		tS <SHFILEINFO>	lFileInfo;

		lFileInfo.Clear ();
		if	(
				(SHGetFileInfo (_T(".exe"), FILE_ATTRIBUTE_NORMAL, &lFileInfo, sizeof (lFileInfo), SHGFI_USEFILEATTRIBUTES | SHGFI_TYPENAME))
			&&	(lFileInfo.szTypeName [0])
			)
		{
			const_cast <CString&> (mFileTypeNameExe) = lFileInfo.szTypeName;
		}

		lFileInfo.Clear ();
		if	(
				(SHGetFileInfo (_T(".dll"), FILE_ATTRIBUTE_NORMAL, &lFileInfo, sizeof (lFileInfo), SHGFI_USEFILEATTRIBUTES | SHGFI_TYPENAME))
			&&	(lFileInfo.szTypeName [0])
			)
		{
			const_cast <CString&> (mFileTypeNameDll) = lFileInfo.szTypeName;
		}

		lFileInfo.Clear ();
		if	(
				(SHGetFileInfo (_T(".drv"), FILE_ATTRIBUTE_NORMAL, &lFileInfo, sizeof (lFileInfo), SHGFI_USEFILEATTRIBUTES | SHGFI_TYPENAME))
			&&	(lFileInfo.szTypeName [0])
			)
		{
			const_cast <CString&> (mFileTypeNameDrv) = lFileInfo.szTypeName;
		}

		lFileInfo.Clear ();
		if	(
				(SHGetFileInfo (_T(".fon"), FILE_ATTRIBUTE_NORMAL, &lFileInfo, sizeof (lFileInfo), SHGFI_USEFILEATTRIBUTES | SHGFI_TYPENAME))
			&&	(lFileInfo.szTypeName [0])
			)
		{
			const_cast <CString&> (mFileTypeNameFon) = lFileInfo.szTypeName;
		}

		lFileInfo.Clear ();
		if	(
				(SHGetFileInfo (_T(".vxd"), FILE_ATTRIBUTE_NORMAL, &lFileInfo, sizeof (lFileInfo), SHGFI_USEFILEATTRIBUTES | SHGFI_TYPENAME))
			&&	(lFileInfo.szTypeName [0])
			)
		{
			const_cast <CString&> (mFileTypeNameVxd) = lFileInfo.szTypeName;
		}

		lFileInfo.Clear ();
		if	(
				(SHGetFileInfo (_T(".lib"), FILE_ATTRIBUTE_NORMAL, &lFileInfo, sizeof (lFileInfo), SHGFI_USEFILEATTRIBUTES | SHGFI_TYPENAME))
			&&	(lFileInfo.szTypeName [0])
			)
		{
			const_cast <CString&> (mFileTypeNameLib) = lFileInfo.szTypeName;
		}
	}

	if	((pFileType & 0x0000000F) == VFT_APP)
	{
		return mFileTypeNameExe;
	}
	else
	if	((pFileType & 0x0000000F) == VFT_DLL)
	{
		return mFileTypeNameDll;
	}
	else
	if	((pFileType & 0x0000000F) == VFT_DRV)
	{
		return mFileTypeNameDrv;
	}
	else
	if	((pFileType & 0x0000000F) == VFT_FONT)
	{
		return mFileTypeNameFon;
	}
	else
	if	((pFileType & 0x0000000F) == VFT_VXD)
	{
		return mFileTypeNameVxd;
	}
	else
	if	((pFileType & 0x0000000F) == VFT_STATIC_LIB)
	{
		return mFileTypeNameLib;
	}
	else
	if	((pFileType & 0x0000000F) == VFT_UNKNOWN)
	{
		return mFileTypeNameUnknown;
	}
#ifdef	_DEBUG
	else
	{
		CString lRet;
		lRet.Format (_T("%8.8X"), pFileType);
		return lRet;
	}
#endif

	return CString();
}

//////////////////////////////////////////////////////////////////////

bool CFileVersionEx::FileFlagsValid (DWORD& pFileFlags, DWORD& pFileFlagsMask) const
{
	pFileFlags = mFileInfo.dwFileFlags;
	pFileFlagsMask = mFileInfo.dwFileFlagsMask;

	if	(
			((mFileInfo.dwFileFlagsMask & VS_FFI_FILEFLAGSMASK) != VS_FFI_FILEFLAGSMASK)
		||	((mFileInfo.dwFileFlags & ~mFileInfo.dwFileFlagsMask) != 0)
		)
	{
		return false;
	}
	return true;
}

CString CFileVersionEx::FileFlagsNames (bool pUseStrings) const
{
	CString	lRet;
	DWORD	lFlags = mFileInfo.dwFileFlags;

	if	(mFileInfo.dwFileFlagsMask)
	{
		lFlags &= mFileInfo.dwFileFlagsMask;
	}

	if	(lFlags & VS_FF_DEBUG)
	{
		lRet += "Debug, ";
	}
	if	(lFlags & VS_FF_PRERELEASE)
	{
		lRet += "Prerelease, ";
	}
	if	(lFlags & VS_FF_PATCHED)
	{
		lRet += "Patched, ";
	}
	if	(lFlags & VS_FF_PRIVATEBUILD)
	{
		CVersionString* lString;
		if	(
				(pUseStrings)
			&&	(lString = mStrings.Find (mPrivateBuildName))
			&&	(!lString->mValue.IsEmpty ())
			)
		{
			lRet += "Private Build \"";
			lRet += lString->mValue;
			lRet += "\", ";
		}
/*
		else
		{
			lRet += "Private Build, ";
		}
*/
	}
	if	(lFlags & VS_FF_SPECIALBUILD)
	{
		CVersionString* lString;
		if	(
				(pUseStrings)
			&&	(lString = mStrings.Find (mSpecialBuildName))
			&&	(!lString->mValue.IsEmpty ())
			)
		{
			lRet += "Special Build \"";
			lRet += lString->mValue;
			lRet += "\", ";
		}
/*
		else
		{
			lRet += "Special Build, ";
		}
*/
	}
	if	(lFlags & VS_FF_INFOINFERRED)
	{
		lRet += "Inferred, ";
	}

	lRet.TrimRight (_T(", "));
	return lRet;
}

//////////////////////////////////////////////////////////////////////

LPCTSTR	CFileVersionEx::mOsTypeDos = _T("DOS");
LPCTSTR	CFileVersionEx::mOsTypeNt = _T("NT");
LPCTSTR	CFileVersionEx::mOsTypePm32 = _T("OS/2 32-bit");
LPCTSTR	CFileVersionEx::mOsTypePm16 = _T("OS/2 16-bit");

CString CFileVersionEx::OsTypeName (DWORD pOsType)
{
	if	((pOsType & 0x000F0000) == VOS_DOS)
	{
		return mOsTypeDos;
	}
	else
	if	((pOsType & 0x000F0000) == VOS_OS216)
	{
		return mOsTypePm16;
	}
	else
	if	((pOsType & 0x000F0000) == VOS_OS232)
	{
		return mOsTypePm32;
	}
	else
	if	((pOsType & 0x000F0000) == VOS_NT)
	{
		return mOsTypeNt;
	}

	return CString();
}

//////////////////////////////////////////////////////////////////////

LPCTSTR	CFileVersionEx::mOsSubTypeWin16 = _T("Windows 16-bit");
LPCTSTR	CFileVersionEx::mOsSubTypeWin32 = _T("Windows 32-bit");
LPCTSTR	CFileVersionEx::mOsSubTypePm16 = _T("Presentation Manager 16-bit");
LPCTSTR	CFileVersionEx::mOsSubTypePm32 = _T("Presentation Manager 32-bit");

CString CFileVersionEx::OsSubTypeName (DWORD pOsType)
{
	if	((pOsType & 0x0000000F) == VOS__WINDOWS16)
	{
		return mOsSubTypeWin16;
	}
	else
	if	((pOsType & 0x0000000F) == VOS__PM16)
	{
		return mOsSubTypePm16;
	}
	else
	if	((pOsType & 0x0000000F) == VOS__PM32)
	{
		return mOsSubTypePm32;
	}
	else
	if	((pOsType & 0x0000000F) == VOS__WINDOWS32)
	{
		return mOsSubTypeWin32;
	}

	return CString();
}

//////////////////////////////////////////////////////////////////////

LPCTSTR	CFileVersionEx::mOsNameWin16 = _T("Windows 95/98/ME");
LPCTSTR	CFileVersionEx::mOsNameWin32 = _T("Windows");
LPCTSTR	CFileVersionEx::mOsNameWinNt = _T("Windows NT/2000/XP");
LPCTSTR	CFileVersionEx::mOsNameWin16Cui = _T("Windows 95/98/ME (console)");
LPCTSTR	CFileVersionEx::mOsNameWin32Cui = _T("Windows (console)");
LPCTSTR	CFileVersionEx::mOsNameWinNtCui = _T("Windows NT/2000/XP (console)");
LPCTSTR	CFileVersionEx::mOsNameOs2 = _T("OS/2 Presentation Manager");
LPCTSTR	CFileVersionEx::mOsNameOs2Cui = _T("OS/2");
LPCTSTR	CFileVersionEx::mOsNamePosixCui = _T("Posix");
LPCTSTR	CFileVersionEx::mOsNameWin9xDriver = _T("Windows 9x driver");
LPCTSTR	CFileVersionEx::mOsNameWinCE = _T("Windows CE");
LPCTSTR	CFileVersionEx::mOsNameUnknown = _T("Unknown");

CString CFileVersionEx::OsName (DWORD pOsType)
{
	if	((pOsType & VOS_DOS_WINDOWS16) == VOS_DOS_WINDOWS16)
	{
		return mOsNameWin16;
	}
	else
	if	((pOsType & VOS_DOS_WINDOWS32) == VOS_DOS_WINDOWS32)
	{
		return mOsNameWin32;
	}
	else
	if	((pOsType & VOS_NT_WINDOWS32) == VOS_NT_WINDOWS32)
	{
		return mOsNameWinNt;
	}
	else
	if	((pOsType & 0x000F000F) & VOS__WINDOWS32)
	{
		return mOsNameWin32;
	}
	else
	if	((pOsType & 0x000F000F) & VOS__WINDOWS16)
	{
		return mOsNameWin16;
	}
	else
	if	((pOsType & 0x000F000F) & VOS_NT)
	{
		return mOsNameWinNtCui;
	}
	else
	if	((pOsType & 0x000F000F) & VOS_DOS)
	{
		return mOsNameWin16Cui;
	}
	else
	if	(
			((pOsType & 0x000F000F) == VOS_OS216_PM16)
		||	((pOsType & 0x000F000F) == VOS_OS232_PM32)
		)
	{
		return mOsNameOs2;
	}
	else
	if	(
			((pOsType & 0x000F000F) == VOS_OS216)
		||	((pOsType & 0x000F000F) == VOS_OS232)
		)
	{
		return mOsNameOs2Cui;
	}
	else
	if	((pOsType & 0x000F000F) == VOS_UNKNOWN)
	{
		return mOsNameUnknown;
	}
#ifdef	_DEBUG
	else
	{
		CString lRet;
		lRet.Format (_T("%8.8X"), pOsType);
		return lRet;
	}
#endif

	return CString();
}

//////////////////////////////////////////////////////////////////////

CString CFileVersionEx::DriverTypeName (DWORD pDriverType)
{
	return CString();
}

//////////////////////////////////////////////////////////////////////

CString CFileVersionEx::FontTypeName (DWORD pFontType)
{
	return CString();
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

inline static bool RemoveTrailingParentheses (CString& pString)
{
	int	lCharPos;

	pString.TrimRight ();

	if	(
			(pString.GetLength () > 2)
		&&	(pString [pString.GetLength ()-1] == _T(')'))
		&&	((lCharPos = pString.ReverseFind (_T('('))) > 0)
		)
	{
		pString.Delete (lCharPos, pString.GetLength () - lCharPos);
		return true;
	}

	return false;
}

inline static bool RemoveTrailingBrackets (CString& pString)
{
	int	lCharPos;

	pString.TrimRight ();

	if	(
			(pString.GetLength () > 2)
		&&	(pString [pString.GetLength ()-1] == _T(']'))
		&&	((lCharPos = pString.ReverseFind (_T('['))) > 0)
		)
	{
		pString.Delete (lCharPos, pString.GetLength () - lCharPos);
		return true;
	}

	return false;
}

inline static bool RemoveTrailingPunctuation (CString& pString)
{
	bool	lRet = false;
	int		lCharPos;

	pString.TrimRight ();

	for	(lCharPos = pString.GetLength ()-1; lCharPos > 0; lCharPos--)
	{
		if	(!_istpunct (pString [lCharPos]))
		{
			break;
		}
	}
	if	(lCharPos < pString.GetLength ()-1)
	{
		pString = pString.Left (lCharPos+1);
		lRet = true;
	}

	return lRet;
}

inline static bool RemoveTrailingVersion (CString& pString, UINT pStrength)
{
	bool	lRet = false;
	int		lCharPos;
	bool	lDigitFound = false;
	bool	lPointFound = false;

	pString.TrimRight ();

	lCharPos = pString.GetLength ()-1;
	while	(lCharPos >= 0)
	{
		if	(pString [lCharPos] == _T('.'))
		{
			lPointFound = true;
		}
		else
		if	(_istdigit (pString [lCharPos]))
		{
			lDigitFound = true;
		}
		else
		if	(
				(_istspace (pString [lCharPos]))
			||	(_istpunct (pString [lCharPos]))
			||	(lDigitFound)
			)
		{
			break;
		}
		lCharPos--;
	}

	if	(
			(lDigitFound)
		&&	(
				(lCharPos > 0)
			||	(
					(pStrength > 2)
				&&	(_istdigit (pString [pString.GetLength ()-1]))
				)
			)
		)
	{
		static CStringArray	lVersionLabels;
		INT_PTR				lNdx;
		CString				lString (pString.Left (lCharPos+1));

		lCharPos = max (lCharPos, 0);
		lString.TrimRight ();

		if	(!lString.IsEmpty ())
		{
			if	(lVersionLabels.GetCount() <= 0)
			{
				lVersionLabels.Add (_T("v"));
				lVersionLabels.Add (_T("v."));
				lVersionLabels.Add (_T("ver"));
				lVersionLabels.Add (_T("ver."));
				lVersionLabels.Add (_T("version"));
				lVersionLabels.Add (_T("SP"));
				lVersionLabels.Add (_T("SP."));
			}

			for	(lNdx = lVersionLabels.GetCount()-1; lNdx >= 0; lNdx--)
			{
				if	(
						(
							(
								(lString.GetLength () > lVersionLabels [lNdx].GetLength ())
							&&	(_istspace ((((LPCTSTR) lString) + lString.GetLength () - lVersionLabels [lNdx].GetLength ()) [-1]))
							)
						||	(
								(pStrength > 2)
							&&	(lString.GetLength () == lVersionLabels [lNdx].GetLength ())
							)
						)
					&&	(_tcsnicmp (((LPCTSTR) lString) + lString.GetLength () - lVersionLabels [lNdx].GetLength (), lVersionLabels [lNdx], lVersionLabels [lNdx].GetLength ()) == 0)
					)
				{
					lString = lString.Left (lString.GetLength () - lVersionLabels [lNdx].GetLength ());
					lString.TrimRight ();
					lString.TrimRight (_T(";,-"));
					lString.TrimRight ();

					pString = lString;
					lRet = true;
					break;
				}
			}
		}

		if	(
				(!lRet)
			&&	(
					(pStrength > 2)
				||	(!lString.IsEmpty ())
				)
			&&	(
					(lPointFound)
				||	(
						(pStrength > 0)
					&&	(
							(_istspace (pString [lCharPos]))
						||	(pString.Find (_T(' ')) < 0)
						)
					)
				)
			)
		{
			pString = lString;
			lRet = true;
		}
	}

	return lRet;
}

static inline void UnpluralizeWords (CString& pString)
{
	int	lNdx;
	int	lNdx2;

	pString.Replace (_T("IES "), _T("Y "));
	pString.Replace (_T("\'S "), _T("S "));

	for	(lNdx = 3; lNdx < pString.GetLength ()-1; lNdx++)
	{
		if	(
				(pString [lNdx] == _T('S'))
			&&	(pString [lNdx-1] != _T('S'))
			&&	(_istalpha (pString [lNdx-1]))
			&&	(_istalpha (pString [lNdx-2]))
			&&	(_istalpha (pString [lNdx-3]))
			)
		{
			for	(lNdx2 = lNdx+1; lNdx2 < pString.GetLength (); lNdx2++)
			{
				if	(_istspace (pString [lNdx2]))
				{
					pString.Delete (lNdx);
					break;
				}
				else
				if	(
						(!_istdigit (pString [lNdx2]))
					&&	(!_istpunct (pString [lNdx2]))
					)
				{
					break;
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

CString CFileVersionEx::MassageName (const CString& pName, bool pSingleLine)
{
	CString	lRet (pName);

	lRet.TrimLeft (_T(" -"));
	lRet.TrimRight (_T(" -"));
	lRet.TrimLeft ();
	lRet.TrimRight ();

	if	(!lRet.IsEmpty ())
	{
		if	(pSingleLine)
		{
			lRet.Replace (_T("\r\n"), _T(" "));
			lRet.Replace (_T('\n'), _T(' '));
			lRet.Replace (_T('\r'), _T(' '));
		}
		else
		{
			lRet.Replace (_T("\r\n"), _T("\n"));
			lRet.Replace (_T('\r'), _T('\n'));
			lRet.Replace (_T("\n"), _T("\r\n"));
		}
		lRet.Replace (_T('\t'), _T(' '));

		lRet.Replace (_T("(R)"), _T("®"));
		lRet.Replace (_T("(r)"), _T("®"));
		lRet.Replace (_T("(C)"), _T("©"));
		lRet.Replace (_T("(c)"), _T("©"));
		lRet.Replace (_T("(TM)"), _T("\x99"));
		lRet.Replace (_T("(tm)"), _T("\x99"));
		lRet.Replace (_T("(Tm)"), _T("\x99"));

		lRet.Replace (_T(" ®"), _T("®"));
		lRet.Replace (_T(" \x99"), _T("\x99"));
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////

CString	CFileVersionEx::MassageProductName (const CString& pProductName, UINT pStrength)
{
	CString	lRet (CFileVersionEx::MassageName (pProductName, true));

	lRet.TrimLeft ();
	lRet.TrimRight ();

	if	(!lRet.IsEmpty ())
	{

		while	(
					(RemoveTrailingParentheses (lRet))
				||	(RemoveTrailingBrackets (lRet))
				)
		{
			if	(pStrength > 0)
			{
				RemoveTrailingPunctuation (lRet);
			}
		}

		if	(
				(pStrength != 1)
			&&	(RemoveTrailingVersion (lRet, pStrength))
			)
		{
			while	(
						(RemoveTrailingParentheses (lRet))
					||	(RemoveTrailingBrackets (lRet))
					)
			{
				if	(pStrength > 0)
				{
					RemoveTrailingPunctuation (lRet);
				}
			}
		}

		if	(pStrength > 0)
		{
			CharUpperBuff (lRet.GetBuffer (lRet.GetLength ()), lRet.GetLength ());
			lRet.ReleaseBuffer ();
			lRet.TrimRight ();

			CString	lOriginal (lRet);

			lRet.Remove (_T('®'));
			lRet.Remove (_T('\x99'));
			lRet.Remove (_T('©'));
			lRet.Remove (_T('\''));
			lRet.Replace (_T('\"'), _T(' '));
			lRet.Replace (_T('.'), _T(' '));
			lRet.Replace (_T(','), _T(' '));
			lRet.Replace (_T(';'), _T(' '));
			lRet.Replace (_T(':'), _T(' '));
			lRet.Replace (_T('?'), _T(' '));
			lRet.Replace (_T('-'), _T(' '));
			lRet.Replace (_T('_'), _T(' '));
			lRet.Replace (_T('/'), _T(' '));
			lRet.Replace (_T('\\'), _T(' '));
			lRet.Replace (_T('('), _T(' '));
			lRet.Replace (_T(')'), _T(' '));
			lRet.Replace (_T('|'), _T(' '));

			RemoveTrailingPunctuation (lRet);

			while (lRet.Replace (_T("  "), _T(" "))) {}
			lRet += _T(' ');

			if	(
					(pStrength != 1)
				&&	(lRet.Replace (_T("WINDOWS "), _T("WINDOWS")))
				)
			{
				lRet.Replace (_T("WINDOWSNT "), _T("WINDOWS "));
				lRet.Replace (_T("WINDOWSME "), _T("WINDOWS "));
				lRet.Replace (_T("WINDOWSMILLENNIUM "), _T("WINDOWS "));
				lRet.Replace (_T("WINDOWS98 "), _T("WINDOWS "));
				lRet.Replace (_T("WINDOWS95 "), _T("WINDOWS "));
				lRet.Replace (_T("WINDOWS9X "), _T("WINDOWS "));
				lRet.Replace (_T("WINDOWS2000 "), _T("WINDOWS "));
				lRet.Replace (_T("WINDOWS2003 "), _T("WINDOWS "));
				lRet.Replace (_T("WINDOWS2K "), _T("WINDOWS "));
				lRet.Replace (_T("WINDOWSXP "), _T("WINDOWS "));
				lRet.Replace (_T("WINDOWSWHISTLER "), _T("WINDOWS "));
				lRet.Replace (_T("WINDOWS"), _T("WINDOWS "));
				lRet.Replace (_T(" WORKSTATION "), _T(" "));
				lRet.Replace (_T(" SERVER "), _T(" "));
				lRet.Replace (_T(" HOME "), _T(" "));
				lRet.Replace (_T(" PROFESSIONAL "), _T(" "));
				lRet.Replace (_T(" PERSONAL "), _T(" "));
				lRet.Replace (_T(" EDITION "), _T(" "));
			}
			lRet.Replace (_T("MICROSOFT WINDOWS"), _T("MICROSOFTWINDOWS"));
			lRet.Replace (_T(" OPERATING "), _T(" "));
			lRet.Replace (_T(" SYSTEM "), _T(" "));

			RemoveTrailingPunctuation (lRet);
			lRet += _T(' ');
			lRet.Replace (_T("WINDOWS"), _T("WINDOWS\x1"));
			UnpluralizeWords (lRet);
			lRet.Replace (_T("WINDOWS\x1"), _T("WINDOWS"));
			lRet += _T('\x1');

			lRet.Replace (_T(" INSTALLER "), _T(" INSTALL "));
//			lRet.Replace (_T(" SETUP "), _T(" INSTALL "));
			lRet.Replace (_T(" TECHNOLOGY "), _T(" "));
			lRet.Replace (_T(" SUPPORT "), _T(" "));
			lRet.Replace (_T(" UNICODE "), _T(" "));
			lRet.Replace (_T(" ANSII "), _T(" "));
			lRet.Replace (_T(" EXTENSION "), _T(" "));
			lRet.Replace (_T(" EXE "), _T(" "));
			lRet.Replace (_T(" EXECUTABLE "), _T(" "));
			lRet.Replace (_T(" DLL "), _T(" "));
			lRet.Replace (_T(" FOR "), _T(" "));
			lRet.Replace (_T(" AND "), _T(" "));
			lRet.Replace (_T(" TO "), _T(" "));
			lRet.Replace (_T(" 16 "), _T(" "));
			lRet.Replace (_T(" 32 "), _T(" "));

			if	(pStrength > 1)
			{
				while	(
							(lRet.Replace (_T(" LIBRARY \x1"), _T(" \x1")))
						||	(lRet.Replace (_T(" MODULE \x1"), _T(" \x1")))
						||	(lRet.Replace (_T(" OBJECT \x1"), _T(" \x1")))
						||	(lRet.Replace (_T(" RUNTIME \x1"), _T(" \x1")))
						||	(lRet.Replace (_T(" APPLICATION \x1"), _T(" \x1")))
						||	(lRet.Replace (_T(" TOOLKIT \x1"), _T(" \x1")))
						||	(lRet.Replace (_T(" TOOL \x1"), _T(" \x1")))
						||	(lRet.Replace (_T(" SOFTWARE \x1"), _T(" \x1")))
						||	(lRet.Replace (_T(" FREEWARE \x1"), _T(" \x1")))
						||	(lRet.Replace (_T(" SHAREWARE \x1"), _T(" \x1")))
						||	(lRet.Replace (_T(" BETA \x1"), _T(" \x1")))
						||	(lRet.Replace (_T(" MFC \x1"), _T(" \x1")))
						||	(lRet.Replace (_T(" EDITION \x1"), _T(" \x1")))
						||	(lRet.Replace (_T(" PERSONAL \x1"), _T(" \x1")))
						||	(lRet.Replace (_T(" PROFESSIONAL \x1"), _T(" \x1")))
						||	(lRet.Replace (_T(" XP \x1"), _T(" \x1")))
						)
				{}
			}

			lRet.TrimRight (_T(" \x1"));
			lRet.TrimLeft ();

			if	(lRet != lOriginal)
			{
				while	(
							(RemoveTrailingParentheses (lRet))
						||	(RemoveTrailingBrackets (lRet))
						)
				{
					RemoveTrailingPunctuation (lRet);
					lRet.TrimRight ();
				}
			}

			while (lRet.Replace (_T(" "), _T(""))) {}
		}
		else
		{
			lRet.TrimLeft ();
			lRet.TrimRight ();

			while (lRet.Replace (_T("  "), _T(" "))) {}
		}
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////

CString	CFileVersionEx::MassageCompanyName (const CString& pCompanyName, UINT pStrength)
{
	CString	lRet (CFileVersionEx::MassageName (pCompanyName, true));

	lRet.TrimLeft ();
	lRet.TrimRight ();

	if	(!lRet.IsEmpty ())
	{
		while	(
					(RemoveTrailingParentheses (lRet))
				||	(RemoveTrailingBrackets (lRet))
				)
		{}

		if	(pStrength > 0)
		{
			CharUpperBuff (lRet.GetBuffer (lRet.GetLength ()), lRet.GetLength ());
			lRet.ReleaseBuffer ();
			lRet.TrimRight ();

			CString	lOriginal (lRet);

			lRet.Remove (_T('®'));
			lRet.Remove (_T('\x99'));
			lRet.Remove (_T('©'));
			lRet.Remove (_T('\''));
			lRet.Replace (_T('\"'), _T(' '));
			lRet.Replace (_T('.'), _T(' '));
			lRet.Replace (_T(','), _T(' '));
			lRet.Replace (_T(';'), _T(' '));
			lRet.Replace (_T(':'), _T(' '));
			lRet.Replace (_T('-'), _T(' '));
			lRet.Replace (_T('_'), _T(' '));
			lRet.Replace (_T('?'), _T(' '));
			lRet.Replace (_T('/'), _T(' '));
			lRet.Replace (_T('\\'), _T(' '));
			lRet.Replace (_T('('), _T(' '));
			lRet.Replace (_T(')'), _T(' '));
			lRet.Replace (_T('|'), _T(' '));

			lRet.TrimRight ();
			lRet += _T('\x1');
			lRet.Replace (_T(" CORP\x1"), _T(" COMPANY\x1"));
			lRet.Replace (_T(" CORPORATION"), _T(" COMPANY"));
			lRet.Replace (_T(" INC\x1"), _T(" COMPANY\x1"));
			lRet.Replace (_T(" INCORPORATED"), _T(" COMPANY"));
			lRet.Replace (_T(" CO\x1"), _T(" COMPANY\x1"));
			lRet.Replace (_T(" LTD\x1"), _T(" COMPANY\x1"));
			lRet.Replace (_T(" LIMITED\x1"), _T(" COMPANY\x1"));
			lRet.Replace (_T(" GMBH\x1"), _T(" COMPANY\x1"));
			lRet.Replace (_T(" LLC\x1"), _T(" COMPANY\x1"));
			lRet.Replace (_T('\x1'), _T(' '));

			RemoveTrailingPunctuation (lRet);
			lRet += _T(' ');
			UnpluralizeWords (lRet);

			lRet.Replace (_T(" COMPANY "), _T(" "));
			lRet.Replace (_T(" HTTP://"), _T(""));

			if	(pStrength > 1)
			{
				lRet.Replace (_T(" TECHNOLOGY "), _T(" "));
				lRet.Replace (_T(" SOFTWARE "), _T(" "));
				lRet.Replace (_T(" SYSTEM "), _T(" "));
				lRet.Replace (_T(" SERVICE "), _T(" "));
				lRet.Replace (_T(" COMPUTER "), _T(" "));
				lRet.Replace (_T(" GROUP "), _T(" "));
			}

			RemoveTrailingPunctuation (lRet);
			lRet.TrimLeft ();
			lRet.TrimRight ();

			if	(lRet != lOriginal)
			{
				while	(
							(RemoveTrailingParentheses (lRet))
						||	(RemoveTrailingBrackets (lRet))
						)
				{
					RemoveTrailingPunctuation (lRet);
					lRet.TrimRight ();
				}
			}

			while (lRet.Replace (_T(" "), _T(""))) {}
		}
		else
		{
			lRet.Replace (_T(" Corporation."), _T(" Corporation"));
			lRet.Replace (_T(" Incorporated."), _T(" Incorporated"));
			lRet.Replace (_T(" Company."), _T(" Company"));
		}
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CFileVersionEx::OneCodePageValid () const
{
	bool				lRet = (mLocales.GetCount() <= 0);
	int 				lNdx;
	CVersionLocale *	lLocale;

	for (lNdx = 0; lNdx < (int)mLocales.GetCount(); lNdx++)
	{
		lLocale = mLocales [lNdx];

		if	(
				(lLocale->mCodePage == CP_WINANSI)
			||	(lLocale->mCodePage == CP_WINUNICODE)
			||	(IsValidCodePage (lLocale->mCodePage))
			)
		{
			lRet = true;
			break;
		}
		else
		{
			LogWinErr (LogIfActive, GetLastError ());
		}
	}
	return lRet;
}

bool CFileVersionEx::AllCodePagesValid () const
{
	bool				lRet = true;
	int 				lNdx;
	CVersionLocale *	lLocale;

	for (lNdx = 0; lNdx < (int)mLocales.GetCount(); lNdx++)
	{
		lLocale = mLocales [lNdx];

		if	(!IsValidCodePage (lLocale->mCodePage))
		{
			lRet = false;
			break;
		}
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

LPCTSTR  CFileVersionEx::mFileVersionName = _T("FileVersion");
LPCTSTR  CFileVersionEx::mProductVersionName = _T("ProductVersion");
LPCTSTR  CFileVersionEx::mCompanyNameName = _T("CompanyName");
LPCTSTR  CFileVersionEx::mCompanyNameAlt [2] = {_T("Company"), _T("Company Name")};
LPCTSTR  CFileVersionEx::mProductNameName = _T("ProductName");
LPCTSTR  CFileVersionEx::mProductNameAlt [2] = {_T("Product"), _T("Product Name")};
LPCTSTR  CFileVersionEx::mFileDescriptionName = _T("FileDescription");
LPCTSTR  CFileVersionEx::mCopyrightName = _T("LegalCopyright");
LPCTSTR  CFileVersionEx::mTrademarkName = _T("LegalTrademarks");
LPCTSTR  CFileVersionEx::mPrivateBuildName = _T("PrivateBuild");
LPCTSTR  CFileVersionEx::mSpecialBuildName = _T("SpecialBuild");

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////


CFileVersionEx::CVersionStrings::CVersionStrings (LPCTSTR pKey)
{
	if	(_tcslen (pKey) == 8)
	{
		mLanguage = (WORD) _tcstoul (CString (_T("0x")) + CString (pKey).Left (4), NULL, 16);
		mCodePage = (WORD) _tcstoul (CString (_T("0x")) + CString (pKey).Mid (4, 4), NULL, 16);
	}

	int 	lSize;
	CString lValue;

	if	(
			(lSize = GetLocaleInfo (GetLCID (), LOCALE_SENGLANGUAGE, NULL, 0))
		&&	(GetLocaleInfo (GetLCID (), LOCALE_SENGLANGUAGE, lValue.GetBuffer (lSize), lSize))
		)
	{
		mLanguageName = (LPCTSTR) lValue;
	}
	lValue.ReleaseBuffer ();

	if	(
			(lSize = GetLocaleInfo (GetLCID (), LOCALE_SENGCOUNTRY, NULL, 0))
		&&	(GetLocaleInfo (GetLCID (), LOCALE_SENGCOUNTRY, lValue.GetBuffer (lSize), lSize))
		)
	{
		mCountryName = (LPCTSTR) lValue;
	}
	lValue.ReleaseBuffer ();
}

CFileVersionEx::CVersionStrings::CVersionStrings (const CVersionStrings& pSource)
:	mLanguage (pSource.mLanguage),
	mCodePage (pSource.mCodePage),
	mLanguageName (pSource.mLanguageName),
	mCountryName (pSource.mCountryName)
{
	Append (pSource);
}

CFileVersionEx::CVersionString* CFileVersionEx::CVersionStrings::Find (const CString& pName) const
{
	int					lNdx;
	CVersionString*	lString;

	for	(lNdx = 0; lNdx < (int)GetCount(); lNdx++)
	{
		lString = &(const_cast <CVersionStrings *> (this)->operator[] (lNdx));

		if	(lString->mName.CompareNoCase (pName) == 0)
		{
			return lString;
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CFileVersionEx::CVersionLocale::CVersionLocale (DWORD pLocale)
{
	mLanguage = LOWORD (pLocale);
	mCodePage = HIWORD (pLocale);

	int 	lSize;
	CString lValue;

	if	(
			(lSize = GetLocaleInfo (GetLCID (), LOCALE_SENGLANGUAGE, NULL, 0))
		&&	(GetLocaleInfo (GetLCID (), LOCALE_SENGLANGUAGE, lValue.GetBuffer (lSize), lSize))
		)
	{
		mLanguageName = (LPCTSTR) lValue;
	}
	lValue.ReleaseBuffer ();

	if	(
			(lSize = GetLocaleInfo (GetLCID (), LOCALE_SENGCOUNTRY, NULL, 0))
		&&	(GetLocaleInfo (GetLCID (), LOCALE_SENGCOUNTRY, lValue.GetBuffer (lSize), lSize))
		)
	{
		mCountryName = (LPCTSTR) lValue;
	}
	lValue.ReleaseBuffer ();
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CFileVersionEx::CVersionStrings * CFileVersionEx::CVersionStringsList::FindLang (WORD pLanguage) const
{
	int 				lNdx;
	CVersionStrings *	lStrings;

	for (lNdx = 0; lNdx < (int)GetCount(); lNdx++)
	{
		lStrings = GetAt (lNdx);
		if	(lStrings->mLanguage == pLanguage)
		{
			return lStrings;
		}
	}
	return NULL;
}

CFileVersionEx::CVersionString* CFileVersionEx::CVersionStringsList::Find (const CString& pName) const
{
	int 				lNdx;
	CVersionStrings *	lStrings;
	CVersionString*	lRet = NULL;

	for (lNdx = 0; lNdx < (int)GetCount(); lNdx++)
	{
		lStrings = GetAt (lNdx);
		if	(lRet = lStrings->Find (pName))
		{
			break;
		}
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CFileVersionEx::CVersionLocale * CFileVersionEx::CVersionLocaleList::FindLang (WORD pLanguage) const
{
	int 				lNdx;
	CVersionLocale *	lLocale;

	for (lNdx = 0; lNdx < (int)GetCount(); lNdx++)
	{
		lLocale = GetAt (lNdx);
		if	(lLocale->mLanguage == pLanguage)
		{
			return lLocale;
		}
	}
	return NULL;
}
