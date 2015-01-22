
#include <wininet.h>
#include <iostream>

#define MAX_INPUT_LEN 65000
#define CONTENT_TYPE_STR "Content-type: application/x-www-form-urlencoded"

//text/xml\r\n"

bool ErrorOut  ( DWORD dError,const  TCHAR * szCallingFunc);

bool _Post(_bstr_t server, _bstr_t url, _bstr_t proxy, _bstr_t user, _bstr_t password, _bstr_t data, _bstr_t method, _bstr_t pathTo, _bstr_t& out)
{
	int port = 0;
	bool ssl = false;


  HINTERNET hOpen, hConnect, hReq;
  DWORD  dwSize, dwCode;
  DWORD  dwFlags = INTERNET_FLAG_RELOAD | 
                   INTERNET_FLAG_NO_CACHE_WRITE |
                   INTERNET_FLAG_KEEP_CONNECTION |
				   INTERNET_FLAG_NO_COOKIES;
  
  char *lpBuffer, szData[51];
  INTERNET_PORT dwPort;

  wchar_t* accept[2]={L"*/*",0}; 
  
  if(proxy.length()==0)     
  {  
    // No proxy was specified
    if ( !(hOpen = InternetOpen ( "HeadDump",  LOCAL_INTERNET_ACCESS , NULL, 0, 0) ) )
    {
      ErrorOut ( GetLastError(), "InternetOpen");
      return false;
    }
  }
  else
  {
    // Use proxy server
    if ( !(hOpen = InternetOpen ( "HeadDump",  CERN_PROXY_INTERNET_ACCESS, proxy, NULL, 0) ) )
    {
      ErrorOut ( GetLastError(), "InternetOpen");
      return false;
    }
  }
  
  
  if(ssl)
  {
    dwPort = INTERNET_DEFAULT_HTTPS_PORT;
    dwFlags |= INTERNET_FLAG_SECURE | 
      INTERNET_FLAG_IGNORE_CERT_CN_INVALID |
      INTERNET_FLAG_IGNORE_CERT_DATE_INVALID  ;
  }
  else
    dwPort = INTERNET_DEFAULT_HTTP_PORT;

  if(port)
    dwPort = port;
  
  
  if ( !(hConnect = InternetConnect ( hOpen, server, dwPort, user,  password, INTERNET_SERVICE_HTTP, 0  , 1) ) )
  {
    ErrorOut (GetLastError(), "InternetConnect");
    return false;
  }
  
  if ( !(hReq = HttpOpenRequest (hConnect, method, url, "HTTP/1.0", "", (const char**) accept, dwFlags ,1 )))
  {
    ErrorOut (GetLastError(), "HttpOpenRequest");
    return false;
  }
 
   
again:
  //Resolver::Trace("Send Request = " + data);
  std::string header = CONTENT_TYPE_STR;
	//header += "\r\n";

	std::string _data = data;

  if ( !HttpSendRequest (hReq, // handle from HttpOpenRequest
    header.c_str(), -1, (void*) _data.c_str(), _data.length()) ) 
  {
    ErrorOut (GetLastError(), "HttpSendRequest");
    
    dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA                 |
      SECURITY_FLAG_IGNORE_REVOCATION        |   
      SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTP  |
      SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTPS |
      SECURITY_FLAG_IGNORE_CERT_DATE_INVALID |
      SECURITY_FLAG_IGNORE_CERT_CN_INVALID;
    
    if ( !InternetSetOption (hReq,  
      INTERNET_OPTION_SECURITY_FLAGS,
      &dwFlags, 
      sizeof (dwFlags) ) )
    {
      ErrorOut(GetLastError(),"InternetSetOptionFailed");
      return false;
    }
  }
  
  dwSize = sizeof (DWORD) ;  
  if ( !HttpQueryInfo (hReq, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwCode, &dwSize, NULL))
  {
    ErrorOut (GetLastError(), "HttpQueryInfo");
    return false;
  }
  
  if ( dwCode == HTTP_STATUS_DENIED || dwCode == HTTP_STATUS_PROXY_AUTH_REQ)
  {  
    
    // We have to read all outstanding data on the Internet handle
    // before we can resubmit request. Just discard the data.
    do
    {
      InternetReadFile (hReq, (LPVOID)szData, 50, &dwSize);
    }
    while (dwSize != 0);
    
    if(user.length())
    {
      // This is a secure page.

      if ( !InternetSetOption (hConnect, INTERNET_OPTION_USERNAME, (LPVOID)(char*)user, user.length() ))
      {
        ErrorOut(GetLastError(),"InternetSetOptionFailed");
        return false;
      }
      
      if ( !InternetSetOption (hConnect, INTERNET_OPTION_PASSWORD, (LPVOID)(char*)password, password.length() ))
      {
        ErrorOut(GetLastError(),"InternetSetOptionFailed");
        return false;
      }
      goto again;    
    }
    else
    {
      if ( InternetErrorDlg (GetDesktopWindow(),
        hReq, 
        ERROR_INTERNET_INCORRECT_PASSWORD,
        FLAGS_ERROR_UI_FILTER_FOR_ERRORS |
        FLAGS_ERROR_UI_FLAGS_GENERATE_DATA |
        FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS,
        NULL) == ERROR_INTERNET_FORCE_RETRY)
        goto again;
    }
    
  }
  
  
  // First time we will find out the size of the headers.
  HttpQueryInfo (hReq,HTTP_QUERY_RAW_HEADERS_CRLF,  NULL, &dwSize, NULL);
  lpBuffer =  new char [dwSize + 1 ];
  
  // Now we call HttpQueryInfo again to get the headers.
  if (!HttpQueryInfoA (hReq,HTTP_QUERY_RAW_HEADERS_CRLF, (LPVOID) lpBuffer,
    &dwSize, NULL))
  {
    ErrorOut (GetLastError(), "HttpQueryInfo");
    return false;
  }
  *(lpBuffer + dwSize) = '\0';
  
  HANDLE hFile = INVALID_HANDLE_VALUE;
  if(pathTo.length())
    hFile = CreateFile (pathTo,GENERIC_WRITE,FILE_SHARE_READ,0,CREATE_ALWAYS,FILE_FLAG_WRITE_THROUGH,0);

  do
  {
    if (!InternetReadFile (hReq, (LPVOID)szData, 50, &dwSize) )
    {
      ErrorOut (GetLastError (), "InternetReadFile");
      return false;
    }
    if (dwSize != 0) 
    {
      // dump text to the screen
      szData[dwSize] = '\0';
      out += szData;
      DWORD res;
      if (hFile != INVALID_HANDLE_VALUE)
        WriteFile(hFile, szData, dwSize, &res, 0);
    }
  }
  while (dwSize);

  if (hFile != INVALID_HANDLE_VALUE)
    CloseHandle(hFile);
  
  if (!InternetCloseHandle (hReq) )
  {
    ErrorOut (GetLastError (), "CloseHandle on hReq");
    return false;
  }
  if (!InternetCloseHandle (hConnect) )
  {
    ErrorOut (GetLastError (), "CloseHandle on hConnect");
    return false;
  }
  if (!InternetCloseHandle (hOpen) )
  {
    ErrorOut (GetLastError (), "CloseHandle on hOpen");
    return false;
  }
  
  
  delete [] lpBuffer;
  
  return true;
}




