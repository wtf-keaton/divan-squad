#include "../divancrt/winapi/winapi.hh"
#include <cstdio>
#include "hook.hh"

#include "../menu/menu.hh"

#include "../variables.hh"

#include "../features/visuals/visuals.hh"
#include "../features/misc/misc.hh"
#include "../features/aimbot/aimbot.hh"

#include "../sdk/features.hh"

#include "../fonts/roboto.hh"
#include "../fonts/divan.hh"

#include "../render/render.hh"


HRESULT __stdcall hooks::hk_resize( IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags )
{

    if ( render_target_view )
    {
        render_target_view->Release();
        render_target_view = nullptr;
    }
    if ( device_context )
    {
        device_context->Release();
        device_context = nullptr;
    }
    if ( device )
    {
        device->Release();
        device = nullptr;
    }

    return spoof_call< HRESULT >( o_resize, swapChain, bufferCount, width, height, newFormat, swapChainFlags );
}

static bool init_once = true;
float width, height;

DXGI_SWAP_CHAIN_DESC backBufferDesc{};
HWND m_window{};

HRESULT __stdcall hooks::hk_present( IDXGISwapChain* pSwapChain, UINT sync_interval, UINT flags )
{
    if ( !device )
    {
        ID3D11Texture2D* backBuffer = nullptr;

        if ( FAILED( pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( PVOID* ) &backBuffer ) ) )
        {
            if ( backBuffer )
                backBuffer->Release();
            return spoof_call< HRESULT >( o_present, pSwapChain, sync_interval, flags );
        }

        if ( FAILED( pSwapChain->GetDevice( __uuidof( device ), reinterpret_cast< PVOID* >( &device ) ) ) )
        {
            if ( backBuffer )
                backBuffer->Release();
            return spoof_call< HRESULT >( o_present, pSwapChain, sync_interval, flags );
        }

        if ( FAILED( device->CreateRenderTargetView( backBuffer, nullptr, &render_target_view ) ) )
        {
            if ( backBuffer )
                backBuffer->Release();
            return spoof_call< HRESULT >( o_present, pSwapChain, sync_interval, flags );
        }

        backBuffer->Release();
        backBuffer = nullptr;

        device->GetImmediateContext( &device_context );

        pSwapChain->GetDesc( &backBufferDesc );
        width = ( int ) backBufferDesc.BufferDesc.Width;
        height = ( int ) backBufferDesc.BufferDesc.Height;
        m_window = backBufferDesc.OutputWindow;

        ImGui::CreateContext();

        ImGui_ImplWin32_Init( m_window );
        ImGui_ImplDX11_Init( device, device_context );

        ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF( roboto_compressed_data, roboto_compressed_size, 16.f );
        font = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF( roboto_compressed_data, roboto_compressed_size, 16.f );
        icons = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF( divan_compressed_data, divan_compressed_size, 18.f );

        ImGui_ImplDX11_CreateDeviceObjects();
    }

    static auto world_pattern = reinterpret_cast< uintptr_t > ( utils->find_pattern( _( "48 8B 3D ? ? ? ? 48 8B C7" ) ) );
    static auto world_address = utils->absolute_address( world_pattern, 3, 7 );

    auto myworld = *( UWorld** ) ( world_address );
    if ( !myworld )
        return spoof_call< HRESULT >( o_present, pSwapChain, sync_interval, flags );


    if ( ImGui::GetCurrentContext() != NULL )
    {
        ImGuiIO& io = ImGui::GetIO();
        POINT pos;

        // Get Mouse Position 
        if ( winapi::GetCursorPos( &pos ) && winapi::ScreenToClient( m_window, &pos ) )
        {
            io.MousePos = ImVec2( ( float ) pos.x, ( float ) pos.y );
        }

        if ( winapi::GetAsyncKeyState( VK_LBUTTON ) )
            io.MouseDown[ 0 ] = true;
        else
            io.MouseDown[ 0 ] = false;

    }

    device_context->OMSetRenderTargets( 1, &render_target_view, nullptr );

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame( width, height );

    ImGui::NewFrame();

    // render here
    render->render_text( ImGui::GetOverlayDrawList(), _( "Divan Technologies | Squad" ), ImVec2( 20, 25 ), 16.f, IM_COL32( 255, 255, 255, 255 ), false, font );

    if ( winapi::GetAsyncKeyState( VK_INSERT ) & 1 )
        vars::menu::open = !vars::menu::open;

    visuals->draw( myworld );
    misc->exec( myworld );
    aimbot->exec( myworld );

    if ( vars::menu::open ) menu::draw();
    render->draw_cursor( vars::menu::open );

    if ( vars::aimbot::draw_fov && ( vars::aimbot::FOV > 0 ) ) ImGui::GetOverlayDrawList()->AddCircle({width / 2, height / 2}, vars::aimbot::FOV, ImColor(255, 255, 255), 30.f, 1.f);

    
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );

    return spoof_call< HRESULT >( o_present, pSwapChain, sync_interval, flags );
}