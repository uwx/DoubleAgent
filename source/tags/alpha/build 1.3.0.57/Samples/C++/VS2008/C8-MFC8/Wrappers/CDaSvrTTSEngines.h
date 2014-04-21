// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CDaSvrTTSEngines wrapper class

class CDaSvrTTSEngines : public COleDispatchDriver
{
public:
	CDaSvrTTSEngines(){} // Calls COleDispatchDriver default constructor
	CDaSvrTTSEngines(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDaSvrTTSEngines(const CDaSvrTTSEngines& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDaSvrTTSEngines methods
public:
	LPDISPATCH get_Item(long Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Index);
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

	// IDaSvrTTSEngines properties
public:

};
