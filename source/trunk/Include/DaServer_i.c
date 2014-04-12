

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Sat Apr 12 03:39:10 2014
 */
/* Compiler settings for Server\DaServer.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_DoubleAgentSvr,0x1147E501,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaServer,0x1147E502,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaSvrCommand,0x1147E506,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaSvrCommands,0x1147E505,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaSvrUserInput,0x1147E508,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaSvrSpeechInput,0x1147E50C,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaSvrAudioOutput,0x1147E50B,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaSvrPropertySheet,0x1147E50A,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaSvrBalloon,0x1147E509,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaSvrCharacter,0x1147E504,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaSvrCommandsWindow,0x1147E507,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaSvrNotifySink15,0x1147E503,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaSvrNotifySink,0x1147E516,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaSvrCharacterFiles,0x1147E523,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaSvrFormattedText,0x1147E52F,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaSvrTTSEngine,0x1147E519,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaSvrTTSEngines,0x1147E51A,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaSvrSREngine,0x1147E51B,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaSvrSREngines,0x1147E51C,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaServer2,0x1147E51D,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaSvrCharacter2,0x1147E51E,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaSvrBalloon2,0x1147E526,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaSvrPropertySheet2,0x1147E525,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaSvrSettings,0x1147E527,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaSvrCommandsWindow2,0x1147E529,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaSvrUserInput2,0x1147E52A,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaSvrCommand2,0x1147E52C,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaSvrCommands2,0x1147E52B,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaSvrNotifySink2,0x1147E571,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, DIID__DaSvrEvents2,0x1147E52E,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaServer,0x1147E500,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaSvrCharacter,0x1147E50D,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaSvrCommand,0x1147E50E,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaSvrCommands,0x1147E50F,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaSvrCommandsWindow,0x1147E510,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaSvrUserInput,0x1147E511,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaSvrBalloon,0x1147E512,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaSvrPropertySheet,0x1147E513,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaSvrAudioOutput,0x1147E514,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaSvrSpeechInput,0x1147E515,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaSvrAnimationNames,0x1147E517,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaSvrSettings,0x1147E528,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaSvrCharacterFiles,0x1147E524,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaSvrFormattedText,0x1147E570,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaSvrTTSEngine,0x1147E51F,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaSvrTTSEngines,0x1147E520,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaSvrSREngine,0x1147E521,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaSvrSREngines,0x1147E522,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



