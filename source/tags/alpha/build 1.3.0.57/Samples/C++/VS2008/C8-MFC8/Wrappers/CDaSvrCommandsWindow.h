// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CDaSvrCommandsWindow wrapper class

class CDaSvrCommandsWindow : public COleDispatchDriver
{
public:
	CDaSvrCommandsWindow(){} // Calls COleDispatchDriver default constructor
	CDaSvrCommandsWindow(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDaSvrCommandsWindow(const CDaSvrCommandsWindow& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDaSvrCommandsWindow2 methods
public:
	void SetVisible(long Visible)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Visible);
	}
	void GetVisible(long * Visible)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Visible);
	}
	void GetPosition(long * Left, long * Top)
	{
		static BYTE parms[] = VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Left, Top);
	}
	void GetSize(long * Width, long * Height)
	{
		static BYTE parms[] = VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Width, Height);
	}
	BOOL get_Visible()
	{
		BOOL result;
		InvokeHelper(0x60030001, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Visible(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60030001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	short get_Left()
	{
		short result;
		InvokeHelper(0x60030002, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	short get_Top()
	{
		short result;
		InvokeHelper(0x60030003, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	short get_Height()
	{
		short result;
		InvokeHelper(0x60030004, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	short get_Width()
	{
		short result;
		InvokeHelper(0x60030005, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}

	// IDaSvrCommandsWindow2 properties
public:

};
