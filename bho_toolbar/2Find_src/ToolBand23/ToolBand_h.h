

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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

#ifndef __ToolBand_h_h__
#define __ToolBand_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IPugiObj_FWD_DEFINED__
#define __IPugiObj_FWD_DEFINED__
typedef interface IPugiObj IPugiObj;

#endif 	/* __IPugiObj_FWD_DEFINED__ */


#ifndef __IContextItem_FWD_DEFINED__
#define __IContextItem_FWD_DEFINED__
typedef interface IContextItem IContextItem;

#endif 	/* __IContextItem_FWD_DEFINED__ */


#ifndef __PugiObj_FWD_DEFINED__
#define __PugiObj_FWD_DEFINED__

#ifdef __cplusplus
typedef class PugiObj PugiObj;
#else
typedef struct PugiObj PugiObj;
#endif /* __cplusplus */

#endif 	/* __PugiObj_FWD_DEFINED__ */


#ifndef __ContextItem_FWD_DEFINED__
#define __ContextItem_FWD_DEFINED__

#ifdef __cplusplus
typedef class ContextItem ContextItem;
#else
typedef struct ContextItem ContextItem;
#endif /* __cplusplus */

#endif 	/* __ContextItem_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IPugiObj_INTERFACE_DEFINED__
#define __IPugiObj_INTERFACE_DEFINED__

/* interface IPugiObj */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPugiObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7B9A715E-9D87-4c21-BF9E-F914F2FA953F")
    IPugiObj : public IDispatch
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_ver( 
            /* [retval][out] */ VARIANT *newVal) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_NID( 
            /* [in] */ VARIANT newVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IPugiObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPugiObj * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPugiObj * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPugiObj * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPugiObj * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPugiObj * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPugiObj * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPugiObj * This,
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
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ver )( 
            IPugiObj * This,
            /* [retval][out] */ VARIANT *newVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_NID )( 
            IPugiObj * This,
            /* [in] */ VARIANT newVal);
        
        END_INTERFACE
    } IPugiObjVtbl;

    interface IPugiObj
    {
        CONST_VTBL struct IPugiObjVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPugiObj_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IPugiObj_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IPugiObj_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IPugiObj_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IPugiObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IPugiObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IPugiObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IPugiObj_get_ver(This,newVal)	\
    ( (This)->lpVtbl -> get_ver(This,newVal) ) 

#define IPugiObj_put_NID(This,newVal)	\
    ( (This)->lpVtbl -> put_NID(This,newVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IPugiObj_INTERFACE_DEFINED__ */


#ifndef __IContextItem_INTERFACE_DEFINED__
#define __IContextItem_INTERFACE_DEFINED__

/* interface IContextItem */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IContextItem;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("EAF2CCEE-21A1-4203-9F36-4929FD104D43")
    IContextItem : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Run( 
            /* [in] */ LPDISPATCH pDispatch) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IContextItemVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IContextItem * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IContextItem * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IContextItem * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IContextItem * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IContextItem * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IContextItem * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IContextItem * This,
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
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Run )( 
            IContextItem * This,
            /* [in] */ LPDISPATCH pDispatch);
        
        END_INTERFACE
    } IContextItemVtbl;

    interface IContextItem
    {
        CONST_VTBL struct IContextItemVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IContextItem_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IContextItem_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IContextItem_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IContextItem_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IContextItem_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IContextItem_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IContextItem_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IContextItem_Run(This,pDispatch)	\
    ( (This)->lpVtbl -> Run(This,pDispatch) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IContextItem_INTERFACE_DEFINED__ */



#ifndef __PugiLib_LIBRARY_DEFINED__
#define __PugiLib_LIBRARY_DEFINED__

/* library PugiLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_PugiLib;

EXTERN_C const CLSID CLSID_PugiObj;

#ifdef __cplusplus

class DECLSPEC_UUID("5F1ABCDB-A875-46c1-8345-B72A4567E486")
PugiObj;
#endif

EXTERN_C const CLSID CLSID_ContextItem;

#ifdef __cplusplus

class DECLSPEC_UUID("FC2493D6-A673-49FE-A2EE-EFE03E95C27C")
ContextItem;
#endif
#endif /* __PugiLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


