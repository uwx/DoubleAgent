

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Sun Apr 04 05:22:46 2010
 */
/* Compiler settings for .\Server\DaServer.odl:
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


#ifndef __DaServerOdl_h__
#define __DaServerOdl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDaServer_FWD_DEFINED__
#define __IDaServer_FWD_DEFINED__
typedef interface IDaServer IDaServer;
#endif 	/* __IDaServer_FWD_DEFINED__ */


#ifndef __IDaSvrUserInput_FWD_DEFINED__
#define __IDaSvrUserInput_FWD_DEFINED__
typedef interface IDaSvrUserInput IDaSvrUserInput;
#endif 	/* __IDaSvrUserInput_FWD_DEFINED__ */


#ifndef __IDaSvrCommand_FWD_DEFINED__
#define __IDaSvrCommand_FWD_DEFINED__
typedef interface IDaSvrCommand IDaSvrCommand;
#endif 	/* __IDaSvrCommand_FWD_DEFINED__ */


#ifndef __IDaSvrCommands_FWD_DEFINED__
#define __IDaSvrCommands_FWD_DEFINED__
typedef interface IDaSvrCommands IDaSvrCommands;
#endif 	/* __IDaSvrCommands_FWD_DEFINED__ */


#ifndef __IDaSvrSpeechInput_FWD_DEFINED__
#define __IDaSvrSpeechInput_FWD_DEFINED__
typedef interface IDaSvrSpeechInput IDaSvrSpeechInput;
#endif 	/* __IDaSvrSpeechInput_FWD_DEFINED__ */


#ifndef __IDaSvrAudioOutput_FWD_DEFINED__
#define __IDaSvrAudioOutput_FWD_DEFINED__
typedef interface IDaSvrAudioOutput IDaSvrAudioOutput;
#endif 	/* __IDaSvrAudioOutput_FWD_DEFINED__ */


#ifndef __IDaSvrPropertySheet_FWD_DEFINED__
#define __IDaSvrPropertySheet_FWD_DEFINED__
typedef interface IDaSvrPropertySheet IDaSvrPropertySheet;
#endif 	/* __IDaSvrPropertySheet_FWD_DEFINED__ */


#ifndef __IDaSvrBalloon_FWD_DEFINED__
#define __IDaSvrBalloon_FWD_DEFINED__
typedef interface IDaSvrBalloon IDaSvrBalloon;
#endif 	/* __IDaSvrBalloon_FWD_DEFINED__ */


#ifndef __IDaSvrCharacter_FWD_DEFINED__
#define __IDaSvrCharacter_FWD_DEFINED__
typedef interface IDaSvrCharacter IDaSvrCharacter;
#endif 	/* __IDaSvrCharacter_FWD_DEFINED__ */


#ifndef __IDaSvrCommandsWindow_FWD_DEFINED__
#define __IDaSvrCommandsWindow_FWD_DEFINED__
typedef interface IDaSvrCommandsWindow IDaSvrCommandsWindow;
#endif 	/* __IDaSvrCommandsWindow_FWD_DEFINED__ */


#ifndef __IDaSvrNotifySink15_FWD_DEFINED__
#define __IDaSvrNotifySink15_FWD_DEFINED__
typedef interface IDaSvrNotifySink15 IDaSvrNotifySink15;
#endif 	/* __IDaSvrNotifySink15_FWD_DEFINED__ */


#ifndef __IDaSvrNotifySink_FWD_DEFINED__
#define __IDaSvrNotifySink_FWD_DEFINED__
typedef interface IDaSvrNotifySink IDaSvrNotifySink;
#endif 	/* __IDaSvrNotifySink_FWD_DEFINED__ */


#ifndef __IDaSvrCharacterFiles_FWD_DEFINED__
#define __IDaSvrCharacterFiles_FWD_DEFINED__
typedef interface IDaSvrCharacterFiles IDaSvrCharacterFiles;
#endif 	/* __IDaSvrCharacterFiles_FWD_DEFINED__ */


#ifndef __IDaSvrSpeechEngine_FWD_DEFINED__
#define __IDaSvrSpeechEngine_FWD_DEFINED__
typedef interface IDaSvrSpeechEngine IDaSvrSpeechEngine;
#endif 	/* __IDaSvrSpeechEngine_FWD_DEFINED__ */


#ifndef __IDaSvrSpeechEngines_FWD_DEFINED__
#define __IDaSvrSpeechEngines_FWD_DEFINED__
typedef interface IDaSvrSpeechEngines IDaSvrSpeechEngines;
#endif 	/* __IDaSvrSpeechEngines_FWD_DEFINED__ */


#ifndef __IDaSvrRecognitionEngine_FWD_DEFINED__
#define __IDaSvrRecognitionEngine_FWD_DEFINED__
typedef interface IDaSvrRecognitionEngine IDaSvrRecognitionEngine;
#endif 	/* __IDaSvrRecognitionEngine_FWD_DEFINED__ */


#ifndef __IDaSvrRecognitionEngines_FWD_DEFINED__
#define __IDaSvrRecognitionEngines_FWD_DEFINED__
typedef interface IDaSvrRecognitionEngines IDaSvrRecognitionEngines;
#endif 	/* __IDaSvrRecognitionEngines_FWD_DEFINED__ */


#ifndef __IDaServer2_FWD_DEFINED__
#define __IDaServer2_FWD_DEFINED__
typedef interface IDaServer2 IDaServer2;
#endif 	/* __IDaServer2_FWD_DEFINED__ */


#ifndef __IDaSvrCharacter2_FWD_DEFINED__
#define __IDaSvrCharacter2_FWD_DEFINED__
typedef interface IDaSvrCharacter2 IDaSvrCharacter2;
#endif 	/* __IDaSvrCharacter2_FWD_DEFINED__ */


#ifndef __DaServer_FWD_DEFINED__
#define __DaServer_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaServer DaServer;
#else
typedef struct DaServer DaServer;
#endif /* __cplusplus */

#endif 	/* __DaServer_FWD_DEFINED__ */


#ifndef __DaSvrCharacter_FWD_DEFINED__
#define __DaSvrCharacter_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaSvrCharacter DaSvrCharacter;
#else
typedef struct DaSvrCharacter DaSvrCharacter;
#endif /* __cplusplus */

#endif 	/* __DaSvrCharacter_FWD_DEFINED__ */


#ifndef __DaSvrCommand_FWD_DEFINED__
#define __DaSvrCommand_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaSvrCommand DaSvrCommand;
#else
typedef struct DaSvrCommand DaSvrCommand;
#endif /* __cplusplus */

#endif 	/* __DaSvrCommand_FWD_DEFINED__ */


#ifndef __DaSvrCommands_FWD_DEFINED__
#define __DaSvrCommands_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaSvrCommands DaSvrCommands;
#else
typedef struct DaSvrCommands DaSvrCommands;
#endif /* __cplusplus */

#endif 	/* __DaSvrCommands_FWD_DEFINED__ */


#ifndef __DaSvrCommandsWindow_FWD_DEFINED__
#define __DaSvrCommandsWindow_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaSvrCommandsWindow DaSvrCommandsWindow;
#else
typedef struct DaSvrCommandsWindow DaSvrCommandsWindow;
#endif /* __cplusplus */

#endif 	/* __DaSvrCommandsWindow_FWD_DEFINED__ */


#ifndef __DaSvrUserInput_FWD_DEFINED__
#define __DaSvrUserInput_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaSvrUserInput DaSvrUserInput;
#else
typedef struct DaSvrUserInput DaSvrUserInput;
#endif /* __cplusplus */

#endif 	/* __DaSvrUserInput_FWD_DEFINED__ */


#ifndef __DaSvrBalloon_FWD_DEFINED__
#define __DaSvrBalloon_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaSvrBalloon DaSvrBalloon;
#else
typedef struct DaSvrBalloon DaSvrBalloon;
#endif /* __cplusplus */

#endif 	/* __DaSvrBalloon_FWD_DEFINED__ */


#ifndef __DaSvrPropertySheet_FWD_DEFINED__
#define __DaSvrPropertySheet_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaSvrPropertySheet DaSvrPropertySheet;
#else
typedef struct DaSvrPropertySheet DaSvrPropertySheet;
#endif /* __cplusplus */

#endif 	/* __DaSvrPropertySheet_FWD_DEFINED__ */


#ifndef __DaSvrAudioOutput_FWD_DEFINED__
#define __DaSvrAudioOutput_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaSvrAudioOutput DaSvrAudioOutput;
#else
typedef struct DaSvrAudioOutput DaSvrAudioOutput;
#endif /* __cplusplus */

#endif 	/* __DaSvrAudioOutput_FWD_DEFINED__ */


#ifndef __DaSvrSpeechInput_FWD_DEFINED__
#define __DaSvrSpeechInput_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaSvrSpeechInput DaSvrSpeechInput;
#else
typedef struct DaSvrSpeechInput DaSvrSpeechInput;
#endif /* __cplusplus */

#endif 	/* __DaSvrSpeechInput_FWD_DEFINED__ */


#ifndef __DaSvrAnimationNames_FWD_DEFINED__
#define __DaSvrAnimationNames_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaSvrAnimationNames DaSvrAnimationNames;
#else
typedef struct DaSvrAnimationNames DaSvrAnimationNames;
#endif /* __cplusplus */

#endif 	/* __DaSvrAnimationNames_FWD_DEFINED__ */


#ifndef __DaSvrCharacterFiles_FWD_DEFINED__
#define __DaSvrCharacterFiles_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaSvrCharacterFiles DaSvrCharacterFiles;
#else
typedef struct DaSvrCharacterFiles DaSvrCharacterFiles;
#endif /* __cplusplus */

#endif 	/* __DaSvrCharacterFiles_FWD_DEFINED__ */


#ifndef __DaSvrSpeechEngine_FWD_DEFINED__
#define __DaSvrSpeechEngine_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaSvrSpeechEngine DaSvrSpeechEngine;
#else
typedef struct DaSvrSpeechEngine DaSvrSpeechEngine;
#endif /* __cplusplus */

#endif 	/* __DaSvrSpeechEngine_FWD_DEFINED__ */


#ifndef __DaSvrSpeechEngines_FWD_DEFINED__
#define __DaSvrSpeechEngines_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaSvrSpeechEngines DaSvrSpeechEngines;
#else
typedef struct DaSvrSpeechEngines DaSvrSpeechEngines;
#endif /* __cplusplus */

#endif 	/* __DaSvrSpeechEngines_FWD_DEFINED__ */


#ifndef __DaSvrRecognitionEngine_FWD_DEFINED__
#define __DaSvrRecognitionEngine_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaSvrRecognitionEngine DaSvrRecognitionEngine;
#else
typedef struct DaSvrRecognitionEngine DaSvrRecognitionEngine;
#endif /* __cplusplus */

#endif 	/* __DaSvrRecognitionEngine_FWD_DEFINED__ */


#ifndef __DaSvrRecognitionEngines_FWD_DEFINED__
#define __DaSvrRecognitionEngines_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaSvrRecognitionEngines DaSvrRecognitionEngines;
#else
typedef struct DaSvrRecognitionEngines DaSvrRecognitionEngines;
#endif /* __cplusplus */

#endif 	/* __DaSvrRecognitionEngines_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __DoubleAgentSvr_LIBRARY_DEFINED__
#define __DoubleAgentSvr_LIBRARY_DEFINED__

/* library DoubleAgentSvr */
/* [helpstring][version][uuid] */ 




















#ifndef _DA_COMMON_TYPES_
#define _DA_COMMON_TYPES_
typedef 
enum CharacterStyle
    {	CharacterStyle_SoundEffects	= 0x1,
	CharacterStyle_IdleEnabled	= 0x2,
	CharacterStyle_AutoPopupMenu	= 0x4,
	CharacterStyle_IconShown	= 0x8,
	CharacterStyle_Smoothed	= 0x30,
	CharacterStyle_SmoothEdges	= 0x20
    } 	CharacterStyle;

typedef 
enum BalloonStyle
    {	BalloonStyle_Enabled	= 0x1,
	BalloonStyle_SizeToText	= 0x2,
	BalloonStyle_AutoHide	= 0x4,
	BalloonStyle_AutoPace	= 0x8,
	BalloonStyle_ShowPartialLines	= 0x10
    } 	BalloonStyle;

typedef 
enum ActiveType
    {	ActiveType_Inactive	= 0,
	ActiveType_Active	= 1,
	ActiveType_InputActive	= 2
    } 	ActiveType;

typedef 
enum StopType
    {	StopType_Play	= 0x1,
	StopType_Move	= 0x2,
	StopType_Speak	= 0x4,
	StopType_QueuedPrepare	= 0x8,
	StopType_ImmediatePrepate	= 0x10,
	StopType_Visibility	= 0x20,
	StopType_All	= 0xffffffff
    } 	StopType;

typedef 
enum PrepareType
    {	PrepareType_Animation	= 0,
	PrepareType_State	= 1,
	PrepareType_Wave	= 2
    } 	PrepareType;

typedef 
enum MoveCause
    {	MoveCause_NeverMoved	= 0,
	MoveCause_UserMoved	= 1,
	MoveCause_ProgramMoved	= 2,
	MoveCause_OtherProgramMoved	= 3,
	MoveCause_SystemMoved	= 4
    } 	MoveCause;

typedef 
enum VisibilityCause
    {	VisibilityCause_NeverShown	= 0,
	VisibilityCause_UserHid	= 1,
	VisibilityCause_UserShowed	= 2,
	VisibilityCause_ProgramHid	= 3,
	VisibilityCause_ProgramShowed	= 4,
	VisibilityCause_OtherProgramHid	= 5,
	VisibilityCause_OtherProgramShowed	= 6
    } 	VisibilityCause;

typedef 
enum AudioStatus
    {	AudioStatus_Available	= 0,
	AudioStatus_UserSpeaking	= 3,
	AudioStatus_CharacterSpeaking	= 4,
	AudioStatus_CharacterListening	= 5,
	AudioStatus_Error	= 6
    } 	AudioStatus;

typedef 
enum ListenStatus
    {	ListenStatus_Available	= 0,
	ListenStatus_CharacterInactive	= 2,
	ListenStatus_InitializeFailed	= 4,
	ListenStatus_SpeechDisabled	= 5,
	ListenStatus_Error	= 6
    } 	ListenStatus;

typedef 
enum ListenCompleteCause
    {	ListenComplete_ProgramDisabled	= 1,
	ListenComplete_ProgramTimedOut	= 2,
	ListenComplete_UserTimedOut	= 3,
	ListenComplete_UserReleasedKey	= 4,
	ListenComplete_UserSpeechEnded	= 5,
	ListenComplete_CharacterClientDeactivated	= 6,
	ListenComplete_DefaultCharacterChanged	= 7,
	ListenComplete_UserDisabled	= 8
    } 	ListenCompleteCause;

typedef 
enum SpeechGender
    {	SpeechGender_Neutral	= 0,
	SpeechGender_Female	= 1,
	SpeechGender_Male	= 2
    } 	SpeechGender;

#endif // _DA_COMMON_TYPES_
typedef 
enum FilesFilter
    {	FilesFilter_PathDoubleAgent	= 0x1,
	FilesFilter_PathMsAgent	= 0x2,
	FilesFilter_PathMsOffice	= 0x4,
	FilesFilter_PathMask	= 0x7,
	FilesFilter_ExcludeNonSpeaking	= 0x10,
	FilesFilter_ExcludeSpeaking	= 0x20,
	FilesFilter_ExcludeMask	= 0x30,
	FilesFilter_NoValidateVersion	= 0x10000,
	FilesFilter_FilterMask	= 0x10030,
	FilesFilter_ValidMask	= 0x10037
    } 	FilesFilter;

#define	NeverMoved	( MoveCause_NeverMoved )

#define	UserMoved	( MoveCause_UserMoved )

#define	ProgramMoved	( MoveCause_ProgramMoved )

#define	OtherProgramMoved	( MoveCause_OtherProgramMoved )

#define	SystemMoved	( MoveCause_SystemMoved )

#define	NeverShown	( VisibilityCause_NeverShown )

#define	UserHid	( VisibilityCause_UserHid )

#define	UserShowed	( VisibilityCause_UserShowed )

#define	ProgramHid	( VisibilityCause_ProgramHid )

#define	ProgramShowed	( VisibilityCause_ProgramShowed )

#define	OtherProgramHid	( VisibilityCause_OtherProgramHid )

#define	OtherProgramShowed	( VisibilityCause_OtherProgramShowed )

#define	UserHidViaCharacterMenu	( 7 )

#define	UserHidViaTaskbarIcon	( VisibilityCause_UserHid )

#define	CSHELPCAUSE_COMMAND	( 1 )

#define	CSHELPCAUSE_OTHERPROGRAM	( 2 )

#define	CSHELPCAUSE_OPENCOMMANDSWINDOW	( 3 )

#define	CSHELPCAUSE_CLOSECOMMANDSWINDOW	( 4 )

#define	CSHELPCAUSE_SHOWCHARACTER	( 5 )

#define	CSHELPCAUSE_HIDECHARACTER	( 6 )

#define	CSHELPCAUSE_CHARACTER	( 7 )

#define	ACTIVATE_NOTTOPMOST	( ActiveType_Inactive )

#define	ACTIVATE_TOPMOST	( ActiveType_Active )

#define	ACTIVATE_NOTACTIVE	( ActiveType_Inactive )

#define	ACTIVATE_ACTIVE	( ActiveType_Active )

#define	ACTIVATE_INPUTACTIVE	( ActiveType_InputActive )

#define	PREPARE_ANIMATION	( PrepareType_Animation )

#define	PREPARE_STATE	( PrepareType_State )

#define	PREPARE_WAVE	( PrepareType_Wave )

#define	STOP_TYPE_PLAY	( StopType_Play )

#define	STOP_TYPE_MOVE	( StopType_Move )

#define	STOP_TYPE_SPEAK	( StopType_Speak )

#define	STOP_TYPE_PREPARE	( StopType_QueuedPrepare )

#define	STOP_TYPE_NONQUEUEDPREPARE	( StopType_ImmediatePrepate )

#define	STOP_TYPE_VISIBLE	( StopType_Visibility )

#define	STOP_TYPE_ALL	( StopType_All )

#define	BALLOON_STYLE_BALLOON_ON	( BalloonStyle_Enabled )

#define	BALLOON_STYLE_SIZETOTEXT	( BalloonStyle_SizeToText )

#define	BALLOON_STYLE_AUTOHIDE	( BalloonStyle_AutoHide )

#define	BALLOON_STYLE_AUTOPACE	( BalloonStyle_AutoPace )

#define	BALLOON_STYLE_PARTIAL_LINES	( BalloonStyle_ShowPartialLines )

#define	AUDIO_STATUS_AVAILABLE	( AudioStatus_Available )

#define	AUDIO_STATUS_NOAUDIO	( 1 )

#define	AUDIO_STATUS_CANTOPENAUDIO	( 2 )

#define	AUDIO_STATUS_USERSPEAKING	( AudioStatus_UserSpeaking )

#define	AUDIO_STATUS_CHARACTERSPEAKING	( AudioStatus_CharacterSpeaking )

#define	AUDIO_STATUS_SROVERRIDEABLE	( AudioStatus_CharacterListening )

#define	AUDIO_STATUS_ERROR	( AudioStatus_Error )

#define	LISTEN_STATUS_CANLISTEN	( ListenStatus_Available )

#define	LISTEN_STATUS_NOAUDIO	( 1 )

#define	LISTEN_STATUS_NOTACTIVE	( ListenStatus_CharacterInactive )

#define	LISTEN_STATUS_CANTOPENAUDIO	( 3 )

#define	LISTEN_STATUS_COULDNTINITIALIZESPEECH	( ListenStatus_InitializeFailed )

#define	LISTEN_STATUS_SPEECHDISABLED	( ListenStatus_SpeechDisabled )

#define	LISTEN_STATUS_ERROR	( ListenStatus_Error )

#define	LSCOMPLETE_CAUSE_PROGRAMDISABLED	( ListenComplete_ProgramDisabled )

#define	LSCOMPLETE_CAUSE_PROGRAMTIMEDOUT	( ListenComplete_ProgramTimedOut )

#define	LSCOMPLETE_CAUSE_USERTIMEDOUT	( ListenComplete_UserTimedOut )

#define	LSCOMPLETE_CAUSE_USERRELEASEDKEY	( ListenComplete_UserReleasedKey )

#define	LSCOMPLETE_CAUSE_USERUTTERANCEENDED	( ListenComplete_UserSpeechEnded )

#define	LSCOMPLETE_CAUSE_CLIENTDEACTIVATED	( ListenComplete_CharacterClientDeactivated )

#define	LSCOMPLETE_CAUSE_DEFAULTCHARCHANGE	( ListenComplete_DefaultCharacterChanged )

#define	LSCOMPLETE_CAUSE_USERDISABLED	( ListenComplete_UserDisabled )

#ifndef GENDER_NEUTRAL
#define GENDER_NEUTRAL 0
#endif
#ifndef GENDER_FEMALE
#define GENDER_FEMALE 1
#endif
#ifndef GENDER_MALE
#define GENDER_MALE 2
#endif
#define	DISPID_IAgent_Load	( 0x60020000 )

#define	DISPID_IAgent_Unload	( 0x60020001 )

#define	DISPID_IAgent_Register	( 0x60020002 )

#define	DISPID_IAgent_Unregister	( 0x60020003 )

#define	DISPID_IAgent_GetCharacter	( 0x60020004 )

#define	DISPID_IAgent_GetSuspended	( 0x60020005 )

#define	DISPID_IAgentEx_GetCharacterEx	( 0x60030000 )

#define	DISPID_IAgentEx_GetVersion	( 0x60030001 )

#define	DISPID_IAgentEx_ShowDefaultCharacterProperties	( 0x60030002 )

#define	DISPID_IAgentUserInput_GetCount	( 0x60020000 )

#define	DISPID_IAgentUserInput_GetItemID	( 0x60020001 )

#define	DISPID_IAgentUserInput_GetItemConfidence	( 0x60020002 )

#define	DISPID_IAgentUserInput_GetItemText	( 0x60020003 )

#define	DISPID_IAgentUserInput_GetAllItemData	( 0x60020004 )

#define	DISPID_IAgentCommand_SetCaption	( 0x60020000 )

#define	DISPID_IAgentCommand_GetCaption	( 0x60020001 )

#define	DISPID_IAgentCommand_SetVoice	( 0x60020002 )

#define	DISPID_IAgentCommand_GetVoice	( 0x60020003 )

#define	DISPID_IAgentCommand_SetEnabled	( 0x60020004 )

#define	DISPID_IAgentCommand_GetEnabled	( 0x60020005 )

#define	DISPID_IAgentCommand_SetVisible	( 0x60020006 )

#define	DISPID_IAgentCommand_GetVisible	( 0x60020007 )

#define	DISPID_IAgentCommand_SetConfidenceThreshold	( 0x60020008 )

#define	DISPID_IAgentCommand_GetConfidenceThreshold	( 0x60020009 )

#define	DISPID_IAgentCommand_SetConfidenceText	( 0x6002000a )

#define	DISPID_IAgentCommand_GetConfidenceText	( 0x6002000b )

#define	DISPID_IAgentCommand_GetID	( 0x6002000c )

#define	DISPID_IAgentCommandEx_SetHelpContextID	( 0x60030000 )

#define	DISPID_IAgentCommandEx_GetHelpContextID	( 0x60030001 )

#define	DISPID_IAgentCommandEx_SetVoiceCaption	( 0x60030002 )

#define	DISPID_IAgentCommandEx_GetVoiceCaption	( 0x60030003 )

#define	DISPID_IAgentCommands_GetCommand	( 0x60020000 )

#define	DISPID_IAgentCommands_GetCount	( 0x60020001 )

#define	DISPID_IAgentCommands_SetCaption	( 0x60020002 )

#define	DISPID_IAgentCommands_GetCaption	( 0x60020003 )

#define	DISPID_IAgentCommands_SetVoice	( 0x60020004 )

#define	DISPID_IAgentCommands_GetVoice	( 0x60020005 )

#define	DISPID_IAgentCommands_SetVisible	( 0x60020006 )

#define	DISPID_IAgentCommands_GetVisible	( 0x60020007 )

#define	DISPID_IAgentCommands_Add	( 0x60020008 )

#define	DISPID_IAgentCommands_Insert	( 0x60020009 )

#define	DISPID_IAgentCommands_Remove	( 0x6002000a )

#define	DISPID_IAgentCommands_RemoveAll	( 0x6002000b )

#define	DISPID_IAgentCommandsEx_GetCommandEx	( 0x60030000 )

#define	DISPID_IAgentCommandsEx_SetDefaultID	( 0x60030001 )

#define	DISPID_IAgentCommandsEx_GetDefaultID	( 0x60030002 )

#define	DISPID_IAgentCommandsEx_SetHelpContextID	( 0x60030003 )

#define	DISPID_IAgentCommandsEx_GetHelpContextID	( 0x60030004 )

#define	DISPID_IAgentCommandsEx_SetFontName	( 0x60030005 )

#define	DISPID_IAgentCommandsEx_GetFontName	( 0x60030006 )

#define	DISPID_IAgentCommandsEx_SetFontSize	( 0x60030007 )

#define	DISPID_IAgentCommandsEx_GetFontSize	( 0x60030008 )

#define	DISPID_IAgentCommandsEx_SetVoiceCaption	( 0x60030009 )

#define	DISPID_IAgentCommandsEx_GetVoiceCaption	( 0x6003000a )

#define	DISPID_IAgentCommandsEx_AddEx	( 0x6003000b )

#define	DISPID_IAgentCommandsEx_InsertEx	( 0x6003000c )

#define	DISPID_IAgentCommandsEx_SetGlobalVoiceCommandsEnabled	( 0x6003000d )

#define	DISPID_IAgentCommandsEx_GetGlobalVoiceCommandsEnabled	( 0x6003000e )

#define	DISPID_IAgentSpeechInputProperties_GetInstalled	( 0x60020000 )

#define	DISPID_IAgentSpeechInputProperties_GetEnabled	( 0x60020001 )

#define	DISPID_IAgentSpeechInputProperties_GetHotKey	( 0x60020002 )

#define	DISPID_IAgentSpeechInputProperties_GetLCID	( 0x60020003 )

#define	DISPID_IAgentSpeechInputProperties_GetEngine	( 0x60020004 )

#define	DISPID_IAgentSpeechInputProperties_SetEngine	( 0x60020005 )

#define	DISPID_IAgentSpeechInputProperties_GetListeningTip	( 0x60020006 )

#define	DISPID_IAgentAudioOutputProperties_GetEnabled	( 0x60020000 )

#define	DISPID_IAgentAudioOutputProperties_GetUsingSoundEffects	( 0x60020001 )

#define	DISPID_IAgentAudioOutputPropertiesEx_GetStatus	( 0x60030000 )

#define	DISPID_ISvrPropertySheet_GetVisible	( 0x60020000 )

#define	DISPID_ISvrPropertySheet_SetVisible	( 0x60020001 )

#define	DISPID_ISvrPropertySheet_GetPosition	( 0x60020002 )

#define	DISPID_ISvrPropertySheet_GetSize	( 0x60020003 )

#define	DISPID_ISvrPropertySheet_GetPage	( 0x60020004 )

#define	DISPID_ISvrPropertySheet_SetPage	( 0x60020005 )

#define	DISPID_IAgentBalloon_GetEnabled	( 0x60020000 )

#define	DISPID_IAgentBalloon_GetNumLines	( 0x60020001 )

#define	DISPID_IAgentBalloon_GetNumCharsPerLine	( 0x60020002 )

#define	DISPID_IAgentBalloon_GetFontName	( 0x60020003 )

#define	DISPID_IAgentBalloon_GetFontSize	( 0x60020004 )

#define	DISPID_IAgentBalloon_GetFontBold	( 0x60020005 )

#define	DISPID_IAgentBalloon_GetFontItalic	( 0x60020006 )

#define	DISPID_IAgentBalloon_GetFontStrikethru	( 0x60020007 )

#define	DISPID_IAgentBalloon_GetFontUnderline	( 0x60020008 )

#define	DISPID_IAgentBalloon_GetForeColor	( 0x60020009 )

#define	DISPID_IAgentBalloon_GetBackColor	( 0x6002000a )

#define	DISPID_IAgentBalloon_GetBorderColor	( 0x6002000b )

#define	DISPID_IAgentBalloon_SetVisible	( 0x6002000c )

#define	DISPID_IAgentBalloon_GetVisible	( 0x6002000d )

#define	DISPID_IAgentBalloon_SetFontName	( 0x6002000e )

#define	DISPID_IAgentBalloon_SetFontSize	( 0x6002000f )

#define	DISPID_IAgentBalloon_SetFontCharSet	( 0x60020010 )

#define	DISPID_IAgentBalloon_GetFontCharSet	( 0x60020011 )

#define	DISPID_IAgentBalloonEx_SetStyle	( 0x60030000 )

#define	DISPID_IAgentBalloonEx_GetStyle	( 0x60030001 )

#define	DISPID_IAgentBalloonEx_SetNumLines	( 0x60030002 )

#define	DISPID_IAgentBalloonEx_SetNumCharsPerLine	( 0x60030003 )

#define	DISPID_IAgentCharacter_GetVisible	( 0x60020000 )

#define	DISPID_IAgentCharacter_SetPosition	( 0x60020001 )

#define	DISPID_IAgentCharacter_GetPosition	( 0x60020002 )

#define	DISPID_IAgentCharacter_SetSize	( 0x60020003 )

#define	DISPID_IAgentCharacter_GetSize	( 0x60020004 )

#define	DISPID_IAgentCharacter_GetName	( 0x60020005 )

#define	DISPID_IAgentCharacter_GetDescription	( 0x60020006 )

#define	DISPID_IAgentCharacter_GetTTSSpeed	( 0x60020007 )

#define	DISPID_IAgentCharacter_GetTTSPitch	( 0x60020008 )

#define	DISPID_IAgentCharacter_Activate	( 0x60020009 )

#define	DISPID_IAgentCharacter_SetIdleOn	( 0x6002000a )

#define	DISPID_IAgentCharacter_GetIdleOn	( 0x6002000b )

#define	DISPID_IAgentCharacter_Prepare	( 0x6002000c )

#define	DISPID_IAgentCharacter_Play	( 0x6002000d )

#define	DISPID_IAgentCharacter_Stop	( 0x6002000e )

#define	DISPID_IAgentCharacter_StopAll	( 0x6002000f )

#define	DISPID_IAgentCharacter_Wait	( 0x60020010 )

#define	DISPID_IAgentCharacter_Interrupt	( 0x60020011 )

#define	DISPID_IAgentCharacter_Show	( 0x60020012 )

#define	DISPID_IAgentCharacter_Hide	( 0x60020013 )

#define	DISPID_IAgentCharacter_Speak	( 0x60020014 )

#define	DISPID_IAgentCharacter_MoveTo	( 0x60020015 )

#define	DISPID_IAgentCharacter_GestureAt	( 0x60020016 )

#define	DISPID_IAgentCharacter_GetMoveCause	( 0x60020017 )

#define	DISPID_IAgentCharacter_GetVisibilityCause	( 0x60020018 )

#define	DISPID_IAgentCharacter_HasOtherClients	( 0x60020019 )

#define	DISPID_IAgentCharacter_SetSoundEffectsOn	( 0x6002001a )

#define	DISPID_IAgentCharacter_GetSoundEffectsOn	( 0x6002001b )

#define	DISPID_IAgentCharacter_SetName	( 0x6002001c )

#define	DISPID_IAgentCharacter_SetDescription	( 0x6002001d )

#define	DISPID_IAgentCharacter_GetExtraData	( 0x6002001e )

#define	DISPID_IAgentCharacterEx_ShowPopupMenu	( 0x60030000 )

#define	DISPID_IAgentCharacterEx_SetAutoPopupMenu	( 0x60030001 )

#define	DISPID_IAgentCharacterEx_GetAutoPopupMenu	( 0x60030002 )

#define	DISPID_IAgentCharacterEx_GetHelpFileName	( 0x60030003 )

#define	DISPID_IAgentCharacterEx_SetHelpFileName	( 0x60030004 )

#define	DISPID_IAgentCharacterEx_SetHelpModeOn	( 0x60030005 )

#define	DISPID_IAgentCharacterEx_GetHelpModeOn	( 0x60030006 )

#define	DISPID_IAgentCharacterEx_SetHelpContextID	( 0x60030007 )

#define	DISPID_IAgentCharacterEx_GetHelpContextID	( 0x60030008 )

#define	DISPID_IAgentCharacterEx_GetActive	( 0x60030009 )

#define	DISPID_IAgentCharacterEx_Listen	( 0x6003000a )

#define	DISPID_IAgentCharacterEx_SetLanguageID	( 0x6003000b )

#define	DISPID_IAgentCharacterEx_GetLanguageID	( 0x6003000c )

#define	DISPID_IAgentCharacterEx_GetTTSModeID	( 0x6003000d )

#define	DISPID_IAgentCharacterEx_SetTTSModeID	( 0x6003000e )

#define	DISPID_IAgentCharacterEx_GetSRModeID	( 0x6003000f )

#define	DISPID_IAgentCharacterEx_SetSRModeID	( 0x60030010 )

#define	DISPID_IAgentCharacterEx_GetGUID	( 0x60030011 )

#define	DISPID_IAgentCharacterEx_GetOriginalSize	( 0x60030012 )

#define	DISPID_IAgentCharacterEx_Think	( 0x60030013 )

#define	DISPID_IAgentCharacterEx_GetVersion	( 0x60030014 )

#define	DISPID_IAgentCharacterEx_GetAnimationNames	( 0x60030015 )

#define	DISPID_IAgentCharacterEx_GetSRStatus	( 0x60030016 )

#define	DISPID_IAgentNotifySink_Command	( 0x60020000 )

#define	DISPID_IAgentNotifySink_ActivateInputState	( 0x60020001 )

#define	DISPID_IAgentNotifySink_Restart	( 0x60020002 )

#define	DISPID_IAgentNotifySink_Shutdown	( 0x60020003 )

#define	DISPID_IAgentNotifySink_VisibleState	( 0x60020004 )

#define	DISPID_IAgentNotifySink_Click	( 0x60020005 )

#define	DISPID_IAgentNotifySink_DblClick	( 0x60020006 )

#define	DISPID_IAgentNotifySink_DragStart	( 0x60020007 )

#define	DISPID_IAgentNotifySink_DragComplete	( 0x60020008 )

#define	DISPID_IAgentNotifySink_RequestStart	( 0x60020009 )

#define	DISPID_IAgentNotifySink_RequestComplete	( 0x6002000a )

#define	DISPID_IAgentNotifySink_BookMark	( 0x6002000b )

#define	DISPID_IAgentNotifySink_Idle	( 0x6002000c )

#define	DISPID_IAgentNotifySink_Move	( 0x6002000d )

#define	DISPID_IAgentNotifySink_Size	( 0x6002000e )

#define	DISPID_IAgentNotifySink_BalloonVisibleState	( 0x6002000f )

#define	DISPID_IAgentNotifySinkEx_HelpComplete	( 0x60030000 )

#define	DISPID_IAgentNotifySinkEx_ListeningState	( 0x60030001 )

#define	DISPID_IAgentNotifySinkEx_DefaultCharacterChange	( 0x60030002 )

#define	DISPID_IAgentNotifySinkEx_AgentPropertyChange	( 0x60030003 )

#define	DISPID_IAgentNotifySinkEx_ActiveClientChange	( 0x60030004 )

#define	DISPID_ISvrCommandWindow_SetVisible	( 0x60020000 )

#define	DISPID_ISvrCommandWindow_GetVisible	( 0x60020001 )

#define	DISPID_ISvrCommandWindow_GetPosition	( 0x60020002 )

#define	DISPID_ISvrCommandWindow_GetSize	( 0x60020003 )

#define	DISPID_IDaSvrCharacterFiles_FilePaths	( DISPID_VALUE )

#define	DISPID_IDaSvrCharacterFiles_SearchPath	( 0x60020000 )

#define	DISPID_IDaSvrCharacterFiles_DefaultSearchPath	( 0x60020001 )

#define	DISPID_IDaSvrCharacterFiles_Filter	( 0x60020003 )

#define	DISPID_IDaSvrSpeechEngine_GetTTSModeID	( 0x60020000 )

#define	DISPID_IDaSvrSpeechEngine_GetDisplayName	( 0x60020001 )

#define	DISPID_IDaSvrSpeechEngine_GetManufacturer	( 0x60020002 )

#define	DISPID_IDaSvrSpeechEngine_GetVersion	( 0x60020003 )

#define	DISPID_IDaSvrSpeechEngine_GetGender	( 0x60020004 )

#define	DISPID_IDaSvrSpeechEngine_GetLanguageID	( 0x60020005 )

#define	DISPID_IDaSvrSpeechEngine_GetLanguageName	( 0x60020006 )

#define	DISPID_IDaSvrRecognitionEngine_GetSRModeID	( 0x60020000 )

#define	DISPID_IDaSvrRecognitionEngine_GetDisplayName	( 0x60020001 )

#define	DISPID_IDaSvrRecognitionEngine_GetManufacturer	( 0x60020002 )

#define	DISPID_IDaSvrRecognitionEngine_GetVersion	( 0x60020003 )

#define	DISPID_IDaSvrRecognitionEngine_GetLanguageID	( 0x60020004 )

#define	DISPID_IDaSvrRecognitionEngine_GetLanguageName	( 0x60020005 )

#define	DISPID_IDaSvrRecognitionEngine_GetLanguageIDs	( 0x60020006 )

#define	DISPID_IDaSvrRecognitionEngine_GetLanguageNames	( 0x60020007 )

#define	DISPID_IDaServer2_GetCharacter2	( 0x60040000 )

#define	DISPID_IDaServer2_GetCharacterFiles	( 0x60040001 )

#define	DISPID_IDaServer2_CharacterStyle	( 0x60040002 )

#define	DISPID_IDaServer2_GetSpeechEngines	( 0x60040003 )

#define	DISPID_IDaServer2_FindSpeechEngines	( 0x60040004 )

#define	DISPID_IDaServer2_GetCharacterSpeechEngine	( 0x60040005 )

#define	DISPID_IDaServer2_FindCharacterSpeechEngines	( 0x60040006 )

#define	DISPID_IDaServer2_GetRecognitionEngines	( 0x60040007 )

#define	DISPID_IDaServer2_FindRecognitionEngines	( 0x60040008 )

#define	DISPID_IDaServer2_GetCharacterRecognitionEngine	( 0x60040009 )

#define	DISPID_IDaServer2_FindCharacterRecognitionEngines	( 0x6004000a )

#define	DISPID_IDaSvrCharacter2_Style	( 0x60040000 )

#define	DISPID_IDaSvrCharacter2_HasIcon	( 0x60040001 )

#define	DISPID_IDaSvrCharacter2_GenerateIcon	( 0x60040002 )

#define	DISPID_IDaSvrCharacter2_IconVisible	( 0x60040003 )

#define	DISPID_IDaSvrCharacter2_GetIconIdentity	( 0x60040004 )

#define	DISPID_IDaSvrCharacter2_SetIconIdentity	( 0x60040005 )

#define	DISPID_IDaSvrCharacter2_IconIdentity	( 0x60040006 )

#define	DISPID_IDaSvrCharacter2_IconTip	( 0x60040007 )

#define	DISPID_IDaSvrCharacter2_GetSpeechEngine	( 0x60040008 )

#define	DISPID_IDaSvrCharacter2_FindSpeechEngines	( 0x60040009 )

#define	DISPID_IDaSvrCharacter2_GetRecognitionEngine	( 0x6004000a )

#define	DISPID_IDaSvrCharacter2_FindRecognitionEngines	( 0x6004000b )


EXTERN_C const IID LIBID_DoubleAgentSvr;

#ifndef __IDaServer_INTERFACE_DEFINED__
#define __IDaServer_INTERFACE_DEFINED__

/* interface IDaServer */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaServer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E502-A208-11DE-ABF2-002421116FB2")
    IDaServer : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Load( 
            /* [in] */ VARIANT vLoadKey,
            /* [out] */ long *pdwCharID,
            /* [out] */ long *pdwReqID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Unload( 
            /* [in] */ long dwCharID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Register( 
            /* [in] */ IUnknown *punkNotifySink,
            /* [out] */ long *pdwSinkID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Unregister( 
            /* [in] */ long dwSinkID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCharacter( 
            /* [in] */ long dwCharID,
            /* [out] */ IDispatch **ppunkCharacter) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE GetSuspended( 
            /* [out] */ long *pbSuspended) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCharacterEx( 
            /* [in] */ long dwCharID,
            /* [out] */ IDaSvrCharacter **ppCharacterEx) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVersion( 
            /* [out] */ short *psMajor,
            /* [out] */ short *psMinor) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ShowDefaultCharacterProperties( 
            /* [in] */ short x,
            /* [in] */ short y,
            /* [in] */ long bUseDefaultPosition) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaServerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaServer * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaServer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaServer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaServer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaServer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaServer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaServer * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Load )( 
            IDaServer * This,
            /* [in] */ VARIANT vLoadKey,
            /* [out] */ long *pdwCharID,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Unload )( 
            IDaServer * This,
            /* [in] */ long dwCharID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Register )( 
            IDaServer * This,
            /* [in] */ IUnknown *punkNotifySink,
            /* [out] */ long *pdwSinkID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Unregister )( 
            IDaServer * This,
            /* [in] */ long dwSinkID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCharacter )( 
            IDaServer * This,
            /* [in] */ long dwCharID,
            /* [out] */ IDispatch **ppunkCharacter);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetSuspended )( 
            IDaServer * This,
            /* [out] */ long *pbSuspended);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCharacterEx )( 
            IDaServer * This,
            /* [in] */ long dwCharID,
            /* [out] */ IDaSvrCharacter **ppCharacterEx);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVersion )( 
            IDaServer * This,
            /* [out] */ short *psMajor,
            /* [out] */ short *psMinor);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ShowDefaultCharacterProperties )( 
            IDaServer * This,
            /* [in] */ short x,
            /* [in] */ short y,
            /* [in] */ long bUseDefaultPosition);
        
        END_INTERFACE
    } IDaServerVtbl;

    interface IDaServer
    {
        CONST_VTBL struct IDaServerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaServer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaServer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaServer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaServer_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaServer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaServer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaServer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaServer_Load(This,vLoadKey,pdwCharID,pdwReqID)	\
    ( (This)->lpVtbl -> Load(This,vLoadKey,pdwCharID,pdwReqID) ) 

#define IDaServer_Unload(This,dwCharID)	\
    ( (This)->lpVtbl -> Unload(This,dwCharID) ) 

#define IDaServer_Register(This,punkNotifySink,pdwSinkID)	\
    ( (This)->lpVtbl -> Register(This,punkNotifySink,pdwSinkID) ) 

#define IDaServer_Unregister(This,dwSinkID)	\
    ( (This)->lpVtbl -> Unregister(This,dwSinkID) ) 

#define IDaServer_GetCharacter(This,dwCharID,ppunkCharacter)	\
    ( (This)->lpVtbl -> GetCharacter(This,dwCharID,ppunkCharacter) ) 

#define IDaServer_GetSuspended(This,pbSuspended)	\
    ( (This)->lpVtbl -> GetSuspended(This,pbSuspended) ) 

#define IDaServer_GetCharacterEx(This,dwCharID,ppCharacterEx)	\
    ( (This)->lpVtbl -> GetCharacterEx(This,dwCharID,ppCharacterEx) ) 

#define IDaServer_GetVersion(This,psMajor,psMinor)	\
    ( (This)->lpVtbl -> GetVersion(This,psMajor,psMinor) ) 

#define IDaServer_ShowDefaultCharacterProperties(This,x,y,bUseDefaultPosition)	\
    ( (This)->lpVtbl -> ShowDefaultCharacterProperties(This,x,y,bUseDefaultPosition) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaServer_INTERFACE_DEFINED__ */


#ifndef __IDaSvrUserInput_INTERFACE_DEFINED__
#define __IDaSvrUserInput_INTERFACE_DEFINED__

/* interface IDaSvrUserInput */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrUserInput;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E508-A208-11DE-ABF2-002421116FB2")
    IDaSvrUserInput : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCount( 
            /* [out] */ long *pdwCount) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetItemID( 
            /* [in] */ long dwItemIndex,
            /* [out] */ long *pdwCommandID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetItemConfidence( 
            /* [in] */ long dwItemIndex,
            /* [out] */ long *plConfidence) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetItemText( 
            /* [in] */ long dwItemIndex,
            /* [out] */ BSTR *pbszText) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetAllItemData( 
            /* [out] */ VARIANT *pdwItemIndices,
            /* [out] */ VARIANT *plConfidences,
            /* [out] */ VARIANT *pbszText) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrUserInputVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrUserInput * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrUserInput * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrUserInput * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrUserInput * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrUserInput * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrUserInput * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrUserInput * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IDaSvrUserInput * This,
            /* [out] */ long *pdwCount);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetItemID )( 
            IDaSvrUserInput * This,
            /* [in] */ long dwItemIndex,
            /* [out] */ long *pdwCommandID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetItemConfidence )( 
            IDaSvrUserInput * This,
            /* [in] */ long dwItemIndex,
            /* [out] */ long *plConfidence);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetItemText )( 
            IDaSvrUserInput * This,
            /* [in] */ long dwItemIndex,
            /* [out] */ BSTR *pbszText);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetAllItemData )( 
            IDaSvrUserInput * This,
            /* [out] */ VARIANT *pdwItemIndices,
            /* [out] */ VARIANT *plConfidences,
            /* [out] */ VARIANT *pbszText);
        
        END_INTERFACE
    } IDaSvrUserInputVtbl;

    interface IDaSvrUserInput
    {
        CONST_VTBL struct IDaSvrUserInputVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrUserInput_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrUserInput_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrUserInput_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrUserInput_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrUserInput_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrUserInput_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrUserInput_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrUserInput_GetCount(This,pdwCount)	\
    ( (This)->lpVtbl -> GetCount(This,pdwCount) ) 

#define IDaSvrUserInput_GetItemID(This,dwItemIndex,pdwCommandID)	\
    ( (This)->lpVtbl -> GetItemID(This,dwItemIndex,pdwCommandID) ) 

#define IDaSvrUserInput_GetItemConfidence(This,dwItemIndex,plConfidence)	\
    ( (This)->lpVtbl -> GetItemConfidence(This,dwItemIndex,plConfidence) ) 

#define IDaSvrUserInput_GetItemText(This,dwItemIndex,pbszText)	\
    ( (This)->lpVtbl -> GetItemText(This,dwItemIndex,pbszText) ) 

#define IDaSvrUserInput_GetAllItemData(This,pdwItemIndices,plConfidences,pbszText)	\
    ( (This)->lpVtbl -> GetAllItemData(This,pdwItemIndices,plConfidences,pbszText) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrUserInput_INTERFACE_DEFINED__ */


#ifndef __IDaSvrCommand_INTERFACE_DEFINED__
#define __IDaSvrCommand_INTERFACE_DEFINED__

/* interface IDaSvrCommand */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrCommand;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E506-A208-11DE-ABF2-002421116FB2")
    IDaSvrCommand : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetCaption( 
            /* [in] */ BSTR bszCaption) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCaption( 
            /* [out] */ BSTR *pbszCaption) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetVoice( 
            /* [in] */ BSTR bszVoice) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVoice( 
            /* [out] */ BSTR *pbszVoice) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetEnabled( 
            /* [in] */ long bEnabled) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetEnabled( 
            /* [out] */ long *pbEnabled) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetVisible( 
            /* [in] */ long bVisible) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVisible( 
            /* [out] */ long *pbVisible) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetConfidenceThreshold( 
            /* [in] */ long lThreshold) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetConfidenceThreshold( 
            /* [out] */ long *plThreshold) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetConfidenceText( 
            /* [in] */ BSTR bszTipText) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetConfidenceText( 
            /* [out] */ BSTR *pbszTipText) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetID( 
            /* [out] */ long *pdwID) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE SetHelpContextID( 
            /* [in] */ long ulID) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE GetHelpContextID( 
            /* [out] */ long *pulID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetVoiceCaption( 
            /* [in] */ BSTR bszVoiceCaption) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVoiceCaption( 
            /* [out] */ BSTR *pbszVoiceCaption) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrCommandVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrCommand * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrCommand * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrCommand * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrCommand * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrCommand * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrCommand * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrCommand * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetCaption )( 
            IDaSvrCommand * This,
            /* [in] */ BSTR bszCaption);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCaption )( 
            IDaSvrCommand * This,
            /* [out] */ BSTR *pbszCaption);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetVoice )( 
            IDaSvrCommand * This,
            /* [in] */ BSTR bszVoice);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVoice )( 
            IDaSvrCommand * This,
            /* [out] */ BSTR *pbszVoice);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetEnabled )( 
            IDaSvrCommand * This,
            /* [in] */ long bEnabled);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetEnabled )( 
            IDaSvrCommand * This,
            /* [out] */ long *pbEnabled);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetVisible )( 
            IDaSvrCommand * This,
            /* [in] */ long bVisible);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVisible )( 
            IDaSvrCommand * This,
            /* [out] */ long *pbVisible);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetConfidenceThreshold )( 
            IDaSvrCommand * This,
            /* [in] */ long lThreshold);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetConfidenceThreshold )( 
            IDaSvrCommand * This,
            /* [out] */ long *plThreshold);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetConfidenceText )( 
            IDaSvrCommand * This,
            /* [in] */ BSTR bszTipText);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetConfidenceText )( 
            IDaSvrCommand * This,
            /* [out] */ BSTR *pbszTipText);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetID )( 
            IDaSvrCommand * This,
            /* [out] */ long *pdwID);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *SetHelpContextID )( 
            IDaSvrCommand * This,
            /* [in] */ long ulID);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetHelpContextID )( 
            IDaSvrCommand * This,
            /* [out] */ long *pulID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetVoiceCaption )( 
            IDaSvrCommand * This,
            /* [in] */ BSTR bszVoiceCaption);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVoiceCaption )( 
            IDaSvrCommand * This,
            /* [out] */ BSTR *pbszVoiceCaption);
        
        END_INTERFACE
    } IDaSvrCommandVtbl;

    interface IDaSvrCommand
    {
        CONST_VTBL struct IDaSvrCommandVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrCommand_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrCommand_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrCommand_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrCommand_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrCommand_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrCommand_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrCommand_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrCommand_SetCaption(This,bszCaption)	\
    ( (This)->lpVtbl -> SetCaption(This,bszCaption) ) 

#define IDaSvrCommand_GetCaption(This,pbszCaption)	\
    ( (This)->lpVtbl -> GetCaption(This,pbszCaption) ) 

#define IDaSvrCommand_SetVoice(This,bszVoice)	\
    ( (This)->lpVtbl -> SetVoice(This,bszVoice) ) 

#define IDaSvrCommand_GetVoice(This,pbszVoice)	\
    ( (This)->lpVtbl -> GetVoice(This,pbszVoice) ) 

#define IDaSvrCommand_SetEnabled(This,bEnabled)	\
    ( (This)->lpVtbl -> SetEnabled(This,bEnabled) ) 

#define IDaSvrCommand_GetEnabled(This,pbEnabled)	\
    ( (This)->lpVtbl -> GetEnabled(This,pbEnabled) ) 

#define IDaSvrCommand_SetVisible(This,bVisible)	\
    ( (This)->lpVtbl -> SetVisible(This,bVisible) ) 

#define IDaSvrCommand_GetVisible(This,pbVisible)	\
    ( (This)->lpVtbl -> GetVisible(This,pbVisible) ) 

#define IDaSvrCommand_SetConfidenceThreshold(This,lThreshold)	\
    ( (This)->lpVtbl -> SetConfidenceThreshold(This,lThreshold) ) 

#define IDaSvrCommand_GetConfidenceThreshold(This,plThreshold)	\
    ( (This)->lpVtbl -> GetConfidenceThreshold(This,plThreshold) ) 

#define IDaSvrCommand_SetConfidenceText(This,bszTipText)	\
    ( (This)->lpVtbl -> SetConfidenceText(This,bszTipText) ) 

#define IDaSvrCommand_GetConfidenceText(This,pbszTipText)	\
    ( (This)->lpVtbl -> GetConfidenceText(This,pbszTipText) ) 

#define IDaSvrCommand_GetID(This,pdwID)	\
    ( (This)->lpVtbl -> GetID(This,pdwID) ) 

#define IDaSvrCommand_SetHelpContextID(This,ulID)	\
    ( (This)->lpVtbl -> SetHelpContextID(This,ulID) ) 

#define IDaSvrCommand_GetHelpContextID(This,pulID)	\
    ( (This)->lpVtbl -> GetHelpContextID(This,pulID) ) 

#define IDaSvrCommand_SetVoiceCaption(This,bszVoiceCaption)	\
    ( (This)->lpVtbl -> SetVoiceCaption(This,bszVoiceCaption) ) 

#define IDaSvrCommand_GetVoiceCaption(This,pbszVoiceCaption)	\
    ( (This)->lpVtbl -> GetVoiceCaption(This,pbszVoiceCaption) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrCommand_INTERFACE_DEFINED__ */


#ifndef __IDaSvrCommands_INTERFACE_DEFINED__
#define __IDaSvrCommands_INTERFACE_DEFINED__

/* interface IDaSvrCommands */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrCommands;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E505-A208-11DE-ABF2-002421116FB2")
    IDaSvrCommands : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCommand( 
            /* [in] */ long dwCommandID,
            /* [out] */ IUnknown **ppunkCommand) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCount( 
            /* [out] */ long *pdwCount) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetCaption( 
            /* [in] */ BSTR bszCaption) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCaption( 
            /* [out] */ BSTR *pbszCaption) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetVoice( 
            /* [in] */ BSTR bszVoice) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVoice( 
            /* [out] */ BSTR *pbszVoice) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetVisible( 
            /* [in] */ long bVisible) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVisible( 
            /* [out] */ long *pbVisible) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ BSTR bszCaption,
            /* [in] */ BSTR bszVoice,
            /* [in] */ long bEnabled,
            /* [in] */ long bVisible,
            /* [out] */ long *pdwID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Insert( 
            /* [in] */ BSTR bszCaption,
            /* [in] */ BSTR bszVoice,
            /* [in] */ long bEnabled,
            /* [in] */ long bVisible,
            /* [in] */ long dwRefID,
            /* [in] */ long bBefore,
            /* [out] */ long *pdwID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Remove( 
            /* [in] */ long dwID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RemoveAll( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCommandEx( 
            /* [in] */ long dwCommandID,
            /* [out] */ IDaSvrCommand **ppCommandEx) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetDefaultID( 
            /* [in] */ long dwID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetDefaultID( 
            /* [out] */ long *pdwID) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE SetHelpContextID( 
            /* [in] */ long ulHelpID) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE GetHelpContextID( 
            /* [out] */ long *pulHelpID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetFontName( 
            /* [in] */ BSTR bszFontName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFontName( 
            /* [out] */ BSTR *pbszFontName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetFontSize( 
            /* [in] */ long lFontSize) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFontSize( 
            /* [out] */ long *lFontSize) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetVoiceCaption( 
            /* [in] */ BSTR bszVoiceCaption) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVoiceCaption( 
            /* [out] */ BSTR *bszVoiceCaption) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AddEx( 
            /* [in] */ BSTR bszCaption,
            /* [in] */ BSTR bszVoice,
            /* [in] */ BSTR bszVoiceCaption,
            /* [in] */ long bEnabled,
            /* [in] */ long bVisible,
            /* [in] */ long ulHelpID,
            /* [out] */ long *pdwID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE InsertEx( 
            /* [in] */ BSTR bszCaption,
            /* [in] */ BSTR bszVoice,
            /* [in] */ BSTR bszVoiceCaption,
            /* [in] */ long bEnabled,
            /* [in] */ long bVisible,
            /* [in] */ long ulHelpID,
            /* [in] */ long dwRefID,
            /* [in] */ long bBefore,
            /* [out] */ long *pdwID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetGlobalVoiceCommandsEnabled( 
            /* [in] */ long bEnable) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetGlobalVoiceCommandsEnabled( 
            /* [out] */ long *pbEnabled) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrCommandsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrCommands * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrCommands * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrCommands * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrCommands * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrCommands * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrCommands * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrCommands * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCommand )( 
            IDaSvrCommands * This,
            /* [in] */ long dwCommandID,
            /* [out] */ IUnknown **ppunkCommand);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IDaSvrCommands * This,
            /* [out] */ long *pdwCount);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetCaption )( 
            IDaSvrCommands * This,
            /* [in] */ BSTR bszCaption);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCaption )( 
            IDaSvrCommands * This,
            /* [out] */ BSTR *pbszCaption);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetVoice )( 
            IDaSvrCommands * This,
            /* [in] */ BSTR bszVoice);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVoice )( 
            IDaSvrCommands * This,
            /* [out] */ BSTR *pbszVoice);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetVisible )( 
            IDaSvrCommands * This,
            /* [in] */ long bVisible);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVisible )( 
            IDaSvrCommands * This,
            /* [out] */ long *pbVisible);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            IDaSvrCommands * This,
            /* [in] */ BSTR bszCaption,
            /* [in] */ BSTR bszVoice,
            /* [in] */ long bEnabled,
            /* [in] */ long bVisible,
            /* [out] */ long *pdwID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Insert )( 
            IDaSvrCommands * This,
            /* [in] */ BSTR bszCaption,
            /* [in] */ BSTR bszVoice,
            /* [in] */ long bEnabled,
            /* [in] */ long bVisible,
            /* [in] */ long dwRefID,
            /* [in] */ long bBefore,
            /* [out] */ long *pdwID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Remove )( 
            IDaSvrCommands * This,
            /* [in] */ long dwID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RemoveAll )( 
            IDaSvrCommands * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCommandEx )( 
            IDaSvrCommands * This,
            /* [in] */ long dwCommandID,
            /* [out] */ IDaSvrCommand **ppCommandEx);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetDefaultID )( 
            IDaSvrCommands * This,
            /* [in] */ long dwID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetDefaultID )( 
            IDaSvrCommands * This,
            /* [out] */ long *pdwID);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *SetHelpContextID )( 
            IDaSvrCommands * This,
            /* [in] */ long ulHelpID);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetHelpContextID )( 
            IDaSvrCommands * This,
            /* [out] */ long *pulHelpID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetFontName )( 
            IDaSvrCommands * This,
            /* [in] */ BSTR bszFontName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFontName )( 
            IDaSvrCommands * This,
            /* [out] */ BSTR *pbszFontName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetFontSize )( 
            IDaSvrCommands * This,
            /* [in] */ long lFontSize);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFontSize )( 
            IDaSvrCommands * This,
            /* [out] */ long *lFontSize);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetVoiceCaption )( 
            IDaSvrCommands * This,
            /* [in] */ BSTR bszVoiceCaption);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVoiceCaption )( 
            IDaSvrCommands * This,
            /* [out] */ BSTR *bszVoiceCaption);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AddEx )( 
            IDaSvrCommands * This,
            /* [in] */ BSTR bszCaption,
            /* [in] */ BSTR bszVoice,
            /* [in] */ BSTR bszVoiceCaption,
            /* [in] */ long bEnabled,
            /* [in] */ long bVisible,
            /* [in] */ long ulHelpID,
            /* [out] */ long *pdwID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *InsertEx )( 
            IDaSvrCommands * This,
            /* [in] */ BSTR bszCaption,
            /* [in] */ BSTR bszVoice,
            /* [in] */ BSTR bszVoiceCaption,
            /* [in] */ long bEnabled,
            /* [in] */ long bVisible,
            /* [in] */ long ulHelpID,
            /* [in] */ long dwRefID,
            /* [in] */ long bBefore,
            /* [out] */ long *pdwID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetGlobalVoiceCommandsEnabled )( 
            IDaSvrCommands * This,
            /* [in] */ long bEnable);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetGlobalVoiceCommandsEnabled )( 
            IDaSvrCommands * This,
            /* [out] */ long *pbEnabled);
        
        END_INTERFACE
    } IDaSvrCommandsVtbl;

    interface IDaSvrCommands
    {
        CONST_VTBL struct IDaSvrCommandsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrCommands_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrCommands_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrCommands_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrCommands_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrCommands_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrCommands_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrCommands_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrCommands_GetCommand(This,dwCommandID,ppunkCommand)	\
    ( (This)->lpVtbl -> GetCommand(This,dwCommandID,ppunkCommand) ) 

#define IDaSvrCommands_GetCount(This,pdwCount)	\
    ( (This)->lpVtbl -> GetCount(This,pdwCount) ) 

#define IDaSvrCommands_SetCaption(This,bszCaption)	\
    ( (This)->lpVtbl -> SetCaption(This,bszCaption) ) 

#define IDaSvrCommands_GetCaption(This,pbszCaption)	\
    ( (This)->lpVtbl -> GetCaption(This,pbszCaption) ) 

#define IDaSvrCommands_SetVoice(This,bszVoice)	\
    ( (This)->lpVtbl -> SetVoice(This,bszVoice) ) 

#define IDaSvrCommands_GetVoice(This,pbszVoice)	\
    ( (This)->lpVtbl -> GetVoice(This,pbszVoice) ) 

#define IDaSvrCommands_SetVisible(This,bVisible)	\
    ( (This)->lpVtbl -> SetVisible(This,bVisible) ) 

#define IDaSvrCommands_GetVisible(This,pbVisible)	\
    ( (This)->lpVtbl -> GetVisible(This,pbVisible) ) 

#define IDaSvrCommands_Add(This,bszCaption,bszVoice,bEnabled,bVisible,pdwID)	\
    ( (This)->lpVtbl -> Add(This,bszCaption,bszVoice,bEnabled,bVisible,pdwID) ) 

#define IDaSvrCommands_Insert(This,bszCaption,bszVoice,bEnabled,bVisible,dwRefID,bBefore,pdwID)	\
    ( (This)->lpVtbl -> Insert(This,bszCaption,bszVoice,bEnabled,bVisible,dwRefID,bBefore,pdwID) ) 

#define IDaSvrCommands_Remove(This,dwID)	\
    ( (This)->lpVtbl -> Remove(This,dwID) ) 

#define IDaSvrCommands_RemoveAll(This)	\
    ( (This)->lpVtbl -> RemoveAll(This) ) 

#define IDaSvrCommands_GetCommandEx(This,dwCommandID,ppCommandEx)	\
    ( (This)->lpVtbl -> GetCommandEx(This,dwCommandID,ppCommandEx) ) 

#define IDaSvrCommands_SetDefaultID(This,dwID)	\
    ( (This)->lpVtbl -> SetDefaultID(This,dwID) ) 

#define IDaSvrCommands_GetDefaultID(This,pdwID)	\
    ( (This)->lpVtbl -> GetDefaultID(This,pdwID) ) 

#define IDaSvrCommands_SetHelpContextID(This,ulHelpID)	\
    ( (This)->lpVtbl -> SetHelpContextID(This,ulHelpID) ) 

#define IDaSvrCommands_GetHelpContextID(This,pulHelpID)	\
    ( (This)->lpVtbl -> GetHelpContextID(This,pulHelpID) ) 

#define IDaSvrCommands_SetFontName(This,bszFontName)	\
    ( (This)->lpVtbl -> SetFontName(This,bszFontName) ) 

#define IDaSvrCommands_GetFontName(This,pbszFontName)	\
    ( (This)->lpVtbl -> GetFontName(This,pbszFontName) ) 

#define IDaSvrCommands_SetFontSize(This,lFontSize)	\
    ( (This)->lpVtbl -> SetFontSize(This,lFontSize) ) 

#define IDaSvrCommands_GetFontSize(This,lFontSize)	\
    ( (This)->lpVtbl -> GetFontSize(This,lFontSize) ) 

#define IDaSvrCommands_SetVoiceCaption(This,bszVoiceCaption)	\
    ( (This)->lpVtbl -> SetVoiceCaption(This,bszVoiceCaption) ) 

#define IDaSvrCommands_GetVoiceCaption(This,bszVoiceCaption)	\
    ( (This)->lpVtbl -> GetVoiceCaption(This,bszVoiceCaption) ) 

#define IDaSvrCommands_AddEx(This,bszCaption,bszVoice,bszVoiceCaption,bEnabled,bVisible,ulHelpID,pdwID)	\
    ( (This)->lpVtbl -> AddEx(This,bszCaption,bszVoice,bszVoiceCaption,bEnabled,bVisible,ulHelpID,pdwID) ) 

#define IDaSvrCommands_InsertEx(This,bszCaption,bszVoice,bszVoiceCaption,bEnabled,bVisible,ulHelpID,dwRefID,bBefore,pdwID)	\
    ( (This)->lpVtbl -> InsertEx(This,bszCaption,bszVoice,bszVoiceCaption,bEnabled,bVisible,ulHelpID,dwRefID,bBefore,pdwID) ) 

#define IDaSvrCommands_SetGlobalVoiceCommandsEnabled(This,bEnable)	\
    ( (This)->lpVtbl -> SetGlobalVoiceCommandsEnabled(This,bEnable) ) 

#define IDaSvrCommands_GetGlobalVoiceCommandsEnabled(This,pbEnabled)	\
    ( (This)->lpVtbl -> GetGlobalVoiceCommandsEnabled(This,pbEnabled) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrCommands_INTERFACE_DEFINED__ */


#ifndef __IDaSvrSpeechInput_INTERFACE_DEFINED__
#define __IDaSvrSpeechInput_INTERFACE_DEFINED__

/* interface IDaSvrSpeechInput */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrSpeechInput;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E50C-A208-11DE-ABF2-002421116FB2")
    IDaSvrSpeechInput : public IDispatch
    {
    public:
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE GetInstalled( 
            /* [out] */ long *pbInstalled) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetEnabled( 
            /* [out] */ long *pbEnabled) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetHotKey( 
            /* [out] */ BSTR *pbszHotCharKey) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE GetLCID( 
            /* [out] */ unsigned long *plcidCurrent) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE GetEngine( 
            /* [out] */ BSTR *pbszEngine) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE SetEngine( 
            /* [in] */ BSTR bszEngine) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetListeningTip( 
            /* [out] */ long *pbListeningTip) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrSpeechInputVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrSpeechInput * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrSpeechInput * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrSpeechInput * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrSpeechInput * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrSpeechInput * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrSpeechInput * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrSpeechInput * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetInstalled )( 
            IDaSvrSpeechInput * This,
            /* [out] */ long *pbInstalled);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetEnabled )( 
            IDaSvrSpeechInput * This,
            /* [out] */ long *pbEnabled);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetHotKey )( 
            IDaSvrSpeechInput * This,
            /* [out] */ BSTR *pbszHotCharKey);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetLCID )( 
            IDaSvrSpeechInput * This,
            /* [out] */ unsigned long *plcidCurrent);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetEngine )( 
            IDaSvrSpeechInput * This,
            /* [out] */ BSTR *pbszEngine);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *SetEngine )( 
            IDaSvrSpeechInput * This,
            /* [in] */ BSTR bszEngine);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetListeningTip )( 
            IDaSvrSpeechInput * This,
            /* [out] */ long *pbListeningTip);
        
        END_INTERFACE
    } IDaSvrSpeechInputVtbl;

    interface IDaSvrSpeechInput
    {
        CONST_VTBL struct IDaSvrSpeechInputVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrSpeechInput_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrSpeechInput_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrSpeechInput_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrSpeechInput_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrSpeechInput_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrSpeechInput_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrSpeechInput_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrSpeechInput_GetInstalled(This,pbInstalled)	\
    ( (This)->lpVtbl -> GetInstalled(This,pbInstalled) ) 

#define IDaSvrSpeechInput_GetEnabled(This,pbEnabled)	\
    ( (This)->lpVtbl -> GetEnabled(This,pbEnabled) ) 

#define IDaSvrSpeechInput_GetHotKey(This,pbszHotCharKey)	\
    ( (This)->lpVtbl -> GetHotKey(This,pbszHotCharKey) ) 

#define IDaSvrSpeechInput_GetLCID(This,plcidCurrent)	\
    ( (This)->lpVtbl -> GetLCID(This,plcidCurrent) ) 

#define IDaSvrSpeechInput_GetEngine(This,pbszEngine)	\
    ( (This)->lpVtbl -> GetEngine(This,pbszEngine) ) 

#define IDaSvrSpeechInput_SetEngine(This,bszEngine)	\
    ( (This)->lpVtbl -> SetEngine(This,bszEngine) ) 

#define IDaSvrSpeechInput_GetListeningTip(This,pbListeningTip)	\
    ( (This)->lpVtbl -> GetListeningTip(This,pbListeningTip) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrSpeechInput_INTERFACE_DEFINED__ */


#ifndef __IDaSvrAudioOutput_INTERFACE_DEFINED__
#define __IDaSvrAudioOutput_INTERFACE_DEFINED__

/* interface IDaSvrAudioOutput */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrAudioOutput;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E50B-A208-11DE-ABF2-002421116FB2")
    IDaSvrAudioOutput : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetEnabled( 
            /* [out] */ long *pbEnabled) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetUsingSoundEffects( 
            /* [out] */ long *pbUsingSoundEffects) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE GetStatus( 
            /* [out] */ long *plStatus) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrAudioOutputVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrAudioOutput * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrAudioOutput * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrAudioOutput * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrAudioOutput * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrAudioOutput * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrAudioOutput * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrAudioOutput * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetEnabled )( 
            IDaSvrAudioOutput * This,
            /* [out] */ long *pbEnabled);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetUsingSoundEffects )( 
            IDaSvrAudioOutput * This,
            /* [out] */ long *pbUsingSoundEffects);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetStatus )( 
            IDaSvrAudioOutput * This,
            /* [out] */ long *plStatus);
        
        END_INTERFACE
    } IDaSvrAudioOutputVtbl;

    interface IDaSvrAudioOutput
    {
        CONST_VTBL struct IDaSvrAudioOutputVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrAudioOutput_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrAudioOutput_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrAudioOutput_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrAudioOutput_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrAudioOutput_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrAudioOutput_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrAudioOutput_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrAudioOutput_GetEnabled(This,pbEnabled)	\
    ( (This)->lpVtbl -> GetEnabled(This,pbEnabled) ) 

#define IDaSvrAudioOutput_GetUsingSoundEffects(This,pbUsingSoundEffects)	\
    ( (This)->lpVtbl -> GetUsingSoundEffects(This,pbUsingSoundEffects) ) 

#define IDaSvrAudioOutput_GetStatus(This,plStatus)	\
    ( (This)->lpVtbl -> GetStatus(This,plStatus) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrAudioOutput_INTERFACE_DEFINED__ */


#ifndef __IDaSvrPropertySheet_INTERFACE_DEFINED__
#define __IDaSvrPropertySheet_INTERFACE_DEFINED__

/* interface IDaSvrPropertySheet */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrPropertySheet;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E50A-A208-11DE-ABF2-002421116FB2")
    IDaSvrPropertySheet : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVisible( 
            /* [out] */ long *pbVisible) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetVisible( 
            /* [in] */ long bVisible) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetPosition( 
            /* [out] */ long *plLeft,
            /* [out] */ long *plTop) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetSize( 
            /* [out] */ long *plWidth,
            /* [out] */ long *plHeight) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetPage( 
            /* [out] */ BSTR *pbszPage) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetPage( 
            /* [in] */ BSTR bszPage) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrPropertySheetVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrPropertySheet * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrPropertySheet * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrPropertySheet * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrPropertySheet * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrPropertySheet * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrPropertySheet * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrPropertySheet * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVisible )( 
            IDaSvrPropertySheet * This,
            /* [out] */ long *pbVisible);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetVisible )( 
            IDaSvrPropertySheet * This,
            /* [in] */ long bVisible);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetPosition )( 
            IDaSvrPropertySheet * This,
            /* [out] */ long *plLeft,
            /* [out] */ long *plTop);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetSize )( 
            IDaSvrPropertySheet * This,
            /* [out] */ long *plWidth,
            /* [out] */ long *plHeight);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetPage )( 
            IDaSvrPropertySheet * This,
            /* [out] */ BSTR *pbszPage);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetPage )( 
            IDaSvrPropertySheet * This,
            /* [in] */ BSTR bszPage);
        
        END_INTERFACE
    } IDaSvrPropertySheetVtbl;

    interface IDaSvrPropertySheet
    {
        CONST_VTBL struct IDaSvrPropertySheetVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrPropertySheet_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrPropertySheet_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrPropertySheet_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrPropertySheet_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrPropertySheet_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrPropertySheet_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrPropertySheet_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrPropertySheet_GetVisible(This,pbVisible)	\
    ( (This)->lpVtbl -> GetVisible(This,pbVisible) ) 

#define IDaSvrPropertySheet_SetVisible(This,bVisible)	\
    ( (This)->lpVtbl -> SetVisible(This,bVisible) ) 

#define IDaSvrPropertySheet_GetPosition(This,plLeft,plTop)	\
    ( (This)->lpVtbl -> GetPosition(This,plLeft,plTop) ) 

#define IDaSvrPropertySheet_GetSize(This,plWidth,plHeight)	\
    ( (This)->lpVtbl -> GetSize(This,plWidth,plHeight) ) 

#define IDaSvrPropertySheet_GetPage(This,pbszPage)	\
    ( (This)->lpVtbl -> GetPage(This,pbszPage) ) 

#define IDaSvrPropertySheet_SetPage(This,bszPage)	\
    ( (This)->lpVtbl -> SetPage(This,bszPage) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrPropertySheet_INTERFACE_DEFINED__ */


#ifndef __IDaSvrBalloon_INTERFACE_DEFINED__
#define __IDaSvrBalloon_INTERFACE_DEFINED__

/* interface IDaSvrBalloon */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrBalloon;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E509-A208-11DE-ABF2-002421116FB2")
    IDaSvrBalloon : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetEnabled( 
            /* [out] */ long *pbEnabled) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetNumLines( 
            /* [out] */ long *plLines) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetNumCharsPerLine( 
            /* [out] */ long *plCharsPerLine) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFontName( 
            /* [out] */ BSTR *pbszFontName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFontSize( 
            /* [out] */ long *plFontSize) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFontBold( 
            /* [out] */ long *pbFontBold) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFontItalic( 
            /* [out] */ long *pbFontItalic) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFontStrikethru( 
            /* [out] */ long *pbFontStrikethru) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFontUnderline( 
            /* [out] */ long *pbFontUnderline) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetForeColor( 
            /* [out] */ long *plFGColor) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetBackColor( 
            /* [out] */ long *plBGColor) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetBorderColor( 
            /* [out] */ long *plBorderColor) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetVisible( 
            /* [in] */ long bVisible) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVisible( 
            /* [out] */ long *pbVisible) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetFontName( 
            /* [in] */ BSTR bszFontName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetFontSize( 
            /* [in] */ long lFontSize) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetFontCharSet( 
            /* [in] */ short sFontCharSet) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFontCharSet( 
            /* [out] */ short *psFontCharSet) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetStyle( 
            /* [in] */ long lStyle) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetStyle( 
            /* [out] */ long *plStyle) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetNumLines( 
            /* [in] */ long lLines) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetNumCharsPerLine( 
            /* [in] */ long lCharsPerLine) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrBalloonVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrBalloon * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrBalloon * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrBalloon * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrBalloon * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrBalloon * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrBalloon * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrBalloon * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetEnabled )( 
            IDaSvrBalloon * This,
            /* [out] */ long *pbEnabled);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetNumLines )( 
            IDaSvrBalloon * This,
            /* [out] */ long *plLines);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetNumCharsPerLine )( 
            IDaSvrBalloon * This,
            /* [out] */ long *plCharsPerLine);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFontName )( 
            IDaSvrBalloon * This,
            /* [out] */ BSTR *pbszFontName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFontSize )( 
            IDaSvrBalloon * This,
            /* [out] */ long *plFontSize);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFontBold )( 
            IDaSvrBalloon * This,
            /* [out] */ long *pbFontBold);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFontItalic )( 
            IDaSvrBalloon * This,
            /* [out] */ long *pbFontItalic);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFontStrikethru )( 
            IDaSvrBalloon * This,
            /* [out] */ long *pbFontStrikethru);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFontUnderline )( 
            IDaSvrBalloon * This,
            /* [out] */ long *pbFontUnderline);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetForeColor )( 
            IDaSvrBalloon * This,
            /* [out] */ long *plFGColor);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetBackColor )( 
            IDaSvrBalloon * This,
            /* [out] */ long *plBGColor);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetBorderColor )( 
            IDaSvrBalloon * This,
            /* [out] */ long *plBorderColor);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetVisible )( 
            IDaSvrBalloon * This,
            /* [in] */ long bVisible);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVisible )( 
            IDaSvrBalloon * This,
            /* [out] */ long *pbVisible);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetFontName )( 
            IDaSvrBalloon * This,
            /* [in] */ BSTR bszFontName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetFontSize )( 
            IDaSvrBalloon * This,
            /* [in] */ long lFontSize);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetFontCharSet )( 
            IDaSvrBalloon * This,
            /* [in] */ short sFontCharSet);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFontCharSet )( 
            IDaSvrBalloon * This,
            /* [out] */ short *psFontCharSet);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetStyle )( 
            IDaSvrBalloon * This,
            /* [in] */ long lStyle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetStyle )( 
            IDaSvrBalloon * This,
            /* [out] */ long *plStyle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetNumLines )( 
            IDaSvrBalloon * This,
            /* [in] */ long lLines);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetNumCharsPerLine )( 
            IDaSvrBalloon * This,
            /* [in] */ long lCharsPerLine);
        
        END_INTERFACE
    } IDaSvrBalloonVtbl;

    interface IDaSvrBalloon
    {
        CONST_VTBL struct IDaSvrBalloonVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrBalloon_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrBalloon_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrBalloon_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrBalloon_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrBalloon_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrBalloon_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrBalloon_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrBalloon_GetEnabled(This,pbEnabled)	\
    ( (This)->lpVtbl -> GetEnabled(This,pbEnabled) ) 

#define IDaSvrBalloon_GetNumLines(This,plLines)	\
    ( (This)->lpVtbl -> GetNumLines(This,plLines) ) 

#define IDaSvrBalloon_GetNumCharsPerLine(This,plCharsPerLine)	\
    ( (This)->lpVtbl -> GetNumCharsPerLine(This,plCharsPerLine) ) 

#define IDaSvrBalloon_GetFontName(This,pbszFontName)	\
    ( (This)->lpVtbl -> GetFontName(This,pbszFontName) ) 

#define IDaSvrBalloon_GetFontSize(This,plFontSize)	\
    ( (This)->lpVtbl -> GetFontSize(This,plFontSize) ) 

#define IDaSvrBalloon_GetFontBold(This,pbFontBold)	\
    ( (This)->lpVtbl -> GetFontBold(This,pbFontBold) ) 

#define IDaSvrBalloon_GetFontItalic(This,pbFontItalic)	\
    ( (This)->lpVtbl -> GetFontItalic(This,pbFontItalic) ) 

#define IDaSvrBalloon_GetFontStrikethru(This,pbFontStrikethru)	\
    ( (This)->lpVtbl -> GetFontStrikethru(This,pbFontStrikethru) ) 

#define IDaSvrBalloon_GetFontUnderline(This,pbFontUnderline)	\
    ( (This)->lpVtbl -> GetFontUnderline(This,pbFontUnderline) ) 

#define IDaSvrBalloon_GetForeColor(This,plFGColor)	\
    ( (This)->lpVtbl -> GetForeColor(This,plFGColor) ) 

#define IDaSvrBalloon_GetBackColor(This,plBGColor)	\
    ( (This)->lpVtbl -> GetBackColor(This,plBGColor) ) 

#define IDaSvrBalloon_GetBorderColor(This,plBorderColor)	\
    ( (This)->lpVtbl -> GetBorderColor(This,plBorderColor) ) 

#define IDaSvrBalloon_SetVisible(This,bVisible)	\
    ( (This)->lpVtbl -> SetVisible(This,bVisible) ) 

#define IDaSvrBalloon_GetVisible(This,pbVisible)	\
    ( (This)->lpVtbl -> GetVisible(This,pbVisible) ) 

#define IDaSvrBalloon_SetFontName(This,bszFontName)	\
    ( (This)->lpVtbl -> SetFontName(This,bszFontName) ) 

#define IDaSvrBalloon_SetFontSize(This,lFontSize)	\
    ( (This)->lpVtbl -> SetFontSize(This,lFontSize) ) 

#define IDaSvrBalloon_SetFontCharSet(This,sFontCharSet)	\
    ( (This)->lpVtbl -> SetFontCharSet(This,sFontCharSet) ) 

#define IDaSvrBalloon_GetFontCharSet(This,psFontCharSet)	\
    ( (This)->lpVtbl -> GetFontCharSet(This,psFontCharSet) ) 

#define IDaSvrBalloon_SetStyle(This,lStyle)	\
    ( (This)->lpVtbl -> SetStyle(This,lStyle) ) 

#define IDaSvrBalloon_GetStyle(This,plStyle)	\
    ( (This)->lpVtbl -> GetStyle(This,plStyle) ) 

#define IDaSvrBalloon_SetNumLines(This,lLines)	\
    ( (This)->lpVtbl -> SetNumLines(This,lLines) ) 

#define IDaSvrBalloon_SetNumCharsPerLine(This,lCharsPerLine)	\
    ( (This)->lpVtbl -> SetNumCharsPerLine(This,lCharsPerLine) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrBalloon_INTERFACE_DEFINED__ */


#ifndef __IDaSvrCharacter_INTERFACE_DEFINED__
#define __IDaSvrCharacter_INTERFACE_DEFINED__

/* interface IDaSvrCharacter */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrCharacter;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E504-A208-11DE-ABF2-002421116FB2")
    IDaSvrCharacter : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVisible( 
            /* [out] */ long *pbVisible) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetPosition( 
            /* [in] */ long lLeft,
            /* [in] */ long lTop) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetPosition( 
            /* [out] */ long *plLeft,
            /* [out] */ long *plTop) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetSize( 
            /* [in] */ long lWidth,
            /* [in] */ long lHeight) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetSize( 
            /* [out] */ long *plWidth,
            /* [out] */ long *plHeight) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetName( 
            /* [out] */ BSTR *pbszName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetDescription( 
            /* [out] */ BSTR *pbszDescription) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetTTSSpeed( 
            /* [out] */ long *pdwSpeed) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetTTSPitch( 
            /* [out] */ short *pwPitch) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Activate( 
            /* [in] */ short sState) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetIdleOn( 
            /* [in] */ long bOn) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetIdleOn( 
            /* [out] */ long *pbOn) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Prepare( 
            /* [in] */ long dwType,
            /* [in] */ BSTR bszName,
            /* [in] */ long bQueue,
            /* [out] */ long *pdwReqID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Play( 
            /* [in] */ BSTR bszAnimation,
            /* [out] */ long *pdwReqID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Stop( 
            /* [in] */ long dwReqID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE StopAll( 
            /* [in] */ long lTypes) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Wait( 
            /* [in] */ long dwReqID,
            /* [out] */ long *pdwReqID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Interrupt( 
            /* [in] */ long dwReqID,
            /* [out] */ long *pdwReqID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Show( 
            /* [in] */ long bFast,
            /* [out] */ long *pdwReqID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Hide( 
            /* [in] */ long bFast,
            /* [out] */ long *pdwReqID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Speak( 
            /* [in] */ BSTR bszText,
            /* [in] */ BSTR bszUrl,
            /* [out] */ long *pdwReqID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE MoveTo( 
            /* [in] */ short x,
            /* [in] */ short y,
            /* [in] */ long lSpeed,
            /* [out] */ long *pdwReqID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GestureAt( 
            /* [in] */ short x,
            /* [in] */ short y,
            /* [out] */ long *pdwReqID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetMoveCause( 
            /* [out] */ long *pdwCause) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVisibilityCause( 
            /* [out] */ long *pdwCause) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE HasOtherClients( 
            /* [out] */ long *plNumOtherClients) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetSoundEffectsOn( 
            /* [in] */ long bOn) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetSoundEffectsOn( 
            /* [out] */ long *pbOn) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetName( 
            /* [in] */ BSTR bszName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetDescription( 
            /* [in] */ BSTR bszDescription) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetExtraData( 
            /* [out] */ BSTR *pbszExtraData) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ShowPopupMenu( 
            /* [in] */ short x,
            /* [in] */ short y) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetAutoPopupMenu( 
            /* [in] */ long bAutoPopupMenu) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetAutoPopupMenu( 
            /* [out] */ long *pbAutoPopupMenu) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE GetHelpFileName( 
            /* [out] */ BSTR *pbszName) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE SetHelpFileName( 
            /* [in] */ BSTR bszName) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE SetHelpModeOn( 
            /* [in] */ long bHelpModeOn) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE GetHelpModeOn( 
            /* [out] */ long *pbHelpModeOn) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE SetHelpContextID( 
            /* [in] */ long ulID) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE GetHelpContextID( 
            /* [out] */ long *pulID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetActive( 
            /* [out] */ short *psState) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Listen( 
            /* [in] */ long bListen) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetLanguageID( 
            /* [in] */ long langid) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetLanguageID( 
            /* [out] */ long *plangid) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetTTSModeID( 
            /* [out] */ BSTR *pbszModeID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetTTSModeID( 
            /* [in] */ BSTR bszModeID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetSRModeID( 
            /* [out] */ BSTR *pbszModeID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetSRModeID( 
            /* [in] */ BSTR bszModeID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetGUID( 
            /* [out] */ BSTR *pbszID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetOriginalSize( 
            /* [out] */ long *plWidth,
            /* [out] */ long *plHeight) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Think( 
            /* [in] */ BSTR bszText,
            /* [out] */ long *pdwReqID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVersion( 
            /* [out] */ short *psMajor,
            /* [out] */ short *psMinor) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetAnimationNames( 
            /* [out] */ IUnknown **punkEnum) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetSRStatus( 
            /* [out] */ long *plStatus) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrCharacterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrCharacter * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrCharacter * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrCharacter * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrCharacter * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrCharacter * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrCharacter * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrCharacter * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVisible )( 
            IDaSvrCharacter * This,
            /* [out] */ long *pbVisible);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetPosition )( 
            IDaSvrCharacter * This,
            /* [in] */ long lLeft,
            /* [in] */ long lTop);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetPosition )( 
            IDaSvrCharacter * This,
            /* [out] */ long *plLeft,
            /* [out] */ long *plTop);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetSize )( 
            IDaSvrCharacter * This,
            /* [in] */ long lWidth,
            /* [in] */ long lHeight);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetSize )( 
            IDaSvrCharacter * This,
            /* [out] */ long *plWidth,
            /* [out] */ long *plHeight);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetName )( 
            IDaSvrCharacter * This,
            /* [out] */ BSTR *pbszName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetDescription )( 
            IDaSvrCharacter * This,
            /* [out] */ BSTR *pbszDescription);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetTTSSpeed )( 
            IDaSvrCharacter * This,
            /* [out] */ long *pdwSpeed);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetTTSPitch )( 
            IDaSvrCharacter * This,
            /* [out] */ short *pwPitch);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Activate )( 
            IDaSvrCharacter * This,
            /* [in] */ short sState);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetIdleOn )( 
            IDaSvrCharacter * This,
            /* [in] */ long bOn);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetIdleOn )( 
            IDaSvrCharacter * This,
            /* [out] */ long *pbOn);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Prepare )( 
            IDaSvrCharacter * This,
            /* [in] */ long dwType,
            /* [in] */ BSTR bszName,
            /* [in] */ long bQueue,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Play )( 
            IDaSvrCharacter * This,
            /* [in] */ BSTR bszAnimation,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IDaSvrCharacter * This,
            /* [in] */ long dwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *StopAll )( 
            IDaSvrCharacter * This,
            /* [in] */ long lTypes);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Wait )( 
            IDaSvrCharacter * This,
            /* [in] */ long dwReqID,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Interrupt )( 
            IDaSvrCharacter * This,
            /* [in] */ long dwReqID,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Show )( 
            IDaSvrCharacter * This,
            /* [in] */ long bFast,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Hide )( 
            IDaSvrCharacter * This,
            /* [in] */ long bFast,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Speak )( 
            IDaSvrCharacter * This,
            /* [in] */ BSTR bszText,
            /* [in] */ BSTR bszUrl,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *MoveTo )( 
            IDaSvrCharacter * This,
            /* [in] */ short x,
            /* [in] */ short y,
            /* [in] */ long lSpeed,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GestureAt )( 
            IDaSvrCharacter * This,
            /* [in] */ short x,
            /* [in] */ short y,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetMoveCause )( 
            IDaSvrCharacter * This,
            /* [out] */ long *pdwCause);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVisibilityCause )( 
            IDaSvrCharacter * This,
            /* [out] */ long *pdwCause);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *HasOtherClients )( 
            IDaSvrCharacter * This,
            /* [out] */ long *plNumOtherClients);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetSoundEffectsOn )( 
            IDaSvrCharacter * This,
            /* [in] */ long bOn);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetSoundEffectsOn )( 
            IDaSvrCharacter * This,
            /* [out] */ long *pbOn);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetName )( 
            IDaSvrCharacter * This,
            /* [in] */ BSTR bszName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetDescription )( 
            IDaSvrCharacter * This,
            /* [in] */ BSTR bszDescription);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetExtraData )( 
            IDaSvrCharacter * This,
            /* [out] */ BSTR *pbszExtraData);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ShowPopupMenu )( 
            IDaSvrCharacter * This,
            /* [in] */ short x,
            /* [in] */ short y);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetAutoPopupMenu )( 
            IDaSvrCharacter * This,
            /* [in] */ long bAutoPopupMenu);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetAutoPopupMenu )( 
            IDaSvrCharacter * This,
            /* [out] */ long *pbAutoPopupMenu);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetHelpFileName )( 
            IDaSvrCharacter * This,
            /* [out] */ BSTR *pbszName);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *SetHelpFileName )( 
            IDaSvrCharacter * This,
            /* [in] */ BSTR bszName);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *SetHelpModeOn )( 
            IDaSvrCharacter * This,
            /* [in] */ long bHelpModeOn);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetHelpModeOn )( 
            IDaSvrCharacter * This,
            /* [out] */ long *pbHelpModeOn);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *SetHelpContextID )( 
            IDaSvrCharacter * This,
            /* [in] */ long ulID);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetHelpContextID )( 
            IDaSvrCharacter * This,
            /* [out] */ long *pulID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetActive )( 
            IDaSvrCharacter * This,
            /* [out] */ short *psState);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Listen )( 
            IDaSvrCharacter * This,
            /* [in] */ long bListen);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetLanguageID )( 
            IDaSvrCharacter * This,
            /* [in] */ long langid);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetLanguageID )( 
            IDaSvrCharacter * This,
            /* [out] */ long *plangid);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetTTSModeID )( 
            IDaSvrCharacter * This,
            /* [out] */ BSTR *pbszModeID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetTTSModeID )( 
            IDaSvrCharacter * This,
            /* [in] */ BSTR bszModeID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetSRModeID )( 
            IDaSvrCharacter * This,
            /* [out] */ BSTR *pbszModeID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetSRModeID )( 
            IDaSvrCharacter * This,
            /* [in] */ BSTR bszModeID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetGUID )( 
            IDaSvrCharacter * This,
            /* [out] */ BSTR *pbszID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetOriginalSize )( 
            IDaSvrCharacter * This,
            /* [out] */ long *plWidth,
            /* [out] */ long *plHeight);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Think )( 
            IDaSvrCharacter * This,
            /* [in] */ BSTR bszText,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVersion )( 
            IDaSvrCharacter * This,
            /* [out] */ short *psMajor,
            /* [out] */ short *psMinor);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetAnimationNames )( 
            IDaSvrCharacter * This,
            /* [out] */ IUnknown **punkEnum);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetSRStatus )( 
            IDaSvrCharacter * This,
            /* [out] */ long *plStatus);
        
        END_INTERFACE
    } IDaSvrCharacterVtbl;

    interface IDaSvrCharacter
    {
        CONST_VTBL struct IDaSvrCharacterVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrCharacter_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrCharacter_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrCharacter_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrCharacter_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrCharacter_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrCharacter_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrCharacter_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrCharacter_GetVisible(This,pbVisible)	\
    ( (This)->lpVtbl -> GetVisible(This,pbVisible) ) 

#define IDaSvrCharacter_SetPosition(This,lLeft,lTop)	\
    ( (This)->lpVtbl -> SetPosition(This,lLeft,lTop) ) 

#define IDaSvrCharacter_GetPosition(This,plLeft,plTop)	\
    ( (This)->lpVtbl -> GetPosition(This,plLeft,plTop) ) 

#define IDaSvrCharacter_SetSize(This,lWidth,lHeight)	\
    ( (This)->lpVtbl -> SetSize(This,lWidth,lHeight) ) 

#define IDaSvrCharacter_GetSize(This,plWidth,plHeight)	\
    ( (This)->lpVtbl -> GetSize(This,plWidth,plHeight) ) 

#define IDaSvrCharacter_GetName(This,pbszName)	\
    ( (This)->lpVtbl -> GetName(This,pbszName) ) 

#define IDaSvrCharacter_GetDescription(This,pbszDescription)	\
    ( (This)->lpVtbl -> GetDescription(This,pbszDescription) ) 

#define IDaSvrCharacter_GetTTSSpeed(This,pdwSpeed)	\
    ( (This)->lpVtbl -> GetTTSSpeed(This,pdwSpeed) ) 

#define IDaSvrCharacter_GetTTSPitch(This,pwPitch)	\
    ( (This)->lpVtbl -> GetTTSPitch(This,pwPitch) ) 

#define IDaSvrCharacter_Activate(This,sState)	\
    ( (This)->lpVtbl -> Activate(This,sState) ) 

#define IDaSvrCharacter_SetIdleOn(This,bOn)	\
    ( (This)->lpVtbl -> SetIdleOn(This,bOn) ) 

#define IDaSvrCharacter_GetIdleOn(This,pbOn)	\
    ( (This)->lpVtbl -> GetIdleOn(This,pbOn) ) 

#define IDaSvrCharacter_Prepare(This,dwType,bszName,bQueue,pdwReqID)	\
    ( (This)->lpVtbl -> Prepare(This,dwType,bszName,bQueue,pdwReqID) ) 

#define IDaSvrCharacter_Play(This,bszAnimation,pdwReqID)	\
    ( (This)->lpVtbl -> Play(This,bszAnimation,pdwReqID) ) 

#define IDaSvrCharacter_Stop(This,dwReqID)	\
    ( (This)->lpVtbl -> Stop(This,dwReqID) ) 

#define IDaSvrCharacter_StopAll(This,lTypes)	\
    ( (This)->lpVtbl -> StopAll(This,lTypes) ) 

#define IDaSvrCharacter_Wait(This,dwReqID,pdwReqID)	\
    ( (This)->lpVtbl -> Wait(This,dwReqID,pdwReqID) ) 

#define IDaSvrCharacter_Interrupt(This,dwReqID,pdwReqID)	\
    ( (This)->lpVtbl -> Interrupt(This,dwReqID,pdwReqID) ) 

#define IDaSvrCharacter_Show(This,bFast,pdwReqID)	\
    ( (This)->lpVtbl -> Show(This,bFast,pdwReqID) ) 

#define IDaSvrCharacter_Hide(This,bFast,pdwReqID)	\
    ( (This)->lpVtbl -> Hide(This,bFast,pdwReqID) ) 

#define IDaSvrCharacter_Speak(This,bszText,bszUrl,pdwReqID)	\
    ( (This)->lpVtbl -> Speak(This,bszText,bszUrl,pdwReqID) ) 

#define IDaSvrCharacter_MoveTo(This,x,y,lSpeed,pdwReqID)	\
    ( (This)->lpVtbl -> MoveTo(This,x,y,lSpeed,pdwReqID) ) 

#define IDaSvrCharacter_GestureAt(This,x,y,pdwReqID)	\
    ( (This)->lpVtbl -> GestureAt(This,x,y,pdwReqID) ) 

#define IDaSvrCharacter_GetMoveCause(This,pdwCause)	\
    ( (This)->lpVtbl -> GetMoveCause(This,pdwCause) ) 

#define IDaSvrCharacter_GetVisibilityCause(This,pdwCause)	\
    ( (This)->lpVtbl -> GetVisibilityCause(This,pdwCause) ) 

#define IDaSvrCharacter_HasOtherClients(This,plNumOtherClients)	\
    ( (This)->lpVtbl -> HasOtherClients(This,plNumOtherClients) ) 

#define IDaSvrCharacter_SetSoundEffectsOn(This,bOn)	\
    ( (This)->lpVtbl -> SetSoundEffectsOn(This,bOn) ) 

#define IDaSvrCharacter_GetSoundEffectsOn(This,pbOn)	\
    ( (This)->lpVtbl -> GetSoundEffectsOn(This,pbOn) ) 

#define IDaSvrCharacter_SetName(This,bszName)	\
    ( (This)->lpVtbl -> SetName(This,bszName) ) 

#define IDaSvrCharacter_SetDescription(This,bszDescription)	\
    ( (This)->lpVtbl -> SetDescription(This,bszDescription) ) 

#define IDaSvrCharacter_GetExtraData(This,pbszExtraData)	\
    ( (This)->lpVtbl -> GetExtraData(This,pbszExtraData) ) 

#define IDaSvrCharacter_ShowPopupMenu(This,x,y)	\
    ( (This)->lpVtbl -> ShowPopupMenu(This,x,y) ) 

#define IDaSvrCharacter_SetAutoPopupMenu(This,bAutoPopupMenu)	\
    ( (This)->lpVtbl -> SetAutoPopupMenu(This,bAutoPopupMenu) ) 

#define IDaSvrCharacter_GetAutoPopupMenu(This,pbAutoPopupMenu)	\
    ( (This)->lpVtbl -> GetAutoPopupMenu(This,pbAutoPopupMenu) ) 

#define IDaSvrCharacter_GetHelpFileName(This,pbszName)	\
    ( (This)->lpVtbl -> GetHelpFileName(This,pbszName) ) 

#define IDaSvrCharacter_SetHelpFileName(This,bszName)	\
    ( (This)->lpVtbl -> SetHelpFileName(This,bszName) ) 

#define IDaSvrCharacter_SetHelpModeOn(This,bHelpModeOn)	\
    ( (This)->lpVtbl -> SetHelpModeOn(This,bHelpModeOn) ) 

#define IDaSvrCharacter_GetHelpModeOn(This,pbHelpModeOn)	\
    ( (This)->lpVtbl -> GetHelpModeOn(This,pbHelpModeOn) ) 

#define IDaSvrCharacter_SetHelpContextID(This,ulID)	\
    ( (This)->lpVtbl -> SetHelpContextID(This,ulID) ) 

#define IDaSvrCharacter_GetHelpContextID(This,pulID)	\
    ( (This)->lpVtbl -> GetHelpContextID(This,pulID) ) 

#define IDaSvrCharacter_GetActive(This,psState)	\
    ( (This)->lpVtbl -> GetActive(This,psState) ) 

#define IDaSvrCharacter_Listen(This,bListen)	\
    ( (This)->lpVtbl -> Listen(This,bListen) ) 

#define IDaSvrCharacter_SetLanguageID(This,langid)	\
    ( (This)->lpVtbl -> SetLanguageID(This,langid) ) 

#define IDaSvrCharacter_GetLanguageID(This,plangid)	\
    ( (This)->lpVtbl -> GetLanguageID(This,plangid) ) 

#define IDaSvrCharacter_GetTTSModeID(This,pbszModeID)	\
    ( (This)->lpVtbl -> GetTTSModeID(This,pbszModeID) ) 

#define IDaSvrCharacter_SetTTSModeID(This,bszModeID)	\
    ( (This)->lpVtbl -> SetTTSModeID(This,bszModeID) ) 

#define IDaSvrCharacter_GetSRModeID(This,pbszModeID)	\
    ( (This)->lpVtbl -> GetSRModeID(This,pbszModeID) ) 

#define IDaSvrCharacter_SetSRModeID(This,bszModeID)	\
    ( (This)->lpVtbl -> SetSRModeID(This,bszModeID) ) 

#define IDaSvrCharacter_GetGUID(This,pbszID)	\
    ( (This)->lpVtbl -> GetGUID(This,pbszID) ) 

#define IDaSvrCharacter_GetOriginalSize(This,plWidth,plHeight)	\
    ( (This)->lpVtbl -> GetOriginalSize(This,plWidth,plHeight) ) 

#define IDaSvrCharacter_Think(This,bszText,pdwReqID)	\
    ( (This)->lpVtbl -> Think(This,bszText,pdwReqID) ) 

#define IDaSvrCharacter_GetVersion(This,psMajor,psMinor)	\
    ( (This)->lpVtbl -> GetVersion(This,psMajor,psMinor) ) 

#define IDaSvrCharacter_GetAnimationNames(This,punkEnum)	\
    ( (This)->lpVtbl -> GetAnimationNames(This,punkEnum) ) 

#define IDaSvrCharacter_GetSRStatus(This,plStatus)	\
    ( (This)->lpVtbl -> GetSRStatus(This,plStatus) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrCharacter_INTERFACE_DEFINED__ */


#ifndef __IDaSvrCommandsWindow_INTERFACE_DEFINED__
#define __IDaSvrCommandsWindow_INTERFACE_DEFINED__

/* interface IDaSvrCommandsWindow */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrCommandsWindow;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E507-A208-11DE-ABF2-002421116FB2")
    IDaSvrCommandsWindow : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetVisible( 
            /* [in] */ long bVisible) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVisible( 
            /* [out] */ long *pbVisible) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetPosition( 
            /* [out] */ long *plLeft,
            /* [out] */ long *plTop) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetSize( 
            /* [out] */ long *plWidth,
            /* [out] */ long *plHeight) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrCommandsWindowVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrCommandsWindow * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrCommandsWindow * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrCommandsWindow * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrCommandsWindow * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrCommandsWindow * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrCommandsWindow * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrCommandsWindow * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetVisible )( 
            IDaSvrCommandsWindow * This,
            /* [in] */ long bVisible);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVisible )( 
            IDaSvrCommandsWindow * This,
            /* [out] */ long *pbVisible);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetPosition )( 
            IDaSvrCommandsWindow * This,
            /* [out] */ long *plLeft,
            /* [out] */ long *plTop);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetSize )( 
            IDaSvrCommandsWindow * This,
            /* [out] */ long *plWidth,
            /* [out] */ long *plHeight);
        
        END_INTERFACE
    } IDaSvrCommandsWindowVtbl;

    interface IDaSvrCommandsWindow
    {
        CONST_VTBL struct IDaSvrCommandsWindowVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrCommandsWindow_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrCommandsWindow_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrCommandsWindow_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrCommandsWindow_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrCommandsWindow_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrCommandsWindow_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrCommandsWindow_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrCommandsWindow_SetVisible(This,bVisible)	\
    ( (This)->lpVtbl -> SetVisible(This,bVisible) ) 

#define IDaSvrCommandsWindow_GetVisible(This,pbVisible)	\
    ( (This)->lpVtbl -> GetVisible(This,pbVisible) ) 

#define IDaSvrCommandsWindow_GetPosition(This,plLeft,plTop)	\
    ( (This)->lpVtbl -> GetPosition(This,plLeft,plTop) ) 

#define IDaSvrCommandsWindow_GetSize(This,plWidth,plHeight)	\
    ( (This)->lpVtbl -> GetSize(This,plWidth,plHeight) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrCommandsWindow_INTERFACE_DEFINED__ */


#ifndef __IDaSvrNotifySink15_INTERFACE_DEFINED__
#define __IDaSvrNotifySink15_INTERFACE_DEFINED__

/* interface IDaSvrNotifySink15 */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrNotifySink15;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E503-A208-11DE-ABF2-002421116FB2")
    IDaSvrNotifySink15 : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Command( 
            /* [in] */ long dwCommandID,
            /* [in] */ IUnknown *punkUserInput) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ActivateInputState( 
            /* [in] */ long dwCharID,
            /* [in] */ long bActivated) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE Restart( void) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE Shutdown( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE VisibleState( 
            /* [in] */ long dwCharID,
            /* [in] */ long bVisible,
            /* [in] */ long dwCause) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Click( 
            /* [in] */ long dwCharID,
            /* [in] */ short fwKeys,
            /* [in] */ long x,
            /* [in] */ long y) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE DblClick( 
            /* [in] */ long dwCharID,
            /* [in] */ short fwKeys,
            /* [in] */ long x,
            /* [in] */ long y) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE DragStart( 
            /* [in] */ long dwCharID,
            /* [in] */ short fwKeys,
            /* [in] */ long x,
            /* [in] */ long y) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE DragComplete( 
            /* [in] */ long dwCharID,
            /* [in] */ short fwKeys,
            /* [in] */ long x,
            /* [in] */ long y) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RequestStart( 
            /* [in] */ long dwRequestID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RequestComplete( 
            /* [in] */ long dwRequestID,
            /* [in] */ long hrStatus) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE BookMark( 
            /* [in] */ long dwBookMarkID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Idle( 
            /* [in] */ long dwCharID,
            /* [in] */ long bStart) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Move( 
            /* [in] */ long dwCharID,
            /* [in] */ long x,
            /* [in] */ long y,
            /* [in] */ long dwCause) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Size( 
            /* [in] */ long dwCharID,
            /* [in] */ long lWidth,
            /* [in] */ long lHeight) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE BalloonVisibleState( 
            /* [in] */ long dwCharID,
            /* [in] */ long bVisible) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrNotifySink15Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrNotifySink15 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrNotifySink15 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrNotifySink15 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Command )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwCommandID,
            /* [in] */ IUnknown *punkUserInput);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ActivateInputState )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwCharID,
            /* [in] */ long bActivated);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *Restart )( 
            IDaSvrNotifySink15 * This);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *Shutdown )( 
            IDaSvrNotifySink15 * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *VisibleState )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwCharID,
            /* [in] */ long bVisible,
            /* [in] */ long dwCause);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Click )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwCharID,
            /* [in] */ short fwKeys,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DblClick )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwCharID,
            /* [in] */ short fwKeys,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DragStart )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwCharID,
            /* [in] */ short fwKeys,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DragComplete )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwCharID,
            /* [in] */ short fwKeys,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RequestStart )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwRequestID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RequestComplete )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwRequestID,
            /* [in] */ long hrStatus);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *BookMark )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwBookMarkID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Idle )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwCharID,
            /* [in] */ long bStart);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Move )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwCharID,
            /* [in] */ long x,
            /* [in] */ long y,
            /* [in] */ long dwCause);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Size )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwCharID,
            /* [in] */ long lWidth,
            /* [in] */ long lHeight);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *BalloonVisibleState )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwCharID,
            /* [in] */ long bVisible);
        
        END_INTERFACE
    } IDaSvrNotifySink15Vtbl;

    interface IDaSvrNotifySink15
    {
        CONST_VTBL struct IDaSvrNotifySink15Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrNotifySink15_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrNotifySink15_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrNotifySink15_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrNotifySink15_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrNotifySink15_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrNotifySink15_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrNotifySink15_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrNotifySink15_Command(This,dwCommandID,punkUserInput)	\
    ( (This)->lpVtbl -> Command(This,dwCommandID,punkUserInput) ) 

#define IDaSvrNotifySink15_ActivateInputState(This,dwCharID,bActivated)	\
    ( (This)->lpVtbl -> ActivateInputState(This,dwCharID,bActivated) ) 

#define IDaSvrNotifySink15_Restart(This)	\
    ( (This)->lpVtbl -> Restart(This) ) 

#define IDaSvrNotifySink15_Shutdown(This)	\
    ( (This)->lpVtbl -> Shutdown(This) ) 

#define IDaSvrNotifySink15_VisibleState(This,dwCharID,bVisible,dwCause)	\
    ( (This)->lpVtbl -> VisibleState(This,dwCharID,bVisible,dwCause) ) 

#define IDaSvrNotifySink15_Click(This,dwCharID,fwKeys,x,y)	\
    ( (This)->lpVtbl -> Click(This,dwCharID,fwKeys,x,y) ) 

#define IDaSvrNotifySink15_DblClick(This,dwCharID,fwKeys,x,y)	\
    ( (This)->lpVtbl -> DblClick(This,dwCharID,fwKeys,x,y) ) 

#define IDaSvrNotifySink15_DragStart(This,dwCharID,fwKeys,x,y)	\
    ( (This)->lpVtbl -> DragStart(This,dwCharID,fwKeys,x,y) ) 

#define IDaSvrNotifySink15_DragComplete(This,dwCharID,fwKeys,x,y)	\
    ( (This)->lpVtbl -> DragComplete(This,dwCharID,fwKeys,x,y) ) 

#define IDaSvrNotifySink15_RequestStart(This,dwRequestID)	\
    ( (This)->lpVtbl -> RequestStart(This,dwRequestID) ) 

#define IDaSvrNotifySink15_RequestComplete(This,dwRequestID,hrStatus)	\
    ( (This)->lpVtbl -> RequestComplete(This,dwRequestID,hrStatus) ) 

#define IDaSvrNotifySink15_BookMark(This,dwBookMarkID)	\
    ( (This)->lpVtbl -> BookMark(This,dwBookMarkID) ) 

#define IDaSvrNotifySink15_Idle(This,dwCharID,bStart)	\
    ( (This)->lpVtbl -> Idle(This,dwCharID,bStart) ) 

#define IDaSvrNotifySink15_Move(This,dwCharID,x,y,dwCause)	\
    ( (This)->lpVtbl -> Move(This,dwCharID,x,y,dwCause) ) 

#define IDaSvrNotifySink15_Size(This,dwCharID,lWidth,lHeight)	\
    ( (This)->lpVtbl -> Size(This,dwCharID,lWidth,lHeight) ) 

#define IDaSvrNotifySink15_BalloonVisibleState(This,dwCharID,bVisible)	\
    ( (This)->lpVtbl -> BalloonVisibleState(This,dwCharID,bVisible) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrNotifySink15_INTERFACE_DEFINED__ */


#ifndef __IDaSvrNotifySink_INTERFACE_DEFINED__
#define __IDaSvrNotifySink_INTERFACE_DEFINED__

/* interface IDaSvrNotifySink */
/* [object][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrNotifySink;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E516-A208-11DE-ABF2-002421116FB2")
    IDaSvrNotifySink : public IDaSvrNotifySink15
    {
    public:
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE HelpComplete( 
            /* [in] */ long dwCharID,
            /* [in] */ long dwCommandID,
            /* [in] */ long dwCause) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ListeningState( 
            /* [in] */ long dwCharID,
            /* [in] */ long bListening,
            /* [in] */ long dwCause) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE DefaultCharacterChange( 
            /* [in] */ BSTR bszGUID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AgentPropertyChange( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ActiveClientChange( 
            /* [in] */ long dwCharID,
            /* [in] */ long lStatus) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrNotifySinkVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrNotifySink * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrNotifySink * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrNotifySink * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrNotifySink * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrNotifySink * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrNotifySink * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrNotifySink * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Command )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCommandID,
            /* [in] */ IUnknown *punkUserInput);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ActivateInputState )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCharID,
            /* [in] */ long bActivated);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *Restart )( 
            IDaSvrNotifySink * This);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *Shutdown )( 
            IDaSvrNotifySink * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *VisibleState )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCharID,
            /* [in] */ long bVisible,
            /* [in] */ long dwCause);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Click )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCharID,
            /* [in] */ short fwKeys,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DblClick )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCharID,
            /* [in] */ short fwKeys,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DragStart )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCharID,
            /* [in] */ short fwKeys,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DragComplete )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCharID,
            /* [in] */ short fwKeys,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RequestStart )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwRequestID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RequestComplete )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwRequestID,
            /* [in] */ long hrStatus);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *BookMark )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwBookMarkID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Idle )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCharID,
            /* [in] */ long bStart);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Move )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCharID,
            /* [in] */ long x,
            /* [in] */ long y,
            /* [in] */ long dwCause);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Size )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCharID,
            /* [in] */ long lWidth,
            /* [in] */ long lHeight);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *BalloonVisibleState )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCharID,
            /* [in] */ long bVisible);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *HelpComplete )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCharID,
            /* [in] */ long dwCommandID,
            /* [in] */ long dwCause);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ListeningState )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCharID,
            /* [in] */ long bListening,
            /* [in] */ long dwCause);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DefaultCharacterChange )( 
            IDaSvrNotifySink * This,
            /* [in] */ BSTR bszGUID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AgentPropertyChange )( 
            IDaSvrNotifySink * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ActiveClientChange )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwCharID,
            /* [in] */ long lStatus);
        
        END_INTERFACE
    } IDaSvrNotifySinkVtbl;

    interface IDaSvrNotifySink
    {
        CONST_VTBL struct IDaSvrNotifySinkVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrNotifySink_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrNotifySink_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrNotifySink_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrNotifySink_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrNotifySink_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrNotifySink_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrNotifySink_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrNotifySink_Command(This,dwCommandID,punkUserInput)	\
    ( (This)->lpVtbl -> Command(This,dwCommandID,punkUserInput) ) 

#define IDaSvrNotifySink_ActivateInputState(This,dwCharID,bActivated)	\
    ( (This)->lpVtbl -> ActivateInputState(This,dwCharID,bActivated) ) 

#define IDaSvrNotifySink_Restart(This)	\
    ( (This)->lpVtbl -> Restart(This) ) 

#define IDaSvrNotifySink_Shutdown(This)	\
    ( (This)->lpVtbl -> Shutdown(This) ) 

#define IDaSvrNotifySink_VisibleState(This,dwCharID,bVisible,dwCause)	\
    ( (This)->lpVtbl -> VisibleState(This,dwCharID,bVisible,dwCause) ) 

#define IDaSvrNotifySink_Click(This,dwCharID,fwKeys,x,y)	\
    ( (This)->lpVtbl -> Click(This,dwCharID,fwKeys,x,y) ) 

#define IDaSvrNotifySink_DblClick(This,dwCharID,fwKeys,x,y)	\
    ( (This)->lpVtbl -> DblClick(This,dwCharID,fwKeys,x,y) ) 

#define IDaSvrNotifySink_DragStart(This,dwCharID,fwKeys,x,y)	\
    ( (This)->lpVtbl -> DragStart(This,dwCharID,fwKeys,x,y) ) 

#define IDaSvrNotifySink_DragComplete(This,dwCharID,fwKeys,x,y)	\
    ( (This)->lpVtbl -> DragComplete(This,dwCharID,fwKeys,x,y) ) 

#define IDaSvrNotifySink_RequestStart(This,dwRequestID)	\
    ( (This)->lpVtbl -> RequestStart(This,dwRequestID) ) 

#define IDaSvrNotifySink_RequestComplete(This,dwRequestID,hrStatus)	\
    ( (This)->lpVtbl -> RequestComplete(This,dwRequestID,hrStatus) ) 

#define IDaSvrNotifySink_BookMark(This,dwBookMarkID)	\
    ( (This)->lpVtbl -> BookMark(This,dwBookMarkID) ) 

#define IDaSvrNotifySink_Idle(This,dwCharID,bStart)	\
    ( (This)->lpVtbl -> Idle(This,dwCharID,bStart) ) 

#define IDaSvrNotifySink_Move(This,dwCharID,x,y,dwCause)	\
    ( (This)->lpVtbl -> Move(This,dwCharID,x,y,dwCause) ) 

#define IDaSvrNotifySink_Size(This,dwCharID,lWidth,lHeight)	\
    ( (This)->lpVtbl -> Size(This,dwCharID,lWidth,lHeight) ) 

#define IDaSvrNotifySink_BalloonVisibleState(This,dwCharID,bVisible)	\
    ( (This)->lpVtbl -> BalloonVisibleState(This,dwCharID,bVisible) ) 


#define IDaSvrNotifySink_HelpComplete(This,dwCharID,dwCommandID,dwCause)	\
    ( (This)->lpVtbl -> HelpComplete(This,dwCharID,dwCommandID,dwCause) ) 

#define IDaSvrNotifySink_ListeningState(This,dwCharID,bListening,dwCause)	\
    ( (This)->lpVtbl -> ListeningState(This,dwCharID,bListening,dwCause) ) 

#define IDaSvrNotifySink_DefaultCharacterChange(This,bszGUID)	\
    ( (This)->lpVtbl -> DefaultCharacterChange(This,bszGUID) ) 

#define IDaSvrNotifySink_AgentPropertyChange(This)	\
    ( (This)->lpVtbl -> AgentPropertyChange(This) ) 

#define IDaSvrNotifySink_ActiveClientChange(This,dwCharID,lStatus)	\
    ( (This)->lpVtbl -> ActiveClientChange(This,dwCharID,lStatus) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrNotifySink_INTERFACE_DEFINED__ */


#ifndef __IDaSvrCharacterFiles_INTERFACE_DEFINED__
#define __IDaSvrCharacterFiles_INTERFACE_DEFINED__

/* interface IDaSvrCharacterFiles */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrCharacterFiles;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E523-A208-11DE-ABF2-002421116FB2")
    IDaSvrCharacterFiles : public IDispatch
    {
    public:
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_FilePaths( 
            /* [retval][out] */ SAFEARRAY * *FilePaths) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_SearchPath( 
            /* [retval][out] */ BSTR *SearchPath) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_SearchPath( 
            /* [in] */ BSTR SearchPath) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_DefaultSearchPath( 
            /* [retval][out] */ BSTR *DefaultSearchPath) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Filter( 
            /* [retval][out] */ long *Filter) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Filter( 
            /* [in] */ long Filter) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrCharacterFilesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrCharacterFiles * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrCharacterFiles * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrCharacterFiles * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrCharacterFiles * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrCharacterFiles * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrCharacterFiles * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrCharacterFiles * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_FilePaths )( 
            IDaSvrCharacterFiles * This,
            /* [retval][out] */ SAFEARRAY * *FilePaths);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_SearchPath )( 
            IDaSvrCharacterFiles * This,
            /* [retval][out] */ BSTR *SearchPath);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_SearchPath )( 
            IDaSvrCharacterFiles * This,
            /* [in] */ BSTR SearchPath);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_DefaultSearchPath )( 
            IDaSvrCharacterFiles * This,
            /* [retval][out] */ BSTR *DefaultSearchPath);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Filter )( 
            IDaSvrCharacterFiles * This,
            /* [retval][out] */ long *Filter);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Filter )( 
            IDaSvrCharacterFiles * This,
            /* [in] */ long Filter);
        
        END_INTERFACE
    } IDaSvrCharacterFilesVtbl;

    interface IDaSvrCharacterFiles
    {
        CONST_VTBL struct IDaSvrCharacterFilesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrCharacterFiles_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrCharacterFiles_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrCharacterFiles_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrCharacterFiles_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrCharacterFiles_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrCharacterFiles_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrCharacterFiles_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrCharacterFiles_get_FilePaths(This,FilePaths)	\
    ( (This)->lpVtbl -> get_FilePaths(This,FilePaths) ) 

#define IDaSvrCharacterFiles_get_SearchPath(This,SearchPath)	\
    ( (This)->lpVtbl -> get_SearchPath(This,SearchPath) ) 

#define IDaSvrCharacterFiles_put_SearchPath(This,SearchPath)	\
    ( (This)->lpVtbl -> put_SearchPath(This,SearchPath) ) 

#define IDaSvrCharacterFiles_get_DefaultSearchPath(This,DefaultSearchPath)	\
    ( (This)->lpVtbl -> get_DefaultSearchPath(This,DefaultSearchPath) ) 

#define IDaSvrCharacterFiles_get_Filter(This,Filter)	\
    ( (This)->lpVtbl -> get_Filter(This,Filter) ) 

#define IDaSvrCharacterFiles_put_Filter(This,Filter)	\
    ( (This)->lpVtbl -> put_Filter(This,Filter) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrCharacterFiles_INTERFACE_DEFINED__ */


#ifndef __IDaSvrSpeechEngine_INTERFACE_DEFINED__
#define __IDaSvrSpeechEngine_INTERFACE_DEFINED__

/* interface IDaSvrSpeechEngine */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrSpeechEngine;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E519-A208-11DE-ABF2-002421116FB2")
    IDaSvrSpeechEngine : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetTTSModeID( 
            /* [out] */ BSTR *TTSModeID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetDisplayName( 
            /* [out] */ BSTR *DisplayName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetManufacturer( 
            /* [out] */ BSTR *Manufacturer) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVersion( 
            /* [out] */ short *MajorVersion,
            /* [out] */ short *MinorVersion) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetGender( 
            /* [out] */ short *Gender) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetLanguageID( 
            /* [out] */ long *LanguageID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetLanguageName( 
            /* [out] */ BSTR *LanguageName,
            /* [defaultvalue][in] */ boolean EnglishName = TRUE) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrSpeechEngineVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrSpeechEngine * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrSpeechEngine * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrSpeechEngine * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrSpeechEngine * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrSpeechEngine * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrSpeechEngine * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrSpeechEngine * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetTTSModeID )( 
            IDaSvrSpeechEngine * This,
            /* [out] */ BSTR *TTSModeID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetDisplayName )( 
            IDaSvrSpeechEngine * This,
            /* [out] */ BSTR *DisplayName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetManufacturer )( 
            IDaSvrSpeechEngine * This,
            /* [out] */ BSTR *Manufacturer);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVersion )( 
            IDaSvrSpeechEngine * This,
            /* [out] */ short *MajorVersion,
            /* [out] */ short *MinorVersion);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetGender )( 
            IDaSvrSpeechEngine * This,
            /* [out] */ short *Gender);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetLanguageID )( 
            IDaSvrSpeechEngine * This,
            /* [out] */ long *LanguageID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetLanguageName )( 
            IDaSvrSpeechEngine * This,
            /* [out] */ BSTR *LanguageName,
            /* [defaultvalue][in] */ boolean EnglishName);
        
        END_INTERFACE
    } IDaSvrSpeechEngineVtbl;

    interface IDaSvrSpeechEngine
    {
        CONST_VTBL struct IDaSvrSpeechEngineVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrSpeechEngine_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrSpeechEngine_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrSpeechEngine_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrSpeechEngine_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrSpeechEngine_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrSpeechEngine_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrSpeechEngine_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrSpeechEngine_GetTTSModeID(This,TTSModeID)	\
    ( (This)->lpVtbl -> GetTTSModeID(This,TTSModeID) ) 

#define IDaSvrSpeechEngine_GetDisplayName(This,DisplayName)	\
    ( (This)->lpVtbl -> GetDisplayName(This,DisplayName) ) 

#define IDaSvrSpeechEngine_GetManufacturer(This,Manufacturer)	\
    ( (This)->lpVtbl -> GetManufacturer(This,Manufacturer) ) 

#define IDaSvrSpeechEngine_GetVersion(This,MajorVersion,MinorVersion)	\
    ( (This)->lpVtbl -> GetVersion(This,MajorVersion,MinorVersion) ) 

#define IDaSvrSpeechEngine_GetGender(This,Gender)	\
    ( (This)->lpVtbl -> GetGender(This,Gender) ) 

#define IDaSvrSpeechEngine_GetLanguageID(This,LanguageID)	\
    ( (This)->lpVtbl -> GetLanguageID(This,LanguageID) ) 

#define IDaSvrSpeechEngine_GetLanguageName(This,LanguageName,EnglishName)	\
    ( (This)->lpVtbl -> GetLanguageName(This,LanguageName,EnglishName) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrSpeechEngine_INTERFACE_DEFINED__ */


#ifndef __IDaSvrSpeechEngines_INTERFACE_DEFINED__
#define __IDaSvrSpeechEngines_INTERFACE_DEFINED__

/* interface IDaSvrSpeechEngines */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrSpeechEngines;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E51A-A208-11DE-ABF2-002421116FB2")
    IDaSvrSpeechEngines : public IDispatch
    {
    public:
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ long Index,
            /* [retval][out] */ IDaSvrSpeechEngine **SpeechEngine) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *Count) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrSpeechEnginesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrSpeechEngines * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrSpeechEngines * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrSpeechEngines * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrSpeechEngines * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrSpeechEngines * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrSpeechEngines * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrSpeechEngines * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IDaSvrSpeechEngines * This,
            /* [in] */ long Index,
            /* [retval][out] */ IDaSvrSpeechEngine **SpeechEngine);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IDaSvrSpeechEngines * This,
            /* [retval][out] */ long *Count);
        
        END_INTERFACE
    } IDaSvrSpeechEnginesVtbl;

    interface IDaSvrSpeechEngines
    {
        CONST_VTBL struct IDaSvrSpeechEnginesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrSpeechEngines_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrSpeechEngines_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrSpeechEngines_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrSpeechEngines_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrSpeechEngines_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrSpeechEngines_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrSpeechEngines_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrSpeechEngines_get_Item(This,Index,SpeechEngine)	\
    ( (This)->lpVtbl -> get_Item(This,Index,SpeechEngine) ) 

#define IDaSvrSpeechEngines_get_Count(This,Count)	\
    ( (This)->lpVtbl -> get_Count(This,Count) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrSpeechEngines_INTERFACE_DEFINED__ */


#ifndef __IDaSvrRecognitionEngine_INTERFACE_DEFINED__
#define __IDaSvrRecognitionEngine_INTERFACE_DEFINED__

/* interface IDaSvrRecognitionEngine */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrRecognitionEngine;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E51B-A208-11DE-ABF2-002421116FB2")
    IDaSvrRecognitionEngine : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetSRModeID( 
            /* [out] */ BSTR *SRModeID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetDisplayName( 
            /* [out] */ BSTR *DisplayName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetManufacturer( 
            /* [out] */ BSTR *Manufacturer) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVersion( 
            /* [out] */ short *MajorVersion,
            /* [out] */ short *MinorVersion) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetLanguageID( 
            /* [out] */ long *LanguageID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetLanguageName( 
            /* [out] */ BSTR *LanguageName,
            /* [defaultvalue][in] */ boolean EnglishName = TRUE) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetLanguageIDs( 
            /* [out] */ SAFEARRAY * *LanguageIDs) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetLanguageNames( 
            /* [out] */ SAFEARRAY * *LanguageNames,
            /* [defaultvalue][in] */ boolean EnglishNames = TRUE) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrRecognitionEngineVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrRecognitionEngine * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrRecognitionEngine * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrRecognitionEngine * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrRecognitionEngine * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrRecognitionEngine * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrRecognitionEngine * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrRecognitionEngine * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetSRModeID )( 
            IDaSvrRecognitionEngine * This,
            /* [out] */ BSTR *SRModeID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetDisplayName )( 
            IDaSvrRecognitionEngine * This,
            /* [out] */ BSTR *DisplayName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetManufacturer )( 
            IDaSvrRecognitionEngine * This,
            /* [out] */ BSTR *Manufacturer);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVersion )( 
            IDaSvrRecognitionEngine * This,
            /* [out] */ short *MajorVersion,
            /* [out] */ short *MinorVersion);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetLanguageID )( 
            IDaSvrRecognitionEngine * This,
            /* [out] */ long *LanguageID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetLanguageName )( 
            IDaSvrRecognitionEngine * This,
            /* [out] */ BSTR *LanguageName,
            /* [defaultvalue][in] */ boolean EnglishName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetLanguageIDs )( 
            IDaSvrRecognitionEngine * This,
            /* [out] */ SAFEARRAY * *LanguageIDs);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetLanguageNames )( 
            IDaSvrRecognitionEngine * This,
            /* [out] */ SAFEARRAY * *LanguageNames,
            /* [defaultvalue][in] */ boolean EnglishNames);
        
        END_INTERFACE
    } IDaSvrRecognitionEngineVtbl;

    interface IDaSvrRecognitionEngine
    {
        CONST_VTBL struct IDaSvrRecognitionEngineVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrRecognitionEngine_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrRecognitionEngine_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrRecognitionEngine_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrRecognitionEngine_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrRecognitionEngine_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrRecognitionEngine_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrRecognitionEngine_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrRecognitionEngine_GetSRModeID(This,SRModeID)	\
    ( (This)->lpVtbl -> GetSRModeID(This,SRModeID) ) 

#define IDaSvrRecognitionEngine_GetDisplayName(This,DisplayName)	\
    ( (This)->lpVtbl -> GetDisplayName(This,DisplayName) ) 

#define IDaSvrRecognitionEngine_GetManufacturer(This,Manufacturer)	\
    ( (This)->lpVtbl -> GetManufacturer(This,Manufacturer) ) 

#define IDaSvrRecognitionEngine_GetVersion(This,MajorVersion,MinorVersion)	\
    ( (This)->lpVtbl -> GetVersion(This,MajorVersion,MinorVersion) ) 

#define IDaSvrRecognitionEngine_GetLanguageID(This,LanguageID)	\
    ( (This)->lpVtbl -> GetLanguageID(This,LanguageID) ) 

#define IDaSvrRecognitionEngine_GetLanguageName(This,LanguageName,EnglishName)	\
    ( (This)->lpVtbl -> GetLanguageName(This,LanguageName,EnglishName) ) 

#define IDaSvrRecognitionEngine_GetLanguageIDs(This,LanguageIDs)	\
    ( (This)->lpVtbl -> GetLanguageIDs(This,LanguageIDs) ) 

#define IDaSvrRecognitionEngine_GetLanguageNames(This,LanguageNames,EnglishNames)	\
    ( (This)->lpVtbl -> GetLanguageNames(This,LanguageNames,EnglishNames) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrRecognitionEngine_INTERFACE_DEFINED__ */


#ifndef __IDaSvrRecognitionEngines_INTERFACE_DEFINED__
#define __IDaSvrRecognitionEngines_INTERFACE_DEFINED__

/* interface IDaSvrRecognitionEngines */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrRecognitionEngines;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E51C-A208-11DE-ABF2-002421116FB2")
    IDaSvrRecognitionEngines : public IDispatch
    {
    public:
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ long Index,
            /* [retval][out] */ IDaSvrRecognitionEngine **RecognitionEngine) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *Count) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrRecognitionEnginesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrRecognitionEngines * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrRecognitionEngines * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrRecognitionEngines * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrRecognitionEngines * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrRecognitionEngines * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrRecognitionEngines * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrRecognitionEngines * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IDaSvrRecognitionEngines * This,
            /* [in] */ long Index,
            /* [retval][out] */ IDaSvrRecognitionEngine **RecognitionEngine);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IDaSvrRecognitionEngines * This,
            /* [retval][out] */ long *Count);
        
        END_INTERFACE
    } IDaSvrRecognitionEnginesVtbl;

    interface IDaSvrRecognitionEngines
    {
        CONST_VTBL struct IDaSvrRecognitionEnginesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrRecognitionEngines_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrRecognitionEngines_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrRecognitionEngines_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrRecognitionEngines_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrRecognitionEngines_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrRecognitionEngines_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrRecognitionEngines_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrRecognitionEngines_get_Item(This,Index,RecognitionEngine)	\
    ( (This)->lpVtbl -> get_Item(This,Index,RecognitionEngine) ) 

#define IDaSvrRecognitionEngines_get_Count(This,Count)	\
    ( (This)->lpVtbl -> get_Count(This,Count) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrRecognitionEngines_INTERFACE_DEFINED__ */


#ifndef __IDaServer2_INTERFACE_DEFINED__
#define __IDaServer2_INTERFACE_DEFINED__

/* interface IDaServer2 */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaServer2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E51D-A208-11DE-ABF2-002421116FB2")
    IDaServer2 : public IDaServer
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCharacter2( 
            /* [in] */ long CharacterID,
            /* [retval][out] */ IDaSvrCharacter2 **Character2) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCharacterFiles( 
            /* [retval][out] */ IDaSvrCharacterFiles **CharacterFiles) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_CharacterStyle( 
            /* [retval][out] */ long *CharacterStyle) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_CharacterStyle( 
            /* [in] */ long CharacterStyle) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetSpeechEngines( 
            /* [retval][out] */ IDaSvrSpeechEngines **SpeechEngines) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE FindSpeechEngines( 
            /* [in] */ long LanguageID,
            /* [in] */ short Gender,
            /* [retval][out] */ IDaSvrSpeechEngines **SpeechEngines) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCharacterSpeechEngine( 
            /* [in] */ VARIANT LoadKey,
            /* [retval][out] */ IDaSvrSpeechEngine **SpeechEngine) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE FindCharacterSpeechEngines( 
            /* [in] */ VARIANT LoadKey,
            /* [defaultvalue][in] */ long LanguageID,
            /* [retval][out] */ IDaSvrSpeechEngines **SpeechEngines) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetRecognitionEngines( 
            /* [retval][out] */ IDaSvrRecognitionEngines **RecognitionEngines) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE FindRecognitionEngines( 
            /* [in] */ long LanguageID,
            /* [retval][out] */ IDaSvrRecognitionEngines **RecognitionEngines) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCharacterRecognitionEngine( 
            /* [in] */ VARIANT LoadKey,
            /* [retval][out] */ IDaSvrRecognitionEngine **RecognitionEngine) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE FindCharacterRecognitionEngines( 
            /* [in] */ VARIANT LoadKey,
            /* [defaultvalue][in] */ long LanguageID,
            /* [retval][out] */ IDaSvrRecognitionEngines **RecognitionEngines) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaServer2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaServer2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaServer2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaServer2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaServer2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaServer2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaServer2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaServer2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Load )( 
            IDaServer2 * This,
            /* [in] */ VARIANT vLoadKey,
            /* [out] */ long *pdwCharID,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Unload )( 
            IDaServer2 * This,
            /* [in] */ long dwCharID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Register )( 
            IDaServer2 * This,
            /* [in] */ IUnknown *punkNotifySink,
            /* [out] */ long *pdwSinkID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Unregister )( 
            IDaServer2 * This,
            /* [in] */ long dwSinkID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCharacter )( 
            IDaServer2 * This,
            /* [in] */ long dwCharID,
            /* [out] */ IDispatch **ppunkCharacter);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetSuspended )( 
            IDaServer2 * This,
            /* [out] */ long *pbSuspended);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCharacterEx )( 
            IDaServer2 * This,
            /* [in] */ long dwCharID,
            /* [out] */ IDaSvrCharacter **ppCharacterEx);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVersion )( 
            IDaServer2 * This,
            /* [out] */ short *psMajor,
            /* [out] */ short *psMinor);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ShowDefaultCharacterProperties )( 
            IDaServer2 * This,
            /* [in] */ short x,
            /* [in] */ short y,
            /* [in] */ long bUseDefaultPosition);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCharacter2 )( 
            IDaServer2 * This,
            /* [in] */ long CharacterID,
            /* [retval][out] */ IDaSvrCharacter2 **Character2);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCharacterFiles )( 
            IDaServer2 * This,
            /* [retval][out] */ IDaSvrCharacterFiles **CharacterFiles);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_CharacterStyle )( 
            IDaServer2 * This,
            /* [retval][out] */ long *CharacterStyle);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_CharacterStyle )( 
            IDaServer2 * This,
            /* [in] */ long CharacterStyle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetSpeechEngines )( 
            IDaServer2 * This,
            /* [retval][out] */ IDaSvrSpeechEngines **SpeechEngines);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *FindSpeechEngines )( 
            IDaServer2 * This,
            /* [in] */ long LanguageID,
            /* [in] */ short Gender,
            /* [retval][out] */ IDaSvrSpeechEngines **SpeechEngines);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCharacterSpeechEngine )( 
            IDaServer2 * This,
            /* [in] */ VARIANT LoadKey,
            /* [retval][out] */ IDaSvrSpeechEngine **SpeechEngine);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *FindCharacterSpeechEngines )( 
            IDaServer2 * This,
            /* [in] */ VARIANT LoadKey,
            /* [defaultvalue][in] */ long LanguageID,
            /* [retval][out] */ IDaSvrSpeechEngines **SpeechEngines);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetRecognitionEngines )( 
            IDaServer2 * This,
            /* [retval][out] */ IDaSvrRecognitionEngines **RecognitionEngines);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *FindRecognitionEngines )( 
            IDaServer2 * This,
            /* [in] */ long LanguageID,
            /* [retval][out] */ IDaSvrRecognitionEngines **RecognitionEngines);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCharacterRecognitionEngine )( 
            IDaServer2 * This,
            /* [in] */ VARIANT LoadKey,
            /* [retval][out] */ IDaSvrRecognitionEngine **RecognitionEngine);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *FindCharacterRecognitionEngines )( 
            IDaServer2 * This,
            /* [in] */ VARIANT LoadKey,
            /* [defaultvalue][in] */ long LanguageID,
            /* [retval][out] */ IDaSvrRecognitionEngines **RecognitionEngines);
        
        END_INTERFACE
    } IDaServer2Vtbl;

    interface IDaServer2
    {
        CONST_VTBL struct IDaServer2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaServer2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaServer2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaServer2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaServer2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaServer2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaServer2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaServer2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaServer2_Load(This,vLoadKey,pdwCharID,pdwReqID)	\
    ( (This)->lpVtbl -> Load(This,vLoadKey,pdwCharID,pdwReqID) ) 

#define IDaServer2_Unload(This,dwCharID)	\
    ( (This)->lpVtbl -> Unload(This,dwCharID) ) 

#define IDaServer2_Register(This,punkNotifySink,pdwSinkID)	\
    ( (This)->lpVtbl -> Register(This,punkNotifySink,pdwSinkID) ) 

#define IDaServer2_Unregister(This,dwSinkID)	\
    ( (This)->lpVtbl -> Unregister(This,dwSinkID) ) 

#define IDaServer2_GetCharacter(This,dwCharID,ppunkCharacter)	\
    ( (This)->lpVtbl -> GetCharacter(This,dwCharID,ppunkCharacter) ) 

#define IDaServer2_GetSuspended(This,pbSuspended)	\
    ( (This)->lpVtbl -> GetSuspended(This,pbSuspended) ) 

#define IDaServer2_GetCharacterEx(This,dwCharID,ppCharacterEx)	\
    ( (This)->lpVtbl -> GetCharacterEx(This,dwCharID,ppCharacterEx) ) 

#define IDaServer2_GetVersion(This,psMajor,psMinor)	\
    ( (This)->lpVtbl -> GetVersion(This,psMajor,psMinor) ) 

#define IDaServer2_ShowDefaultCharacterProperties(This,x,y,bUseDefaultPosition)	\
    ( (This)->lpVtbl -> ShowDefaultCharacterProperties(This,x,y,bUseDefaultPosition) ) 


#define IDaServer2_GetCharacter2(This,CharacterID,Character2)	\
    ( (This)->lpVtbl -> GetCharacter2(This,CharacterID,Character2) ) 

#define IDaServer2_GetCharacterFiles(This,CharacterFiles)	\
    ( (This)->lpVtbl -> GetCharacterFiles(This,CharacterFiles) ) 

#define IDaServer2_get_CharacterStyle(This,CharacterStyle)	\
    ( (This)->lpVtbl -> get_CharacterStyle(This,CharacterStyle) ) 

#define IDaServer2_put_CharacterStyle(This,CharacterStyle)	\
    ( (This)->lpVtbl -> put_CharacterStyle(This,CharacterStyle) ) 

#define IDaServer2_GetSpeechEngines(This,SpeechEngines)	\
    ( (This)->lpVtbl -> GetSpeechEngines(This,SpeechEngines) ) 

#define IDaServer2_FindSpeechEngines(This,LanguageID,Gender,SpeechEngines)	\
    ( (This)->lpVtbl -> FindSpeechEngines(This,LanguageID,Gender,SpeechEngines) ) 

#define IDaServer2_GetCharacterSpeechEngine(This,LoadKey,SpeechEngine)	\
    ( (This)->lpVtbl -> GetCharacterSpeechEngine(This,LoadKey,SpeechEngine) ) 

#define IDaServer2_FindCharacterSpeechEngines(This,LoadKey,LanguageID,SpeechEngines)	\
    ( (This)->lpVtbl -> FindCharacterSpeechEngines(This,LoadKey,LanguageID,SpeechEngines) ) 

#define IDaServer2_GetRecognitionEngines(This,RecognitionEngines)	\
    ( (This)->lpVtbl -> GetRecognitionEngines(This,RecognitionEngines) ) 

#define IDaServer2_FindRecognitionEngines(This,LanguageID,RecognitionEngines)	\
    ( (This)->lpVtbl -> FindRecognitionEngines(This,LanguageID,RecognitionEngines) ) 

#define IDaServer2_GetCharacterRecognitionEngine(This,LoadKey,RecognitionEngine)	\
    ( (This)->lpVtbl -> GetCharacterRecognitionEngine(This,LoadKey,RecognitionEngine) ) 

#define IDaServer2_FindCharacterRecognitionEngines(This,LoadKey,LanguageID,RecognitionEngines)	\
    ( (This)->lpVtbl -> FindCharacterRecognitionEngines(This,LoadKey,LanguageID,RecognitionEngines) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaServer2_INTERFACE_DEFINED__ */


#ifndef __IDaSvrCharacter2_INTERFACE_DEFINED__
#define __IDaSvrCharacter2_INTERFACE_DEFINED__

/* interface IDaSvrCharacter2 */
/* [object][hidden][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrCharacter2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E51E-A208-11DE-ABF2-002421116FB2")
    IDaSvrCharacter2 : public IDaSvrCharacter
    {
    public:
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Style( 
            /* [retval][out] */ long *Style) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Style( 
            /* [in] */ long Style) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_HasIcon( 
            /* [retval][out] */ boolean *HasIcon) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GenerateIcon( 
            /* [defaultvalue][in] */ long ClipLeft = 0,
            /* [defaultvalue][in] */ long ClipTop = 0,
            /* [defaultvalue][in] */ long ClipWidth = -1,
            /* [defaultvalue][in] */ long ClipHeight = -1) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_IconVisible( 
            /* [retval][out] */ boolean *IconVisible) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_IconIdentity( 
            /* [retval][out] */ BSTR *IconIdentity) = 0;
        
        virtual /* [restricted][hidden][id] */ HRESULT STDMETHODCALLTYPE GetIconIdentity( 
            /* [out] */ GUID *IconIdentity) = 0;
        
        virtual /* [restricted][hidden][id] */ HRESULT STDMETHODCALLTYPE SetIconIdentity( 
            /* [in] */ const GUID *IconIdentity) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_IconIdentity( 
            /* [in] */ BSTR IconIdentity) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_IconTip( 
            /* [retval][out] */ BSTR *IconTip) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_IconTip( 
            /* [in] */ BSTR IconTip) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetSpeechEngine( 
            /* [defaultvalue][in] */ boolean GetDefault,
            /* [retval][out] */ IDaSvrSpeechEngine **SpeechEngine) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE FindSpeechEngines( 
            /* [defaultvalue][in] */ long LanguageID,
            /* [retval][out] */ IDaSvrSpeechEngines **SpeechEngines) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetRecognitionEngine( 
            /* [defaultvalue][in] */ boolean GetDefault,
            /* [retval][out] */ IDaSvrRecognitionEngine **RecognitionEngine) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE FindRecognitionEngines( 
            /* [defaultvalue][in] */ long LanguageID,
            /* [retval][out] */ IDaSvrRecognitionEngines **RecognitionEngines) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrCharacter2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrCharacter2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrCharacter2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrCharacter2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrCharacter2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrCharacter2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrCharacter2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrCharacter2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVisible )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *pbVisible);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetPosition )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long lLeft,
            /* [in] */ long lTop);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetPosition )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *plLeft,
            /* [out] */ long *plTop);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetSize )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long lWidth,
            /* [in] */ long lHeight);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetSize )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *plWidth,
            /* [out] */ long *plHeight);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetName )( 
            IDaSvrCharacter2 * This,
            /* [out] */ BSTR *pbszName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetDescription )( 
            IDaSvrCharacter2 * This,
            /* [out] */ BSTR *pbszDescription);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetTTSSpeed )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *pdwSpeed);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetTTSPitch )( 
            IDaSvrCharacter2 * This,
            /* [out] */ short *pwPitch);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Activate )( 
            IDaSvrCharacter2 * This,
            /* [in] */ short sState);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetIdleOn )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long bOn);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetIdleOn )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *pbOn);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Prepare )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long dwType,
            /* [in] */ BSTR bszName,
            /* [in] */ long bQueue,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Play )( 
            IDaSvrCharacter2 * This,
            /* [in] */ BSTR bszAnimation,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long dwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *StopAll )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long lTypes);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Wait )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long dwReqID,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Interrupt )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long dwReqID,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Show )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long bFast,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Hide )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long bFast,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Speak )( 
            IDaSvrCharacter2 * This,
            /* [in] */ BSTR bszText,
            /* [in] */ BSTR bszUrl,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *MoveTo )( 
            IDaSvrCharacter2 * This,
            /* [in] */ short x,
            /* [in] */ short y,
            /* [in] */ long lSpeed,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GestureAt )( 
            IDaSvrCharacter2 * This,
            /* [in] */ short x,
            /* [in] */ short y,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetMoveCause )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *pdwCause);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVisibilityCause )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *pdwCause);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *HasOtherClients )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *plNumOtherClients);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetSoundEffectsOn )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long bOn);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetSoundEffectsOn )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *pbOn);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetName )( 
            IDaSvrCharacter2 * This,
            /* [in] */ BSTR bszName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetDescription )( 
            IDaSvrCharacter2 * This,
            /* [in] */ BSTR bszDescription);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetExtraData )( 
            IDaSvrCharacter2 * This,
            /* [out] */ BSTR *pbszExtraData);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ShowPopupMenu )( 
            IDaSvrCharacter2 * This,
            /* [in] */ short x,
            /* [in] */ short y);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetAutoPopupMenu )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long bAutoPopupMenu);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetAutoPopupMenu )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *pbAutoPopupMenu);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetHelpFileName )( 
            IDaSvrCharacter2 * This,
            /* [out] */ BSTR *pbszName);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *SetHelpFileName )( 
            IDaSvrCharacter2 * This,
            /* [in] */ BSTR bszName);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *SetHelpModeOn )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long bHelpModeOn);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetHelpModeOn )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *pbHelpModeOn);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *SetHelpContextID )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long ulID);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetHelpContextID )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *pulID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetActive )( 
            IDaSvrCharacter2 * This,
            /* [out] */ short *psState);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Listen )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long bListen);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetLanguageID )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long langid);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetLanguageID )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *plangid);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetTTSModeID )( 
            IDaSvrCharacter2 * This,
            /* [out] */ BSTR *pbszModeID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetTTSModeID )( 
            IDaSvrCharacter2 * This,
            /* [in] */ BSTR bszModeID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetSRModeID )( 
            IDaSvrCharacter2 * This,
            /* [out] */ BSTR *pbszModeID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetSRModeID )( 
            IDaSvrCharacter2 * This,
            /* [in] */ BSTR bszModeID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetGUID )( 
            IDaSvrCharacter2 * This,
            /* [out] */ BSTR *pbszID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetOriginalSize )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *plWidth,
            /* [out] */ long *plHeight);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Think )( 
            IDaSvrCharacter2 * This,
            /* [in] */ BSTR bszText,
            /* [out] */ long *pdwReqID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVersion )( 
            IDaSvrCharacter2 * This,
            /* [out] */ short *psMajor,
            /* [out] */ short *psMinor);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetAnimationNames )( 
            IDaSvrCharacter2 * This,
            /* [out] */ IUnknown **punkEnum);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetSRStatus )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *plStatus);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Style )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ long *Style);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Style )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long Style);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_HasIcon )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ boolean *HasIcon);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GenerateIcon )( 
            IDaSvrCharacter2 * This,
            /* [defaultvalue][in] */ long ClipLeft,
            /* [defaultvalue][in] */ long ClipTop,
            /* [defaultvalue][in] */ long ClipWidth,
            /* [defaultvalue][in] */ long ClipHeight);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_IconVisible )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ boolean *IconVisible);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_IconIdentity )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ BSTR *IconIdentity);
        
        /* [restricted][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetIconIdentity )( 
            IDaSvrCharacter2 * This,
            /* [out] */ GUID *IconIdentity);
        
        /* [restricted][hidden][id] */ HRESULT ( STDMETHODCALLTYPE *SetIconIdentity )( 
            IDaSvrCharacter2 * This,
            /* [in] */ const GUID *IconIdentity);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_IconIdentity )( 
            IDaSvrCharacter2 * This,
            /* [in] */ BSTR IconIdentity);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_IconTip )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ BSTR *IconTip);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_IconTip )( 
            IDaSvrCharacter2 * This,
            /* [in] */ BSTR IconTip);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetSpeechEngine )( 
            IDaSvrCharacter2 * This,
            /* [defaultvalue][in] */ boolean GetDefault,
            /* [retval][out] */ IDaSvrSpeechEngine **SpeechEngine);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *FindSpeechEngines )( 
            IDaSvrCharacter2 * This,
            /* [defaultvalue][in] */ long LanguageID,
            /* [retval][out] */ IDaSvrSpeechEngines **SpeechEngines);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetRecognitionEngine )( 
            IDaSvrCharacter2 * This,
            /* [defaultvalue][in] */ boolean GetDefault,
            /* [retval][out] */ IDaSvrRecognitionEngine **RecognitionEngine);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *FindRecognitionEngines )( 
            IDaSvrCharacter2 * This,
            /* [defaultvalue][in] */ long LanguageID,
            /* [retval][out] */ IDaSvrRecognitionEngines **RecognitionEngines);
        
        END_INTERFACE
    } IDaSvrCharacter2Vtbl;

    interface IDaSvrCharacter2
    {
        CONST_VTBL struct IDaSvrCharacter2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrCharacter2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrCharacter2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrCharacter2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrCharacter2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrCharacter2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrCharacter2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrCharacter2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrCharacter2_GetVisible(This,pbVisible)	\
    ( (This)->lpVtbl -> GetVisible(This,pbVisible) ) 

#define IDaSvrCharacter2_SetPosition(This,lLeft,lTop)	\
    ( (This)->lpVtbl -> SetPosition(This,lLeft,lTop) ) 

#define IDaSvrCharacter2_GetPosition(This,plLeft,plTop)	\
    ( (This)->lpVtbl -> GetPosition(This,plLeft,plTop) ) 

#define IDaSvrCharacter2_SetSize(This,lWidth,lHeight)	\
    ( (This)->lpVtbl -> SetSize(This,lWidth,lHeight) ) 

#define IDaSvrCharacter2_GetSize(This,plWidth,plHeight)	\
    ( (This)->lpVtbl -> GetSize(This,plWidth,plHeight) ) 

#define IDaSvrCharacter2_GetName(This,pbszName)	\
    ( (This)->lpVtbl -> GetName(This,pbszName) ) 

#define IDaSvrCharacter2_GetDescription(This,pbszDescription)	\
    ( (This)->lpVtbl -> GetDescription(This,pbszDescription) ) 

#define IDaSvrCharacter2_GetTTSSpeed(This,pdwSpeed)	\
    ( (This)->lpVtbl -> GetTTSSpeed(This,pdwSpeed) ) 

#define IDaSvrCharacter2_GetTTSPitch(This,pwPitch)	\
    ( (This)->lpVtbl -> GetTTSPitch(This,pwPitch) ) 

#define IDaSvrCharacter2_Activate(This,sState)	\
    ( (This)->lpVtbl -> Activate(This,sState) ) 

#define IDaSvrCharacter2_SetIdleOn(This,bOn)	\
    ( (This)->lpVtbl -> SetIdleOn(This,bOn) ) 

#define IDaSvrCharacter2_GetIdleOn(This,pbOn)	\
    ( (This)->lpVtbl -> GetIdleOn(This,pbOn) ) 

#define IDaSvrCharacter2_Prepare(This,dwType,bszName,bQueue,pdwReqID)	\
    ( (This)->lpVtbl -> Prepare(This,dwType,bszName,bQueue,pdwReqID) ) 

#define IDaSvrCharacter2_Play(This,bszAnimation,pdwReqID)	\
    ( (This)->lpVtbl -> Play(This,bszAnimation,pdwReqID) ) 

#define IDaSvrCharacter2_Stop(This,dwReqID)	\
    ( (This)->lpVtbl -> Stop(This,dwReqID) ) 

#define IDaSvrCharacter2_StopAll(This,lTypes)	\
    ( (This)->lpVtbl -> StopAll(This,lTypes) ) 

#define IDaSvrCharacter2_Wait(This,dwReqID,pdwReqID)	\
    ( (This)->lpVtbl -> Wait(This,dwReqID,pdwReqID) ) 

#define IDaSvrCharacter2_Interrupt(This,dwReqID,pdwReqID)	\
    ( (This)->lpVtbl -> Interrupt(This,dwReqID,pdwReqID) ) 

#define IDaSvrCharacter2_Show(This,bFast,pdwReqID)	\
    ( (This)->lpVtbl -> Show(This,bFast,pdwReqID) ) 

#define IDaSvrCharacter2_Hide(This,bFast,pdwReqID)	\
    ( (This)->lpVtbl -> Hide(This,bFast,pdwReqID) ) 

#define IDaSvrCharacter2_Speak(This,bszText,bszUrl,pdwReqID)	\
    ( (This)->lpVtbl -> Speak(This,bszText,bszUrl,pdwReqID) ) 

#define IDaSvrCharacter2_MoveTo(This,x,y,lSpeed,pdwReqID)	\
    ( (This)->lpVtbl -> MoveTo(This,x,y,lSpeed,pdwReqID) ) 

#define IDaSvrCharacter2_GestureAt(This,x,y,pdwReqID)	\
    ( (This)->lpVtbl -> GestureAt(This,x,y,pdwReqID) ) 

#define IDaSvrCharacter2_GetMoveCause(This,pdwCause)	\
    ( (This)->lpVtbl -> GetMoveCause(This,pdwCause) ) 

#define IDaSvrCharacter2_GetVisibilityCause(This,pdwCause)	\
    ( (This)->lpVtbl -> GetVisibilityCause(This,pdwCause) ) 

#define IDaSvrCharacter2_HasOtherClients(This,plNumOtherClients)	\
    ( (This)->lpVtbl -> HasOtherClients(This,plNumOtherClients) ) 

#define IDaSvrCharacter2_SetSoundEffectsOn(This,bOn)	\
    ( (This)->lpVtbl -> SetSoundEffectsOn(This,bOn) ) 

#define IDaSvrCharacter2_GetSoundEffectsOn(This,pbOn)	\
    ( (This)->lpVtbl -> GetSoundEffectsOn(This,pbOn) ) 

#define IDaSvrCharacter2_SetName(This,bszName)	\
    ( (This)->lpVtbl -> SetName(This,bszName) ) 

#define IDaSvrCharacter2_SetDescription(This,bszDescription)	\
    ( (This)->lpVtbl -> SetDescription(This,bszDescription) ) 

#define IDaSvrCharacter2_GetExtraData(This,pbszExtraData)	\
    ( (This)->lpVtbl -> GetExtraData(This,pbszExtraData) ) 

#define IDaSvrCharacter2_ShowPopupMenu(This,x,y)	\
    ( (This)->lpVtbl -> ShowPopupMenu(This,x,y) ) 

#define IDaSvrCharacter2_SetAutoPopupMenu(This,bAutoPopupMenu)	\
    ( (This)->lpVtbl -> SetAutoPopupMenu(This,bAutoPopupMenu) ) 

#define IDaSvrCharacter2_GetAutoPopupMenu(This,pbAutoPopupMenu)	\
    ( (This)->lpVtbl -> GetAutoPopupMenu(This,pbAutoPopupMenu) ) 

#define IDaSvrCharacter2_GetHelpFileName(This,pbszName)	\
    ( (This)->lpVtbl -> GetHelpFileName(This,pbszName) ) 

#define IDaSvrCharacter2_SetHelpFileName(This,bszName)	\
    ( (This)->lpVtbl -> SetHelpFileName(This,bszName) ) 

#define IDaSvrCharacter2_SetHelpModeOn(This,bHelpModeOn)	\
    ( (This)->lpVtbl -> SetHelpModeOn(This,bHelpModeOn) ) 

#define IDaSvrCharacter2_GetHelpModeOn(This,pbHelpModeOn)	\
    ( (This)->lpVtbl -> GetHelpModeOn(This,pbHelpModeOn) ) 

#define IDaSvrCharacter2_SetHelpContextID(This,ulID)	\
    ( (This)->lpVtbl -> SetHelpContextID(This,ulID) ) 

#define IDaSvrCharacter2_GetHelpContextID(This,pulID)	\
    ( (This)->lpVtbl -> GetHelpContextID(This,pulID) ) 

#define IDaSvrCharacter2_GetActive(This,psState)	\
    ( (This)->lpVtbl -> GetActive(This,psState) ) 

#define IDaSvrCharacter2_Listen(This,bListen)	\
    ( (This)->lpVtbl -> Listen(This,bListen) ) 

#define IDaSvrCharacter2_SetLanguageID(This,langid)	\
    ( (This)->lpVtbl -> SetLanguageID(This,langid) ) 

#define IDaSvrCharacter2_GetLanguageID(This,plangid)	\
    ( (This)->lpVtbl -> GetLanguageID(This,plangid) ) 

#define IDaSvrCharacter2_GetTTSModeID(This,pbszModeID)	\
    ( (This)->lpVtbl -> GetTTSModeID(This,pbszModeID) ) 

#define IDaSvrCharacter2_SetTTSModeID(This,bszModeID)	\
    ( (This)->lpVtbl -> SetTTSModeID(This,bszModeID) ) 

#define IDaSvrCharacter2_GetSRModeID(This,pbszModeID)	\
    ( (This)->lpVtbl -> GetSRModeID(This,pbszModeID) ) 

#define IDaSvrCharacter2_SetSRModeID(This,bszModeID)	\
    ( (This)->lpVtbl -> SetSRModeID(This,bszModeID) ) 

#define IDaSvrCharacter2_GetGUID(This,pbszID)	\
    ( (This)->lpVtbl -> GetGUID(This,pbszID) ) 

#define IDaSvrCharacter2_GetOriginalSize(This,plWidth,plHeight)	\
    ( (This)->lpVtbl -> GetOriginalSize(This,plWidth,plHeight) ) 

#define IDaSvrCharacter2_Think(This,bszText,pdwReqID)	\
    ( (This)->lpVtbl -> Think(This,bszText,pdwReqID) ) 

#define IDaSvrCharacter2_GetVersion(This,psMajor,psMinor)	\
    ( (This)->lpVtbl -> GetVersion(This,psMajor,psMinor) ) 

#define IDaSvrCharacter2_GetAnimationNames(This,punkEnum)	\
    ( (This)->lpVtbl -> GetAnimationNames(This,punkEnum) ) 

#define IDaSvrCharacter2_GetSRStatus(This,plStatus)	\
    ( (This)->lpVtbl -> GetSRStatus(This,plStatus) ) 


#define IDaSvrCharacter2_get_Style(This,Style)	\
    ( (This)->lpVtbl -> get_Style(This,Style) ) 

#define IDaSvrCharacter2_put_Style(This,Style)	\
    ( (This)->lpVtbl -> put_Style(This,Style) ) 

#define IDaSvrCharacter2_get_HasIcon(This,HasIcon)	\
    ( (This)->lpVtbl -> get_HasIcon(This,HasIcon) ) 

#define IDaSvrCharacter2_GenerateIcon(This,ClipLeft,ClipTop,ClipWidth,ClipHeight)	\
    ( (This)->lpVtbl -> GenerateIcon(This,ClipLeft,ClipTop,ClipWidth,ClipHeight) ) 

#define IDaSvrCharacter2_get_IconVisible(This,IconVisible)	\
    ( (This)->lpVtbl -> get_IconVisible(This,IconVisible) ) 

#define IDaSvrCharacter2_get_IconIdentity(This,IconIdentity)	\
    ( (This)->lpVtbl -> get_IconIdentity(This,IconIdentity) ) 

#define IDaSvrCharacter2_GetIconIdentity(This,IconIdentity)	\
    ( (This)->lpVtbl -> GetIconIdentity(This,IconIdentity) ) 

#define IDaSvrCharacter2_SetIconIdentity(This,IconIdentity)	\
    ( (This)->lpVtbl -> SetIconIdentity(This,IconIdentity) ) 

#define IDaSvrCharacter2_put_IconIdentity(This,IconIdentity)	\
    ( (This)->lpVtbl -> put_IconIdentity(This,IconIdentity) ) 

#define IDaSvrCharacter2_get_IconTip(This,IconTip)	\
    ( (This)->lpVtbl -> get_IconTip(This,IconTip) ) 

#define IDaSvrCharacter2_put_IconTip(This,IconTip)	\
    ( (This)->lpVtbl -> put_IconTip(This,IconTip) ) 

#define IDaSvrCharacter2_GetSpeechEngine(This,GetDefault,SpeechEngine)	\
    ( (This)->lpVtbl -> GetSpeechEngine(This,GetDefault,SpeechEngine) ) 

#define IDaSvrCharacter2_FindSpeechEngines(This,LanguageID,SpeechEngines)	\
    ( (This)->lpVtbl -> FindSpeechEngines(This,LanguageID,SpeechEngines) ) 

#define IDaSvrCharacter2_GetRecognitionEngine(This,GetDefault,RecognitionEngine)	\
    ( (This)->lpVtbl -> GetRecognitionEngine(This,GetDefault,RecognitionEngine) ) 

#define IDaSvrCharacter2_FindRecognitionEngines(This,LanguageID,RecognitionEngines)	\
    ( (This)->lpVtbl -> FindRecognitionEngines(This,LanguageID,RecognitionEngines) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrCharacter2_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_DaServer;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E500-A208-11DE-ABF2-002421116FB2")
DaServer;
#endif

EXTERN_C const CLSID CLSID_DaSvrCharacter;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E50D-A208-11DE-ABF2-002421116FB2")
DaSvrCharacter;
#endif

EXTERN_C const CLSID CLSID_DaSvrCommand;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E50E-A208-11DE-ABF2-002421116FB2")
DaSvrCommand;
#endif

EXTERN_C const CLSID CLSID_DaSvrCommands;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E50F-A208-11DE-ABF2-002421116FB2")
DaSvrCommands;
#endif

EXTERN_C const CLSID CLSID_DaSvrCommandsWindow;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E510-A208-11DE-ABF2-002421116FB2")
DaSvrCommandsWindow;
#endif

EXTERN_C const CLSID CLSID_DaSvrUserInput;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E511-A208-11DE-ABF2-002421116FB2")
DaSvrUserInput;
#endif

EXTERN_C const CLSID CLSID_DaSvrBalloon;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E512-A208-11DE-ABF2-002421116FB2")
DaSvrBalloon;
#endif

EXTERN_C const CLSID CLSID_DaSvrPropertySheet;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E513-A208-11DE-ABF2-002421116FB2")
DaSvrPropertySheet;
#endif

EXTERN_C const CLSID CLSID_DaSvrAudioOutput;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E514-A208-11DE-ABF2-002421116FB2")
DaSvrAudioOutput;
#endif

EXTERN_C const CLSID CLSID_DaSvrSpeechInput;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E515-A208-11DE-ABF2-002421116FB2")
DaSvrSpeechInput;
#endif

EXTERN_C const CLSID CLSID_DaSvrAnimationNames;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E517-A208-11DE-ABF2-002421116FB2")
DaSvrAnimationNames;
#endif

EXTERN_C const CLSID CLSID_DaSvrCharacterFiles;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E524-A208-11DE-ABF2-002421116FB2")
DaSvrCharacterFiles;
#endif

EXTERN_C const CLSID CLSID_DaSvrSpeechEngine;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E51F-A208-11DE-ABF2-002421116FB2")
DaSvrSpeechEngine;
#endif

EXTERN_C const CLSID CLSID_DaSvrSpeechEngines;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E520-A208-11DE-ABF2-002421116FB2")
DaSvrSpeechEngines;
#endif

EXTERN_C const CLSID CLSID_DaSvrRecognitionEngine;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E521-A208-11DE-ABF2-002421116FB2")
DaSvrRecognitionEngine;
#endif

EXTERN_C const CLSID CLSID_DaSvrRecognitionEngines;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E522-A208-11DE-ABF2-002421116FB2")
DaSvrRecognitionEngines;
#endif
#endif /* __DoubleAgentSvr_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


