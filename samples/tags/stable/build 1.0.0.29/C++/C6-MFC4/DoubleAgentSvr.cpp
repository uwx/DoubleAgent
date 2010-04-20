// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "doubleagentsvr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// IDaServer properties

/////////////////////////////////////////////////////////////////////////////
// IDaServer operations

void IDaServer::Load(const VARIANT& vLoadKey, long* pdwCharID, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &vLoadKey, pdwCharID, pdwReqID);
}

void IDaServer::Unload(long dwCharID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID);
}

void IDaServer::Register(LPUNKNOWN punkNotifySink, long* pdwSinkID)
{
	static BYTE parms[] =
		VTS_UNKNOWN VTS_PI4;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 punkNotifySink, pdwSinkID);
}

void IDaServer::Unregister(long dwSinkID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwSinkID);
}

void IDaServer::GetCharacter(long dwCharID, LPDISPATCH* ppunkCharacter)
{
	static BYTE parms[] =
		VTS_I4 VTS_PDISPATCH;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, ppunkCharacter);
}

void IDaServer::GetCharacterEx(long dwCharID, LPDISPATCH* ppCharacterEx)
{
	static BYTE parms[] =
		VTS_I4 VTS_PDISPATCH;
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, ppCharacterEx);
}

void IDaServer::GetVersion(short* psMajor, short* psMinor)
{
	static BYTE parms[] =
		VTS_PI2 VTS_PI2;
	InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 psMajor, psMinor);
}

void IDaServer::ShowDefaultCharacterProperties(short x, short y, long bUseDefaultPosition)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I4;
	InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 x, y, bUseDefaultPosition);
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrCharacter properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrCharacter operations

void IDaSvrCharacter::GetVisible(long* pbVisible)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbVisible);
}

void IDaSvrCharacter::SetPosition(long lLeft, long lTop)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lLeft, lTop);
}

void IDaSvrCharacter::GetPosition(long* plLeft, long* plTop)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plLeft, plTop);
}

void IDaSvrCharacter::SetSize(long lWidth, long lHeight)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lWidth, lHeight);
}

void IDaSvrCharacter::GetSize(long* plWidth, long* plHeight)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plWidth, plHeight);
}

void IDaSvrCharacter::GetName(BSTR* pbszName)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszName);
}

void IDaSvrCharacter::GetDescription(BSTR* pbszDescription)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszDescription);
}

void IDaSvrCharacter::GetTTSSpeed(long* pdwSpeed)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020007, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pdwSpeed);
}

void IDaSvrCharacter::GetTTSPitch(short* pwPitch)
{
	static BYTE parms[] =
		VTS_PI2;
	InvokeHelper(0x60020008, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pwPitch);
}

void IDaSvrCharacter::Activate(short sState)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sState);
}

void IDaSvrCharacter::SetIdleOn(long bOn)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bOn);
}

void IDaSvrCharacter::GetIdleOn(long* pbOn)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbOn);
}

void IDaSvrCharacter::Prepare(long dwType, LPCTSTR bszName, long bQueue, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR VTS_I4 VTS_PI4;
	InvokeHelper(0x6002000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwType, bszName, bQueue, pdwReqID);
}

void IDaSvrCharacter::Play(LPCTSTR bszAnimation, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_PI4;
	InvokeHelper(0x6002000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszAnimation, pdwReqID);
}

void IDaSvrCharacter::Stop(long dwReqID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwReqID);
}

void IDaSvrCharacter::StopAll(long lTypes)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lTypes);
}

void IDaSvrCharacter::Wait(long dwReqID, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_I4 VTS_PI4;
	InvokeHelper(0x60020010, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwReqID, pdwReqID);
}

void IDaSvrCharacter::Interrupt(long dwReqID, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_I4 VTS_PI4;
	InvokeHelper(0x60020011, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwReqID, pdwReqID);
}

void IDaSvrCharacter::Show(long bFast, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_I4 VTS_PI4;
	InvokeHelper(0x60020012, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bFast, pdwReqID);
}

void IDaSvrCharacter::Hide(long bFast, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_I4 VTS_PI4;
	InvokeHelper(0x60020013, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bFast, pdwReqID);
}

void IDaSvrCharacter::Speak(LPCTSTR bszText, LPCTSTR bszUrl, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_PI4;
	InvokeHelper(0x60020014, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszText, bszUrl, pdwReqID);
}

void IDaSvrCharacter::MoveTo(short x, short y, long lSpeed, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I4 VTS_PI4;
	InvokeHelper(0x60020015, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 x, y, lSpeed, pdwReqID);
}

void IDaSvrCharacter::GestureAt(short x, short y, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_PI4;
	InvokeHelper(0x60020016, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 x, y, pdwReqID);
}

void IDaSvrCharacter::GetMoveCause(long* pdwCause)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020017, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pdwCause);
}

void IDaSvrCharacter::GetVisibilityCause(long* pdwCause)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020018, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pdwCause);
}

void IDaSvrCharacter::HasOtherClients(long* plNumOtherClients)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020019, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plNumOtherClients);
}

void IDaSvrCharacter::SetSoundEffectsOn(long bOn)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002001a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bOn);
}

void IDaSvrCharacter::GetSoundEffectsOn(long* pbOn)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x6002001b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbOn);
}

void IDaSvrCharacter::SetName(LPCTSTR bszName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6002001c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszName);
}

void IDaSvrCharacter::SetDescription(LPCTSTR bszDescription)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6002001d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszDescription);
}

void IDaSvrCharacter::GetExtraData(BSTR* pbszExtraData)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x6002001e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszExtraData);
}

void IDaSvrCharacter::ShowPopupMenu(short x, short y)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 x, y);
}

void IDaSvrCharacter::SetAutoPopupMenu(long bAutoPopupMenu)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bAutoPopupMenu);
}

void IDaSvrCharacter::GetAutoPopupMenu(long* pbAutoPopupMenu)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbAutoPopupMenu);
}

void IDaSvrCharacter::GetActive(short* psState)
{
	static BYTE parms[] =
		VTS_PI2;
	InvokeHelper(0x60030009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 psState);
}

void IDaSvrCharacter::Listen(long bListen)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6003000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bListen);
}

void IDaSvrCharacter::SetLanguageID(long langid)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6003000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 langid);
}

void IDaSvrCharacter::GetLanguageID(long* plangid)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x6003000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plangid);
}

void IDaSvrCharacter::GetTTSModeID(BSTR* pbszModeID)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x6003000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszModeID);
}

void IDaSvrCharacter::SetTTSModeID(LPCTSTR bszModeID)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6003000e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszModeID);
}

void IDaSvrCharacter::GetSRModeID(BSTR* pbszModeID)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x6003000f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszModeID);
}

void IDaSvrCharacter::SetSRModeID(LPCTSTR bszModeID)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030010, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszModeID);
}

void IDaSvrCharacter::GetGUID(BSTR* pbszID)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60030011, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszID);
}

void IDaSvrCharacter::GetOriginalSize(long* plWidth, long* plHeight)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60030012, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plWidth, plHeight);
}

void IDaSvrCharacter::Think(LPCTSTR bszText, long* pdwReqID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_PI4;
	InvokeHelper(0x60030013, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszText, pdwReqID);
}

void IDaSvrCharacter::GetVersion(short* psMajor, short* psMinor)
{
	static BYTE parms[] =
		VTS_PI2 VTS_PI2;
	InvokeHelper(0x60030014, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 psMajor, psMinor);
}

void IDaSvrCharacter::GetAnimationNames(LPUNKNOWN* punkEnum)
{
	static BYTE parms[] =
		VTS_PUNKNOWN;
	InvokeHelper(0x60030015, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 punkEnum);
}

void IDaSvrCharacter::GetSRStatus(long* plStatus)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60030016, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plStatus);
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrUserInput properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrUserInput operations

void IDaSvrUserInput::GetCount(long* pdwCount)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pdwCount);
}

void IDaSvrUserInput::GetItemID(long dwItemIndex, long* pdwCommandID)
{
	static BYTE parms[] =
		VTS_I4 VTS_PI4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwItemIndex, pdwCommandID);
}

void IDaSvrUserInput::GetItemConfidence(long dwItemIndex, long* plConfidence)
{
	static BYTE parms[] =
		VTS_I4 VTS_PI4;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwItemIndex, plConfidence);
}

void IDaSvrUserInput::GetItemText(long dwItemIndex, BSTR* pbszText)
{
	static BYTE parms[] =
		VTS_I4 VTS_PBSTR;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwItemIndex, pbszText);
}

void IDaSvrUserInput::GetAllItemData(VARIANT* pdwItemIndices, VARIANT* plConfidences, VARIANT* pbszText)
{
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pdwItemIndices, plConfidences, pbszText);
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrCommand properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrCommand operations

void IDaSvrCommand::SetCaption(LPCTSTR bszCaption)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszCaption);
}

void IDaSvrCommand::GetCaption(BSTR* pbszCaption)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszCaption);
}

void IDaSvrCommand::SetVoice(LPCTSTR bszVoice)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszVoice);
}

void IDaSvrCommand::GetVoice(BSTR* pbszVoice)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszVoice);
}

void IDaSvrCommand::SetEnabled(long bEnabled)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bEnabled);
}

void IDaSvrCommand::GetEnabled(long* pbEnabled)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbEnabled);
}

void IDaSvrCommand::SetVisible(long bVisible)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bVisible);
}

void IDaSvrCommand::GetVisible(long* pbVisible)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020007, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbVisible);
}

void IDaSvrCommand::SetConfidenceThreshold(long lThreshold)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020008, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lThreshold);
}

void IDaSvrCommand::GetConfidenceThreshold(long* plThreshold)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plThreshold);
}

void IDaSvrCommand::SetConfidenceText(LPCTSTR bszTipText)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszTipText);
}

void IDaSvrCommand::GetConfidenceText(BSTR* pbszTipText)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszTipText);
}

void IDaSvrCommand::GetID(long* pdwID)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x6002000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pdwID);
}

void IDaSvrCommand::SetVoiceCaption(LPCTSTR bszVoiceCaption)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszVoiceCaption);
}

void IDaSvrCommand::GetVoiceCaption(BSTR* pbszVoiceCaption)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60030003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszVoiceCaption);
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrCommands properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrCommands operations

void IDaSvrCommands::GetCommand(long dwCommandID, LPUNKNOWN* ppunkCommand)
{
	static BYTE parms[] =
		VTS_I4 VTS_PUNKNOWN;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCommandID, ppunkCommand);
}

void IDaSvrCommands::GetCount(long* pdwCount)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pdwCount);
}

void IDaSvrCommands::SetCaption(LPCTSTR bszCaption)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszCaption);
}

void IDaSvrCommands::GetCaption(BSTR* pbszCaption)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszCaption);
}

void IDaSvrCommands::SetVoice(LPCTSTR bszVoice)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszVoice);
}

void IDaSvrCommands::GetVoice(BSTR* pbszVoice)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszVoice);
}

void IDaSvrCommands::SetVisible(long bVisible)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bVisible);
}

void IDaSvrCommands::GetVisible(long* pbVisible)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020007, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbVisible);
}

void IDaSvrCommands::Add(LPCTSTR bszCaption, LPCTSTR bszVoice, long bEnabled, long bVisible, long* pdwID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_PI4;
	InvokeHelper(0x60020008, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszCaption, bszVoice, bEnabled, bVisible, pdwID);
}

void IDaSvrCommands::Insert(LPCTSTR bszCaption, LPCTSTR bszVoice, long bEnabled, long bVisible, long dwRefID, long bBefore, long* pdwID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PI4;
	InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszCaption, bszVoice, bEnabled, bVisible, dwRefID, bBefore, pdwID);
}

void IDaSvrCommands::Remove(long dwID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwID);
}

void IDaSvrCommands::RemoveAll()
{
	InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void IDaSvrCommands::GetCommandEx(long dwCommandID, LPDISPATCH* ppCommandEx)
{
	static BYTE parms[] =
		VTS_I4 VTS_PDISPATCH;
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCommandID, ppCommandEx);
}

void IDaSvrCommands::SetDefaultID(long dwID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwID);
}

void IDaSvrCommands::GetDefaultID(long* pdwID)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pdwID);
}

void IDaSvrCommands::SetFontName(LPCTSTR bszFontName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszFontName);
}

void IDaSvrCommands::GetFontName(BSTR* pbszFontName)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60030006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszFontName);
}

void IDaSvrCommands::SetFontSize(long lFontSize)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030007, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lFontSize);
}

void IDaSvrCommands::GetFontSize(long* lFontSize)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60030008, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lFontSize);
}

void IDaSvrCommands::SetVoiceCaption(LPCTSTR bszVoiceCaption)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszVoiceCaption);
}

void IDaSvrCommands::GetVoiceCaption(BSTR* bszVoiceCaption)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x6003000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszVoiceCaption);
}

void IDaSvrCommands::AddEx(LPCTSTR bszCaption, LPCTSTR bszVoice, LPCTSTR bszVoiceCaption, long bEnabled, long bVisible, long ulHelpID, long* pdwID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_PI4;
	InvokeHelper(0x6003000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszCaption, bszVoice, bszVoiceCaption, bEnabled, bVisible, ulHelpID, pdwID);
}

void IDaSvrCommands::InsertEx(LPCTSTR bszCaption, LPCTSTR bszVoice, LPCTSTR bszVoiceCaption, long bEnabled, long bVisible, long ulHelpID, long dwRefID, long bBefore, long* pdwID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PI4;
	InvokeHelper(0x6003000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszCaption, bszVoice, bszVoiceCaption, bEnabled, bVisible, ulHelpID, dwRefID, bBefore, pdwID);
}

void IDaSvrCommands::SetGlobalVoiceCommandsEnabled(long bEnable)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6003000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bEnable);
}

void IDaSvrCommands::GetGlobalVoiceCommandsEnabled(long* pbEnabled)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x6003000e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbEnabled);
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrSpeechInputProperties properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrSpeechInputProperties operations

void IDaSvrSpeechInputProperties::GetEnabled(long* pbEnabled)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbEnabled);
}

void IDaSvrSpeechInputProperties::GetHotKey(BSTR* pbszHotCharKey)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszHotCharKey);
}

void IDaSvrSpeechInputProperties::GetListeningTip(long* pbListeningTip)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbListeningTip);
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrAudioOutputProperties properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrAudioOutputProperties operations

void IDaSvrAudioOutputProperties::GetEnabled(long* pbEnabled)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbEnabled);
}

void IDaSvrAudioOutputProperties::GetUsingSoundEffects(long* pbUsingSoundEffects)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbUsingSoundEffects);
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrPropertySheet properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrPropertySheet operations

void IDaSvrPropertySheet::GetVisible(long* pbVisible)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbVisible);
}

void IDaSvrPropertySheet::SetVisible(long bVisible)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bVisible);
}

void IDaSvrPropertySheet::GetPosition(long* plLeft, long* plTop)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plLeft, plTop);
}

void IDaSvrPropertySheet::GetSize(long* plWidth, long* plHeight)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plWidth, plHeight);
}

void IDaSvrPropertySheet::GetPage(BSTR* pbszPage)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszPage);
}

void IDaSvrPropertySheet::SetPage(LPCTSTR bszPage)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszPage);
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrBalloon properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrBalloon operations

void IDaSvrBalloon::GetEnabled(long* pbEnabled)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbEnabled);
}

void IDaSvrBalloon::GetNumLines(long* plLines)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plLines);
}

void IDaSvrBalloon::GetNumCharsPerLine(long* plCharsPerLine)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plCharsPerLine);
}

void IDaSvrBalloon::GetFontName(BSTR* pbszFontName)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbszFontName);
}

void IDaSvrBalloon::GetFontSize(long* plFontSize)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plFontSize);
}

void IDaSvrBalloon::GetFontBold(long* pbFontBold)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbFontBold);
}

void IDaSvrBalloon::GetFontItalic(long* pbFontItalic)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbFontItalic);
}

void IDaSvrBalloon::GetFontStrikethru(long* pbFontStrikethru)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020007, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbFontStrikethru);
}

void IDaSvrBalloon::GetFontUnderline(long* pbFontUnderline)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020008, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbFontUnderline);
}

void IDaSvrBalloon::GetForeColor(long* plFGColor)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plFGColor);
}

void IDaSvrBalloon::GetBackColor(long* plBGColor)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plBGColor);
}

void IDaSvrBalloon::GetBorderColor(long* plBorderColor)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plBorderColor);
}

void IDaSvrBalloon::SetVisible(long bVisible)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bVisible);
}

void IDaSvrBalloon::GetVisible(long* pbVisible)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x6002000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbVisible);
}

void IDaSvrBalloon::SetFontName(LPCTSTR bszFontName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6002000e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszFontName);
}

void IDaSvrBalloon::SetFontSize(long lFontSize)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lFontSize);
}

void IDaSvrBalloon::SetFontCharSet(short sFontCharSet)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x60020010, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sFontCharSet);
}

void IDaSvrBalloon::GetFontCharSet(short* psFontCharSet)
{
	static BYTE parms[] =
		VTS_PI2;
	InvokeHelper(0x60020011, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 psFontCharSet);
}

void IDaSvrBalloon::SetStyle(long lStyle)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lStyle);
}

void IDaSvrBalloon::GetStyle(long* plStyle)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plStyle);
}

void IDaSvrBalloon::SetNumLines(long lLines)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lLines);
}

void IDaSvrBalloon::SetNumCharsPerLine(long lCharsPerLine)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lCharsPerLine);
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrCommandWindow properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrCommandWindow operations

void IDaSvrCommandWindow::SetVisible(long bVisible)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bVisible);
}

void IDaSvrCommandWindow::GetVisible(long* pbVisible)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pbVisible);
}

void IDaSvrCommandWindow::GetPosition(long* plLeft, long* plTop)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plLeft, plTop);
}

void IDaSvrCommandWindow::GetSize(long* plWidth, long* plHeight)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 plWidth, plHeight);
}


/////////////////////////////////////////////////////////////////////////////
// IDaAgentNotifySink15 properties

/////////////////////////////////////////////////////////////////////////////
// IDaAgentNotifySink15 operations

void IDaAgentNotifySink15::Command(long dwCommandID, LPUNKNOWN punkUserInput)
{
	static BYTE parms[] =
		VTS_I4 VTS_UNKNOWN;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCommandID, punkUserInput);
}

void IDaAgentNotifySink15::ActivateInputState(long dwCharID, long bActivated)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, bActivated);
}

void IDaAgentNotifySink15::VisibleState(long dwCharID, long bVisible, long dwCause)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, bVisible, dwCause);
}

void IDaAgentNotifySink15::Click(long dwCharID, short fwKeys, long x, long y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I2 VTS_I4 VTS_I4;
	InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, fwKeys, x, y);
}

void IDaAgentNotifySink15::DblClick(long dwCharID, short fwKeys, long x, long y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I2 VTS_I4 VTS_I4;
	InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, fwKeys, x, y);
}

void IDaAgentNotifySink15::DragStart(long dwCharID, short fwKeys, long x, long y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I2 VTS_I4 VTS_I4;
	InvokeHelper(0x60020007, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, fwKeys, x, y);
}

void IDaAgentNotifySink15::DragComplete(long dwCharID, short fwKeys, long x, long y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I2 VTS_I4 VTS_I4;
	InvokeHelper(0x60020008, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, fwKeys, x, y);
}

void IDaAgentNotifySink15::RequestStart(long dwRequestID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwRequestID);
}

void IDaAgentNotifySink15::RequestComplete(long dwRequestID, long hrStatus)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwRequestID, hrStatus);
}

void IDaAgentNotifySink15::BookMark(long dwBookMarkID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwBookMarkID);
}

void IDaAgentNotifySink15::Idle(long dwCharID, long bStart)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x6002000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, bStart);
}

void IDaAgentNotifySink15::Move(long dwCharID, long x, long y, long dwCause)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x6002000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, x, y, dwCause);
}

void IDaAgentNotifySink15::Size(long dwCharID, long lWidth, long lHeight)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x6002000e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, lWidth, lHeight);
}

void IDaAgentNotifySink15::BalloonVisibleState(long dwCharID, long bVisible)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x6002000f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, bVisible);
}


/////////////////////////////////////////////////////////////////////////////
// IDaNotifySink properties

/////////////////////////////////////////////////////////////////////////////
// IDaNotifySink operations

void IDaNotifySink::Command(long dwCommandID, LPUNKNOWN punkUserInput)
{
	static BYTE parms[] =
		VTS_I4 VTS_UNKNOWN;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCommandID, punkUserInput);
}

void IDaNotifySink::ActivateInputState(long dwCharID, long bActivated)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, bActivated);
}

void IDaNotifySink::VisibleState(long dwCharID, long bVisible, long dwCause)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, bVisible, dwCause);
}

void IDaNotifySink::Click(long dwCharID, short fwKeys, long x, long y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I2 VTS_I4 VTS_I4;
	InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, fwKeys, x, y);
}

void IDaNotifySink::DblClick(long dwCharID, short fwKeys, long x, long y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I2 VTS_I4 VTS_I4;
	InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, fwKeys, x, y);
}

void IDaNotifySink::DragStart(long dwCharID, short fwKeys, long x, long y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I2 VTS_I4 VTS_I4;
	InvokeHelper(0x60020007, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, fwKeys, x, y);
}

void IDaNotifySink::DragComplete(long dwCharID, short fwKeys, long x, long y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I2 VTS_I4 VTS_I4;
	InvokeHelper(0x60020008, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, fwKeys, x, y);
}

void IDaNotifySink::RequestStart(long dwRequestID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwRequestID);
}

void IDaNotifySink::RequestComplete(long dwRequestID, long hrStatus)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwRequestID, hrStatus);
}

void IDaNotifySink::BookMark(long dwBookMarkID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwBookMarkID);
}

void IDaNotifySink::Idle(long dwCharID, long bStart)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x6002000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, bStart);
}

void IDaNotifySink::Move(long dwCharID, long x, long y, long dwCause)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x6002000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, x, y, dwCause);
}

void IDaNotifySink::Size(long dwCharID, long lWidth, long lHeight)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x6002000e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, lWidth, lHeight);
}

void IDaNotifySink::BalloonVisibleState(long dwCharID, long bVisible)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x6002000f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, bVisible);
}

void IDaNotifySink::ListeningState(long dwCharID, long bListening, long dwCause)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, bListening, dwCause);
}

void IDaNotifySink::DefaultCharacterChange(LPCTSTR bszGUID)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bszGUID);
}

void IDaNotifySink::AgentPropertyChange()
{
	InvokeHelper(0x60030003, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void IDaNotifySink::ActiveClientChange(long dwCharID, long lStatus)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x60030004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 dwCharID, lStatus);
}
