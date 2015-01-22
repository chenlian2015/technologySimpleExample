// CBandEditCtrl class


class ICBandEditCtrl
{
public:
  virtual ~ICBandEditCtrl() {}
  virtual void Create(CBandToolBarCtrl* ctrl) = 0;
  virtual bool IsFocus(HWND hwnd) = 0;
  virtual void SetCurSel(int idx) {}
  virtual void InsertString(int idx, const TCHAR* str) {}
  virtual void SetItemDataPtr(int idx,void *) {}
  virtual void ResetContent() {}
  virtual void ClearHistory() {}
  virtual void DestroyWindow() = 0;
  virtual void ShowWindow(bool ) = 0;
  virtual void SetFont(HFONT) = 0;
  virtual _bstr_t GetText() = 0;
  virtual void SetFocus2() = 0;
  virtual void SetFocus() = 0;
  virtual int FindStringExact(const TCHAR* str) {return 0;}
  virtual void SetWindowText(const TCHAR* str) = 0;
  virtual void SetEditSel(int s, int e) = 0;
  virtual void AddinHistoryAndUpdateWords(_bstr_t URL) {}
  virtual void MoveWindow(RECT* rc) = 0;
	STDMETHOD(TranslateAcceleratorIO)(LPMSG pMsg) = 0;
  zstring getName() {return m_combo->m_name;}
	CBandToolBarCtrl* m_toolbar;
  Combo* m_combo;
};

//class CBandEditCtrl : public CWindowImpl<CBandEditCtrl, CComboBox>,
//class CBandEditCtrl : public CWindowImpl<CBandEditCtrl, CEdit>,

template <class T>
class CBandEditCtrl : public CWindowImpl<CBandEditCtrl, T>,
                      public IDropTarget ,
                      public ICBandEditCtrl
{
public:	
  CBandEditCtrl() : m_cRef(0), m_edit2(NULL, this, 1),m_auto(true){m_toolbar = NULL;}
  virtual ~CBandEditCtrl() 
  {
    if(m_edit2.m_hWnd)
    {
      m_edit2.UnsubclassWindow();
      m_edit2.DestroyWindow();
    }
  }
  typedef CWindowImpl<CBandEditCtrl, T> baseClass;
  DECLARE_WND_SUPERCLASS(_T("2Find_ComboBox"), TEXT("COMBOBOX"))
protected:
    CContainedWindow m_edit2;
//private:
  bool IsFocus(HWND hwnd)
  {
    return hwnd==m_edit2 || hwnd==m_hWnd;
  }
  void MoveWindow(RECT* rc)
  {
    baseClass::MoveWindow(rc);
  }
  void SetEditSel(int s, int e)
  {
    baseClass::SetEditSel(s,e);
  }
  void SetWindowText(const TCHAR* str)
  {
    baseClass::SetWindowText(str);
  }
  void SetFocus()
  {
    baseClass::SetFocus();
  }
  void SetFocus2()
  {
    baseClass::SetFocus();
    baseClass::SetActiveWindow();
    baseClass::SetFocus();
  }
  void Create(CBandToolBarCtrl* ctrl)
  {
    m_combo->m_style = WS_VSCROLL |WS_CHILD|WS_VISIBLE|CBS_AUTOHSCROLL|WS_TABSTOP;
    
#ifdef _BIBLE
    if(ctrl->m_toolbar->m_ctlBandEdits.size()<2)
#endif
      m_combo->m_style |= (m_combo->m_values.empty() ? CBS_DROPDOWN : CBS_DROPDOWNLIST);
        
    if (!baseClass::Create(*ctrl, m_combo->m_rc, NULL, m_combo->m_style))//, WS_EX_CLIENTEDGE))
      return;

    HWND wnd;
    COMBOBOXINFO pcbi;
    pcbi.cbSize = sizeof COMBOBOXINFO;
    ::GetComboBoxInfo(*this,&pcbi);
    wnd = pcbi.hwndItem;

    //HWND hEdit = m_wndCombo.GetEditCtrl();
    //wnd = ::GetTopWindow(m_ctlBandEdit);

    //wnd = m_ctlBandEdit;

    if(wnd)
      m_edit2.SubclassWindow(wnd);

    LimitText(m_combo->m_limit);

    CToolTipCtrl toolTip = ctrl->GetToolTips();

    if(m_edit2.m_hWnd)
    {
        CToolInfo ti0(TTF_SUBCLASS, m_edit2.m_hWnd, 0, 0, const_cast<TCHAR*>(m_combo->m_hint.c_str()));
        toolTip.AddTool((LPTOOLINFO) &ti0);
    }

    RegisterDragDrop(m_hWnd,	(LPDROPTARGET)(this));

  }
  void ShowWindow(bool s)
  {
    baseClass::ShowWindow(s);
  }
  void SetFont(HFONT font)
  {
    baseClass::SetFont(font);
    if(m_edit2.m_hWnd)
      m_edit2.SetFont(font);
  }
  void DestroyWindow()
  {
    if(m_hWnd) baseClass::DestroyWindow();
  }
// Operations
	STDMETHOD(TranslateAcceleratorIO)(LPMSG pMsg)
  {
	  int nVirtKey = (int)(pMsg->wParam);
    if(nVirtKey==VK_TAB)
    	  return S_OK;
	  if (WM_KEYUP == pMsg->message && VK_RETURN == nVirtKey && pMsg->hwnd==m_edit2.m_hWnd)
	  {
	    Process();
		  return S_OK; 
	  }
		  else if (
          /*(WM_KEYUP == pMsg->message || WM_KEYDOWN == pMsg->message) && */	
          VK_BACK == nVirtKey ||
          (VK_END <= nVirtKey && VK_LEFT >= nVirtKey) || VK_RIGHT == nVirtKey ||
				  VK_DELETE == nVirtKey || ( (GetKeyState(VK_CONTROL)&0x80) && (nVirtKey=='C' || nVirtKey=='V' || nVirtKey=='X')))
	  {
      m_auto = false;
      TranslateMessage(pMsg);
		  DispatchMessage(pMsg);
      m_auto = true;
      if(WM_KEYDOWN == pMsg->message && VK_BACK == nVirtKey) 
      {
        m_auto = false;
      }
		  return S_OK;
	  }
	  else
		  return S_FALSE;
  }

    // Support Drag and Drop
	STDMETHODIMP QueryInterface(REFIID riid, VOID** ppv)
  {     
	  *ppv=NULL;      
	  if (IID_IUnknown==riid || IID_IDropTarget==riid)         
		  *ppv=this;      
	  if (NULL!=*ppv) 
    {         
		  ((LPUNKNOWN)*ppv)->AddRef();         
		  return NOERROR;         
	  }      
    return E_NOINTERFACE;
  }   
  STDMETHODIMP_(ULONG) AddRef(void) { return ++m_cRef;}  
	STDMETHODIMP_(ULONG) Release(void) {if (0!=--m_cRef)return m_cRef;delete this;return 0;}      

  STDMETHODIMP DragEnter(LPDATAOBJECT pDataObj , DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect)
  {     
	  m_pIDataObject=NULL;
    FORMATETC formatDetails;     
	  STGMEDIUM stmg;     

	  // Let's get the text from the "clipboard"
 	  formatDetails.cfFormat = CF_TEXT;
	  formatDetails.dwAspect = DVASPECT_CONTENT;
	  formatDetails.lindex = -1;
	  formatDetails.ptd = NULL;
	  formatDetails.tymed = TYMED_HGLOBAL;
	  if (SUCCEEDED(pDataObj->GetData(&formatDetails, &stmg)))
    {
      *pdwEffect=DROPEFFECT_COPY;
      m_pIDataObject=pDataObj;     
      m_pIDataObject->AddRef();      
    }
    return NOERROR;     
  }       
  STDMETHODIMP DragOver(DWORD, POINTL, LPDWORD pdwEffect)
  {     
    if (m_pIDataObject == NULL) 
      *pdwEffect=DROPEFFECT_NONE;         
    else 
      *pdwEffect=DROPEFFECT_COPY;
    
    return NOERROR;     
  }       
  
  STDMETHODIMP DragLeave()
  { 
    if(m_pIDataObject)
      m_pIDataObject->Release();
    return NOERROR;     
  }      
  STDMETHODIMP Drop(LPDATAOBJECT pDataObj, DWORD, POINTL, LPDWORD pdwEffect)
  {     
    *pdwEffect=DROPEFFECT_NONE;      
    if (m_pIDataObject == NULL)         
      return E_FAIL;
    
    DragLeave();      
    
    FORMATETC formatDetails;     
    STGMEDIUM stmg;     
    
    // Let's get the text from the "clipboard"
    formatDetails.cfFormat = CF_TEXT;
    formatDetails.dwAspect = DVASPECT_CONTENT;
    formatDetails.lindex = -1;
    formatDetails.ptd = NULL;
    formatDetails.tymed = TYMED_HGLOBAL;
    
    if (SUCCEEDED(pDataObj->GetData(&formatDetails, &stmg)))
    {	
      char* sClipboard = (char*)GlobalLock(stmg.hGlobal);
      ::SetWindowTextA(m_hWnd,sClipboard);
      
      if(isGetValue(_T("RunSearchDragAutomatically")))
        Process();
      
      GlobalUnlock(stmg.hGlobal);
      
      if (stmg.hGlobal != NULL) 
        ReleaseStgMedium(&stmg);         
    }
    
    *pdwEffect=DROPEFFECT_COPY;      
    return NOERROR;     
  } 
                       
	BEGIN_MSG_MAP_EX(CBandEditCtrl)
    REFLECTED_COMMAND_CODE_HANDLER(CBN_SETFOCUS, OnTVSetFocus)
    REFLECTED_COMMAND_CODE_HANDLER(CBN_KILLFOCUS, OnTVKillFocus)
	  MESSAGE_HANDLER_EX(WM_MOUSEWHEEL, OnMouseWheel)
  ALT_MSG_MAP(1)
		MESSAGE_HANDLER(WM_CHAR, OnChar)
  END_MSG_MAP()


  LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM){return 0;}
  LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
  {
		if(wParam != VK_RETURN)
			bHandled = FALSE;
    return 0;
  }

  LRESULT Process()
  {
#ifdef _BIBLE
    setValue(_T("SearchUsing"),m_combo->m_command=="Search" ? _T("2") : _T("0"));
#endif
    m_toolbar->RunSearch();
    AddinHistoryAndUpdateWords(GetText());
    return 0;
  }

  LPDATAOBJECT      m_pIDataObject;
 
  _bstr_t GetText()
  {
    vector<TCHAR> cvalue(1024);
    GetWindowText(&cvalue.front(),cvalue.size() );
    return &cvalue.front();
  }
  
  _bstr_t GetLBText(int idx)
  {
    vector<TCHAR> cvalue(1024);
    CComboBox::GetLBText(idx,&cvalue.front());
    return &cvalue.front();
  }
  LRESULT OnTVSetFocus(WORD, WORD pnmh, HWND,BOOL& )
  {
    if(m_toolbar && m_toolbar->m_pBand)
      m_toolbar->m_pBand->FocusChange(true);
    return 0;
  }
  LRESULT OnTVKillFocus(WORD, WORD pnmh, HWND,BOOL& hand)
  {
    if(m_toolbar && m_toolbar->m_pBand)
      //m_toolbar->m_pBand->FocusChange(false);
    hand = FALSE;
    return 0;
  }
protected:
	ULONG m_cRef;
  bool m_auto;
};

class CBandComboBoxCtrl : public CBandEditCtrl<CComboBox>
{
public:
	BEGIN_MSG_MAP_EX(CBandComboBoxCtrl)
    REFLECTED_COMMAND_CODE_HANDLER(CBN_EDITUPDATE, OnTVSelDate)
    REFLECTED_COMMAND_CODE_HANDLER(CBN_SELENDOK, OnTVSelChanged)
    REFLECTED_COMMAND_CODE_HANDLER(CBN_SETFOCUS, OnTVSetFocus)
    REFLECTED_COMMAND_CODE_HANDLER(CBN_KILLFOCUS, OnTVKillFocus)
	  MESSAGE_HANDLER_EX(WM_MOUSEWHEEL, OnMouseWheel)
  ALT_MSG_MAP(1)
		MESSAGE_HANDLER(WM_CHAR, OnChar)
  END_MSG_MAP()
private:
  void SetItemDataPtr(int idx,void *v)
  {
    baseClass::SetItemDataPtr(idx,v);
  }
  void InsertString(int idx, const TCHAR* str)
  {
    baseClass::InsertString(idx,str);
  }
  void ResetContent() 
  {
    baseClass::ResetContent();
  }
  void SetCurSel(int idx)
  {
    baseClass::SetCurSel(idx);
  }
  void AddinHistoryAndUpdateWords(_bstr_t URL)
  {
    if(std::basic_string<TCHAR>(URL).find_first_not_of(_T(" "))!=std::string::npos)
    {
      if(isGetValue(_T("KeepHistory")) && m_combo->m_values.empty())
      {
        int h = GetCount();
        int pos = CComboBox::FindStringExact(-1,URL);
        if(pos== CB_ERR) 
        {
          // not found
          if(h<m_combo->m_histmax)
          {
            // normal
            InsertString(0,URL);
          }
          else
          {
            // replace last
            DeleteString(h-1);
            InsertString(0,URL);
          }
        }
        else
        {
          // move to last
          DeleteString(pos);
          InsertString(0,URL);
        }

        {
          CRegKey keyAppID;
          keyAppID.Attach(GetAppRegistryKey());
          keyAppID.RecurseDeleteKey(getHistName(m_combo->m_name));
        }
        {
          CRegKey keyAppID;
          keyAppID.Create(GetAppRegistryKey(),getHistName(m_combo->m_name));
          for(int i=0;i<GetCount();i++)
          {
            keyAppID.SetValue(1,GetLBText(i));
          }
        }
        SetCurSel(0);
      }
    }

    m_toolbar->UpdateAutoWords();
  }
  void ClearHistory()
  {
    if (m_combo->m_style != CBS_DROPDOWNLIST) 
    {
      CRegKey keyAppID;
      keyAppID.Attach(GetAppRegistryKey());
      keyAppID.RecurseDeleteKey(getHistName(getName()));
      ResetContent();
    }
  }
  int FindStringExact(const TCHAR* str)
  {
    return CComboBox::FindStringExact(-1,str);
  }
  _bstr_t GetText()
  {
    vector<TCHAR> cvalue(1024);
    GetWindowText(&cvalue.front(),cvalue.size() );
    if(m_combo->m_style==CBS_DROPDOWNLIST)
    {
      zstring* s = (zstring*)GetItemDataPtr(CComboBox::FindStringExact(-1,&cvalue.front()));
      return s ? s->c_str() : &cvalue.front();
    }

    return &cvalue.front();
  }
  LRESULT OnTVSelDate(WORD, WORD pnmh, HWND,BOOL& )
  {
    // Get the text in the edit box
    _bstr_t str = GetText();
    int nLength = GetWindowTextLength();

    m_toolbar->UpdateAutoWords();

    // if we are not to auto update the text, get outta here
    if(!isGetValue(_T("AutoComplete")))
      return 0;

    if(!m_auto)
    {
      return 0;
    }

    // Currently selected range
    DWORD dwCurSel = GetEditSel();
    WORD dStart = LOWORD(dwCurSel);
    WORD dEnd   = HIWORD(dwCurSel);

    // Search for, and select in, and string in the combo box that is prefixed
    // by the text in the edit box
    if (SelectString(-1, str) == CB_ERR)
    {
      SetWindowText(str);		// No text selected, so restore what was there before
      if (dwCurSel != CB_ERR)
        SetEditSel(dStart, dEnd);	//restore cursor postion
    }

    // Set the text selection as the additional text that we have added
    if (dEnd < nLength && dwCurSel != CB_ERR)
      SetEditSel(dStart, dEnd);
    else
      SetEditSel(nLength, -1);
    return 0;
  }

  LRESULT OnTVSelChanged(WORD, WORD pnmh, HWND,BOOL& )
  {
    int iItem = GetCurSel();
    if(iItem>=0)
    {
      if(m_combo->m_values.empty())
      {
        m_edit2.SetWindowText(GetLBText(iItem));
        if(isGetValue(_T("RunSearchAutomatically")))
      	  Process();
      }
      else
      {
        if(onCombo(m_toolbar,iItem))
        {
          zstring s;
          s += char(iItem + '0');
          setValue(_T("SearchUsing"),s);
          SetCurSel(iItem);
        }
      }
      m_toolbar->UpdateAutoWords();
    }
    return 0;
  }
};
