//[!ShortName].cpp : Implementation of [!ClassName]
[!crlf]
//***************************************************************************//
//                                                                           //
//  This file was created using the DeskBand ATL Object Wizard 2.0           //
//  By Erik Thompson © 2001                                                  //
//  Email questions and comments to erikt@radbytes.com						 //
//                                                                           //
//***************************************************************************//
[!crlf]
#include "stdafx.h"
#include <wchar.h>
#include "[!ProjectNameHeader]"
#include "[!HeaderName]"
[!crlf]
const WCHAR TITLE_[!ClassName][] = L"[!ClassName]";
[!crlf]
/////////////////////////////////////////////////////////////////////////////
// [!ClassName]
[!crlf]
[!ClassName]::[!ClassName](): 
	m_dwBandID(0), 
	m_dwViewMode(0), 
	m_bShow(FALSE), 
	m_bEnterHelpMode(FALSE), 
	m_hWndParent(NULL), 
	m_hWnd(NULL),
	m_pSite(NULL)
{
}
[!crlf]
BOOL [!ClassName]::RegisterAndCreateWindow()
{
	RECT rect;
	::GetClientRect(m_hWndParent, &rect);
[!crlf]
	//
	// TODO: Replace with your implementation
	//
[!crlf]
	m_hWnd = ::CreateWindow(TEXT("STATIC"), 
				TEXT("The [!ClassName] DeskBand"), 
				WS_CHILD | WS_VISIBLE | SS_CENTER,
				rect.left, rect.top, 
				rect.right - rect.left, 
				rect.bottom - rect.top, 
				m_hWndParent, 
				NULL, 
				NULL, 
				NULL);
	return ::IsWindow(m_hWnd);
}
[!crlf]
// IDeskBand
STDMETHODIMP [!ClassName]::GetBandInfo(DWORD dwBandID, DWORD dwViewMode, DESKBANDINFO* pdbi)
{
	m_dwBandID = dwBandID;
	m_dwViewMode = dwViewMode;
[!crlf]
	if (pdbi)
	{
		if (pdbi->dwMask & DBIM_MINSIZE)
		{
			pdbi->ptMinSize.x = 250;
			pdbi->ptMinSize.y = 20;
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
			pdbi->ptActual.x = 250;
			pdbi->ptActual.y = 20;
		}
		if (pdbi->dwMask & DBIM_TITLE)
		{
			wcscpy(pdbi->wszTitle, TITLE_[!ClassName]);
		}
		if (pdbi->dwMask & DBIM_BKCOLOR)
		{
			//Use the default background color by removing this flag.
			pdbi->dwMask &= ~DBIM_BKCOLOR;
		}
		if (pdbi->dwMask & DBIM_MODEFLAGS)
		{
			pdbi->dwModeFlags = DBIMF_VARIABLEHEIGHT;
		}
	}
	return S_OK;
}
[!crlf]
// IOleWindow
STDMETHODIMP [!ClassName]::GetWindow(HWND* phwnd)
{
	HRESULT hr = S_OK;
	if (NULL == phwnd)
	{
		hr = E_INVALIDARG;
	}
	else
	{
		*phwnd = m_hWnd;
	}
	return hr;
}
[!crlf]
STDMETHODIMP [!ClassName]::ContextSensitiveHelp(BOOL fEnterMode)
{
	m_bEnterHelpMode = fEnterMode;

	return S_OK;
}
[!crlf]
// IDockingWindow
STDMETHODIMP [!ClassName]::CloseDW(unsigned long dwReserved)
{
	if (::IsWindow(m_hWnd))
	{
		::DestroyWindow(m_hWnd);
	}
	return S_OK;
}
[!crlf]
STDMETHODIMP [!ClassName]::ResizeBorderDW(const RECT* prcBorder, IUnknown* punkToolbarSite, BOOL fReserved)
{
	// Not used by any band object.
	return E_NOTIMPL;
}
[!crlf]
STDMETHODIMP [!ClassName]::ShowDW(BOOL fShow)
{
	HRESULT hr = S_OK;
	m_bShow = fShow;
	ShowWindow(m_hWnd, m_bShow ? SW_SHOW : SW_HIDE);
	return hr;
}

// IObjectWithSite
STDMETHODIMP [!ClassName]::SetSite(IUnknown* pUnkSite)
{
//If a site is being held, release it.
	if(m_pSite)
	{
		m_pSite->Release();
		m_pSite = NULL;
	}
[!crlf]
	//If punkSite is not NULL, a new site is being set.
	if(pUnkSite)
	{
		//Get the parent window.
		IOleWindow  *pOleWindow = NULL;
[!crlf]
		m_hWndParent = NULL;
[!crlf]
		if(SUCCEEDED(pUnkSite->QueryInterface(IID_IOleWindow, (LPVOID*)&pOleWindow)))
		{
			pOleWindow->GetWindow(&m_hWndParent);
			pOleWindow->Release();
		}
[!crlf]
		if(!::IsWindow(m_hWndParent))
			return E_FAIL;
[!crlf]
		if(!RegisterAndCreateWindow())
			return E_FAIL;
[!crlf]
		//Get and keep the IInputObjectSite pointer.
		if(SUCCEEDED(pUnkSite->QueryInterface(IID_IInputObjectSite, (LPVOID*)&m_pSite)))
		{
			return S_OK;
		}  
		return E_FAIL;
	}
	return S_OK;
}
[!crlf]
STDMETHODIMP [!ClassName]::GetSite(REFIID riid, void **ppvSite)
{
	*ppvSite = NULL;
[!crlf]
	if(m_pSite)
	{
	   return m_pSite->QueryInterface(riid, ppvSite);
	}
	return E_FAIL;
}
[!crlf]
// IPersist
STDMETHODIMP [!ClassName]::GetClassID(CLSID *pClassID)
{
	*pClassID = CLSID_[!CoClassName];
	return S_OK;
}
[!crlf]
// IPersistStream
STDMETHODIMP [!ClassName]::IsDirty(void)
{
	return S_FALSE;
}
[!crlf]
STDMETHODIMP [!ClassName]::Load(IStream *pStm)
{
	return S_OK;
}
[!crlf]
STDMETHODIMP [!ClassName]::Save(IStream *pStm, BOOL fClearDirty)
{
	return S_OK;
}
[!crlf]
STDMETHODIMP [!ClassName]::GetSizeMax(ULARGE_INTEGER *pcbSize)
{
	return E_NOTIMPL;
}
