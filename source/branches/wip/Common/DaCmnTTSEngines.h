/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
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

class CDaCmnTTSEngines
{
public:
	CDaCmnTTSEngines ();
	virtual ~CDaCmnTTSEngines ();

// Attributes
public:
	CAtlPtrTypeArray <class CSapi5VoiceInfo>	mSapi5Voices;
#ifndef	_WIN64
	CAtlPtrTypeArray <class CSapi4VoiceInfo>	mSapi4Voices;
#endif

// Operations
public:
	HRESULT UseAllVoices ();
	HRESULT UseTheseVoices (class CAgentFile * pFile, LANGID pLangId, short pGender);
};

/////////////////////////////////////////////////////////////////////////////
