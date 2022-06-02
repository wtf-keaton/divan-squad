#define IMGUI_DEFINE_MATH_OPERATORS

#include "render.hh"
#include "../divancrt/imgui/imgui_internal.h"

float c_render::render_text( ImDrawList* draw_list, const char* text, ImVec2 pos, float size, ImColor color, bool center, ImFont* pFont )
{
    ImVec2 textSize = pFont->CalcTextSizeA( size, FLT_MAX, 0.0f, text );

    if ( !pFont->ContainerAtlas )
        return 0.f;

    draw_list->PushTextureID( pFont->ContainerAtlas->TexID );

    if ( center )
        pos.x -= textSize.x / 2.0f;

    draw_list->AddText( pFont, size, ImVec2( pos.x + 1, pos.y + 1 ), ImColor( 0, 0, 0, 100 ), text );
    draw_list->AddText( pFont, size, ImVec2( pos.x - 1, pos.y + 1 ), ImColor( 0, 0, 0, 100 ), text );
    draw_list->AddText( pFont, size, ImVec2( pos.x + 1, pos.y - 1 ), ImColor( 0, 0, 0, 100 ), text );
    draw_list->AddText( pFont, size, ImVec2( pos.x - 1, pos.y - 1 ), ImColor( 0, 0, 0, 100 ), text );
    draw_list->AddText( pFont, size, ImVec2( pos.x + 1, pos.y ), ImColor( 0, 0, 0, 100 ), text );
    draw_list->AddText( pFont, size, ImVec2( pos.x - 1, pos.y ), ImColor( 0, 0, 0, 100 ), text );
    draw_list->AddText( pFont, size, ImVec2( pos.x, pos.y + 1 ), ImColor( 0, 0, 0, 100 ), text );
    draw_list->AddText( pFont, size, ImVec2( pos.x, pos.y - 1 ), ImColor( 0, 0, 0, 100 ), text );


    draw_list->AddText( pFont, size, pos, color, text );

    draw_list->PopTextureID();

    return pos.y + textSize.y;
}

void c_render::render_box( ImDrawList* draw_list, float x1, float y1, float x2, float y2, ImColor color, float thickness, float rounding )
{
    ImVec2 a( x1, y1 ), b( x1 + x2, y1 + y2 );
    draw_list->PathRect( a, b, rounding, -1 );
    draw_list->PathStroke( color, true, thickness );
}

void c_render::draw_cursor( bool menu_open )
{
    auto& io = ImGui::GetIO();
    auto draw = ImGui::GetOverlayDrawList();

    if ( menu_open )
    {
        draw->AddLine( io.MousePos, io.MousePos + ImVec2( 0, 25 ), IM_COL32( 200, 200, 200, 255 ), 1.7f );
        draw->AddLine( io.MousePos, io.MousePos + ImVec2( 17, 17 ), IM_COL32( 200, 200, 200, 255 ), 2.f );
        draw->AddLine( io.MousePos + ImVec2( 0, 25 ), io.MousePos + ImVec2( 17, 17 ), IM_COL32( 200, 200, 200, 255 ), 2.f );
    }
}

void c_render::esp_draw( ImDrawList* draw_list, ImVec2 x1, ImU32 color, const char* buf, ImFont* font )
{
    draw_list->AddRectFilled(
        ImVec2( x1.x - 10, x1.y - 2 ),
        ImVec2( x1.x + ImGui::CalcTextSize( buf ).x + 8, x1.y + ImGui::CalcTextSize( buf ).y + 2 ),
        IM_COL32( 0, 0, 0, 100 ) );

    draw_list->AddRectFilled(
        ImVec2( x1.x - 10, x1.y - 2 ),
        ImVec2( x1.x - 7, x1.y + ImGui::CalcTextSize( buf ).y + 2 ),
        color );

    this->render_text( draw_list, buf, ImVec2( x1.x, x1.y ),
        16.f, color, false, font );
}