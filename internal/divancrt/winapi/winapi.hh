#pragma once
#include "../pe/pe.hh"

#include <inttypes.h>

#undef GetModuleHandle

namespace winapi
{
    DWORD64 GetModuleHandle( const wchar_t* module_name );
    uint32_t GetModuleSize( uint64_t module_address );
    FARPROC GetProcAddress( DWORD64 m_dwImageBase, const char* m_szProcName );

    SHORT GetAsyncKeyState( INT key );
    BOOL GetCursorPos( LPPOINT lpPoint );
    BOOL ScreenToClient( HWND hWnd, LPPOINT lpPoint );

    void qsort( void* _Base, size_t _NumOfElements, size_t _SizeOfElements, _CoreCrtNonSecureSearchSortCompareFunction _CompareFunction );
    uint64_t GetTickCount64( );
}