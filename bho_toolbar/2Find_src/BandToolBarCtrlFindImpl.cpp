// BandToolBarCtrlFindImpl.cpp: implementation of the CBandToolBarCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToolBand.h"
#include "BandToolBarCtrl.h"
#include "ToolBandObj.h"
#include <algorithm>
  
#include "ATLHOST.H"

void CBandToolBarCtrl::ClearSearchResults(_bstr_t searchID)
{
  ClearSearchResults(GetHtmlDocument(),searchID);
}

void CBandToolBarCtrl::FindText(_bstr_t searchText, long lFlags, BOOL bNNF /* =FALSE  (for internal use)*/)
{
  if(IHTMLDocument2 *lpHtmlDocument = GetHtmlDocument())
  {
    FindText(lpHtmlDocument,searchText,lFlags,bNNF);

#ifdef _2FIND
    zstring s = searchText;
    std::reverse(s.begin(),s.end());
    FindText(lpHtmlDocument,s.c_str(),lFlags,bNNF);
#endif
  }
}
                               
void CBandToolBarCtrl::FindText2(_bstr_t searchText, long lFlags , _bstr_t matchStyle, _bstr_t searchID )
{
  FindText2(GetHtmlDocument(),searchText,lFlags,matchStyle,searchID);
}


/////////////////////////////////////////////////////////////////////////////

void CBandToolBarCtrl::ClearSearchResults(IHTMLDocument2 *lpHtmlDocument,_bstr_t searchID)
{
  if(!lpHtmlDocument)
    return;

  CComBSTR testid(searchID.length()+1,(const char*)searchID);
	CComBSTR testtag(5,"SPAN");

	IHTMLElementCollection *lpAllElements;
	lpHtmlDocument->get_all(&lpAllElements);
	//lpHtmlDocument->Release();

  IHTMLBodyElement *lpBody;
	IHTMLElement *lpBodyElm;
	lpHtmlDocument->get_body(&lpBodyElm);
  if(!lpBodyElm)
    return;
	//lpHtmlDocument->Release();

  lpBodyElm->QueryInterface(IID_IHTMLBodyElement,(void**)&lpBody);
  lpBodyElm->Release();
  
  if(lpBody)
  {
    IUnknown *lpUnk;
	  IEnumVARIANT *lpNewEnum;
	  if (SUCCEEDED(lpAllElements->get__newEnum(&lpUnk)) && lpUnk != NULL)
	  {
		  lpUnk->QueryInterface(IID_IEnumVARIANT,(void**)&lpNewEnum);
		  VARIANT varElement;
		  IHTMLElement *lpElement;


		  while (lpNewEnum->Next(1, &varElement, NULL) == S_OK)
		  {
			  _ASSERTE(varElement.vt == VT_DISPATCH);
			  varElement.pdispVal->QueryInterface(IID_IHTMLElement,(void**)&lpElement);
			  if (lpElement)
			  {
				  CComBSTR id;
				  CComBSTR tag;
				  lpElement->get_id(&id);
				  lpElement->get_tagName(&tag);
				  if((id==testid)&&(tag==testtag))
				  {
					  BSTR innerText;
					  lpElement->get_innerHTML(&innerText);
					  lpElement->put_outerHTML(innerText);
				  }
			  }
			  VariantClear(&varElement);
		  }
	  }
  }
  {
    IHTMLFramesCollection2 *frames = 0;
    lpHtmlDocument->get_frames(&frames);
  
    if(frames)
    {
      long p;
      frames->get_length(&p);
      for(long f=0;f<p;f++)
      {
        variant_t frame;
        frames->item(&_variant_t(f),&frame);
        if((IDispatch*)frame)
        {
      	  IHTMLWindow2  *elem = 0;
      	  ((IDispatch*)frame)->QueryInterface(IID_IHTMLWindow2, (void**)&elem);
          if(elem)
          {
            IHTMLDocument2*  lpHtmlDocument = NULL;
            elem->get_document(&lpHtmlDocument);
            ClearSearchResults(lpHtmlDocument,searchID);
          }
        }
      }
    }
  }
}

void CBandToolBarCtrl::FindText(IHTMLDocument2 *lpHtmlDocument,_bstr_t searchText, long lFlags, BOOL bNNF)
{
  if(!lpHtmlDocument)
    return;

  static _bstr_t sLastSearch;
	static BSTR    lastBookmark = 0;

	if(sLastSearch != searchText)
		lastBookmark = NULL;
	sLastSearch = searchText;
	
	IHTMLElement *lpBodyElm;
	IHTMLBodyElement *lpBody;
	IHTMLTxtRange *lpTxtRange;

	lpHtmlDocument->get_body(&lpBodyElm);
  if(!lpBodyElm)
    return;
	//lpHtmlDocument->Release();

  lpBodyElm->QueryInterface(IID_IHTMLBodyElement,(void**)&lpBody);
	lpBodyElm->Release();
  
  {
    IHTMLFramesCollection2 *frames = 0;
    lpHtmlDocument->get_frames(&frames);

    if(frames)
    {
      long p;
      frames->get_length(&p);
      for(long f=0;f<p;f++)
      {
        variant_t frame;
        frames->item(&_variant_t(f),&frame);
        if((IDispatch*)frame)
        {
      	  IHTMLWindow2  *elem = 0;
      	  ((IDispatch*)frame)->QueryInterface(IID_IHTMLWindow2, (void**)&elem);
          if(elem)
          {
            IHTMLDocument2*  lpHtmlDocument = NULL;
            elem->get_document(&lpHtmlDocument);
            FindText(lpHtmlDocument,searchText,lFlags,bNNF);
          }
        }
      }
    }
  }
  if(lpBody)
  {
	  lpBody->createTextRange(&lpTxtRange);
	  lpBody->Release();

	  CComBSTR search(searchText.length()+1,(LPCTSTR)searchText);
	  bool bFound,bTest;
	  long t;

	  if(lastBookmark!=NULL)
	  {
		  lpTxtRange->moveToBookmark(lastBookmark,(VARIANT_BOOL*)&bTest);
		  if(!bTest)
		  {
			  lastBookmark=NULL;
			  lpTxtRange->moveStart((BSTR)CComBSTR("Textedit"),1,&t);
			  lpTxtRange->moveEnd((BSTR)CComBSTR("Textedit"),1,&t);
		  } else
		  {
			  lpTxtRange->moveStart((BSTR)CComBSTR("Character"),1,&t);
			  lpTxtRange->moveEnd((BSTR)CComBSTR("Textedit"),1,&t);
		  }
	  } else
	  {
		  lpTxtRange->moveStart((BSTR)CComBSTR("Textedit"),0,&t);
		  lpTxtRange->moveEnd((BSTR)CComBSTR("Textedit"),1,&t);
	  }
	  lpTxtRange->findText((BSTR)search,0,lFlags,(VARIANT_BOOL*)&bFound);

	  if(!bFound)
	  {
		  if(lastBookmark==NULL && !bNNF)
		  {
			  //::MessageBox(0,searchText,L"Cannot find the string: ",0);
		  } else if(lastBookmark!=NULL)
		  {
			  lastBookmark = NULL;
			  FindText(searchText,lFlags,TRUE);
		  }
	  } else
	  {
		  if(lpTxtRange->getBookmark(&lastBookmark)!=S_OK)
			  lastBookmark=NULL;

      VARIANT_BOOL pfRet;
      HRESULT h0= lpTxtRange->queryCommandEnabled(_bstr_t("Bold"),& pfRet);
      if(h0==0 && pfRet==VARIANT_TRUE)
      {
        HRESULT h1 = lpTxtRange->select();
        HRESULT h2 = lpTxtRange->scrollIntoView(TRUE);
      }
	  }

	  lpTxtRange->Release();
  }
}

void CBandToolBarCtrl::FindText2(IHTMLDocument2 *lpHtmlDocument,_bstr_t searchText, long lFlags /* =2 */, _bstr_t matchStyle, _bstr_t searchID )
{
  if(!lpHtmlDocument)
    return;

	IHTMLElement *lpBodyElm;
	IHTMLBodyElement *lpBody;
	IHTMLTxtRange *lpTxtRange;

	lpHtmlDocument->get_body(&lpBodyElm);
  if(!lpBodyElm)
    return;
	//lpHtmlDocument->Release();
	
  lpBodyElm->QueryInterface(IID_IHTMLBodyElement,(void**)&lpBody);
  lpBodyElm->Release();
  
  if(lpBody)
  {
	  lpBody->createTextRange(&lpTxtRange);
	  lpBody->Release();

	  CComBSTR html;
	  CComBSTR newhtml;
	  CComBSTR search(searchText.length()+1,(LPCTSTR)searchText);

	  long t;
	  VARIANT_BOOL bFound;
    HRESULT h;
	  while(lpTxtRange->findText(search,0,lFlags,&bFound)==S_OK && bFound==VARIANT_TRUE)
	  {
		  newhtml.Empty();
		  h = lpTxtRange->get_htmlText(&html);
		  newhtml.Append("<span id='");
		  newhtml.Append((LPCTSTR)searchID);
		  newhtml.Append("' style='");
		  newhtml.Append((LPCTSTR)matchStyle);
		  newhtml.Append("'>");
		  if(searchText==_bstr_t(" "))
			  newhtml.Append("&nbsp;"); // doesn't work very well, but prevents (some) shit
		  else 
			  newhtml.AppendBSTR(html);
		  newhtml.Append("</span>");

      VARIANT_BOOL pfRet;
      HRESULT h0= lpTxtRange->queryCommandEnabled(_bstr_t("Bold"),& pfRet);
      if(h0==0 && pfRet==VARIANT_TRUE)
      {
        h = lpTxtRange->pasteHTML(newhtml);
      }
   	  h = lpTxtRange->moveStart((BSTR)_bstr_t("Character"),1,&t);
	    h = lpTxtRange->moveEnd((BSTR)CComBSTR("Textedit"),1,&t);
	  }

	  lpTxtRange->Release();
  }
  {
    IHTMLFramesCollection2 *frames = 0;
    lpHtmlDocument->get_frames(&frames);
  
    if(frames)
    {
      long p;
      frames->get_length(&p);
      for(long f=0;f<p;f++)
      {
        variant_t frame;
        frames->item(&_variant_t(f),&frame);
        if((IDispatch*)frame)
        {
      	  IHTMLWindow2  *elem = 0;
      	  ((IDispatch*)frame)->QueryInterface(IID_IHTMLWindow2, (void**)&elem);
          if(elem)
          {
            IHTMLDocument2*  lpHtmlDocument = NULL;
            HRESULT ff = elem->get_document(&lpHtmlDocument);
            FindText2(lpHtmlDocument,searchText,lFlags,matchStyle,searchID);
          }
        }
      }
    }
  }
}
