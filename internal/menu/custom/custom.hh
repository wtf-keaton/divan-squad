#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS

#include "../../divancrt/imgui/imgui.h"
#include "../../divancrt/imgui/imgui_internal.h"

class c_custom {
public:
    bool tab( const char* label, bool selected, ImVec2 size_arg = ImVec2( 0, 0 ) );
    bool subtab( const char* label, bool selected, ImVec2 size_arg = ImVec2( 0, 0 ) );
    void checkbox( const char* label, bool* v, const char* color_picker, float color[ 4 ] );
};

inline c_custom* custom;