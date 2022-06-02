#pragma once
#include <Windows.h>
#include "../features/visuals/visuals.hh"
#include "../spoofcall/spoof.hh"

#include "../vmt/vmthook.hh"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../divancrt/imgui/imgui.h"
#include "../divancrt/imgui/imgui_internal.h"
#include "../divancrt/imgui/imgui_impl_dx11.h"
#include "../divancrt/imgui/imgui_impl_win32.h"

inline ID3D11Device* device = nullptr;
inline ID3D11DeviceContext* device_context = nullptr;
inline ID3D11RenderTargetView* render_target_view = nullptr;

inline c_vmt_hook renderer_hook;
using f_resize = HRESULT( __stdcall* )( IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags );
inline f_resize o_resize{};

using f_present = HRESULT( __stdcall* )( IDXGISwapChain* pthis, UINT sync_interval, UINT flags );
inline f_present o_present{};


namespace hooks {
    extern HRESULT __stdcall hk_present( IDXGISwapChain* pSwapChain, UINT sync_interval, UINT flags );

    extern HRESULT __stdcall hk_resize( IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags );

    inline void init( ) {
        auto m_pswap_chain = GetDynamicRHI( )->GetSwapChain( );

        char buf[ 1024 ];


        o_present = *( decltype( o_present )* ) ( *( uintptr_t* ) m_pswap_chain + 64 );
        o_resize = *( decltype( o_resize )* ) ( *( uintptr_t* ) m_pswap_chain + 104 );

        renderer_hook.setup( m_pswap_chain );
        renderer_hook.set_hook( 8, hooks::hk_present );
        renderer_hook.set_hook( 13, hooks::hk_resize );
    }
}

