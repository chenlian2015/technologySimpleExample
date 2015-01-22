// ConfigDlg.h : Declaration of the CConfigDlg

#ifndef __CONFIGDLG_H_
#define __CONFIGDLG_H_

#include "resource.h"       // main symbols
#include <atlhost.h>
#include "BandToolBarCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg
class CConfigDlg : 
	public CAxDialogImpl<CConfigDlg>
{
public:
    CConfigDlg() : m_pToolBar(NULL)
	{
	}

	~CConfigDlg()
	{
	}

    void SetToolBar(CBandToolBarCtrl* pToolBar) {m_pToolBar = pToolBar;}

    enum { IDD = IDD_CONFIGDLG };

BEGIN_MSG_MAP(CConfigDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_HANDLER(IDC_RADIO_NO_TEXT, BN_CLICKED, OnClickedRadio_no_text)
	COMMAND_HANDLER(IDC_RADIO_NO_TEXT_RIGHT, BN_CLICKED, OnClickedRadio_no_text_right)
	COMMAND_HANDLER(IDC_RADIO_TEXT_UNDER, BN_CLICKED, OnClickedRadio_text_under)
	COMMAND_HANDLER(ID_APPLY, BN_CLICKED, OnClickedApply)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnOK)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
	    CenterWindow(GetParent());

        ::CheckDlgButton(m_hWnd, IDC_RADIO_NO_TEXT_RIGHT, BST_CHECKED); 
        _Module.m_nToolBarButtonType = 1;

		return 1;  // Let the system set the focus
	}

	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
        if (m_pToolBar)
            m_pToolBar->UpdateToolBarCtrlType((CBandToolBarCtrl::TextOptions)_Module.m_nToolBarButtonType);

		EndDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}
	LRESULT OnClickedRadio_no_text(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
        _Module.m_nToolBarButtonType = 2;

		return 0;
	}
	LRESULT OnClickedRadio_no_text_right(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
        _Module.m_nToolBarButtonType = 1;
		return 0;
	}
	LRESULT OnClickedRadio_text_under(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
        _Module.m_nToolBarButtonType = 0;
		return 0;
	}

	LRESULT OnClickedApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
        if (m_pToolBar)
            m_pToolBar->UpdateToolBarCtrlType((CBandToolBarCtrl::TextOptions)_Module.m_nToolBarButtonType);

		return 0;
	}

protected:
    CBandToolBarCtrl* m_pToolBar;
};

#endif //__CONFIGDLG_H_
