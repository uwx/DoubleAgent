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
#if !defined(AFX_CLIPBOARD_H__878A13BD_0AEB_4D48_A568_CC1B0DFB1442__INCLUDED_)
#define AFX_CLIPBOARD_H__878A13BD_0AEB_4D48_A568_CC1B0DFB1442__INCLUDED_
#pragma once

#include <AfxOle.h>

//////////////////////////////////////////////////////////////////////

class CClipboard
{
public:
	CClipboard ();
	virtual ~CClipboard ();

	static void Empty ();
	static void Flush ();

	bool HasText ();
	bool PutText (LPCTSTR pText, bool pAppend = false, bool pFlush = true);
	bool GetText (CString& pText);

	static bool HasText (COleDataObject& pDataObject);
	static bool GetText (CString& pText, COleDataObject& pDataObject);

	bool PutFile (const CString& pFile, bool pAppend = false, bool pFlush = true, UINT pFormatId = 0);
	bool PutFiles (const CStringArray& pFiles, bool pAppend = false, bool pFlush = true, UINT pFormatId = 0);

	static bool PutFile (const CString& pFile, COleDataSource& pDataSource, UINT pFormatId = 0);
	static bool PutFiles (const CStringArray& pFiles, COleDataSource& pDataSource, UINT pFormatId = 0);

	bool HasFiles (UINT pFormatId = 0);
	bool GetFiles (CStringArray& pFiles, UINT pFormatId = 0);
	static bool HasFiles (COleDataObject& pDataObject, UINT pFormatId = 0);
	static bool GetFiles (CStringArray& pFiles, COleDataObject& pDataObject, UINT pFormatId = 0);

	bool PutIdl (LPCITEMIDLIST pIdl, bool pAppend = false, bool pFlush = true, UINT pFormatId = 0);
	bool PutIdls (LPCITEMIDLIST * pIdls, UINT pIdlCount, bool pAppend = false, bool pFlush = true, UINT pFormatId = 0);
	bool PutFileIdl (const CString& pFile, bool pAppend = false, bool pFlush = true, UINT pFormatId = 0);
	bool PutFileIdls (const CStringArray& pFiles, bool pAppend = false, bool pFlush = true, UINT pFormatId = 0);

	static bool PutIdl (LPCITEMIDLIST pIdl, COleDataSource& pDataSource, UINT pFormatId = 0);
	static bool PutIdls (LPCITEMIDLIST * pIdls, UINT pIdlCount, COleDataSource& pDataSource, UINT pFormatId = 0);
	static bool PutFileIdl (const CString& pFile, COleDataSource& pDataSource, UINT pFormatId = 0);
	static bool PutFileIdls (const CStringArray& pFiles, COleDataSource& pDataSource, UINT pFormatId = 0);

	bool HasIdls (UINT pFormatId = 0);
	bool GetIdls (LPITEMIDLIST*& pIdls, int& pIdlCount, UINT pFormatId = 0);
	static bool HasIdls (COleDataObject& pDataObject, UINT pFormatId = 0);
	static bool GetIdls (LPITEMIDLIST*& pIdls, int& pIdlCount, COleDataObject& pDataObject, UINT pFormatId = 0);
	static void ReleaseIdls (LPITEMIDLIST * pIdls, int pIdlCount);

	bool HasData (UINT pFormatId);
	bool PutData (const CByteArray& pData, UINT pFormatId, bool pAppend = true, bool pFlush = true, int pNdx = -1);
	bool GetData (CByteArray& pData, UINT pFormatId, int pNdx = -1);

	static bool HasData (UINT pFormatId, COleDataSource& pDataSource);
	static bool PutData (const CByteArray& pData, UINT pFormatId, COleDataSource& pDataSource, bool pAppend = true, bool pFlush = true, int pNdx = -1);
	static bool GetData (CByteArray& pData, UINT pFormatId, COleDataObject& pDataObject, int pNdx = -1);

	bool PutStream (IStream * pStream, bool pCopy = false, bool pRelease = false, bool pAppend = false, bool pFlush = true, UINT pFormatId = 0, int pNdx = -1);
	bool GetStream (IStream ** pStream, UINT pFormatId = 0, int pNdx = -1);
	STGMEDIUM * PeekStream (UINT pFormatId = 0, int pNdx = -1);
	bool PeekStream (GUID& pStreamClassId, UINT pFormatId = 0, int pNdx = -1);

	static bool PutStream (IStream * pStream, COleDataSource& pDataSource, bool pCopy = false, bool pRelease = false, UINT pFormatId = 0, int pNdx = -1);
	static bool GetStream (IStream ** pStream, COleDataObject& pDataObject, UINT pFormatId = 0, int pNdx = -1);

	bool PutStorage (IStorage * pStorage, bool pCopy = false, bool pRelease = false, bool pAppend = false, bool pFlush = true, UINT pFormatId = 0, int pNdx = -1);
	bool GetStorage (IStorage ** pStorage, UINT pFormatId = 0, int pNdx = -1);
	STGMEDIUM * PeekStorage (UINT pFormatId = 0, int pNdx = -1);
	bool PeekStorage (GUID& pStorageClassId, UINT pFormatId = 0, int pNdx = -1);

	static void DumpDataObject (UINT pLogLevel, COleDataObject& pDataObject, LPCTSTR pTitle = NULL, bool pDumpData = false);
	friend CString ClipboardFormatName (CLIPFORMAT pFormatId);
	friend CString MediaTypeName (DWORD pMediaType);
};

//////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_CLIPBOARD_H__878A13BD_0AEB_4D48_A568_CC1B0DFB1442__INCLUDED_)
