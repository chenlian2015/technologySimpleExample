

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Fri Jan 23 14:02:58 2015
 */
/* Compiler settings for ToolBand.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
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

MIDL_DEFINE_GUID(IID, IID_IPugiObj,0x7B9A715E,0x9D87,0x4c21,0xBF,0x9E,0xF9,0x14,0xF2,0xFA,0x95,0x3F);


MIDL_DEFINE_GUID(IID, IID_IContextItem,0xEAF2CCEE,0x21A1,0x4203,0x9F,0x36,0x49,0x29,0xFD,0x10,0x4D,0x43);


MIDL_DEFINE_GUID(IID, LIBID_PugiLib,0x6D3F5DE4,0xE980,0x4407,0xA1,0x0F,0x9A,0xC7,0x71,0xAB,0xAA,0xE6);


MIDL_DEFINE_GUID(CLSID, CLSID_PugiObj,0x5F1ABCDB,0xA875,0x46c1,0x83,0x45,0xB7,0x2A,0x45,0x67,0xE4,0x86);


MIDL_DEFINE_GUID(CLSID, CLSID_ContextItem,0xFC2493D6,0xA673,0x49FE,0xA2,0xEE,0xEF,0xE0,0x3E,0x95,0xC2,0x7C);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



