

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Mon May 31 01:31:13 2010
 */
/* Compiler settings for .\Core\DaCore.odl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 7.00.0555 
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

MIDL_DEFINE_GUID(IID, IID_IDaPreview,0x1147E560,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(CLSID, CLSID_DaPreview,0x1147E561,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID__IAgentStreamInfo,0x1147E562,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID__IAgentStreamSource,0x1147E567,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);


MIDL_DEFINE_GUID(IID, IID__IAgentStreamRender,0x1147E568,0xA208,0x11DE,0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



