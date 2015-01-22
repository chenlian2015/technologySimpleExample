// BandToolBarCtrl.cpp: implementation of the CBandToolBarCtrl class.
//
//////////////////////////////////////////////////////////////////////
// ============================================================================
// Used Sample by Rashid Thadha Aug 2001
// Modified by Mike Melnikov
// ============================================================================

// TO DO
// cool own draw buttons 
//http://www.codeproject.com/buttonctrl/cwinxpbuttonst.asp
//http://www.codeproject.com/buttonctrl/cxshadebutton.asp

//what's new 
// sample Context menu handler in HTML page
// source reorganased a little (as always) I tried to add more comments
// several fonts
// ticker
// lot of resource and memory leaks fixed
// show/hide mechanism : added parameter - now you can hide each button and each MenuItem!!
// fix status bar in menus
// you can add text into buttons with bitmaps (not icons)
// added encoding for search parameters
// added update dll mechanism
// added direct parameter to add search in list on options page
// Script tag - can execute java scripts
// it is 1:00 a.m. now  and I added small TBInfo struct in this project. look on it - you will like it 
// wordfindHint attribute - now you can see hints for auto-generated buttons, too
// added new mechanism to monitoring connection to WWW
// removed Scope attribute from Setting - implemented using CHECKBOX now
//I am sure I forgot about some other features I implemented :(

#include "stdafx.h"

#include <objbase.h>
#include <WinInet.h>
#include <time.h>
#include <tom.h>
#include <set>
#include <map>
#include <algorithm>	  

#include "ToolBand.h"
#include "BandToolBarCtrl.h"
#include "ToolBandObj.h"
#include "atlmisc.h"
#include "dhtmlEvent.h"
#include "utils.h"
#include "NamesInc.cpp"


// some addintional defines of new interfaces - can't find them in standard header  
_COM_SMARTPTR_TYPEDEF(IHTMLElement2, __uuidof(IHTMLElement2));
_COM_SMARTPTR_TYPEDEF(IHTMLInputElement, __uuidof(IHTMLInputElement));
_COM_SMARTPTR_TYPEDEF(IHTMLSelectElement, __uuidof(IHTMLSelectElement));
_COM_SMARTPTR_TYPEDEF(IHTMLDocument3, __uuidof(IHTMLDocument3));

bool ShowSecurityInfo(_bstr_t);
bool ToggleAutoLogin(zstring& loginUrl,zstring& loginParams,zstring& loginHeades);

// some defines that you can use to customize your toolbar
// put them in stdafx.h file
#define SHOW_DEBUG_MESSAGES 1        // show some warning messages describing problems 
//#define UPDATE_DLL_FROM_EXE 1      // process updating toolbar using exe setup from your server
//#define UPDATE_DLL 1               // process updating toolbar visiting install page
//#define SELECT_SEARCH_FROM_COMBO 1 // select search engine from combo box 
//#define SELECT_SEARCH_FROM_MENU 1  // use search engine defined using "Switch to" (command=..Web)  
//#define USE_RESOURCE_FILES 1       // use XML and bitmaps from resources
//#define CHECK_EACH_START 1         // check new version on each start of IE
//#define DONT_CHECK_LOCAL_FILES 1   // don't monitor CRC of local files
//#define DONT_CHECK_VERSION_FILE 1  // don't check was version file changed before updating
//#define BLUE_ON_OVER_TEXT 1        // change text in buttons to blue on mouse over event 
//#define USE_SIMPLE_MENU 1          // use menu without bitmaps (icons)
//#define USE_TICKER 1               // can use ticker control
//#define APPLY_BACKGROUND 1         // apply background to IE bar 
//#define NO_OPTIONS_PAGE 1          // don't have HTML based options page
//#define NO_FIND_BUTTTONS 1         // do not add buttons with text from combo boxes
//#define NO_HIGHTLIGHT 1            // don't have highlight feature
//#define ADD_CONTEXT_ITEM 1         // add sample Context menu handler in HTML page

//all defines started with "_" (instead of _DEBUG) was used in my old private projects 

#if defined(USE_RESOURCE_FILES) || defined(_ODBC_DIRECT)
int loadFlag = LR_DEFAULTCOLOR;
#else
int loadFlag = LR_LOADFROMFILE;
#endif

//some forward declarations
bool onCombo(CBandToolBarCtrl* ctrl,int& iItem);
DWORD GetCRC(_bstr_t& strFilePath);
enum {
  url_none = 0,
  url_normal = 1,
  url_unsafe = 2,
  url_reserved = 4
};
const zstring urlencode(const zstring& _s, int _flags = url_normal);

// some global variables :-(
_bstr_t gurl;
zstring g_last_url;

bool g_DO_HTML = false;

_variant_t varEmpty;

std::map<zstring,zstring> gLastSearchWords;

static zstring gAppendCommand;
static zstring gAppendCommandDisplay;

// type of index image
#if defined(_ODBC_DIRECT)
  typedef  zstring   image_index;
  std::set<zstring>  gImages;
#else
  typedef  int       image_index;
#endif

  // format of backgound for button icons
#if defined(_LOOKJET) || defined(_SEARCHHIT) || defined(_ADRRESSS_BAR_REDIRECT)
  int clr_format = CLR_NONE;
#elif defined(_ODBC_DIRECT) || defined(_MASTER_BAR)
  int clr_format = 0xff00ff;
#else
  int clr_format = CLR_DEFAULT;
#endif

  // some toolbar messages
#ifdef _GERMAN
const _bstr_t alertTitle = optionsTittle + _T(" Toolbar Warnung");
const _bstr_t corruptedStr = optionsTittle + " Toolbar kann keine Daten von " + serverpath + " erhalten.\nBitte eine Internet-Verbindung herstellen und den Browser neu starten.";
const _bstr_t uninstallMsg = "Sind Sie sicher, dass die " + optionsTittle + " Toolbar von Ihrem Computer entfernt werden soll?";
#else
const _bstr_t alertTitle = optionsTittle + _T(" Toolbar Alert");
const _bstr_t corruptedStr = optionsTittle + _T(" Toolbar can't retrive infomation from ") + serverpath + _T("\nPlease connect the internet and restart your browser.");
const _bstr_t uninstallMsg = _T("This will remove ") + optionsTittle + _T(" Toolbar from your computer! Are you sure?");
#endif

// some toolbar specific includes
#if defined (_ODBC_DIRECT) || defined(_MASTER_BAR)
#include "Toolband7\CPage.cpp"
#endif
#ifdef _PROXY_CHANGE_TOOLBAR
#include "Toolband26\Proxy.cpp"
#endif
#ifdef _ODBC_DIRECT
#include "PostInc.cpp"
#endif

// getting Company and Application keys
HKEY GetCompanyRegistryKey()
{
  CRegKey h;
  h.Create(HKEY_CURRENT_USER,_T("SOFTWARE\\")+optionsTittle);
  return h.Detach();
}

HKEY GetAppRegistryKey()
{
  CRegKey h;
  h.Create(GetCompanyRegistryKey(),sProfileName);
  return h.Detach();
}

int GetBitmap(image_index s)
{
#if defined(_ODBC_DIRECT)
  #include "GetBitmap.inl"
#else
  return s;
#endif
}

bool checkFile(zstring& strFile, bool update = false)
{
#ifdef USE_RESOURCE_FILES
  if(strFile.find(_T("bmp"))!=zstring::npos)
    return true;
#endif

  _bstr_t path = getprogpath(strFile.c_str());

  _bstr_t sstrFile = strFile.c_str();

#ifdef _ECOACH
  bool isXML = false;
  if(sstrFile == _bstr_t("ecoach.xml"))
  {
    isXML = true;
    zstring updateVer = getValue(zstring(_T("updateVer")),_T(""));
    sstrFile = updateVer.empty() ? _T("ecoach.asp") : updateVer.c_str();
    sstrFile += "?URL=" + gurl;
  }
#endif

#ifdef _2FIND
  const _bstr_t serverpath = getValue(_T("SecretPath"),_T("http://www.2find.co.il/toolbar/")).c_str();
#endif

  _bstr_t pathFromFolder = serverpath;
  _bstr_t pathFrom = pathFromFolder + sstrFile;
  _bstr_t pathTo = path;

  zstring oldcrc = getValue(strFile);

  DWORD crc = GetCRC(path);

  zstring newcrc = IntToStr(crc);

  bool download =
#ifndef DONT_CHECK_LOCAL_FILES
    oldcrc==_T("1") || oldcrc!=newcrc ||
#endif
    crc==0 || update || isGetValue2(_T("gUpdate"));

#ifdef _ECOACH
  if(strFile.find(_T(".bmp"))!=zstring::npos)
  {
    if(isGetValue2(_T("BeforeNavigate2")))
      download = false;
  }
#endif

  if(download)
  {
    bool canDown = true;

    HRESULT hr = 0;

    if(canDown || crc==0)
    {
#ifdef _ODBC_DIRECT
      #include "Toolband7\checkFile.cpp"
#else

      ///::MessageBox(0,_T("try to load file ")+sstrFile+_T(" from ")+pathFrom,alertTitle,0);

      hr = URLDownloadToFile( NULL,pathFrom,pathTo,0,0);
      CRegKey keyAppID;
      keyAppID.Create(GetAppRegistryKey(),getHistName(_T("files")));
      keyAppID.SetValue(1,pathTo);
#endif
    }
    else
    {
      if(crc==0)
      {
        hr = -1;
      }
      else
      {
        strFile = path;
        return true; // working offline
      }
    }

    if ( SUCCEEDED(hr) )
    {
    }
    else if(crc==0)
    {
//      if(!isGetValue2(_T("gUpdate")))
      {
#ifdef SHOW_DEBUG_MESSAGES
        ::MessageBox(0,_T("Can't load file ")+sstrFile+_T(" from ")+pathFrom,alertTitle,0);
#endif
        if(isGetValue(_T("showcorrupted")))
        {
         //::MessageBox(0,corruptedStr,0,0);
          setValue(_T("showcorrupted"),_T("0"));
        }
        return false;
      }
    }
    else
    {
      strFile = path;
      return true; // working offline
    }

  zstring oldnewcrc = newcrc;

  newcrc = IntToStr(GetCRC(path));
  setValue(strFile,newcrc);
#ifdef _ECOACH
  if(isXML && isGetValue2(zstring(_T("BeforeNavigate2"))) && oldnewcrc==newcrc)
     return false;
#endif
  }

  strFile = path;
  return true;
}

// in: CLSID-string of a IE-Bar element (toolbar or Pugi), and IWebBrowser interface
void ShowBandObject(IWebBrowser2 *pIE,const _bstr_t& pszBarCLSID,bool bShow)
{
  // Show
  _variant_t vtBandGUID = pszBarCLSID;
  _variant_t vtShow = bShow;
  pIE->ShowBrowserBar(&vtBandGUID, &vtShow, &varEmpty);
}
// in: CLSID-string of a IE-Bar element (toolbar or Pugi), and IWebBrowser interface
void ShowBandObject(IWebBrowser2 *pIE,const _bstr_t& pszBarCLSID, const _bstr_t url, bool bShow = true)
{
  ShowBandObject(pIE,pszBarCLSID,bShow);
  _variant_t varURL = url;
  pIE->Navigate2(&varURL, &varEmpty, &varEmpty, &varEmpty, &varEmpty);
}

// set this toolbar to show on next IE start (should be called when user disable our toolbar from list)
void __cdecl ReEnable(void* lpParam)
{
    Sleep(200);
    CoInitialize(NULL);
    IWebBrowser2 *pIE = RunIe(false);
    ShowBandObject(pIE,bandUUID,true);
    pIE->Release();
    CoUninitialize();
}

// register (remove) toolbar from IE toolbars list
void addKey(bool a)
{
  HKEY hKeyLocal = NULL;
  RegCreateKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Internet Explorer\\Toolbar"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKeyLocal, NULL);

  CRegKey keydel;
  keydel.Open(HKEY_CURRENT_USER,_T("Software\\Microsoft\\Windows\\CurrentVersion\\Runonce"));
  if(a)
    keydel.DeleteValue(_T("Delete")+optionsTittle);
  else
    keydel.SetValue(_T("rundll32.exe advpack.dll,DelNodeRunDLL32 \"")+getdllpath()+ _T("\""),_T("Delete")+optionsTittle);


#ifdef _MASTER_BAR
  #include "Toolband27\DisableOtherToolbars.inl"
#endif

  if(a)
    RegSetValueEx(hKeyLocal, bandUUID, 0, REG_BINARY, NULL, 0);
  else
    RegDeleteValue(hKeyLocal, bandUUID);
  RegCloseKey(hKeyLocal);

  RegCreateKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\")+optionsTittle+sProfileName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKeyLocal, NULL);
  if(a)
  {
    _bstr_t s =  optionsTittle + _T(" - Toolbar");
#if defined(_ODBC_DIRECT)
    s = "OBCdirect Business Navigator Browser";
#endif

    RegSetValueEx(hKeyLocal, _T("DisplayName"), 0, REG_SZ, (BYTE*)(const TCHAR*)s, sizeof(TCHAR)*s.length());
    _bstr_t s1 = "regsvr32 /u /s \"" + getdllpath() +"\" ";
    RegSetValueEx(hKeyLocal, _T("UninstallString"), 0, REG_SZ, (BYTE*)(const TCHAR*)s1, sizeof(TCHAR)*s1.length());
  }
  else
  {
    RegDeleteValue(hKeyLocal, _T("DisplayName"));
    RegDeleteValue(hKeyLocal, _T("UninstallString"));
  }
  RegCloseKey(hKeyLocal);
}

// called on dll register - process all first time registration
void ShowBand()
{
  addKey(true);

  setValue(_T("gUpdate"),_T("1"));

#if !defined(_ODBC_DIRECT)
  setValue(_T("NID"),getValue(_T("NID"),createGUID()));
#endif

  setValue(_T("toolbar_id"),getValue(_T("toolbar_id"),createGUID()));

#ifdef _PRAIZE
  #include "Toolband8\addButtonToStandartToollbar.inl"
#endif
#ifdef _MASTER_BAR
  #include "Toolband27\OnShow.inl"
#endif

  // open new IE instance and "select" our toolbar from toolbar's list
#ifndef _DEBUG
  bool wasupdate = getValue(zstring(_T("UpdateBegin")),_T("0"))==zstring(_T("1"));

#ifdef UPDATE_DLL_FROM_EXE
  if(wasupdate)
    DeleteFile(exeName);
#endif

  ShowBandObject(RunIe(),bandUUID, wasupdate ? urlAfterUpdate : firstURL ,true);

  setValue(_T("UpdateBegin"),_T("0"));
#endif

  setValue(_T("gUpdate"),_T("0"));
}

// replace some common templates in urls
_bstr_t replaceUrl(const zstring& ss)
{
  zstring s = ss;
  replace(s,_T("%url"),gurl);

  replace(s,_T("%id"),getValue(_T("toolbar_id"),_T("0")));
  replace(s,_T("%nid"),getValue(_T("NID"),_T("")));
  replace(s,_T("%pass"),getValue(_T("Pass"),_T("")));
  replace(s,_T("%CustID"),getValue(_T("CustID"),_T("")));
  
  COLORREF color = GetSysColor(COLOR_3DFACE);
  char str[256];
  sprintf(str,"%06X",color);
  replace(s,_T("%bgc"),_bstr_t(str));

  const zstring p[] = {_T("10"),_T("15"),_T("20"),_T("25"),_T("30"),_T("40"),_T("50")};
  zstring size = getValue(_T("PageSize"),_T("1")).c_str();
  replace(s,_T("%p"),p[size[0]-'0']);
  return s.c_str();
}

// called on unregister of dll - delete all files and registry entries from computer
void UNregServer()
{
  addKey(false);

  for(RegIterator it(_T("files"));it;it++)
    DeleteFile(it);

#ifndef _2FIND
  CRegKey keyAppID;
  keyAppID.Attach(GetCompanyRegistryKey());
  keyAppID.RecurseDeleteKey(sProfileName);
#endif

  ShowBandObject(RunIe(),bandUUID,replaceUrl((const char*)urlAfterUninstall), false);
}

// heart of this project
// Toolbar class
// represent XML structure and process most work 

struct Toolbar : public XmlObject
{
  ~Toolbar() {}
  
  // Settings class - collection of settings that appear in Options page
  struct Settings : public XmlObject
  {
    Settings() {}
    zstring  m_updateVer;
    zstring  m_wordfindHint;

    // fonst used in toolbar
    struct Font : public XmlObject
    {
      int     m_size;
      zstring m_name;
      virtual void determine(XmlParserProxy& p)
      {
        XML::determineMember<>(m_size,_T("size"),p);
        XML::determineMember<>(m_name,_T("name"),p);
      }
    };
    XML::vectorDel<Font*>  m_fonts;

    // colors for HightLighting
    struct Highlight : public XmlObject
    {
      vector<zstring> m_colors;
      virtual void determine(XmlParserProxy& p)
      {
        XML::determineMember<zstring>(m_colors,_T("COLOR"),p);
      }
    }m_highlight;

    zstring  m_popuponload;

    // store setting to show popup messages
    struct Popup : public XmlObject
    {
      Popup() : m_time(0) {}
      zstring m_url;
      int     m_width;
      int     m_height;
      zstring m_show;
      int     m_time;
      virtual void determine(XmlParserProxy& p)
      {
        XML::determineMember<zstring>(m_url,_T("url"),p);
        XML::determineMember<zstring>(m_height,_T("height"),p);
        XML::determineMember<zstring>(m_width,_T("width"),p);
        XML::determineMember<zstring>(m_show,_T("Show"),p);
        XML::determineMember<zstring>(m_time,_T("Time"),p);
      }
    }m_popup;

    void init()
    {
      for(unsigned int j=0;j<m_settings.size();j++)
        m_settings[j]->init();
    }

    // Setting class - parent for all objects that can be used in Options page
    struct Setting : public XmlObject
    {
      zstring  m_id;
      zstring  m_default;
      void init() { setValue(m_id,get());}
      zstring get() { return getValue(m_id,m_default.size() ? m_default : _T("1"));}
      virtual void onOpen(CBandToolBarCtrl*ctrl, IHTMLDocument3* pHtmlDoc3) = 0;
      virtual void onSelChange(IHTMLElementPtr p,zstring id) { };
      virtual void onClick(CBandToolBarCtrl*ctrl,zstring id) { };
      virtual void determine(XmlParserProxy& p)
      {
        XML::determineMember<>(m_id,_T("id"),p);
        XML::determineMember<>(m_default,_T("default"),p);
      }
    };

    // check box in options page, also can set some default parameters for toolbar 
    struct CheckBox : public Setting
    {
      virtual void onOpen(CBandToolBarCtrl*ctrl,IHTMLDocument3* pHtmlDoc3)
      {
        if(get()==zstring(_T("1")))
        {
          IHTMLElementPtr el;
          pHtmlDoc3->getElementById(_bstr_t(m_id.c_str()),&el);
          if(el)
          {
            if(IHTMLInputElementPtr i = el)
              i->put_checked(VARIANT_TRUE);
          }
        }
      }
      virtual void onClick(CBandToolBarCtrl*ctrl,zstring id)
      {
        if(id==m_id)
          setValue(m_id,get()==zstring(_T("1")) ? _T("0") : _T("1") );
      }
    };

#ifndef NO_OPTIONS_PAGE
    // we can put some buttons in HTML page and assign some command to it
    struct Button : public Setting
    {
      zstring m_command;
      virtual void onOpen(CBandToolBarCtrl*ctrl, IHTMLDocument3* pHtmlDoc3) {}
      virtual void onClick(CBandToolBarCtrl*ctrl,zstring id)
      {
        if(id==m_id)
        {
          if(Commands::Command* c = ctrl->m_toolbar->selectCommand(m_command))
            c->apply(ctrl);
        }
      }
      virtual void determine(XmlParserProxy& p)
      {
        XML::determineMember<>(m_command,_T("command"),p);
        Setting::determine(p);
      }
    };

    // radio control on option page
    struct Radio : public Setting
    {
      zstring m_name;
      vector<zstring>  m_ids;
      virtual void onOpen(CBandToolBarCtrl*ctrl,IHTMLDocument3* pHtmlDoc3)
      {
        IHTMLElementPtr el;
        for(unsigned int i=0;i<m_ids.size();i++)
        {
          pHtmlDoc3->getElementById(_bstr_t(m_ids[i].c_str()),&el);
          if(el!=NULL && getValue(m_name,m_default)==zstring(IntToStr(i)))
            el->click();
        }
      }
      virtual void onClick(CBandToolBarCtrl*ctrl,zstring id)
      {
        for(unsigned int i=0;i<m_ids.size();i++)
          if(m_ids[i]==id)
            setValue(m_name,zstring(IntToStr(i)));
      }
      virtual void determine(XmlParserProxy& p)
      {
        XML::determineMember<zstring>(m_ids,_T("id"),p);
        XML::determineMember<>(m_name,_T("name"),p);
        Setting::determine(p);
      }
    };
    // combo box control in options page - used only to seleect default search engine
    struct Select : public Setting
    {
      virtual void onOpen(CBandToolBarCtrl*ctrl, IHTMLDocument3* pHtmlDoc3)
      {
        IHTMLElementPtr e3 = NULL;
        pHtmlDoc3->getElementById(_bstr_t(m_id.c_str()),&e3);
        if(e3)
        {
          IHTMLDocument2Ptr pHtmlDoc2 = pHtmlDoc3;
          IHTMLSelectElementPtr e = e3;
          if(e)
          {
            // register handler
            e->put_onchange(_variant_t(CHtmlEventObject<CBandToolBarCtrl>::CreateHandler(ctrl, CBandToolBarCtrl::OnSelChange, 1 )));

            for(unsigned int b=0;b<ctrl->m_toolbar->m_toolbarItems.size();b++)
            {
                zstring command = ctrl->m_toolbar->m_toolbarItems[b]->m_command;
                if(Commands::Command* c = ctrl->m_toolbar->selectCommand(command))
                {
                  if(c->canAddToSelect())
                  {
                    IHTMLElementPtr element;
                    pHtmlDoc2->createElement(_bstr_t(_T("OPTION")),&element);
                    IHTMLOptionElementPtr option = element;
                    option->put_text(_bstr_t(ctrl->m_toolbar->m_toolbarItems[b]->m_caption.c_str()));
                    e->add(element,_variant_t(long(-1)));
                  }
                }
            }

            zstring ss = get();
            long ind = ss[0] - '0';
            e->put_selectedIndex(ind);
          }
        }
      }
      virtual void onSelChange(IHTMLElementPtr p,zstring id)
      {
         IHTMLSelectElementPtr s = p;
         long ind;
         s->get_selectedIndex(&ind);
         TCHAR ss[] = _T("0");
         ss[0] += static_cast<TCHAR>(ind);
         setValue(m_id,ss);
      }
    };
#endif

    virtual void determine(XmlParserProxy& p)
    {
      XML::determineMember<>(m_updateVer,_T("updateVer"),p);
      XML::determineMember<>(m_wordfindHint,_T("WordFindHint"),p);
      XML::determineMember<Font>(m_fonts,_T("FONT"),p);
      XML::determineMember<>(m_highlight,_T("HIGHLIGHT"),p);
      XML::determineMember<>(m_popuponload,_T("POPUPONLOAD"),p);
      XML::determineMember<>(m_popup,_T("POPUP"),p);
      XML::determineMember<CheckBox>(m_settings,_T("CHECKBOX"),p);
#ifndef NO_OPTIONS_PAGE  
      XML::determineMember<Radio>(m_settings,_T("RADIO"),p);
      XML::determineMember<Select>(m_settings,_T("SELECT"),p);
      XML::determineMember<Button>(m_settings,_T("BUTTON"),p);
#endif
    }

    XML::vectorDel<Setting*> m_settings;

  }m_settings;

  // parent for all menu items in toolbar's menus
  struct IMenuItem  : public XmlObject
  {
    zstring m_hint;
    zstring m_command;
    zstring m_vis_id; // id of visiable "group"
    virtual void appendMenu(CMenuHandle,int& ,int) = 0;   // all this popup menu (if any)
    virtual IMenuItem* getItem(int& items,int c) = 0;     // get child menu item by position
    bool shouldShow() {return m_vis_id.empty() || isGetValue(m_vis_id);}
  };

  // vector of all commands that can our toolbar execute (may be make it a map?)
  struct Commands : public XmlObject
  {
    // parent of all command classes
    struct Command : public XmlObject
    {
      zstring m_name;
      virtual bool check() {return true;}              // load (and check) additional files (if any)
      virtual bool canAddToSelect() {return false;}    // is it search "engine command"?
      virtual void apply(CBandToolBarCtrl* ) = 0;      // execute this command
      virtual void applyPresed(CBandToolBarCtrl* ctrl) {} // execute this command after button (hightlight) was pressed
      virtual void determine(XmlParserProxy& p)
      {
        XML::determineMember<>(m_name,_T("name"),p);
      }
    };

    // all "speacial" types of Commans (uninstall, highlight, etc.)
    struct Special : public Command
    {
      zstring m_type;
      zstring m_display;

      virtual void determine(XmlParserProxy& p)
      {
        Command::determine(p);
        XML::determineMember<>(m_type,_T("type"),p);
        XML::determineMember<>(m_display,_T("display"),p);
      }

      virtual void apply(CBandToolBarCtrl* ctrl)
      {
        if(m_type==_T("Hide toolbar"))
        {
          ShowBandObject(ctrl->m_pBand->m_pWebBrowser,bandUUID,false);
        }
        else if(m_type==_T("Message"))
        {
          ::MessageBox(0,_T("Searchit IE Toolbar 1.0"),_T("Searchit IE Toolbar 1.0"),MB_OKCANCEL);
        }
        else if(m_type==_T("Search"))
        {
          ctrl->RunSearch();
        }
        else if(m_type==_T("ClearHistory"))
        {
          CRegKey keyAppID;
          keyAppID.Attach(GetAppRegistryKey());
          keyAppID.RecurseDeleteKey(_T("History1"));
          keyAppID.RecurseDeleteKey(_T("History2"));
          for(Toolbar::IBandEdits it(ctrl);it;it++)
            it->ClearHistory();
        }
        else if(m_type==_T("Uninstall"))
        {
          if(MessageBox(0,uninstallMsg,alertTitle,MB_YESNO)==IDYES)
          {
            ctrl->m_pBand->m_pWebBrowser->Quit();
            DllUnregisterServer();
          }
        }
        else if(m_type==_T("Update"))
        {
          setValue(_T("gUpdate"),_T("1"));
          ctrl->checkUpdate();
          IWebBrowser2 *pIE = ctrl->m_pBand->m_pWebBrowser;
          //CloseAllRuningIE();
          //IWebBrowser2 *pIE = 0;
          //if(RunIe(pIE))
          {
            _variant_t varURL = urlAfterUpdate;
#ifdef _2FIND
            zstring verFile = _T("version.txt");
            zstring oldCRC = getValue(verFile);
            if(checkFile(verFile,true))
            {
              zstring newCRC = getValue(verFile);
              if(oldCRC==newCRC)
                varURL  = urlAfterUpdate2;
            }
#endif
            pIE->Navigate2(&varURL, &varEmpty, &varEmpty, &varEmpty, &varEmpty);
          }
        }
        else if(m_type==_T("ShowDialog"))
        {
#ifdef _ODBC_DIRECT
#include "Toolband7\ShowDialog.cpp"
#endif
#ifdef _SITE_SAFE
#include "Toolband3\ShowDialog.cpp"
#endif
        }
        else if(m_type==_T("Make Default Toolbar"))
        {
          setValue(_T("Toolbar Extension"),gAppendCommand);
          setValue("Toolbar Extension Display",gAppendCommandDisplay);
        }
#ifdef _MICROSOLUTIONS
  #include "Toolband22\popup.cpp"
#endif
        else if(m_type==_T("Switch to"))
        {
          gAppendCommand = m_name;
          for(Toolbar::IBandEdits it(ctrl);it;it++)
          {
            gLastSearchWords[it->getName()] = it->GetText();
          }
#ifdef _GERMAN
 #include "Toolband21\SwitchTo.cpp"
#endif
          ctrl->onOptionChange();
          if(ToolbarItem* item = ctrl->m_toolbar->Find2("search"))
          {
            item->m_caption = m_display;
            gAppendCommandDisplay = m_display;
          }
          ctrl->checkUpdate();
        }
#ifdef _ODBC_DIRECT
 #include "Toolband7\startAutoLogin.cpp"
#endif
      }
    };
    // execute java script on current page
    struct Script : public Command
    {
      zstring m_body;
      virtual void apply(CBandToolBarCtrl* ctrl)
      {
          IHTMLDocument2Ptr pHtmlDoc = ctrl->GetHtmlDocument();
          if(pHtmlDoc)
          {
            CComPtr<IDispatch> spDisp;
      		  pHtmlDoc->get_Script( &spDisp );
			      CComQIPtr<IHTMLWindow2> spWin = spDisp;
            VARIANT pvarRet;
            spWin->execScript(_bstr_t(m_body.c_str()),_bstr_t("JavaScript"),&pvarRet);
          }
      }
      virtual void determine(XmlParserProxy& p)
      {
        Command::determine(p);
        XML::determineMember<>(m_body,_T("body"),p);
      }
    };

    // open web URL in browser
    struct WebJump : public Command
    {
      zstring m_href;
      zstring m_newbrowserwindow;
      zstring m_post;
      zstring m_target;
      zstring m_options;
      zstring m_addtoselect;

      // I don't know better solution now
      virtual bool canAddToSelect() {return m_addtoselect==_T("yes");}

      virtual void apply(CBandToolBarCtrl* ctrl) { do_apply(ctrl,m_href); }
      void do_apply(CBandToolBarCtrl* ctrl,zstring s)
      {
        IHTMLDocument2Ptr pHtmlDoc = ctrl->GetHtmlDocument();
        zstring post = m_post;

        for(Toolbar::IBandEdits it(ctrl);it;it++)
        {
           gLastSearchWords[it->getName()] = it->GetText(); // GetText knows about CBS_DROPDOWNLIST style
           zstring strEdit = replaceSpace((urlencode(zstring(it->GetText()))),vector<zstring>());
#ifdef _BIBLE
           if(it->m_combo->m_name==zstring("book"))
             strEdit = IntToStr(it->FindStringExact(strEdit.c_str())+1);
#endif
           zstring mask = _T("%")+it->getName();

           if((replace(s,mask,strEdit) || replace(post,mask,strEdit)) && it->m_combo->m_style!=CBS_DROPDOWNLIST)
             it->AddinHistoryAndUpdateWords(it->GetText());
        }

        CComBSTR  url;
        CComBSTR  domain;
        CComBSTR  title;
        if(pHtmlDoc)
        {
          if(pHtmlDoc->get_URL(&url)==0)
            gurl = urlencode(zstring(_bstr_t(url))).c_str();
          pHtmlDoc->get_domain(&domain);
          pHtmlDoc->get_title(&title);
        }

        replace(s,_T("%d"),_bstr_t(domain));
        replace(s,_T("%title"),_bstr_t(title));
        
        _variant_t varURL = replaceUrl(s);
        _variant_t postData = getPostData((const TCHAR*)replaceUrl(post));
        _variant_t varHeaders = _T("");

#ifdef _DYNABAR
        varHeaders = "Referer: DynaBar toolbar";
#endif

        if(m_newbrowserwindow==_T("true") ||
          (m_newbrowserwindow.empty() && isGetValue2(_T("OpenNew"))) )
        {
          IWebBrowser2 *pIE = RunIe();
          pIE->Navigate2(&varURL, &varEmpty, &varEmpty, &postData, &varHeaders);
          //long wnd;
          //pIE->get_HWND((long*)&wnd);
          //SetFocus(wnd);
        }
        else
        {
          if(m_newbrowserwindow==_T("modaldialog"))
          {
            IHTMLDocument2Ptr pHtmlDoc = ctrl->GetHtmlDocument();
            if(pHtmlDoc)
            {
              IHTMLWindow2Ptr win = 0;
              pHtmlDoc->get_parentWindow(&win);
              if(win)
                win->showModalDialog(_bstr_t(varURL),&_variant_t(""),&_variant_t(m_options.c_str()),&varEmpty);
            }
          }
          // show HTMl page that can have a lot of "our" page links
          else if(m_newbrowserwindow==_T("toolbarwindow"))
          {
#if defined(_ALEXA) || defined (_TYCOON) || defined (_ESTELLEREYNA)
            CHtmlPage* dlg = new CHtmlPage;
            dlg->m_parentBrowser = ctrl->m_pBand->m_pWebBrowser;
            dlg->m_url = varURL;
            dlg->Create(*ctrl);
            dlg->ShowWindow(SW_SHOW);
            dlg->m_wasShow = true;

            CRect rc,rc2;
            ctrl->GetItemRect(ctrl->m_lastPresedButton, rc);
            ctrl->ClientToScreen(rc);
            dlg->GetWindowRect(&rc2);
            dlg->MoveWindow(rc.left,rc.bottom,rc2.Width(),rc2.Height());
#endif
          }
          else
          {
            _variant_t target = m_target.c_str();
            ctrl->m_pBand->m_pWebBrowser->Navigate2(&varURL, &varEmpty, &target, &postData, &varHeaders);
          }
        }
      }
      virtual void determine(XmlParserProxy& p)
      {
        Command::determine(p);
        XML::determineMember<>(m_href,_T("href"),p);
        XML::determineMember<>(m_post,_T("post"),p);
        XML::determineMember<>(m_target,_T("target"),p);
        XML::determineMember<>(m_options,_T("options"),p);
        XML::determineMember<>(m_newbrowserwindow,_T("newbrowserwindow"),p);
        XML::determineMember<>(m_addtoselect,_T("addtoselect"),p);
      }
    };
    // open previous loaded HTML page
    struct LocalHTML : public WebJump
    {
      zstring m_ref_path;
      virtual bool check()
      {
        m_ref_path = m_href;
        if(m_newbrowserwindow==_T("toolbarwindow"))
          return true;
        return checkFile(m_ref_path)!=0;
      }
      virtual void apply(CBandToolBarCtrl* ctrl)
      {
        if(check())
          do_apply(ctrl,m_ref_path);
      }
    };

    // execute some local executable or some shell command (create email)
    struct ShellExecute : public Command
    {
      zstring m_command;
      zstring m_not_found;

      virtual void apply(CBandToolBarCtrl* ctrl)
      {
        HINSTANCE ret = ::ShellExecute(NULL,NULL,m_command.c_str(),NULL,NULL,SW_SHOWNORMAL);
        if(int(ret)<32)
          MessageBox(0,m_not_found.c_str(),alertTitle,0);
      }
      virtual void determine(XmlParserProxy& p)
      {
        Command::determine(p);
        XML::determineMember<>(m_command,_T("command"),p);
        XML::determineMember<>(m_not_found,_T("not_found"),p);
      }
    };

#ifndef NO_HIGHTLIGHT
    // process ala Google Highlight
    struct Highlight : public Command
    {
      virtual void applyPresed(CBandToolBarCtrl* ctrl)
      {
        for(unsigned int c=0;c<ctrl->m_toolbar->m_settings.m_highlight.m_colors.size();c++)
        {
          ctrl->ClearSearchResults();
        }
        ctrl->CheckButton(WM_USER+ctrl->m_toolbar->Find(_T("Highlight")),FALSE);
      }
      virtual void apply(CBandToolBarCtrl* ctrl)
      {
        applyPresed(ctrl);

        zstring str0;
        for(Toolbar::IBandEdits it(ctrl);it;it++)
          str0 += it->GetText() + " ";
        // Get the text in the edit box
        std::vector<zstring> v;
        replaceSpace(str0,v);

        for(int id=0;id<v.size();id++)
        {
          zstring val = v[id];
          replace(val,_T("\""),zstring(_T("")));
          ctrl->FindText2(val.c_str(),tomMatchWord,getColor(ctrl,id % ctrl->m_toolbar->m_settings.m_highlight.m_colors.size()));
        }
        ctrl->MoveFocusToHTML();

        ctrl->CheckButton(WM_USER+ctrl->m_toolbar->Find(_T("Highlight")),TRUE);
      }
      _bstr_t getColor(CBandToolBarCtrl* ctrl,int c)
      {
        return (_T("color:#000000;background-color:") + ctrl->m_toolbar->m_settings.m_highlight.m_colors[c] + _T(";")).c_str();
      }
    };
#endif

    virtual void determine(XmlParserProxy& p)
    {
      XML::determineMember<WebJump>(m_commands,_T("WEBJUMP"),p);
      XML::determineMember<Script>(m_commands,_T("SCRIPT"),p);
      XML::determineMember<Special>(m_commands,_T("SPECIAL"),p);
      XML::determineMember<LocalHTML>(m_commands,_T("LOCALHTML"),p);
      XML::determineMember<ShellExecute>(m_commands,_T("SHELLEXECUTE"),p);
#if !defined(NO_HIGHTLIGHT)
      XML::determineMember<Highlight>(m_commands,_T("HIGHLIGHT"),p);
#endif
    }
    XML::vectorDel<Command*> m_commands;
  }m_commands;

  // parent of all Toolbar's controls
  struct ToolbarItem : public XmlObject
  {
    ToolbarItem() : m_id(-2),m_command_id(0) {}
    int                m_id;
    Commands::Command* m_command_id;
    zstring            m_hint;
    zstring            m_caption;
    zstring            m_command;
    zstring            m_vis_id; // id of visiable "group"
    zstring& getName()    {return m_caption;}
    zstring& getToolTip() {return m_hint;}

    virtual void onCommand(CBandToolBarCtrl* ) {}   // button pressed
    virtual void Invalidate(CBandToolBarCtrl* ) {}  // invalidate owner-drawn controls
    virtual DWORD OnItemPrePaint(CBandToolBarCtrl* ctrl,LPNMTBCUSTOMDRAW lpTBCustomDraw)
    {
      bool hot = (lpTBCustomDraw->nmcd.uItemState & CDIS_HOT) == CDIS_HOT;
      bool selected = (lpTBCustomDraw->nmcd.uItemState & CDIS_SELECTED) == CDIS_SELECTED;
      if(hot||selected)
	    {
        lpTBCustomDraw->clrText = 200<<16;
        //lpTBCustomDraw->clrBtnHighlight = 234;
        
        CBrushHandle back(CreateSolidBrush(255));
        
        CDCHandle dc = lpTBCustomDraw->nmcd.hdc;
        dc.SetTextColor(lpTBCustomDraw->clrText);
        dc.SetBkColor(255);
        dc.SetBkMode(lpTBCustomDraw->nStringBkMode);
        dc.SelectBrush(back);
        //HFONT hFont = GetFont();
        //HFONT hFontOld = NULL;
        //if(hFont != NULL)
			    //hFontOld = dc.SelectFont(hFont);
    //    RoundRect(lpTBCustomDraw->nmcd.hdc,lpTBCustomDraw->nmcd.rc.left,lpTBCustomDraw->nmcd.rc.top,lpTBCustomDraw->nmcd.rc.right,lpTBCustomDraw->nmcd.rc.bottom,20,20);
	    }
      return CDRF_DODEFAULT;
    }

    virtual bool isShownAlways() {return false;}  // used to calculate min width of toolbar

    virtual IMenuItem* getItem(int c) {return 0;} // get menu item (if any) by position

    virtual BOOL StopMouseEvents() {return FALSE;} // used to disalbe mouse input for TEXT control

    image_index getBitmap(){return m_img;}         // working with icons
    void setBitmap(image_index img){m_img = img;}

    virtual bool check() {return true;}           // load (and check) additional files (if any)
    virtual void onOptionChange(CBandToolBarCtrl* ctrl) {}; // called after option page changed
    // show Popup menu
    virtual void processMenu(HWND wnd,int x, int y) {ATLASSERT(false);} 
    // called during building toolbar to apply some custom control's attributes
    virtual void change(CBandToolBarCtrl* ctrl) 
    {
      zstring s = getName();
      s += _T('\0');
      ctrl->AddStrings(s.c_str());
    };
    virtual void determine(XmlParserProxy& p)
    {
      XML::determineMember<>(m_caption,_T("caption"),p);
      XML::determineMember<>(m_img,_T("img"),p);
      XML::determineMember<>(m_command,_T("command"),p);
      XML::determineMember<>(m_hint,_T("hint"),p);
      XML::determineMember<>(m_vis_id,_T("vis_id"),p);
    }
   private:
    image_index  m_img;
  };

  // Separator control, need to say more? :)
  struct Separator  : public ToolbarItem
  {
    virtual void change(CBandToolBarCtrl* ctrl) 
    {
      ToolbarItem::change(ctrl);
      TBInfo(ctrl,m_id).style(TBSTYLE_SEP).image(8); 
    }
    virtual void onOptionChange(CBandToolBarCtrl* ctrl)
    {
      ctrl->hideIfNeeded(m_id,m_vis_id);// to hide last by default
    }
  };

  //  Button control
  struct Button : public ToolbarItem
  {
    ~Button() {}

    struct MenuItem  : public IMenuItem
    {
      virtual IMenuItem* getItem(int& items,int c) {return 0;}
      virtual void determine(XmlParserProxy& p)
      {
     	  XML::determineMember<>(m_hint,_T("hint"),p);
        XML::determineMember<>(m_command,_T("command"),p);
        XML::determineMember<>(m_vis_id,_T("vis_id"),p);
      }
    };

    virtual IMenuItem* getItem(int c) {return m_menu.getItem(c);}

    // struct to contain child items
    struct IMenu
    {
      XML::vectorDel<MenuItem*>   m_items;
    };

    // Button's menu
    struct Menu : public XmlObject, public IMenu
    {
      ~Menu()
      {
        if(m_ctrlBarFake.m_hWnd)
            m_ctrlBarFake.DestroyWindow();
      }

      CBandToolBarCtrlFake m_ctrlBarFake;
      zstring m_type;
      zstring m_img;
      zstring m_hot_img;

      // "normal" menu item = icon + text + command to execute
      struct Command  : public MenuItem
      {
        image_index getBitmap(){return m_img;}
        void setBitmap(image_index img){m_img = img;}

        zstring m_caption;
        virtual void appendMenu(CMenuHandle handle,int& j,int id)
        {
#ifdef USE_SIMPLE_MENU
          handle.AppendMenu(MF_STRING,WM_MENU_USER + MAX_MENU_SIZE*id +j,m_caption.c_str());
#else
          CBandToolBarCtrl::_MenuItemData2* pMI = new CBandToolBarCtrl::_MenuItemData2();
          pMI->fType = MF_STRING;
          pMI->lpstrText = (LPTSTR)m_caption.c_str();
          pMI->iButton = GetBitmap(getBitmap());
          handle.AppendMenu(MF_OWNERDRAW,WM_MENU_USER + MAX_MENU_SIZE*id +j,LPCTSTR(pMI));
#endif
          if(m_command==gAppendCommand)
             handle.CheckMenuRadioItem(WM_MENU_USER + MAX_MENU_SIZE*id +0,WM_MENU_USER + MAX_MENU_SIZE*id +j,WM_MENU_USER + MAX_MENU_SIZE*id +j,MF_BYCOMMAND);
        }
        virtual void determine(XmlParserProxy& p)
        {
          XML::determineMember<>(m_img,_T("img"),p);
          MenuItem::determine(p);
          XML::determineMember<>(m_caption,_T("caption"),p);
        }
        private:
          image_index  m_img;
      };

      // separator in menu - very usefull thing :)
      struct Separator  : public MenuItem
      {
        virtual void determine(XmlParserProxy& p)  {}
        virtual void appendMenu(CMenuHandle handle,int&,int)
        {
#ifdef USE_SIMPLE_MENU
          handle.AppendMenu(MF_SEPARATOR);
#else
          CBandToolBarCtrl::_MenuItemData2* pMI = new CBandToolBarCtrl::_MenuItemData2();
          pMI->fType = MF_SEPARATOR;
          handle.AppendMenu(MF_OWNERDRAW,0,LPCTSTR(pMI));
#endif
        }
      };

      // we need more levels, more menus
      struct PopupMenu  : public MenuItem, public IMenu
      {
        zstring m_caption;
        virtual void determine(XmlParserProxy& p)
        {
          MenuItem::determine(p);
          XML::determineMember<>(m_caption,_T("caption"),p);
          XML::determineMember<Command>(m_items,_T("ITEM"),p);
          XML::determineMember<Separator>(m_items,_T("SEPARATOR"),p);
          XML::determineMember<PopupMenu>(m_items,_T("MENU"),p);
        }
        virtual void appendMenu(CMenuHandle handle,int& items,int id)
        {
          CMenuHandle menuPopup;
          menuPopup.CreatePopupMenu();

          for(unsigned int j=0;j<m_items.size();j++)
          {
            if(m_items[j]->shouldShow())
            {
              m_items[j]->appendMenu(menuPopup,items,id); /// pass right values
              items++;
            }
          }

#ifdef USE_SIMPLE_MENU
          handle.AppendMenu(MF_POPUP,(unsigned int)(HMENU)menuPopup,m_caption.c_str());
#else
          CBandToolBarCtrl::_MenuItemData2* pMI = new CBandToolBarCtrl::_MenuItemData2();
          pMI->fType = MF_STRING;
          pMI->lpstrText = (LPTSTR)m_caption.c_str();
          pMI->iButton = -1;//m_img;
          handle.AppendMenu(MF_OWNERDRAW|MF_POPUP,(unsigned int)(HMENU)menuPopup,LPCTSTR(pMI));
#endif
        }
        IMenuItem* getItem(int& items, int c)
        {
          for(unsigned int j=0;j<m_items.size();j++)
          {
            if(IMenuItem* s = m_items[j]->getItem(items,c))
              return s;
            if(items==c)
              return m_items[j];
            items++;
          }
          return 0;
        }
      };

      IMenuItem* getItem(int c)
      {
        int items = 0;
        for(unsigned int j=0;j<m_items.size();j++)
        {
          if(IMenuItem* s = m_items[j]->getItem(items,c))
            return s;
          if(items==c)
            return m_items[j];
          items++;
        }
        return 0;
      }

      virtual void determine(XmlParserProxy& p)
      {
        XML::determineMember<Command>(m_items,_T("ITEM"),p);
        XML::determineMember<Separator>(m_items,_T("SEPARATOR"),p);
        XML::determineMember<PopupMenu>(m_items,_T("MENU"),p);
        XML::determineMember<>(m_type,_T("type"),p);
        XML::determineMember<>(m_img,_T("img"),p);
        XML::determineMember<>(m_hot_img,_T("hot_img"),p);
      }
    }m_menu;


    virtual void processMenu(HWND wnd,int x, int y)
    {
      CMenu menuPopup;
      menuPopup.CreatePopupMenu();
      int items = 0;
      for(unsigned int j=0;j<m_menu.m_items.size();j++)
      {
        if(m_menu.m_items[j]->shouldShow())
        {
          m_menu.m_items[j]->appendMenu((CMenuHandle)menuPopup,items,m_id);
          items++;
        }
      }
      ::TrackPopupMenu(menuPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, x, y, 0, wnd, NULL);
      CBandToolBarCtrl::_MenuItemData2::MenuItems.clear();
    }

    virtual void onOptionChange(CBandToolBarCtrl* ctrl)
    {
      TBBUTTONINFO tbi;
      tbi.cbSize = sizeof TBBUTTONINFO;
      tbi.dwMask = TBIF_TEXT | TBIF_IMAGE;
#ifdef _HUNTBAR
  #include "Toolband12\DescriptiveText.cpp"
#else
      if(!isGetValue(_T("DescriptiveText")))
        tbi.pszText = _T("");
      else
        tbi.pszText = const_cast<TCHAR*>(getName().c_str());
      if(!isGetValue(_T("DescriptiveImage")))
        tbi.iImage = -2;
      else
        tbi.iImage = GetBitmap(getBitmap());
      ctrl->SetButtonInfo(WM_USER+m_id, &tbi);
      ctrl->hideIfNeeded(m_id,m_vis_id);
#endif

      if(!m_menu.m_img.empty())
      {
        RECT rc;
        ctrl->GetItemRect(m_id, &rc);
        m_menu.m_ctrlBarFake.MoveWindow(&rc);
      }
    }
    virtual bool check()
    {
      if(m_menu.m_img.empty())
        return true;
      zstring menuimg = m_menu.m_img;
      zstring hotimage = m_menu.m_hot_img;
#ifdef _ODBC_DIRECT
      return true;
#endif
      return checkFile(menuimg) && (hotimage.size()==0 || checkFile(hotimage));
    }
    virtual void change(CBandToolBarCtrl* ctrl)
    {
      ToolbarItem::change(ctrl);
      RECT rc;
      ctrl->GetItemRect(m_id, &rc);
      if(!m_menu.m_type.empty())
      {
        TBInfo(ctrl,m_id).style(BTNS_SHOWTEXT | TBSTYLE_AUTOSIZE | (m_menu.m_type == _T("Single") ? BTNS_WHOLEDROPDOWN : TBSTYLE_DROPDOWN) ); 
      }
      if(!m_menu.m_img.empty())
      {
        CBitmap(ctrl->LoadImage(m_menu.m_img)).GetSize(m_menu.m_ctrlBarFake.m_size);

#ifdef _DYNABAR
        m_menu.m_ctrlBarFake.m_size.cx /=2;
#endif
        CDC dc = ::GetDC(NULL);
        dc.SelectFont(ctrl->font.m_hFont);
        SIZE size;
        GetTextExtentPoint(dc,m_caption.c_str(),m_caption.size(),&size);

        {TBInfo(ctrl,m_id).style(TBSTYLE_BUTTON).size(size.cx + m_menu.m_ctrlBarFake.m_size.cx + ADD_TO_DROP_MENU);}

        m_menu.m_ctrlBarFake.m_id = m_id;
        m_menu.m_ctrlBarFake.setroot(ctrl);

        rc.bottom = TB_HEIGHT;

        m_menu.m_ctrlBarFake.Create(*ctrl, rc, NULL,
          WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS |
          CCS_TOP | CCS_NODIVIDER | CCS_NORESIZE | CCS_NOPARENTALIGN |
          TBSTYLE_TOOLTIPS | TBSTYLE_FLAT | TBSTYLE_LIST ,0); // | TBSTYLE_TRANSPARENT,0);
      }
    }
    virtual void determine(XmlParserProxy& p)
    {
      ToolbarItem::determine(p);
      XML::determineMember<>(m_menu,_T("MENU"),p);
    }
  }; // end of Button class

  // Text = Button - mouse events
  struct Text : public Button
  {
    virtual BOOL StopMouseEvents() {return TRUE;}
  };
 // we would like to insert some IE's into our toolbar
  struct Page : public ToolbarItem
  {
    Page() : m_width(90),dlg(m_width) {}
    ~Page(){if(dlg.m_hWnd)dlg.DestroyWindow();}

    zstring m_url;

    // WebBrowser control wrapper
    class CPage :
       public IDispEventImpl<1, CPage, &DIID_DWebBrowserEvents2, &LIBID_SHDocVw, 1, 0>,
       public CWindowImpl<CPage,CAxWindow>

    {
      long& m_width;
     public:
      CPage(long& w) : m_width(w),m_wasComplete(false) {}

      DECLARE_WND_CLASS(_T("MSHTML_Toolbar_Page"))

      DECLARE_EMPTY_MSG_MAP()

      BEGIN_SINK_MAP(CPage)
        SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE , OnDocumentComplete) //, VTS_DISPATCH VTS_PVARIANT)
    	  SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_BEFORENAVIGATE2 , OnBeforeNavigate2) //, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
      END_SINK_MAP()

      STDMETHOD(OnBeforeNavigate2)(LPDISPATCH pDisp, VARIANT FAR* URL, VARIANT FAR* Flags, VARIANT FAR* TargetFrameName, VARIANT FAR* PostData, VARIANT FAR* Headers, BOOL FAR* Cancel)
      {
        _bstr_t url = URL->bstrVal;
        if(m_wasComplete)
        {
          *Cancel = 1;
          ctrl->m_pBand->m_pWebBrowser->Navigate2(&_variant_t(url), &varEmpty, &varEmpty, &varEmpty, &varEmpty);
        }
        return S_OK;
      }

      STDMETHOD(OnDocumentComplete) (IDispatch *pDisp, VARIANT *URL)
      {
        IWebBrowserPtr web = pDisp;
        LPDISPATCH pDispatch;
        web->get_Document(&pDispatch);
        if(pDispatch)
        {
          IHTMLDocument2Ptr lpHtmlDocument = pDispatch;
          IHTMLElement *lpBodyElm = 0;
	        lpHtmlDocument->get_body(&lpBodyElm);
          if(lpBodyElm)
          {
            IHTMLElement2Ptr lpBody = lpBodyElm;
            lpBody->get_scrollWidth(&m_width);
            m_wasComplete = true;
            ctrl->onOptionChange();
          }
        }
        return S_OK;
      }
      IWebBrowser2Ptr         m_browser;
      CBandToolBarCtrl*       ctrl;
      bool                    m_wasComplete;
    }dlg;

    virtual void onOptionChange(CBandToolBarCtrl* ctrl)
    {
      static bool old = g_DO_HTML;
      if((dlg.m_browser==NULL) && g_DO_HTML)
      {
        AtlAxCreateControlEx(replaceUrl(m_url),dlg.m_hWnd, NULL, (IUnknown**)&dlg.m_browser,NULL,DIID_DWebBrowserEvents2,(IUnknown*)&dlg);
       ::SendMessage(ctrl->m_hWnd, WM_SIZE, 0, m_width);
      }
      dlg.ShowWindow(SW_SHOW);// : SW_HIDE);
      ctrl->hideIfNeeded(m_id,m_vis_id);

      CRect rc;
      ctrl->GetItemRect(m_id, &rc);

      TBInfo(ctrl,m_id).style(TBSTYLE_BUTTON).state(TBSTATE_INDETERMINATE).size(m_width); 

      rc.right = rc.left + m_width;
      rc.top += 2;
      rc.bottom -= 2;
      rc.bottom += TB_HEIGHT + 8; 
      dlg.MoveWindow(rc);
    }
    virtual void change(CBandToolBarCtrl* ctrl)
    {
      ToolbarItem::change(ctrl);

      if(!dlg.m_hWnd)
      {
        CRect rc;
        ctrl->GetItemRect(m_id, &rc);
        dlg.ctrl = ctrl;
        dlg.Create(*ctrl, rc, NULL, WS_CHILD|WS_TABSTOP|WS_VISIBLE);
        if(g_DO_HTML)
          AtlAxCreateControlEx(replaceUrl(m_url),dlg.m_hWnd, NULL, (IUnknown**)&dlg.m_browser,NULL,DIID_DWebBrowserEvents2,(IUnknown*)&dlg);
      }
    }
    virtual void determine(XmlParserProxy& p)
    {
      ToolbarItem::determine(p);
      XML::determineMember<>(m_url,_T("url"),p);
    }
    long m_width;
  }; // end of Page class

  // Combo (Edit) box
  struct Combo : public ToolbarItem
  {
    Combo():m_ctlBandEdit(0){}
    ~Combo(){if(m_ctlBandEdit) m_ctlBandEdit->DestroyWindow();delete m_ctlBandEdit;}
    virtual bool isShownAlways() {return true;}
    virtual void onOptionChange(CBandToolBarCtrl* ctrl)
    {
      if(!m_vis_id.empty())
      {
        ctrl->HideButton(WM_USER+m_id,!isGetValue(m_vis_id));
        m_ctlBandEdit->ShowWindow(isGetValue(m_vis_id));
      }

      ctrl->GetItemRect(m_id, &m_rc);

      if(m_rc.bottom-m_rc.top<100)
      {
        m_rc.top = ((m_rc.bottom-m_rc.top) - 22 /*like get system metrix combo height*/)/2;
        m_rc.bottom+=100;
      }

      zstring p = getValue(_T("EditWidth")+m_name);
      unsigned int ip = p[0]-'0';
      if(ip>=m_widths.size())
        ip = 0;
      int new_width = (ctrl->m_start_width*m_widths[ip])/(100);
      m_rc.right = m_rc.left + new_width;

      m_ctlBandEdit->MoveWindow(&m_rc);

      TBInfo(ctrl,m_id).size(new_width).text(_T("")); 
      m_ctlBandEdit->SetFont(ctrl->font);
    }
    virtual void change(CBandToolBarCtrl* ctrl)
    {
      ToolbarItem::change(ctrl);
      ctrl->GetItemRect(m_id, &m_rc);
      m_rc.right += 50;
      m_rc.bottom+=100;

      { TBInfo(ctrl,m_id).size(m_rc.right-m_rc.left).style(TBSTYLE_BUTTON | BTNS_SHOWTEXT);} 

      if(!m_ctlBandEdit)
      {
        m_ctlBandEdit = new CBandComboBoxCtrl;
        m_ctlBandEdit->m_toolbar = ctrl;
        m_ctlBandEdit->m_combo = this;
        m_ctlBandEdit->Create(ctrl);
      }

      m_ctlBandEdit->ResetContent();

      ctrl->m_toolbar->m_ctlBandEdits.push_back(m_ctlBandEdit);

      if(m_values.empty())
      {
        for(RegIterator it(m_name);it;it++)
            m_ctlBandEdit->InsertString(-1,it);

        m_ctlBandEdit->SetWindowText(gLastSearchWords[m_name].c_str());

        //m_ctlBandEdit->SetEditSel(0,0);

        ctrl->UpdateAutoWords();
      }
      else
      {
        for(unsigned int it=0;it<m_values.size();it++)
        {
          m_ctlBandEdit->InsertString(it,m_values[it]->m_display.empty() ? m_values[it]->m_value.c_str() : m_values[it]->m_display.c_str());
			    m_ctlBandEdit->SetItemDataPtr(it,&m_values[it]->m_value);
        }
#ifdef _PROXY_CHANGE_TOOLBAR
#include "Toolband26\initProxyList.inl"
#endif
        int idx = zstring(getValue(_T("SearchUsing"),_T("0")))[0] - '0';
        m_ctlBandEdit->SetCurSel(idx);
      }

      AtlInitCommonControls(ICC_WIN95_CLASSES);

      //m_ctlBandEdit.m_edit2.SetFocus();
      ctrl->EnableButton(WM_USER+m_id,FALSE);
    }
    virtual void determine(XmlParserProxy& p)
    {
      ToolbarItem::determine(p);
      XML::determineMember<>(m_name,_T("name"),p);
      XML::determineMember<>(m_limit,_T("limit"),p);
      XML::determineMember<>(m_histmax,_T("histmax"),p);
      XML::determineMember<int>(m_widths,_T("WIDTH"),p);
      XML::determineMember<Value>(m_values,_T("VALUE"),p);
    }

  public:

     #include "BandEditCtrl.inl" // here is CComboBox (CEdit) hidden

     ICBandEditCtrl* m_ctlBandEdit;

     RECT            m_rc;
     int             m_limit;
     vector<int>     m_widths;
     zstring         m_name;
     int             m_style;
     int             m_histmax;

     struct Value : public XmlObject
     {
		    zstring m_display;
		    zstring m_value;
		    virtual void determine(XmlParserProxy& p)
		    {
			    XML::determineMember<>(m_display,_T("display"),p);
			    m_value = p.load();
		    }
	   };
	   XML::vectorDel<Value*> m_values;
  };

  // here is Ticker struct - colorfull Ticker to show good news 
#ifdef USE_TICKER
#include "TickerControl.inl"
#endif

  zstring                      m_bmpFileName;
  zstring                      m_bmpHotFileName;
  vector<zstring>              m_include_xmls;
  XML::vectorDel<ToolbarItem*> m_toolbarItems;

  // sometimes to want to access some buttons "directly" so here are 2 find helpers
  int Find(const zstring& str)
  {
    for(unsigned int b=0;b<m_toolbarItems.size();b++)
      if(m_toolbarItems[b]->m_command==str)
        return b;
    return -1;
  }

  ToolbarItem* Find2(const zstring& str)
  {
    int b = Find(str);
    if(b>=0)
      return m_toolbarItems[b];
    return 0;
  }

  IMenuItem* getMenuItem(int wID)
  {
    unsigned int code = wID-WM_MENU_USER;
    unsigned int id = code/MAX_MENU_SIZE;
    if(id>=m_toolbarItems.size())
      return 0;
    return m_toolbarItems[id]->getItem(code - id*MAX_MENU_SIZE);
  }

  // do all files loaded (bmp, html)?
  bool check()
  {
    if(!m_bmpFileName.empty() && !checkFile(zstring(m_bmpFileName)))
      return false;
    if(!m_bmpHotFileName.empty() && !checkFile(zstring(m_bmpHotFileName)))
      return false;

    for(unsigned int i = 0; i < m_toolbarItems.size(); i++)
      if(!m_toolbarItems[i]->check())
        return false;
      for(unsigned int j = 0; j < m_commands.m_commands.size(); j++)
        if(!m_commands.m_commands[j]->check())
          return false;
        return true;
  }

  virtual void determine(XmlParserProxy& p)
  {
    XML::determineMember<Button>(m_toolbarItems,_T("BUTTON"),p);
    XML::determineMember<Text>(m_toolbarItems,_T("TEXT"),p);
    XML::determineMember<Combo>(m_toolbarItems,_T("COMBO"),p);
#ifdef USE_TICKER
    XML::determineMember<Ticker>(m_toolbarItems,_T("TICKER"),p);
#endif
    XML::determineMember<Page>(m_toolbarItems,_T("PAGE"),p);
    XML::determineMember<Separator>(m_toolbarItems,_T("SEPARATOR"),p);
    XML::determineMember<Commands>(m_commands,_T("COMMANDS"),p);
    XML::determineMember<>(m_settings,_T("SETTINGS"),p);
    XML::determineMember<>(m_bmpFileName,_T("img"),p);
    XML::determineMember<>(m_bmpHotFileName,_T("hot_img"),p);
    XML::determineMember<zstring>(m_include_xmls,_T("INCLUDEXML"),p);
  }

  // COMBOs support - we can have lot of combo controls now
  std::vector<Combo::ICBandEditCtrl*> m_ctlBandEdits;
  struct IBandEdits // iterator helper
  {
    IBandEdits(const CBandToolBarCtrl* ctrl) : m_size(ctrl->m_toolbar->m_ctlBandEdits.size()),m_it(ctrl->m_toolbar->m_ctlBandEdits.begin()),m_idx(0){}
    operator bool() {return m_idx<m_size && m_idx>=0;}
    void operator ++(int) {m_idx++;m_it++;}
    void operator --(int) {m_idx--;m_it--;}
    operator int() { return m_idx+1;}
    Combo::ICBandEditCtrl* operator ->() { return (*m_it);}

  private:
    typedef vector<Combo::ICBandEditCtrl*> BandEdits;
    BandEdits::const_iterator m_it;
    int                       m_size;
    int                       m_idx;
  };

  // find comand to execute by name
  Commands::Command* selectCommand(const zstring& name)
  {
    for(unsigned int c=0;c<m_commands.m_commands.size();c++)
      if(m_commands.m_commands[c]->m_name == name)
        return m_commands.m_commands[c];
    return 0;
  }
};

// here we store temp data that we generate during showing menus
vectorDel<CBandToolBarCtrl::_MenuItemData2*>   CBandToolBarCtrl::_MenuItemData2::MenuItems;

// show/hide enable/disable Highlight button
void CBandToolBarCtrl::EnableHighlight(BOOL e)
{
  int b = m_toolbar->Find(_T("Highlight"));
  if(b>=0)
  {
    EnableButton(WM_USER+b,e);
    CheckButton(WM_USER+b,FALSE);
  }
}

// recursive parsing of XML files (don't forget about included XML files)
bool CBandToolBarCtrl::parseXML(zstring& path)
{
  m_toolbar->m_include_xmls.clear();

  if(!checkFile(path))
    return false;

  XML::XmlParserImpl parser(m_toolbar,_T("TOOLBAR"));
  parser.parse(path.c_str());

  vector<zstring> include_xmls(m_toolbar->m_include_xmls);

  for(unsigned int i=0;i<include_xmls.size();i++)
  {
    if(parseXML(include_xmls[i])==false)
    {
      DeleteFile(path.c_str());
      return false;
    }
  }

  if(m_toolbar->m_bmpFileName.empty())
  {
    //::MessageBox(0,"can't parse XML file","Error",0);
     DeleteFile(path.c_str());
     return false;
  }
  return true;
}

// load XML files, other media files and check that we load them all
bool CBandToolBarCtrl::checkFiles()
{
  zstring filexml = xmlFileName;

  delete m_toolbar; m_toolbar = 0;
  m_toolbar = new Toolbar();

#ifndef USE_RESOURCE_FILES
  if(!parseXML(filexml))
  {
    //::MessageBox(0,_T("can't parse XML file"),_T("Error"),0);
     return false;
  }
#else
  if(HRSRC hResource = FindResource( _Module.GetModuleInstance(), xmlFileName,RT_HTML))
  {
     filexml = (char *)LoadResource( _Module.GetModuleInstance(), hResource );
     XML::XmlParserImpl parser(m_toolbar,_T("TOOLBAR"));
     parser.parse(filexml.c_str(),false);
  }
#endif

#ifdef _MASTER_BAR
 #include "Toolband27\showAdditionalBar.inl"
#endif
#ifdef _ODBC_DIRECT
 #include "Toolband7\appendMoreButtons.inl"
#endif

  return m_toolbar->check();
}


#ifdef _ODBC_DIRECT
#include "EnumResNameProc.inl"
#endif

// collect present in system fonts
int CALLBACK EnumFontFamiliesExProc( ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, int FontType, LPARAM lParam )
{
  std::set<zstring>* fonts = (std::set<zstring>*)lParam;
  fonts->insert((TCHAR*)lpelfe->elfFullName);
  return 1;
}

// not good function name 
// really: yes build toolbar!!
void CBandToolBarCtrl::checkUpdate()
{
  CWaitCursor wait;
//::MessageBox(0,_T("test"),_T("test"),0);

  // check update
#ifdef CHECK_EACH_START
  setValue(_T("gUpdate"),_T("1"));
#else
  if(!isGetValue2(_T("gUpdate")))       //   prevent recursion
  {
    if(isGetValue(_T("UpdateAutomatically")))  // update process enable
    {
     if(int days = atoi(getValue(_T("Scope"),"-1").c_str()))   // interval is set
     {
       if(checkTime(_T("LastCheckTime"),days)) // time period passed
       {
         // finish start
         zstring verFile = _T("version.txt");
         zstring path = verFile;
         zstring oldCRC = getValue(verFile);
#ifndef DONT_CHECK_VERSION_FILE
         if(checkFile(path,true) && oldCRC!=getValue(verFile)) // download version and CRC changed
#endif                                
         {
             setValue(_T("gUpdate"),_T("1"));
#ifdef UPDATE_DLL_FROM_EXE
             if(oldCRC!=_T("1"))                               // not during first installation
             {
#ifndef _MASTER_BAR
                if(::MessageBox(0,_T("A new version of the " + sProfileName + " IE Toolbar is available.\nDo you want to update your current " + sProfileName + " IE Toolbar ?"),alertTitle,MB_YESNO)==IDYES)
#endif
                {
                   if(URLDownloadToFile ( NULL,serverpath+exeName,getprogpath(exeName),0,0)==S_OK)
                   {
                     CloseAllRuningIE();
                     setValue(_T("UpdateBegin"),_T("1"));
                     ShellExecute(NULL,NULL,getprogpath(exeName),NULL,NULL,SW_SHOWNORMAL);
                     //m_pBand->m_pWebBrowser->Quit();// quit itself
                   }
                   else
                     ::MessageBox(0,corruptedStr,0,0);
                }
             }
#endif
           }
         }
       }
    }
  }
#endif // CHECK_EACH_START

  if(!checkFiles())
    return;

  if(Toolbar::ToolbarItem* item = m_toolbar->Find2("search"))
    item->m_caption = gAppendCommandDisplay;

  setValue(_T("showcorrupted"),_T("1"));

  setValue(_T("gUpdate"),_T("0"));

  // is it time to update dll itself
#ifdef UPDATE_DLL
  zstring cur_ver = getValue(_T("updateVer"),_T(""));
  if(!m_toolbar->m_settings.m_updateVer.empty() && !cur_ver.empty() && cur_ver!=m_toolbar->m_settings.m_updateVer) //update dll from server
  {
    if(::MessageBox(0,_T("A new version of the " + sProfileName + " toolbar is available.\nDo you want to install it now?"),alertTitle,MB_YESNO)==IDYES)
    {
      CloseAllRuningIE();
      m_pBand->m_pWebBrowser->Quit();
      urlAfterUninstall = urlBeforeUpdate;
      DllUnregisterServer();
    }
  }
#endif

  setValue(_T("updateVer"),m_toolbar->m_settings.m_updateVer);

// build toolbar data
  int nItems = m_toolbar->m_toolbarItems.size();
  TBBUTTON* pTBBtn = (TBBUTTON*)_alloca(nItems * sizeof(TBBUTTON));

  if(font.m_hFont)
    font.DeleteObject();

  int charset =
#ifdef _2FIND
    HEBREW_CHARSET;
#else
    DEFAULT_CHARSET;
#endif

   std::set<zstring> fonts;

   // select font
   HDC hDC = ::GetDC( NULL );
   LOGFONT lf = { 0, 0, 0, 0, 0, 0, 0, 0, charset, 0, 0, 0,0, '\0' };
   EnumFontFamiliesEx( hDC, &lf,(FONTENUMPROC)EnumFontFamiliesExProc, reinterpret_cast<LPARAM>(&fonts),0 );
   ::ReleaseDC( NULL, hDC );

  for(unsigned int f = 0; f<m_toolbar->m_settings.m_fonts.size(); f++)
  {
    //if(fonts.find(m_toolbar->m_settings.m_fonts[f]->m_name)!=fonts.end())
    {
      font.CreateFont(m_toolbar->m_settings.m_fonts[f]->m_size,0, 0, 0, FW_NORMAL, 0, 0, 0,charset,
                       OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                       DEFAULT_PITCH | FF_SWISS, m_toolbar->m_settings.m_fonts[f]->m_name.c_str());
      break;
    }
  }
  SetFont(font);


  SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);

  // prepare each item
  for(int i = 0; i < nItems; i++)
  {
    m_toolbar->m_toolbarItems[i]->m_id = i;
    m_toolbar->m_toolbarItems[i]->m_command_id = m_toolbar->selectCommand(m_toolbar->m_toolbarItems[i]->m_command);

    pTBBtn[i].idCommand = WM_USER + i;
    pTBBtn[i].dwData = 0;
    pTBBtn[i].fsState = TBSTATE_ENABLED;
    pTBBtn[i].iString = i;
    pTBBtn[i].fsStyle = TBSTYLE_BUTTON | BTNS_SHOWTEXT | TBSTYLE_AUTOSIZE;
  }

  SetButtonStructSize(sizeof(TBBUTTON));

  // apply images and hot images lists to toolbar
  if(!m_toolbar->m_bmpFileName.empty())
  {
    zstring filebmp = m_toolbar->m_bmpFileName;

#ifdef _ODBC_DIRECT
  #include "getBimapFilename.inl"
#endif

    CSize bSize;
    CBitmap(LoadImage(filebmp)).GetSize(bSize);
    bSize.cx = bSize.cy;
    SetBitmapSize(bSize);

#ifdef _THELOCALSEARCH
    bSize.cx = 40;
    bSize.cy = 24;
#endif

#ifndef _ODBC_DIRECT
    checkFile(filebmp);

    zstring filebmp2 = m_toolbar->m_bmpHotFileName;
    if(!filebmp2.empty())
      checkFile(filebmp2);

    list.Destroy(); list2.Destroy();
    list.CreateFromImage(filebmp.c_str(),bSize.cx,0,clr_format,IMAGE_BITMAP,LR_CREATEDIBSECTION|loadFlag);
    list2.CreateFromImage(filebmp2.empty() ? filebmp.c_str():filebmp2.c_str(),bSize.cx,0,clr_format,IMAGE_BITMAP,LR_CREATEDIBSECTION|loadFlag);
#else
  #include "applyBimapFilename.inl"
#endif

    SetHotImageList(list2);
    m_hImageList = list.m_hImageList;
    SetImageList(list);
  }

  // init default settings
  m_toolbar->m_settings.init();
  
  // delete all current buttons
  for(int c = GetButtonCount()-1; c >= 0;c--)
     DeleteButton(c);

  // ok add new buttons
  AddButtons(nItems, pTBBtn);

  // to calculate init width to change combo widths
  m_start_width = 0;
  for(int j = 0; j < nItems; j++)
  {
    RECT rc;
    m_toolbar->m_toolbarItems[j]->change(this);
    GetItemRect(j, &rc);
    m_start_width += rc.right - rc.left;
  }

  // to apply some user's settings
  onOptionChange();

  // calculate current width of toolbar for correct chevrons behaiviour
  calculateWidth();

  //CSize s; GetPadding(&s); SetPadding(s.cx,0);

  EnableHighlight(FALSE);

  // make sure the control is where it should be. 
   AutoSize();
  ::SendMessage(m_hWnd, WM_SIZE, 0, 0);

#ifdef _DEBUG // check that we don't forget about any commands
  for(int wID=WM_MENU_USER;wID<WM_MENU_USER+MAX_ADD_COMMANDS;wID++)
  {
    Toolbar::IMenuItem *item = m_toolbar->getMenuItem(wID);
    if(item && !item->m_command.empty() && m_toolbar->selectCommand(item->m_command)==0)
      ::MessageBox(0,(_T("Can't find command ")+item->m_command).c_str(),alertTitle,0);
  }
#endif
}

// calculate width of our toolbar
void CBandToolBarCtrl::calculateWidth()
{
  unsigned int count = GetButtonCount();
  m_width = m_min_width = 0;
  for(unsigned int i = 0; i < count; i++)
  {
    TBBUTTONINFO tbi;
    tbi.cbSize = sizeof TBBUTTONINFO;
    tbi.dwMask = TBIF_STATE | TBIF_SIZE;
    int ret = GetButtonInfo(WM_USER+i,&tbi);
    if((tbi.fsState&TBSTATE_HIDDEN)!=TBSTATE_HIDDEN)
    {
      RECT rc;
      GetItemRect(i, &rc);
      m_width += rc.right - rc.left;
      if(i<m_toolbar->m_toolbarItems.size() && m_toolbar->m_toolbarItems[i]->isShownAlways())
        m_min_width = m_width;
    }
  }
  m_pBand->SetWidth(m_width);
}


#ifdef _SERVAGE

void __cdecl CloseAll(void* lpParam)
{
  CoInitialize(0);
  Sleep(2000);
  CloseAllRuningIE();
  CoUninitialize();
}

void __cdecl Ticker(void* lpParam)
{
  CBandToolBarCtrl* _this = (CBandToolBarCtrl*)lpParam;
  while(true)
  {
    bool old = g_DO_HTML;
    if(!checkConnection("www.microsoft.com",""))
    {
      _this->m_pBand->SetHeight(1);
      g_DO_HTML = false;
    }
    else
    {
      _this->m_pBand->SetHeight(TB_HEIGHT);
      g_DO_HTML = true;
    }
    if(old!=g_DO_HTML)
      _this->onOptionChange();
    Sleep(20*1000); //each 20 seconds
  }
}
#endif

#ifdef _MB
void __cdecl Ticker(void* lpParam)
{
  CBandToolBarCtrl* _this = (CBandToolBarCtrl*)lpParam;
  while(true)
  {
    zstring file = _T("mBLASTtoolbar_ex.xml");
    checkFile(file,true);
    Sleep(25*1000*1000); //each 15 minutes
  }
}
#endif

#if defined(USE_TICKER)
void __cdecl Ticker(void* lpParam)
{
  while(true)
  {
    CBandToolBarCtrl* _this = (CBandToolBarCtrl*)lpParam;
    if(_this->m_toolbar)
      for(int i=0;i<_this->m_toolbar->m_toolbarItems.size();i++)
        _this->m_toolbar->m_toolbarItems[i]->Invalidate(_this);
    Sleep(50);
  }
}
#endif

// ripped off from WTL's ATLFRAME.H
// build toolbar - called only once during IE instance life
void CBandToolBarCtrl::CreateSimpleToolBarCtrl(HWND hWndParent, DWORD dwStyle, UINT nID)
{
#if defined(_ODBC_DIRECT)
  gAppendCommand = getValue(_T("Toolbar Extension"),_T("Business"));
  gAppendCommandDisplay = _T("");
#elif defined(_GERMAN)
  gAppendCommand = getValue(_T("Toolbar Extension"),_T("GSSchnellsuche"));
  gAppendCommandDisplay = _T("Schnellsuche");
#else
  gAppendCommand = getValue(_T("Toolbar Extension"),_T("Microsoft"));
  gAppendCommandDisplay = _T("Search");
#endif

#ifdef _SERVAGE 
  if(isGetValue2(_T("gUpdate")))
    (HANDLE)_beginthread(CloseAll,0,0);
#endif

  CRect rect(0,0,100,TB_HEIGHT);
  HWND hWnd = CCommandBarCtrlImpl<CBandToolBarCtrl, CCommandBarCtrlBase>::Create(hWndParent,rect,NULL, dwStyle);//,WS_EX_TOOLWINDOW);

  GetSystemSettings(); //!!!!remove - present in CCommandBarCtrlImpl

#if defined(_ODBC_DIRECT)
  #include "ToolBand7\UserXml.cpp"
#endif

  // Modify toolbar style according to new text options
  checkUpdate();

#if defined(USE_TICKER) || defined(_SERVAGE) || defined (_MB)
  m_ticker_id = (HANDLE)_beginthread(Ticker,0,this);
#endif
}

// get current HTML document helper
IHTMLDocument2Ptr CBandToolBarCtrl::GetHtmlDocument()
{
  LPDISPATCH pDispatch = 0;
  if(m_pBand)
    m_pBand->m_pWebBrowser->get_Document(&pDispatch);
  IHTMLDocument2Ptr pHtmlDoc2 = pDispatch;
  if(pDispatch)
    pDispatch->Release();
  return pHtmlDoc2;
}

// process search (after Enter press or "Go" button)
void CBandToolBarCtrl::RunSearch()
{
#ifdef SELECT_SEARCH_FROM_MENU
  if(Toolbar::Commands::Command* c  = m_toolbar->selectCommand(gAppendCommand+"Web"))
    c->apply(this);
  return;
#endif

  int idxCur = 0;
  int idx = zstring(getValue(_T("SearchUsing"),_T("0")))[0] - '0';

#ifdef SELECT_SEARCH_FROM_COMBO
  for(XML::vectorDel<Toolbar::Commands::Command*>::iterator c=m_toolbar->m_commands.m_commands.begin();c!=m_toolbar->m_commands.m_commands.end();c++)
  {
#else
  for(unsigned int b=0;b<m_toolbar->m_toolbarItems.size();b++)
  {
    zstring command = m_toolbar->m_toolbarItems[b]->m_command;
    Toolbar::Commands::Command* c0 = (m_toolbar->selectCommand(command));
    if(!c0)
      continue;
    Toolbar::Commands::Command** c = &c0;
#endif

    if((*c)->canAddToSelect())
    {
      if(idxCur==idx)
      {
        (*c)->apply(this);
        break;
      }
      idxCur++;
    }
  }
}

// menu item selected
LRESULT CBandToolBarCtrl::OnCommand(WORD /*wNotifyCode*/, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
  if(hWndCtl) // fix for combo box - don't know how to make it better
  {
    bHandled = false;
    return S_OK;
  }
  if(Toolbar::IMenuItem* item = m_toolbar->getMenuItem(wID))
  {
    if(Toolbar::Commands::Command* c = m_toolbar->selectCommand(item->m_command))
      c->apply(this);
  }
  return S_OK;
}
// button pressed
LRESULT CBandToolBarCtrl::OnButton(WORD /*wNotifyCode*/, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
  if(hWndCtl!=m_hWnd)
  {
    bHandled = FALSE;
    return S_OK;
  }

  unsigned int id = wID-WM_USER;
  if(id<m_toolbar->m_toolbarItems.size())
  {
    m_lastPresedButton = id;
    if( Toolbar::Commands::Command* command = m_toolbar->m_toolbarItems[id]->m_command_id)
    {
      if(IsButtonChecked(wID)!=FALSE)
        command->applyPresed(this);
      else
        command->apply(this);
    }
    m_toolbar->m_toolbarItems[id]->onCommand(this);
  }
  else
  {
#ifndef NO_FIND_BUTTTONS
    TCHAR cValue[1024];
    GetButtonText(wID,cValue);
    zstring s = cValue;

    int idx;
    while( (idx = s.find(_T('\'')))  != zstring::npos)
      s.erase(idx,1);

    replace(s,_T("\""),zstring(_T("")));
    FindText(s.c_str());
    MoveFocusToHTML();
#endif
  }
  return S_OK;
}

// set focus to current document helper
void CBandToolBarCtrl::MoveFocusToHTML()
{
  IHTMLDocument2Ptr pHtmlDoc = GetHtmlDocument();
  if(pHtmlDoc)
  {
    IHTMLWindow2Ptr win = 0;
    pHtmlDoc->get_parentWindow(&win);
    if(win)
      win->focus();
  }
}

// catch mouse over even 
LRESULT CBandToolBarCtrl::OnHookMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
  DWORD dwPoint = ::GetMessagePos();
  POINT point = { GET_X_LPARAM(dwPoint), GET_Y_LPARAM(dwPoint) };
  ScreenToClient(&point);
  unsigned int nHit = HitTest(&point);
  bHandled = FALSE;
  if(m_toolbar && nHit>=0 && nHit<m_toolbar->m_toolbarItems.size())
  {
    bHandled = m_toolbar->m_toolbarItems[nHit]->StopMouseEvents();
  }
  return S_OK;
}
// pre process keyboard input 
STDMETHODIMP CBandToolBarCtrl::TranslateAcceleratorIO(LPMSG pMsg)
{
  for(Toolbar::IBandEdits it(this);it;it++)
    if (it->IsFocus(GetFocus()))
      return it->TranslateAcceleratorIO(pMsg);
  return S_FALSE;
}

// process tooltip events
LRESULT CBandToolBarReflectorCtrl::OnToolTip(int,LPNMHDR lParam,BOOL&)
{
  LPTOOLTIPTEXT lpttt;
  lpttt = (LPTOOLTIPTEXT) lParam;
  lpttt->hinst = _Module.GetModuleInstance();

  unsigned int id = lpttt->hdr.idFrom - WM_USER;
  if(id<m_pBand->m_wndToolBar.m_toolbar->m_toolbarItems.size())
    lpttt->lpszText = const_cast<TCHAR*>(m_pBand->m_wndToolBar.m_toolbar->m_toolbarItems[id]->getToolTip().c_str());
  else
  {
    zstring s = m_pBand->m_wndToolBar.m_toolbar->m_settings.m_wordfindHint;
//      replace(s,"%s","");
    lpttt->lpszText = const_cast<TCHAR*>(s.c_str());
  }
  return S_OK;
}

// menu item selected
LRESULT CBandToolBarCtrlFake::OnCommand(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  if(Toolbar::Commands::Command* c = m_father->m_toolbar->selectCommand(m_father->m_toolbar->getMenuItem(wID)->m_command))
    c->apply(m_father);
  return 0;
}

// button with our logo pressed
LRESULT CBandToolBarCtrlFake::OnButton(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  m_father->m_toolbar->m_toolbarItems[wID-WM_USER]->m_command_id->apply(m_father);
  return 0;
}

// we should show drop down menu
LRESULT CBandToolBarCtrlFake::OnToolbarDropDown(LPNMHDR pnmh)
{
  NMTOOLBAR* ptb = (NMTOOLBAR *) pnmh;
  RECT rc;
  m_father->GetItemRect(m_father->CommandToIndex(ptb->iItem), &rc);
  m_father->MapWindowPoints(HWND_DESKTOP, (POINT *)&rc, 2);

  m_father->m_toolbar->m_toolbarItems[ptb->iItem-WM_USER]->processMenu(*this,rc.left, rc.bottom);
  return 0;
}

// On create event
LRESULT CBandToolBarCtrlFake:: OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
  CRect rc;
  GetWindowRect(rc);

  TBBUTTON* pTBBtn = (TBBUTTON*)_alloca(1 * sizeof(TBBUTTON));

  int com = WM_USER + m_id;

  int idx = 0;
#ifdef _DYNABAR
  idx = zstring(getValue(_T("SearchUsing"),_T("0")))[0] - '0';
#endif

  pTBBtn[0].iBitmap = idx;
  pTBBtn[0].idCommand = com;
  pTBBtn[0].fsState = TBSTATE_ENABLED;
  pTBBtn[0].fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE | BTNS_SHOWTEXT;
  pTBBtn[0].dwData = 0;
  pTBBtn[0].iString=0; //Set zstring to zero

  Toolbar::Button* item = (Toolbar::Button*)m_father->m_toolbar->m_toolbarItems[m_id];

  if(!item->m_menu.m_items.empty())
  {
    pTBBtn[0].fsStyle |= item->m_menu.m_type == _T("Single") ? BTNS_WHOLEDROPDOWN : TBSTYLE_DROPDOWN;
    if(item->m_menu.m_type != _T("None"))
      SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
  }
  else
  {
    //SetPadding(1,4);
  }

  AddStrings((item->m_caption + _T('\0')).c_str());

  SetButtonStructSize(sizeof(TBBUTTON));

  list.Create(m_size.cx,m_size.cy,ILC_COLORDDB|ILC_MASK,0,1);
  list2.Create(m_size.cx,m_size.cy,ILC_COLORDDB|ILC_MASK,0,1);
  list.Add(CBitmap(m_father->LoadImage(item->m_menu.m_img)),clr_format);
  list2.Add(CBitmap(m_father->LoadImage(item->m_menu.m_hot_img.empty() ? item->m_menu.m_img : item->m_menu.m_hot_img)),clr_format);

  SetHotImageList(list2);
  m_hImageList = list.m_hImageList;
  SetImageList(list);

  AddButtons(1, pTBBtn);

  //CSize butsize( rc.Width(), rc.Height()); SetButtonSize(butsize);

  AutoSize();
  return 0;
}

#ifndef NO_OPTIONS_PAGE
// on sel change event in option page
void CBandToolBarCtrl::OnSelChange( DISPID id, VARIANT* pVarResult )
{
   HRESULT hr;
   IHTMLDocument2Ptr pHtmlDoc = GetHtmlDocument();
   if( pHtmlDoc != NULL )
   {
     IHTMLWindow2Ptr  pWindow;
     IHTMLEventObjPtr pEvent;
     hr = pHtmlDoc->get_parentWindow(&pWindow);
     ATLASSERT( SUCCEEDED( hr ) );
     hr = pWindow->get_event(&pEvent);

     IHTMLElementPtr p;
     hr = pEvent->get_srcElement(&p);

     CComBSTR  id;
     p->get_id(&id);
                        
     for(unsigned int i=0;i<m_toolbar->m_settings.m_settings.size();i++)
       m_toolbar->m_settings.m_settings[i]->onSelChange(p,(const TCHAR*)_bstr_t(id));
  }
}

// on mouse down event in options page
void CBandToolBarCtrl::OnMouseDown( DISPID id, VARIANT* pVarResult )
{
  if(m_pBand->m_onOpen)
    return;

  IHTMLDocument2Ptr pHtmlDoc = GetHtmlDocument();
  if(pHtmlDoc)
  {
    IHTMLWindow2Ptr  pWindow; pHtmlDoc->get_parentWindow(&pWindow);if(pWindow==NULL) return;
    IHTMLEventObjPtr pEvent;  pWindow->get_event(&pEvent);         if(pEvent==NULL)  return;
    IHTMLElementPtr p;        pEvent->get_srcElement(&p);          if(p==NULL)       return;
    CComBSTR  id;
    p->get_id(&id);
    if(id)
    {
      for(unsigned int i=0;i<m_toolbar->m_settings.m_settings.size();i++)
        m_toolbar->m_settings.m_settings[i]->onClick(this,(const TCHAR*)_bstr_t(id));
      onOptionChange();
      UpdateAutoWords();
    }
  }
}
#endif

// hook keyboard events - to implement TAB to switch between toolbars, combo boxes
LRESULT CBandToolBarCtrl::OnHookKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	if(wParam == VK_TAB)
  {
    HWND hwnd = GetFocus();
    for(Toolbar::IBandEdits it(this);it;it++)
    {
      if (it->IsFocus(hwnd))
      {
        if(::GetKeyState(VK_SHIFT) & 0x80)
          it--;
        else
          it++;
        if(it)
          it->SetFocus2();
        else
          ::SetFocus(GetParent());
      	bHandled = TRUE;
        return S_OK;
      }
    }
    HWND w5 = ::GetParent(::GetParent(hwnd));
    HWND w6 = ::GetParent(w5);
    HWND w1 = GetParent();
    if(w1==w6 && w5!=m_hWnd)
    {
      if(m_toolbar->m_ctlBandEdits.size())
        m_toolbar->m_ctlBandEdits[0]->SetFocus();
    	bHandled = TRUE;
    }
  }
  return S_OK;
}

// add / remove auto generated "find" words
void CBandToolBarCtrl::UpdateAutoWords()
{
  zstring str0;
  for(Toolbar::IBandEdits it(this);it;it++)
    str0 += it->GetText() + " ";

  // Get the text in the edit box
  std::vector<zstring> v;
  replaceSpace(str0,v);

  EnableHighlight(v.size());

#ifndef NO_FIND_BUTTTONS
  if(!m_toolbar->m_settings.m_highlight.m_colors.size())
    return;

  int count = m_toolbar->m_toolbarItems.size();

  int c = GetButtonCount();

  int newcount =  v.size() - (c - count);

  for(unsigned int cc=c-1; cc >= count + v.size();cc--)
    DeleteButton(cc);

  if(newcount > 0)
  {
    TBBUTTON* pTBBtn = (TBBUTTON*)_alloca(newcount * sizeof(TBBUTTON));

    for(int i=0; i < newcount; i++)
    {
      int com = WM_USER + c + i;
      pTBBtn[i].iBitmap = (c - count + i) % m_toolbar->m_settings.m_highlight.m_colors.size();
      pTBBtn[i].idCommand = com;
      pTBBtn[i].fsState = TBSTATE_ENABLED;
      pTBBtn[i].fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE | BTNS_SHOWTEXT;
      pTBBtn[i].dwData = 0;
      pTBBtn[i].iString = 0;
    }
    AddButtons(newcount, pTBBtn);
  }

  BOOL hide = !isGetValue(_T("ShowFindButtons")) || v.size()==0;

  for(unsigned int i=0;i<v.size();i++)
  {
    TBInfo(this,count + i).text(const_cast<TCHAR*>(v[i].c_str())); 
    HideButton(WM_USER + count + i,hide);
  }

  //HideButton(m_toolbar->Find(_T("ShowFindButtons")),hide);

  calculateWidth();
#endif
}

// we should show drop down menu
LRESULT CBandToolBarCtrl::OnToolbarDropDown(int, LPNMHDR pnmh, BOOL& bHandled)
{
  NMTOOLBAR* ptb = (NMTOOLBAR *) pnmh;
  CRect rc = ptb->rcButton;
  MapWindowPoints(HWND_DESKTOP, (POINT *)&rc, 2);

  if(ptb->hdr.hwndFrom!=m_hWnd) // another toolbar
  {
    bHandled = false;
    return S_OK;
  }
  m_toolbar->m_toolbarItems[ptb->iItem-WM_USER]->processMenu(*this,rc.left, rc.bottom);
  return S_OK;
}

// process some custom procedures during combo line changes
bool onCombo(CBandToolBarCtrl* ctrl,int& iItem)
{
#ifdef _PROXY_CHANGE_TOOLBAR
  #include "Toolband26\ProxyChange.cpp"
#endif
#ifdef _DYNABAR
  #include "Toolband10\changeImageOnButton.inl"
#endif
#ifdef _BIBLE
  #include "Toolband28\onCombo.cpp"
#endif
  return true;
}

// update all controls after option changed
void CBandToolBarCtrl::onOptionChange()
{
/*  std::for_each(m_toolbar->m_toolbarItems.begin(),
           m_toolbar->m_toolbarItems.end(),
           std::bind2nd(std::mem_fun1(&Toolbar::ToolbarItem::onOptionChange),this));
*/
  for(unsigned int n = 0; n < m_toolbar->m_toolbarItems.size(); n++)
      m_toolbar->m_toolbarItems[n]->onOptionChange(this);
}

// "main" crop
CBandToolBarCtrl::CBandToolBarCtrl() : 
        m_pBand(0),m_lastPresedButton(0),m_toolbar(0),h_image(0),m_start_width(0),m_ticker_id(0)
{
}

// clear all resources 
CBandToolBarCtrl::~CBandToolBarCtrl()
{
  if(m_hWnd)  
    DestroyWindow();
  if(h_image)
    FreeLibrary(h_image);
}
        
void CBandToolBarCtrl::hideIfNeeded(int id,const zstring& command)
{
  if(!command.empty())
    HideButton(WM_USER+id,!isGetValue(command));
}

HBITMAP CBandToolBarCtrl::LoadImage(const zstring& str)
{
#ifndef _ODBC_DIRECT
    zstring img = str;
    checkFile(img);
    return AtlLoadBitmapImage(img.c_str(),loadFlag);
#else
    return (HBITMAP)::LoadImage(h_image, str.c_str(), IMAGE_BITMAP, 0, 0, loadFlag);
#endif
}

#if defined(BLUE_ON_OVER_TEXT) || defined(USE_TICKER)
DWORD CBandToolBarReflectorCtrl::OnItemPrePaint(int , LPNMCUSTOMDRAW pnmh)
{
	LPNMTBCUSTOMDRAW lpTBCustomDraw = (LPNMTBCUSTOMDRAW)pnmh;
	bool bDisabled = ((lpTBCustomDraw->nmcd.uItemState & CDIS_DISABLED) == CDIS_DISABLED);
	if(!bDisabled)
  {
    if(m_pBand->m_wndToolBar.m_toolbar) // on quit bug fix
      if(lpTBCustomDraw->nmcd.dwItemSpec-WM_USER<m_pBand->m_wndToolBar.m_toolbar->m_toolbarItems.size()) // generated words
        return m_pBand->m_wndToolBar.m_toolbar->m_toolbarItems[lpTBCustomDraw->nmcd.dwItemSpec-WM_USER]->OnItemPrePaint(&m_pBand->m_wndToolBar,lpTBCustomDraw);
  }
  return CDRF_DODEFAULT;
}
#endif

void CBandToolBarCtrl::OnSize(UINT wParam, CSize size)
{
#ifdef _SERVAGE
  if(m_toolbar && m_toolbar->m_toolbarItems.size() && size.cx)
  {
    ((Toolbar::Page*)(m_toolbar->m_toolbarItems[0]))->m_width = size.cx;
    onOptionChange();
  }
#endif
  SetMsgHandled(false);
}

LRESULT CPugiObjWrap::OnMenuItem(WPARAM wParam)
{
  WORD wFlags = HIWORD(wParam);
  WORD wID = LOWORD(wParam);
  if(wID==0 || MF_POPUP==(wFlags&MF_POPUP))
    return 0;
  root()->m_pBand->m_pWebBrowser->put_StatusText(_bstr_t(root()->m_toolbar->getMenuItem(wID)->m_hint.c_str()));
  return 0;
}

// CPugiObj methods
STDMETHODIMP CPugiObj::BeforeNavigate2(LPDISPATCH pDisp , VARIANT* URL,
                                       VARIANT* Flags, VARIANT* TargetFrameName,
                                       VARIANT* PostData, VARIANT* Headers, BOOL* Cancel)
{
  if(gurl.length()==0)
	 gurl = URL;
#ifdef _ECOACH
  #include "Toolband2\BeforeNavigate2.inl"
#endif
#ifdef  _ADRRESSS_BAR_REDIRECT
  #include "Toolband24\Redirect.cpp"
#endif
  return S_OK;
}

STDMETHODIMP CPugiObj::OnQuit()
{
  if(m_wndToolBar.m_ticker_id)
    TerminateThread(m_wndToolBar.m_ticker_id,0);
  m_wndToolBar.m_pBand = 0;
  delete m_wndToolBar.m_toolbar; m_wndToolBar.m_toolbar = 0;
  if(m_wndToolBar.font.m_hFont)
    m_wndToolBar.font.DeleteObject();
  DispEventUnadvise(m_pWebBrowser);
  return S_OK;
}

STDMETHODIMP CPugiObj::HasFocusIO()
{
  HWND hwnd = GetFocus();
  for(Toolbar::IBandEdits it(&m_wndToolBar);it;it++)
    if(it->IsFocus(hwnd))
      return S_OK;
  return S_FALSE;
}

STDMETHODIMP CPugiObj::TitleChange(BSTR Text)
{
#ifdef _2FIND
  #include "Toolband1\TitleChange.cpp"
#endif
  return S_OK;
}

STDMETHODIMP CPugiObj::OnDocumentComplete(IDispatch *pDisp, VARIANT *_url)
{
  if(m_wndToolBar.GetHtmlDocument())
    g_last_url = _bstr_t(*_url);
  return S_OK;
}

STDMETHODIMP CPugiObj::OnDownloadComplete()
{
  IHTMLDocument2Ptr pHtmlDoc2 = m_wndToolBar.GetHtmlDocument();
  if( pHtmlDoc2 == NULL ) return S_OK; // only if document is loaded

  CComBSTR  title;
  pHtmlDoc2->get_title(&title);

  CComBSTR  url;
  pHtmlDoc2->get_URL(&url);

#ifdef _SEARCH_EXPLORER
 #include "Toolband5\popup.cpp"
#endif
#ifdef _THELOCALSEARCH
 #include "Toolband19\popup.cpp"
#endif
#ifdef _SITE_SAFE
 #include "Toolband3\popup.cpp"
#endif

#ifndef NO_OPTIONS_PAGE
  if(zstring(_bstr_t(title)).find(zstring(optionsTittle))==zstring::npos)
    return S_OK;

  // register handler
  pHtmlDoc2->put_onclick(_variant_t(CHtmlEventObject<CBandToolBarCtrl>::CreateHandler(&m_wndToolBar, CBandToolBarCtrl::OnMouseDown, 1 )));

  IHTMLDocument3Ptr pHtmlDoc3 = pHtmlDoc2;
  m_onOpen = true;
  for(unsigned int i=0;i<m_wndToolBar.m_toolbar->m_settings.m_settings.size();i++)
    m_wndToolBar.m_toolbar->m_settings.m_settings[i]->onOpen(&m_wndToolBar,pHtmlDoc3);
  m_onOpen = false;
#endif
  return S_OK;
}

STDMETHODIMP CPugiObj::UIActivateIO(BOOL fActivate, LPMSG pMsg)
{
  if(fActivate && m_wndToolBar.m_toolbar->m_ctlBandEdits.size())
  	m_wndToolBar.m_toolbar->m_ctlBandEdits[0]->SetFocus();
	return S_OK;
}

#ifdef _ODBC_DIRECT
 #include "Toolband7\BandToolBarCtrl.cpp"
#endif
