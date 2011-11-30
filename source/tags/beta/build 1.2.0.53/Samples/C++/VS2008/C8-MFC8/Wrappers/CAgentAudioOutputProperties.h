// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CAgentAudioOutputProperties wrapper class

class CAgentAudioOutputProperties : public COleDispatchDriver
{
public:
	CAgentAudioOutputProperties(){} // Calls COleDispatchDriver default constructor
	CAgentAudioOutputProperties(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAgentAudioOutputProperties(const CAgentAudioOutputProperties& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IAgentAudioOutputProperties methods
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

	// IAgentAudioOutputProperties properties
public:

};
