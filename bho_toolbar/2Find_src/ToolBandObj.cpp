//PugiObj.cpp : Implementation of CPugiObj

//***************************************************************************//
//                                                                           //
//  This file was created using the DeskBand ATL Object Wizard 2.0           //
//  By Erik Thompson © 2001                                                  //
//  Email questions and comments to erikt@radbytes.com						 //
//                                                                           //
//***************************************************************************//

// ============================================================================
// Used Sample by Rashid Thadha Aug 2001
// Modified by Mike Melnikov
// ============================================================================


#include "stdafx.h"

#include "ToolBand.h"
#include "ToolBandObj.h"

#include <wchar.h>

const WCHAR TITLE_CPugiObj[] = L"";


#ifndef DBIMF_USECHEVRON
#define DBIMF_USECHEVRON 0x0080
#endif 

#ifndef DBIMF_BREAK
#define DBIMF_BREAK 0x0100
#endif

void __cdecl ReEnable(void* lpParam);

/////////////////////////////////////////////////////////////////////////////
// CPugiObj

CPugiObj::CPugiObj(): 
	m_dwBandID(0), 
	m_dwViewMode(0), 
	m_bShow(FALSE), 
	m_bEnterHelpMode(FALSE), 
	m_hWndParent(NULL), 
	m_pSite(NULL),
  m_onOpen(false)
{
}

CPugiObj::~CPugiObj()
{
}

#include <AtlFrame.h>

BOOL CPugiObj::RegisterAndCreateWindow()
{
	  RECT rectClientParent;
	  ::GetClientRect(m_hWndParent, &rectClientParent);

    m_wndToolBar.m_pBand = this;
    m_wndInvisibleChildWnd.m_pBand = this;

    // We need to create an Invisible Child Window using the Parent Window, this will also be used to reflect Command
    // messages from the rebar
    HWND hWndChild = m_wndInvisibleChildWnd.Create(m_hWndParent,rectClientParent,NULL, WS_CHILD);
    
    DWORD dStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | 
     CCS_TOP | CCS_NODIVIDER | CCS_NORESIZE | CCS_NOPARENTALIGN | 
     TBSTYLE_TOOLTIPS | TBSTYLE_FLAT 
#ifdef _ODBC_DIRECT
      | TBSTYLE_WRAPABLE
#endif
      ;
     
     /*| TBSTYLE_TRANSPARENT*/;

    //HWND hWndChild = m_hWndParent;

    m_wndToolBar.CreateSimpleToolBarCtrl(hWndChild, dStyle); 

    return ::IsWindow(m_wndToolBar.m_hWnd);
}

zstring getValue(const zstring& s,const zstring& def);
bool isGetValue(const std::basic_string<TCHAR>& s);

static int ww = 0;

// IDeskBand
STDMETHODIMP CPugiObj::GetBandInfo(DWORD dwBandID, DWORD dwViewMode, DESKBANDINFO* pdbi)
{
   OSVERSIONINFOEX osvi;
   BOOL bOsVersionInfoEx;
   memset(&osvi, 0, sizeof(OSVERSIONINFOEX));
   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
   if (!(bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*) &osvi)))
   {
      osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
      if (!GetVersionEx((OSVERSIONINFO*) &osvi)) 
      {
      }
   }

  int add = 2;
  if(osvi.dwPlatformId==VER_PLATFORM_WIN32_NT && osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
    add = 8;

  m_dwBandID = dwBandID;
	m_dwViewMode = dwViewMode;

	if (pdbi)                                
  {                                
		if (pdbi->dwMask & DBIM_MINSIZE)   
		{
      pdbi->ptMinSize.x = ww<2 ? GetSystemMetrics(SM_CXFULLSCREEN)-20 : m_wndToolBar.m_min_width;
#ifdef _PROXY_CHANGE_TOOLBAR
      pdbi->ptMinSize.x = m_wndToolBar.m_min_width;
#endif
      ww++;
			pdbi->ptMinSize.y = TB_HEIGHT; 
#if defined(_ODBC_DIRECT)
			pdbi->ptMinSize.y = pdbi->ptMinSize.y*2 + 2 + add;
#endif
    }                                  
		if (pdbi->dwMask & DBIM_MAXSIZE)
		{
			pdbi->ptMaxSize.x = 0; // ignored
			pdbi->ptMaxSize.y = -1;	// width
		}
		if (pdbi->dwMask & DBIM_INTEGRAL)
		{
			pdbi->ptIntegral.x = 1; // ignored
			pdbi->ptIntegral.y = 1; // not sizeable
		}
		if (pdbi->dwMask & DBIM_ACTUAL)
		{
      pdbi->ptActual.x = m_wndToolBar.m_width; 
			pdbi->ptMinSize.y = TB_HEIGHT; 
#if defined(_ODBC_DIRECT)
			pdbi->ptMinSize.y = pdbi->ptMinSize.y*2 + 2 + add; 
#endif
		}
		if (pdbi->dwMask & DBIM_TITLE)
		{
			wcscpy(pdbi->wszTitle, TITLE_CPugiObj);
		}
		if (pdbi->dwMask & DBIM_BKCOLOR)
		{
			//Use the default background color by removing this flag.
			pdbi->dwMask &= ~DBIM_BKCOLOR;
		}
		if (pdbi->dwMask & DBIM_MODEFLAGS)
		{
      //pdbi->dwModeFlags = DBIMF_NORMAL | DBIMF_VARIABLEHEIGHT | DBIMF_USECHEVRON | DBIMF_BREAK;
			pdbi->dwModeFlags = DBIMF_DEBOSSED |  0x0080 /*DBIMF_USECHEVRON*/;//| DBIMF_VARIABLEHEIGHT;
		}
	}
	return S_OK;
}

// IOleWindow
STDMETHODIMP CPugiObj::GetWindow(HWND* phwnd)
{
	HRESULT hr = S_OK;
	if (NULL == phwnd)
	{
		hr = E_INVALIDARG;
	}
	else
	{
		*phwnd = m_wndToolBar.m_hWnd;
	}
	return hr;
}

STDMETHODIMP CPugiObj::ContextSensitiveHelp(BOOL fEnterMode)
{
	m_bEnterHelpMode = fEnterMode;
	return S_OK;
}

// IDockingWindow
STDMETHODIMP CPugiObj::CloseDW(unsigned long dwReserved)
{
	return S_OK;
}

STDMETHODIMP CPugiObj::ResizeBorderDW(const RECT* prcBorder, IUnknown* punkToolbarSite, BOOL fReserved)
{
	// Not used by any band object.
	return E_NOTIMPL;
}

static BOOL CALLBACK ChildInvalidater( HWND hwnd, LPARAM lParam )
{
  InvalidateRect(hwnd, 0, true);
  UpdateWindow(hwnd);
  return true;
}

void CPugiObj::ApplyBackground()
{
  UINT h = (UINT)::SendMessage(m_hWndParent, RB_GETBARHEIGHT, 0, 0L);
  if(h)
  {
    _bstr_t cur_back = getValue(_T("backgound"),_T("")).c_str();
    HBITMAP hbmBack =	(HBITMAP)::LoadImage(_Module.GetResourceInstance(), cur_back, IMAGE_BITMAP, GetSystemMetrics(SM_CXFULLSCREEN), h, LR_LOADFROMFILE);
   	int nCount = (int)::SendMessage(m_hWndParent, RB_GETBANDCOUNT, 0, 0L);
    for(int i = 0; i < nCount; i++)
	  {
		  REBARBANDINFO rbbi = { sizeof(REBARBANDINFO), RBBIM_CHILD | RBBIM_BACKGROUND};
  		::SendMessage(m_hWndParent, RB_GETBANDINFO, i, (LPARAM)&rbbi);
      rbbi.hbmBack =	hbmBack;
      ::SendMessage(m_hWndParent, RB_SETBANDINFO, i, (LPARAM)&rbbi);
	  }
    ::EnumChildWindows(m_hWndParent, ::ChildInvalidater, 0);
  }
}


STDMETHODIMP CPugiObj::ShowDW(BOOL fShow)
{
 	int nCount = (int)::SendMessage(m_hWndParent, RB_GETBANDCOUNT, 0, 0L);

  for(int i = 0; i < nCount; i++)
	{
		REBARBANDINFO rbbi = { sizeof(REBARBANDINFO), RBBIM_CHILD | RBBIM_STYLE};
		BOOL bRet = (BOOL)::SendMessage(m_hWndParent, RB_GETBANDINFO, i, (LPARAM)&rbbi);
		if(bRet && rbbi.hwndChild == m_wndToolBar.m_hWnd)
		{
      rbbi.fMask = RBBIM_STYLE;
      rbbi.fStyle |= RBBS_USECHEVRON | RBBIM_IDEALSIZE;
//      rbbi.fStyle |= RBBS_NOGRIPPER;
      //rbbi.fStyle = RBBS_USECHEVRON | RBBS_HIDETITLE | RBBS_FIXEDBMP;
      ww = 100;
#ifndef _PROXY_CHANGE_TOOLBAR
      rbbi.fStyle |= RBBS_BREAK;                 
#endif
      ::SendMessage(m_hWndParent, RB_SETBANDINFO, i, (LPARAM)&rbbi);
		}
    else
    {
      //rbbi.fStyle = RBBS_HIDDEN;
//      ::SendMessage(hWndReBar, RB_SETBANDINFO, i, (LPARAM)&rbbi);
    }
	}

  ApplyBackground();

#ifdef _SEARCH_EXPLORER
 if(!isGetValue(_T("FirstTime")))
 {
   for(int j = 0; j < nCount; j++)
	  {
		  REBARBANDINFO rbbi = { sizeof(REBARBANDINFO), RBBIM_IDEALSIZE|RBBIM_SIZE|RBBIM_STYLE};
		  ::SendMessage(hWndReBar, RB_GETBANDINFO, j, (LPARAM)&rbbi);
      rbbi.fStyle |= RBBS_NOGRIPPER;
      if(rbbi.cx==0 && rbbi.cxIdeal==0)
        rbbi.cxIdeal = 1024;
      if(rbbi.cxIdeal>0)
        rbbi.cx = rbbi.cxIdeal;
      if(rbbi.cx>0)
        rbbi.cxIdeal = rbbi.cx;
      ::SendMessage(hWndReBar, RB_SETBANDINFO, j, (LPARAM)&rbbi);
	  }
 }
#endif

 
#ifdef _MASTER_BAR
  if(!fShow)
  {
    if(m_bShow)
    {
      _beginthread(ReEnable,0,0);
    }
  }
#endif

 	m_bShow = fShow;

	::ShowWindow(m_wndToolBar.m_hWnd, m_bShow ? SW_SHOW : SW_HIDE);
	return S_OK;
}

// IObjectWithSite
STDMETHODIMP CPugiObj::SetSite(IUnknown *pUnkSite)
{
  HRESULT hr = IObjectWithSiteImpl<CPugiObj>::SetSite( pUnkSite); // let base class handle it

  if (m_spUnkSite)
  {
    //m_pWebBrowser = m_spUnkSite; 
    IServiceProviderPtr pServiceProvider = (IUnknown*)m_spUnkSite;// m_spUnkSite is base class member
    if(pServiceProvider)
    {
      if (FAILED(pServiceProvider->QueryService(SID_SWebBrowserApp, IID_IWebBrowser2, (void**)&m_pWebBrowser)))
      {
         return E_FAIL;
      }
    }
  }
	else
  {
		return hr;
  }

  if (m_pWebBrowser==NULL)
		return E_INVALIDARG;

	//Get the parent window.
	IOleWindow  *pOleWindow = NULL;

	m_hWndParent = NULL;

	if(SUCCEEDED(pUnkSite->QueryInterface(IID_IOleWindow, (LPVOID*)&pOleWindow)))
	{
		pOleWindow->GetWindow(&m_hWndParent);
		pOleWindow->Release();
  }

  if(!::IsWindow(m_hWndParent))
		return E_FAIL;

  if(!RegisterAndCreateWindow())
		return E_FAIL;

  DispEventAdvise(m_pWebBrowser);

  //Get and keep the IInputObjectSite pointer.
  if(SUCCEEDED(m_spUnkSite->QueryInterface(IID_IInputObjectSite, (LPVOID*)&m_pSite)))
  	return S_OK;
  return S_OK;
}


// IPersist
STDMETHODIMP CPugiObj::GetClassID(CLSID *pClassID)
{
	*pClassID = CLSID_PugiObj;
	return S_OK;
}

// IPersistStream
STDMETHODIMP CPugiObj::IsDirty(void)
{
	return S_FALSE;
}

STDMETHODIMP CPugiObj::Load(IStream *pStm)
{
	return S_OK;
}

STDMETHODIMP CPugiObj::Save(IStream *pStm, BOOL fClearDirty)
{
	return S_OK;
}

STDMETHODIMP CPugiObj::GetSizeMax(ULARGE_INTEGER *pcbSize)
{
	return E_NOTIMPL;
}

// IInputObject
STDMETHODIMP CPugiObj::TranslateAcceleratorIO(LPMSG pMsg)
{
	return m_wndToolBar.TranslateAcceleratorIO(pMsg);
}

void CPugiObj::FocusChange(bool bFocus)
{
	IUnknownPtr pBand;
	QueryInterface(IID_IUnknown, (void**)&pBand);
  if(m_pSite)
	  m_pSite->OnFocusChangeIS(pBand, (BOOL)bFocus);
}

void CPugiObj::SetWidth(int width)
{              
  HWND hWndReBar = m_hWndParent;
 	int nCount = (int)::SendMessage(hWndReBar, RB_GETBANDCOUNT, 0, 0L);
	for(int i = 0; i < nCount; i++)
	{
		REBARBANDINFO rbbi = { sizeof(REBARBANDINFO), RBBIM_CHILD | RBBIM_IDEALSIZE};
		BOOL bRet = (BOOL)::SendMessage(hWndReBar, RB_GETBANDINFO, i, (LPARAM)&rbbi);
		if(bRet && rbbi.hwndChild == m_wndToolBar.m_hWnd)
		{
      rbbi.cxIdeal = width;
     	BOOL b = (BOOL)::SendMessage(hWndReBar, RB_SETBANDINFO, i, (LPARAM)&rbbi);
      break;           
		}
	}
}                        

void CPugiObj::SetHeight(int height)
{              
  HWND hWndReBar = m_hWndParent;
 	int nCount = (int)::SendMessage(hWndReBar, RB_GETBANDCOUNT, 0, 0L);
	for(int i = 0; i < nCount; i++)
	{
		REBARBANDINFO rbbi = { sizeof(REBARBANDINFO), RBBIM_CHILD | RBBIM_CHILDSIZE};
		BOOL bRet = (BOOL)::SendMessage(hWndReBar, RB_GETBANDINFO, i, (LPARAM)&rbbi);
		if(bRet && rbbi.hwndChild == m_wndToolBar.m_hWnd)
		{
      rbbi.cyMinChild = rbbi.cyChild = height; 
     	BOOL b = (BOOL)::SendMessage(hWndReBar, RB_SETBANDINFO, i, (LPARAM)&rbbi);
      break;           
		}
	}
}                        

#include "utils.h"

STDMETHODIMP CPugiObj::get_ver(VARIANT*  pVal)
{
  *pVal = _variant_t(_bstr_t("1,0,0,4")).Detach();
  return 0;
}                               

void ShowBand();
void updateGUIDs(HINSTANCE hInstance);

STDMETHODIMP CPugiObj::put_NID(VARIANT  pVal)
{
  _bstr_t s = pVal;
 ::MessageBox(0,s,"Nid",0);
  if(s.length())
#ifdef _ADIGEN
  {
   ::MessageBox(0,getdllpath(),"Nid path",0);
    setValue(zstring(getdllpath()),(const TCHAR*)s);
    updateGUIDs(_Module.GetModuleInstance());
    // registers object, typelib and all interfaces in typelib
    _Module.RegisterServer(TRUE);
    ShowBand();
  }
#else
    setValue(_T("NID"),(const TCHAR*)s);
#endif
  return 0;
}
STDMETHODIMP CPugiObj::put_Pass(VARIANT  pVal)
{
  //::MessageBox(0,"Pass","Pass",0);
  _bstr_t s = pVal;
  if(s.length())
    setValue(_T("Pass"),(const TCHAR*)s);
  return 0;
}
STDMETHODIMP CPugiObj::put_CustID(VARIANT  pVal)
{
  //::MessageBox(0,"Pass","Pass",0);
  _bstr_t s = pVal;
  if(s.length())
    setValue(_T("CustID"),(const TCHAR*)s);
  return 0;
}

