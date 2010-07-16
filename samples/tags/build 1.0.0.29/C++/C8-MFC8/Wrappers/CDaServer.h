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


	// IDaServer methods
public:
	void Load(VARIANT& vLoadKey, long * pdwCharID, long * pdwReqID)
	{
		static BYTE parms[] = VTS_VARIANT VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &vLoadKey, pdwCharID, pdwReqID);
	}
	void Unload(long dwCharID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwCharID);
	}
	void Register(LPUNKNOWN punkNotifySink, long * pdwSinkID)
	{
		static BYTE parms[] = VTS_UNKNOWN VTS_PI4 ;
		InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, punkNotifySink, pdwSinkID);
	}
	void Unregister(long dwSinkID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwSinkID);
	}
	void GetCharacter(long dwCharID, LPDISPATCH * ppunkCharacter)
	{
		static BYTE parms[] = VTS_I4 VTS_PDISPATCH ;
		InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwCharID, ppunkCharacter);
	}
	void GetSuspended(long * pbSuspended)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbSuspended);
	}
	void GetCharacterEx(long dwCharID, LPDISPATCH * ppCharacterEx)
	{
		static BYTE parms[] = VTS_I4 VTS_PDISPATCH ;
		InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwCharID, ppCharacterEx);
	}
	void GetVersion(short * psMajor, short * psMinor)
	{
		static BYTE parms[] = VTS_PI2 VTS_PI2 ;
		InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, psMajor, psMinor);
	}
	void ShowDefaultCharacterProperties(short x, short y, long bUseDefaultPosition)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_I4 ;
		InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, x, y, bUseDefaultPosition);
	}

	// IDaServer properties
public:

};
