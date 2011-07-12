// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CAgentNotifySink wrapper class

class CAgentNotifySink : public COleDispatchDriver
{
public:
	CAgentNotifySink(){} // Calls COleDispatchDriver default constructor
	CAgentNotifySink(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAgentNotifySink(const CAgentNotifySink& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IAgentNotifySink methods
public:
	void Command(long dwCommandID, LPUNKNOWN punkUserInput)
	{
		static BYTE parms[] = VTS_I4 VTS_UNKNOWN ;
		InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwCommandID, punkUserInput);
	}
	void ActivateInputState(long dwCharID, long bActivated)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwCharID, bActivated);
	}
	void Restart()
	{
		InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Shutdown()
	{
		InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void VisibleState(long dwCharID, long bVisible, long dwCause)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwCharID, bVisible, dwCause);
	}
	void Click(long dwCharID, short fwKeys, long x, long y)
	{
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwCharID, fwKeys, x, y);
	}
	void DblClick(long dwCharID, short fwKeys, long x, long y)
	{
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwCharID, fwKeys, x, y);
	}
	void DragStart(long dwCharID, short fwKeys, long x, long y)
	{
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x60020007, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwCharID, fwKeys, x, y);
	}
	void DragComplete(long dwCharID, short fwKeys, long x, long y)
	{
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x60020008, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwCharID, fwKeys, x, y);
	}
	void RequestStart(long dwRequestID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwRequestID);
	}
	void RequestComplete(long dwRequestID, long hrStatus)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwRequestID, hrStatus);
	}
	void BookMark(long dwBookMarkID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwBookMarkID);
	}
	void Idle(long dwCharID, long bStart)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x6002000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwCharID, bStart);
	}
	void Move(long dwCharID, long x, long y, long dwCause)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x6002000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwCharID, x, y, dwCause);
	}
	void Size(long dwCharID, long lWidth, long lHeight)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x6002000e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwCharID, lWidth, lHeight);
	}
	void BalloonVisibleState(long dwCharID, long bVisible)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x6002000f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwCharID, bVisible);
	}

	// IAgentNotifySink properties
public:

};
