// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// IDaServer wrapper class

class IDaServer : public COleDispatchDriver
{
public:
	IDaServer() {}		// Calls COleDispatchDriver default constructor
	IDaServer(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaServer(const IDaServer& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void Load(const VARIANT& vLoadKey, long* pdwCharID, long* pdwReqID);
	void Unload(long dwCharID);
	void Register(LPUNKNOWN punkNotifySink, long* pdwSinkID);
	void Unregister(long dwSinkID);
	void GetCharacter(long dwCharID, LPDISPATCH* ppunkCharacter);
	void GetCharacterEx(long dwCharID, LPDISPATCH* ppCharacterEx);
	void GetVersion(short* psMajor, short* psMinor);
	void ShowDefaultCharacterProperties(short x, short y, long bUseDefaultPosition);
};
/////////////////////////////////////////////////////////////////////////////
// IDaSvrCharacter wrapper class

class IDaSvrCharacter : public COleDispatchDriver
{
public:
	IDaSvrCharacter() {}		// Calls COleDispatchDriver default constructor
	IDaSvrCharacter(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaSvrCharacter(const IDaSvrCharacter& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void GetVisible(long* pbVisible);
	void SetPosition(long lLeft, long lTop);
	void GetPosition(long* plLeft, long* plTop);
	void SetSize(long lWidth, long lHeight);
	void GetSize(long* plWidth, long* plHeight);
	void GetName(BSTR* pbszName);
	void GetDescription(BSTR* pbszDescription);
	void GetTTSSpeed(long* pdwSpeed);
	void GetTTSPitch(short* pwPitch);
	void Activate(short sState);
	void SetIdleOn(long bOn);
	void GetIdleOn(long* pbOn);
	void Prepare(long dwType, LPCTSTR bszName, long bQueue, long* pdwReqID);
	void Play(LPCTSTR bszAnimation, long* pdwReqID);
	void Stop(long dwReqID);
	void StopAll(long lTypes);
	void Wait(long dwReqID, long* pdwReqID);
	void Interrupt(long dwReqID, long* pdwReqID);
	void Show(long bFast, long* pdwReqID);
	void Hide(long bFast, long* pdwReqID);
	void Speak(LPCTSTR bszText, LPCTSTR bszUrl, long* pdwReqID);
	void MoveTo(short x, short y, long lSpeed, long* pdwReqID);
	void GestureAt(short x, short y, long* pdwReqID);
	void GetMoveCause(long* pdwCause);
	void GetVisibilityCause(long* pdwCause);
	void HasOtherClients(long* plNumOtherClients);
	void SetSoundEffectsOn(long bOn);
	void GetSoundEffectsOn(long* pbOn);
	void SetName(LPCTSTR bszName);
	void SetDescription(LPCTSTR bszDescription);
	void GetExtraData(BSTR* pbszExtraData);
	void ShowPopupMenu(short x, short y);
	void SetAutoPopupMenu(long bAutoPopupMenu);
	void GetAutoPopupMenu(long* pbAutoPopupMenu);
	void GetActive(short* psState);
	void Listen(long bListen);
	void SetLanguageID(long langid);
	void GetLanguageID(long* plangid);
	void GetTTSModeID(BSTR* pbszModeID);
	void SetTTSModeID(LPCTSTR bszModeID);
	void GetSRModeID(BSTR* pbszModeID);
	void SetSRModeID(LPCTSTR bszModeID);
	void GetGUID(BSTR* pbszID);
	void GetOriginalSize(long* plWidth, long* plHeight);
	void Think(LPCTSTR bszText, long* pdwReqID);
	void GetVersion(short* psMajor, short* psMinor);
	void GetAnimationNames(LPUNKNOWN* punkEnum);
	void GetSRStatus(long* plStatus);
};
/////////////////////////////////////////////////////////////////////////////
// IDaSvrUserInput wrapper class

class IDaSvrUserInput : public COleDispatchDriver
{
public:
	IDaSvrUserInput() {}		// Calls COleDispatchDriver default constructor
	IDaSvrUserInput(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaSvrUserInput(const IDaSvrUserInput& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void GetCount(long* pdwCount);
	void GetItemID(long dwItemIndex, long* pdwCommandID);
	void GetItemConfidence(long dwItemIndex, long* plConfidence);
	void GetItemText(long dwItemIndex, BSTR* pbszText);
	void GetAllItemData(VARIANT* pdwItemIndices, VARIANT* plConfidences, VARIANT* pbszText);
};
/////////////////////////////////////////////////////////////////////////////
// IDaSvrCommand wrapper class

class IDaSvrCommand : public COleDispatchDriver
{
public:
	IDaSvrCommand() {}		// Calls COleDispatchDriver default constructor
	IDaSvrCommand(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaSvrCommand(const IDaSvrCommand& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void SetCaption(LPCTSTR bszCaption);
	void GetCaption(BSTR* pbszCaption);
	void SetVoice(LPCTSTR bszVoice);
	void GetVoice(BSTR* pbszVoice);
	void SetEnabled(long bEnabled);
	void GetEnabled(long* pbEnabled);
	void SetVisible(long bVisible);
	void GetVisible(long* pbVisible);
	void SetConfidenceThreshold(long lThreshold);
	void GetConfidenceThreshold(long* plThreshold);
	void SetConfidenceText(LPCTSTR bszTipText);
	void GetConfidenceText(BSTR* pbszTipText);
	void GetID(long* pdwID);
	void SetVoiceCaption(LPCTSTR bszVoiceCaption);
	void GetVoiceCaption(BSTR* pbszVoiceCaption);
};
/////////////////////////////////////////////////////////////////////////////
// IDaSvrCommands wrapper class

class IDaSvrCommands : public COleDispatchDriver
{
public:
	IDaSvrCommands() {}		// Calls COleDispatchDriver default constructor
	IDaSvrCommands(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaSvrCommands(const IDaSvrCommands& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void GetCommand(long dwCommandID, LPUNKNOWN* ppunkCommand);
	void GetCount(long* pdwCount);
	void SetCaption(LPCTSTR bszCaption);
	void GetCaption(BSTR* pbszCaption);
	void SetVoice(LPCTSTR bszVoice);
	void GetVoice(BSTR* pbszVoice);
	void SetVisible(long bVisible);
	void GetVisible(long* pbVisible);
	void Add(LPCTSTR bszCaption, LPCTSTR bszVoice, long bEnabled, long bVisible, long* pdwID);
	void Insert(LPCTSTR bszCaption, LPCTSTR bszVoice, long bEnabled, long bVisible, long dwRefID, long bBefore, long* pdwID);
	void Remove(long dwID);
	void RemoveAll();
	void GetCommandEx(long dwCommandID, LPDISPATCH* ppCommandEx);
	void SetDefaultID(long dwID);
	void GetDefaultID(long* pdwID);
	void SetFontName(LPCTSTR bszFontName);
	void GetFontName(BSTR* pbszFontName);
	void SetFontSize(long lFontSize);
	void GetFontSize(long* lFontSize);
	void SetVoiceCaption(LPCTSTR bszVoiceCaption);
	void GetVoiceCaption(BSTR* bszVoiceCaption);
	void AddEx(LPCTSTR bszCaption, LPCTSTR bszVoice, LPCTSTR bszVoiceCaption, long bEnabled, long bVisible, long ulHelpID, long* pdwID);
	void InsertEx(LPCTSTR bszCaption, LPCTSTR bszVoice, LPCTSTR bszVoiceCaption, long bEnabled, long bVisible, long ulHelpID, long dwRefID, long bBefore, long* pdwID);
	void SetGlobalVoiceCommandsEnabled(long bEnable);
	void GetGlobalVoiceCommandsEnabled(long* pbEnabled);
};
/////////////////////////////////////////////////////////////////////////////
// IDaSvrSpeechInputProperties wrapper class

class IDaSvrSpeechInputProperties : public COleDispatchDriver
{
public:
	IDaSvrSpeechInputProperties() {}		// Calls COleDispatchDriver default constructor
	IDaSvrSpeechInputProperties(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaSvrSpeechInputProperties(const IDaSvrSpeechInputProperties& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void GetEnabled(long* pbEnabled);
	void GetHotKey(BSTR* pbszHotCharKey);
	void GetListeningTip(long* pbListeningTip);
};
/////////////////////////////////////////////////////////////////////////////
// IDaSvrAudioOutputProperties wrapper class

class IDaSvrAudioOutputProperties : public COleDispatchDriver
{
public:
	IDaSvrAudioOutputProperties() {}		// Calls COleDispatchDriver default constructor
	IDaSvrAudioOutputProperties(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaSvrAudioOutputProperties(const IDaSvrAudioOutputProperties& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void GetEnabled(long* pbEnabled);
	void GetUsingSoundEffects(long* pbUsingSoundEffects);
};
/////////////////////////////////////////////////////////////////////////////
// IDaSvrPropertySheet wrapper class

class IDaSvrPropertySheet : public COleDispatchDriver
{
public:
	IDaSvrPropertySheet() {}		// Calls COleDispatchDriver default constructor
	IDaSvrPropertySheet(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaSvrPropertySheet(const IDaSvrPropertySheet& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void GetVisible(long* pbVisible);
	void SetVisible(long bVisible);
	void GetPosition(long* plLeft, long* plTop);
	void GetSize(long* plWidth, long* plHeight);
	void GetPage(BSTR* pbszPage);
	void SetPage(LPCTSTR bszPage);
};
/////////////////////////////////////////////////////////////////////////////
// IDaSvrBalloon wrapper class

class IDaSvrBalloon : public COleDispatchDriver
{
public:
	IDaSvrBalloon() {}		// Calls COleDispatchDriver default constructor
	IDaSvrBalloon(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaSvrBalloon(const IDaSvrBalloon& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void GetEnabled(long* pbEnabled);
	void GetNumLines(long* plLines);
	void GetNumCharsPerLine(long* plCharsPerLine);
	void GetFontName(BSTR* pbszFontName);
	void GetFontSize(long* plFontSize);
	void GetFontBold(long* pbFontBold);
	void GetFontItalic(long* pbFontItalic);
	void GetFontStrikethru(long* pbFontStrikethru);
	void GetFontUnderline(long* pbFontUnderline);
	void GetForeColor(long* plFGColor);
	void GetBackColor(long* plBGColor);
	void GetBorderColor(long* plBorderColor);
	void SetVisible(long bVisible);
	void GetVisible(long* pbVisible);
	void SetFontName(LPCTSTR bszFontName);
	void SetFontSize(long lFontSize);
	void SetFontCharSet(short sFontCharSet);
	void GetFontCharSet(short* psFontCharSet);
	void SetStyle(long lStyle);
	void GetStyle(long* plStyle);
	void SetNumLines(long lLines);
	void SetNumCharsPerLine(long lCharsPerLine);
};
/////////////////////////////////////////////////////////////////////////////
// IDaSvrCommandWindow wrapper class

class IDaSvrCommandWindow : public COleDispatchDriver
{
public:
	IDaSvrCommandWindow() {}		// Calls COleDispatchDriver default constructor
	IDaSvrCommandWindow(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaSvrCommandWindow(const IDaSvrCommandWindow& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void SetVisible(long bVisible);
	void GetVisible(long* pbVisible);
	void GetPosition(long* plLeft, long* plTop);
	void GetSize(long* plWidth, long* plHeight);
};
/////////////////////////////////////////////////////////////////////////////
// IDaAgentNotifySink15 wrapper class

class IDaAgentNotifySink15 : public COleDispatchDriver
{
public:
	IDaAgentNotifySink15() {}		// Calls COleDispatchDriver default constructor
	IDaAgentNotifySink15(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaAgentNotifySink15(const IDaAgentNotifySink15& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void Command(long dwCommandID, LPUNKNOWN punkUserInput);
	void ActivateInputState(long dwCharID, long bActivated);
	void VisibleState(long dwCharID, long bVisible, long dwCause);
	void Click(long dwCharID, short fwKeys, long x, long y);
	void DblClick(long dwCharID, short fwKeys, long x, long y);
	void DragStart(long dwCharID, short fwKeys, long x, long y);
	void DragComplete(long dwCharID, short fwKeys, long x, long y);
	void RequestStart(long dwRequestID);
	void RequestComplete(long dwRequestID, long hrStatus);
	void BookMark(long dwBookMarkID);
	void Idle(long dwCharID, long bStart);
	void Move(long dwCharID, long x, long y, long dwCause);
	void Size(long dwCharID, long lWidth, long lHeight);
	void BalloonVisibleState(long dwCharID, long bVisible);
};
/////////////////////////////////////////////////////////////////////////////
// IDaNotifySink wrapper class

class IDaNotifySink : public COleDispatchDriver
{
public:
	IDaNotifySink() {}		// Calls COleDispatchDriver default constructor
	IDaNotifySink(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDaNotifySink(const IDaNotifySink& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void Command(long dwCommandID, LPUNKNOWN punkUserInput);
	void ActivateInputState(long dwCharID, long bActivated);
	void VisibleState(long dwCharID, long bVisible, long dwCause);
	void Click(long dwCharID, short fwKeys, long x, long y);
	void DblClick(long dwCharID, short fwKeys, long x, long y);
	void DragStart(long dwCharID, short fwKeys, long x, long y);
	void DragComplete(long dwCharID, short fwKeys, long x, long y);
	void RequestStart(long dwRequestID);
	void RequestComplete(long dwRequestID, long hrStatus);
	void BookMark(long dwBookMarkID);
	void Idle(long dwCharID, long bStart);
	void Move(long dwCharID, long x, long y, long dwCause);
	void Size(long dwCharID, long lWidth, long lHeight);
	void BalloonVisibleState(long dwCharID, long bVisible);
	void ListeningState(long dwCharID, long bListening, long dwCause);
	void DefaultCharacterChange(LPCTSTR bszGUID);
	void AgentPropertyChange();
	void ActiveClientChange(long dwCharID, long lStatus);
};
