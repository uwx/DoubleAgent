/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2012 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of the Double Agent Server.

    The Double Agent Server is free software:
    you can redistribute it and/or modify it under the terms of the
    GNU Lesser Public License as published by the Free Software Foundation,
    either version 3 of the License, or (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser Public License for more details.

    You should have received a copy of the GNU Lesser Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#pragma once

static inline CAtlString EncodeTraceString (LPCTSTR pString)
{
	CAtlString	lString (pString);
	lString.Replace (_T("\r"), _T("%0D"));
	lString.Replace (_T("\n"), _T("%0A"));
	lString.Replace (_T("\t"), _T("%09"));
	lString.Replace (_T("\f"), _T("%0C"));
	return lString;
}

static inline CAtlString DecodeTraceString (LPCTSTR pString)
{
	CAtlString	lString (pString);
	lString.Replace (_T("%0D"), _T("\r"));
	lString.Replace (_T("%0A"), _T("\n"));
	lString.Replace (_T("%09"), _T("\t"));
	lString.Replace (_T("%0C"), _T("\f"));
	return lString;
}

/////////////////////////////////////////////////////////////////////////////
