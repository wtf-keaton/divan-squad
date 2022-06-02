#pragma once
#include <inttypes.h>

#include "../divancrt/pe/pe.hh"
#include "xor.hh"

namespace nocrt
{
    extern  int strcmp(const char* cs, const char* ct);
    extern int wcsicmp(const wchar_t* m_wszStr1, const wchar_t* m_wszStr2);
}

class c_utils {
public:
    uintptr_t absolute_address( uintptr_t address, int offset, int size );
    PBYTE find_pattern( const char* Pattern, const wchar_t* Module = nullptr );
    uintptr_t get_base_address( ) {
        return *( uintptr_t* ) ( __readgsqword( 0x60 ) + 0x10 );
    }
};

inline c_utils* utils;