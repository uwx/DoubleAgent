

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Nov 04 18:32:08 2010
 */
/* Compiler settings for .\Core\DaCore.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif /* __RPCPROXY_H_VERSION__ */


#include "DaCoreOdl.h"

#define TYPE_FORMAT_STRING_SIZE   191                               
#define PROC_FORMAT_STRING_SIZE   2221                              
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   3            

typedef struct _DaCore_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } DaCore_MIDL_TYPE_FORMAT_STRING;

typedef struct _DaCore_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } DaCore_MIDL_PROC_FORMAT_STRING;

typedef struct _DaCore_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } DaCore_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const DaCore_MIDL_TYPE_FORMAT_STRING DaCore__MIDL_TypeFormatString;
extern const DaCore_MIDL_PROC_FORMAT_STRING DaCore__MIDL_ProcFormatString;
extern const DaCore_MIDL_EXPR_FORMAT_STRING DaCore__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IDaPreview_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IDaPreview_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO _IAgentStreamInfo_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO _IAgentStreamInfo_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO _IAgentStreamSource_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO _IAgentStreamSource_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO _IAgentStreamRender_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO _IAgentStreamRender_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const DaCore_MIDL_PROC_FORMAT_STRING DaCore__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure PlayAnimation */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x3 ),	/* 3 */
/*  8 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 16 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x1 ),	/* 1 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAnimationName */

/* 24 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 26 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 28 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 30 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 32 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 34 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PlayState */

/* 36 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 38 */	NdrFcLong( 0x0 ),	/* 0 */
/* 42 */	NdrFcShort( 0x4 ),	/* 4 */
/* 44 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 46 */	NdrFcShort( 0x0 ),	/* 0 */
/* 48 */	NdrFcShort( 0x8 ),	/* 8 */
/* 50 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 52 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 54 */	NdrFcShort( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0x1 ),	/* 1 */
/* 58 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pStateName */

/* 60 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 62 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 64 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 66 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 68 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 70 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetIsPlaying */

/* 72 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 74 */	NdrFcLong( 0x0 ),	/* 0 */
/* 78 */	NdrFcShort( 0x5 ),	/* 5 */
/* 80 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 82 */	NdrFcShort( 0x0 ),	/* 0 */
/* 84 */	NdrFcShort( 0x21 ),	/* 33 */
/* 86 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 88 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 90 */	NdrFcShort( 0x0 ),	/* 0 */
/* 92 */	NdrFcShort( 0x0 ),	/* 0 */
/* 94 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pIsPlaying */

/* 96 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 98 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 100 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */

/* 102 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 104 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 106 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure StopPlaying */

/* 108 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 110 */	NdrFcLong( 0x0 ),	/* 0 */
/* 114 */	NdrFcShort( 0x6 ),	/* 6 */
/* 116 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 118 */	NdrFcShort( 0x0 ),	/* 0 */
/* 120 */	NdrFcShort( 0x8 ),	/* 8 */
/* 122 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 124 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 126 */	NdrFcShort( 0x0 ),	/* 0 */
/* 128 */	NdrFcShort( 0x0 ),	/* 0 */
/* 130 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 132 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 134 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 136 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetSoundsEnabled */

/* 138 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 140 */	NdrFcLong( 0x0 ),	/* 0 */
/* 144 */	NdrFcShort( 0x7 ),	/* 7 */
/* 146 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 148 */	NdrFcShort( 0x0 ),	/* 0 */
/* 150 */	NdrFcShort( 0x21 ),	/* 33 */
/* 152 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 154 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 156 */	NdrFcShort( 0x0 ),	/* 0 */
/* 158 */	NdrFcShort( 0x0 ),	/* 0 */
/* 160 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pEnabled */

/* 162 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 164 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 166 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */

/* 168 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 170 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 172 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure EnableSounds */

/* 174 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 176 */	NdrFcLong( 0x0 ),	/* 0 */
/* 180 */	NdrFcShort( 0x8 ),	/* 8 */
/* 182 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 184 */	NdrFcShort( 0x5 ),	/* 5 */
/* 186 */	NdrFcShort( 0x8 ),	/* 8 */
/* 188 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 190 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 192 */	NdrFcShort( 0x0 ),	/* 0 */
/* 194 */	NdrFcShort( 0x0 ),	/* 0 */
/* 196 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pEnabled */

/* 198 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 200 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 202 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */

/* 204 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 206 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 208 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetIdleEnabled */

/* 210 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 212 */	NdrFcLong( 0x0 ),	/* 0 */
/* 216 */	NdrFcShort( 0x9 ),	/* 9 */
/* 218 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 220 */	NdrFcShort( 0x0 ),	/* 0 */
/* 222 */	NdrFcShort( 0x21 ),	/* 33 */
/* 224 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 226 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 228 */	NdrFcShort( 0x0 ),	/* 0 */
/* 230 */	NdrFcShort( 0x0 ),	/* 0 */
/* 232 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pEnabled */

/* 234 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 236 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 238 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */

/* 240 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 242 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 244 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure EnableIdle */

/* 246 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 248 */	NdrFcLong( 0x0 ),	/* 0 */
/* 252 */	NdrFcShort( 0xa ),	/* 10 */
/* 254 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 256 */	NdrFcShort( 0x5 ),	/* 5 */
/* 258 */	NdrFcShort( 0x8 ),	/* 8 */
/* 260 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 262 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 264 */	NdrFcShort( 0x0 ),	/* 0 */
/* 266 */	NdrFcShort( 0x0 ),	/* 0 */
/* 268 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pEnabled */

/* 270 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 272 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 274 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */

/* 276 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 278 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 280 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetBkColor */

/* 282 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 284 */	NdrFcLong( 0x0 ),	/* 0 */
/* 288 */	NdrFcShort( 0xb ),	/* 11 */
/* 290 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 292 */	NdrFcShort( 0x8 ),	/* 8 */
/* 294 */	NdrFcShort( 0x8 ),	/* 8 */
/* 296 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 298 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 300 */	NdrFcShort( 0x0 ),	/* 0 */
/* 302 */	NdrFcShort( 0x0 ),	/* 0 */
/* 304 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pBkColor */

/* 306 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 308 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 310 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 312 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 314 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 316 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetFrameSize */

/* 318 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 320 */	NdrFcLong( 0x0 ),	/* 0 */
/* 324 */	NdrFcShort( 0xc ),	/* 12 */
/* 326 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 328 */	NdrFcShort( 0x0 ),	/* 0 */
/* 330 */	NdrFcShort( 0x34 ),	/* 52 */
/* 332 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 334 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 336 */	NdrFcShort( 0x0 ),	/* 0 */
/* 338 */	NdrFcShort( 0x0 ),	/* 0 */
/* 340 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pFrameSize */

/* 342 */	NdrFcShort( 0x2112 ),	/* Flags:  must free, out, simple ref, srv alloc size=8 */
/* 344 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 346 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 348 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 350 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 352 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetCharacterFrameSize */

/* 354 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 356 */	NdrFcLong( 0x0 ),	/* 0 */
/* 360 */	NdrFcShort( 0xd ),	/* 13 */
/* 362 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 364 */	NdrFcShort( 0x0 ),	/* 0 */
/* 366 */	NdrFcShort( 0x34 ),	/* 52 */
/* 368 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 370 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 372 */	NdrFcShort( 0x0 ),	/* 0 */
/* 374 */	NdrFcShort( 0x1 ),	/* 1 */
/* 376 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pCharacterPath */

/* 378 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 380 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 382 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter pFrameSize */

/* 384 */	NdrFcShort( 0x2112 ),	/* Flags:  must free, out, simple ref, srv alloc size=8 */
/* 386 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 388 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 390 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 392 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 394 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetFrameFormat */

/* 396 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 398 */	NdrFcLong( 0x0 ),	/* 0 */
/* 402 */	NdrFcShort( 0xe ),	/* 14 */
/* 404 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 406 */	NdrFcShort( 0x0 ),	/* 0 */
/* 408 */	NdrFcShort( 0x35 ),	/* 53 */
/* 410 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 412 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 414 */	NdrFcShort( 0x0 ),	/* 0 */
/* 416 */	NdrFcShort( 0x0 ),	/* 0 */
/* 418 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pFrameFormat */

/* 420 */	NdrFcShort( 0x2012 ),	/* Flags:  must free, out, srv alloc size=8 */
/* 422 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 424 */	NdrFcShort( 0x36 ),	/* Type Offset=54 */

	/* Return value */

/* 426 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 428 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 430 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetCharacterFrameFormat */

/* 432 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 434 */	NdrFcLong( 0x0 ),	/* 0 */
/* 438 */	NdrFcShort( 0xf ),	/* 15 */
/* 440 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 442 */	NdrFcShort( 0x0 ),	/* 0 */
/* 444 */	NdrFcShort( 0x35 ),	/* 53 */
/* 446 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 448 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 450 */	NdrFcShort( 0x0 ),	/* 0 */
/* 452 */	NdrFcShort( 0x1 ),	/* 1 */
/* 454 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pCharacterPath */

/* 456 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 458 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 460 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter pFrameFormat */

/* 462 */	NdrFcShort( 0x2012 ),	/* Flags:  must free, out, srv alloc size=8 */
/* 464 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 466 */	NdrFcShort( 0x36 ),	/* Type Offset=54 */

	/* Return value */

/* 468 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 470 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 472 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnAppActive */

/* 474 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 476 */	NdrFcLong( 0x0 ),	/* 0 */
/* 480 */	NdrFcShort( 0x10 ),	/* 16 */
/* 482 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 484 */	NdrFcShort( 0x5 ),	/* 5 */
/* 486 */	NdrFcShort( 0x8 ),	/* 8 */
/* 488 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 490 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 492 */	NdrFcShort( 0x0 ),	/* 0 */
/* 494 */	NdrFcShort( 0x0 ),	/* 0 */
/* 496 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pActive */

/* 498 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 500 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 502 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */

/* 504 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 506 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 508 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RenderFrame */

/* 510 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 512 */	NdrFcLong( 0x0 ),	/* 0 */
/* 516 */	NdrFcShort( 0x11 ),	/* 17 */
/* 518 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 520 */	NdrFcShort( 0x2c ),	/* 44 */
/* 522 */	NdrFcShort( 0x8 ),	/* 8 */
/* 524 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 526 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 528 */	NdrFcShort( 0x0 ),	/* 0 */
/* 530 */	NdrFcShort( 0x1 ),	/* 1 */
/* 532 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pDC */

/* 534 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 536 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 538 */	NdrFcShort( 0x56 ),	/* Type Offset=86 */

	/* Parameter pPosition */

/* 540 */	NdrFcShort( 0x10a ),	/* Flags:  must free, in, simple ref, */
/* 542 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 544 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 546 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 548 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 550 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RenderAnimationFrame */

/* 552 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 554 */	NdrFcLong( 0x0 ),	/* 0 */
/* 558 */	NdrFcShort( 0x12 ),	/* 18 */
/* 560 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 562 */	NdrFcShort( 0x32 ),	/* 50 */
/* 564 */	NdrFcShort( 0x8 ),	/* 8 */
/* 566 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 568 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 570 */	NdrFcShort( 0x0 ),	/* 0 */
/* 572 */	NdrFcShort( 0x1 ),	/* 1 */
/* 574 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAnimationName */

/* 576 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 578 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 580 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter pFrameNum */

/* 582 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 584 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 586 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter pDC */

/* 588 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 590 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 592 */	NdrFcShort( 0x56 ),	/* Type Offset=86 */

	/* Parameter pPosition */

/* 594 */	NdrFcShort( 0x10a ),	/* Flags:  must free, in, simple ref, */
/* 596 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 598 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 600 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 602 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 604 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RenderCharacterFrame */

/* 606 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 608 */	NdrFcLong( 0x0 ),	/* 0 */
/* 612 */	NdrFcShort( 0x13 ),	/* 19 */
/* 614 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 616 */	NdrFcShort( 0x32 ),	/* 50 */
/* 618 */	NdrFcShort( 0x8 ),	/* 8 */
/* 620 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 622 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 624 */	NdrFcShort( 0x0 ),	/* 0 */
/* 626 */	NdrFcShort( 0x1 ),	/* 1 */
/* 628 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pCharacterPath */

/* 630 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 632 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 634 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter pAnimationName */

/* 636 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 638 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 640 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter pFrameNum */

/* 642 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 644 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 646 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter pDC */

/* 648 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 650 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 652 */	NdrFcShort( 0x56 ),	/* Type Offset=86 */

	/* Parameter pPosition */

/* 654 */	NdrFcShort( 0x10a ),	/* Flags:  must free, in, simple ref, */
/* 656 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 658 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 660 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 662 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 664 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OpenFile */

/* 666 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 668 */	NdrFcLong( 0x0 ),	/* 0 */
/* 672 */	NdrFcShort( 0x14 ),	/* 20 */
/* 674 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 676 */	NdrFcShort( 0x0 ),	/* 0 */
/* 678 */	NdrFcShort( 0x8 ),	/* 8 */
/* 680 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 682 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 684 */	NdrFcShort( 0x0 ),	/* 0 */
/* 686 */	NdrFcShort( 0x1 ),	/* 1 */
/* 688 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pCharacterPath */

/* 690 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 692 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 694 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter pParentWnd */

/* 696 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 698 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 700 */	NdrFcShort( 0x64 ),	/* Type Offset=100 */

	/* Return value */

/* 702 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 704 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 706 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetCharacterPath */

/* 708 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 710 */	NdrFcLong( 0x0 ),	/* 0 */
/* 714 */	NdrFcShort( 0x15 ),	/* 21 */
/* 716 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 718 */	NdrFcShort( 0x0 ),	/* 0 */
/* 720 */	NdrFcShort( 0x8 ),	/* 8 */
/* 722 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 724 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 726 */	NdrFcShort( 0x1 ),	/* 1 */
/* 728 */	NdrFcShort( 0x0 ),	/* 0 */
/* 730 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pCharacterPath */

/* 732 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 734 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 736 */	NdrFcShort( 0x76 ),	/* Type Offset=118 */

	/* Return value */

/* 738 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 740 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 742 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetCharacterName */

/* 744 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 746 */	NdrFcLong( 0x0 ),	/* 0 */
/* 750 */	NdrFcShort( 0x16 ),	/* 22 */
/* 752 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 754 */	NdrFcShort( 0x6 ),	/* 6 */
/* 756 */	NdrFcShort( 0x8 ),	/* 8 */
/* 758 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 760 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 762 */	NdrFcShort( 0x1 ),	/* 1 */
/* 764 */	NdrFcShort( 0x0 ),	/* 0 */
/* 766 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pCharacterName */

/* 768 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 770 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 772 */	NdrFcShort( 0x76 ),	/* Type Offset=118 */

	/* Parameter pLangID */

/* 774 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 776 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 778 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 780 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 782 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 784 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetCharacterDescription */

/* 786 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 788 */	NdrFcLong( 0x0 ),	/* 0 */
/* 792 */	NdrFcShort( 0x17 ),	/* 23 */
/* 794 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 796 */	NdrFcShort( 0x6 ),	/* 6 */
/* 798 */	NdrFcShort( 0x8 ),	/* 8 */
/* 800 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 802 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 804 */	NdrFcShort( 0x1 ),	/* 1 */
/* 806 */	NdrFcShort( 0x0 ),	/* 0 */
/* 808 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pCharacterDescription */

/* 810 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 812 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 814 */	NdrFcShort( 0x76 ),	/* Type Offset=118 */

	/* Parameter pLangID */

/* 816 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 818 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 820 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 822 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 824 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 826 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetCharacterGuid */

/* 828 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 830 */	NdrFcLong( 0x0 ),	/* 0 */
/* 834 */	NdrFcShort( 0x18 ),	/* 24 */
/* 836 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 838 */	NdrFcShort( 0x0 ),	/* 0 */
/* 840 */	NdrFcShort( 0x4c ),	/* 76 */
/* 842 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 844 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 846 */	NdrFcShort( 0x0 ),	/* 0 */
/* 848 */	NdrFcShort( 0x0 ),	/* 0 */
/* 850 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pGuid */

/* 852 */	NdrFcShort( 0x4112 ),	/* Flags:  must free, out, simple ref, srv alloc size=16 */
/* 854 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 856 */	NdrFcShort( 0x8a ),	/* Type Offset=138 */

	/* Return value */

/* 858 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 860 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 862 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetAgentFile */


	/* Procedure GetMaxSequenceDuration */

/* 864 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 866 */	NdrFcLong( 0x0 ),	/* 0 */
/* 870 */	NdrFcShort( 0x3 ),	/* 3 */
/* 872 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 874 */	NdrFcShort( 0x0 ),	/* 0 */
/* 876 */	NdrFcShort( 0x24 ),	/* 36 */
/* 878 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 880 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 882 */	NdrFcShort( 0x0 ),	/* 0 */
/* 884 */	NdrFcShort( 0x0 ),	/* 0 */
/* 886 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAgentFile */


	/* Parameter pMaxSequenceDuration */

/* 888 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 890 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 892 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 894 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 896 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 898 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetMaxSequenceFrames */

/* 900 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 902 */	NdrFcLong( 0x0 ),	/* 0 */
/* 906 */	NdrFcShort( 0x4 ),	/* 4 */
/* 908 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 910 */	NdrFcShort( 0x0 ),	/* 0 */
/* 912 */	NdrFcShort( 0x24 ),	/* 36 */
/* 914 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 916 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 918 */	NdrFcShort( 0x0 ),	/* 0 */
/* 920 */	NdrFcShort( 0x0 ),	/* 0 */
/* 922 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pMaxSequenceFrames */

/* 924 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 926 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 928 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 930 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 932 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 934 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetBkColor */


	/* Procedure GetAgentStreamInfo */


	/* Procedure GetAnimationIndex */

/* 936 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 938 */	NdrFcLong( 0x0 ),	/* 0 */
/* 942 */	NdrFcShort( 0x5 ),	/* 5 */
/* 944 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 946 */	NdrFcShort( 0x0 ),	/* 0 */
/* 948 */	NdrFcShort( 0x24 ),	/* 36 */
/* 950 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 952 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 954 */	NdrFcShort( 0x0 ),	/* 0 */
/* 956 */	NdrFcShort( 0x0 ),	/* 0 */
/* 958 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pBkColor */


	/* Parameter pAgentStreamInfo */


	/* Parameter pAnimationNdx */

/* 960 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 962 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 964 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */


	/* Return value */

/* 966 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 968 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 970 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetAgentStreamInfo */


	/* Procedure SetAnimationIndex */

/* 972 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 974 */	NdrFcLong( 0x0 ),	/* 0 */
/* 978 */	NdrFcShort( 0x6 ),	/* 6 */
/* 980 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 982 */	NdrFcShort( 0x8 ),	/* 8 */
/* 984 */	NdrFcShort( 0x8 ),	/* 8 */
/* 986 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 988 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 990 */	NdrFcShort( 0x0 ),	/* 0 */
/* 992 */	NdrFcShort( 0x0 ),	/* 0 */
/* 994 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAgentStreamInfo */


	/* Parameter pAnimationNdx */

/* 996 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 998 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1000 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 1002 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1004 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1006 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetAnimationName */

/* 1008 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1010 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1014 */	NdrFcShort( 0x7 ),	/* 7 */
/* 1016 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1018 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1020 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1022 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1024 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 1026 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1028 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1030 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAnimationName */

/* 1032 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 1034 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1036 */	NdrFcShort( 0x76 ),	/* Type Offset=118 */

	/* Return value */

/* 1038 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1040 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1042 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetAnimationName */

/* 1044 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1046 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1050 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1052 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1054 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1056 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1058 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1060 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1062 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1064 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1066 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAnimationName */

/* 1068 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1070 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1072 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 1074 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1076 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1078 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetAnimationSource */

/* 1080 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1082 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1086 */	NdrFcShort( 0x9 ),	/* 9 */
/* 1088 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1090 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1092 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1094 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1096 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 1098 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1100 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1102 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAnimationSource */

/* 1104 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 1106 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1108 */	NdrFcShort( 0x76 ),	/* Type Offset=118 */

	/* Return value */

/* 1110 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1112 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1114 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetAnimationSource */

/* 1116 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1118 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1122 */	NdrFcShort( 0xa ),	/* 10 */
/* 1124 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1126 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1128 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1130 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1132 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1134 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1136 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1138 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAnimationSource */

/* 1140 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1142 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1144 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 1146 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1148 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1150 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CalcAnimationFrameCount */

/* 1152 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1154 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1158 */	NdrFcShort( 0xb ),	/* 11 */
/* 1160 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1162 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1164 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1166 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 1168 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1170 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1172 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1174 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAnimationFrameCount */

/* 1176 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1178 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1180 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pAnimationNdx */

/* 1182 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1184 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1186 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1188 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1190 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1192 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CalcAnimationDuration */

/* 1194 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1196 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1200 */	NdrFcShort( 0xc ),	/* 12 */
/* 1202 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1204 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1206 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1208 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 1210 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1212 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1214 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1216 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAnimationDuration */

/* 1218 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1220 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1222 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pAnimationNdx */

/* 1224 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1226 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1228 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1230 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1232 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1234 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure FindAnimationSpeakingFrame */

/* 1236 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1238 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1242 */	NdrFcShort( 0xd ),	/* 13 */
/* 1244 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1246 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1248 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1250 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 1252 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1254 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1256 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1258 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pSpeakingFrameNdx */

/* 1260 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1262 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1264 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pAnimationNdx */

/* 1266 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1268 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1270 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pStartFrameNdx */

/* 1272 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1274 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1276 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1278 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1280 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1282 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NewAnimationSequence */

/* 1284 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1286 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1290 */	NdrFcShort( 0xe ),	/* 14 */
/* 1292 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1294 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1296 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1298 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 1300 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1302 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1304 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1306 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 1308 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1310 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1312 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure EndAnimationSequence */

/* 1314 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1316 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1320 */	NdrFcShort( 0xf ),	/* 15 */
/* 1322 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1324 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1326 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1328 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 1330 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1332 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1334 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1336 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 1338 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1340 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1342 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ClearAnimationSequences */

/* 1344 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1346 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1350 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1352 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1354 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1356 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1358 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 1360 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1362 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1364 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1366 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 1368 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1370 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1372 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetSequenceFrameCount */

/* 1374 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1376 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1380 */	NdrFcShort( 0x11 ),	/* 17 */
/* 1382 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1384 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1386 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1388 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1390 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1392 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1394 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1396 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pSequenceFrameCount */

/* 1398 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1400 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1402 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1404 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1406 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1408 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetSequenceDuration */

/* 1410 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1412 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1416 */	NdrFcShort( 0x12 ),	/* 18 */
/* 1418 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1420 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1422 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1424 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1426 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1428 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1430 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1432 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pSequenceDuration */

/* 1434 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1436 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1438 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1440 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1442 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1444 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetSequenceLoop */

/* 1446 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1448 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1452 */	NdrFcShort( 0x13 ),	/* 19 */
/* 1454 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1456 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1458 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1460 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1462 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1464 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1466 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1468 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pLoopDuration */

/* 1470 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1472 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1474 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1476 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1478 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1480 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CalcSequenceTimeNdx */

/* 1482 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1484 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1488 */	NdrFcShort( 0x14 ),	/* 20 */
/* 1490 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1492 */	NdrFcShort( 0xd ),	/* 13 */
/* 1494 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1496 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 1498 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1500 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1502 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1504 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pTimeNdx */

/* 1506 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1508 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1510 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pFrameNum */

/* 1512 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1514 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1516 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pClampFrameNum */

/* 1518 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1520 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1522 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */

/* 1524 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1526 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1528 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CalcSequenceFrameNum */

/* 1530 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1532 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1536 */	NdrFcShort( 0x15 ),	/* 21 */
/* 1538 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1540 */	NdrFcShort( 0xd ),	/* 13 */
/* 1542 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1544 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 1546 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1548 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1550 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1552 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pFrameNum */

/* 1554 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1556 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1558 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pTimeNdx */

/* 1560 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1562 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1564 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pClampTimeNdx */

/* 1566 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1568 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1570 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */

/* 1572 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1574 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1576 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CalcSequenceAnimationFrameNdx */

/* 1578 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1580 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1584 */	NdrFcShort( 0x16 ),	/* 22 */
/* 1586 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1588 */	NdrFcShort( 0xd ),	/* 13 */
/* 1590 */	NdrFcShort( 0x40 ),	/* 64 */
/* 1592 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x5,		/* 5 */
/* 1594 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1596 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1598 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1600 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAnimationNdx */

/* 1602 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1604 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1606 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pFrameNdx */

/* 1608 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1610 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1612 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pTimeNdx */

/* 1614 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1616 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1618 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pClampTimeNdx */

/* 1620 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1622 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1624 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */

/* 1626 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1628 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1630 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SequenceAll */

/* 1632 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1634 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1638 */	NdrFcShort( 0x17 ),	/* 23 */
/* 1640 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1642 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1644 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1646 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 1648 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1650 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1652 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1654 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 1656 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1658 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1660 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SequenceAnimation */

/* 1662 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1664 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1668 */	NdrFcShort( 0x18 ),	/* 24 */
/* 1670 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1672 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1674 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1676 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 1678 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1680 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1682 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1684 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAnimationNdx */

/* 1686 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1688 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1690 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pMaxLoopTime */

/* 1692 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1694 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1696 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1698 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1700 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1702 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SequenceAnimationLoop */

/* 1704 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1706 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1710 */	NdrFcShort( 0x19 ),	/* 25 */
/* 1712 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1714 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1716 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1718 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 1720 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1722 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1724 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1726 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAnimationNdx */

/* 1728 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1730 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1732 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pMaxLoopTime */

/* 1734 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1736 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1738 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1740 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1742 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1744 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SequenceAnimationExit */

/* 1746 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1748 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1752 */	NdrFcShort( 0x1a ),	/* 26 */
/* 1754 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1756 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1758 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1760 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 1762 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1764 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1766 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1768 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAnimationNdx */

/* 1770 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1772 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1774 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pLastFrameNdx */

/* 1776 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1778 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1780 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1782 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1784 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1786 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CueSequenceAudio */

/* 1788 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1790 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1794 */	NdrFcShort( 0x1b ),	/* 27 */
/* 1796 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1798 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1800 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1802 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1804 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1806 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1808 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1810 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pStartFrameNum */

/* 1812 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1814 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1816 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1818 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1820 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1822 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ClearSequenceAudio */

/* 1824 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1826 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1830 */	NdrFcShort( 0x1c ),	/* 28 */
/* 1832 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1834 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1836 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1838 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 1840 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1842 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1844 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1846 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 1848 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1850 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1852 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetAgentFile */

/* 1854 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1856 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1860 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1862 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1864 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1866 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1868 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1870 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1872 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1874 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1876 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAgentFile */

/* 1878 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1880 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1882 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1884 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1886 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1888 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetSmoothing */


	/* Procedure GetBkColor */

/* 1890 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1892 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1896 */	NdrFcShort( 0x7 ),	/* 7 */
/* 1898 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1900 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1902 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1904 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1906 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1908 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1910 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1912 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pSmoothing */


	/* Parameter pBkColor */

/* 1914 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1916 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1918 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 1920 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1922 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1924 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetBkColor */

/* 1926 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1928 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1932 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1934 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1936 */	NdrFcShort( 0x1c ),	/* 28 */
/* 1938 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1940 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1942 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1944 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1946 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1948 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pBkColor */

/* 1950 */	NdrFcShort( 0x148 ),	/* Flags:  in, base type, simple ref, */
/* 1952 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1954 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1956 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1958 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1960 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SegmentDurationChanged */

/* 1962 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1964 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1968 */	NdrFcShort( 0x9 ),	/* 9 */
/* 1970 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1972 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1974 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1976 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 1978 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1980 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1982 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1984 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 1986 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1988 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1990 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetRenderWnd */

/* 1992 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1994 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1998 */	NdrFcShort( 0x3 ),	/* 3 */
/* 2000 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2002 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2004 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2006 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2008 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 2010 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2012 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2014 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pRenderWnd */

/* 2016 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 2018 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2020 */	NdrFcShort( 0xa6 ),	/* Type Offset=166 */

	/* Return value */

/* 2022 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2024 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2026 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetRenderWnd */

/* 2028 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2030 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2034 */	NdrFcShort( 0x4 ),	/* 4 */
/* 2036 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2038 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2040 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2042 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 2044 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2046 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2048 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2050 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pRenderWnd */

/* 2052 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2054 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2056 */	NdrFcShort( 0x64 ),	/* Type Offset=100 */

	/* Return value */

/* 2058 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2060 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2062 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetBkColor */

/* 2064 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2066 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2070 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2072 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2074 */	NdrFcShort( 0x1c ),	/* 28 */
/* 2076 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2078 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2080 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2082 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2084 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2086 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pBkColor */

/* 2088 */	NdrFcShort( 0x148 ),	/* Flags:  in, base type, simple ref, */
/* 2090 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2092 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2094 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2096 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2098 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetSmoothing */

/* 2100 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2102 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2106 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2108 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2110 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2112 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2114 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2116 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2118 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2120 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2122 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pSmoothing */

/* 2124 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2126 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2128 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2130 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2132 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2134 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetImageSize */

/* 2136 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2138 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2142 */	NdrFcShort( 0x9 ),	/* 9 */
/* 2144 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2146 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2148 */	NdrFcShort( 0x40 ),	/* 64 */
/* 2150 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 2152 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2154 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2156 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2158 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pImageWidth */

/* 2160 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2162 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2164 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pImageHeight */

/* 2166 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2168 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2170 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2172 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2174 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2176 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DrawSampleImage */

/* 2178 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2180 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2184 */	NdrFcShort( 0xa ),	/* 10 */
/* 2186 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2188 */	NdrFcShort( 0x34 ),	/* 52 */
/* 2190 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2192 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 2194 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2196 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2198 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2200 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pDC */

/* 2202 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2204 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2206 */	NdrFcShort( 0x56 ),	/* Type Offset=86 */

	/* Parameter pTargetRect */

/* 2208 */	NdrFcShort( 0x10a ),	/* Flags:  must free, in, simple ref, */
/* 2210 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2212 */	NdrFcShort( 0xb4 ),	/* Type Offset=180 */

	/* Return value */

/* 2214 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2216 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2218 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const DaCore_MIDL_TYPE_FORMAT_STRING DaCore__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x12, 0x0,	/* FC_UP */
/*  4 */	NdrFcShort( 0xe ),	/* Offset= 14 (18) */
/*  6 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/*  8 */	NdrFcShort( 0x2 ),	/* 2 */
/* 10 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 12 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 14 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 16 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 18 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 20 */	NdrFcShort( 0x8 ),	/* 8 */
/* 22 */	NdrFcShort( 0xfff0 ),	/* Offset= -16 (6) */
/* 24 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 26 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 28 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 30 */	NdrFcShort( 0x0 ),	/* 0 */
/* 32 */	NdrFcShort( 0x4 ),	/* 4 */
/* 34 */	NdrFcShort( 0x0 ),	/* 0 */
/* 36 */	NdrFcShort( 0xffde ),	/* Offset= -34 (2) */
/* 38 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 40 */	0x3,		/* FC_SMALL */
			0x5c,		/* FC_PAD */
/* 42 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 44 */	NdrFcShort( 0x2 ),	/* Offset= 2 (46) */
/* 46 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 48 */	NdrFcShort( 0x8 ),	/* 8 */
/* 50 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 52 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 54 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 56 */	NdrFcShort( 0x2 ),	/* Offset= 2 (58) */
/* 58 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 60 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 62 */	
			0x12, 0x0,	/* FC_UP */
/* 64 */	NdrFcShort( 0x2 ),	/* Offset= 2 (66) */
/* 66 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x48,		/* 72 */
/* 68 */	NdrFcShort( 0x4 ),	/* 4 */
/* 70 */	NdrFcShort( 0x2 ),	/* 2 */
/* 72 */	NdrFcLong( 0x48746457 ),	/* 1215587415 */
/* 76 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 78 */	NdrFcLong( 0x52746457 ),	/* 1383359575 */
/* 82 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 84 */	NdrFcShort( 0xffff ),	/* Offset= -1 (83) */
/* 86 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 88 */	NdrFcShort( 0x1 ),	/* 1 */
/* 90 */	NdrFcShort( 0x4 ),	/* 4 */
/* 92 */	NdrFcShort( 0x0 ),	/* 0 */
/* 94 */	NdrFcShort( 0xffe0 ),	/* Offset= -32 (62) */
/* 96 */	
			0x11, 0x0,	/* FC_RP */
/* 98 */	NdrFcShort( 0xffcc ),	/* Offset= -52 (46) */
/* 100 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 102 */	NdrFcShort( 0x2 ),	/* 2 */
/* 104 */	NdrFcShort( 0x4 ),	/* 4 */
/* 106 */	NdrFcShort( 0x0 ),	/* 0 */
/* 108 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (62) */
/* 110 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 112 */	NdrFcShort( 0x6 ),	/* Offset= 6 (118) */
/* 114 */	
			0x13, 0x0,	/* FC_OP */
/* 116 */	NdrFcShort( 0xff9e ),	/* Offset= -98 (18) */
/* 118 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 120 */	NdrFcShort( 0x0 ),	/* 0 */
/* 122 */	NdrFcShort( 0x4 ),	/* 4 */
/* 124 */	NdrFcShort( 0x0 ),	/* 0 */
/* 126 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (114) */
/* 128 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 130 */	NdrFcShort( 0x8 ),	/* Offset= 8 (138) */
/* 132 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 134 */	NdrFcShort( 0x8 ),	/* 8 */
/* 136 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 138 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 140 */	NdrFcShort( 0x10 ),	/* 16 */
/* 142 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 144 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 146 */	0x0,		/* 0 */
			NdrFcShort( 0xfff1 ),	/* Offset= -15 (132) */
			0x5b,		/* FC_END */
/* 150 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 152 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 154 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/* 156 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 158 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 160 */	NdrFcShort( 0x6 ),	/* Offset= 6 (166) */
/* 162 */	
			0x13, 0x0,	/* FC_OP */
/* 164 */	NdrFcShort( 0xff9e ),	/* Offset= -98 (66) */
/* 166 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 168 */	NdrFcShort( 0x2 ),	/* 2 */
/* 170 */	NdrFcShort( 0x4 ),	/* 4 */
/* 172 */	NdrFcShort( 0x0 ),	/* 0 */
/* 174 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (162) */
/* 176 */	
			0x11, 0x0,	/* FC_RP */
/* 178 */	NdrFcShort( 0x2 ),	/* Offset= 2 (180) */
/* 180 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 182 */	NdrFcShort( 0x10 ),	/* 16 */
/* 184 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 186 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 188 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */

			0x0
        }
    };

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            },
            {
            HDC_UserSize
            ,HDC_UserMarshal
            ,HDC_UserUnmarshal
            ,HDC_UserFree
            },
            {
            HWND_UserSize
            ,HWND_UserMarshal
            ,HWND_UserUnmarshal
            ,HWND_UserFree
            }

        };



/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDaPreview, ver. 0.0,
   GUID={0x1147E560,0xA208,0x11DE,{0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2}} */

#pragma code_seg(".orpc")
static const unsigned short IDaPreview_FormatStringOffsetTable[] =
    {
    0,
    36,
    72,
    108,
    138,
    174,
    210,
    246,
    282,
    318,
    354,
    396,
    432,
    474,
    510,
    552,
    606,
    666,
    708,
    744,
    786,
    828
    };

static const MIDL_STUBLESS_PROXY_INFO IDaPreview_ProxyInfo =
    {
    &Object_StubDesc,
    DaCore__MIDL_ProcFormatString.Format,
    &IDaPreview_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IDaPreview_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    DaCore__MIDL_ProcFormatString.Format,
    &IDaPreview_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(25) _IDaPreviewProxyVtbl = 
{
    &IDaPreview_ProxyInfo,
    &IID_IDaPreview,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* IDaPreview::PlayAnimation */ ,
    (void *) (INT_PTR) -1 /* IDaPreview::PlayState */ ,
    (void *) (INT_PTR) -1 /* IDaPreview::GetIsPlaying */ ,
    (void *) (INT_PTR) -1 /* IDaPreview::StopPlaying */ ,
    (void *) (INT_PTR) -1 /* IDaPreview::GetSoundsEnabled */ ,
    (void *) (INT_PTR) -1 /* IDaPreview::EnableSounds */ ,
    (void *) (INT_PTR) -1 /* IDaPreview::GetIdleEnabled */ ,
    (void *) (INT_PTR) -1 /* IDaPreview::EnableIdle */ ,
    (void *) (INT_PTR) -1 /* IDaPreview::SetBkColor */ ,
    (void *) (INT_PTR) -1 /* IDaPreview::GetFrameSize */ ,
    (void *) (INT_PTR) -1 /* IDaPreview::GetCharacterFrameSize */ ,
    (void *) (INT_PTR) -1 /* IDaPreview::GetFrameFormat */ ,
    (void *) (INT_PTR) -1 /* IDaPreview::GetCharacterFrameFormat */ ,
    (void *) (INT_PTR) -1 /* IDaPreview::OnAppActive */ ,
    (void *) (INT_PTR) -1 /* IDaPreview::RenderFrame */ ,
    (void *) (INT_PTR) -1 /* IDaPreview::RenderAnimationFrame */ ,
    (void *) (INT_PTR) -1 /* IDaPreview::RenderCharacterFrame */ ,
    (void *) (INT_PTR) -1 /* IDaPreview::OpenFile */ ,
    (void *) (INT_PTR) -1 /* IDaPreview::GetCharacterPath */ ,
    (void *) (INT_PTR) -1 /* IDaPreview::GetCharacterName */ ,
    (void *) (INT_PTR) -1 /* IDaPreview::GetCharacterDescription */ ,
    (void *) (INT_PTR) -1 /* IDaPreview::GetCharacterGuid */
};

const CInterfaceStubVtbl _IDaPreviewStubVtbl =
{
    &IID_IDaPreview,
    &IDaPreview_ServerInfo,
    25,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: _IAgentStreamInfo, ver. 0.0,
   GUID={0x1147E562,0xA208,0x11DE,{0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2}} */

#pragma code_seg(".orpc")
static const unsigned short _IAgentStreamInfo_FormatStringOffsetTable[] =
    {
    864,
    900,
    936,
    972,
    1008,
    1044,
    1080,
    1116,
    1152,
    1194,
    1236,
    1284,
    1314,
    1344,
    1374,
    1410,
    1446,
    1482,
    1530,
    1578,
    1632,
    1662,
    1704,
    1746,
    1788,
    1824
    };

static const MIDL_STUBLESS_PROXY_INFO _IAgentStreamInfo_ProxyInfo =
    {
    &Object_StubDesc,
    DaCore__MIDL_ProcFormatString.Format,
    &_IAgentStreamInfo_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO _IAgentStreamInfo_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    DaCore__MIDL_ProcFormatString.Format,
    &_IAgentStreamInfo_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(29) __IAgentStreamInfoProxyVtbl = 
{
    &_IAgentStreamInfo_ProxyInfo,
    &IID__IAgentStreamInfo,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* _IAgentStreamInfo::GetMaxSequenceDuration */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamInfo::GetMaxSequenceFrames */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamInfo::GetAnimationIndex */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamInfo::SetAnimationIndex */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamInfo::GetAnimationName */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamInfo::SetAnimationName */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamInfo::GetAnimationSource */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamInfo::SetAnimationSource */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamInfo::CalcAnimationFrameCount */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamInfo::CalcAnimationDuration */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamInfo::FindAnimationSpeakingFrame */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamInfo::NewAnimationSequence */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamInfo::EndAnimationSequence */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamInfo::ClearAnimationSequences */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamInfo::GetSequenceFrameCount */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamInfo::GetSequenceDuration */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamInfo::GetSequenceLoop */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamInfo::CalcSequenceTimeNdx */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamInfo::CalcSequenceFrameNum */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamInfo::CalcSequenceAnimationFrameNdx */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamInfo::SequenceAll */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamInfo::SequenceAnimation */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamInfo::SequenceAnimationLoop */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamInfo::SequenceAnimationExit */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamInfo::CueSequenceAudio */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamInfo::ClearSequenceAudio */
};

const CInterfaceStubVtbl __IAgentStreamInfoStubVtbl =
{
    &IID__IAgentStreamInfo,
    &_IAgentStreamInfo_ServerInfo,
    29,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: _IAgentStreamSource, ver. 0.0,
   GUID={0x1147E567,0xA208,0x11DE,{0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2}} */

#pragma code_seg(".orpc")
static const unsigned short _IAgentStreamSource_FormatStringOffsetTable[] =
    {
    864,
    1854,
    936,
    972,
    1890,
    1926,
    1962
    };

static const MIDL_STUBLESS_PROXY_INFO _IAgentStreamSource_ProxyInfo =
    {
    &Object_StubDesc,
    DaCore__MIDL_ProcFormatString.Format,
    &_IAgentStreamSource_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO _IAgentStreamSource_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    DaCore__MIDL_ProcFormatString.Format,
    &_IAgentStreamSource_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(10) __IAgentStreamSourceProxyVtbl = 
{
    &_IAgentStreamSource_ProxyInfo,
    &IID__IAgentStreamSource,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* _IAgentStreamSource::GetAgentFile */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamSource::SetAgentFile */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamSource::GetAgentStreamInfo */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamSource::SetAgentStreamInfo */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamSource::GetBkColor */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamSource::SetBkColor */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamSource::SegmentDurationChanged */
};

const CInterfaceStubVtbl __IAgentStreamSourceStubVtbl =
{
    &IID__IAgentStreamSource,
    &_IAgentStreamSource_ServerInfo,
    10,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Standard interface: __MIDL_itf_DaCore_0000_0003, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: _IAgentStreamRender, ver. 0.0,
   GUID={0x1147E568,0xA208,0x11DE,{0xAB,0xF2,0x00,0x24,0x21,0x11,0x6F,0xB2}} */

#pragma code_seg(".orpc")
static const unsigned short _IAgentStreamRender_FormatStringOffsetTable[] =
    {
    1992,
    2028,
    936,
    2064,
    1890,
    2100,
    2136,
    2178
    };

static const MIDL_STUBLESS_PROXY_INFO _IAgentStreamRender_ProxyInfo =
    {
    &Object_StubDesc,
    DaCore__MIDL_ProcFormatString.Format,
    &_IAgentStreamRender_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO _IAgentStreamRender_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    DaCore__MIDL_ProcFormatString.Format,
    &_IAgentStreamRender_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(11) __IAgentStreamRenderProxyVtbl = 
{
    &_IAgentStreamRender_ProxyInfo,
    &IID__IAgentStreamRender,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* _IAgentStreamRender::GetRenderWnd */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamRender::SetRenderWnd */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamRender::GetBkColor */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamRender::SetBkColor */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamRender::GetSmoothing */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamRender::SetSmoothing */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamRender::GetImageSize */ ,
    (void *) (INT_PTR) -1 /* _IAgentStreamRender::DrawSampleImage */
};

const CInterfaceStubVtbl __IAgentStreamRenderStubVtbl =
{
    &IID__IAgentStreamRender,
    &_IAgentStreamRender_ServerInfo,
    11,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    DaCore__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x700022b, /* MIDL Version 7.0.555 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };

const CInterfaceProxyVtbl * const _DaCore_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IDaPreviewProxyVtbl,
    ( CInterfaceProxyVtbl *) &__IAgentStreamInfoProxyVtbl,
    ( CInterfaceProxyVtbl *) &__IAgentStreamSourceProxyVtbl,
    ( CInterfaceProxyVtbl *) &__IAgentStreamRenderProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _DaCore_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IDaPreviewStubVtbl,
    ( CInterfaceStubVtbl *) &__IAgentStreamInfoStubVtbl,
    ( CInterfaceStubVtbl *) &__IAgentStreamSourceStubVtbl,
    ( CInterfaceStubVtbl *) &__IAgentStreamRenderStubVtbl,
    0
};

PCInterfaceName const _DaCore_InterfaceNamesList[] = 
{
    "IDaPreview",
    "_IAgentStreamInfo",
    "_IAgentStreamSource",
    "_IAgentStreamRender",
    0
};


#define _DaCore_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _DaCore, pIID, n)

int __stdcall _DaCore_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _DaCore, 4, 2 )
    IID_BS_LOOKUP_NEXT_TEST( _DaCore, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _DaCore, 4, *pIndex )
    
}

const ExtendedProxyFileInfo DaCore_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _DaCore_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _DaCore_StubVtblList,
    (const PCInterfaceName * ) & _DaCore_InterfaceNamesList,
    0, /* no delegation */
    & _DaCore_IID_Lookup, 
    4,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

