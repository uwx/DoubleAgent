// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CAgentCtlCharacters wrapper class

class CAgentCtlCharacters : public COleDispatchDriver
{
public:
	CAgentCtlCharacters(){} // Calls COleDispatchDriver default constructor
	CAgentCtlCharacters(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAgentCtlCharacters(const CAgentCtlCharacters& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IAgentCtlCharacters methods
public:
	LPDISPATCH get_Item(LPCTSTR CharacterID)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, CharacterID);
		return result;
	}
	LPDISPATCH Character(LPCTSTR CharacterID)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, CharacterID);
		return result;
	}
	LPUNKNOWN get_Enum()
	{
		LPUNKNOWN result;
		InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}
	void Unload(LPCTSTR CharacterID)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, CharacterID);
	}
	LPDISPATCH Load(LPCTSTR CharacterID, VARIANT& LoadKey)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR VTS_VARIANT ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, CharacterID, &LoadKey);
		return result;
	}

	// IAgentCtlCharacters properties
public:

};
