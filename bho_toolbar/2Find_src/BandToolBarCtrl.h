// BandToolBarCtrl.h: interface for the CBandToolBarCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BANDTOOLBARCTRL_H__DE3301F1_2EBB_4B03_8B41_51930CE14993__INCLUDED_)
#define AFX_BANDTOOLBARCTRL_H__DE3301F1_2EBB_4B03_8B41_51930CE14993__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ATLHOST.H"
#include "ATLCTRLW.H"
#include <ATLcrack.H>
#include <exdispid.h>	// dispatch IDs for DWebBrowserEvents2

#include "XmlParserImpl.h"
using namespace XML;

#define WM_MENU_USER (WM_USER + 1000)

#if defined(_BROKEN) || defined(_QIDION) || defined(_ARUBABAR)
  #define TB_HEIGHT 25
#else
  #define TB_HEIGHT 22
#endif
//22

class CPugiObj;

#define MAX_MENU_SIZE 1450
#define MAX_ADD_COMMANDS (26*MAX_MENU_SIZE)

class CBandToolBarReflectorCtrl : public CWindowImpl<CBandToolBarReflectorCtrl, CToolBarCtrl>,
                                  public CCustomDraw<CBandToolBarReflectorCtrl>
{
public:
    CBandToolBarReflectorCtrl() {}
    virtual ~CBandToolBarReflectorCtrl()
    {
        if (::IsWindow(m_hWnd))
            DestroyWindow();
        m_pBand = 0;
    }

#if defined(BLUE_ON_OVER_TEXT) || defined(USE_TICKER)
    DWORD OnPrePaint(int , LPNMCUSTOMDRAW ) {return CDRF_NOTIFYITEMDRAW;}
  	DWORD OnItemPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW pnmh);
#endif

    DECLARE_WND_SUPERCLASS( _T("BandToolBarReflectorCtrl"), CToolBarCtrl::GetWndClassName() )

    // Make sure we reflect everything back to the child
  BEGIN_MSG_MAP_EX(CBandToolBarReflectorCtrl)
		CHAIN_MSG_MAP( CCustomDraw<CBandToolBarReflectorCtrl> )
    NOTIFY_CODE_HANDLER(TTN_GETDISPINFO, OnToolTip)
    REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	CPugiObj* m_pBand;
protected:
    LRESULT OnToolTip(int,LPNMHDR lParam,BOOL&);
};

class CBandToolBarCtrl;

struct CPugiObjWrap
{
	CPugiObjWrap() {}
  ~CPugiObjWrap() {list2.Destroy();}
  CImageList list,list2;
  virtual CBandToolBarCtrl* root() = 0;
  LRESULT OnMenuItem(WPARAM wParam);
};

class CBandToolBarCtrlFake : public CCommandBarCtrlImpl<CBandToolBarCtrlFake, CCommandBarCtrlBase>,
						                 public CPugiObjWrap
{
  BEGIN_MSG_MAP_EX(CBandToolBarCtrlFake)
    REFLECTED_NOTIFY_CODE_HANDLER_EX(TBN_DROPDOWN,   OnToolbarDropDown)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    REFLECTED_COMMAND_RANGE_HANDLER(WM_USER+0,WM_MENU_USER, OnButton)
    COMMAND_ID_HANDLER(WM_MENU_USER,OnCommand)
    MESSAGE_HANDLER(WM_MENUSELECT, OnMenuItem)

//      MESSAGE_HANDLER(WM_MOUSEMOVE, OnHookMouseMove)
//      MESSAGE_HANDLER(WM_LBUTTONDOWN, OnHookMouseMove)
//      MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnHookMouseMove)

 END_MSG_MAP()

//	LRESULT OnHookMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

  LRESULT OnCommand(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnButton(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnToolbarDropDown(LPNMHDR pnmh);
  LRESULT OnMenuItem(UINT , WPARAM wParam, LPARAM lParam, BOOL& ) {return CPugiObjWrap::OnMenuItem(wParam);}  
  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

  virtual CBandToolBarCtrl* root() {return m_father;}
  void setroot(CBandToolBarCtrl* ctrl) {m_father=ctrl;}
 public:
   SIZE               m_size;
   int                m_id;
 private:
   CBandToolBarCtrl*  m_father;
};


struct Toolbar;

class CBandToolBarCtrl : //public CWindowImpl<CBandToolBarCtrl, CToolBarCtrl> ,
                         public CCommandBarCtrlImpl<CBandToolBarCtrl, CCommandBarCtrlBase>,
                         public CPugiObjWrap
{
public:
	CBandToolBarCtrl();
	virtual ~CBandToolBarCtrl();

    DECLARE_WND_SUPERCLASS( _T("BandToolBarCtrl"), CToolBarCtrl::GetWndClassName() )
    void CreateSimpleToolBarCtrl(HWND hWndParent, DWORD dwStyle = 0, UINT nID = ATL_IDW_TOOLBAR);

    typedef CCommandBarCtrlImpl<CBandToolBarCtrl, CCommandBarCtrlBase>	_baseClass;

     void OnSize(UINT wParam, CSize size);

  BEGIN_MSG_MAP_EX(CBandToolBarCtrl)
      REFLECTED_COMMAND_RANGE_HANDLER(WM_USER+0,WM_USER+MAX_ADD_COMMANDS, OnButton)
      COMMAND_RANGE_HANDLER(WM_MENU_USER,WM_MENU_USER+MAX_ADD_COMMANDS,OnCommand)
      REFLECTED_NOTIFY_CODE_HANDLER(TBN_DROPDOWN,   OnToolbarDropDown)
      DEFAULT_REFLECTION_HANDLER()
      MESSAGE_HANDLER(WM_MOUSEMOVE, OnHookMouseMove)
      MESSAGE_HANDLER(WM_LBUTTONDOWN, OnHookMouseMove)
      MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnHookMouseMove)

      MESSAGE_HANDLER(WM_MENUSELECT, OnMenuItem)
      MSG_WM_SIZE(OnSize)

  		//CHAIN_MSG_MAP( _baseClass )
		  MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
		  MESSAGE_HANDLER(WM_MEASUREITEM, OnMeasureItem)
      // to make hook and catch keyboard
		  MESSAGE_HANDLER(WM_CREATE, OnCreate)
		  MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
  		MESSAGE_HANDLER(GetGetBarMessage(), OnInternalGetBar)
		  MESSAGE_HANDLER(WM_FORWARDMSG, OnForwardMsg)

  ALT_MSG_MAP(1)
  		MESSAGE_HANDLER(GetGetBarMessage(), OnParentInternalGetBar)
      COMMAND_RANGE_HANDLER(WM_USER+0,WM_USER+MAX_ADD_COMMANDS, OnButton)
      NOTIFY_CODE_HANDLER(TBN_DROPDOWN,   OnToolbarDropDown)
      MESSAGE_HANDLER(WM_MOUSEMOVE, OnHookMouseMove) // other toolbars
      MESSAGE_HANDLER(WM_LBUTTONDOWN, OnHookMouseMove) // other toolbars
      //MSG_WM_ACTIVATE(OnInit)
  ALT_MSG_MAP(3)		// Message hook messages
    //MESSAGE_HANDLER(WM_KEYUP, OnHookKeyDown)
    MESSAGE_HANDLER(WM_KEYDOWN, OnHookKeyDown)
	END_MSG_MAP()

  LRESULT OnMenuItem(UINT , WPARAM wParam, LPARAM lParam, BOOL& ) {return CPugiObjWrap::OnMenuItem(wParam);}  

  LRESULT OnHookKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);

  void  OnTimer(UINT wParam, TIMERPROC lParam);

  struct _MenuItemData2 : CCommandBarCtrlImpl<CBandToolBarCtrl, CCommandBarCtrlBase>::_MenuItemData
  {
    _MenuItemData2() {MenuItems.push_back(this);}
    static vectorDel<_MenuItemData2*>   MenuItems;
  };

  void DrawItem3D(LPDRAWITEMSTRUCT lpDrawItemStruct)
  {
		_MenuItemData* pmd = (_MenuItemData*)lpDrawItemStruct->itemData;
		CDCHandle dc = lpDrawItemStruct->hDC;
		const RECT& rcItem = lpDrawItemStruct->rcItem;
		CBandToolBarCtrl* pT = static_cast<CBandToolBarCtrl*>(this);

		if(pmd->fType & MFT_SEPARATOR)
		{
			// draw separator
			RECT rc = rcItem;
			rc.top += (rc.bottom - rc.top) / 2;	// vertical center
			dc.DrawEdge(&rc, EDGE_ETCHED, BF_TOP);	// draw separator line
		}
		else		// not a separator
		{
			BOOL bDisabled = lpDrawItemStruct->itemState & ODS_GRAYED;
			BOOL bSelected = lpDrawItemStruct->itemState & ODS_SELECTED;
			BOOL bChecked = lpDrawItemStruct->itemState & ODS_CHECKED;
			BOOL bHasImage = FALSE;

			if(LOWORD(lpDrawItemStruct->itemID) == (WORD)-1)
				bSelected = FALSE;
			RECT rcButn = { rcItem.left, rcItem.top, rcItem.left + m_szButton.cx, rcItem.top + m_szButton.cy };			// button rect
			::OffsetRect(&rcButn, 0, ((rcItem.bottom - rcItem.top) - (rcButn.bottom - rcButn.top)) / 2);	// center vertically

			int iButton = pmd->iButton;
			if(iButton >= 0)
			{
				bHasImage = TRUE;

				// calc drawing point
				SIZE sz = { rcButn.right - rcButn.left - m_szBitmap.cx, rcButn.bottom - rcButn.top - m_szBitmap.cy };
				sz.cx /= 2;
				sz.cy /= 2;
				POINT point = { rcButn.left + sz.cx, rcButn.top + sz.cy };

				// draw disabled or normal
				if(!bDisabled)
				{
					// normal - fill background depending on state
					if(!bChecked || bSelected)
					{
						dc.FillRect(&rcButn, (bChecked && !bSelected) ? COLOR_3DLIGHT : COLOR_MENU);
					}
					else
					{
						COLORREF crTxt = dc.SetTextColor(::GetSysColor(COLOR_BTNFACE));
						COLORREF crBk = dc.SetBkColor(::GetSysColor(COLOR_BTNHILIGHT));
						CBrush hbr(CDCHandle::GetHalftoneBrush());
						dc.SetBrushOrg(rcButn.left, rcButn.top);
						dc.FillRect(&rcButn, hbr);
						dc.SetTextColor(crTxt);
						dc.SetBkColor(crBk);
					}

					// draw pushed-in or popped-out edge
					if(bSelected || bChecked)
					{
						RECT rc2 = rcButn;
						dc.DrawEdge(&rc2, bChecked ? BDR_SUNKENOUTER : BDR_RAISEDINNER, BF_RECT);
            if(list2.m_hImageList)
					      list2.Draw(dc,iButton, point, ILD_TRANSPARENT);
					}
          else
          {
					  // draw the image
					  ::ImageList_Draw(m_hImageList, iButton, dc, point.x, point.y, ILD_TRANSPARENT);
          }
				}
				else
				{
					HBRUSH hBrushBackground = ::GetSysColorBrush(COLOR_MENU);
					pT->DrawBitmapDisabled(dc, iButton, point, hBrushBackground);
				}
			}
			else
			{
				// no image - look for custom checked/unchecked bitmaps
				CMenuItemInfo info;
				info.fMask = MIIM_CHECKMARKS | MIIM_TYPE;
				::GetMenuItemInfo((HMENU)lpDrawItemStruct->hwndItem, lpDrawItemStruct->itemID, MF_BYCOMMAND, &info);
				if(bChecked || info.hbmpUnchecked != NULL)
				{
					BOOL bRadio = ((info.fType & MFT_RADIOCHECK) != 0);
					bHasImage = pT->DrawCheckmark(dc, rcButn, bSelected, bDisabled, bRadio, bChecked ? info.hbmpChecked : info.hbmpUnchecked);
				}
			}

			// draw item text
			int cxButn = m_szButton.cx;
			COLORREF colorBG = ::GetSysColor(bSelected ? COLOR_HIGHLIGHT : COLOR_MENU);
			if(bSelected || lpDrawItemStruct->itemAction == ODA_SELECT)
			{
				RECT rcBG = rcItem;
				if(bHasImage)
					rcBG.left += cxButn + s_kcxGap;
				dc.FillRect(&rcBG, bSelected ? COLOR_HIGHLIGHT : COLOR_MENU);
			}

			// calc text rectangle and colors
			RECT rcText = rcItem;
			rcText.left += cxButn + s_kcxGap + s_kcxTextMargin;
			rcText.right -= cxButn;
			dc.SetBkMode(TRANSPARENT);
			COLORREF colorText = ::GetSysColor(bDisabled ?  (bSelected ? COLOR_GRAYTEXT : COLOR_3DSHADOW) : (bSelected ? COLOR_HIGHLIGHTTEXT : COLOR_MENUTEXT));

			// font already selected by Windows
			if(bDisabled && (!bSelected || colorText == colorBG))
			{
				// disabled - draw shadow text shifted down and right 1 pixel (unles selected)
				RECT rcDisabled = rcText;
				::OffsetRect(&rcDisabled, 1, 1);
				pT->DrawMenuText(dc, rcDisabled, pmd->lpstrText, ::GetSysColor(COLOR_3DHILIGHT));
			}
			pT->DrawMenuText(dc, rcText, pmd->lpstrText, colorText); // finally!
		}
  }


	LRESULT OnHookMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

  void OnMouseDown( DISPID id, VARIANT* pVarResult );
  void OnSelChange( DISPID id, VARIANT* pVarResult );

  void FindText(_bstr_t searchText, long lFlags = 0, BOOL bNNF =FALSE  /*(for internal use)*/);
  void FindText2(_bstr_t searchText, long lFlags , _bstr_t matchStyle, _bstr_t searchID = "2find");
  void ClearSearchResults(_bstr_t searchID = "2find");

  void FindText(IHTMLDocument2 *lpHtmlDocument,_bstr_t searchText, long lFlags =2, BOOL bNNF =FALSE  /*(for internal use)*/);
  void FindText2(IHTMLDocument2 *lpHtmlDocument,_bstr_t searchText, long lFlags /* =2 */, _bstr_t matchStyle, _bstr_t searchID = "2find");
  void ClearSearchResults(IHTMLDocument2 *lpHtmlDocument,_bstr_t searchID = "2find");

  void onOptionChange();

  void checkUpdate();
  bool checkFiles();

  int m_width;
  int m_start_width;
  int m_min_width;
  int m_nButtonCount;
  int m_lastPresedButton;

  virtual CBandToolBarCtrl* root() {return this;}

  CPugiObj* m_pBand;
  Toolbar* m_toolbar;

  HMODULE h_image;

  STDMETHOD(TranslateAcceleratorIO)(LPMSG pMsg);

  void              RunSearch();
  void              MoveFocusToHTML();
  IHTMLDocument2Ptr GetHtmlDocument();
  CFontHandle font;

  LRESULT OnToolbarDropDown(int , LPNMHDR pnmh, BOOL& bHandled);

  HBITMAP LoadImage(const zstring& str);

#ifdef _ODBC_DIRECT
  #include "Toolband7\ToolbandInc.h"
#endif

  void UpdateAutoWords();
  void EnableHighlight(BOOL e);
  void hideIfNeeded(int id,const zstring&);
  void calculateWidth();

  HANDLE m_ticker_id;
protected:

  LRESULT OnButton(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnCommand(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

protected:

    void UpdateWidth();
    bool parseXML(zstring& path);
};

// TBBUTTONINFO  helper
  struct TBInfo
  {
    TBBUTTONINFO m_tbi;
    CBandToolBarCtrl* m_ctrl;
    int m_id;
    TBInfo(CBandToolBarCtrl* ctrl,int id) : m_ctrl(ctrl),m_id(id) {m_tbi.cbSize = sizeof TBBUTTONINFO;m_tbi.dwMask = 0;}
    TBInfo& image(int image)   {m_tbi.iImage = image; m_tbi.dwMask|=TBIF_IMAGE;return *this;}
    TBInfo& state(int state)   {m_tbi.fsState = state;m_tbi.dwMask|=TBIF_STATE;return *this;}
    TBInfo& style(int style)   {m_tbi.fsStyle = style;m_tbi.dwMask|=TBIF_STYLE;return *this;}
    TBInfo& text (TCHAR* text) {m_tbi.pszText = text; m_tbi.dwMask|=TBIF_TEXT;return *this;}
    TBInfo& size (int size)    {m_tbi.cx = size;      m_tbi.dwMask|=TBIF_SIZE;return *this;}

    ~TBInfo() {m_ctrl->SetButtonInfo(WM_USER + m_id, &m_tbi);}
  };

#endif // !defined(AFX_BANDTOOLBARCTRL_H__DE3301F1_2EBB_4B03_8B41_51930CE14993__INCLUDED_)
