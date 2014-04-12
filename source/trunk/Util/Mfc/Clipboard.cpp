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
#include "StdAfx.h"
#include <comdef.h>
#include <shlwapi.h>
#if	(_MFC_VER < 0x0800)
#if (_MSC_VER >= 1400)
#undef	_PROPSHEETHEADERA
#define	_PROPSHEETHEADERA  _PROPSHEETHEADERA_V2
#undef	_PROPSHEETHEADERW
#define	_PROPSHEETHEADERW  _PROPSHEETHEADERW_V2
#endif
#endif
#include "Clipboard.h"
#include "ItemIdList.h"
#include "MallocPtr.h"
#include "AfxTemplEx.h"
#include "StringArrayEx.h"
#include "Log.h"

#pragma comment(lib, "shlwapi.lib")

#ifndef	CF_EMBEDDEDOBJECT
#define CF_EMBEDDEDOBJECT   TEXT("Embedded Object")
#endif

#ifdef	_DEBUG
//#define	_DEBUG_SHELLIDLIST	LogNormal
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////

CClipboard::CClipboard ()
{
}

CClipboard::~CClipboard ()
{
}

void CClipboard::Empty ()
{
	COleDataSource *	lDataSource;
	if	(lDataSource = COleDataSource::GetClipboardOwner ())
	{
		lDataSource->Empty ();
	}
	::EmptyClipboard ();
}

void CClipboard::Flush ()
{
	COleDataSource *	lDataSource;
	if	(lDataSource = COleDataSource::GetClipboardOwner ())
	{
		lDataSource->ExternalAddRef ();
		lDataSource->SetClipboard ();
	}
	COleDataSource::FlushClipboard ();
}

//////////////////////////////////////////////////////////////////////

bool CClipboard::HasText ()
{
	bool			lRet = false;
	COleDataObject	lDataObject;

	try
	{
		if	(lDataObject.AttachClipboard ())
		{
			lRet = HasText (lDataObject);
		}
	}
	catch AnyException

	lDataObject.Release ();

	return lRet;
}

bool CClipboard::HasText (COleDataObject& pDataObject)
{
	bool	lRet = false;

	try
	{
#ifdef	_UNICODE
		if	(pDataObject.IsDataAvailable (CF_UNICODETEXT))
		{
			lRet = true;
		}
#else
		if	(pDataObject.IsDataAvailable (CF_TEXT))
		{
			lRet = true;
		}
#endif
	}
	catch AnyException

	return lRet;
}

bool CClipboard::PutText (LPCTSTR pText, bool pAppend, bool pFlush)
{
	bool				lRet = false;
	bool				lAppended = false;
	COleDataSource *	lDataSource;
	HGLOBAL				lTextHandle;
	LPTSTR				lTextPtr;

	try
	{
		if	(
				(pText)
			&&	(lTextHandle = GlobalAlloc (GHND | GMEM_SHARE, (_tcslen (pText) + 1) * sizeof (TCHAR)))
			)
		{
			if	(lTextPtr = (LPTSTR) GlobalLock (lTextHandle))
			{
				try
				{
					_tcscpy (lTextPtr, pText);
					if	(
							(
								(pAppend)
							&&	(lDataSource = COleDataSource::GetClipboardOwner ())
							&&	(lAppended = true)
							)
						||	(lDataSource = new COleDataSource)
						)
					{
						GlobalUnlock (lTextHandle);
#ifdef	_UNICODE
						lDataSource->CacheGlobalData (CF_UNICODETEXT, lTextHandle);
#else
						lDataSource->CacheGlobalData (CF_TEXT, lTextHandle);
#endif
						if	(lAppended)
						{
							lDataSource->ExternalAddRef ();
						}
						lDataSource->SetClipboard ();
						if	(pFlush)
						{
							lDataSource->FlushClipboard ();
						}
						lRet = true;
					}
				}
				catch AnyException
			}

			if	(!lRet)
			{
				GlobalFree (lTextHandle);
			}
		}
	}
	catch AnyException

	return lRet;
}

bool CClipboard::GetText (CString& pText)
{
	bool			lRet = false;
	COleDataObject	lDataObject;

	try
	{
		if	(lDataObject.AttachClipboard ())
		{
			lRet = GetText (pText, lDataObject);
		}
	}
	catch AnyException

	lDataObject.Release ();

	return lRet;
}

bool CClipboard::GetText (CString& pText, COleDataObject& pDataObject)
{
	bool	lRet = false;

	try
	{
		HGLOBAL	lMemHandle;
		LPCSTR	lTextPtrA;
		LPCWSTR	lTextPtrW;

#ifdef	_UNICODE
		if	(
				(pDataObject.IsDataAvailable (CF_UNICODETEXT))
			&&	(lMemHandle = pDataObject.GetGlobalData (CF_UNICODETEXT))
			)
		{
			if	(lTextPtrW = (LPCWSTR) GlobalLock (lMemHandle))
			{
				pText = lTextPtrW;
				lRet = true;
				GlobalUnlock (lMemHandle);
			}
			GlobalFree (lMemHandle);
		}
		else
		if	(
				(pDataObject.IsDataAvailable (CF_TEXT))
			&&	(lMemHandle = pDataObject.GetGlobalData (CF_TEXT))
			)
		{
			if	(lTextPtrA = (LPCSTR) GlobalLock (lMemHandle))
			{
				pText = lTextPtrA;
				lRet = true;
				GlobalUnlock (lMemHandle);
			}
			GlobalFree (lMemHandle);
		}
#else
		if	(
				(pDataObject.IsDataAvailable (CF_TEXT))
			&&	(lMemHandle = pDataObject.GetGlobalData (CF_TEXT))
			)
		{
			if	(lTextPtrA = (LPCSTR) GlobalLock (lMemHandle))
			{
				pText = lTextPtrA;
				lRet = true;
				GlobalUnlock (lMemHandle);
			}
			GlobalFree (lMemHandle);
		}
		else
		if	(
				(pDataObject.IsDataAvailable (CF_UNICODETEXT))
			&&	(lMemHandle = pDataObject.GetGlobalData (CF_UNICODETEXT))
			)
		{
			if	(lTextPtrW = (LPCWSTR) GlobalLock (lMemHandle))
			{
				pText = lTextPtrW;
				lRet = true;
				GlobalUnlock (lMemHandle);
			}
			GlobalFree (lMemHandle);
		}
#endif
	}
	catch AnyException

	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CClipboard::PutFile (const CString& pFile, bool pAppend, bool pFlush, UINT pFormatId)
{
	CStringArray	lFiles;

	lFiles.Add (pFile);
	return PutFiles (lFiles, pAppend, pFlush, pFormatId);
}

bool CClipboard::PutFiles (const CStringArray& pFiles, bool pAppend, bool pFlush, UINT pFormatId)
{
	bool					lRet = false;
	bool					lAppended = false;
	tPtr <COleDataSource>	lDataSource;

	try
	{
		if	(
				(
					(pAppend)
				&&	(lDataSource = COleDataSource::GetClipboardOwner ())
				&&	(lAppended = true)
				)
			||	(lDataSource = new COleDataSource)
			)
		{
			if	(PutFiles (pFiles, *lDataSource, pFormatId))
			{
				if	(lAppended)
				{
					lDataSource->ExternalAddRef ();
				}
				lDataSource->SetClipboard ();
				if	(pFlush)
				{
					lDataSource->FlushClipboard ();
				}
				lDataSource.Detach ();
				lRet = true;
			}
			else
			if	(lAppended)
			{
				lDataSource.Detach ();
			}
		}
	}
	catch AnyException

	return lRet;
}

//////////////////////////////////////////////////////////////////////

bool CClipboard::PutFile (const CString& pFile, COleDataSource& pDataSource, UINT pFormatId)
{
	CStringArray	lFiles;

	lFiles.Add (pFile);
	return PutFiles (lFiles, pDataSource, pFormatId);
}

bool CClipboard::PutFiles (const CStringArray& pFiles, COleDataSource& pDataSource, UINT pFormatId)
{
	bool				lRet = false;
	int					lNdx;
	UINT_PTR			lDataSize = 0;
	HGLOBAL				lDataHandle;
	DROPFILES *			lDataPtr;
	LPTSTR				lFileName;

	if	(pFormatId == 0)
	{
		pFormatId = CF_HDROP;
	}

	for	(lNdx = 0; lNdx <= pFiles.GetUpperBound(); lNdx++)
	{
		lDataSize += pFiles [lNdx].GetLength ();
	}

	if	(lDataSize)
	{
		lDataSize += pFiles.GetSize () + 1;
		lDataSize *= sizeof (TCHAR);
		lDataSize += sizeof (DROPFILES);
	}

	try
	{
		if	(
				(lDataSize)
			&&	(lDataHandle = GlobalAlloc (GHND | GMEM_SHARE, lDataSize))
			)
		{
			if	(lDataPtr = (DROPFILES *) GlobalLock (lDataHandle))
			{
				try
				{
					memset (lDataPtr, 0, lDataSize);
					lDataPtr->pFiles = sizeof (DROPFILES);
#ifdef	_UNICODE
					lDataPtr->fWide = TRUE;
#endif
					lFileName = (LPTSTR) ((LPBYTE) lDataPtr + sizeof (DROPFILES));

					for	(lNdx = 0; lNdx <= pFiles.GetUpperBound(); lNdx++)
					{
						if	(!pFiles [lNdx].IsEmpty ())
						{
							_tcscpy (lFileName, pFiles [lNdx]);
							lFileName += pFiles [lNdx].GetLength () + 1;
						}
					}

					GlobalUnlock (lDataHandle);
					pDataSource.CacheGlobalData (pFormatId, lDataHandle);
					lRet = true;
				}
				catch AnyException
			}

			if	(!lRet)
			{
				GlobalFree (lDataHandle);
			}
		}
	}
	catch AnyException

	return lRet;
}

//////////////////////////////////////////////////////////////////////

bool CClipboard::HasFiles (UINT pFormatId)
{
	bool			lRet = false;
	COleDataObject	lDataObject;

	try
	{
		if	(
				(lDataObject.AttachClipboard ())
			&&	(HasFiles (lDataObject, pFormatId))
			)
		{
			lRet = true;
		}
	}
	catch AnyException

	lDataObject.Release ();

	return lRet;
}

bool CClipboard::GetFiles (CStringArray& pFiles, UINT pFormatId)
{
	bool			lRet = false;
	COleDataObject	lDataObject;

	try
	{
		if	(lDataObject.AttachClipboard ())
		{
			lRet = GetFiles (pFiles, lDataObject);
		}
	}
	catch AnyException

	lDataObject.Release ();

	return lRet;
}

bool CClipboard::HasFiles (COleDataObject& pDataObject, UINT pFormatId)
{
	bool	lRet = false;

	if	(pFormatId == 0)
	{
		pFormatId = CF_HDROP;
	}

	try
	{
		if	(pDataObject.IsDataAvailable (pFormatId))
		{
			lRet = true;
		}
	}
	catch AnyException

	return lRet;
}

bool CClipboard::GetFiles (CStringArray& pFiles, COleDataObject& pDataObject, UINT pFormatId)
{
	bool	lRet = false;
	HGLOBAL	lDataHandle;
	HDROP	lDropData;

	if	(pFormatId == 0)
	{
		pFormatId = CF_HDROP;
	}

	try
	{
		if	(
				(pDataObject.IsDataAvailable (pFormatId))
			&&	(lDataHandle = pDataObject.GetGlobalData (pFormatId))
			)
		{
			try
			{
				if	(lDropData = (HDROP) GlobalLock (lDataHandle))
				{
					UINT	lFileCount = DragQueryFile (lDropData, -1, NULL, 0);
					UINT	lFileNdx;
					CString	lFileName;

					for	(lFileNdx = 0; lFileNdx < lFileCount; lFileNdx++)
					{
						if	(DragQueryFile (lDropData, lFileNdx, lFileName.GetBuffer (MAX_PATH), MAX_PATH))
						{
							lFileName.ReleaseBuffer ();
							pFiles.Add (lFileName);
							lRet = true;
						}
						else
						{
							lFileName.ReleaseBuffer ();
						}
					}
					GlobalUnlock (lDataHandle);
				}
			}
			catch AnyException

			GlobalFree (lDataHandle);
		}
	}
	catch AnyException

	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////
#ifdef	_DEBUG_SHELLIDLIST

static void LogCIDA (UINT pLogLevel, LPBYTE pCida, LPCTSTR pTitle = NULL)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString		lTitle (pTitle);
			CIDA *		lCida = (CIDA *) pCida;
			UINT		lNdx;

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("CIDA");
			}

			LogMessage (pLogLevel, _T("%s [%p] [%u]"), lTitle, pCida, lCida->cidl);

			for	(lNdx = 0; lNdx <= lCida->cidl; lNdx++)
			{
				try
				{
					lTitle.Format (_T("[%u] Offset [%u]"), lNdx, lCida->aoffset [lNdx]);
					LogIdList (pLogLevel, (LPCITEMIDLIST) (((LPBYTE) pCida) + lCida->aoffset [lNdx]), lTitle);
				}
				catch AnyExceptionSilent
			}

			UINT	lSize = sizeof (lCida->cidl) + (sizeof (lCida->aoffset [0]) * (lCida->cidl+1));
			UINT	lIdlSize = 0;

			for	(lNdx = 0; lNdx <= lCida->cidl; lNdx++)
			{
				lIdlSize += CItemIdList::IdlSize ((LPCITEMIDLIST) (((LPBYTE) pCida) + lCida->aoffset [lNdx]));
			}

			LogMessage (pLogLevel, _T("Base [%u] Idls [%u] Total [%u]"), lSize, lIdlSize, lSize+lIdlSize);
		}
		catch AnyExceptionSilent
	}
}

#endif
//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CClipboard::PutIdl (LPCITEMIDLIST pIdl, bool pAppend, bool pFlush, UINT pFormatId)
{
	bool					lRet = false;
	bool					lAppended = false;
	tPtr <COleDataSource>	lDataSource;

	try
	{
		if	(
				(
					(pAppend)
				&&	(lDataSource = COleDataSource::GetClipboardOwner ())
				&&	(lAppended = true)
				)
			||	(lDataSource = new COleDataSource)
			)
		{
			if	(PutIdl (pIdl, *lDataSource, pFormatId))
			{
				if	(lAppended)
				{
					lDataSource->ExternalAddRef ();
				}
				lDataSource->SetClipboard ();
				if	(pFlush)
				{
					lDataSource->FlushClipboard ();
				}
				lDataSource.Detach ();
				lRet = true;
			}
			else
			if	(lAppended)
			{
				lDataSource.Detach ();
			}
		}
	}
	catch AnyException

	return lRet;
}

bool CClipboard::PutIdls (LPCITEMIDLIST * pIdls, UINT pIdlCount, bool pAppend, bool pFlush, UINT pFormatId)
{
	bool					lRet = false;
	bool					lAppended = false;
	tPtr <COleDataSource>	lDataSource;

	try
	{
		if	(
				(
					(pAppend)
				&&	(lDataSource = COleDataSource::GetClipboardOwner ())
				&&	(lAppended = true)
				)
			||	(lDataSource = new COleDataSource)
			)
		{
			if	(PutIdls (pIdls, pIdlCount, *lDataSource, pFormatId))
			{
				if	(lAppended)
				{
					lDataSource->ExternalAddRef ();
				}
				lDataSource->SetClipboard ();
				if	(pFlush)
				{
					lDataSource->FlushClipboard ();
				}
				lDataSource.Detach ();
				lRet = true;
			}
			else
			if	(lAppended)
			{
				lDataSource.Detach ();
			}
		}
	}
	catch AnyException

	return lRet;
}

bool CClipboard::PutFileIdl (const CString& pFile, bool pAppend, bool pFlush, UINT pFormatId)
{
	bool					lRet = false;
	bool					lAppended = false;
	tPtr <COleDataSource>	lDataSource;

	try
	{
		if	(
				(
					(pAppend)
				&&	(lDataSource = COleDataSource::GetClipboardOwner ())
				&&	(lAppended = true)
				)
			||	(lDataSource = new COleDataSource)
			)
		{
			if	(PutFileIdl (pFile, *lDataSource, pFormatId))
			{
				if	(lAppended)
				{
					lDataSource->ExternalAddRef ();
				}
				lDataSource->SetClipboard ();
				if	(pFlush)
				{
					lDataSource->FlushClipboard ();
				}
				lDataSource.Detach ();
				lRet = true;
			}
			else
			if	(lAppended)
			{
				lDataSource.Detach ();
			}
		}
	}
	catch AnyException

	return lRet;
}

bool CClipboard::PutFileIdls (const CStringArray& pFiles, bool pAppend, bool pFlush, UINT pFormatId)
{
	bool					lRet = false;
	bool					lAppended = false;
	tPtr <COleDataSource>	lDataSource;

	try
	{
		if	(
				(
					(pAppend)
				&&	(lDataSource = COleDataSource::GetClipboardOwner ())
				&&	(lAppended = true)
				)
			||	(lDataSource = new COleDataSource)
			)
		{
			if	(PutFileIdls (pFiles, *lDataSource, pFormatId))
			{
				if	(lAppended)
				{
					lDataSource->ExternalAddRef ();
				}
				lDataSource->SetClipboard ();
				if	(pFlush)
				{
					lDataSource->FlushClipboard ();
				}
				lDataSource.Detach ();
				lRet = true;
			}
			else
			if	(lAppended)
			{
				lDataSource.Detach ();
			}
		}
	}
	catch AnyException

	return lRet;
}

//////////////////////////////////////////////////////////////////////

bool CClipboard::PutIdl (LPCITEMIDLIST pIdl, COleDataSource& pDataSource, UINT pFormatId)
{
	bool		lRet = false;
	IMallocPtr	lMalloc;

	if	(pFormatId == 0)
	{
		pFormatId =	RegisterClipboardFormat (CFSTR_SHELLIDLIST);
	}

	if	(SUCCEEDED (CoGetMalloc (1, &lMalloc)))
	{
		UINT			lDataSize = 0;
		HGLOBAL			lDataHandle;
		LPBYTE			lDataPtr;
		UINT			lDataOffset;
		CItemIdList		lParentIdList (pIdl, lMalloc);
		CItemIdList		lIdList (NULL, lMalloc);
		LPITEMIDLIST	lIdl = lParentIdList;
		LPITEMIDLIST	lNextIdl;

		while (lIdl->mkid.cb)
		{
			lNextIdl = (LPITEMIDLIST) (((LPBYTE) lIdl) + lIdl->mkid.cb);
			if	(lNextIdl->mkid.cb == 0)
			{
				lIdList = lIdl;
				lIdl->mkid.cb = 0;
			}
			lIdl = lNextIdl;
		}
		if	(!lIdList)
		{
			lIdList.Attach (lParentIdList.Detach ());
		}

		lDataSize = sizeof (CIDA) + sizeof (UINT);
		MemAlign (lDataSize);

		if	(lParentIdList)
		{
			lDataSize += lParentIdList.Size ();
		}
		else
		{
			lDataSize += sizeof (USHORT);
		}
		MemAlign (lDataSize);
		lDataSize += lIdList.Size ();
		MemAlign (lDataSize);

		try
		{
			if	(
					(lDataSize)
				&&	(lDataHandle = GlobalAlloc (GHND | GMEM_SHARE, lDataSize))
				)
			{
				if	(lDataPtr = (LPBYTE) GlobalLock (lDataHandle))
				{
					try
					{
						((CIDA *) lDataPtr)->cidl = 1;
						lDataOffset = sizeof (CIDA) + sizeof (UINT);
						MemAlign (lDataOffset);
						((CIDA *) lDataPtr)->aoffset [0] = lDataOffset;

						if	(lParentIdList)
						{
							memcpy (lDataPtr + lDataOffset, (LPBYTE) (LPITEMIDLIST) lParentIdList, lParentIdList.Size ());
							lDataOffset += lParentIdList.Size ();
						}
						else
						{
							lDataOffset += sizeof (USHORT);
						}
						MemAlign (lDataOffset);
						((CIDA *) lDataPtr)->aoffset [1] = lDataOffset;
						memcpy (lDataPtr + lDataOffset, (LPBYTE) (LPITEMIDLIST) lIdList, lIdList.Size ());

#ifdef	_DEBUG_SHELLIDLIST
						LogMessage (_DEBUG_SHELLIDLIST|LogNoPrefix, _T("Clipboard CFSTR_SHELLIDLIST (a) [%p] [%u]"), lDataPtr, lDataSize);
						LogDump (_DEBUG_SHELLIDLIST|LogNoPrefix|LogHighVolume, lDataPtr, lDataSize, _T("  "));
#endif
						GlobalUnlock (lDataHandle);
						pDataSource.CacheGlobalData (pFormatId, lDataHandle);
						lRet = true;
					}
					catch AnyException
				}

				if	(!lRet)
				{
					GlobalFree (lDataHandle);
				}
			}
		}
		catch AnyException
	}

	return lRet;
}

bool CClipboard::PutIdls (LPCITEMIDLIST * pIdls, UINT pIdlCount, COleDataSource& pDataSource, UINT pFormatId)
{
	if	(pIdlCount == 1)
	{
		return PutIdl (pIdls [0], pDataSource, pFormatId);
	}
	else
	if	(pIdlCount)
	{
		bool		lRet = false;
		IMallocPtr	lMalloc;

		if	(pFormatId == 0)
		{
			pFormatId = RegisterClipboardFormat (CFSTR_SHELLIDLIST);
		}

		if	(SUCCEEDED (CoGetMalloc (1, &lMalloc)))
		{
			UINT						lDataSize = 0;
			HGLOBAL						lDataHandle;
			LPBYTE						lDataPtr;
			UINT						lDataOffset;
			int							lNdx;
			CItemIdList					lParentIdList (NULL, lMalloc);
			int							lParentIdSize;
			CTypeArray <LPCITEMIDLIST>	lIdList;

			for	(lNdx = 0; lNdx < (int) pIdlCount; lNdx++)
			{
				LPCITEMIDLIST	lIdl = pIdls [lNdx];

				if	(lNdx == 0)
				{
					lParentIdList = lIdl;
					ILRemoveLastID (lParentIdList);
				}
				else
				if	(ILIsEqual (lParentIdList, lIdl))
				{
					ILRemoveLastID (lParentIdList);
				}
				else
				if	(!ILIsParent (lParentIdList, lIdl, FALSE))
				{
					while	(
								(lParentIdList)
							&&	(ILRemoveLastID (lParentIdList))
							&&	(lParentIdList->mkid.cb != 0)
							)
					{
						if	(ILIsParent (lParentIdList, lIdl, FALSE))
						{
							break;
						}
					}
				}

				if	(
						(!lParentIdList)
					||	(lParentIdList->mkid.cb == 0)
					)
				{
					lParentIdList.Free ();
					break;
				}
			}

			if	(lParentIdSize = lParentIdList.Size ())
			{
				lParentIdSize -= sizeof (lParentIdList->mkid.cb);
			}

			for	(lNdx = 0; lNdx < (int) pIdlCount; lNdx++)
			{
				LPCITEMIDLIST	lIdl = pIdls [lNdx];

				if	(lParentIdList)
				{
					lIdl = (LPCITEMIDLIST) (((LPBYTE) lIdl) + lParentIdSize);
				}
				lIdList.Add (lIdl);
			}

			if	(lParentIdList)
			{
				lDataSize += lParentIdList.Size ();
			}
			else
			{
				lDataSize += sizeof (USHORT);
			}
			lDataSize += sizeof (CIDA);

			for	(lNdx = 0; lNdx <= lIdList.GetUpperBound(); lNdx++)
			{
				lDataSize += CItemIdList::IdlSize (lIdList [lNdx]);
				lDataSize += sizeof (UINT);
			}

			try
			{
				if	(
						(lDataSize > 0)
					&&	(lIdList.GetSize () > 0)
					&&	(lDataHandle = GlobalAlloc (GHND | GMEM_SHARE, lDataSize))
					)
				{
					if	(lDataPtr = (LPBYTE) GlobalLock (lDataHandle))
					{
						try
						{
							((CIDA *) lDataPtr)->cidl = (UINT) lIdList.GetSize ();
							((CIDA *) lDataPtr)->aoffset [0] = lDataOffset = (UINT) (sizeof (CIDA) + (lIdList.GetSize () * sizeof (UINT)));

							if	(lParentIdList)
							{
								memcpy (lDataPtr + lDataOffset, (LPBYTE) (LPITEMIDLIST) lParentIdList, lParentIdList.Size ());
								lDataOffset += lParentIdList.Size ();
							}
							else
							{
								lDataOffset += sizeof (USHORT);
							}

							for	(lNdx = 0; lNdx <= lIdList.GetUpperBound(); lNdx++)
							{
								((CIDA *) lDataPtr)->aoffset [lNdx+1] = (UINT) lDataOffset;
								memcpy (lDataPtr + lDataOffset, (LPBYTE) lIdList [lNdx], CItemIdList::IdlSize (lIdList [lNdx]));
								lDataOffset += CItemIdList::IdlSize (lIdList [lNdx]);
							}

#ifdef	_DEBUG_SHELLIDLIST
							LogMessage (_DEBUG_SHELLIDLIST|LogNoPrefix, _T("Clipboard CFSTR_SHELLIDLIST (b) [%p] [%u]"), lDataPtr, lDataSize);
							LogDump (_DEBUG_SHELLIDLIST|LogNoPrefix|LogHighVolume, lDataPtr, lDataSize, _T("  "));
#endif
							GlobalUnlock (lDataHandle);
							pDataSource.CacheGlobalData (pFormatId, lDataHandle);
							lRet = true;
						}
						catch AnyException
					}

					if	(!lRet)
					{
						GlobalFree (lDataHandle);
					}
				}
			}
			catch AnyException
		}

		return lRet;
	}
	else
	{
		return false;
	}
}

//////////////////////////////////////////////////////////////////////

bool CClipboard::PutFileIdl (const CString& pFile, COleDataSource& pDataSource, UINT pFormatId)
{
	bool			lRet = false;
	UINT			lDataSize = 0;
	HGLOBAL			lDataHandle;
	LPBYTE			lDataPtr;
	UINT			lDataOffset;
	IMallocPtr		lMalloc;
	IShellFolderPtr	lDesktop;

	if	(pFormatId == 0)
	{
		pFormatId = RegisterClipboardFormat (CFSTR_SHELLIDLIST);
	}

	if	(
			(SUCCEEDED (CoGetMalloc (1, &lMalloc)))
		&&	(SUCCEEDED (SHGetDesktopFolder (&lDesktop)))
		)
	{
		CItemIdList				lParentIdList (NULL, lMalloc);
		CItemIdList				lIdList;
		CString					lPath ((LPCTSTR) pFile);
		CString					lFile ((LPCTSTR) pFile);
		tMallocPtr <OLECHAR>	lDispPath;
		tMallocPtr <OLECHAR>	lDispFile;
		IShellFolderPtr			lParentFolder;

		if	(PathIsRoot (lPath))
		{
			lDispPath = AfxAllocTaskOleString (lPath);
		}
		else
		{
			PathRemoveFileSpec (lPath.GetBuffer (MAX_PATH));
			lPath.ReleaseBuffer ();
			PathStripPath (lFile.GetBuffer (MAX_PATH));
			lFile.ReleaseBuffer ();

			lDispPath = AfxAllocTaskOleString (lPath);
			lDispFile = AfxAllocTaskOleString (lFile);
		}

		if	(
				(lDispFile)
			?	(
					(SUCCEEDED (lDesktop->ParseDisplayName (NULL, NULL, lDispPath, NULL, lParentIdList.Free (), NULL)))
				&&	(SUCCEEDED (lDesktop->BindToObject (lParentIdList, NULL, __uuidof (IShellFolder), (void**) &lParentFolder)))
				&&	(SUCCEEDED (lParentFolder->ParseDisplayName (NULL, NULL, lDispFile, NULL, lIdList.Free (), NULL)))
				)
			:	(SUCCEEDED (lDesktop->ParseDisplayName (NULL, NULL, lDispPath, NULL, lIdList.Free (), NULL)))
			)
		{
			lDataSize = sizeof (CIDA) + sizeof (UINT);
			MemAlign (lDataSize);

			if	(lParentIdList)
			{
				lDataSize += lParentIdList.Size ();
			}
			else
			{
				lDataSize += sizeof (USHORT);
			}
			MemAlign (lDataSize);
			lDataSize += lIdList.Size ();
			MemAlign (lDataSize);
		}

		try
		{
			if	(
					(lDataSize)
				&&	(lDataHandle = GlobalAlloc (GHND | GMEM_SHARE, lDataSize))
				)
			{
				if	(lDataPtr = (LPBYTE) GlobalLock (lDataHandle))
				{
					try
					{
						((CIDA *) lDataPtr)->cidl = 1;
						lDataOffset = sizeof (CIDA) + sizeof (UINT);
						MemAlign (lDataOffset);

						((CIDA *) lDataPtr)->aoffset [0] = lDataOffset;
						if	(lParentIdList)
						{
							memcpy (lDataPtr + lDataOffset, (LPBYTE) (LPITEMIDLIST) lParentIdList, lParentIdList.Size ());
							lDataOffset += lParentIdList.Size ();
						}
						else
						{
							lDataOffset += sizeof (USHORT);
						}
						MemAlign (lDataOffset);

						((CIDA *) lDataPtr)->aoffset [1] = lDataOffset;
						memcpy (lDataPtr + lDataOffset, (LPBYTE) (LPITEMIDLIST) lIdList, lIdList.Size ());

#ifdef	_DEBUG_SHELLIDLIST
						LogMessage (_DEBUG_SHELLIDLIST|LogNoPrefix, _T("Clipboard CFSTR_SHELLIDLIST (c) [%p] [%u] [%u] [%p]"), lDataPtr, lDataSize, GlobalSize (lDataHandle), lDataHandle);
/*
						if	(lParentIdList)
						{
							LogIdList (_DEBUG_SHELLIDLIST|LogNoPrefix, lParentIdList, _T("Parent"));
						}
						LogIdList (_DEBUG_SHELLIDLIST|LogNoPrefix, lIdList, _T("File"));
*/
						LogCIDA (_DEBUG_SHELLIDLIST|LogNoPrefix, lDataPtr);
						//LogDump (_DEBUG_SHELLIDLIST|LogNoPrefix|LogHighVolume, lDataPtr, lDataSize, _T("  "));
#endif
						GlobalUnlock (lDataHandle);
						pDataSource.CacheGlobalData (pFormatId, lDataHandle);
						lRet = true;
					}
					catch AnyException
				}

				if	(!lRet)
				{
					GlobalFree (lDataHandle);
				}
			}
		}
		catch AnyException
	}

	return lRet;
}

bool CClipboard::PutFileIdls (const CStringArray& pFiles, COleDataSource& pDataSource, UINT pFormatId)
{
	if	(pFiles.GetSize () == 1)
	{
		return PutFileIdl (pFiles [0], pDataSource, pFormatId);
	}
	else
	if	(pFiles.GetSize () > 0)
	{
		bool			lRet = false;
		UINT			lDataSize = 0;
		HGLOBAL			lDataHandle;
		LPBYTE			lDataPtr;
		UINT			lDataOffset;
		IMallocPtr		lMalloc;
		IShellFolderPtr	lDesktop;

		if	(pFormatId == 0)
		{
			pFormatId = RegisterClipboardFormat (CFSTR_SHELLIDLIST);
		}

		if	(
				(SUCCEEDED (CoGetMalloc (1, &lMalloc)))
			&&	(SUCCEEDED (SHGetDesktopFolder (&lDesktop)))
			)
		{
			int							lNdx;
			CString						lParentPath;
			CItemIdList					lParentIdList (NULL, lMalloc);
			IShellFolderPtr				lParentFolder;
			CString						lPath;
			tMallocPtr <OLECHAR>		lDispPath;
			COwnPtrArray <CItemIdList>	lIdList;

			for	(lNdx = 0; lNdx <= pFiles.GetUpperBound(); lNdx++)
			{
				if	(PathIsRoot (pFiles [lNdx]))
				{
					lParentPath.Empty ();
					break;
				}

				if	(lNdx == 0)
				{
					lParentPath = pFiles [lNdx];
					PathRemoveFileSpec (lParentPath.GetBuffer (lParentPath.GetLength ()));
					lParentPath.ReleaseBuffer ();
				}
				else
				{
					PathCommonPrefix (lParentPath, pFiles [lNdx], lPath.GetBuffer (MAX_PATH));
					lPath.ReleaseBuffer ();
					lParentPath = lPath;
				}

				if	(lParentPath.IsEmpty ())
				{
					break;
				}
				else
				if	(lParentPath.CompareNoCase (pFiles [lNdx]) == 0)
				{
					PathRemoveFileSpec (lParentPath.GetBuffer (lParentPath.GetLength ()));
					lParentPath.ReleaseBuffer ();
				}
			}

			if	(!lParentPath.IsEmpty ())
			{
				PathRemoveBackslash (lParentPath.GetBuffer (lParentPath.GetLength ()));
				lParentPath.ReleaseBuffer ();
			}

			if	(
					(
						(lParentPath.IsEmpty ())
					&&	((lParentFolder = lDesktop) != NULL)
					)
				||	(
						(SUCCEEDED (lDesktop->ParseDisplayName (NULL, NULL, lDispPath = AfxAllocTaskOleString (lParentPath), NULL, lParentIdList.Free (), NULL)))
					&&	(SUCCEEDED (lDesktop->BindToObject (lParentIdList, NULL, __uuidof (IShellFolder), (void**) &lParentFolder)))
					)
				)
			{
				if	(lParentIdList)
				{
					lDataSize += lParentIdList.Size ();
				}
				else
				{
					lDataSize += sizeof (USHORT);
				}
				lDataSize += sizeof (CIDA);

				for	(lNdx = 0; lNdx <= pFiles.GetUpperBound(); lNdx++)
				{
					if	(
							(!lParentPath.IsEmpty ())
						&&	(PathRelativePathTo (lPath.GetBuffer (MAX_PATH), lParentPath, FILE_ATTRIBUTE_DIRECTORY, pFiles [lNdx], 0))
						)
					{
						lPath.ReleaseBuffer ();
						lPath.Delete (0, 2);
						lDispPath = AfxAllocTaskOleString (lPath);
					}
					else
					{
						lDispPath = AfxAllocTaskOleString (pFiles [lNdx]);
					}

					lIdList.Add (new CItemIdList (NULL, lMalloc));

					if	(SUCCEEDED (lParentFolder->ParseDisplayName (NULL, NULL, lDispPath, NULL, lIdList [lIdList.GetUpperBound()]->Free (), NULL)))
					{
						lDataSize += lIdList [lIdList.GetUpperBound()]->Size ();
						lDataSize += sizeof (UINT);
					}
					else
					{
						lIdList.DeleteAt ((int)lIdList.GetUpperBound());
					}
				}
			}

			try
			{
				if	(
						(lDataSize > 0)
					&&	(lIdList.GetSize () > 0)
					&&	(lDataHandle = GlobalAlloc (GHND | GMEM_SHARE, lDataSize))
					)
				{
					if	(lDataPtr = (LPBYTE) GlobalLock (lDataHandle))
					{
						try
						{
							((CIDA *) lDataPtr)->cidl = (UINT) lIdList.GetSize ();
							((CIDA *) lDataPtr)->aoffset [0] = lDataOffset = (UINT) (sizeof (CIDA) + (lIdList.GetSize () * sizeof (UINT)));

							if	(lParentIdList)
							{
								memcpy (lDataPtr + lDataOffset, (LPBYTE) (LPITEMIDLIST) lParentIdList, lParentIdList.Size ());
								lDataOffset += lParentIdList.Size ();
							}
							else
							{
								lDataOffset += sizeof (USHORT);
							}

							for	(lNdx = 0; lNdx <= lIdList.GetUpperBound(); lNdx++)
							{
								((CIDA *) lDataPtr)->aoffset [lNdx+1] = lDataOffset;
								memcpy (lDataPtr + lDataOffset, (LPBYTE) (LPITEMIDLIST) (*lIdList [lNdx]), lIdList [lNdx]->Size ());
								lDataOffset += lIdList [lNdx]->Size ();
							}

#ifdef	_DEBUG_SHELLIDLIST
							LogMessage (_DEBUG_SHELLIDLIST|LogNoPrefix, _T("Clipboard CFSTR_SHELLIDLIST (d) [%p] [%u]"), lDataPtr, lDataSize);
							LogDump (_DEBUG_SHELLIDLIST|LogNoPrefix|LogHighVolume, lDataPtr, lDataSize, _T("  "));
#endif
							GlobalUnlock (lDataHandle);
							pDataSource.CacheGlobalData (pFormatId, lDataHandle);
							lRet = true;
						}
						catch AnyException
					}

					if	(!lRet)
					{
						GlobalFree (lDataHandle);
					}
				}
			}
			catch AnyException
		}

		return lRet;
	}
	else
	{
		return false;
	}
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CClipboard::HasIdls (UINT pFormatId)
{
	bool			lRet = false;
	COleDataObject	lDataObject;

	try
	{
		if	(
				(lDataObject.AttachClipboard ())
			&&	(HasIdls (lDataObject, pFormatId))
			)
		{
			lRet = true;
		}
	}
	catch AnyException

	lDataObject.Release ();

	return lRet;
}

bool CClipboard::GetIdls (LPITEMIDLIST*& pIdls, int& pIdlCount, UINT pFormatId)
{
	bool			lRet = false;
	COleDataObject	lDataObject;

	try
	{
		if	(lDataObject.AttachClipboard ())
		{
			lRet = GetIdls (pIdls, pIdlCount, lDataObject, pFormatId);
		}
	}
	catch AnyException

	lDataObject.Release ();

	return lRet;
}

bool CClipboard::HasIdls (COleDataObject& pDataObject, UINT pFormatId)
{
	bool	lRet = false;

	try
	{
		if	(pFormatId == 0)
		{
			pFormatId = RegisterClipboardFormat (CFSTR_SHELLIDLIST);
		}

		if	(pDataObject.IsDataAvailable (pFormatId))
		{
			lRet = true;
		}
	}
	catch AnyException

	return lRet;
}

bool CClipboard::GetIdls (LPITEMIDLIST*& pIdls, int& pIdlCount, COleDataObject& pDataObject, UINT pFormatId)
{
	bool	lRet = false;
	HGLOBAL	lDataHandle;
	LPIDA	lDropData;

	pIdls = NULL;
	pIdlCount = 0;

	try
	{
		if	(pFormatId == 0)
		{
			pFormatId = RegisterClipboardFormat (CFSTR_SHELLIDLIST);
		}

		if	(
				(pDataObject.IsDataAvailable (pFormatId))
			&&	(lDataHandle = pDataObject.GetGlobalData (pFormatId))
			)
		{
			try
			{
				if	(lDropData = (LPIDA) GlobalLock (lDataHandle))
				{
#ifdef	_DEBUG_SHELLIDLIST
					LogMessage (_DEBUG_SHELLIDLIST|LogNoPrefix, _T("Data [%p] [%u]"), lDataHandle, GlobalSize (lDataHandle));
					LogCIDA (_DEBUG_SHELLIDLIST|LogNoPrefix, (LPBYTE) lDropData);
#endif
					if	(lDropData->cidl > 0)
					{
						CItemIdList		lParentIdList;
						CItemIdList		lItemIdList;
						UINT			lNdx;
						LPCITEMIDLIST	lIdList = (LPCITEMIDLIST) (((LPBYTE) lDropData) + lDropData->aoffset [0]);

						if	(pIdls = (LPITEMIDLIST *) CoTaskMemAlloc (lDropData->cidl * sizeof (LPITEMIDLIST)))
						{
							memset (pIdls, 0, lDropData->cidl * sizeof (LPITEMIDLIST));
							pIdlCount = lDropData->cidl;

							if	(lIdList->mkid.cb)
							{
								lParentIdList = lIdList;
#ifdef	_DEBUG_SHELLIDLIST
								LogIdList (_DEBUG_SHELLIDLIST|LogNoPrefix, lParentIdList, _T("Parent"));
								LogMessage (_DEBUG_SHELLIDLIST|LogNoPrefix, _T("Parent Folder [%s]"), lParentIdList.Path ());
#endif
							}

							for	(lNdx = 0; lNdx < lDropData->cidl; lNdx++)
							{
								lIdList = (LPCITEMIDLIST) (((LPBYTE) lDropData) + lDropData->aoffset [lNdx+1]);
								if	(lParentIdList)
								{
									lItemIdList = lParentIdList + lIdList;
								}
								else
								{
									lItemIdList = lIdList;
								}
#ifdef	_DEBUG_SHELLIDLIST
								LogIdList (_DEBUG_SHELLIDLIST|LogNoPrefix, lItemIdList, _T("Item"));
								LogMessage (_DEBUG_SHELLIDLIST|LogNoPrefix, _T("Item [%d] [%s]"), lNdx, lItemIdList.Path ());
#endif
								pIdls [lNdx] = lItemIdList.Detach ();
							}

							lRet = true;
						}
					}

					GlobalUnlock (lDataHandle);
				}
			}
			catch AnyException

			GlobalFree (lDataHandle);
		}
	}
	catch AnyException

	return lRet;
}

void CClipboard::ReleaseIdls (LPITEMIDLIST * pIdls, int pIdlCount)
{
	if	(pIdls)
	{
		CItemIdList	lIdList;
		int			lNdx;

		for	(lNdx = 0; lNdx < pIdlCount; lNdx++)
		{
			if	(lIdList.Attach (pIdls [lNdx]))
			{
				lIdList.Free ();
			}
		}

		CoTaskMemFree (pIdls);
	}
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CClipboard::PutStream (IStream * pStream, bool pCopy, bool pRelease, bool pAppend, bool pFlush, UINT pFormatId, int pNdx)
{
	bool				lRet = false;
	bool				lAppended = false;
	COleDataSource *	lDataSource;

	try
	{
		if	(
				(
					(pAppend)
				&&	(lDataSource = COleDataSource::GetClipboardOwner ())
				&&	(lAppended = true)
				)
			||	(lDataSource = new COleDataSource)
			)
		{
			if	(PutStream (pStream, *lDataSource, pCopy, pRelease, pFormatId, pNdx))
			{
				if	(lAppended)
				{
					lDataSource->ExternalAddRef ();
				}
				lDataSource->SetClipboard ();
				if	(pFlush)
				{
					lDataSource->FlushClipboard ();
				}
				lRet = true;
			}
			else
			if	(!lAppended)
			{
				delete lDataSource;
			}
		}
	}
	catch AnyException

	return lRet;
}

bool CClipboard::PutStream (IStream * pStream, COleDataSource& pDataSource, bool pCopy, bool pRelease, UINT pFormatId, int pNdx)
{
	bool				lRet = false;
	tS <STGMEDIUM>		lStgMedium;
	tS <FORMATETC>		lFormatEtc;

	if	(pFormatId == 0)
	{
		pFormatId = RegisterClipboardFormat (CF_EMBEDDEDOBJECT);
	}

	lFormatEtc.cfFormat = pFormatId;
	lFormatEtc.dwAspect = DVASPECT_CONTENT;
	lFormatEtc.tymed = TYMED_ISTREAM;
	lFormatEtc.lindex = pNdx;

	lStgMedium.tymed = TYMED_ISTREAM;
	if	(pCopy)
	{
		CreateStreamOnHGlobal (NULL, TRUE, &lStgMedium.pstm);
	}
	else
	{
		lStgMedium.pstm = pStream;
	}

	try
	{
		if	(
				(pStream)
			&&	(&lStgMedium.pstm)
			)
		{
			LARGE_INTEGER	lStart = {0,0};
			tS <STATSTG>	lStat;

			if	(
					(
						(pCopy)
					?	(
							(SUCCEEDED (pStream->Seek (lStart, STREAM_SEEK_SET, NULL)))
						&&	(SUCCEEDED (pStream->Stat (&lStat, STATFLAG_NONAME)))
						&&	(SUCCEEDED (lStgMedium.pstm->SetSize (lStat.cbSize)))
						&&	(SUCCEEDED (pStream->CopyTo (lStgMedium.pstm, lStat.cbSize, NULL, NULL)))
						)
					:	(SUCCEEDED (lStgMedium.pstm->Stat (&lStat, STATFLAG_NONAME)))
					)
				&&	(SUCCEEDED (lStgMedium.pstm->Seek (lStart, STREAM_SEEK_SET, NULL)))
				)
			{
				LogMessage (LogVerbose|LogTime, _T("PutClipboardStream [Size %I64d]"), lStat.cbSize.QuadPart);

				pDataSource.CacheData (lFormatEtc.cfFormat, &lStgMedium, &lFormatEtc);
				lRet = true;
			}
		}
	}
	catch AnyException

	try
	{
		if	(
				(pCopy)
			&&	(!lRet)
			)
		{
			ReleaseStgMedium (&lStgMedium);
		}
	}
	catch AnyException

	try
	{
		if	(
				(pRelease)
			&&	(pStream)
			)
		{
			pStream->Release ();
		}
	}
	catch AnyException

	return lRet;
}

//////////////////////////////////////////////////////////////////////

bool CClipboard::GetStream (IStream ** pStream, UINT pFormatId, int pNdx)
{
	bool			lRet = false;
	COleDataObject	lDataObject;

	try
	{
		if	(lDataObject.AttachClipboard ())
		{
			lRet = GetStream (pStream, lDataObject, pFormatId, pNdx);
		}
	}
	catch AnyException

	return lRet;
}

bool CClipboard::GetStream (IStream ** pStream, COleDataObject& pDataObject, UINT pFormatId, int pNdx)
{
	bool			lRet = false;
	tS <STGMEDIUM>	lStgMedium;
	tS <FORMATETC>	lFormatEtc;

	lStgMedium.tymed = TYMED_ISTREAM;

	try
	{
		if	(pFormatId == 0)
		{
			pFormatId = RegisterClipboardFormat (CF_EMBEDDEDOBJECT);
		}

		lFormatEtc.cfFormat = pFormatId;
		lFormatEtc.dwAspect = DVASPECT_CONTENT;
		lFormatEtc.tymed = TYMED_ISTREAM;
		lFormatEtc.lindex = pNdx;

		if	(
				(pDataObject.GetData (pFormatId, &lStgMedium, &lFormatEtc))
			&&	(lStgMedium.tymed == TYMED_ISTREAM)
			&&	(lStgMedium.pstm)
			)
		{
			if	(pStream)
			{
				LARGE_INTEGER	lStart = {0,0};
				tS <STATSTG>	lStat;

				try
				{
					if	(*pStream)
					{
						(*pStream)->Release ();
					}
				}
				catch AnyException

				(*pStream) = NULL;

				if	(
						(SUCCEEDED (lStgMedium.pstm->Stat (&lStat, STATFLAG_NONAME)))
					&&	(SUCCEEDED (lStgMedium.pstm->Seek (lStart, STREAM_SEEK_SET, NULL)))
					&&	(SUCCEEDED (CreateStreamOnHGlobal (NULL, TRUE, pStream)))
					&&	(SUCCEEDED (lStgMedium.pstm->CopyTo (*pStream, lStat.cbSize, NULL, NULL)))
					&&	(SUCCEEDED ((*pStream)->SetSize (lStat.cbSize)))
					&&	(SUCCEEDED ((*pStream)->Seek (lStart, STREAM_SEEK_SET, NULL)))
					)
				{
					//LogMessage (LogDetails|LogTime, _T("GetClipboardStream [Size %I64d]"), lStat.cbSize.QuadPart);
					lRet = true;
				}
			}
			else
			{
				lRet = true;
			}
		}
	}
	catch AnyException

	try
	{
		if	(
				(!lRet)
			&&	(pStream)
			&&	(*pStream)
			)
		{
			(*pStream)->Release ();
			(*pStream) = NULL;
		}
	}
	catch AnyException

	try
	{
		ReleaseStgMedium (&lStgMedium);
	}
	catch AnyException

	return lRet;
}

//////////////////////////////////////////////////////////////////////

STGMEDIUM * CClipboard::PeekStream (UINT pFormatId, int pNdx)
{
	bool				lRet = false;
	COleDataObject		lDataObject;
	tPtr <STGMEDIUM>	lStgMedium = new STGMEDIUM;
	tS <FORMATETC>		lFormatEtc;

	memset (lStgMedium.Ptr (), 0, sizeof (STGMEDIUM));
	lStgMedium->tymed = TYMED_ISTREAM;

	try
	{
		if	(pFormatId == 0)
		{
			pFormatId = RegisterClipboardFormat (CF_EMBEDDEDOBJECT);
		}

		lFormatEtc.cfFormat = pFormatId;
		lFormatEtc.dwAspect = DVASPECT_CONTENT;
		lFormatEtc.tymed = TYMED_ISTREAM;
		lFormatEtc.lindex = pNdx;

		if	(
				(lDataObject.AttachClipboard ())
			&&	(lDataObject.GetData (pFormatId, lStgMedium, &lFormatEtc))
			&&	(lStgMedium->tymed == TYMED_ISTREAM)
			&&	(lStgMedium->pstm)
			)
		{
			return lStgMedium.Detach ();
		}
	}
	catch AnyException

	try
	{
		ReleaseStgMedium (lStgMedium);
	}
	catch AnyException

	return NULL;
}

bool CClipboard::PeekStream (GUID& pStreamClassId, UINT pFormatId, int pNdx)
{
	bool	lRet = false;

	try
	{
		tPtr <STGMEDIUM>	lStgMedium;
		LARGE_INTEGER		lStreamStart = {0,0};

		if	(lStgMedium = PeekStream (pFormatId, pNdx))
		{
			if	(
					(SUCCEEDED (LogComErr (LogVerbose|LogTime, lStgMedium->pstm->Seek (lStreamStart, STREAM_SEEK_SET, NULL))))
				&&	(SUCCEEDED (LogComErr (LogVerbose|LogTime, ReadClassStm (lStgMedium->pstm, &pStreamClassId))))
				)
			{
				lRet = true;
			}
			ReleaseStgMedium (lStgMedium);
		}
	}
	catch AnyException

	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

bool CClipboard::PutStorage (IStorage * pStorage, bool pCopy, bool pRelease, bool pAppend, bool pFlush, UINT pFormatId, int pNdx)
{
	bool				lRet = false;
	bool				lAppended = false;
	COleDataSource *	lDataSource;
	tS <STGMEDIUM>		lStgMedium;
	tS <FORMATETC>		lFormatEtc;
	ILockBytesPtr		lLockBytes;

	if	(pFormatId == 0)
	{
		pFormatId = RegisterClipboardFormat (CF_EMBEDDEDOBJECT);
	}

	lFormatEtc.cfFormat = pFormatId;
	lFormatEtc.dwAspect = DVASPECT_CONTENT;
	lFormatEtc.tymed = TYMED_ISTORAGE;
	lFormatEtc.lindex = pNdx;

	lStgMedium.tymed = TYMED_ISTORAGE;
	if	(pCopy)
	{
		if	(SUCCEEDED (CreateILockBytesOnHGlobal (NULL, TRUE, &lLockBytes)))
		{
			StgCreateDocfileOnILockBytes (lLockBytes, STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE, 0, &lStgMedium.pstg);
		}
	}
	else
	{
		lStgMedium.pstg = pStorage;
	}

	try
	{
		if	(
				(pStorage)
			&&	(lStgMedium.pstg)
			)
		{

			if	(
					(!pCopy)
				||	(SUCCEEDED (pStorage->CopyTo (0, NULL, NULL, lStgMedium.pstg)))
				)
			{
				if	(
						(
							(pAppend)
						&&	(lDataSource = COleDataSource::GetClipboardOwner ())
						&&	(lAppended = true)
						)
					||	(lDataSource = new COleDataSource)
					)
				{
					lDataSource->CacheData (lFormatEtc.cfFormat, &lStgMedium, &lFormatEtc);
					if	(lAppended)
					{
						lDataSource->ExternalAddRef ();
					}
					lDataSource->SetClipboard ();
					if	(pFlush)
					{
						lDataSource->FlushClipboard ();
					}
					lRet = true;
				}
			}
		}
	}
	catch AnyException

	try
	{
		if	(
				(pCopy)
			&&	(!lRet)
			)
		{
			ReleaseStgMedium (&lStgMedium);
		}
	}
	catch AnyException

	try
	{
		if	(
				(pRelease)
			&&	(pStorage)
			)
		{
			pStorage->Release ();
		}
	}
	catch AnyException

	return lRet;
}

//////////////////////////////////////////////////////////////////////

bool CClipboard::GetStorage (IStorage ** pStorage, UINT pFormatId, int pNdx)
{
	bool			lRet = false;
	COleDataObject	lDataObject;
	tS <STGMEDIUM>	lStgMedium;
	tS <FORMATETC>	lFormatEtc;
	ILockBytesPtr	lLockBytes;

	lStgMedium.tymed = TYMED_ISTORAGE;

	try
	{
		if	(pFormatId == 0)
		{
			pFormatId = RegisterClipboardFormat (CF_EMBEDDEDOBJECT);
		}

		lFormatEtc.cfFormat = pFormatId;
		lFormatEtc.dwAspect = DVASPECT_CONTENT;
		lFormatEtc.tymed = TYMED_ISTORAGE;
		lFormatEtc.lindex = pNdx;

		if	(
				(lDataObject.AttachClipboard ())
			&&	(lDataObject.GetData (pFormatId, &lStgMedium, &lFormatEtc))
			&&	(lStgMedium.tymed == TYMED_ISTORAGE)
			&&	(lStgMedium.pstg)
			)
		{
			if	(pStorage)
			{
				try
				{
					if	(*pStorage)
					{
						(*pStorage)->Release ();
					}
				}
				catch AnyException

				(*pStorage) = NULL;

				if	(
						(SUCCEEDED (CreateILockBytesOnHGlobal (NULL, TRUE, &lLockBytes)))
					&&	(SUCCEEDED (StgCreateDocfileOnILockBytes (lLockBytes, STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE, 0, pStorage)))
					&&	(SUCCEEDED (lStgMedium.pstg->CopyTo (0, NULL, NULL, *pStorage)))
					)
				{
					lRet = true;
				}
			}
			else
			{
				lRet = true;
			}
		}
	}
	catch AnyException

	try
	{
		if	(
				(!lRet)
			&&	(pStorage)
			&&	(*pStorage)
			)
		{
			(*pStorage)->Release ();
			(*pStorage) = NULL;
		}
	}
	catch AnyException

	try
	{
		ReleaseStgMedium (&lStgMedium);
	}
	catch AnyException

	return lRet;
}

//////////////////////////////////////////////////////////////////////

STGMEDIUM * CClipboard::PeekStorage (UINT pFormatId, int pNdx)
{
	bool				lRet = false;
	COleDataObject		lDataObject;
	tPtr <STGMEDIUM>	lStgMedium = new tS <STGMEDIUM>;
	tS <FORMATETC>		lFormatEtc;

	lStgMedium->tymed = TYMED_ISTORAGE;

	try
	{
		if	(pFormatId == 0)
		{
			pFormatId = RegisterClipboardFormat (CF_EMBEDDEDOBJECT);
		}

		lFormatEtc.cfFormat = pFormatId;
		lFormatEtc.dwAspect = DVASPECT_CONTENT;
		lFormatEtc.tymed = TYMED_ISTORAGE;
		lFormatEtc.lindex = pNdx;

		if	(
				(lDataObject.AttachClipboard ())
			&&	(lDataObject.GetData (pFormatId, lStgMedium, &lFormatEtc))
			&&	(lStgMedium->tymed == TYMED_ISTORAGE)
			&&	(lStgMedium->pstg)
			)
		{
			return lStgMedium.Detach ();
		}
	}
	catch AnyException

	try
	{
		ReleaseStgMedium (lStgMedium);
	}
	catch AnyException

	return NULL;
}

bool CClipboard::PeekStorage (GUID& pStorageClassId, UINT pFormatId, int pNdx)
{
	bool	lRet = false;

	try
	{
		tPtr <STGMEDIUM>	lStgMedium;

		if	(lStgMedium = PeekStorage (pFormatId, pNdx))
		{
			if	(SUCCEEDED (LogComErr (LogVerbose|LogTime, ReadClassStg (lStgMedium->pstg, &pStorageClassId))))
			{
				lRet = true;
			}
			ReleaseStgMedium (lStgMedium);
		}
	}
	catch AnyException

	return lRet;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

void CClipboard::DumpDataObject (UINT pLogLevel, COleDataObject& pDataObject, LPCTSTR pTitle, bool pDumpData)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("DataObject");
			}

			LogMessage (pLogLevel, _T("%s"), lTitle);

			try
			{
				tS <FORMATETC>	lFormat;

				LogMessage (pLogLevel, _T("  Formats"));

				pDataObject.BeginEnumFormats ();
				while	(pDataObject.GetNextFormat (&lFormat))
				{
					LogMessage (pLogLevel|LogDebugFast, _T("    (%d) [%u] [%4.4X] [%s] Media [%u] [%s]"), lFormat.lindex, lFormat.cfFormat, lFormat.cfFormat, ClipboardFormatName(lFormat.cfFormat), lFormat.tymed, MediaTypeName(lFormat.tymed));
					lFormat.Clear ();
				}
			}
			catch AnyExceptionDebug

			try
			{
				CString	lText;

				if	(GetText (lText, pDataObject))
				{
					LogMessage (LogDebug, _T("  Text [%s]"), lText);
				}
			}
			catch AnyExceptionDebug

			try
			{
				CStringArray	lFiles;

				if	(GetFiles (lFiles, pDataObject))
				{
					LogMessage (LogDebug, _T("  Files [%d]"), lFiles.GetSize ());

					for	(int lNdx = 0; lNdx < lFiles.GetSize (); lNdx++)
					{
						LogMessage (pLogLevel|LogDebugFast, _T("    [%s]"), lFiles[lNdx]);
					}
				}
			}
			catch AnyExceptionDebug

			try
			{
				LPITEMIDLIST *	lItems = NULL;
				int				lItemCount = 0;

				if	(GetIdls (lItems, lItemCount, pDataObject))
				{
					LogMessage (pLogLevel, _T("  IdLists [%d]"), lTitle, lItemCount);

					for	(int lNdx = 0; lNdx < lItemCount; lNdx++)
					{
						LogIdList (pLogLevel|LogDebugFast, lItems[lNdx], _T(""), _T("   "));
					}

					CClipboard::ReleaseIdls (lItems, lItemCount);
				}
			}
			catch AnyExceptionDebug

			if	(pDumpData)
			{
				try
				{
					tS <FORMATETC>	lFormat;

					pDataObject.BeginEnumFormats ();
					while	(pDataObject.GetNextFormat (&lFormat))
					{
						if	(
								(lFormat.cfFormat == CF_TEXT)
							||	(lFormat.cfFormat == CF_UNICODETEXT)
							||	(lFormat.cfFormat == CF_HDROP)
							||	(lFormat.cfFormat == RegisterClipboardFormat (CFSTR_SHELLIDLIST))
							)
						{
							continue;
						}

						LogMessage (pLogLevel|LogDebugFast, _T("  Format [%u] [%4.4X] [%s]"), lFormat.cfFormat, lFormat.cfFormat, ClipboardFormatName(lFormat.cfFormat));

						if	(lFormat.tymed == TYMED_HGLOBAL)
						{
							try
							{
								HGLOBAL	lMemHandle;
								SIZE_T	lMemSize;
								LPBYTE	lMemData;

								if	(
										(pDataObject.IsDataAvailable (lFormat.cfFormat))
									&&	(lMemHandle = pDataObject.GetGlobalData (lFormat.cfFormat))
									)
								{
									if	(
											(lMemSize = GlobalSize (lMemHandle))
										&&	(lMemData = (LPBYTE) GlobalLock (lMemHandle))
										)
									{
										LogDump (pLogLevel|LogDebugFast, lMemData, min(lMemSize,1024), _T("    "));
										GlobalUnlock (lMemHandle);
									}
									GlobalFree (lMemHandle);
								}
							}
							catch AnyExceptionSilent
						}

						if	(lFormat.tymed == TYMED_ISTREAM)
						{
							try
							{
								IStreamPtr		lStream;
								tS <STATSTG>	lStat;

								if	(
										(GetStream (&lStream, pDataObject, lFormat.cfFormat, lFormat.lindex))
									&&	(SUCCEEDED (lStream->Stat (&lStat, STATFLAG_NONAME)))
									)
								{
									HGLOBAL	lMemHandle;
									SIZE_T	lMemSize;
									LPBYTE	lMemData;

									LogMessage (pLogLevel, _T("    Stream Size [%I64u]"), lStat.cbSize.QuadPart);

									if	(
											(SUCCEEDED (GetHGlobalFromStream (lStream, &lMemHandle)))
										&&	(lMemSize = GlobalSize (lMemHandle))
										)
									{
										LogMessage (pLogLevel, _T("    Stream Data [%u]"), lMemSize);

										if	(lMemData = (LPBYTE) GlobalLock (lMemHandle))
										{
											LogDump (pLogLevel|LogDebugFast, lMemData, min(lMemSize,1024), _T("      "), false);
											GlobalUnlock (lMemHandle);
										}
									}
								}
							}
							catch AnyExceptionSilent
						}

						lFormat.Clear ();
					}
				}
				catch AnyExceptionDebug
			}

			LogMessage (pLogLevel, _T("%s end"), lTitle);
		}
		catch AnyExceptionDebug
	}
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CString ClipboardFormatName (CLIPFORMAT pFormatId)
{
	CString	lRet;

	switch (pFormatId)
	{
		case CF_TEXT:			lRet = _T("CF_TEXT"); break;
		case CF_BITMAP:			lRet = _T("CF_BITMAP"); break;
		case CF_METAFILEPICT:	lRet = _T("CF_METAFILEPICT"); break;
		case CF_SYLK:			lRet = _T("CF_SYLK"); break;
		case CF_DIF:			lRet = _T("CF_DIF"); break;
		case CF_TIFF:			lRet = _T("CF_TIFF"); break;
		case CF_OEMTEXT:		lRet = _T("CF_OEMTEXT"); break;
		case CF_DIB:			lRet = _T("CF_DIB"); break;
		case CF_PALETTE:		lRet = _T("CF_PALETTE"); break;
		case CF_PENDATA:		lRet = _T("CF_PENDATA"); break;
		case CF_RIFF:			lRet = _T("CF_RIFF"); break;
		case CF_WAVE:			lRet = _T("CF_WAVE"); break;
		case CF_UNICODETEXT:	lRet = _T("CF_UNICODETEXT"); break;
		case CF_ENHMETAFILE:	lRet = _T("CF_ENHMETAFILE"); break;
#ifdef	CF_HDROP
		case CF_HDROP:			lRet = _T("CF_HDROP"); break;
#endif
#ifdef	CF_LOCALE
		case CF_LOCALE:			lRet = _T("CF_LOCALE"); break;
#endif
#ifdef	CF_DIBV5
		case CF_DIBV5:			lRet = _T("CF_DIBV5"); break;
#endif

		default:
		{
			if	(GetClipboardFormatName (pFormatId, lRet.GetBuffer (MAX_PATH), MAX_PATH))
			{
				lRet.ReleaseBuffer ();
			}
			else
			{
				lRet.Format (_T("%4.4X"), pFormatId);
			}
		}
	}

	return lRet;
}

CString MediaTypeName (DWORD pMediaType)
{
	CStringArray	Types;

	if	(pMediaType == TYMED_NULL)
	{
		Types.Add (_T("TYMED_ENHMF"));
	}
	else
	{
		if	(pMediaType & TYMED_HGLOBAL)
		{
			Types.Add (_T("TYMED_HGLOBAL"));
		}
		if	(pMediaType & TYMED_FILE)
		{
			Types.Add (_T("TYMED_FILE"));
		}
		if	(pMediaType & TYMED_ISTREAM)
		{
			Types.Add (_T("TYMED_ISTREAM"));
		}
		if	(pMediaType & TYMED_ISTORAGE)
		{
			Types.Add (_T("TYMED_ISTORAGE"));
		}
		if	(pMediaType & TYMED_GDI)
		{
			Types.Add (_T("TYMED_GDI"));
		}
		if	(pMediaType & TYMED_MFPICT)
		{
			Types.Add (_T("TYMED_MFPICT"));
		}
		if	(pMediaType & TYMED_ENHMF)
		{
			Types.Add (_T("TYMED_ENHMF"));
		}
	}

	return JoinStringArray (Types, _T(" "));
}
