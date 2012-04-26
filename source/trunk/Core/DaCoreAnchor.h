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
#pragma once
#include "DaCoreExp.h"
#include "AgentFileCache.h"
#include "SapiVoiceCache.h"
#include "SapiInputCache.h"

/////////////////////////////////////////////////////////////////////////////

class CDaCoreAnchor :
	public CAgentFileCache,
	public CSapiVoiceCache,
	public CSapiInputCache
{
	DECLARE_DLL_OBJECT_EX(CDaCoreAnchor, _DACORE_IMPEXP)
protected:
	_DACORE_IMPEXP CDaCoreAnchor (class CComModule & pModule);
public:
	_DACORE_IMPEXP virtual ~CDaCoreAnchor ();

	class CComModule & Module;
};

/////////////////////////////////////////////////////////////////////////////
