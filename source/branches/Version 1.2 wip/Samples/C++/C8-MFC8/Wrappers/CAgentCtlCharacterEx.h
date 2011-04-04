// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CAgentCtlCharacterEx wrapper class

class CAgentCtlCharacterEx : public COleDispatchDriver
{
public:
	CAgentCtlCharacterEx(){} // Calls COleDispatchDriver default constructor
	CAgentCtlCharacterEx(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAgentCtlCharacterEx(const CAgentCtlCharacterEx& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IAgentCtlCharacterEx methods
public:
	LPDISPATCH get_Balloon()
	{
		LPDISPATCH result;
		InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Commands()
	{
		LPDISPATCH result;
		InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString get_Name()
	{
		CString result;
		InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_Description()
	{
		CString result;
		InvokeHelper(0x1c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	BOOL get_Visible()
	{
		BOOL result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Left(short newValue)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	short get_Left()
	{
		short result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	void put_Top(short newValue)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	short get_Top()
	{
		short result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	void put_Height(short newValue)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	short get_Height()
	{
		short result;
		InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	void put_Width(short newValue)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	short get_Width()
	{
		short result;
		InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	long get_Speed()
	{
		long result;
		InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_Pitch()
	{
		long result;
		InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_IdleOn(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x1d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_IdleOn()
	{
		BOOL result;
		InvokeHelper(0x1d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL Activate(VARIANT& State)
	{
		BOOL result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, &State);
		return result;
	}
	LPDISPATCH Play(LPCTSTR Animation)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Animation);
		return result;
	}
	LPDISPATCH Get(LPCTSTR Type, LPCTSTR Name, VARIANT& Queue)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_VARIANT ;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Type, Name, &Queue);
		return result;
	}
	void Stop(VARIANT& Request)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Request);
	}
	LPDISPATCH Wait(LPDISPATCH WaitForRequest)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x16, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, WaitForRequest);
		return result;
	}
	LPDISPATCH Interrupt(LPDISPATCH InterruptRequest)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, InterruptRequest);
		return result;
	}
	LPDISPATCH Speak(VARIANT& Text, VARIANT& Url)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Text, &Url);
		return result;
	}
	LPDISPATCH GestureAt(short x, short y)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, x, y);
		return result;
	}
	LPDISPATCH MoveTo(short x, short y, VARIANT& Speed)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_VARIANT ;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, x, y, &Speed);
		return result;
	}
	LPDISPATCH Hide(VARIANT& Fast)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Fast);
		return result;
	}
	LPDISPATCH Show(VARIANT& Fast)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Fast);
		return result;
	}
	void StopAll(VARIANT& Types)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Types);
	}
	short get_MoveCause()
	{
		short result;
		InvokeHelper(0x20, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	short get_VisibilityCause()
	{
		short result;
		InvokeHelper(0x21, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	BOOL get_HasOtherClients()
	{
		BOOL result;
		InvokeHelper(0x22, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_SoundEffectsOn(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x23, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_SoundEffectsOn()
	{
		BOOL result;
		InvokeHelper(0x23, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Name(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x18, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void put_Description(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_ExtraData()
	{
		CString result;
		InvokeHelper(0x24, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	BOOL ShowPopupMenu(short x, short y)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0x25, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, x, y);
		return result;
	}
	void put_AutoPopupMenu(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x26, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_AutoPopupMenu()
	{
		BOOL result;
		InvokeHelper(0x26, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_HelpModeOn(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x27, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_HelpModeOn()
	{
		BOOL result;
		InvokeHelper(0x27, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_HelpContextID(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x28, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_HelpContextID()
	{
		long result;
		InvokeHelper(0x28, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	short get_Active()
	{
		short result;
		InvokeHelper(0x29, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	BOOL Listen(BOOL Listen)
	{
		BOOL result;
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x2a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, Listen);
		return result;
	}
	void put_LanguageID(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_LanguageID()
	{
		long result;
		InvokeHelper(0x2b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString get_SRModeID()
	{
		CString result;
		InvokeHelper(0x2e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_SRModeID(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x2e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_TTSModeID()
	{
		CString result;
		InvokeHelper(0x2f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_TTSModeID(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x2f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_HelpFile()
	{
		CString result;
		InvokeHelper(0x30, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_HelpFile(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x30, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_GUID()
	{
		CString result;
		InvokeHelper(0x31, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	short get_OriginalHeight()
	{
		short result;
		InvokeHelper(0x32, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	short get_OriginalWidth()
	{
		short result;
		InvokeHelper(0x33, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH Think(LPCTSTR Text)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x34, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Text);
		return result;
	}
	CString get_Version()
	{
		CString result;
		InvokeHelper(0x35, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_AnimationNames()
	{
		LPDISPATCH result;
		InvokeHelper(0x36, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_SRStatus()
	{
		long result;
		InvokeHelper(0x37, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}

	// IAgentCtlCharacterEx properties
public:

};
