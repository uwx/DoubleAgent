// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "doubleagentsvr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// IDaSvrUserInput properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrUserInput operations

void IDaSvrUserInput::GetAllItemData(VARIANT* ItemIndices, VARIANT* ItemConfidences, VARIANT* ItemText)
{
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ItemIndices, ItemConfidences, ItemText);
}

long IDaSvrUserInput::GetCount()
{
	long result;
	InvokeHelper(0x60030000, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long IDaSvrUserInput::GetItemCommandID(long ItemIndex)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030001, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms,
		ItemIndex);
	return result;
}

long IDaSvrUserInput::GetItemConfidence(long ItemIndex)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030002, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms,
		ItemIndex);
	return result;
}

CString IDaSvrUserInput::GetItemText(long ItemIndex)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030003, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms,
		ItemIndex);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrCharacterFiles properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrCharacterFiles operations

CString IDaSvrCharacterFiles::GetSearchPath()
{
	CString result;
	InvokeHelper(0x60020001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacterFiles::SetSearchPath(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IDaSvrCharacterFiles::GetDefaultSearchPath()
{
	CString result;
	InvokeHelper(0x60020002, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long IDaSvrCharacterFiles::GetFilter()
{
	long result;
	InvokeHelper(0x60020003, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacterFiles::SetFilter(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020003, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString IDaSvrCharacterFiles::GetDefaultFilePath()
{
	CString result;
	InvokeHelper(0x60020004, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IDaSvrCharacterFiles::GetDefaultFileName()
{
	CString result;
	InvokeHelper(0x60020005, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrTTSEngine properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrTTSEngine operations

CString IDaSvrTTSEngine::GetTTSModeID()
{
	CString result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IDaSvrTTSEngine::GetDisplayName()
{
	CString result;
	InvokeHelper(0x60020001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IDaSvrTTSEngine::GetManufacturer()
{
	CString result;
	InvokeHelper(0x60020002, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrTTSEngine::GetVersion(short* MajorVersion, short* MinorVersion)
{
	static BYTE parms[] =
		VTS_PI2 VTS_PI2;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 MajorVersion, MinorVersion);
}

long IDaSvrTTSEngine::GetGender()
{
	long result;
	InvokeHelper(0x60020004, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long IDaSvrTTSEngine::GetLanguageID()
{
	long result;
	InvokeHelper(0x60020005, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString IDaSvrTTSEngine::GetLanguageName(BOOL EnglishName)
{
	CString result;
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60020006, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms,
		EnglishName);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrTTSEngines properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrTTSEngines operations

LPDISPATCH IDaSvrTTSEngines::GetItem(long Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}

long IDaSvrTTSEngines::GetCount()
{
	long result;
	InvokeHelper(0xfffffff8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrSREngine properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrSREngine operations

CString IDaSvrSREngine::GetSRModeID()
{
	CString result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IDaSvrSREngine::GetDisplayName()
{
	CString result;
	InvokeHelper(0x60020001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IDaSvrSREngine::GetManufacturer()
{
	CString result;
	InvokeHelper(0x60020002, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrSREngine::GetVersion(short* MajorVersion, short* MinorVersion)
{
	static BYTE parms[] =
		VTS_PI2 VTS_PI2;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 MajorVersion, MinorVersion);
}

long IDaSvrSREngine::GetLanguageID()
{
	long result;
	InvokeHelper(0x60020004, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString IDaSvrSREngine::GetLanguageName(BOOL EnglishName)
{
	CString result;
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60020005, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms,
		EnglishName);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrSREngines properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrSREngines operations

LPDISPATCH IDaSvrSREngines::GetItem(long Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}

long IDaSvrSREngines::GetCount()
{
	long result;
	InvokeHelper(0xfffffff8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IDaServer properties

/////////////////////////////////////////////////////////////////////////////
// IDaServer operations

void IDaServer::Load(const VARIANT& LoadKey, long* CharacterID, long* RequestID)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &LoadKey, CharacterID, RequestID);
}

void IDaServer::Unload(long CharacterID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CharacterID);
}

void IDaServer::GetVersion(short* MajorVersion, short* MinorVersion)
{
	static BYTE parms[] =
		VTS_PI2 VTS_PI2;
	InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 MajorVersion, MinorVersion);
}

LPDISPATCH IDaServer::GetCharacter(long CharacterID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60040000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		CharacterID);
	return result;
}

LPDISPATCH IDaServer::GetCharacterFiles()
{
	LPDISPATCH result;
	InvokeHelper(0x60040001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long IDaServer::GetCharacterStyle()
{
	long result;
	InvokeHelper(0x60040002, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaServer::SetCharacterStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60040002, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH IDaServer::GetTTSEngines()
{
	LPDISPATCH result;
	InvokeHelper(0x60040003, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IDaServer::FindTTSEngines(long LanguageID, short Gender)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_I2;
	InvokeHelper(0x60040004, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		LanguageID, Gender);
	return result;
}

LPDISPATCH IDaServer::GetCharacterTTSEngine(const VARIANT& LoadKey)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x60040005, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&LoadKey);
	return result;
}

LPDISPATCH IDaServer::FindCharacterTTSEngines(const VARIANT& LoadKey, long LanguageID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_I4;
	InvokeHelper(0x60040006, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&LoadKey, LanguageID);
	return result;
}

LPDISPATCH IDaServer::GetSREngines()
{
	LPDISPATCH result;
	InvokeHelper(0x60040007, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IDaServer::FindSREngines(long LanguageID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60040008, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		LanguageID);
	return result;
}

LPDISPATCH IDaServer::GetCharacterSREngine(const VARIANT& LoadKey)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x60040009, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&LoadKey);
	return result;
}

LPDISPATCH IDaServer::FindCharacterSREngines(const VARIANT& LoadKey, long LanguageID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_I4;
	InvokeHelper(0x6004000a, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&LoadKey, LanguageID);
	return result;
}

LPDISPATCH IDaServer::GetPropertySheet()
{
	LPDISPATCH result;
	InvokeHelper(0x6004000b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IDaServer::GetCommandsWindow()
{
	LPDISPATCH result;
	InvokeHelper(0x6004000c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IDaServer::GetSettings()
{
	LPDISPATCH result;
	InvokeHelper(0x6004000d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrCharacter properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrCharacter operations

void IDaSvrCharacter::SetPosition(long Left, long Top)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Left, Top);
}

void IDaSvrCharacter::GetPosition(long* Left, long* Top)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Left, Top);
}

void IDaSvrCharacter::SetSize(long Width, long Height)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Width, Height);
}

void IDaSvrCharacter::GetSize(long* Width, long* Height)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Width, Height);
}

void IDaSvrCharacter::Activate(short State)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 State);
}

void IDaSvrCharacter::Prepare(long Type, LPCTSTR Name, long Queue, long* RequestID)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR VTS_I4 VTS_PI4;
	InvokeHelper(0x6002000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Type, Name, Queue, RequestID);
}

void IDaSvrCharacter::Play(LPCTSTR Animation, long* RequestID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_PI4;
	InvokeHelper(0x6002000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Animation, RequestID);
}

void IDaSvrCharacter::Stop(long RequestID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 RequestID);
}

void IDaSvrCharacter::StopAll(long Types)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Types);
}

void IDaSvrCharacter::Wait(long WaitForRequestID, long* RequestID)
{
	static BYTE parms[] =
		VTS_I4 VTS_PI4;
	InvokeHelper(0x60020010, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 WaitForRequestID, RequestID);
}

void IDaSvrCharacter::Interrupt(long InterruptRequestID, long* RequestID)
{
	static BYTE parms[] =
		VTS_I4 VTS_PI4;
	InvokeHelper(0x60020011, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 InterruptRequestID, RequestID);
}

void IDaSvrCharacter::Show(long Fast, long* RequestID)
{
	static BYTE parms[] =
		VTS_I4 VTS_PI4;
	InvokeHelper(0x60020012, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Fast, RequestID);
}

void IDaSvrCharacter::Hide(long Fast, long* RequestID)
{
	static BYTE parms[] =
		VTS_I4 VTS_PI4;
	InvokeHelper(0x60020013, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Fast, RequestID);
}

void IDaSvrCharacter::Speak(LPCTSTR Text, LPCTSTR Url, long* RequestID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_PI4;
	InvokeHelper(0x60020014, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Text, Url, RequestID);
}

void IDaSvrCharacter::MoveTo(short x, short y, long Speed, long* RequestID)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I4 VTS_PI4;
	InvokeHelper(0x60020015, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 x, y, Speed, RequestID);
}

void IDaSvrCharacter::GestureAt(short x, short y, long* RequestID)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_PI4;
	InvokeHelper(0x60020016, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 x, y, RequestID);
}

void IDaSvrCharacter::ShowPopupMenu(short x, short y)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 x, y);
}

void IDaSvrCharacter::Listen(long Listen)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6003000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Listen);
}

void IDaSvrCharacter::GetOriginalSize(long* Width, long* Height)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60030012, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Width, Height);
}

void IDaSvrCharacter::Think(LPCTSTR Text, long* RequestID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_PI4;
	InvokeHelper(0x60030013, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Text, RequestID);
}

void IDaSvrCharacter::GetVersion(short* MajorVersion, short* MinorVersion)
{
	static BYTE parms[] =
		VTS_PI2 VTS_PI2;
	InvokeHelper(0x60030014, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 MajorVersion, MinorVersion);
}

void IDaSvrCharacter::GetAnimationNames(LPUNKNOWN* punkEnum)
{
	static BYTE parms[] =
		VTS_PUNKNOWN;
	InvokeHelper(0x60030015, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 punkEnum);
}

LPDISPATCH IDaSvrCharacter::GetBalloon()
{
	LPDISPATCH result;
	InvokeHelper(0x60040000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IDaSvrCharacter::GetCommands()
{
	LPDISPATCH result;
	InvokeHelper(0x60040001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long IDaSvrCharacter::GetStyle()
{
	long result;
	InvokeHelper(0x60040002, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter::SetStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60040002, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL IDaSvrCharacter::GetHasIcon()
{
	BOOL result;
	InvokeHelper(0x60040003, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter::GenerateIcon(long ClipLeft, long ClipTop, long ClipWidth, long ClipHeight)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x60040004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ClipLeft, ClipTop, ClipWidth, ClipHeight);
}

BOOL IDaSvrCharacter::GetIconVisible()
{
	BOOL result;
	InvokeHelper(0x60040005, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString IDaSvrCharacter::GetIconIdentity()
{
	CString result;
	InvokeHelper(0x60040008, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter::SetIconIdentity(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60040008, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IDaSvrCharacter::GetIconTip()
{
	CString result;
	InvokeHelper(0x60040009, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter::SetIconTip(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60040009, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH IDaSvrCharacter::GetTTSEngine(BOOL GetDefault)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6004000a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		GetDefault);
	return result;
}

LPDISPATCH IDaSvrCharacter::FindTTSEngines(long LanguageID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6004000b, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		LanguageID);
	return result;
}

LPDISPATCH IDaSvrCharacter::GetSREngine(BOOL GetDefault)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6004000c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		GetDefault);
	return result;
}

LPDISPATCH IDaSvrCharacter::FindSREngines(long LanguageID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6004000d, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		LanguageID);
	return result;
}

long IDaSvrCharacter::GetCharacterID()
{
	long result;
	InvokeHelper(0x60040010, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString IDaSvrCharacter::GetUniqueID()
{
	CString result;
	InvokeHelper(0x60040011, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IDaSvrCharacter::GetName()
{
	CString result;
	InvokeHelper(0x60040012, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60040012, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IDaSvrCharacter::GetDescription()
{
	CString result;
	InvokeHelper(0x60040013, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter::SetDescription(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60040013, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IDaSvrCharacter::GetExtraData()
{
	CString result;
	InvokeHelper(0x60040014, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IDaSvrCharacter::GetFileName()
{
	CString result;
	InvokeHelper(0x60040015, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IDaSvrCharacter::GetFilePath()
{
	CString result;
	InvokeHelper(0x60040016, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long IDaSvrCharacter::GetLanguageID()
{
	long result;
	InvokeHelper(0x60040017, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter::SetLanguageID(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60040017, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString IDaSvrCharacter::GetTTSModeID()
{
	CString result;
	InvokeHelper(0x60040018, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter::SetTTSModeID(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60040018, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IDaSvrCharacter::GetSRModeID()
{
	CString result;
	InvokeHelper(0x60040019, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter::SetSRModeID(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60040019, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

short IDaSvrCharacter::GetLeft()
{
	short result;
	InvokeHelper(0x6004001a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter::SetLeft(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x6004001a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IDaSvrCharacter::GetTop()
{
	short result;
	InvokeHelper(0x6004001b, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter::SetTop(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x6004001b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IDaSvrCharacter::GetWidth()
{
	short result;
	InvokeHelper(0x6004001c, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter::SetWidth(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x6004001c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IDaSvrCharacter::GetHeight()
{
	short result;
	InvokeHelper(0x6004001d, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter::SetHeight(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x6004001d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IDaSvrCharacter::GetOriginalWidth()
{
	short result;
	InvokeHelper(0x6004001e, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short IDaSvrCharacter::GetOriginalHeight()
{
	short result;
	InvokeHelper(0x6004001f, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

BOOL IDaSvrCharacter::GetVisible()
{
	BOOL result;
	InvokeHelper(0x60040020, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long IDaSvrCharacter::GetActiveState()
{
	long result;
	InvokeHelper(0x60040021, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter::SetActiveState(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60040021, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL IDaSvrCharacter::GetIdleState()
{
	BOOL result;
	InvokeHelper(0x60040022, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long IDaSvrCharacter::GetOtherClientCount()
{
	long result;
	InvokeHelper(0x60040023, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long IDaSvrCharacter::GetMoveCause()
{
	long result;
	InvokeHelper(0x60040024, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long IDaSvrCharacter::GetVisibilityCause()
{
	long result;
	InvokeHelper(0x60040025, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long IDaSvrCharacter::GetListeningStatus()
{
	long result;
	InvokeHelper(0x60040026, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH IDaSvrCharacter::GetAnimationNames()
{
	LPDISPATCH result;
	InvokeHelper(0x60040027, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrBalloon properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrBalloon operations

BOOL IDaSvrBalloon::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long IDaSvrBalloon::GetStyle()
{
	long result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon::SetStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL IDaSvrBalloon::GetVisible()
{
	BOOL result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long IDaSvrBalloon::GetNumberOfLines()
{
	long result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon::SetNumberOfLines(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IDaSvrBalloon::GetCharsPerLine()
{
	long result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon::SetCharsPerLine(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IDaSvrBalloon::GetTextColor()
{
	long result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon::SetTextColor(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IDaSvrBalloon::GetBackColor()
{
	long result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon::SetBackColor(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IDaSvrBalloon::GetBorderColor()
{
	long result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon::SetBorderColor(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString IDaSvrBalloon::GetFontName()
{
	CString result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon::SetFontName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long IDaSvrBalloon::GetFontSize()
{
	long result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon::SetFontSize(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL IDaSvrBalloon::GetFontBold()
{
	BOOL result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon::SetFontBold(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaSvrBalloon::GetFontItalic()
{
	BOOL result;
	InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon::SetFontItalic(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaSvrBalloon::GetFontStrikethru()
{
	BOOL result;
	InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon::SetFontStrikethru(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaSvrBalloon::GetFontUnderline()
{
	BOOL result;
	InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon::SetFontUnderline(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

short IDaSvrBalloon::GetFontCharSet()
{
	short result;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon::SetFontCharSet(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0xf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrCommands properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrCommands operations

void IDaSvrCommands::Add(LPCTSTR Caption, LPCTSTR VoiceGrammar, long Enabled, long Visible, long* CommandID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_PI4;
	InvokeHelper(0x60020008, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Caption, VoiceGrammar, Enabled, Visible, CommandID);
}

void IDaSvrCommands::Insert(LPCTSTR Caption, LPCTSTR VoiceGrammar, long Enabled, long Visible, long RefCommandID, long Before, long* CommandID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PI4;
	InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Caption, VoiceGrammar, Enabled, Visible, RefCommandID, Before, CommandID);
}

void IDaSvrCommands::Remove(long CommandID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CommandID);
}

void IDaSvrCommands::RemoveAll()
{
	InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void IDaSvrCommands::AddEx(LPCTSTR Caption, LPCTSTR VoiceGrammar, LPCTSTR VoiceCaption, long Enabled, long Visible, long HelpContextID, long* CommandID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_PI4;
	InvokeHelper(0x6003000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Caption, VoiceGrammar, VoiceCaption, Enabled, Visible, HelpContextID, CommandID);
}

void IDaSvrCommands::InsertEx(LPCTSTR Caption, LPCTSTR VoiceGrammar, LPCTSTR VoiceCaption, long Enabled, long Visible, long HelpContextID, long RefCommandID, long Before, long* CommandID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PI4;
	InvokeHelper(0x6003000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Caption, VoiceGrammar, VoiceCaption, Enabled, Visible, HelpContextID, RefCommandID, Before, CommandID);
}

LPDISPATCH IDaSvrCommands::GetItem(long Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}

long IDaSvrCommands::GetCount()
{
	long result;
	InvokeHelper(0xfffffff8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH IDaSvrCommands::GetCommand(long CommandID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60040000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		CommandID);
	return result;
}

CString IDaSvrCommands::GetCaption()
{
	CString result;
	InvokeHelper(0x60040001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCommands::SetCaption(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60040001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IDaSvrCommands::GetVoiceGrammar()
{
	CString result;
	InvokeHelper(0x60040002, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCommands::SetVoiceGrammar(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60040002, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL IDaSvrCommands::GetVisible()
{
	BOOL result;
	InvokeHelper(0x60040003, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaSvrCommands::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60040003, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long IDaSvrCommands::GetDefaultCommand()
{
	long result;
	InvokeHelper(0x60040004, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaSvrCommands::SetDefaultCommand(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60040004, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString IDaSvrCommands::GetFontName()
{
	CString result;
	InvokeHelper(0x60040005, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCommands::SetFontName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60040005, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long IDaSvrCommands::GetFontSize()
{
	long result;
	InvokeHelper(0x60040006, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaSvrCommands::SetFontSize(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60040006, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString IDaSvrCommands::GetVoiceCaption()
{
	CString result;
	InvokeHelper(0x60040007, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCommands::SetVoiceCaption(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60040007, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL IDaSvrCommands::GetGlobalVoiceCommandsEnabled()
{
	BOOL result;
	InvokeHelper(0x60040008, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaSvrCommands::SetGlobalVoiceCommandsEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60040008, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrCommand properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrCommand operations

CString IDaSvrCommand::GetCaption()
{
	CString result;
	InvokeHelper(0x60030004, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCommand::SetCaption(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030004, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL IDaSvrCommand::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x60030005, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaSvrCommand::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60030005, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaSvrCommand::GetVisible()
{
	BOOL result;
	InvokeHelper(0x60030006, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaSvrCommand::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60030006, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long IDaSvrCommand::GetCommandID()
{
	long result;
	InvokeHelper(0x60030007, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString IDaSvrCommand::GetVoiceCaption()
{
	CString result;
	InvokeHelper(0x60030008, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCommand::SetVoiceCaption(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030008, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IDaSvrCommand::GetVoiceGrammar()
{
	CString result;
	InvokeHelper(0x60030009, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCommand::SetVoiceGrammar(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030009, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long IDaSvrCommand::GetConfidenceThreshold()
{
	long result;
	InvokeHelper(0x6003000a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaSvrCommand::SetConfidenceThreshold(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6003000a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString IDaSvrCommand::GetConfidenceText()
{
	CString result;
	InvokeHelper(0x6003000b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCommand::SetConfidenceText(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6003000b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrPropertySheet properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrPropertySheet operations

void IDaSvrPropertySheet::GetPosition(long* Left, long* Top)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Left, Top);
}

void IDaSvrPropertySheet::GetSize(long* Width, long* Height)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Width, Height);
}

void IDaSvrPropertySheet::SetLeft(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IDaSvrPropertySheet::GetLeft()
{
	short result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IDaSvrPropertySheet::SetTop(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IDaSvrPropertySheet::GetTop()
{
	short result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short IDaSvrPropertySheet::GetHeight()
{
	short result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short IDaSvrPropertySheet::GetWidth()
{
	short result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IDaSvrPropertySheet::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaSvrPropertySheet::GetVisible()
{
	BOOL result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaSvrPropertySheet::SetPage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IDaSvrPropertySheet::GetPage()
{
	CString result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrPropertySheet::SetPosition(long Left, long Top)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Left, Top);
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrCommandsWindow properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrCommandsWindow operations

void IDaSvrCommandsWindow::GetPosition(long* Left, long* Top)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Left, Top);
}

void IDaSvrCommandsWindow::GetSize(long* Width, long* Height)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Width, Height);
}

BOOL IDaSvrCommandsWindow::GetVisible()
{
	BOOL result;
	InvokeHelper(0x60030001, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaSvrCommandsWindow::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60030001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

short IDaSvrCommandsWindow::GetLeft()
{
	short result;
	InvokeHelper(0x60030002, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short IDaSvrCommandsWindow::GetTop()
{
	short result;
	InvokeHelper(0x60030003, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short IDaSvrCommandsWindow::GetHeight()
{
	short result;
	InvokeHelper(0x60030004, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short IDaSvrCommandsWindow::GetWidth()
{
	short result;
	InvokeHelper(0x60030005, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrSetting properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrSetting operations

BOOL IDaSvrSetting::GetSoundEffectsEnabled()
{
	BOOL result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL IDaSvrSetting::GetBalloonEnabled()
{
	BOOL result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH IDaSvrSetting::GetBalloonFont()
{
	LPDISPATCH result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL IDaSvrSetting::GetTTSEnabled()
{
	BOOL result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

short IDaSvrSetting::GetTTSSpeed()
{
	short result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

BOOL IDaSvrSetting::GetSREnabled()
{
	BOOL result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString IDaSvrSetting::GetSRHotKey()
{
	CString result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

short IDaSvrSetting::GetSRHotKeyTime()
{
	short result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

BOOL IDaSvrSetting::GetSRListeningTip()
{
	BOOL result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL IDaSvrSetting::GetSRListeningPrompt()
{
	BOOL result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long IDaSvrSetting::GetAudioStatus()
{
	long result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}
