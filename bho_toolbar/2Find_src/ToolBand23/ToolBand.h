/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu May 29 13:50:29 2003
 */
/* Compiler settings for C:\ToolBand\ToolBand23\ToolBand.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
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

#ifdef __cplusplus
extern "C"{
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

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

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
            /* [retval][out] */ VARIANT __RPC_FAR *newVal) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_NID( 
            /* [in] */ VARIANT newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPugiObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPugiObj __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPugiObj __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPugiObj __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPugiObj __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPugiObj __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPugiObj __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPugiObj __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ver )( 
            IPugiObj __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *newVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_NID )( 
            IPugiObj __RPC_FAR * This,
            /* [in] */ VARIANT newVal);
        
        END_INTERFACE
    } IPugiObjVtbl;

    interface IPugiObj
    {
        CONST_VTBL struct IPugiObjVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPugiObj_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPugiObj_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPugiObj_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPugiObj_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPugiObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPugiObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPugiObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPugiObj_get_ver(This,newVal)	\
    (This)->lpVtbl -> get_ver(This,newVal)

#define IPugiObj_put_NID(This,newVal)	\
    (This)->lpVtbl -> put_NID(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IPugiObj_get_ver_Proxy( 
    IPugiObj __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *newVal);


void __RPC_STUB IPugiObj_get_ver_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE IPugiObj_put_NID_Proxy( 
    IPugiObj __RPC_FAR * This,
    /* [in] */ VARIANT newVal);


void __RPC_STUB IPugiObj_put_NID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



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
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IContextItem __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IContextItem __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IContextItem __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IContextItem __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IContextItem __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IContextItem __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IContextItem __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Run )( 
            IContextItem __RPC_FAR * This,
            /* [in] */ LPDISPATCH pDispatch);
        
        END_INTERFACE
    } IContextItemVtbl;

    interface IContextItem
    {
        CONST_VTBL struct IContextItemVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IContextItem_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IContextItem_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IContextItem_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IContextItem_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IContextItem_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IContextItem_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IContextItem_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IContextItem_Run(This,pDispatch)	\
    (This)->lpVtbl -> Run(This,pDispatch)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IContextItem_Run_Proxy( 
    IContextItem __RPC_FAR * This,
    /* [in] */ LPDISPATCH pDispatch);


void __RPC_STUB IContextItem_Run_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



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

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
