// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CDaCtlSREngines wrapper class

class CDaCtlSREngines : public COleDispatchDriver
{
public:
	CDaCtlSREngines(){} // Calls COleDispatchDriver default constructor
	CDaCtlSREngines(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDaCtlSREngines(const CDaCtlSREngines& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDaCtlSREngines methods
public:
	LPDISPATCH get_Item(VARIANT& Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, &Index);
		return result;
	}
	long get_Count()
	{
		long result;
		InvokeHelper(0xfffffff8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPUNKNOWN get__NewEnum()
	{
		LPUNKNOWN result;
		InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}

	// IDaCtlSREngines properties
public:

};
