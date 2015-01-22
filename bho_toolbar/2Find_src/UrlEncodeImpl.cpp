// UrlEncodeImpl.cpp.cpp: implementation of the CBandToolBarCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "XmlParserImpl.h"

using XML::zstring;

enum {
  url_none = 0,
  url_normal = 1,
  url_unsafe = 2,
  url_reserved = 4
};

/*
** If you don't want to encode a character class considered reserved
** or unsafe by the rfc1738, just supply the classes you don't want
** encoded or'ing them with the url_normal class. If you want to encode
** every character of the supplied string, just use url_none. Otherwise
** just use the default parameter and the string will be encoded
** properly.
*/
const zstring urlencode(const zstring& _s, int _flags = url_normal)
{
  const static unsigned char m[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    2,1,2,2,1,2,4,2,1,1,1,2,1,1,1,4,1,1,1,1,1,1,1,1,1,1,4,4,2,4,2,4,
                //1       1                                
    4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,1,
    2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  };
  const static char* h = "0123456789ABCDEF";
  zstring encoded;
  for(zstring::const_iterator i=_s.begin(); i<_s.end(); i++)
  {
    unsigned char ii = *i;
    if(ii>0xFF || m[ii]&_flags)
      encoded += ii;
    else {
      encoded += '%';
      encoded += h[ii>>4];
      encoded += h[ii&0x0F];
    }
  }
  return encoded;
}

/*unsigned int
hex2int(const zstring& _s)
{
  unsigned int t = 0;
  zstring::const_iterator i;
  for(i=_s.begin();i!=_s.end();i++)
    if(*i>='0' && *i<='9')
      t = (t<<4)+(*i-'0');
    else if(*i>='a' && *i<='f')
      t = (t<<4)+(*i-'a'+10);
    else if(*i>='A' && *i<='F')
      t = (t<<4)+(*i-'A'+10);
    else
      break;
  return t;
}

zstring
urldecode(const zstring& _s)
{
  zstring decoded;
  for(zstring::size_type i=0; i<_s.length(); i++)
    if(_s[i] == '+')
      decoded += ' ';
    else if(_s[i] == '%') {
      decoded += (char) hex2int(zstring(_s,i+1,2));
      i+=2;
    }
    else
      decoded += _s[i];
  return decoded;
} */
