// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CDaSvrCommands wrapper class

class CDaSvrCommands : public COleDispatchDriver
{
public:
	CDaSvrCommands(){} // Calls COleDispatchDriver default constructor
	CDaSvrCommands(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDaSvrCommands(const CDaSvrCommands& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDaSvrCommands2 methods
public:
	void GetCommand(long CommandID, LPUNKNOWN * Command)
	{
		static BYTE parms[] = VTS_I4 VTS_PUNKNOWN ;
		InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, CommandID, Command);
	}
	void GetCount(long * Count)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Count);
	}
	void SetCaption(LPCTSTR Caption)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Caption);
	}
	void GetCaption(BSTR * Caption)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Caption);
	}
	void SetVoice(LPCTSTR Voice)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Voice);
	}
	void GetVoice(BSTR * Voice)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Voice);
	}
	void SetVisible(long Visible)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Visible);
	}
	void GetVisible(long * Visible)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020007, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Visible);
	}
	void Add(LPCTSTR Caption, LPCTSTR VoiceGrammar, long Enabled, long Visible, long * CommandID)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x60020008, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Caption, VoiceGrammar, Enabled, Visible, CommandID);
	}
	void Insert(LPCTSTR Caption, LPCTSTR VoiceGrammar, long Enabled, long Visible, long RefCommandID, long Before, long * CommandID)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Caption, VoiceGrammar, Enabled, Visible, RefCommandID, Before, CommandID);
	}
	void Remove(long CommandID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, CommandID);
	}
	void RemoveAll()
	{
		InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void GetCommandEx(long CommandID, LPDISPATCH * Command)
	{
		static BYTE parms[] = VTS_I4 VTS_PDISPATCH ;
		InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, CommandID, Command);
	}
	void SetDefaultID(long CommandID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, CommandID);
	}
	void GetDefaultID(long * CommandID)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, CommandID);
	}
	void SetHelpContextID(long HelpContextID)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030003, DISPATCH_METHOD, VT_EMPTY, NULL, parms, HelpContextID);
	}
	void GetHelpContextID(long * pulHelpID)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60030004, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pulHelpID);
	}
	void SetFontName(LPCTSTR FontName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60030005, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FontName);
	}
	void GetFontName(BSTR * FontName)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60030006, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FontName);
	}
	void SetFontSize(long FontSize)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030007, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FontSize);
	}
	void GetFontSize(long * FontSize)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60030008, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FontSize);
	}
	void SetVoiceCaption(LPCTSTR VoiceCaption)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60030009, DISPATCH_METHOD, VT_EMPTY, NULL, parms, VoiceCaption);
	}
	void GetVoiceCaption(BSTR * VoiceCaption)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x6003000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, VoiceCaption);
	}
	void AddEx(LPCTSTR Caption, LPCTSTR VoiceGrammar, LPCTSTR VoiceCaption, long Enabled, long Visible, long HelpContextID, long * CommandID)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x6003000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Caption, VoiceGrammar, VoiceCaption, Enabled, Visible, HelpContextID, CommandID);
	}
	void InsertEx(LPCTSTR Caption, LPCTSTR VoiceGrammar, LPCTSTR VoiceCaption, long Enabled, long Visible, long HelpContextID, long RefCommandID, long Before, long * CommandID)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x6003000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Caption, VoiceGrammar, VoiceCaption, Enabled, Visible, HelpContextID, RefCommandID, Before, CommandID);
	}
	void SetGlobalVoiceCommandsEnabled(long Enabled)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6003000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Enabled);
	}
	void GetGlobalVoiceCommandsEnabled(long * Enabled)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x6003000e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Enabled);
	}
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
	LPDISPATCH get_Command(long CommandID)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60040000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, CommandID);
		return result;
	}
	CString get_Caption()
	{
		CString result;
		InvokeHelper(0x60040001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Caption(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60040001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_VoiceGrammar()
	{
		CString result;
		InvokeHelper(0x60040002, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_VoiceGrammar(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60040002, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_Visible()
	{
		BOOL result;
		InvokeHelper(0x60040003, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Visible(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60040003, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_DefaultCommand()
	{
		long result;
		InvokeHelper(0x60040004, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_DefaultCommand(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60040004, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_FontName()
	{
		CString result;
		InvokeHelper(0x60040005, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_FontName(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60040005, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_FontSize()
	{
		long result;
		InvokeHelper(0x60040006, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_FontSize(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60040006, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_VoiceCaption()
	{
		CString result;
		InvokeHelper(0x60040007, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_VoiceCaption(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60040007, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_GlobalVoiceCommandsEnabled()
	{
		BOOL result;
		InvokeHelper(0x60040008, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_GlobalVoiceCommandsEnabled(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60040008, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPUNKNOWN get__NewEnum()
	{
		LPUNKNOWN result;
		InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}

	// IDaSvrCommands2 properties
public:

};
