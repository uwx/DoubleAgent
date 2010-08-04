

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Mon Aug 02 15:16:02 2010
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

#if defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */

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

#define TYPE_FORMAT_STRING_SIZE   195                               
#define PROC_FORMAT_STRING_SIZE   2451                              
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

#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
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
/*  8 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 16 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x1 ),	/* 1 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */
/* 24 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAnimationName */

/* 26 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 28 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 30 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 32 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 34 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 36 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PlayState */

/* 38 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 40 */	NdrFcLong( 0x0 ),	/* 0 */
/* 44 */	NdrFcShort( 0x4 ),	/* 4 */
/* 46 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 48 */	NdrFcShort( 0x0 ),	/* 0 */
/* 50 */	NdrFcShort( 0x8 ),	/* 8 */
/* 52 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 54 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 56 */	NdrFcShort( 0x0 ),	/* 0 */
/* 58 */	NdrFcShort( 0x1 ),	/* 1 */
/* 60 */	NdrFcShort( 0x0 ),	/* 0 */
/* 62 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pStateName */

/* 64 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 66 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 68 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 70 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 72 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 74 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetIsPlaying */

/* 76 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 78 */	NdrFcLong( 0x0 ),	/* 0 */
/* 82 */	NdrFcShort( 0x5 ),	/* 5 */
/* 84 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 86 */	NdrFcShort( 0x0 ),	/* 0 */
/* 88 */	NdrFcShort( 0x21 ),	/* 33 */
/* 90 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 92 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 94 */	NdrFcShort( 0x0 ),	/* 0 */
/* 96 */	NdrFcShort( 0x0 ),	/* 0 */
/* 98 */	NdrFcShort( 0x0 ),	/* 0 */
/* 100 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pIsPlaying */

/* 102 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 104 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 106 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */

/* 108 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 110 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 112 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure StopPlaying */

/* 114 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 116 */	NdrFcLong( 0x0 ),	/* 0 */
/* 120 */	NdrFcShort( 0x6 ),	/* 6 */
/* 122 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 124 */	NdrFcShort( 0x0 ),	/* 0 */
/* 126 */	NdrFcShort( 0x8 ),	/* 8 */
/* 128 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 130 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 132 */	NdrFcShort( 0x0 ),	/* 0 */
/* 134 */	NdrFcShort( 0x0 ),	/* 0 */
/* 136 */	NdrFcShort( 0x0 ),	/* 0 */
/* 138 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 140 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 142 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 144 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetSoundsEnabled */

/* 146 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 148 */	NdrFcLong( 0x0 ),	/* 0 */
/* 152 */	NdrFcShort( 0x7 ),	/* 7 */
/* 154 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 156 */	NdrFcShort( 0x0 ),	/* 0 */
/* 158 */	NdrFcShort( 0x21 ),	/* 33 */
/* 160 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 162 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 164 */	NdrFcShort( 0x0 ),	/* 0 */
/* 166 */	NdrFcShort( 0x0 ),	/* 0 */
/* 168 */	NdrFcShort( 0x0 ),	/* 0 */
/* 170 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pEnabled */

/* 172 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 174 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 176 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */

/* 178 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 180 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 182 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure EnableSounds */

/* 184 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 186 */	NdrFcLong( 0x0 ),	/* 0 */
/* 190 */	NdrFcShort( 0x8 ),	/* 8 */
/* 192 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 194 */	NdrFcShort( 0x5 ),	/* 5 */
/* 196 */	NdrFcShort( 0x8 ),	/* 8 */
/* 198 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 200 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 202 */	NdrFcShort( 0x0 ),	/* 0 */
/* 204 */	NdrFcShort( 0x0 ),	/* 0 */
/* 206 */	NdrFcShort( 0x0 ),	/* 0 */
/* 208 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pEnabled */

/* 210 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 212 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 214 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */

/* 216 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 218 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 220 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetIdleEnabled */

/* 222 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 224 */	NdrFcLong( 0x0 ),	/* 0 */
/* 228 */	NdrFcShort( 0x9 ),	/* 9 */
/* 230 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 232 */	NdrFcShort( 0x0 ),	/* 0 */
/* 234 */	NdrFcShort( 0x21 ),	/* 33 */
/* 236 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 238 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 240 */	NdrFcShort( 0x0 ),	/* 0 */
/* 242 */	NdrFcShort( 0x0 ),	/* 0 */
/* 244 */	NdrFcShort( 0x0 ),	/* 0 */
/* 246 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pEnabled */

/* 248 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 250 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 252 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */

/* 254 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 256 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 258 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure EnableIdle */

/* 260 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 262 */	NdrFcLong( 0x0 ),	/* 0 */
/* 266 */	NdrFcShort( 0xa ),	/* 10 */
/* 268 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 270 */	NdrFcShort( 0x5 ),	/* 5 */
/* 272 */	NdrFcShort( 0x8 ),	/* 8 */
/* 274 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 276 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 278 */	NdrFcShort( 0x0 ),	/* 0 */
/* 280 */	NdrFcShort( 0x0 ),	/* 0 */
/* 282 */	NdrFcShort( 0x0 ),	/* 0 */
/* 284 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pEnabled */

/* 286 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 288 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 290 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */

/* 292 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 294 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 296 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetBkColor */

/* 298 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 300 */	NdrFcLong( 0x0 ),	/* 0 */
/* 304 */	NdrFcShort( 0xb ),	/* 11 */
/* 306 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 308 */	NdrFcShort( 0x8 ),	/* 8 */
/* 310 */	NdrFcShort( 0x8 ),	/* 8 */
/* 312 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 314 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 316 */	NdrFcShort( 0x0 ),	/* 0 */
/* 318 */	NdrFcShort( 0x0 ),	/* 0 */
/* 320 */	NdrFcShort( 0x0 ),	/* 0 */
/* 322 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pBkColor */

/* 324 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 326 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 328 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 330 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 332 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 334 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetFrameSize */

/* 336 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 338 */	NdrFcLong( 0x0 ),	/* 0 */
/* 342 */	NdrFcShort( 0xc ),	/* 12 */
/* 344 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 346 */	NdrFcShort( 0x0 ),	/* 0 */
/* 348 */	NdrFcShort( 0x34 ),	/* 52 */
/* 350 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 352 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 354 */	NdrFcShort( 0x0 ),	/* 0 */
/* 356 */	NdrFcShort( 0x0 ),	/* 0 */
/* 358 */	NdrFcShort( 0x0 ),	/* 0 */
/* 360 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pFrameSize */

/* 362 */	NdrFcShort( 0x2112 ),	/* Flags:  must free, out, simple ref, srv alloc size=8 */
/* 364 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 366 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 368 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 370 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 372 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetCharacterFrameSize */

/* 374 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 376 */	NdrFcLong( 0x0 ),	/* 0 */
/* 380 */	NdrFcShort( 0xd ),	/* 13 */
/* 382 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 384 */	NdrFcShort( 0x0 ),	/* 0 */
/* 386 */	NdrFcShort( 0x34 ),	/* 52 */
/* 388 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 390 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 392 */	NdrFcShort( 0x0 ),	/* 0 */
/* 394 */	NdrFcShort( 0x1 ),	/* 1 */
/* 396 */	NdrFcShort( 0x0 ),	/* 0 */
/* 398 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pCharacterPath */

/* 400 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 402 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 404 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter pFrameSize */

/* 406 */	NdrFcShort( 0x2112 ),	/* Flags:  must free, out, simple ref, srv alloc size=8 */
/* 408 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 410 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 412 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 414 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 416 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetFrameFormat */

/* 418 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 420 */	NdrFcLong( 0x0 ),	/* 0 */
/* 424 */	NdrFcShort( 0xe ),	/* 14 */
/* 426 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 428 */	NdrFcShort( 0x0 ),	/* 0 */
/* 430 */	NdrFcShort( 0x35 ),	/* 53 */
/* 432 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 434 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 436 */	NdrFcShort( 0x0 ),	/* 0 */
/* 438 */	NdrFcShort( 0x0 ),	/* 0 */
/* 440 */	NdrFcShort( 0x0 ),	/* 0 */
/* 442 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pFrameFormat */

/* 444 */	NdrFcShort( 0x2012 ),	/* Flags:  must free, out, srv alloc size=8 */
/* 446 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 448 */	NdrFcShort( 0x36 ),	/* Type Offset=54 */

	/* Return value */

/* 450 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 452 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 454 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetCharacterFrameFormat */

/* 456 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 458 */	NdrFcLong( 0x0 ),	/* 0 */
/* 462 */	NdrFcShort( 0xf ),	/* 15 */
/* 464 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 466 */	NdrFcShort( 0x0 ),	/* 0 */
/* 468 */	NdrFcShort( 0x35 ),	/* 53 */
/* 470 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 472 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 474 */	NdrFcShort( 0x0 ),	/* 0 */
/* 476 */	NdrFcShort( 0x1 ),	/* 1 */
/* 478 */	NdrFcShort( 0x0 ),	/* 0 */
/* 480 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pCharacterPath */

/* 482 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 484 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 486 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter pFrameFormat */

/* 488 */	NdrFcShort( 0x2012 ),	/* Flags:  must free, out, srv alloc size=8 */
/* 490 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 492 */	NdrFcShort( 0x36 ),	/* Type Offset=54 */

	/* Return value */

/* 494 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 496 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 498 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnAppActive */

/* 500 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 502 */	NdrFcLong( 0x0 ),	/* 0 */
/* 506 */	NdrFcShort( 0x10 ),	/* 16 */
/* 508 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 510 */	NdrFcShort( 0x5 ),	/* 5 */
/* 512 */	NdrFcShort( 0x8 ),	/* 8 */
/* 514 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 516 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 518 */	NdrFcShort( 0x0 ),	/* 0 */
/* 520 */	NdrFcShort( 0x0 ),	/* 0 */
/* 522 */	NdrFcShort( 0x0 ),	/* 0 */
/* 524 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pActive */

/* 526 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 528 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 530 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */

/* 532 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 534 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 536 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RenderFrame */

/* 538 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 540 */	NdrFcLong( 0x0 ),	/* 0 */
/* 544 */	NdrFcShort( 0x11 ),	/* 17 */
/* 546 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 548 */	NdrFcShort( 0x2c ),	/* 44 */
/* 550 */	NdrFcShort( 0x8 ),	/* 8 */
/* 552 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 554 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 556 */	NdrFcShort( 0x0 ),	/* 0 */
/* 558 */	NdrFcShort( 0x1 ),	/* 1 */
/* 560 */	NdrFcShort( 0x0 ),	/* 0 */
/* 562 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pDC */

/* 564 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 566 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 568 */	NdrFcShort( 0x56 ),	/* Type Offset=86 */

	/* Parameter pPosition */

/* 570 */	NdrFcShort( 0x10a ),	/* Flags:  must free, in, simple ref, */
/* 572 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 574 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 576 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 578 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 580 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RenderAnimationFrame */

/* 582 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 584 */	NdrFcLong( 0x0 ),	/* 0 */
/* 588 */	NdrFcShort( 0x12 ),	/* 18 */
/* 590 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 592 */	NdrFcShort( 0x32 ),	/* 50 */
/* 594 */	NdrFcShort( 0x8 ),	/* 8 */
/* 596 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x5,		/* 5 */
/* 598 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 600 */	NdrFcShort( 0x0 ),	/* 0 */
/* 602 */	NdrFcShort( 0x1 ),	/* 1 */
/* 604 */	NdrFcShort( 0x0 ),	/* 0 */
/* 606 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAnimationName */

/* 608 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 610 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 612 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter pFrameNum */

/* 614 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 616 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 618 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter pDC */

/* 620 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 622 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 624 */	NdrFcShort( 0x56 ),	/* Type Offset=86 */

	/* Parameter pPosition */

/* 626 */	NdrFcShort( 0x10a ),	/* Flags:  must free, in, simple ref, */
/* 628 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 630 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 632 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 634 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 636 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RenderCharacterFrame */

/* 638 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 640 */	NdrFcLong( 0x0 ),	/* 0 */
/* 644 */	NdrFcShort( 0x13 ),	/* 19 */
/* 646 */	NdrFcShort( 0x38 ),	/* X64 Stack size/offset = 56 */
/* 648 */	NdrFcShort( 0x32 ),	/* 50 */
/* 650 */	NdrFcShort( 0x8 ),	/* 8 */
/* 652 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 654 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 656 */	NdrFcShort( 0x0 ),	/* 0 */
/* 658 */	NdrFcShort( 0x1 ),	/* 1 */
/* 660 */	NdrFcShort( 0x0 ),	/* 0 */
/* 662 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pCharacterPath */

/* 664 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 666 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 668 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter pAnimationName */

/* 670 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 672 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 674 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter pFrameNum */

/* 676 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 678 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 680 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter pDC */

/* 682 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 684 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 686 */	NdrFcShort( 0x56 ),	/* Type Offset=86 */

	/* Parameter pPosition */

/* 688 */	NdrFcShort( 0x10a ),	/* Flags:  must free, in, simple ref, */
/* 690 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 692 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 694 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 696 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 698 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OpenFile */

/* 700 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 702 */	NdrFcLong( 0x0 ),	/* 0 */
/* 706 */	NdrFcShort( 0x14 ),	/* 20 */
/* 708 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 710 */	NdrFcShort( 0x0 ),	/* 0 */
/* 712 */	NdrFcShort( 0x8 ),	/* 8 */
/* 714 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 716 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 718 */	NdrFcShort( 0x0 ),	/* 0 */
/* 720 */	NdrFcShort( 0x1 ),	/* 1 */
/* 722 */	NdrFcShort( 0x0 ),	/* 0 */
/* 724 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pCharacterPath */

/* 726 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 728 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 730 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Parameter pParentWnd */

/* 732 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 734 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 736 */	NdrFcShort( 0x64 ),	/* Type Offset=100 */

	/* Return value */

/* 738 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 740 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 742 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetCharacterPath */

/* 744 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 746 */	NdrFcLong( 0x0 ),	/* 0 */
/* 750 */	NdrFcShort( 0x15 ),	/* 21 */
/* 752 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 754 */	NdrFcShort( 0x0 ),	/* 0 */
/* 756 */	NdrFcShort( 0x8 ),	/* 8 */
/* 758 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 760 */	0xa,		/* 10 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 762 */	NdrFcShort( 0x1 ),	/* 1 */
/* 764 */	NdrFcShort( 0x0 ),	/* 0 */
/* 766 */	NdrFcShort( 0x0 ),	/* 0 */
/* 768 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pCharacterPath */

/* 770 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 772 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 774 */	NdrFcShort( 0x76 ),	/* Type Offset=118 */

	/* Return value */

/* 776 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 778 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 780 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetCharacterName */

/* 782 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 784 */	NdrFcLong( 0x0 ),	/* 0 */
/* 788 */	NdrFcShort( 0x16 ),	/* 22 */
/* 790 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 792 */	NdrFcShort( 0x6 ),	/* 6 */
/* 794 */	NdrFcShort( 0x8 ),	/* 8 */
/* 796 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 798 */	0xa,		/* 10 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 800 */	NdrFcShort( 0x1 ),	/* 1 */
/* 802 */	NdrFcShort( 0x0 ),	/* 0 */
/* 804 */	NdrFcShort( 0x0 ),	/* 0 */
/* 806 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pCharacterName */

/* 808 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 810 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 812 */	NdrFcShort( 0x76 ),	/* Type Offset=118 */

	/* Parameter pLangID */

/* 814 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 816 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 818 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 820 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 822 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 824 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetCharacterDescription */

/* 826 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 828 */	NdrFcLong( 0x0 ),	/* 0 */
/* 832 */	NdrFcShort( 0x17 ),	/* 23 */
/* 834 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 836 */	NdrFcShort( 0x6 ),	/* 6 */
/* 838 */	NdrFcShort( 0x8 ),	/* 8 */
/* 840 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 842 */	0xa,		/* 10 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 844 */	NdrFcShort( 0x1 ),	/* 1 */
/* 846 */	NdrFcShort( 0x0 ),	/* 0 */
/* 848 */	NdrFcShort( 0x0 ),	/* 0 */
/* 850 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pCharacterDescription */

/* 852 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 854 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 856 */	NdrFcShort( 0x76 ),	/* Type Offset=118 */

	/* Parameter pLangID */

/* 858 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 860 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 862 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 864 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 866 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 868 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetCharacterGuid */

/* 870 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 872 */	NdrFcLong( 0x0 ),	/* 0 */
/* 876 */	NdrFcShort( 0x18 ),	/* 24 */
/* 878 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 880 */	NdrFcShort( 0x0 ),	/* 0 */
/* 882 */	NdrFcShort( 0x4c ),	/* 76 */
/* 884 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 886 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 888 */	NdrFcShort( 0x0 ),	/* 0 */
/* 890 */	NdrFcShort( 0x0 ),	/* 0 */
/* 892 */	NdrFcShort( 0x0 ),	/* 0 */
/* 894 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pGuid */

/* 896 */	NdrFcShort( 0x4112 ),	/* Flags:  must free, out, simple ref, srv alloc size=16 */
/* 898 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 900 */	NdrFcShort( 0x8a ),	/* Type Offset=138 */

	/* Return value */

/* 902 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 904 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 906 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetMaxSequenceDuration */

/* 908 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 910 */	NdrFcLong( 0x0 ),	/* 0 */
/* 914 */	NdrFcShort( 0x3 ),	/* 3 */
/* 916 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 918 */	NdrFcShort( 0x0 ),	/* 0 */
/* 920 */	NdrFcShort( 0x24 ),	/* 36 */
/* 922 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 924 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 926 */	NdrFcShort( 0x0 ),	/* 0 */
/* 928 */	NdrFcShort( 0x0 ),	/* 0 */
/* 930 */	NdrFcShort( 0x0 ),	/* 0 */
/* 932 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pMaxSequenceDuration */

/* 934 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 936 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 938 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 940 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 942 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 944 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetMaxSequenceFrames */

/* 946 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 948 */	NdrFcLong( 0x0 ),	/* 0 */
/* 952 */	NdrFcShort( 0x4 ),	/* 4 */
/* 954 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 956 */	NdrFcShort( 0x0 ),	/* 0 */
/* 958 */	NdrFcShort( 0x24 ),	/* 36 */
/* 960 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 962 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 964 */	NdrFcShort( 0x0 ),	/* 0 */
/* 966 */	NdrFcShort( 0x0 ),	/* 0 */
/* 968 */	NdrFcShort( 0x0 ),	/* 0 */
/* 970 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pMaxSequenceFrames */

/* 972 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 974 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 976 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 978 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 980 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 982 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetBkColor */


	/* Procedure GetAnimationIndex */

/* 984 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 986 */	NdrFcLong( 0x0 ),	/* 0 */
/* 990 */	NdrFcShort( 0x5 ),	/* 5 */
/* 992 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 994 */	NdrFcShort( 0x0 ),	/* 0 */
/* 996 */	NdrFcShort( 0x24 ),	/* 36 */
/* 998 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1000 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1002 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1004 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1006 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1008 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pBkColor */


	/* Parameter pAnimationNdx */

/* 1010 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1012 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1014 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 1016 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1018 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1020 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetAnimationIndex */

/* 1022 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1024 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1028 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1030 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1032 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1034 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1036 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1038 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1040 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1042 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1044 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1046 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAnimationNdx */

/* 1048 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1050 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1052 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1054 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1056 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1058 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetAnimationName */

/* 1060 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1062 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1066 */	NdrFcShort( 0x7 ),	/* 7 */
/* 1068 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1070 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1072 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1074 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1076 */	0xa,		/* 10 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 1078 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1080 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1082 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1084 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAnimationName */

/* 1086 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 1088 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1090 */	NdrFcShort( 0x76 ),	/* Type Offset=118 */

	/* Return value */

/* 1092 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1094 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1096 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetAnimationName */

/* 1098 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1100 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1104 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1106 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1108 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1110 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1112 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1114 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1116 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1118 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1120 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1122 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAnimationName */

/* 1124 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1126 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1128 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 1130 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1132 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1134 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetAnimationSource */

/* 1136 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1138 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1142 */	NdrFcShort( 0x9 ),	/* 9 */
/* 1144 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1146 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1148 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1150 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1152 */	0xa,		/* 10 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 1154 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1156 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1158 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1160 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAnimationSource */

/* 1162 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 1164 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1166 */	NdrFcShort( 0x76 ),	/* Type Offset=118 */

	/* Return value */

/* 1168 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1170 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1172 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetAnimationSource */

/* 1174 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1176 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1180 */	NdrFcShort( 0xa ),	/* 10 */
/* 1182 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1184 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1186 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1188 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1190 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1192 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1194 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1196 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1198 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAnimationSource */

/* 1200 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1202 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1204 */	NdrFcShort( 0x1c ),	/* Type Offset=28 */

	/* Return value */

/* 1206 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1208 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1210 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CalcAnimationFrameCount */

/* 1212 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1214 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1218 */	NdrFcShort( 0xb ),	/* 11 */
/* 1220 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 1222 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1224 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1226 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 1228 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1230 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1232 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1234 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1236 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAnimationFrameCount */

/* 1238 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1240 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1242 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pAnimationNdx */

/* 1244 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1246 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1248 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1250 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1252 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1254 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CalcAnimationDuration */

/* 1256 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1258 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1262 */	NdrFcShort( 0xc ),	/* 12 */
/* 1264 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 1266 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1268 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1270 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 1272 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1274 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1276 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1278 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1280 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAnimationDuration */

/* 1282 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1284 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1286 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pAnimationNdx */

/* 1288 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1290 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1292 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1294 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1296 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1298 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure FindAnimationSpeakingFrame */

/* 1300 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1302 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1306 */	NdrFcShort( 0xd ),	/* 13 */
/* 1308 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 1310 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1312 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1314 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 1316 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1318 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1320 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1322 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1324 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pSpeakingFrameNdx */

/* 1326 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1328 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1330 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pAnimationNdx */

/* 1332 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1334 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1336 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pStartFrameNdx */

/* 1338 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1340 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1342 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1344 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1346 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 1348 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure NewAnimationSequence */

/* 1350 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1352 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1356 */	NdrFcShort( 0xe ),	/* 14 */
/* 1358 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1360 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1362 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1364 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 1366 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1368 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1370 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1372 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1374 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 1376 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1378 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1380 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure EndAnimationSequence */

/* 1382 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1384 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1388 */	NdrFcShort( 0xf ),	/* 15 */
/* 1390 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1392 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1394 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1396 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 1398 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1400 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1402 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1404 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1406 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 1408 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1410 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1412 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ClearAnimationSequences */

/* 1414 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1416 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1420 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1422 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1424 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1426 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1428 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 1430 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1432 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1434 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1436 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1438 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 1440 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1442 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1444 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetSequenceFrameCount */

/* 1446 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1448 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1452 */	NdrFcShort( 0x11 ),	/* 17 */
/* 1454 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1456 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1458 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1460 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1462 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1464 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1466 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1468 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1470 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pSequenceFrameCount */

/* 1472 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1474 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1476 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1478 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1480 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1482 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetSequenceDuration */

/* 1484 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1486 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1490 */	NdrFcShort( 0x12 ),	/* 18 */
/* 1492 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1494 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1496 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1498 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1500 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1502 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1504 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1506 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1508 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pSequenceDuration */

/* 1510 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1512 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1514 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1516 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1518 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1520 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetSequenceLoop */

/* 1522 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1524 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1528 */	NdrFcShort( 0x13 ),	/* 19 */
/* 1530 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1532 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1534 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1536 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1538 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1540 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1542 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1544 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1546 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pLoopDuration */

/* 1548 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1550 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1552 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1554 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1556 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1558 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CalcSequenceTimeNdx */

/* 1560 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1562 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1566 */	NdrFcShort( 0x14 ),	/* 20 */
/* 1568 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 1570 */	NdrFcShort( 0xd ),	/* 13 */
/* 1572 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1574 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 1576 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1578 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1580 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1582 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1584 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pTimeNdx */

/* 1586 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1588 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1590 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pFrameNum */

/* 1592 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1594 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1596 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pClampFrameNum */

/* 1598 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1600 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1602 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */

/* 1604 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1606 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 1608 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CalcSequenceFrameNum */

/* 1610 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1612 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1616 */	NdrFcShort( 0x15 ),	/* 21 */
/* 1618 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 1620 */	NdrFcShort( 0xd ),	/* 13 */
/* 1622 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1624 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 1626 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1628 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1630 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1632 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1634 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pFrameNum */

/* 1636 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1638 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1640 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pTimeNdx */

/* 1642 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1644 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1646 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pClampTimeNdx */

/* 1648 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1650 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1652 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */

/* 1654 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1656 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 1658 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CalcSequenceAnimationFrameNdx */

/* 1660 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1662 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1666 */	NdrFcShort( 0x16 ),	/* 22 */
/* 1668 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 1670 */	NdrFcShort( 0xd ),	/* 13 */
/* 1672 */	NdrFcShort( 0x40 ),	/* 64 */
/* 1674 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x5,		/* 5 */
/* 1676 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1678 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1680 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1682 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1684 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAnimationNdx */

/* 1686 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1688 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1690 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pFrameNdx */

/* 1692 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1694 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1696 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pTimeNdx */

/* 1698 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1700 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1702 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pClampTimeNdx */

/* 1704 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1706 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 1708 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */

/* 1710 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1712 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 1714 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SequenceAll */

/* 1716 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1718 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1722 */	NdrFcShort( 0x17 ),	/* 23 */
/* 1724 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1726 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1728 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1730 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 1732 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1734 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1736 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1738 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1740 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 1742 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1744 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1746 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SequenceAnimation */

/* 1748 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1750 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1754 */	NdrFcShort( 0x18 ),	/* 24 */
/* 1756 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 1758 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1760 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1762 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 1764 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1766 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1768 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1770 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1772 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAnimationNdx */

/* 1774 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1776 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1778 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pMaxLoopTime */

/* 1780 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1782 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1784 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1786 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1788 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1790 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SequenceAnimationLoop */

/* 1792 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1794 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1798 */	NdrFcShort( 0x19 ),	/* 25 */
/* 1800 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 1802 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1804 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1806 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 1808 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1810 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1812 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1814 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1816 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAnimationNdx */

/* 1818 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1820 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1822 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pMaxLoopTime */

/* 1824 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1826 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1828 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1830 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1832 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1834 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SequenceAnimationExit */

/* 1836 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1838 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1842 */	NdrFcShort( 0x1a ),	/* 26 */
/* 1844 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 1846 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1848 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1850 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 1852 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1854 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1856 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1858 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1860 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAnimationNdx */

/* 1862 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1864 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1866 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pLastFrameNdx */

/* 1868 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1870 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1872 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1874 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1876 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1878 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CueSequenceAudio */

/* 1880 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1882 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1886 */	NdrFcShort( 0x1b ),	/* 27 */
/* 1888 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1890 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1892 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1894 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1896 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1898 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1900 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1902 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1904 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pStartFrameNum */

/* 1906 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1908 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1910 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1912 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1914 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1916 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ClearSequenceAudio */

/* 1918 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1920 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1924 */	NdrFcShort( 0x1c ),	/* 28 */
/* 1926 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1928 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1930 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1932 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 1934 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1936 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1938 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1940 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1942 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 1944 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1946 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1948 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetAgentFile */

/* 1950 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1952 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1956 */	NdrFcShort( 0x3 ),	/* 3 */
/* 1958 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1960 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1962 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1964 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1966 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1968 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1970 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1972 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1974 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAgentFile */

/* 1976 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1978 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 1980 */	0xb9,		/* FC_UINT3264 */
			0x0,		/* 0 */

	/* Return value */

/* 1982 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1984 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 1986 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetAgentFile */

/* 1988 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1990 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1994 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1996 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 1998 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2000 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2002 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2004 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2006 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2008 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2010 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2012 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAgentFile */

/* 2014 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2016 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2018 */	0xb9,		/* FC_UINT3264 */
			0x0,		/* 0 */

	/* Return value */

/* 2020 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2022 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 2024 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetAgentStreamInfo */

/* 2026 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2028 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2032 */	NdrFcShort( 0x5 ),	/* 5 */
/* 2034 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 2036 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2038 */	NdrFcShort( 0x24 ),	/* 36 */
/* 2040 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2042 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2044 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2046 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2048 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2050 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAgentStreamInfo */

/* 2052 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2054 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2056 */	0xb9,		/* FC_UINT3264 */
			0x0,		/* 0 */

	/* Return value */

/* 2058 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2060 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 2062 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetAgentStreamInfo */

/* 2064 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2066 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2070 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2072 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 2074 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2076 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2078 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2080 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2082 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2084 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2086 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2088 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pAgentStreamInfo */

/* 2090 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2092 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2094 */	0xb9,		/* FC_UINT3264 */
			0x0,		/* 0 */

	/* Return value */

/* 2096 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2098 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 2100 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetSmoothing */


	/* Procedure GetBkColor */

/* 2102 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2104 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2108 */	NdrFcShort( 0x7 ),	/* 7 */
/* 2110 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 2112 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2114 */	NdrFcShort( 0x24 ),	/* 36 */
/* 2116 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2118 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2120 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2122 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2124 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2126 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pSmoothing */


	/* Parameter pBkColor */

/* 2128 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2130 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2132 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 2134 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2136 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 2138 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetBkColor */

/* 2140 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2142 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2146 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2148 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 2150 */	NdrFcShort( 0x1c ),	/* 28 */
/* 2152 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2154 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2156 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2158 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2160 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2162 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2164 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pBkColor */

/* 2166 */	NdrFcShort( 0x148 ),	/* Flags:  in, base type, simple ref, */
/* 2168 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2170 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2172 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2174 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 2176 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SegmentDurationChanged */

/* 2178 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2180 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2184 */	NdrFcShort( 0x9 ),	/* 9 */
/* 2186 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 2188 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2190 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2192 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 2194 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2196 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2198 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2200 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2202 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 2204 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2206 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2208 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetRenderWnd */

/* 2210 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2212 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2216 */	NdrFcShort( 0x3 ),	/* 3 */
/* 2218 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 2220 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2222 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2224 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2226 */	0xa,		/* 10 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 2228 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2230 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2232 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2234 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pRenderWnd */

/* 2236 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 2238 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2240 */	NdrFcShort( 0xaa ),	/* Type Offset=170 */

	/* Return value */

/* 2242 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2244 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 2246 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetRenderWnd */

/* 2248 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2250 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2254 */	NdrFcShort( 0x4 ),	/* 4 */
/* 2256 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 2258 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2260 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2262 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 2264 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2266 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2268 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2270 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2272 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pRenderWnd */

/* 2274 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2276 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2278 */	NdrFcShort( 0x64 ),	/* Type Offset=100 */

	/* Return value */

/* 2280 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2282 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 2284 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetBkColor */

/* 2286 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2288 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2292 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2294 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 2296 */	NdrFcShort( 0x1c ),	/* 28 */
/* 2298 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2300 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2302 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2304 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2306 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2308 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2310 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pBkColor */

/* 2312 */	NdrFcShort( 0x148 ),	/* Flags:  in, base type, simple ref, */
/* 2314 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2316 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2318 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2320 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 2322 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetSmoothing */

/* 2324 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2326 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2330 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2332 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 2334 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2336 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2338 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2340 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2342 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2344 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2346 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2348 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pSmoothing */

/* 2350 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2352 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2354 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2356 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2358 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 2360 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetImageSize */

/* 2362 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2364 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2368 */	NdrFcShort( 0x9 ),	/* 9 */
/* 2370 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 2372 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2374 */	NdrFcShort( 0x40 ),	/* 64 */
/* 2376 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 2378 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2380 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2382 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2384 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2386 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pImageWidth */

/* 2388 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2390 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2392 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pImageHeight */

/* 2394 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2396 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 2398 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2400 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2402 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 2404 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DrawSampleImage */

/* 2406 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2408 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2412 */	NdrFcShort( 0xa ),	/* 10 */
/* 2414 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 2416 */	NdrFcShort( 0x34 ),	/* 52 */
/* 2418 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2420 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 2422 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2424 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2426 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2428 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2430 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pDC */

/* 2432 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2434 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 2436 */	NdrFcShort( 0x56 ),	/* Type Offset=86 */

	/* Parameter pTargetRect */

/* 2438 */	NdrFcShort( 0x10a ),	/* Flags:  must free, in, simple ref, */
/* 2440 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 2442 */	NdrFcShort( 0xb8 ),	/* Type Offset=184 */

	/* Return value */

/* 2444 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2446 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 2448 */	0x8,		/* FC_LONG */
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
/* 32 */	NdrFcShort( 0x8 ),	/* 8 */
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
/* 90 */	NdrFcShort( 0x8 ),	/* 8 */
/* 92 */	NdrFcShort( 0x0 ),	/* 0 */
/* 94 */	NdrFcShort( 0xffe0 ),	/* Offset= -32 (62) */
/* 96 */	
			0x11, 0x0,	/* FC_RP */
/* 98 */	NdrFcShort( 0xffcc ),	/* Offset= -52 (46) */
/* 100 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 102 */	NdrFcShort( 0x2 ),	/* 2 */
/* 104 */	NdrFcShort( 0x8 ),	/* 8 */
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
/* 122 */	NdrFcShort( 0x8 ),	/* 8 */
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
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 156 */	0xb9,		/* FC_UINT3264 */
			0x5c,		/* FC_PAD */
/* 158 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/* 160 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 162 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 164 */	NdrFcShort( 0x6 ),	/* Offset= 6 (170) */
/* 166 */	
			0x13, 0x0,	/* FC_OP */
/* 168 */	NdrFcShort( 0xff9a ),	/* Offset= -102 (66) */
/* 170 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 172 */	NdrFcShort( 0x2 ),	/* 2 */
/* 174 */	NdrFcShort( 0x8 ),	/* 8 */
/* 176 */	NdrFcShort( 0x0 ),	/* 0 */
/* 178 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (166) */
/* 180 */	
			0x11, 0x0,	/* FC_RP */
/* 182 */	NdrFcShort( 0x2 ),	/* Offset= 2 (184) */
/* 184 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 186 */	NdrFcShort( 0x10 ),	/* 16 */
/* 188 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 190 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 192 */	0x5c,		/* FC_PAD */
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
    38,
    76,
    114,
    146,
    184,
    222,
    260,
    298,
    336,
    374,
    418,
    456,
    500,
    538,
    582,
    638,
    700,
    744,
    782,
    826,
    870
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
    908,
    946,
    984,
    1022,
    1060,
    1098,
    1136,
    1174,
    1212,
    1256,
    1300,
    1350,
    1382,
    1414,
    1446,
    1484,
    1522,
    1560,
    1610,
    1660,
    1716,
    1748,
    1792,
    1836,
    1880,
    1918
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
    1950,
    1988,
    2026,
    2064,
    2102,
    2140,
    2178
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
    2210,
    2248,
    984,
    2286,
    2102,
    2324,
    2362,
    2406
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
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* defined(_M_AMD64)*/

