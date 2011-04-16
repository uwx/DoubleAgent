// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CDaCtlCommandsWindow wrapper class

class CDaCtlCommandsWindow : public COleDispatchDriver
{
public:
	CDaCtlCommandsWindow(){} // Calls COleDispatchDriver default constructor
	CDaCtlCommandsWindow(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDaCtlCommandsWindow(const CDaCtlCommandsWindow& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDaCtlCommandsWindow methods
public:
	BOOL get_Visible()
	{
		BOOL result;
		InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Visible(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	short get_Left()
	{
		short result;
		InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	short get_Top()
	{
		short result;
		InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	short get_Height()
	{
		short result;
		InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	short get_Width()
	{
		short result;
		InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}

	// IDaCtlCommandsWindow properties
public:

};
