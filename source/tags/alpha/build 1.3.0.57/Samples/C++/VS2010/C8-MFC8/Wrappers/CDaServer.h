// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CDaServer wrapper class

class CDaServer : public COleDispatchDriver
{
public:
	CDaServer(){} // Calls COleDispatchDriver default constructor
	CDaServer(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDaServer(const CDaServer& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDaServer2 methods
public:
	void Load(VARIANT& LoadKey, long * CharacterID, long * RequestID)
	{
		static BYTE parms[] = VTS_VARIANT VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &LoadKey, CharacterID, RequestID);
	}
	void Unload(long CharacterID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, CharacterID);
	}
	void Register(LPUNKNOWN NotifySink, long * Cookie)
	{
		static BYTE parms[] = VTS_UNKNOWN VTS_PI4 ;
		InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, NotifySink, Cookie);
	}
	void Unregister(long Cookie)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Cookie);
	}
	void GetCharacter(long CharacterID, LPDISPATCH * Character)
	{
		static BYTE parms[] = VTS_I4 VTS_PDISPATCH ;
		InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms, CharacterID, Character);
	}
	void GetSuspended(long * Suspended)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Suspended);
	}
	void GetCharacterEx(long CharacterID, LPDISPATCH * Character)
	{
		static BYTE parms[] = VTS_I4 VTS_PDISPATCH ;
		InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, CharacterID, Character);
	}
	void GetVersion(short * MajorVersion, short * MinorVersion)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, MajorVersion, MinorVersion);
	}
	void ShowDefaultCharacterProperties(short x, short y, long UseDefaultPosition)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_I4 ;
		InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, x, y, UseDefaultPosition);
	}
	LPDISPATCH get_Character(long CharacterID)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60040000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, CharacterID);
		return result;
	}
	LPDISPATCH get_CharacterFiles()
	{
		LPDISPATCH result;
		InvokeHelper(0x60040001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_CharacterStyle()
	{
		long result;
		InvokeHelper(0x60040002, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_CharacterStyle(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60040002, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_TTSEngines()
	{
		LPDISPATCH result;
		InvokeHelper(0x60040003, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH FindTTSEngines(long LanguageID, short Gender)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 VTS_I2 ;
		InvokeHelper(0x60040004, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, LanguageID, Gender);
		return result;
	}
	LPDISPATCH GetCharacterTTSEngine(VARIANT& LoadKey)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x60040005, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &LoadKey);
		return result;
	}
	LPDISPATCH FindCharacterTTSEngines(VARIANT& LoadKey, long LanguageID)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT VTS_I4 ;
		InvokeHelper(0x60040006, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &LoadKey, LanguageID);
		return result;
	}
	LPDISPATCH get_SREngines()
	{
		LPDISPATCH result;
		InvokeHelper(0x60040007, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH FindSREngines(long LanguageID)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60040008, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, LanguageID);
		return result;
	}
	LPDISPATCH GetCharacterSREngine(VARIANT& LoadKey)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x60040009, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &LoadKey);
		return result;
	}
	LPDISPATCH FindCharacterSREngines(VARIANT& LoadKey, long LanguageID)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT VTS_I4 ;
		InvokeHelper(0x6004000a, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &LoadKey, LanguageID);
		return result;
	}
	LPDISPATCH get_PropertySheet()
	{
		LPDISPATCH result;
		InvokeHelper(0x6004000b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_CommandsWindow()
	{
		LPDISPATCH result;
		InvokeHelper(0x6004000c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Settings()
	{
		LPDISPATCH result;
		InvokeHelper(0x6004000d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}

	// IDaServer2 properties
public:

};
