#include "engine.hh"

#include "../../spoofcall/spoof.hh"

uintptr_t engine::functions::GetModuleBase( )
{
    return *( uintptr_t* ) ( __readgsqword( 0x60 ) + 0x10 );
}

using engine_malloc = LPVOID( * )( SIZE_T size, unsigned int  align );
void* engine::functions::malloc( size_t size )
{
    static auto fmalloc_ = reinterpret_cast< engine_malloc >( utils->find_pattern( _( "40 57 48 83 EC 30 48 C7 44 24 ? ? ? ? ? 48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 41 8B F0 48 8B EA" ) ) + 0xA0 );
    return spoof_call< engine_malloc >( fmalloc_, size, 0 );
}

using engine_free = __int64( * )( __int64  m_lpAddress );
void engine::functions::free( void* ptr )
{
    static auto func = reinterpret_cast< __int64( __fastcall* )( __int64 a1 ) >( utils->find_pattern( _( "48 85 C9 74 2E 53" ) ) );
    spoof_call< engine_free >( func, ptr );
}