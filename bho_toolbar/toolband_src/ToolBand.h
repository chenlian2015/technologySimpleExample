/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Oct 01 17:12:05 2002
 */
/* Compiler settings for C:\MyProjects\toolband_demo\ToolBand.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
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

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

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
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IToolBandObj __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IToolBandObj __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IToolBandObj __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IToolBandObj __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IToolBandObj __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IToolBandObj __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IToolBandObj __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } IToolBandObjVtbl;

    interface IToolBandObj
    {
        CONST_VTBL struct IToolBandObjVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IToolBandObj_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IToolBandObj_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IToolBandObj_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IToolBandObj_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IToolBandObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IToolBandObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IToolBandObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


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
