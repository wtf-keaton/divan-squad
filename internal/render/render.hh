#pragma once

#include "../divancrt/imgui/imgui.h"

class c_render
{
public:
    float render_text( ImDrawList* draw_list, const char* text, ImVec2 pos, float size, ImColor color, bool center, ImFont* pFont );
    void render_box( ImDrawList* draw_list, float x1, float y1, float x2, float y2, ImColor color, float thickness = 1.f, float rounding = 0.f );
    void esp_draw( ImDrawList* draw_list, ImVec2 x1, ImU32 color, const char* buf, ImFont* font );
    void draw_cursor( bool menu_open );
};

inline c_render* render;