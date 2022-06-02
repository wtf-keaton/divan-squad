#include "hooks/hook.hh"

#include "sdk/sdk.hh"

#pragma comment(linker, "/MERGE:.data=.divan")
#pragma comment(linker, "/MERGE:.rdata=.divan")
#pragma comment(linker, "/MERGE:.text=.divan")
#pragma comment(linker, "/SECTION:.divan,EWR")

void cheat_main( ) {
    SetSpoofStub( reinterpret_cast< PVOID >( utils->find_pattern( _( "FF 27" ) ) ) );

    hooks::init( );
}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD ul_reason_for_call, LPVOID lpReserved )
{
    if ( ul_reason_for_call == 1 )
    {
#ifdef _DEBUG

		if ( AllocConsole( ) ) {
			FILE* file{};
			freopen_s( &file, "CONOUT$", "w", stdout );
		}
#endif
        cheat_main( );
    }

    return TRUE;
}

#ifndef _DEBUG
extern "C" {
    int _fltused = 0;
    BOOL WINAPI _DllMainCRTStartup( HINSTANCE m_hInstance, DWORD m_dwReason, LPVOID m_lpReserved )
    {
        return DllMain( m_hInstance, m_dwReason, m_lpReserved );
    }
}
#endif