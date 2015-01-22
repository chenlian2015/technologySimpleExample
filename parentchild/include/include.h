#pragma once

#include <windows.h>
const UINT  CHILD_WINDOW_CREATE = WM_USER+1;

struct WindowInfo
{
	int ndatasize;
	HWND hWndFrom;
	WCHAR szChildClassName[MAX_PATH];
	WCHAR szChildWindowName[MAX_PATH];
};

class FileMappingTool
{

private:
	WCHAR m_FileMappingName[MAX_PATH];
	LPVOID m_pAddr;
	long m_lLength;

public:

	FileMappingTool(LPWSTR name, long lLength):m_pAddr(NULL),m_lLength(lLength)
	{
		wcscpy_s(m_FileMappingName, MAX_PATH, name);
	}

	DWORD CloseFileMappingAlways(){

		UnmapViewOfFile(m_pAddr);
		m_pAddr = NULL;
	}

	BOOL OpenFileMappingAlways(){

		if(NULL != m_pAddr)
		{
			return TRUE;
		}
		if (m_lLength <=0)
		{
			return FALSE;
		}

		HANDLE hFileMap = OpenFileMappingW(FILE_MAP_WRITE|FILE_SHARE_READ, TRUE, m_FileMappingName);

		if (NULL == hFileMap)
		{

			HANDLE hFile = CreateFileW(m_FileMappingName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, NULL);
			if (INVALID_HANDLE_VALUE == hFile)
			{
				return FALSE;
			}

			hFileMap = ::CreateFileMappingW(hFile, NULL, PAGE_READWRITE, 0, m_lLength, m_FileMappingName);
		//	CloseHandle(hFile);
			if (NULL == hFileMap)
			{
				return FALSE;
			}
		}


		m_pAddr = ::MapViewOfFileEx(hFileMap, FILE_MAP_WRITE, 0, 0, m_lLength, NULL);
	//	CloseHandle(hFileMap);

		if (NULL == m_pAddr)
		{
			return FALSE;
		}

		return TRUE;
	}

	BOOL WriteToFileMapping(LPVOID data, LONG size)
	{
		if (NULL != m_pAddr)
		{
			memcpy(m_pAddr, data, size);
		}
		else
		{
			return FALSE;
		}

		return TRUE;
	}

	BOOL ReadFromFileMapping(LPVOID data, LONG size)
	{

		if (NULL != m_pAddr)
		{
			memcpy(data, m_pAddr, size);
		}
		else
		{
			return FALSE;
		}

		return TRUE;
	}
};