#include <vector>
#include "utils.hh"
#include "../divancrt/vcruntime/vcruntime.hh"

PBYTE c_utils::find_pattern(const char* Pattern, const wchar_t* Module )
{
	//find pattern utils
#define InRange(x, a, b) (x >= a && x <= b) 
#define GetBits(x) (InRange(x, '0', '9') ? (x - '0') : ((x - 'A') + 0xA))
#define GetByte(x) ((BYTE)(GetBits(x[0]) << 4 | GetBits(x[1])))

	//get module range
	PBYTE ModuleStart = ( PBYTE ) this->get_base_address( ); 
	if ( !ModuleStart ) 
		return nullptr;

	PIMAGE_NT_HEADERS NtHeader = ( ( PIMAGE_NT_HEADERS ) ( ModuleStart + ( ( PIMAGE_DOS_HEADER ) ModuleStart )->e_lfanew ) );
	PBYTE ModuleEnd = ( PBYTE ) ( ModuleStart + NtHeader->OptionalHeader.SizeOfImage - 0x1000 ); ModuleStart += 0x1000;

	//scan pattern main
	PBYTE FirstMatch = nullptr;
	const char* CurPatt = Pattern;
	for ( ; ModuleStart < ModuleEnd; ++ModuleStart )
	{
		bool SkipByte = ( *CurPatt == '\?' );
		if ( SkipByte || *ModuleStart == GetByte( CurPatt ) ) {
			if ( !FirstMatch ) FirstMatch = ModuleStart;
			SkipByte ? CurPatt += 2 : CurPatt += 3;
			if ( CurPatt[ -1 ] == 0 ) return FirstMatch;
		}

		else if ( FirstMatch ) {
			ModuleStart = FirstMatch;
			FirstMatch = nullptr;
			CurPatt = Pattern;
		}
	}

	return nullptr;
}

uintptr_t c_utils::absolute_address(uintptr_t address, int offset, int size) {
	uintptr_t addrs = address + size + *(int*)(address + offset);
	return addrs;
}