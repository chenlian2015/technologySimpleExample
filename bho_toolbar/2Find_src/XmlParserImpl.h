/** $Header:: /ApoBase/XmlParserImpl.h 25    7.03.02 18:29 Zmike            $*
/* Contents-----------------------------------------------------------------**
**                                                                          **
**                                                                          **
$*                                                                          */
/*--------------------------------------------------------------------------**
** Designed by Mike Melnikov.                                               **
** COPYRIGHT (C) 2001 by ApoServer Crew of ApoSoft                          **
** All rights reserved.                                                     **
** -------------------------------------------------------------------------*/

#if !defined(AFX_XMLPARSER2DLG_H__ED375D19_9AE5_4F1D_9C49_4D681711DED0__INCLUDED_)
#define AFX_XMLPARSER2DLG_H__ED375D19_9AE5_4F1D_9C49_4D681711DED0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ApoBaseMembers.h"

#include <sstream>

#include "XmlParser.h"

////////////////////////////////////////////////////////////////////////////
// XmlParser dialog

namespace  XML
{
  typedef std::basic_string<TCHAR> zstring;

  // forward defenitions
  namespace XMLPrivate
  {
    class XmlParserProxyRead;
    class XmlParserProxySave;
  }
  namespace XMLPrivate
  {
      template <class T> struct Value2;
  }

  class XmlParserProxy;


  struct APOBASE_API XmlObject
  {
      virtual ~XmlObject() {}
      virtual void determine(XmlParserProxy& parser) = 0;
      virtual void end() {};
  };


  class APOBASE_API _XmlParserImpl : public XML::XmlParser
  {
  public:
    _XmlParserImpl(XmlObject* d) : 
        m_proxy(0),m_document(0),m_delete_document(false)  {init(d);}

    _XmlParserImpl(XmlObject* d,_bstr_t topName);

    virtual ~_XmlParserImpl() {if(m_delete_document) delete m_document;}

    void setLanguage(const _bstr_t& lang="en") {m_lang=lang;}

    void init(XmlObject* d,bool deletePrev=true) {if(deletePrev) delete m_document; m_document = d;}
 
#if defined(_ODBC_DIRECT)
    virtual void parse(const _bstr_t& inputStr ,bool isFile = true);
#endif

  protected:
    virtual void startNode(_bstr_t& str) {determine(str);}
    virtual void endNode() { read_pop_back();}

    virtual void processAttribute(_bstr_t& bstrName,_bstr_t& strValue);
    
    virtual void on_end_parse();

    _bstr_t&     getLanguage() {return m_lang;}

            void determine(_bstr_t& str);

    virtual void setLoadStr(_bstr_t ) = 0;

    XmlParserProxy*                    m_proxy;
    XmlObject*                         m_document;
    std::vector<MSXML::IXMLDOMNodePtr> m_curnodes;

  private:

    bool                               m_delete_document;

    void read_pop_back();

    _bstr_t                            m_lang;

    std::vector<XmlObject*>            m_current;
  };
  
  
  template<typename C, class S = std::basic_string<C > >
  class APOBASE_API XmlParserImplT : public _XmlParserImpl
  {
  public:
    XmlParserImplT(XmlObject* d) : _XmlParserImpl(d) {}

    XmlParserImplT(XmlObject* d,_bstr_t topName) : _XmlParserImpl(d,topName) {}

    virtual ~XmlParserImplT() {}

  protected:

    virtual void do_save();

    virtual void on_parse();

    S&           load()        {return m_currentStr;}
    virtual void setLoadStr(_bstr_t str) {m_currentStr = (const C*)str;}

  private:

#ifdef _XML_SAVE
    void do_save_member(XmlObject& t,const C* str);
    void do_save_attrib(const S& m_key, S& m_str);
    void do_save_string(S& s);

    void saveNode(const S& str);
    void saveText(const S& str); 
#endif
    void do_read_member(XmlObject& t,const C* str);
    bool do_read_attrib(const S& m_key, S& m_str);
    void do_read_string(S& s);

    S                           m_currentStr;

    friend class XmlParserProxy;
    friend class XMLPrivate::XmlParserProxyRead;
    friend class XMLPrivate::XmlParserProxySave;
  };

  typedef XmlParserImplT<TCHAR> XmlParserImpl;

  struct AtribValue{};

  class XmlParserProxy
  {
    protected:
      XmlParserProxy(XmlParserImpl& p) : m_p(p){}
      XmlParserImpl& m_p;
    public:
      virtual void do_attrib(const TCHAR* key, zstring& str) = 0;
      virtual void do_member(XmlObject& t,const TCHAR* str) = 0;
      virtual void do_string(zstring& s) = 0;
      virtual void do_int(int& s) = 0;
      virtual void do_float(float& s) = 0;
      virtual bool isRead() = 0;
      
      zstring      load()        {return static_cast<const TCHAR*>(_bstr_t(m_p.load().c_str()));}
      _bstr_t&     getLanguage() {return m_p.getLanguage();}
  };

  template <class T = void*>
  class determineMember
  {
    public:
    template <class OT>
    determineMember(OT& t,const TCHAR* str,XmlParserProxy& p,T* _t=0)
    {
       determine(t,str,p,_t,type(t));
    }
  };   

  namespace XMLPrivate
  {
      template <class T>
      struct Value2 : public XmlObject
      {
        Value2(T& v) : m_val(v){}
        T& m_val;
        virtual void determine(XmlParserProxy& p) {ATLASSERT(false);}
        virtual void end() {delete this;}
      };

      inline void Value2<zstring>::determine(XmlParserProxy& p) { p.do_string(m_val);}
      inline void Value2<int>    ::determine(XmlParserProxy& p) { p.do_int(m_val);}
      inline void Value2<float>  ::determine(XmlParserProxy& p) { p.do_float(m_val);}

#ifdef _XML_SAVE
      class XmlParserProxySave : public XmlParserProxy
      {
        virtual void do_attrib(const TCHAR* key, zstring& str) {m_p.do_save_attrib(key,str);}
        virtual void do_member(XmlObject& t,const TCHAR* str) {m_p.do_save_member(t,str);}
        virtual void do_string(zstring& s) {m_p.do_save_string(s);}
        virtual void do_int(int& i) {	static wchar_t buff[20];m_p.saveText((const TCHAR*)_bstr_t(_itow(i, buff, 10)));}
        virtual void do_float(float& f) {	std::stringstream str;str << f;m_p.saveText((const TCHAR*)_bstr_t(str.str().c_str()));}
        virtual bool isRead() { return false;}
        public:
          XmlParserProxySave(XmlParserImpl& p) : XmlParserProxy(p){}
      };
#endif
      class XmlParserProxyRead : public XmlParserProxy
      {
        virtual void do_attrib(const TCHAR* key, zstring& str) {Value2<zstring>* v = new Value2<zstring>(str);m_p.do_read_member(*v,key);delete v;}
        virtual void do_member(XmlObject& t,const TCHAR* str) {m_p.do_read_member(t,str);}
        virtual void do_string(zstring& s) {m_p.do_read_string(s);}
        virtual void do_int(int& i) {wchar_t *endptr=0; i=wcstol(_bstr_t(m_p.load().c_str()),&endptr,10);}
        virtual void do_float(float& f) {wchar_t *endptr=0; f=static_cast<float>(wcstod(_bstr_t(m_p.load().c_str()),&endptr));}
        virtual bool isRead() { return true;}
        public:
          XmlParserProxyRead(XmlParserImpl& p) : XmlParserProxy(p){}
      };

  }

  APOBASE_API XmlObject& xmlproxy(zstring &s);
  APOBASE_API XmlObject& xmlproxy(int     &i);
  APOBASE_API XmlObject& xmlproxy(float   &i);

  
  struct vec_t{};      template <class T> vec_t       type(vector<T>& ) {return vec_t();}
  struct common_t{};   inline             common_t    type(int& )       {return common_t();}
  struct Object_t{};   inline             common_t    type(float& )     {return common_t(); }
                       inline             common_t    type(zstring&   ) {return common_t(); }
                       inline             Object_t    type(XmlObject& ) {return Object_t(); }
  struct ObjectPtr_t{};inline             ObjectPtr_t type(XmlObject* ) {return ObjectPtr_t(); }

  template <class T>
  inline void determine(T& t,const TCHAR* str,XmlParserProxy& p,void*,common_t)
  {
    XML::XmlObject& o = xmlproxy(t);
    p.do_member(o,str);
    delete &o;
  }
  inline void determine(zstring& t,const TCHAR* str,XmlParserProxy& p,AtribValue*,common_t)
  {
    p.do_attrib(str,t);
  }
  template <class T>
  inline void determine(T& t,const TCHAR* str,XmlParserProxy& p,void*,Object_t)
  {
    p.do_member(t,str);
  }
  
  template <class T,class NewT>
  inline void determine(T& t,const TCHAR* str,XmlParserProxy& p,NewT* _t,ObjectPtr_t)
  {
#ifdef _XML_SAVE
    if(p.isRead())
    {
#endif
      if(str == p.load())
      {
        t = new NewT;
        throw t;
      }
#ifdef _XML_SAVE
    }
    else
    {
     if(t && (typeid(*t) == typeid(NewT)) )
       determineMember<>(*t,str,p);
    }
#endif
  }

  template <class T,typename NewT>
  inline void determineVec(vector<T>& vect,const TCHAR* str,XmlParserProxy& p,NewT* _t, common_t t_)
  {
#ifdef _XML_SAVE
    if(p.isRead())
    {
#endif
      if(p.load()==str)
      {
        vect.push_back(T());
        throw &xmlproxy(*vect.rbegin());
      }
#ifdef _XML_SAVE
    }
    else
    {
      for(int i=0;i<vect.size();i++)
      {
         determine(vect[i],str,p,_t,t_);
      }
    }
#endif
  }
  template <class T,typename NewT>
  inline void determineVec(vector<T>& vect,const TCHAR* str,XmlParserProxy& p,NewT* _t,Object_t t_)
  {
#ifdef _XML_SAVE
    if(p.isRead())
    {
#endif
      if(p.load()==str)
      {
        NewT* node = new NewT();
        vect.push_back(node);
        throw node;
      }
#ifdef _XML_SAVE
    }
    else
    {
      for(int i=0;i<vect.size();i++)
      {
         determine(vect[i],str,p,_t,ObjectPtr_t());
      }
    }
#endif
  }

  template <class T,typename NewT>
  inline void determine(vector<T>& vect,const TCHAR* str,XmlParserProxy& p,NewT* _t,vec_t)
  {
    determineVec(vect,str,p,_t,type(*_t));
  }


  // XmlParserImplT implementation

  template<typename C, class S >
  inline void XmlParserImplT<C,S>::on_parse()
  {
    m_proxy = new XMLPrivate::XmlParserProxyRead(*this);
  }

  template<typename C, class S >
  inline void XmlParserImplT<C,S>::do_save()
  {
  #ifdef _XML_SAVE
    m_proxy = new XMLPrivate::XmlParserProxySave(*this);
    m_document->determine(*m_proxy);
    m_curnodes.clear();
    delete m_proxy; m_proxy = 0;
  #endif
  }

  #ifdef _XML_SAVE

  template<typename C, class S >
  inline void XmlParserImplT<C,S>::saveNode(const S& str)
  {
    _bstr_t bstrName = str.c_str();

    MSXML::IXMLDOMElementPtr curnode = 
      m_pIXMLDOMDocument->createElement(bstrName);

    MSXML::IXMLDOMNodePtr m_curnode;
    if(m_curnodes.size())
    {
      m_curnode = m_curnodes[m_curnodes.size()-1];
      m_curnode = m_curnode->appendChild(curnode);
    }
    else
      m_curnode = m_pIXMLDOMDocument->appendChild(curnode);

    m_curnodes.push_back(m_curnode);
  }

  template<typename C, class S >
  inline void XmlParserImplT<C,S>::saveText(const S& str) 
  {
    (*m_curnodes.rbegin())->put_text(_bstr_t(str.c_str()));
  }

  // ATTRIB
  template<typename C, class S >
  inline void XmlParserImplT<C,S>::do_save_attrib(const S& m_key, S& m_str)
  {
    MSXML::IXMLDOMElementPtr pEle = (*m_curnodes.rbegin());
    _bstr_t bstrRootAttrName = m_key.c_str();
    _bstr_t val = m_str.c_str();
    pEle->setAttribute(bstrRootAttrName,_variant_t(val));
  }
  // MEMBER
  template<typename C, class S >
  inline void XmlParserImplT<C,S>::do_save_member(XmlObject& t,const C* str)
  {
    saveNode(str);
    t.determine(*m_proxy);
    m_curnodes.pop_back();
  }
  // STRING
  template<typename C, class S >
  inline void XmlParserImplT<C,S>::do_save_string(S& s)
  {
    saveText(s);
  }
  #endif

  // MEMBER
  template<typename C, class S >
  inline void XmlParserImplT<C,S>::do_read_member(XmlObject& t,const C* str)
  {
    if(load()==str)
      throw &t;
  }
  // STRING

  template<typename C, class S >
  inline void XmlParserImplT<C,S>::do_read_string(S& s)
  { 
    s = load();
  }

} // end namespace XML

#endif // !defined(AFX_XMLPARSER2DLG_H__ED375D19_9AE5_4F1D_9C49_4D681711DED0__INCLUDED_)
