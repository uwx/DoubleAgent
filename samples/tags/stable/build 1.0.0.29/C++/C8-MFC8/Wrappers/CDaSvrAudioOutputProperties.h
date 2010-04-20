// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CDaSvrAudioOutputProperties wrapper class

class CDaSvrAudioOutputProperties : public COleDispatchDriver
{
public:
	CDaSvrAudioOutputProperties(){} // Calls COleDispatchDriver default constructor
	CDaSvrAudioOutputProperties(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDaSvrAudioOutputProperties(const CDaSvrAudioOutputProperties& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDaSvrAudioOutputProperties methods
public:
	void GetEnabled(long * pbEnabled)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbEnabled);
	}
	void GetUsingSoundEffects(long * pbUsingSoundEffects)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbUsingSoundEffects);
	}
	void GetStatus(long * plStatus)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, plStatus);
	}

	// IDaSvrAudioOutputProperties properties
public:

};
