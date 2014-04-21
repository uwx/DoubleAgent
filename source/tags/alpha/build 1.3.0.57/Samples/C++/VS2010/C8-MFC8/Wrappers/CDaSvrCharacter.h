// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CDaSvrCharacter wrapper class

class CDaSvrCharacter : public COleDispatchDriver
{
public:
	CDaSvrCharacter(){} // Calls COleDispatchDriver default constructor
	CDaSvrCharacter(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDaSvrCharacter(const CDaSvrCharacter& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDaSvrCharacter2 methods
public:
	void GetVisible(long * Visible)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Visible);
	}
	void SetPosition(long Left, long Top)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Left, Top);
	}
	void GetPosition(long * Left, long * Top)
	{
		static BYTE parms[] = VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Left, Top);
	}
	void SetSize(long Width, long Height)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Width, Height);
	}
	void GetSize(long * Width, long * Height)
	{
		static BYTE parms[] = VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Width, Height);
	}
	void GetName(BSTR * Name)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Name);
	}
	void GetDescription(BSTR * Description)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Description);
	}
	void GetTTSSpeed(long * Speed)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020007, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Speed);
	}
	void GetTTSPitch(short * Pitch)
	{
		static BYTE parms[] = VTS_PI2 ;
		InvokeHelper(0x60020008, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Pitch);
	}
	void Activate(short State)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms, State);
	}
	void SetIdleOn(long On)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, On);
	}
	void GetIdleOn(long * On)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, On);
	}
	void Prepare(long Type, LPCTSTR Name, long Queue, long * RequestID)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR VTS_I4 VTS_PI4 ;
		InvokeHelper(0x6002000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Type, Name, Queue, RequestID);
	}
	void Play(LPCTSTR Animation, long * RequestID)
	{
		static BYTE parms[] = VTS_BSTR VTS_PI4 ;
		InvokeHelper(0x6002000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Animation, RequestID);
	}
	void Stop(long RequestID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6002000e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, RequestID);
	}
	void StopAll(long Types)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6002000f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Types);
	}
	void Wait(long WaitForRequestID, long * RequestID)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 ;
		InvokeHelper(0x60020010, DISPATCH_METHOD, VT_EMPTY, NULL, parms, WaitForRequestID, RequestID);
	}
	void Interrupt(long InterruptRequestID, long * RequestID)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 ;
		InvokeHelper(0x60020011, DISPATCH_METHOD, VT_EMPTY, NULL, parms, InterruptRequestID, RequestID);
	}
	void Show(long Fast, long * RequestID)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 ;
		InvokeHelper(0x60020012, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Fast, RequestID);
	}
	void Hide(long Fast, long * RequestID)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 ;
		InvokeHelper(0x60020013, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Fast, RequestID);
	}
	void Speak(LPCTSTR Text, LPCTSTR Url, long * RequestID)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_PI4 ;
		InvokeHelper(0x60020014, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Text, Url, RequestID);
	}
	void MoveTo(short x, short y, long Speed, long * RequestID)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x60020015, DISPATCH_METHOD, VT_EMPTY, NULL, parms, x, y, Speed, RequestID);
	}
	void GestureAt(short x, short y, long * RequestID)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_PI4 ;
		InvokeHelper(0x60020016, DISPATCH_METHOD, VT_EMPTY, NULL, parms, x, y, RequestID);
	}
	void GetMoveCause(long * Cause)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020017, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Cause);
	}
	void GetVisibilityCause(long * Cause)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020018, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Cause);
	}
	void HasOtherClients(long * OtherClientCount)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020019, DISPATCH_METHOD, VT_EMPTY, NULL, parms, OtherClientCount);
	}
	void SetSoundEffectsOn(long On)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6002001a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, On);
	}
	void GetSoundEffectsOn(long * On)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x6002001b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, On);
	}
	void SetName(LPCTSTR Name)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6002001c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Name);
	}
	void SetDescription(LPCTSTR Description)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6002001d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Description);
	}
	void GetExtraData(BSTR * ExtraData)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x6002001e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ExtraData);
	}
	void ShowPopupMenu(short x, short y)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, x, y);
	}
	void SetAutoPopupMenu(long AutoPopupMenu)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, AutoPopupMenu);
	}
	void GetAutoPopupMenu(long * AutoPopupMenu)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, AutoPopupMenu);
	}
	void GetHelpFileName(BSTR * Name)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60030003, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Name);
	}
	void SetHelpFileName(LPCTSTR Name)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60030004, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Name);
	}
	void SetHelpModeOn(long HelpModeOn)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030005, DISPATCH_METHOD, VT_EMPTY, NULL, parms, HelpModeOn);
	}
	void GetHelpModeOn(long * HelpModeOn)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60030006, DISPATCH_METHOD, VT_EMPTY, NULL, parms, HelpModeOn);
	}
	void SetHelpContextID(long ContextID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030007, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ContextID);
	}
	void GetHelpContextID(long * ContextID)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60030008, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ContextID);
	}
	void GetActive(short * State)
	{
		static BYTE parms[] = VTS_PI2 ;
		InvokeHelper(0x60030009, DISPATCH_METHOD, VT_EMPTY, NULL, parms, State);
	}
	void Listen(long Listen)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6003000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Listen);
	}
	void SetLanguageID(long LanguageID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6003000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, LanguageID);
	}
	void GetLanguageID(long * LanguageID)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x6003000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, LanguageID);
	}
	void GetTTSModeID(BSTR * ModeID)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x6003000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ModeID);
	}
	void SetTTSModeID(LPCTSTR ModeID)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6003000e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ModeID);
	}
	void GetSRModeID(BSTR * ModeID)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x6003000f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ModeID);
	}
	void SetSRModeID(LPCTSTR ModeID)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60030010, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ModeID);
	}
	void GetGUID(BSTR * CharGUID)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60030011, DISPATCH_METHOD, VT_EMPTY, NULL, parms, CharGUID);
	}
	void GetOriginalSize(long * Width, long * Height)
	{
		static BYTE parms[] = VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x60030012, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Width, Height);
	}
	void Think(LPCTSTR Text, long * RequestID)
	{
		static BYTE parms[] = VTS_BSTR VTS_PI4 ;
		InvokeHelper(0x60030013, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Text, RequestID);
	}
	void GetVersion(short * MajorVersion, short * MinorVersion)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x60030014, DISPATCH_METHOD, VT_EMPTY, NULL, parms, MajorVersion, MinorVersion);
	}
	void GetAnimationNames(LPUNKNOWN * punkEnum)
	{
		static BYTE parms[] = VTS_PUNKNOWN ;
		InvokeHelper(0x60030015, DISPATCH_METHOD, VT_EMPTY, NULL, parms, punkEnum);
	}
	void GetSRStatus(long * Status)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60030016, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Status);
	}
	LPDISPATCH get_Balloon()
	{
		LPDISPATCH result;
		InvokeHelper(0x60040000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Commands()
	{
		LPDISPATCH result;
		InvokeHelper(0x60040001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_Style()
	{
		long result;
		InvokeHelper(0x60040002, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Style(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60040002, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_HasIcon()
	{
		BOOL result;
		InvokeHelper(0x60040003, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void GenerateIcon(long ClipLeft, long ClipTop, long ClipWidth, long ClipHeight)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x60040004, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ClipLeft, ClipTop, ClipWidth, ClipHeight);
	}
	BOOL get_IconVisible()
	{
		BOOL result;
		InvokeHelper(0x60040005, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	CString get_IconIdentity()
	{
		CString result;
		InvokeHelper(0x60040008, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_IconIdentity(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60040008, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void GetIconIdentity(GUID * IconIdentity)
	{
		static BYTE parms[] = VTS_UNKNOWN ;
		InvokeHelper(0x60040006, DISPATCH_METHOD, VT_EMPTY, NULL, parms, IconIdentity);
	}
	void SetIconIdentity(GUID * IconIdentity)
	{
		static BYTE parms[] = VTS_UNKNOWN ;
		InvokeHelper(0x60040007, DISPATCH_METHOD, VT_EMPTY, NULL, parms, IconIdentity);
	}
	CString get_IconTip()
	{
		CString result;
		InvokeHelper(0x60040009, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_IconTip(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60040009, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_TTSEngine(BOOL GetDefault)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6004000a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, GetDefault);
		return result;
	}
	LPDISPATCH FindTTSEngines(long LanguageID)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6004000b, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, LanguageID);
		return result;
	}
	LPDISPATCH get_SREngine(BOOL GetDefault)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6004000c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, GetDefault);
		return result;
	}
	LPDISPATCH FindSREngines(long LanguageID)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6004000d, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, LanguageID);
		return result;
	}
	long get_CharacterID()
	{
		long result;
		InvokeHelper(0x60040010, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString get_UniqueID()
	{
		CString result;
		InvokeHelper(0x60040011, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_Name()
	{
		CString result;
		InvokeHelper(0x60040012, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Name(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60040012, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_Description()
	{
		CString result;
		InvokeHelper(0x60040013, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Description(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60040013, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_ExtraData()
	{
		CString result;
		InvokeHelper(0x60040014, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_FileName()
	{
		CString result;
		InvokeHelper(0x60040015, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_FilePath()
	{
		CString result;
		InvokeHelper(0x60040016, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long get_LanguageID()
	{
		long result;
		InvokeHelper(0x60040017, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_LanguageID(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60040017, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_TTSModeID()
	{
		CString result;
		InvokeHelper(0x60040018, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_TTSModeID(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60040018, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_SRModeID()
	{
		CString result;
		InvokeHelper(0x60040019, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_SRModeID(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60040019, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	short get_Left()
	{
		short result;
		InvokeHelper(0x6004001a, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	void put_Left(short newValue)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x6004001a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	short get_Top()
	{
		short result;
		InvokeHelper(0x6004001b, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	void put_Top(short newValue)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x6004001b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	short get_Width()
	{
		short result;
		InvokeHelper(0x6004001c, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	void put_Width(short newValue)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x6004001c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	short get_Height()
	{
		short result;
		InvokeHelper(0x6004001d, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	void put_Height(short newValue)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x6004001d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	short get_OriginalWidth()
	{
		short result;
		InvokeHelper(0x6004001e, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	short get_OriginalHeight()
	{
		short result;
		InvokeHelper(0x6004001f, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	BOOL get_Visible()
	{
		BOOL result;
		InvokeHelper(0x60040020, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	long get_ActiveState()
	{
		long result;
		InvokeHelper(0x60040021, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ActiveState(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60040021, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_IdleState()
	{
		BOOL result;
		InvokeHelper(0x60040022, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	long get_OtherClientCount()
	{
		long result;
		InvokeHelper(0x60040023, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_MoveCause()
	{
		long result;
		InvokeHelper(0x60040024, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_VisibilityCause()
	{
		long result;
		InvokeHelper(0x60040025, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_ListeningStatus()
	{
		long result;
		InvokeHelper(0x60040026, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_AnimationNames()
	{
		LPDISPATCH result;
		InvokeHelper(0x60040027, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}

	// IDaSvrCharacter2 properties
public:

};
