/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
#include "StdAfx.h"
#include "DaError.h"
#include "DaErrorRes.h"
#include "DaVersion.h"
#include "Localize.h"

//////////////////////////////////////////////////////////////////////

CServerError::CServerError (HRESULT pResult, const GUID& pGuid)
{
	mGuid = pGuid;
	mDescription = DaErrorDescription (pResult);
#ifdef	_STRICT_COMPATIBILITY
	if	(!mDescription.IsEmpty ())
	{
		mSource = _T(_SERVER_PROGID_NAME);
	}
#else
	mSource = _T(_SERVER_PROGID_NAME);
	if	(mDescription.IsEmpty ())
	{
		mDescription = GetSysDescription (pResult);
	}
#endif
}

//////////////////////////////////////////////////////////////////////

CControlError::CControlError (HRESULT pResult, const GUID& pGuid)
{
	mSource = _T(_CONTROL_PROGID_NAME);
	mGuid = pGuid;
	mDescription = DaErrorDescription (pResult);
	if	(mDescription.IsEmpty ())
	{
		mDescription = GetSysDescription (pResult);
	}
}

//////////////////////////////////////////////////////////////////////

CAtlString DaErrorDescription (HRESULT pResult)
{
	CAtlString	lDescription;

	switch (pResult)
	{
		case E_FAIL											:	lDescription = CLocalize::LoadString (IDS_AGENTERR_UNKNOWN); break;
		case AGENTERR_CLIENTINVALID							:	lDescription = CLocalize::LoadString (IDS_AGENTERR_CLIENTINVALID); break;
		case AGENTERR_CHARACTERINVALID						:	lDescription = CLocalize::LoadString (IDS_AGENTERR_CHARACTERINVALID); break;
		case AGENTERR_ANIMATIONNOTFOUND						:	lDescription = CLocalize::LoadString (IDS_AGENTERR_ANIMATIONNOTFOUND); break;
		case AGENTERR_STATENOTFOUND							:	lDescription = CLocalize::LoadString (IDS_AGENTERR_STATENOTFOUND); break;
		case AGENTERR_COMMANDNOTFOUND						:	lDescription = CLocalize::LoadString (IDS_AGENTERR_COMMANDNOTFOUND); break;
		case AGENTERR_COMMANDALREADYINUSE					:	lDescription = CLocalize::LoadString (IDS_AGENTERR_COMMANDALREADYINUSE); break;
		case AGENTERR_MENUNOTFOUND							:	lDescription = CLocalize::LoadString (IDS_AGENTERR_CLIENTINVALID); break;
		case AGENTERR_CHARACTERNOTVISIBLE					:	lDescription = CLocalize::LoadString (IDS_AGENTERR_CHARACTERNOTVISIBLE); break;
		case AGENTERR_CHARACTERALREADYLOADED				:	lDescription = CLocalize::LoadString (IDS_AGENTERR_CHARACTERALREADYLOADED); break;
		case AGENTERR_NOBALLOON								:	lDescription = CLocalize::LoadString (IDS_AGENTERR_NOBALLOON); break;
		case AGENTERR_NOCOMMANDSWINDOW						:	lDescription = CLocalize::LoadString (IDS_AGENTERR_NOCOMMANDSWINDOW); break;
		case AGENTERR_INVALIDPREPARETYPE					:	lDescription = CLocalize::LoadString (IDS_AGENTERR_INVALIDPREPARETYPE); break;
		case AGENTERR_INVALIDANIMATION						:	lDescription = CLocalize::LoadString (IDS_AGENTERR_INVALIDANIMATION); break;
		case AGENTERR_CANTMOVEDURINGDRAG					:	lDescription = CLocalize::LoadString (IDS_AGENTERR_CANTMOVEDURINGDRAG); break;
		case AGENTERR_CHARACTERNOTACTIVE					:	lDescription = CLocalize::LoadString (IDS_AGENTERR_CHARACTERNOTACTIVE); break;
		case AGENTERR_LANGUAGENOTFOUND						:	lDescription = CLocalize::LoadString (IDS_AGENTERR_LANGUAGENOTFOUND); break;
		case AGENTERR_TTSLANGUAGENOTFOUND					:	lDescription = CLocalize::LoadString (IDS_AGENTERR_TTSLANGUAGENOTFOUND); break;
		case AGENTERR_SRLANGUAGENOTFOUND					:	lDescription = CLocalize::LoadString (IDS_AGENTERR_TTSLANGUAGENOTFOUND); break;
		case AGENTERR_LANGUAGEMISMATCH						:	lDescription = CLocalize::LoadString (IDS_AGENTERR_LANGUAGEMISMATCH); break;
		case AGENTERR_SPEAKINGDISABLED						:	lDescription = CLocalize::LoadString (IDS_AGENTERR_SPEAKINGDISABLED); break;
		case AGENTERR_NOCHARACTERS							:	lDescription = CLocalize::LoadString (IDS_AGENTERR_NOCHARACTERS); break;
		case AGENTERR_DEFAULTCHARACTER						:	lDescription = CLocalize::LoadString (IDS_AGENTERR_DEFAULTCHARACTER); break;
		case AGENTREQERR_OBJECTNOTFOUND						:	lDescription = CLocalize::LoadString (IDS_AGENTREQERR_OBJECTNOTFOUND); break;
		case AGENTREQERR_OBJECTINVALID						:	lDescription = CLocalize::LoadString (IDS_AGENTREQERR_OBJECTINVALID); break;
		case AGENTREQERR_CANTSTOPOTHERS						:	lDescription = CLocalize::LoadString (IDS_AGENTREQERR_CANTSTOPOTHERS); break;
		case AGENTREQERR_CANTINTERRUPTSELF					:	lDescription = CLocalize::LoadString (IDS_AGENTREQERR_CANTINTERRUPTSELF); break;
		case AGENTREQERR_CANTWAITONSELF						:	lDescription = CLocalize::LoadString (IDS_AGENTREQERR_CANTWAITONSELF); break;
		case AGENTREQERR_INVALIDBOOKMARK					:	lDescription = CLocalize::LoadString (IDS_AGENTREQERR_INVALIDBOOKMARK); break;
		case AGENTREQERR_REMOVED							:	lDescription = CLocalize::LoadString (IDS_AGENTREQERR_REMOVED); break;
		case AGENTREQERR_INTERRUPTEDLISTENKEY				:	lDescription = CLocalize::LoadString (IDS_AGENTREQERR_INTERRUPTEDLISTENKEY); break;
		case AGENTREQERR_INTERRUPTEDHEARING					:	lDescription = CLocalize::LoadString (IDS_AGENTREQERR_INTERRUPTEDHEARING); break;
		case AGENTREQERR_INTERRUPTEDCODE					:	lDescription = CLocalize::LoadString (IDS_AGENTREQERR_INTERRUPTEDCODE); break;
		case AGENTREQERR_INTERRUPTEDUSER					:	lDescription = CLocalize::LoadString (IDS_AGENTREQERR_INTERRUPTEDUSER); break;
		case AGENTREQERR_INVALIDLASTTAG						:	lDescription = CLocalize::LoadString (IDS_AGENTREQERR_INVALIDLASTTAG); break;
		case AGENTPROVERROR_INIT							:	lDescription = CLocalize::LoadString (IDS_AGENTPROVERROR_INIT); break;
		case AGENTPROVERROR_CHARACTERVERSION				:	lDescription = CLocalize::LoadString (IDS_AGENTPROVERROR_CHARACTERVERSION); break;
		case AGENTPROVERROR_VERSION							:	lDescription = CLocalize::LoadString (IDS_AGENTPROVERROR_VERSION); break;
		case AGENTPROVERROR_MAGIC							:	lDescription = CLocalize::LoadString (IDS_AGENTPROVERROR_MAGIC); break;
		case AGENTPROVERROR_CHARACTERINVALID				:	lDescription = CLocalize::LoadString (IDS_AGENTERR_CHARACTERINVALID); break;
		case AGENTPROVERROR_WAVEINVALID						:	lDescription = CLocalize::LoadString (IDS_AGENTPROVERROR_WAVEINVALID); break;
		case AGENTPROVERROR_WAVECORRUPT						:	lDescription = CLocalize::LoadString (IDS_AGENTPROVERROR_WAVECORRUPT); break;
		case AGENTPROVERROR_MMIO							:	lDescription = CLocalize::LoadString (IDS_AGENTPROVERROR_MMIO); break;
		case AGENTPROVERROR_PROTOCOL						:	lDescription = CLocalize::LoadString (IDS_AGENTPROVERROR_PROTOCOL); break;
		case AGENTAUDIOERROR_DEVICE							:	lDescription = CLocalize::LoadString (IDS_AGENTAUDIOERROR_DEVICE); break;
		case AGENTAUDIOERROR_TTSENUMERATOR					:	lDescription = CLocalize::LoadString (IDS_AGENTAUDIOERROR_TTS); break;
		case AGENTAUDIOERROR_TTSSELECT						:	lDescription = CLocalize::LoadString (IDS_AGENTAUDIOERROR_TTS); break;
		case AGENTAUDIOERROR_TTSREGISTER					:	lDescription = CLocalize::LoadString (IDS_AGENTAUDIOERROR_TTS); break;
		case AGENTAUDIOERROR_TTSUNEXPECTED					:	lDescription = CLocalize::LoadString (IDS_AGENTAUDIOERROR_TTS); break;
		case AGENTAUDIOERROR_LWVINIT						:	lDescription = CLocalize::LoadString (IDS_AGENTAUDIOERROR_LWV); break;
		case AGENTAUDIOERROR_LWVREGISTER					:	lDescription = CLocalize::LoadString (IDS_AGENTAUDIOERROR_LWV); break;
		case AGENTAUDIOERROR_LWVUNEXPECTED					:	lDescription = CLocalize::LoadString (IDS_AGENTAUDIOERROR_LWV); break;
		case AGENTCTLERROR_SERVERINIT						:	lDescription = CLocalize::LoadString (IDS_AGENTCTLERROR_SERVERINIT); break;
		case AGENTVOICEERROR_NOTINSTALLED					:	lDescription = CLocalize::LoadString (IDS_AGENTVOICEERROR_NOTINSTALLED); break;
		case AGENTVOICEERROR_INVALIDMENU					:	lDescription = CLocalize::LoadString (IDS_AGENTVOICEERROR_INVALIDMENU); break;
		case AGENTVOICEERROR_UNCLOSEDALTERNATIVE			:	lDescription = CLocalize::LoadString (IDS_AGENTVOICEERROR_UNCLOSEDALTERNATIVE); break;
		case AGENTVOICEERROR_UNCLOSEDOPTIONAL				:	lDescription = CLocalize::LoadString (IDS_AGENTVOICEERROR_UNCLOSEDOPTIONAL); break;
		case AGENTVOICEERROR_UNEXPECTEDENDOFALTERNATIVE		:	lDescription = CLocalize::LoadString (IDS_AGENTVOICEERROR_UNEXPECTEDENDOFALTERNATIVE); break;
		case AGENTVOICEERROR_UNEXPECTEDENDOFOPTIONAL		:	lDescription = CLocalize::LoadString (IDS_AGENTVOICEERROR_UNEXPECTEDENDOFOPTIONAL); break;
		case AGENTVOICEERROR_UNEXPECTEDALTERNATIVE			:	lDescription = CLocalize::LoadString (IDS_AGENTVOICEERROR_UNEXPECTEDALTERNATIVE); break;
		case AGENTVOICEERROR_SRMODENOTFOUND					:	lDescription = CLocalize::LoadString (IDS_AGENTVOICEERROR_SRMODENOTFOUND); break;
		case AGENTVOICEERROR_SPEECHDISABLED					:	lDescription = CLocalize::LoadString (IDS_AGENTVOICEERROR_SPEECHDISABLED); break;
	}

	return lDescription;
}
