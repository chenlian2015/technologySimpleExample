// ThreadMessage.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ThreadMessage.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

volatile bool bThreadOn = true;

WCHAR bufForChild[MAX_PATH] = {0};
WCHAR buf[MAX_PATH] = {0};

RECT rect = {0};
HWND hWndMain = NULL;
DWORD IDMainThread = 0;
DWORD IDChild = 0;
int nPaint = 0;
int nGetParentThreadMessageTimes = 0;
int nDealChildThreadMessage = 0;

bool DealChildThreadMessage(const MSG &msg);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_THREADMESSAGE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_THREADMESSAGE));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (DealChildThreadMessage(msg))
		{
			continue;
		}

		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{

			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
	}

	return (int) msg.wParam;
}


bool DealChildThreadMessage(const MSG &msg)
{
	if (WM_USER + 2 != msg.message)
	{
		return false;
	}

	nDealChildThreadMessage++;

	if (PostThreadMessageW(IDChild, WM_USER+1, 2, 1))
	{
		wsprintfW(bufForChild, L"DealChildThreadMessage %d times", nDealChildThreadMessage);
		
	}
	
	InvalidateRect(hWndMain, &rect, TRUE);

	return true;
}


 DWORD __stdcall  ThreadStartAddress(LPVOID p)
{
	DWORD idMainThread = *(DWORD*)p;
	
	
	while (bThreadOn)
	{
		PostThreadMessageW(idMainThread, WM_USER+2, 1, 2);
		Sleep(1000);
		
		
		MSG msg;
		if(GetMessageW(&msg, NULL, 0, 0) && 0!=msg.message)
		{
			if (WM_USER+1 == msg.message)
			{
				nGetParentThreadMessageTimes++;
				wsprintfW(buf, L"Deal Parent Thread Message %d times, success", nGetParentThreadMessageTimes);
				wcscpy_s(buf, MAX_PATH, buf);
			}
		}
		else
		{
			break;
		}
	};


	return 0;
}


void testPostThreadToNoWindowThread()
{

	IDMainThread = GetCurrentThreadId();

	HANDLE hThread = CreateThread(NULL, 0, ThreadStartAddress, &IDMainThread, 0, &IDChild);
	if (NULL == hThread || INVALID_HANDLE_VALUE == hThread)
	{
		MessageBoxW(NULL, L"CreateThread Fail", L"Fail", MB_OK);
	}

}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_THREADMESSAGE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_THREADMESSAGE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   hWndMain = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   rect.left = 0;
   rect.top = 0;
   rect.right = 300;
   rect.bottom = 300;

   testPostThreadToNoWindowThread();

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	
	
	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here..

		TextOutW(hdc, 0, 0, buf, _tcslen(buf));
		TextOutW(hdc, 0, 50, bufForChild, _tcslen(bufForChild));
		
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;	


	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
