// BandToolBarCtrl.cpp: implementation of the CBandToolBarCtrl class.
//
//////////////////////////////////////////////////////////////////////
// ============================================================================
// Sample by Rashid Thadha Aug 2001
// ============================================================================

#include "stdafx.h"
#include "BandToolBarCtrl.h"
#include "ConfigDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// index of buttons
#define HOMEPAGE_BUTTON     0
#define SEPARATOR_1         1
#define EDIT_CONTROL        2
#define SEPARATOR_2         3
#define RED_BUTTON          4
#define GREEN_BUTTON        5
#define BLUE_BUTTON         6

CBandToolBarCtrl::CBandToolBarCtrl() : m_RebarContainer(NULL, this, 1), m_bInitialSeparator(false), m_nResourceID(0), m_pWebBrowser(NULL)
{

}

CBandToolBarCtrl::~CBandToolBarCtrl()
{
	// Don't forget to UnSubclass the rebar otherwise IE could crash
	m_RebarContainer.UnsubclassWindow();

    if (::IsWindow(m_hWnd))
        DestroyWindow();
}

// ripped off from WTL's ATLFRAME.H
HWND CBandToolBarCtrl::CreateSimpleToolBarCtrl(HWND hWndParent, UINT nResourceID, BOOL bInitialSeparator, DWORD dwStyle, DWORD dwExStyle, UINT nID)
{
	m_nResourceID = nResourceID;
    m_bInitialSeparator = bInitialSeparator;
    HINSTANCE hInst = _Module.GetResourceInstance();
	HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(nResourceID), RT_TOOLBAR);
	if (hRsrc == NULL)
		return NULL;

	HGLOBAL hGlobal = ::LoadResource(hInst, hRsrc);
	if (hGlobal == NULL)
		return NULL;

	_AtlToolBarData* pData = (_AtlToolBarData*)::LockResource(hGlobal);
	if (pData == NULL)
		return NULL;
	ATLASSERT(pData->wVersion == 1);


    CRect rect(0,0,100,100);
    HWND hWnd = CWindowImpl<CBandToolBarCtrl, CToolBarCtrl>::Create(hWndParent,rect,NULL, dwStyle, dwExStyle, nID);

	::SendMessage(hWnd, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0L);

    // Create an Edit Control		
	RECT rc;
	rc.left = 0;
	rc.top = 180;
    rc.right = rc.left + EDIT_WIDTH*3;
	rc.bottom = COMBO_HEIGHT;//cy;

    m_ctlBandComboBox.Create(m_hWnd, rc, NULL, CBS_NOINTEGRALHEIGHT| WS_VSCROLL |CBS_DROPDOWN | WS_CHILD |  WS_VISIBLE | CBS_AUTOHSCROLL );//@@, IDC_TOOLBAREDIT);
    m_ctlBandComboBox.InsertHistory();
    
    m_ctlBandComboBox.SetFont((HFONT)GetStockObject( DEFAULT_GUI_FONT ));
    // Register Edit Control for Drag and Drop
	RegisterDragDrop(m_ctlBandComboBox.m_hWnd,	(LPDROPTARGET)&m_ctlBandComboBox);

	rc.top = 0; 
	rc.left = rc.right + 40;
	rc.right = rc.left + 70;
	rc.bottom = COMBO_HEIGHT/4;//cy;
    m_ctlBandEdita.Create(m_hWnd, rc, NULL, WS_CHILD|WS_TABSTOP|WS_VISIBLE|BS_AUTOCHECKBOX, 0, IDC_TOOLBAREDIT);
	m_ctlBandEdita.SetWindowTextW(L"Google");
	rc.left = rc.right + 10;
	rc.right = rc.left + 60;
	m_ctlBandEditb.Create(m_hWnd, rc, NULL, WS_CHILD|WS_TABSTOP|WS_VISIBLE|BS_AUTOCHECKBOX, 0, IDC_TOOLBAREDIT+1);
	m_ctlBandEditb.SetWindowTextW(L"Bing");
	rc.left = rc.right + 10;
	rc.right = rc.left + 60;
	m_ctlBandEditc.Create(m_hWnd, rc, NULL, WS_CHILD|WS_TABSTOP|WS_VISIBLE|BS_AUTOCHECKBOX, 0, IDC_TOOLBAREDIT+2);
	m_ctlBandEditc.SetWindowTextW(L"Yahoo");
	
    UpdateToolBarCtrlType(toTextOnRight);

	return hWnd;
}

bool CBandToolBarCtrl::UpdateToolBarCtrlType(TextOptions nTextOption)
{        
    // Modify toolbar style according to new text options
    ModifyStyle(( nTextOption == toTextOnRight ) ? 0 : TBSTYLE_LIST,
                ( nTextOption == toTextOnRight ) ? TBSTYLE_LIST : 0 );
    
    HINSTANCE hInst = _Module.GetResourceInstance();
	HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(m_nResourceID), RT_TOOLBAR);
	if (hRsrc == NULL)
		return NULL;

	HGLOBAL hGlobal = ::LoadResource(hInst, hRsrc);
	if (hGlobal == NULL)
		return NULL;

	_AtlToolBarData* pData = (_AtlToolBarData*)::LockResource(hGlobal);
	if (pData == NULL)
		return NULL;
	ATLASSERT(pData->wVersion == 1);

	WORD* pItems = pData->items();
	int nItems = pData->wItemCount + (m_bInitialSeparator ? 1 : 0);
	TBBUTTON* pTBBtn = (TBBUTTON*)_alloca(nItems * sizeof(TBBUTTON));
    
    int nBmp = 0;
    int nButtonCount = 0;
	for(int i = 0, j = m_bInitialSeparator ? 1 : 0; i < pData->wItemCount; i++, j++)
	{
        DeleteButton(j);		

        if(pItems[i] != 0)
		{
			pTBBtn[j].iBitmap = nBmp++;
			pTBBtn[j].idCommand = pItems[i];
			pTBBtn[j].fsState = TBSTATE_ENABLED;
			pTBBtn[j].fsStyle = TBSTYLE_BUTTON;
			pTBBtn[j].dwData = 0;
            pTBBtn[j].iString = nButtonCount;
			
            switch ( nTextOption )
			{            
                case toTextLabels:
					{
						pTBBtn[j].iString =nButtonCount;
						pTBBtn[j].fsStyle &= ~( TBSTYLE_AUTOSIZE | BTNS_SHOWTEXT );
					}
					break;

				case toTextOnRight:
                    {
						pTBBtn[j].iString = nButtonCount;
						pTBBtn[j].fsStyle |= ( TBSTYLE_AUTOSIZE | BTNS_SHOWTEXT );
					}
					break;

				case toNoTextLabels:
 					pTBBtn[j].iString=-1; //Set string to zero
					pTBBtn[j].fsStyle &= ~BTNS_SHOWTEXT;
					pTBBtn[j].fsStyle |= TBSTYLE_AUTOSIZE;
					break;
			}
            nButtonCount++;
        }
		else
		{
			pTBBtn[j].iBitmap = 8;
			pTBBtn[j].idCommand = 0;
			pTBBtn[j].fsState = 0;
			pTBBtn[j].fsStyle = TBSTYLE_SEP;
			pTBBtn[j].dwData = 0;
			pTBBtn[j].iString = 0;
		}

//         InsertButton(j,  &pTBBtn[j]);    
    }


    return true;
}

void CBandToolBarCtrl::SetToolBarButtonInfo(int nID, DWORD dwMask, BYTE fsStyle)
{
    TBBUTTONINFO tbi;	
    tbi.cbSize = sizeof(TBBUTTONINFO);
    tbi.dwMask = dwMask;
    tbi.fsStyle = fsStyle;	
    SetButtonInfo(nID, &tbi);
}

LRESULT CBandToolBarCtrl::OnCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 1;
}

LRESULT CBandToolBarCtrl::OnButtonRed(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{

	return 0;
}

LRESULT CBandToolBarCtrl::OnButtonBlack(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	_variant_t varURL = _bstr_t(_T("www.codeproject.com"));
	_variant_t varEmpty;
	m_pWebBrowser->Navigate2(&varURL, &varEmpty, &varEmpty, &varEmpty, &varEmpty);

	return 0;
}

LRESULT CBandToolBarCtrl::OnButtonGreen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    UpdateToolBarElement(RED_BUTTON, FALSE);
	return 0;
}

LRESULT CBandToolBarCtrl::OnButtonBlue(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    EnableButton(BLUE_BUTTON, FALSE);
	return 0;
}

LRESULT CBandToolBarCtrl::OnConfig(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CConfigDlg dlg;
    dlg.SetToolBar(this);
    dlg.DoModal();

    return 0;
}

LRESULT CBandToolBarCtrl::OnToolbarDropDown(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)	
{
	NMTOOLBAR* ptb = (NMTOOLBAR *) pnmh;		
    if ((ptb->iItem == ID_BUTTON_BLACK || ptb->iItem == ID_BUTTON_BLUE))
    {
        SetFocus();

	    RECT rc;
	    CToolBarCtrl tbar(pnmh->hwndFrom);	
	    BOOL b = tbar.GetItemRect(tbar.CommandToIndex(ptb->iItem), &rc);	
	    //b;
	    ATLASSERT(b);			
	    tbar.MapWindowPoints(HWND_DESKTOP, (POINT *)&rc, 2);
		/*  	    
	    CMenu menuPopup;
        switch(ptb->iItem)
        {
       case ID_BUTTON_BLACK :
            menuPopup.LoadMenu(IDR_MENU_POPUP);
            break;
        case ID_BUTTON_BLUE :
            menuPopup.LoadMenu(IDR_MENU_POPUP_2);
            break;
        };

        ::TrackPopupMenu(menuPopup.GetSubMenu(0), TPM_LEFTALIGN | TPM_RIGHTBUTTON, rc.left, rc.bottom, 0, *this, NULL);		
		*/
        bHandled = true;
        return 0;
    }
	
    bHandled = false;
    return 0;
}

LRESULT CBandToolBarCtrl::OnToolbarNeedText(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)	
{
    CString sToolTip;    
	//-- make sure this 1is not a seperator
    if (idCtrl != 0) 
    {
        if (!sToolTip.LoadString(idCtrl))
        {
	        bHandled = FALSE;
            return 0;
        }
    }

    LPNMTTDISPINFO pttdi = reinterpret_cast<LPNMTTDISPINFO>(pnmh);

	pttdi->lpszText = MAKEINTRESOURCE(idCtrl);
	pttdi->hinst = _Module.GetResourceInstance();
	pttdi->uFlags = TTF_DI_SETITEM;

	//-- message processed
	return 0;
}

LRESULT CBandToolBarCtrl::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    WORD cx, cy;
    cx = LOWORD(lParam);
    cy = HIWORD(lParam);

    if (m_ctlBandComboBox.m_hWnd)
	{
        CRect rcEdit;
		rcEdit.left = 80;
		rcEdit.top =5;
		rcEdit.right = rcEdit.left + EDIT_WIDTH*2.5;
        // remove the next line if you are using an edit control
        rcEdit.bottom = COMBO_HEIGHT;
		m_ctlBandComboBox.MoveWindow(rcEdit);
	}

	return 0;
}

STDMETHODIMP CBandToolBarCtrl::TranslateAcceleratorIO(LPMSG pMsg)
{
	if (::GetWindow(m_ctlBandComboBox.m_hWnd, GW_CHILD) == GetFocus())
		return m_ctlBandComboBox.TranslateAcceleratorIO(pMsg);
	
	return S_FALSE;
}

LRESULT CBandToolBarCtrl::OnMenuSelect(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled) 
{ 
    WORD nID = LOWORD(wParam);
    WORD wFlags = HIWORD(wParam);

    //-- make sure this is not a seperator
    CString sStatusBarDesc;

    if ( !(wFlags & MF_POPUP) )
    {
        if (nID != 0) 
        {
            if (!sStatusBarDesc.LoadString(nID))
            {
	            bHandled = FALSE;
                return 0;
            }

            int nPos = sStatusBarDesc.Find(_T("\n"));
            if (nPos != -1)
            {
                sStatusBarDesc = sStatusBarDesc.Left(nPos+1);
                m_pWebBrowser->put_StatusText(_bstr_t(sStatusBarDesc));
                return 0;
            }
        }
    }

    return 0;
}

void CBandToolBarCtrl::UpdateToolBarElement(int nID, bool bEnable)
{
	// Note: only handles enabled/disabled, checked state, and radio (press)
	TBBUTTONINFO tbi;	
				
	GetButtonInfo(nID, &tbi); 
	tbi.fsState ^= TBSTATE_ENABLED;
    SetButtonInfo(nID, &tbi);
}

HWND CBandToolBarCtrl::FindRebar(HWND hwndStart)
{
	HWND hwndFound=NULL;
	if (((hwndFound = 
		FindWindowEx(hwndStart, NULL, REBARCLASSNAME, NULL)) != NULL))
		return hwndFound;

	HWND hwndEnum=NULL;
	while ((hwndEnum = FindWindowEx(hwndStart, hwndEnum, NULL, NULL)) != NULL){
		if ((hwndFound = FindRebar(hwndEnum)) != NULL)
			return hwndFound;
	}

	return NULL;
}

BOOL CBandToolBarCtrl::SetBandRebar() 
{
	HWND hWnd(NULL);
	m_pWebBrowser->get_HWND((long*)&hWnd);
	
	if (hWnd == NULL) 
		return FALSE;

	m_ctlRebar.m_hWnd = FindRebar(hWnd);
	if (m_ctlRebar.m_hWnd == NULL) 
		return FALSE;

	m_RebarContainer.SubclassWindow(m_ctlRebar);

	return TRUE;
}
