

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Tue Jan 05 02:49:38 2010
 */
/* Compiler settings for .\Control\DaControl.odl:
    Oicf, W1, Zp8, env=Win64 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

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

MIDL_DEFINE_GUID(IID, LIBID_DoubleAgentCtl,0x1147E550,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaCtlCharacters,0x1147E53C,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaCtlCharacter,0x1147E53D,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaCtlBalloon,0x1147E53F,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaCtlCommands,0x1147E540,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaCtlCommand,0x1147E541,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaCtlRequest,0x1147E542,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaCtlAudioObject,0x1147E543,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaCtlSpeechInput,0x1147E544,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaCtlPropertySheet,0x1147E545,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaCtlUserInput,0x1147E546,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaCtlCommandsWindow,0x1147E547,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaCtlAnimationNames,0x1147E548,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID_IDaControl,0x1147E549,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, DIID__DaCtlEvents,0x1147E54A,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaControl,0x1147E530,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaCtlCharacters,0x1147E531,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaCtlCharacter,0x1147E532,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaCtlBalloon,0x1147E533,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaCtlCommands,0x1147E534,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaCtlCommand,0x1147E535,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaCtlRequest,0x1147E536,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaCtlAudioOutput,0x1147E537,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaCtlSpeechInput,0x1147E538,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaCtlPropertySheet,0x1147E539,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaCtlUserInput,0x1147E53A,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaCtlCommandsWindow,0x1147E53B,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaCtlAnimationNames,0x1147E54B,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



