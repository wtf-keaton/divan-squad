#pragma once
#include "divancrt/imgui/imgui.h"

// Мне блять так нравится, и не ебёт
#define VAR( type, name, value) inline type name = value

namespace vars
{
    namespace aimbot
    {
        VAR( bool, enable, false );
        VAR( int, FOV, 0 );
        VAR( bool, draw_fov, false );
        VAR( bool, draw_aimbone, false );
        VAR( bool, draw_crosshair, false );

        VAR(int, smooth, 2.f );
        VAR( bool, head, false );
        VAR( bool, chest, false );

        VAR( bool, teleport_to, false );
    }

    namespace visuals
    {

  

        namespace players
        {
            VAR( bool, enable, false );
            VAR( int, distance, 1800 );

            VAR( bool, disable_team, false );
            VAR( bool, names, false );
            inline float name_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, box, false );
            inline float box_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, skelet, false );
            inline float skelet_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, box_outline, false );
            VAR( bool, snapline, false );
            inline float snapline_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };

        }

        namespace transport
        {
            VAR( bool, enable, false );
            VAR( int, distance, 100 );

            VAR( bool, minsk, false );
            inline float minsk_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, tech_pickup, false );
            inline float tech_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, jeep, false );
            inline float jeep_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, luv, false );
            inline float luv_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, m939, false );
            inline float m939_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, man, false );
            inline float man_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, ural375D, false );
            inline float ural375D_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, ural4320, false );
            inline float ural4320_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, kamaz, false );
            inline float kamaz_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, matv, false );
            inline float matv_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, tigrm, false );
            inline float tigrm_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, tapv, false );
            inline float tapv_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, brdm, false );
            inline float brdm_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, lppv, false );
            inline float lppv_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, btr80, false );
            inline float btr80_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, btr82a, false );
            inline float btr82a_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, lav6, false );
            inline float lav6_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, lav3, false );
            inline float lav3_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, m113a3, false );
            inline float m113a3_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, m1126, false );
            inline float m1126_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, fv432, false );
            inline float fv432_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, fv107, false );
            inline float fv107_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, mtlb, false );
            inline float mtlb_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, bmp1, false );
            inline float bmp1_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, bmp2, false );
            inline float bmp2_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, m2a3, false );
            inline float m2a3_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, fv510, false );
            inline float fv510_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, fv4034, false );
            inline float fv4034_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, msvs, false );
            inline float msvs_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, m1a2, false );
            inline float m1a2_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, t62, false );
            inline float t62_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, t72b3, false );
            inline float t72b3_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, mi8, false );
            inline float mi8_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, mi17, false );
            inline float mi17_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, uh60, false );
            inline float uh60_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
            VAR( bool, sa330, false );
            inline float sa330_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
        }

        namespace world
        {
            VAR( bool, enable, false );
            VAR( bool, ammocrate, false );
            inline float ammocrate_color[ 4 ] = { 1.0f, 0.0f, 0.0f, 1.0f };
            VAR( int, distance, 100 );

        }
    }
    namespace misc
    {
        VAR( bool, enable, false );

        VAR( bool, speed_hack, false );
        VAR( int, speed_value, 1 );

        VAR( bool, no_recoil, false );
        VAR( bool, debug, false );

        //VAR( float, FOV, 89.f );
    }
    namespace menu
    {
        VAR( bool, open, true );
    }
}