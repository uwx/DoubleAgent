// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CAgentUserInput wrapper class

class CAgentUserInput : public COleDispatchDriver
{
public:
	CAgentUserInput(){} // Calls COleDispatchDriver default constructor
	CAgentUserInput(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAgentUserInput(const CAgentUserInput& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IAgentUserInput methods
public:
	void GetCount(long * pdwCount)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pdwCount);
	}
	void GetItemID(long dwItemIndex, long * pdwCommandID)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 ;
		InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwItemIndex, pdwCommandID);
	}
	void GetItemConfidence(long dwItemIndex, long * plConfidence)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 ;
		InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwItemIndex, plConfidence);
	}
	void GetItemText(long dwItemIndex, BSTR * pbszText)
	{
		static BYTE parms[] = VTS_I4 VTS_PBSTR ;
		InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms, dwItemIndex, pbszText);
	}
	void GetAllItemData(VARIANT * pdwItemIndices, VARIANT * plConfidences, VARIANT * pbszText)
	{
		static BYTE parms[] = VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT ;
		InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pdwItemIndices, plConfidences, pbszText);
	}

	// IAgentUserInput properties
public:

};
