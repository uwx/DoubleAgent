// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CDaSvrBalloon wrapper class

class CDaSvrBalloon : public COleDispatchDriver
{
public:
	CDaSvrBalloon(){} // Calls COleDispatchDriver default constructor
	CDaSvrBalloon(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDaSvrBalloon(const CDaSvrBalloon& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDaSvrBalloon methods
public:
	void GetEnabled(long * pbEnabled)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbEnabled);
	}
	void GetNumLines(long * plLines)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, plLines);
	}
	void GetNumCharsPerLine(long * plCharsPerLine)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, plCharsPerLine);
	}
	void GetFontName(BSTR * pbszFontName)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbszFontName);
	}
	void GetFontSize(long * plFontSize)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms, plFontSize);
	}
	void GetFontBold(long * pbFontBold)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbFontBold);
	}
	void GetFontItalic(long * pbFontItalic)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020006, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbFontItalic);
	}
	void GetFontStrikethru(long * pbFontStrikethru)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020007, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbFontStrikethru);
	}
	void GetFontUnderline(long * pbFontUnderline)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020008, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbFontUnderline);
	}
	void GetForeColor(long * plFGColor)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60020009, DISPATCH_METHOD, VT_EMPTY, NULL, parms, plFGColor);
	}
	void GetBackColor(long * plBGColor)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, plBGColor);
	}
	void GetBorderColor(long * plBorderColor)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, plBorderColor);
	}
	void SetVisible(long bVisible)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6002000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bVisible);
	}
	void GetVisible(long * pbVisible)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x6002000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pbVisible);
	}
	void SetFontName(LPCTSTR bszFontName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6002000e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bszFontName);
	}
	void SetFontSize(long lFontSize)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6002000f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lFontSize);
	}
	void SetFontCharSet(short sFontCharSet)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x60020010, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sFontCharSet);
	}
	void GetFontCharSet(short * psFontCharSet)
	{
		static BYTE parms[] = VTS_PI2 ;
		InvokeHelper(0x60020011, DISPATCH_METHOD, VT_EMPTY, NULL, parms, psFontCharSet);
	}
	void SetStyle(long lStyle)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lStyle);
	}
	void GetStyle(long * plStyle)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, plStyle);
	}
	void SetNumLines(long lLines)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lLines);
	}
	void SetNumCharsPerLine(long lCharsPerLine)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030003, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lCharsPerLine);
	}

	// IDaSvrBalloon properties
public:

};
