// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// IDaCtlCharacter2 wrapper class

class IDaCtlCharacter2 : public COleDispatchDriver
{
public:
	IDaCtlCharacter2() {}		// Calls COleDispatchDriver default constructor
	IDaCtlCharacter2(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaCtlCharacter2(const IDaCtlCharacter2& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetBalloon();
	LPDISPATCH GetCommands();
	CString GetName();
	CString GetDescription();
	BOOL GetVisible();
	void SetLeft(short nNewValue);
	short GetLeft();
	void SetTop(short nNewValue);
	short GetTop();
	void SetHeight(short nNewValue);
	short GetHeight();
	void SetWidth(short nNewValue);
	short GetWidth();
	BOOL Activate(const VARIANT& State);
	LPDISPATCH Play(LPCTSTR Animation);
	LPDISPATCH Get(LPCTSTR Type, LPCTSTR Name, const VARIANT& Queue);
	void Stop(const VARIANT& Request);
	LPDISPATCH Wait(LPDISPATCH WaitForRequest);
	LPDISPATCH Interrupt(LPDISPATCH InterruptRequest);
	LPDISPATCH Speak(const VARIANT& Text, const VARIANT& Url);
	LPDISPATCH GestureAt(short X, short y);
	LPDISPATCH MoveTo(short X, short y, const VARIANT& Speed);
	LPDISPATCH Hide(const VARIANT& Fast);
	LPDISPATCH Show(const VARIANT& Fast);
	void StopAll(const VARIANT& Types);
	long GetMoveCause();
	long GetVisibilityCause();
	BOOL GetHasOtherClients();
	void SetName(LPCTSTR lpszNewValue);
	void SetDescription(LPCTSTR lpszNewValue);
	CString GetExtraData();
	BOOL ShowPopupMenu(short X, short y);
	void SetAutoPopupMenu(BOOL bNewValue);
	BOOL GetAutoPopupMenu();
	BOOL Listen(BOOL Listen);
	void SetLanguageID(long nNewValue);
	long GetLanguageID();
	CString GetSRModeID();
	void SetSRModeID(LPCTSTR lpszNewValue);
	CString GetTTSModeID();
	void SetTTSModeID(LPCTSTR lpszNewValue);
	short GetOriginalHeight();
	short GetOriginalWidth();
	LPDISPATCH Think(LPCTSTR Text);
	CString GetVersion();
	long GetStyle();
	void SetStyle(long nNewValue);
	BOOL GetHasIcon();
	void GenerateIcon(long ClipLeft, long ClipTop, long ClipWidth, long ClipHeight);
	BOOL GetIconShown();
	void SetIconShown(BOOL bNewValue);
	BOOL GetIconVisible();
	CString GetIconIdentity();
	void SetIconIdentity(LPCTSTR lpszNewValue);
	CString GetIconTip();
	void SetIconTip(LPCTSTR lpszNewValue);
	LPDISPATCH GetTTSEngine(const VARIANT& GetDefault);
	LPDISPATCH FindTTSEngines(const VARIANT& LanguageID);
	LPDISPATCH GetSREngine(const VARIANT& GetDefault);
	LPDISPATCH FindSREngines(const VARIANT& LanguageID);
	CString GetUniqueID();
	CString GetCharacterID();
	BOOL GetConnected();
	CString GetFileName();
	CString GetFilePath();
	void SetActiveState(long nNewValue);
	long GetActiveState();
	BOOL GetIdleState();
	void SetIdleEnabled(BOOL bNewValue);
	BOOL GetIdleEnabled();
	void SetSoundEffectsEnabled(BOOL bNewValue);
	BOOL GetSoundEffectsEnabled();
	LPDISPATCH Prepare(long ResourceType, LPCTSTR Name, const VARIANT& Queue);
	long GetListeningStatus();
	BOOL GetSmoothed();
	BOOL GetSmoothEdges();
	// method 'GetAnimations' not emitted because of invalid return type or parameter type
	// method 'GetStates' not emitted because of invalid return type or parameter type
};
/////////////////////////////////////////////////////////////////////////////
// IDaCtlBalloon2 wrapper class

class IDaCtlBalloon2 : public COleDispatchDriver
{
public:
	IDaCtlBalloon2() {}		// Calls COleDispatchDriver default constructor
	IDaCtlBalloon2(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaCtlBalloon2(const IDaCtlBalloon2& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL GetEnabled();
	CString GetFontName();
	long GetFontSize();
	void SetVisible(BOOL bNewValue);
	BOOL GetVisible();
	void SetFontName(LPCTSTR lpszNewValue);
	void SetFontSize(long nNewValue);
	void SetFontCharSet(short nNewValue);
	short GetFontCharSet();
	void SetStyle(long nNewValue);
	long GetStyle();
	long GetNumberOfLines();
	void SetNumberOfLines(long nNewValue);
	long GetCharsPerLine();
	void SetCharsPerLine(long nNewValue);
	unsigned long GetTextColor();
	void SetTextColor(unsigned long newValue);
	unsigned long GetBackColor();
	void SetBackColor(unsigned long newValue);
	unsigned long GetBorderColor();
	void SetBorderColor(unsigned long newValue);
	BOOL GetFontBold();
	void SetFontBold(BOOL bNewValue);
	BOOL GetFontItalic();
	void SetFontItalic(BOOL bNewValue);
	BOOL GetFontStrikethru();
	void SetFontStrikethru(BOOL bNewValue);
	BOOL GetFontUnderline();
	void SetFontUnderline(BOOL bNewValue);
	BOOL GetSizeToText();
	void SetSizeToText(BOOL bNewValue);
	BOOL GetAutoHide();
	void SetAutoHide(BOOL bNewValue);
	BOOL GetAutoPace();
	void SetAutoPace(BOOL bNewValue);
	BOOL GetShowPartialLines();
	void SetShowPartialLines(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// IDaCtlCommands2 wrapper class

class IDaCtlCommands2 : public COleDispatchDriver
{
public:
	IDaCtlCommands2() {}		// Calls COleDispatchDriver default constructor
	IDaCtlCommands2(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaCtlCommands2(const IDaCtlCommands2& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetItem(LPCTSTR Name);
	long GetCount();
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	LPDISPATCH Add(LPCTSTR Name, const VARIANT& Caption, const VARIANT& VoiceGrammar, const VARIANT& Enabled, const VARIANT& Visible);
	LPDISPATCH Insert(LPCTSTR Name, LPCTSTR RefName, const VARIANT& Before, const VARIANT& Caption, const VARIANT& VoiceGrammar, const VARIANT& Enabled, const VARIANT& Visible);
	void Remove(LPCTSTR Name);
	void RemoveAll();
	CString GetDefaultCommand();
	void SetDefaultCommand(LPCTSTR lpszNewValue);
	CString GetFontName();
	void SetFontName(LPCTSTR lpszNewValue);
	long GetFontSize();
	void SetFontSize(long nNewValue);
	CString GetVoiceCaption();
	void SetVoiceCaption(LPCTSTR lpszNewValue);
	BOOL GetGlobalVoiceCommandsEnabled();
	void SetGlobalVoiceCommandsEnabled(BOOL bNewValue);
	CString GetVoiceGrammar();
	void SetVoiceGrammar(LPCTSTR lpszNewValue);
	LPDISPATCH GetIndex(long Index);
};
/////////////////////////////////////////////////////////////////////////////
// IDaCtlCommand2 wrapper class

class IDaCtlCommand2 : public COleDispatchDriver
{
public:
	IDaCtlCommand2() {}		// Calls COleDispatchDriver default constructor
	IDaCtlCommand2(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaCtlCommand2(const IDaCtlCommand2& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	CString GetConfidenceText();
	void SetConfidenceText(LPCTSTR lpszNewValue);
	CString GetVoiceCaption();
	void SetVoiceCaption(LPCTSTR lpszNewValue);
	CString GetVoiceGrammar();
	void SetVoiceGrammar(LPCTSTR lpszNewValue);
	long GetConfidenceThreshold();
	void SetConfidenceThreshold(long nNewValue);
	CString GetName();
};
/////////////////////////////////////////////////////////////////////////////
// IDaCtlRequest wrapper class

class IDaCtlRequest : public COleDispatchDriver
{
public:
	IDaCtlRequest() {}		// Calls COleDispatchDriver default constructor
	IDaCtlRequest(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaCtlRequest(const IDaCtlRequest& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetStatus();
	CString GetDescription();
	long GetNumber();
};
/////////////////////////////////////////////////////////////////////////////
// IDaCtlTTSEngine wrapper class

class IDaCtlTTSEngine : public COleDispatchDriver
{
public:
	IDaCtlTTSEngine() {}		// Calls COleDispatchDriver default constructor
	IDaCtlTTSEngine(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaCtlTTSEngine(const IDaCtlTTSEngine& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString GetTTSModeID();
	CString GetDisplayName();
	CString GetManufacturer();
	void GetVersion(short* MajorVersion, short* MinorVersion);
	long GetGender();
	long GetLanguageID();
	CString GetLanguageName(BOOL EnglishName);
};
/////////////////////////////////////////////////////////////////////////////
// IDaCtlTTSEngines wrapper class

class IDaCtlTTSEngines : public COleDispatchDriver
{
public:
	IDaCtlTTSEngines() {}		// Calls COleDispatchDriver default constructor
	IDaCtlTTSEngines(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaCtlTTSEngines(const IDaCtlTTSEngines& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetItem(const VARIANT& Index);
	long GetCount();
};
/////////////////////////////////////////////////////////////////////////////
// IDaCtlSREngine wrapper class

class IDaCtlSREngine : public COleDispatchDriver
{
public:
	IDaCtlSREngine() {}		// Calls COleDispatchDriver default constructor
	IDaCtlSREngine(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaCtlSREngine(const IDaCtlSREngine& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString GetSRModeID();
	CString GetDisplayName();
	CString GetManufacturer();
	void GetVersion(short* MajorVersion, short* MinorVersion);
	long GetLanguageID();
	CString GetLanguageName(BOOL EnglishName);
	// method 'GetLanguageIDs' not emitted because of invalid return type or parameter type
	// method 'GetLanguageNames' not emitted because of invalid return type or parameter type
};
/////////////////////////////////////////////////////////////////////////////
// IDaCtlSREngines wrapper class

class IDaCtlSREngines : public COleDispatchDriver
{
public:
	IDaCtlSREngines() {}		// Calls COleDispatchDriver default constructor
	IDaCtlSREngines(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaCtlSREngines(const IDaCtlSREngines& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetItem(const VARIANT& Index);
	long GetCount();
};
/////////////////////////////////////////////////////////////////////////////
// IDaCtlUserInput wrapper class

class IDaCtlUserInput : public COleDispatchDriver
{
public:
	IDaCtlUserInput() {}		// Calls COleDispatchDriver default constructor
	IDaCtlUserInput(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaCtlUserInput(const IDaCtlUserInput& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	short GetCount();
	CString GetName();
	CString GetCharacterID();
	long GetConfidence();
	CString GetVoice();
	CString GetAlt1Name();
	long GetAlt1Confidence();
	CString GetAlt1Voice();
	CString GetAlt2Name();
	long GetAlt2Confidence();
	CString GetAlt2Voice();
};
/////////////////////////////////////////////////////////////////////////////
// IDaCtlCommandsWindow wrapper class

class IDaCtlCommandsWindow : public COleDispatchDriver
{
public:
	IDaCtlCommandsWindow() {}		// Calls COleDispatchDriver default constructor
	IDaCtlCommandsWindow(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaCtlCommandsWindow(const IDaCtlCommandsWindow& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	short GetLeft();
	short GetTop();
	short GetHeight();
	short GetWidth();
};
/////////////////////////////////////////////////////////////////////////////
// IDaCtlCharacters2 wrapper class

class IDaCtlCharacters2 : public COleDispatchDriver
{
public:
	IDaCtlCharacters2() {}		// Calls COleDispatchDriver default constructor
	IDaCtlCharacters2(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaCtlCharacters2(const IDaCtlCharacters2& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetItem(LPCTSTR CharacterID);
	void Unload(LPCTSTR CharacterID);
	LPDISPATCH Load(LPCTSTR CharacterID, const VARIANT& Provider);
	long GetCount();
	LPDISPATCH GetIndex(long Index);
};
/////////////////////////////////////////////////////////////////////////////
// IDaCtlPropertySheet2 wrapper class

class IDaCtlPropertySheet2 : public COleDispatchDriver
{
public:
	IDaCtlPropertySheet2() {}		// Calls COleDispatchDriver default constructor
	IDaCtlPropertySheet2(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaCtlPropertySheet2(const IDaCtlPropertySheet2& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	short GetHeight();
	short GetWidth();
	void SetVisible(BOOL bNewValue);
	BOOL GetVisible();
	void SetPage(LPCTSTR lpszNewValue);
	CString GetPage();
	void SetLeft(short nNewValue);
	short GetLeft();
	void SetTop(short nNewValue);
	short GetTop();
};
/////////////////////////////////////////////////////////////////////////////
// IDaCtlCharacterFiles wrapper class

class IDaCtlCharacterFiles : public COleDispatchDriver
{
public:
	IDaCtlCharacterFiles() {}		// Calls COleDispatchDriver default constructor
	IDaCtlCharacterFiles(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaCtlCharacterFiles(const IDaCtlCharacterFiles& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	// method 'GetFilePaths' not emitted because of invalid return type or parameter type
	CString GetSearchPath();
	void SetSearchPath(LPCTSTR lpszNewValue);
	CString GetDefaultSearchPath();
	BOOL GetDoubleAgentFiles();
	void SetDoubleAgentFiles(BOOL bNewValue);
	BOOL GetMsAgentFiles();
	void SetMsAgentFiles(BOOL bNewValue);
	BOOL GetMsOfficeFiles();
	void SetMsOfficeFiles(BOOL bNewValue);
	BOOL GetSpeakingCharacters();
	void SetSpeakingCharacters(BOOL bNewValue);
	BOOL GetNonSpeakingCharacters();
	void SetNonSpeakingCharacters(BOOL bNewValue);
	BOOL GetVerifyVersion();
	void SetVerifyVersion(BOOL bNewValue);
	CString GetDefaultFilePath();
	CString GetDefaultFileName();
};
/////////////////////////////////////////////////////////////////////////////
// IDaControl2 wrapper class

class IDaControl2 : public CWnd
{
protected:
	DECLARE_DYNCREATE(IDaControl2)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x1147E530, 0xA208, 0x11DE, { 0xAB, 0xF2, 0x00, 0x24, 0x21, 0x11, 0x6F, 0xB2 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); }

// Attributes
public:

// Operations
public:
	LPDISPATCH GetCharacters();
	LPDISPATCH GetPropertySheet();
	LPDISPATCH GetCommandsWindow();
	BOOL GetConnected();
	void SetConnected(BOOL bNewValue);
	BOOL GetRaiseRequestErrors();
	void SetRaiseRequestErrors(BOOL bNewValue);
	void SetAutoSize(BOOL bNewValue);
	BOOL GetAutoSize();
	void SetBackColor(unsigned long newValue);
	unsigned long GetBackColor();
	void SetBorderColor(unsigned long newValue);
	unsigned long GetBorderColor();
	void SetBorderStyle(long nNewValue);
	long GetBorderStyle();
	void SetBorderWidth(long nNewValue);
	long GetBorderWidth();
	void SetBorderVisible(BOOL bNewValue);
	BOOL GetBorderVisible();
	void SetMousePointer(long nNewValue);
	long GetMousePointer();
	long GetCharacterStyle();
	void SetCharacterStyle(long nNewValue);
	LPDISPATCH GetCharacterFiles();
	LPDISPATCH GetTTSEngines();
	LPDISPATCH FindTTSEngines(const VARIANT& LanguageID, const VARIANT& Gender);
	LPDISPATCH GetCharacterTTSEngine(const VARIANT& Provider);
	LPDISPATCH FindCharacterTTSEngines(const VARIANT& Provider, const VARIANT& LanguageID);
	LPDISPATCH GetSREngines();
	LPDISPATCH FindSREngines(const VARIANT& LanguageID);
	LPDISPATCH GetCharacterSREngine(const VARIANT& Provider);
	LPDISPATCH FindCharacterSREngines(const VARIANT& Provider, const VARIANT& LanguageID);
	LPDISPATCH GetSettings();
	short GetAutoConnect();
	void SetAutoConnect(short nNewValue);
	LPDISPATCH GetControlCharacter();
	void SetControlCharacter(LPDISPATCH newValue);
};
/////////////////////////////////////////////////////////////////////////////
// IDaCtlSettings wrapper class

class IDaCtlSettings : public COleDispatchDriver
{
public:
	IDaCtlSettings() {}		// Calls COleDispatchDriver default constructor
	IDaCtlSettings(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaCtlSettings(const IDaCtlSettings& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL GetSoundEffectsEnabled();
	BOOL GetBalloonEnabled();
	LPDISPATCH GetBalloonFont();
	BOOL GetTTSEnabled();
	short GetTTSSpeed();
	BOOL GetSREnabled();
	CString GetSRHotKey();
	short GetSRHotKeyTime();
	BOOL GetSRListeningTip();
	BOOL GetSRListeningPrompt();
	long GetAudioStatus();
};
