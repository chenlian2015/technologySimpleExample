//[!ShortName].h : Declaration of the [!ClassName]
[!crlf]
//***************************************************************************//
//                                                                           //
//  This file was created using the DeskBand ATL Object Wizard 2.0           //
//  By Erik Thompson © 2001                                                  //
//  Email questions and comments to erikt@radbytes.com						 //
//                                                                           //
//***************************************************************************//
[!crlf]
#ifndef __[!UpperShortName]_H_
#define __[!UpperShortName]_H_
[!crlf]
#include "resource.h"       // main symbols
[!crlf]
//
// These are needed for IDeskBand
//
[!crlf]
#include <shlguid.h>
#include <shlobj.h>
[!crlf]
/////////////////////////////////////////////////////////////////////////////
// [!ClassName]
class ATL_NO_VTABLE [!ClassName] : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<[!ClassName], &CLSID_[!CoClassName]>,
	public IDeskBand,
	public IObjectWithSite,
	public IPersistStream,
	public IDispatchImpl<[!InterfaceName], &IID_[!InterfaceName], &LIBID_[!LibName]>
{
public:
	[!ClassName]();
[!crlf]
DECLARE_REGISTRY_RESOURCEID(IDR_[!UpperShortName])
[!crlf]
DECLARE_PROTECT_FINAL_CONSTRUCT()
[!crlf]
BEGIN_CATEGORY_MAP([!ClassName])
[!if=(RBINFOBAND, "0")]//[!endif]	IMPLEMENTED_CATEGORY(CATID_InfoBand)
[!if=(RBCOMMBAND, "0")]//[!endif]	IMPLEMENTED_CATEGORY(CATID_CommBand)
[!if=(RBDESKBAND, "0")]//[!endif]	IMPLEMENTED_CATEGORY(CATID_DeskBand)
END_CATEGORY_MAP()
[!crlf]
BEGIN_COM_MAP([!ClassName])
	COM_INTERFACE_ENTRY([!InterfaceName])
	COM_INTERFACE_ENTRY(IOleWindow)
	COM_INTERFACE_ENTRY_IID(IID_IDockingWindow, IDockingWindow)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY_IID(IID_IDeskBand, IDeskBand)
	COM_INTERFACE_ENTRY(IPersist)
	COM_INTERFACE_ENTRY(IPersistStream)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()
[!crlf]
// IDeskBand
public:
	STDMETHOD(GetBandInfo)(DWORD dwBandID, DWORD dwViewMode, DESKBANDINFO* pdbi);
[!crlf]
// IObjectWithSite
public:
	STDMETHOD(SetSite)(IUnknown* pUnkSite);
	STDMETHOD(GetSite)(REFIID riid, void **ppvSite);
[!crlf]
// IOleWindow
public:
	STDMETHOD(GetWindow)(HWND* phwnd);
	STDMETHOD(ContextSensitiveHelp)(BOOL fEnterMode);
[!crlf]
// IDockingWindow
public:
	STDMETHOD(CloseDW)(unsigned long dwReserved);
	STDMETHOD(ResizeBorderDW)(const RECT* prcBorder, IUnknown* punkToolbarSite, BOOL fReserved);
	STDMETHOD(ShowDW)(BOOL fShow);
[!crlf]
// IPersist
public:
	STDMETHOD(GetClassID)(CLSID *pClassID);
[!crlf]
// IPersistStream
public:
	STDMETHOD(IsDirty)(void);
	STDMETHOD(Load)(IStream *pStm);
	STDMETHOD(Save)(IStream *pStm, BOOL fClearDirty);
	STDMETHOD(GetSizeMax)(ULARGE_INTEGER *pcbSize);
[!crlf]
// [!InterfaceName]
public:
[!crlf]
protected:
	BOOL RegisterAndCreateWindow();
protected:
	DWORD m_dwBandID;
	DWORD m_dwViewMode;
	BOOL m_bShow;
	BOOL m_bEnterHelpMode;
	HWND m_hWndParent;
	HWND m_hWnd;
	IInputObjectSite* m_pSite;
};
[!crlf]
#endif //__[!UpperShortName]_H_