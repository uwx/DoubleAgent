// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CAgentCommandWindow wrapper class

class CAgentCommandWindow : public COleDispatchDriver
{
public:
	CAgentCommandWindow(){} // Calls COleDispatchDriver default constructor
	CAgentCommandWindow(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAgentCommandWindow(const CAgentCommandWindow& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IAgentCommandWindow methods
public:
	void SetVisible(long bVisible)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bVisible);
	}
	void GetVisible(long * pbVisible)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbVisible);
	}
	void GetPosition(long * plLeft, long * plTop)
	{
		static BYTE parms[] = VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, plLeft, plTop);
	}
	void GetSize(long * plWidth, long * plHeight)
	{
		static BYTE parms[] = VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms, plWidth, plHeight);
	}

	// IAgentCommandWindow properties
public:

};
