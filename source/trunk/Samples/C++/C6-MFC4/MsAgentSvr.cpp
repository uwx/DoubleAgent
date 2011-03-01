// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "msagentsvr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// IAgent properties

/////////////////////////////////////////////////////////////////////////////
// IAgent operations

void IAgent::Load(const VARIANT& vLoadKey, long* pdwCharID, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &vLoadKey, pdwCharID, pdwReqID);
}

void IAgent::Unload(long dwCharID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID);
}

void IAgent::Register(LPUNKNOWN punkNotifySink, long* pdwSinkID)
{
	static BYTE parms[] =
		VTS_UNKNOWN VTS_PI4;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 punkNotifySink, pdwSinkID);
}

void IAgent::Unregister(long dwSinkID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwSinkID);
}

void IAgent::GetCharacter(long dwCharID, LPDISPATCH* ppunkCharacter)
{
	static BYTE parms[] =
		VTS_I4 VTS_PDISPATCH;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, ppunkCharacter);
}

void IAgent::GetSuspended(long* pbSuspended)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbSuspended);
}


/////////////////////////////////////////////////////////////////////////////
// IAgentUserInput properties

/////////////////////////////////////////////////////////////////////////////
// IAgentUserInput operations

void IAgentUserInput::GetCount(long* pdwCount)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pdwCount);
}

void IAgentUserInput::GetItemID(long dwItemIndex, long* pdwCommandID)
{
	static BYTE parms[] =
		VTS_I4 VTS_PI4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwItemIndex, pdwCommandID);
}

void IAgentUserInput::GetItemConfidence(long dwItemIndex, long* plConfidence)
{
	static BYTE parms[] =
		VTS_I4 VTS_PI4;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwItemIndex, plConfidence);
}

void IAgentUserInput::GetItemText(long dwItemIndex, BSTR* pbszText)
{
	static BYTE parms[] =
		VTS_I4 VTS_PBSTR;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwItemIndex, pbszText);
}

void IAgentUserInput::GetAllItemData(VARIANT* pdwItemIndices, VARIANT* plConfidences, VARIANT* pbszText)
{
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pdwItemIndices, plConfidences, pbszText);
}


/////////////////////////////////////////////////////////////////////////////
// IAgentCommand properties

/////////////////////////////////////////////////////////////////////////////
// IAgentCommand operations

void IAgentCommand::SetCaption(LPCTSTR bszCaption)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszCaption);
}

void IAgentCommand::GetCaption(BSTR* pbszCaption)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszCaption);
}

void IAgentCommand::SetVoice(LPCTSTR bszVoice)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszVoice);
}

void IAgentCommand::GetVoice(BSTR* pbszVoice)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszVoice);
}

void IAgentCommand::SetEnabled(long bEnabled)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bEnabled);
}

void IAgentCommand::GetEnabled(long* pbEnabled)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbEnabled);
}

void IAgentCommand::SetVisible(long bVisible)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bVisible);
}

void IAgentCommand::GetVisible(long* pbVisible)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020007, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbVisible);
}

void IAgentCommand::SetConfidenceThreshold(long lThreshold)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020008, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lThreshold);
}

void IAgentCommand::GetConfidenceThreshold(long* plThreshold)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plThreshold);
}

void IAgentCommand::SetConfidenceText(LPCTSTR bszTipText)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszTipText);
}

void IAgentCommand::GetConfidenceText(BSTR* pbszTipText)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszTipText);
}

void IAgentCommand::GetID(long* pdwID)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x6002000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pdwID);
}


/////////////////////////////////////////////////////////////////////////////
// IAgentCommandEx properties

/////////////////////////////////////////////////////////////////////////////
// IAgentCommandEx operations

void IAgentCommandEx::SetCaption(LPCTSTR bszCaption)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszCaption);
}

void IAgentCommandEx::GetCaption(BSTR* pbszCaption)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszCaption);
}

void IAgentCommandEx::SetVoice(LPCTSTR bszVoice)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszVoice);
}

void IAgentCommandEx::GetVoice(BSTR* pbszVoice)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszVoice);
}

void IAgentCommandEx::SetEnabled(long bEnabled)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bEnabled);
}

void IAgentCommandEx::GetEnabled(long* pbEnabled)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbEnabled);
}

void IAgentCommandEx::SetVisible(long bVisible)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bVisible);
}

void IAgentCommandEx::GetVisible(long* pbVisible)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020007, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbVisible);
}

void IAgentCommandEx::SetConfidenceThreshold(long lThreshold)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020008, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lThreshold);
}

void IAgentCommandEx::GetConfidenceThreshold(long* plThreshold)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plThreshold);
}

void IAgentCommandEx::SetConfidenceText(LPCTSTR bszTipText)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszTipText);
}

void IAgentCommandEx::GetConfidenceText(BSTR* pbszTipText)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszTipText);
}

void IAgentCommandEx::GetID(long* pdwID)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x6002000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pdwID);
}

void IAgentCommandEx::SetHelpContextID(long ulID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ulID);
}

void IAgentCommandEx::GetHelpContextID(long* pulID)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pulID);
}

void IAgentCommandEx::SetVoiceCaption(LPCTSTR bszVoiceCaption)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszVoiceCaption);
}

void IAgentCommandEx::GetVoiceCaption(BSTR* pbszVoiceCaption)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60030003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszVoiceCaption);
}


/////////////////////////////////////////////////////////////////////////////
// IAgentCommands properties

/////////////////////////////////////////////////////////////////////////////
// IAgentCommands operations

void IAgentCommands::GetCommand(long dwCommandID, LPUNKNOWN* ppunkCommand)
{
	static BYTE parms[] =
		VTS_I4 VTS_PUNKNOWN;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCommandID, ppunkCommand);
}

void IAgentCommands::GetCount(long* pdwCount)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pdwCount);
}

void IAgentCommands::SetCaption(LPCTSTR bszCaption)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszCaption);
}

void IAgentCommands::GetCaption(BSTR* pbszCaption)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszCaption);
}

void IAgentCommands::SetVoice(LPCTSTR bszVoice)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszVoice);
}

void IAgentCommands::GetVoice(BSTR* pbszVoice)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszVoice);
}

void IAgentCommands::SetVisible(long bVisible)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bVisible);
}

void IAgentCommands::GetVisible(long* pbVisible)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020007, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbVisible);
}

void IAgentCommands::Add(LPCTSTR bszCaption, LPCTSTR bszVoice, long bEnabled, long bVisible, long* pdwID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_PI4;
	InvokeHelper(0x60020008, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszCaption, bszVoice, bEnabled, bVisible, pdwID);
}

void IAgentCommands::Insert(LPCTSTR bszCaption, LPCTSTR bszVoice, long bEnabled, long bVisible, long dwRefID, long bBefore, long* pdwID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PI4;
	InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszCaption, bszVoice, bEnabled, bVisible, dwRefID, bBefore, pdwID);
}

void IAgentCommands::Remove(long dwID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwID);
}

void IAgentCommands::RemoveAll()
{
	InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// IAgentCommandsEx properties

/////////////////////////////////////////////////////////////////////////////
// IAgentCommandsEx operations

void IAgentCommandsEx::GetCommand(long dwCommandID, LPUNKNOWN* ppunkCommand)
{
	static BYTE parms[] =
		VTS_I4 VTS_PUNKNOWN;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCommandID, ppunkCommand);
}

void IAgentCommandsEx::GetCount(long* pdwCount)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pdwCount);
}

void IAgentCommandsEx::SetCaption(LPCTSTR bszCaption)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszCaption);
}

void IAgentCommandsEx::GetCaption(BSTR* pbszCaption)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszCaption);
}

void IAgentCommandsEx::SetVoice(LPCTSTR bszVoice)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszVoice);
}

void IAgentCommandsEx::GetVoice(BSTR* pbszVoice)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszVoice);
}

void IAgentCommandsEx::SetVisible(long bVisible)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bVisible);
}

void IAgentCommandsEx::GetVisible(long* pbVisible)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020007, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbVisible);
}

void IAgentCommandsEx::Add(LPCTSTR bszCaption, LPCTSTR bszVoice, long bEnabled, long bVisible, long* pdwID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_PI4;
	InvokeHelper(0x60020008, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszCaption, bszVoice, bEnabled, bVisible, pdwID);
}

void IAgentCommandsEx::Insert(LPCTSTR bszCaption, LPCTSTR bszVoice, long bEnabled, long bVisible, long dwRefID, long bBefore, long* pdwID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PI4;
	InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszCaption, bszVoice, bEnabled, bVisible, dwRefID, bBefore, pdwID);
}

void IAgentCommandsEx::Remove(long dwID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwID);
}

void IAgentCommandsEx::RemoveAll()
{
	InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void IAgentCommandsEx::GetCommandEx(long dwCommandID, LPDISPATCH* ppCommandEx)
{
	static BYTE parms[] =
		VTS_I4 VTS_PDISPATCH;
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCommandID, ppCommandEx);
}

void IAgentCommandsEx::SetDefaultID(long dwID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwID);
}

void IAgentCommandsEx::GetDefaultID(long* pdwID)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pdwID);
}

void IAgentCommandsEx::SetHelpContextID(long ulHelpID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ulHelpID);
}

void IAgentCommandsEx::GetHelpContextID(long* pulHelpID)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60030004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pulHelpID);
}

void IAgentCommandsEx::SetFontName(LPCTSTR bszFontName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszFontName);
}

void IAgentCommandsEx::GetFontName(BSTR* pbszFontName)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60030006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszFontName);
}

void IAgentCommandsEx::SetFontSize(long lFontSize)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030007, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lFontSize);
}

void IAgentCommandsEx::GetFontSize(long* lFontSize)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60030008, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lFontSize);
}

void IAgentCommandsEx::SetVoiceCaption(LPCTSTR bszVoiceCaption)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszVoiceCaption);
}

void IAgentCommandsEx::GetVoiceCaption(BSTR* bszVoiceCaption)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x6003000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszVoiceCaption);
}

void IAgentCommandsEx::AddEx(LPCTSTR bszCaption, LPCTSTR bszVoice, LPCTSTR bszVoiceCaption, long bEnabled, long bVisible, long ulHelpID, long* pdwID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_PI4;
	InvokeHelper(0x6003000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszCaption, bszVoice, bszVoiceCaption, bEnabled, bVisible, ulHelpID, pdwID);
}

void IAgentCommandsEx::InsertEx(LPCTSTR bszCaption, LPCTSTR bszVoice, LPCTSTR bszVoiceCaption, long bEnabled, long bVisible, long ulHelpID, long dwRefID, long bBefore, long* pdwID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PI4;
	InvokeHelper(0x6003000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszCaption, bszVoice, bszVoiceCaption, bEnabled, bVisible, ulHelpID, dwRefID, bBefore, pdwID);
}

void IAgentCommandsEx::SetGlobalVoiceCommandsEnabled(long bEnable)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6003000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bEnable);
}

void IAgentCommandsEx::GetGlobalVoiceCommandsEnabled(long* pbEnabled)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x6003000e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbEnabled);
}


/////////////////////////////////////////////////////////////////////////////
// IAgentSpeechInputProperties properties

/////////////////////////////////////////////////////////////////////////////
// IAgentSpeechInputProperties operations

void IAgentSpeechInputProperties::GetInstalled(long* pbInstalled)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbInstalled);
}

void IAgentSpeechInputProperties::GetEnabled(long* pbEnabled)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbEnabled);
}

void IAgentSpeechInputProperties::GetHotKey(BSTR* pbszHotCharKey)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszHotCharKey);
}

void IAgentSpeechInputProperties::GetLCID(unsigned long* plcidCurrent)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plcidCurrent);
}

void IAgentSpeechInputProperties::GetEngine(BSTR* pbszEngine)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszEngine);
}

void IAgentSpeechInputProperties::SetEngine(LPCTSTR bszEngine)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszEngine);
}

void IAgentSpeechInputProperties::GetListeningTip(long* pbListeningTip)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbListeningTip);
}


/////////////////////////////////////////////////////////////////////////////
// IAgentAudioOutputProperties properties

/////////////////////////////////////////////////////////////////////////////
// IAgentAudioOutputProperties operations

void IAgentAudioOutputProperties::GetEnabled(long* pbEnabled)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbEnabled);
}

void IAgentAudioOutputProperties::GetUsingSoundEffects(long* pbUsingSoundEffects)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbUsingSoundEffects);
}


/////////////////////////////////////////////////////////////////////////////
// IAgentAudioOutputPropertiesEx properties

/////////////////////////////////////////////////////////////////////////////
// IAgentAudioOutputPropertiesEx operations

void IAgentAudioOutputPropertiesEx::GetEnabled(long* pbEnabled)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbEnabled);
}

void IAgentAudioOutputPropertiesEx::GetUsingSoundEffects(long* pbUsingSoundEffects)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbUsingSoundEffects);
}

void IAgentAudioOutputPropertiesEx::GetStatus(long* plStatus)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plStatus);
}


/////////////////////////////////////////////////////////////////////////////
// IAgentPropertySheet properties

/////////////////////////////////////////////////////////////////////////////
// IAgentPropertySheet operations

void IAgentPropertySheet::GetVisible(long* pbVisible)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbVisible);
}

void IAgentPropertySheet::SetVisible(long bVisible)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bVisible);
}

void IAgentPropertySheet::GetPosition(long* plLeft, long* plTop)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plLeft, plTop);
}

void IAgentPropertySheet::GetSize(long* plWidth, long* plHeight)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plWidth, plHeight);
}

void IAgentPropertySheet::GetPage(BSTR* pbszPage)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszPage);
}

void IAgentPropertySheet::SetPage(LPCTSTR bszPage)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszPage);
}


/////////////////////////////////////////////////////////////////////////////
// IAgentBalloon properties

/////////////////////////////////////////////////////////////////////////////
// IAgentBalloon operations

void IAgentBalloon::GetEnabled(long* pbEnabled)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbEnabled);
}

void IAgentBalloon::GetNumLines(long* plLines)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plLines);
}

void IAgentBalloon::GetNumCharsPerLine(long* plCharsPerLine)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plCharsPerLine);
}

void IAgentBalloon::GetFontName(BSTR* pbszFontName)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszFontName);
}

void IAgentBalloon::GetFontSize(long* plFontSize)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plFontSize);
}

void IAgentBalloon::GetFontBold(long* pbFontBold)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbFontBold);
}

void IAgentBalloon::GetFontItalic(long* pbFontItalic)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbFontItalic);
}

void IAgentBalloon::GetFontStrikethru(long* pbFontStrikethru)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020007, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbFontStrikethru);
}

void IAgentBalloon::GetFontUnderline(long* pbFontUnderline)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020008, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbFontUnderline);
}

void IAgentBalloon::GetForeColor(long* plFGColor)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plFGColor);
}

void IAgentBalloon::GetBackColor(long* plBGColor)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plBGColor);
}

void IAgentBalloon::GetBorderColor(long* plBorderColor)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plBorderColor);
}

void IAgentBalloon::SetVisible(long bVisible)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bVisible);
}

void IAgentBalloon::GetVisible(long* pbVisible)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x6002000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbVisible);
}

void IAgentBalloon::SetFontName(LPCTSTR bszFontName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6002000e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszFontName);
}

void IAgentBalloon::SetFontSize(long lFontSize)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lFontSize);
}

void IAgentBalloon::SetFontCharSet(short sFontCharSet)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x60020010, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sFontCharSet);
}

void IAgentBalloon::GetFontCharSet(short* psFontCharSet)
{
	static BYTE parms[] =
		VTS_PI2;
	InvokeHelper(0x60020011, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 psFontCharSet);
}


/////////////////////////////////////////////////////////////////////////////
// IAgentBalloonEx properties

/////////////////////////////////////////////////////////////////////////////
// IAgentBalloonEx operations

void IAgentBalloonEx::GetEnabled(long* pbEnabled)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbEnabled);
}

void IAgentBalloonEx::GetNumLines(long* plLines)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plLines);
}

void IAgentBalloonEx::GetNumCharsPerLine(long* plCharsPerLine)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plCharsPerLine);
}

void IAgentBalloonEx::GetFontName(BSTR* pbszFontName)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszFontName);
}

void IAgentBalloonEx::GetFontSize(long* plFontSize)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plFontSize);
}

void IAgentBalloonEx::GetFontBold(long* pbFontBold)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbFontBold);
}

void IAgentBalloonEx::GetFontItalic(long* pbFontItalic)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbFontItalic);
}

void IAgentBalloonEx::GetFontStrikethru(long* pbFontStrikethru)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020007, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbFontStrikethru);
}

void IAgentBalloonEx::GetFontUnderline(long* pbFontUnderline)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020008, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbFontUnderline);
}

void IAgentBalloonEx::GetForeColor(long* plFGColor)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plFGColor);
}

void IAgentBalloonEx::GetBackColor(long* plBGColor)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plBGColor);
}

void IAgentBalloonEx::GetBorderColor(long* plBorderColor)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plBorderColor);
}

void IAgentBalloonEx::SetVisible(long bVisible)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bVisible);
}

void IAgentBalloonEx::GetVisible(long* pbVisible)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x6002000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbVisible);
}

void IAgentBalloonEx::SetFontName(LPCTSTR bszFontName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6002000e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszFontName);
}

void IAgentBalloonEx::SetFontSize(long lFontSize)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lFontSize);
}

void IAgentBalloonEx::SetFontCharSet(short sFontCharSet)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x60020010, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sFontCharSet);
}

void IAgentBalloonEx::GetFontCharSet(short* psFontCharSet)
{
	static BYTE parms[] =
		VTS_PI2;
	InvokeHelper(0x60020011, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 psFontCharSet);
}

void IAgentBalloonEx::SetStyle(long lStyle)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lStyle);
}

void IAgentBalloonEx::GetStyle(long* plStyle)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plStyle);
}

void IAgentBalloonEx::SetNumLines(long lLines)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lLines);
}

void IAgentBalloonEx::SetNumCharsPerLine(long lCharsPerLine)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lCharsPerLine);
}


/////////////////////////////////////////////////////////////////////////////
// IAgentCharacter properties

/////////////////////////////////////////////////////////////////////////////
// IAgentCharacter operations

void IAgentCharacter::GetVisible(long* pbVisible)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbVisible);
}

void IAgentCharacter::SetPosition(long lLeft, long lTop)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lLeft, lTop);
}

void IAgentCharacter::GetPosition(long* plLeft, long* plTop)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plLeft, plTop);
}

void IAgentCharacter::SetSize(long lWidth, long lHeight)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lWidth, lHeight);
}

void IAgentCharacter::GetSize(long* plWidth, long* plHeight)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plWidth, plHeight);
}

void IAgentCharacter::GetName(BSTR* pbszName)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszName);
}

void IAgentCharacter::GetDescription(BSTR* pbszDescription)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszDescription);
}

void IAgentCharacter::GetTTSSpeed(long* pdwSpeed)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020007, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pdwSpeed);
}

void IAgentCharacter::GetTTSPitch(short* pwPitch)
{
	static BYTE parms[] =
		VTS_PI2;
	InvokeHelper(0x60020008, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pwPitch);
}

void IAgentCharacter::Activate(short sState)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sState);
}

void IAgentCharacter::SetIdleOn(long bOn)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bOn);
}

void IAgentCharacter::GetIdleOn(long* pbOn)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbOn);
}

void IAgentCharacter::Prepare(long dwType, LPCTSTR bszName, long bQueue, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR VTS_I4 VTS_PI4;
	InvokeHelper(0x6002000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwType, bszName, bQueue, pdwReqID);
}

void IAgentCharacter::Play(LPCTSTR bszAnimation, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_PI4;
	InvokeHelper(0x6002000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszAnimation, pdwReqID);
}

void IAgentCharacter::Stop(long dwReqID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwReqID);
}

void IAgentCharacter::StopAll(long lTypes)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lTypes);
}

void IAgentCharacter::Wait(long dwReqID, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_I4 VTS_PI4;
	InvokeHelper(0x60020010, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwReqID, pdwReqID);
}

void IAgentCharacter::Interrupt(long dwReqID, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_I4 VTS_PI4;
	InvokeHelper(0x60020011, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwReqID, pdwReqID);
}

void IAgentCharacter::Show(long bFast, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_I4 VTS_PI4;
	InvokeHelper(0x60020012, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bFast, pdwReqID);
}

void IAgentCharacter::Hide(long bFast, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_I4 VTS_PI4;
	InvokeHelper(0x60020013, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bFast, pdwReqID);
}

void IAgentCharacter::Speak(LPCTSTR bszText, LPCTSTR bszUrl, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_PI4;
	InvokeHelper(0x60020014, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszText, bszUrl, pdwReqID);
}

void IAgentCharacter::MoveTo(short x, short y, long lSpeed, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I4 VTS_PI4;
	InvokeHelper(0x60020015, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 x, y, lSpeed, pdwReqID);
}

void IAgentCharacter::GestureAt(short x, short y, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_PI4;
	InvokeHelper(0x60020016, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 x, y, pdwReqID);
}

void IAgentCharacter::GetMoveCause(long* pdwCause)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020017, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pdwCause);
}

void IAgentCharacter::GetVisibilityCause(long* pdwCause)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020018, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pdwCause);
}

void IAgentCharacter::HasOtherClients(long* plNumOtherClients)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020019, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plNumOtherClients);
}

void IAgentCharacter::SetSoundEffectsOn(long bOn)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002001a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bOn);
}

void IAgentCharacter::GetSoundEffectsOn(long* pbOn)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x6002001b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbOn);
}

void IAgentCharacter::SetName(LPCTSTR bszName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6002001c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszName);
}

void IAgentCharacter::SetDescription(LPCTSTR bszDescription)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6002001d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszDescription);
}

void IAgentCharacter::GetExtraData(BSTR* pbszExtraData)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x6002001e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszExtraData);
}


/////////////////////////////////////////////////////////////////////////////
// IAgentCharacterEx properties

/////////////////////////////////////////////////////////////////////////////
// IAgentCharacterEx operations

void IAgentCharacterEx::GetVisible(long* pbVisible)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbVisible);
}

void IAgentCharacterEx::SetPosition(long lLeft, long lTop)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lLeft, lTop);
}

void IAgentCharacterEx::GetPosition(long* plLeft, long* plTop)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plLeft, plTop);
}

void IAgentCharacterEx::SetSize(long lWidth, long lHeight)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lWidth, lHeight);
}

void IAgentCharacterEx::GetSize(long* plWidth, long* plHeight)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plWidth, plHeight);
}

void IAgentCharacterEx::GetName(BSTR* pbszName)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszName);
}

void IAgentCharacterEx::GetDescription(BSTR* pbszDescription)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszDescription);
}

void IAgentCharacterEx::GetTTSSpeed(long* pdwSpeed)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020007, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pdwSpeed);
}

void IAgentCharacterEx::GetTTSPitch(short* pwPitch)
{
	static BYTE parms[] =
		VTS_PI2;
	InvokeHelper(0x60020008, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pwPitch);
}

void IAgentCharacterEx::Activate(short sState)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sState);
}

void IAgentCharacterEx::SetIdleOn(long bOn)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bOn);
}

void IAgentCharacterEx::GetIdleOn(long* pbOn)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbOn);
}

void IAgentCharacterEx::Prepare(long dwType, LPCTSTR bszName, long bQueue, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR VTS_I4 VTS_PI4;
	InvokeHelper(0x6002000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwType, bszName, bQueue, pdwReqID);
}

void IAgentCharacterEx::Play(LPCTSTR bszAnimation, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_PI4;
	InvokeHelper(0x6002000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszAnimation, pdwReqID);
}

void IAgentCharacterEx::Stop(long dwReqID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwReqID);
}

void IAgentCharacterEx::StopAll(long lTypes)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lTypes);
}

void IAgentCharacterEx::Wait(long dwReqID, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_I4 VTS_PI4;
	InvokeHelper(0x60020010, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwReqID, pdwReqID);
}

void IAgentCharacterEx::Interrupt(long dwReqID, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_I4 VTS_PI4;
	InvokeHelper(0x60020011, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwReqID, pdwReqID);
}

void IAgentCharacterEx::Show(long bFast, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_I4 VTS_PI4;
	InvokeHelper(0x60020012, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bFast, pdwReqID);
}

void IAgentCharacterEx::Hide(long bFast, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_I4 VTS_PI4;
	InvokeHelper(0x60020013, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bFast, pdwReqID);
}

void IAgentCharacterEx::Speak(LPCTSTR bszText, LPCTSTR bszUrl, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_PI4;
	InvokeHelper(0x60020014, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszText, bszUrl, pdwReqID);
}

void IAgentCharacterEx::MoveTo(short x, short y, long lSpeed, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I4 VTS_PI4;
	InvokeHelper(0x60020015, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 x, y, lSpeed, pdwReqID);
}

void IAgentCharacterEx::GestureAt(short x, short y, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_PI4;
	InvokeHelper(0x60020016, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 x, y, pdwReqID);
}

void IAgentCharacterEx::GetMoveCause(long* pdwCause)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020017, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pdwCause);
}

void IAgentCharacterEx::GetVisibilityCause(long* pdwCause)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020018, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pdwCause);
}

void IAgentCharacterEx::HasOtherClients(long* plNumOtherClients)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020019, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plNumOtherClients);
}

void IAgentCharacterEx::SetSoundEffectsOn(long bOn)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002001a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bOn);
}

void IAgentCharacterEx::GetSoundEffectsOn(long* pbOn)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x6002001b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbOn);
}

void IAgentCharacterEx::SetName(LPCTSTR bszName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6002001c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszName);
}

void IAgentCharacterEx::SetDescription(LPCTSTR bszDescription)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6002001d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszDescription);
}

void IAgentCharacterEx::GetExtraData(BSTR* pbszExtraData)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x6002001e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszExtraData);
}

void IAgentCharacterEx::ShowPopupMenu(short x, short y)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 x, y);
}

void IAgentCharacterEx::SetAutoPopupMenu(long bAutoPopupMenu)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bAutoPopupMenu);
}

void IAgentCharacterEx::GetAutoPopupMenu(long* pbAutoPopupMenu)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbAutoPopupMenu);
}

void IAgentCharacterEx::GetHelpFileName(BSTR* pbszName)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60030003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszName);
}

void IAgentCharacterEx::SetHelpFileName(LPCTSTR bszName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszName);
}

void IAgentCharacterEx::SetHelpModeOn(long bHelpModeOn)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bHelpModeOn);
}

void IAgentCharacterEx::GetHelpModeOn(long* pbHelpModeOn)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60030006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbHelpModeOn);
}

void IAgentCharacterEx::SetHelpContextID(long ulID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030007, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ulID);
}

void IAgentCharacterEx::GetHelpContextID(long* pulID)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60030008, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pulID);
}

void IAgentCharacterEx::GetActive(short* psState)
{
	static BYTE parms[] =
		VTS_PI2;
	InvokeHelper(0x60030009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 psState);
}

void IAgentCharacterEx::Listen(long bListen)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6003000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bListen);
}

void IAgentCharacterEx::SetLanguageID(long langid)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6003000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 langid);
}

void IAgentCharacterEx::GetLanguageID(long* plangid)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x6003000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plangid);
}

void IAgentCharacterEx::GetTTSModeID(BSTR* pbszModeID)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x6003000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszModeID);
}

void IAgentCharacterEx::SetTTSModeID(LPCTSTR bszModeID)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6003000e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszModeID);
}

void IAgentCharacterEx::GetSRModeID(BSTR* pbszModeID)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x6003000f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszModeID);
}

void IAgentCharacterEx::SetSRModeID(LPCTSTR bszModeID)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030010, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszModeID);
}

void IAgentCharacterEx::GetGUID(BSTR* pbszID)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60030011, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszID);
}

void IAgentCharacterEx::GetOriginalSize(long* plWidth, long* plHeight)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60030012, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plWidth, plHeight);
}

void IAgentCharacterEx::Think(LPCTSTR bszText, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_PI4;
	InvokeHelper(0x60030013, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszText, pdwReqID);
}

void IAgentCharacterEx::GetVersion(short* psMajor, short* psMinor)
{
	static BYTE parms[] =
		VTS_PI2 VTS_PI2;
	InvokeHelper(0x60030014, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 psMajor, psMinor);
}

void IAgentCharacterEx::GetAnimationNames(LPUNKNOWN* punkEnum)
{
	static BYTE parms[] =
		VTS_PUNKNOWN;
	InvokeHelper(0x60030015, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 punkEnum);
}

void IAgentCharacterEx::GetSRStatus(long* plStatus)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60030016, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plStatus);
}


/////////////////////////////////////////////////////////////////////////////
// IAgentEx properties

/////////////////////////////////////////////////////////////////////////////
// IAgentEx operations

void IAgentEx::Load(const VARIANT& vLoadKey, long* pdwCharID, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &vLoadKey, pdwCharID, pdwReqID);
}

void IAgentEx::Unload(long dwCharID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID);
}

void IAgentEx::Register(LPUNKNOWN punkNotifySink, long* pdwSinkID)
{
	static BYTE parms[] =
		VTS_UNKNOWN VTS_PI4;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 punkNotifySink, pdwSinkID);
}

void IAgentEx::Unregister(long dwSinkID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwSinkID);
}

void IAgentEx::GetCharacter(long dwCharID, LPDISPATCH* ppunkCharacter)
{
	static BYTE parms[] =
		VTS_I4 VTS_PDISPATCH;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, ppunkCharacter);
}

void IAgentEx::GetSuspended(long* pbSuspended)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbSuspended);
}

void IAgentEx::GetCharacterEx(long dwCharID, LPDISPATCH* ppCharacterEx)
{
	static BYTE parms[] =
		VTS_I4 VTS_PDISPATCH;
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, ppCharacterEx);
}

void IAgentEx::GetVersion(short* psMajor, short* psMinor)
{
	static BYTE parms[] =
		VTS_PI2 VTS_PI2;
	InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 psMajor, psMinor);
}

void IAgentEx::ShowDefaultCharacterProperties(short x, short y, long bUseDefaultPosition)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I4;
	InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 x, y, bUseDefaultPosition);
}


/////////////////////////////////////////////////////////////////////////////
// IAgentNotifySink properties

/////////////////////////////////////////////////////////////////////////////
// IAgentNotifySink operations

void IAgentNotifySink::Command(long dwCommandID, LPUNKNOWN punkUserInput)
{
	static BYTE parms[] =
		VTS_I4 VTS_UNKNOWN;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCommandID, punkUserInput);
}

void IAgentNotifySink::ActivateInputState(long dwCharID, long bActivated)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, bActivated);
}

void IAgentNotifySink::Restart()
{
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void IAgentNotifySink::Shutdown()
{
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void IAgentNotifySink::VisibleState(long dwCharID, long bVisible, long dwCause)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, bVisible, dwCause);
}

void IAgentNotifySink::Click(long dwCharID, short fwKeys, long x, long y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I2 VTS_I4 VTS_I4;
	InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, fwKeys, x, y);
}

void IAgentNotifySink::DblClick(long dwCharID, short fwKeys, long x, long y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I2 VTS_I4 VTS_I4;
	InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, fwKeys, x, y);
}

void IAgentNotifySink::DragStart(long dwCharID, short fwKeys, long x, long y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I2 VTS_I4 VTS_I4;
	InvokeHelper(0x60020007, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, fwKeys, x, y);
}

void IAgentNotifySink::DragComplete(long dwCharID, short fwKeys, long x, long y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I2 VTS_I4 VTS_I4;
	InvokeHelper(0x60020008, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, fwKeys, x, y);
}

void IAgentNotifySink::RequestStart(long dwRequestID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwRequestID);
}

void IAgentNotifySink::RequestComplete(long dwRequestID, long hrStatus)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwRequestID, hrStatus);
}

void IAgentNotifySink::BookMark(long dwBookMarkID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwBookMarkID);
}

void IAgentNotifySink::Idle(long dwCharID, long bStart)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x6002000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, bStart);
}

void IAgentNotifySink::Move(long dwCharID, long x, long y, long dwCause)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x6002000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, x, y, dwCause);
}

void IAgentNotifySink::Size(long dwCharID, long lWidth, long lHeight)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x6002000e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, lWidth, lHeight);
}

void IAgentNotifySink::BalloonVisibleState(long dwCharID, long bVisible)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x6002000f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, bVisible);
}


/////////////////////////////////////////////////////////////////////////////
// IAgentNotifySinkEx properties

/////////////////////////////////////////////////////////////////////////////
// IAgentNotifySinkEx operations

void IAgentNotifySinkEx::Command(long dwCommandID, LPUNKNOWN punkUserInput)
{
	static BYTE parms[] =
		VTS_I4 VTS_UNKNOWN;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCommandID, punkUserInput);
}

void IAgentNotifySinkEx::ActivateInputState(long dwCharID, long bActivated)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, bActivated);
}

void IAgentNotifySinkEx::Restart()
{
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void IAgentNotifySinkEx::Shutdown()
{
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void IAgentNotifySinkEx::VisibleState(long dwCharID, long bVisible, long dwCause)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, bVisible, dwCause);
}

void IAgentNotifySinkEx::Click(long dwCharID, short fwKeys, long x, long y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I2 VTS_I4 VTS_I4;
	InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, fwKeys, x, y);
}

void IAgentNotifySinkEx::DblClick(long dwCharID, short fwKeys, long x, long y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I2 VTS_I4 VTS_I4;
	InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, fwKeys, x, y);
}

void IAgentNotifySinkEx::DragStart(long dwCharID, short fwKeys, long x, long y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I2 VTS_I4 VTS_I4;
	InvokeHelper(0x60020007, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, fwKeys, x, y);
}

void IAgentNotifySinkEx::DragComplete(long dwCharID, short fwKeys, long x, long y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I2 VTS_I4 VTS_I4;
	InvokeHelper(0x60020008, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, fwKeys, x, y);
}

void IAgentNotifySinkEx::RequestStart(long dwRequestID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwRequestID);
}

void IAgentNotifySinkEx::RequestComplete(long dwRequestID, long hrStatus)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwRequestID, hrStatus);
}

void IAgentNotifySinkEx::BookMark(long dwBookMarkID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwBookMarkID);
}

void IAgentNotifySinkEx::Idle(long dwCharID, long bStart)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x6002000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, bStart);
}

void IAgentNotifySinkEx::Move(long dwCharID, long x, long y, long dwCause)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x6002000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, x, y, dwCause);
}

void IAgentNotifySinkEx::Size(long dwCharID, long lWidth, long lHeight)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x6002000e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, lWidth, lHeight);
}

void IAgentNotifySinkEx::BalloonVisibleState(long dwCharID, long bVisible)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x6002000f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, bVisible);
}

void IAgentNotifySinkEx::HelpComplete(long dwCharID, long dwCommandID, long dwCause)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, dwCommandID, dwCause);
}

void IAgentNotifySinkEx::ListeningState(long dwCharID, long bListening, long dwCause)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, bListening, dwCause);
}

void IAgentNotifySinkEx::DefaultCharacterChange(LPCTSTR bszGUID)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszGUID);
}

void IAgentNotifySinkEx::AgentPropertyChange()
{
	InvokeHelper(0x60030003, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void IAgentNotifySinkEx::ActiveClientChange(long dwCharID, long lStatus)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x60030004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, lStatus);
}


/////////////////////////////////////////////////////////////////////////////
// IAgentCommandWindow properties

/////////////////////////////////////////////////////////////////////////////
// IAgentCommandWindow operations

void IAgentCommandWindow::SetVisible(long bVisible)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bVisible);
}

void IAgentCommandWindow::GetVisible(long* pbVisible)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbVisible);
}

void IAgentCommandWindow::GetPosition(long* plLeft, long* plTop)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plLeft, plTop);
}

void IAgentCommandWindow::GetSize(long* plWidth, long* plHeight)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plWidth, plHeight);
}
