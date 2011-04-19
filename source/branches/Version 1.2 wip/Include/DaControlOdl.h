

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Tue Apr 19 00:15:35 2011
 */
/* Compiler settings for Control\DaControl.odl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __DaControlOdl_h__
#define __DaControlOdl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDaCtlCharacters_FWD_DEFINED__
#define __IDaCtlCharacters_FWD_DEFINED__
typedef interface IDaCtlCharacters IDaCtlCharacters;
#endif 	/* __IDaCtlCharacters_FWD_DEFINED__ */


#ifndef __IDaCtlCharacter_FWD_DEFINED__
#define __IDaCtlCharacter_FWD_DEFINED__
typedef interface IDaCtlCharacter IDaCtlCharacter;
#endif 	/* __IDaCtlCharacter_FWD_DEFINED__ */


#ifndef __IDaCtlBalloon_FWD_DEFINED__
#define __IDaCtlBalloon_FWD_DEFINED__
typedef interface IDaCtlBalloon IDaCtlBalloon;
#endif 	/* __IDaCtlBalloon_FWD_DEFINED__ */


#ifndef __IDaCtlCommands_FWD_DEFINED__
#define __IDaCtlCommands_FWD_DEFINED__
typedef interface IDaCtlCommands IDaCtlCommands;
#endif 	/* __IDaCtlCommands_FWD_DEFINED__ */


#ifndef __IDaCtlCommand_FWD_DEFINED__
#define __IDaCtlCommand_FWD_DEFINED__
typedef interface IDaCtlCommand IDaCtlCommand;
#endif 	/* __IDaCtlCommand_FWD_DEFINED__ */


#ifndef __IDaCtlRequest_FWD_DEFINED__
#define __IDaCtlRequest_FWD_DEFINED__
typedef interface IDaCtlRequest IDaCtlRequest;
#endif 	/* __IDaCtlRequest_FWD_DEFINED__ */


#ifndef __IDaCtlAudioOutput_FWD_DEFINED__
#define __IDaCtlAudioOutput_FWD_DEFINED__
typedef interface IDaCtlAudioOutput IDaCtlAudioOutput;
#endif 	/* __IDaCtlAudioOutput_FWD_DEFINED__ */


#ifndef __IDaCtlSpeechInput_FWD_DEFINED__
#define __IDaCtlSpeechInput_FWD_DEFINED__
typedef interface IDaCtlSpeechInput IDaCtlSpeechInput;
#endif 	/* __IDaCtlSpeechInput_FWD_DEFINED__ */


#ifndef __IDaCtlPropertySheet_FWD_DEFINED__
#define __IDaCtlPropertySheet_FWD_DEFINED__
typedef interface IDaCtlPropertySheet IDaCtlPropertySheet;
#endif 	/* __IDaCtlPropertySheet_FWD_DEFINED__ */


#ifndef __IDaCtlUserInput_FWD_DEFINED__
#define __IDaCtlUserInput_FWD_DEFINED__
typedef interface IDaCtlUserInput IDaCtlUserInput;
#endif 	/* __IDaCtlUserInput_FWD_DEFINED__ */


#ifndef __IDaCtlCommandsWindow_FWD_DEFINED__
#define __IDaCtlCommandsWindow_FWD_DEFINED__
typedef interface IDaCtlCommandsWindow IDaCtlCommandsWindow;
#endif 	/* __IDaCtlCommandsWindow_FWD_DEFINED__ */


#ifndef __IDaCtlAnimationNames_FWD_DEFINED__
#define __IDaCtlAnimationNames_FWD_DEFINED__
typedef interface IDaCtlAnimationNames IDaCtlAnimationNames;
#endif 	/* __IDaCtlAnimationNames_FWD_DEFINED__ */


#ifndef __IDaControl_FWD_DEFINED__
#define __IDaControl_FWD_DEFINED__
typedef interface IDaControl IDaControl;
#endif 	/* __IDaControl_FWD_DEFINED__ */


#ifndef ___DaCtlEvents_FWD_DEFINED__
#define ___DaCtlEvents_FWD_DEFINED__
typedef interface _DaCtlEvents _DaCtlEvents;
#endif 	/* ___DaCtlEvents_FWD_DEFINED__ */


#ifndef __IDaCtlCharacterFiles_FWD_DEFINED__
#define __IDaCtlCharacterFiles_FWD_DEFINED__
typedef interface IDaCtlCharacterFiles IDaCtlCharacterFiles;
#endif 	/* __IDaCtlCharacterFiles_FWD_DEFINED__ */


#ifndef __IDaCtlFormattedText_FWD_DEFINED__
#define __IDaCtlFormattedText_FWD_DEFINED__
typedef interface IDaCtlFormattedText IDaCtlFormattedText;
#endif 	/* __IDaCtlFormattedText_FWD_DEFINED__ */


#ifndef __IDaCtlTTSEngine_FWD_DEFINED__
#define __IDaCtlTTSEngine_FWD_DEFINED__
typedef interface IDaCtlTTSEngine IDaCtlTTSEngine;
#endif 	/* __IDaCtlTTSEngine_FWD_DEFINED__ */


#ifndef __IDaCtlTTSEngines_FWD_DEFINED__
#define __IDaCtlTTSEngines_FWD_DEFINED__
typedef interface IDaCtlTTSEngines IDaCtlTTSEngines;
#endif 	/* __IDaCtlTTSEngines_FWD_DEFINED__ */


#ifndef __IDaCtlSREngine_FWD_DEFINED__
#define __IDaCtlSREngine_FWD_DEFINED__
typedef interface IDaCtlSREngine IDaCtlSREngine;
#endif 	/* __IDaCtlSREngine_FWD_DEFINED__ */


#ifndef __IDaCtlSREngines_FWD_DEFINED__
#define __IDaCtlSREngines_FWD_DEFINED__
typedef interface IDaCtlSREngines IDaCtlSREngines;
#endif 	/* __IDaCtlSREngines_FWD_DEFINED__ */


#ifndef __IDaControl2_FWD_DEFINED__
#define __IDaControl2_FWD_DEFINED__
typedef interface IDaControl2 IDaControl2;
#endif 	/* __IDaControl2_FWD_DEFINED__ */


#ifndef __IDaCtlCharacter2_FWD_DEFINED__
#define __IDaCtlCharacter2_FWD_DEFINED__
typedef interface IDaCtlCharacter2 IDaCtlCharacter2;
#endif 	/* __IDaCtlCharacter2_FWD_DEFINED__ */


#ifndef __IDaCtlCharacters2_FWD_DEFINED__
#define __IDaCtlCharacters2_FWD_DEFINED__
typedef interface IDaCtlCharacters2 IDaCtlCharacters2;
#endif 	/* __IDaCtlCharacters2_FWD_DEFINED__ */


#ifndef __IDaCtlBalloon2_FWD_DEFINED__
#define __IDaCtlBalloon2_FWD_DEFINED__
typedef interface IDaCtlBalloon2 IDaCtlBalloon2;
#endif 	/* __IDaCtlBalloon2_FWD_DEFINED__ */


#ifndef __IDaCtlSettings_FWD_DEFINED__
#define __IDaCtlSettings_FWD_DEFINED__
typedef interface IDaCtlSettings IDaCtlSettings;
#endif 	/* __IDaCtlSettings_FWD_DEFINED__ */


#ifndef __IDaCtlCommands2_FWD_DEFINED__
#define __IDaCtlCommands2_FWD_DEFINED__
typedef interface IDaCtlCommands2 IDaCtlCommands2;
#endif 	/* __IDaCtlCommands2_FWD_DEFINED__ */


#ifndef __IDaCtlCommand2_FWD_DEFINED__
#define __IDaCtlCommand2_FWD_DEFINED__
typedef interface IDaCtlCommand2 IDaCtlCommand2;
#endif 	/* __IDaCtlCommand2_FWD_DEFINED__ */


#ifndef __IDaCtlPropertySheet2_FWD_DEFINED__
#define __IDaCtlPropertySheet2_FWD_DEFINED__
typedef interface IDaCtlPropertySheet2 IDaCtlPropertySheet2;
#endif 	/* __IDaCtlPropertySheet2_FWD_DEFINED__ */


#ifndef ___DaCtlEvents2_FWD_DEFINED__
#define ___DaCtlEvents2_FWD_DEFINED__
typedef interface _DaCtlEvents2 _DaCtlEvents2;
#endif 	/* ___DaCtlEvents2_FWD_DEFINED__ */


#ifndef __DaControl_FWD_DEFINED__
#define __DaControl_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaControl DaControl;
#else
typedef struct DaControl DaControl;
#endif /* __cplusplus */

#endif 	/* __DaControl_FWD_DEFINED__ */


#ifndef __DaCtlCharacters_FWD_DEFINED__
#define __DaCtlCharacters_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlCharacters DaCtlCharacters;
#else
typedef struct DaCtlCharacters DaCtlCharacters;
#endif /* __cplusplus */

#endif 	/* __DaCtlCharacters_FWD_DEFINED__ */


#ifndef __DaCtlCharacter_FWD_DEFINED__
#define __DaCtlCharacter_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlCharacter DaCtlCharacter;
#else
typedef struct DaCtlCharacter DaCtlCharacter;
#endif /* __cplusplus */

#endif 	/* __DaCtlCharacter_FWD_DEFINED__ */


#ifndef __DaCtlBalloon_FWD_DEFINED__
#define __DaCtlBalloon_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlBalloon DaCtlBalloon;
#else
typedef struct DaCtlBalloon DaCtlBalloon;
#endif /* __cplusplus */

#endif 	/* __DaCtlBalloon_FWD_DEFINED__ */


#ifndef __DaCtlCommands_FWD_DEFINED__
#define __DaCtlCommands_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlCommands DaCtlCommands;
#else
typedef struct DaCtlCommands DaCtlCommands;
#endif /* __cplusplus */

#endif 	/* __DaCtlCommands_FWD_DEFINED__ */


#ifndef __DaCtlCommand_FWD_DEFINED__
#define __DaCtlCommand_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlCommand DaCtlCommand;
#else
typedef struct DaCtlCommand DaCtlCommand;
#endif /* __cplusplus */

#endif 	/* __DaCtlCommand_FWD_DEFINED__ */


#ifndef __DaCtlRequest_FWD_DEFINED__
#define __DaCtlRequest_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlRequest DaCtlRequest;
#else
typedef struct DaCtlRequest DaCtlRequest;
#endif /* __cplusplus */

#endif 	/* __DaCtlRequest_FWD_DEFINED__ */


#ifndef __DaCtlAudioOutput_FWD_DEFINED__
#define __DaCtlAudioOutput_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlAudioOutput DaCtlAudioOutput;
#else
typedef struct DaCtlAudioOutput DaCtlAudioOutput;
#endif /* __cplusplus */

#endif 	/* __DaCtlAudioOutput_FWD_DEFINED__ */


#ifndef __DaCtlSpeechInput_FWD_DEFINED__
#define __DaCtlSpeechInput_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlSpeechInput DaCtlSpeechInput;
#else
typedef struct DaCtlSpeechInput DaCtlSpeechInput;
#endif /* __cplusplus */

#endif 	/* __DaCtlSpeechInput_FWD_DEFINED__ */


#ifndef __DaCtlPropertySheet_FWD_DEFINED__
#define __DaCtlPropertySheet_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlPropertySheet DaCtlPropertySheet;
#else
typedef struct DaCtlPropertySheet DaCtlPropertySheet;
#endif /* __cplusplus */

#endif 	/* __DaCtlPropertySheet_FWD_DEFINED__ */


#ifndef __DaCtlUserInput_FWD_DEFINED__
#define __DaCtlUserInput_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlUserInput DaCtlUserInput;
#else
typedef struct DaCtlUserInput DaCtlUserInput;
#endif /* __cplusplus */

#endif 	/* __DaCtlUserInput_FWD_DEFINED__ */


#ifndef __DaCtlCommandsWindow_FWD_DEFINED__
#define __DaCtlCommandsWindow_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlCommandsWindow DaCtlCommandsWindow;
#else
typedef struct DaCtlCommandsWindow DaCtlCommandsWindow;
#endif /* __cplusplus */

#endif 	/* __DaCtlCommandsWindow_FWD_DEFINED__ */


#ifndef __DaCtlAnimationNames_FWD_DEFINED__
#define __DaCtlAnimationNames_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlAnimationNames DaCtlAnimationNames;
#else
typedef struct DaCtlAnimationNames DaCtlAnimationNames;
#endif /* __cplusplus */

#endif 	/* __DaCtlAnimationNames_FWD_DEFINED__ */


#ifndef __DaCtlSettings_FWD_DEFINED__
#define __DaCtlSettings_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlSettings DaCtlSettings;
#else
typedef struct DaCtlSettings DaCtlSettings;
#endif /* __cplusplus */

#endif 	/* __DaCtlSettings_FWD_DEFINED__ */


#ifndef __DaCtlCharacterFiles_FWD_DEFINED__
#define __DaCtlCharacterFiles_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlCharacterFiles DaCtlCharacterFiles;
#else
typedef struct DaCtlCharacterFiles DaCtlCharacterFiles;
#endif /* __cplusplus */

#endif 	/* __DaCtlCharacterFiles_FWD_DEFINED__ */


#ifndef __DaCtlFormattedText_FWD_DEFINED__
#define __DaCtlFormattedText_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlFormattedText DaCtlFormattedText;
#else
typedef struct DaCtlFormattedText DaCtlFormattedText;
#endif /* __cplusplus */

#endif 	/* __DaCtlFormattedText_FWD_DEFINED__ */


#ifndef __DaCtlTTSEngine_FWD_DEFINED__
#define __DaCtlTTSEngine_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlTTSEngine DaCtlTTSEngine;
#else
typedef struct DaCtlTTSEngine DaCtlTTSEngine;
#endif /* __cplusplus */

#endif 	/* __DaCtlTTSEngine_FWD_DEFINED__ */


#ifndef __DaCtlTTSEngines_FWD_DEFINED__
#define __DaCtlTTSEngines_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlTTSEngines DaCtlTTSEngines;
#else
typedef struct DaCtlTTSEngines DaCtlTTSEngines;
#endif /* __cplusplus */

#endif 	/* __DaCtlTTSEngines_FWD_DEFINED__ */


#ifndef __DaCtlSREngine_FWD_DEFINED__
#define __DaCtlSREngine_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlSREngine DaCtlSREngine;
#else
typedef struct DaCtlSREngine DaCtlSREngine;
#endif /* __cplusplus */

#endif 	/* __DaCtlSREngine_FWD_DEFINED__ */


#ifndef __DaCtlSREngines_FWD_DEFINED__
#define __DaCtlSREngines_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaCtlSREngines DaCtlSREngines;
#else
typedef struct DaCtlSREngines DaCtlSREngines;
#endif /* __cplusplus */

#endif 	/* __DaCtlSREngines_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __DoubleAgentCtl_LIBRARY_DEFINED__
#define __DoubleAgentCtl_LIBRARY_DEFINED__

/* library DoubleAgentCtl */
/* [control][helpfile][helpstring][version][uuid] */ 

class __declspec(uuid("{1147E550-A208-11DE-ABF2-002421116FB2}")) DoubleAgentCtl_TypeLib;
#define	DoubleAgentCtl_MajorVer	( 1 )

#define	DoubleAgentCtl_MinorVer	( 1 )






























#ifndef _DA_COMMON_TYPES_
#define _DA_COMMON_TYPES_
typedef /* [helpstring][uuid] */  DECLSPEC_UUID("1147E56A-A208-11DE-ABF2-002421116FB2") 
enum BalloonStyleFlags
    {	BalloonStyle_Enabled	= 0x1,
	BalloonStyle_SizeToText	= 0x2,
	BalloonStyle_AutoHide	= 0x4,
	BalloonStyle_AutoPace	= 0x8,
	BalloonStyle_NoAppend	= 0x10,
	BalloonStyle_ShowPartialLines	= 0x20
    } 	BalloonStyleFlags;

typedef /* [helpstring][uuid] */  DECLSPEC_UUID("1147E56C-A208-11DE-ABF2-002421116FB2") 
enum StopAllFlags
    {	StopAll_Play	= 0x1,
	StopAll_Move	= 0x2,
	StopAll_Speak	= 0x4,
	StopAll_QueuedPrepare	= 0x8,
	StopAll_ImmediatePrepare	= 0x10,
	StopAll_Visibility	= 0x20,
	StopAll_Everything	= 0xffffffff
    } 	StopAllFlags;

typedef /* [helpstring][uuid] */  DECLSPEC_UUID("1147E56D-A208-11DE-ABF2-002421116FB2") 
enum PrepareResourceType
    {	PrepareResource_Animation	= 0,
	PrepareResource_State	= 1,
	PrepareResource_Wave	= 2
    } 	PrepareResourceType;

typedef /* [helpstring][uuid] */  DECLSPEC_UUID("1147E56E-A208-11DE-ABF2-002421116FB2") 
enum MoveCauseType
    {	MoveCause_NeverMoved	= 0,
	MoveCause_UserMoved	= 1,
	MoveCause_ProgramMoved	= 2,
	MoveCause_OtherProgramMoved	= 3
    } 	MoveCauseType;

typedef /* [helpstring][uuid] */  DECLSPEC_UUID("1147E56F-A208-11DE-ABF2-002421116FB2") 
enum VisibilityCauseType
    {	VisibilityCause_NeverShown	= 0,
	VisibilityCause_UserHid	= 1,
	VisibilityCause_UserShowed	= 2,
	VisibilityCause_ProgramHid	= 3,
	VisibilityCause_ProgramShowed	= 4,
	VisibilityCause_OtherProgramHid	= 5,
	VisibilityCause_OtherProgramShowed	= 6
    } 	VisibilityCauseType;

typedef /* [helpstring][uuid] */  DECLSPEC_UUID("1147E590-A208-11DE-ABF2-002421116FB2") 
enum AudioStatusType
    {	AudioStatus_Available	= 0,
	AudioStatus_Disabled	= 1,
	AudioStatus_UserSpeaking	= 3,
	AudioStatus_CharacterSpeaking	= 4,
	AudioStatus_CharacterListening	= 5,
	AudioStatus_Error	= 6
    } 	AudioStatusType;

typedef /* [helpstring][uuid] */  DECLSPEC_UUID("1147E591-A208-11DE-ABF2-002421116FB2") 
enum ListeningStatusType
    {	ListeningStatus_Available	= 0,
	ListeningStatus_CharacterInactive	= 2,
	ListeningStatus_InitializeFailed	= 4,
	ListeningStatus_InputDisabled	= 5,
	ListeningStatus_Error	= 6
    } 	ListeningStatusType;

typedef /* [helpstring][uuid] */  DECLSPEC_UUID("1147E592-A208-11DE-ABF2-002421116FB2") 
enum ListenCompleteType
    {	ListenComplete_ProgramDisabled	= 1,
	ListenComplete_ProgramTimedOut	= 2,
	ListenComplete_UserTimedOut	= 3,
	ListenComplete_UserReleasedKey	= 4,
	ListenComplete_UserSpeechEnded	= 5,
	ListenComplete_CharacterClientDeactivated	= 6,
	ListenComplete_DefaultCharacterChanged	= 7,
	ListenComplete_UserDisabled	= 8
    } 	ListenCompleteType;

typedef /* [helpstring][uuid] */  DECLSPEC_UUID("1147E593-A208-11DE-ABF2-002421116FB2") 
enum SpeechGenderType
    {	SpeechGender_Neutral	= 0,
	SpeechGender_Female	= 1,
	SpeechGender_Male	= 2
    } 	SpeechGenderType;

#define	PropertySheet_PageName_Character	( "Character" )

#define	PropertySheet_PageName_Output	( "Output" )

#define	PropertySheet_PageName_Speech	( "Speech" )

#define	PropertySheet_PageName_Copyright	( "Copyright" )

#endif // _DA_COMMON_TYPES_
#ifndef _ActiveStateType_Defined
#define _ActiveStateType_Defined
typedef /* [helpstring][uuid] */  DECLSPEC_UUID("1147E56B-A208-11DE-ABF2-002421116FB2") 
enum ActiveStateType
    {	ActiveState_Inactive	= 0,
	ActiveState_Active	= 1,
	ActiveState_InputActive	= 2,
	ActiveState_Suspended	= 4,
	ActiveState_Suspended_Active	= 5,
	ActiveState_Suspended_InputActive	= 6
    } 	ActiveStateType;

#endif
#ifndef _CharacterStyleFlags_Defined
#define _CharacterStyleFlags_Defined
typedef /* [helpstring][uuid] */  DECLSPEC_UUID("1147E569-A208-11DE-ABF2-002421116FB2") 
enum CharacterStyleFlags
    {	CharacterStyle_SoundEffects	= 0x1,
	CharacterStyle_IdleEnabled	= 0x2,
	CharacterStyle_AutoPopupMenu	= 0x4,
	CharacterStyle_IconShown	= 0x8,
	CharacterStyle_Smoothed	= 0x30,
	CharacterStyle_SmoothEdges	= 0x20,
	CharacterStyle_SuspendPause	= 0x100,
	CharacterStyle_SuspendStop	= 0x200,
	CharacterStyle_SuspendHide	= 0x400
    } 	CharacterStyleFlags;

#endif
typedef /* [helpstring][uuid] */  DECLSPEC_UUID("1147E594-A208-11DE-ABF2-002421116FB2") 
enum RequestStatus
    {	RequestStatus_Success	= 0,
	RequestStatus_Failed	= 1,
	RequestStatus_Pending	= 2,
	RequestStatus_Interrupted	= 3,
	RequestStatus_InProgress	= 4
    } 	RequestStatus;

#define	RequestSuccess	( RequestStatus_Success )

#define	RequestFailed	( RequestStatus_Failed )

#define	RequestPending	( RequestStatus_Pending )

#define	RequestInterrupted	( RequestStatus_Interrupted )

#define	RequestInProgress	( RequestStatus_InProgress )

#define	DISPID_IAgentCtl_Characters	( 3 )

#define	DISPID_IAgentCtl_AudioOutput	( 4 )

#define	DISPID_IAgentCtl_SpeechInput	( 5 )

#define	DISPID_IAgentCtl_PropertySheet	( 8 )

#define	DISPID_IAgentCtl_Connected	( 9 )

#define	DISPID_IAgentCtl_CommandsWindow	( 12 )

#define	DISPID_IAgentCtl_Suspended	( 14 )

#define	DISPID_IAgentCtlEx_ShowDefaultCharacterProperties	( 20 )

#define	DISPID_IAgentCtlEx_RaiseRequestErrors	( 21 )

#define	DISPID_IDaControl2_CharacterStyle	( 30 )

#define	DISPID_IDaControl2_CharacterFiles	( 31 )

#define	DISPID_IDaControl2_TTSEngines	( 32 )

#define	DISPID_IDaControl2_FindTTSEngines	( 33 )

#define	DISPID_IDaControl2_GetCharacterTTSEngine	( 34 )

#define	DISPID_IDaControl2_FindCharacterTTSEngines	( 35 )

#define	DISPID_IDaControl2_SREngines	( 36 )

#define	DISPID_IDaControl2_FindSREngines	( 37 )

#define	DISPID_IDaControl2_GetCharacterSREngine	( 38 )

#define	DISPID_IDaControl2_FindCharacterSREngines	( 39 )

#define	DISPID_IDaControl2_Settings	( 40 )

#define	DISPID_IDaControl2_AutoConnect	( 41 )

#define	DISPID_IDaControl2_ControlCharacter	( 42 )

#define	DISPID_AgentEvents_ActivateInput	( 1 )

#define	DISPID_AgentEvents_Click	( 2 )

#define	DISPID_AgentEvents_DeactivateInput	( 3 )

#define	DISPID_AgentEvents_DblClick	( 4 )

#define	DISPID_AgentEvents_DragStart	( 5 )

#define	DISPID_AgentEvents_DragComplete	( 6 )

#define	DISPID_AgentEvents_Hide	( 7 )

#define	DISPID_AgentEvents_RequestStart	( 9 )

#define	DISPID_AgentEvents_RequestComplete	( 11 )

#define	DISPID_AgentEvents_Shutdown	( 12 )

#define	DISPID_AgentEvents_Show	( 15 )

#define	DISPID_AgentEvents_Bookmark	( 16 )

#define	DISPID_AgentEvents_Command	( 17 )

#define	DISPID_AgentEvents_IdleStart	( 19 )

#define	DISPID_AgentEvents_IdleComplete	( 20 )

#define	DISPID_AgentEvents_Restart	( 21 )

#define	DISPID_AgentEvents_Move	( 22 )

#define	DISPID_AgentEvents_Size	( 23 )

#define	DISPID_AgentEvents_BalloonShow	( 24 )

#define	DISPID_AgentEvents_BalloonHide	( 25 )

#define	DISPID_AgentEvents_HelpComplete	( 26 )

#define	DISPID_AgentEvents_ListenStart	( 27 )

#define	DISPID_AgentEvents_ListenComplete	( 28 )

#define	DISPID_AgentEvents_DefaultCharacterChange	( 30 )

#define	DISPID_AgentEvents_AgentPropertyChange	( 31 )

#define	DISPID_AgentEvents_ActiveClientChange	( 32 )

#define	DISPID_DaCtlEvents2_SpeechStart	( 33 )

#define	DISPID_DaCtlEvents2_SpeechEnd	( 34 )

#define	DISPID_DaCtlEvents2_SpeechWord	( 35 )

#define	DISPID_IAgentCtlCharacters_Load	( 1 )

#define	DISPID_IAgentCtlCharacters_Unload	( 2 )

#define	DISPID_IAgentCtlCharacters_Character	( 3 )

#define	DISPID_IDaCtlCharacters2_Count	( 4 )

#define	DISPID_IDaCtlCharacters2_Index	( 5 )

#define	DISPID_IAgentCtlCharacter_Visible	( 2 )

#define	DISPID_IAgentCtlCharacter_Left	( 3 )

#define	DISPID_IAgentCtlCharacter_Top	( 4 )

#define	DISPID_IAgentCtlCharacter_Height	( 5 )

#define	DISPID_IAgentCtlCharacter_Width	( 6 )

#define	DISPID_IAgentCtlCharacter_Speed	( 10 )

#define	DISPID_IAgentCtlCharacter_Pitch	( 11 )

#define	DISPID_IAgentCtlCharacter_Play	( 13 )

#define	DISPID_IAgentCtlCharacter_Stop	( 14 )

#define	DISPID_IAgentCtlCharacter_Speak	( 15 )

#define	DISPID_IAgentCtlCharacter_GestureAt	( 17 )

#define	DISPID_IAgentCtlCharacter_MoveTo	( 18 )

#define	DISPID_IAgentCtlCharacter_Hide	( 19 )

#define	DISPID_IAgentCtlCharacter_Show	( 20 )

#define	DISPID_IAgentCtlCharacter_Interrupt	( 21 )

#define	DISPID_IAgentCtlCharacter_Wait	( 22 )

#define	DISPID_IAgentCtlCharacter_Balloon	( 23 )

#define	DISPID_IAgentCtlCharacter_Name	( 24 )

#define	DISPID_IAgentCtlCharacter_Commands	( 25 )

#define	DISPID_IAgentCtlCharacter_Activate	( 26 )

#define	DISPID_IAgentCtlCharacter_Get	( 27 )

#define	DISPID_IAgentCtlCharacter_Description	( 28 )

#define	DISPID_IAgentCtlCharacter_IdleOn	( 29 )

#define	DISPID_IAgentCtlCharacter_StopAll	( 31 )

#define	DISPID_IAgentCtlCharacter_MoveCause	( 32 )

#define	DISPID_IAgentCtlCharacter_VisibilityCause	( 33 )

#define	DISPID_IAgentCtlCharacter_HasOtherClients	( 34 )

#define	DISPID_IAgentCtlCharacter_SoundEffectsOn	( 35 )

#define	DISPID_IAgentCtlCharacter_ExtraData	( 36 )

#define	DISPID_IAgentCtlCharacterEx_ShowPopupMenu	( 37 )

#define	DISPID_IAgentCtlCharacterEx_AutoPopupMenu	( 38 )

#define	DISPID_IAgentCtlCharacterEx_HelpModeOn	( 39 )

#define	DISPID_IAgentCtlCharacterEx_HelpContextID	( 40 )

#define	DISPID_IAgentCtlCharacterEx_Active	( 41 )

#define	DISPID_IAgentCtlCharacterEx_Listen	( 42 )

#define	DISPID_IAgentCtlCharacterEx_LanguageID	( 43 )

#define	DISPID_IAgentCtlCharacterEx_SRModeID	( 46 )

#define	DISPID_IAgentCtlCharacterEx_TTSModeID	( 47 )

#define	DISPID_IAgentCtlCharacterEx_HelpFile	( 48 )

#define	DISPID_IAgentCtlCharacterEx_GUID	( 49 )

#define	DISPID_IAgentCtlCharacterEx_OriginalHeight	( 50 )

#define	DISPID_IAgentCtlCharacterEx_OriginalWidth	( 51 )

#define	DISPID_IAgentCtlCharacterEx_Think	( 52 )

#define	DISPID_IAgentCtlCharacterEx_Version	( 53 )

#define	DISPID_IAgentCtlCharacterEx_AnimationNames	( 54 )

#define	DISPID_IAgentCtlCharacterEx_SRStatus	( 55 )

#define	DISPID_IDaCtlCharacter2_Style	( 60 )

#define	DISPID_IDaCtlCharacter2_HasIcon	( 61 )

#define	DISPID_IDaCtlCharacter2_GenerateIcon	( 62 )

#define	DISPID_IDaCtlCharacter2_IconShown	( 63 )

#define	DISPID_IDaCtlCharacter2_IconVisible	( 64 )

#define	DISPID_IDaCtlCharacter2_IconIdentity	( 65 )

#define	DISPID_IDaCtlCharacter2_IconTip	( 66 )

#define	DISPID_IDaCtlCharacter2_TTSEngine	( 67 )

#define	DISPID_IDaCtlCharacter2_FindTTSEngines	( 68 )

#define	DISPID_IDaCtlCharacter2_SREngine	( 69 )

#define	DISPID_IDaCtlCharacter2_FindSREngines	( 70 )

#define	DISPID_IDaCtlCharacter2_UniqueID	( 71 )

#define	DISPID_IDaCtlCharacter2_CharacterID	( 72 )

#define	DISPID_IDaCtlCharacter2_Connected	( 73 )

#define	DISPID_IDaCtlCharacter2_FileName	( 74 )

#define	DISPID_IDaCtlCharacter2_FilePath	( 75 )

#define	DISPID_IDaCtlCharacter2_ActiveState	( 76 )

#define	DISPID_IDaCtlCharacter2_IdleState	( 77 )

#define	DISPID_IDaCtlCharacter2_IdleEnabled	( 78 )

#define	DISPID_IDaCtlCharacter2_SoundEffectsEnabled	( 79 )

#define	DISPID_IDaCtlCharacter2_Prepare	( 80 )

#define	DISPID_IDaCtlCharacter2_ListeningStatus	( 81 )

#define	DISPID_IDaCtlCharacter2_Smoothed	( 82 )

#define	DISPID_IDaCtlCharacter2_SmoothEdges	( 83 )

#define	DISPID_IDaCtlCharacter2_Animations	( 84 )

#define	DISPID_IDaCtlCharacter2_States	( 85 )

#define	DISPID_IDaCtlCharacter2_SpeakFormatted	( 86 )

#define	DISPID_IDaCtlCharacter2_ThinkFormatted	( 87 )

#define	DISPID_IDaCtlCharacter2_NewFormattedText	( 88 )

#define	DISPID_IDaCtlCharacter2_SuspendPause	( 89 )

#define	DISPID_IDaCtlCharacter2_SuspendStop	( 90 )

#define	DISPID_IDaCtlCharacter2_SuspendHide	( 91 )

#define	DISPID_IAgentCtlRequest_Status	( 1 )

#define	DISPID_IAgentCtlRequest_Description	( 2 )

#define	DISPID_IAgentCtlRequest_Number	( 3 )

#define	DISPID_IAgentCtlBalloon_Enabled	( 1 )

#define	DISPID_IAgentCtlBalloon_NumberOfLines	( 2 )

#define	DISPID_IAgentCtlBalloon_CharsPerLine	( 3 )

#define	DISPID_IAgentCtlBalloon_FontName	( 4 )

#define	DISPID_IAgentCtlBalloon_FontSize	( 5 )

#define	DISPID_IAgentCtlBalloon_ForeColor	( 7 )

#define	DISPID_IAgentCtlBalloon_BackColor	( 8 )

#define	DISPID_IAgentCtlBalloon_BorderColor	( 9 )

#define	DISPID_IAgentCtlBalloon_FontBold	( 10 )

#define	DISPID_IAgentCtlBalloon_FontItalic	( 11 )

#define	DISPID_IAgentCtlBalloon_FontStrikethru	( 12 )

#define	DISPID_IAgentCtlBalloon_FontUnderline	( 13 )

#define	DISPID_IAgentCtlBalloon_Visible	( 14 )

#define	DISPID_IAgentCtlBalloon_FontCharSet	( 15 )

#define	DISPID_IAgentCtlBalloonEx_Style	( 16 )

#define	DISPID_IDaCtlBalloon2_TextColor	( 6 )

#define	DISPID_IDaCtlBalloon2_SizeToText	( 17 )

#define	DISPID_IDaCtlBalloon2_AutoHide	( 18 )

#define	DISPID_IDaCtlBalloon2_AutoPace	( 19 )

#define	DISPID_IDaCtlBalloon2_NoAppend	( 20 )

#define	DISPID_IDaCtlBalloon2_ShowPartialLines	( 21 )

#define	DISPID_IAgentCtlCommands_Count	( 2 )

#define	DISPID_IAgentCtlCommands_Caption	( 3 )

#define	DISPID_IAgentCtlCommands_Voice	( 4 )

#define	DISPID_IAgentCtlCommands_Visible	( 5 )

#define	DISPID_IAgentCtlCommands_Add	( 10 )

#define	DISPID_IAgentCtlCommands_Insert	( 11 )

#define	DISPID_IAgentCtlCommands_Remove	( 13 )

#define	DISPID_IAgentCtlCommands_RemoveAll	( 14 )

#define	DISPID_IAgentCtlCommands_Command	( 15 )

#define	DISPID_IAgentCtlCommandsEx_DefaultCommand	( 16 )

#define	DISPID_IAgentCtlCommandsEx_HelpContextID	( 17 )

#define	DISPID_IAgentCtlCommandsEx_FontName	( 21 )

#define	DISPID_IAgentCtlCommandsEx_FontSize	( 23 )

#define	DISPID_IAgentCtlCommandsEx_VoiceCaption	( 22 )

#define	DISPID_IAgentCtlCommandsEx_GlobalVoiceCommandsEnabled	( 24 )

#define	DISPID_IDaCtlCommands2_VoiceGrammar	( 25 )

#define	DISPID_IDaCtlCommands2_Index	( 26 )

#define	DISPID_IAgentCtlCommand_Voice	( 1 )

#define	DISPID_IAgentCtlCommand_Caption	( 2 )

#define	DISPID_IAgentCtlCommand_Confidence	( 3 )

#define	DISPID_IAgentCtlCommand_ConfidenceText	( 4 )

#define	DISPID_IAgentCtlCommand_Enabled	( 5 )

#define	DISPID_IAgentCtlCommand_Visible	( 6 )

#define	DISPID_IAgentCtlCommandEx_HelpContextID	( 7 )

#define	DISPID_IAgentCtlCommandEx_VoiceCaption	( 8 )

#define	DISPID_IDaCtlCommand2_VoiceGrammar	( 9 )

#define	DISPID_IDaCtlCommand2_ConfidenceThreshold	( 10 )

#define	DISPID_IDaCtlCommand2_Name	( 11 )

#define	DISPID_IAgentCtlUserInput_Count	( 0x60020000 )

#define	DISPID_IAgentCtlUserInput_Name	( 0x60020001 )

#define	DISPID_IAgentCtlUserInput_CharacterID	( 0x60020002 )

#define	DISPID_IAgentCtlUserInput_Confidence	( 0x60020003 )

#define	DISPID_IAgentCtlUserInput_Voice	( 0x60020004 )

#define	DISPID_IAgentCtlUserInput_Alt1Name	( 0x60020005 )

#define	DISPID_IAgentCtlUserInput_Alt1Confidence	( 0x60020006 )

#define	DISPID_IAgentCtlUserInput_Alt1Voice	( 0x60020007 )

#define	DISPID_IAgentCtlUserInput_Alt2Name	( 0x60020008 )

#define	DISPID_IAgentCtlUserInput_Alt2Confidence	( 0x60020009 )

#define	DISPID_IAgentCtlUserInput_Alt2Voice	( 0x6002000a )

#define	DISPID_IAgentCtlCommandsWindow_Visible	( 5 )

#define	DISPID_IAgentCtlCommandsWindow_Left	( 6 )

#define	DISPID_IAgentCtlCommandsWindow_Top	( 7 )

#define	DISPID_IAgentCtlCommandsWindow_Height	( 8 )

#define	DISPID_IAgentCtlCommandsWindow_Width	( 9 )

#define	DISPID_IAgentCtlAudioObject_Enabled	( 1 )

#define	DISPID_IAgentCtlAudioObject_SoundEffects	( 2 )

#define	DISPID_IAgentCtlAudioObjectEx_Status	( 3 )

#define	DISPID_IAgentCtlSpeechInput_Enabled	( 1 )

#define	DISPID_IAgentCtlSpeechInput_Language	( 2 )

#define	DISPID_IAgentCtlSpeechInput_HotKey	( 3 )

#define	DISPID_IAgentCtlSpeechInput_Installed	( 4 )

#define	DISPID_IAgentCtlSpeechInput_Engine	( 5 )

#define	DISPID_IAgentCtlSpeechInput_ListeningTip	( 6 )

#define	DISPID_IDaCtlSettings_SoundEffectsEnabled	( 1 )

#define	DISPID_IDaCtlSettings_BalloonEnabled	( 2 )

#define	DISPID_IDaCtlSettings_BalloonFont	( 3 )

#define	DISPID_IDaCtlSettings_TTSEnabled	( 4 )

#define	DISPID_IDaCtlSettings_TTSSpeed	( 5 )

#define	DISPID_IDaCtlSettings_SREnabled	( 6 )

#define	DISPID_IDaCtlSettings_SRHotKey	( 7 )

#define	DISPID_IDaCtlSettings_SRHotKeyTime	( 8 )

#define	DISPID_IDaCtlSettings_SRListeningTip	( 9 )

#define	DISPID_IDaCtlSettings_SRListeningPrompt	( 10 )

#define	DISPID_IDaCtlSettings_AudioStatus	( 11 )

#define	DISPID_IAgentCtlPropertySheet_Left	( 1 )

#define	DISPID_IAgentCtlPropertySheet_Top	( 2 )

#define	DISPID_IAgentCtlPropertySheet_Height	( 3 )

#define	DISPID_IAgentCtlPropertySheet_Width	( 4 )

#define	DISPID_IAgentCtlPropertySheet_Page	( 5 )

#define	DISPID_IAgentCtlPropertySheet_Visible	( 6 )

#define	DISPID_IDaCtlCharacterFiles_FilePaths	( 0x60020000 )

#define	DISPID_IDaCtlCharacterFiles_SearchPath	( 0x60020001 )

#define	DISPID_IDaCtlCharacterFiles_DefaultSearchPath	( 0x60020002 )

#define	DISPID_IDaCtlCharacterFiles_DoubleAgentFiles	( 0x60020003 )

#define	DISPID_IDaCtlCharacterFiles_MsAgentFiles	( 0x60020004 )

#define	DISPID_IDaCtlCharacterFiles_MsOfficeFiles	( 0x60020005 )

#define	DISPID_IDaCtlCharacterFiles_SpeakingCharacters	( 0x60020006 )

#define	DISPID_IDaCtlCharacterFiles_NonSpeakingCharacters	( 0x60020007 )

#define	DISPID_IDaCtlCharacterFiles_VerifyVersion	( 0x60020008 )

#define	DISPID_IDaCtlCharacterFiles_DefaultFilePath	( 0x60020009 )

#define	DISPID_IDaCtlCharacterFiles_DefaultFileName	( 0x6002000a )

#define	DISPID_IDaCtlCharacterFiles_CompliantCharacters	( 0x6002000b )

#define	DISPID_IDaCtlCharacterFiles_NonCompliantCharacters	( 0x6002000c )

#define	DISPID_IDaCtlFormattedText_RawText	( DISPID_VALUE )

#define	DISPID_IDaCtlFormattedText_ParsedText	( 0x60020001 )

#define	DISPID_IDaCtlFormattedText_SpeechText	( 0x60020002 )

#define	DISPID_IDaCtlFormattedText_WordCount	( 0x60020003 )

#define	DISPID_IDaCtlFormattedText_DisplayWord	( 0x60020004 )

#define	DISPID_IDaCtlFormattedText_WordIndex	( 0x60020005 )

#define	DISPID_IDaCtlFormattedText_DisplayText	( 0x60020006 )

#define	DISPID_IDaCtlFormattedText_Parse	( 0x60020007 )

#define	DISPID_IDaCtlFormattedText_Append	( 0x60020008 )

#define	DISPID_IDaCtlTTSEngine_TTSModeID	( DISPID_VALUE )

#define	DISPID_IDaCtlTTSEngine_DisplayName	( 1 )

#define	DISPID_IDaCtlTTSEngine_Manufacturer	( 2 )

#define	DISPID_IDaCtlTTSEngine_GetVersion	( 3 )

#define	DISPID_IDaCtlTTSEngine_Gender	( 4 )

#define	DISPID_IDaCtlTTSEngine_LanguageID	( 5 )

#define	DISPID_IDaCtlTTSEngine_LanguageName	( 6 )

#define	DISPID_IDaCtlSREngine_SRModeID	( DISPID_VALUE )

#define	DISPID_IDaCtlSREngine_DisplayName	( 1 )

#define	DISPID_IDaCtlSREngine_Manufacturer	( 2 )

#define	DISPID_IDaCtlSREngine_GetVersion	( 3 )

#define	DISPID_IDaCtlSREngine_LanguageID	( 4 )

#define	DISPID_IDaCtlSREngine_LanguageName	( 5 )

#define	DISPID_IDaCtlSREngine_LanguageIDs	( 6 )

#define	DISPID_IDaCtlSREngine_LanguageNames	( 7 )


EXTERN_C const IID LIBID_DoubleAgentCtl;

#ifndef __IDaCtlCharacters_INTERFACE_DEFINED__
#define __IDaCtlCharacters_INTERFACE_DEFINED__

/* interface IDaCtlCharacters */
/* [object][custom][helpcontext][hidden][nonextensible][oleautomation][dual][unique][uuid] */ 


EXTERN_C const IID IID_IDaCtlCharacters;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E53C-A208-11DE-ABF2-002421116FB2")
    IDaCtlCharacters : public IDispatch
    {
    public:
        virtual /* [propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ BSTR CharacterID,
            /* [retval][out] */ IDaCtlCharacter2 **Item) = 0;
        
        virtual /* [hidden][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE Character( 
            /* [in] */ BSTR CharacterID,
            /* [retval][out] */ IDaCtlCharacter2 **Character) = 0;
        
        virtual /* [restricted][propget][id] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **EnumVariant) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE Unload( 
            /* [in] */ BSTR CharacterID) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE Load( 
            /* [in] */ BSTR CharacterID,
            /* [optional][in] */ VARIANT Provider,
            /* [retval][out] */ IDaCtlRequest **ppidRequest) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlCharactersVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlCharacters * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlCharacters * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlCharacters * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlCharacters * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlCharacters * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlCharacters * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlCharacters * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IDaCtlCharacters * This,
            /* [in] */ BSTR CharacterID,
            /* [retval][out] */ IDaCtlCharacter2 **Item);
        
        /* [hidden][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Character )( 
            IDaCtlCharacters * This,
            /* [in] */ BSTR CharacterID,
            /* [retval][out] */ IDaCtlCharacter2 **Character);
        
        /* [restricted][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IDaCtlCharacters * This,
            /* [retval][out] */ IUnknown **EnumVariant);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Unload )( 
            IDaCtlCharacters * This,
            /* [in] */ BSTR CharacterID);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Load )( 
            IDaCtlCharacters * This,
            /* [in] */ BSTR CharacterID,
            /* [optional][in] */ VARIANT Provider,
            /* [retval][out] */ IDaCtlRequest **ppidRequest);
        
        END_INTERFACE
    } IDaCtlCharactersVtbl;

    interface IDaCtlCharacters
    {
        CONST_VTBL struct IDaCtlCharactersVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlCharacters_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlCharacters_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlCharacters_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlCharacters_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlCharacters_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlCharacters_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlCharacters_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlCharacters_get_Item(This,CharacterID,Item)	\
    ( (This)->lpVtbl -> get_Item(This,CharacterID,Item) ) 

#define IDaCtlCharacters_Character(This,CharacterID,Character)	\
    ( (This)->lpVtbl -> Character(This,CharacterID,Character) ) 

#define IDaCtlCharacters_get__NewEnum(This,EnumVariant)	\
    ( (This)->lpVtbl -> get__NewEnum(This,EnumVariant) ) 

#define IDaCtlCharacters_Unload(This,CharacterID)	\
    ( (This)->lpVtbl -> Unload(This,CharacterID) ) 

#define IDaCtlCharacters_Load(This,CharacterID,Provider,ppidRequest)	\
    ( (This)->lpVtbl -> Load(This,CharacterID,Provider,ppidRequest) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlCharacters_INTERFACE_DEFINED__ */


#ifndef __IDaCtlCharacter_INTERFACE_DEFINED__
#define __IDaCtlCharacter_INTERFACE_DEFINED__

/* interface IDaCtlCharacter */
/* [object][helpcontext][hidden][nonextensible][oleautomation][dual][unique][uuid] */ 


EXTERN_C const IID IID_IDaCtlCharacter;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E53D-A208-11DE-ABF2-002421116FB2")
    IDaCtlCharacter : public IDispatch
    {
    public:
        virtual /* [propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Balloon( 
            /* [retval][out] */ IDaCtlBalloon2 **Balloon) = 0;
        
        virtual /* [propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Commands( 
            /* [retval][out] */ IDaCtlCommands2 **Commands) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *Name) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Description( 
            /* [retval][out] */ BSTR *Description) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Visible( 
            /* [retval][out] */ VARIANT_BOOL *Visible) = 0;
        
        virtual /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE put_Left( 
            short Left) = 0;
        
        virtual /* [displaybind][bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_Left( 
            /* [retval][out] */ short *Left) = 0;
        
        virtual /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE put_Top( 
            short Top) = 0;
        
        virtual /* [displaybind][bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_Top( 
            /* [retval][out] */ short *Top) = 0;
        
        virtual /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE put_Height( 
            short Height) = 0;
        
        virtual /* [displaybind][bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_Height( 
            /* [retval][out] */ short *Height) = 0;
        
        virtual /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE put_Width( 
            short Width) = 0;
        
        virtual /* [displaybind][bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_Width( 
            /* [retval][out] */ short *Width) = 0;
        
        virtual /* [nonbrowsable][hidden][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Speed( 
            /* [retval][out] */ long *Speed) = 0;
        
        virtual /* [nonbrowsable][hidden][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Pitch( 
            /* [retval][out] */ long *Pitch) = 0;
        
        virtual /* [nonbrowsable][hidden][propput][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE put_IdleOn( 
            /* [in] */ VARIANT_BOOL On) = 0;
        
        virtual /* [nonbrowsable][hidden][propget][id] */ HRESULT STDMETHODCALLTYPE get_IdleOn( 
            /* [retval][out] */ VARIANT_BOOL *On) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE Activate( 
            /* [optional][in] */ VARIANT State,
            /* [retval][out] */ VARIANT_BOOL *Success) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE Play( 
            /* [in] */ BSTR Animation,
            /* [retval][out] */ IDaCtlRequest **Request) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE Get( 
            /* [in] */ BSTR Type,
            /* [in] */ BSTR Name,
            /* [optional][in] */ VARIANT Queue,
            /* [retval][out] */ IDaCtlRequest **Request) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE Stop( 
            /* [optional][in] */ VARIANT Request) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE Wait( 
            /* [in] */ IDaCtlRequest *WaitForRequest,
            /* [retval][out] */ IDaCtlRequest **Request) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE Interrupt( 
            /* [in] */ IDaCtlRequest *InterruptRequest,
            /* [retval][out] */ IDaCtlRequest **Request) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE Speak( 
            /* [optional][in] */ VARIANT Text,
            /* [optional][in] */ VARIANT Url,
            /* [retval][out] */ IDaCtlRequest **Request) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE GestureAt( 
            /* [in] */ short X,
            /* [in] */ short Y,
            /* [retval][out] */ IDaCtlRequest **Request) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE MoveTo( 
            /* [in] */ short X,
            /* [in] */ short Y,
            /* [optional][in] */ VARIANT Speed,
            /* [retval][out] */ IDaCtlRequest **Request) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE Hide( 
            /* [optional][in] */ VARIANT Fast,
            /* [retval][out] */ IDaCtlRequest **Request) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE Show( 
            /* [optional][in] */ VARIANT Fast,
            /* [retval][out] */ IDaCtlRequest **Request) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE StopAll( 
            /* [optional][in] */ VARIANT Types) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_MoveCause( 
            /* [retval][out] */ MoveCauseType *MoveCause) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_VisibilityCause( 
            /* [retval][out] */ VisibilityCauseType *VisibilityCause) = 0;
        
        virtual /* [bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_HasOtherClients( 
            /* [retval][out] */ VARIANT_BOOL *HasOtherClients) = 0;
        
        virtual /* [nonbrowsable][hidden][propput][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE put_SoundEffectsOn( 
            /* [in] */ VARIANT_BOOL On) = 0;
        
        virtual /* [nonbrowsable][hidden][propget][id] */ HRESULT STDMETHODCALLTYPE get_SoundEffectsOn( 
            /* [retval][out] */ VARIANT_BOOL *On) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Name( 
            /* [in] */ BSTR Name) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_Description( 
            /* [in] */ BSTR Description) = 0;
        
        virtual /* [bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_ExtraData( 
            /* [retval][out] */ BSTR *ExtraData) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE ShowPopupMenu( 
            /* [in] */ short X,
            /* [in] */ short Y,
            /* [retval][out] */ VARIANT_BOOL *Showed) = 0;
        
        virtual /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE put_AutoPopupMenu( 
            /* [in] */ VARIANT_BOOL Enabled) = 0;
        
        virtual /* [displaybind][bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_AutoPopupMenu( 
            /* [retval][out] */ VARIANT_BOOL *Enabled) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][propput][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE put_HelpModeOn( 
            /* [in] */ VARIANT_BOOL On) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][propget][id] */ HRESULT STDMETHODCALLTYPE get_HelpModeOn( 
            /* [retval][out] */ VARIANT_BOOL *On) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][propput][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE put_HelpContextID( 
            /* [in] */ long ID) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][propget][id] */ HRESULT STDMETHODCALLTYPE get_HelpContextID( 
            /* [retval][out] */ long *ID) = 0;
        
        virtual /* [nonbrowsable][hidden][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Active( 
            /* [retval][out] */ short *State) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE Listen( 
            /* [in] */ VARIANT_BOOL Listen,
            /* [retval][out] */ VARIANT_BOOL *StartedListening) = 0;
        
        virtual /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE put_LanguageID( 
            /* [in] */ long LanguageID) = 0;
        
        virtual /* [displaybind][bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_LanguageID( 
            /* [retval][out] */ long *LanguageID) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_SRModeID( 
            /* [retval][out] */ BSTR *EngineModeId) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_SRModeID( 
            /* [in] */ BSTR EngineModeId) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_TTSModeID( 
            /* [retval][out] */ BSTR *EngineModeId) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_TTSModeID( 
            /* [in] */ BSTR EngineModeId) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_HelpFile( 
            /* [retval][out] */ BSTR *File) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][propput][id] */ HRESULT STDMETHODCALLTYPE put_HelpFile( 
            /* [in] */ BSTR File) = 0;
        
        virtual /* [nonbrowsable][hidden][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_GUID( 
            /* [retval][out] */ BSTR *CharGUID) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_OriginalHeight( 
            /* [retval][out] */ short *Height) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_OriginalWidth( 
            /* [retval][out] */ short *Width) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE Think( 
            /* [in] */ BSTR Text,
            /* [retval][out] */ IDaCtlRequest **Request) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Version( 
            /* [retval][out] */ BSTR *Version) = 0;
        
        virtual /* [nonbrowsable][hidden][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_AnimationNames( 
            /* [retval][out] */ IDaCtlAnimationNames **Names) = 0;
        
        virtual /* [nonbrowsable][hidden][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_SRStatus( 
            /* [retval][out] */ long *Status) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlCharacterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlCharacter * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlCharacter * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlCharacter * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlCharacter * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlCharacter * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlCharacter * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlCharacter * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Balloon )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ IDaCtlBalloon2 **Balloon);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Commands )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ IDaCtlCommands2 **Commands);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ BSTR *Name);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Description )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ BSTR *Description);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ VARIANT_BOOL *Visible);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_Left )( 
            IDaCtlCharacter * This,
            short Left);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Left )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ short *Left);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_Top )( 
            IDaCtlCharacter * This,
            short Top);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Top )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ short *Top);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_Height )( 
            IDaCtlCharacter * This,
            short Height);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ short *Height);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_Width )( 
            IDaCtlCharacter * This,
            short Width);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ short *Width);
        
        /* [nonbrowsable][hidden][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Speed )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ long *Speed);
        
        /* [nonbrowsable][hidden][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Pitch )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ long *Pitch);
        
        /* [nonbrowsable][hidden][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_IdleOn )( 
            IDaCtlCharacter * This,
            /* [in] */ VARIANT_BOOL On);
        
        /* [nonbrowsable][hidden][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_IdleOn )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ VARIANT_BOOL *On);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Activate )( 
            IDaCtlCharacter * This,
            /* [optional][in] */ VARIANT State,
            /* [retval][out] */ VARIANT_BOOL *Success);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Play )( 
            IDaCtlCharacter * This,
            /* [in] */ BSTR Animation,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Get )( 
            IDaCtlCharacter * This,
            /* [in] */ BSTR Type,
            /* [in] */ BSTR Name,
            /* [optional][in] */ VARIANT Queue,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IDaCtlCharacter * This,
            /* [optional][in] */ VARIANT Request);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Wait )( 
            IDaCtlCharacter * This,
            /* [in] */ IDaCtlRequest *WaitForRequest,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Interrupt )( 
            IDaCtlCharacter * This,
            /* [in] */ IDaCtlRequest *InterruptRequest,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Speak )( 
            IDaCtlCharacter * This,
            /* [optional][in] */ VARIANT Text,
            /* [optional][in] */ VARIANT Url,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GestureAt )( 
            IDaCtlCharacter * This,
            /* [in] */ short X,
            /* [in] */ short Y,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *MoveTo )( 
            IDaCtlCharacter * This,
            /* [in] */ short X,
            /* [in] */ short Y,
            /* [optional][in] */ VARIANT Speed,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Hide )( 
            IDaCtlCharacter * This,
            /* [optional][in] */ VARIANT Fast,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Show )( 
            IDaCtlCharacter * This,
            /* [optional][in] */ VARIANT Fast,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *StopAll )( 
            IDaCtlCharacter * This,
            /* [optional][in] */ VARIANT Types);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_MoveCause )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ MoveCauseType *MoveCause);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_VisibilityCause )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ VisibilityCauseType *VisibilityCause);
        
        /* [bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_HasOtherClients )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ VARIANT_BOOL *HasOtherClients);
        
        /* [nonbrowsable][hidden][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_SoundEffectsOn )( 
            IDaCtlCharacter * This,
            /* [in] */ VARIANT_BOOL On);
        
        /* [nonbrowsable][hidden][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_SoundEffectsOn )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ VARIANT_BOOL *On);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Name )( 
            IDaCtlCharacter * This,
            /* [in] */ BSTR Name);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Description )( 
            IDaCtlCharacter * This,
            /* [in] */ BSTR Description);
        
        /* [bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_ExtraData )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ BSTR *ExtraData);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *ShowPopupMenu )( 
            IDaCtlCharacter * This,
            /* [in] */ short X,
            /* [in] */ short Y,
            /* [retval][out] */ VARIANT_BOOL *Showed);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_AutoPopupMenu )( 
            IDaCtlCharacter * This,
            /* [in] */ VARIANT_BOOL Enabled);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_AutoPopupMenu )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ VARIANT_BOOL *Enabled);
        
        /* [nonbrowsable][restricted][hidden][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_HelpModeOn )( 
            IDaCtlCharacter * This,
            /* [in] */ VARIANT_BOOL On);
        
        /* [nonbrowsable][restricted][hidden][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_HelpModeOn )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ VARIANT_BOOL *On);
        
        /* [nonbrowsable][restricted][hidden][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_HelpContextID )( 
            IDaCtlCharacter * This,
            /* [in] */ long ID);
        
        /* [nonbrowsable][restricted][hidden][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_HelpContextID )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ long *ID);
        
        /* [nonbrowsable][hidden][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Active )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ short *State);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Listen )( 
            IDaCtlCharacter * This,
            /* [in] */ VARIANT_BOOL Listen,
            /* [retval][out] */ VARIANT_BOOL *StartedListening);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_LanguageID )( 
            IDaCtlCharacter * This,
            /* [in] */ long LanguageID);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_LanguageID )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ long *LanguageID);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SRModeID )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ BSTR *EngineModeId);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_SRModeID )( 
            IDaCtlCharacter * This,
            /* [in] */ BSTR EngineModeId);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_TTSModeID )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ BSTR *EngineModeId);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_TTSModeID )( 
            IDaCtlCharacter * This,
            /* [in] */ BSTR EngineModeId);
        
        /* [nonbrowsable][restricted][hidden][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_HelpFile )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ BSTR *File);
        
        /* [nonbrowsable][restricted][hidden][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_HelpFile )( 
            IDaCtlCharacter * This,
            /* [in] */ BSTR File);
        
        /* [nonbrowsable][hidden][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_GUID )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ BSTR *CharGUID);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_OriginalHeight )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ short *Height);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_OriginalWidth )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ short *Width);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Think )( 
            IDaCtlCharacter * This,
            /* [in] */ BSTR Text,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Version )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ BSTR *Version);
        
        /* [nonbrowsable][hidden][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_AnimationNames )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ IDaCtlAnimationNames **Names);
        
        /* [nonbrowsable][hidden][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SRStatus )( 
            IDaCtlCharacter * This,
            /* [retval][out] */ long *Status);
        
        END_INTERFACE
    } IDaCtlCharacterVtbl;

    interface IDaCtlCharacter
    {
        CONST_VTBL struct IDaCtlCharacterVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlCharacter_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlCharacter_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlCharacter_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlCharacter_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlCharacter_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlCharacter_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlCharacter_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlCharacter_get_Balloon(This,Balloon)	\
    ( (This)->lpVtbl -> get_Balloon(This,Balloon) ) 

#define IDaCtlCharacter_get_Commands(This,Commands)	\
    ( (This)->lpVtbl -> get_Commands(This,Commands) ) 

#define IDaCtlCharacter_get_Name(This,Name)	\
    ( (This)->lpVtbl -> get_Name(This,Name) ) 

#define IDaCtlCharacter_get_Description(This,Description)	\
    ( (This)->lpVtbl -> get_Description(This,Description) ) 

#define IDaCtlCharacter_get_Visible(This,Visible)	\
    ( (This)->lpVtbl -> get_Visible(This,Visible) ) 

#define IDaCtlCharacter_put_Left(This,Left)	\
    ( (This)->lpVtbl -> put_Left(This,Left) ) 

#define IDaCtlCharacter_get_Left(This,Left)	\
    ( (This)->lpVtbl -> get_Left(This,Left) ) 

#define IDaCtlCharacter_put_Top(This,Top)	\
    ( (This)->lpVtbl -> put_Top(This,Top) ) 

#define IDaCtlCharacter_get_Top(This,Top)	\
    ( (This)->lpVtbl -> get_Top(This,Top) ) 

#define IDaCtlCharacter_put_Height(This,Height)	\
    ( (This)->lpVtbl -> put_Height(This,Height) ) 

#define IDaCtlCharacter_get_Height(This,Height)	\
    ( (This)->lpVtbl -> get_Height(This,Height) ) 

#define IDaCtlCharacter_put_Width(This,Width)	\
    ( (This)->lpVtbl -> put_Width(This,Width) ) 

#define IDaCtlCharacter_get_Width(This,Width)	\
    ( (This)->lpVtbl -> get_Width(This,Width) ) 

#define IDaCtlCharacter_get_Speed(This,Speed)	\
    ( (This)->lpVtbl -> get_Speed(This,Speed) ) 

#define IDaCtlCharacter_get_Pitch(This,Pitch)	\
    ( (This)->lpVtbl -> get_Pitch(This,Pitch) ) 

#define IDaCtlCharacter_put_IdleOn(This,On)	\
    ( (This)->lpVtbl -> put_IdleOn(This,On) ) 

#define IDaCtlCharacter_get_IdleOn(This,On)	\
    ( (This)->lpVtbl -> get_IdleOn(This,On) ) 

#define IDaCtlCharacter_Activate(This,State,Success)	\
    ( (This)->lpVtbl -> Activate(This,State,Success) ) 

#define IDaCtlCharacter_Play(This,Animation,Request)	\
    ( (This)->lpVtbl -> Play(This,Animation,Request) ) 

#define IDaCtlCharacter_Get(This,Type,Name,Queue,Request)	\
    ( (This)->lpVtbl -> Get(This,Type,Name,Queue,Request) ) 

#define IDaCtlCharacter_Stop(This,Request)	\
    ( (This)->lpVtbl -> Stop(This,Request) ) 

#define IDaCtlCharacter_Wait(This,WaitForRequest,Request)	\
    ( (This)->lpVtbl -> Wait(This,WaitForRequest,Request) ) 

#define IDaCtlCharacter_Interrupt(This,InterruptRequest,Request)	\
    ( (This)->lpVtbl -> Interrupt(This,InterruptRequest,Request) ) 

#define IDaCtlCharacter_Speak(This,Text,Url,Request)	\
    ( (This)->lpVtbl -> Speak(This,Text,Url,Request) ) 

#define IDaCtlCharacter_GestureAt(This,X,Y,Request)	\
    ( (This)->lpVtbl -> GestureAt(This,X,Y,Request) ) 

#define IDaCtlCharacter_MoveTo(This,X,Y,Speed,Request)	\
    ( (This)->lpVtbl -> MoveTo(This,X,Y,Speed,Request) ) 

#define IDaCtlCharacter_Hide(This,Fast,Request)	\
    ( (This)->lpVtbl -> Hide(This,Fast,Request) ) 

#define IDaCtlCharacter_Show(This,Fast,Request)	\
    ( (This)->lpVtbl -> Show(This,Fast,Request) ) 

#define IDaCtlCharacter_StopAll(This,Types)	\
    ( (This)->lpVtbl -> StopAll(This,Types) ) 

#define IDaCtlCharacter_get_MoveCause(This,MoveCause)	\
    ( (This)->lpVtbl -> get_MoveCause(This,MoveCause) ) 

#define IDaCtlCharacter_get_VisibilityCause(This,VisibilityCause)	\
    ( (This)->lpVtbl -> get_VisibilityCause(This,VisibilityCause) ) 

#define IDaCtlCharacter_get_HasOtherClients(This,HasOtherClients)	\
    ( (This)->lpVtbl -> get_HasOtherClients(This,HasOtherClients) ) 

#define IDaCtlCharacter_put_SoundEffectsOn(This,On)	\
    ( (This)->lpVtbl -> put_SoundEffectsOn(This,On) ) 

#define IDaCtlCharacter_get_SoundEffectsOn(This,On)	\
    ( (This)->lpVtbl -> get_SoundEffectsOn(This,On) ) 

#define IDaCtlCharacter_put_Name(This,Name)	\
    ( (This)->lpVtbl -> put_Name(This,Name) ) 

#define IDaCtlCharacter_put_Description(This,Description)	\
    ( (This)->lpVtbl -> put_Description(This,Description) ) 

#define IDaCtlCharacter_get_ExtraData(This,ExtraData)	\
    ( (This)->lpVtbl -> get_ExtraData(This,ExtraData) ) 

#define IDaCtlCharacter_ShowPopupMenu(This,X,Y,Showed)	\
    ( (This)->lpVtbl -> ShowPopupMenu(This,X,Y,Showed) ) 

#define IDaCtlCharacter_put_AutoPopupMenu(This,Enabled)	\
    ( (This)->lpVtbl -> put_AutoPopupMenu(This,Enabled) ) 

#define IDaCtlCharacter_get_AutoPopupMenu(This,Enabled)	\
    ( (This)->lpVtbl -> get_AutoPopupMenu(This,Enabled) ) 

#define IDaCtlCharacter_put_HelpModeOn(This,On)	\
    ( (This)->lpVtbl -> put_HelpModeOn(This,On) ) 

#define IDaCtlCharacter_get_HelpModeOn(This,On)	\
    ( (This)->lpVtbl -> get_HelpModeOn(This,On) ) 

#define IDaCtlCharacter_put_HelpContextID(This,ID)	\
    ( (This)->lpVtbl -> put_HelpContextID(This,ID) ) 

#define IDaCtlCharacter_get_HelpContextID(This,ID)	\
    ( (This)->lpVtbl -> get_HelpContextID(This,ID) ) 

#define IDaCtlCharacter_get_Active(This,State)	\
    ( (This)->lpVtbl -> get_Active(This,State) ) 

#define IDaCtlCharacter_Listen(This,Listen,StartedListening)	\
    ( (This)->lpVtbl -> Listen(This,Listen,StartedListening) ) 

#define IDaCtlCharacter_put_LanguageID(This,LanguageID)	\
    ( (This)->lpVtbl -> put_LanguageID(This,LanguageID) ) 

#define IDaCtlCharacter_get_LanguageID(This,LanguageID)	\
    ( (This)->lpVtbl -> get_LanguageID(This,LanguageID) ) 

#define IDaCtlCharacter_get_SRModeID(This,EngineModeId)	\
    ( (This)->lpVtbl -> get_SRModeID(This,EngineModeId) ) 

#define IDaCtlCharacter_put_SRModeID(This,EngineModeId)	\
    ( (This)->lpVtbl -> put_SRModeID(This,EngineModeId) ) 

#define IDaCtlCharacter_get_TTSModeID(This,EngineModeId)	\
    ( (This)->lpVtbl -> get_TTSModeID(This,EngineModeId) ) 

#define IDaCtlCharacter_put_TTSModeID(This,EngineModeId)	\
    ( (This)->lpVtbl -> put_TTSModeID(This,EngineModeId) ) 

#define IDaCtlCharacter_get_HelpFile(This,File)	\
    ( (This)->lpVtbl -> get_HelpFile(This,File) ) 

#define IDaCtlCharacter_put_HelpFile(This,File)	\
    ( (This)->lpVtbl -> put_HelpFile(This,File) ) 

#define IDaCtlCharacter_get_GUID(This,CharGUID)	\
    ( (This)->lpVtbl -> get_GUID(This,CharGUID) ) 

#define IDaCtlCharacter_get_OriginalHeight(This,Height)	\
    ( (This)->lpVtbl -> get_OriginalHeight(This,Height) ) 

#define IDaCtlCharacter_get_OriginalWidth(This,Width)	\
    ( (This)->lpVtbl -> get_OriginalWidth(This,Width) ) 

#define IDaCtlCharacter_Think(This,Text,Request)	\
    ( (This)->lpVtbl -> Think(This,Text,Request) ) 

#define IDaCtlCharacter_get_Version(This,Version)	\
    ( (This)->lpVtbl -> get_Version(This,Version) ) 

#define IDaCtlCharacter_get_AnimationNames(This,Names)	\
    ( (This)->lpVtbl -> get_AnimationNames(This,Names) ) 

#define IDaCtlCharacter_get_SRStatus(This,Status)	\
    ( (This)->lpVtbl -> get_SRStatus(This,Status) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlCharacter_INTERFACE_DEFINED__ */


#ifndef __IDaCtlBalloon_INTERFACE_DEFINED__
#define __IDaCtlBalloon_INTERFACE_DEFINED__

/* interface IDaCtlBalloon */
/* [object][helpcontext][hidden][nonextensible][oleautomation][dual][unique][uuid] */ 


EXTERN_C const IID IID_IDaCtlBalloon;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E53F-A208-11DE-ABF2-002421116FB2")
    IDaCtlBalloon : public IDispatch
    {
    public:
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Enabled( 
            /* [retval][out] */ VARIANT_BOOL *Enabled) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][propget] */ HRESULT STDMETHODCALLTYPE get__NumberOfLines( 
            /* [retval][out] */ long *Lines) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][propget] */ HRESULT STDMETHODCALLTYPE get__CharsPerLine( 
            /* [retval][out] */ long *CharsPerLine) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_FontName( 
            /* [retval][out] */ BSTR *FontName) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_FontSize( 
            /* [retval][out] */ long *FontSize) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][propget] */ HRESULT STDMETHODCALLTYPE get__FontBold( 
            /* [retval][out] */ VARIANT_BOOL *FontBold) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][propget] */ HRESULT STDMETHODCALLTYPE get__FontItalic( 
            /* [retval][out] */ VARIANT_BOOL *FontItalic) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][propget] */ HRESULT STDMETHODCALLTYPE get__FontStrikethru( 
            /* [retval][out] */ VARIANT_BOOL *FontStrikethru) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][propget] */ HRESULT STDMETHODCALLTYPE get__FontUnderline( 
            /* [retval][out] */ VARIANT_BOOL *FontUnderline) = 0;
        
        virtual /* [nonbrowsable][hidden][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_ForeColor( 
            /* [retval][out] */ long *ForeColor) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][propget] */ HRESULT STDMETHODCALLTYPE get__BackColor( 
            /* [retval][out] */ long *BackColor) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][propget] */ HRESULT STDMETHODCALLTYPE get__BorderColor( 
            /* [retval][out] */ long *BorderColor) = 0;
        
        virtual /* [defaultbind][displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE put_Visible( 
            /* [in] */ VARIANT_BOOL Visible) = 0;
        
        virtual /* [defaultbind][displaybind][bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_Visible( 
            /* [retval][out] */ VARIANT_BOOL *Visible) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_FontName( 
            /* [in] */ BSTR FontName) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_FontSize( 
            /* [in] */ long FontSize) = 0;
        
        virtual /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE put_FontCharSet( 
            /* [in] */ short FontCharSet) = 0;
        
        virtual /* [displaybind][bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_FontCharSet( 
            /* [retval][out] */ short *FontCharSet) = 0;
        
        virtual /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE put_Style( 
            /* [in] */ long Style) = 0;
        
        virtual /* [displaybind][bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_Style( 
            /* [retval][out] */ long *Style) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlBalloonVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlBalloon * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlBalloon * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlBalloon * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlBalloon * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlBalloon * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlBalloon * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlBalloon * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ VARIANT_BOOL *Enabled);
        
        /* [nonbrowsable][restricted][hidden][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NumberOfLines )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ long *Lines);
        
        /* [nonbrowsable][restricted][hidden][propget] */ HRESULT ( STDMETHODCALLTYPE *get__CharsPerLine )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ long *CharsPerLine);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_FontName )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ BSTR *FontName);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_FontSize )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ long *FontSize);
        
        /* [nonbrowsable][restricted][hidden][propget] */ HRESULT ( STDMETHODCALLTYPE *get__FontBold )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ VARIANT_BOOL *FontBold);
        
        /* [nonbrowsable][restricted][hidden][propget] */ HRESULT ( STDMETHODCALLTYPE *get__FontItalic )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ VARIANT_BOOL *FontItalic);
        
        /* [nonbrowsable][restricted][hidden][propget] */ HRESULT ( STDMETHODCALLTYPE *get__FontStrikethru )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ VARIANT_BOOL *FontStrikethru);
        
        /* [nonbrowsable][restricted][hidden][propget] */ HRESULT ( STDMETHODCALLTYPE *get__FontUnderline )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ VARIANT_BOOL *FontUnderline);
        
        /* [nonbrowsable][hidden][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_ForeColor )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ long *ForeColor);
        
        /* [nonbrowsable][restricted][hidden][propget] */ HRESULT ( STDMETHODCALLTYPE *get__BackColor )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ long *BackColor);
        
        /* [nonbrowsable][restricted][hidden][propget] */ HRESULT ( STDMETHODCALLTYPE *get__BorderColor )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ long *BorderColor);
        
        /* [defaultbind][displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IDaCtlBalloon * This,
            /* [in] */ VARIANT_BOOL Visible);
        
        /* [defaultbind][displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ VARIANT_BOOL *Visible);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FontName )( 
            IDaCtlBalloon * This,
            /* [in] */ BSTR FontName);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FontSize )( 
            IDaCtlBalloon * This,
            /* [in] */ long FontSize);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_FontCharSet )( 
            IDaCtlBalloon * This,
            /* [in] */ short FontCharSet);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_FontCharSet )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ short *FontCharSet);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_Style )( 
            IDaCtlBalloon * This,
            /* [in] */ long Style);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Style )( 
            IDaCtlBalloon * This,
            /* [retval][out] */ long *Style);
        
        END_INTERFACE
    } IDaCtlBalloonVtbl;

    interface IDaCtlBalloon
    {
        CONST_VTBL struct IDaCtlBalloonVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlBalloon_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlBalloon_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlBalloon_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlBalloon_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlBalloon_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlBalloon_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlBalloon_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlBalloon_get_Enabled(This,Enabled)	\
    ( (This)->lpVtbl -> get_Enabled(This,Enabled) ) 

#define IDaCtlBalloon_get__NumberOfLines(This,Lines)	\
    ( (This)->lpVtbl -> get__NumberOfLines(This,Lines) ) 

#define IDaCtlBalloon_get__CharsPerLine(This,CharsPerLine)	\
    ( (This)->lpVtbl -> get__CharsPerLine(This,CharsPerLine) ) 

#define IDaCtlBalloon_get_FontName(This,FontName)	\
    ( (This)->lpVtbl -> get_FontName(This,FontName) ) 

#define IDaCtlBalloon_get_FontSize(This,FontSize)	\
    ( (This)->lpVtbl -> get_FontSize(This,FontSize) ) 

#define IDaCtlBalloon_get__FontBold(This,FontBold)	\
    ( (This)->lpVtbl -> get__FontBold(This,FontBold) ) 

#define IDaCtlBalloon_get__FontItalic(This,FontItalic)	\
    ( (This)->lpVtbl -> get__FontItalic(This,FontItalic) ) 

#define IDaCtlBalloon_get__FontStrikethru(This,FontStrikethru)	\
    ( (This)->lpVtbl -> get__FontStrikethru(This,FontStrikethru) ) 

#define IDaCtlBalloon_get__FontUnderline(This,FontUnderline)	\
    ( (This)->lpVtbl -> get__FontUnderline(This,FontUnderline) ) 

#define IDaCtlBalloon_get_ForeColor(This,ForeColor)	\
    ( (This)->lpVtbl -> get_ForeColor(This,ForeColor) ) 

#define IDaCtlBalloon_get__BackColor(This,BackColor)	\
    ( (This)->lpVtbl -> get__BackColor(This,BackColor) ) 

#define IDaCtlBalloon_get__BorderColor(This,BorderColor)	\
    ( (This)->lpVtbl -> get__BorderColor(This,BorderColor) ) 

#define IDaCtlBalloon_put_Visible(This,Visible)	\
    ( (This)->lpVtbl -> put_Visible(This,Visible) ) 

#define IDaCtlBalloon_get_Visible(This,Visible)	\
    ( (This)->lpVtbl -> get_Visible(This,Visible) ) 

#define IDaCtlBalloon_put_FontName(This,FontName)	\
    ( (This)->lpVtbl -> put_FontName(This,FontName) ) 

#define IDaCtlBalloon_put_FontSize(This,FontSize)	\
    ( (This)->lpVtbl -> put_FontSize(This,FontSize) ) 

#define IDaCtlBalloon_put_FontCharSet(This,FontCharSet)	\
    ( (This)->lpVtbl -> put_FontCharSet(This,FontCharSet) ) 

#define IDaCtlBalloon_get_FontCharSet(This,FontCharSet)	\
    ( (This)->lpVtbl -> get_FontCharSet(This,FontCharSet) ) 

#define IDaCtlBalloon_put_Style(This,Style)	\
    ( (This)->lpVtbl -> put_Style(This,Style) ) 

#define IDaCtlBalloon_get_Style(This,Style)	\
    ( (This)->lpVtbl -> get_Style(This,Style) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlBalloon_INTERFACE_DEFINED__ */


#ifndef __IDaCtlCommands_INTERFACE_DEFINED__
#define __IDaCtlCommands_INTERFACE_DEFINED__

/* interface IDaCtlCommands */
/* [object][custom][helpcontext][hidden][nonextensible][oleautomation][dual][unique][uuid] */ 


EXTERN_C const IID IID_IDaCtlCommands;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E540-A208-11DE-ABF2-002421116FB2")
    IDaCtlCommands : public IDispatch
    {
    public:
        virtual /* [propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ BSTR Name,
            /* [retval][out] */ IDaCtlCommand2 **Item) = 0;
        
        virtual /* [hidden][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE Command( 
            /* [in] */ BSTR Name,
            /* [retval][out] */ IDaCtlCommand2 **Command) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *Count) = 0;
        
        virtual /* [defaultbind][displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Caption( 
            /* [retval][out] */ BSTR *Caption) = 0;
        
        virtual /* [defaultbind][displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_Caption( 
            /* [in] */ BSTR Caption) = 0;
        
        virtual /* [nonbrowsable][hidden][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Voice( 
            /* [retval][out] */ BSTR *Voice) = 0;
        
        virtual /* [nonbrowsable][hidden][propput][id] */ HRESULT STDMETHODCALLTYPE put_Voice( 
            /* [in] */ BSTR Voice) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Visible( 
            /* [retval][out] */ VARIANT_BOOL *Visible) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_Visible( 
            /* [in] */ VARIANT_BOOL Visible) = 0;
        
        virtual /* [restricted][propget][id] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **EnumVariant) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ BSTR Name,
            /* [optional][in] */ VARIANT Caption,
            /* [optional][in] */ VARIANT VoiceGrammar,
            /* [optional][in] */ VARIANT Enabled,
            /* [optional][in] */ VARIANT Visible,
            /* [retval][out] */ IDaCtlCommand2 **Command) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE Insert( 
            /* [in] */ BSTR Name,
            /* [in] */ BSTR RefName,
            /* [optional][in] */ VARIANT Before,
            /* [optional][in] */ VARIANT Caption,
            /* [optional][in] */ VARIANT VoiceGrammar,
            /* [optional][in] */ VARIANT Enabled,
            /* [optional][in] */ VARIANT Visible,
            /* [retval][out] */ IDaCtlCommand2 **Command) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE Remove( 
            /* [in] */ BSTR Name) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE RemoveAll( void) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_DefaultCommand( 
            /* [retval][out] */ BSTR *Name) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_DefaultCommand( 
            /* [in] */ BSTR Name) = 0;
        
        virtual /* [nonbrowsable][hidden][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_HelpContextID( 
            /* [retval][out] */ long *ID) = 0;
        
        virtual /* [nonbrowsable][hidden][propput][id] */ HRESULT STDMETHODCALLTYPE put_HelpContextID( 
            /* [in] */ long ID) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_FontName( 
            /* [retval][out] */ BSTR *FontName) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_FontName( 
            /* [in] */ BSTR FontName) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_FontSize( 
            /* [retval][out] */ long *FontSize) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_FontSize( 
            /* [in] */ long FontSize) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_VoiceCaption( 
            /* [retval][out] */ BSTR *VoiceCaption) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_VoiceCaption( 
            /* [in] */ BSTR VoiceCaption) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_GlobalVoiceCommandsEnabled( 
            /* [retval][out] */ VARIANT_BOOL *Enabled) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_GlobalVoiceCommandsEnabled( 
            /* [in] */ VARIANT_BOOL Enabled) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlCommandsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlCommands * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlCommands * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlCommands * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlCommands * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlCommands * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlCommands * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlCommands * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IDaCtlCommands * This,
            /* [in] */ BSTR Name,
            /* [retval][out] */ IDaCtlCommand2 **Item);
        
        /* [hidden][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Command )( 
            IDaCtlCommands * This,
            /* [in] */ BSTR Name,
            /* [retval][out] */ IDaCtlCommand2 **Command);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IDaCtlCommands * This,
            /* [retval][out] */ long *Count);
        
        /* [defaultbind][displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Caption )( 
            IDaCtlCommands * This,
            /* [retval][out] */ BSTR *Caption);
        
        /* [defaultbind][displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Caption )( 
            IDaCtlCommands * This,
            /* [in] */ BSTR Caption);
        
        /* [nonbrowsable][hidden][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Voice )( 
            IDaCtlCommands * This,
            /* [retval][out] */ BSTR *Voice);
        
        /* [nonbrowsable][hidden][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Voice )( 
            IDaCtlCommands * This,
            /* [in] */ BSTR Voice);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IDaCtlCommands * This,
            /* [retval][out] */ VARIANT_BOOL *Visible);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IDaCtlCommands * This,
            /* [in] */ VARIANT_BOOL Visible);
        
        /* [restricted][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IDaCtlCommands * This,
            /* [retval][out] */ IUnknown **EnumVariant);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            IDaCtlCommands * This,
            /* [in] */ BSTR Name,
            /* [optional][in] */ VARIANT Caption,
            /* [optional][in] */ VARIANT VoiceGrammar,
            /* [optional][in] */ VARIANT Enabled,
            /* [optional][in] */ VARIANT Visible,
            /* [retval][out] */ IDaCtlCommand2 **Command);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Insert )( 
            IDaCtlCommands * This,
            /* [in] */ BSTR Name,
            /* [in] */ BSTR RefName,
            /* [optional][in] */ VARIANT Before,
            /* [optional][in] */ VARIANT Caption,
            /* [optional][in] */ VARIANT VoiceGrammar,
            /* [optional][in] */ VARIANT Enabled,
            /* [optional][in] */ VARIANT Visible,
            /* [retval][out] */ IDaCtlCommand2 **Command);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Remove )( 
            IDaCtlCommands * This,
            /* [in] */ BSTR Name);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *RemoveAll )( 
            IDaCtlCommands * This);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_DefaultCommand )( 
            IDaCtlCommands * This,
            /* [retval][out] */ BSTR *Name);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_DefaultCommand )( 
            IDaCtlCommands * This,
            /* [in] */ BSTR Name);
        
        /* [nonbrowsable][hidden][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_HelpContextID )( 
            IDaCtlCommands * This,
            /* [retval][out] */ long *ID);
        
        /* [nonbrowsable][hidden][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_HelpContextID )( 
            IDaCtlCommands * This,
            /* [in] */ long ID);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_FontName )( 
            IDaCtlCommands * This,
            /* [retval][out] */ BSTR *FontName);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FontName )( 
            IDaCtlCommands * This,
            /* [in] */ BSTR FontName);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_FontSize )( 
            IDaCtlCommands * This,
            /* [retval][out] */ long *FontSize);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FontSize )( 
            IDaCtlCommands * This,
            /* [in] */ long FontSize);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_VoiceCaption )( 
            IDaCtlCommands * This,
            /* [retval][out] */ BSTR *VoiceCaption);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_VoiceCaption )( 
            IDaCtlCommands * This,
            /* [in] */ BSTR VoiceCaption);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_GlobalVoiceCommandsEnabled )( 
            IDaCtlCommands * This,
            /* [retval][out] */ VARIANT_BOOL *Enabled);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_GlobalVoiceCommandsEnabled )( 
            IDaCtlCommands * This,
            /* [in] */ VARIANT_BOOL Enabled);
        
        END_INTERFACE
    } IDaCtlCommandsVtbl;

    interface IDaCtlCommands
    {
        CONST_VTBL struct IDaCtlCommandsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlCommands_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlCommands_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlCommands_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlCommands_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlCommands_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlCommands_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlCommands_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlCommands_get_Item(This,Name,Item)	\
    ( (This)->lpVtbl -> get_Item(This,Name,Item) ) 

#define IDaCtlCommands_Command(This,Name,Command)	\
    ( (This)->lpVtbl -> Command(This,Name,Command) ) 

#define IDaCtlCommands_get_Count(This,Count)	\
    ( (This)->lpVtbl -> get_Count(This,Count) ) 

#define IDaCtlCommands_get_Caption(This,Caption)	\
    ( (This)->lpVtbl -> get_Caption(This,Caption) ) 

#define IDaCtlCommands_put_Caption(This,Caption)	\
    ( (This)->lpVtbl -> put_Caption(This,Caption) ) 

#define IDaCtlCommands_get_Voice(This,Voice)	\
    ( (This)->lpVtbl -> get_Voice(This,Voice) ) 

#define IDaCtlCommands_put_Voice(This,Voice)	\
    ( (This)->lpVtbl -> put_Voice(This,Voice) ) 

#define IDaCtlCommands_get_Visible(This,Visible)	\
    ( (This)->lpVtbl -> get_Visible(This,Visible) ) 

#define IDaCtlCommands_put_Visible(This,Visible)	\
    ( (This)->lpVtbl -> put_Visible(This,Visible) ) 

#define IDaCtlCommands_get__NewEnum(This,EnumVariant)	\
    ( (This)->lpVtbl -> get__NewEnum(This,EnumVariant) ) 

#define IDaCtlCommands_Add(This,Name,Caption,VoiceGrammar,Enabled,Visible,Command)	\
    ( (This)->lpVtbl -> Add(This,Name,Caption,VoiceGrammar,Enabled,Visible,Command) ) 

#define IDaCtlCommands_Insert(This,Name,RefName,Before,Caption,VoiceGrammar,Enabled,Visible,Command)	\
    ( (This)->lpVtbl -> Insert(This,Name,RefName,Before,Caption,VoiceGrammar,Enabled,Visible,Command) ) 

#define IDaCtlCommands_Remove(This,Name)	\
    ( (This)->lpVtbl -> Remove(This,Name) ) 

#define IDaCtlCommands_RemoveAll(This)	\
    ( (This)->lpVtbl -> RemoveAll(This) ) 

#define IDaCtlCommands_get_DefaultCommand(This,Name)	\
    ( (This)->lpVtbl -> get_DefaultCommand(This,Name) ) 

#define IDaCtlCommands_put_DefaultCommand(This,Name)	\
    ( (This)->lpVtbl -> put_DefaultCommand(This,Name) ) 

#define IDaCtlCommands_get_HelpContextID(This,ID)	\
    ( (This)->lpVtbl -> get_HelpContextID(This,ID) ) 

#define IDaCtlCommands_put_HelpContextID(This,ID)	\
    ( (This)->lpVtbl -> put_HelpContextID(This,ID) ) 

#define IDaCtlCommands_get_FontName(This,FontName)	\
    ( (This)->lpVtbl -> get_FontName(This,FontName) ) 

#define IDaCtlCommands_put_FontName(This,FontName)	\
    ( (This)->lpVtbl -> put_FontName(This,FontName) ) 

#define IDaCtlCommands_get_FontSize(This,FontSize)	\
    ( (This)->lpVtbl -> get_FontSize(This,FontSize) ) 

#define IDaCtlCommands_put_FontSize(This,FontSize)	\
    ( (This)->lpVtbl -> put_FontSize(This,FontSize) ) 

#define IDaCtlCommands_get_VoiceCaption(This,VoiceCaption)	\
    ( (This)->lpVtbl -> get_VoiceCaption(This,VoiceCaption) ) 

#define IDaCtlCommands_put_VoiceCaption(This,VoiceCaption)	\
    ( (This)->lpVtbl -> put_VoiceCaption(This,VoiceCaption) ) 

#define IDaCtlCommands_get_GlobalVoiceCommandsEnabled(This,Enabled)	\
    ( (This)->lpVtbl -> get_GlobalVoiceCommandsEnabled(This,Enabled) ) 

#define IDaCtlCommands_put_GlobalVoiceCommandsEnabled(This,Enabled)	\
    ( (This)->lpVtbl -> put_GlobalVoiceCommandsEnabled(This,Enabled) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlCommands_INTERFACE_DEFINED__ */


#ifndef __IDaCtlCommand_INTERFACE_DEFINED__
#define __IDaCtlCommand_INTERFACE_DEFINED__

/* interface IDaCtlCommand */
/* [object][helpcontext][hidden][nonextensible][oleautomation][dual][unique][uuid] */ 


EXTERN_C const IID IID_IDaCtlCommand;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E541-A208-11DE-ABF2-002421116FB2")
    IDaCtlCommand : public IDispatch
    {
    public:
        virtual /* [nonbrowsable][hidden][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Voice( 
            /* [retval][out] */ BSTR *Voice) = 0;
        
        virtual /* [nonbrowsable][hidden][propput][id] */ HRESULT STDMETHODCALLTYPE put_Voice( 
            /* [in] */ BSTR Voice) = 0;
        
        virtual /* [defaultbind][displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Caption( 
            /* [retval][out] */ BSTR *Caption) = 0;
        
        virtual /* [defaultbind][displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_Caption( 
            /* [in] */ BSTR Caption) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Enabled( 
            /* [retval][out] */ VARIANT_BOOL *Enabled) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_Enabled( 
            /* [in] */ VARIANT_BOOL Enabled) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Visible( 
            /* [retval][out] */ VARIANT_BOOL *Visible) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_Visible( 
            /* [in] */ VARIANT_BOOL Visible) = 0;
        
        virtual /* [nonbrowsable][hidden][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Confidence( 
            /* [retval][out] */ long *Confidence) = 0;
        
        virtual /* [nonbrowsable][hidden][propput][id] */ HRESULT STDMETHODCALLTYPE put_Confidence( 
            /* [in] */ long Confidence) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_ConfidenceText( 
            /* [retval][out] */ BSTR *Text) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_ConfidenceText( 
            /* [in] */ BSTR Text) = 0;
        
        virtual /* [nonbrowsable][hidden][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_HelpContextID( 
            /* [retval][out] */ long *ID) = 0;
        
        virtual /* [nonbrowsable][hidden][propput][id] */ HRESULT STDMETHODCALLTYPE put_HelpContextID( 
            /* [in] */ long ID) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_VoiceCaption( 
            /* [retval][out] */ BSTR *VoiceCaption) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_VoiceCaption( 
            /* [in] */ BSTR VoiceCaption) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlCommandVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlCommand * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlCommand * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlCommand * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlCommand * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlCommand * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlCommand * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlCommand * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [nonbrowsable][hidden][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Voice )( 
            IDaCtlCommand * This,
            /* [retval][out] */ BSTR *Voice);
        
        /* [nonbrowsable][hidden][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Voice )( 
            IDaCtlCommand * This,
            /* [in] */ BSTR Voice);
        
        /* [defaultbind][displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Caption )( 
            IDaCtlCommand * This,
            /* [retval][out] */ BSTR *Caption);
        
        /* [defaultbind][displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Caption )( 
            IDaCtlCommand * This,
            /* [in] */ BSTR Caption);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IDaCtlCommand * This,
            /* [retval][out] */ VARIANT_BOOL *Enabled);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            IDaCtlCommand * This,
            /* [in] */ VARIANT_BOOL Enabled);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IDaCtlCommand * This,
            /* [retval][out] */ VARIANT_BOOL *Visible);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IDaCtlCommand * This,
            /* [in] */ VARIANT_BOOL Visible);
        
        /* [nonbrowsable][hidden][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Confidence )( 
            IDaCtlCommand * This,
            /* [retval][out] */ long *Confidence);
        
        /* [nonbrowsable][hidden][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Confidence )( 
            IDaCtlCommand * This,
            /* [in] */ long Confidence);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_ConfidenceText )( 
            IDaCtlCommand * This,
            /* [retval][out] */ BSTR *Text);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ConfidenceText )( 
            IDaCtlCommand * This,
            /* [in] */ BSTR Text);
        
        /* [nonbrowsable][hidden][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_HelpContextID )( 
            IDaCtlCommand * This,
            /* [retval][out] */ long *ID);
        
        /* [nonbrowsable][hidden][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_HelpContextID )( 
            IDaCtlCommand * This,
            /* [in] */ long ID);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_VoiceCaption )( 
            IDaCtlCommand * This,
            /* [retval][out] */ BSTR *VoiceCaption);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_VoiceCaption )( 
            IDaCtlCommand * This,
            /* [in] */ BSTR VoiceCaption);
        
        END_INTERFACE
    } IDaCtlCommandVtbl;

    interface IDaCtlCommand
    {
        CONST_VTBL struct IDaCtlCommandVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlCommand_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlCommand_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlCommand_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlCommand_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlCommand_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlCommand_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlCommand_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlCommand_get_Voice(This,Voice)	\
    ( (This)->lpVtbl -> get_Voice(This,Voice) ) 

#define IDaCtlCommand_put_Voice(This,Voice)	\
    ( (This)->lpVtbl -> put_Voice(This,Voice) ) 

#define IDaCtlCommand_get_Caption(This,Caption)	\
    ( (This)->lpVtbl -> get_Caption(This,Caption) ) 

#define IDaCtlCommand_put_Caption(This,Caption)	\
    ( (This)->lpVtbl -> put_Caption(This,Caption) ) 

#define IDaCtlCommand_get_Enabled(This,Enabled)	\
    ( (This)->lpVtbl -> get_Enabled(This,Enabled) ) 

#define IDaCtlCommand_put_Enabled(This,Enabled)	\
    ( (This)->lpVtbl -> put_Enabled(This,Enabled) ) 

#define IDaCtlCommand_get_Visible(This,Visible)	\
    ( (This)->lpVtbl -> get_Visible(This,Visible) ) 

#define IDaCtlCommand_put_Visible(This,Visible)	\
    ( (This)->lpVtbl -> put_Visible(This,Visible) ) 

#define IDaCtlCommand_get_Confidence(This,Confidence)	\
    ( (This)->lpVtbl -> get_Confidence(This,Confidence) ) 

#define IDaCtlCommand_put_Confidence(This,Confidence)	\
    ( (This)->lpVtbl -> put_Confidence(This,Confidence) ) 

#define IDaCtlCommand_get_ConfidenceText(This,Text)	\
    ( (This)->lpVtbl -> get_ConfidenceText(This,Text) ) 

#define IDaCtlCommand_put_ConfidenceText(This,Text)	\
    ( (This)->lpVtbl -> put_ConfidenceText(This,Text) ) 

#define IDaCtlCommand_get_HelpContextID(This,ID)	\
    ( (This)->lpVtbl -> get_HelpContextID(This,ID) ) 

#define IDaCtlCommand_put_HelpContextID(This,ID)	\
    ( (This)->lpVtbl -> put_HelpContextID(This,ID) ) 

#define IDaCtlCommand_get_VoiceCaption(This,VoiceCaption)	\
    ( (This)->lpVtbl -> get_VoiceCaption(This,VoiceCaption) ) 

#define IDaCtlCommand_put_VoiceCaption(This,VoiceCaption)	\
    ( (This)->lpVtbl -> put_VoiceCaption(This,VoiceCaption) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlCommand_INTERFACE_DEFINED__ */


#ifndef __IDaCtlRequest_INTERFACE_DEFINED__
#define __IDaCtlRequest_INTERFACE_DEFINED__

/* interface IDaCtlRequest */
/* [object][helpcontext][nonextensible][oleautomation][dual][unique][uuid] */ 


EXTERN_C const IID IID_IDaCtlRequest;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E542-A208-11DE-ABF2-002421116FB2")
    IDaCtlRequest : public IDispatch
    {
    public:
        virtual /* [hidden][readonly][propget][id] */ HRESULT STDMETHODCALLTYPE get_ID( 
            /* [retval][out] */ long *ID) = 0;
        
        virtual /* [defaultbind][displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Status( 
            /* [retval][out] */ long *Status) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Description( 
            /* [retval][out] */ BSTR *Description) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Number( 
            /* [retval][out] */ long *Number) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlRequestVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlRequest * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlRequest * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlRequest * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlRequest * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlRequest * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlRequest * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlRequest * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [hidden][readonly][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_ID )( 
            IDaCtlRequest * This,
            /* [retval][out] */ long *ID);
        
        /* [defaultbind][displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Status )( 
            IDaCtlRequest * This,
            /* [retval][out] */ long *Status);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Description )( 
            IDaCtlRequest * This,
            /* [retval][out] */ BSTR *Description);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Number )( 
            IDaCtlRequest * This,
            /* [retval][out] */ long *Number);
        
        END_INTERFACE
    } IDaCtlRequestVtbl;

    interface IDaCtlRequest
    {
        CONST_VTBL struct IDaCtlRequestVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlRequest_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlRequest_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlRequest_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlRequest_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlRequest_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlRequest_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlRequest_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlRequest_get_ID(This,ID)	\
    ( (This)->lpVtbl -> get_ID(This,ID) ) 

#define IDaCtlRequest_get_Status(This,Status)	\
    ( (This)->lpVtbl -> get_Status(This,Status) ) 

#define IDaCtlRequest_get_Description(This,Description)	\
    ( (This)->lpVtbl -> get_Description(This,Description) ) 

#define IDaCtlRequest_get_Number(This,Number)	\
    ( (This)->lpVtbl -> get_Number(This,Number) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlRequest_INTERFACE_DEFINED__ */


#ifndef __IDaCtlAudioOutput_INTERFACE_DEFINED__
#define __IDaCtlAudioOutput_INTERFACE_DEFINED__

/* interface IDaCtlAudioOutput */
/* [object][helpcontext][hidden][nonextensible][oleautomation][dual][unique][uuid] */ 


EXTERN_C const IID IID_IDaCtlAudioOutput;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E543-A208-11DE-ABF2-002421116FB2")
    IDaCtlAudioOutput : public IDispatch
    {
    public:
        virtual /* [propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Enabled( 
            /* [retval][out] */ VARIANT_BOOL *AudioEnabled) = 0;
        
        virtual /* [propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_SoundEffects( 
            /* [retval][out] */ VARIANT_BOOL *SoundEffects) = 0;
        
        virtual /* [propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Status( 
            /* [retval][out] */ short *Available) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlAudioOutputVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlAudioOutput * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlAudioOutput * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlAudioOutput * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlAudioOutput * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlAudioOutput * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlAudioOutput * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlAudioOutput * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IDaCtlAudioOutput * This,
            /* [retval][out] */ VARIANT_BOOL *AudioEnabled);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SoundEffects )( 
            IDaCtlAudioOutput * This,
            /* [retval][out] */ VARIANT_BOOL *SoundEffects);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Status )( 
            IDaCtlAudioOutput * This,
            /* [retval][out] */ short *Available);
        
        END_INTERFACE
    } IDaCtlAudioOutputVtbl;

    interface IDaCtlAudioOutput
    {
        CONST_VTBL struct IDaCtlAudioOutputVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlAudioOutput_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlAudioOutput_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlAudioOutput_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlAudioOutput_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlAudioOutput_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlAudioOutput_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlAudioOutput_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlAudioOutput_get_Enabled(This,AudioEnabled)	\
    ( (This)->lpVtbl -> get_Enabled(This,AudioEnabled) ) 

#define IDaCtlAudioOutput_get_SoundEffects(This,SoundEffects)	\
    ( (This)->lpVtbl -> get_SoundEffects(This,SoundEffects) ) 

#define IDaCtlAudioOutput_get_Status(This,Available)	\
    ( (This)->lpVtbl -> get_Status(This,Available) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlAudioOutput_INTERFACE_DEFINED__ */


#ifndef __IDaCtlSpeechInput_INTERFACE_DEFINED__
#define __IDaCtlSpeechInput_INTERFACE_DEFINED__

/* interface IDaCtlSpeechInput */
/* [object][helpcontext][hidden][nonextensible][oleautomation][dual][unique][uuid] */ 


EXTERN_C const IID IID_IDaCtlSpeechInput;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E544-A208-11DE-ABF2-002421116FB2")
    IDaCtlSpeechInput : public IDispatch
    {
    public:
        virtual /* [propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Enabled( 
            /* [retval][out] */ VARIANT_BOOL *VoiceEnabled) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Language( 
            /* [retval][out] */ BSTR *Language) = 0;
        
        virtual /* [propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_HotKey( 
            /* [retval][out] */ BSTR *HotKey) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Installed( 
            /* [retval][out] */ VARIANT_BOOL *VoiceInstalled) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Engine( 
            /* [retval][out] */ BSTR *Engine) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][propput][id] */ HRESULT STDMETHODCALLTYPE put_Engine( 
            /* [in] */ BSTR Engine) = 0;
        
        virtual /* [propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_ListeningTip( 
            /* [retval][out] */ VARIANT_BOOL *ListeningTip) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlSpeechInputVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlSpeechInput * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlSpeechInput * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlSpeechInput * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlSpeechInput * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlSpeechInput * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlSpeechInput * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlSpeechInput * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IDaCtlSpeechInput * This,
            /* [retval][out] */ VARIANT_BOOL *VoiceEnabled);
        
        /* [nonbrowsable][restricted][hidden][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Language )( 
            IDaCtlSpeechInput * This,
            /* [retval][out] */ BSTR *Language);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_HotKey )( 
            IDaCtlSpeechInput * This,
            /* [retval][out] */ BSTR *HotKey);
        
        /* [nonbrowsable][restricted][hidden][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Installed )( 
            IDaCtlSpeechInput * This,
            /* [retval][out] */ VARIANT_BOOL *VoiceInstalled);
        
        /* [nonbrowsable][restricted][hidden][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Engine )( 
            IDaCtlSpeechInput * This,
            /* [retval][out] */ BSTR *Engine);
        
        /* [nonbrowsable][restricted][hidden][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Engine )( 
            IDaCtlSpeechInput * This,
            /* [in] */ BSTR Engine);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_ListeningTip )( 
            IDaCtlSpeechInput * This,
            /* [retval][out] */ VARIANT_BOOL *ListeningTip);
        
        END_INTERFACE
    } IDaCtlSpeechInputVtbl;

    interface IDaCtlSpeechInput
    {
        CONST_VTBL struct IDaCtlSpeechInputVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlSpeechInput_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlSpeechInput_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlSpeechInput_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlSpeechInput_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlSpeechInput_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlSpeechInput_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlSpeechInput_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlSpeechInput_get_Enabled(This,VoiceEnabled)	\
    ( (This)->lpVtbl -> get_Enabled(This,VoiceEnabled) ) 

#define IDaCtlSpeechInput_get_Language(This,Language)	\
    ( (This)->lpVtbl -> get_Language(This,Language) ) 

#define IDaCtlSpeechInput_get_HotKey(This,HotKey)	\
    ( (This)->lpVtbl -> get_HotKey(This,HotKey) ) 

#define IDaCtlSpeechInput_get_Installed(This,VoiceInstalled)	\
    ( (This)->lpVtbl -> get_Installed(This,VoiceInstalled) ) 

#define IDaCtlSpeechInput_get_Engine(This,Engine)	\
    ( (This)->lpVtbl -> get_Engine(This,Engine) ) 

#define IDaCtlSpeechInput_put_Engine(This,Engine)	\
    ( (This)->lpVtbl -> put_Engine(This,Engine) ) 

#define IDaCtlSpeechInput_get_ListeningTip(This,ListeningTip)	\
    ( (This)->lpVtbl -> get_ListeningTip(This,ListeningTip) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlSpeechInput_INTERFACE_DEFINED__ */


#ifndef __IDaCtlPropertySheet_INTERFACE_DEFINED__
#define __IDaCtlPropertySheet_INTERFACE_DEFINED__

/* interface IDaCtlPropertySheet */
/* [object][helpcontext][hidden][nonextensible][oleautomation][dual][unique][uuid] */ 


EXTERN_C const IID IID_IDaCtlPropertySheet;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E545-A208-11DE-ABF2-002421116FB2")
    IDaCtlPropertySheet : public IDispatch
    {
    public:
        virtual /* [nonbrowsable][restricted][hidden][propget] */ HRESULT STDMETHODCALLTYPE get__Left( 
            /* [retval][out] */ short *Left) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][propget] */ HRESULT STDMETHODCALLTYPE get__Top( 
            /* [retval][out] */ short *Top) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Height( 
            /* [retval][out] */ short *Height) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Width( 
            /* [retval][out] */ short *Width) = 0;
        
        virtual /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE put_Visible( 
            /* [in] */ VARIANT_BOOL Visible) = 0;
        
        virtual /* [displaybind][bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_Visible( 
            /* [retval][out] */ VARIANT_BOOL *Visible) = 0;
        
        virtual /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE put_Page( 
            /* [in] */ BSTR Page) = 0;
        
        virtual /* [displaybind][bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_Page( 
            /* [retval][out] */ BSTR *Page) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlPropertySheetVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlPropertySheet * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlPropertySheet * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlPropertySheet * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlPropertySheet * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlPropertySheet * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlPropertySheet * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlPropertySheet * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [nonbrowsable][restricted][hidden][propget] */ HRESULT ( STDMETHODCALLTYPE *get__Left )( 
            IDaCtlPropertySheet * This,
            /* [retval][out] */ short *Left);
        
        /* [nonbrowsable][restricted][hidden][propget] */ HRESULT ( STDMETHODCALLTYPE *get__Top )( 
            IDaCtlPropertySheet * This,
            /* [retval][out] */ short *Top);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IDaCtlPropertySheet * This,
            /* [retval][out] */ short *Height);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IDaCtlPropertySheet * This,
            /* [retval][out] */ short *Width);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IDaCtlPropertySheet * This,
            /* [in] */ VARIANT_BOOL Visible);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IDaCtlPropertySheet * This,
            /* [retval][out] */ VARIANT_BOOL *Visible);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_Page )( 
            IDaCtlPropertySheet * This,
            /* [in] */ BSTR Page);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Page )( 
            IDaCtlPropertySheet * This,
            /* [retval][out] */ BSTR *Page);
        
        END_INTERFACE
    } IDaCtlPropertySheetVtbl;

    interface IDaCtlPropertySheet
    {
        CONST_VTBL struct IDaCtlPropertySheetVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlPropertySheet_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlPropertySheet_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlPropertySheet_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlPropertySheet_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlPropertySheet_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlPropertySheet_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlPropertySheet_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlPropertySheet_get__Left(This,Left)	\
    ( (This)->lpVtbl -> get__Left(This,Left) ) 

#define IDaCtlPropertySheet_get__Top(This,Top)	\
    ( (This)->lpVtbl -> get__Top(This,Top) ) 

#define IDaCtlPropertySheet_get_Height(This,Height)	\
    ( (This)->lpVtbl -> get_Height(This,Height) ) 

#define IDaCtlPropertySheet_get_Width(This,Width)	\
    ( (This)->lpVtbl -> get_Width(This,Width) ) 

#define IDaCtlPropertySheet_put_Visible(This,Visible)	\
    ( (This)->lpVtbl -> put_Visible(This,Visible) ) 

#define IDaCtlPropertySheet_get_Visible(This,Visible)	\
    ( (This)->lpVtbl -> get_Visible(This,Visible) ) 

#define IDaCtlPropertySheet_put_Page(This,Page)	\
    ( (This)->lpVtbl -> put_Page(This,Page) ) 

#define IDaCtlPropertySheet_get_Page(This,Page)	\
    ( (This)->lpVtbl -> get_Page(This,Page) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlPropertySheet_INTERFACE_DEFINED__ */


#ifndef __IDaCtlUserInput_INTERFACE_DEFINED__
#define __IDaCtlUserInput_INTERFACE_DEFINED__

/* interface IDaCtlUserInput */
/* [object][helpcontext][nonextensible][oleautomation][dual][unique][uuid] */ 


EXTERN_C const IID IID_IDaCtlUserInput;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E546-A208-11DE-ABF2-002421116FB2")
    IDaCtlUserInput : public IDispatch
    {
    public:
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ short *Count) = 0;
        
        virtual /* [defaultbind][displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *Name) = 0;
        
        virtual /* [bindable][displaybind][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_CharacterID( 
            /* [retval][out] */ BSTR *CharacterID) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Confidence( 
            /* [retval][out] */ long *Confidence) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Voice( 
            /* [retval][out] */ BSTR *Voice) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Alt1Name( 
            /* [retval][out] */ BSTR *Alt1Name) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Alt1Confidence( 
            /* [retval][out] */ long *Alt1Confidence) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Alt1Voice( 
            /* [retval][out] */ BSTR *Alt1Voice) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Alt2Name( 
            /* [retval][out] */ BSTR *Alt2Name) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Alt2Confidence( 
            /* [retval][out] */ long *Alt2Confidence) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Alt2Voice( 
            /* [retval][out] */ BSTR *Alt2Voice) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlUserInputVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlUserInput * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlUserInput * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlUserInput * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlUserInput * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlUserInput * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlUserInput * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlUserInput * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IDaCtlUserInput * This,
            /* [retval][out] */ short *Count);
        
        /* [defaultbind][displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IDaCtlUserInput * This,
            /* [retval][out] */ BSTR *Name);
        
        /* [bindable][displaybind][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_CharacterID )( 
            IDaCtlUserInput * This,
            /* [retval][out] */ BSTR *CharacterID);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Confidence )( 
            IDaCtlUserInput * This,
            /* [retval][out] */ long *Confidence);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Voice )( 
            IDaCtlUserInput * This,
            /* [retval][out] */ BSTR *Voice);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Alt1Name )( 
            IDaCtlUserInput * This,
            /* [retval][out] */ BSTR *Alt1Name);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Alt1Confidence )( 
            IDaCtlUserInput * This,
            /* [retval][out] */ long *Alt1Confidence);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Alt1Voice )( 
            IDaCtlUserInput * This,
            /* [retval][out] */ BSTR *Alt1Voice);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Alt2Name )( 
            IDaCtlUserInput * This,
            /* [retval][out] */ BSTR *Alt2Name);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Alt2Confidence )( 
            IDaCtlUserInput * This,
            /* [retval][out] */ long *Alt2Confidence);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Alt2Voice )( 
            IDaCtlUserInput * This,
            /* [retval][out] */ BSTR *Alt2Voice);
        
        END_INTERFACE
    } IDaCtlUserInputVtbl;

    interface IDaCtlUserInput
    {
        CONST_VTBL struct IDaCtlUserInputVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlUserInput_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlUserInput_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlUserInput_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlUserInput_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlUserInput_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlUserInput_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlUserInput_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlUserInput_get_Count(This,Count)	\
    ( (This)->lpVtbl -> get_Count(This,Count) ) 

#define IDaCtlUserInput_get_Name(This,Name)	\
    ( (This)->lpVtbl -> get_Name(This,Name) ) 

#define IDaCtlUserInput_get_CharacterID(This,CharacterID)	\
    ( (This)->lpVtbl -> get_CharacterID(This,CharacterID) ) 

#define IDaCtlUserInput_get_Confidence(This,Confidence)	\
    ( (This)->lpVtbl -> get_Confidence(This,Confidence) ) 

#define IDaCtlUserInput_get_Voice(This,Voice)	\
    ( (This)->lpVtbl -> get_Voice(This,Voice) ) 

#define IDaCtlUserInput_get_Alt1Name(This,Alt1Name)	\
    ( (This)->lpVtbl -> get_Alt1Name(This,Alt1Name) ) 

#define IDaCtlUserInput_get_Alt1Confidence(This,Alt1Confidence)	\
    ( (This)->lpVtbl -> get_Alt1Confidence(This,Alt1Confidence) ) 

#define IDaCtlUserInput_get_Alt1Voice(This,Alt1Voice)	\
    ( (This)->lpVtbl -> get_Alt1Voice(This,Alt1Voice) ) 

#define IDaCtlUserInput_get_Alt2Name(This,Alt2Name)	\
    ( (This)->lpVtbl -> get_Alt2Name(This,Alt2Name) ) 

#define IDaCtlUserInput_get_Alt2Confidence(This,Alt2Confidence)	\
    ( (This)->lpVtbl -> get_Alt2Confidence(This,Alt2Confidence) ) 

#define IDaCtlUserInput_get_Alt2Voice(This,Alt2Voice)	\
    ( (This)->lpVtbl -> get_Alt2Voice(This,Alt2Voice) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlUserInput_INTERFACE_DEFINED__ */


#ifndef __IDaCtlCommandsWindow_INTERFACE_DEFINED__
#define __IDaCtlCommandsWindow_INTERFACE_DEFINED__

/* interface IDaCtlCommandsWindow */
/* [object][helpcontext][nonextensible][oleautomation][dual][unique][uuid] */ 


EXTERN_C const IID IID_IDaCtlCommandsWindow;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E547-A208-11DE-ABF2-002421116FB2")
    IDaCtlCommandsWindow : public IDispatch
    {
    public:
        virtual /* [defaultbind][displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Visible( 
            /* [retval][out] */ VARIANT_BOOL *Visible) = 0;
        
        virtual /* [defaultbind][displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_Visible( 
            /* [in] */ VARIANT_BOOL Visible) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Left( 
            /* [retval][out] */ short *Left) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Top( 
            /* [retval][out] */ short *Top) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Height( 
            /* [retval][out] */ short *Height) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Width( 
            /* [retval][out] */ short *Width) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlCommandsWindowVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlCommandsWindow * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlCommandsWindow * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlCommandsWindow * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlCommandsWindow * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlCommandsWindow * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlCommandsWindow * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlCommandsWindow * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [defaultbind][displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IDaCtlCommandsWindow * This,
            /* [retval][out] */ VARIANT_BOOL *Visible);
        
        /* [defaultbind][displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IDaCtlCommandsWindow * This,
            /* [in] */ VARIANT_BOOL Visible);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Left )( 
            IDaCtlCommandsWindow * This,
            /* [retval][out] */ short *Left);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Top )( 
            IDaCtlCommandsWindow * This,
            /* [retval][out] */ short *Top);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IDaCtlCommandsWindow * This,
            /* [retval][out] */ short *Height);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IDaCtlCommandsWindow * This,
            /* [retval][out] */ short *Width);
        
        END_INTERFACE
    } IDaCtlCommandsWindowVtbl;

    interface IDaCtlCommandsWindow
    {
        CONST_VTBL struct IDaCtlCommandsWindowVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlCommandsWindow_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlCommandsWindow_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlCommandsWindow_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlCommandsWindow_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlCommandsWindow_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlCommandsWindow_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlCommandsWindow_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlCommandsWindow_get_Visible(This,Visible)	\
    ( (This)->lpVtbl -> get_Visible(This,Visible) ) 

#define IDaCtlCommandsWindow_put_Visible(This,Visible)	\
    ( (This)->lpVtbl -> put_Visible(This,Visible) ) 

#define IDaCtlCommandsWindow_get_Left(This,Left)	\
    ( (This)->lpVtbl -> get_Left(This,Left) ) 

#define IDaCtlCommandsWindow_get_Top(This,Top)	\
    ( (This)->lpVtbl -> get_Top(This,Top) ) 

#define IDaCtlCommandsWindow_get_Height(This,Height)	\
    ( (This)->lpVtbl -> get_Height(This,Height) ) 

#define IDaCtlCommandsWindow_get_Width(This,Width)	\
    ( (This)->lpVtbl -> get_Width(This,Width) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlCommandsWindow_INTERFACE_DEFINED__ */


#ifndef __IDaCtlAnimationNames_INTERFACE_DEFINED__
#define __IDaCtlAnimationNames_INTERFACE_DEFINED__

/* interface IDaCtlAnimationNames */
/* [object][helpcontext][hidden][nonextensible][oleautomation][dual][unique][uuid] */ 


EXTERN_C const IID IID_IDaCtlAnimationNames;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E548-A208-11DE-ABF2-002421116FB2")
    IDaCtlAnimationNames : public IDispatch
    {
    public:
        virtual /* [restricted][propget][id] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **EnumVariant) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlAnimationNamesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlAnimationNames * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlAnimationNames * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlAnimationNames * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlAnimationNames * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlAnimationNames * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlAnimationNames * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlAnimationNames * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [restricted][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IDaCtlAnimationNames * This,
            /* [retval][out] */ IUnknown **EnumVariant);
        
        END_INTERFACE
    } IDaCtlAnimationNamesVtbl;

    interface IDaCtlAnimationNames
    {
        CONST_VTBL struct IDaCtlAnimationNamesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlAnimationNames_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlAnimationNames_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlAnimationNames_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlAnimationNames_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlAnimationNames_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlAnimationNames_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlAnimationNames_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlAnimationNames_get__NewEnum(This,EnumVariant)	\
    ( (This)->lpVtbl -> get__NewEnum(This,EnumVariant) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlAnimationNames_INTERFACE_DEFINED__ */


#ifndef __IDaControl_INTERFACE_DEFINED__
#define __IDaControl_INTERFACE_DEFINED__

/* interface IDaControl */
/* [object][helpcontext][hidden][nonextensible][oleautomation][dual][unique][uuid] */ 


EXTERN_C const IID IID_IDaControl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E549-A208-11DE-ABF2-002421116FB2")
    IDaControl : public IDispatch
    {
    public:
        virtual /* [propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Characters( 
            /* [retval][out] */ IDaCtlCharacters2 **Characters) = 0;
        
        virtual /* [nonbrowsable][hidden][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_AudioOutput( 
            /* [retval][out] */ IDaCtlAudioOutput **AudioOutput) = 0;
        
        virtual /* [nonbrowsable][hidden][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_SpeechInput( 
            /* [retval][out] */ IDaCtlSpeechInput **SpeechInput) = 0;
        
        virtual /* [propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_PropertySheet( 
            /* [retval][out] */ IDaCtlPropertySheet2 **PropSheet) = 0;
        
        virtual /* [propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_CommandsWindow( 
            /* [retval][out] */ IDaCtlCommandsWindow **CommandsWindow) = 0;
        
        virtual /* [bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Connected( 
            /* [retval][out] */ VARIANT_BOOL *Connected) = 0;
        
        virtual /* [bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_Connected( 
            VARIANT_BOOL Connected) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Suspended( 
            /* [retval][out] */ VARIANT_BOOL *Suspended) = 0;
        
        virtual /* [hidden][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE ShowDefaultCharacterProperties( 
            /* [optional][in] */ VARIANT X,
            /* [optional][in] */ VARIANT y) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_RaiseRequestErrors( 
            /* [retval][out] */ VARIANT_BOOL *RaiseErrors) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_RaiseRequestErrors( 
            /* [in] */ VARIANT_BOOL RaiseErrors) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaControlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaControl * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaControl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaControl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaControl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaControl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaControl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaControl * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Characters )( 
            IDaControl * This,
            /* [retval][out] */ IDaCtlCharacters2 **Characters);
        
        /* [nonbrowsable][hidden][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_AudioOutput )( 
            IDaControl * This,
            /* [retval][out] */ IDaCtlAudioOutput **AudioOutput);
        
        /* [nonbrowsable][hidden][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SpeechInput )( 
            IDaControl * This,
            /* [retval][out] */ IDaCtlSpeechInput **SpeechInput);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_PropertySheet )( 
            IDaControl * This,
            /* [retval][out] */ IDaCtlPropertySheet2 **PropSheet);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_CommandsWindow )( 
            IDaControl * This,
            /* [retval][out] */ IDaCtlCommandsWindow **CommandsWindow);
        
        /* [bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Connected )( 
            IDaControl * This,
            /* [retval][out] */ VARIANT_BOOL *Connected);
        
        /* [bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Connected )( 
            IDaControl * This,
            VARIANT_BOOL Connected);
        
        /* [nonbrowsable][restricted][hidden][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Suspended )( 
            IDaControl * This,
            /* [retval][out] */ VARIANT_BOOL *Suspended);
        
        /* [hidden][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *ShowDefaultCharacterProperties )( 
            IDaControl * This,
            /* [optional][in] */ VARIANT X,
            /* [optional][in] */ VARIANT y);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_RaiseRequestErrors )( 
            IDaControl * This,
            /* [retval][out] */ VARIANT_BOOL *RaiseErrors);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_RaiseRequestErrors )( 
            IDaControl * This,
            /* [in] */ VARIANT_BOOL RaiseErrors);
        
        END_INTERFACE
    } IDaControlVtbl;

    interface IDaControl
    {
        CONST_VTBL struct IDaControlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaControl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaControl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaControl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaControl_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaControl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaControl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaControl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaControl_get_Characters(This,Characters)	\
    ( (This)->lpVtbl -> get_Characters(This,Characters) ) 

#define IDaControl_get_AudioOutput(This,AudioOutput)	\
    ( (This)->lpVtbl -> get_AudioOutput(This,AudioOutput) ) 

#define IDaControl_get_SpeechInput(This,SpeechInput)	\
    ( (This)->lpVtbl -> get_SpeechInput(This,SpeechInput) ) 

#define IDaControl_get_PropertySheet(This,PropSheet)	\
    ( (This)->lpVtbl -> get_PropertySheet(This,PropSheet) ) 

#define IDaControl_get_CommandsWindow(This,CommandsWindow)	\
    ( (This)->lpVtbl -> get_CommandsWindow(This,CommandsWindow) ) 

#define IDaControl_get_Connected(This,Connected)	\
    ( (This)->lpVtbl -> get_Connected(This,Connected) ) 

#define IDaControl_put_Connected(This,Connected)	\
    ( (This)->lpVtbl -> put_Connected(This,Connected) ) 

#define IDaControl_get_Suspended(This,Suspended)	\
    ( (This)->lpVtbl -> get_Suspended(This,Suspended) ) 

#define IDaControl_ShowDefaultCharacterProperties(This,X,y)	\
    ( (This)->lpVtbl -> ShowDefaultCharacterProperties(This,X,y) ) 

#define IDaControl_get_RaiseRequestErrors(This,RaiseErrors)	\
    ( (This)->lpVtbl -> get_RaiseRequestErrors(This,RaiseErrors) ) 

#define IDaControl_put_RaiseRequestErrors(This,RaiseErrors)	\
    ( (This)->lpVtbl -> put_RaiseRequestErrors(This,RaiseErrors) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaControl_INTERFACE_DEFINED__ */


#ifndef ___DaCtlEvents_DISPINTERFACE_DEFINED__
#define ___DaCtlEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DaCtlEvents */
/* [helpcontext][hidden][uuid] */ 


EXTERN_C const IID DIID__DaCtlEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("1147E54A-A208-11DE-ABF2-002421116FB2")
    _DaCtlEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DaCtlEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DaCtlEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DaCtlEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DaCtlEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DaCtlEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DaCtlEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DaCtlEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DaCtlEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DaCtlEventsVtbl;

    interface _DaCtlEvents
    {
        CONST_VTBL struct _DaCtlEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DaCtlEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DaCtlEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DaCtlEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DaCtlEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DaCtlEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DaCtlEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DaCtlEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DaCtlEvents_DISPINTERFACE_DEFINED__ */


#ifndef __IDaCtlCharacterFiles_INTERFACE_DEFINED__
#define __IDaCtlCharacterFiles_INTERFACE_DEFINED__

/* interface IDaCtlCharacterFiles */
/* [object][helpcontext][nonextensible][oleautomation][dual][unique][uuid] */ 


EXTERN_C const IID IID_IDaCtlCharacterFiles;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E557-A208-11DE-ABF2-002421116FB2")
    IDaCtlCharacterFiles : public IDispatch
    {
    public:
        virtual /* [defaultbind][displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_FilePaths( 
            /* [retval][out] */ SAFEARRAY * *FilePaths) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_SearchPath( 
            /* [retval][out] */ BSTR *SearchPath) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_SearchPath( 
            /* [in] */ BSTR SearchPath) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_DefaultSearchPath( 
            /* [retval][out] */ BSTR *DefaultSearchPath) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_DoubleAgentFiles( 
            /* [retval][out] */ VARIANT_BOOL *DoubleAgentFiles) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_DoubleAgentFiles( 
            /* [in] */ VARIANT_BOOL DoubleAgentFiles) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_MsAgentFiles( 
            /* [retval][out] */ VARIANT_BOOL *MsAgentFiles) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_MsAgentFiles( 
            /* [in] */ VARIANT_BOOL MsAgentFiles) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_MsOfficeFiles( 
            /* [retval][out] */ VARIANT_BOOL *MsOfficeFiles) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_MsOfficeFiles( 
            /* [in] */ VARIANT_BOOL MsOfficeFiles) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_SpeakingCharacters( 
            /* [retval][out] */ VARIANT_BOOL *SpeakingCharacters) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_SpeakingCharacters( 
            /* [in] */ VARIANT_BOOL SpeakingCharacters) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_NonSpeakingCharacters( 
            /* [retval][out] */ VARIANT_BOOL *NonSpeakingCharacters) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_NonSpeakingCharacters( 
            /* [in] */ VARIANT_BOOL NonSpeakingCharacters) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_VerifyVersion( 
            /* [retval][out] */ VARIANT_BOOL *VerifyVersion) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_VerifyVersion( 
            /* [in] */ VARIANT_BOOL VerifyVersion) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_DefaultFilePath( 
            /* [retval][out] */ BSTR *DefaultFilePath) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_DefaultFileName( 
            /* [retval][out] */ BSTR *DefaultFileName) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_CompliantCharacters( 
            /* [retval][out] */ VARIANT_BOOL *CompliantCharacters) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_CompliantCharacters( 
            /* [in] */ VARIANT_BOOL CompliantCharacters) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_NonCompliantCharacters( 
            /* [retval][out] */ VARIANT_BOOL *NonCompliantCharacters) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_NonCompliantCharacters( 
            /* [in] */ VARIANT_BOOL NonCompliantCharacters) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlCharacterFilesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlCharacterFiles * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlCharacterFiles * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlCharacterFiles * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlCharacterFiles * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlCharacterFiles * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlCharacterFiles * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlCharacterFiles * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [defaultbind][displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_FilePaths )( 
            IDaCtlCharacterFiles * This,
            /* [retval][out] */ SAFEARRAY * *FilePaths);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SearchPath )( 
            IDaCtlCharacterFiles * This,
            /* [retval][out] */ BSTR *SearchPath);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_SearchPath )( 
            IDaCtlCharacterFiles * This,
            /* [in] */ BSTR SearchPath);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_DefaultSearchPath )( 
            IDaCtlCharacterFiles * This,
            /* [retval][out] */ BSTR *DefaultSearchPath);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_DoubleAgentFiles )( 
            IDaCtlCharacterFiles * This,
            /* [retval][out] */ VARIANT_BOOL *DoubleAgentFiles);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_DoubleAgentFiles )( 
            IDaCtlCharacterFiles * This,
            /* [in] */ VARIANT_BOOL DoubleAgentFiles);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_MsAgentFiles )( 
            IDaCtlCharacterFiles * This,
            /* [retval][out] */ VARIANT_BOOL *MsAgentFiles);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_MsAgentFiles )( 
            IDaCtlCharacterFiles * This,
            /* [in] */ VARIANT_BOOL MsAgentFiles);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_MsOfficeFiles )( 
            IDaCtlCharacterFiles * This,
            /* [retval][out] */ VARIANT_BOOL *MsOfficeFiles);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_MsOfficeFiles )( 
            IDaCtlCharacterFiles * This,
            /* [in] */ VARIANT_BOOL MsOfficeFiles);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SpeakingCharacters )( 
            IDaCtlCharacterFiles * This,
            /* [retval][out] */ VARIANT_BOOL *SpeakingCharacters);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_SpeakingCharacters )( 
            IDaCtlCharacterFiles * This,
            /* [in] */ VARIANT_BOOL SpeakingCharacters);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_NonSpeakingCharacters )( 
            IDaCtlCharacterFiles * This,
            /* [retval][out] */ VARIANT_BOOL *NonSpeakingCharacters);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_NonSpeakingCharacters )( 
            IDaCtlCharacterFiles * This,
            /* [in] */ VARIANT_BOOL NonSpeakingCharacters);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_VerifyVersion )( 
            IDaCtlCharacterFiles * This,
            /* [retval][out] */ VARIANT_BOOL *VerifyVersion);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_VerifyVersion )( 
            IDaCtlCharacterFiles * This,
            /* [in] */ VARIANT_BOOL VerifyVersion);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_DefaultFilePath )( 
            IDaCtlCharacterFiles * This,
            /* [retval][out] */ BSTR *DefaultFilePath);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_DefaultFileName )( 
            IDaCtlCharacterFiles * This,
            /* [retval][out] */ BSTR *DefaultFileName);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_CompliantCharacters )( 
            IDaCtlCharacterFiles * This,
            /* [retval][out] */ VARIANT_BOOL *CompliantCharacters);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_CompliantCharacters )( 
            IDaCtlCharacterFiles * This,
            /* [in] */ VARIANT_BOOL CompliantCharacters);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_NonCompliantCharacters )( 
            IDaCtlCharacterFiles * This,
            /* [retval][out] */ VARIANT_BOOL *NonCompliantCharacters);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_NonCompliantCharacters )( 
            IDaCtlCharacterFiles * This,
            /* [in] */ VARIANT_BOOL NonCompliantCharacters);
        
        END_INTERFACE
    } IDaCtlCharacterFilesVtbl;

    interface IDaCtlCharacterFiles
    {
        CONST_VTBL struct IDaCtlCharacterFilesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlCharacterFiles_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlCharacterFiles_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlCharacterFiles_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlCharacterFiles_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlCharacterFiles_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlCharacterFiles_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlCharacterFiles_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlCharacterFiles_get_FilePaths(This,FilePaths)	\
    ( (This)->lpVtbl -> get_FilePaths(This,FilePaths) ) 

#define IDaCtlCharacterFiles_get_SearchPath(This,SearchPath)	\
    ( (This)->lpVtbl -> get_SearchPath(This,SearchPath) ) 

#define IDaCtlCharacterFiles_put_SearchPath(This,SearchPath)	\
    ( (This)->lpVtbl -> put_SearchPath(This,SearchPath) ) 

#define IDaCtlCharacterFiles_get_DefaultSearchPath(This,DefaultSearchPath)	\
    ( (This)->lpVtbl -> get_DefaultSearchPath(This,DefaultSearchPath) ) 

#define IDaCtlCharacterFiles_get_DoubleAgentFiles(This,DoubleAgentFiles)	\
    ( (This)->lpVtbl -> get_DoubleAgentFiles(This,DoubleAgentFiles) ) 

#define IDaCtlCharacterFiles_put_DoubleAgentFiles(This,DoubleAgentFiles)	\
    ( (This)->lpVtbl -> put_DoubleAgentFiles(This,DoubleAgentFiles) ) 

#define IDaCtlCharacterFiles_get_MsAgentFiles(This,MsAgentFiles)	\
    ( (This)->lpVtbl -> get_MsAgentFiles(This,MsAgentFiles) ) 

#define IDaCtlCharacterFiles_put_MsAgentFiles(This,MsAgentFiles)	\
    ( (This)->lpVtbl -> put_MsAgentFiles(This,MsAgentFiles) ) 

#define IDaCtlCharacterFiles_get_MsOfficeFiles(This,MsOfficeFiles)	\
    ( (This)->lpVtbl -> get_MsOfficeFiles(This,MsOfficeFiles) ) 

#define IDaCtlCharacterFiles_put_MsOfficeFiles(This,MsOfficeFiles)	\
    ( (This)->lpVtbl -> put_MsOfficeFiles(This,MsOfficeFiles) ) 

#define IDaCtlCharacterFiles_get_SpeakingCharacters(This,SpeakingCharacters)	\
    ( (This)->lpVtbl -> get_SpeakingCharacters(This,SpeakingCharacters) ) 

#define IDaCtlCharacterFiles_put_SpeakingCharacters(This,SpeakingCharacters)	\
    ( (This)->lpVtbl -> put_SpeakingCharacters(This,SpeakingCharacters) ) 

#define IDaCtlCharacterFiles_get_NonSpeakingCharacters(This,NonSpeakingCharacters)	\
    ( (This)->lpVtbl -> get_NonSpeakingCharacters(This,NonSpeakingCharacters) ) 

#define IDaCtlCharacterFiles_put_NonSpeakingCharacters(This,NonSpeakingCharacters)	\
    ( (This)->lpVtbl -> put_NonSpeakingCharacters(This,NonSpeakingCharacters) ) 

#define IDaCtlCharacterFiles_get_VerifyVersion(This,VerifyVersion)	\
    ( (This)->lpVtbl -> get_VerifyVersion(This,VerifyVersion) ) 

#define IDaCtlCharacterFiles_put_VerifyVersion(This,VerifyVersion)	\
    ( (This)->lpVtbl -> put_VerifyVersion(This,VerifyVersion) ) 

#define IDaCtlCharacterFiles_get_DefaultFilePath(This,DefaultFilePath)	\
    ( (This)->lpVtbl -> get_DefaultFilePath(This,DefaultFilePath) ) 

#define IDaCtlCharacterFiles_get_DefaultFileName(This,DefaultFileName)	\
    ( (This)->lpVtbl -> get_DefaultFileName(This,DefaultFileName) ) 

#define IDaCtlCharacterFiles_get_CompliantCharacters(This,CompliantCharacters)	\
    ( (This)->lpVtbl -> get_CompliantCharacters(This,CompliantCharacters) ) 

#define IDaCtlCharacterFiles_put_CompliantCharacters(This,CompliantCharacters)	\
    ( (This)->lpVtbl -> put_CompliantCharacters(This,CompliantCharacters) ) 

#define IDaCtlCharacterFiles_get_NonCompliantCharacters(This,NonCompliantCharacters)	\
    ( (This)->lpVtbl -> get_NonCompliantCharacters(This,NonCompliantCharacters) ) 

#define IDaCtlCharacterFiles_put_NonCompliantCharacters(This,NonCompliantCharacters)	\
    ( (This)->lpVtbl -> put_NonCompliantCharacters(This,NonCompliantCharacters) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlCharacterFiles_INTERFACE_DEFINED__ */


#ifndef __IDaCtlFormattedText_INTERFACE_DEFINED__
#define __IDaCtlFormattedText_INTERFACE_DEFINED__

/* interface IDaCtlFormattedText */
/* [object][helpcontext][nonextensible][oleautomation][dual][unique][uuid] */ 


EXTERN_C const IID IID_IDaCtlFormattedText;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E581-A208-11DE-ABF2-002421116FB2")
    IDaCtlFormattedText : public IDispatch
    {
    public:
        virtual /* [defaultbind][displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_RawText( 
            /* [retval][out] */ BSTR *RawText) = 0;
        
        virtual /* [defaultbind][displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_RawText( 
            /* [in] */ BSTR RawText) = 0;
        
        virtual /* [defaultbind][displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_ParsedText( 
            /* [retval][out] */ BSTR *ParsedText) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_SpeechText( 
            /* [retval][out] */ BSTR *SpeechText) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_WordCount( 
            /* [retval][out] */ long *WordCount) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_DisplayWord( 
            /* [in] */ long WordIndex,
            /* [retval][out] */ BSTR *DisplayWord) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_WordIndex( 
            /* [retval][out] */ long *WordIndex) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_DisplayText( 
            /* [retval][out] */ BSTR *DisplayText) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE Parse( 
            /* [in] */ BSTR RawText) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE Append( 
            /* [in] */ IDaCtlFormattedText *FormattedText) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlFormattedTextVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlFormattedText * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlFormattedText * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlFormattedText * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlFormattedText * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlFormattedText * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlFormattedText * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlFormattedText * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [defaultbind][displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_RawText )( 
            IDaCtlFormattedText * This,
            /* [retval][out] */ BSTR *RawText);
        
        /* [defaultbind][displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_RawText )( 
            IDaCtlFormattedText * This,
            /* [in] */ BSTR RawText);
        
        /* [defaultbind][displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_ParsedText )( 
            IDaCtlFormattedText * This,
            /* [retval][out] */ BSTR *ParsedText);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SpeechText )( 
            IDaCtlFormattedText * This,
            /* [retval][out] */ BSTR *SpeechText);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_WordCount )( 
            IDaCtlFormattedText * This,
            /* [retval][out] */ long *WordCount);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_DisplayWord )( 
            IDaCtlFormattedText * This,
            /* [in] */ long WordIndex,
            /* [retval][out] */ BSTR *DisplayWord);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_WordIndex )( 
            IDaCtlFormattedText * This,
            /* [retval][out] */ long *WordIndex);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_DisplayText )( 
            IDaCtlFormattedText * This,
            /* [retval][out] */ BSTR *DisplayText);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Parse )( 
            IDaCtlFormattedText * This,
            /* [in] */ BSTR RawText);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Append )( 
            IDaCtlFormattedText * This,
            /* [in] */ IDaCtlFormattedText *FormattedText);
        
        END_INTERFACE
    } IDaCtlFormattedTextVtbl;

    interface IDaCtlFormattedText
    {
        CONST_VTBL struct IDaCtlFormattedTextVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlFormattedText_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlFormattedText_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlFormattedText_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlFormattedText_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlFormattedText_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlFormattedText_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlFormattedText_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlFormattedText_get_RawText(This,RawText)	\
    ( (This)->lpVtbl -> get_RawText(This,RawText) ) 

#define IDaCtlFormattedText_put_RawText(This,RawText)	\
    ( (This)->lpVtbl -> put_RawText(This,RawText) ) 

#define IDaCtlFormattedText_get_ParsedText(This,ParsedText)	\
    ( (This)->lpVtbl -> get_ParsedText(This,ParsedText) ) 

#define IDaCtlFormattedText_get_SpeechText(This,SpeechText)	\
    ( (This)->lpVtbl -> get_SpeechText(This,SpeechText) ) 

#define IDaCtlFormattedText_get_WordCount(This,WordCount)	\
    ( (This)->lpVtbl -> get_WordCount(This,WordCount) ) 

#define IDaCtlFormattedText_get_DisplayWord(This,WordIndex,DisplayWord)	\
    ( (This)->lpVtbl -> get_DisplayWord(This,WordIndex,DisplayWord) ) 

#define IDaCtlFormattedText_get_WordIndex(This,WordIndex)	\
    ( (This)->lpVtbl -> get_WordIndex(This,WordIndex) ) 

#define IDaCtlFormattedText_get_DisplayText(This,DisplayText)	\
    ( (This)->lpVtbl -> get_DisplayText(This,DisplayText) ) 

#define IDaCtlFormattedText_Parse(This,RawText)	\
    ( (This)->lpVtbl -> Parse(This,RawText) ) 

#define IDaCtlFormattedText_Append(This,FormattedText)	\
    ( (This)->lpVtbl -> Append(This,FormattedText) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlFormattedText_INTERFACE_DEFINED__ */


#ifndef __IDaCtlTTSEngine_INTERFACE_DEFINED__
#define __IDaCtlTTSEngine_INTERFACE_DEFINED__

/* interface IDaCtlTTSEngine */
/* [object][helpcontext][nonextensible][oleautomation][dual][unique][uuid] */ 


EXTERN_C const IID IID_IDaCtlTTSEngine;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E54C-A208-11DE-ABF2-002421116FB2")
    IDaCtlTTSEngine : public IDispatch
    {
    public:
        virtual /* [defaultbind][displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_TTSModeID( 
            /* [retval][out] */ BSTR *TTSModeID) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_DisplayName( 
            /* [retval][out] */ BSTR *DisplayName) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Manufacturer( 
            /* [retval][out] */ BSTR *Manufacturer) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE GetVersion( 
            /* [out] */ short *MajorVersion,
            /* [out] */ short *MinorVersion) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Gender( 
            /* [retval][out] */ SpeechGenderType *Gender) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_LanguageID( 
            /* [retval][out] */ long *LanguageID) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_LanguageName( 
            /* [defaultvalue][in] */ VARIANT_BOOL EnglishName,
            /* [retval][out] */ BSTR *LanguageName) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlTTSEngineVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlTTSEngine * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlTTSEngine * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlTTSEngine * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlTTSEngine * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlTTSEngine * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlTTSEngine * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlTTSEngine * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [defaultbind][displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_TTSModeID )( 
            IDaCtlTTSEngine * This,
            /* [retval][out] */ BSTR *TTSModeID);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_DisplayName )( 
            IDaCtlTTSEngine * This,
            /* [retval][out] */ BSTR *DisplayName);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Manufacturer )( 
            IDaCtlTTSEngine * This,
            /* [retval][out] */ BSTR *Manufacturer);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVersion )( 
            IDaCtlTTSEngine * This,
            /* [out] */ short *MajorVersion,
            /* [out] */ short *MinorVersion);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Gender )( 
            IDaCtlTTSEngine * This,
            /* [retval][out] */ SpeechGenderType *Gender);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_LanguageID )( 
            IDaCtlTTSEngine * This,
            /* [retval][out] */ long *LanguageID);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_LanguageName )( 
            IDaCtlTTSEngine * This,
            /* [defaultvalue][in] */ VARIANT_BOOL EnglishName,
            /* [retval][out] */ BSTR *LanguageName);
        
        END_INTERFACE
    } IDaCtlTTSEngineVtbl;

    interface IDaCtlTTSEngine
    {
        CONST_VTBL struct IDaCtlTTSEngineVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlTTSEngine_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlTTSEngine_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlTTSEngine_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlTTSEngine_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlTTSEngine_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlTTSEngine_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlTTSEngine_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlTTSEngine_get_TTSModeID(This,TTSModeID)	\
    ( (This)->lpVtbl -> get_TTSModeID(This,TTSModeID) ) 

#define IDaCtlTTSEngine_get_DisplayName(This,DisplayName)	\
    ( (This)->lpVtbl -> get_DisplayName(This,DisplayName) ) 

#define IDaCtlTTSEngine_get_Manufacturer(This,Manufacturer)	\
    ( (This)->lpVtbl -> get_Manufacturer(This,Manufacturer) ) 

#define IDaCtlTTSEngine_GetVersion(This,MajorVersion,MinorVersion)	\
    ( (This)->lpVtbl -> GetVersion(This,MajorVersion,MinorVersion) ) 

#define IDaCtlTTSEngine_get_Gender(This,Gender)	\
    ( (This)->lpVtbl -> get_Gender(This,Gender) ) 

#define IDaCtlTTSEngine_get_LanguageID(This,LanguageID)	\
    ( (This)->lpVtbl -> get_LanguageID(This,LanguageID) ) 

#define IDaCtlTTSEngine_get_LanguageName(This,EnglishName,LanguageName)	\
    ( (This)->lpVtbl -> get_LanguageName(This,EnglishName,LanguageName) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlTTSEngine_INTERFACE_DEFINED__ */


#ifndef __IDaCtlTTSEngines_INTERFACE_DEFINED__
#define __IDaCtlTTSEngines_INTERFACE_DEFINED__

/* interface IDaCtlTTSEngines */
/* [object][custom][helpcontext][nonextensible][oleautomation][dual][unique][uuid] */ 


EXTERN_C const IID IID_IDaCtlTTSEngines;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E54D-A208-11DE-ABF2-002421116FB2")
    IDaCtlTTSEngines : public IDispatch
    {
    public:
        virtual /* [propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT Index,
            /* [retval][out] */ IDaCtlTTSEngine **TTSEngine) = 0;
        
        virtual /* [defaultbind][displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *Value) = 0;
        
        virtual /* [restricted][propget][id] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **Enum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlTTSEnginesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlTTSEngines * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlTTSEngines * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlTTSEngines * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlTTSEngines * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlTTSEngines * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlTTSEngines * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlTTSEngines * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IDaCtlTTSEngines * This,
            /* [in] */ VARIANT Index,
            /* [retval][out] */ IDaCtlTTSEngine **TTSEngine);
        
        /* [defaultbind][displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IDaCtlTTSEngines * This,
            /* [retval][out] */ long *Value);
        
        /* [restricted][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IDaCtlTTSEngines * This,
            /* [retval][out] */ IUnknown **Enum);
        
        END_INTERFACE
    } IDaCtlTTSEnginesVtbl;

    interface IDaCtlTTSEngines
    {
        CONST_VTBL struct IDaCtlTTSEnginesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlTTSEngines_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlTTSEngines_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlTTSEngines_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlTTSEngines_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlTTSEngines_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlTTSEngines_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlTTSEngines_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlTTSEngines_get_Item(This,Index,TTSEngine)	\
    ( (This)->lpVtbl -> get_Item(This,Index,TTSEngine) ) 

#define IDaCtlTTSEngines_get_Count(This,Value)	\
    ( (This)->lpVtbl -> get_Count(This,Value) ) 

#define IDaCtlTTSEngines_get__NewEnum(This,Enum)	\
    ( (This)->lpVtbl -> get__NewEnum(This,Enum) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlTTSEngines_INTERFACE_DEFINED__ */


#ifndef __IDaCtlSREngine_INTERFACE_DEFINED__
#define __IDaCtlSREngine_INTERFACE_DEFINED__

/* interface IDaCtlSREngine */
/* [object][helpcontext][nonextensible][oleautomation][dual][unique][uuid] */ 


EXTERN_C const IID IID_IDaCtlSREngine;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E54E-A208-11DE-ABF2-002421116FB2")
    IDaCtlSREngine : public IDispatch
    {
    public:
        virtual /* [defaultbind][displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_SRModeID( 
            /* [retval][out] */ BSTR *SRModeID) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_DisplayName( 
            /* [retval][out] */ BSTR *DisplayName) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Manufacturer( 
            /* [retval][out] */ BSTR *Manufacturer) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE GetVersion( 
            /* [out] */ short *MajorVersion,
            /* [out] */ short *MinorVersion) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_LanguageID( 
            /* [retval][out] */ long *LanguageID) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_LanguageName( 
            /* [defaultvalue][in] */ VARIANT_BOOL EnglishName,
            /* [retval][out] */ BSTR *LanguageName) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_LanguageIDs( 
            /* [retval][out] */ SAFEARRAY * *LanguageIDs) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_LanguageNames( 
            /* [defaultvalue][in] */ VARIANT_BOOL EnglishNames,
            /* [retval][out] */ SAFEARRAY * *LanguageNames) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlSREngineVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlSREngine * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlSREngine * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlSREngine * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlSREngine * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlSREngine * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlSREngine * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlSREngine * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [defaultbind][displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SRModeID )( 
            IDaCtlSREngine * This,
            /* [retval][out] */ BSTR *SRModeID);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_DisplayName )( 
            IDaCtlSREngine * This,
            /* [retval][out] */ BSTR *DisplayName);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Manufacturer )( 
            IDaCtlSREngine * This,
            /* [retval][out] */ BSTR *Manufacturer);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVersion )( 
            IDaCtlSREngine * This,
            /* [out] */ short *MajorVersion,
            /* [out] */ short *MinorVersion);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_LanguageID )( 
            IDaCtlSREngine * This,
            /* [retval][out] */ long *LanguageID);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_LanguageName )( 
            IDaCtlSREngine * This,
            /* [defaultvalue][in] */ VARIANT_BOOL EnglishName,
            /* [retval][out] */ BSTR *LanguageName);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_LanguageIDs )( 
            IDaCtlSREngine * This,
            /* [retval][out] */ SAFEARRAY * *LanguageIDs);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_LanguageNames )( 
            IDaCtlSREngine * This,
            /* [defaultvalue][in] */ VARIANT_BOOL EnglishNames,
            /* [retval][out] */ SAFEARRAY * *LanguageNames);
        
        END_INTERFACE
    } IDaCtlSREngineVtbl;

    interface IDaCtlSREngine
    {
        CONST_VTBL struct IDaCtlSREngineVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlSREngine_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlSREngine_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlSREngine_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlSREngine_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlSREngine_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlSREngine_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlSREngine_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlSREngine_get_SRModeID(This,SRModeID)	\
    ( (This)->lpVtbl -> get_SRModeID(This,SRModeID) ) 

#define IDaCtlSREngine_get_DisplayName(This,DisplayName)	\
    ( (This)->lpVtbl -> get_DisplayName(This,DisplayName) ) 

#define IDaCtlSREngine_get_Manufacturer(This,Manufacturer)	\
    ( (This)->lpVtbl -> get_Manufacturer(This,Manufacturer) ) 

#define IDaCtlSREngine_GetVersion(This,MajorVersion,MinorVersion)	\
    ( (This)->lpVtbl -> GetVersion(This,MajorVersion,MinorVersion) ) 

#define IDaCtlSREngine_get_LanguageID(This,LanguageID)	\
    ( (This)->lpVtbl -> get_LanguageID(This,LanguageID) ) 

#define IDaCtlSREngine_get_LanguageName(This,EnglishName,LanguageName)	\
    ( (This)->lpVtbl -> get_LanguageName(This,EnglishName,LanguageName) ) 

#define IDaCtlSREngine_get_LanguageIDs(This,LanguageIDs)	\
    ( (This)->lpVtbl -> get_LanguageIDs(This,LanguageIDs) ) 

#define IDaCtlSREngine_get_LanguageNames(This,EnglishNames,LanguageNames)	\
    ( (This)->lpVtbl -> get_LanguageNames(This,EnglishNames,LanguageNames) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlSREngine_INTERFACE_DEFINED__ */


#ifndef __IDaCtlSREngines_INTERFACE_DEFINED__
#define __IDaCtlSREngines_INTERFACE_DEFINED__

/* interface IDaCtlSREngines */
/* [object][custom][helpcontext][nonextensible][oleautomation][dual][unique][uuid] */ 


EXTERN_C const IID IID_IDaCtlSREngines;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E54F-A208-11DE-ABF2-002421116FB2")
    IDaCtlSREngines : public IDispatch
    {
    public:
        virtual /* [propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT Index,
            /* [retval][out] */ IDaCtlSREngine **SREngine) = 0;
        
        virtual /* [defaultbind][displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *Value) = 0;
        
        virtual /* [restricted][propget][id] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **Enum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlSREnginesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlSREngines * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlSREngines * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlSREngines * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlSREngines * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlSREngines * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlSREngines * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlSREngines * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IDaCtlSREngines * This,
            /* [in] */ VARIANT Index,
            /* [retval][out] */ IDaCtlSREngine **SREngine);
        
        /* [defaultbind][displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IDaCtlSREngines * This,
            /* [retval][out] */ long *Value);
        
        /* [restricted][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IDaCtlSREngines * This,
            /* [retval][out] */ IUnknown **Enum);
        
        END_INTERFACE
    } IDaCtlSREnginesVtbl;

    interface IDaCtlSREngines
    {
        CONST_VTBL struct IDaCtlSREnginesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlSREngines_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlSREngines_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlSREngines_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlSREngines_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlSREngines_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlSREngines_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlSREngines_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlSREngines_get_Item(This,Index,SREngine)	\
    ( (This)->lpVtbl -> get_Item(This,Index,SREngine) ) 

#define IDaCtlSREngines_get_Count(This,Value)	\
    ( (This)->lpVtbl -> get_Count(This,Value) ) 

#define IDaCtlSREngines_get__NewEnum(This,Enum)	\
    ( (This)->lpVtbl -> get__NewEnum(This,Enum) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlSREngines_INTERFACE_DEFINED__ */


#ifndef __IDaControl2_INTERFACE_DEFINED__
#define __IDaControl2_INTERFACE_DEFINED__

/* interface IDaControl2 */
/* [helpcontext][nonextensible][oleautomation][dual][unique][uuid][object] */ 


EXTERN_C const IID IID_IDaControl2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E551-A208-11DE-ABF2-002421116FB2")
    IDaControl2 : public IDaControl
    {
    public:
        virtual /* [requestedit][displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_AutoSize( 
            /* [in] */ VARIANT_BOOL AutoSize) = 0;
        
        virtual /* [requestedit][displaybind][bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_AutoSize( 
            /* [retval][out] */ VARIANT_BOOL *AutoSize) = 0;
        
        virtual /* [requestedit][displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_BackColor( 
            /* [in] */ /* external definition not present */ OLE_COLOR BackColor) = 0;
        
        virtual /* [requestedit][displaybind][bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_BackColor( 
            /* [retval][out] */ /* external definition not present */ OLE_COLOR *BackColor) = 0;
        
        virtual /* [requestedit][displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_BorderColor( 
            /* [in] */ /* external definition not present */ OLE_COLOR BorderColor) = 0;
        
        virtual /* [requestedit][displaybind][bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_BorderColor( 
            /* [retval][out] */ /* external definition not present */ OLE_COLOR *BorderColor) = 0;
        
        virtual /* [requestedit][displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_BorderStyle( 
            /* [in] */ long BorderStyle) = 0;
        
        virtual /* [requestedit][displaybind][bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_BorderStyle( 
            /* [retval][out] */ long *BorderStyle) = 0;
        
        virtual /* [requestedit][displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_BorderWidth( 
            /* [in] */ long BorderWidth) = 0;
        
        virtual /* [requestedit][displaybind][bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_BorderWidth( 
            /* [retval][out] */ long *BorderWidth) = 0;
        
        virtual /* [requestedit][displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_BorderVisible( 
            /* [in] */ VARIANT_BOOL BorderVisible) = 0;
        
        virtual /* [requestedit][displaybind][bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_BorderVisible( 
            /* [retval][out] */ VARIANT_BOOL *BorderVisible) = 0;
        
        virtual /* [requestedit][displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_MousePointer( 
            /* [in] */ long MousePointer) = 0;
        
        virtual /* [requestedit][displaybind][bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_MousePointer( 
            /* [retval][out] */ long *MousePointer) = 0;
        
        virtual /* [restricted][hidden][requestedit][bindable][propput][id][helpstring] */ HRESULT STDMETHODCALLTYPE put_MouseIcon( 
            /* [in] */ /* external definition not present */ IPictureDisp *MouseIcon) = 0;
        
        virtual /* [restricted][hidden][requestedit][bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_MouseIcon( 
            /* [retval][out] */ /* external definition not present */ IPictureDisp **MouseIcon) = 0;
        
        virtual /* [bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_CharacterStyle( 
            /* [retval][out] */ long *CharacterStyle) = 0;
        
        virtual /* [bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_CharacterStyle( 
            /* [in] */ long CharacterStyle) = 0;
        
        virtual /* [propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_CharacterFiles( 
            /* [retval][out] */ IDaCtlCharacterFiles **CharacterFiles) = 0;
        
        virtual /* [propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_TTSEngines( 
            /* [retval][out] */ IDaCtlTTSEngines **TTSEngines) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE FindTTSEngines( 
            /* [optional][in] */ VARIANT LanguageID,
            /* [optional][in] */ VARIANT Gender,
            /* [retval][out] */ IDaCtlTTSEngines **TTSEngines) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE GetCharacterTTSEngine( 
            /* [optional][in] */ VARIANT Provider,
            /* [retval][out] */ IDaCtlTTSEngine **TTSEngine) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE FindCharacterTTSEngines( 
            /* [optional][in] */ VARIANT Provider,
            /* [optional][in] */ VARIANT LanguageID,
            /* [retval][out] */ IDaCtlTTSEngines **TTSEngines) = 0;
        
        virtual /* [propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_SREngines( 
            /* [retval][out] */ IDaCtlSREngines **SREngines) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE FindSREngines( 
            /* [optional][in] */ VARIANT LanguageID,
            /* [retval][out] */ IDaCtlSREngines **SREngines) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE GetCharacterSREngine( 
            /* [optional][in] */ VARIANT Provider,
            /* [retval][out] */ IDaCtlSREngine **SREngine) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE FindCharacterSREngines( 
            /* [optional][in] */ VARIANT Provider,
            /* [optional][in] */ VARIANT LanguageID,
            /* [retval][out] */ IDaCtlSREngines **SREngines) = 0;
        
        virtual /* [propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Settings( 
            /* [retval][out] */ IDaCtlSettings **Settings) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_AutoConnect( 
            /* [retval][out] */ short *AutoConnect) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_AutoConnect( 
            /* [in] */ short AutoConnect) = 0;
        
        virtual /* [propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_ControlCharacter( 
            /* [retval][out] */ IDaCtlCharacter2 **ControlCharacter) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_ControlCharacter( 
            /* [in] */ IDaCtlCharacter2 *ControlCharacter) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaControl2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaControl2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaControl2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaControl2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaControl2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaControl2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaControl2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaControl2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Characters )( 
            IDaControl2 * This,
            /* [retval][out] */ IDaCtlCharacters2 **Characters);
        
        /* [nonbrowsable][hidden][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_AudioOutput )( 
            IDaControl2 * This,
            /* [retval][out] */ IDaCtlAudioOutput **AudioOutput);
        
        /* [nonbrowsable][hidden][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SpeechInput )( 
            IDaControl2 * This,
            /* [retval][out] */ IDaCtlSpeechInput **SpeechInput);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_PropertySheet )( 
            IDaControl2 * This,
            /* [retval][out] */ IDaCtlPropertySheet2 **PropSheet);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_CommandsWindow )( 
            IDaControl2 * This,
            /* [retval][out] */ IDaCtlCommandsWindow **CommandsWindow);
        
        /* [bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Connected )( 
            IDaControl2 * This,
            /* [retval][out] */ VARIANT_BOOL *Connected);
        
        /* [bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Connected )( 
            IDaControl2 * This,
            VARIANT_BOOL Connected);
        
        /* [nonbrowsable][restricted][hidden][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Suspended )( 
            IDaControl2 * This,
            /* [retval][out] */ VARIANT_BOOL *Suspended);
        
        /* [hidden][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *ShowDefaultCharacterProperties )( 
            IDaControl2 * This,
            /* [optional][in] */ VARIANT X,
            /* [optional][in] */ VARIANT y);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_RaiseRequestErrors )( 
            IDaControl2 * This,
            /* [retval][out] */ VARIANT_BOOL *RaiseErrors);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_RaiseRequestErrors )( 
            IDaControl2 * This,
            /* [in] */ VARIANT_BOOL RaiseErrors);
        
        /* [requestedit][displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_AutoSize )( 
            IDaControl2 * This,
            /* [in] */ VARIANT_BOOL AutoSize);
        
        /* [requestedit][displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_AutoSize )( 
            IDaControl2 * This,
            /* [retval][out] */ VARIANT_BOOL *AutoSize);
        
        /* [requestedit][displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_BackColor )( 
            IDaControl2 * This,
            /* [in] */ /* external definition not present */ OLE_COLOR BackColor);
        
        /* [requestedit][displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_BackColor )( 
            IDaControl2 * This,
            /* [retval][out] */ /* external definition not present */ OLE_COLOR *BackColor);
        
        /* [requestedit][displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_BorderColor )( 
            IDaControl2 * This,
            /* [in] */ /* external definition not present */ OLE_COLOR BorderColor);
        
        /* [requestedit][displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_BorderColor )( 
            IDaControl2 * This,
            /* [retval][out] */ /* external definition not present */ OLE_COLOR *BorderColor);
        
        /* [requestedit][displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_BorderStyle )( 
            IDaControl2 * This,
            /* [in] */ long BorderStyle);
        
        /* [requestedit][displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_BorderStyle )( 
            IDaControl2 * This,
            /* [retval][out] */ long *BorderStyle);
        
        /* [requestedit][displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_BorderWidth )( 
            IDaControl2 * This,
            /* [in] */ long BorderWidth);
        
        /* [requestedit][displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_BorderWidth )( 
            IDaControl2 * This,
            /* [retval][out] */ long *BorderWidth);
        
        /* [requestedit][displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_BorderVisible )( 
            IDaControl2 * This,
            /* [in] */ VARIANT_BOOL BorderVisible);
        
        /* [requestedit][displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_BorderVisible )( 
            IDaControl2 * This,
            /* [retval][out] */ VARIANT_BOOL *BorderVisible);
        
        /* [requestedit][displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_MousePointer )( 
            IDaControl2 * This,
            /* [in] */ long MousePointer);
        
        /* [requestedit][displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_MousePointer )( 
            IDaControl2 * This,
            /* [retval][out] */ long *MousePointer);
        
        /* [restricted][hidden][requestedit][bindable][propput][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_MouseIcon )( 
            IDaControl2 * This,
            /* [in] */ /* external definition not present */ IPictureDisp *MouseIcon);
        
        /* [restricted][hidden][requestedit][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_MouseIcon )( 
            IDaControl2 * This,
            /* [retval][out] */ /* external definition not present */ IPictureDisp **MouseIcon);
        
        /* [bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_CharacterStyle )( 
            IDaControl2 * This,
            /* [retval][out] */ long *CharacterStyle);
        
        /* [bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_CharacterStyle )( 
            IDaControl2 * This,
            /* [in] */ long CharacterStyle);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_CharacterFiles )( 
            IDaControl2 * This,
            /* [retval][out] */ IDaCtlCharacterFiles **CharacterFiles);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_TTSEngines )( 
            IDaControl2 * This,
            /* [retval][out] */ IDaCtlTTSEngines **TTSEngines);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *FindTTSEngines )( 
            IDaControl2 * This,
            /* [optional][in] */ VARIANT LanguageID,
            /* [optional][in] */ VARIANT Gender,
            /* [retval][out] */ IDaCtlTTSEngines **TTSEngines);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetCharacterTTSEngine )( 
            IDaControl2 * This,
            /* [optional][in] */ VARIANT Provider,
            /* [retval][out] */ IDaCtlTTSEngine **TTSEngine);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *FindCharacterTTSEngines )( 
            IDaControl2 * This,
            /* [optional][in] */ VARIANT Provider,
            /* [optional][in] */ VARIANT LanguageID,
            /* [retval][out] */ IDaCtlTTSEngines **TTSEngines);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SREngines )( 
            IDaControl2 * This,
            /* [retval][out] */ IDaCtlSREngines **SREngines);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *FindSREngines )( 
            IDaControl2 * This,
            /* [optional][in] */ VARIANT LanguageID,
            /* [retval][out] */ IDaCtlSREngines **SREngines);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetCharacterSREngine )( 
            IDaControl2 * This,
            /* [optional][in] */ VARIANT Provider,
            /* [retval][out] */ IDaCtlSREngine **SREngine);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *FindCharacterSREngines )( 
            IDaControl2 * This,
            /* [optional][in] */ VARIANT Provider,
            /* [optional][in] */ VARIANT LanguageID,
            /* [retval][out] */ IDaCtlSREngines **SREngines);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Settings )( 
            IDaControl2 * This,
            /* [retval][out] */ IDaCtlSettings **Settings);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_AutoConnect )( 
            IDaControl2 * This,
            /* [retval][out] */ short *AutoConnect);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_AutoConnect )( 
            IDaControl2 * This,
            /* [in] */ short AutoConnect);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_ControlCharacter )( 
            IDaControl2 * This,
            /* [retval][out] */ IDaCtlCharacter2 **ControlCharacter);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ControlCharacter )( 
            IDaControl2 * This,
            /* [in] */ IDaCtlCharacter2 *ControlCharacter);
        
        END_INTERFACE
    } IDaControl2Vtbl;

    interface IDaControl2
    {
        CONST_VTBL struct IDaControl2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaControl2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaControl2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaControl2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaControl2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaControl2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaControl2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaControl2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaControl2_get_Characters(This,Characters)	\
    ( (This)->lpVtbl -> get_Characters(This,Characters) ) 

#define IDaControl2_get_AudioOutput(This,AudioOutput)	\
    ( (This)->lpVtbl -> get_AudioOutput(This,AudioOutput) ) 

#define IDaControl2_get_SpeechInput(This,SpeechInput)	\
    ( (This)->lpVtbl -> get_SpeechInput(This,SpeechInput) ) 

#define IDaControl2_get_PropertySheet(This,PropSheet)	\
    ( (This)->lpVtbl -> get_PropertySheet(This,PropSheet) ) 

#define IDaControl2_get_CommandsWindow(This,CommandsWindow)	\
    ( (This)->lpVtbl -> get_CommandsWindow(This,CommandsWindow) ) 

#define IDaControl2_get_Connected(This,Connected)	\
    ( (This)->lpVtbl -> get_Connected(This,Connected) ) 

#define IDaControl2_put_Connected(This,Connected)	\
    ( (This)->lpVtbl -> put_Connected(This,Connected) ) 

#define IDaControl2_get_Suspended(This,Suspended)	\
    ( (This)->lpVtbl -> get_Suspended(This,Suspended) ) 

#define IDaControl2_ShowDefaultCharacterProperties(This,X,y)	\
    ( (This)->lpVtbl -> ShowDefaultCharacterProperties(This,X,y) ) 

#define IDaControl2_get_RaiseRequestErrors(This,RaiseErrors)	\
    ( (This)->lpVtbl -> get_RaiseRequestErrors(This,RaiseErrors) ) 

#define IDaControl2_put_RaiseRequestErrors(This,RaiseErrors)	\
    ( (This)->lpVtbl -> put_RaiseRequestErrors(This,RaiseErrors) ) 


#define IDaControl2_put_AutoSize(This,AutoSize)	\
    ( (This)->lpVtbl -> put_AutoSize(This,AutoSize) ) 

#define IDaControl2_get_AutoSize(This,AutoSize)	\
    ( (This)->lpVtbl -> get_AutoSize(This,AutoSize) ) 

#define IDaControl2_put_BackColor(This,BackColor)	\
    ( (This)->lpVtbl -> put_BackColor(This,BackColor) ) 

#define IDaControl2_get_BackColor(This,BackColor)	\
    ( (This)->lpVtbl -> get_BackColor(This,BackColor) ) 

#define IDaControl2_put_BorderColor(This,BorderColor)	\
    ( (This)->lpVtbl -> put_BorderColor(This,BorderColor) ) 

#define IDaControl2_get_BorderColor(This,BorderColor)	\
    ( (This)->lpVtbl -> get_BorderColor(This,BorderColor) ) 

#define IDaControl2_put_BorderStyle(This,BorderStyle)	\
    ( (This)->lpVtbl -> put_BorderStyle(This,BorderStyle) ) 

#define IDaControl2_get_BorderStyle(This,BorderStyle)	\
    ( (This)->lpVtbl -> get_BorderStyle(This,BorderStyle) ) 

#define IDaControl2_put_BorderWidth(This,BorderWidth)	\
    ( (This)->lpVtbl -> put_BorderWidth(This,BorderWidth) ) 

#define IDaControl2_get_BorderWidth(This,BorderWidth)	\
    ( (This)->lpVtbl -> get_BorderWidth(This,BorderWidth) ) 

#define IDaControl2_put_BorderVisible(This,BorderVisible)	\
    ( (This)->lpVtbl -> put_BorderVisible(This,BorderVisible) ) 

#define IDaControl2_get_BorderVisible(This,BorderVisible)	\
    ( (This)->lpVtbl -> get_BorderVisible(This,BorderVisible) ) 

#define IDaControl2_put_MousePointer(This,MousePointer)	\
    ( (This)->lpVtbl -> put_MousePointer(This,MousePointer) ) 

#define IDaControl2_get_MousePointer(This,MousePointer)	\
    ( (This)->lpVtbl -> get_MousePointer(This,MousePointer) ) 

#define IDaControl2_put_MouseIcon(This,MouseIcon)	\
    ( (This)->lpVtbl -> put_MouseIcon(This,MouseIcon) ) 

#define IDaControl2_get_MouseIcon(This,MouseIcon)	\
    ( (This)->lpVtbl -> get_MouseIcon(This,MouseIcon) ) 

#define IDaControl2_get_CharacterStyle(This,CharacterStyle)	\
    ( (This)->lpVtbl -> get_CharacterStyle(This,CharacterStyle) ) 

#define IDaControl2_put_CharacterStyle(This,CharacterStyle)	\
    ( (This)->lpVtbl -> put_CharacterStyle(This,CharacterStyle) ) 

#define IDaControl2_get_CharacterFiles(This,CharacterFiles)	\
    ( (This)->lpVtbl -> get_CharacterFiles(This,CharacterFiles) ) 

#define IDaControl2_get_TTSEngines(This,TTSEngines)	\
    ( (This)->lpVtbl -> get_TTSEngines(This,TTSEngines) ) 

#define IDaControl2_FindTTSEngines(This,LanguageID,Gender,TTSEngines)	\
    ( (This)->lpVtbl -> FindTTSEngines(This,LanguageID,Gender,TTSEngines) ) 

#define IDaControl2_GetCharacterTTSEngine(This,Provider,TTSEngine)	\
    ( (This)->lpVtbl -> GetCharacterTTSEngine(This,Provider,TTSEngine) ) 

#define IDaControl2_FindCharacterTTSEngines(This,Provider,LanguageID,TTSEngines)	\
    ( (This)->lpVtbl -> FindCharacterTTSEngines(This,Provider,LanguageID,TTSEngines) ) 

#define IDaControl2_get_SREngines(This,SREngines)	\
    ( (This)->lpVtbl -> get_SREngines(This,SREngines) ) 

#define IDaControl2_FindSREngines(This,LanguageID,SREngines)	\
    ( (This)->lpVtbl -> FindSREngines(This,LanguageID,SREngines) ) 

#define IDaControl2_GetCharacterSREngine(This,Provider,SREngine)	\
    ( (This)->lpVtbl -> GetCharacterSREngine(This,Provider,SREngine) ) 

#define IDaControl2_FindCharacterSREngines(This,Provider,LanguageID,SREngines)	\
    ( (This)->lpVtbl -> FindCharacterSREngines(This,Provider,LanguageID,SREngines) ) 

#define IDaControl2_get_Settings(This,Settings)	\
    ( (This)->lpVtbl -> get_Settings(This,Settings) ) 

#define IDaControl2_get_AutoConnect(This,AutoConnect)	\
    ( (This)->lpVtbl -> get_AutoConnect(This,AutoConnect) ) 

#define IDaControl2_put_AutoConnect(This,AutoConnect)	\
    ( (This)->lpVtbl -> put_AutoConnect(This,AutoConnect) ) 

#define IDaControl2_get_ControlCharacter(This,ControlCharacter)	\
    ( (This)->lpVtbl -> get_ControlCharacter(This,ControlCharacter) ) 

#define IDaControl2_put_ControlCharacter(This,ControlCharacter)	\
    ( (This)->lpVtbl -> put_ControlCharacter(This,ControlCharacter) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaControl2_INTERFACE_DEFINED__ */


#ifndef __IDaCtlCharacter2_INTERFACE_DEFINED__
#define __IDaCtlCharacter2_INTERFACE_DEFINED__

/* interface IDaCtlCharacter2 */
/* [object][helpcontext][hidden][nonextensible][oleautomation][dual][unique][uuid] */ 


EXTERN_C const IID IID_IDaCtlCharacter2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E552-A208-11DE-ABF2-002421116FB2")
    IDaCtlCharacter2 : public IDaCtlCharacter
    {
    public:
        virtual /* [bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Style( 
            /* [retval][out] */ long *Style) = 0;
        
        virtual /* [bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_Style( 
            /* [in] */ long Style) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_HasIcon( 
            /* [retval][out] */ VARIANT_BOOL *HasIcon) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE GenerateIcon( 
            /* [defaultvalue][in] */ long ClipLeft = 0,
            /* [defaultvalue][in] */ long ClipTop = 0,
            /* [defaultvalue][in] */ long ClipWidth = -1,
            /* [defaultvalue][in] */ long ClipHeight = -1) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_IconShown( 
            /* [retval][out] */ VARIANT_BOOL *IconShown) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_IconShown( 
            /* [in] */ VARIANT_BOOL IconShown) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_IconVisible( 
            /* [retval][out] */ VARIANT_BOOL *IconVisible) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_IconIdentity( 
            /* [retval][out] */ BSTR *IconIdentity) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_IconIdentity( 
            /* [in] */ BSTR IconIdentity) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_IconTip( 
            /* [retval][out] */ BSTR *IconTip) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_IconTip( 
            /* [in] */ BSTR IconTip) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE SpeakFormatted( 
            /* [in] */ IDaCtlFormattedText *FormattedText,
            /* [retval][out] */ IDaCtlRequest **Request) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE ThinkFormatted( 
            /* [in] */ IDaCtlFormattedText *FormattedText,
            /* [retval][out] */ IDaCtlRequest **Request) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE NewFormattedText( 
            /* [retval][out] */ IDaCtlFormattedText **FormattedText) = 0;
        
        virtual /* [propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_TTSEngine( 
            /* [optional][in] */ VARIANT GetDefault,
            /* [retval][out] */ IDaCtlTTSEngine **TTSEngine) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE FindTTSEngines( 
            /* [optional][in] */ VARIANT LanguageID,
            /* [retval][out] */ IDaCtlTTSEngines **TTSEngines) = 0;
        
        virtual /* [propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_SREngine( 
            /* [optional][in] */ VARIANT GetDefault,
            /* [retval][out] */ IDaCtlSREngine **SREngine) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE FindSREngines( 
            /* [optional][in] */ VARIANT LanguageID,
            /* [retval][out] */ IDaCtlSREngines **SREngines) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_UniqueID( 
            /* [retval][out] */ BSTR *CharGUID) = 0;
        
        virtual /* [defaultbind][displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_CharacterID( 
            /* [retval][out] */ BSTR *CharacterID) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Connected( 
            /* [retval][out] */ VARIANT_BOOL *Connected) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_FileName( 
            /* [retval][out] */ BSTR *FileName) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_FilePath( 
            /* [retval][out] */ BSTR *FilePath) = 0;
        
        virtual /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE put_ActiveState( 
            /* [in] */ ActiveStateType ActiveState) = 0;
        
        virtual /* [displaybind][bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_ActiveState( 
            /* [retval][out] */ ActiveStateType *ActiveState) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_IdleState( 
            /* [retval][out] */ VARIANT_BOOL *IdleState) = 0;
        
        virtual /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE put_IdleEnabled( 
            /* [in] */ VARIANT_BOOL Enabled) = 0;
        
        virtual /* [displaybind][bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_IdleEnabled( 
            /* [retval][out] */ VARIANT_BOOL *Enabled) = 0;
        
        virtual /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE put_SoundEffectsEnabled( 
            /* [in] */ VARIANT_BOOL Enabled) = 0;
        
        virtual /* [displaybind][bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_SoundEffectsEnabled( 
            /* [retval][out] */ VARIANT_BOOL *Enabled) = 0;
        
        virtual /* [id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE Prepare( 
            /* [in] */ PrepareResourceType ResourceType,
            /* [in] */ BSTR Name,
            /* [optional][in] */ VARIANT Queue,
            /* [retval][out] */ IDaCtlRequest **Request) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_ListeningStatus( 
            /* [retval][out] */ ListeningStatusType *ListeningStatus) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Smoothed( 
            /* [retval][out] */ VARIANT_BOOL *Smoothed) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_SmoothEdges( 
            /* [retval][out] */ VARIANT_BOOL *SmoothEdges) = 0;
        
        virtual /* [propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Animations( 
            /* [retval][out] */ SAFEARRAY * *Animations) = 0;
        
        virtual /* [propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_States( 
            /* [retval][out] */ SAFEARRAY * *States) = 0;
        
        virtual /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE put_SuspendPause( 
            /* [in] */ VARIANT_BOOL SuspendPause) = 0;
        
        virtual /* [displaybind][bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_SuspendPause( 
            /* [retval][out] */ VARIANT_BOOL *SuspendPause) = 0;
        
        virtual /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE put_SuspendStop( 
            /* [in] */ VARIANT_BOOL SuspendStop) = 0;
        
        virtual /* [displaybind][bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_SuspendStop( 
            /* [retval][out] */ VARIANT_BOOL *SuspendStop) = 0;
        
        virtual /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE put_SuspendHide( 
            /* [in] */ VARIANT_BOOL SuspendHide) = 0;
        
        virtual /* [displaybind][bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_SuspendHide( 
            /* [retval][out] */ VARIANT_BOOL *SuspendHide) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlCharacter2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlCharacter2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlCharacter2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlCharacter2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlCharacter2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlCharacter2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlCharacter2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlCharacter2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Balloon )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ IDaCtlBalloon2 **Balloon);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Commands )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ IDaCtlCommands2 **Commands);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ BSTR *Name);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Description )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ BSTR *Description);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ VARIANT_BOOL *Visible);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_Left )( 
            IDaCtlCharacter2 * This,
            short Left);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Left )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ short *Left);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_Top )( 
            IDaCtlCharacter2 * This,
            short Top);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Top )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ short *Top);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_Height )( 
            IDaCtlCharacter2 * This,
            short Height);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ short *Height);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_Width )( 
            IDaCtlCharacter2 * This,
            short Width);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ short *Width);
        
        /* [nonbrowsable][hidden][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Speed )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ long *Speed);
        
        /* [nonbrowsable][hidden][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Pitch )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ long *Pitch);
        
        /* [nonbrowsable][hidden][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_IdleOn )( 
            IDaCtlCharacter2 * This,
            /* [in] */ VARIANT_BOOL On);
        
        /* [nonbrowsable][hidden][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_IdleOn )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ VARIANT_BOOL *On);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Activate )( 
            IDaCtlCharacter2 * This,
            /* [optional][in] */ VARIANT State,
            /* [retval][out] */ VARIANT_BOOL *Success);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Play )( 
            IDaCtlCharacter2 * This,
            /* [in] */ BSTR Animation,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Get )( 
            IDaCtlCharacter2 * This,
            /* [in] */ BSTR Type,
            /* [in] */ BSTR Name,
            /* [optional][in] */ VARIANT Queue,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IDaCtlCharacter2 * This,
            /* [optional][in] */ VARIANT Request);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Wait )( 
            IDaCtlCharacter2 * This,
            /* [in] */ IDaCtlRequest *WaitForRequest,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Interrupt )( 
            IDaCtlCharacter2 * This,
            /* [in] */ IDaCtlRequest *InterruptRequest,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Speak )( 
            IDaCtlCharacter2 * This,
            /* [optional][in] */ VARIANT Text,
            /* [optional][in] */ VARIANT Url,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GestureAt )( 
            IDaCtlCharacter2 * This,
            /* [in] */ short X,
            /* [in] */ short Y,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *MoveTo )( 
            IDaCtlCharacter2 * This,
            /* [in] */ short X,
            /* [in] */ short Y,
            /* [optional][in] */ VARIANT Speed,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Hide )( 
            IDaCtlCharacter2 * This,
            /* [optional][in] */ VARIANT Fast,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Show )( 
            IDaCtlCharacter2 * This,
            /* [optional][in] */ VARIANT Fast,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *StopAll )( 
            IDaCtlCharacter2 * This,
            /* [optional][in] */ VARIANT Types);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_MoveCause )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ MoveCauseType *MoveCause);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_VisibilityCause )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ VisibilityCauseType *VisibilityCause);
        
        /* [bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_HasOtherClients )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ VARIANT_BOOL *HasOtherClients);
        
        /* [nonbrowsable][hidden][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_SoundEffectsOn )( 
            IDaCtlCharacter2 * This,
            /* [in] */ VARIANT_BOOL On);
        
        /* [nonbrowsable][hidden][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_SoundEffectsOn )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ VARIANT_BOOL *On);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Name )( 
            IDaCtlCharacter2 * This,
            /* [in] */ BSTR Name);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Description )( 
            IDaCtlCharacter2 * This,
            /* [in] */ BSTR Description);
        
        /* [bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_ExtraData )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ BSTR *ExtraData);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *ShowPopupMenu )( 
            IDaCtlCharacter2 * This,
            /* [in] */ short X,
            /* [in] */ short Y,
            /* [retval][out] */ VARIANT_BOOL *Showed);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_AutoPopupMenu )( 
            IDaCtlCharacter2 * This,
            /* [in] */ VARIANT_BOOL Enabled);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_AutoPopupMenu )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ VARIANT_BOOL *Enabled);
        
        /* [nonbrowsable][restricted][hidden][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_HelpModeOn )( 
            IDaCtlCharacter2 * This,
            /* [in] */ VARIANT_BOOL On);
        
        /* [nonbrowsable][restricted][hidden][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_HelpModeOn )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ VARIANT_BOOL *On);
        
        /* [nonbrowsable][restricted][hidden][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_HelpContextID )( 
            IDaCtlCharacter2 * This,
            /* [in] */ long ID);
        
        /* [nonbrowsable][restricted][hidden][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_HelpContextID )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ long *ID);
        
        /* [nonbrowsable][hidden][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Active )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ short *State);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Listen )( 
            IDaCtlCharacter2 * This,
            /* [in] */ VARIANT_BOOL Listen,
            /* [retval][out] */ VARIANT_BOOL *StartedListening);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_LanguageID )( 
            IDaCtlCharacter2 * This,
            /* [in] */ long LanguageID);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_LanguageID )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ long *LanguageID);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SRModeID )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ BSTR *EngineModeId);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_SRModeID )( 
            IDaCtlCharacter2 * This,
            /* [in] */ BSTR EngineModeId);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_TTSModeID )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ BSTR *EngineModeId);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_TTSModeID )( 
            IDaCtlCharacter2 * This,
            /* [in] */ BSTR EngineModeId);
        
        /* [nonbrowsable][restricted][hidden][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_HelpFile )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ BSTR *File);
        
        /* [nonbrowsable][restricted][hidden][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_HelpFile )( 
            IDaCtlCharacter2 * This,
            /* [in] */ BSTR File);
        
        /* [nonbrowsable][hidden][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_GUID )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ BSTR *CharGUID);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_OriginalHeight )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ short *Height);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_OriginalWidth )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ short *Width);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Think )( 
            IDaCtlCharacter2 * This,
            /* [in] */ BSTR Text,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Version )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ BSTR *Version);
        
        /* [nonbrowsable][hidden][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_AnimationNames )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ IDaCtlAnimationNames **Names);
        
        /* [nonbrowsable][hidden][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SRStatus )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ long *Status);
        
        /* [bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Style )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ long *Style);
        
        /* [bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Style )( 
            IDaCtlCharacter2 * This,
            /* [in] */ long Style);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_HasIcon )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ VARIANT_BOOL *HasIcon);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GenerateIcon )( 
            IDaCtlCharacter2 * This,
            /* [defaultvalue][in] */ long ClipLeft,
            /* [defaultvalue][in] */ long ClipTop,
            /* [defaultvalue][in] */ long ClipWidth,
            /* [defaultvalue][in] */ long ClipHeight);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_IconShown )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ VARIANT_BOOL *IconShown);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_IconShown )( 
            IDaCtlCharacter2 * This,
            /* [in] */ VARIANT_BOOL IconShown);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_IconVisible )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ VARIANT_BOOL *IconVisible);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_IconIdentity )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ BSTR *IconIdentity);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_IconIdentity )( 
            IDaCtlCharacter2 * This,
            /* [in] */ BSTR IconIdentity);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_IconTip )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ BSTR *IconTip);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_IconTip )( 
            IDaCtlCharacter2 * This,
            /* [in] */ BSTR IconTip);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SpeakFormatted )( 
            IDaCtlCharacter2 * This,
            /* [in] */ IDaCtlFormattedText *FormattedText,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *ThinkFormatted )( 
            IDaCtlCharacter2 * This,
            /* [in] */ IDaCtlFormattedText *FormattedText,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *NewFormattedText )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ IDaCtlFormattedText **FormattedText);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_TTSEngine )( 
            IDaCtlCharacter2 * This,
            /* [optional][in] */ VARIANT GetDefault,
            /* [retval][out] */ IDaCtlTTSEngine **TTSEngine);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *FindTTSEngines )( 
            IDaCtlCharacter2 * This,
            /* [optional][in] */ VARIANT LanguageID,
            /* [retval][out] */ IDaCtlTTSEngines **TTSEngines);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SREngine )( 
            IDaCtlCharacter2 * This,
            /* [optional][in] */ VARIANT GetDefault,
            /* [retval][out] */ IDaCtlSREngine **SREngine);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *FindSREngines )( 
            IDaCtlCharacter2 * This,
            /* [optional][in] */ VARIANT LanguageID,
            /* [retval][out] */ IDaCtlSREngines **SREngines);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_UniqueID )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ BSTR *CharGUID);
        
        /* [defaultbind][displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_CharacterID )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ BSTR *CharacterID);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Connected )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ VARIANT_BOOL *Connected);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_FileName )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ BSTR *FileName);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_FilePath )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ BSTR *FilePath);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_ActiveState )( 
            IDaCtlCharacter2 * This,
            /* [in] */ ActiveStateType ActiveState);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_ActiveState )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ ActiveStateType *ActiveState);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_IdleState )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ VARIANT_BOOL *IdleState);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_IdleEnabled )( 
            IDaCtlCharacter2 * This,
            /* [in] */ VARIANT_BOOL Enabled);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_IdleEnabled )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ VARIANT_BOOL *Enabled);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_SoundEffectsEnabled )( 
            IDaCtlCharacter2 * This,
            /* [in] */ VARIANT_BOOL Enabled);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_SoundEffectsEnabled )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ VARIANT_BOOL *Enabled);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Prepare )( 
            IDaCtlCharacter2 * This,
            /* [in] */ PrepareResourceType ResourceType,
            /* [in] */ BSTR Name,
            /* [optional][in] */ VARIANT Queue,
            /* [retval][out] */ IDaCtlRequest **Request);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_ListeningStatus )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ ListeningStatusType *ListeningStatus);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Smoothed )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ VARIANT_BOOL *Smoothed);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SmoothEdges )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ VARIANT_BOOL *SmoothEdges);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Animations )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ SAFEARRAY * *Animations);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_States )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ SAFEARRAY * *States);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_SuspendPause )( 
            IDaCtlCharacter2 * This,
            /* [in] */ VARIANT_BOOL SuspendPause);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_SuspendPause )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ VARIANT_BOOL *SuspendPause);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_SuspendStop )( 
            IDaCtlCharacter2 * This,
            /* [in] */ VARIANT_BOOL SuspendStop);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_SuspendStop )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ VARIANT_BOOL *SuspendStop);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_SuspendHide )( 
            IDaCtlCharacter2 * This,
            /* [in] */ VARIANT_BOOL SuspendHide);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_SuspendHide )( 
            IDaCtlCharacter2 * This,
            /* [retval][out] */ VARIANT_BOOL *SuspendHide);
        
        END_INTERFACE
    } IDaCtlCharacter2Vtbl;

    interface IDaCtlCharacter2
    {
        CONST_VTBL struct IDaCtlCharacter2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlCharacter2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlCharacter2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlCharacter2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlCharacter2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlCharacter2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlCharacter2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlCharacter2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlCharacter2_get_Balloon(This,Balloon)	\
    ( (This)->lpVtbl -> get_Balloon(This,Balloon) ) 

#define IDaCtlCharacter2_get_Commands(This,Commands)	\
    ( (This)->lpVtbl -> get_Commands(This,Commands) ) 

#define IDaCtlCharacter2_get_Name(This,Name)	\
    ( (This)->lpVtbl -> get_Name(This,Name) ) 

#define IDaCtlCharacter2_get_Description(This,Description)	\
    ( (This)->lpVtbl -> get_Description(This,Description) ) 

#define IDaCtlCharacter2_get_Visible(This,Visible)	\
    ( (This)->lpVtbl -> get_Visible(This,Visible) ) 

#define IDaCtlCharacter2_put_Left(This,Left)	\
    ( (This)->lpVtbl -> put_Left(This,Left) ) 

#define IDaCtlCharacter2_get_Left(This,Left)	\
    ( (This)->lpVtbl -> get_Left(This,Left) ) 

#define IDaCtlCharacter2_put_Top(This,Top)	\
    ( (This)->lpVtbl -> put_Top(This,Top) ) 

#define IDaCtlCharacter2_get_Top(This,Top)	\
    ( (This)->lpVtbl -> get_Top(This,Top) ) 

#define IDaCtlCharacter2_put_Height(This,Height)	\
    ( (This)->lpVtbl -> put_Height(This,Height) ) 

#define IDaCtlCharacter2_get_Height(This,Height)	\
    ( (This)->lpVtbl -> get_Height(This,Height) ) 

#define IDaCtlCharacter2_put_Width(This,Width)	\
    ( (This)->lpVtbl -> put_Width(This,Width) ) 

#define IDaCtlCharacter2_get_Width(This,Width)	\
    ( (This)->lpVtbl -> get_Width(This,Width) ) 

#define IDaCtlCharacter2_get_Speed(This,Speed)	\
    ( (This)->lpVtbl -> get_Speed(This,Speed) ) 

#define IDaCtlCharacter2_get_Pitch(This,Pitch)	\
    ( (This)->lpVtbl -> get_Pitch(This,Pitch) ) 

#define IDaCtlCharacter2_put_IdleOn(This,On)	\
    ( (This)->lpVtbl -> put_IdleOn(This,On) ) 

#define IDaCtlCharacter2_get_IdleOn(This,On)	\
    ( (This)->lpVtbl -> get_IdleOn(This,On) ) 

#define IDaCtlCharacter2_Activate(This,State,Success)	\
    ( (This)->lpVtbl -> Activate(This,State,Success) ) 

#define IDaCtlCharacter2_Play(This,Animation,Request)	\
    ( (This)->lpVtbl -> Play(This,Animation,Request) ) 

#define IDaCtlCharacter2_Get(This,Type,Name,Queue,Request)	\
    ( (This)->lpVtbl -> Get(This,Type,Name,Queue,Request) ) 

#define IDaCtlCharacter2_Stop(This,Request)	\
    ( (This)->lpVtbl -> Stop(This,Request) ) 

#define IDaCtlCharacter2_Wait(This,WaitForRequest,Request)	\
    ( (This)->lpVtbl -> Wait(This,WaitForRequest,Request) ) 

#define IDaCtlCharacter2_Interrupt(This,InterruptRequest,Request)	\
    ( (This)->lpVtbl -> Interrupt(This,InterruptRequest,Request) ) 

#define IDaCtlCharacter2_Speak(This,Text,Url,Request)	\
    ( (This)->lpVtbl -> Speak(This,Text,Url,Request) ) 

#define IDaCtlCharacter2_GestureAt(This,X,Y,Request)	\
    ( (This)->lpVtbl -> GestureAt(This,X,Y,Request) ) 

#define IDaCtlCharacter2_MoveTo(This,X,Y,Speed,Request)	\
    ( (This)->lpVtbl -> MoveTo(This,X,Y,Speed,Request) ) 

#define IDaCtlCharacter2_Hide(This,Fast,Request)	\
    ( (This)->lpVtbl -> Hide(This,Fast,Request) ) 

#define IDaCtlCharacter2_Show(This,Fast,Request)	\
    ( (This)->lpVtbl -> Show(This,Fast,Request) ) 

#define IDaCtlCharacter2_StopAll(This,Types)	\
    ( (This)->lpVtbl -> StopAll(This,Types) ) 

#define IDaCtlCharacter2_get_MoveCause(This,MoveCause)	\
    ( (This)->lpVtbl -> get_MoveCause(This,MoveCause) ) 

#define IDaCtlCharacter2_get_VisibilityCause(This,VisibilityCause)	\
    ( (This)->lpVtbl -> get_VisibilityCause(This,VisibilityCause) ) 

#define IDaCtlCharacter2_get_HasOtherClients(This,HasOtherClients)	\
    ( (This)->lpVtbl -> get_HasOtherClients(This,HasOtherClients) ) 

#define IDaCtlCharacter2_put_SoundEffectsOn(This,On)	\
    ( (This)->lpVtbl -> put_SoundEffectsOn(This,On) ) 

#define IDaCtlCharacter2_get_SoundEffectsOn(This,On)	\
    ( (This)->lpVtbl -> get_SoundEffectsOn(This,On) ) 

#define IDaCtlCharacter2_put_Name(This,Name)	\
    ( (This)->lpVtbl -> put_Name(This,Name) ) 

#define IDaCtlCharacter2_put_Description(This,Description)	\
    ( (This)->lpVtbl -> put_Description(This,Description) ) 

#define IDaCtlCharacter2_get_ExtraData(This,ExtraData)	\
    ( (This)->lpVtbl -> get_ExtraData(This,ExtraData) ) 

#define IDaCtlCharacter2_ShowPopupMenu(This,X,Y,Showed)	\
    ( (This)->lpVtbl -> ShowPopupMenu(This,X,Y,Showed) ) 

#define IDaCtlCharacter2_put_AutoPopupMenu(This,Enabled)	\
    ( (This)->lpVtbl -> put_AutoPopupMenu(This,Enabled) ) 

#define IDaCtlCharacter2_get_AutoPopupMenu(This,Enabled)	\
    ( (This)->lpVtbl -> get_AutoPopupMenu(This,Enabled) ) 

#define IDaCtlCharacter2_put_HelpModeOn(This,On)	\
    ( (This)->lpVtbl -> put_HelpModeOn(This,On) ) 

#define IDaCtlCharacter2_get_HelpModeOn(This,On)	\
    ( (This)->lpVtbl -> get_HelpModeOn(This,On) ) 

#define IDaCtlCharacter2_put_HelpContextID(This,ID)	\
    ( (This)->lpVtbl -> put_HelpContextID(This,ID) ) 

#define IDaCtlCharacter2_get_HelpContextID(This,ID)	\
    ( (This)->lpVtbl -> get_HelpContextID(This,ID) ) 

#define IDaCtlCharacter2_get_Active(This,State)	\
    ( (This)->lpVtbl -> get_Active(This,State) ) 

#define IDaCtlCharacter2_Listen(This,Listen,StartedListening)	\
    ( (This)->lpVtbl -> Listen(This,Listen,StartedListening) ) 

#define IDaCtlCharacter2_put_LanguageID(This,LanguageID)	\
    ( (This)->lpVtbl -> put_LanguageID(This,LanguageID) ) 

#define IDaCtlCharacter2_get_LanguageID(This,LanguageID)	\
    ( (This)->lpVtbl -> get_LanguageID(This,LanguageID) ) 

#define IDaCtlCharacter2_get_SRModeID(This,EngineModeId)	\
    ( (This)->lpVtbl -> get_SRModeID(This,EngineModeId) ) 

#define IDaCtlCharacter2_put_SRModeID(This,EngineModeId)	\
    ( (This)->lpVtbl -> put_SRModeID(This,EngineModeId) ) 

#define IDaCtlCharacter2_get_TTSModeID(This,EngineModeId)	\
    ( (This)->lpVtbl -> get_TTSModeID(This,EngineModeId) ) 

#define IDaCtlCharacter2_put_TTSModeID(This,EngineModeId)	\
    ( (This)->lpVtbl -> put_TTSModeID(This,EngineModeId) ) 

#define IDaCtlCharacter2_get_HelpFile(This,File)	\
    ( (This)->lpVtbl -> get_HelpFile(This,File) ) 

#define IDaCtlCharacter2_put_HelpFile(This,File)	\
    ( (This)->lpVtbl -> put_HelpFile(This,File) ) 

#define IDaCtlCharacter2_get_GUID(This,CharGUID)	\
    ( (This)->lpVtbl -> get_GUID(This,CharGUID) ) 

#define IDaCtlCharacter2_get_OriginalHeight(This,Height)	\
    ( (This)->lpVtbl -> get_OriginalHeight(This,Height) ) 

#define IDaCtlCharacter2_get_OriginalWidth(This,Width)	\
    ( (This)->lpVtbl -> get_OriginalWidth(This,Width) ) 

#define IDaCtlCharacter2_Think(This,Text,Request)	\
    ( (This)->lpVtbl -> Think(This,Text,Request) ) 

#define IDaCtlCharacter2_get_Version(This,Version)	\
    ( (This)->lpVtbl -> get_Version(This,Version) ) 

#define IDaCtlCharacter2_get_AnimationNames(This,Names)	\
    ( (This)->lpVtbl -> get_AnimationNames(This,Names) ) 

#define IDaCtlCharacter2_get_SRStatus(This,Status)	\
    ( (This)->lpVtbl -> get_SRStatus(This,Status) ) 


#define IDaCtlCharacter2_get_Style(This,Style)	\
    ( (This)->lpVtbl -> get_Style(This,Style) ) 

#define IDaCtlCharacter2_put_Style(This,Style)	\
    ( (This)->lpVtbl -> put_Style(This,Style) ) 

#define IDaCtlCharacter2_get_HasIcon(This,HasIcon)	\
    ( (This)->lpVtbl -> get_HasIcon(This,HasIcon) ) 

#define IDaCtlCharacter2_GenerateIcon(This,ClipLeft,ClipTop,ClipWidth,ClipHeight)	\
    ( (This)->lpVtbl -> GenerateIcon(This,ClipLeft,ClipTop,ClipWidth,ClipHeight) ) 

#define IDaCtlCharacter2_get_IconShown(This,IconShown)	\
    ( (This)->lpVtbl -> get_IconShown(This,IconShown) ) 

#define IDaCtlCharacter2_put_IconShown(This,IconShown)	\
    ( (This)->lpVtbl -> put_IconShown(This,IconShown) ) 

#define IDaCtlCharacter2_get_IconVisible(This,IconVisible)	\
    ( (This)->lpVtbl -> get_IconVisible(This,IconVisible) ) 

#define IDaCtlCharacter2_get_IconIdentity(This,IconIdentity)	\
    ( (This)->lpVtbl -> get_IconIdentity(This,IconIdentity) ) 

#define IDaCtlCharacter2_put_IconIdentity(This,IconIdentity)	\
    ( (This)->lpVtbl -> put_IconIdentity(This,IconIdentity) ) 

#define IDaCtlCharacter2_get_IconTip(This,IconTip)	\
    ( (This)->lpVtbl -> get_IconTip(This,IconTip) ) 

#define IDaCtlCharacter2_put_IconTip(This,IconTip)	\
    ( (This)->lpVtbl -> put_IconTip(This,IconTip) ) 

#define IDaCtlCharacter2_SpeakFormatted(This,FormattedText,Request)	\
    ( (This)->lpVtbl -> SpeakFormatted(This,FormattedText,Request) ) 

#define IDaCtlCharacter2_ThinkFormatted(This,FormattedText,Request)	\
    ( (This)->lpVtbl -> ThinkFormatted(This,FormattedText,Request) ) 

#define IDaCtlCharacter2_NewFormattedText(This,FormattedText)	\
    ( (This)->lpVtbl -> NewFormattedText(This,FormattedText) ) 

#define IDaCtlCharacter2_get_TTSEngine(This,GetDefault,TTSEngine)	\
    ( (This)->lpVtbl -> get_TTSEngine(This,GetDefault,TTSEngine) ) 

#define IDaCtlCharacter2_FindTTSEngines(This,LanguageID,TTSEngines)	\
    ( (This)->lpVtbl -> FindTTSEngines(This,LanguageID,TTSEngines) ) 

#define IDaCtlCharacter2_get_SREngine(This,GetDefault,SREngine)	\
    ( (This)->lpVtbl -> get_SREngine(This,GetDefault,SREngine) ) 

#define IDaCtlCharacter2_FindSREngines(This,LanguageID,SREngines)	\
    ( (This)->lpVtbl -> FindSREngines(This,LanguageID,SREngines) ) 

#define IDaCtlCharacter2_get_UniqueID(This,CharGUID)	\
    ( (This)->lpVtbl -> get_UniqueID(This,CharGUID) ) 

#define IDaCtlCharacter2_get_CharacterID(This,CharacterID)	\
    ( (This)->lpVtbl -> get_CharacterID(This,CharacterID) ) 

#define IDaCtlCharacter2_get_Connected(This,Connected)	\
    ( (This)->lpVtbl -> get_Connected(This,Connected) ) 

#define IDaCtlCharacter2_get_FileName(This,FileName)	\
    ( (This)->lpVtbl -> get_FileName(This,FileName) ) 

#define IDaCtlCharacter2_get_FilePath(This,FilePath)	\
    ( (This)->lpVtbl -> get_FilePath(This,FilePath) ) 

#define IDaCtlCharacter2_put_ActiveState(This,ActiveState)	\
    ( (This)->lpVtbl -> put_ActiveState(This,ActiveState) ) 

#define IDaCtlCharacter2_get_ActiveState(This,ActiveState)	\
    ( (This)->lpVtbl -> get_ActiveState(This,ActiveState) ) 

#define IDaCtlCharacter2_get_IdleState(This,IdleState)	\
    ( (This)->lpVtbl -> get_IdleState(This,IdleState) ) 

#define IDaCtlCharacter2_put_IdleEnabled(This,Enabled)	\
    ( (This)->lpVtbl -> put_IdleEnabled(This,Enabled) ) 

#define IDaCtlCharacter2_get_IdleEnabled(This,Enabled)	\
    ( (This)->lpVtbl -> get_IdleEnabled(This,Enabled) ) 

#define IDaCtlCharacter2_put_SoundEffectsEnabled(This,Enabled)	\
    ( (This)->lpVtbl -> put_SoundEffectsEnabled(This,Enabled) ) 

#define IDaCtlCharacter2_get_SoundEffectsEnabled(This,Enabled)	\
    ( (This)->lpVtbl -> get_SoundEffectsEnabled(This,Enabled) ) 

#define IDaCtlCharacter2_Prepare(This,ResourceType,Name,Queue,Request)	\
    ( (This)->lpVtbl -> Prepare(This,ResourceType,Name,Queue,Request) ) 

#define IDaCtlCharacter2_get_ListeningStatus(This,ListeningStatus)	\
    ( (This)->lpVtbl -> get_ListeningStatus(This,ListeningStatus) ) 

#define IDaCtlCharacter2_get_Smoothed(This,Smoothed)	\
    ( (This)->lpVtbl -> get_Smoothed(This,Smoothed) ) 

#define IDaCtlCharacter2_get_SmoothEdges(This,SmoothEdges)	\
    ( (This)->lpVtbl -> get_SmoothEdges(This,SmoothEdges) ) 

#define IDaCtlCharacter2_get_Animations(This,Animations)	\
    ( (This)->lpVtbl -> get_Animations(This,Animations) ) 

#define IDaCtlCharacter2_get_States(This,States)	\
    ( (This)->lpVtbl -> get_States(This,States) ) 

#define IDaCtlCharacter2_put_SuspendPause(This,SuspendPause)	\
    ( (This)->lpVtbl -> put_SuspendPause(This,SuspendPause) ) 

#define IDaCtlCharacter2_get_SuspendPause(This,SuspendPause)	\
    ( (This)->lpVtbl -> get_SuspendPause(This,SuspendPause) ) 

#define IDaCtlCharacter2_put_SuspendStop(This,SuspendStop)	\
    ( (This)->lpVtbl -> put_SuspendStop(This,SuspendStop) ) 

#define IDaCtlCharacter2_get_SuspendStop(This,SuspendStop)	\
    ( (This)->lpVtbl -> get_SuspendStop(This,SuspendStop) ) 

#define IDaCtlCharacter2_put_SuspendHide(This,SuspendHide)	\
    ( (This)->lpVtbl -> put_SuspendHide(This,SuspendHide) ) 

#define IDaCtlCharacter2_get_SuspendHide(This,SuspendHide)	\
    ( (This)->lpVtbl -> get_SuspendHide(This,SuspendHide) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlCharacter2_INTERFACE_DEFINED__ */


#ifndef __IDaCtlCharacters2_INTERFACE_DEFINED__
#define __IDaCtlCharacters2_INTERFACE_DEFINED__

/* interface IDaCtlCharacters2 */
/* [object][helpcontext][nonextensible][oleautomation][dual][unique][uuid] */ 


EXTERN_C const IID IID_IDaCtlCharacters2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E559-A208-11DE-ABF2-002421116FB2")
    IDaCtlCharacters2 : public IDaCtlCharacters
    {
    public:
        virtual /* [defaultbind][displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *Count) = 0;
        
        virtual /* [propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Index( 
            /* [in] */ long Index,
            /* [retval][out] */ IDaCtlCharacter2 **Character) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlCharacters2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlCharacters2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlCharacters2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlCharacters2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlCharacters2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlCharacters2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlCharacters2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlCharacters2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IDaCtlCharacters2 * This,
            /* [in] */ BSTR CharacterID,
            /* [retval][out] */ IDaCtlCharacter2 **Item);
        
        /* [hidden][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Character )( 
            IDaCtlCharacters2 * This,
            /* [in] */ BSTR CharacterID,
            /* [retval][out] */ IDaCtlCharacter2 **Character);
        
        /* [restricted][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IDaCtlCharacters2 * This,
            /* [retval][out] */ IUnknown **EnumVariant);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Unload )( 
            IDaCtlCharacters2 * This,
            /* [in] */ BSTR CharacterID);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Load )( 
            IDaCtlCharacters2 * This,
            /* [in] */ BSTR CharacterID,
            /* [optional][in] */ VARIANT Provider,
            /* [retval][out] */ IDaCtlRequest **ppidRequest);
        
        /* [defaultbind][displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IDaCtlCharacters2 * This,
            /* [retval][out] */ long *Count);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Index )( 
            IDaCtlCharacters2 * This,
            /* [in] */ long Index,
            /* [retval][out] */ IDaCtlCharacter2 **Character);
        
        END_INTERFACE
    } IDaCtlCharacters2Vtbl;

    interface IDaCtlCharacters2
    {
        CONST_VTBL struct IDaCtlCharacters2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlCharacters2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlCharacters2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlCharacters2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlCharacters2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlCharacters2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlCharacters2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlCharacters2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlCharacters2_get_Item(This,CharacterID,Item)	\
    ( (This)->lpVtbl -> get_Item(This,CharacterID,Item) ) 

#define IDaCtlCharacters2_Character(This,CharacterID,Character)	\
    ( (This)->lpVtbl -> Character(This,CharacterID,Character) ) 

#define IDaCtlCharacters2_get__NewEnum(This,EnumVariant)	\
    ( (This)->lpVtbl -> get__NewEnum(This,EnumVariant) ) 

#define IDaCtlCharacters2_Unload(This,CharacterID)	\
    ( (This)->lpVtbl -> Unload(This,CharacterID) ) 

#define IDaCtlCharacters2_Load(This,CharacterID,Provider,ppidRequest)	\
    ( (This)->lpVtbl -> Load(This,CharacterID,Provider,ppidRequest) ) 


#define IDaCtlCharacters2_get_Count(This,Count)	\
    ( (This)->lpVtbl -> get_Count(This,Count) ) 

#define IDaCtlCharacters2_get_Index(This,Index,Character)	\
    ( (This)->lpVtbl -> get_Index(This,Index,Character) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlCharacters2_INTERFACE_DEFINED__ */


#ifndef __IDaCtlBalloon2_INTERFACE_DEFINED__
#define __IDaCtlBalloon2_INTERFACE_DEFINED__

/* interface IDaCtlBalloon2 */
/* [object][helpcontext][nonextensible][oleautomation][dual][unique][uuid] */ 


EXTERN_C const IID IID_IDaCtlBalloon2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E55A-A208-11DE-ABF2-002421116FB2")
    IDaCtlBalloon2 : public IDaCtlBalloon
    {
    public:
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_NumberOfLines( 
            /* [retval][out] */ long *NumberOfLines) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_NumberOfLines( 
            /* [in] */ long NumberOfLines) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_CharsPerLine( 
            /* [retval][out] */ long *CharsPerLine) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_CharsPerLine( 
            /* [in] */ long CharsPerLine) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_TextColor( 
            /* [retval][out] */ /* external definition not present */ OLE_COLOR *TextColor) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_TextColor( 
            /* [in] */ /* external definition not present */ OLE_COLOR TextColor) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_BackColor( 
            /* [retval][out] */ /* external definition not present */ OLE_COLOR *BackColor) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_BackColor( 
            /* [in] */ /* external definition not present */ OLE_COLOR BackColor) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_BorderColor( 
            /* [retval][out] */ /* external definition not present */ OLE_COLOR *BorderColor) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_BorderColor( 
            /* [in] */ /* external definition not present */ OLE_COLOR BorderColor) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_FontBold( 
            /* [retval][out] */ VARIANT_BOOL *FontBold) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_FontBold( 
            /* [in] */ VARIANT_BOOL FontBold) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_FontItalic( 
            /* [retval][out] */ VARIANT_BOOL *FontItalic) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_FontItalic( 
            /* [in] */ VARIANT_BOOL FontItalic) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_FontStrikethru( 
            /* [retval][out] */ VARIANT_BOOL *FontStrikethru) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_FontStrikethru( 
            /* [in] */ VARIANT_BOOL FontStrikethru) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_FontUnderline( 
            /* [retval][out] */ VARIANT_BOOL *FontUnderline) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_FontUnderline( 
            /* [in] */ VARIANT_BOOL FontUnderline) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_SizeToText( 
            /* [retval][out] */ VARIANT_BOOL *SizeToText) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_SizeToText( 
            /* [in] */ VARIANT_BOOL SizeToText) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_AutoHide( 
            /* [retval][out] */ VARIANT_BOOL *AutoHide) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_AutoHide( 
            /* [in] */ VARIANT_BOOL AutoHide) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_AutoPace( 
            /* [retval][out] */ VARIANT_BOOL *AutoPace) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_AutoPace( 
            /* [in] */ VARIANT_BOOL AutoPace) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_NoAppend( 
            /* [retval][out] */ VARIANT_BOOL *NoAppend) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_NoAppend( 
            /* [in] */ VARIANT_BOOL NoAppend) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_ShowPartialLines( 
            /* [retval][out] */ VARIANT_BOOL *ShowPartialLines) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_ShowPartialLines( 
            /* [in] */ VARIANT_BOOL ShowPartialLines) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlBalloon2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlBalloon2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlBalloon2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlBalloon2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlBalloon2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlBalloon2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlBalloon2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlBalloon2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ VARIANT_BOOL *Enabled);
        
        /* [nonbrowsable][restricted][hidden][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NumberOfLines )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ long *Lines);
        
        /* [nonbrowsable][restricted][hidden][propget] */ HRESULT ( STDMETHODCALLTYPE *get__CharsPerLine )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ long *CharsPerLine);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_FontName )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ BSTR *FontName);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_FontSize )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ long *FontSize);
        
        /* [nonbrowsable][restricted][hidden][propget] */ HRESULT ( STDMETHODCALLTYPE *get__FontBold )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ VARIANT_BOOL *FontBold);
        
        /* [nonbrowsable][restricted][hidden][propget] */ HRESULT ( STDMETHODCALLTYPE *get__FontItalic )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ VARIANT_BOOL *FontItalic);
        
        /* [nonbrowsable][restricted][hidden][propget] */ HRESULT ( STDMETHODCALLTYPE *get__FontStrikethru )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ VARIANT_BOOL *FontStrikethru);
        
        /* [nonbrowsable][restricted][hidden][propget] */ HRESULT ( STDMETHODCALLTYPE *get__FontUnderline )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ VARIANT_BOOL *FontUnderline);
        
        /* [nonbrowsable][hidden][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_ForeColor )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ long *ForeColor);
        
        /* [nonbrowsable][restricted][hidden][propget] */ HRESULT ( STDMETHODCALLTYPE *get__BackColor )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ long *BackColor);
        
        /* [nonbrowsable][restricted][hidden][propget] */ HRESULT ( STDMETHODCALLTYPE *get__BorderColor )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ long *BorderColor);
        
        /* [defaultbind][displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IDaCtlBalloon2 * This,
            /* [in] */ VARIANT_BOOL Visible);
        
        /* [defaultbind][displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ VARIANT_BOOL *Visible);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FontName )( 
            IDaCtlBalloon2 * This,
            /* [in] */ BSTR FontName);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FontSize )( 
            IDaCtlBalloon2 * This,
            /* [in] */ long FontSize);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_FontCharSet )( 
            IDaCtlBalloon2 * This,
            /* [in] */ short FontCharSet);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_FontCharSet )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ short *FontCharSet);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_Style )( 
            IDaCtlBalloon2 * This,
            /* [in] */ long Style);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Style )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ long *Style);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_NumberOfLines )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ long *NumberOfLines);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_NumberOfLines )( 
            IDaCtlBalloon2 * This,
            /* [in] */ long NumberOfLines);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_CharsPerLine )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ long *CharsPerLine);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_CharsPerLine )( 
            IDaCtlBalloon2 * This,
            /* [in] */ long CharsPerLine);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_TextColor )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ /* external definition not present */ OLE_COLOR *TextColor);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_TextColor )( 
            IDaCtlBalloon2 * This,
            /* [in] */ /* external definition not present */ OLE_COLOR TextColor);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_BackColor )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ /* external definition not present */ OLE_COLOR *BackColor);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_BackColor )( 
            IDaCtlBalloon2 * This,
            /* [in] */ /* external definition not present */ OLE_COLOR BackColor);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_BorderColor )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ /* external definition not present */ OLE_COLOR *BorderColor);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_BorderColor )( 
            IDaCtlBalloon2 * This,
            /* [in] */ /* external definition not present */ OLE_COLOR BorderColor);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_FontBold )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ VARIANT_BOOL *FontBold);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FontBold )( 
            IDaCtlBalloon2 * This,
            /* [in] */ VARIANT_BOOL FontBold);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_FontItalic )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ VARIANT_BOOL *FontItalic);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FontItalic )( 
            IDaCtlBalloon2 * This,
            /* [in] */ VARIANT_BOOL FontItalic);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_FontStrikethru )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ VARIANT_BOOL *FontStrikethru);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FontStrikethru )( 
            IDaCtlBalloon2 * This,
            /* [in] */ VARIANT_BOOL FontStrikethru);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_FontUnderline )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ VARIANT_BOOL *FontUnderline);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FontUnderline )( 
            IDaCtlBalloon2 * This,
            /* [in] */ VARIANT_BOOL FontUnderline);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SizeToText )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ VARIANT_BOOL *SizeToText);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_SizeToText )( 
            IDaCtlBalloon2 * This,
            /* [in] */ VARIANT_BOOL SizeToText);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_AutoHide )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ VARIANT_BOOL *AutoHide);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_AutoHide )( 
            IDaCtlBalloon2 * This,
            /* [in] */ VARIANT_BOOL AutoHide);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_AutoPace )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ VARIANT_BOOL *AutoPace);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_AutoPace )( 
            IDaCtlBalloon2 * This,
            /* [in] */ VARIANT_BOOL AutoPace);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_NoAppend )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ VARIANT_BOOL *NoAppend);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_NoAppend )( 
            IDaCtlBalloon2 * This,
            /* [in] */ VARIANT_BOOL NoAppend);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_ShowPartialLines )( 
            IDaCtlBalloon2 * This,
            /* [retval][out] */ VARIANT_BOOL *ShowPartialLines);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ShowPartialLines )( 
            IDaCtlBalloon2 * This,
            /* [in] */ VARIANT_BOOL ShowPartialLines);
        
        END_INTERFACE
    } IDaCtlBalloon2Vtbl;

    interface IDaCtlBalloon2
    {
        CONST_VTBL struct IDaCtlBalloon2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlBalloon2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlBalloon2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlBalloon2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlBalloon2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlBalloon2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlBalloon2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlBalloon2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlBalloon2_get_Enabled(This,Enabled)	\
    ( (This)->lpVtbl -> get_Enabled(This,Enabled) ) 

#define IDaCtlBalloon2_get__NumberOfLines(This,Lines)	\
    ( (This)->lpVtbl -> get__NumberOfLines(This,Lines) ) 

#define IDaCtlBalloon2_get__CharsPerLine(This,CharsPerLine)	\
    ( (This)->lpVtbl -> get__CharsPerLine(This,CharsPerLine) ) 

#define IDaCtlBalloon2_get_FontName(This,FontName)	\
    ( (This)->lpVtbl -> get_FontName(This,FontName) ) 

#define IDaCtlBalloon2_get_FontSize(This,FontSize)	\
    ( (This)->lpVtbl -> get_FontSize(This,FontSize) ) 

#define IDaCtlBalloon2_get__FontBold(This,FontBold)	\
    ( (This)->lpVtbl -> get__FontBold(This,FontBold) ) 

#define IDaCtlBalloon2_get__FontItalic(This,FontItalic)	\
    ( (This)->lpVtbl -> get__FontItalic(This,FontItalic) ) 

#define IDaCtlBalloon2_get__FontStrikethru(This,FontStrikethru)	\
    ( (This)->lpVtbl -> get__FontStrikethru(This,FontStrikethru) ) 

#define IDaCtlBalloon2_get__FontUnderline(This,FontUnderline)	\
    ( (This)->lpVtbl -> get__FontUnderline(This,FontUnderline) ) 

#define IDaCtlBalloon2_get_ForeColor(This,ForeColor)	\
    ( (This)->lpVtbl -> get_ForeColor(This,ForeColor) ) 

#define IDaCtlBalloon2_get__BackColor(This,BackColor)	\
    ( (This)->lpVtbl -> get__BackColor(This,BackColor) ) 

#define IDaCtlBalloon2_get__BorderColor(This,BorderColor)	\
    ( (This)->lpVtbl -> get__BorderColor(This,BorderColor) ) 

#define IDaCtlBalloon2_put_Visible(This,Visible)	\
    ( (This)->lpVtbl -> put_Visible(This,Visible) ) 

#define IDaCtlBalloon2_get_Visible(This,Visible)	\
    ( (This)->lpVtbl -> get_Visible(This,Visible) ) 

#define IDaCtlBalloon2_put_FontName(This,FontName)	\
    ( (This)->lpVtbl -> put_FontName(This,FontName) ) 

#define IDaCtlBalloon2_put_FontSize(This,FontSize)	\
    ( (This)->lpVtbl -> put_FontSize(This,FontSize) ) 

#define IDaCtlBalloon2_put_FontCharSet(This,FontCharSet)	\
    ( (This)->lpVtbl -> put_FontCharSet(This,FontCharSet) ) 

#define IDaCtlBalloon2_get_FontCharSet(This,FontCharSet)	\
    ( (This)->lpVtbl -> get_FontCharSet(This,FontCharSet) ) 

#define IDaCtlBalloon2_put_Style(This,Style)	\
    ( (This)->lpVtbl -> put_Style(This,Style) ) 

#define IDaCtlBalloon2_get_Style(This,Style)	\
    ( (This)->lpVtbl -> get_Style(This,Style) ) 


#define IDaCtlBalloon2_get_NumberOfLines(This,NumberOfLines)	\
    ( (This)->lpVtbl -> get_NumberOfLines(This,NumberOfLines) ) 

#define IDaCtlBalloon2_put_NumberOfLines(This,NumberOfLines)	\
    ( (This)->lpVtbl -> put_NumberOfLines(This,NumberOfLines) ) 

#define IDaCtlBalloon2_get_CharsPerLine(This,CharsPerLine)	\
    ( (This)->lpVtbl -> get_CharsPerLine(This,CharsPerLine) ) 

#define IDaCtlBalloon2_put_CharsPerLine(This,CharsPerLine)	\
    ( (This)->lpVtbl -> put_CharsPerLine(This,CharsPerLine) ) 

#define IDaCtlBalloon2_get_TextColor(This,TextColor)	\
    ( (This)->lpVtbl -> get_TextColor(This,TextColor) ) 

#define IDaCtlBalloon2_put_TextColor(This,TextColor)	\
    ( (This)->lpVtbl -> put_TextColor(This,TextColor) ) 

#define IDaCtlBalloon2_get_BackColor(This,BackColor)	\
    ( (This)->lpVtbl -> get_BackColor(This,BackColor) ) 

#define IDaCtlBalloon2_put_BackColor(This,BackColor)	\
    ( (This)->lpVtbl -> put_BackColor(This,BackColor) ) 

#define IDaCtlBalloon2_get_BorderColor(This,BorderColor)	\
    ( (This)->lpVtbl -> get_BorderColor(This,BorderColor) ) 

#define IDaCtlBalloon2_put_BorderColor(This,BorderColor)	\
    ( (This)->lpVtbl -> put_BorderColor(This,BorderColor) ) 

#define IDaCtlBalloon2_get_FontBold(This,FontBold)	\
    ( (This)->lpVtbl -> get_FontBold(This,FontBold) ) 

#define IDaCtlBalloon2_put_FontBold(This,FontBold)	\
    ( (This)->lpVtbl -> put_FontBold(This,FontBold) ) 

#define IDaCtlBalloon2_get_FontItalic(This,FontItalic)	\
    ( (This)->lpVtbl -> get_FontItalic(This,FontItalic) ) 

#define IDaCtlBalloon2_put_FontItalic(This,FontItalic)	\
    ( (This)->lpVtbl -> put_FontItalic(This,FontItalic) ) 

#define IDaCtlBalloon2_get_FontStrikethru(This,FontStrikethru)	\
    ( (This)->lpVtbl -> get_FontStrikethru(This,FontStrikethru) ) 

#define IDaCtlBalloon2_put_FontStrikethru(This,FontStrikethru)	\
    ( (This)->lpVtbl -> put_FontStrikethru(This,FontStrikethru) ) 

#define IDaCtlBalloon2_get_FontUnderline(This,FontUnderline)	\
    ( (This)->lpVtbl -> get_FontUnderline(This,FontUnderline) ) 

#define IDaCtlBalloon2_put_FontUnderline(This,FontUnderline)	\
    ( (This)->lpVtbl -> put_FontUnderline(This,FontUnderline) ) 

#define IDaCtlBalloon2_get_SizeToText(This,SizeToText)	\
    ( (This)->lpVtbl -> get_SizeToText(This,SizeToText) ) 

#define IDaCtlBalloon2_put_SizeToText(This,SizeToText)	\
    ( (This)->lpVtbl -> put_SizeToText(This,SizeToText) ) 

#define IDaCtlBalloon2_get_AutoHide(This,AutoHide)	\
    ( (This)->lpVtbl -> get_AutoHide(This,AutoHide) ) 

#define IDaCtlBalloon2_put_AutoHide(This,AutoHide)	\
    ( (This)->lpVtbl -> put_AutoHide(This,AutoHide) ) 

#define IDaCtlBalloon2_get_AutoPace(This,AutoPace)	\
    ( (This)->lpVtbl -> get_AutoPace(This,AutoPace) ) 

#define IDaCtlBalloon2_put_AutoPace(This,AutoPace)	\
    ( (This)->lpVtbl -> put_AutoPace(This,AutoPace) ) 

#define IDaCtlBalloon2_get_NoAppend(This,NoAppend)	\
    ( (This)->lpVtbl -> get_NoAppend(This,NoAppend) ) 

#define IDaCtlBalloon2_put_NoAppend(This,NoAppend)	\
    ( (This)->lpVtbl -> put_NoAppend(This,NoAppend) ) 

#define IDaCtlBalloon2_get_ShowPartialLines(This,ShowPartialLines)	\
    ( (This)->lpVtbl -> get_ShowPartialLines(This,ShowPartialLines) ) 

#define IDaCtlBalloon2_put_ShowPartialLines(This,ShowPartialLines)	\
    ( (This)->lpVtbl -> put_ShowPartialLines(This,ShowPartialLines) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlBalloon2_INTERFACE_DEFINED__ */


#ifndef __IDaCtlSettings_INTERFACE_DEFINED__
#define __IDaCtlSettings_INTERFACE_DEFINED__

/* interface IDaCtlSettings */
/* [object][helpcontext][nonextensible][oleautomation][dual][unique][uuid] */ 


EXTERN_C const IID IID_IDaCtlSettings;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E55B-A208-11DE-ABF2-002421116FB2")
    IDaCtlSettings : public IDispatch
    {
    public:
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_SoundEffectsEnabled( 
            /* [retval][out] */ VARIANT_BOOL *SoundEffectsEnabled) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_BalloonEnabled( 
            /* [retval][out] */ VARIANT_BOOL *BalloonEnabled) = 0;
        
        virtual /* [bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_BalloonFont( 
            /* [retval][out] */ /* external definition not present */ IFontDisp **BalloonFont) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_TTSEnabled( 
            /* [retval][out] */ VARIANT_BOOL *TTSEnabled) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_TTSSpeed( 
            /* [retval][out] */ short *TTSSpeed) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_SREnabled( 
            /* [retval][out] */ VARIANT_BOOL *SREnabled) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_SRHotKey( 
            /* [retval][out] */ BSTR *SRHotKey) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_SRHotKeyTime( 
            /* [retval][out] */ short *SRHotKeyTime) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_SRListeningTip( 
            /* [retval][out] */ VARIANT_BOOL *SRListeningTip) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_SRListeningPrompt( 
            /* [retval][out] */ VARIANT_BOOL *SRListeningPrompt) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_AudioStatus( 
            /* [retval][out] */ AudioStatusType *AudioStatus) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlSettingsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlSettings * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlSettings * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlSettings * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlSettings * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlSettings * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlSettings * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlSettings * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SoundEffectsEnabled )( 
            IDaCtlSettings * This,
            /* [retval][out] */ VARIANT_BOOL *SoundEffectsEnabled);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_BalloonEnabled )( 
            IDaCtlSettings * This,
            /* [retval][out] */ VARIANT_BOOL *BalloonEnabled);
        
        /* [bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_BalloonFont )( 
            IDaCtlSettings * This,
            /* [retval][out] */ /* external definition not present */ IFontDisp **BalloonFont);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_TTSEnabled )( 
            IDaCtlSettings * This,
            /* [retval][out] */ VARIANT_BOOL *TTSEnabled);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_TTSSpeed )( 
            IDaCtlSettings * This,
            /* [retval][out] */ short *TTSSpeed);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SREnabled )( 
            IDaCtlSettings * This,
            /* [retval][out] */ VARIANT_BOOL *SREnabled);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SRHotKey )( 
            IDaCtlSettings * This,
            /* [retval][out] */ BSTR *SRHotKey);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SRHotKeyTime )( 
            IDaCtlSettings * This,
            /* [retval][out] */ short *SRHotKeyTime);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SRListeningTip )( 
            IDaCtlSettings * This,
            /* [retval][out] */ VARIANT_BOOL *SRListeningTip);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SRListeningPrompt )( 
            IDaCtlSettings * This,
            /* [retval][out] */ VARIANT_BOOL *SRListeningPrompt);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_AudioStatus )( 
            IDaCtlSettings * This,
            /* [retval][out] */ AudioStatusType *AudioStatus);
        
        END_INTERFACE
    } IDaCtlSettingsVtbl;

    interface IDaCtlSettings
    {
        CONST_VTBL struct IDaCtlSettingsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlSettings_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlSettings_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlSettings_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlSettings_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlSettings_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlSettings_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlSettings_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlSettings_get_SoundEffectsEnabled(This,SoundEffectsEnabled)	\
    ( (This)->lpVtbl -> get_SoundEffectsEnabled(This,SoundEffectsEnabled) ) 

#define IDaCtlSettings_get_BalloonEnabled(This,BalloonEnabled)	\
    ( (This)->lpVtbl -> get_BalloonEnabled(This,BalloonEnabled) ) 

#define IDaCtlSettings_get_BalloonFont(This,BalloonFont)	\
    ( (This)->lpVtbl -> get_BalloonFont(This,BalloonFont) ) 

#define IDaCtlSettings_get_TTSEnabled(This,TTSEnabled)	\
    ( (This)->lpVtbl -> get_TTSEnabled(This,TTSEnabled) ) 

#define IDaCtlSettings_get_TTSSpeed(This,TTSSpeed)	\
    ( (This)->lpVtbl -> get_TTSSpeed(This,TTSSpeed) ) 

#define IDaCtlSettings_get_SREnabled(This,SREnabled)	\
    ( (This)->lpVtbl -> get_SREnabled(This,SREnabled) ) 

#define IDaCtlSettings_get_SRHotKey(This,SRHotKey)	\
    ( (This)->lpVtbl -> get_SRHotKey(This,SRHotKey) ) 

#define IDaCtlSettings_get_SRHotKeyTime(This,SRHotKeyTime)	\
    ( (This)->lpVtbl -> get_SRHotKeyTime(This,SRHotKeyTime) ) 

#define IDaCtlSettings_get_SRListeningTip(This,SRListeningTip)	\
    ( (This)->lpVtbl -> get_SRListeningTip(This,SRListeningTip) ) 

#define IDaCtlSettings_get_SRListeningPrompt(This,SRListeningPrompt)	\
    ( (This)->lpVtbl -> get_SRListeningPrompt(This,SRListeningPrompt) ) 

#define IDaCtlSettings_get_AudioStatus(This,AudioStatus)	\
    ( (This)->lpVtbl -> get_AudioStatus(This,AudioStatus) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlSettings_INTERFACE_DEFINED__ */


#ifndef __IDaCtlCommands2_INTERFACE_DEFINED__
#define __IDaCtlCommands2_INTERFACE_DEFINED__

/* interface IDaCtlCommands2 */
/* [object][helpcontext][nonextensible][oleautomation][dual][unique][uuid] */ 


EXTERN_C const IID IID_IDaCtlCommands2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E55D-A208-11DE-ABF2-002421116FB2")
    IDaCtlCommands2 : public IDaCtlCommands
    {
    public:
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_VoiceGrammar( 
            /* [retval][out] */ BSTR *VoiceGrammar) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_VoiceGrammar( 
            /* [in] */ BSTR VoiceGrammar) = 0;
        
        virtual /* [propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Index( 
            /* [in] */ long Index,
            /* [retval][out] */ IDaCtlCommand2 **Command) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlCommands2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlCommands2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlCommands2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlCommands2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlCommands2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlCommands2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlCommands2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlCommands2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IDaCtlCommands2 * This,
            /* [in] */ BSTR Name,
            /* [retval][out] */ IDaCtlCommand2 **Item);
        
        /* [hidden][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Command )( 
            IDaCtlCommands2 * This,
            /* [in] */ BSTR Name,
            /* [retval][out] */ IDaCtlCommand2 **Command);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IDaCtlCommands2 * This,
            /* [retval][out] */ long *Count);
        
        /* [defaultbind][displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Caption )( 
            IDaCtlCommands2 * This,
            /* [retval][out] */ BSTR *Caption);
        
        /* [defaultbind][displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Caption )( 
            IDaCtlCommands2 * This,
            /* [in] */ BSTR Caption);
        
        /* [nonbrowsable][hidden][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Voice )( 
            IDaCtlCommands2 * This,
            /* [retval][out] */ BSTR *Voice);
        
        /* [nonbrowsable][hidden][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Voice )( 
            IDaCtlCommands2 * This,
            /* [in] */ BSTR Voice);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IDaCtlCommands2 * This,
            /* [retval][out] */ VARIANT_BOOL *Visible);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IDaCtlCommands2 * This,
            /* [in] */ VARIANT_BOOL Visible);
        
        /* [restricted][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IDaCtlCommands2 * This,
            /* [retval][out] */ IUnknown **EnumVariant);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            IDaCtlCommands2 * This,
            /* [in] */ BSTR Name,
            /* [optional][in] */ VARIANT Caption,
            /* [optional][in] */ VARIANT VoiceGrammar,
            /* [optional][in] */ VARIANT Enabled,
            /* [optional][in] */ VARIANT Visible,
            /* [retval][out] */ IDaCtlCommand2 **Command);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Insert )( 
            IDaCtlCommands2 * This,
            /* [in] */ BSTR Name,
            /* [in] */ BSTR RefName,
            /* [optional][in] */ VARIANT Before,
            /* [optional][in] */ VARIANT Caption,
            /* [optional][in] */ VARIANT VoiceGrammar,
            /* [optional][in] */ VARIANT Enabled,
            /* [optional][in] */ VARIANT Visible,
            /* [retval][out] */ IDaCtlCommand2 **Command);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Remove )( 
            IDaCtlCommands2 * This,
            /* [in] */ BSTR Name);
        
        /* [id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *RemoveAll )( 
            IDaCtlCommands2 * This);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_DefaultCommand )( 
            IDaCtlCommands2 * This,
            /* [retval][out] */ BSTR *Name);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_DefaultCommand )( 
            IDaCtlCommands2 * This,
            /* [in] */ BSTR Name);
        
        /* [nonbrowsable][hidden][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_HelpContextID )( 
            IDaCtlCommands2 * This,
            /* [retval][out] */ long *ID);
        
        /* [nonbrowsable][hidden][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_HelpContextID )( 
            IDaCtlCommands2 * This,
            /* [in] */ long ID);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_FontName )( 
            IDaCtlCommands2 * This,
            /* [retval][out] */ BSTR *FontName);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FontName )( 
            IDaCtlCommands2 * This,
            /* [in] */ BSTR FontName);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_FontSize )( 
            IDaCtlCommands2 * This,
            /* [retval][out] */ long *FontSize);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FontSize )( 
            IDaCtlCommands2 * This,
            /* [in] */ long FontSize);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_VoiceCaption )( 
            IDaCtlCommands2 * This,
            /* [retval][out] */ BSTR *VoiceCaption);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_VoiceCaption )( 
            IDaCtlCommands2 * This,
            /* [in] */ BSTR VoiceCaption);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_GlobalVoiceCommandsEnabled )( 
            IDaCtlCommands2 * This,
            /* [retval][out] */ VARIANT_BOOL *Enabled);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_GlobalVoiceCommandsEnabled )( 
            IDaCtlCommands2 * This,
            /* [in] */ VARIANT_BOOL Enabled);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_VoiceGrammar )( 
            IDaCtlCommands2 * This,
            /* [retval][out] */ BSTR *VoiceGrammar);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_VoiceGrammar )( 
            IDaCtlCommands2 * This,
            /* [in] */ BSTR VoiceGrammar);
        
        /* [propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Index )( 
            IDaCtlCommands2 * This,
            /* [in] */ long Index,
            /* [retval][out] */ IDaCtlCommand2 **Command);
        
        END_INTERFACE
    } IDaCtlCommands2Vtbl;

    interface IDaCtlCommands2
    {
        CONST_VTBL struct IDaCtlCommands2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlCommands2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlCommands2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlCommands2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlCommands2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlCommands2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlCommands2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlCommands2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlCommands2_get_Item(This,Name,Item)	\
    ( (This)->lpVtbl -> get_Item(This,Name,Item) ) 

#define IDaCtlCommands2_Command(This,Name,Command)	\
    ( (This)->lpVtbl -> Command(This,Name,Command) ) 

#define IDaCtlCommands2_get_Count(This,Count)	\
    ( (This)->lpVtbl -> get_Count(This,Count) ) 

#define IDaCtlCommands2_get_Caption(This,Caption)	\
    ( (This)->lpVtbl -> get_Caption(This,Caption) ) 

#define IDaCtlCommands2_put_Caption(This,Caption)	\
    ( (This)->lpVtbl -> put_Caption(This,Caption) ) 

#define IDaCtlCommands2_get_Voice(This,Voice)	\
    ( (This)->lpVtbl -> get_Voice(This,Voice) ) 

#define IDaCtlCommands2_put_Voice(This,Voice)	\
    ( (This)->lpVtbl -> put_Voice(This,Voice) ) 

#define IDaCtlCommands2_get_Visible(This,Visible)	\
    ( (This)->lpVtbl -> get_Visible(This,Visible) ) 

#define IDaCtlCommands2_put_Visible(This,Visible)	\
    ( (This)->lpVtbl -> put_Visible(This,Visible) ) 

#define IDaCtlCommands2_get__NewEnum(This,EnumVariant)	\
    ( (This)->lpVtbl -> get__NewEnum(This,EnumVariant) ) 

#define IDaCtlCommands2_Add(This,Name,Caption,VoiceGrammar,Enabled,Visible,Command)	\
    ( (This)->lpVtbl -> Add(This,Name,Caption,VoiceGrammar,Enabled,Visible,Command) ) 

#define IDaCtlCommands2_Insert(This,Name,RefName,Before,Caption,VoiceGrammar,Enabled,Visible,Command)	\
    ( (This)->lpVtbl -> Insert(This,Name,RefName,Before,Caption,VoiceGrammar,Enabled,Visible,Command) ) 

#define IDaCtlCommands2_Remove(This,Name)	\
    ( (This)->lpVtbl -> Remove(This,Name) ) 

#define IDaCtlCommands2_RemoveAll(This)	\
    ( (This)->lpVtbl -> RemoveAll(This) ) 

#define IDaCtlCommands2_get_DefaultCommand(This,Name)	\
    ( (This)->lpVtbl -> get_DefaultCommand(This,Name) ) 

#define IDaCtlCommands2_put_DefaultCommand(This,Name)	\
    ( (This)->lpVtbl -> put_DefaultCommand(This,Name) ) 

#define IDaCtlCommands2_get_HelpContextID(This,ID)	\
    ( (This)->lpVtbl -> get_HelpContextID(This,ID) ) 

#define IDaCtlCommands2_put_HelpContextID(This,ID)	\
    ( (This)->lpVtbl -> put_HelpContextID(This,ID) ) 

#define IDaCtlCommands2_get_FontName(This,FontName)	\
    ( (This)->lpVtbl -> get_FontName(This,FontName) ) 

#define IDaCtlCommands2_put_FontName(This,FontName)	\
    ( (This)->lpVtbl -> put_FontName(This,FontName) ) 

#define IDaCtlCommands2_get_FontSize(This,FontSize)	\
    ( (This)->lpVtbl -> get_FontSize(This,FontSize) ) 

#define IDaCtlCommands2_put_FontSize(This,FontSize)	\
    ( (This)->lpVtbl -> put_FontSize(This,FontSize) ) 

#define IDaCtlCommands2_get_VoiceCaption(This,VoiceCaption)	\
    ( (This)->lpVtbl -> get_VoiceCaption(This,VoiceCaption) ) 

#define IDaCtlCommands2_put_VoiceCaption(This,VoiceCaption)	\
    ( (This)->lpVtbl -> put_VoiceCaption(This,VoiceCaption) ) 

#define IDaCtlCommands2_get_GlobalVoiceCommandsEnabled(This,Enabled)	\
    ( (This)->lpVtbl -> get_GlobalVoiceCommandsEnabled(This,Enabled) ) 

#define IDaCtlCommands2_put_GlobalVoiceCommandsEnabled(This,Enabled)	\
    ( (This)->lpVtbl -> put_GlobalVoiceCommandsEnabled(This,Enabled) ) 


#define IDaCtlCommands2_get_VoiceGrammar(This,VoiceGrammar)	\
    ( (This)->lpVtbl -> get_VoiceGrammar(This,VoiceGrammar) ) 

#define IDaCtlCommands2_put_VoiceGrammar(This,VoiceGrammar)	\
    ( (This)->lpVtbl -> put_VoiceGrammar(This,VoiceGrammar) ) 

#define IDaCtlCommands2_get_Index(This,Index,Command)	\
    ( (This)->lpVtbl -> get_Index(This,Index,Command) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlCommands2_INTERFACE_DEFINED__ */


#ifndef __IDaCtlCommand2_INTERFACE_DEFINED__
#define __IDaCtlCommand2_INTERFACE_DEFINED__

/* interface IDaCtlCommand2 */
/* [object][helpcontext][nonextensible][oleautomation][dual][unique][uuid] */ 


EXTERN_C const IID IID_IDaCtlCommand2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E55E-A208-11DE-ABF2-002421116FB2")
    IDaCtlCommand2 : public IDaCtlCommand
    {
    public:
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_VoiceGrammar( 
            /* [retval][out] */ BSTR *VoiceGrammar) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_VoiceGrammar( 
            /* [in] */ BSTR VoiceGrammar) = 0;
        
        virtual /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_ConfidenceThreshold( 
            /* [retval][out] */ long *ConfidenceThreshold) = 0;
        
        virtual /* [displaybind][bindable][propput][id] */ HRESULT STDMETHODCALLTYPE put_ConfidenceThreshold( 
            /* [in] */ long ConfidenceThreshold) = 0;
        
        virtual /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *Name) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlCommand2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlCommand2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlCommand2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlCommand2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlCommand2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlCommand2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlCommand2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlCommand2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [nonbrowsable][hidden][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Voice )( 
            IDaCtlCommand2 * This,
            /* [retval][out] */ BSTR *Voice);
        
        /* [nonbrowsable][hidden][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Voice )( 
            IDaCtlCommand2 * This,
            /* [in] */ BSTR Voice);
        
        /* [defaultbind][displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Caption )( 
            IDaCtlCommand2 * This,
            /* [retval][out] */ BSTR *Caption);
        
        /* [defaultbind][displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Caption )( 
            IDaCtlCommand2 * This,
            /* [in] */ BSTR Caption);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IDaCtlCommand2 * This,
            /* [retval][out] */ VARIANT_BOOL *Enabled);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            IDaCtlCommand2 * This,
            /* [in] */ VARIANT_BOOL Enabled);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IDaCtlCommand2 * This,
            /* [retval][out] */ VARIANT_BOOL *Visible);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IDaCtlCommand2 * This,
            /* [in] */ VARIANT_BOOL Visible);
        
        /* [nonbrowsable][hidden][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Confidence )( 
            IDaCtlCommand2 * This,
            /* [retval][out] */ long *Confidence);
        
        /* [nonbrowsable][hidden][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Confidence )( 
            IDaCtlCommand2 * This,
            /* [in] */ long Confidence);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_ConfidenceText )( 
            IDaCtlCommand2 * This,
            /* [retval][out] */ BSTR *Text);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ConfidenceText )( 
            IDaCtlCommand2 * This,
            /* [in] */ BSTR Text);
        
        /* [nonbrowsable][hidden][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_HelpContextID )( 
            IDaCtlCommand2 * This,
            /* [retval][out] */ long *ID);
        
        /* [nonbrowsable][hidden][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_HelpContextID )( 
            IDaCtlCommand2 * This,
            /* [in] */ long ID);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_VoiceCaption )( 
            IDaCtlCommand2 * This,
            /* [retval][out] */ BSTR *VoiceCaption);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_VoiceCaption )( 
            IDaCtlCommand2 * This,
            /* [in] */ BSTR VoiceCaption);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_VoiceGrammar )( 
            IDaCtlCommand2 * This,
            /* [retval][out] */ BSTR *VoiceGrammar);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_VoiceGrammar )( 
            IDaCtlCommand2 * This,
            /* [in] */ BSTR VoiceGrammar);
        
        /* [displaybind][bindable][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_ConfidenceThreshold )( 
            IDaCtlCommand2 * This,
            /* [retval][out] */ long *ConfidenceThreshold);
        
        /* [displaybind][bindable][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ConfidenceThreshold )( 
            IDaCtlCommand2 * This,
            /* [in] */ long ConfidenceThreshold);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IDaCtlCommand2 * This,
            /* [retval][out] */ BSTR *Name);
        
        END_INTERFACE
    } IDaCtlCommand2Vtbl;

    interface IDaCtlCommand2
    {
        CONST_VTBL struct IDaCtlCommand2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlCommand2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlCommand2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlCommand2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlCommand2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlCommand2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlCommand2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlCommand2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlCommand2_get_Voice(This,Voice)	\
    ( (This)->lpVtbl -> get_Voice(This,Voice) ) 

#define IDaCtlCommand2_put_Voice(This,Voice)	\
    ( (This)->lpVtbl -> put_Voice(This,Voice) ) 

#define IDaCtlCommand2_get_Caption(This,Caption)	\
    ( (This)->lpVtbl -> get_Caption(This,Caption) ) 

#define IDaCtlCommand2_put_Caption(This,Caption)	\
    ( (This)->lpVtbl -> put_Caption(This,Caption) ) 

#define IDaCtlCommand2_get_Enabled(This,Enabled)	\
    ( (This)->lpVtbl -> get_Enabled(This,Enabled) ) 

#define IDaCtlCommand2_put_Enabled(This,Enabled)	\
    ( (This)->lpVtbl -> put_Enabled(This,Enabled) ) 

#define IDaCtlCommand2_get_Visible(This,Visible)	\
    ( (This)->lpVtbl -> get_Visible(This,Visible) ) 

#define IDaCtlCommand2_put_Visible(This,Visible)	\
    ( (This)->lpVtbl -> put_Visible(This,Visible) ) 

#define IDaCtlCommand2_get_Confidence(This,Confidence)	\
    ( (This)->lpVtbl -> get_Confidence(This,Confidence) ) 

#define IDaCtlCommand2_put_Confidence(This,Confidence)	\
    ( (This)->lpVtbl -> put_Confidence(This,Confidence) ) 

#define IDaCtlCommand2_get_ConfidenceText(This,Text)	\
    ( (This)->lpVtbl -> get_ConfidenceText(This,Text) ) 

#define IDaCtlCommand2_put_ConfidenceText(This,Text)	\
    ( (This)->lpVtbl -> put_ConfidenceText(This,Text) ) 

#define IDaCtlCommand2_get_HelpContextID(This,ID)	\
    ( (This)->lpVtbl -> get_HelpContextID(This,ID) ) 

#define IDaCtlCommand2_put_HelpContextID(This,ID)	\
    ( (This)->lpVtbl -> put_HelpContextID(This,ID) ) 

#define IDaCtlCommand2_get_VoiceCaption(This,VoiceCaption)	\
    ( (This)->lpVtbl -> get_VoiceCaption(This,VoiceCaption) ) 

#define IDaCtlCommand2_put_VoiceCaption(This,VoiceCaption)	\
    ( (This)->lpVtbl -> put_VoiceCaption(This,VoiceCaption) ) 


#define IDaCtlCommand2_get_VoiceGrammar(This,VoiceGrammar)	\
    ( (This)->lpVtbl -> get_VoiceGrammar(This,VoiceGrammar) ) 

#define IDaCtlCommand2_put_VoiceGrammar(This,VoiceGrammar)	\
    ( (This)->lpVtbl -> put_VoiceGrammar(This,VoiceGrammar) ) 

#define IDaCtlCommand2_get_ConfidenceThreshold(This,ConfidenceThreshold)	\
    ( (This)->lpVtbl -> get_ConfidenceThreshold(This,ConfidenceThreshold) ) 

#define IDaCtlCommand2_put_ConfidenceThreshold(This,ConfidenceThreshold)	\
    ( (This)->lpVtbl -> put_ConfidenceThreshold(This,ConfidenceThreshold) ) 

#define IDaCtlCommand2_get_Name(This,Name)	\
    ( (This)->lpVtbl -> get_Name(This,Name) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlCommand2_INTERFACE_DEFINED__ */


#ifndef __IDaCtlPropertySheet2_INTERFACE_DEFINED__
#define __IDaCtlPropertySheet2_INTERFACE_DEFINED__

/* interface IDaCtlPropertySheet2 */
/* [object][helpcontext][nonextensible][oleautomation][dual][unique][uuid] */ 


EXTERN_C const IID IID_IDaCtlPropertySheet2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E55F-A208-11DE-ABF2-002421116FB2")
    IDaCtlPropertySheet2 : public IDaCtlPropertySheet
    {
    public:
        virtual /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE put_Left( 
            /* [in] */ short Left) = 0;
        
        virtual /* [displaybind][bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_Left( 
            /* [retval][out] */ short *Left) = 0;
        
        virtual /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT STDMETHODCALLTYPE put_Top( 
            /* [in] */ short Top) = 0;
        
        virtual /* [displaybind][bindable][propget][id] */ HRESULT STDMETHODCALLTYPE get_Top( 
            /* [retval][out] */ short *Top) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaCtlPropertySheet2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaCtlPropertySheet2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaCtlPropertySheet2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaCtlPropertySheet2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaCtlPropertySheet2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaCtlPropertySheet2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaCtlPropertySheet2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaCtlPropertySheet2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [nonbrowsable][restricted][hidden][propget] */ HRESULT ( STDMETHODCALLTYPE *get__Left )( 
            IDaCtlPropertySheet2 * This,
            /* [retval][out] */ short *Left);
        
        /* [nonbrowsable][restricted][hidden][propget] */ HRESULT ( STDMETHODCALLTYPE *get__Top )( 
            IDaCtlPropertySheet2 * This,
            /* [retval][out] */ short *Top);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IDaCtlPropertySheet2 * This,
            /* [retval][out] */ short *Height);
        
        /* [displaybind][bindable][readonly][propget][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IDaCtlPropertySheet2 * This,
            /* [retval][out] */ short *Width);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IDaCtlPropertySheet2 * This,
            /* [in] */ VARIANT_BOOL Visible);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IDaCtlPropertySheet2 * This,
            /* [retval][out] */ VARIANT_BOOL *Visible);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_Page )( 
            IDaCtlPropertySheet2 * This,
            /* [in] */ BSTR Page);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Page )( 
            IDaCtlPropertySheet2 * This,
            /* [retval][out] */ BSTR *Page);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_Left )( 
            IDaCtlPropertySheet2 * This,
            /* [in] */ short Left);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Left )( 
            IDaCtlPropertySheet2 * This,
            /* [retval][out] */ short *Left);
        
        /* [displaybind][bindable][propput][id][helpcontext][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_Top )( 
            IDaCtlPropertySheet2 * This,
            /* [in] */ short Top);
        
        /* [displaybind][bindable][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Top )( 
            IDaCtlPropertySheet2 * This,
            /* [retval][out] */ short *Top);
        
        END_INTERFACE
    } IDaCtlPropertySheet2Vtbl;

    interface IDaCtlPropertySheet2
    {
        CONST_VTBL struct IDaCtlPropertySheet2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaCtlPropertySheet2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaCtlPropertySheet2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaCtlPropertySheet2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaCtlPropertySheet2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaCtlPropertySheet2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaCtlPropertySheet2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaCtlPropertySheet2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaCtlPropertySheet2_get__Left(This,Left)	\
    ( (This)->lpVtbl -> get__Left(This,Left) ) 

#define IDaCtlPropertySheet2_get__Top(This,Top)	\
    ( (This)->lpVtbl -> get__Top(This,Top) ) 

#define IDaCtlPropertySheet2_get_Height(This,Height)	\
    ( (This)->lpVtbl -> get_Height(This,Height) ) 

#define IDaCtlPropertySheet2_get_Width(This,Width)	\
    ( (This)->lpVtbl -> get_Width(This,Width) ) 

#define IDaCtlPropertySheet2_put_Visible(This,Visible)	\
    ( (This)->lpVtbl -> put_Visible(This,Visible) ) 

#define IDaCtlPropertySheet2_get_Visible(This,Visible)	\
    ( (This)->lpVtbl -> get_Visible(This,Visible) ) 

#define IDaCtlPropertySheet2_put_Page(This,Page)	\
    ( (This)->lpVtbl -> put_Page(This,Page) ) 

#define IDaCtlPropertySheet2_get_Page(This,Page)	\
    ( (This)->lpVtbl -> get_Page(This,Page) ) 


#define IDaCtlPropertySheet2_put_Left(This,Left)	\
    ( (This)->lpVtbl -> put_Left(This,Left) ) 

#define IDaCtlPropertySheet2_get_Left(This,Left)	\
    ( (This)->lpVtbl -> get_Left(This,Left) ) 

#define IDaCtlPropertySheet2_put_Top(This,Top)	\
    ( (This)->lpVtbl -> put_Top(This,Top) ) 

#define IDaCtlPropertySheet2_get_Top(This,Top)	\
    ( (This)->lpVtbl -> get_Top(This,Top) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaCtlPropertySheet2_INTERFACE_DEFINED__ */


#ifndef ___DaCtlEvents2_DISPINTERFACE_DEFINED__
#define ___DaCtlEvents2_DISPINTERFACE_DEFINED__

/* dispinterface _DaCtlEvents2 */
/* [helpcontext][hidden][uuid] */ 


EXTERN_C const IID DIID__DaCtlEvents2;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("1147E582-A208-11DE-ABF2-002421116FB2")
    _DaCtlEvents2 : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DaCtlEvents2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DaCtlEvents2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DaCtlEvents2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DaCtlEvents2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DaCtlEvents2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DaCtlEvents2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DaCtlEvents2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DaCtlEvents2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DaCtlEvents2Vtbl;

    interface _DaCtlEvents2
    {
        CONST_VTBL struct _DaCtlEvents2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DaCtlEvents2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DaCtlEvents2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DaCtlEvents2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DaCtlEvents2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DaCtlEvents2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DaCtlEvents2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DaCtlEvents2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DaCtlEvents2_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_DaControl;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E530-A208-11DE-ABF2-002421116FB2")
DaControl;
#endif

EXTERN_C const CLSID CLSID_DaCtlCharacters;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E531-A208-11DE-ABF2-002421116FB2")
DaCtlCharacters;
#endif

EXTERN_C const CLSID CLSID_DaCtlCharacter;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E532-A208-11DE-ABF2-002421116FB2")
DaCtlCharacter;
#endif

EXTERN_C const CLSID CLSID_DaCtlBalloon;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E533-A208-11DE-ABF2-002421116FB2")
DaCtlBalloon;
#endif

EXTERN_C const CLSID CLSID_DaCtlCommands;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E534-A208-11DE-ABF2-002421116FB2")
DaCtlCommands;
#endif

EXTERN_C const CLSID CLSID_DaCtlCommand;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E535-A208-11DE-ABF2-002421116FB2")
DaCtlCommand;
#endif

EXTERN_C const CLSID CLSID_DaCtlRequest;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E536-A208-11DE-ABF2-002421116FB2")
DaCtlRequest;
#endif

EXTERN_C const CLSID CLSID_DaCtlAudioOutput;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E537-A208-11DE-ABF2-002421116FB2")
DaCtlAudioOutput;
#endif

EXTERN_C const CLSID CLSID_DaCtlSpeechInput;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E538-A208-11DE-ABF2-002421116FB2")
DaCtlSpeechInput;
#endif

EXTERN_C const CLSID CLSID_DaCtlPropertySheet;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E539-A208-11DE-ABF2-002421116FB2")
DaCtlPropertySheet;
#endif

EXTERN_C const CLSID CLSID_DaCtlUserInput;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E53A-A208-11DE-ABF2-002421116FB2")
DaCtlUserInput;
#endif

EXTERN_C const CLSID CLSID_DaCtlCommandsWindow;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E53B-A208-11DE-ABF2-002421116FB2")
DaCtlCommandsWindow;
#endif

EXTERN_C const CLSID CLSID_DaCtlAnimationNames;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E54B-A208-11DE-ABF2-002421116FB2")
DaCtlAnimationNames;
#endif

EXTERN_C const CLSID CLSID_DaCtlSettings;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E55C-A208-11DE-ABF2-002421116FB2")
DaCtlSettings;
#endif

EXTERN_C const CLSID CLSID_DaCtlCharacterFiles;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E558-A208-11DE-ABF2-002421116FB2")
DaCtlCharacterFiles;
#endif

EXTERN_C const CLSID CLSID_DaCtlFormattedText;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E580-A208-11DE-ABF2-002421116FB2")
DaCtlFormattedText;
#endif

EXTERN_C const CLSID CLSID_DaCtlTTSEngine;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E553-A208-11DE-ABF2-002421116FB2")
DaCtlTTSEngine;
#endif

EXTERN_C const CLSID CLSID_DaCtlTTSEngines;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E554-A208-11DE-ABF2-002421116FB2")
DaCtlTTSEngines;
#endif

EXTERN_C const CLSID CLSID_DaCtlSREngine;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E555-A208-11DE-ABF2-002421116FB2")
DaCtlSREngine;
#endif

EXTERN_C const CLSID CLSID_DaCtlSREngines;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E556-A208-11DE-ABF2-002421116FB2")
DaCtlSREngines;
#endif
#endif /* __DoubleAgentCtl_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


