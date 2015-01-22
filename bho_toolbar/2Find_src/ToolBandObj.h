//PugiObj.h : Declaration of the CPugiObj

//***************************************************************************//
//                                                                           //
//  This file was created using the DeskBand ATL Object Wizard 2.0           //
//  By Erik Thompson © 2001                                                  //
//  Email questions and comments to erikt@radbytes.com						 //
//                                                                           //
//***************************************************************************//

#ifndef __PugiOBJ_H_
#define __PugiOBJ_H_

//#include "resource.h"       // main symbols

//
// These are needed for IDeskBand
//
// Contains the CATIDs.
#include <ObjSafe.h>

#include <shlobj.h>
#include "BandToolBarCtrl.h"
                                     
/////////////////////////////////////////////////////////////////////////////
// CPugiObj
class ATL_NO_VTABLE CPugiObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPugiObj, &CLSID_PugiObj>,
	public IDeskBand,
	public IObjectWithSiteImpl<CPugiObj>,
	public IPersistStream,
	public IInputObject,
	public IDispatchImpl<IPugiObj, &IID_IPugiObj, &LIBID_PugiLib>,
  public IDispEventImpl<0, CPugiObj, &DIID_DWebBrowserEvents2, &LIBID_SHDocVw, 1, 0>
{
public:
 
  using IDispatchImpl<IPugiObj, &IID_IPugiObj, &LIBID_PugiLib>::_tih;

  CPugiObj();
  ~CPugiObj();

#define IDR_TOOLBANDOBJ                 101

  DECLARE_REGISTRY_RESOURCEID(IDR_TOOLBANDOBJ)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_CATEGORY_MAP(CPugiObj)
//	IMPLEMENTED_CATEGORY(CATID_InfoBand)
	IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
	IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
//	IMPLEMENTED_CATEGORY(CATID_CommBand)
//	IMPLEMENTED_CATEGORY(CATID_DeskBand)
END_CATEGORY_MAP()

BEGIN_COM_MAP(CPugiObj)
	COM_INTERFACE_ENTRY(IPugiObj)
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

// IPugiObj
public:
	void FocusChange(bool bFocus);
  STDMETHOD (get_ver)(VARIANT*  pVal);
  STDMETHOD (put_NID)(VARIANT  pVal);
  STDMETHOD (put_Pass)(VARIANT  pVal);
  STDMETHOD (put_CustID)(VARIANT  pVal);

public:
  CBandToolBarCtrl m_wndToolBar;
  IWebBrowser2Ptr m_pWebBrowser;
  bool   m_onOpen;

  BEGIN_SINK_MAP(CPugiObj)
  	SINK_ENTRY_EX(0, DIID_DWebBrowserEvents2, DISPID_BEFORENAVIGATE2, BeforeNavigate2)
  	SINK_ENTRY_EX(0, DIID_DWebBrowserEvents2, DISPID_TITLECHANGE, TitleChange)
  	SINK_ENTRY_EX(0, DIID_DWebBrowserEvents2, DISPID_ONQUIT, OnQuit)
  	SINK_ENTRY_EX(0, DIID_DWebBrowserEvents2, DISPID_DOWNLOADCOMPLETE, OnDownloadComplete)
    SINK_ENTRY_EX(0, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete)
  END_SINK_MAP()

protected:
	BOOL RegisterAndCreateWindow();

  STDMETHOD(BeforeNavigate2) (LPDISPATCH pDisp , VARIANT* URL,
		VARIANT* Flags, VARIANT* TargetFrameName,
		VARIANT* PostData, VARIANT* Headers, BOOL* Cancel);
  STDMETHOD(OnDownloadComplete) ();
  STDMETHOD(OnDocumentComplete)(IDispatch *pDisp, VARIANT *_url);
  STDMETHOD(TitleChange) (BSTR Text);
  STDMETHOD(OnQuit)();


public:
  void ApplyBackground();
  void SetWidth(int width);
  void SetHeight(int );

protected:
	DWORD m_dwBandID;
	DWORD m_dwViewMode;
	BOOL m_bShow;
	BOOL m_bEnterHelpMode;
	HWND m_hWndParent;
	//HWND m_hWnd;
	IInputObjectSite* m_pSite;

  CBandToolBarReflectorCtrl m_wndInvisibleChildWnd; // Used for Reflecting Messages

  BOOL ManageSourceCodeWindow(BOOL bForceWindow);
  static LRESULT CALLBACK IEKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
  static BOOL CALLBACK WndEnumProc(HWND hwnd, LPARAM lParam);
  HRESULT RetrieveBrowserWindow();
	RECT m_rcIE, m_rcHTML;
	HWND m_hwndBrowser;

};

#ifdef ADD_CONTEXT_ITEM
/////////////////////////////////////////////////////////////////////////////
// CContextItem
class ATL_NO_VTABLE CContextItem : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CContextItem, &CLSID_ContextItem>,
	public IDispatchImpl<IContextItem, &IID_IContextItem, &LIBID_PugiLib>
{
public:
	CContextItem()
	{
	}
  
#define IDR_CONTEXTITEM 102

  DECLARE_REGISTRY_RESOURCEID(IDR_CONTEXTITEM)
    
    DECLARE_PROTECT_FINAL_CONSTRUCT()
    
    BEGIN_COM_MAP(CContextItem)
    COM_INTERFACE_ENTRY(IContextItem)
    COM_INTERFACE_ENTRY(IDispatch)
    END_COM_MAP()
    
    // IContextItem
public:
  STDMETHOD(Run)( LPDISPATCH pDispatch)
  {
    // получаем объект window
    CComQIPtr<IHTMLWindow2> pWindow = pDispatch;
    if ( pWindow )
    {
      // Получаем текст выделения
      CComPtr<IHTMLDocument2> pDoc;
      CComPtr<IHTMLSelectionObject> pSel;
      CComPtr<IDispatch> pDisp;
      CComQIPtr<IHTMLTxtRange> pRange;
      CComBSTR bsText;
      
      pWindow->get_document( &pDoc );
      if (pDoc == NULL)	return E_FAIL;
      
      pDoc->get_selection( &pSel );
      if (pSel == NULL)   return E_FAIL;
      
      pSel->createRange( &pDisp );
      if (pDisp == NULL)  return E_FAIL;
      
      pRange = pDisp;
      if (pRange == NULL) return E_FAIL;
      pRange->get_text( &bsText ); // в bsText - текст выделения !!!
      
      
      // Открываем результаты поиска в новом окне		
      CComBSTR sUrl = L"http://www.rsdn.ru/cgi-bin/search.exe?query=";
      sUrl+=bsText;
      CComPtr<IHTMLWindow2> pnewWindow;		
      
      pWindow->open( sUrl, L"_blank", L"", FALSE, &pnewWindow);
      return S_OK;
    }
    
    return E_FAIL;
  }
  };
#endif

#endif //__PugiOBJ_H_