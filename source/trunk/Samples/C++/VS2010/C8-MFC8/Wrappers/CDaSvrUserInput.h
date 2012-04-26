// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CDaSvrUserInput wrapper class

class CDaSvrUserInput : public COleDispatchDriver
{
public:
	CDaSvrUserInput(){} // Calls COleDispatchDriver default constructor
	CDaSvrUserInput(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDaSvrUserInput(const CDaSvrUserInput& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDaSvrUserInput2 methods
public:
	void GetCount(long * Count)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Count);
	}
	void GetItemID(long ItemIndex, long * CommandID)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 ;
		InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ItemIndex, CommandID);
	}
	void GetItemConfidence(long ItemIndex, long * Confidence)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 ;
		InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ItemIndex, Confidence);
	}
	void GetItemText(long ItemIndex, BSTR * Text)
	{
		static BYTE parms[] = VTS_I4 VTS_PBSTR ;
		InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ItemIndex, Text);
	}
	void GetAllItemData(VARIANT * ItemIndices, VARIANT * ItemConfidences, VARIANT * ItemText)
	{
		static BYTE parms[] = VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT ;
		InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms, ItemIndices, ItemConfidences, ItemText);
	}
	long get_Count()
	{
		long result;
		InvokeHelper(0x60030000, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	long get_ItemCommandID(long ItemIndex)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030001, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, ItemIndex);
		return result;
	}
	long get_ItemConfidence(long ItemIndex)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030002, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, ItemIndex);
		return result;
	}
	CString get_ItemText(long ItemIndex)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030003, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, ItemIndex);
		return result;
	}

	// IDaSvrUserInput2 properties
public:

};
