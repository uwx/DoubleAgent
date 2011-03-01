// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CDaCtlTTSEngines wrapper class

class CDaCtlTTSEngines : public COleDispatchDriver
{
public:
	CDaCtlTTSEngines(){} // Calls COleDispatchDriver default constructor
	CDaCtlTTSEngines(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDaCtlTTSEngines(const CDaCtlTTSEngines& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDaCtlTTSEngines methods
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

	// IDaCtlTTSEngines properties
public:

};
