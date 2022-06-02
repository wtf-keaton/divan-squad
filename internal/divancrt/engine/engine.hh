#pragma once
#include "../../sdk/sdk.hh"

namespace engine
{
    // Screen Size
    inline float width, height;

    namespace functions
    {
        /// <summary>
        /// Engine implementation of vcruntime malloc for allocate memory
        /// </summary>
        /// <param name="size">Size to allocate memory</param>
        /// <returns>Allocated memory address</returns>
        extern void* malloc( size_t size );

        /// <summary>
        /// Engine implementation of vcruntime free for free allocated memory
        /// </summary>
        /// <param name="ptr">Allocated memory address to release</param>
        extern void free( void* ptr );

        /// <summary>
        /// Getting module base address
        /// </summary>
        /// <returns>Main module base address</returns>
        extern uintptr_t GetModuleBase( );
    }
}