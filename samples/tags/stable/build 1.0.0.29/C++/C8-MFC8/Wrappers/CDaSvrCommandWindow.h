// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CDaSvrCommandWindow wrapper class

class CDaSvrCommandWindow : public COleDispatchDriver
{
public:
	CDaSvrCommandWindow(){} // Calls COleDispatchDriver default constructor
	CDaSvrCommandWindow(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDaSvrCommandWindow(const CDaSvrCommandWindow& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDaSvrCommandWindow methods
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

	// IDaSvrCommandWindow properties
public:

};
