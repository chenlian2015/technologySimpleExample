// Pugi.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f Pugips.mk in the project directory.

// ============================================================================
// Used Sample by Rashid Thadha Aug 2001
// Modified by Mike Melnikov
// ============================================================================

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "ToolBand.h"

#include "ToolBand_i.c"
#include "..\ToolBandObj.h"

                                          
CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
 OBJECT_ENTRY(CLSID_PugiObj, CPugiObj)
#ifdef ADD_CONTEXT_ITEM
  OBJECT_ENTRY(CLSID_ContextItem, CContextItem)
#endif
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

void MyAtExit()
{
	__asm int 3
}

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
		///////////////////////////////////////////////////
		//TT	Set an exit handler.
		//TT	This will permit us to step through the app's exit
			//atexit(MyAtExit);

 ///////////////////////////////////////////////////
//TT	Tell MSVC to dump memory leaks *after* any
//		global dtors are called
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		_Module.Init(ObjectMap, hInstance, &LIBID_PugiLib);
        AtlAxWinInit();
        DisableThreadLibraryCalls(hInstance);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
	{
        _Module.Term();
///////////////////////////////////////////////////
//TT	Dump memory leaks before _tWinMain returns
//TT	Note the global dtors have not yet been called
//    	_CrtDumpMemoryLeaks();
	}
    return TRUE;    // ok  
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{                                  
    return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

void ShowBand();
void UNregServer();

STDAPI DllRegisterServer(void)         
{                                 
  // registers object, typelib and all interfaces in typelib
  _Module.RegisterServer(TRUE);

  ShowBand();
                                       
  return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry
                               
STDAPI DllUnregisterServer(void)
{
  UNregServer();

  return _Module.UnregisterServer(TRUE);
}


