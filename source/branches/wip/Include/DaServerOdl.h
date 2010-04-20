

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Tue Apr 20 04:27:58 2010
 */
/* Compiler settings for .\Server\DaServer.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
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


#ifndef __IDaSvrCommand_FWD_DEFINED__
#define __IDaSvrCommand_FWD_DEFINED__
typedef interface IDaSvrCommand IDaSvrCommand;
#endif 	/* __IDaSvrCommand_FWD_DEFINED__ */


#ifndef __IDaSvrCommands_FWD_DEFINED__
#define __IDaSvrCommands_FWD_DEFINED__
typedef interface IDaSvrCommands IDaSvrCommands;
#endif 	/* __IDaSvrCommands_FWD_DEFINED__ */


#ifndef __IDaSvrUserInput_FWD_DEFINED__
#define __IDaSvrUserInput_FWD_DEFINED__
typedef interface IDaSvrUserInput IDaSvrUserInput;
#endif 	/* __IDaSvrUserInput_FWD_DEFINED__ */


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


#ifndef __IDaSvrTTSEngine_FWD_DEFINED__
#define __IDaSvrTTSEngine_FWD_DEFINED__
typedef interface IDaSvrTTSEngine IDaSvrTTSEngine;
#endif 	/* __IDaSvrTTSEngine_FWD_DEFINED__ */


#ifndef __IDaSvrTTSEngines_FWD_DEFINED__
#define __IDaSvrTTSEngines_FWD_DEFINED__
typedef interface IDaSvrTTSEngines IDaSvrTTSEngines;
#endif 	/* __IDaSvrTTSEngines_FWD_DEFINED__ */


#ifndef __IDaSvrSREngine_FWD_DEFINED__
#define __IDaSvrSREngine_FWD_DEFINED__
typedef interface IDaSvrSREngine IDaSvrSREngine;
#endif 	/* __IDaSvrSREngine_FWD_DEFINED__ */


#ifndef __IDaSvrSREngines_FWD_DEFINED__
#define __IDaSvrSREngines_FWD_DEFINED__
typedef interface IDaSvrSREngines IDaSvrSREngines;
#endif 	/* __IDaSvrSREngines_FWD_DEFINED__ */


#ifndef __IDaServer2_FWD_DEFINED__
#define __IDaServer2_FWD_DEFINED__
typedef interface IDaServer2 IDaServer2;
#endif 	/* __IDaServer2_FWD_DEFINED__ */


#ifndef __IDaSvrCharacter2_FWD_DEFINED__
#define __IDaSvrCharacter2_FWD_DEFINED__
typedef interface IDaSvrCharacter2 IDaSvrCharacter2;
#endif 	/* __IDaSvrCharacter2_FWD_DEFINED__ */


#ifndef __IDaSvrBalloon2_FWD_DEFINED__
#define __IDaSvrBalloon2_FWD_DEFINED__
typedef interface IDaSvrBalloon2 IDaSvrBalloon2;
#endif 	/* __IDaSvrBalloon2_FWD_DEFINED__ */


#ifndef __IDaSvrPropertySheet2_FWD_DEFINED__
#define __IDaSvrPropertySheet2_FWD_DEFINED__
typedef interface IDaSvrPropertySheet2 IDaSvrPropertySheet2;
#endif 	/* __IDaSvrPropertySheet2_FWD_DEFINED__ */


#ifndef __IDaSvrSettings_FWD_DEFINED__
#define __IDaSvrSettings_FWD_DEFINED__
typedef interface IDaSvrSettings IDaSvrSettings;
#endif 	/* __IDaSvrSettings_FWD_DEFINED__ */


#ifndef __IDaSvrCommandsWindow2_FWD_DEFINED__
#define __IDaSvrCommandsWindow2_FWD_DEFINED__
typedef interface IDaSvrCommandsWindow2 IDaSvrCommandsWindow2;
#endif 	/* __IDaSvrCommandsWindow2_FWD_DEFINED__ */


#ifndef __IDaSvrUserInput2_FWD_DEFINED__
#define __IDaSvrUserInput2_FWD_DEFINED__
typedef interface IDaSvrUserInput2 IDaSvrUserInput2;
#endif 	/* __IDaSvrUserInput2_FWD_DEFINED__ */


#ifndef __IDaSvrCommand2_FWD_DEFINED__
#define __IDaSvrCommand2_FWD_DEFINED__
typedef interface IDaSvrCommand2 IDaSvrCommand2;
#endif 	/* __IDaSvrCommand2_FWD_DEFINED__ */


#ifndef __IDaSvrCommands2_FWD_DEFINED__
#define __IDaSvrCommands2_FWD_DEFINED__
typedef interface IDaSvrCommands2 IDaSvrCommands2;
#endif 	/* __IDaSvrCommands2_FWD_DEFINED__ */


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


#ifndef __DaSvrSettings_FWD_DEFINED__
#define __DaSvrSettings_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaSvrSettings DaSvrSettings;
#else
typedef struct DaSvrSettings DaSvrSettings;
#endif /* __cplusplus */

#endif 	/* __DaSvrSettings_FWD_DEFINED__ */


#ifndef __DaSvrCharacterFiles_FWD_DEFINED__
#define __DaSvrCharacterFiles_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaSvrCharacterFiles DaSvrCharacterFiles;
#else
typedef struct DaSvrCharacterFiles DaSvrCharacterFiles;
#endif /* __cplusplus */

#endif 	/* __DaSvrCharacterFiles_FWD_DEFINED__ */


#ifndef __DaSvrTTSEngine_FWD_DEFINED__
#define __DaSvrTTSEngine_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaSvrTTSEngine DaSvrTTSEngine;
#else
typedef struct DaSvrTTSEngine DaSvrTTSEngine;
#endif /* __cplusplus */

#endif 	/* __DaSvrTTSEngine_FWD_DEFINED__ */


#ifndef __DaSvrTTSEngines_FWD_DEFINED__
#define __DaSvrTTSEngines_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaSvrTTSEngines DaSvrTTSEngines;
#else
typedef struct DaSvrTTSEngines DaSvrTTSEngines;
#endif /* __cplusplus */

#endif 	/* __DaSvrTTSEngines_FWD_DEFINED__ */


#ifndef __DaSvrSREngine_FWD_DEFINED__
#define __DaSvrSREngine_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaSvrSREngine DaSvrSREngine;
#else
typedef struct DaSvrSREngine DaSvrSREngine;
#endif /* __cplusplus */

#endif 	/* __DaSvrSREngine_FWD_DEFINED__ */


#ifndef __DaSvrSREngines_FWD_DEFINED__
#define __DaSvrSREngines_FWD_DEFINED__

#ifdef __cplusplus
typedef class DaSvrSREngines DaSvrSREngines;
#else
typedef struct DaSvrSREngines DaSvrSREngines;
#endif /* __cplusplus */

#endif 	/* __DaSvrSREngines_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __DoubleAgentSvr_LIBRARY_DEFINED__
#define __DoubleAgentSvr_LIBRARY_DEFINED__

/* library DoubleAgentSvr */
/* [helpstring][version][uuid] */ 



























#ifndef _DA_COMMON_TYPES_
#define _DA_COMMON_TYPES_
typedef /* [helpstring] */ 
enum CharacterStyleFlags
    {	CharacterStyle_SoundEffects	= 0x1,
	CharacterStyle_IdleEnabled	= 0x2,
	CharacterStyle_AutoPopupMenu	= 0x4,
	CharacterStyle_IconShown	= 0x8,
	CharacterStyle_Smoothed	= 0x30,
	CharacterStyle_SmoothEdges	= 0x20
    } 	CharacterStyleFlags;

typedef /* [helpstring] */ 
enum BalloonStyleFlags
    {	BalloonStyle_Enabled	= 0x1,
	BalloonStyle_SizeToText	= 0x2,
	BalloonStyle_AutoHide	= 0x4,
	BalloonStyle_AutoPace	= 0x8,
	BalloonStyle_ShowPartialLines	= 0x10
    } 	BalloonStyleFlags;

typedef /* [helpstring] */ 
enum ActiveStateType
    {	ActiveState_Inactive	= 0,
	ActiveState_Active	= 1,
	ActiveState_InputActive	= 2
    } 	ActiveStateType;

typedef /* [helpstring] */ 
enum StopAllFlags
    {	StopAll_Play	= 0x1,
	StopAll_Move	= 0x2,
	StopAll_Speak	= 0x4,
	StopAll_QueuedPrepare	= 0x8,
	StopAll_ImmediatePrepare	= 0x10,
	StopAll_Visibility	= 0x20,
	StopAll_Everything	= 0xffffffff
    } 	StopAllFlags;

typedef /* [helpstring] */ 
enum PrepareResourceType
    {	PrepareResource_Animation	= 0,
	PrepareResource_State	= 1,
	PrepareResource_Wave	= 2
    } 	PrepareResourceType;

typedef /* [helpstring] */ 
enum MoveCauseType
    {	MoveCause_NeverMoved	= 0,
	MoveCause_UserMoved	= 1,
	MoveCause_ProgramMoved	= 2,
	MoveCause_OtherProgramMoved	= 3
    } 	MoveCauseType;

typedef /* [helpstring] */ 
enum VisibilityCauseType
    {	VisibilityCause_NeverShown	= 0,
	VisibilityCause_UserHid	= 1,
	VisibilityCause_UserShowed	= 2,
	VisibilityCause_ProgramHid	= 3,
	VisibilityCause_ProgramShowed	= 4,
	VisibilityCause_OtherProgramHid	= 5,
	VisibilityCause_OtherProgramShowed	= 6
    } 	VisibilityCauseType;

typedef /* [helpstring] */ 
enum AudioStatusType
    {	AudioStatus_Available	= 0,
	AudioStatus_Disabled	= 1,
	AudioStatus_UserSpeaking	= 3,
	AudioStatus_CharacterSpeaking	= 4,
	AudioStatus_CharacterListening	= 5,
	AudioStatus_Error	= 6
    } 	AudioStatusType;

typedef /* [helpstring] */ 
enum ListeningStatusType
    {	ListeningStatus_Available	= 0,
	ListeningStatus_CharacterInactive	= 2,
	ListeningStatus_InitializeFailed	= 4,
	ListeningStatus_InputDisabled	= 5,
	ListeningStatus_Error	= 6
    } 	ListeningStatusType;

typedef /* [helpstring] */ 
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

typedef /* [helpstring] */ 
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
typedef 
enum FilesFilterFlags
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
    } 	FilesFilterFlags;

#define	NeverMoved	( MoveCause_NeverMoved )

#define	UserMoved	( MoveCause_UserMoved )

#define	ProgramMoved	( MoveCause_ProgramMoved )

#define	OtherProgramMoved	( MoveCause_OtherProgramMoved )

#define	SystemMoved	( 4 )

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

#define	ACTIVATE_NOTTOPMOST	( ActiveState_Inactive )

#define	ACTIVATE_TOPMOST	( ActiveState_Active )

#define	ACTIVATE_NOTACTIVE	( ActiveState_Inactive )

#define	ACTIVATE_ACTIVE	( ActiveState_Active )

#define	ACTIVATE_INPUTACTIVE	( ActiveState_InputActive )

#define	PREPARE_ANIMATION	( PrepareResource_Animation )

#define	PREPARE_STATE	( PrepareResource_State )

#define	PREPARE_WAVE	( PrepareResource_Wave )

#define	STOP_TYPE_PLAY	( StopAll_Play )

#define	STOP_TYPE_MOVE	( StopAll_Move )

#define	STOP_TYPE_SPEAK	( StopAll_Speak )

#define	STOP_TYPE_PREPARE	( StopAll_QueuedPrepare )

#define	STOP_TYPE_NONQUEUEDPREPARE	( StopAll_ImmediatePrepare )

#define	STOP_TYPE_VISIBLE	( StopAll_Visibility )

#define	STOP_TYPE_ALL	( StopAll_Everything )

#define	BALLOON_STYLE_BALLOON_ON	( BalloonStyle_Enabled )

#define	BALLOON_STYLE_SIZETOTEXT	( BalloonStyle_SizeToText )

#define	BALLOON_STYLE_AUTOHIDE	( BalloonStyle_AutoHide )

#define	BALLOON_STYLE_AUTOPACE	( BalloonStyle_AutoPace )

#define	BALLOON_STYLE_PARTIAL_LINES	( BalloonStyle_ShowPartialLines )

#define	AUDIO_STATUS_AVAILABLE	( AudioStatus_Available )

#define	AUDIO_STATUS_NOAUDIO	( AudioStatus_Disabled )

#define	AUDIO_STATUS_CANTOPENAUDIO	( 2 )

#define	AUDIO_STATUS_USERSPEAKING	( AudioStatus_UserSpeaking )

#define	AUDIO_STATUS_CHARACTERSPEAKING	( AudioStatus_CharacterSpeaking )

#define	AUDIO_STATUS_SROVERRIDEABLE	( AudioStatus_CharacterListening )

#define	AUDIO_STATUS_ERROR	( AudioStatus_Error )

#define	LISTEN_STATUS_CANLISTEN	( ListeningStatus_Available )

#define	LISTEN_STATUS_NOAUDIO	( 1 )

#define	LISTEN_STATUS_NOTACTIVE	( ListeningStatus_CharacterInactive )

#define	LISTEN_STATUS_CANTOPENAUDIO	( 3 )

#define	LISTEN_STATUS_COULDNTINITIALIZESPEECH	( ListeningStatus_InitializeFailed )

#define	LISTEN_STATUS_SPEECHDISABLED	( ListeningStatus_InputDisabled )

#define	LISTEN_STATUS_ERROR	( ListeningStatus_Error )

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

#define	DISPID_IDaServer2_Character	( 0x60040000 )

#define	DISPID_IDaServer2_CharacterFiles	( 0x60040001 )

#define	DISPID_IDaServer2_CharacterStyle	( 0x60040002 )

#define	DISPID_IDaServer2_TTSEngines	( 0x60040003 )

#define	DISPID_IDaServer2_FindTTSEngines	( 0x60040004 )

#define	DISPID_IDaServer2_GetCharacterTTSEngine	( 0x60040005 )

#define	DISPID_IDaServer2_FindCharacterTTSEngines	( 0x60040006 )

#define	DISPID_IDaServer2_SREngines	( 0x60040007 )

#define	DISPID_IDaServer2_FindSREngines	( 0x60040008 )

#define	DISPID_IDaServer2_GetCharacterSREngine	( 0x60040009 )

#define	DISPID_IDaServer2_FindCharacterSREngines	( 0x6004000a )

#define	DISPID_IDaServer2_PropertySheet	( 0x6004000b )

#define	DISPID_IDaServer2_CommandsWindow	( 0x6004000c )

#define	DISPID_IDaServer2_Settings	( 0x6004000d )

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

#define	DISPID_IDaSvrCharacter2_Balloon	( 0x60040000 )

#define	DISPID_IDaSvrCharacter2_Commands	( 0x60040001 )

#define	DISPID_IDaSvrCharacter2_Style	( 0x60040002 )

#define	DISPID_IDaSvrCharacter2_HasIcon	( 0x60040003 )

#define	DISPID_IDaSvrCharacter2_GenerateIcon	( 0x60040004 )

#define	DISPID_IDaSvrCharacter2_IconVisible	( 0x60040005 )

#define	DISPID_IDaSvrCharacter2_GetIconIdentity	( 0x60040006 )

#define	DISPID_IDaSvrCharacter2_SetIconIdentity	( 0x60040007 )

#define	DISPID_IDaSvrCharacter2_IconIdentity	( 0x60040008 )

#define	DISPID_IDaSvrCharacter2_IconTip	( 0x60040009 )

#define	DISPID_IDaSvrCharacter2_TTSEngine	( 0x6004000a )

#define	DISPID_IDaSvrCharacter2_FindTTSEngines	( 0x6004000b )

#define	DISPID_IDaSvrCharacter2_SREngine	( 0x6004000c )

#define	DISPID_IDaSvrCharacter2_FindSREngines	( 0x6004000d )

#define	DISPID_IDaSvrCharacter2_CharacterID	( 0x60040010 )

#define	DISPID_IDaSvrCharacter2_GUID	( 0x60040011 )

#define	DISPID_IDaSvrCharacter2_Name	( 0x60040012 )

#define	DISPID_IDaSvrCharacter2_Description	( 0x60040013 )

#define	DISPID_IDaSvrCharacter2_ExtraData	( 0x60040014 )

#define	DISPID_IDaSvrCharacter2_FileName	( 0x60040015 )

#define	DISPID_IDaSvrCharacter2_FilePath	( 0x60040016 )

#define	DISPID_IDaSvrCharacter2_LanguageID	( 0x60040017 )

#define	DISPID_IDaSvrCharacter2_TTSModeID	( 0x60040018 )

#define	DISPID_IDaSvrCharacter2_SRModeID	( 0x60040019 )

#define	DISPID_IDaSvrCharacter2_Left	( 0x6004001a )

#define	DISPID_IDaSvrCharacter2_Top	( 0x6004001b )

#define	DISPID_IDaSvrCharacter2_Width	( 0x6004001c )

#define	DISPID_IDaSvrCharacter2_Height	( 0x6004001d )

#define	DISPID_IDaSvrCharacter2_OriginalWidth	( 0x6004001e )

#define	DISPID_IDaSvrCharacter2_OriginalHeight	( 0x6004001f )

#define	DISPID_IDaSvrCharacter2_Visible	( 0x60040020 )

#define	DISPID_IDaSvrCharacter2_ActiveState	( 0x60040021 )

#define	DISPID_IDaSvrCharacter2_IdleState	( 0x60040022 )

#define	DISPID_IDaSvrCharacter2_OtherClientCount	( 0x60040023 )

#define	DISPID_IDaSvrCharacter2_MoveCause	( 0x60040024 )

#define	DISPID_IDaSvrCharacter2_VisibilityCause	( 0x60040025 )

#define	DISPID_IDaSvrCharacter2_ListeningStatus	( 0x60040026 )

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

#define	DISPID_IDaSvrBalloon2_Enabled	( 1 )

#define	DISPID_IDaSvrBalloon2_Style	( 2 )

#define	DISPID_IDaSvrBalloon2_Visible	( 3 )

#define	DISPID_IDaSvrBalloon2_NumberOfLines	( 4 )

#define	DISPID_IDaSvrBalloon2_CharsPerLine	( 5 )

#define	DISPID_IDaSvrBalloon2_TextColor	( 6 )

#define	DISPID_IDaSvrBalloon2_BackColor	( 7 )

#define	DISPID_IDaSvrBalloon2_BorderColor	( 8 )

#define	DISPID_IDaSvrBalloon2_FontName	( 9 )

#define	DISPID_IDaSvrBalloon2_FontSize	( 10 )

#define	DISPID_IDaSvrBalloon2_FontBold	( 11 )

#define	DISPID_IDaSvrBalloon2_FontItalic	( 12 )

#define	DISPID_IDaSvrBalloon2_FontStrikethru	( 13 )

#define	DISPID_IDaSvrBalloon2_FontUnderline	( 14 )

#define	DISPID_IDaSvrBalloon2_FontCharSet	( 15 )

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

#define	DISPID_IDaSvrCommands2_Command	( 0x60040000 )

#define	DISPID_IDaSvrCommands2_Caption	( 0x60040001 )

#define	DISPID_IDaSvrCommands2_VoiceGrammar	( 0x60040002 )

#define	DISPID_IDaSvrCommands2_Visible	( 0x60040003 )

#define	DISPID_IDaSvrCommands2_DefaultCommand	( 0x60040004 )

#define	DISPID_IDaSvrCommands2_FontName	( 0x60040005 )

#define	DISPID_IDaSvrCommands2_FontSize	( 0x60040006 )

#define	DISPID_IDaSvrCommands2_VoiceCaption	( 0x60040007 )

#define	DISPID_IDaSvrCommands2_GlobalVoiceCommandsEnabled	( 0x60040008 )

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

#define	DISPID_IDaSvrCommand2_Caption	( DISPID_VALUE )

#define	DISPID_IDaSvrCommand2_Enabled	( 0x60030004 )

#define	DISPID_IDaSvrCommand2_Visible	( 0x60030005 )

#define	DISPID_IDaSvrCommand2_CommandID	( 0x60030006 )

#define	DISPID_IDaSvrCommand2_VoiceCaption	( 0x60030007 )

#define	DISPID_IDaSvrCommand2_VoiceGrammar	( 0x60030008 )

#define	DISPID_IDaSvrCommand2_ConfidenceThreshold	( 0x60030009 )

#define	DISPID_IDaSvrCommand2_ConfidenceText	( 0x6003000a )

#define	DISPID_IAgentUserInput_GetCount	( 0x60020000 )

#define	DISPID_IAgentUserInput_GetItemID	( 0x60020001 )

#define	DISPID_IAgentUserInput_GetItemConfidence	( 0x60020002 )

#define	DISPID_IAgentUserInput_GetItemText	( 0x60020003 )

#define	DISPID_IAgentUserInput_GetAllItemData	( 0x60020004 )

#define	DISPID_IDaSvrUserInput2_Count	( 0x60030000 )

#define	DISPID_IDaSvrUserInput2_ItemCommandID	( 0x60030001 )

#define	DISPID_IDaSvrUserInput2_ItemConfidence	( 0x60030002 )

#define	DISPID_IDaSvrUserInput2_ItemText	( 0x60030003 )

#define	DISPID_IAgentCommandWindow_SetVisible	( 0x60020000 )

#define	DISPID_IAgentCommandWindow_GetVisible	( 0x60020001 )

#define	DISPID_IAgentCommandWindow_GetPosition	( 0x60020002 )

#define	DISPID_IAgentCommandWindow_GetSize	( 0x60020003 )

#define	DISPID_IDaSvrCommandsWindow2_Visible	( 0x60030001 )

#define	DISPID_IDaSvrCommandsWindow2_Left	( 0x60030002 )

#define	DISPID_IDaSvrCommandsWindow2_Top	( 0x60030003 )

#define	DISPID_IDaSvrCommandsWindow2_Height	( 0x60030004 )

#define	DISPID_IDaSvrCommandsWindow2_Width	( 0x60030005 )

#define	DISPID_IAgentPropertySheet_GetVisible	( 0x60020000 )

#define	DISPID_IAgentPropertySheet_SetVisible	( 0x60020001 )

#define	DISPID_IAgentPropertySheet_GetPosition	( 0x60020002 )

#define	DISPID_IAgentPropertySheet_GetSize	( 0x60020003 )

#define	DISPID_IAgentPropertySheet_GetPage	( 0x60020004 )

#define	DISPID_IAgentPropertySheet_SetPage	( 0x60020005 )

#define	DISPID_IDaSvrPropertySheet2_Left	( 1 )

#define	DISPID_IDaSvrPropertySheet2_Top	( 2 )

#define	DISPID_IDaSvrPropertySheet2_Height	( 3 )

#define	DISPID_IDaSvrPropertySheet2_Width	( 4 )

#define	DISPID_IDaSvrPropertySheet2_Page	( 5 )

#define	DISPID_IDaSvrPropertySheet2_Visible	( 6 )

#define	DISPID_IAgentAudioOutputProperties_GetEnabled	( 0x60020000 )

#define	DISPID_IAgentAudioOutputProperties_GetUsingSoundEffects	( 0x60020001 )

#define	DISPID_IAgentAudioOutputPropertiesEx_GetStatus	( 0x60030000 )

#define	DISPID_IAgentSpeechInputProperties_GetInstalled	( 0x60020000 )

#define	DISPID_IAgentSpeechInputProperties_GetEnabled	( 0x60020001 )

#define	DISPID_IAgentSpeechInputProperties_GetHotKey	( 0x60020002 )

#define	DISPID_IAgentSpeechInputProperties_GetLCID	( 0x60020003 )

#define	DISPID_IAgentSpeechInputProperties_GetEngine	( 0x60020004 )

#define	DISPID_IAgentSpeechInputProperties_SetEngine	( 0x60020005 )

#define	DISPID_IAgentSpeechInputProperties_GetListeningTip	( 0x60020006 )

#define	DISPID_IDaSvrSettings_SoundEffectsEnabled	( 1 )

#define	DISPID_IDaSvrSettings_BalloonEnabled	( 2 )

#define	DISPID_IDaSvrSettings_BalloonFont	( 3 )

#define	DISPID_IDaSvrSettings_TTSEnabled	( 4 )

#define	DISPID_IDaSvrSettings_TTSSpeed	( 5 )

#define	DISPID_IDaSvrSettings_SREnabled	( 6 )

#define	DISPID_IDaSvrSettings_SRHotKey	( 7 )

#define	DISPID_IDaSvrSettings_SRHotKeyTime	( 8 )

#define	DISPID_IDaSvrSettings_SRListeningTip	( 9 )

#define	DISPID_IDaSvrSettings_SRListeningPrompt	( 10 )

#define	DISPID_IDaSvrSettings_AudioStatus	( 11 )

#define	DISPID_IDaSvrCharacterFiles_FilePaths	( DISPID_VALUE )

#define	DISPID_IDaSvrCharacterFiles_SearchPath	( 0x60020000 )

#define	DISPID_IDaSvrCharacterFiles_DefaultSearchPath	( 0x60020001 )

#define	DISPID_IDaSvrCharacterFiles_Filter	( 0x60020003 )

#define	DISPID_IDaSvrCharacterFiles_DefaultFilePath	( 0x60020004 )

#define	DISPID_IDaSvrCharacterFiles_DefaultFileName	( 0x60020005 )

#define	DISPID_IDaSvrTTSEngine_TTSModeID	( DISPID_VALUE )

#define	DISPID_IDaSvrTTSEngine_DisplayName	( 0x60020001 )

#define	DISPID_IDaSvrTTSEngine_Manufacturer	( 0x60020002 )

#define	DISPID_IDaSvrTTSEngine_GetVersion	( 0x60020003 )

#define	DISPID_IDaSvrTTSEngine_Gender	( 0x60020004 )

#define	DISPID_IDaSvrTTSEngine_LanguageID	( 0x60020005 )

#define	DISPID_IDaSvrTTSEngine_LanguageName	( 0x60020006 )

#define	DISPID_IDaSvrSREngine_SRModeID	( DISPID_VALUE )

#define	DISPID_IDaSvrSREngine_DisplayName	( 0x60020001 )

#define	DISPID_IDaSvrSREngine_Manufacturer	( 0x60020002 )

#define	DISPID_IDaSvrSREngine_GetVersion	( 0x60020003 )

#define	DISPID_IDaSvrSREngine_LanguageID	( 0x60020004 )

#define	DISPID_IDaSvrSREngine_LanguageName	( 0x60020005 )

#define	DISPID_IDaSvrSREngine_LanguageIDs	( 0x60020006 )

#define	DISPID_IDaSvrSREngine_LanguageNames	( 0x60020007 )


EXTERN_C const IID LIBID_DoubleAgentSvr;

#ifndef __IDaServer_INTERFACE_DEFINED__
#define __IDaServer_INTERFACE_DEFINED__

/* interface IDaServer */
/* [object][hidden][nonextensible][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IDaServer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E502-A208-11DE-ABF2-002421116FB2")
    IDaServer : public IDispatch
    {
    public:
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE Load( 
            /* [in] */ VARIANT LoadKey,
            /* [out] */ long *CharacterID,
            /* [out] */ long *RequestID) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE Unload( 
            /* [in] */ long CharacterID) = 0;
        
        virtual /* [restricted][id][helpstring] */ HRESULT STDMETHODCALLTYPE Register( 
            /* [in] */ IUnknown *NotifySink,
            /* [out] */ long *Cookie) = 0;
        
        virtual /* [restricted][id][helpstring] */ HRESULT STDMETHODCALLTYPE Unregister( 
            /* [in] */ long Cookie) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetCharacter( 
            /* [in] */ long CharacterID,
            /* [out] */ IDispatch **Character) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetSuspended( 
            /* [out] */ long *Suspended) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetCharacterEx( 
            /* [in] */ long CharacterID,
            /* [out] */ IDaSvrCharacter **Character) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE GetVersion( 
            /* [out] */ short *MajorVersion,
            /* [out] */ short *MinorVersion) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE ShowDefaultCharacterProperties( 
            /* [in] */ short x,
            /* [in] */ short y,
            /* [in] */ long UseDefaultPosition) = 0;
        
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
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Load )( 
            IDaServer * This,
            /* [in] */ VARIANT LoadKey,
            /* [out] */ long *CharacterID,
            /* [out] */ long *RequestID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Unload )( 
            IDaServer * This,
            /* [in] */ long CharacterID);
        
        /* [restricted][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Register )( 
            IDaServer * This,
            /* [in] */ IUnknown *NotifySink,
            /* [out] */ long *Cookie);
        
        /* [restricted][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Unregister )( 
            IDaServer * This,
            /* [in] */ long Cookie);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetCharacter )( 
            IDaServer * This,
            /* [in] */ long CharacterID,
            /* [out] */ IDispatch **Character);
        
        /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetSuspended )( 
            IDaServer * This,
            /* [out] */ long *Suspended);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetCharacterEx )( 
            IDaServer * This,
            /* [in] */ long CharacterID,
            /* [out] */ IDaSvrCharacter **Character);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVersion )( 
            IDaServer * This,
            /* [out] */ short *MajorVersion,
            /* [out] */ short *MinorVersion);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *ShowDefaultCharacterProperties )( 
            IDaServer * This,
            /* [in] */ short x,
            /* [in] */ short y,
            /* [in] */ long UseDefaultPosition);
        
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


#define IDaServer_Load(This,LoadKey,CharacterID,RequestID)	\
    ( (This)->lpVtbl -> Load(This,LoadKey,CharacterID,RequestID) ) 

#define IDaServer_Unload(This,CharacterID)	\
    ( (This)->lpVtbl -> Unload(This,CharacterID) ) 

#define IDaServer_Register(This,NotifySink,Cookie)	\
    ( (This)->lpVtbl -> Register(This,NotifySink,Cookie) ) 

#define IDaServer_Unregister(This,Cookie)	\
    ( (This)->lpVtbl -> Unregister(This,Cookie) ) 

#define IDaServer_GetCharacter(This,CharacterID,Character)	\
    ( (This)->lpVtbl -> GetCharacter(This,CharacterID,Character) ) 

#define IDaServer_GetSuspended(This,Suspended)	\
    ( (This)->lpVtbl -> GetSuspended(This,Suspended) ) 

#define IDaServer_GetCharacterEx(This,CharacterID,Character)	\
    ( (This)->lpVtbl -> GetCharacterEx(This,CharacterID,Character) ) 

#define IDaServer_GetVersion(This,MajorVersion,MinorVersion)	\
    ( (This)->lpVtbl -> GetVersion(This,MajorVersion,MinorVersion) ) 

#define IDaServer_ShowDefaultCharacterProperties(This,x,y,UseDefaultPosition)	\
    ( (This)->lpVtbl -> ShowDefaultCharacterProperties(This,x,y,UseDefaultPosition) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaServer_INTERFACE_DEFINED__ */


#ifndef __IDaSvrCommand_INTERFACE_DEFINED__
#define __IDaSvrCommand_INTERFACE_DEFINED__

/* interface IDaSvrCommand */
/* [object][hidden][nonextensible][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IDaSvrCommand;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E506-A208-11DE-ABF2-002421116FB2")
    IDaSvrCommand : public IDispatch
    {
    public:
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetCaption( 
            /* [in] */ BSTR Caption) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetCaption( 
            /* [out] */ BSTR *Caption) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetVoice( 
            /* [in] */ BSTR Voice) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetVoice( 
            /* [out] */ BSTR *Voice) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetEnabled( 
            /* [in] */ long Enabled) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetEnabled( 
            /* [out] */ long *Enabled) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetVisible( 
            /* [in] */ long Visible) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetVisible( 
            /* [out] */ long *Visible) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetConfidenceThreshold( 
            /* [in] */ long Threshold) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetConfidenceThreshold( 
            /* [out] */ long *Threshold) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetConfidenceText( 
            /* [in] */ BSTR TipText) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetConfidenceText( 
            /* [out] */ BSTR *TipText) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetID( 
            /* [out] */ long *CommandID) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetHelpContextID( 
            /* [in] */ long ContextID) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetHelpContextID( 
            /* [out] */ long *ContextID) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetVoiceCaption( 
            /* [in] */ BSTR VoiceCaption) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetVoiceCaption( 
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
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetCaption )( 
            IDaSvrCommand * This,
            /* [in] */ BSTR Caption);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetCaption )( 
            IDaSvrCommand * This,
            /* [out] */ BSTR *Caption);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetVoice )( 
            IDaSvrCommand * This,
            /* [in] */ BSTR Voice);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVoice )( 
            IDaSvrCommand * This,
            /* [out] */ BSTR *Voice);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetEnabled )( 
            IDaSvrCommand * This,
            /* [in] */ long Enabled);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEnabled )( 
            IDaSvrCommand * This,
            /* [out] */ long *Enabled);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetVisible )( 
            IDaSvrCommand * This,
            /* [in] */ long Visible);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVisible )( 
            IDaSvrCommand * This,
            /* [out] */ long *Visible);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetConfidenceThreshold )( 
            IDaSvrCommand * This,
            /* [in] */ long Threshold);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetConfidenceThreshold )( 
            IDaSvrCommand * This,
            /* [out] */ long *Threshold);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetConfidenceText )( 
            IDaSvrCommand * This,
            /* [in] */ BSTR TipText);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetConfidenceText )( 
            IDaSvrCommand * This,
            /* [out] */ BSTR *TipText);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetID )( 
            IDaSvrCommand * This,
            /* [out] */ long *CommandID);
        
        /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetHelpContextID )( 
            IDaSvrCommand * This,
            /* [in] */ long ContextID);
        
        /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetHelpContextID )( 
            IDaSvrCommand * This,
            /* [out] */ long *ContextID);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetVoiceCaption )( 
            IDaSvrCommand * This,
            /* [in] */ BSTR VoiceCaption);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVoiceCaption )( 
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


#define IDaSvrCommand_SetCaption(This,Caption)	\
    ( (This)->lpVtbl -> SetCaption(This,Caption) ) 

#define IDaSvrCommand_GetCaption(This,Caption)	\
    ( (This)->lpVtbl -> GetCaption(This,Caption) ) 

#define IDaSvrCommand_SetVoice(This,Voice)	\
    ( (This)->lpVtbl -> SetVoice(This,Voice) ) 

#define IDaSvrCommand_GetVoice(This,Voice)	\
    ( (This)->lpVtbl -> GetVoice(This,Voice) ) 

#define IDaSvrCommand_SetEnabled(This,Enabled)	\
    ( (This)->lpVtbl -> SetEnabled(This,Enabled) ) 

#define IDaSvrCommand_GetEnabled(This,Enabled)	\
    ( (This)->lpVtbl -> GetEnabled(This,Enabled) ) 

#define IDaSvrCommand_SetVisible(This,Visible)	\
    ( (This)->lpVtbl -> SetVisible(This,Visible) ) 

#define IDaSvrCommand_GetVisible(This,Visible)	\
    ( (This)->lpVtbl -> GetVisible(This,Visible) ) 

#define IDaSvrCommand_SetConfidenceThreshold(This,Threshold)	\
    ( (This)->lpVtbl -> SetConfidenceThreshold(This,Threshold) ) 

#define IDaSvrCommand_GetConfidenceThreshold(This,Threshold)	\
    ( (This)->lpVtbl -> GetConfidenceThreshold(This,Threshold) ) 

#define IDaSvrCommand_SetConfidenceText(This,TipText)	\
    ( (This)->lpVtbl -> SetConfidenceText(This,TipText) ) 

#define IDaSvrCommand_GetConfidenceText(This,TipText)	\
    ( (This)->lpVtbl -> GetConfidenceText(This,TipText) ) 

#define IDaSvrCommand_GetID(This,CommandID)	\
    ( (This)->lpVtbl -> GetID(This,CommandID) ) 

#define IDaSvrCommand_SetHelpContextID(This,ContextID)	\
    ( (This)->lpVtbl -> SetHelpContextID(This,ContextID) ) 

#define IDaSvrCommand_GetHelpContextID(This,ContextID)	\
    ( (This)->lpVtbl -> GetHelpContextID(This,ContextID) ) 

#define IDaSvrCommand_SetVoiceCaption(This,VoiceCaption)	\
    ( (This)->lpVtbl -> SetVoiceCaption(This,VoiceCaption) ) 

#define IDaSvrCommand_GetVoiceCaption(This,pbszVoiceCaption)	\
    ( (This)->lpVtbl -> GetVoiceCaption(This,pbszVoiceCaption) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrCommand_INTERFACE_DEFINED__ */


#ifndef __IDaSvrCommands_INTERFACE_DEFINED__
#define __IDaSvrCommands_INTERFACE_DEFINED__

/* interface IDaSvrCommands */
/* [object][hidden][nonextensible][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IDaSvrCommands;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E505-A208-11DE-ABF2-002421116FB2")
    IDaSvrCommands : public IDispatch
    {
    public:
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetCommand( 
            /* [in] */ long CommandID,
            /* [out] */ IUnknown **Command) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetCount( 
            /* [out] */ long *Count) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetCaption( 
            /* [in] */ BSTR Caption) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetCaption( 
            /* [out] */ BSTR *Caption) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetVoice( 
            /* [in] */ BSTR Voice) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetVoice( 
            /* [out] */ BSTR *Voice) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetVisible( 
            /* [in] */ long Visible) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetVisible( 
            /* [out] */ long *Visible) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ BSTR Caption,
            /* [in] */ BSTR VoiceGrammar,
            /* [in] */ long Enabled,
            /* [in] */ long Visible,
            /* [out] */ long *CommandID) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE Insert( 
            /* [in] */ BSTR Caption,
            /* [in] */ BSTR VoiceGrammar,
            /* [in] */ long Enabled,
            /* [in] */ long Visible,
            /* [in] */ long RefCommandID,
            /* [in] */ long Before,
            /* [out] */ long *CommandID) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE Remove( 
            /* [in] */ long CommandID) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE RemoveAll( void) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetCommandEx( 
            /* [in] */ long CommandID,
            /* [out] */ IDaSvrCommand **Command) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetDefaultID( 
            /* [in] */ long CommandID) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetDefaultID( 
            /* [out] */ long *CommandID) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetHelpContextID( 
            /* [in] */ long HelpContextID) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetHelpContextID( 
            /* [out] */ long *pulHelpID) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetFontName( 
            /* [in] */ BSTR FontName) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetFontName( 
            /* [out] */ BSTR *FontName) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetFontSize( 
            /* [in] */ long FontSize) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetFontSize( 
            /* [out] */ long *FontSize) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetVoiceCaption( 
            /* [in] */ BSTR VoiceCaption) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetVoiceCaption( 
            /* [out] */ BSTR *VoiceCaption) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE AddEx( 
            /* [in] */ BSTR Caption,
            /* [in] */ BSTR VoiceGrammar,
            /* [in] */ BSTR VoiceCaption,
            /* [in] */ long Enabled,
            /* [in] */ long Visible,
            /* [in] */ long HelpContextID,
            /* [out] */ long *CommandID) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE InsertEx( 
            /* [in] */ BSTR Caption,
            /* [in] */ BSTR VoiceGrammar,
            /* [in] */ BSTR VoiceCaption,
            /* [in] */ long Enabled,
            /* [in] */ long Visible,
            /* [in] */ long HelpContextID,
            /* [in] */ long RefCommandID,
            /* [in] */ long Before,
            /* [out] */ long *CommandID) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetGlobalVoiceCommandsEnabled( 
            /* [in] */ long Enabled) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetGlobalVoiceCommandsEnabled( 
            /* [out] */ long *Enabled) = 0;
        
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
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetCommand )( 
            IDaSvrCommands * This,
            /* [in] */ long CommandID,
            /* [out] */ IUnknown **Command);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IDaSvrCommands * This,
            /* [out] */ long *Count);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetCaption )( 
            IDaSvrCommands * This,
            /* [in] */ BSTR Caption);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetCaption )( 
            IDaSvrCommands * This,
            /* [out] */ BSTR *Caption);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetVoice )( 
            IDaSvrCommands * This,
            /* [in] */ BSTR Voice);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVoice )( 
            IDaSvrCommands * This,
            /* [out] */ BSTR *Voice);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetVisible )( 
            IDaSvrCommands * This,
            /* [in] */ long Visible);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVisible )( 
            IDaSvrCommands * This,
            /* [out] */ long *Visible);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            IDaSvrCommands * This,
            /* [in] */ BSTR Caption,
            /* [in] */ BSTR VoiceGrammar,
            /* [in] */ long Enabled,
            /* [in] */ long Visible,
            /* [out] */ long *CommandID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Insert )( 
            IDaSvrCommands * This,
            /* [in] */ BSTR Caption,
            /* [in] */ BSTR VoiceGrammar,
            /* [in] */ long Enabled,
            /* [in] */ long Visible,
            /* [in] */ long RefCommandID,
            /* [in] */ long Before,
            /* [out] */ long *CommandID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Remove )( 
            IDaSvrCommands * This,
            /* [in] */ long CommandID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *RemoveAll )( 
            IDaSvrCommands * This);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetCommandEx )( 
            IDaSvrCommands * This,
            /* [in] */ long CommandID,
            /* [out] */ IDaSvrCommand **Command);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetDefaultID )( 
            IDaSvrCommands * This,
            /* [in] */ long CommandID);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetDefaultID )( 
            IDaSvrCommands * This,
            /* [out] */ long *CommandID);
        
        /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetHelpContextID )( 
            IDaSvrCommands * This,
            /* [in] */ long HelpContextID);
        
        /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetHelpContextID )( 
            IDaSvrCommands * This,
            /* [out] */ long *pulHelpID);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetFontName )( 
            IDaSvrCommands * This,
            /* [in] */ BSTR FontName);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetFontName )( 
            IDaSvrCommands * This,
            /* [out] */ BSTR *FontName);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetFontSize )( 
            IDaSvrCommands * This,
            /* [in] */ long FontSize);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetFontSize )( 
            IDaSvrCommands * This,
            /* [out] */ long *FontSize);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetVoiceCaption )( 
            IDaSvrCommands * This,
            /* [in] */ BSTR VoiceCaption);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVoiceCaption )( 
            IDaSvrCommands * This,
            /* [out] */ BSTR *VoiceCaption);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *AddEx )( 
            IDaSvrCommands * This,
            /* [in] */ BSTR Caption,
            /* [in] */ BSTR VoiceGrammar,
            /* [in] */ BSTR VoiceCaption,
            /* [in] */ long Enabled,
            /* [in] */ long Visible,
            /* [in] */ long HelpContextID,
            /* [out] */ long *CommandID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *InsertEx )( 
            IDaSvrCommands * This,
            /* [in] */ BSTR Caption,
            /* [in] */ BSTR VoiceGrammar,
            /* [in] */ BSTR VoiceCaption,
            /* [in] */ long Enabled,
            /* [in] */ long Visible,
            /* [in] */ long HelpContextID,
            /* [in] */ long RefCommandID,
            /* [in] */ long Before,
            /* [out] */ long *CommandID);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetGlobalVoiceCommandsEnabled )( 
            IDaSvrCommands * This,
            /* [in] */ long Enabled);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetGlobalVoiceCommandsEnabled )( 
            IDaSvrCommands * This,
            /* [out] */ long *Enabled);
        
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


#define IDaSvrCommands_GetCommand(This,CommandID,Command)	\
    ( (This)->lpVtbl -> GetCommand(This,CommandID,Command) ) 

#define IDaSvrCommands_GetCount(This,Count)	\
    ( (This)->lpVtbl -> GetCount(This,Count) ) 

#define IDaSvrCommands_SetCaption(This,Caption)	\
    ( (This)->lpVtbl -> SetCaption(This,Caption) ) 

#define IDaSvrCommands_GetCaption(This,Caption)	\
    ( (This)->lpVtbl -> GetCaption(This,Caption) ) 

#define IDaSvrCommands_SetVoice(This,Voice)	\
    ( (This)->lpVtbl -> SetVoice(This,Voice) ) 

#define IDaSvrCommands_GetVoice(This,Voice)	\
    ( (This)->lpVtbl -> GetVoice(This,Voice) ) 

#define IDaSvrCommands_SetVisible(This,Visible)	\
    ( (This)->lpVtbl -> SetVisible(This,Visible) ) 

#define IDaSvrCommands_GetVisible(This,Visible)	\
    ( (This)->lpVtbl -> GetVisible(This,Visible) ) 

#define IDaSvrCommands_Add(This,Caption,VoiceGrammar,Enabled,Visible,CommandID)	\
    ( (This)->lpVtbl -> Add(This,Caption,VoiceGrammar,Enabled,Visible,CommandID) ) 

#define IDaSvrCommands_Insert(This,Caption,VoiceGrammar,Enabled,Visible,RefCommandID,Before,CommandID)	\
    ( (This)->lpVtbl -> Insert(This,Caption,VoiceGrammar,Enabled,Visible,RefCommandID,Before,CommandID) ) 

#define IDaSvrCommands_Remove(This,CommandID)	\
    ( (This)->lpVtbl -> Remove(This,CommandID) ) 

#define IDaSvrCommands_RemoveAll(This)	\
    ( (This)->lpVtbl -> RemoveAll(This) ) 

#define IDaSvrCommands_GetCommandEx(This,CommandID,Command)	\
    ( (This)->lpVtbl -> GetCommandEx(This,CommandID,Command) ) 

#define IDaSvrCommands_SetDefaultID(This,CommandID)	\
    ( (This)->lpVtbl -> SetDefaultID(This,CommandID) ) 

#define IDaSvrCommands_GetDefaultID(This,CommandID)	\
    ( (This)->lpVtbl -> GetDefaultID(This,CommandID) ) 

#define IDaSvrCommands_SetHelpContextID(This,HelpContextID)	\
    ( (This)->lpVtbl -> SetHelpContextID(This,HelpContextID) ) 

#define IDaSvrCommands_GetHelpContextID(This,pulHelpID)	\
    ( (This)->lpVtbl -> GetHelpContextID(This,pulHelpID) ) 

#define IDaSvrCommands_SetFontName(This,FontName)	\
    ( (This)->lpVtbl -> SetFontName(This,FontName) ) 

#define IDaSvrCommands_GetFontName(This,FontName)	\
    ( (This)->lpVtbl -> GetFontName(This,FontName) ) 

#define IDaSvrCommands_SetFontSize(This,FontSize)	\
    ( (This)->lpVtbl -> SetFontSize(This,FontSize) ) 

#define IDaSvrCommands_GetFontSize(This,FontSize)	\
    ( (This)->lpVtbl -> GetFontSize(This,FontSize) ) 

#define IDaSvrCommands_SetVoiceCaption(This,VoiceCaption)	\
    ( (This)->lpVtbl -> SetVoiceCaption(This,VoiceCaption) ) 

#define IDaSvrCommands_GetVoiceCaption(This,VoiceCaption)	\
    ( (This)->lpVtbl -> GetVoiceCaption(This,VoiceCaption) ) 

#define IDaSvrCommands_AddEx(This,Caption,VoiceGrammar,VoiceCaption,Enabled,Visible,HelpContextID,CommandID)	\
    ( (This)->lpVtbl -> AddEx(This,Caption,VoiceGrammar,VoiceCaption,Enabled,Visible,HelpContextID,CommandID) ) 

#define IDaSvrCommands_InsertEx(This,Caption,VoiceGrammar,VoiceCaption,Enabled,Visible,HelpContextID,RefCommandID,Before,CommandID)	\
    ( (This)->lpVtbl -> InsertEx(This,Caption,VoiceGrammar,VoiceCaption,Enabled,Visible,HelpContextID,RefCommandID,Before,CommandID) ) 

#define IDaSvrCommands_SetGlobalVoiceCommandsEnabled(This,Enabled)	\
    ( (This)->lpVtbl -> SetGlobalVoiceCommandsEnabled(This,Enabled) ) 

#define IDaSvrCommands_GetGlobalVoiceCommandsEnabled(This,Enabled)	\
    ( (This)->lpVtbl -> GetGlobalVoiceCommandsEnabled(This,Enabled) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrCommands_INTERFACE_DEFINED__ */


#ifndef __IDaSvrUserInput_INTERFACE_DEFINED__
#define __IDaSvrUserInput_INTERFACE_DEFINED__

/* interface IDaSvrUserInput */
/* [object][hidden][nonextensible][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IDaSvrUserInput;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E508-A208-11DE-ABF2-002421116FB2")
    IDaSvrUserInput : public IDispatch
    {
    public:
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetCount( 
            /* [out] */ long *Count) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetItemID( 
            /* [in] */ long ItemIndex,
            /* [out] */ long *CommandID) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetItemConfidence( 
            /* [in] */ long ItemIndex,
            /* [out] */ long *Confidence) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetItemText( 
            /* [in] */ long ItemIndex,
            /* [out] */ BSTR *Text) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE GetAllItemData( 
            /* [out] */ VARIANT *ItemIndices,
            /* [out] */ VARIANT *ItemConfidences,
            /* [out] */ VARIANT *ItemText) = 0;
        
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
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IDaSvrUserInput * This,
            /* [out] */ long *Count);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetItemID )( 
            IDaSvrUserInput * This,
            /* [in] */ long ItemIndex,
            /* [out] */ long *CommandID);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetItemConfidence )( 
            IDaSvrUserInput * This,
            /* [in] */ long ItemIndex,
            /* [out] */ long *Confidence);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetItemText )( 
            IDaSvrUserInput * This,
            /* [in] */ long ItemIndex,
            /* [out] */ BSTR *Text);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetAllItemData )( 
            IDaSvrUserInput * This,
            /* [out] */ VARIANT *ItemIndices,
            /* [out] */ VARIANT *ItemConfidences,
            /* [out] */ VARIANT *ItemText);
        
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


#define IDaSvrUserInput_GetCount(This,Count)	\
    ( (This)->lpVtbl -> GetCount(This,Count) ) 

#define IDaSvrUserInput_GetItemID(This,ItemIndex,CommandID)	\
    ( (This)->lpVtbl -> GetItemID(This,ItemIndex,CommandID) ) 

#define IDaSvrUserInput_GetItemConfidence(This,ItemIndex,Confidence)	\
    ( (This)->lpVtbl -> GetItemConfidence(This,ItemIndex,Confidence) ) 

#define IDaSvrUserInput_GetItemText(This,ItemIndex,Text)	\
    ( (This)->lpVtbl -> GetItemText(This,ItemIndex,Text) ) 

#define IDaSvrUserInput_GetAllItemData(This,ItemIndices,ItemConfidences,ItemText)	\
    ( (This)->lpVtbl -> GetAllItemData(This,ItemIndices,ItemConfidences,ItemText) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrUserInput_INTERFACE_DEFINED__ */


#ifndef __IDaSvrSpeechInput_INTERFACE_DEFINED__
#define __IDaSvrSpeechInput_INTERFACE_DEFINED__

/* interface IDaSvrSpeechInput */
/* [object][hidden][nonextensible][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrSpeechInput;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E50C-A208-11DE-ABF2-002421116FB2")
    IDaSvrSpeechInput : public IDispatch
    {
    public:
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetInstalled( 
            /* [out] */ long *Installed) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE GetEnabled( 
            /* [out] */ long *Enabled) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE GetHotKey( 
            /* [out] */ BSTR *HotKey) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetLCID( 
            /* [out] */ unsigned long *LCIDCurrent) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetEngine( 
            /* [out] */ BSTR *Engine) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetEngine( 
            /* [in] */ BSTR Engine) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE GetListeningTip( 
            /* [out] */ long *ListeningTip) = 0;
        
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
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetInstalled )( 
            IDaSvrSpeechInput * This,
            /* [out] */ long *Installed);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEnabled )( 
            IDaSvrSpeechInput * This,
            /* [out] */ long *Enabled);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetHotKey )( 
            IDaSvrSpeechInput * This,
            /* [out] */ BSTR *HotKey);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetLCID )( 
            IDaSvrSpeechInput * This,
            /* [out] */ unsigned long *LCIDCurrent);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEngine )( 
            IDaSvrSpeechInput * This,
            /* [out] */ BSTR *Engine);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetEngine )( 
            IDaSvrSpeechInput * This,
            /* [in] */ BSTR Engine);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetListeningTip )( 
            IDaSvrSpeechInput * This,
            /* [out] */ long *ListeningTip);
        
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


#define IDaSvrSpeechInput_GetInstalled(This,Installed)	\
    ( (This)->lpVtbl -> GetInstalled(This,Installed) ) 

#define IDaSvrSpeechInput_GetEnabled(This,Enabled)	\
    ( (This)->lpVtbl -> GetEnabled(This,Enabled) ) 

#define IDaSvrSpeechInput_GetHotKey(This,HotKey)	\
    ( (This)->lpVtbl -> GetHotKey(This,HotKey) ) 

#define IDaSvrSpeechInput_GetLCID(This,LCIDCurrent)	\
    ( (This)->lpVtbl -> GetLCID(This,LCIDCurrent) ) 

#define IDaSvrSpeechInput_GetEngine(This,Engine)	\
    ( (This)->lpVtbl -> GetEngine(This,Engine) ) 

#define IDaSvrSpeechInput_SetEngine(This,Engine)	\
    ( (This)->lpVtbl -> SetEngine(This,Engine) ) 

#define IDaSvrSpeechInput_GetListeningTip(This,ListeningTip)	\
    ( (This)->lpVtbl -> GetListeningTip(This,ListeningTip) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrSpeechInput_INTERFACE_DEFINED__ */


#ifndef __IDaSvrAudioOutput_INTERFACE_DEFINED__
#define __IDaSvrAudioOutput_INTERFACE_DEFINED__

/* interface IDaSvrAudioOutput */
/* [object][hidden][nonextensible][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IDaSvrAudioOutput;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E50B-A208-11DE-ABF2-002421116FB2")
    IDaSvrAudioOutput : public IDispatch
    {
    public:
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE GetEnabled( 
            /* [out] */ long *Enabled) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE GetUsingSoundEffects( 
            /* [out] */ long *UsingSoundEffects) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetStatus( 
            /* [out] */ long *Status) = 0;
        
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
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEnabled )( 
            IDaSvrAudioOutput * This,
            /* [out] */ long *Enabled);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetUsingSoundEffects )( 
            IDaSvrAudioOutput * This,
            /* [out] */ long *UsingSoundEffects);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetStatus )( 
            IDaSvrAudioOutput * This,
            /* [out] */ long *Status);
        
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


#define IDaSvrAudioOutput_GetEnabled(This,Enabled)	\
    ( (This)->lpVtbl -> GetEnabled(This,Enabled) ) 

#define IDaSvrAudioOutput_GetUsingSoundEffects(This,UsingSoundEffects)	\
    ( (This)->lpVtbl -> GetUsingSoundEffects(This,UsingSoundEffects) ) 

#define IDaSvrAudioOutput_GetStatus(This,Status)	\
    ( (This)->lpVtbl -> GetStatus(This,Status) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrAudioOutput_INTERFACE_DEFINED__ */


#ifndef __IDaSvrPropertySheet_INTERFACE_DEFINED__
#define __IDaSvrPropertySheet_INTERFACE_DEFINED__

/* interface IDaSvrPropertySheet */
/* [object][hidden][nonextensible][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IDaSvrPropertySheet;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E50A-A208-11DE-ABF2-002421116FB2")
    IDaSvrPropertySheet : public IDispatch
    {
    public:
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetVisible( 
            /* [out] */ long *Visible) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetVisible( 
            /* [in] */ long Visible) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE GetPosition( 
            /* [out] */ long *Left,
            /* [out] */ long *Top) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE GetSize( 
            /* [out] */ long *Width,
            /* [out] */ long *Height) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetPage( 
            /* [out] */ BSTR *Page) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetPage( 
            /* [in] */ BSTR Page) = 0;
        
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
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVisible )( 
            IDaSvrPropertySheet * This,
            /* [out] */ long *Visible);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetVisible )( 
            IDaSvrPropertySheet * This,
            /* [in] */ long Visible);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetPosition )( 
            IDaSvrPropertySheet * This,
            /* [out] */ long *Left,
            /* [out] */ long *Top);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetSize )( 
            IDaSvrPropertySheet * This,
            /* [out] */ long *Width,
            /* [out] */ long *Height);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetPage )( 
            IDaSvrPropertySheet * This,
            /* [out] */ BSTR *Page);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetPage )( 
            IDaSvrPropertySheet * This,
            /* [in] */ BSTR Page);
        
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


#define IDaSvrPropertySheet_GetVisible(This,Visible)	\
    ( (This)->lpVtbl -> GetVisible(This,Visible) ) 

#define IDaSvrPropertySheet_SetVisible(This,Visible)	\
    ( (This)->lpVtbl -> SetVisible(This,Visible) ) 

#define IDaSvrPropertySheet_GetPosition(This,Left,Top)	\
    ( (This)->lpVtbl -> GetPosition(This,Left,Top) ) 

#define IDaSvrPropertySheet_GetSize(This,Width,Height)	\
    ( (This)->lpVtbl -> GetSize(This,Width,Height) ) 

#define IDaSvrPropertySheet_GetPage(This,Page)	\
    ( (This)->lpVtbl -> GetPage(This,Page) ) 

#define IDaSvrPropertySheet_SetPage(This,Page)	\
    ( (This)->lpVtbl -> SetPage(This,Page) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrPropertySheet_INTERFACE_DEFINED__ */


#ifndef __IDaSvrBalloon_INTERFACE_DEFINED__
#define __IDaSvrBalloon_INTERFACE_DEFINED__

/* interface IDaSvrBalloon */
/* [object][hidden][nonextensible][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IDaSvrBalloon;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E509-A208-11DE-ABF2-002421116FB2")
    IDaSvrBalloon : public IDispatch
    {
    public:
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetEnabled( 
            /* [out] */ long *Enabled) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetNumLines( 
            /* [out] */ long *Lines) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetNumCharsPerLine( 
            /* [out] */ long *CharsPerLine) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetFontName( 
            /* [out] */ BSTR *FontName) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetFontSize( 
            /* [out] */ long *FontSize) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetFontBold( 
            /* [out] */ long *FontBold) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetFontItalic( 
            /* [out] */ long *FontItalic) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetFontStrikethru( 
            /* [out] */ long *FontStrikethru) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetFontUnderline( 
            /* [out] */ long *FontUnderline) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetForeColor( 
            /* [out] */ long *ForeColor) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetBackColor( 
            /* [out] */ long *BackColor) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetBorderColor( 
            /* [out] */ long *BorderColor) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetVisible( 
            /* [in] */ long Visible) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetVisible( 
            /* [out] */ long *Visible) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetFontName( 
            /* [in] */ BSTR FontName) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetFontSize( 
            /* [in] */ long FontSize) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetFontCharSet( 
            /* [in] */ short FontCharSet) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetFontCharSet( 
            /* [out] */ short *FontCharSet) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetStyle( 
            /* [in] */ long Style) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetStyle( 
            /* [out] */ long *Style) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetNumLines( 
            /* [in] */ long Lines) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetNumCharsPerLine( 
            /* [in] */ long CharsPerLine) = 0;
        
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
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEnabled )( 
            IDaSvrBalloon * This,
            /* [out] */ long *Enabled);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetNumLines )( 
            IDaSvrBalloon * This,
            /* [out] */ long *Lines);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetNumCharsPerLine )( 
            IDaSvrBalloon * This,
            /* [out] */ long *CharsPerLine);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetFontName )( 
            IDaSvrBalloon * This,
            /* [out] */ BSTR *FontName);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetFontSize )( 
            IDaSvrBalloon * This,
            /* [out] */ long *FontSize);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetFontBold )( 
            IDaSvrBalloon * This,
            /* [out] */ long *FontBold);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetFontItalic )( 
            IDaSvrBalloon * This,
            /* [out] */ long *FontItalic);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetFontStrikethru )( 
            IDaSvrBalloon * This,
            /* [out] */ long *FontStrikethru);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetFontUnderline )( 
            IDaSvrBalloon * This,
            /* [out] */ long *FontUnderline);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetForeColor )( 
            IDaSvrBalloon * This,
            /* [out] */ long *ForeColor);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetBackColor )( 
            IDaSvrBalloon * This,
            /* [out] */ long *BackColor);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetBorderColor )( 
            IDaSvrBalloon * This,
            /* [out] */ long *BorderColor);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetVisible )( 
            IDaSvrBalloon * This,
            /* [in] */ long Visible);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVisible )( 
            IDaSvrBalloon * This,
            /* [out] */ long *Visible);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetFontName )( 
            IDaSvrBalloon * This,
            /* [in] */ BSTR FontName);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetFontSize )( 
            IDaSvrBalloon * This,
            /* [in] */ long FontSize);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetFontCharSet )( 
            IDaSvrBalloon * This,
            /* [in] */ short FontCharSet);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetFontCharSet )( 
            IDaSvrBalloon * This,
            /* [out] */ short *FontCharSet);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetStyle )( 
            IDaSvrBalloon * This,
            /* [in] */ long Style);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetStyle )( 
            IDaSvrBalloon * This,
            /* [out] */ long *Style);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetNumLines )( 
            IDaSvrBalloon * This,
            /* [in] */ long Lines);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetNumCharsPerLine )( 
            IDaSvrBalloon * This,
            /* [in] */ long CharsPerLine);
        
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


#define IDaSvrBalloon_GetEnabled(This,Enabled)	\
    ( (This)->lpVtbl -> GetEnabled(This,Enabled) ) 

#define IDaSvrBalloon_GetNumLines(This,Lines)	\
    ( (This)->lpVtbl -> GetNumLines(This,Lines) ) 

#define IDaSvrBalloon_GetNumCharsPerLine(This,CharsPerLine)	\
    ( (This)->lpVtbl -> GetNumCharsPerLine(This,CharsPerLine) ) 

#define IDaSvrBalloon_GetFontName(This,FontName)	\
    ( (This)->lpVtbl -> GetFontName(This,FontName) ) 

#define IDaSvrBalloon_GetFontSize(This,FontSize)	\
    ( (This)->lpVtbl -> GetFontSize(This,FontSize) ) 

#define IDaSvrBalloon_GetFontBold(This,FontBold)	\
    ( (This)->lpVtbl -> GetFontBold(This,FontBold) ) 

#define IDaSvrBalloon_GetFontItalic(This,FontItalic)	\
    ( (This)->lpVtbl -> GetFontItalic(This,FontItalic) ) 

#define IDaSvrBalloon_GetFontStrikethru(This,FontStrikethru)	\
    ( (This)->lpVtbl -> GetFontStrikethru(This,FontStrikethru) ) 

#define IDaSvrBalloon_GetFontUnderline(This,FontUnderline)	\
    ( (This)->lpVtbl -> GetFontUnderline(This,FontUnderline) ) 

#define IDaSvrBalloon_GetForeColor(This,ForeColor)	\
    ( (This)->lpVtbl -> GetForeColor(This,ForeColor) ) 

#define IDaSvrBalloon_GetBackColor(This,BackColor)	\
    ( (This)->lpVtbl -> GetBackColor(This,BackColor) ) 

#define IDaSvrBalloon_GetBorderColor(This,BorderColor)	\
    ( (This)->lpVtbl -> GetBorderColor(This,BorderColor) ) 

#define IDaSvrBalloon_SetVisible(This,Visible)	\
    ( (This)->lpVtbl -> SetVisible(This,Visible) ) 

#define IDaSvrBalloon_GetVisible(This,Visible)	\
    ( (This)->lpVtbl -> GetVisible(This,Visible) ) 

#define IDaSvrBalloon_SetFontName(This,FontName)	\
    ( (This)->lpVtbl -> SetFontName(This,FontName) ) 

#define IDaSvrBalloon_SetFontSize(This,FontSize)	\
    ( (This)->lpVtbl -> SetFontSize(This,FontSize) ) 

#define IDaSvrBalloon_SetFontCharSet(This,FontCharSet)	\
    ( (This)->lpVtbl -> SetFontCharSet(This,FontCharSet) ) 

#define IDaSvrBalloon_GetFontCharSet(This,FontCharSet)	\
    ( (This)->lpVtbl -> GetFontCharSet(This,FontCharSet) ) 

#define IDaSvrBalloon_SetStyle(This,Style)	\
    ( (This)->lpVtbl -> SetStyle(This,Style) ) 

#define IDaSvrBalloon_GetStyle(This,Style)	\
    ( (This)->lpVtbl -> GetStyle(This,Style) ) 

#define IDaSvrBalloon_SetNumLines(This,Lines)	\
    ( (This)->lpVtbl -> SetNumLines(This,Lines) ) 

#define IDaSvrBalloon_SetNumCharsPerLine(This,CharsPerLine)	\
    ( (This)->lpVtbl -> SetNumCharsPerLine(This,CharsPerLine) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrBalloon_INTERFACE_DEFINED__ */


#ifndef __IDaSvrCharacter_INTERFACE_DEFINED__
#define __IDaSvrCharacter_INTERFACE_DEFINED__

/* interface IDaSvrCharacter */
/* [object][hidden][nonextensible][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IDaSvrCharacter;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E504-A208-11DE-ABF2-002421116FB2")
    IDaSvrCharacter : public IDispatch
    {
    public:
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetVisible( 
            /* [out] */ long *Visible) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE SetPosition( 
            /* [in] */ long Left,
            /* [in] */ long Top) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE GetPosition( 
            /* [out] */ long *Left,
            /* [out] */ long *Top) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE SetSize( 
            /* [in] */ long Width,
            /* [in] */ long Height) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE GetSize( 
            /* [out] */ long *Width,
            /* [out] */ long *Height) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetName( 
            /* [out] */ BSTR *Name) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetDescription( 
            /* [out] */ BSTR *Description) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetTTSSpeed( 
            /* [out] */ long *Speed) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetTTSPitch( 
            /* [out] */ short *Pitch) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE Activate( 
            /* [defaultvalue][in] */ short State = ActiveState_InputActive) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetIdleOn( 
            /* [in] */ long On) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE GetIdleOn( 
            /* [out] */ long *On) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE Prepare( 
            /* [in] */ long Type,
            /* [in] */ BSTR Name,
            /* [in] */ long Queue,
            /* [out] */ long *RequestID) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE Play( 
            /* [in] */ BSTR Animation,
            /* [out] */ long *RequestID) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE Stop( 
            /* [in] */ long RequestID) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE StopAll( 
            /* [in] */ long Types) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE Wait( 
            /* [in] */ long WaitForRequestID,
            /* [out] */ long *RequestID) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE Interrupt( 
            /* [in] */ long InterruptRequestID,
            /* [out] */ long *RequestID) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE Show( 
            /* [in] */ long Fast,
            /* [out] */ long *RequestID) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE Hide( 
            /* [in] */ long Fast,
            /* [out] */ long *RequestID) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE Speak( 
            /* [in] */ BSTR Text,
            /* [in] */ BSTR Url,
            /* [out] */ long *RequestID) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE MoveTo( 
            /* [in] */ short x,
            /* [in] */ short y,
            /* [in] */ long Speed,
            /* [out] */ long *RequestID) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE GestureAt( 
            /* [in] */ short x,
            /* [in] */ short y,
            /* [out] */ long *RequestID) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetMoveCause( 
            /* [out] */ long *Cause) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetVisibilityCause( 
            /* [out] */ long *Cause) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE HasOtherClients( 
            /* [out] */ long *OtherClientCount) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetSoundEffectsOn( 
            /* [in] */ long On) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetSoundEffectsOn( 
            /* [out] */ long *On) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetName( 
            /* [in] */ BSTR Name) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetDescription( 
            /* [in] */ BSTR Description) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetExtraData( 
            /* [out] */ BSTR *ExtraData) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE ShowPopupMenu( 
            /* [in] */ short x,
            /* [in] */ short y) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetAutoPopupMenu( 
            /* [in] */ long AutoPopupMenu) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetAutoPopupMenu( 
            /* [out] */ long *AutoPopupMenu) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetHelpFileName( 
            /* [out] */ BSTR *Name) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetHelpFileName( 
            /* [in] */ BSTR Name) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetHelpModeOn( 
            /* [in] */ long HelpModeOn) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetHelpModeOn( 
            /* [out] */ long *HelpModeOn) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetHelpContextID( 
            /* [in] */ long ContextID) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetHelpContextID( 
            /* [out] */ long *ContextID) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetActive( 
            /* [out] */ short *State) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE Listen( 
            /* [in] */ long Listen) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetLanguageID( 
            /* [in] */ long LanguageID) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetLanguageID( 
            /* [out] */ long *LanguageID) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetTTSModeID( 
            /* [out] */ BSTR *ModeID) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetTTSModeID( 
            /* [in] */ BSTR ModeID) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetSRModeID( 
            /* [out] */ BSTR *ModeID) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetSRModeID( 
            /* [in] */ BSTR ModeID) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetGUID( 
            /* [out] */ BSTR *GUID) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE GetOriginalSize( 
            /* [out] */ long *Width,
            /* [out] */ long *Height) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE Think( 
            /* [in] */ BSTR Text,
            /* [out] */ long *RequestID) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE GetVersion( 
            /* [out] */ short *MajorVersion,
            /* [out] */ short *MinorVersion) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetAnimationNames( 
            /* [out] */ IUnknown **punkEnum) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetSRStatus( 
            /* [out] */ long *Status) = 0;
        
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
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVisible )( 
            IDaSvrCharacter * This,
            /* [out] */ long *Visible);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetPosition )( 
            IDaSvrCharacter * This,
            /* [in] */ long Left,
            /* [in] */ long Top);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetPosition )( 
            IDaSvrCharacter * This,
            /* [out] */ long *Left,
            /* [out] */ long *Top);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetSize )( 
            IDaSvrCharacter * This,
            /* [in] */ long Width,
            /* [in] */ long Height);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetSize )( 
            IDaSvrCharacter * This,
            /* [out] */ long *Width,
            /* [out] */ long *Height);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetName )( 
            IDaSvrCharacter * This,
            /* [out] */ BSTR *Name);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetDescription )( 
            IDaSvrCharacter * This,
            /* [out] */ BSTR *Description);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetTTSSpeed )( 
            IDaSvrCharacter * This,
            /* [out] */ long *Speed);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetTTSPitch )( 
            IDaSvrCharacter * This,
            /* [out] */ short *Pitch);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Activate )( 
            IDaSvrCharacter * This,
            /* [defaultvalue][in] */ short State);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetIdleOn )( 
            IDaSvrCharacter * This,
            /* [in] */ long On);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetIdleOn )( 
            IDaSvrCharacter * This,
            /* [out] */ long *On);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Prepare )( 
            IDaSvrCharacter * This,
            /* [in] */ long Type,
            /* [in] */ BSTR Name,
            /* [in] */ long Queue,
            /* [out] */ long *RequestID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Play )( 
            IDaSvrCharacter * This,
            /* [in] */ BSTR Animation,
            /* [out] */ long *RequestID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IDaSvrCharacter * This,
            /* [in] */ long RequestID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *StopAll )( 
            IDaSvrCharacter * This,
            /* [in] */ long Types);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Wait )( 
            IDaSvrCharacter * This,
            /* [in] */ long WaitForRequestID,
            /* [out] */ long *RequestID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Interrupt )( 
            IDaSvrCharacter * This,
            /* [in] */ long InterruptRequestID,
            /* [out] */ long *RequestID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Show )( 
            IDaSvrCharacter * This,
            /* [in] */ long Fast,
            /* [out] */ long *RequestID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Hide )( 
            IDaSvrCharacter * This,
            /* [in] */ long Fast,
            /* [out] */ long *RequestID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Speak )( 
            IDaSvrCharacter * This,
            /* [in] */ BSTR Text,
            /* [in] */ BSTR Url,
            /* [out] */ long *RequestID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *MoveTo )( 
            IDaSvrCharacter * This,
            /* [in] */ short x,
            /* [in] */ short y,
            /* [in] */ long Speed,
            /* [out] */ long *RequestID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GestureAt )( 
            IDaSvrCharacter * This,
            /* [in] */ short x,
            /* [in] */ short y,
            /* [out] */ long *RequestID);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetMoveCause )( 
            IDaSvrCharacter * This,
            /* [out] */ long *Cause);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVisibilityCause )( 
            IDaSvrCharacter * This,
            /* [out] */ long *Cause);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *HasOtherClients )( 
            IDaSvrCharacter * This,
            /* [out] */ long *OtherClientCount);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetSoundEffectsOn )( 
            IDaSvrCharacter * This,
            /* [in] */ long On);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetSoundEffectsOn )( 
            IDaSvrCharacter * This,
            /* [out] */ long *On);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetName )( 
            IDaSvrCharacter * This,
            /* [in] */ BSTR Name);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetDescription )( 
            IDaSvrCharacter * This,
            /* [in] */ BSTR Description);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetExtraData )( 
            IDaSvrCharacter * This,
            /* [out] */ BSTR *ExtraData);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *ShowPopupMenu )( 
            IDaSvrCharacter * This,
            /* [in] */ short x,
            /* [in] */ short y);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetAutoPopupMenu )( 
            IDaSvrCharacter * This,
            /* [in] */ long AutoPopupMenu);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetAutoPopupMenu )( 
            IDaSvrCharacter * This,
            /* [out] */ long *AutoPopupMenu);
        
        /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetHelpFileName )( 
            IDaSvrCharacter * This,
            /* [out] */ BSTR *Name);
        
        /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetHelpFileName )( 
            IDaSvrCharacter * This,
            /* [in] */ BSTR Name);
        
        /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetHelpModeOn )( 
            IDaSvrCharacter * This,
            /* [in] */ long HelpModeOn);
        
        /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetHelpModeOn )( 
            IDaSvrCharacter * This,
            /* [out] */ long *HelpModeOn);
        
        /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetHelpContextID )( 
            IDaSvrCharacter * This,
            /* [in] */ long ContextID);
        
        /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetHelpContextID )( 
            IDaSvrCharacter * This,
            /* [out] */ long *ContextID);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetActive )( 
            IDaSvrCharacter * This,
            /* [out] */ short *State);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Listen )( 
            IDaSvrCharacter * This,
            /* [in] */ long Listen);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetLanguageID )( 
            IDaSvrCharacter * This,
            /* [in] */ long LanguageID);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetLanguageID )( 
            IDaSvrCharacter * This,
            /* [out] */ long *LanguageID);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetTTSModeID )( 
            IDaSvrCharacter * This,
            /* [out] */ BSTR *ModeID);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetTTSModeID )( 
            IDaSvrCharacter * This,
            /* [in] */ BSTR ModeID);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetSRModeID )( 
            IDaSvrCharacter * This,
            /* [out] */ BSTR *ModeID);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetSRModeID )( 
            IDaSvrCharacter * This,
            /* [in] */ BSTR ModeID);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetGUID )( 
            IDaSvrCharacter * This,
            /* [out] */ BSTR *GUID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetOriginalSize )( 
            IDaSvrCharacter * This,
            /* [out] */ long *Width,
            /* [out] */ long *Height);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Think )( 
            IDaSvrCharacter * This,
            /* [in] */ BSTR Text,
            /* [out] */ long *RequestID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVersion )( 
            IDaSvrCharacter * This,
            /* [out] */ short *MajorVersion,
            /* [out] */ short *MinorVersion);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetAnimationNames )( 
            IDaSvrCharacter * This,
            /* [out] */ IUnknown **punkEnum);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetSRStatus )( 
            IDaSvrCharacter * This,
            /* [out] */ long *Status);
        
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


#define IDaSvrCharacter_GetVisible(This,Visible)	\
    ( (This)->lpVtbl -> GetVisible(This,Visible) ) 

#define IDaSvrCharacter_SetPosition(This,Left,Top)	\
    ( (This)->lpVtbl -> SetPosition(This,Left,Top) ) 

#define IDaSvrCharacter_GetPosition(This,Left,Top)	\
    ( (This)->lpVtbl -> GetPosition(This,Left,Top) ) 

#define IDaSvrCharacter_SetSize(This,Width,Height)	\
    ( (This)->lpVtbl -> SetSize(This,Width,Height) ) 

#define IDaSvrCharacter_GetSize(This,Width,Height)	\
    ( (This)->lpVtbl -> GetSize(This,Width,Height) ) 

#define IDaSvrCharacter_GetName(This,Name)	\
    ( (This)->lpVtbl -> GetName(This,Name) ) 

#define IDaSvrCharacter_GetDescription(This,Description)	\
    ( (This)->lpVtbl -> GetDescription(This,Description) ) 

#define IDaSvrCharacter_GetTTSSpeed(This,Speed)	\
    ( (This)->lpVtbl -> GetTTSSpeed(This,Speed) ) 

#define IDaSvrCharacter_GetTTSPitch(This,Pitch)	\
    ( (This)->lpVtbl -> GetTTSPitch(This,Pitch) ) 

#define IDaSvrCharacter_Activate(This,State)	\
    ( (This)->lpVtbl -> Activate(This,State) ) 

#define IDaSvrCharacter_SetIdleOn(This,On)	\
    ( (This)->lpVtbl -> SetIdleOn(This,On) ) 

#define IDaSvrCharacter_GetIdleOn(This,On)	\
    ( (This)->lpVtbl -> GetIdleOn(This,On) ) 

#define IDaSvrCharacter_Prepare(This,Type,Name,Queue,RequestID)	\
    ( (This)->lpVtbl -> Prepare(This,Type,Name,Queue,RequestID) ) 

#define IDaSvrCharacter_Play(This,Animation,RequestID)	\
    ( (This)->lpVtbl -> Play(This,Animation,RequestID) ) 

#define IDaSvrCharacter_Stop(This,RequestID)	\
    ( (This)->lpVtbl -> Stop(This,RequestID) ) 

#define IDaSvrCharacter_StopAll(This,Types)	\
    ( (This)->lpVtbl -> StopAll(This,Types) ) 

#define IDaSvrCharacter_Wait(This,WaitForRequestID,RequestID)	\
    ( (This)->lpVtbl -> Wait(This,WaitForRequestID,RequestID) ) 

#define IDaSvrCharacter_Interrupt(This,InterruptRequestID,RequestID)	\
    ( (This)->lpVtbl -> Interrupt(This,InterruptRequestID,RequestID) ) 

#define IDaSvrCharacter_Show(This,Fast,RequestID)	\
    ( (This)->lpVtbl -> Show(This,Fast,RequestID) ) 

#define IDaSvrCharacter_Hide(This,Fast,RequestID)	\
    ( (This)->lpVtbl -> Hide(This,Fast,RequestID) ) 

#define IDaSvrCharacter_Speak(This,Text,Url,RequestID)	\
    ( (This)->lpVtbl -> Speak(This,Text,Url,RequestID) ) 

#define IDaSvrCharacter_MoveTo(This,x,y,Speed,RequestID)	\
    ( (This)->lpVtbl -> MoveTo(This,x,y,Speed,RequestID) ) 

#define IDaSvrCharacter_GestureAt(This,x,y,RequestID)	\
    ( (This)->lpVtbl -> GestureAt(This,x,y,RequestID) ) 

#define IDaSvrCharacter_GetMoveCause(This,Cause)	\
    ( (This)->lpVtbl -> GetMoveCause(This,Cause) ) 

#define IDaSvrCharacter_GetVisibilityCause(This,Cause)	\
    ( (This)->lpVtbl -> GetVisibilityCause(This,Cause) ) 

#define IDaSvrCharacter_HasOtherClients(This,OtherClientCount)	\
    ( (This)->lpVtbl -> HasOtherClients(This,OtherClientCount) ) 

#define IDaSvrCharacter_SetSoundEffectsOn(This,On)	\
    ( (This)->lpVtbl -> SetSoundEffectsOn(This,On) ) 

#define IDaSvrCharacter_GetSoundEffectsOn(This,On)	\
    ( (This)->lpVtbl -> GetSoundEffectsOn(This,On) ) 

#define IDaSvrCharacter_SetName(This,Name)	\
    ( (This)->lpVtbl -> SetName(This,Name) ) 

#define IDaSvrCharacter_SetDescription(This,Description)	\
    ( (This)->lpVtbl -> SetDescription(This,Description) ) 

#define IDaSvrCharacter_GetExtraData(This,ExtraData)	\
    ( (This)->lpVtbl -> GetExtraData(This,ExtraData) ) 

#define IDaSvrCharacter_ShowPopupMenu(This,x,y)	\
    ( (This)->lpVtbl -> ShowPopupMenu(This,x,y) ) 

#define IDaSvrCharacter_SetAutoPopupMenu(This,AutoPopupMenu)	\
    ( (This)->lpVtbl -> SetAutoPopupMenu(This,AutoPopupMenu) ) 

#define IDaSvrCharacter_GetAutoPopupMenu(This,AutoPopupMenu)	\
    ( (This)->lpVtbl -> GetAutoPopupMenu(This,AutoPopupMenu) ) 

#define IDaSvrCharacter_GetHelpFileName(This,Name)	\
    ( (This)->lpVtbl -> GetHelpFileName(This,Name) ) 

#define IDaSvrCharacter_SetHelpFileName(This,Name)	\
    ( (This)->lpVtbl -> SetHelpFileName(This,Name) ) 

#define IDaSvrCharacter_SetHelpModeOn(This,HelpModeOn)	\
    ( (This)->lpVtbl -> SetHelpModeOn(This,HelpModeOn) ) 

#define IDaSvrCharacter_GetHelpModeOn(This,HelpModeOn)	\
    ( (This)->lpVtbl -> GetHelpModeOn(This,HelpModeOn) ) 

#define IDaSvrCharacter_SetHelpContextID(This,ContextID)	\
    ( (This)->lpVtbl -> SetHelpContextID(This,ContextID) ) 

#define IDaSvrCharacter_GetHelpContextID(This,ContextID)	\
    ( (This)->lpVtbl -> GetHelpContextID(This,ContextID) ) 

#define IDaSvrCharacter_GetActive(This,State)	\
    ( (This)->lpVtbl -> GetActive(This,State) ) 

#define IDaSvrCharacter_Listen(This,Listen)	\
    ( (This)->lpVtbl -> Listen(This,Listen) ) 

#define IDaSvrCharacter_SetLanguageID(This,LanguageID)	\
    ( (This)->lpVtbl -> SetLanguageID(This,LanguageID) ) 

#define IDaSvrCharacter_GetLanguageID(This,LanguageID)	\
    ( (This)->lpVtbl -> GetLanguageID(This,LanguageID) ) 

#define IDaSvrCharacter_GetTTSModeID(This,ModeID)	\
    ( (This)->lpVtbl -> GetTTSModeID(This,ModeID) ) 

#define IDaSvrCharacter_SetTTSModeID(This,ModeID)	\
    ( (This)->lpVtbl -> SetTTSModeID(This,ModeID) ) 

#define IDaSvrCharacter_GetSRModeID(This,ModeID)	\
    ( (This)->lpVtbl -> GetSRModeID(This,ModeID) ) 

#define IDaSvrCharacter_SetSRModeID(This,ModeID)	\
    ( (This)->lpVtbl -> SetSRModeID(This,ModeID) ) 

#define IDaSvrCharacter_GetGUID(This,GUID)	\
    ( (This)->lpVtbl -> GetGUID(This,GUID) ) 

#define IDaSvrCharacter_GetOriginalSize(This,Width,Height)	\
    ( (This)->lpVtbl -> GetOriginalSize(This,Width,Height) ) 

#define IDaSvrCharacter_Think(This,Text,RequestID)	\
    ( (This)->lpVtbl -> Think(This,Text,RequestID) ) 

#define IDaSvrCharacter_GetVersion(This,MajorVersion,MinorVersion)	\
    ( (This)->lpVtbl -> GetVersion(This,MajorVersion,MinorVersion) ) 

#define IDaSvrCharacter_GetAnimationNames(This,punkEnum)	\
    ( (This)->lpVtbl -> GetAnimationNames(This,punkEnum) ) 

#define IDaSvrCharacter_GetSRStatus(This,Status)	\
    ( (This)->lpVtbl -> GetSRStatus(This,Status) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrCharacter_INTERFACE_DEFINED__ */


#ifndef __IDaSvrCommandsWindow_INTERFACE_DEFINED__
#define __IDaSvrCommandsWindow_INTERFACE_DEFINED__

/* interface IDaSvrCommandsWindow */
/* [object][hidden][nonextensible][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IDaSvrCommandsWindow;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E507-A208-11DE-ABF2-002421116FB2")
    IDaSvrCommandsWindow : public IDispatch
    {
    public:
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetVisible( 
            /* [in] */ long Visible) = 0;
        
        virtual /* [hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetVisible( 
            /* [out] */ long *Visible) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE GetPosition( 
            /* [out] */ long *Left,
            /* [out] */ long *Top) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE GetSize( 
            /* [out] */ long *Width,
            /* [out] */ long *Height) = 0;
        
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
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetVisible )( 
            IDaSvrCommandsWindow * This,
            /* [in] */ long Visible);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVisible )( 
            IDaSvrCommandsWindow * This,
            /* [out] */ long *Visible);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetPosition )( 
            IDaSvrCommandsWindow * This,
            /* [out] */ long *Left,
            /* [out] */ long *Top);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetSize )( 
            IDaSvrCommandsWindow * This,
            /* [out] */ long *Width,
            /* [out] */ long *Height);
        
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


#define IDaSvrCommandsWindow_SetVisible(This,Visible)	\
    ( (This)->lpVtbl -> SetVisible(This,Visible) ) 

#define IDaSvrCommandsWindow_GetVisible(This,Visible)	\
    ( (This)->lpVtbl -> GetVisible(This,Visible) ) 

#define IDaSvrCommandsWindow_GetPosition(This,Left,Top)	\
    ( (This)->lpVtbl -> GetPosition(This,Left,Top) ) 

#define IDaSvrCommandsWindow_GetSize(This,Width,Height)	\
    ( (This)->lpVtbl -> GetSize(This,Width,Height) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrCommandsWindow_INTERFACE_DEFINED__ */


#ifndef __IDaSvrNotifySink15_INTERFACE_DEFINED__
#define __IDaSvrNotifySink15_INTERFACE_DEFINED__

/* interface IDaSvrNotifySink15 */
/* [object][hidden][nonextensible][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IDaSvrNotifySink15;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E503-A208-11DE-ABF2-002421116FB2")
    IDaSvrNotifySink15 : public IDispatch
    {
    public:
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE Command( 
            /* [in] */ long CommandID,
            /* [in] */ IDaSvrUserInput2 *UserInput) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE ActivateInputState( 
            /* [in] */ long CharacterID,
            /* [in] */ long Activated) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE Restart( void) = 0;
        
        virtual /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE Shutdown( void) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE VisibleState( 
            /* [in] */ long CharacterID,
            /* [in] */ long Visible,
            /* [in] */ long Cause) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE Click( 
            /* [in] */ long CharacterID,
            /* [in] */ short Keys,
            /* [in] */ long x,
            /* [in] */ long y) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE DblClick( 
            /* [in] */ long CharacterID,
            /* [in] */ short Keys,
            /* [in] */ long x,
            /* [in] */ long y) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE DragStart( 
            /* [in] */ long CharacterID,
            /* [in] */ short Keys,
            /* [in] */ long x,
            /* [in] */ long y) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE DragComplete( 
            /* [in] */ long CharacterID,
            /* [in] */ short Keys,
            /* [in] */ long x,
            /* [in] */ long y) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE RequestStart( 
            /* [in] */ long RequestID) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE RequestComplete( 
            /* [in] */ long RequestID,
            /* [in] */ long hrStatus) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE BookMark( 
            /* [in] */ long dwBookMarkID) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE Idle( 
            /* [in] */ long CharacterID,
            /* [in] */ long Start) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE Move( 
            /* [in] */ long CharacterID,
            /* [in] */ long x,
            /* [in] */ long y,
            /* [in] */ long Cause) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE Size( 
            /* [in] */ long CharacterID,
            /* [in] */ long Width,
            /* [in] */ long Height) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE BalloonVisibleState( 
            /* [in] */ long CharacterID,
            /* [in] */ long Visible) = 0;
        
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
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Command )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long CommandID,
            /* [in] */ IDaSvrUserInput2 *UserInput);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *ActivateInputState )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long CharacterID,
            /* [in] */ long Activated);
        
        /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Restart )( 
            IDaSvrNotifySink15 * This);
        
        /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Shutdown )( 
            IDaSvrNotifySink15 * This);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *VisibleState )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long CharacterID,
            /* [in] */ long Visible,
            /* [in] */ long Cause);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Click )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long CharacterID,
            /* [in] */ short Keys,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *DblClick )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long CharacterID,
            /* [in] */ short Keys,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *DragStart )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long CharacterID,
            /* [in] */ short Keys,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *DragComplete )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long CharacterID,
            /* [in] */ short Keys,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *RequestStart )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long RequestID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *RequestComplete )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long RequestID,
            /* [in] */ long hrStatus);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *BookMark )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long dwBookMarkID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Idle )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long CharacterID,
            /* [in] */ long Start);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Move )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long CharacterID,
            /* [in] */ long x,
            /* [in] */ long y,
            /* [in] */ long Cause);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Size )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long CharacterID,
            /* [in] */ long Width,
            /* [in] */ long Height);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *BalloonVisibleState )( 
            IDaSvrNotifySink15 * This,
            /* [in] */ long CharacterID,
            /* [in] */ long Visible);
        
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


#define IDaSvrNotifySink15_Command(This,CommandID,UserInput)	\
    ( (This)->lpVtbl -> Command(This,CommandID,UserInput) ) 

#define IDaSvrNotifySink15_ActivateInputState(This,CharacterID,Activated)	\
    ( (This)->lpVtbl -> ActivateInputState(This,CharacterID,Activated) ) 

#define IDaSvrNotifySink15_Restart(This)	\
    ( (This)->lpVtbl -> Restart(This) ) 

#define IDaSvrNotifySink15_Shutdown(This)	\
    ( (This)->lpVtbl -> Shutdown(This) ) 

#define IDaSvrNotifySink15_VisibleState(This,CharacterID,Visible,Cause)	\
    ( (This)->lpVtbl -> VisibleState(This,CharacterID,Visible,Cause) ) 

#define IDaSvrNotifySink15_Click(This,CharacterID,Keys,x,y)	\
    ( (This)->lpVtbl -> Click(This,CharacterID,Keys,x,y) ) 

#define IDaSvrNotifySink15_DblClick(This,CharacterID,Keys,x,y)	\
    ( (This)->lpVtbl -> DblClick(This,CharacterID,Keys,x,y) ) 

#define IDaSvrNotifySink15_DragStart(This,CharacterID,Keys,x,y)	\
    ( (This)->lpVtbl -> DragStart(This,CharacterID,Keys,x,y) ) 

#define IDaSvrNotifySink15_DragComplete(This,CharacterID,Keys,x,y)	\
    ( (This)->lpVtbl -> DragComplete(This,CharacterID,Keys,x,y) ) 

#define IDaSvrNotifySink15_RequestStart(This,RequestID)	\
    ( (This)->lpVtbl -> RequestStart(This,RequestID) ) 

#define IDaSvrNotifySink15_RequestComplete(This,RequestID,hrStatus)	\
    ( (This)->lpVtbl -> RequestComplete(This,RequestID,hrStatus) ) 

#define IDaSvrNotifySink15_BookMark(This,dwBookMarkID)	\
    ( (This)->lpVtbl -> BookMark(This,dwBookMarkID) ) 

#define IDaSvrNotifySink15_Idle(This,CharacterID,Start)	\
    ( (This)->lpVtbl -> Idle(This,CharacterID,Start) ) 

#define IDaSvrNotifySink15_Move(This,CharacterID,x,y,Cause)	\
    ( (This)->lpVtbl -> Move(This,CharacterID,x,y,Cause) ) 

#define IDaSvrNotifySink15_Size(This,CharacterID,Width,Height)	\
    ( (This)->lpVtbl -> Size(This,CharacterID,Width,Height) ) 

#define IDaSvrNotifySink15_BalloonVisibleState(This,CharacterID,Visible)	\
    ( (This)->lpVtbl -> BalloonVisibleState(This,CharacterID,Visible) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrNotifySink15_INTERFACE_DEFINED__ */


#ifndef __IDaSvrNotifySink_INTERFACE_DEFINED__
#define __IDaSvrNotifySink_INTERFACE_DEFINED__

/* interface IDaSvrNotifySink */
/* [object][hidden][nonextensible][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IDaSvrNotifySink;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E516-A208-11DE-ABF2-002421116FB2")
    IDaSvrNotifySink : public IDaSvrNotifySink15
    {
    public:
        virtual /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE HelpComplete( 
            /* [in] */ long CharacterID,
            /* [in] */ long CommandID,
            /* [in] */ long Cause) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE ListeningState( 
            /* [in] */ long CharacterID,
            /* [in] */ long Listening,
            /* [in] */ long Cause) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE DefaultCharacterChange( 
            /* [in] */ BSTR bszGUID) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE AgentPropertyChange( void) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE ActiveClientChange( 
            /* [in] */ long CharacterID,
            /* [in] */ long Status) = 0;
        
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
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Command )( 
            IDaSvrNotifySink * This,
            /* [in] */ long CommandID,
            /* [in] */ IDaSvrUserInput2 *UserInput);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *ActivateInputState )( 
            IDaSvrNotifySink * This,
            /* [in] */ long CharacterID,
            /* [in] */ long Activated);
        
        /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Restart )( 
            IDaSvrNotifySink * This);
        
        /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Shutdown )( 
            IDaSvrNotifySink * This);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *VisibleState )( 
            IDaSvrNotifySink * This,
            /* [in] */ long CharacterID,
            /* [in] */ long Visible,
            /* [in] */ long Cause);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Click )( 
            IDaSvrNotifySink * This,
            /* [in] */ long CharacterID,
            /* [in] */ short Keys,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *DblClick )( 
            IDaSvrNotifySink * This,
            /* [in] */ long CharacterID,
            /* [in] */ short Keys,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *DragStart )( 
            IDaSvrNotifySink * This,
            /* [in] */ long CharacterID,
            /* [in] */ short Keys,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *DragComplete )( 
            IDaSvrNotifySink * This,
            /* [in] */ long CharacterID,
            /* [in] */ short Keys,
            /* [in] */ long x,
            /* [in] */ long y);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *RequestStart )( 
            IDaSvrNotifySink * This,
            /* [in] */ long RequestID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *RequestComplete )( 
            IDaSvrNotifySink * This,
            /* [in] */ long RequestID,
            /* [in] */ long hrStatus);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *BookMark )( 
            IDaSvrNotifySink * This,
            /* [in] */ long dwBookMarkID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Idle )( 
            IDaSvrNotifySink * This,
            /* [in] */ long CharacterID,
            /* [in] */ long Start);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Move )( 
            IDaSvrNotifySink * This,
            /* [in] */ long CharacterID,
            /* [in] */ long x,
            /* [in] */ long y,
            /* [in] */ long Cause);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Size )( 
            IDaSvrNotifySink * This,
            /* [in] */ long CharacterID,
            /* [in] */ long Width,
            /* [in] */ long Height);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *BalloonVisibleState )( 
            IDaSvrNotifySink * This,
            /* [in] */ long CharacterID,
            /* [in] */ long Visible);
        
        /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *HelpComplete )( 
            IDaSvrNotifySink * This,
            /* [in] */ long CharacterID,
            /* [in] */ long CommandID,
            /* [in] */ long Cause);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *ListeningState )( 
            IDaSvrNotifySink * This,
            /* [in] */ long CharacterID,
            /* [in] */ long Listening,
            /* [in] */ long Cause);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *DefaultCharacterChange )( 
            IDaSvrNotifySink * This,
            /* [in] */ BSTR bszGUID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *AgentPropertyChange )( 
            IDaSvrNotifySink * This);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *ActiveClientChange )( 
            IDaSvrNotifySink * This,
            /* [in] */ long CharacterID,
            /* [in] */ long Status);
        
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


#define IDaSvrNotifySink_Command(This,CommandID,UserInput)	\
    ( (This)->lpVtbl -> Command(This,CommandID,UserInput) ) 

#define IDaSvrNotifySink_ActivateInputState(This,CharacterID,Activated)	\
    ( (This)->lpVtbl -> ActivateInputState(This,CharacterID,Activated) ) 

#define IDaSvrNotifySink_Restart(This)	\
    ( (This)->lpVtbl -> Restart(This) ) 

#define IDaSvrNotifySink_Shutdown(This)	\
    ( (This)->lpVtbl -> Shutdown(This) ) 

#define IDaSvrNotifySink_VisibleState(This,CharacterID,Visible,Cause)	\
    ( (This)->lpVtbl -> VisibleState(This,CharacterID,Visible,Cause) ) 

#define IDaSvrNotifySink_Click(This,CharacterID,Keys,x,y)	\
    ( (This)->lpVtbl -> Click(This,CharacterID,Keys,x,y) ) 

#define IDaSvrNotifySink_DblClick(This,CharacterID,Keys,x,y)	\
    ( (This)->lpVtbl -> DblClick(This,CharacterID,Keys,x,y) ) 

#define IDaSvrNotifySink_DragStart(This,CharacterID,Keys,x,y)	\
    ( (This)->lpVtbl -> DragStart(This,CharacterID,Keys,x,y) ) 

#define IDaSvrNotifySink_DragComplete(This,CharacterID,Keys,x,y)	\
    ( (This)->lpVtbl -> DragComplete(This,CharacterID,Keys,x,y) ) 

#define IDaSvrNotifySink_RequestStart(This,RequestID)	\
    ( (This)->lpVtbl -> RequestStart(This,RequestID) ) 

#define IDaSvrNotifySink_RequestComplete(This,RequestID,hrStatus)	\
    ( (This)->lpVtbl -> RequestComplete(This,RequestID,hrStatus) ) 

#define IDaSvrNotifySink_BookMark(This,dwBookMarkID)	\
    ( (This)->lpVtbl -> BookMark(This,dwBookMarkID) ) 

#define IDaSvrNotifySink_Idle(This,CharacterID,Start)	\
    ( (This)->lpVtbl -> Idle(This,CharacterID,Start) ) 

#define IDaSvrNotifySink_Move(This,CharacterID,x,y,Cause)	\
    ( (This)->lpVtbl -> Move(This,CharacterID,x,y,Cause) ) 

#define IDaSvrNotifySink_Size(This,CharacterID,Width,Height)	\
    ( (This)->lpVtbl -> Size(This,CharacterID,Width,Height) ) 

#define IDaSvrNotifySink_BalloonVisibleState(This,CharacterID,Visible)	\
    ( (This)->lpVtbl -> BalloonVisibleState(This,CharacterID,Visible) ) 


#define IDaSvrNotifySink_HelpComplete(This,CharacterID,CommandID,Cause)	\
    ( (This)->lpVtbl -> HelpComplete(This,CharacterID,CommandID,Cause) ) 

#define IDaSvrNotifySink_ListeningState(This,CharacterID,Listening,Cause)	\
    ( (This)->lpVtbl -> ListeningState(This,CharacterID,Listening,Cause) ) 

#define IDaSvrNotifySink_DefaultCharacterChange(This,bszGUID)	\
    ( (This)->lpVtbl -> DefaultCharacterChange(This,bszGUID) ) 

#define IDaSvrNotifySink_AgentPropertyChange(This)	\
    ( (This)->lpVtbl -> AgentPropertyChange(This) ) 

#define IDaSvrNotifySink_ActiveClientChange(This,CharacterID,Status)	\
    ( (This)->lpVtbl -> ActiveClientChange(This,CharacterID,Status) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrNotifySink_INTERFACE_DEFINED__ */


#ifndef __IDaSvrCharacterFiles_INTERFACE_DEFINED__
#define __IDaSvrCharacterFiles_INTERFACE_DEFINED__

/* interface IDaSvrCharacterFiles */
/* [object][hidden][nonextensible][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IDaSvrCharacterFiles;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E523-A208-11DE-ABF2-002421116FB2")
    IDaSvrCharacterFiles : public IDispatch
    {
    public:
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_FilePaths( 
            /* [retval][out] */ SAFEARRAY * *FilePaths) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_SearchPath( 
            /* [retval][out] */ BSTR *SearchPath) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_SearchPath( 
            /* [in] */ BSTR SearchPath) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_DefaultSearchPath( 
            /* [retval][out] */ BSTR *DefaultSearchPath) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Filter( 
            /* [retval][out] */ long *Filter) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Filter( 
            /* [in] */ long Filter) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_DefaultFilePath( 
            /* [retval][out] */ BSTR *DefaultFilePath) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_DefaultFileName( 
            /* [retval][out] */ BSTR *DefaultFileName) = 0;
        
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
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_FilePaths )( 
            IDaSvrCharacterFiles * This,
            /* [retval][out] */ SAFEARRAY * *FilePaths);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SearchPath )( 
            IDaSvrCharacterFiles * This,
            /* [retval][out] */ BSTR *SearchPath);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_SearchPath )( 
            IDaSvrCharacterFiles * This,
            /* [in] */ BSTR SearchPath);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_DefaultSearchPath )( 
            IDaSvrCharacterFiles * This,
            /* [retval][out] */ BSTR *DefaultSearchPath);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Filter )( 
            IDaSvrCharacterFiles * This,
            /* [retval][out] */ long *Filter);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Filter )( 
            IDaSvrCharacterFiles * This,
            /* [in] */ long Filter);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_DefaultFilePath )( 
            IDaSvrCharacterFiles * This,
            /* [retval][out] */ BSTR *DefaultFilePath);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_DefaultFileName )( 
            IDaSvrCharacterFiles * This,
            /* [retval][out] */ BSTR *DefaultFileName);
        
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

#define IDaSvrCharacterFiles_get_DefaultFilePath(This,DefaultFilePath)	\
    ( (This)->lpVtbl -> get_DefaultFilePath(This,DefaultFilePath) ) 

#define IDaSvrCharacterFiles_get_DefaultFileName(This,DefaultFileName)	\
    ( (This)->lpVtbl -> get_DefaultFileName(This,DefaultFileName) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrCharacterFiles_INTERFACE_DEFINED__ */


#ifndef __IDaSvrTTSEngine_INTERFACE_DEFINED__
#define __IDaSvrTTSEngine_INTERFACE_DEFINED__

/* interface IDaSvrTTSEngine */
/* [object][hidden][nonextensible][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IDaSvrTTSEngine;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E519-A208-11DE-ABF2-002421116FB2")
    IDaSvrTTSEngine : public IDispatch
    {
    public:
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_TTSModeID( 
            /* [retval][out] */ BSTR *TTSModeID) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_DisplayName( 
            /* [retval][out] */ BSTR *DisplayName) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Manufacturer( 
            /* [retval][out] */ BSTR *Manufacturer) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE GetVersion( 
            /* [out] */ short *MajorVersion,
            /* [out] */ short *MinorVersion) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Gender( 
            /* [retval][out] */ SpeechGenderType *Gender) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_LanguageID( 
            /* [retval][out] */ long *LanguageID) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_LanguageName( 
            /* [defaultvalue][in] */ VARIANT_BOOL EnglishName,
            /* [retval][out] */ BSTR *LanguageName) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrTTSEngineVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrTTSEngine * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrTTSEngine * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrTTSEngine * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrTTSEngine * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrTTSEngine * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrTTSEngine * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrTTSEngine * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_TTSModeID )( 
            IDaSvrTTSEngine * This,
            /* [retval][out] */ BSTR *TTSModeID);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_DisplayName )( 
            IDaSvrTTSEngine * This,
            /* [retval][out] */ BSTR *DisplayName);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Manufacturer )( 
            IDaSvrTTSEngine * This,
            /* [retval][out] */ BSTR *Manufacturer);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVersion )( 
            IDaSvrTTSEngine * This,
            /* [out] */ short *MajorVersion,
            /* [out] */ short *MinorVersion);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Gender )( 
            IDaSvrTTSEngine * This,
            /* [retval][out] */ SpeechGenderType *Gender);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_LanguageID )( 
            IDaSvrTTSEngine * This,
            /* [retval][out] */ long *LanguageID);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_LanguageName )( 
            IDaSvrTTSEngine * This,
            /* [defaultvalue][in] */ VARIANT_BOOL EnglishName,
            /* [retval][out] */ BSTR *LanguageName);
        
        END_INTERFACE
    } IDaSvrTTSEngineVtbl;

    interface IDaSvrTTSEngine
    {
        CONST_VTBL struct IDaSvrTTSEngineVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrTTSEngine_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrTTSEngine_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrTTSEngine_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrTTSEngine_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrTTSEngine_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrTTSEngine_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrTTSEngine_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrTTSEngine_get_TTSModeID(This,TTSModeID)	\
    ( (This)->lpVtbl -> get_TTSModeID(This,TTSModeID) ) 

#define IDaSvrTTSEngine_get_DisplayName(This,DisplayName)	\
    ( (This)->lpVtbl -> get_DisplayName(This,DisplayName) ) 

#define IDaSvrTTSEngine_get_Manufacturer(This,Manufacturer)	\
    ( (This)->lpVtbl -> get_Manufacturer(This,Manufacturer) ) 

#define IDaSvrTTSEngine_GetVersion(This,MajorVersion,MinorVersion)	\
    ( (This)->lpVtbl -> GetVersion(This,MajorVersion,MinorVersion) ) 

#define IDaSvrTTSEngine_get_Gender(This,Gender)	\
    ( (This)->lpVtbl -> get_Gender(This,Gender) ) 

#define IDaSvrTTSEngine_get_LanguageID(This,LanguageID)	\
    ( (This)->lpVtbl -> get_LanguageID(This,LanguageID) ) 

#define IDaSvrTTSEngine_get_LanguageName(This,EnglishName,LanguageName)	\
    ( (This)->lpVtbl -> get_LanguageName(This,EnglishName,LanguageName) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrTTSEngine_INTERFACE_DEFINED__ */


#ifndef __IDaSvrTTSEngines_INTERFACE_DEFINED__
#define __IDaSvrTTSEngines_INTERFACE_DEFINED__

/* interface IDaSvrTTSEngines */
/* [object][custom][hidden][nonextensible][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IDaSvrTTSEngines;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E51A-A208-11DE-ABF2-002421116FB2")
    IDaSvrTTSEngines : public IDispatch
    {
    public:
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ long Index,
            /* [retval][out] */ IDaSvrTTSEngine **TTSEngine) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *Count) = 0;
        
        virtual /* [restricted][propget][id] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **ppunkEnum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrTTSEnginesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrTTSEngines * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrTTSEngines * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrTTSEngines * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrTTSEngines * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrTTSEngines * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrTTSEngines * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrTTSEngines * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IDaSvrTTSEngines * This,
            /* [in] */ long Index,
            /* [retval][out] */ IDaSvrTTSEngine **TTSEngine);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IDaSvrTTSEngines * This,
            /* [retval][out] */ long *Count);
        
        /* [restricted][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IDaSvrTTSEngines * This,
            /* [retval][out] */ IUnknown **ppunkEnum);
        
        END_INTERFACE
    } IDaSvrTTSEnginesVtbl;

    interface IDaSvrTTSEngines
    {
        CONST_VTBL struct IDaSvrTTSEnginesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrTTSEngines_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrTTSEngines_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrTTSEngines_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrTTSEngines_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrTTSEngines_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrTTSEngines_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrTTSEngines_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrTTSEngines_get_Item(This,Index,TTSEngine)	\
    ( (This)->lpVtbl -> get_Item(This,Index,TTSEngine) ) 

#define IDaSvrTTSEngines_get_Count(This,Count)	\
    ( (This)->lpVtbl -> get_Count(This,Count) ) 

#define IDaSvrTTSEngines_get__NewEnum(This,ppunkEnum)	\
    ( (This)->lpVtbl -> get__NewEnum(This,ppunkEnum) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrTTSEngines_INTERFACE_DEFINED__ */


#ifndef __IDaSvrSREngine_INTERFACE_DEFINED__
#define __IDaSvrSREngine_INTERFACE_DEFINED__

/* interface IDaSvrSREngine */
/* [object][hidden][nonextensible][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IDaSvrSREngine;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E51B-A208-11DE-ABF2-002421116FB2")
    IDaSvrSREngine : public IDispatch
    {
    public:
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_SRModeID( 
            /* [retval][out] */ BSTR *SRModeID) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_DisplayName( 
            /* [retval][out] */ BSTR *DisplayName) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Manufacturer( 
            /* [retval][out] */ BSTR *Manufacturer) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE GetVersion( 
            /* [out] */ short *MajorVersion,
            /* [out] */ short *MinorVersion) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_LanguageID( 
            /* [retval][out] */ long *LanguageID) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_LanguageName( 
            /* [defaultvalue][in] */ VARIANT_BOOL EnglishName,
            /* [retval][out] */ BSTR *LanguageName) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_LanguageIDs( 
            /* [retval][out] */ SAFEARRAY * *LanguageIDs) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_LanguageNames( 
            /* [defaultvalue][in] */ VARIANT_BOOL EnglishNames,
            /* [retval][out] */ SAFEARRAY * *LanguageNames) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrSREngineVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrSREngine * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrSREngine * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrSREngine * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrSREngine * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrSREngine * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrSREngine * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrSREngine * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SRModeID )( 
            IDaSvrSREngine * This,
            /* [retval][out] */ BSTR *SRModeID);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_DisplayName )( 
            IDaSvrSREngine * This,
            /* [retval][out] */ BSTR *DisplayName);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Manufacturer )( 
            IDaSvrSREngine * This,
            /* [retval][out] */ BSTR *Manufacturer);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVersion )( 
            IDaSvrSREngine * This,
            /* [out] */ short *MajorVersion,
            /* [out] */ short *MinorVersion);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_LanguageID )( 
            IDaSvrSREngine * This,
            /* [retval][out] */ long *LanguageID);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_LanguageName )( 
            IDaSvrSREngine * This,
            /* [defaultvalue][in] */ VARIANT_BOOL EnglishName,
            /* [retval][out] */ BSTR *LanguageName);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_LanguageIDs )( 
            IDaSvrSREngine * This,
            /* [retval][out] */ SAFEARRAY * *LanguageIDs);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_LanguageNames )( 
            IDaSvrSREngine * This,
            /* [defaultvalue][in] */ VARIANT_BOOL EnglishNames,
            /* [retval][out] */ SAFEARRAY * *LanguageNames);
        
        END_INTERFACE
    } IDaSvrSREngineVtbl;

    interface IDaSvrSREngine
    {
        CONST_VTBL struct IDaSvrSREngineVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrSREngine_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrSREngine_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrSREngine_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrSREngine_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrSREngine_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrSREngine_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrSREngine_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrSREngine_get_SRModeID(This,SRModeID)	\
    ( (This)->lpVtbl -> get_SRModeID(This,SRModeID) ) 

#define IDaSvrSREngine_get_DisplayName(This,DisplayName)	\
    ( (This)->lpVtbl -> get_DisplayName(This,DisplayName) ) 

#define IDaSvrSREngine_get_Manufacturer(This,Manufacturer)	\
    ( (This)->lpVtbl -> get_Manufacturer(This,Manufacturer) ) 

#define IDaSvrSREngine_GetVersion(This,MajorVersion,MinorVersion)	\
    ( (This)->lpVtbl -> GetVersion(This,MajorVersion,MinorVersion) ) 

#define IDaSvrSREngine_get_LanguageID(This,LanguageID)	\
    ( (This)->lpVtbl -> get_LanguageID(This,LanguageID) ) 

#define IDaSvrSREngine_get_LanguageName(This,EnglishName,LanguageName)	\
    ( (This)->lpVtbl -> get_LanguageName(This,EnglishName,LanguageName) ) 

#define IDaSvrSREngine_get_LanguageIDs(This,LanguageIDs)	\
    ( (This)->lpVtbl -> get_LanguageIDs(This,LanguageIDs) ) 

#define IDaSvrSREngine_get_LanguageNames(This,EnglishNames,LanguageNames)	\
    ( (This)->lpVtbl -> get_LanguageNames(This,EnglishNames,LanguageNames) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrSREngine_INTERFACE_DEFINED__ */


#ifndef __IDaSvrSREngines_INTERFACE_DEFINED__
#define __IDaSvrSREngines_INTERFACE_DEFINED__

/* interface IDaSvrSREngines */
/* [object][custom][hidden][nonextensible][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IDaSvrSREngines;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E51C-A208-11DE-ABF2-002421116FB2")
    IDaSvrSREngines : public IDispatch
    {
    public:
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ long Index,
            /* [retval][out] */ IDaSvrSREngine **SREngine) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *Count) = 0;
        
        virtual /* [restricted][propget][id] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **ppunkEnum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrSREnginesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrSREngines * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrSREngines * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrSREngines * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrSREngines * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrSREngines * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrSREngines * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrSREngines * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IDaSvrSREngines * This,
            /* [in] */ long Index,
            /* [retval][out] */ IDaSvrSREngine **SREngine);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IDaSvrSREngines * This,
            /* [retval][out] */ long *Count);
        
        /* [restricted][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IDaSvrSREngines * This,
            /* [retval][out] */ IUnknown **ppunkEnum);
        
        END_INTERFACE
    } IDaSvrSREnginesVtbl;

    interface IDaSvrSREngines
    {
        CONST_VTBL struct IDaSvrSREnginesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrSREngines_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrSREngines_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrSREngines_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrSREngines_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrSREngines_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrSREngines_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrSREngines_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrSREngines_get_Item(This,Index,SREngine)	\
    ( (This)->lpVtbl -> get_Item(This,Index,SREngine) ) 

#define IDaSvrSREngines_get_Count(This,Count)	\
    ( (This)->lpVtbl -> get_Count(This,Count) ) 

#define IDaSvrSREngines_get__NewEnum(This,ppunkEnum)	\
    ( (This)->lpVtbl -> get__NewEnum(This,ppunkEnum) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrSREngines_INTERFACE_DEFINED__ */


#ifndef __IDaServer2_INTERFACE_DEFINED__
#define __IDaServer2_INTERFACE_DEFINED__

/* interface IDaServer2 */
/* [object][hidden][nonextensible][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IDaServer2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E51D-A208-11DE-ABF2-002421116FB2")
    IDaServer2 : public IDaServer
    {
    public:
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Character( 
            /* [in] */ long CharacterID,
            /* [retval][out] */ IDaSvrCharacter2 **Character) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_CharacterFiles( 
            /* [retval][out] */ IDaSvrCharacterFiles **CharacterFiles) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_CharacterStyle( 
            /* [retval][out] */ long *CharacterStyle) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_CharacterStyle( 
            /* [in] */ long CharacterStyle) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_TTSEngines( 
            /* [retval][out] */ IDaSvrTTSEngines **TTSEngines) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE FindTTSEngines( 
            /* [in] */ long LanguageID,
            /* [in] */ short Gender,
            /* [retval][out] */ IDaSvrTTSEngines **TTSEngines) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE GetCharacterTTSEngine( 
            /* [in] */ VARIANT LoadKey,
            /* [retval][out] */ IDaSvrTTSEngine **TTSEngine) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE FindCharacterTTSEngines( 
            /* [in] */ VARIANT LoadKey,
            /* [defaultvalue][in] */ long LanguageID,
            /* [retval][out] */ IDaSvrTTSEngines **TTSEngines) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_SREngines( 
            /* [retval][out] */ IDaSvrSREngines **SREngines) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE FindSREngines( 
            /* [in] */ long LanguageID,
            /* [retval][out] */ IDaSvrSREngines **SREngines) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE GetCharacterSREngine( 
            /* [in] */ VARIANT LoadKey,
            /* [retval][out] */ IDaSvrSREngine **SREngine) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE FindCharacterSREngines( 
            /* [in] */ VARIANT LoadKey,
            /* [defaultvalue][in] */ long LanguageID,
            /* [retval][out] */ IDaSvrSREngines **SREngines) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_PropertySheet( 
            /* [retval][out] */ IDaSvrPropertySheet2 **PropertySheet) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_CommandsWindow( 
            /* [retval][out] */ IDaSvrCommandsWindow2 **CommandsWindow) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Settings( 
            /* [retval][out] */ IDaSvrSettings **Settings) = 0;
        
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
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Load )( 
            IDaServer2 * This,
            /* [in] */ VARIANT LoadKey,
            /* [out] */ long *CharacterID,
            /* [out] */ long *RequestID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Unload )( 
            IDaServer2 * This,
            /* [in] */ long CharacterID);
        
        /* [restricted][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Register )( 
            IDaServer2 * This,
            /* [in] */ IUnknown *NotifySink,
            /* [out] */ long *Cookie);
        
        /* [restricted][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Unregister )( 
            IDaServer2 * This,
            /* [in] */ long Cookie);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetCharacter )( 
            IDaServer2 * This,
            /* [in] */ long CharacterID,
            /* [out] */ IDispatch **Character);
        
        /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetSuspended )( 
            IDaServer2 * This,
            /* [out] */ long *Suspended);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetCharacterEx )( 
            IDaServer2 * This,
            /* [in] */ long CharacterID,
            /* [out] */ IDaSvrCharacter **Character);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVersion )( 
            IDaServer2 * This,
            /* [out] */ short *MajorVersion,
            /* [out] */ short *MinorVersion);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *ShowDefaultCharacterProperties )( 
            IDaServer2 * This,
            /* [in] */ short x,
            /* [in] */ short y,
            /* [in] */ long UseDefaultPosition);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Character )( 
            IDaServer2 * This,
            /* [in] */ long CharacterID,
            /* [retval][out] */ IDaSvrCharacter2 **Character);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_CharacterFiles )( 
            IDaServer2 * This,
            /* [retval][out] */ IDaSvrCharacterFiles **CharacterFiles);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_CharacterStyle )( 
            IDaServer2 * This,
            /* [retval][out] */ long *CharacterStyle);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_CharacterStyle )( 
            IDaServer2 * This,
            /* [in] */ long CharacterStyle);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_TTSEngines )( 
            IDaServer2 * This,
            /* [retval][out] */ IDaSvrTTSEngines **TTSEngines);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *FindTTSEngines )( 
            IDaServer2 * This,
            /* [in] */ long LanguageID,
            /* [in] */ short Gender,
            /* [retval][out] */ IDaSvrTTSEngines **TTSEngines);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetCharacterTTSEngine )( 
            IDaServer2 * This,
            /* [in] */ VARIANT LoadKey,
            /* [retval][out] */ IDaSvrTTSEngine **TTSEngine);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *FindCharacterTTSEngines )( 
            IDaServer2 * This,
            /* [in] */ VARIANT LoadKey,
            /* [defaultvalue][in] */ long LanguageID,
            /* [retval][out] */ IDaSvrTTSEngines **TTSEngines);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SREngines )( 
            IDaServer2 * This,
            /* [retval][out] */ IDaSvrSREngines **SREngines);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *FindSREngines )( 
            IDaServer2 * This,
            /* [in] */ long LanguageID,
            /* [retval][out] */ IDaSvrSREngines **SREngines);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetCharacterSREngine )( 
            IDaServer2 * This,
            /* [in] */ VARIANT LoadKey,
            /* [retval][out] */ IDaSvrSREngine **SREngine);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *FindCharacterSREngines )( 
            IDaServer2 * This,
            /* [in] */ VARIANT LoadKey,
            /* [defaultvalue][in] */ long LanguageID,
            /* [retval][out] */ IDaSvrSREngines **SREngines);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_PropertySheet )( 
            IDaServer2 * This,
            /* [retval][out] */ IDaSvrPropertySheet2 **PropertySheet);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_CommandsWindow )( 
            IDaServer2 * This,
            /* [retval][out] */ IDaSvrCommandsWindow2 **CommandsWindow);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Settings )( 
            IDaServer2 * This,
            /* [retval][out] */ IDaSvrSettings **Settings);
        
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


#define IDaServer2_Load(This,LoadKey,CharacterID,RequestID)	\
    ( (This)->lpVtbl -> Load(This,LoadKey,CharacterID,RequestID) ) 

#define IDaServer2_Unload(This,CharacterID)	\
    ( (This)->lpVtbl -> Unload(This,CharacterID) ) 

#define IDaServer2_Register(This,NotifySink,Cookie)	\
    ( (This)->lpVtbl -> Register(This,NotifySink,Cookie) ) 

#define IDaServer2_Unregister(This,Cookie)	\
    ( (This)->lpVtbl -> Unregister(This,Cookie) ) 

#define IDaServer2_GetCharacter(This,CharacterID,Character)	\
    ( (This)->lpVtbl -> GetCharacter(This,CharacterID,Character) ) 

#define IDaServer2_GetSuspended(This,Suspended)	\
    ( (This)->lpVtbl -> GetSuspended(This,Suspended) ) 

#define IDaServer2_GetCharacterEx(This,CharacterID,Character)	\
    ( (This)->lpVtbl -> GetCharacterEx(This,CharacterID,Character) ) 

#define IDaServer2_GetVersion(This,MajorVersion,MinorVersion)	\
    ( (This)->lpVtbl -> GetVersion(This,MajorVersion,MinorVersion) ) 

#define IDaServer2_ShowDefaultCharacterProperties(This,x,y,UseDefaultPosition)	\
    ( (This)->lpVtbl -> ShowDefaultCharacterProperties(This,x,y,UseDefaultPosition) ) 


#define IDaServer2_get_Character(This,CharacterID,Character)	\
    ( (This)->lpVtbl -> get_Character(This,CharacterID,Character) ) 

#define IDaServer2_get_CharacterFiles(This,CharacterFiles)	\
    ( (This)->lpVtbl -> get_CharacterFiles(This,CharacterFiles) ) 

#define IDaServer2_get_CharacterStyle(This,CharacterStyle)	\
    ( (This)->lpVtbl -> get_CharacterStyle(This,CharacterStyle) ) 

#define IDaServer2_put_CharacterStyle(This,CharacterStyle)	\
    ( (This)->lpVtbl -> put_CharacterStyle(This,CharacterStyle) ) 

#define IDaServer2_get_TTSEngines(This,TTSEngines)	\
    ( (This)->lpVtbl -> get_TTSEngines(This,TTSEngines) ) 

#define IDaServer2_FindTTSEngines(This,LanguageID,Gender,TTSEngines)	\
    ( (This)->lpVtbl -> FindTTSEngines(This,LanguageID,Gender,TTSEngines) ) 

#define IDaServer2_GetCharacterTTSEngine(This,LoadKey,TTSEngine)	\
    ( (This)->lpVtbl -> GetCharacterTTSEngine(This,LoadKey,TTSEngine) ) 

#define IDaServer2_FindCharacterTTSEngines(This,LoadKey,LanguageID,TTSEngines)	\
    ( (This)->lpVtbl -> FindCharacterTTSEngines(This,LoadKey,LanguageID,TTSEngines) ) 

#define IDaServer2_get_SREngines(This,SREngines)	\
    ( (This)->lpVtbl -> get_SREngines(This,SREngines) ) 

#define IDaServer2_FindSREngines(This,LanguageID,SREngines)	\
    ( (This)->lpVtbl -> FindSREngines(This,LanguageID,SREngines) ) 

#define IDaServer2_GetCharacterSREngine(This,LoadKey,SREngine)	\
    ( (This)->lpVtbl -> GetCharacterSREngine(This,LoadKey,SREngine) ) 

#define IDaServer2_FindCharacterSREngines(This,LoadKey,LanguageID,SREngines)	\
    ( (This)->lpVtbl -> FindCharacterSREngines(This,LoadKey,LanguageID,SREngines) ) 

#define IDaServer2_get_PropertySheet(This,PropertySheet)	\
    ( (This)->lpVtbl -> get_PropertySheet(This,PropertySheet) ) 

#define IDaServer2_get_CommandsWindow(This,CommandsWindow)	\
    ( (This)->lpVtbl -> get_CommandsWindow(This,CommandsWindow) ) 

#define IDaServer2_get_Settings(This,Settings)	\
    ( (This)->lpVtbl -> get_Settings(This,Settings) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaServer2_INTERFACE_DEFINED__ */


#ifndef __IDaSvrCharacter2_INTERFACE_DEFINED__
#define __IDaSvrCharacter2_INTERFACE_DEFINED__

/* interface IDaSvrCharacter2 */
/* [object][hidden][nonextensible][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IDaSvrCharacter2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E51E-A208-11DE-ABF2-002421116FB2")
    IDaSvrCharacter2 : public IDaSvrCharacter
    {
    public:
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Balloon( 
            /* [retval][out] */ IDaSvrBalloon2 **Balloon) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Commands( 
            /* [retval][out] */ IDaSvrCommands2 **Commands) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Style( 
            /* [retval][out] */ long *Style) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Style( 
            /* [in] */ long Style) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_HasIcon( 
            /* [retval][out] */ VARIANT_BOOL *HasIcon) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE GenerateIcon( 
            /* [defaultvalue][in] */ long ClipLeft = 0,
            /* [defaultvalue][in] */ long ClipTop = 0,
            /* [defaultvalue][in] */ long ClipWidth = -1,
            /* [defaultvalue][in] */ long ClipHeight = -1) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_IconVisible( 
            /* [retval][out] */ VARIANT_BOOL *IconVisible) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_IconIdentity( 
            /* [retval][out] */ BSTR *IconIdentity) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_IconIdentity( 
            /* [in] */ BSTR IconIdentity) = 0;
        
        virtual /* [restricted][hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE GetIconIdentity( 
            /* [out] */ GUID *IconIdentity) = 0;
        
        virtual /* [restricted][hidden][id][helpstring] */ HRESULT STDMETHODCALLTYPE SetIconIdentity( 
            /* [in] */ const GUID *IconIdentity) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_IconTip( 
            /* [retval][out] */ BSTR *IconTip) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_IconTip( 
            /* [in] */ BSTR IconTip) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_TTSEngine( 
            /* [defaultvalue][in] */ VARIANT_BOOL GetDefault,
            /* [retval][out] */ IDaSvrTTSEngine **TTSEngine) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE FindTTSEngines( 
            /* [defaultvalue][in] */ long LanguageID,
            /* [retval][out] */ IDaSvrTTSEngines **TTSEngines) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_SREngine( 
            /* [defaultvalue][in] */ VARIANT_BOOL GetDefault,
            /* [retval][out] */ IDaSvrSREngine **SREngine) = 0;
        
        virtual /* [id][helpstring] */ HRESULT STDMETHODCALLTYPE FindSREngines( 
            /* [defaultvalue][in] */ long LanguageID,
            /* [retval][out] */ IDaSvrSREngines **SREngines) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_CharacterID( 
            /* [retval][out] */ long *CharacterID) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_GUID( 
            /* [retval][out] */ BSTR *GUID) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *Name) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Name( 
            /* [in] */ BSTR Name) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Description( 
            /* [retval][out] */ BSTR *Description) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Description( 
            /* [in] */ BSTR Description) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_ExtraData( 
            /* [retval][out] */ BSTR *ExtraData) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_FileName( 
            /* [retval][out] */ BSTR *FileName) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_FilePath( 
            /* [retval][out] */ BSTR *FilePath) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_LanguageID( 
            /* [retval][out] */ long *LanguageID) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_LanguageID( 
            /* [in] */ long LanguageID) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_TTSModeID( 
            /* [retval][out] */ BSTR *TTSModeID) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_TTSModeID( 
            /* [in] */ BSTR TTSModeID) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_SRModeID( 
            /* [retval][out] */ BSTR *SRModeID) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_SRModeID( 
            /* [in] */ BSTR SRModeID) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Left( 
            /* [retval][out] */ short *Left) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Left( 
            /* [in] */ short Left) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Top( 
            /* [retval][out] */ short *Top) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Top( 
            /* [in] */ short Top) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Width( 
            /* [retval][out] */ short *Width) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Width( 
            /* [in] */ short Width) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Height( 
            /* [retval][out] */ short *Height) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Height( 
            /* [in] */ short Height) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_OriginalWidth( 
            /* [retval][out] */ short *OriginalWidth) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_OriginalHeight( 
            /* [retval][out] */ short *OriginalHeight) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Visible( 
            /* [retval][out] */ VARIANT_BOOL *Visible) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_ActiveState( 
            /* [retval][out] */ ActiveStateType *ActiveState) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_ActiveState( 
            /* [in] */ ActiveStateType ActiveState) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_IdleState( 
            /* [retval][out] */ VARIANT_BOOL *IdleState) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_OtherClientCount( 
            /* [retval][out] */ long *OtherClientCount) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_MoveCause( 
            /* [retval][out] */ MoveCauseType *MoveCause) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_VisibilityCause( 
            /* [retval][out] */ VisibilityCauseType *VisibilityCause) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_ListeningStatus( 
            /* [retval][out] */ ListeningStatusType *ListeningStatus) = 0;
        
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
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVisible )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *Visible);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetPosition )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long Left,
            /* [in] */ long Top);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetPosition )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *Left,
            /* [out] */ long *Top);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetSize )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long Width,
            /* [in] */ long Height);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetSize )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *Width,
            /* [out] */ long *Height);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetName )( 
            IDaSvrCharacter2 * This,
            /* [out] */ BSTR *Name);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetDescription )( 
            IDaSvrCharacter2 * This,
            /* [out] */ BSTR *Description);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetTTSSpeed )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *Speed);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetTTSPitch )( 
            IDaSvrCharacter2 * This,
            /* [out] */ short *Pitch);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Activate )( 
            IDaSvrCharacter2 * This,
            /* [defaultvalue][in] */ short State);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetIdleOn )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long On);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE *GetIdleOn )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *On);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Prepare )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long Type,
            /* [in] */ BSTR Name,
            /* [in] */ long Queue,
            /* [out] */ long *RequestID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Play )( 
            IDaSvrCharacter2 * This,
            /* [in] */ BSTR Animation,
            /* [out] */ long *RequestID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long RequestID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *StopAll )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long Types);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Wait )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long WaitForRequestID,
            /* [out] */ long *RequestID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Interrupt )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long InterruptRequestID,
            /* [out] */ long *RequestID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Show )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long Fast,
            /* [out] */ long *RequestID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Hide )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long Fast,
            /* [out] */ long *RequestID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Speak )( 
            IDaSvrCharacter2 * This,
            /* [in] */ BSTR Text,
            /* [in] */ BSTR Url,
            /* [out] */ long *RequestID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *MoveTo )( 
            IDaSvrCharacter2 * This,
            /* [in] */ short x,
            /* [in] */ short y,
            /* [in] */ long Speed,
            /* [out] */ long *RequestID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GestureAt )( 
            IDaSvrCharacter2 * This,
            /* [in] */ short x,
            /* [in] */ short y,
            /* [out] */ long *RequestID);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetMoveCause )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *Cause);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVisibilityCause )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *Cause);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *HasOtherClients )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *OtherClientCount);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetSoundEffectsOn )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long On);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetSoundEffectsOn )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *On);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetName )( 
            IDaSvrCharacter2 * This,
            /* [in] */ BSTR Name);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetDescription )( 
            IDaSvrCharacter2 * This,
            /* [in] */ BSTR Description);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetExtraData )( 
            IDaSvrCharacter2 * This,
            /* [out] */ BSTR *ExtraData);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *ShowPopupMenu )( 
            IDaSvrCharacter2 * This,
            /* [in] */ short x,
            /* [in] */ short y);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetAutoPopupMenu )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long AutoPopupMenu);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetAutoPopupMenu )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *AutoPopupMenu);
        
        /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetHelpFileName )( 
            IDaSvrCharacter2 * This,
            /* [out] */ BSTR *Name);
        
        /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetHelpFileName )( 
            IDaSvrCharacter2 * This,
            /* [in] */ BSTR Name);
        
        /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetHelpModeOn )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long HelpModeOn);
        
        /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetHelpModeOn )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *HelpModeOn);
        
        /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetHelpContextID )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long ContextID);
        
        /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetHelpContextID )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *ContextID);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetActive )( 
            IDaSvrCharacter2 * This,
            /* [out] */ short *State);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Listen )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long Listen);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetLanguageID )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long LanguageID);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetLanguageID )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *LanguageID);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetTTSModeID )( 
            IDaSvrCharacter2 * This,
            /* [out] */ BSTR *ModeID);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetTTSModeID )( 
            IDaSvrCharacter2 * This,
            /* [in] */ BSTR ModeID);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetSRModeID )( 
            IDaSvrCharacter2 * This,
            /* [out] */ BSTR *ModeID);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetSRModeID )( 
            IDaSvrCharacter2 * This,
            /* [in] */ BSTR ModeID);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetGUID )( 
            IDaSvrCharacter2 * This,
            /* [out] */ BSTR *GUID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetOriginalSize )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *Width,
            /* [out] */ long *Height);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Think )( 
            IDaSvrCharacter2 * This,
            /* [in] */ BSTR Text,
            /* [out] */ long *RequestID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVersion )( 
            IDaSvrCharacter2 * This,
            /* [out] */ short *MajorVersion,
            /* [out] */ short *MinorVersion);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetAnimationNames )( 
            IDaSvrCharacter2 * This,
            /* [out] */ IUnknown **punkEnum);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetSRStatus )( 
            IDaSvrCharacter2 * This,
            /* [out] */ long *Status);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Balloon )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ IDaSvrBalloon2 **Balloon);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Commands )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ IDaSvrCommands2 **Commands);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Style )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ long *Style);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Style )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long Style);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_HasIcon )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ VARIANT_BOOL *HasIcon);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GenerateIcon )( 
            IDaSvrCharacter2 * This,
            /* [defaultvalue][in] */ long ClipLeft,
            /* [defaultvalue][in] */ long ClipTop,
            /* [defaultvalue][in] */ long ClipWidth,
            /* [defaultvalue][in] */ long ClipHeight);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_IconVisible )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ VARIANT_BOOL *IconVisible);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_IconIdentity )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ BSTR *IconIdentity);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_IconIdentity )( 
            IDaSvrCharacter2 * This,
            /* [in] */ BSTR IconIdentity);
        
        /* [restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetIconIdentity )( 
            IDaSvrCharacter2 * This,
            /* [out] */ GUID *IconIdentity);
        
        /* [restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetIconIdentity )( 
            IDaSvrCharacter2 * This,
            /* [in] */ const GUID *IconIdentity);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_IconTip )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ BSTR *IconTip);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_IconTip )( 
            IDaSvrCharacter2 * This,
            /* [in] */ BSTR IconTip);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_TTSEngine )( 
            IDaSvrCharacter2 * This,
            /* [defaultvalue][in] */ VARIANT_BOOL GetDefault,
            /* [retval][out] */ IDaSvrTTSEngine **TTSEngine);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *FindTTSEngines )( 
            IDaSvrCharacter2 * This,
            /* [defaultvalue][in] */ long LanguageID,
            /* [retval][out] */ IDaSvrTTSEngines **TTSEngines);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SREngine )( 
            IDaSvrCharacter2 * This,
            /* [defaultvalue][in] */ VARIANT_BOOL GetDefault,
            /* [retval][out] */ IDaSvrSREngine **SREngine);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *FindSREngines )( 
            IDaSvrCharacter2 * This,
            /* [defaultvalue][in] */ long LanguageID,
            /* [retval][out] */ IDaSvrSREngines **SREngines);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_CharacterID )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ long *CharacterID);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_GUID )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ BSTR *GUID);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ BSTR *Name);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Name )( 
            IDaSvrCharacter2 * This,
            /* [in] */ BSTR Name);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Description )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ BSTR *Description);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Description )( 
            IDaSvrCharacter2 * This,
            /* [in] */ BSTR Description);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_ExtraData )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ BSTR *ExtraData);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_FileName )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ BSTR *FileName);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_FilePath )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ BSTR *FilePath);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_LanguageID )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ long *LanguageID);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_LanguageID )( 
            IDaSvrCharacter2 * This,
            /* [in] */ long LanguageID);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_TTSModeID )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ BSTR *TTSModeID);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_TTSModeID )( 
            IDaSvrCharacter2 * This,
            /* [in] */ BSTR TTSModeID);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SRModeID )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ BSTR *SRModeID);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_SRModeID )( 
            IDaSvrCharacter2 * This,
            /* [in] */ BSTR SRModeID);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Left )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ short *Left);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Left )( 
            IDaSvrCharacter2 * This,
            /* [in] */ short Left);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Top )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ short *Top);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Top )( 
            IDaSvrCharacter2 * This,
            /* [in] */ short Top);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ short *Width);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Width )( 
            IDaSvrCharacter2 * This,
            /* [in] */ short Width);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ short *Height);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Height )( 
            IDaSvrCharacter2 * This,
            /* [in] */ short Height);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_OriginalWidth )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ short *OriginalWidth);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_OriginalHeight )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ short *OriginalHeight);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ VARIANT_BOOL *Visible);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_ActiveState )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ ActiveStateType *ActiveState);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ActiveState )( 
            IDaSvrCharacter2 * This,
            /* [in] */ ActiveStateType ActiveState);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_IdleState )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ VARIANT_BOOL *IdleState);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_OtherClientCount )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ long *OtherClientCount);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_MoveCause )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ MoveCauseType *MoveCause);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_VisibilityCause )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ VisibilityCauseType *VisibilityCause);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_ListeningStatus )( 
            IDaSvrCharacter2 * This,
            /* [retval][out] */ ListeningStatusType *ListeningStatus);
        
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


#define IDaSvrCharacter2_GetVisible(This,Visible)	\
    ( (This)->lpVtbl -> GetVisible(This,Visible) ) 

#define IDaSvrCharacter2_SetPosition(This,Left,Top)	\
    ( (This)->lpVtbl -> SetPosition(This,Left,Top) ) 

#define IDaSvrCharacter2_GetPosition(This,Left,Top)	\
    ( (This)->lpVtbl -> GetPosition(This,Left,Top) ) 

#define IDaSvrCharacter2_SetSize(This,Width,Height)	\
    ( (This)->lpVtbl -> SetSize(This,Width,Height) ) 

#define IDaSvrCharacter2_GetSize(This,Width,Height)	\
    ( (This)->lpVtbl -> GetSize(This,Width,Height) ) 

#define IDaSvrCharacter2_GetName(This,Name)	\
    ( (This)->lpVtbl -> GetName(This,Name) ) 

#define IDaSvrCharacter2_GetDescription(This,Description)	\
    ( (This)->lpVtbl -> GetDescription(This,Description) ) 

#define IDaSvrCharacter2_GetTTSSpeed(This,Speed)	\
    ( (This)->lpVtbl -> GetTTSSpeed(This,Speed) ) 

#define IDaSvrCharacter2_GetTTSPitch(This,Pitch)	\
    ( (This)->lpVtbl -> GetTTSPitch(This,Pitch) ) 

#define IDaSvrCharacter2_Activate(This,State)	\
    ( (This)->lpVtbl -> Activate(This,State) ) 

#define IDaSvrCharacter2_SetIdleOn(This,On)	\
    ( (This)->lpVtbl -> SetIdleOn(This,On) ) 

#define IDaSvrCharacter2_GetIdleOn(This,On)	\
    ( (This)->lpVtbl -> GetIdleOn(This,On) ) 

#define IDaSvrCharacter2_Prepare(This,Type,Name,Queue,RequestID)	\
    ( (This)->lpVtbl -> Prepare(This,Type,Name,Queue,RequestID) ) 

#define IDaSvrCharacter2_Play(This,Animation,RequestID)	\
    ( (This)->lpVtbl -> Play(This,Animation,RequestID) ) 

#define IDaSvrCharacter2_Stop(This,RequestID)	\
    ( (This)->lpVtbl -> Stop(This,RequestID) ) 

#define IDaSvrCharacter2_StopAll(This,Types)	\
    ( (This)->lpVtbl -> StopAll(This,Types) ) 

#define IDaSvrCharacter2_Wait(This,WaitForRequestID,RequestID)	\
    ( (This)->lpVtbl -> Wait(This,WaitForRequestID,RequestID) ) 

#define IDaSvrCharacter2_Interrupt(This,InterruptRequestID,RequestID)	\
    ( (This)->lpVtbl -> Interrupt(This,InterruptRequestID,RequestID) ) 

#define IDaSvrCharacter2_Show(This,Fast,RequestID)	\
    ( (This)->lpVtbl -> Show(This,Fast,RequestID) ) 

#define IDaSvrCharacter2_Hide(This,Fast,RequestID)	\
    ( (This)->lpVtbl -> Hide(This,Fast,RequestID) ) 

#define IDaSvrCharacter2_Speak(This,Text,Url,RequestID)	\
    ( (This)->lpVtbl -> Speak(This,Text,Url,RequestID) ) 

#define IDaSvrCharacter2_MoveTo(This,x,y,Speed,RequestID)	\
    ( (This)->lpVtbl -> MoveTo(This,x,y,Speed,RequestID) ) 

#define IDaSvrCharacter2_GestureAt(This,x,y,RequestID)	\
    ( (This)->lpVtbl -> GestureAt(This,x,y,RequestID) ) 

#define IDaSvrCharacter2_GetMoveCause(This,Cause)	\
    ( (This)->lpVtbl -> GetMoveCause(This,Cause) ) 

#define IDaSvrCharacter2_GetVisibilityCause(This,Cause)	\
    ( (This)->lpVtbl -> GetVisibilityCause(This,Cause) ) 

#define IDaSvrCharacter2_HasOtherClients(This,OtherClientCount)	\
    ( (This)->lpVtbl -> HasOtherClients(This,OtherClientCount) ) 

#define IDaSvrCharacter2_SetSoundEffectsOn(This,On)	\
    ( (This)->lpVtbl -> SetSoundEffectsOn(This,On) ) 

#define IDaSvrCharacter2_GetSoundEffectsOn(This,On)	\
    ( (This)->lpVtbl -> GetSoundEffectsOn(This,On) ) 

#define IDaSvrCharacter2_SetName(This,Name)	\
    ( (This)->lpVtbl -> SetName(This,Name) ) 

#define IDaSvrCharacter2_SetDescription(This,Description)	\
    ( (This)->lpVtbl -> SetDescription(This,Description) ) 

#define IDaSvrCharacter2_GetExtraData(This,ExtraData)	\
    ( (This)->lpVtbl -> GetExtraData(This,ExtraData) ) 

#define IDaSvrCharacter2_ShowPopupMenu(This,x,y)	\
    ( (This)->lpVtbl -> ShowPopupMenu(This,x,y) ) 

#define IDaSvrCharacter2_SetAutoPopupMenu(This,AutoPopupMenu)	\
    ( (This)->lpVtbl -> SetAutoPopupMenu(This,AutoPopupMenu) ) 

#define IDaSvrCharacter2_GetAutoPopupMenu(This,AutoPopupMenu)	\
    ( (This)->lpVtbl -> GetAutoPopupMenu(This,AutoPopupMenu) ) 

#define IDaSvrCharacter2_GetHelpFileName(This,Name)	\
    ( (This)->lpVtbl -> GetHelpFileName(This,Name) ) 

#define IDaSvrCharacter2_SetHelpFileName(This,Name)	\
    ( (This)->lpVtbl -> SetHelpFileName(This,Name) ) 

#define IDaSvrCharacter2_SetHelpModeOn(This,HelpModeOn)	\
    ( (This)->lpVtbl -> SetHelpModeOn(This,HelpModeOn) ) 

#define IDaSvrCharacter2_GetHelpModeOn(This,HelpModeOn)	\
    ( (This)->lpVtbl -> GetHelpModeOn(This,HelpModeOn) ) 

#define IDaSvrCharacter2_SetHelpContextID(This,ContextID)	\
    ( (This)->lpVtbl -> SetHelpContextID(This,ContextID) ) 

#define IDaSvrCharacter2_GetHelpContextID(This,ContextID)	\
    ( (This)->lpVtbl -> GetHelpContextID(This,ContextID) ) 

#define IDaSvrCharacter2_GetActive(This,State)	\
    ( (This)->lpVtbl -> GetActive(This,State) ) 

#define IDaSvrCharacter2_Listen(This,Listen)	\
    ( (This)->lpVtbl -> Listen(This,Listen) ) 

#define IDaSvrCharacter2_SetLanguageID(This,LanguageID)	\
    ( (This)->lpVtbl -> SetLanguageID(This,LanguageID) ) 

#define IDaSvrCharacter2_GetLanguageID(This,LanguageID)	\
    ( (This)->lpVtbl -> GetLanguageID(This,LanguageID) ) 

#define IDaSvrCharacter2_GetTTSModeID(This,ModeID)	\
    ( (This)->lpVtbl -> GetTTSModeID(This,ModeID) ) 

#define IDaSvrCharacter2_SetTTSModeID(This,ModeID)	\
    ( (This)->lpVtbl -> SetTTSModeID(This,ModeID) ) 

#define IDaSvrCharacter2_GetSRModeID(This,ModeID)	\
    ( (This)->lpVtbl -> GetSRModeID(This,ModeID) ) 

#define IDaSvrCharacter2_SetSRModeID(This,ModeID)	\
    ( (This)->lpVtbl -> SetSRModeID(This,ModeID) ) 

#define IDaSvrCharacter2_GetGUID(This,GUID)	\
    ( (This)->lpVtbl -> GetGUID(This,GUID) ) 

#define IDaSvrCharacter2_GetOriginalSize(This,Width,Height)	\
    ( (This)->lpVtbl -> GetOriginalSize(This,Width,Height) ) 

#define IDaSvrCharacter2_Think(This,Text,RequestID)	\
    ( (This)->lpVtbl -> Think(This,Text,RequestID) ) 

#define IDaSvrCharacter2_GetVersion(This,MajorVersion,MinorVersion)	\
    ( (This)->lpVtbl -> GetVersion(This,MajorVersion,MinorVersion) ) 

#define IDaSvrCharacter2_GetAnimationNames(This,punkEnum)	\
    ( (This)->lpVtbl -> GetAnimationNames(This,punkEnum) ) 

#define IDaSvrCharacter2_GetSRStatus(This,Status)	\
    ( (This)->lpVtbl -> GetSRStatus(This,Status) ) 


#define IDaSvrCharacter2_get_Balloon(This,Balloon)	\
    ( (This)->lpVtbl -> get_Balloon(This,Balloon) ) 

#define IDaSvrCharacter2_get_Commands(This,Commands)	\
    ( (This)->lpVtbl -> get_Commands(This,Commands) ) 

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

#define IDaSvrCharacter2_put_IconIdentity(This,IconIdentity)	\
    ( (This)->lpVtbl -> put_IconIdentity(This,IconIdentity) ) 

#define IDaSvrCharacter2_GetIconIdentity(This,IconIdentity)	\
    ( (This)->lpVtbl -> GetIconIdentity(This,IconIdentity) ) 

#define IDaSvrCharacter2_SetIconIdentity(This,IconIdentity)	\
    ( (This)->lpVtbl -> SetIconIdentity(This,IconIdentity) ) 

#define IDaSvrCharacter2_get_IconTip(This,IconTip)	\
    ( (This)->lpVtbl -> get_IconTip(This,IconTip) ) 

#define IDaSvrCharacter2_put_IconTip(This,IconTip)	\
    ( (This)->lpVtbl -> put_IconTip(This,IconTip) ) 

#define IDaSvrCharacter2_get_TTSEngine(This,GetDefault,TTSEngine)	\
    ( (This)->lpVtbl -> get_TTSEngine(This,GetDefault,TTSEngine) ) 

#define IDaSvrCharacter2_FindTTSEngines(This,LanguageID,TTSEngines)	\
    ( (This)->lpVtbl -> FindTTSEngines(This,LanguageID,TTSEngines) ) 

#define IDaSvrCharacter2_get_SREngine(This,GetDefault,SREngine)	\
    ( (This)->lpVtbl -> get_SREngine(This,GetDefault,SREngine) ) 

#define IDaSvrCharacter2_FindSREngines(This,LanguageID,SREngines)	\
    ( (This)->lpVtbl -> FindSREngines(This,LanguageID,SREngines) ) 

#define IDaSvrCharacter2_get_CharacterID(This,CharacterID)	\
    ( (This)->lpVtbl -> get_CharacterID(This,CharacterID) ) 

#define IDaSvrCharacter2_get_GUID(This,GUID)	\
    ( (This)->lpVtbl -> get_GUID(This,GUID) ) 

#define IDaSvrCharacter2_get_Name(This,Name)	\
    ( (This)->lpVtbl -> get_Name(This,Name) ) 

#define IDaSvrCharacter2_put_Name(This,Name)	\
    ( (This)->lpVtbl -> put_Name(This,Name) ) 

#define IDaSvrCharacter2_get_Description(This,Description)	\
    ( (This)->lpVtbl -> get_Description(This,Description) ) 

#define IDaSvrCharacter2_put_Description(This,Description)	\
    ( (This)->lpVtbl -> put_Description(This,Description) ) 

#define IDaSvrCharacter2_get_ExtraData(This,ExtraData)	\
    ( (This)->lpVtbl -> get_ExtraData(This,ExtraData) ) 

#define IDaSvrCharacter2_get_FileName(This,FileName)	\
    ( (This)->lpVtbl -> get_FileName(This,FileName) ) 

#define IDaSvrCharacter2_get_FilePath(This,FilePath)	\
    ( (This)->lpVtbl -> get_FilePath(This,FilePath) ) 

#define IDaSvrCharacter2_get_LanguageID(This,LanguageID)	\
    ( (This)->lpVtbl -> get_LanguageID(This,LanguageID) ) 

#define IDaSvrCharacter2_put_LanguageID(This,LanguageID)	\
    ( (This)->lpVtbl -> put_LanguageID(This,LanguageID) ) 

#define IDaSvrCharacter2_get_TTSModeID(This,TTSModeID)	\
    ( (This)->lpVtbl -> get_TTSModeID(This,TTSModeID) ) 

#define IDaSvrCharacter2_put_TTSModeID(This,TTSModeID)	\
    ( (This)->lpVtbl -> put_TTSModeID(This,TTSModeID) ) 

#define IDaSvrCharacter2_get_SRModeID(This,SRModeID)	\
    ( (This)->lpVtbl -> get_SRModeID(This,SRModeID) ) 

#define IDaSvrCharacter2_put_SRModeID(This,SRModeID)	\
    ( (This)->lpVtbl -> put_SRModeID(This,SRModeID) ) 

#define IDaSvrCharacter2_get_Left(This,Left)	\
    ( (This)->lpVtbl -> get_Left(This,Left) ) 

#define IDaSvrCharacter2_put_Left(This,Left)	\
    ( (This)->lpVtbl -> put_Left(This,Left) ) 

#define IDaSvrCharacter2_get_Top(This,Top)	\
    ( (This)->lpVtbl -> get_Top(This,Top) ) 

#define IDaSvrCharacter2_put_Top(This,Top)	\
    ( (This)->lpVtbl -> put_Top(This,Top) ) 

#define IDaSvrCharacter2_get_Width(This,Width)	\
    ( (This)->lpVtbl -> get_Width(This,Width) ) 

#define IDaSvrCharacter2_put_Width(This,Width)	\
    ( (This)->lpVtbl -> put_Width(This,Width) ) 

#define IDaSvrCharacter2_get_Height(This,Height)	\
    ( (This)->lpVtbl -> get_Height(This,Height) ) 

#define IDaSvrCharacter2_put_Height(This,Height)	\
    ( (This)->lpVtbl -> put_Height(This,Height) ) 

#define IDaSvrCharacter2_get_OriginalWidth(This,OriginalWidth)	\
    ( (This)->lpVtbl -> get_OriginalWidth(This,OriginalWidth) ) 

#define IDaSvrCharacter2_get_OriginalHeight(This,OriginalHeight)	\
    ( (This)->lpVtbl -> get_OriginalHeight(This,OriginalHeight) ) 

#define IDaSvrCharacter2_get_Visible(This,Visible)	\
    ( (This)->lpVtbl -> get_Visible(This,Visible) ) 

#define IDaSvrCharacter2_get_ActiveState(This,ActiveState)	\
    ( (This)->lpVtbl -> get_ActiveState(This,ActiveState) ) 

#define IDaSvrCharacter2_put_ActiveState(This,ActiveState)	\
    ( (This)->lpVtbl -> put_ActiveState(This,ActiveState) ) 

#define IDaSvrCharacter2_get_IdleState(This,IdleState)	\
    ( (This)->lpVtbl -> get_IdleState(This,IdleState) ) 

#define IDaSvrCharacter2_get_OtherClientCount(This,OtherClientCount)	\
    ( (This)->lpVtbl -> get_OtherClientCount(This,OtherClientCount) ) 

#define IDaSvrCharacter2_get_MoveCause(This,MoveCause)	\
    ( (This)->lpVtbl -> get_MoveCause(This,MoveCause) ) 

#define IDaSvrCharacter2_get_VisibilityCause(This,VisibilityCause)	\
    ( (This)->lpVtbl -> get_VisibilityCause(This,VisibilityCause) ) 

#define IDaSvrCharacter2_get_ListeningStatus(This,ListeningStatus)	\
    ( (This)->lpVtbl -> get_ListeningStatus(This,ListeningStatus) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrCharacter2_INTERFACE_DEFINED__ */


#ifndef __IDaSvrBalloon2_INTERFACE_DEFINED__
#define __IDaSvrBalloon2_INTERFACE_DEFINED__

/* interface IDaSvrBalloon2 */
/* [object][hidden][nonextensible][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IDaSvrBalloon2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E526-A208-11DE-ABF2-002421116FB2")
    IDaSvrBalloon2 : public IDaSvrBalloon
    {
    public:
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Enabled( 
            /* [retval][out] */ VARIANT_BOOL *Enabled) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Style( 
            /* [retval][out] */ long *Style) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Style( 
            /* [in] */ long Style) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Visible( 
            /* [retval][out] */ VARIANT_BOOL *Visible) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Visible( 
            /* [in] */ VARIANT_BOOL Visible) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_NumberOfLines( 
            /* [retval][out] */ long *NumberOfLines) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_NumberOfLines( 
            /* [in] */ long NumberOfLines) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_CharsPerLine( 
            /* [retval][out] */ long *CharsPerLine) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_CharsPerLine( 
            /* [in] */ long CharsPerLine) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_TextColor( 
            /* [retval][out] */ long *TextColor) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_TextColor( 
            /* [in] */ long TextColor) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_BackColor( 
            /* [retval][out] */ long *BackColor) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_BackColor( 
            /* [in] */ long BackColor) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_BorderColor( 
            /* [retval][out] */ long *BorderColor) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_BorderColor( 
            /* [in] */ long BorderColor) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_FontName( 
            /* [retval][out] */ BSTR *FontName) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_FontName( 
            /* [in] */ BSTR FontName) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_FontSize( 
            /* [retval][out] */ long *FontSize) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_FontSize( 
            /* [in] */ long FontSize) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_FontBold( 
            /* [retval][out] */ VARIANT_BOOL *FontBold) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_FontBold( 
            /* [in] */ VARIANT_BOOL FontBold) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_FontItalic( 
            /* [retval][out] */ VARIANT_BOOL *FontItalic) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_FontItalic( 
            /* [in] */ VARIANT_BOOL FontItalic) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_FontStrikethru( 
            /* [retval][out] */ VARIANT_BOOL *FontStrikethru) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_FontStrikethru( 
            /* [in] */ VARIANT_BOOL FontStrikethru) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_FontUnderline( 
            /* [retval][out] */ VARIANT_BOOL *FontUnderline) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_FontUnderline( 
            /* [in] */ VARIANT_BOOL FontUnderline) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_FontCharSet( 
            /* [retval][out] */ short *FontCharSet) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_FontCharSet( 
            /* [in] */ short FontCharSet) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrBalloon2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrBalloon2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrBalloon2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrBalloon2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrBalloon2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrBalloon2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrBalloon2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrBalloon2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEnabled )( 
            IDaSvrBalloon2 * This,
            /* [out] */ long *Enabled);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetNumLines )( 
            IDaSvrBalloon2 * This,
            /* [out] */ long *Lines);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetNumCharsPerLine )( 
            IDaSvrBalloon2 * This,
            /* [out] */ long *CharsPerLine);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetFontName )( 
            IDaSvrBalloon2 * This,
            /* [out] */ BSTR *FontName);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetFontSize )( 
            IDaSvrBalloon2 * This,
            /* [out] */ long *FontSize);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetFontBold )( 
            IDaSvrBalloon2 * This,
            /* [out] */ long *FontBold);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetFontItalic )( 
            IDaSvrBalloon2 * This,
            /* [out] */ long *FontItalic);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetFontStrikethru )( 
            IDaSvrBalloon2 * This,
            /* [out] */ long *FontStrikethru);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetFontUnderline )( 
            IDaSvrBalloon2 * This,
            /* [out] */ long *FontUnderline);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetForeColor )( 
            IDaSvrBalloon2 * This,
            /* [out] */ long *ForeColor);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetBackColor )( 
            IDaSvrBalloon2 * This,
            /* [out] */ long *BackColor);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetBorderColor )( 
            IDaSvrBalloon2 * This,
            /* [out] */ long *BorderColor);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetVisible )( 
            IDaSvrBalloon2 * This,
            /* [in] */ long Visible);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVisible )( 
            IDaSvrBalloon2 * This,
            /* [out] */ long *Visible);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetFontName )( 
            IDaSvrBalloon2 * This,
            /* [in] */ BSTR FontName);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetFontSize )( 
            IDaSvrBalloon2 * This,
            /* [in] */ long FontSize);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetFontCharSet )( 
            IDaSvrBalloon2 * This,
            /* [in] */ short FontCharSet);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetFontCharSet )( 
            IDaSvrBalloon2 * This,
            /* [out] */ short *FontCharSet);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetStyle )( 
            IDaSvrBalloon2 * This,
            /* [in] */ long Style);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetStyle )( 
            IDaSvrBalloon2 * This,
            /* [out] */ long *Style);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetNumLines )( 
            IDaSvrBalloon2 * This,
            /* [in] */ long Lines);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetNumCharsPerLine )( 
            IDaSvrBalloon2 * This,
            /* [in] */ long CharsPerLine);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IDaSvrBalloon2 * This,
            /* [retval][out] */ VARIANT_BOOL *Enabled);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Style )( 
            IDaSvrBalloon2 * This,
            /* [retval][out] */ long *Style);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Style )( 
            IDaSvrBalloon2 * This,
            /* [in] */ long Style);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IDaSvrBalloon2 * This,
            /* [retval][out] */ VARIANT_BOOL *Visible);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IDaSvrBalloon2 * This,
            /* [in] */ VARIANT_BOOL Visible);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_NumberOfLines )( 
            IDaSvrBalloon2 * This,
            /* [retval][out] */ long *NumberOfLines);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_NumberOfLines )( 
            IDaSvrBalloon2 * This,
            /* [in] */ long NumberOfLines);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_CharsPerLine )( 
            IDaSvrBalloon2 * This,
            /* [retval][out] */ long *CharsPerLine);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_CharsPerLine )( 
            IDaSvrBalloon2 * This,
            /* [in] */ long CharsPerLine);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_TextColor )( 
            IDaSvrBalloon2 * This,
            /* [retval][out] */ long *TextColor);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_TextColor )( 
            IDaSvrBalloon2 * This,
            /* [in] */ long TextColor);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_BackColor )( 
            IDaSvrBalloon2 * This,
            /* [retval][out] */ long *BackColor);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_BackColor )( 
            IDaSvrBalloon2 * This,
            /* [in] */ long BackColor);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_BorderColor )( 
            IDaSvrBalloon2 * This,
            /* [retval][out] */ long *BorderColor);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_BorderColor )( 
            IDaSvrBalloon2 * This,
            /* [in] */ long BorderColor);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_FontName )( 
            IDaSvrBalloon2 * This,
            /* [retval][out] */ BSTR *FontName);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FontName )( 
            IDaSvrBalloon2 * This,
            /* [in] */ BSTR FontName);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_FontSize )( 
            IDaSvrBalloon2 * This,
            /* [retval][out] */ long *FontSize);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FontSize )( 
            IDaSvrBalloon2 * This,
            /* [in] */ long FontSize);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_FontBold )( 
            IDaSvrBalloon2 * This,
            /* [retval][out] */ VARIANT_BOOL *FontBold);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FontBold )( 
            IDaSvrBalloon2 * This,
            /* [in] */ VARIANT_BOOL FontBold);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_FontItalic )( 
            IDaSvrBalloon2 * This,
            /* [retval][out] */ VARIANT_BOOL *FontItalic);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FontItalic )( 
            IDaSvrBalloon2 * This,
            /* [in] */ VARIANT_BOOL FontItalic);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_FontStrikethru )( 
            IDaSvrBalloon2 * This,
            /* [retval][out] */ VARIANT_BOOL *FontStrikethru);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FontStrikethru )( 
            IDaSvrBalloon2 * This,
            /* [in] */ VARIANT_BOOL FontStrikethru);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_FontUnderline )( 
            IDaSvrBalloon2 * This,
            /* [retval][out] */ VARIANT_BOOL *FontUnderline);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FontUnderline )( 
            IDaSvrBalloon2 * This,
            /* [in] */ VARIANT_BOOL FontUnderline);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_FontCharSet )( 
            IDaSvrBalloon2 * This,
            /* [retval][out] */ short *FontCharSet);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FontCharSet )( 
            IDaSvrBalloon2 * This,
            /* [in] */ short FontCharSet);
        
        END_INTERFACE
    } IDaSvrBalloon2Vtbl;

    interface IDaSvrBalloon2
    {
        CONST_VTBL struct IDaSvrBalloon2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrBalloon2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrBalloon2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrBalloon2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrBalloon2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrBalloon2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrBalloon2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrBalloon2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrBalloon2_GetEnabled(This,Enabled)	\
    ( (This)->lpVtbl -> GetEnabled(This,Enabled) ) 

#define IDaSvrBalloon2_GetNumLines(This,Lines)	\
    ( (This)->lpVtbl -> GetNumLines(This,Lines) ) 

#define IDaSvrBalloon2_GetNumCharsPerLine(This,CharsPerLine)	\
    ( (This)->lpVtbl -> GetNumCharsPerLine(This,CharsPerLine) ) 

#define IDaSvrBalloon2_GetFontName(This,FontName)	\
    ( (This)->lpVtbl -> GetFontName(This,FontName) ) 

#define IDaSvrBalloon2_GetFontSize(This,FontSize)	\
    ( (This)->lpVtbl -> GetFontSize(This,FontSize) ) 

#define IDaSvrBalloon2_GetFontBold(This,FontBold)	\
    ( (This)->lpVtbl -> GetFontBold(This,FontBold) ) 

#define IDaSvrBalloon2_GetFontItalic(This,FontItalic)	\
    ( (This)->lpVtbl -> GetFontItalic(This,FontItalic) ) 

#define IDaSvrBalloon2_GetFontStrikethru(This,FontStrikethru)	\
    ( (This)->lpVtbl -> GetFontStrikethru(This,FontStrikethru) ) 

#define IDaSvrBalloon2_GetFontUnderline(This,FontUnderline)	\
    ( (This)->lpVtbl -> GetFontUnderline(This,FontUnderline) ) 

#define IDaSvrBalloon2_GetForeColor(This,ForeColor)	\
    ( (This)->lpVtbl -> GetForeColor(This,ForeColor) ) 

#define IDaSvrBalloon2_GetBackColor(This,BackColor)	\
    ( (This)->lpVtbl -> GetBackColor(This,BackColor) ) 

#define IDaSvrBalloon2_GetBorderColor(This,BorderColor)	\
    ( (This)->lpVtbl -> GetBorderColor(This,BorderColor) ) 

#define IDaSvrBalloon2_SetVisible(This,Visible)	\
    ( (This)->lpVtbl -> SetVisible(This,Visible) ) 

#define IDaSvrBalloon2_GetVisible(This,Visible)	\
    ( (This)->lpVtbl -> GetVisible(This,Visible) ) 

#define IDaSvrBalloon2_SetFontName(This,FontName)	\
    ( (This)->lpVtbl -> SetFontName(This,FontName) ) 

#define IDaSvrBalloon2_SetFontSize(This,FontSize)	\
    ( (This)->lpVtbl -> SetFontSize(This,FontSize) ) 

#define IDaSvrBalloon2_SetFontCharSet(This,FontCharSet)	\
    ( (This)->lpVtbl -> SetFontCharSet(This,FontCharSet) ) 

#define IDaSvrBalloon2_GetFontCharSet(This,FontCharSet)	\
    ( (This)->lpVtbl -> GetFontCharSet(This,FontCharSet) ) 

#define IDaSvrBalloon2_SetStyle(This,Style)	\
    ( (This)->lpVtbl -> SetStyle(This,Style) ) 

#define IDaSvrBalloon2_GetStyle(This,Style)	\
    ( (This)->lpVtbl -> GetStyle(This,Style) ) 

#define IDaSvrBalloon2_SetNumLines(This,Lines)	\
    ( (This)->lpVtbl -> SetNumLines(This,Lines) ) 

#define IDaSvrBalloon2_SetNumCharsPerLine(This,CharsPerLine)	\
    ( (This)->lpVtbl -> SetNumCharsPerLine(This,CharsPerLine) ) 


#define IDaSvrBalloon2_get_Enabled(This,Enabled)	\
    ( (This)->lpVtbl -> get_Enabled(This,Enabled) ) 

#define IDaSvrBalloon2_get_Style(This,Style)	\
    ( (This)->lpVtbl -> get_Style(This,Style) ) 

#define IDaSvrBalloon2_put_Style(This,Style)	\
    ( (This)->lpVtbl -> put_Style(This,Style) ) 

#define IDaSvrBalloon2_get_Visible(This,Visible)	\
    ( (This)->lpVtbl -> get_Visible(This,Visible) ) 

#define IDaSvrBalloon2_put_Visible(This,Visible)	\
    ( (This)->lpVtbl -> put_Visible(This,Visible) ) 

#define IDaSvrBalloon2_get_NumberOfLines(This,NumberOfLines)	\
    ( (This)->lpVtbl -> get_NumberOfLines(This,NumberOfLines) ) 

#define IDaSvrBalloon2_put_NumberOfLines(This,NumberOfLines)	\
    ( (This)->lpVtbl -> put_NumberOfLines(This,NumberOfLines) ) 

#define IDaSvrBalloon2_get_CharsPerLine(This,CharsPerLine)	\
    ( (This)->lpVtbl -> get_CharsPerLine(This,CharsPerLine) ) 

#define IDaSvrBalloon2_put_CharsPerLine(This,CharsPerLine)	\
    ( (This)->lpVtbl -> put_CharsPerLine(This,CharsPerLine) ) 

#define IDaSvrBalloon2_get_TextColor(This,TextColor)	\
    ( (This)->lpVtbl -> get_TextColor(This,TextColor) ) 

#define IDaSvrBalloon2_put_TextColor(This,TextColor)	\
    ( (This)->lpVtbl -> put_TextColor(This,TextColor) ) 

#define IDaSvrBalloon2_get_BackColor(This,BackColor)	\
    ( (This)->lpVtbl -> get_BackColor(This,BackColor) ) 

#define IDaSvrBalloon2_put_BackColor(This,BackColor)	\
    ( (This)->lpVtbl -> put_BackColor(This,BackColor) ) 

#define IDaSvrBalloon2_get_BorderColor(This,BorderColor)	\
    ( (This)->lpVtbl -> get_BorderColor(This,BorderColor) ) 

#define IDaSvrBalloon2_put_BorderColor(This,BorderColor)	\
    ( (This)->lpVtbl -> put_BorderColor(This,BorderColor) ) 

#define IDaSvrBalloon2_get_FontName(This,FontName)	\
    ( (This)->lpVtbl -> get_FontName(This,FontName) ) 

#define IDaSvrBalloon2_put_FontName(This,FontName)	\
    ( (This)->lpVtbl -> put_FontName(This,FontName) ) 

#define IDaSvrBalloon2_get_FontSize(This,FontSize)	\
    ( (This)->lpVtbl -> get_FontSize(This,FontSize) ) 

#define IDaSvrBalloon2_put_FontSize(This,FontSize)	\
    ( (This)->lpVtbl -> put_FontSize(This,FontSize) ) 

#define IDaSvrBalloon2_get_FontBold(This,FontBold)	\
    ( (This)->lpVtbl -> get_FontBold(This,FontBold) ) 

#define IDaSvrBalloon2_put_FontBold(This,FontBold)	\
    ( (This)->lpVtbl -> put_FontBold(This,FontBold) ) 

#define IDaSvrBalloon2_get_FontItalic(This,FontItalic)	\
    ( (This)->lpVtbl -> get_FontItalic(This,FontItalic) ) 

#define IDaSvrBalloon2_put_FontItalic(This,FontItalic)	\
    ( (This)->lpVtbl -> put_FontItalic(This,FontItalic) ) 

#define IDaSvrBalloon2_get_FontStrikethru(This,FontStrikethru)	\
    ( (This)->lpVtbl -> get_FontStrikethru(This,FontStrikethru) ) 

#define IDaSvrBalloon2_put_FontStrikethru(This,FontStrikethru)	\
    ( (This)->lpVtbl -> put_FontStrikethru(This,FontStrikethru) ) 

#define IDaSvrBalloon2_get_FontUnderline(This,FontUnderline)	\
    ( (This)->lpVtbl -> get_FontUnderline(This,FontUnderline) ) 

#define IDaSvrBalloon2_put_FontUnderline(This,FontUnderline)	\
    ( (This)->lpVtbl -> put_FontUnderline(This,FontUnderline) ) 

#define IDaSvrBalloon2_get_FontCharSet(This,FontCharSet)	\
    ( (This)->lpVtbl -> get_FontCharSet(This,FontCharSet) ) 

#define IDaSvrBalloon2_put_FontCharSet(This,FontCharSet)	\
    ( (This)->lpVtbl -> put_FontCharSet(This,FontCharSet) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrBalloon2_INTERFACE_DEFINED__ */


#ifndef __IDaSvrPropertySheet2_INTERFACE_DEFINED__
#define __IDaSvrPropertySheet2_INTERFACE_DEFINED__

/* interface IDaSvrPropertySheet2 */
/* [object][hidden][nonextensible][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IDaSvrPropertySheet2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E525-A208-11DE-ABF2-002421116FB2")
    IDaSvrPropertySheet2 : public IDaSvrPropertySheet
    {
    public:
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Left( 
            /* [retval][out] */ short *Left) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Top( 
            /* [retval][out] */ short *Top) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Height( 
            /* [retval][out] */ short *Height) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Width( 
            /* [retval][out] */ short *Width) = 0;
        
        virtual /* [propput][id][helpstring] */ HRESULT STDMETHODCALLTYPE put_Visible( 
            /* [in] */ VARIANT_BOOL Visible) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Visible( 
            /* [retval][out] */ VARIANT_BOOL *Visible) = 0;
        
        virtual /* [propput][id][helpstring] */ HRESULT STDMETHODCALLTYPE put_Page( 
            /* [in] */ BSTR Page) = 0;
        
        virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Page( 
            /* [retval][out] */ BSTR *Page) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrPropertySheet2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrPropertySheet2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrPropertySheet2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrPropertySheet2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrPropertySheet2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrPropertySheet2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrPropertySheet2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrPropertySheet2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVisible )( 
            IDaSvrPropertySheet2 * This,
            /* [out] */ long *Visible);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetVisible )( 
            IDaSvrPropertySheet2 * This,
            /* [in] */ long Visible);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetPosition )( 
            IDaSvrPropertySheet2 * This,
            /* [out] */ long *Left,
            /* [out] */ long *Top);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetSize )( 
            IDaSvrPropertySheet2 * This,
            /* [out] */ long *Width,
            /* [out] */ long *Height);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetPage )( 
            IDaSvrPropertySheet2 * This,
            /* [out] */ BSTR *Page);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetPage )( 
            IDaSvrPropertySheet2 * This,
            /* [in] */ BSTR Page);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Left )( 
            IDaSvrPropertySheet2 * This,
            /* [retval][out] */ short *Left);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Top )( 
            IDaSvrPropertySheet2 * This,
            /* [retval][out] */ short *Top);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IDaSvrPropertySheet2 * This,
            /* [retval][out] */ short *Height);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IDaSvrPropertySheet2 * This,
            /* [retval][out] */ short *Width);
        
        /* [propput][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IDaSvrPropertySheet2 * This,
            /* [in] */ VARIANT_BOOL Visible);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IDaSvrPropertySheet2 * This,
            /* [retval][out] */ VARIANT_BOOL *Visible);
        
        /* [propput][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *put_Page )( 
            IDaSvrPropertySheet2 * This,
            /* [in] */ BSTR Page);
        
        /* [propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Page )( 
            IDaSvrPropertySheet2 * This,
            /* [retval][out] */ BSTR *Page);
        
        END_INTERFACE
    } IDaSvrPropertySheet2Vtbl;

    interface IDaSvrPropertySheet2
    {
        CONST_VTBL struct IDaSvrPropertySheet2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrPropertySheet2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrPropertySheet2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrPropertySheet2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrPropertySheet2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrPropertySheet2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrPropertySheet2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrPropertySheet2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrPropertySheet2_GetVisible(This,Visible)	\
    ( (This)->lpVtbl -> GetVisible(This,Visible) ) 

#define IDaSvrPropertySheet2_SetVisible(This,Visible)	\
    ( (This)->lpVtbl -> SetVisible(This,Visible) ) 

#define IDaSvrPropertySheet2_GetPosition(This,Left,Top)	\
    ( (This)->lpVtbl -> GetPosition(This,Left,Top) ) 

#define IDaSvrPropertySheet2_GetSize(This,Width,Height)	\
    ( (This)->lpVtbl -> GetSize(This,Width,Height) ) 

#define IDaSvrPropertySheet2_GetPage(This,Page)	\
    ( (This)->lpVtbl -> GetPage(This,Page) ) 

#define IDaSvrPropertySheet2_SetPage(This,Page)	\
    ( (This)->lpVtbl -> SetPage(This,Page) ) 


#define IDaSvrPropertySheet2_get_Left(This,Left)	\
    ( (This)->lpVtbl -> get_Left(This,Left) ) 

#define IDaSvrPropertySheet2_get_Top(This,Top)	\
    ( (This)->lpVtbl -> get_Top(This,Top) ) 

#define IDaSvrPropertySheet2_get_Height(This,Height)	\
    ( (This)->lpVtbl -> get_Height(This,Height) ) 

#define IDaSvrPropertySheet2_get_Width(This,Width)	\
    ( (This)->lpVtbl -> get_Width(This,Width) ) 

#define IDaSvrPropertySheet2_put_Visible(This,Visible)	\
    ( (This)->lpVtbl -> put_Visible(This,Visible) ) 

#define IDaSvrPropertySheet2_get_Visible(This,Visible)	\
    ( (This)->lpVtbl -> get_Visible(This,Visible) ) 

#define IDaSvrPropertySheet2_put_Page(This,Page)	\
    ( (This)->lpVtbl -> put_Page(This,Page) ) 

#define IDaSvrPropertySheet2_get_Page(This,Page)	\
    ( (This)->lpVtbl -> get_Page(This,Page) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrPropertySheet2_INTERFACE_DEFINED__ */


#ifndef __IDaSvrSettings_INTERFACE_DEFINED__
#define __IDaSvrSettings_INTERFACE_DEFINED__

/* interface IDaSvrSettings */
/* [object][hidden][nonextensible][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IDaSvrSettings;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E527-A208-11DE-ABF2-002421116FB2")
    IDaSvrSettings : public IDispatch
    {
    public:
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_SoundEffectsEnabled( 
            /* [retval][out] */ VARIANT_BOOL *SoundEffectsEnabled) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_BalloonEnabled( 
            /* [retval][out] */ VARIANT_BOOL *BalloonEnabled) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_BalloonFont( 
            /* [retval][out] */ /* external definition not present */ IFontDisp **BalloonFont) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_TTSEnabled( 
            /* [retval][out] */ VARIANT_BOOL *TTSEnabled) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_TTSSpeed( 
            /* [retval][out] */ short *TTSSpeed) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_SREnabled( 
            /* [retval][out] */ VARIANT_BOOL *SREnabled) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_SRHotKey( 
            /* [retval][out] */ BSTR *SRHotKey) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_SRHotKeyTime( 
            /* [retval][out] */ short *SRHotKeyTime) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_SRListeningTip( 
            /* [retval][out] */ VARIANT_BOOL *SRListeningTip) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_SRListeningPrompt( 
            /* [retval][out] */ VARIANT_BOOL *SRListeningPrompt) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_AudioStatus( 
            /* [retval][out] */ AudioStatusType *AudioStatus) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrSettingsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrSettings * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrSettings * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrSettings * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrSettings * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrSettings * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrSettings * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrSettings * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SoundEffectsEnabled )( 
            IDaSvrSettings * This,
            /* [retval][out] */ VARIANT_BOOL *SoundEffectsEnabled);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_BalloonEnabled )( 
            IDaSvrSettings * This,
            /* [retval][out] */ VARIANT_BOOL *BalloonEnabled);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_BalloonFont )( 
            IDaSvrSettings * This,
            /* [retval][out] */ /* external definition not present */ IFontDisp **BalloonFont);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_TTSEnabled )( 
            IDaSvrSettings * This,
            /* [retval][out] */ VARIANT_BOOL *TTSEnabled);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_TTSSpeed )( 
            IDaSvrSettings * This,
            /* [retval][out] */ short *TTSSpeed);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SREnabled )( 
            IDaSvrSettings * This,
            /* [retval][out] */ VARIANT_BOOL *SREnabled);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SRHotKey )( 
            IDaSvrSettings * This,
            /* [retval][out] */ BSTR *SRHotKey);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SRHotKeyTime )( 
            IDaSvrSettings * This,
            /* [retval][out] */ short *SRHotKeyTime);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SRListeningTip )( 
            IDaSvrSettings * This,
            /* [retval][out] */ VARIANT_BOOL *SRListeningTip);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_SRListeningPrompt )( 
            IDaSvrSettings * This,
            /* [retval][out] */ VARIANT_BOOL *SRListeningPrompt);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_AudioStatus )( 
            IDaSvrSettings * This,
            /* [retval][out] */ AudioStatusType *AudioStatus);
        
        END_INTERFACE
    } IDaSvrSettingsVtbl;

    interface IDaSvrSettings
    {
        CONST_VTBL struct IDaSvrSettingsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrSettings_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrSettings_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrSettings_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrSettings_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrSettings_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrSettings_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrSettings_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrSettings_get_SoundEffectsEnabled(This,SoundEffectsEnabled)	\
    ( (This)->lpVtbl -> get_SoundEffectsEnabled(This,SoundEffectsEnabled) ) 

#define IDaSvrSettings_get_BalloonEnabled(This,BalloonEnabled)	\
    ( (This)->lpVtbl -> get_BalloonEnabled(This,BalloonEnabled) ) 

#define IDaSvrSettings_get_BalloonFont(This,BalloonFont)	\
    ( (This)->lpVtbl -> get_BalloonFont(This,BalloonFont) ) 

#define IDaSvrSettings_get_TTSEnabled(This,TTSEnabled)	\
    ( (This)->lpVtbl -> get_TTSEnabled(This,TTSEnabled) ) 

#define IDaSvrSettings_get_TTSSpeed(This,TTSSpeed)	\
    ( (This)->lpVtbl -> get_TTSSpeed(This,TTSSpeed) ) 

#define IDaSvrSettings_get_SREnabled(This,SREnabled)	\
    ( (This)->lpVtbl -> get_SREnabled(This,SREnabled) ) 

#define IDaSvrSettings_get_SRHotKey(This,SRHotKey)	\
    ( (This)->lpVtbl -> get_SRHotKey(This,SRHotKey) ) 

#define IDaSvrSettings_get_SRHotKeyTime(This,SRHotKeyTime)	\
    ( (This)->lpVtbl -> get_SRHotKeyTime(This,SRHotKeyTime) ) 

#define IDaSvrSettings_get_SRListeningTip(This,SRListeningTip)	\
    ( (This)->lpVtbl -> get_SRListeningTip(This,SRListeningTip) ) 

#define IDaSvrSettings_get_SRListeningPrompt(This,SRListeningPrompt)	\
    ( (This)->lpVtbl -> get_SRListeningPrompt(This,SRListeningPrompt) ) 

#define IDaSvrSettings_get_AudioStatus(This,AudioStatus)	\
    ( (This)->lpVtbl -> get_AudioStatus(This,AudioStatus) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrSettings_INTERFACE_DEFINED__ */


#ifndef __IDaSvrCommandsWindow2_INTERFACE_DEFINED__
#define __IDaSvrCommandsWindow2_INTERFACE_DEFINED__

/* interface IDaSvrCommandsWindow2 */
/* [object][hidden][nonextensible][oleautomation][dual][unique][uuid] */ 


EXTERN_C const IID IID_IDaSvrCommandsWindow2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E529-A208-11DE-ABF2-002421116FB2")
    IDaSvrCommandsWindow2 : public IDaSvrCommandsWindow
    {
    public:
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Visible( 
            /* [retval][out] */ VARIANT_BOOL *Visible) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Visible( 
            /* [in] */ VARIANT_BOOL Visible) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Left( 
            /* [retval][out] */ short *Left) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Top( 
            /* [retval][out] */ short *Top) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Height( 
            /* [retval][out] */ short *Height) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Width( 
            /* [retval][out] */ short *Width) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrCommandsWindow2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrCommandsWindow2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrCommandsWindow2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrCommandsWindow2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrCommandsWindow2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrCommandsWindow2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrCommandsWindow2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrCommandsWindow2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetVisible )( 
            IDaSvrCommandsWindow2 * This,
            /* [in] */ long Visible);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVisible )( 
            IDaSvrCommandsWindow2 * This,
            /* [out] */ long *Visible);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetPosition )( 
            IDaSvrCommandsWindow2 * This,
            /* [out] */ long *Left,
            /* [out] */ long *Top);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetSize )( 
            IDaSvrCommandsWindow2 * This,
            /* [out] */ long *Width,
            /* [out] */ long *Height);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IDaSvrCommandsWindow2 * This,
            /* [retval][out] */ VARIANT_BOOL *Visible);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IDaSvrCommandsWindow2 * This,
            /* [in] */ VARIANT_BOOL Visible);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Left )( 
            IDaSvrCommandsWindow2 * This,
            /* [retval][out] */ short *Left);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Top )( 
            IDaSvrCommandsWindow2 * This,
            /* [retval][out] */ short *Top);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IDaSvrCommandsWindow2 * This,
            /* [retval][out] */ short *Height);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IDaSvrCommandsWindow2 * This,
            /* [retval][out] */ short *Width);
        
        END_INTERFACE
    } IDaSvrCommandsWindow2Vtbl;

    interface IDaSvrCommandsWindow2
    {
        CONST_VTBL struct IDaSvrCommandsWindow2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrCommandsWindow2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrCommandsWindow2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrCommandsWindow2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrCommandsWindow2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrCommandsWindow2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrCommandsWindow2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrCommandsWindow2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrCommandsWindow2_SetVisible(This,Visible)	\
    ( (This)->lpVtbl -> SetVisible(This,Visible) ) 

#define IDaSvrCommandsWindow2_GetVisible(This,Visible)	\
    ( (This)->lpVtbl -> GetVisible(This,Visible) ) 

#define IDaSvrCommandsWindow2_GetPosition(This,Left,Top)	\
    ( (This)->lpVtbl -> GetPosition(This,Left,Top) ) 

#define IDaSvrCommandsWindow2_GetSize(This,Width,Height)	\
    ( (This)->lpVtbl -> GetSize(This,Width,Height) ) 


#define IDaSvrCommandsWindow2_get_Visible(This,Visible)	\
    ( (This)->lpVtbl -> get_Visible(This,Visible) ) 

#define IDaSvrCommandsWindow2_put_Visible(This,Visible)	\
    ( (This)->lpVtbl -> put_Visible(This,Visible) ) 

#define IDaSvrCommandsWindow2_get_Left(This,Left)	\
    ( (This)->lpVtbl -> get_Left(This,Left) ) 

#define IDaSvrCommandsWindow2_get_Top(This,Top)	\
    ( (This)->lpVtbl -> get_Top(This,Top) ) 

#define IDaSvrCommandsWindow2_get_Height(This,Height)	\
    ( (This)->lpVtbl -> get_Height(This,Height) ) 

#define IDaSvrCommandsWindow2_get_Width(This,Width)	\
    ( (This)->lpVtbl -> get_Width(This,Width) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrCommandsWindow2_INTERFACE_DEFINED__ */


#ifndef __IDaSvrUserInput2_INTERFACE_DEFINED__
#define __IDaSvrUserInput2_INTERFACE_DEFINED__

/* interface IDaSvrUserInput2 */
/* [object][hidden][nonextensible][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IDaSvrUserInput2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E52A-A208-11DE-ABF2-002421116FB2")
    IDaSvrUserInput2 : public IDaSvrUserInput
    {
    public:
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pCount) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_ItemCommandID( 
            /* [in] */ long ItemIndex,
            /* [retval][out] */ long *ItemCommandID) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_ItemConfidence( 
            /* [in] */ long ItemIndex,
            /* [retval][out] */ long *ItemConfidence) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_ItemText( 
            /* [in] */ long ItemIndex,
            /* [retval][out] */ BSTR *ItemText) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrUserInput2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrUserInput2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrUserInput2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrUserInput2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrUserInput2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrUserInput2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrUserInput2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrUserInput2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IDaSvrUserInput2 * This,
            /* [out] */ long *Count);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetItemID )( 
            IDaSvrUserInput2 * This,
            /* [in] */ long ItemIndex,
            /* [out] */ long *CommandID);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetItemConfidence )( 
            IDaSvrUserInput2 * This,
            /* [in] */ long ItemIndex,
            /* [out] */ long *Confidence);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetItemText )( 
            IDaSvrUserInput2 * This,
            /* [in] */ long ItemIndex,
            /* [out] */ BSTR *Text);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetAllItemData )( 
            IDaSvrUserInput2 * This,
            /* [out] */ VARIANT *ItemIndices,
            /* [out] */ VARIANT *ItemConfidences,
            /* [out] */ VARIANT *ItemText);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IDaSvrUserInput2 * This,
            /* [retval][out] */ long *pCount);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_ItemCommandID )( 
            IDaSvrUserInput2 * This,
            /* [in] */ long ItemIndex,
            /* [retval][out] */ long *ItemCommandID);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_ItemConfidence )( 
            IDaSvrUserInput2 * This,
            /* [in] */ long ItemIndex,
            /* [retval][out] */ long *ItemConfidence);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_ItemText )( 
            IDaSvrUserInput2 * This,
            /* [in] */ long ItemIndex,
            /* [retval][out] */ BSTR *ItemText);
        
        END_INTERFACE
    } IDaSvrUserInput2Vtbl;

    interface IDaSvrUserInput2
    {
        CONST_VTBL struct IDaSvrUserInput2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrUserInput2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrUserInput2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrUserInput2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrUserInput2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrUserInput2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrUserInput2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrUserInput2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrUserInput2_GetCount(This,Count)	\
    ( (This)->lpVtbl -> GetCount(This,Count) ) 

#define IDaSvrUserInput2_GetItemID(This,ItemIndex,CommandID)	\
    ( (This)->lpVtbl -> GetItemID(This,ItemIndex,CommandID) ) 

#define IDaSvrUserInput2_GetItemConfidence(This,ItemIndex,Confidence)	\
    ( (This)->lpVtbl -> GetItemConfidence(This,ItemIndex,Confidence) ) 

#define IDaSvrUserInput2_GetItemText(This,ItemIndex,Text)	\
    ( (This)->lpVtbl -> GetItemText(This,ItemIndex,Text) ) 

#define IDaSvrUserInput2_GetAllItemData(This,ItemIndices,ItemConfidences,ItemText)	\
    ( (This)->lpVtbl -> GetAllItemData(This,ItemIndices,ItemConfidences,ItemText) ) 


#define IDaSvrUserInput2_get_Count(This,pCount)	\
    ( (This)->lpVtbl -> get_Count(This,pCount) ) 

#define IDaSvrUserInput2_get_ItemCommandID(This,ItemIndex,ItemCommandID)	\
    ( (This)->lpVtbl -> get_ItemCommandID(This,ItemIndex,ItemCommandID) ) 

#define IDaSvrUserInput2_get_ItemConfidence(This,ItemIndex,ItemConfidence)	\
    ( (This)->lpVtbl -> get_ItemConfidence(This,ItemIndex,ItemConfidence) ) 

#define IDaSvrUserInput2_get_ItemText(This,ItemIndex,ItemText)	\
    ( (This)->lpVtbl -> get_ItemText(This,ItemIndex,ItemText) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrUserInput2_INTERFACE_DEFINED__ */


#ifndef __IDaSvrCommand2_INTERFACE_DEFINED__
#define __IDaSvrCommand2_INTERFACE_DEFINED__

/* interface IDaSvrCommand2 */
/* [object][hidden][nonextensible][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IDaSvrCommand2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E52C-A208-11DE-ABF2-002421116FB2")
    IDaSvrCommand2 : public IDaSvrCommand
    {
    public:
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Caption( 
            /* [retval][out] */ BSTR *Caption) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Caption( 
            /* [in] */ BSTR Caption) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Enabled( 
            /* [retval][out] */ VARIANT_BOOL *Enabled) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Enabled( 
            /* [in] */ VARIANT_BOOL Enabled) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Visible( 
            /* [retval][out] */ VARIANT_BOOL *Visible) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Visible( 
            /* [in] */ VARIANT_BOOL Visible) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_CommandID( 
            /* [retval][out] */ long *CommandID) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_VoiceCaption( 
            /* [retval][out] */ BSTR *VoiceCaption) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_VoiceCaption( 
            /* [in] */ BSTR VoiceCaption) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_VoiceGrammar( 
            /* [retval][out] */ BSTR *VoiceGrammar) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_VoiceGrammar( 
            /* [in] */ BSTR VoiceGrammar) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_ConfidenceThreshold( 
            /* [retval][out] */ long *ConfidenceThreshold) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_ConfidenceThreshold( 
            /* [in] */ long ConfidenceThreshold) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_ConfidenceText( 
            /* [retval][out] */ BSTR *ConfidenceText) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_ConfidenceText( 
            /* [in] */ BSTR ConfidenceText) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrCommand2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrCommand2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrCommand2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrCommand2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrCommand2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrCommand2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrCommand2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrCommand2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetCaption )( 
            IDaSvrCommand2 * This,
            /* [in] */ BSTR Caption);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetCaption )( 
            IDaSvrCommand2 * This,
            /* [out] */ BSTR *Caption);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetVoice )( 
            IDaSvrCommand2 * This,
            /* [in] */ BSTR Voice);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVoice )( 
            IDaSvrCommand2 * This,
            /* [out] */ BSTR *Voice);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetEnabled )( 
            IDaSvrCommand2 * This,
            /* [in] */ long Enabled);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetEnabled )( 
            IDaSvrCommand2 * This,
            /* [out] */ long *Enabled);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetVisible )( 
            IDaSvrCommand2 * This,
            /* [in] */ long Visible);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVisible )( 
            IDaSvrCommand2 * This,
            /* [out] */ long *Visible);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetConfidenceThreshold )( 
            IDaSvrCommand2 * This,
            /* [in] */ long Threshold);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetConfidenceThreshold )( 
            IDaSvrCommand2 * This,
            /* [out] */ long *Threshold);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetConfidenceText )( 
            IDaSvrCommand2 * This,
            /* [in] */ BSTR TipText);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetConfidenceText )( 
            IDaSvrCommand2 * This,
            /* [out] */ BSTR *TipText);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetID )( 
            IDaSvrCommand2 * This,
            /* [out] */ long *CommandID);
        
        /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetHelpContextID )( 
            IDaSvrCommand2 * This,
            /* [in] */ long ContextID);
        
        /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetHelpContextID )( 
            IDaSvrCommand2 * This,
            /* [out] */ long *ContextID);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetVoiceCaption )( 
            IDaSvrCommand2 * This,
            /* [in] */ BSTR VoiceCaption);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVoiceCaption )( 
            IDaSvrCommand2 * This,
            /* [out] */ BSTR *pbszVoiceCaption);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Caption )( 
            IDaSvrCommand2 * This,
            /* [retval][out] */ BSTR *Caption);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Caption )( 
            IDaSvrCommand2 * This,
            /* [in] */ BSTR Caption);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IDaSvrCommand2 * This,
            /* [retval][out] */ VARIANT_BOOL *Enabled);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            IDaSvrCommand2 * This,
            /* [in] */ VARIANT_BOOL Enabled);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IDaSvrCommand2 * This,
            /* [retval][out] */ VARIANT_BOOL *Visible);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IDaSvrCommand2 * This,
            /* [in] */ VARIANT_BOOL Visible);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_CommandID )( 
            IDaSvrCommand2 * This,
            /* [retval][out] */ long *CommandID);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_VoiceCaption )( 
            IDaSvrCommand2 * This,
            /* [retval][out] */ BSTR *VoiceCaption);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_VoiceCaption )( 
            IDaSvrCommand2 * This,
            /* [in] */ BSTR VoiceCaption);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_VoiceGrammar )( 
            IDaSvrCommand2 * This,
            /* [retval][out] */ BSTR *VoiceGrammar);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_VoiceGrammar )( 
            IDaSvrCommand2 * This,
            /* [in] */ BSTR VoiceGrammar);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_ConfidenceThreshold )( 
            IDaSvrCommand2 * This,
            /* [retval][out] */ long *ConfidenceThreshold);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ConfidenceThreshold )( 
            IDaSvrCommand2 * This,
            /* [in] */ long ConfidenceThreshold);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_ConfidenceText )( 
            IDaSvrCommand2 * This,
            /* [retval][out] */ BSTR *ConfidenceText);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ConfidenceText )( 
            IDaSvrCommand2 * This,
            /* [in] */ BSTR ConfidenceText);
        
        END_INTERFACE
    } IDaSvrCommand2Vtbl;

    interface IDaSvrCommand2
    {
        CONST_VTBL struct IDaSvrCommand2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrCommand2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrCommand2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrCommand2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrCommand2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrCommand2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrCommand2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrCommand2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrCommand2_SetCaption(This,Caption)	\
    ( (This)->lpVtbl -> SetCaption(This,Caption) ) 

#define IDaSvrCommand2_GetCaption(This,Caption)	\
    ( (This)->lpVtbl -> GetCaption(This,Caption) ) 

#define IDaSvrCommand2_SetVoice(This,Voice)	\
    ( (This)->lpVtbl -> SetVoice(This,Voice) ) 

#define IDaSvrCommand2_GetVoice(This,Voice)	\
    ( (This)->lpVtbl -> GetVoice(This,Voice) ) 

#define IDaSvrCommand2_SetEnabled(This,Enabled)	\
    ( (This)->lpVtbl -> SetEnabled(This,Enabled) ) 

#define IDaSvrCommand2_GetEnabled(This,Enabled)	\
    ( (This)->lpVtbl -> GetEnabled(This,Enabled) ) 

#define IDaSvrCommand2_SetVisible(This,Visible)	\
    ( (This)->lpVtbl -> SetVisible(This,Visible) ) 

#define IDaSvrCommand2_GetVisible(This,Visible)	\
    ( (This)->lpVtbl -> GetVisible(This,Visible) ) 

#define IDaSvrCommand2_SetConfidenceThreshold(This,Threshold)	\
    ( (This)->lpVtbl -> SetConfidenceThreshold(This,Threshold) ) 

#define IDaSvrCommand2_GetConfidenceThreshold(This,Threshold)	\
    ( (This)->lpVtbl -> GetConfidenceThreshold(This,Threshold) ) 

#define IDaSvrCommand2_SetConfidenceText(This,TipText)	\
    ( (This)->lpVtbl -> SetConfidenceText(This,TipText) ) 

#define IDaSvrCommand2_GetConfidenceText(This,TipText)	\
    ( (This)->lpVtbl -> GetConfidenceText(This,TipText) ) 

#define IDaSvrCommand2_GetID(This,CommandID)	\
    ( (This)->lpVtbl -> GetID(This,CommandID) ) 

#define IDaSvrCommand2_SetHelpContextID(This,ContextID)	\
    ( (This)->lpVtbl -> SetHelpContextID(This,ContextID) ) 

#define IDaSvrCommand2_GetHelpContextID(This,ContextID)	\
    ( (This)->lpVtbl -> GetHelpContextID(This,ContextID) ) 

#define IDaSvrCommand2_SetVoiceCaption(This,VoiceCaption)	\
    ( (This)->lpVtbl -> SetVoiceCaption(This,VoiceCaption) ) 

#define IDaSvrCommand2_GetVoiceCaption(This,pbszVoiceCaption)	\
    ( (This)->lpVtbl -> GetVoiceCaption(This,pbszVoiceCaption) ) 


#define IDaSvrCommand2_get_Caption(This,Caption)	\
    ( (This)->lpVtbl -> get_Caption(This,Caption) ) 

#define IDaSvrCommand2_put_Caption(This,Caption)	\
    ( (This)->lpVtbl -> put_Caption(This,Caption) ) 

#define IDaSvrCommand2_get_Enabled(This,Enabled)	\
    ( (This)->lpVtbl -> get_Enabled(This,Enabled) ) 

#define IDaSvrCommand2_put_Enabled(This,Enabled)	\
    ( (This)->lpVtbl -> put_Enabled(This,Enabled) ) 

#define IDaSvrCommand2_get_Visible(This,Visible)	\
    ( (This)->lpVtbl -> get_Visible(This,Visible) ) 

#define IDaSvrCommand2_put_Visible(This,Visible)	\
    ( (This)->lpVtbl -> put_Visible(This,Visible) ) 

#define IDaSvrCommand2_get_CommandID(This,CommandID)	\
    ( (This)->lpVtbl -> get_CommandID(This,CommandID) ) 

#define IDaSvrCommand2_get_VoiceCaption(This,VoiceCaption)	\
    ( (This)->lpVtbl -> get_VoiceCaption(This,VoiceCaption) ) 

#define IDaSvrCommand2_put_VoiceCaption(This,VoiceCaption)	\
    ( (This)->lpVtbl -> put_VoiceCaption(This,VoiceCaption) ) 

#define IDaSvrCommand2_get_VoiceGrammar(This,VoiceGrammar)	\
    ( (This)->lpVtbl -> get_VoiceGrammar(This,VoiceGrammar) ) 

#define IDaSvrCommand2_put_VoiceGrammar(This,VoiceGrammar)	\
    ( (This)->lpVtbl -> put_VoiceGrammar(This,VoiceGrammar) ) 

#define IDaSvrCommand2_get_ConfidenceThreshold(This,ConfidenceThreshold)	\
    ( (This)->lpVtbl -> get_ConfidenceThreshold(This,ConfidenceThreshold) ) 

#define IDaSvrCommand2_put_ConfidenceThreshold(This,ConfidenceThreshold)	\
    ( (This)->lpVtbl -> put_ConfidenceThreshold(This,ConfidenceThreshold) ) 

#define IDaSvrCommand2_get_ConfidenceText(This,ConfidenceText)	\
    ( (This)->lpVtbl -> get_ConfidenceText(This,ConfidenceText) ) 

#define IDaSvrCommand2_put_ConfidenceText(This,ConfidenceText)	\
    ( (This)->lpVtbl -> put_ConfidenceText(This,ConfidenceText) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrCommand2_INTERFACE_DEFINED__ */


#ifndef __IDaSvrCommands2_INTERFACE_DEFINED__
#define __IDaSvrCommands2_INTERFACE_DEFINED__

/* interface IDaSvrCommands2 */
/* [object][custom][hidden][nonextensible][oleautomation][dual][uuid] */ 


EXTERN_C const IID IID_IDaSvrCommands2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1147E52B-A208-11DE-ABF2-002421116FB2")
    IDaSvrCommands2 : public IDaSvrCommands
    {
    public:
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ long Index,
            /* [retval][out] */ IDaSvrCommand2 **Command) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *Count) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Command( 
            /* [in] */ long CommandID,
            /* [retval][out] */ IDaSvrCommand2 **Command) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Caption( 
            /* [retval][out] */ BSTR *Caption) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Caption( 
            /* [in] */ BSTR Caption) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_VoiceGrammar( 
            /* [retval][out] */ BSTR *VoiceGrammar) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_VoiceGrammar( 
            /* [in] */ BSTR VoiceGrammar) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_Visible( 
            /* [retval][out] */ VARIANT_BOOL *Visible) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_Visible( 
            /* [in] */ VARIANT_BOOL Visible) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_DefaultCommand( 
            /* [retval][out] */ long *CommandID) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_DefaultCommand( 
            /* [in] */ long CommandID) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_FontName( 
            /* [retval][out] */ BSTR *FontName) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_FontName( 
            /* [in] */ BSTR FontName) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_FontSize( 
            /* [retval][out] */ long *FontSize) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_FontSize( 
            /* [in] */ long FontSize) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_VoiceCaption( 
            /* [retval][out] */ BSTR *VoiceCaption) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_VoiceCaption( 
            /* [in] */ BSTR VoiceCaption) = 0;
        
        virtual /* [propget][id][helpstring] */ HRESULT STDMETHODCALLTYPE get_GlobalVoiceCommandsEnabled( 
            /* [retval][out] */ VARIANT_BOOL *Enabled) = 0;
        
        virtual /* [propput][id] */ HRESULT STDMETHODCALLTYPE put_GlobalVoiceCommandsEnabled( 
            /* [in] */ VARIANT_BOOL Enabled) = 0;
        
        virtual /* [restricted][propget][id] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **ppunkEnum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDaSvrCommands2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDaSvrCommands2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDaSvrCommands2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDaSvrCommands2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDaSvrCommands2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDaSvrCommands2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDaSvrCommands2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDaSvrCommands2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetCommand )( 
            IDaSvrCommands2 * This,
            /* [in] */ long CommandID,
            /* [out] */ IUnknown **Command);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IDaSvrCommands2 * This,
            /* [out] */ long *Count);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetCaption )( 
            IDaSvrCommands2 * This,
            /* [in] */ BSTR Caption);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetCaption )( 
            IDaSvrCommands2 * This,
            /* [out] */ BSTR *Caption);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetVoice )( 
            IDaSvrCommands2 * This,
            /* [in] */ BSTR Voice);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVoice )( 
            IDaSvrCommands2 * This,
            /* [out] */ BSTR *Voice);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetVisible )( 
            IDaSvrCommands2 * This,
            /* [in] */ long Visible);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVisible )( 
            IDaSvrCommands2 * This,
            /* [out] */ long *Visible);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            IDaSvrCommands2 * This,
            /* [in] */ BSTR Caption,
            /* [in] */ BSTR VoiceGrammar,
            /* [in] */ long Enabled,
            /* [in] */ long Visible,
            /* [out] */ long *CommandID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Insert )( 
            IDaSvrCommands2 * This,
            /* [in] */ BSTR Caption,
            /* [in] */ BSTR VoiceGrammar,
            /* [in] */ long Enabled,
            /* [in] */ long Visible,
            /* [in] */ long RefCommandID,
            /* [in] */ long Before,
            /* [out] */ long *CommandID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *Remove )( 
            IDaSvrCommands2 * This,
            /* [in] */ long CommandID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *RemoveAll )( 
            IDaSvrCommands2 * This);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetCommandEx )( 
            IDaSvrCommands2 * This,
            /* [in] */ long CommandID,
            /* [out] */ IDaSvrCommand **Command);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetDefaultID )( 
            IDaSvrCommands2 * This,
            /* [in] */ long CommandID);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetDefaultID )( 
            IDaSvrCommands2 * This,
            /* [out] */ long *CommandID);
        
        /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetHelpContextID )( 
            IDaSvrCommands2 * This,
            /* [in] */ long HelpContextID);
        
        /* [nonbrowsable][restricted][hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetHelpContextID )( 
            IDaSvrCommands2 * This,
            /* [out] */ long *pulHelpID);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetFontName )( 
            IDaSvrCommands2 * This,
            /* [in] */ BSTR FontName);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetFontName )( 
            IDaSvrCommands2 * This,
            /* [out] */ BSTR *FontName);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetFontSize )( 
            IDaSvrCommands2 * This,
            /* [in] */ long FontSize);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetFontSize )( 
            IDaSvrCommands2 * This,
            /* [out] */ long *FontSize);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetVoiceCaption )( 
            IDaSvrCommands2 * This,
            /* [in] */ BSTR VoiceCaption);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetVoiceCaption )( 
            IDaSvrCommands2 * This,
            /* [out] */ BSTR *VoiceCaption);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *AddEx )( 
            IDaSvrCommands2 * This,
            /* [in] */ BSTR Caption,
            /* [in] */ BSTR VoiceGrammar,
            /* [in] */ BSTR VoiceCaption,
            /* [in] */ long Enabled,
            /* [in] */ long Visible,
            /* [in] */ long HelpContextID,
            /* [out] */ long *CommandID);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *InsertEx )( 
            IDaSvrCommands2 * This,
            /* [in] */ BSTR Caption,
            /* [in] */ BSTR VoiceGrammar,
            /* [in] */ BSTR VoiceCaption,
            /* [in] */ long Enabled,
            /* [in] */ long Visible,
            /* [in] */ long HelpContextID,
            /* [in] */ long RefCommandID,
            /* [in] */ long Before,
            /* [out] */ long *CommandID);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *SetGlobalVoiceCommandsEnabled )( 
            IDaSvrCommands2 * This,
            /* [in] */ long Enabled);
        
        /* [hidden][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetGlobalVoiceCommandsEnabled )( 
            IDaSvrCommands2 * This,
            /* [out] */ long *Enabled);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IDaSvrCommands2 * This,
            /* [in] */ long Index,
            /* [retval][out] */ IDaSvrCommand2 **Command);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IDaSvrCommands2 * This,
            /* [retval][out] */ long *Count);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Command )( 
            IDaSvrCommands2 * This,
            /* [in] */ long CommandID,
            /* [retval][out] */ IDaSvrCommand2 **Command);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Caption )( 
            IDaSvrCommands2 * This,
            /* [retval][out] */ BSTR *Caption);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Caption )( 
            IDaSvrCommands2 * This,
            /* [in] */ BSTR Caption);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_VoiceGrammar )( 
            IDaSvrCommands2 * This,
            /* [retval][out] */ BSTR *VoiceGrammar);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_VoiceGrammar )( 
            IDaSvrCommands2 * This,
            /* [in] */ BSTR VoiceGrammar);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IDaSvrCommands2 * This,
            /* [retval][out] */ VARIANT_BOOL *Visible);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IDaSvrCommands2 * This,
            /* [in] */ VARIANT_BOOL Visible);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_DefaultCommand )( 
            IDaSvrCommands2 * This,
            /* [retval][out] */ long *CommandID);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_DefaultCommand )( 
            IDaSvrCommands2 * This,
            /* [in] */ long CommandID);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_FontName )( 
            IDaSvrCommands2 * This,
            /* [retval][out] */ BSTR *FontName);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FontName )( 
            IDaSvrCommands2 * This,
            /* [in] */ BSTR FontName);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_FontSize )( 
            IDaSvrCommands2 * This,
            /* [retval][out] */ long *FontSize);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FontSize )( 
            IDaSvrCommands2 * This,
            /* [in] */ long FontSize);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_VoiceCaption )( 
            IDaSvrCommands2 * This,
            /* [retval][out] */ BSTR *VoiceCaption);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_VoiceCaption )( 
            IDaSvrCommands2 * This,
            /* [in] */ BSTR VoiceCaption);
        
        /* [propget][id][helpstring] */ HRESULT ( STDMETHODCALLTYPE *get_GlobalVoiceCommandsEnabled )( 
            IDaSvrCommands2 * This,
            /* [retval][out] */ VARIANT_BOOL *Enabled);
        
        /* [propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_GlobalVoiceCommandsEnabled )( 
            IDaSvrCommands2 * This,
            /* [in] */ VARIANT_BOOL Enabled);
        
        /* [restricted][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IDaSvrCommands2 * This,
            /* [retval][out] */ IUnknown **ppunkEnum);
        
        END_INTERFACE
    } IDaSvrCommands2Vtbl;

    interface IDaSvrCommands2
    {
        CONST_VTBL struct IDaSvrCommands2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDaSvrCommands2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDaSvrCommands2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDaSvrCommands2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDaSvrCommands2_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDaSvrCommands2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDaSvrCommands2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDaSvrCommands2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDaSvrCommands2_GetCommand(This,CommandID,Command)	\
    ( (This)->lpVtbl -> GetCommand(This,CommandID,Command) ) 

#define IDaSvrCommands2_GetCount(This,Count)	\
    ( (This)->lpVtbl -> GetCount(This,Count) ) 

#define IDaSvrCommands2_SetCaption(This,Caption)	\
    ( (This)->lpVtbl -> SetCaption(This,Caption) ) 

#define IDaSvrCommands2_GetCaption(This,Caption)	\
    ( (This)->lpVtbl -> GetCaption(This,Caption) ) 

#define IDaSvrCommands2_SetVoice(This,Voice)	\
    ( (This)->lpVtbl -> SetVoice(This,Voice) ) 

#define IDaSvrCommands2_GetVoice(This,Voice)	\
    ( (This)->lpVtbl -> GetVoice(This,Voice) ) 

#define IDaSvrCommands2_SetVisible(This,Visible)	\
    ( (This)->lpVtbl -> SetVisible(This,Visible) ) 

#define IDaSvrCommands2_GetVisible(This,Visible)	\
    ( (This)->lpVtbl -> GetVisible(This,Visible) ) 

#define IDaSvrCommands2_Add(This,Caption,VoiceGrammar,Enabled,Visible,CommandID)	\
    ( (This)->lpVtbl -> Add(This,Caption,VoiceGrammar,Enabled,Visible,CommandID) ) 

#define IDaSvrCommands2_Insert(This,Caption,VoiceGrammar,Enabled,Visible,RefCommandID,Before,CommandID)	\
    ( (This)->lpVtbl -> Insert(This,Caption,VoiceGrammar,Enabled,Visible,RefCommandID,Before,CommandID) ) 

#define IDaSvrCommands2_Remove(This,CommandID)	\
    ( (This)->lpVtbl -> Remove(This,CommandID) ) 

#define IDaSvrCommands2_RemoveAll(This)	\
    ( (This)->lpVtbl -> RemoveAll(This) ) 

#define IDaSvrCommands2_GetCommandEx(This,CommandID,Command)	\
    ( (This)->lpVtbl -> GetCommandEx(This,CommandID,Command) ) 

#define IDaSvrCommands2_SetDefaultID(This,CommandID)	\
    ( (This)->lpVtbl -> SetDefaultID(This,CommandID) ) 

#define IDaSvrCommands2_GetDefaultID(This,CommandID)	\
    ( (This)->lpVtbl -> GetDefaultID(This,CommandID) ) 

#define IDaSvrCommands2_SetHelpContextID(This,HelpContextID)	\
    ( (This)->lpVtbl -> SetHelpContextID(This,HelpContextID) ) 

#define IDaSvrCommands2_GetHelpContextID(This,pulHelpID)	\
    ( (This)->lpVtbl -> GetHelpContextID(This,pulHelpID) ) 

#define IDaSvrCommands2_SetFontName(This,FontName)	\
    ( (This)->lpVtbl -> SetFontName(This,FontName) ) 

#define IDaSvrCommands2_GetFontName(This,FontName)	\
    ( (This)->lpVtbl -> GetFontName(This,FontName) ) 

#define IDaSvrCommands2_SetFontSize(This,FontSize)	\
    ( (This)->lpVtbl -> SetFontSize(This,FontSize) ) 

#define IDaSvrCommands2_GetFontSize(This,FontSize)	\
    ( (This)->lpVtbl -> GetFontSize(This,FontSize) ) 

#define IDaSvrCommands2_SetVoiceCaption(This,VoiceCaption)	\
    ( (This)->lpVtbl -> SetVoiceCaption(This,VoiceCaption) ) 

#define IDaSvrCommands2_GetVoiceCaption(This,VoiceCaption)	\
    ( (This)->lpVtbl -> GetVoiceCaption(This,VoiceCaption) ) 

#define IDaSvrCommands2_AddEx(This,Caption,VoiceGrammar,VoiceCaption,Enabled,Visible,HelpContextID,CommandID)	\
    ( (This)->lpVtbl -> AddEx(This,Caption,VoiceGrammar,VoiceCaption,Enabled,Visible,HelpContextID,CommandID) ) 

#define IDaSvrCommands2_InsertEx(This,Caption,VoiceGrammar,VoiceCaption,Enabled,Visible,HelpContextID,RefCommandID,Before,CommandID)	\
    ( (This)->lpVtbl -> InsertEx(This,Caption,VoiceGrammar,VoiceCaption,Enabled,Visible,HelpContextID,RefCommandID,Before,CommandID) ) 

#define IDaSvrCommands2_SetGlobalVoiceCommandsEnabled(This,Enabled)	\
    ( (This)->lpVtbl -> SetGlobalVoiceCommandsEnabled(This,Enabled) ) 

#define IDaSvrCommands2_GetGlobalVoiceCommandsEnabled(This,Enabled)	\
    ( (This)->lpVtbl -> GetGlobalVoiceCommandsEnabled(This,Enabled) ) 


#define IDaSvrCommands2_get_Item(This,Index,Command)	\
    ( (This)->lpVtbl -> get_Item(This,Index,Command) ) 

#define IDaSvrCommands2_get_Count(This,Count)	\
    ( (This)->lpVtbl -> get_Count(This,Count) ) 

#define IDaSvrCommands2_get_Command(This,CommandID,Command)	\
    ( (This)->lpVtbl -> get_Command(This,CommandID,Command) ) 

#define IDaSvrCommands2_get_Caption(This,Caption)	\
    ( (This)->lpVtbl -> get_Caption(This,Caption) ) 

#define IDaSvrCommands2_put_Caption(This,Caption)	\
    ( (This)->lpVtbl -> put_Caption(This,Caption) ) 

#define IDaSvrCommands2_get_VoiceGrammar(This,VoiceGrammar)	\
    ( (This)->lpVtbl -> get_VoiceGrammar(This,VoiceGrammar) ) 

#define IDaSvrCommands2_put_VoiceGrammar(This,VoiceGrammar)	\
    ( (This)->lpVtbl -> put_VoiceGrammar(This,VoiceGrammar) ) 

#define IDaSvrCommands2_get_Visible(This,Visible)	\
    ( (This)->lpVtbl -> get_Visible(This,Visible) ) 

#define IDaSvrCommands2_put_Visible(This,Visible)	\
    ( (This)->lpVtbl -> put_Visible(This,Visible) ) 

#define IDaSvrCommands2_get_DefaultCommand(This,CommandID)	\
    ( (This)->lpVtbl -> get_DefaultCommand(This,CommandID) ) 

#define IDaSvrCommands2_put_DefaultCommand(This,CommandID)	\
    ( (This)->lpVtbl -> put_DefaultCommand(This,CommandID) ) 

#define IDaSvrCommands2_get_FontName(This,FontName)	\
    ( (This)->lpVtbl -> get_FontName(This,FontName) ) 

#define IDaSvrCommands2_put_FontName(This,FontName)	\
    ( (This)->lpVtbl -> put_FontName(This,FontName) ) 

#define IDaSvrCommands2_get_FontSize(This,FontSize)	\
    ( (This)->lpVtbl -> get_FontSize(This,FontSize) ) 

#define IDaSvrCommands2_put_FontSize(This,FontSize)	\
    ( (This)->lpVtbl -> put_FontSize(This,FontSize) ) 

#define IDaSvrCommands2_get_VoiceCaption(This,VoiceCaption)	\
    ( (This)->lpVtbl -> get_VoiceCaption(This,VoiceCaption) ) 

#define IDaSvrCommands2_put_VoiceCaption(This,VoiceCaption)	\
    ( (This)->lpVtbl -> put_VoiceCaption(This,VoiceCaption) ) 

#define IDaSvrCommands2_get_GlobalVoiceCommandsEnabled(This,Enabled)	\
    ( (This)->lpVtbl -> get_GlobalVoiceCommandsEnabled(This,Enabled) ) 

#define IDaSvrCommands2_put_GlobalVoiceCommandsEnabled(This,Enabled)	\
    ( (This)->lpVtbl -> put_GlobalVoiceCommandsEnabled(This,Enabled) ) 

#define IDaSvrCommands2_get__NewEnum(This,ppunkEnum)	\
    ( (This)->lpVtbl -> get__NewEnum(This,ppunkEnum) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDaSvrCommands2_INTERFACE_DEFINED__ */


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

EXTERN_C const CLSID CLSID_DaSvrSettings;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E528-A208-11DE-ABF2-002421116FB2")
DaSvrSettings;
#endif

EXTERN_C const CLSID CLSID_DaSvrCharacterFiles;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E524-A208-11DE-ABF2-002421116FB2")
DaSvrCharacterFiles;
#endif

EXTERN_C const CLSID CLSID_DaSvrTTSEngine;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E51F-A208-11DE-ABF2-002421116FB2")
DaSvrTTSEngine;
#endif

EXTERN_C const CLSID CLSID_DaSvrTTSEngines;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E520-A208-11DE-ABF2-002421116FB2")
DaSvrTTSEngines;
#endif

EXTERN_C const CLSID CLSID_DaSvrSREngine;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E521-A208-11DE-ABF2-002421116FB2")
DaSvrSREngine;
#endif

EXTERN_C const CLSID CLSID_DaSvrSREngines;

#ifdef __cplusplus

class DECLSPEC_UUID("1147E522-A208-11DE-ABF2-002421116FB2")
DaSvrSREngines;
#endif
#endif /* __DoubleAgentSvr_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


