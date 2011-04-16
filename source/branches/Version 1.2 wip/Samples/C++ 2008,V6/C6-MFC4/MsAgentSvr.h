// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// IAgent wrapper class

class IAgent : public COleDispatchDriver
{
public:
	IAgent() {}		// Calls COleDispatchDriver default constructor
	IAgent(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgent(const IAgent& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void Load(const VARIANT& vLoadKey, long* pdwCharID, long* pdwReqID);
	void Unload(long dwCharID);
	void Register(LPUNKNOWN punkNotifySink, long* pdwSinkID);
	void Unregister(long dwSinkID);
	void GetCharacter(long dwCharID, LPDISPATCH* ppunkCharacter);
	void GetSuspended(long* pbSuspended);
};
/////////////////////////////////////////////////////////////////////////////
// IAgentUserInput wrapper class

class IAgentUserInput : public COleDispatchDriver
{
public:
	IAgentUserInput() {}		// Calls COleDispatchDriver default constructor
	IAgentUserInput(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentUserInput(const IAgentUserInput& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
// IAgentCommand wrapper class

class IAgentCommand : public COleDispatchDriver
{
public:
	IAgentCommand() {}		// Calls COleDispatchDriver default constructor
	IAgentCommand(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentCommand(const IAgentCommand& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
};
/////////////////////////////////////////////////////////////////////////////
// IAgentCommandEx wrapper class

class IAgentCommandEx : public COleDispatchDriver
{
public:
	IAgentCommandEx() {}		// Calls COleDispatchDriver default constructor
	IAgentCommandEx(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentCommandEx(const IAgentCommandEx& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
	void SetHelpContextID(long ulID);
	void GetHelpContextID(long* pulID);
	void SetVoiceCaption(LPCTSTR bszVoiceCaption);
	void GetVoiceCaption(BSTR* pbszVoiceCaption);
};
/////////////////////////////////////////////////////////////////////////////
// IAgentCommands wrapper class

class IAgentCommands : public COleDispatchDriver
{
public:
	IAgentCommands() {}		// Calls COleDispatchDriver default constructor
	IAgentCommands(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentCommands(const IAgentCommands& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
};
/////////////////////////////////////////////////////////////////////////////
// IAgentCommandsEx wrapper class

class IAgentCommandsEx : public COleDispatchDriver
{
public:
	IAgentCommandsEx() {}		// Calls COleDispatchDriver default constructor
	IAgentCommandsEx(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentCommandsEx(const IAgentCommandsEx& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
	void SetHelpContextID(long ulHelpID);
	void GetHelpContextID(long* pulHelpID);
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
// IAgentSpeechInputProperties wrapper class

class IAgentSpeechInputProperties : public COleDispatchDriver
{
public:
	IAgentSpeechInputProperties() {}		// Calls COleDispatchDriver default constructor
	IAgentSpeechInputProperties(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentSpeechInputProperties(const IAgentSpeechInputProperties& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void GetInstalled(long* pbInstalled);
	void GetEnabled(long* pbEnabled);
	void GetHotKey(BSTR* pbszHotCharKey);
	void GetLCID(unsigned long* plcidCurrent);
	void GetEngine(BSTR* pbszEngine);
	void SetEngine(LPCTSTR bszEngine);
	void GetListeningTip(long* pbListeningTip);
};
/////////////////////////////////////////////////////////////////////////////
// IAgentAudioOutputProperties wrapper class

class IAgentAudioOutputProperties : public COleDispatchDriver
{
public:
	IAgentAudioOutputProperties() {}		// Calls COleDispatchDriver default constructor
	IAgentAudioOutputProperties(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentAudioOutputProperties(const IAgentAudioOutputProperties& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void GetEnabled(long* pbEnabled);
	void GetUsingSoundEffects(long* pbUsingSoundEffects);
};
/////////////////////////////////////////////////////////////////////////////
// IAgentAudioOutputPropertiesEx wrapper class

class IAgentAudioOutputPropertiesEx : public COleDispatchDriver
{
public:
	IAgentAudioOutputPropertiesEx() {}		// Calls COleDispatchDriver default constructor
	IAgentAudioOutputPropertiesEx(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentAudioOutputPropertiesEx(const IAgentAudioOutputPropertiesEx& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void GetEnabled(long* pbEnabled);
	void GetUsingSoundEffects(long* pbUsingSoundEffects);
	void GetStatus(long* plStatus);
};
/////////////////////////////////////////////////////////////////////////////
// IAgentPropertySheet wrapper class

class IAgentPropertySheet : public COleDispatchDriver
{
public:
	IAgentPropertySheet() {}		// Calls COleDispatchDriver default constructor
	IAgentPropertySheet(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentPropertySheet(const IAgentPropertySheet& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
// IAgentBalloon wrapper class

class IAgentBalloon : public COleDispatchDriver
{
public:
	IAgentBalloon() {}		// Calls COleDispatchDriver default constructor
	IAgentBalloon(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentBalloon(const IAgentBalloon& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
};
/////////////////////////////////////////////////////////////////////////////
// IAgentBalloonEx wrapper class

class IAgentBalloonEx : public COleDispatchDriver
{
public:
	IAgentBalloonEx() {}		// Calls COleDispatchDriver default constructor
	IAgentBalloonEx(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentBalloonEx(const IAgentBalloonEx& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
// IAgentCharacter wrapper class

class IAgentCharacter : public COleDispatchDriver
{
public:
	IAgentCharacter() {}		// Calls COleDispatchDriver default constructor
	IAgentCharacter(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentCharacter(const IAgentCharacter& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
};
/////////////////////////////////////////////////////////////////////////////
// IAgentCharacterEx wrapper class

class IAgentCharacterEx : public COleDispatchDriver
{
public:
	IAgentCharacterEx() {}		// Calls COleDispatchDriver default constructor
	IAgentCharacterEx(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentCharacterEx(const IAgentCharacterEx& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
	void GetHelpFileName(BSTR* pbszName);
	void SetHelpFileName(LPCTSTR bszName);
	void SetHelpModeOn(long bHelpModeOn);
	void GetHelpModeOn(long* pbHelpModeOn);
	void SetHelpContextID(long ulID);
	void GetHelpContextID(long* pulID);
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
// IAgentEx wrapper class

class IAgentEx : public COleDispatchDriver
{
public:
	IAgentEx() {}		// Calls COleDispatchDriver default constructor
	IAgentEx(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentEx(const IAgentEx& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void Load(const VARIANT& vLoadKey, long* pdwCharID, long* pdwReqID);
	void Unload(long dwCharID);
	void Register(LPUNKNOWN punkNotifySink, long* pdwSinkID);
	void Unregister(long dwSinkID);
	void GetCharacter(long dwCharID, LPDISPATCH* ppunkCharacter);
	void GetSuspended(long* pbSuspended);
	void GetCharacterEx(long dwCharID, LPDISPATCH* ppCharacterEx);
	void GetVersion(short* psMajor, short* psMinor);
	void ShowDefaultCharacterProperties(short x, short y, long bUseDefaultPosition);
};
/////////////////////////////////////////////////////////////////////////////
// IAgentNotifySink wrapper class

class IAgentNotifySink : public COleDispatchDriver
{
public:
	IAgentNotifySink() {}		// Calls COleDispatchDriver default constructor
	IAgentNotifySink(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentNotifySink(const IAgentNotifySink& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void Command(long dwCommandID, LPUNKNOWN punkUserInput);
	void ActivateInputState(long dwCharID, long bActivated);
	void Restart();
	void Shutdown();
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
// IAgentNotifySinkEx wrapper class

class IAgentNotifySinkEx : public COleDispatchDriver
{
public:
	IAgentNotifySinkEx() {}		// Calls COleDispatchDriver default constructor
	IAgentNotifySinkEx(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentNotifySinkEx(const IAgentNotifySinkEx& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void Command(long dwCommandID, LPUNKNOWN punkUserInput);
	void ActivateInputState(long dwCharID, long bActivated);
	void Restart();
	void Shutdown();
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
	void HelpComplete(long dwCharID, long dwCommandID, long dwCause);
	void ListeningState(long dwCharID, long bListening, long dwCause);
	void DefaultCharacterChange(LPCTSTR bszGUID);
	void AgentPropertyChange();
	void ActiveClientChange(long dwCharID, long lStatus);
};
/////////////////////////////////////////////////////////////////////////////
// IAgentCommandWindow wrapper class

class IAgentCommandWindow : public COleDispatchDriver
{
public:
	IAgentCommandWindow() {}		// Calls COleDispatchDriver default constructor
	IAgentCommandWindow(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IAgentCommandWindow(const IAgentCommandWindow& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void SetVisible(long bVisible);
	void GetVisible(long* pbVisible);
	void GetPosition(long* plLeft, long* plTop);
	void GetSize(long* plWidth, long* plHeight);
};
