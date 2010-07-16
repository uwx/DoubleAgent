// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "msagentctl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// IAgentCtlCharacters properties

/////////////////////////////////////////////////////////////////////////////
// IAgentCtlCharacters operations

LPDISPATCH IAgentCtlCharacters::GetItem(LPCTSTR CharacterID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		CharacterID);
	return result;
}

LPDISPATCH IAgentCtlCharacters::Character(LPCTSTR CharacterID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		CharacterID);
	return result;
}

void IAgentCtlCharacters::Unload(LPCTSTR CharacterID)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CharacterID);
}

LPDISPATCH IAgentCtlCharacters::Load(LPCTSTR CharacterID, const VARIANT& LoadKey)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		CharacterID, &LoadKey);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IAgentCtlCharacterEx properties

/////////////////////////////////////////////////////////////////////////////
// IAgentCtlCharacterEx operations

LPDISPATCH IAgentCtlCharacterEx::GetBalloon()
{
	LPDISPATCH result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IAgentCtlCharacterEx::GetCommands()
{
	LPDISPATCH result;
	InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString IAgentCtlCharacterEx::GetName()
{
	CString result;
	InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IAgentCtlCharacterEx::GetDescription()
{
	CString result;
	InvokeHelper(0x1c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL IAgentCtlCharacterEx::GetVisible()
{
	BOOL result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IAgentCtlCharacterEx::SetLeft(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IAgentCtlCharacterEx::GetLeft()
{
	short result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IAgentCtlCharacterEx::SetTop(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IAgentCtlCharacterEx::GetTop()
{
	short result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IAgentCtlCharacterEx::SetHeight(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IAgentCtlCharacterEx::GetHeight()
{
	short result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IAgentCtlCharacterEx::SetWidth(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IAgentCtlCharacterEx::GetWidth()
{
	short result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

long IAgentCtlCharacterEx::GetSpeed()
{
	long result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long IAgentCtlCharacterEx::GetPitch()
{
	long result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IAgentCtlCharacterEx::SetIdleOn(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x1d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IAgentCtlCharacterEx::GetIdleOn()
{
	BOOL result;
	InvokeHelper(0x1d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL IAgentCtlCharacterEx::Activate(const VARIANT& State)
{
	BOOL result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		&State);
	return result;
}

LPDISPATCH IAgentCtlCharacterEx::Play(LPCTSTR Animation)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Animation);
	return result;
}

LPDISPATCH IAgentCtlCharacterEx::Get(LPCTSTR Type, LPCTSTR Name, const VARIANT& Queue)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_VARIANT;
	InvokeHelper(0x1b, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Type, Name, &Queue);
	return result;
}

void IAgentCtlCharacterEx::Stop(const VARIANT& Request)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Request);
}

LPDISPATCH IAgentCtlCharacterEx::Wait(LPDISPATCH WaitForRequest)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x16, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		WaitForRequest);
	return result;
}

LPDISPATCH IAgentCtlCharacterEx::Interrupt(LPDISPATCH InterruptRequest)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x15, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		InterruptRequest);
	return result;
}

LPDISPATCH IAgentCtlCharacterEx::Speak(const VARIANT& Text, const VARIANT& Url)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Text, &Url);
	return result;
}

LPDISPATCH IAgentCtlCharacterEx::GestureAt(short x, short y)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		x, y);
	return result;
}

LPDISPATCH IAgentCtlCharacterEx::MoveTo(short x, short y, const VARIANT& Speed)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_VARIANT;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		x, y, &Speed);
	return result;
}

LPDISPATCH IAgentCtlCharacterEx::Hide(const VARIANT& Fast)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Fast);
	return result;
}

LPDISPATCH IAgentCtlCharacterEx::Show(const VARIANT& Fast)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x14, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Fast);
	return result;
}

void IAgentCtlCharacterEx::StopAll(const VARIANT& Types)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x1f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Types);
}

short IAgentCtlCharacterEx::GetMoveCause()
{
	short result;
	InvokeHelper(0x20, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short IAgentCtlCharacterEx::GetVisibilityCause()
{
	short result;
	InvokeHelper(0x21, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

BOOL IAgentCtlCharacterEx::GetHasOtherClients()
{
	BOOL result;
	InvokeHelper(0x22, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IAgentCtlCharacterEx::SetSoundEffectsOn(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x23, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IAgentCtlCharacterEx::GetSoundEffectsOn()
{
	BOOL result;
	InvokeHelper(0x23, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IAgentCtlCharacterEx::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x18, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void IAgentCtlCharacterEx::SetDescription(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IAgentCtlCharacterEx::GetExtraData()
{
	CString result;
	InvokeHelper(0x24, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL IAgentCtlCharacterEx::ShowPopupMenu(short x, short y)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x25, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		x, y);
	return result;
}

void IAgentCtlCharacterEx::SetAutoPopupMenu(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x26, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IAgentCtlCharacterEx::GetAutoPopupMenu()
{
	BOOL result;
	InvokeHelper(0x26, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IAgentCtlCharacterEx::SetHelpModeOn(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x27, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IAgentCtlCharacterEx::GetHelpModeOn()
{
	BOOL result;
	InvokeHelper(0x27, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IAgentCtlCharacterEx::SetHelpContextID(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x28, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IAgentCtlCharacterEx::GetHelpContextID()
{
	long result;
	InvokeHelper(0x28, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

short IAgentCtlCharacterEx::GetActive()
{
	short result;
	InvokeHelper(0x29, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

BOOL IAgentCtlCharacterEx::Listen(BOOL Listen)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x2a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Listen);
	return result;
}

void IAgentCtlCharacterEx::SetLanguageID(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IAgentCtlCharacterEx::GetLanguageID()
{
	long result;
	InvokeHelper(0x2b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString IAgentCtlCharacterEx::GetSRModeID()
{
	CString result;
	InvokeHelper(0x2e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IAgentCtlCharacterEx::SetSRModeID(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IAgentCtlCharacterEx::GetTTSModeID()
{
	CString result;
	InvokeHelper(0x2f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IAgentCtlCharacterEx::SetTTSModeID(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IAgentCtlCharacterEx::GetHelpFile()
{
	CString result;
	InvokeHelper(0x30, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IAgentCtlCharacterEx::SetHelpFile(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x30, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IAgentCtlCharacterEx::GetGuid()
{
	CString result;
	InvokeHelper(0x31, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

short IAgentCtlCharacterEx::GetOriginalHeight()
{
	short result;
	InvokeHelper(0x32, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short IAgentCtlCharacterEx::GetOriginalWidth()
{
	short result;
	InvokeHelper(0x33, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

LPDISPATCH IAgentCtlCharacterEx::Think(LPCTSTR Text)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x34, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Text);
	return result;
}

CString IAgentCtlCharacterEx::GetVersion()
{
	CString result;
	InvokeHelper(0x35, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH IAgentCtlCharacterEx::GetAnimationNames()
{
	LPDISPATCH result;
	InvokeHelper(0x36, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long IAgentCtlCharacterEx::GetSRStatus()
{
	long result;
	InvokeHelper(0x37, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IAgentCtlCharacter properties

/////////////////////////////////////////////////////////////////////////////
// IAgentCtlCharacter operations

LPDISPATCH IAgentCtlCharacter::GetBalloon()
{
	LPDISPATCH result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IAgentCtlCharacter::GetCommands()
{
	LPDISPATCH result;
	InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString IAgentCtlCharacter::GetName()
{
	CString result;
	InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IAgentCtlCharacter::GetDescription()
{
	CString result;
	InvokeHelper(0x1c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL IAgentCtlCharacter::GetVisible()
{
	BOOL result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IAgentCtlCharacter::SetLeft(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IAgentCtlCharacter::GetLeft()
{
	short result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IAgentCtlCharacter::SetTop(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IAgentCtlCharacter::GetTop()
{
	short result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IAgentCtlCharacter::SetHeight(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IAgentCtlCharacter::GetHeight()
{
	short result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IAgentCtlCharacter::SetWidth(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IAgentCtlCharacter::GetWidth()
{
	short result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

long IAgentCtlCharacter::GetSpeed()
{
	long result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long IAgentCtlCharacter::GetPitch()
{
	long result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IAgentCtlCharacter::SetIdleOn(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x1d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IAgentCtlCharacter::GetIdleOn()
{
	BOOL result;
	InvokeHelper(0x1d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL IAgentCtlCharacter::Activate(const VARIANT& State)
{
	BOOL result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		&State);
	return result;
}

LPDISPATCH IAgentCtlCharacter::Play(LPCTSTR Animation)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Animation);
	return result;
}

LPDISPATCH IAgentCtlCharacter::Get(LPCTSTR Type, LPCTSTR Name, const VARIANT& Queue)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_VARIANT;
	InvokeHelper(0x1b, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Type, Name, &Queue);
	return result;
}

void IAgentCtlCharacter::Stop(const VARIANT& Request)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Request);
}

LPDISPATCH IAgentCtlCharacter::Wait(LPDISPATCH WaitForRequest)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x16, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		WaitForRequest);
	return result;
}

LPDISPATCH IAgentCtlCharacter::Interrupt(LPDISPATCH InterruptRequest)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x15, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		InterruptRequest);
	return result;
}

LPDISPATCH IAgentCtlCharacter::Speak(const VARIANT& Text, const VARIANT& Url)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Text, &Url);
	return result;
}

LPDISPATCH IAgentCtlCharacter::GestureAt(short x, short y)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		x, y);
	return result;
}

LPDISPATCH IAgentCtlCharacter::MoveTo(short x, short y, const VARIANT& Speed)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_VARIANT;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		x, y, &Speed);
	return result;
}

LPDISPATCH IAgentCtlCharacter::Hide(const VARIANT& Fast)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Fast);
	return result;
}

LPDISPATCH IAgentCtlCharacter::Show(const VARIANT& Fast)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x14, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Fast);
	return result;
}

void IAgentCtlCharacter::StopAll(const VARIANT& Types)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x1f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Types);
}

short IAgentCtlCharacter::GetMoveCause()
{
	short result;
	InvokeHelper(0x20, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short IAgentCtlCharacter::GetVisibilityCause()
{
	short result;
	InvokeHelper(0x21, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

BOOL IAgentCtlCharacter::GetHasOtherClients()
{
	BOOL result;
	InvokeHelper(0x22, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IAgentCtlCharacter::SetSoundEffectsOn(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x23, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IAgentCtlCharacter::GetSoundEffectsOn()
{
	BOOL result;
	InvokeHelper(0x23, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IAgentCtlCharacter::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x18, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void IAgentCtlCharacter::SetDescription(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IAgentCtlCharacter::GetExtraData()
{
	CString result;
	InvokeHelper(0x24, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IAgentCtlBalloonEx properties

/////////////////////////////////////////////////////////////////////////////
// IAgentCtlBalloonEx operations

BOOL IAgentCtlBalloonEx::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long IAgentCtlBalloonEx::GetNumberOfLines()
{
	long result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long IAgentCtlBalloonEx::GetCharsPerLine()
{
	long result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString IAgentCtlBalloonEx::GetFontName()
{
	CString result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long IAgentCtlBalloonEx::GetFontSize()
{
	long result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL IAgentCtlBalloonEx::GetFontBold()
{
	BOOL result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL IAgentCtlBalloonEx::GetFontItalic()
{
	BOOL result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL IAgentCtlBalloonEx::GetFontStrikethru()
{
	BOOL result;
	InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL IAgentCtlBalloonEx::GetFontUnderline()
{
	BOOL result;
	InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long IAgentCtlBalloonEx::GetForeColor()
{
	long result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long IAgentCtlBalloonEx::GetBackColor()
{
	long result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long IAgentCtlBalloonEx::GetBorderColor()
{
	long result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IAgentCtlBalloonEx::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IAgentCtlBalloonEx::GetVisible()
{
	BOOL result;
	InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IAgentCtlBalloonEx::SetFontName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void IAgentCtlBalloonEx::SetFontSize(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void IAgentCtlBalloonEx::SetFontCharSet(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0xf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IAgentCtlBalloonEx::GetFontCharSet()
{
	short result;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IAgentCtlBalloonEx::SetStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x10, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IAgentCtlBalloonEx::GetStyle()
{
	long result;
	InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IAgentCtlBalloon properties

/////////////////////////////////////////////////////////////////////////////
// IAgentCtlBalloon operations

BOOL IAgentCtlBalloon::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long IAgentCtlBalloon::GetNumberOfLines()
{
	long result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long IAgentCtlBalloon::GetCharsPerLine()
{
	long result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString IAgentCtlBalloon::GetFontName()
{
	CString result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long IAgentCtlBalloon::GetFontSize()
{
	long result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL IAgentCtlBalloon::GetFontBold()
{
	BOOL result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL IAgentCtlBalloon::GetFontItalic()
{
	BOOL result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL IAgentCtlBalloon::GetFontStrikethru()
{
	BOOL result;
	InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL IAgentCtlBalloon::GetFontUnderline()
{
	BOOL result;
	InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long IAgentCtlBalloon::GetForeColor()
{
	long result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long IAgentCtlBalloon::GetBackColor()
{
	long result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long IAgentCtlBalloon::GetBorderColor()
{
	long result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IAgentCtlBalloon::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IAgentCtlBalloon::GetVisible()
{
	BOOL result;
	InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IAgentCtlBalloon::SetFontName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void IAgentCtlBalloon::SetFontSize(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void IAgentCtlBalloon::SetFontCharSet(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0xf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IAgentCtlBalloon::GetFontCharSet()
{
	short result;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IAgentCtlCommandsEx properties

/////////////////////////////////////////////////////////////////////////////
// IAgentCtlCommandsEx operations

LPDISPATCH IAgentCtlCommandsEx::GetItem(LPCTSTR Name)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		Name);
	return result;
}

LPDISPATCH IAgentCtlCommandsEx::Command(LPCTSTR Name)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name);
	return result;
}

long IAgentCtlCommandsEx::GetCount()
{
	long result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString IAgentCtlCommandsEx::GetCaption()
{
	CString result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IAgentCtlCommandsEx::SetCaption(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IAgentCtlCommandsEx::GetVoice()
{
	CString result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IAgentCtlCommandsEx::SetVoice(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL IAgentCtlCommandsEx::GetVisible()
{
	BOOL result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IAgentCtlCommandsEx::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH IAgentCtlCommandsEx::Add(LPCTSTR Name, const VARIANT& Caption, const VARIANT& Voice, const VARIANT& Enabled, const VARIANT& Visible)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name, &Caption, &Voice, &Enabled, &Visible);
	return result;
}

LPDISPATCH IAgentCtlCommandsEx::Insert(LPCTSTR Name, LPCTSTR RefName, const VARIANT& Before, const VARIANT& Caption, const VARIANT& Voice, const VARIANT& Enabled, const VARIANT& Visible)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name, RefName, &Before, &Caption, &Voice, &Enabled, &Visible);
	return result;
}

void IAgentCtlCommandsEx::Remove(LPCTSTR Name)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Name);
}

void IAgentCtlCommandsEx::RemoveAll()
{
	InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void IAgentCtlCommandsEx::SetDefaultCommand(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x10, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IAgentCtlCommandsEx::GetDefaultCommand()
{
	CString result;
	InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IAgentCtlCommandsEx::SetHelpContextID(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x11, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IAgentCtlCommandsEx::GetHelpContextID()
{
	long result;
	InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IAgentCtlCommandsEx::SetFontName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IAgentCtlCommandsEx::GetFontName()
{
	CString result;
	InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long IAgentCtlCommandsEx::GetFontSize()
{
	long result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IAgentCtlCommandsEx::SetFontSize(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void IAgentCtlCommandsEx::SetVoiceCaption(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x16, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IAgentCtlCommandsEx::GetVoiceCaption()
{
	CString result;
	InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IAgentCtlCommandsEx::SetGlobalVoiceCommandsEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x18, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IAgentCtlCommandsEx::GetGlobalVoiceCommandsEnabled()
{
	BOOL result;
	InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IAgentCtlCommands properties

/////////////////////////////////////////////////////////////////////////////
// IAgentCtlCommands operations

LPDISPATCH IAgentCtlCommands::GetItem(LPCTSTR Name)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		Name);
	return result;
}

LPDISPATCH IAgentCtlCommands::Command(LPCTSTR Name)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name);
	return result;
}

long IAgentCtlCommands::GetCount()
{
	long result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString IAgentCtlCommands::GetCaption()
{
	CString result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IAgentCtlCommands::SetCaption(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IAgentCtlCommands::GetVoice()
{
	CString result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IAgentCtlCommands::SetVoice(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL IAgentCtlCommands::GetVisible()
{
	BOOL result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IAgentCtlCommands::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH IAgentCtlCommands::Add(LPCTSTR Name, const VARIANT& Caption, const VARIANT& Voice, const VARIANT& Enabled, const VARIANT& Visible)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name, &Caption, &Voice, &Enabled, &Visible);
	return result;
}

LPDISPATCH IAgentCtlCommands::Insert(LPCTSTR Name, LPCTSTR RefName, const VARIANT& Before, const VARIANT& Caption, const VARIANT& Voice, const VARIANT& Enabled, const VARIANT& Visible)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name, RefName, &Before, &Caption, &Voice, &Enabled, &Visible);
	return result;
}

void IAgentCtlCommands::Remove(LPCTSTR Name)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Name);
}

void IAgentCtlCommands::RemoveAll()
{
	InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// IAgentCtlCommandEx properties

/////////////////////////////////////////////////////////////////////////////
// IAgentCtlCommandEx operations

CString IAgentCtlCommandEx::GetVoice()
{
	CString result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IAgentCtlCommandEx::SetVoice(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IAgentCtlCommandEx::GetCaption()
{
	CString result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IAgentCtlCommandEx::SetCaption(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL IAgentCtlCommandEx::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IAgentCtlCommandEx::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IAgentCtlCommandEx::GetVisible()
{
	BOOL result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IAgentCtlCommandEx::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long IAgentCtlCommandEx::GetConfidence()
{
	long result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IAgentCtlCommandEx::SetConfidence(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString IAgentCtlCommandEx::GetConfidenceText()
{
	CString result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IAgentCtlCommandEx::SetConfidenceText(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void IAgentCtlCommandEx::SetHelpContextID(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IAgentCtlCommandEx::GetHelpContextID()
{
	long result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IAgentCtlCommandEx::SetVoiceCaption(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IAgentCtlCommandEx::GetVoiceCaption()
{
	CString result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IAgentCtlCommand properties

/////////////////////////////////////////////////////////////////////////////
// IAgentCtlCommand operations

CString IAgentCtlCommand::GetVoice()
{
	CString result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IAgentCtlCommand::SetVoice(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IAgentCtlCommand::GetCaption()
{
	CString result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IAgentCtlCommand::SetCaption(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL IAgentCtlCommand::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IAgentCtlCommand::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IAgentCtlCommand::GetVisible()
{
	BOOL result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IAgentCtlCommand::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long IAgentCtlCommand::GetConfidence()
{
	long result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IAgentCtlCommand::SetConfidence(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString IAgentCtlCommand::GetConfidenceText()
{
	CString result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IAgentCtlCommand::SetConfidenceText(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// IAgentCtlRequest properties

/////////////////////////////////////////////////////////////////////////////
// IAgentCtlRequest operations

long IAgentCtlRequest::GetStatus()
{
	long result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString IAgentCtlRequest::GetDescription()
{
	CString result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long IAgentCtlRequest::GetNumber()
{
	long result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IAgentCtlAnimationNames properties

/////////////////////////////////////////////////////////////////////////////
// IAgentCtlAnimationNames operations


/////////////////////////////////////////////////////////////////////////////
// IAgentCtlAudioObject properties

/////////////////////////////////////////////////////////////////////////////
// IAgentCtlAudioObject operations

BOOL IAgentCtlAudioObject::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL IAgentCtlAudioObject::GetSoundEffects()
{
	BOOL result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IAgentCtlAudioObjectEx properties

/////////////////////////////////////////////////////////////////////////////
// IAgentCtlAudioObjectEx operations

BOOL IAgentCtlAudioObjectEx::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL IAgentCtlAudioObjectEx::GetSoundEffects()
{
	BOOL result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

short IAgentCtlAudioObjectEx::GetStatus()
{
	short result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IAgentCtlSpeechInput properties

/////////////////////////////////////////////////////////////////////////////
// IAgentCtlSpeechInput operations

BOOL IAgentCtlSpeechInput::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString IAgentCtlSpeechInput::GetLanguage()
{
	CString result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IAgentCtlSpeechInput::GetHotKey()
{
	CString result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL IAgentCtlSpeechInput::GetInstalled()
{
	BOOL result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString IAgentCtlSpeechInput::GetEngine()
{
	CString result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IAgentCtlSpeechInput::SetEngine(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL IAgentCtlSpeechInput::GetListeningTip()
{
	BOOL result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IAgentCtlPropertySheet properties

/////////////////////////////////////////////////////////////////////////////
// IAgentCtlPropertySheet operations

short IAgentCtlPropertySheet::GetLeft()
{
	short result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short IAgentCtlPropertySheet::GetTop()
{
	short result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short IAgentCtlPropertySheet::GetHeight()
{
	short result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short IAgentCtlPropertySheet::GetWidth()
{
	short result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IAgentCtlPropertySheet::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IAgentCtlPropertySheet::GetVisible()
{
	BOOL result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IAgentCtlPropertySheet::SetPage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IAgentCtlPropertySheet::GetPage()
{
	CString result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IAgentCtlUserInput properties

/////////////////////////////////////////////////////////////////////////////
// IAgentCtlUserInput operations

short IAgentCtlUserInput::GetCount()
{
	short result;
	InvokeHelper(0x60020000, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

CString IAgentCtlUserInput::GetName()
{
	CString result;
	InvokeHelper(0x60020001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IAgentCtlUserInput::GetCharacterID()
{
	CString result;
	InvokeHelper(0x60020002, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long IAgentCtlUserInput::GetConfidence()
{
	long result;
	InvokeHelper(0x60020003, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString IAgentCtlUserInput::GetVoice()
{
	CString result;
	InvokeHelper(0x60020004, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IAgentCtlUserInput::GetAlt1Name()
{
	CString result;
	InvokeHelper(0x60020005, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long IAgentCtlUserInput::GetAlt1Confidence()
{
	long result;
	InvokeHelper(0x60020006, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString IAgentCtlUserInput::GetAlt1Voice()
{
	CString result;
	InvokeHelper(0x60020007, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IAgentCtlUserInput::GetAlt2Name()
{
	CString result;
	InvokeHelper(0x60020008, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long IAgentCtlUserInput::GetAlt2Confidence()
{
	long result;
	InvokeHelper(0x60020009, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString IAgentCtlUserInput::GetAlt2Voice()
{
	CString result;
	InvokeHelper(0x6002000a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IAgentCtlCommandsWindow properties

/////////////////////////////////////////////////////////////////////////////
// IAgentCtlCommandsWindow operations

BOOL IAgentCtlCommandsWindow::GetVisible()
{
	BOOL result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IAgentCtlCommandsWindow::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

short IAgentCtlCommandsWindow::GetLeft()
{
	short result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short IAgentCtlCommandsWindow::GetTop()
{
	short result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short IAgentCtlCommandsWindow::GetHeight()
{
	short result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short IAgentCtlCommandsWindow::GetWidth()
{
	short result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IAgentCtlEx

IMPLEMENT_DYNCREATE(IAgentCtlEx, CWnd)

/////////////////////////////////////////////////////////////////////////////
// IAgentCtlEx properties

/////////////////////////////////////////////////////////////////////////////
// IAgentCtlEx operations

LPDISPATCH IAgentCtlEx::GetCharacters()
{
	LPDISPATCH result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IAgentCtlEx::GetAudioOutput()
{
	LPDISPATCH result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IAgentCtlEx::GetSpeechInput()
{
	LPDISPATCH result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IAgentCtlEx::GetPropertySheet()
{
	LPDISPATCH result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IAgentCtlEx::GetCommandsWindow()
{
	LPDISPATCH result;
	InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL IAgentCtlEx::GetConnected()
{
	BOOL result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IAgentCtlEx::SetConnected(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IAgentCtlEx::GetSuspended()
{
	BOOL result;
	InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IAgentCtlEx::ShowDefaultCharacterProperties(const VARIANT& x, const VARIANT& y)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &x, &y);
}

BOOL IAgentCtlEx::GetRaiseRequestErrors()
{
	BOOL result;
	InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IAgentCtlEx::SetRaiseRequestErrors(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// _AgentEvents properties

/////////////////////////////////////////////////////////////////////////////
// _AgentEvents operations

void _AgentEvents::ActivateInput(LPCTSTR CharacterID)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CharacterID);
}

void _AgentEvents::DeactivateInput(LPCTSTR CharacterID)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CharacterID);
}

void _AgentEvents::Click(LPCTSTR CharacterID, short Button, short Shift, short x, short y)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I2 VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CharacterID, Button, Shift, x, y);
}

void _AgentEvents::DblClick(LPCTSTR CharacterID, short Button, short Shift, short x, short y)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I2 VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CharacterID, Button, Shift, x, y);
}

void _AgentEvents::DragStart(LPCTSTR CharacterID, short Button, short Shift, short x, short y)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I2 VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CharacterID, Button, Shift, x, y);
}

void _AgentEvents::DragComplete(LPCTSTR CharacterID, short Button, short Shift, short x, short y)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I2 VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CharacterID, Button, Shift, x, y);
}

void _AgentEvents::Show(LPCTSTR CharacterID, short Cause)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I2;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CharacterID, Cause);
}

void _AgentEvents::Hide(LPCTSTR CharacterID, short Cause)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I2;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CharacterID, Cause);
}

void _AgentEvents::RequestStart(LPDISPATCH Request)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Request);
}

void _AgentEvents::RequestComplete(LPDISPATCH Request)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Request);
}

void _AgentEvents::Restart()
{
	InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _AgentEvents::Shutdown()
{
	InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _AgentEvents::Bookmark(long BookmarkID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 BookmarkID);
}

void _AgentEvents::Command(LPDISPATCH UserInput)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 UserInput);
}

void _AgentEvents::IdleStart(LPCTSTR CharacterID)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CharacterID);
}

void _AgentEvents::IdleComplete(LPCTSTR CharacterID)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CharacterID);
}

void _AgentEvents::Move(LPCTSTR CharacterID, short x, short y, short Cause)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(0x16, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CharacterID, x, y, Cause);
}

void _AgentEvents::Size(LPCTSTR CharacterID, short Width, short Height)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I2 VTS_I2;
	InvokeHelper(0x17, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CharacterID, Width, Height);
}

void _AgentEvents::BalloonShow(LPCTSTR CharacterID)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CharacterID);
}

void _AgentEvents::BalloonHide(LPCTSTR CharacterID)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CharacterID);
}

void _AgentEvents::HelpComplete(LPCTSTR CharacterID, LPCTSTR Name, short Cause)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CharacterID, Name, Cause);
}

void _AgentEvents::ListenStart(LPCTSTR CharacterID)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CharacterID);
}

void _AgentEvents::ListenComplete(LPCTSTR CharacterID, short Cause)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I2;
	InvokeHelper(0x1c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CharacterID, Cause);
}

void _AgentEvents::DefaultCharacterChange(LPCTSTR GUID)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 GUID);
}

void _AgentEvents::AgentPropertyChange()
{
	InvokeHelper(0x1f, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _AgentEvents::ActiveClientChange(LPCTSTR CharacterID, BOOL Active)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL;
	InvokeHelper(0x20, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CharacterID, Active);
}
