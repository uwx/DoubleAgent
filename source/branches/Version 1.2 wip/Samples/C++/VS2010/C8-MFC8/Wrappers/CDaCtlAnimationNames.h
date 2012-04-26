// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CDaCtlAnimationNames wrapper class

class CDaCtlAnimationNames : public COleDispatchDriver
{
public:
	CDaCtlAnimationNames(){} // Calls COleDispatchDriver default constructor
	CDaCtlAnimationNames(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDaCtlAnimationNames(const CDaCtlAnimationNames& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDaCtlAnimationNames methods
public:
	LPUNKNOWN get__NewEnum()
	{
		LPUNKNOWN result;
		InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}
	CString get_Item(VARIANT& Index)
	{
		CString result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, &Index);
		return result;
	}
	long get_Count()
	{
		long result;
		InvokeHelper(0xfffffff8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}

	// IDaCtlAnimationNames properties
public:

};
