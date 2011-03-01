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
//////////////////////////////////////////////////////////////////////
#pragma pack (push)
#pragma pack (4)
struct VS_VERSION_INFO_W
{
	WORD				wLength;
	WORD				wValueLength;
	WORD				wType;
	WCHAR				szKey [sizeof ("VS_VERSION_INFO")];
	VS_FIXEDFILEINFO	Value;
	WORD				Children;
};

struct StringW
{
	WORD				wLength;
	WORD				wValueLength;
	WORD				wType;
	WCHAR				szKey [1];
	WCHAR				Value [1];
};

struct StringTableW
{
	WORD				wLength;
	WORD				wValueLength;
	WORD				wType;
	WCHAR				szKey [9];
	StringW 			Children;
};

struct StringFileInfoW
{
	WORD				wLength;
	WORD				wValueLength;
	WORD				wType;
	WCHAR				szKey [sizeof ("StringFileInfo")];
	StringTableW		Children;
};

struct VarW
{
	WORD				wLength;
	WORD				wValueLength;
	WORD				wType;
	WCHAR				szKey [sizeof ("Translation")];
	DWORD				Value;
};

struct VarFileInfoW
{
	WORD				wLength;
	WORD				wValueLength;
	WORD				wType;
	WCHAR				szKey [sizeof ("VarFileInfo")];
	VarW				Children;
};
#pragma pack (pop)
//////////////////////////////////////////////////////////////////////
#pragma pack (push)
#pragma pack (2)
struct VS_VERSION_INFO_A
{
	WORD				wLength;
	WORD				wValueLength;
	char				szKey [sizeof ("VS_VERSION_INFO")];
	VS_FIXEDFILEINFO	Value;
	WORD				Children;
};

struct StringA
{
	WORD				wType;
	WORD				wLength;
	WORD				wValueLength;
	char				szKey [1];
	char				Value [1];
};

struct StringTableA
{
	WORD				wLength;
	WORD				wValueLength;
	char				szKey [9];
	StringA 			Children;
};

struct StringFileInfoA
{
	WORD				wLength;
	WORD				wValueLength;
	char				szKey [sizeof ("StringFileInfo")];
	StringTableA		Children;
};

struct VarA
{
	WORD				wLength;
	WORD				wValueLength;
	char				szKey [sizeof ("Translation")];
	DWORD				Value;
};

struct VarFileInfoA
{
	WORD				wLength;
	WORD				wValueLength;
	char				szKey [sizeof ("VarFileInfo")];
	VarA				Children;
};
#pragma pack (pop)
//////////////////////////////////////////////////////////////////////
