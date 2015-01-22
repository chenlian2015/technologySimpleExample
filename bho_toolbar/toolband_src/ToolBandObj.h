//ToolBandObj.h : Declaration of the CToolBandObj

//***************************************************************************//
//                                                                           //
//  This file was created using the DeskBand ATL Object Wizard 2.0           //
//  By Erik Thompson © 2001                                                  //
//  Email questions and comments to erikt@radbytes.com						 //
//                                                                           //
//***************************************************************************//

#ifndef __TOOLBANDOBJ_H_
#define __TOOLBANDOBJ_H_

#include "resource.h"       // main symbols

//
// These are needed for IDeskBand
//

#include <shlguid.h>
#include <shlobj.h>
#include "BandToolBarCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CToolBandObj
class ATL_NO_VTABLE CToolBandObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CToolBandObj, &CLSID_ToolBandObj>,
	public IDeskBand,
	public IObjectWithSite,
	public IPersistStream,
	public IInputObject,
	public IDispatchImpl<IToolBandObj, &IID_IToolBandObj, &LIBID_TOOLBANDLib>
{
public:
	CToolBandObj();

DECLARE_REGISTRY_RESOURCEID(IDR_TOOLBANDOBJ)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_CATEGORY_MAP(CToolBandObj)
//	IMPLEMENTED_CATEGORY(CATID_InfoBand)
//	IMPLEMENTED_CATEGORY(CATID_CommBand)
//	IMPLEMENTED_CATEGORY(CATID_DeskBand)
END_CATEGORY_MAP()

BEGIN_COM_MAP(CToolBandObj)
	COM_INTERFACE_ENTRY(IToolBandObj)
	COM_INTERFACE_ENTRY(IOleWindow)
	COM_INTERFACE_ENTRY_IID(IID_IDockingWindow, IDockingWindow)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY_IID(IID_IDeskBand, IDeskBand)
	COM_INTERFACE_ENTRY(IPersist)
	COM_INTERFACE_ENTRY(IPersistStream)
	COM_INTERFACE_ENTRY(IInputObject)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IDeskBand
public:
	STDMETHOD(GetBandInfo)(DWORD dwBandID, DWORD dwViewMode, DESKBANDINFO* pdbi);

// IObjectWithSite
public:
	STDMETHOD(SetSite)(IUnknown* pUnkSite);
	STDMETHOD(GetSite)(REFIID riid, void **ppvSite);

// IOleWindow
public:
	STDMETHOD(GetWindow)(HWND* phwnd);
	STDMETHOD(ContextSensitiveHelp)(BOOL fEnterMode);

// IDockingWindow
public:
	STDMETHOD(CloseDW)(unsigned long dwReserved);
	STDMETHOD(ResizeBorderDW)(const RECT* prcBorder, IUnknown* punkToolbarSite, BOOL fReserved);
	STDMETHOD(ShowDW)(BOOL fShow);

// IPersist
public:
	STDMETHOD(GetClassID)(CLSID *pClassID);

// IPersistStream
public:
	STDMETHOD(IsDirty)(void);
	STDMETHOD(Load)(IStream *pStm);
	STDMETHOD(Save)(IStream *pStm, BOOL fClearDirty);
	STDMETHOD(GetSizeMax)(ULARGE_INTEGER *pcbSize);

// IInputObject methods
public:
	STDMETHOD (UIActivateIO) (BOOL, LPMSG);
	STDMETHOD (HasFocusIO) (void);
	STDMETHOD (TranslateAcceleratorIO) (LPMSG);


// IToolBandObj
public:
	void FocusChange(bool bFocus);
    CBandToolBarCtrl* GetToolBarCtrl() {return &m_wndToolBar;}

protected:
	BOOL RegisterAndCreateWindow();

protected:
	DWORD m_dwBandID;
	DWORD m_dwViewMode;
	BOOL m_bShow;
	BOOL m_bEnterHelpMode;
	HWND m_hWndParentRebar;
	HWND m_hWnd;
    LPCSTR m_pszVerb;
    LPCTSTR m_pwszVerb;

	IInputObjectSite* m_pSite;
    IWebBrowser2* m_pWebBrowser;

    CBandToolBarReflectorCtrl m_wndInvisibleChildWnd; // Used for Reflecting Messages
    CBandToolBarCtrl m_wndToolBar;
};

#endif //__TOOLBANDOBJ_H_