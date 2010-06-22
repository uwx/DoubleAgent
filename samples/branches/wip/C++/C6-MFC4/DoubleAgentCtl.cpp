// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "doubleagentctl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// IDaCtlCharacter properties

/////////////////////////////////////////////////////////////////////////////
// IDaCtlCharacter operations

LPDISPATCH IDaCtlCharacter::GetBalloon()
{
	LPDISPATCH result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IDaCtlCharacter::GetCommands()
{
	LPDISPATCH result;
	InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString IDaCtlCharacter::GetName()
{
	CString result;
	InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IDaCtlCharacter::GetDescription()
{
	CString result;
	InvokeHelper(0x1c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL IDaCtlCharacter::GetVisible()
{
	BOOL result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaCtlCharacter::SetLeft(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IDaCtlCharacter::GetLeft()
{
	short result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IDaCtlCharacter::SetTop(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IDaCtlCharacter::GetTop()
{
	short result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IDaCtlCharacter::SetHeight(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IDaCtlCharacter::GetHeight()
{
	short result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IDaCtlCharacter::SetWidth(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IDaCtlCharacter::GetWidth()
{
	short result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

BOOL IDaCtlCharacter::Activate(const VARIANT& State)
{
	BOOL result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		&State);
	return result;
}

LPDISPATCH IDaCtlCharacter::Play(LPCTSTR Animation)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Animation);
	return result;
}

LPDISPATCH IDaCtlCharacter::Get(LPCTSTR Type, LPCTSTR Name, const VARIANT& Queue)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_VARIANT;
	InvokeHelper(0x1b, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Type, Name, &Queue);
	return result;
}

void IDaCtlCharacter::Stop(const VARIANT& Request)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Request);
}

LPDISPATCH IDaCtlCharacter::Wait(LPDISPATCH WaitForRequest)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x16, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		WaitForRequest);
	return result;
}

LPDISPATCH IDaCtlCharacter::Interrupt(LPDISPATCH InterruptRequest)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x15, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		InterruptRequest);
	return result;
}

LPDISPATCH IDaCtlCharacter::Speak(const VARIANT& Text, const VARIANT& Url)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Text, &Url);
	return result;
}

LPDISPATCH IDaCtlCharacter::GestureAt(short x, short y)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		x, y);
	return result;
}

LPDISPATCH IDaCtlCharacter::MoveTo(short x, short y, const VARIANT& Speed)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_VARIANT;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		x, y, &Speed);
	return result;
}

LPDISPATCH IDaCtlCharacter::Hide(const VARIANT& Fast)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Fast);
	return result;
}

LPDISPATCH IDaCtlCharacter::Show(const VARIANT& Fast)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x14, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Fast);
	return result;
}

void IDaCtlCharacter::StopAll(const VARIANT& Types)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x1f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Types);
}

long IDaCtlCharacter::GetMoveCause()
{
	long result;
	InvokeHelper(0x20, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long IDaCtlCharacter::GetVisibilityCause()
{
	long result;
	InvokeHelper(0x21, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL IDaCtlCharacter::GetHasOtherClients()
{
	BOOL result;
	InvokeHelper(0x22, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaCtlCharacter::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x18, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void IDaCtlCharacter::SetDescription(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IDaCtlCharacter::GetExtraData()
{
	CString result;
	InvokeHelper(0x24, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL IDaCtlCharacter::ShowPopupMenu(short x, short y)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x25, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		x, y);
	return result;
}

void IDaCtlCharacter::SetAutoPopupMenu(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x26, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaCtlCharacter::GetAutoPopupMenu()
{
	BOOL result;
	InvokeHelper(0x26, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL IDaCtlCharacter::Listen(BOOL Listen)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x2a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Listen);
	return result;
}

void IDaCtlCharacter::SetLanguageID(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IDaCtlCharacter::GetLanguageID()
{
	long result;
	InvokeHelper(0x2b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString IDaCtlCharacter::GetSRModeID()
{
	CString result;
	InvokeHelper(0x2e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaCtlCharacter::SetSRModeID(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IDaCtlCharacter::GetTTSModeID()
{
	CString result;
	InvokeHelper(0x2f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaCtlCharacter::SetTTSModeID(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

short IDaCtlCharacter::GetOriginalHeight()
{
	short result;
	InvokeHelper(0x32, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short IDaCtlCharacter::GetOriginalWidth()
{
	short result;
	InvokeHelper(0x33, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

LPDISPATCH IDaCtlCharacter::Think(LPCTSTR Text)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x34, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Text);
	return result;
}

CString IDaCtlCharacter::GetVersion()
{
	CString result;
	InvokeHelper(0x35, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH IDaCtlCharacter::GetAnimationNames()
{
	LPDISPATCH result;
	InvokeHelper(0x36, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long IDaCtlCharacter::GetStyle()
{
	long result;
	InvokeHelper(0x3c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaCtlCharacter::SetStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL IDaCtlCharacter::GetHasIcon()
{
	BOOL result;
	InvokeHelper(0x3d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaCtlCharacter::GenerateIcon(long ClipLeft, long ClipTop, long ClipWidth, long ClipHeight)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x3e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ClipLeft, ClipTop, ClipWidth, ClipHeight);
}

BOOL IDaCtlCharacter::GetIconShown()
{
	BOOL result;
	InvokeHelper(0x3f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaCtlCharacter::SetIconShown(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x3f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaCtlCharacter::GetIconVisible()
{
	BOOL result;
	InvokeHelper(0x40, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString IDaCtlCharacter::GetIconIdentity()
{
	CString result;
	InvokeHelper(0x41, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaCtlCharacter::SetIconIdentity(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x41, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IDaCtlCharacter::GetIconTip()
{
	CString result;
	InvokeHelper(0x42, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaCtlCharacter::SetIconTip(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x42, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH IDaCtlCharacter::GetTTSEngine(const VARIANT& GetDefault)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x43, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		&GetDefault);
	return result;
}

LPDISPATCH IDaCtlCharacter::FindTTSEngines(const VARIANT& LanguageID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x44, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&LanguageID);
	return result;
}

LPDISPATCH IDaCtlCharacter::GetSREngine(const VARIANT& GetDefault)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x45, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		&GetDefault);
	return result;
}

LPDISPATCH IDaCtlCharacter::FindSREngines(const VARIANT& LanguageID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x46, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&LanguageID);
	return result;
}

CString IDaCtlCharacter::GetUniqueID()
{
	CString result;
	InvokeHelper(0x47, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IDaCtlCharacter::GetCharacterID()
{
	CString result;
	InvokeHelper(0x48, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IDaCtlCharacter::GetFileName()
{
	CString result;
	InvokeHelper(0x49, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IDaCtlCharacter::GetFilePath()
{
	CString result;
	InvokeHelper(0x4a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaCtlCharacter::SetActiveState(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x4b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IDaCtlCharacter::GetActiveState()
{
	long result;
	InvokeHelper(0x4b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL IDaCtlCharacter::GetIdleState()
{
	BOOL result;
	InvokeHelper(0x4c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaCtlCharacter::SetIdleEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x4d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaCtlCharacter::GetIdleEnabled()
{
	BOOL result;
	InvokeHelper(0x4d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaCtlCharacter::SetSoundEffectsEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x4e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaCtlCharacter::GetSoundEffectsEnabled()
{
	BOOL result;
	InvokeHelper(0x4e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH IDaCtlCharacter::Prepare(long ResourceType, LPCTSTR Name, const VARIANT& Queue)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_BSTR VTS_VARIANT;
	InvokeHelper(0x4f, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		ResourceType, Name, &Queue);
	return result;
}

long IDaCtlCharacter::GetListeningStatus()
{
	long result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL IDaCtlCharacter::GetSmoothed()
{
	BOOL result;
	InvokeHelper(0x51, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL IDaCtlCharacter::GetSmoothEdges()
{
	BOOL result;
	InvokeHelper(0x52, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IDaCtlBalloon properties

/////////////////////////////////////////////////////////////////////////////
// IDaCtlBalloon operations

BOOL IDaCtlBalloon::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString IDaCtlBalloon::GetFontName()
{
	CString result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long IDaCtlBalloon::GetFontSize()
{
	long result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaCtlBalloon::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaCtlBalloon::GetVisible()
{
	BOOL result;
	InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaCtlBalloon::SetFontName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void IDaCtlBalloon::SetFontSize(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void IDaCtlBalloon::SetFontCharSet(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0xf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IDaCtlBalloon::GetFontCharSet()
{
	short result;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IDaCtlBalloon::SetStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x10, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IDaCtlBalloon::GetStyle()
{
	long result;
	InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long IDaCtlBalloon::GetNumberOfLines()
{
	long result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaCtlBalloon::SetNumberOfLines(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IDaCtlBalloon::GetCharsPerLine()
{
	long result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaCtlBalloon::SetCharsPerLine(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IDaCtlBalloon::GetTextColor()
{
	long result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaCtlBalloon::SetTextColor(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IDaCtlBalloon::GetBackColor()
{
	long result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaCtlBalloon::SetBackColor(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IDaCtlBalloon::GetBorderColor()
{
	long result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaCtlBalloon::SetBorderColor(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL IDaCtlBalloon::GetFontBold()
{
	BOOL result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaCtlBalloon::SetFontBold(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaCtlBalloon::GetFontItalic()
{
	BOOL result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaCtlBalloon::SetFontItalic(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaCtlBalloon::GetFontStrikethru()
{
	BOOL result;
	InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaCtlBalloon::SetFontStrikethru(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaCtlBalloon::GetFontUnderline()
{
	BOOL result;
	InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaCtlBalloon::SetFontUnderline(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaCtlBalloon::GetSizeToText()
{
	BOOL result;
	InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaCtlBalloon::SetSizeToText(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x11, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaCtlBalloon::GetAutoHide()
{
	BOOL result;
	InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaCtlBalloon::SetAutoHide(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x12, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaCtlBalloon::GetAutoPace()
{
	BOOL result;
	InvokeHelper(0x13, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaCtlBalloon::SetAutoPace(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x13, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaCtlBalloon::GetShowPartialLines()
{
	BOOL result;
	InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaCtlBalloon::SetShowPartialLines(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x14, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// IDaCtlCommands properties

/////////////////////////////////////////////////////////////////////////////
// IDaCtlCommands operations

LPDISPATCH IDaCtlCommands::GetItem(LPCTSTR Name)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		Name);
	return result;
}

long IDaCtlCommands::GetCount()
{
	long result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString IDaCtlCommands::GetCaption()
{
	CString result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaCtlCommands::SetCaption(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL IDaCtlCommands::GetVisible()
{
	BOOL result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaCtlCommands::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH IDaCtlCommands::Add(LPCTSTR Name, const VARIANT& Caption, const VARIANT& VoiceGrammar, const VARIANT& Enabled, const VARIANT& Visible)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name, &Caption, &VoiceGrammar, &Enabled, &Visible);
	return result;
}

LPDISPATCH IDaCtlCommands::Insert(LPCTSTR Name, LPCTSTR RefName, const VARIANT& Before, const VARIANT& Caption, const VARIANT& VoiceGrammar, const VARIANT& Enabled, const VARIANT& Visible)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Name, RefName, &Before, &Caption, &VoiceGrammar, &Enabled, &Visible);
	return result;
}

void IDaCtlCommands::Remove(LPCTSTR Name)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Name);
}

void IDaCtlCommands::RemoveAll()
{
	InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString IDaCtlCommands::GetDefaultCommand()
{
	CString result;
	InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaCtlCommands::SetDefaultCommand(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x10, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IDaCtlCommands::GetFontName()
{
	CString result;
	InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaCtlCommands::SetFontName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long IDaCtlCommands::GetFontSize()
{
	long result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaCtlCommands::SetFontSize(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString IDaCtlCommands::GetVoiceCaption()
{
	CString result;
	InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaCtlCommands::SetVoiceCaption(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x16, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL IDaCtlCommands::GetGlobalVoiceCommandsEnabled()
{
	BOOL result;
	InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaCtlCommands::SetGlobalVoiceCommandsEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x18, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString IDaCtlCommands::GetVoiceGrammar()
{
	CString result;
	InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaCtlCommands::SetVoiceGrammar(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x19, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH IDaCtlCommands::GetIndex(long Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IDaCtlCommand properties

/////////////////////////////////////////////////////////////////////////////
// IDaCtlCommand operations

CString IDaCtlCommand::GetCaption()
{
	CString result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaCtlCommand::SetCaption(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL IDaCtlCommand::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaCtlCommand::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaCtlCommand::GetVisible()
{
	BOOL result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaCtlCommand::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString IDaCtlCommand::GetConfidenceText()
{
	CString result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaCtlCommand::SetConfidenceText(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IDaCtlCommand::GetVoiceCaption()
{
	CString result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaCtlCommand::SetVoiceCaption(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IDaCtlCommand::GetVoiceGrammar()
{
	CString result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaCtlCommand::SetVoiceGrammar(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long IDaCtlCommand::GetConfidenceThreshold()
{
	long result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaCtlCommand::SetConfidenceThreshold(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString IDaCtlCommand::GetName()
{
	CString result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IDaCtlRequest properties

/////////////////////////////////////////////////////////////////////////////
// IDaCtlRequest operations

long IDaCtlRequest::GetStatus()
{
	long result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString IDaCtlRequest::GetDescription()
{
	CString result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long IDaCtlRequest::GetNumber()
{
	long result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IDaCtlAnimationNames properties

/////////////////////////////////////////////////////////////////////////////
// IDaCtlAnimationNames operations

CString IDaCtlAnimationNames::GetItem(const VARIANT& Index)
{
	CString result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms,
		&Index);
	return result;
}

long IDaCtlAnimationNames::GetCount()
{
	long result;
	InvokeHelper(0xfffffff8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IDaCtlTTSEngine properties

/////////////////////////////////////////////////////////////////////////////
// IDaCtlTTSEngine operations

CString IDaCtlTTSEngine::GetTTSModeID()
{
	CString result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IDaCtlTTSEngine::GetDisplayName()
{
	CString result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IDaCtlTTSEngine::GetManufacturer()
{
	CString result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaCtlTTSEngine::GetVersion(short* MajorVersion, short* MinorVersion)
{
	static BYTE parms[] =
		VTS_PI2 VTS_PI2;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 MajorVersion, MinorVersion);
}

long IDaCtlTTSEngine::GetGender()
{
	long result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long IDaCtlTTSEngine::GetLanguageID()
{
	long result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString IDaCtlTTSEngine::GetLanguageName(BOOL EnglishName)
{
	CString result;
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms,
		EnglishName);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IDaCtlTTSEngines properties

/////////////////////////////////////////////////////////////////////////////
// IDaCtlTTSEngines operations

LPDISPATCH IDaCtlTTSEngines::GetItem(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

long IDaCtlTTSEngines::GetCount()
{
	long result;
	InvokeHelper(0xfffffff8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IDaCtlSREngine properties

/////////////////////////////////////////////////////////////////////////////
// IDaCtlSREngine operations

CString IDaCtlSREngine::GetSRModeID()
{
	CString result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IDaCtlSREngine::GetDisplayName()
{
	CString result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IDaCtlSREngine::GetManufacturer()
{
	CString result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaCtlSREngine::GetVersion(short* MajorVersion, short* MinorVersion)
{
	static BYTE parms[] =
		VTS_PI2 VTS_PI2;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 MajorVersion, MinorVersion);
}

long IDaCtlSREngine::GetLanguageID()
{
	long result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString IDaCtlSREngine::GetLanguageName(BOOL EnglishName)
{
	CString result;
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms,
		EnglishName);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IDaCtlSREngines properties

/////////////////////////////////////////////////////////////////////////////
// IDaCtlSREngines operations

LPDISPATCH IDaCtlSREngines::GetItem(const VARIANT& Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		&Index);
	return result;
}

long IDaCtlSREngines::GetCount()
{
	long result;
	InvokeHelper(0xfffffff8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IDaCtlUserInput properties

/////////////////////////////////////////////////////////////////////////////
// IDaCtlUserInput operations

short IDaCtlUserInput::GetCount()
{
	short result;
	InvokeHelper(0x60020000, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

CString IDaCtlUserInput::GetName()
{
	CString result;
	InvokeHelper(0x60020001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IDaCtlUserInput::GetCharacterID()
{
	CString result;
	InvokeHelper(0x60020002, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long IDaCtlUserInput::GetConfidence()
{
	long result;
	InvokeHelper(0x60020003, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString IDaCtlUserInput::GetVoice()
{
	CString result;
	InvokeHelper(0x60020004, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IDaCtlUserInput::GetAlt1Name()
{
	CString result;
	InvokeHelper(0x60020005, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long IDaCtlUserInput::GetAlt1Confidence()
{
	long result;
	InvokeHelper(0x60020006, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString IDaCtlUserInput::GetAlt1Voice()
{
	CString result;
	InvokeHelper(0x60020007, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IDaCtlUserInput::GetAlt2Name()
{
	CString result;
	InvokeHelper(0x60020008, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long IDaCtlUserInput::GetAlt2Confidence()
{
	long result;
	InvokeHelper(0x60020009, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString IDaCtlUserInput::GetAlt2Voice()
{
	CString result;
	InvokeHelper(0x6002000a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IDaCtlCommandsWindow properties

/////////////////////////////////////////////////////////////////////////////
// IDaCtlCommandsWindow operations

BOOL IDaCtlCommandsWindow::GetVisible()
{
	BOOL result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaCtlCommandsWindow::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

short IDaCtlCommandsWindow::GetLeft()
{
	short result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short IDaCtlCommandsWindow::GetTop()
{
	short result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short IDaCtlCommandsWindow::GetHeight()
{
	short result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short IDaCtlCommandsWindow::GetWidth()
{
	short result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IDaCtlCharacters properties

/////////////////////////////////////////////////////////////////////////////
// IDaCtlCharacters operations

LPDISPATCH IDaCtlCharacters::GetItem(LPCTSTR CharacterID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		CharacterID);
	return result;
}

void IDaCtlCharacters::Unload(LPCTSTR CharacterID)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CharacterID);
}

LPDISPATCH IDaCtlCharacters::Load(LPCTSTR CharacterID, const VARIANT& LoadKey)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		CharacterID, &LoadKey);
	return result;
}

long IDaCtlCharacters::GetCount()
{
	long result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH IDaCtlCharacters::GetIndex(long Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IDaCtlPropertySheet properties

/////////////////////////////////////////////////////////////////////////////
// IDaCtlPropertySheet operations

short IDaCtlPropertySheet::GetHeight()
{
	short result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short IDaCtlPropertySheet::GetWidth()
{
	short result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IDaCtlPropertySheet::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaCtlPropertySheet::GetVisible()
{
	BOOL result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaCtlPropertySheet::SetPage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IDaCtlPropertySheet::GetPage()
{
	CString result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaCtlPropertySheet::SetLeft(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IDaCtlPropertySheet::GetLeft()
{
	short result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IDaCtlPropertySheet::SetTop(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IDaCtlPropertySheet::GetTop()
{
	short result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IDaCtlCharacterFiles properties

/////////////////////////////////////////////////////////////////////////////
// IDaCtlCharacterFiles operations

CString IDaCtlCharacterFiles::GetSearchPath()
{
	CString result;
	InvokeHelper(0x60020001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaCtlCharacterFiles::SetSearchPath(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IDaCtlCharacterFiles::GetDefaultSearchPath()
{
	CString result;
	InvokeHelper(0x60020002, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL IDaCtlCharacterFiles::GetDoubleAgentFiles()
{
	BOOL result;
	InvokeHelper(0x60020003, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaCtlCharacterFiles::SetDoubleAgentFiles(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60020003, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaCtlCharacterFiles::GetMsAgentFiles()
{
	BOOL result;
	InvokeHelper(0x60020004, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaCtlCharacterFiles::SetMsAgentFiles(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60020004, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaCtlCharacterFiles::GetMsOfficeFiles()
{
	BOOL result;
	InvokeHelper(0x60020005, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaCtlCharacterFiles::SetMsOfficeFiles(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60020005, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaCtlCharacterFiles::GetSpeakingCharacters()
{
	BOOL result;
	InvokeHelper(0x60020006, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaCtlCharacterFiles::SetSpeakingCharacters(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60020006, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaCtlCharacterFiles::GetNonSpeakingCharacters()
{
	BOOL result;
	InvokeHelper(0x60020007, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaCtlCharacterFiles::SetNonSpeakingCharacters(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60020007, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaCtlCharacterFiles::GetVerifyVersion()
{
	BOOL result;
	InvokeHelper(0x60020008, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaCtlCharacterFiles::SetVerifyVersion(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60020008, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString IDaCtlCharacterFiles::GetDefaultFilePath()
{
	CString result;
	InvokeHelper(0x60020009, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IDaCtlCharacterFiles::GetDefaultFileName()
{
	CString result;
	InvokeHelper(0x6002000a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IDaControl

IMPLEMENT_DYNCREATE(IDaControl, CWnd)

/////////////////////////////////////////////////////////////////////////////
// IDaControl properties

/////////////////////////////////////////////////////////////////////////////
// IDaControl operations

LPDISPATCH IDaControl::GetCharacters()
{
	LPDISPATCH result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IDaControl::GetPropertySheet()
{
	LPDISPATCH result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IDaControl::GetCommandsWindow()
{
	LPDISPATCH result;
	InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL IDaControl::GetConnected()
{
	BOOL result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaControl::SetConnected(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaControl::GetRaiseRequestErrors()
{
	BOOL result;
	InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaControl::SetRaiseRequestErrors(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

void IDaControl::SetAutoSize(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfffffe0c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaControl::GetAutoSize()
{
	BOOL result;
	InvokeHelper(0xfffffe0c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaControl::SetBackColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

unsigned long IDaControl::GetBackColor()
{
	unsigned long result;
	InvokeHelper(DISPID_BACKCOLOR, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaControl::SetBorderColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffe09, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

unsigned long IDaControl::GetBorderColor()
{
	unsigned long result;
	InvokeHelper(0xfffffe09, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaControl::SetBorderStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(DISPID_BORDERSTYLE, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IDaControl::GetBorderStyle()
{
	long result;
	InvokeHelper(DISPID_BORDERSTYLE, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaControl::SetBorderWidth(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffe07, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IDaControl::GetBorderWidth()
{
	long result;
	InvokeHelper(0xfffffe07, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaControl::SetBorderVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xfffffdf9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaControl::GetBorderVisible()
{
	BOOL result;
	InvokeHelper(0xfffffdf9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaControl::SetMousePointer(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IDaControl::GetMousePointer()
{
	long result;
	InvokeHelper(0xfffffdf7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaControl::SetMouseIcon(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH IDaControl::GetMouseIcon()
{
	LPDISPATCH result;
	InvokeHelper(0xfffffdf6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long IDaControl::GetCharacterStyle()
{
	long result;
	InvokeHelper(0x1e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaControl::SetCharacterStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH IDaControl::GetCharacterFiles()
{
	LPDISPATCH result;
	InvokeHelper(0x1f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IDaControl::GetTTSEngines()
{
	LPDISPATCH result;
	InvokeHelper(0x20, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IDaControl::FindTTSEngines(const VARIANT& LanguageID, const VARIANT& Gender)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x21, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&LanguageID, &Gender);
	return result;
}

LPDISPATCH IDaControl::GetCharacterTTSEngine(const VARIANT& LoadKey)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x22, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&LoadKey);
	return result;
}

LPDISPATCH IDaControl::FindCharacterTTSEngines(const VARIANT& LoadKey, const VARIANT& LanguageID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x23, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&LoadKey, &LanguageID);
	return result;
}

LPDISPATCH IDaControl::GetSREngines()
{
	LPDISPATCH result;
	InvokeHelper(0x24, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IDaControl::FindSREngines(const VARIANT& LanguageID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x25, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&LanguageID);
	return result;
}

LPDISPATCH IDaControl::GetCharacterSREngine(const VARIANT& LoadKey)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x26, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&LoadKey);
	return result;
}

LPDISPATCH IDaControl::FindCharacterSREngines(const VARIANT& LoadKey, const VARIANT& LanguageID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x27, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&LoadKey, &LanguageID);
	return result;
}

LPDISPATCH IDaControl::GetSettings()
{
	LPDISPATCH result;
	InvokeHelper(0x28, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL IDaControl::GetAutoConnect()
{
	BOOL result;
	InvokeHelper(0x29, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaControl::SetAutoConnect(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x29, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH IDaControl::GetControlCharacter()
{
	LPDISPATCH result;
	InvokeHelper(0x2a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void IDaControl::SetControlCharacter(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x2a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}


/////////////////////////////////////////////////////////////////////////////
// IDaCtlSettings properties

/////////////////////////////////////////////////////////////////////////////
// IDaCtlSettings operations

BOOL IDaCtlSettings::GetSoundEffectsEnabled()
{
	BOOL result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL IDaCtlSettings::GetBalloonEnabled()
{
	BOOL result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH IDaCtlSettings::GetBalloonFont()
{
	LPDISPATCH result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL IDaCtlSettings::GetTTSEnabled()
{
	BOOL result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

short IDaCtlSettings::GetTTSSpeed()
{
	short result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

BOOL IDaCtlSettings::GetSREnabled()
{
	BOOL result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString IDaCtlSettings::GetSRHotKey()
{
	CString result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

short IDaCtlSettings::GetSRHotKeyTime()
{
	short result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

BOOL IDaCtlSettings::GetSRListeningTip()
{
	BOOL result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL IDaCtlSettings::GetSRListeningPrompt()
{
	BOOL result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long IDaCtlSettings::GetAudioStatus()
{
	long result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}
