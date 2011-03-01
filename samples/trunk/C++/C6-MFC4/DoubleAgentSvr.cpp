// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "DoubleAgentSvr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



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


/////////////////////////////////////////////////////////////////////////////
// IDaSvrUserInput2 properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrUserInput2 operations

void IDaSvrUserInput2::GetAllItemData(VARIANT* ItemIndices, VARIANT* ItemConfidences, VARIANT* ItemText)
{
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ItemIndices, ItemConfidences, ItemText);
}

long IDaSvrUserInput2::GetCount()
{
	long result;
	InvokeHelper(0x60030000, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long IDaSvrUserInput2::GetItemCommandID(long ItemIndex)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030001, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms,
		ItemIndex);
	return result;
}

long IDaSvrUserInput2::GetItemConfidence(long ItemIndex)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030002, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms,
		ItemIndex);
	return result;
}

CString IDaSvrUserInput2::GetItemText(long ItemIndex)
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
// IDaServer2 properties

/////////////////////////////////////////////////////////////////////////////
// IDaServer2 operations

void IDaServer2::Load(const VARIANT& Provider, long* CharacterID, long* RequestID)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Provider, CharacterID, RequestID);
}

void IDaServer2::Unload(long CharacterID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CharacterID);
}

void IDaServer2::GetVersion(short* MajorVersion, short* MinorVersion)
{
	static BYTE parms[] =
		VTS_PI2 VTS_PI2;
	InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 MajorVersion, MinorVersion);
}

LPDISPATCH IDaServer2::GetCharacter(long CharacterID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60040000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		CharacterID);
	return result;
}

LPDISPATCH IDaServer2::GetCharacterFiles()
{
	LPDISPATCH result;
	InvokeHelper(0x60040001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long IDaServer2::GetCharacterStyle()
{
	long result;
	InvokeHelper(0x60040002, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaServer2::SetCharacterStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60040002, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH IDaServer2::GetTTSEngines()
{
	LPDISPATCH result;
	InvokeHelper(0x60040003, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IDaServer2::FindTTSEngines(long LanguageID, short Gender)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_I2;
	InvokeHelper(0x60040004, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		LanguageID, Gender);
	return result;
}

LPDISPATCH IDaServer2::GetCharacterTTSEngine(const VARIANT& Provider)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x60040005, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Provider);
	return result;
}

LPDISPATCH IDaServer2::FindCharacterTTSEngines(const VARIANT& Provider, long LanguageID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_I4;
	InvokeHelper(0x60040006, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Provider, LanguageID);
	return result;
}

LPDISPATCH IDaServer2::GetSREngines()
{
	LPDISPATCH result;
	InvokeHelper(0x60040007, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IDaServer2::FindSREngines(long LanguageID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60040008, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		LanguageID);
	return result;
}

LPDISPATCH IDaServer2::GetCharacterSREngine(const VARIANT& Provider)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x60040009, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Provider);
	return result;
}

LPDISPATCH IDaServer2::FindCharacterSREngines(const VARIANT& Provider, long LanguageID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_I4;
	InvokeHelper(0x6004000a, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Provider, LanguageID);
	return result;
}

LPDISPATCH IDaServer2::GetPropertySheet()
{
	LPDISPATCH result;
	InvokeHelper(0x6004000b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IDaServer2::GetCommandsWindow()
{
	LPDISPATCH result;
	InvokeHelper(0x6004000c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IDaServer2::GetSettings()
{
	LPDISPATCH result;
	InvokeHelper(0x6004000d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrCharacter2 properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrCharacter2 operations

void IDaSvrCharacter2::SetPosition(long Left, long Top)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Left, Top);
}

void IDaSvrCharacter2::GetPosition(long* Left, long* Top)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Left, Top);
}

void IDaSvrCharacter2::SetSize(long Width, long Height)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Width, Height);
}

void IDaSvrCharacter2::GetSize(long* Width, long* Height)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Width, Height);
}

void IDaSvrCharacter2::Activate(short State)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 State);
}

void IDaSvrCharacter2::Prepare(long Type, LPCTSTR Name, long Queue, long* RequestID)
{
	static BYTE parms[] =
		VTS_I4 VTS_BSTR VTS_I4 VTS_PI4;
	InvokeHelper(0x6002000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Type, Name, Queue, RequestID);
}

void IDaSvrCharacter2::Play(LPCTSTR Animation, long* RequestID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_PI4;
	InvokeHelper(0x6002000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Animation, RequestID);
}

void IDaSvrCharacter2::Stop(long RequestID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 RequestID);
}

void IDaSvrCharacter2::StopAll(long Types)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Types);
}

void IDaSvrCharacter2::Wait(long WaitForRequestID, long* RequestID)
{
	static BYTE parms[] =
		VTS_I4 VTS_PI4;
	InvokeHelper(0x60020010, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 WaitForRequestID, RequestID);
}

void IDaSvrCharacter2::Interrupt(long InterruptRequestID, long* RequestID)
{
	static BYTE parms[] =
		VTS_I4 VTS_PI4;
	InvokeHelper(0x60020011, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 InterruptRequestID, RequestID);
}

void IDaSvrCharacter2::Show(long Fast, long* RequestID)
{
	static BYTE parms[] =
		VTS_I4 VTS_PI4;
	InvokeHelper(0x60020012, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Fast, RequestID);
}

void IDaSvrCharacter2::Hide(long Fast, long* RequestID)
{
	static BYTE parms[] =
		VTS_I4 VTS_PI4;
	InvokeHelper(0x60020013, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Fast, RequestID);
}

void IDaSvrCharacter2::Speak(LPCTSTR Text, LPCTSTR Url, long* RequestID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_PI4;
	InvokeHelper(0x60020014, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Text, Url, RequestID);
}

void IDaSvrCharacter2::MoveTo(short X, short Y, long Speed, long* RequestID)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I4 VTS_PI4;
	InvokeHelper(0x60020015, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 X, Y, Speed, RequestID);
}

void IDaSvrCharacter2::GestureAt(short X, short Y, long* RequestID)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_PI4;
	InvokeHelper(0x60020016, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 X, Y, RequestID);
}

void IDaSvrCharacter2::ShowPopupMenu(short X, short Y)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 X, Y);
}

void IDaSvrCharacter2::Listen(long Listen)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6003000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Listen);
}

void IDaSvrCharacter2::GetOriginalSize(long* Width, long* Height)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60030012, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Width, Height);
}

void IDaSvrCharacter2::Think(LPCTSTR Text, long* RequestID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_PI4;
	InvokeHelper(0x60030013, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Text, RequestID);
}

void IDaSvrCharacter2::GetVersion(short* MajorVersion, short* MinorVersion)
{
	static BYTE parms[] =
		VTS_PI2 VTS_PI2;
	InvokeHelper(0x60030014, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 MajorVersion, MinorVersion);
}

LPDISPATCH IDaSvrCharacter2::GetBalloon()
{
	LPDISPATCH result;
	InvokeHelper(0x60040000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IDaSvrCharacter2::GetCommands()
{
	LPDISPATCH result;
	InvokeHelper(0x60040001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long IDaSvrCharacter2::GetStyle()
{
	long result;
	InvokeHelper(0x60040002, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter2::SetStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60040002, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL IDaSvrCharacter2::GetHasIcon()
{
	BOOL result;
	InvokeHelper(0x60040003, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter2::GenerateIcon(long ClipLeft, long ClipTop, long ClipWidth, long ClipHeight)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x60040004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ClipLeft, ClipTop, ClipWidth, ClipHeight);
}

BOOL IDaSvrCharacter2::GetIconVisible()
{
	BOOL result;
	InvokeHelper(0x60040005, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString IDaSvrCharacter2::GetIconIdentity()
{
	CString result;
	InvokeHelper(0x60040008, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter2::SetIconIdentity(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60040008, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IDaSvrCharacter2::GetIconTip()
{
	CString result;
	InvokeHelper(0x60040009, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter2::SetIconTip(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60040009, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH IDaSvrCharacter2::GetTTSEngine(BOOL GetDefault)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6004000a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		GetDefault);
	return result;
}

LPDISPATCH IDaSvrCharacter2::FindTTSEngines(long LanguageID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6004000b, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		LanguageID);
	return result;
}

LPDISPATCH IDaSvrCharacter2::GetSREngine(BOOL GetDefault)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6004000c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		GetDefault);
	return result;
}

LPDISPATCH IDaSvrCharacter2::FindSREngines(long LanguageID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6004000d, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		LanguageID);
	return result;
}

long IDaSvrCharacter2::GetCharacterID()
{
	long result;
	InvokeHelper(0x60040010, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString IDaSvrCharacter2::GetUniqueID()
{
	CString result;
	InvokeHelper(0x60040011, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IDaSvrCharacter2::GetName()
{
	CString result;
	InvokeHelper(0x60040012, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter2::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60040012, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IDaSvrCharacter2::GetDescription()
{
	CString result;
	InvokeHelper(0x60040013, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter2::SetDescription(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60040013, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IDaSvrCharacter2::GetExtraData()
{
	CString result;
	InvokeHelper(0x60040014, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IDaSvrCharacter2::GetFileName()
{
	CString result;
	InvokeHelper(0x60040015, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IDaSvrCharacter2::GetFilePath()
{
	CString result;
	InvokeHelper(0x60040016, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long IDaSvrCharacter2::GetLanguageID()
{
	long result;
	InvokeHelper(0x60040017, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter2::SetLanguageID(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60040017, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString IDaSvrCharacter2::GetTTSModeID()
{
	CString result;
	InvokeHelper(0x60040018, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter2::SetTTSModeID(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60040018, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IDaSvrCharacter2::GetSRModeID()
{
	CString result;
	InvokeHelper(0x60040019, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter2::SetSRModeID(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60040019, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

short IDaSvrCharacter2::GetLeft()
{
	short result;
	InvokeHelper(0x6004001a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter2::SetLeft(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x6004001a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IDaSvrCharacter2::GetTop()
{
	short result;
	InvokeHelper(0x6004001b, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter2::SetTop(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x6004001b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IDaSvrCharacter2::GetWidth()
{
	short result;
	InvokeHelper(0x6004001c, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter2::SetWidth(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x6004001c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IDaSvrCharacter2::GetHeight()
{
	short result;
	InvokeHelper(0x6004001d, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter2::SetHeight(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x6004001d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IDaSvrCharacter2::GetOriginalWidth()
{
	short result;
	InvokeHelper(0x6004001e, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short IDaSvrCharacter2::GetOriginalHeight()
{
	short result;
	InvokeHelper(0x6004001f, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

BOOL IDaSvrCharacter2::GetVisible()
{
	BOOL result;
	InvokeHelper(0x60040020, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long IDaSvrCharacter2::GetActiveState()
{
	long result;
	InvokeHelper(0x60040021, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaSvrCharacter2::SetActiveState(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60040021, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL IDaSvrCharacter2::GetIdleState()
{
	BOOL result;
	InvokeHelper(0x60040022, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long IDaSvrCharacter2::GetOtherClientCount()
{
	long result;
	InvokeHelper(0x60040023, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long IDaSvrCharacter2::GetMoveCause()
{
	long result;
	InvokeHelper(0x60040024, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long IDaSvrCharacter2::GetVisibilityCause()
{
	long result;
	InvokeHelper(0x60040025, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long IDaSvrCharacter2::GetListeningStatus()
{
	long result;
	InvokeHelper(0x60040026, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrBalloon2 properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrBalloon2 operations

BOOL IDaSvrBalloon2::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long IDaSvrBalloon2::GetStyle()
{
	long result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon2::SetStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL IDaSvrBalloon2::GetVisible()
{
	BOOL result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon2::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long IDaSvrBalloon2::GetNumberOfLines()
{
	long result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon2::SetNumberOfLines(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IDaSvrBalloon2::GetCharsPerLine()
{
	long result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon2::SetCharsPerLine(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

unsigned long IDaSvrBalloon2::GetTextColor()
{
	unsigned long result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon2::SetTextColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

unsigned long IDaSvrBalloon2::GetBackColor()
{
	unsigned long result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon2::SetBackColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

unsigned long IDaSvrBalloon2::GetBorderColor()
{
	unsigned long result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon2::SetBorderColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

CString IDaSvrBalloon2::GetFontName()
{
	CString result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon2::SetFontName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long IDaSvrBalloon2::GetFontSize()
{
	long result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon2::SetFontSize(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL IDaSvrBalloon2::GetFontBold()
{
	BOOL result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon2::SetFontBold(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaSvrBalloon2::GetFontItalic()
{
	BOOL result;
	InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon2::SetFontItalic(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaSvrBalloon2::GetFontStrikethru()
{
	BOOL result;
	InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon2::SetFontStrikethru(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaSvrBalloon2::GetFontUnderline()
{
	BOOL result;
	InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon2::SetFontUnderline(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

short IDaSvrBalloon2::GetFontCharSet()
{
	short result;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IDaSvrBalloon2::SetFontCharSet(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0xf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrCommands2 properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrCommands2 operations

void IDaSvrCommands2::Add(LPCTSTR Caption, LPCTSTR VoiceGrammar, long Enabled, long Visible, long* CommandID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_PI4;
	InvokeHelper(0x60020008, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Caption, VoiceGrammar, Enabled, Visible, CommandID);
}

void IDaSvrCommands2::Insert(LPCTSTR Caption, LPCTSTR VoiceGrammar, long Enabled, long Visible, long RefCommandID, long Before, long* CommandID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PI4;
	InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Caption, VoiceGrammar, Enabled, Visible, RefCommandID, Before, CommandID);
}

void IDaSvrCommands2::Remove(long CommandID)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CommandID);
}

void IDaSvrCommands2::RemoveAll()
{
	InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void IDaSvrCommands2::AddEx(LPCTSTR Caption, LPCTSTR VoiceGrammar, LPCTSTR VoiceCaption, long Enabled, long Visible, long HelpContextID, long* CommandID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_PI4;
	InvokeHelper(0x6003000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Caption, VoiceGrammar, VoiceCaption, Enabled, Visible, HelpContextID, CommandID);
}

void IDaSvrCommands2::InsertEx(LPCTSTR Caption, LPCTSTR VoiceGrammar, LPCTSTR VoiceCaption, long Enabled, long Visible, long HelpContextID, long RefCommandID, long Before, long* CommandID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PI4;
	InvokeHelper(0x6003000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Caption, VoiceGrammar, VoiceCaption, Enabled, Visible, HelpContextID, RefCommandID, Before, CommandID);
}

LPDISPATCH IDaSvrCommands2::GetItem(long Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}

long IDaSvrCommands2::GetCount()
{
	long result;
	InvokeHelper(0xfffffff8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH IDaSvrCommands2::GetCommand(long CommandID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60040000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms,
		CommandID);
	return result;
}

CString IDaSvrCommands2::GetCaption()
{
	CString result;
	InvokeHelper(0x60040001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCommands2::SetCaption(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60040001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IDaSvrCommands2::GetVoiceGrammar()
{
	CString result;
	InvokeHelper(0x60040002, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCommands2::SetVoiceGrammar(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60040002, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL IDaSvrCommands2::GetVisible()
{
	BOOL result;
	InvokeHelper(0x60040003, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaSvrCommands2::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60040003, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long IDaSvrCommands2::GetDefaultCommand()
{
	long result;
	InvokeHelper(0x60040004, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaSvrCommands2::SetDefaultCommand(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60040004, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString IDaSvrCommands2::GetFontName()
{
	CString result;
	InvokeHelper(0x60040005, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCommands2::SetFontName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60040005, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long IDaSvrCommands2::GetFontSize()
{
	long result;
	InvokeHelper(0x60040006, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaSvrCommands2::SetFontSize(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60040006, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString IDaSvrCommands2::GetVoiceCaption()
{
	CString result;
	InvokeHelper(0x60040007, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCommands2::SetVoiceCaption(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60040007, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL IDaSvrCommands2::GetGlobalVoiceCommandsEnabled()
{
	BOOL result;
	InvokeHelper(0x60040008, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaSvrCommands2::SetGlobalVoiceCommandsEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60040008, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrCommand2 properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrCommand2 operations

CString IDaSvrCommand2::GetCaption()
{
	CString result;
	InvokeHelper(0x60030004, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCommand2::SetCaption(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030004, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL IDaSvrCommand2::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x60030005, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaSvrCommand2::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60030005, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaSvrCommand2::GetVisible()
{
	BOOL result;
	InvokeHelper(0x60030006, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaSvrCommand2::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60030006, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long IDaSvrCommand2::GetCommandID()
{
	long result;
	InvokeHelper(0x60030007, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CString IDaSvrCommand2::GetVoiceCaption()
{
	CString result;
	InvokeHelper(0x60030008, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCommand2::SetVoiceCaption(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030008, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IDaSvrCommand2::GetVoiceGrammar()
{
	CString result;
	InvokeHelper(0x60030009, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCommand2::SetVoiceGrammar(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030009, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long IDaSvrCommand2::GetConfidenceThreshold()
{
	long result;
	InvokeHelper(0x6003000a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDaSvrCommand2::SetConfidenceThreshold(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6003000a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString IDaSvrCommand2::GetConfidenceText()
{
	CString result;
	InvokeHelper(0x6003000b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrCommand2::SetConfidenceText(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6003000b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrPropertySheet2 properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrPropertySheet2 operations

void IDaSvrPropertySheet2::GetPosition(long* Left, long* Top)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Left, Top);
}

void IDaSvrPropertySheet2::GetSize(long* Width, long* Height)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Width, Height);
}

void IDaSvrPropertySheet2::SetLeft(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IDaSvrPropertySheet2::GetLeft()
{
	short result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IDaSvrPropertySheet2::SetTop(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short IDaSvrPropertySheet2::GetTop()
{
	short result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short IDaSvrPropertySheet2::GetHeight()
{
	short result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short IDaSvrPropertySheet2::GetWidth()
{
	short result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void IDaSvrPropertySheet2::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL IDaSvrPropertySheet2::GetVisible()
{
	BOOL result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaSvrPropertySheet2::SetPage(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString IDaSvrPropertySheet2::GetPage()
{
	CString result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IDaSvrPropertySheet2::SetPosition(long Left, long Top)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Left, Top);
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrCommandsWindow2 properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrCommandsWindow2 operations

void IDaSvrCommandsWindow2::GetPosition(long* Left, long* Top)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Left, Top);
}

void IDaSvrCommandsWindow2::GetSize(long* Width, long* Height)
{
	static BYTE parms[] =
		VTS_PI4 VTS_PI4;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Width, Height);
}

BOOL IDaSvrCommandsWindow2::GetVisible()
{
	BOOL result;
	InvokeHelper(0x60030001, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IDaSvrCommandsWindow2::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60030001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

short IDaSvrCommandsWindow2::GetLeft()
{
	short result;
	InvokeHelper(0x60030002, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short IDaSvrCommandsWindow2::GetTop()
{
	short result;
	InvokeHelper(0x60030003, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short IDaSvrCommandsWindow2::GetHeight()
{
	short result;
	InvokeHelper(0x60030004, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short IDaSvrCommandsWindow2::GetWidth()
{
	short result;
	InvokeHelper(0x60030005, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IDaSvrSettings properties

/////////////////////////////////////////////////////////////////////////////
// IDaSvrSettings operations

BOOL IDaSvrSettings::GetSoundEffectsEnabled()
{
	BOOL result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL IDaSvrSettings::GetBalloonEnabled()
{
	BOOL result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH IDaSvrSettings::GetBalloonFont()
{
	LPDISPATCH result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL IDaSvrSettings::GetTTSEnabled()
{
	BOOL result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

short IDaSvrSettings::GetTTSSpeed()
{
	short result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

BOOL IDaSvrSettings::GetSREnabled()
{
	BOOL result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString IDaSvrSettings::GetSRHotKey()
{
	CString result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

short IDaSvrSettings::GetSRHotKeyTime()
{
	short result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

BOOL IDaSvrSettings::GetSRListeningTip()
{
	BOOL result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL IDaSvrSettings::GetSRListeningPrompt()
{
	BOOL result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long IDaSvrSettings::GetAudioStatus()
{
	long result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}
