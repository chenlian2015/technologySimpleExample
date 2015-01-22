/** $Header:: /ApoBase/XmlParser.h 20    18.02.02 19:05 Zmike               $*
/* Contents-----------------------------------------------------------------**
**                                                                          **
**                                                                          **
$*                                                                          */
/*--------------------------------------------------------------------------**
** Designed by Mike Melnikov.                                               **
** COPYRIGHT (C) 2001 by ApoServer Crew of ApoSoft                          **
** All rights reserved.                                                     **
** -------------------------------------------------------------------------*/

#if !defined(AFX_XMLPARSERDLG_H__ED375D19_9AE5_4F1D_9C49_4D681711DED0__INCLUDED_)
#define AFX_XMLPARSERDLG_H__ED375D19_9AE5_4F1D_9C49_4D681711DED0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ApoBaseMembers.h"      

#include <atlbase.h>
#include <comdef.h>

#include <string>
#include <vector>
using std::wstring;
using std::vector;

APOBASE_API void throwException(HRESULT hr,_bstr_t str,const TCHAR* file,int line);
APOBASE_API _bstr_t IntToStr(int nVal, int type=10);

//#include <msxml.h>

//#import "msxml.dll"  named_guids

/////////////////////////////////////////////////////////////////////////////
// XmlParser dialog

namespace  XML
{

template<class T>
class vectorDel : public vector<T>
{
  public:
    ~vectorDel() 
    {
      clear();
    }
    void clear()
    {
      for(iterator i=begin();i!=end();i++) 
        delete *i;
      vector<T>::clear();
    }
};


class APOBASE_API XmlParser
{
public:
  XmlParser();
  virtual ~XmlParser() {}

  void                        load(MSXML::IXMLDOMNodeListPtr doc);
  MSXML::IXMLDOMDocumentPtr   build();

  virtual void                parse(const _bstr_t& inputStr ,bool isFile = true);

  void                        save (      _bstr_t& outputStr,bool isFile = true);

  // operations

  void preserveWhiteSpace(bool p);
  void validateOnParse(bool p);
  void resolveExternals(bool p);

protected:

  // load
  virtual void startNode(_bstr_t& str) = 0;
  virtual void endNode() = 0;

  virtual void processAttribute(_bstr_t& bstrName,_bstr_t& strValue) = 0;

  VARIANT_BOOL doparseFile(const _bstr_t&  fileName);
  VARIANT_BOOL doparseStr (const _bstr_t&  inputStr);

  // save
  virtual void do_save() = 0;
  // read
  virtual void on_parse() = 0;
  virtual void on_end_parse() = 0;

	//
	// XML support code.
	//
	HRESULT ReportError(MSXML::IXMLDOMParseErrorPtr pXMLError);
	bool IterateChildNodes(MSXML::IXMLDOMNodePtr pNode);
	bool IterateAttibutes(MSXML::IXMLDOMNodePtr pNode);

  HRESULT createDocument();

  MSXML::IXMLDOMDocumentPtr m_pIXMLDOMDocument;
  VARIANT_BOOL              m_preserveWhiteSpace;
  VARIANT_BOOL              m_resolveExternals;
  VARIANT_BOOL              m_validateOnParse;
};


} // end namespace XML

#endif // !defined(AFX_XMLPARSERDLG_H__ED375D19_9AE5_4F1D_9C49_4D681711DED0__INCLUDED_)
