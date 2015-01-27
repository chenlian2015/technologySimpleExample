//ToolBandObj.cpp : Implementation of CToolBandObj

//***************************************************************************//
//                                                                           //
//  This file was created using the DeskBand ATL Object Wizard 2.0           //
//  By Erik Thompson ?2001                                                  //
//  Email questions and comments to erikt@radbytes.com						 //
//                                                                           //
//***************************************************************************//

// ============================================================================
// Sample by Rashid Thadha Aug 2001
// ============================================================================


#include "stdafx.h"

#include "ToolBand.h"
#include "ToolBandObj.h"
#include <wchar.h>

#define IDM_DISPLAY 0

const WCHAR TITLE_CToolBandObj[] = L"ToolBand";

#ifndef RBBS_USECHEVRON
#define RBBS_USECHEVRON     0x00000200  // display drop-down button for this band if it's sized smaller than ideal width
#endif

#if (_WIN32_IE >= 0x0501)               //
#define RBBS_HIDETITLE      0x00000400  // keep band title hidden
#endif // 0x0501                        //

// if you have the latest SDK then you can remove these definitions
#ifndef DBIMF_NORMAL
#define DBIMF_NORMAL 0x0000
#endif

#ifndef DBIMF_FIXED
#define DBIMF_FIXED 0x0001
#endif

#ifndef DBIMF_FIXEDBMP
#define DBIMF_FIXEDBMP 0x0004
#endif

#ifndef DBIMF_VARIABLEHEIGHT
#define DBIMF_VARIABLEHEIGHT 0x0008
#endif

#ifndef DBIMF_UNDELETEABLE
#define DBIMF_UNDELETEABLE 0x0010
#endif

#ifndef DBIMF_DEBOSSED
#define DBIMF_DEBOSSED 0x0020
#endif

#ifndef DBIMF_BKCOLOR
#define DBIMF_BKCOLOR 0x0040
#endif

#ifndef DBIMF_USECHEVRON
#define DBIMF_USECHEVRON 0x0080
#endif 

#ifndef DBIMF_BREAK
#define DBIMF_BREAK 0x0100
#endif

#ifndef DBIMF_ADDTOFRONT
#define DBIMF_ADDTOFRONT 0x0200
#endif

#ifndef DBIMF_TOPALIGN
#define DBIMF_TOPALIGN 0x0400
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBandObj

CToolBandObj::CToolBandObj(): 
	m_dwBandID(0), 
	m_dwViewMode(0), 
	m_bShow(FALSE), 
	m_bEnterHelpMode(FALSE), 
	m_hWndParentRebar(NULL), 
	m_pSite(NULL)
{
}

BOOL CToolBandObj::RegisterAndCreateWindow()
{
	RECT rectClientParent;
	::GetClientRect(m_hWndParentRebar, &rectClientParent);

    // We need to create an Invisible Child Window using the Parent Window, this will also be used to reflect Command
    // messages from the rebar
    HWND hWndChild = m_wndInvisibleChildWnd.Create(m_hWndParentRebar,rectClientParent,NULL, WS_CHILD);
    
    // Now we can create the Tool Bar, using the Invisible Child
    DWORD dStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_TABSTOP | 
    CCS_TOP | CCS_NODIVIDER | CCS_NORESIZE | CCS_NOPARENTALIGN | 
    TBSTYLE_TOOLTIPS | TBSTYLE_FLAT | TBSTYLE_TRANSPARENT;

    HWND hWnd = m_wndToolBar.CreateSimpleToolBarCtrl(hWndChild, IDR_TOOLBAR_TEST, FALSE, dStyle); 
	m_wndToolBar.SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS | TBSTYLE_EX_MIXEDBUTTONS); 
    
    // Set the web Broser
    if (m_pWebBrowser)
    {
        m_wndToolBar.SetWebBrowser(m_pWebBrowser);

        // Call this after you get a valid WebBrowser
        m_wndToolBar.SetBandRebar();
    }

    
    // Disable the Red and Green buttons
    int nState = m_wndToolBar.GetState(ID_BUTTON_RED);
    if (nState != -1)
        m_wndToolBar.SetState(ID_BUTTON_RED, nState & ~TBSTATE_ENABLED);

    nState = m_wndToolBar.GetState(ID_BUTTON_GREEN);
    if (nState != -1)
        m_wndToolBar.SetState(ID_BUTTON_GREEN, nState & ~TBSTATE_ENABLED);
	
    TBBUTTONINFO tbi;			
    tbi.cbSize = sizeof(TBBUTTONINFO);
	tbi.dwMask = TBIF_STYLE;
	tbi.fsStyle = TBSTYLE_SEP;
	//tbi.cx = (unsigned short)EDIT_WIDTH;
    m_wndToolBar.SetButtonInfo(ID_EDIT_BOX, &tbi);
          
    m_wndToolBar.m_ctlBandComboBox.m_pBand = this;
    m_wndToolBar.m_ctlBandComboBox.SetWebBrowser(m_pWebBrowser);

	return ::IsWindow(m_wndToolBar.m_hWnd);
}

// IDeskBand
STDMETHODIMP CToolBandObj::GetBandInfo(DWORD dwBandID, DWORD dwViewMode, DESKBANDINFO* pdbi)
{
	m_dwBandID = dwBandID;
	m_dwViewMode = dwViewMode;

	if (pdbi)
	{
		if (pdbi->dwMask & DBIM_MINSIZE)
		{
            pdbi->ptMinSize.x = 20; //sizeBar.cx; //(250)
			pdbi->ptMinSize.y = 22; // 20
		}
		if (pdbi->dwMask & DBIM_MAXSIZE)
		{
			pdbi->ptMaxSize.x = 0; // ignored (0)
			pdbi->ptMaxSize.y = -1;	// width
		}
		if (pdbi->dwMask & DBIM_INTEGRAL)
		{
			pdbi->ptIntegral.x = 1; // ignored (1)
			pdbi->ptIntegral.y = 1; // not sizeable 1
		}
		if (pdbi->dwMask & DBIM_ACTUAL)
		{
            CRect rc;
            int nWidth(0);
            int nCount = m_wndToolBar.GetButtonCount();
            for(int i = 0; i < nCount; i++)
            {
                m_wndToolBar.GetItemRect(i, &rc);
                nWidth += rc.right - rc.left;
            }

            pdbi->ptActual.x = nWidth; 
			pdbi->ptActual.y = 22; // 22
		}
		if (pdbi->dwMask & DBIM_TITLE)
		{
			wcscpy(pdbi->wszTitle, TITLE_CToolBandObj);
		}
		if (pdbi->dwMask & DBIM_BKCOLOR)
		{
			//Use the default background color by removing this flag.
			pdbi->dwMask &= ~DBIM_BKCOLOR;
		}
		if (pdbi->dwMask & DBIM_MODEFLAGS)
		{
            // Add Chevron
            pdbi->dwModeFlags = DBIMF_NORMAL | DBIMF_VARIABLEHEIGHT | DBIMF_USECHEVRON | DBIMF_BREAK;
		}
	}
	return S_OK;
}

// IOleWindow
STDMETHODIMP CToolBandObj::GetWindow(HWND* phwnd)
{
	HRESULT hr = S_OK;
	if (NULL == phwnd)
	{
		hr = E_INVALIDARG;
	}
	else
	{
		*phwnd = m_wndToolBar.m_hWnd;;
	}
	return hr;
}

STDMETHODIMP CToolBandObj::ContextSensitiveHelp(BOOL fEnterMode)
{
	m_bEnterHelpMode = fEnterMode;
	return S_OK;
}

// IDockingWindow
STDMETHODIMP CToolBandObj::CloseDW(unsigned long dwReserved)
{
	return S_OK;
}

STDMETHODIMP CToolBandObj::ResizeBorderDW(const RECT* prcBorder, IUnknown* punkToolbarSite, BOOL fReserved)
{
	// Not used by any band object.
	return E_NOTIMPL;
}

STDMETHODIMP CToolBandObj::ShowDW(BOOL fShow)
{
/*    HWND hWndReBar = m_hWndParentRebar;
    int nCount = (int)::SendMessage(hWndReBar, RB_GETBANDCOUNT, 0, 0L);
    for (int i = 0; i < nCount; i++)
    {
        REBARBANDINFO rbbi = { sizeof(REBARBANDINFO), RBBIM_CHILD | RBBIM_STYLE};
        BOOL bRet = (BOOL)::SendMessage(hWndReBar, RB_GETBANDINFO, i, (LPARAM)&rbbi);
        if (bRet && rbbi.hwndChild == m_wndToolBar.m_hWnd)
        {
            rbbi.fMask = RBBIM_STYLE;
            rbbi.fStyle = RBBS_USECHEVRON | RBBIM_IDEALSIZE;// | RBBS_BREAK
            BOOL bRet = (BOOL)::SendMessage(hWndReBar, RB_SETBANDINFO, i, (LPARAM)&rbbi);
            break;
        }
    }
*/
    HRESULT hr = S_OK;
	m_bShow = fShow;
	ShowWindow(m_wndToolBar.m_hWnd, m_bShow ? SW_SHOW : SW_HIDE);
	return hr;
}
// IObjectWithSite
STDMETHODIMP CToolBandObj::SetSite(IUnknown* pUnkSite)
{
//If a site is being held, release it.
	if(m_pSite)
	{
		m_pSite->Release();
		m_pSite = NULL;
	}

	//If punkSite is not NULL, a new site is being set.
	if(pUnkSite)
	{
		//Get the parent window.
		IOleWindow  *pOleWindow = NULL;

		m_hWndParentRebar = NULL;

		if(SUCCEEDED(pUnkSite->QueryInterface(IID_IOleWindow, (LPVOID*)&pOleWindow)))
		{
			pOleWindow->GetWindow(&m_hWndParentRebar);
			pOleWindow->Release();
		}

		if(!::IsWindow(m_hWndParentRebar))
			return E_FAIL;

		// Minimum of Explorer 4 required
        IServiceProviderPtr pServiceProvider = pUnkSite;
		if (m_pWebBrowser)
			m_pWebBrowser = NULL;
		
        if (FAILED(pServiceProvider->QueryService(SID_SWebBrowserApp, IID_IWebBrowser2, (void**)&m_pWebBrowser)))
            return E_FAIL;

        if(!RegisterAndCreateWindow())
			return E_FAIL;

		//Get and keep the IInputObjectSite pointer.
		if(SUCCEEDED(pUnkSite->QueryInterface(IID_IInputObjectSite, (LPVOID*)&m_pSite)))
		{
			return S_OK;
		}  
		return E_FAIL;
	}
	return S_OK;
}

STDMETHODIMP CToolBandObj::GetSite(REFIID riid, void **ppvSite)
{
	*ppvSite = NULL;

	if(m_pSite)
	{
	   return m_pSite->QueryInterface(riid, ppvSite);
	}
	return E_FAIL;
}

// IPersist
STDMETHODIMP CToolBandObj::GetClassID(CLSID *pClassID)
{
	*pClassID = CLSID_ToolBandObj;
	return S_OK;
}

// IPersistStream
STDMETHODIMP CToolBandObj::IsDirty(void)
{
	return S_FALSE;
}

STDMETHODIMP CToolBandObj::Load(IStream *pStm)
{
	return S_OK;
}

STDMETHODIMP CToolBandObj::Save(IStream *pStm, BOOL fClearDirty)
{
	return S_OK;
}

STDMETHODIMP CToolBandObj::GetSizeMax(ULARGE_INTEGER *pcbSize)
{
	return E_NOTIMPL;
}

// IInputObject
STDMETHODIMP CToolBandObj::UIActivateIO(BOOL fActivate, LPMSG pMsg)
{
    if(fActivate)
		m_wndToolBar.m_ctlBandComboBox.SetFocus();

	return S_OK;
}

STDMETHODIMP CToolBandObj::HasFocusIO(void)
{
    HWND hwnd = GetFocus();

    // See if the focus has been set to any of the children
    //
    HWND hChild = ::GetWindow(m_wndToolBar.m_hWnd, GW_CHILD);
    while (hChild) 
    {
        // check its child
        HWND hGrandChild = ::GetWindow(hChild, GW_CHILD);

        if (hwnd == hChild || hwnd == hGrandChild)
            return S_OK;

        hChild = ::GetWindow(hChild, GW_HWNDNEXT);
    }

    return S_FALSE;
}

STDMETHODIMP CToolBandObj::TranslateAcceleratorIO(LPMSG pMsg)
{
	return m_wndToolBar.TranslateAcceleratorIO(pMsg);
}

void CToolBandObj::FocusChange(bool bFocus)
{
	IUnknownPtr pBand;
	try
	{
		QueryInterface(IID_IUnknown, (void**)&pBand);
		m_pSite->OnFocusChangeIS(pBand, (BOOL)bFocus);
	}
	catch(...)
	{

	}
}
