/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Oct 01 17:12:05 2002
 */
/* Compiler settings for C:\MyProjects\toolband_demo\ToolBand.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


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

const IID IID_IToolBandObj = {0x95B92D92,0x8B7D,0x4A19,{0xA3,0xF1,0x43,0x11,0x3B,0x4D,0xBC,0xAF}};


const IID LIBID_TOOLBANDLib = {0x5297E905,0x1DFB,0x4A9C,{0x98,0x71,0xA4,0xF9,0x5F,0xD5,0x89,0x45}};


const CLSID CLSID_ToolBandObj = {0x0E1230F8,0xEA50,0x42A9,{0x98,0x3C,0xD2,0x2A,0xBC,0x2E,0xED,0x3B}};


#ifdef __cplusplus
}
#endif

