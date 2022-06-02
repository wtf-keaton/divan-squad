#include <Windows.h>

#include "../divancrt/vcruntime/vcruntime.hh"


#include "../divancrt/engine/engine.hh"
class c_vmt_hook {

private:

public:
    LPVOID base;
    LPVOID original_vmt;
    LPVOID allocated[1024];


    void setup(void* pointer, int size = 1024) {

        base = pointer;

        if (!size)
            return;

        vcruntime->memcpy(allocated, *reinterpret_cast<void**>(base), size * sizeof uintptr_t);
        original_vmt = *reinterpret_cast<void**>(base);
        *reinterpret_cast<void**>(base) = allocated;
    }

    void shutdown() {

        *reinterpret_cast<void**>(base) = original_vmt;
    }

    void clear() {

        engine::functions::free(allocated);
        base = nullptr;
        original_vmt = nullptr;
    }

    void* set_hook(int index, void* function) {

        *reinterpret_cast<void**>((uintptr_t*)allocated + index) = function;
        return *reinterpret_cast<void**>((uintptr_t*)original_vmt + index);

    }

};