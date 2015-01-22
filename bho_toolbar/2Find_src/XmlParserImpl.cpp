/** $Header:: /ApoBase/XmlParserImpl.cpp 15    7.03.02 18:29 Zmike          $*
/* Contents-----------------------------------------------------------------**
**                                                                          **
**                                                                          **
$*                                                                          */
/*--------------------------------------------------------------------------**
** Designed by Mike Melnikov.                                               **
** COPYRIGHT (C) 2001 by ApoServer Crew of ApoSoft                          **
** All rights reserved.                                                     **
** -------------------------------------------------------------------------*/

#include "stdafx.h"

#include "XmlParserImpl.h"

/////////////////////////////////////////////////////////////////////////////
// XmlParser 

/////////////////////////////////////////////////////////////////////////////
// XmlParser message handlers
//using XML::XmlParserImpl;

void Uncompress(char *sourcefile,char *destfile);


namespace XML 
{

struct DocumentImpl : public XmlObject
{
  DocumentImpl(XmlObject& obj,_bstr_t name) : m_obj(obj),m_name(name) {}
  
  _bstr_t m_name;
  XmlObject& m_obj;

	virtual void determine(XmlParserProxy& p)
	{
		XML::determineMember<>(m_obj,m_name,p);
	}
};
  
_XmlParserImpl::_XmlParserImpl(XmlObject* d,_bstr_t topName) : m_proxy(0),m_document(0) 
{
  init(new DocumentImpl(*d,topName));
  m_delete_document = true;
}

#if defined(_ODBC_DIRECT)
void _XmlParserImpl::parse(const _bstr_t& _inputStr ,bool isFile)
{
  _bstr_t inputStr = _inputStr + ".tmp";

  try
  {
    Uncompress(_inputStr,inputStr);
  }
  catch(...)
  {
   //::MessageBox(0,"Failed to uncompress " + _inputStr,"Error",0);
  }

  XmlParser::parse(inputStr,isFile);

  DeleteFile(inputStr);
} 
#endif



void _XmlParserImpl::determine(_bstr_t& str)
{
  static _bstr_t documentID = "#document - document";

  if(m_current.size())
  {
    XmlObject* cur = m_current[m_current.size()-1];
    if(cur)
    {
      setLoadStr(str);
      try
      {
        cur->determine(*m_proxy);
      }
      catch(XmlObject* newObject)
      {
        m_current.push_back(newObject);
        return;
      }
    }
    m_current.push_back(NULL);
  }
  else if(str==documentID)
  {
    m_current.push_back(m_document);
  }
  else
  {
    m_current.push_back(NULL);
  }
}

void _XmlParserImpl::read_pop_back()
{ 
  if(*m_current.rbegin()) 
    (*m_current.rbegin())->end();
  m_current.pop_back(); 
}

void _XmlParserImpl::processAttribute(_bstr_t& bstrName,_bstr_t& strValue)
{
  determine(bstrName);
  determine(strValue);
  read_pop_back();
  read_pop_back();
}

void _XmlParserImpl::on_end_parse()
{
  delete m_proxy; 
  m_proxy = 0;
}

// move inlines here
XML::XmlObject& XML::xmlproxy(zstring &s) {return *(new XMLPrivate::Value2<zstring>(s));}
XML::XmlObject& XML::xmlproxy(int    &i)  {return *(new XMLPrivate::Value2<int>(i));}
XML::XmlObject& XML::xmlproxy(float  &i)  {return *(new XMLPrivate::Value2<float>(i));}

}
