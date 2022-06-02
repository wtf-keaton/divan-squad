#include "custom.hh"


bool c_custom::tab( const char* label, bool selected, ImVec2 size_arg )
{
    using namespace ImGui;
    ImGuiWindow* window = GetCurrentWindow();
    if ( window->SkipItems )
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID( label );
    const ImVec2 label_size = CalcTextSize( label, NULL, true );

    ImGuiButtonFlags flags = ImGuiButtonFlags_None;

    ImVec2 pos = window->DC.CursorPos;
    if ( ( flags & ImGuiButtonFlags_AlignTextBaseLine ) && style.FramePadding.y < window->DC.CurrentLineTextBaseOffset ) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
        pos.y += window->DC.CurrentLineTextBaseOffset - style.FramePadding.y;
    ImVec2 size = CalcItemSize( size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f );

    const ImRect bb( pos, pos + size );
    ItemSize( size, style.FramePadding.y );
    if ( !ItemAdd( bb, id ) )
        return false;

    if ( window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat )
        flags |= ImGuiButtonFlags_Repeat;
    bool hovered, held;
    bool pressed = ButtonBehavior( bb, id, &hovered, &held, flags );
    if ( pressed )
        MarkItemEdited( id );

    // Render
    ImVec4 col = ImVec4( 0.f, 0.f, 0.f, 0.f );
    ImVec4 text_col = ImVec4( 1.f, 1.f, 1.f, 1.f );

    //float t = selected ? 1.0f : 0.0f;
    //float ANIM_SPEED = 0.5f; // Bigger = Slower
    //if ( g.LastActiveId == g.CurrentWindow->GetID( label ) )
    //{
    //    float t_anim = ImSaturate( g.LastActiveIdTimer / ANIM_SPEED );
    //    t = selected ? ( t_anim ) : ( 1.0f - t_anim );
    //}

    if ( selected )
    {
        col = ImVec4( 20 / 255.f, 20 / 255.f, 20 / 255.f, 1.f );
        text_col = ImVec4( 161 / 255.f, 107 / 255.f, 255 / 255.f, 1.f );
    }
    else
    {
        col = ImVec4( 0.f, 0.f, 0.f, 0.f );
        text_col = ImVec4( 1.f, 1.f, 1.f, 1.f );
    }

    RenderFrame( bb.Min, bb.Max, GetColorU32( col ), true, 4.f );

    PushStyleColor( ImGuiCol_Text, text_col );
    RenderTextClipped( bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb );
    PopStyleColor();

    return pressed;
}


bool c_custom::subtab( const char* label, bool selected, ImVec2 size_arg )
{
    using namespace ImGui;
    ImGuiWindow* window = GetCurrentWindow();
    if ( window->SkipItems )
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID( label );
    const ImVec2 label_size = CalcTextSize( label, NULL, true );

    ImGuiButtonFlags flags = ImGuiButtonFlags_None;

    ImVec2 pos = window->DC.CursorPos;
    if ( ( flags & ImGuiButtonFlags_AlignTextBaseLine ) && style.FramePadding.y < window->DC.CurrentLineTextBaseOffset ) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
        pos.y += window->DC.CurrentLineTextBaseOffset - style.FramePadding.y;
    ImVec2 size = CalcItemSize( size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f );

    const ImRect bb( pos, pos + size );
    ItemSize( size, style.FramePadding.y );
    if ( !ItemAdd( bb, id ) )
        return false;

    if ( window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat )
        flags |= ImGuiButtonFlags_Repeat;
    bool hovered, held;
    bool pressed = ButtonBehavior( bb, id, &hovered, &held, flags );
    if ( pressed )
        MarkItemEdited( id );

    // Render
    ImVec4 col = ImVec4( 0.f, 0.f, 0.f, 0.f );
    ImVec4 text_col = ImVec4( 0.5f, 0.5f, 0.5f, 1.f );

    if ( selected )
    {
        col = ImVec4( 20 / 255.f, 20 / 255.f, 20 / 255.f, 1.f );
        text_col = ImVec4( 1.f, 1.f, 1.f, 1.f );
    }
    else
    {
        col = ImVec4( 0.f, 0.f, 0.f, 0.f );
        text_col = ImVec4( 0.5f, 0.5f, 0.5f, 1.f );
    }
    RenderFrame( bb.Min, bb.Max, GetColorU32( col ), true, 4.f );

    PushStyleColor( ImGuiCol_Text, text_col );
    RenderTextClipped( bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb );
    PopStyleColor();
    return pressed;
}

void c_custom::checkbox( const char* label, bool* v, const char* color_picker, float color[ 4 ] )
{
    ImGui::Checkbox( label, v );
    ImGui::SameLine();
    ImGui::ColorEdit4( color_picker, color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview );
}
