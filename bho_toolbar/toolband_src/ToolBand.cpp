// ToolBand.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f ToolBandps.mk in the project directory.

// ============================================================================
// Sample by Rashid Thadha Aug 2001
// ============================================================================

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "ToolBand.h"

#include "ToolBand_i.c"
#include "ToolBandObj.h"


CMyComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_ToolBandObj, CToolBandObj)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(ObjectMap, hInstance, &LIBID_TOOLBANDLib);
        DisableThreadLibraryCalls(hInstance);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
        _Module.Term();
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

STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
	HKEY hKeyLocal = NULL;
	RegCreateKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Internet Explorer\\Toolbar"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKeyLocal, NULL);
	RegSetValueEx(hKeyLocal, _T("{0E1230F8-EA50-42A9-983C-D22ABC2EED3B}"), 0, REG_BINARY, NULL, 0);
	RegCloseKey(hKeyLocal);

    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	HKEY hKeyLocal = NULL;
	RegCreateKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Internet Explorer\\Toolbar"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKeyLocal, NULL);
	RegDeleteValue(hKeyLocal, _T("{0E1230F8-EA50-42A9-983C-D22ABC2EED3B}"));
	RegCloseKey(hKeyLocal);

    return _Module.UnregisterServer(TRUE);
}


