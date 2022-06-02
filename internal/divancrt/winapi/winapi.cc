#include "winapi.hh"

#include "../vcruntime/vcruntime.hh"

#include "../../spoofcall/spoof.hh"

DWORD64 winapi::GetModuleHandle( const wchar_t* module_name )
{
    auto m_pPEB = reinterpret_cast< PPEB64 >( __readgsqword( 0x60 ) );
    for ( PLIST_ENTRY64 m_pListEntry = reinterpret_cast< PLIST_ENTRY64 >( m_pPEB->Ldr->InLoadOrderModuleList.Flink );
        m_pListEntry != &m_pPEB->Ldr->InMemoryOrderModuleList;
        m_pListEntry = reinterpret_cast< PLIST_ENTRY64 >( m_pListEntry->Flink ) )
    {
        PLDR_DATA_TABLE_ENTRY64 m_pEntry = CONTAINING_RECORD( m_pListEntry, LDR_DATA_TABLE_ENTRY64, InLoadOrderLinks );

        if ( !vcruntime->wcsicmp( module_name, m_pEntry->BaseDllName.Buffer ) )
            return m_pEntry->DllBase;
    }

    return 0;
}

uint32_t winapi::GetModuleSize( uint64_t module_address )
{
    PIMAGE_DOS_HEADER m_pDos = reinterpret_cast< PIMAGE_DOS_HEADER >( module_address );
    return reinterpret_cast< PIMAGE_NT_HEADERS64 >( module_address + m_pDos->e_lfanew )->OptionalHeader.SizeOfImage;
}

FARPROC winapi::GetProcAddress( DWORD64 m_dwImageBase, const char* m_szProcName )
{
    PIMAGE_DOS_HEADER m_pDos = reinterpret_cast< PIMAGE_DOS_HEADER >( m_dwImageBase );
    PIMAGE_NT_HEADERS64 m_pNt = reinterpret_cast< PIMAGE_NT_HEADERS64 >( m_dwImageBase + m_pDos->e_lfanew );
    PIMAGE_EXPORT_DIRECTORY m_pExports = reinterpret_cast< PIMAGE_EXPORT_DIRECTORY >( m_dwImageBase + m_pNt->OptionalHeader.DataDirectory[ 0 ].VirtualAddress );
    DWORD* m_pFunctionsTable = reinterpret_cast< DWORD* >( m_dwImageBase + m_pExports->AddressOfFunctions );
    WORD* m_pOrdinalTable = reinterpret_cast< WORD* >( m_dwImageBase + m_pExports->AddressOfNameOrdinals );
    DWORD* m_pNamesTable = reinterpret_cast< DWORD* >( m_dwImageBase + m_pExports->AddressOfNames );
    for ( DWORD i = 0; i < m_pExports->NumberOfNames; i++ )
    {
        if ( !vcruntime->strcmp( reinterpret_cast< char* >( m_dwImageBase + m_pNamesTable[ i ] ), m_szProcName ) )
            return reinterpret_cast< FARPROC >( m_dwImageBase + m_pFunctionsTable[ m_pOrdinalTable[ i ] ] );
    }
    return nullptr;
}

SHORT winapi::GetAsyncKeyState( INT key )
{
    static SHORT( __fastcall * mGetAsyncKeyState )( INT key ) = nullptr;

    if ( !mGetAsyncKeyState )
        mGetAsyncKeyState = reinterpret_cast< decltype( mGetAsyncKeyState ) >( winapi::GetProcAddress( winapi::GetModuleHandle( _( L"win32u.dll" ) ), _( "NtUserGetAsyncKeyState" ) ) );

    return spoof_call<SHORT>( mGetAsyncKeyState, key );
}

BOOL winapi::GetCursorPos( LPPOINT lpPoint ) // Боже что же это за щиткод
{
    static BOOL( __fastcall * mGetCursorPos )( LPPOINT point ) = nullptr;

    if ( !mGetCursorPos )
        mGetCursorPos = reinterpret_cast< decltype( mGetCursorPos ) >( winapi::GetProcAddress( winapi::GetModuleHandle( _( L"User32.dll" ) ), _( "GetCursorPos" ) ) );

    return spoof_call<BOOL>( mGetCursorPos, lpPoint );
}

BOOL winapi::ScreenToClient( HWND hWnd, LPPOINT lpPoint )
{
    static BOOL( __fastcall * mScreenToClient )( HWND, LPPOINT ) = nullptr;

    if ( !mScreenToClient )
        mScreenToClient = reinterpret_cast< decltype( mScreenToClient ) >( winapi::GetProcAddress( winapi::GetModuleHandle( _( L"User32.dll" ) ), _( "ScreenToClient" ) ) );

    return spoof_call<BOOL>( mScreenToClient, hWnd, lpPoint );
}

void winapi::qsort( void* _Base, size_t _NumOfElements, size_t _SizeOfElements, _CoreCrtNonSecureSearchSortCompareFunction _CompareFunction )
{
    static void( __fastcall * mQsort )( void*, size_t, size_t, _CoreCrtNonSecureSearchSortCompareFunction ) = nullptr;

    if ( !mQsort )
        mQsort = reinterpret_cast< decltype( mQsort ) >( winapi::GetProcAddress( winapi::GetModuleHandle( _( L"ucrtbase.dll" ) ), _( "qsort" ) ) );

    // DIVAN_TODO: Add spoof call
    return spoof_call(mQsort, _Base, _NumOfElements, _SizeOfElements, _CompareFunction );
}

typedef struct _KSYSTEM_TIME
{
    ULONG LowPart;
    LONG High1Time;
    LONG High2Time;
} KSYSTEM_TIME, * PKSYSTEM_TIME;

union TICK_COUNT_UNION
{
    KSYSTEM_TIME TickCount;
    UINT64 TickCountQuad;
};

volatile auto TickCountPtr = reinterpret_cast< TICK_COUNT_UNION* volatile >( 0x7FFE0320 );
volatile auto TickCountMultiplierPtr = reinterpret_cast< uint32_t* volatile >( 0x7FFE0004 );


uint64_t winapi::GetTickCount64( )
{
    return TickCountPtr->TickCountQuad * static_cast< uint64_t >( *TickCountMultiplierPtr ) >> 24;
}