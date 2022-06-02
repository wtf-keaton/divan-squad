#include <Windows.h>
#include "../divancrt/vcruntime/vcruntime.hh"

#include "../utils/utils.hh"

#define ConstStrLen(Str) ((sizeof(Str) - sizeof(Str[0])) / sizeof(Str[0]))

//Spoof Return Address
template <typename StrType, typename StrType2>
bool c_strcmp(StrType Str, StrType2 InStr, bool Two) {
	if (!Str || !InStr) return false;
	wchar_t c1, c2; do {
		c1 = *Str++; c2 = *InStr++;
		c1 = vcruntime->tolower(c1); c2 = vcruntime->tolower(c2);
		if (!c1 && (Two ? !c2 : 1)) return true;
	} while (c1 == c2); return false;
}

inline PVOID ReturnAddressStubEnc = 0;
extern "C" void* _spoofer_stub();
template<typename Ret = void, typename First = void*, typename Second = void*, typename Third = void*, typename Fourth = void*, typename... Stack>
__forceinline Ret SpoofCall(void* Func, First a1 = First{}, Second a2 = Second{}, Third a3 = Third{}, Fourth a4 = Fourth{}, Stack... args) {
	struct shell_params { const void* a1; void* a2; void* a3; };
	shell_params CallCtx = { ReturnAddressStubEnc, Func, nullptr };
	typedef Ret(*ShellFn)(First, Second, Third, Fourth, PVOID, PVOID, Stack...); 
	return ((ShellFn)&_spoofer_stub)(a1, a2, a3, a4, &CallCtx, nullptr, args...);
}
__forceinline void SetSpoofStub(PVOID R15_Stub) { ReturnAddressStubEnc = R15_Stub; }
#define spoof_call SpoofCall

//Spoof VFTable Return Address
template<typename Ret = void, typename... Args>
__forceinline Ret VCall(PVOID Class, int Index, Args... AddArgs) {
	return SpoofCall<Ret>((*(PVOID**)Class)[Index], Class, AddArgs...);
}

//Spoof Call Export Return Address
__forceinline PVOID GetExportAddress(PBYTE hDll, const char* Name)
{
	//process image data
	PIMAGE_NT_HEADERS NT_Head = (PIMAGE_NT_HEADERS)(hDll + ((PIMAGE_DOS_HEADER)hDll)->e_lfanew);
	PIMAGE_EXPORT_DIRECTORY ExportDir = (PIMAGE_EXPORT_DIRECTORY)(hDll + NT_Head->OptionalHeader.DataDirectory[0].VirtualAddress);

	//process list
	for (DWORD i = 0; i < ExportDir->NumberOfNames; i++)
	{
		//get ordinal & name
		USHORT Ordinal = ((USHORT*)(hDll + ExportDir->AddressOfNameOrdinals))[i];
		const char* ExpName = (const char*)hDll + ((DWORD*)(hDll + ExportDir->AddressOfNames))[i];
		if (c_strcmp(Name, ExpName, true))
			return (PVOID)(hDll + ((DWORD*)(hDll + ExportDir->AddressOfFunctions))[Ordinal]);
	} return nullptr;
}

#define FC(Mod, Name, ...) [&](){ static PVOID FAddr = nullptr; \
		if (!FAddr) FAddr = GetExportAddress(reinterpret_cast<PBYTE>(utils->module_base(_(L#Mod))), _(#Name)); \
		return SpoofCall<decltype(Name(__VA_ARGS__))>(FAddr, __VA_ARGS__); \
	}()