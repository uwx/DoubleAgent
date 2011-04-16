// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CDaCtlCharacters wrapper class

class CDaCtlCharacters : public COleDispatchDriver
{
public:
	CDaCtlCharacters(){} // Calls COleDispatchDriver default constructor
	CDaCtlCharacters(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDaCtlCharacters(const CDaCtlCharacters& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDaCtlCharacters2 methods
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
	LPUNKNOWN get__NewEnum()
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
	long get_Count()
	{
		long result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Index(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
		return result;
	}

	// IDaCtlCharacters2 properties
public:

};
