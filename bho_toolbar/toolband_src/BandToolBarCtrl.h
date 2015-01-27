// BandToolBarCtrl.h: interface for the CBandToolBarCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BANDTOOLBARCTRL_H__DE3301F1_2EBB_4B03_8B41_51930CE14993__INCLUDED_)
#define AFX_BANDTOOLBARCTRL_H__DE3301F1_2EBB_4B03_8B41_51930CE14993__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "BandEditCtrl.h"

#ifndef TBSTYLE_EX_MIXEDBUTTONS
#define TBSTYLE_EX_MIXEDBUTTONS         0x00000008
#define TBSTYLE_EX_HIDECLIPPEDBUTTONS   0x00000010
#define CCM_SETVERSION       (CCM_FIRST+7)
#define CCM_GETVERSION       (CCM_FIRST+8)
#endif  // !TBSTYLE_EX_MIXEDBUTTONS


// if you dont have the latest SDK, then you can use these, if you do then delete these
#ifndef BTNS_SHOWTEXT
#define BTNS_SHOWTEXT           0x0040
#endif  // !BTNS_SHOWTEXT

#ifndef BTNS_WHOLEDROPDOWN
#define BTNS_WHOLEDROPDOWN      0x0080
#endif  // !BTNS_WHOLEDROPDOWN

class CBandToolBarReflectorCtrl : public CWindowImpl<CBandToolBarReflectorCtrl, CToolBarCtrl>
{
public:
    CBandToolBarReflectorCtrl() {}
    virtual ~CBandToolBarReflectorCtrl() 
    {
        if (::IsWindow(m_hWnd))
            DestroyWindow();
    }

    DECLARE_WND_SUPERCLASS( _T("BandToolBarReflectorCtrl"), CToolBarCtrl::GetWndClassName() )

    // Make sure we reflect everything back to the child
    BEGIN_MSG_MAP(CBandToolBarReflectorCtrl)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

protected:
};


class CBandToolBarCtrl : public CWindowImpl<CBandToolBarCtrl, CToolBarCtrl>
{
public:
	CBandToolBarCtrl();
	virtual ~CBandToolBarCtrl();

    enum TextOptions
    {
        toNone          = -1,
        toTextLabels    =  0,
        toTextOnRight   =  1,
        toNoTextLabels  =  2,
    };


    DECLARE_WND_SUPERCLASS( _T("BandToolBarCtrl"), CToolBarCtrl::GetWndClassName() )
    HWND CreateSimpleToolBarCtrl(HWND hWndParent, UINT nResourceID, BOOL bInitialSeparator = FALSE, DWORD dwStyle = 0, DWORD dwExStyle = 0, UINT nID = ATL_IDW_TOOLBAR);
    bool UpdateToolBarCtrlType(TextOptions nTextOption);
    void UpdateToolBarElement(int nID, bool bEnable);
	BOOL SetBandRebar();
    void SetToolBarButtonInfo(int nID, DWORD dwMask, BYTE fsStyle);
    void SetWebBrowser(IWebBrowser2* pWebBrowser) {m_pWebBrowser = pWebBrowser;}

    BEGIN_MSG_MAP(CBandToolBarCtrl)		
        MESSAGE_HANDLER(OCM_COMMAND, OnCommand)
		OCM_COMMAND_ID_HANDLER(ID_BUTTON_BLACK, OnButtonBlack)
		OCM_COMMAND_ID_HANDLER(ID_BUTTON_RED, OnButtonRed)
		OCM_COMMAND_ID_HANDLER(ID_BUTTON_GREEN, OnButtonGreen)
		OCM_COMMAND_ID_HANDLER(ID_BUTTON_BLUE, OnButtonBlue)
		OCM_NOTIFY_CODE_HANDLER(TBN_DROPDOWN, OnToolbarDropDown)
        NOTIFY_CODE_HANDLER(TTN_NEEDTEXT, OnToolbarNeedText)
        COMMAND_ID_HANDLER(ID_MENUPOPUP_CONFIG, OnConfig)        
        MESSAGE_HANDLER(WM_SIZE, OnSize)
        MESSAGE_HANDLER(WM_MENUSELECT, OnMenuSelect)
        DEFAULT_REFLECTION_HANDLER()
	// handle the rebar chevron messages, does not require reflection
	ALT_MSG_MAP(1)
		COMMAND_ID_HANDLER(ID_BUTTON_BLACK, OnButtonBlack)
		COMMAND_ID_HANDLER(ID_BUTTON_RED, OnButtonRed)
		COMMAND_ID_HANDLER(ID_BUTTON_GREEN, OnButtonGreen)
		COMMAND_ID_HANDLER(ID_BUTTON_BLUE, OnButtonBlue)
		NOTIFY_CODE_HANDLER(TBN_DROPDOWN, OnToolbarDropDown)
	END_MSG_MAP()


    CBandEditCtrl m_ctlBandEdita;
	CBandEditCtrl m_ctlBandEditb;
	CBandEditCtrl m_ctlBandEditc;
    CBandComboBoxCtrl m_ctlBandComboBox;
	CContainedWindow m_RebarContainer;
	CReBarCtrl m_ctlRebar;
	TextOptions  m_eTextOptions;
    IWebBrowser2* m_pWebBrowser;

    STDMETHOD(TranslateAcceleratorIO)(LPMSG pMsg);

protected:
	struct _AtlToolBarData
	{
		WORD wVersion;
		WORD wWidth;
		WORD wHeight;
		WORD wItemCount;
		WORD* items()
			{ return (WORD*)(this+1); }
	};


    LRESULT OnCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);	
    LRESULT OnButtonRed(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnButtonBlack(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnButtonGreen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnButtonBlue(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnConfig(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnToolbarDropDown(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/);
    LRESULT OnToolbarNeedText(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnMenuSelect(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 

	HWND FindRebar(HWND hwndStart);

protected:
    int m_ncy;
    BOOL m_bInitialSeparator;
    UINT m_nResourceID;

};

#endif // !defined(AFX_BANDTOOLBARCTRL_H__DE3301F1_2EBB_4B03_8B41_51930CE14993__INCLUDED_)

