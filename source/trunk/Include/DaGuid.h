/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#ifndef _DAGUID_H
#define _DAGUID_H
/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{1147E500-A208-11DE-ABF2-002421116FB2}")) DaServer;
class __declspec(uuid("{1147E518-A208-11DE-ABF2-002421116FB2}")) DaServerHandler;
class __declspec(uuid("{1147E530-A208-11DE-ABF2-002421116FB2}")) DaControl;
class __declspec(uuid("{1147E563-A208-11DE-ABF2-002421116FB2}")) CDirectShowSource;
class __declspec(uuid("{1147E561-A208-11DE-ABF2-002421116FB2}")) CAgentPreviewWnd;
class __declspec(uuid("{1147E564-A208-11DE-ABF2-002421116FB2}")) CDaShellApp;
class __declspec(uuid("{1147E565-A208-11DE-ABF2-002421116FB2}")) CDaCharacterProps;
class __declspec(uuid("{1147E566-A208-11DE-ABF2-002421116FB2}")) CDaElevatedSettings;

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{D45FD2FC-5C6E-11D1-9EC1-00C04FD7081F}")) AgentServer;
class __declspec(uuid("{FD0B6205-B0F4-4596-B028-5CDB35D288C1}")) AgentServer64;
class __declspec(uuid("{A7B93C73-7B81-11D0-AC5F-00C04FD97575}")) AgentServerTypeLib;
interface __declspec(uuid("{A7B93C91-7B81-11D0-AC5F-00C04FD97575}")) IAgent;
interface __declspec(uuid("{48D12BA0-5B77-11D1-9EC1-00C04FD7081F}")) IAgentEx;
interface __declspec(uuid("{00D18159-8466-11D0-AC63-00C04FD97575}")) IAgentNotifySink;
interface __declspec(uuid("{08C75162-3C9C-11D1-91FE-00C04FD701A5}")) IAgentNotifySinkEx;
interface __declspec(uuid("{A7B93C8F-7B81-11D0-AC5F-00C04FD97575}")) IAgentCharacter;
interface __declspec(uuid("{98BBE491-2EED-11D1-ACAC-00C04FD97575}")) IAgentCharacterEx;
interface __declspec(uuid("{A7B93C85-7B81-11D0-AC5F-00C04FD97575}")) IAgentCommands;
interface __declspec(uuid("{6BA90C00-3910-11D1-ACB3-00C04FD97575}")) IAgentCommandsEx;
interface __declspec(uuid("{A7B93C83-7B81-11D0-AC5F-00C04FD97575}")) IAgentCommand;
interface __declspec(uuid("{B0913412-3B44-11D1-ACBA-00C04FD97575}")) IAgentCommandEx;
interface __declspec(uuid("{6D0ECB23-9968-11D0-AC6E-00C04FD97575}")) IAgentCommandWindow;
interface __declspec(uuid("{A7B93C80-7B81-11D0-AC5F-00C04FD97575}")) IAgentUserInput;
interface __declspec(uuid("{A7B93C8D-7B81-11D0-AC5F-00C04FD97575}")) IAgentBalloon;
interface __declspec(uuid("{D7A6D440-8872-11D1-9EC6-00C04FD7081F}")) IAgentBalloonEx;
interface __declspec(uuid("{A7B93C8B-7B81-11D0-AC5F-00C04FD97575}")) IAgentPropertySheet;
interface __declspec(uuid("{A7B93C89-7B81-11D0-AC5F-00C04FD97575}")) IAgentAudioOutputProperties;
interface __declspec(uuid("{A7B93CA0-7B81-11D0-AC5F-00C04FD97575}")) IAgentAudioOutputPropertiesEx;
interface __declspec(uuid("{A7B93C87-7B81-11D0-AC5F-00C04FD97575}")) IAgentSpeechInputProperties;

class __declspec(uuid("{143A62C8-C33B-11D1-84FE-00C04FA34A14}")) AgentCharacterProps;

/////////////////////////////////////////////////////////////////////////////

class __declspec(uuid("{D45FD31B-5C6E-11D1-9EC1-00C04FD7081F}")) AgentControl;
class __declspec(uuid("{F5BE8BC2-7DE6-11D0-91FE-00C04FD701A5}")) AgentControlTypeLib;
interface __declspec(uuid("{F5BE8BD1-7DE6-11D0-91FE-00C04FD701A5}")) IAgentCtl;
interface __declspec(uuid("{8563FF20-8ECC-11D1-B9B4-00C04FD97575}")) IAgentCtlEx;
interface __declspec(uuid("{F5BE8BE8-7DE6-11D0-91FE-00C04FD701A5}")) IAgentCtlCharacters;
interface __declspec(uuid("{F5BE8BD9-7DE6-11D0-91FE-00C04FD701A5}")) IAgentCtlCharacter;
interface __declspec(uuid("{DE8EF600-2F82-11D1-ACAC-00C04FD97575}")) IAgentCtlCharacterEx;
interface __declspec(uuid("{F5BE8BE1-7DE6-11D0-91FE-00C04FD701A5}")) IAgentCtlCommands;
interface __declspec(uuid("{6BA90C01-3910-11d1-ACB3-00C04FD97575}")) IAgentCtlCommandsEx;
interface __declspec(uuid("{F5BE8BE3-7DE6-11D0-91FE-00C04FD701A5}")) IAgentCtlCommand;
interface __declspec(uuid("{B0913410-3B44-11d1-ACBA-00C04FD97575}")) IAgentCtlCommandEx;
interface __declspec(uuid("{8B77181C-D3EF-11d1-8500-00C04FA34A14}")) IAgentCtlAnimationNames;
interface __declspec(uuid("{F5BE8BD3-7DE6-11D0-91FE-00C04FD701A5}")) IAgentCtlBalloon;
interface __declspec(uuid("{822DB1C0-8879-11d1-9EC6-00C04FD7081F}")) IAgentCtlBalloonEx;
interface __declspec(uuid("{1DAB85C3-803A-11D0-AC63-00C04FD97575}")) IAgentCtlRequest;
interface __declspec(uuid("{C4ABF875-8100-11D0-AC63-00C04FD97575}")) IAgentCtlUserInput;
interface __declspec(uuid("{F5BE8BDB-7DE6-11D0-91FE-00C04FD701A5}")) IAgentCtlAudioObject;
interface __declspec(uuid("{F5BE8BF0-7DE6-11D0-91FE-00C04FD701A5}")) IAgentCtlAudioObjectEx;
interface __declspec(uuid("{F5BE8BDD-7DE6-11D0-91FE-00C04FD701A5}")) IAgentCtlSpeechInput;
interface __declspec(uuid("{F5BE8BDF-7DE6-11D0-91FE-00C04FD701A5}")) IAgentCtlPropertySheet;
interface __declspec(uuid("{6D0ECB27-9968-11D0-AC6E-00C04FD97575}")) IAgentCtlCommandsWindow;
interface __declspec(uuid("{F5BE8BD4-7DE6-11D0-91FE-00C04FD701A5}")) _AgentEvents;

/////////////////////////////////////////////////////////////////////////////
//	Unused UUIDs
/////////////////////////////////////////////////////////////////////////////

//Server
//1147E572-A208-11DE-ABF2-002421116FB2
//1147E573-A208-11DE-ABF2-002421116FB2
//1147E574-A208-11DE-ABF2-002421116FB2
//1147E575-A208-11DE-ABF2-002421116FB2
//1147E576-A208-11DE-ABF2-002421116FB2
//1147E577-A208-11DE-ABF2-002421116FB2
//1147E578-A208-11DE-ABF2-002421116FB2
//1147E579-A208-11DE-ABF2-002421116FB2
//1147E57A-A208-11DE-ABF2-002421116FB2
//1147E57B-A208-11DE-ABF2-002421116FB2
//1147E57C-A208-11DE-ABF2-002421116FB2
//1147E57D-A208-11DE-ABF2-002421116FB2
//1147E57E-A208-11DE-ABF2-002421116FB2
//1147E57F-A208-11DE-ABF2-002421116FB2

//Control
//1147E583-A208-11DE-ABF2-002421116FB2
//1147E584-A208-11DE-ABF2-002421116FB2
//1147E585-A208-11DE-ABF2-002421116FB2
//1147E586-A208-11DE-ABF2-002421116FB2
//1147E587-A208-11DE-ABF2-002421116FB2
//1147E588-A208-11DE-ABF2-002421116FB2
//1147E589-A208-11DE-ABF2-002421116FB2
//1147E58A-A208-11DE-ABF2-002421116FB2
//1147E58B-A208-11DE-ABF2-002421116FB2
//1147E58C-A208-11DE-ABF2-002421116FB2
//1147E58D-A208-11DE-ABF2-002421116FB2
//1147E58E-A208-11DE-ABF2-002421116FB2
//1147E58F-A208-11DE-ABF2-002421116FB2

//Core and Shell
//1147E596-A208-11DE-ABF2-002421116FB2
//1147E597-A208-11DE-ABF2-002421116FB2
//1147E598-A208-11DE-ABF2-002421116FB2
//1147E599-A208-11DE-ABF2-002421116FB2
//1147E59A-A208-11DE-ABF2-002421116FB2
//1147E59B-A208-11DE-ABF2-002421116FB2
//1147E59C-A208-11DE-ABF2-002421116FB2
//1147E59D-A208-11DE-ABF2-002421116FB2
//1147E59E-A208-11DE-ABF2-002421116FB2
//1147E59F-A208-11DE-ABF2-002421116FB2
//1147E5A0-A208-11DE-ABF2-002421116FB2
//1147E5A1-A208-11DE-ABF2-002421116FB2
//1147E5A2-A208-11DE-ABF2-002421116FB2
//1147E5A3-A208-11DE-ABF2-002421116FB2
//1147E5A4-A208-11DE-ABF2-002421116FB2
//1147E5A5-A208-11DE-ABF2-002421116FB2
//1147E5A6-A208-11DE-ABF2-002421116FB2
//1147E5A7-A208-11DE-ABF2-002421116FB2
//1147E5A8-A208-11DE-ABF2-002421116FB2
//1147E5A9-A208-11DE-ABF2-002421116FB2
//1147E5AA-A208-11DE-ABF2-002421116FB2
//1147E5AB-A208-11DE-ABF2-002421116FB2
//1147E5AC-A208-11DE-ABF2-002421116FB2
//1147E5AD-A208-11DE-ABF2-002421116FB2
//1147E5AE-A208-11DE-ABF2-002421116FB2
//1147E5AF-A208-11DE-ABF2-002421116FB2
//1147E5B0-A208-11DE-ABF2-002421116FB2
//1147E5B1-A208-11DE-ABF2-002421116FB2
//1147E5B2-A208-11DE-ABF2-002421116FB2
//1147E5B3-A208-11DE-ABF2-002421116FB2
//1147E5B4-A208-11DE-ABF2-002421116FB2
//1147E5B5-A208-11DE-ABF2-002421116FB2
//1147E5B6-A208-11DE-ABF2-002421116FB2
//1147E5B7-A208-11DE-ABF2-002421116FB2
//1147E5B8-A208-11DE-ABF2-002421116FB2
//1147E5B9-A208-11DE-ABF2-002421116FB2
//1147E5BA-A208-11DE-ABF2-002421116FB2
//1147E5BB-A208-11DE-ABF2-002421116FB2
//1147E5BC-A208-11DE-ABF2-002421116FB2
//1147E5BD-A208-11DE-ABF2-002421116FB2
//1147E5BE-A208-11DE-ABF2-002421116FB2
//1147E5BF-A208-11DE-ABF2-002421116FB2
//Miscellaneous
//1147E5C3-A208-11DE-ABF2-002421116FB2
//1147E5C4-A208-11DE-ABF2-002421116FB2
//1147E5C5-A208-11DE-ABF2-002421116FB2
//1147E5C6-A208-11DE-ABF2-002421116FB2
//1147E5C7-A208-11DE-ABF2-002421116FB2

/////////////////////////////////////////////////////////////////////////////
#endif // _DAGUID_H
