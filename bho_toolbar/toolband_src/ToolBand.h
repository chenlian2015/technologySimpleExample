

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Tue Jan 27 11:32:35 2015
 */
/* Compiler settings for ToolBand.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0595 
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

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __ToolBand_h__
#define __ToolBand_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IToolBandObj_FWD_DEFINED__
#define __IToolBandObj_FWD_DEFINED__
typedef interface IToolBandObj IToolBandObj;

#endif 	/* __IToolBandObj_FWD_DEFINED__ */


#ifndef __ToolBandObj_FWD_DEFINED__
#define __ToolBandObj_FWD_DEFINED__

#ifdef __cplusplus
typedef class ToolBandObj ToolBandObj;
#else
typedef struct ToolBandObj ToolBandObj;
#endif /* __cplusplus */

#endif 	/* __ToolBandObj_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IToolBandObj_INTERFACE_DEFINED__
#define __IToolBandObj_INTERFACE_DEFINED__

/* interface IToolBandObj */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IToolBandObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("95B92D92-8B7D-4A19-A3F1-43113B4DBCAF")
    IToolBandObj : public IDispatch
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct IToolBandObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IToolBandObj * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IToolBandObj * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IToolBandObj * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IToolBandObj * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IToolBandObj * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IToolBandObj * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IToolBandObj * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } IToolBandObjVtbl;

    interface IToolBandObj
    {
        CONST_VTBL struct IToolBandObjVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IToolBandObj_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IToolBandObj_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IToolBandObj_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IToolBandObj_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IToolBandObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IToolBandObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IToolBandObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IToolBandObj_INTERFACE_DEFINED__ */



#ifndef __TOOLBANDLib_LIBRARY_DEFINED__
#define __TOOLBANDLib_LIBRARY_DEFINED__

/* library TOOLBANDLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_TOOLBANDLib;

EXTERN_C const CLSID CLSID_ToolBandObj;

#ifdef __cplusplus

class DECLSPEC_UUID("0E1230F8-EA50-42A9-983C-D22ABC2EED3B")
ToolBandObj;
#endif
#endif /* __TOOLBANDLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


