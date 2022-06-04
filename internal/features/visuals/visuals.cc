#include "visuals.hh"
#include "../../variables.hh"
#include "../../utils/utils.hh"

#include "../../spoofcall/spoof.hh"

#include "../../divancrt/imgui/imgui.h"
#include "../../divancrt/imgui/imgui_internal.h"

#include "../../sdk/features.hh"

bool init_once = true;

bool c_visuals::draw( UWorld* world )
{

    auto level = world->level;
    if ( !level ) return false;

    auto gameinstance = world->gameinstance;
    if ( !gameinstance ) return false;

    auto localplayer = gameinstance->localplayer[ 0 ];
    if ( !localplayer ) return false;

    auto playercontroller = localplayer->playercontroller;
    if ( !playercontroller ) return false;

    auto localpawn = playercontroller->acknowledgedpawn;
    if ( !localpawn ) return false;

    auto localmesh = localpawn->rootcomponent;
    if ( !localmesh ) return false;

    auto localplayerstate = localpawn->playerstate;
    if ( !localplayerstate ) return false;

    auto localsoldier = localplayerstate->Soldier;
    if ( !localsoldier ) return false;

    auto localpos = localmesh->RelativeLocation;


    auto actors = level->actors;

    for ( int i = 0; i < actors.Num(); i++ )
    {
        if ( !actors.IsValidIndex( i ) ) continue;

        if ( auto actor = actors[ i ] )
        {
            auto root = actor->rootcomponent;
            if ( root )
            {
                auto location = root->RelativeLocation;
                auto distance_float = vcruntime->get_distance( localpos, location );

                int distance = int( distance_float ) / 100;

                auto actor_name = sdk::get_object_name( actor );

                if ( vars::visuals::players::enable )
                {
                    //if ( actor->instigator == playercontroller->acknowledgedpawn ) continue;
                    if ( distance < vars::visuals::players::distance )
                    {
                        Vector2 screen_head{}, screen_root{};
                        if ( 
                            vcruntime->wcsstr( actor_name, _( L"BP_Soldier_RU_" ) ) 
                            || vcruntime->wcsstr( actor_name, _( L"BP_Soldier_USA_" ) ) 
                            || vcruntime->wcsstr( actor_name, _( L"BP_Soldier_AUS_" ) ) 
                            || vcruntime->wcsstr( actor_name, _( L"BP_Soldier_CAF_" ) ) 
                            || vcruntime->wcsstr( actor_name, _( L"BP_Soldier_GB_" ) ) 
                            || vcruntime->wcsstr( actor_name, _( L"BP_Soldier_INS_" ) ) 
                            || vcruntime->wcsstr( actor_name, _( L"BP_Soldier_MEA_" ) ) 
                            || vcruntime->wcsstr( actor_name, _( L"BP_Soldier_MIL_" ) ) 
                            
                            )
                        {

                            auto playerstate = actor->instigator->playerstate;

                            if ( playerstate )
                            {
                                if ( vars::visuals::players::disable_team && playerstate->teamId == localplayerstate->teamId ) continue;
                                auto mesh = actor->instigator->controller->character->Mesh;
                                if ( !mesh ) continue;
                                //auto head = sdk::get_bone_with_rotation( mesh, bones::HEAD );
                                //auto root = sdk::get_bone_with_rotation( mesh, bones::ROOT );

                                if ( sdk::world_to_screen( playercontroller, actor->rootcomponent->RelativeLocation, &screen_head )/* && sdk::world_to_screen( playercontroller, root, &screen_root ) */)
                                {
                                    Vector2 box_size{};
                                    box_size.y = vcruntime->abs( screen_head.y - screen_root.y ); // calculate absolute len for box height
                                    box_size.x = box_size.y * 0.25f; // caclulate absolute len for box width
                                    auto start_pos = Vector2{ screen_head.x - box_size.x, screen_head.y };
                                    box_size.x = box_size.x * 2.0f;
                                    Vector2 dist_pos{ box_size.x + 15.f, screen_head.y };

                                    if ( vars::visuals::players::names )
                                    {
                                        char buf[ 1024 ];
                                        ImFormatString( buf, sizeof buf, _( "%s HP: %.0f [%dm]" ), vcruntime->w2c( sdk::get_player_name( ( uint64_t ) actor ) ), playerstate->Soldier->Health, distance );
                                        auto size = font->CalcTextSizeA( 16.f, 14, 0, buf );

                                        render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_head.x, screen_head.y ), ImGui::GetColorU32(
                                            ImVec4(
                                                vars::visuals::players::name_color[ 0 ],
                                                vars::visuals::players::name_color[ 1 ],
                                                vars::visuals::players::name_color[ 2 ],
                                                1.f )
                                        ), buf, font );
                                    }

                                    if ( vars::visuals::players::snapline )
                                    {
                                        ImGui::GetOverlayDrawList()->AddLine( ImVec2( ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y ), ImVec2( screen_head.x, screen_head.y ),
                                            ImGui::GetColorU32(
                                                ImVec4(
                                                    vars::visuals::players::snapline_color[ 0 ],
                                                    vars::visuals::players::snapline_color[ 1 ],
                                                    vars::visuals::players::snapline_color[ 2 ],
                                                    1.f )
                                            ), 1.5f );
                                    }

                                    if ( vars::visuals::players::box )
                                    {
                                        render->render_box( ImGui::GetOverlayDrawList(), start_pos.x, start_pos.y, box_size.x, box_size.y,
                                            ImGui::GetColorU32(
                                                ImVec4(
                                                    vars::visuals::players::box_color[ 0 ],
                                                    vars::visuals::players::box_color[ 1 ],
                                                    vars::visuals::players::box_color[ 2 ],
                                                    1.f ) ), 1 );

                                        if ( vars::visuals::players::box_outline )
                                        {
                                            render->render_box( ImGui::GetOverlayDrawList(), start_pos.x + 1, start_pos.y + 1, box_size.x - 2, box_size.y - 2, ImColor( 0, 0, 0 ), 1 );
                                            render->render_box( ImGui::GetOverlayDrawList(), start_pos.x - 1, start_pos.y - 1, box_size.x + 2, box_size.y + 2, ImColor( 0, 0, 0 ), 1 );
                                        }
                                    }
                                }

                                if ( vars::visuals::players::skelet )
                                {
                                    Vector2 screen_neck{}, screen_pelvis{},
                                        screen_right_thigh{}, screen_left_thigh{}, screen_left_calf{}, screen_right_calf{}, screen_right_foot{}, screen_left_foot{},
                                        screen_upper_arm_right{}, screen_upper_arm_left{}, screen_lower_arm_right{}, screen_lower_arm_left{},
                                        screen_hand_right{}, screen_hand_left{};

                                    if ( 
                                        sdk::world_to_screen( playercontroller, sdk::get_bone_with_rotation( mesh, bones::NECK ), &screen_neck ) &&
                                        sdk::world_to_screen( playercontroller, sdk::get_bone_with_rotation( mesh, bones::PELVIS ), &screen_pelvis ) &&
                                        sdk::world_to_screen( playercontroller, sdk::get_bone_with_rotation( mesh, bones::RIGHT_THIGH ), &screen_right_thigh ) &&
                                        sdk::world_to_screen( playercontroller, sdk::get_bone_with_rotation( mesh, bones::LEFT_THIGH ), &screen_left_thigh ) &&
                                        sdk::world_to_screen( playercontroller, sdk::get_bone_with_rotation( mesh, bones::RIGHT_KNEE ), &screen_right_calf ) &&
                                        sdk::world_to_screen( playercontroller, sdk::get_bone_with_rotation( mesh, bones::LEFT_KNEE ), &screen_left_calf ) &&
                                        sdk::world_to_screen( playercontroller, sdk::get_bone_with_rotation( mesh, bones::RIGHT_FOOT ), &screen_right_foot ) &&
                                        sdk::world_to_screen( playercontroller, sdk::get_bone_with_rotation( mesh, bones::LEFT_FOOT ), &screen_left_foot ) &&
                                        sdk::world_to_screen( playercontroller, sdk::get_bone_with_rotation( mesh, bones::RIGHT_SHOULDER ), &screen_upper_arm_right ) &&
                                        sdk::world_to_screen( playercontroller, sdk::get_bone_with_rotation( mesh, bones::LEFT_SHOULDER ), &screen_upper_arm_left ) &&
                                        sdk::world_to_screen( playercontroller, sdk::get_bone_with_rotation( mesh, bones::RIGHT_ELBOW ), &screen_lower_arm_right ) &&
                                        sdk::world_to_screen( playercontroller, sdk::get_bone_with_rotation( mesh, bones::LEFT_ELBOW ), &screen_lower_arm_left ) &&
                                        sdk::world_to_screen( playercontroller, sdk::get_bone_with_rotation( mesh, bones::RIGHT_HAND ), &screen_hand_right ) &&
                                        sdk::world_to_screen( playercontroller, sdk::get_bone_with_rotation( mesh, bones::LEFT_HAND ), &screen_hand_left ) )
                                    {
                                        auto color =
                                            ImColor(
                                                int( vars::visuals::players::skelet_color[ 0 ] * 255.f ),
                                                int( vars::visuals::players::skelet_color[ 1 ] * 255.f ),
                                                int( vars::visuals::players::skelet_color[ 2 ] * 255.f )
                                            );// vars::visuals::players::skelet_color;

                                        ImGui::GetOverlayDrawList()->AddLine( { screen_neck.x, screen_neck.y }, { screen_upper_arm_right.x, screen_upper_arm_right.y }, color, 1.f );
                                        ImGui::GetOverlayDrawList()->AddLine( { screen_neck.x, screen_neck.y }, { screen_upper_arm_left.x, screen_upper_arm_left.y }, color, 1.f );
                                        ImGui::GetOverlayDrawList()->AddLine( { screen_neck.x, screen_neck.y }, { screen_pelvis.x, screen_pelvis.y }, color, 1.f );

                                        ImGui::GetOverlayDrawList()->AddLine( { screen_pelvis.x, screen_pelvis.y }, { screen_right_thigh.x, screen_right_thigh.y }, color, 1.f );
                                        ImGui::GetOverlayDrawList()->AddLine( { screen_pelvis.x, screen_pelvis.y }, { screen_left_thigh.x, screen_left_thigh.y }, color, 1.f );

                                        ImGui::GetOverlayDrawList()->AddLine( { screen_right_thigh.x, screen_right_thigh.y }, { screen_right_calf.x, screen_right_calf.y }, color, 1.f );
                                        ImGui::GetOverlayDrawList()->AddLine( { screen_left_thigh.x, screen_left_thigh.y }, { screen_left_calf.x, screen_left_calf.y }, color, 1.f );

                                        ImGui::GetOverlayDrawList()->AddLine( { screen_right_calf.x, screen_right_calf.y }, { screen_right_foot.x, screen_right_foot.y }, color, 1.f );
                                        ImGui::GetOverlayDrawList()->AddLine( { screen_left_calf.x, screen_left_calf.y }, { screen_left_foot.x, screen_left_foot.y }, color, 1.f );

                                        ImGui::GetOverlayDrawList()->AddLine( { screen_upper_arm_right.x, screen_upper_arm_right.y }, { screen_lower_arm_right.x, screen_lower_arm_right.y }, color, 1.f );
                                        ImGui::GetOverlayDrawList()->AddLine( { screen_upper_arm_left.x, screen_upper_arm_left.y }, { screen_lower_arm_left.x, screen_lower_arm_left.y }, color, 1.f );
                                    }
                                }
                            }
                        }
                    }
                }
                if ( vars::misc::debug )
                {
                    Vector2 screen_pos{};
                    if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                    {
                        render->render_text( ImGui::GetOverlayDrawList(), vcruntime->w2c( actor_name ), ImVec2( screen_pos.x, screen_pos.y ),
                            16.f, ImGui::GetColorU32(
                                ImVec4(
                                    vars::visuals::transport::kamaz_color[ 0 ],
                                    vars::visuals::transport::kamaz_color[ 1 ],
                                    vars::visuals::transport::kamaz_color[ 2 ],
                                    1.f )
                            ), false, font );
                    }
                }
                if ( vars::visuals::world::enable )
                {
                    Vector2 screen_pos{};
                    if ( vars::visuals::world::ammocrate )
                    {
                        if ( distance < vars::visuals::world::distance )
                        {
                            auto name = localplayerstate->teamId == 1 ? _( L"Team1SpawnerAmmoBox" ) : _( L"Team2SpawnerAmmoBox" );
                            if ( vcruntime->wcsstr( actor_name, name ) )
                            {
                                if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                                {

                                    char buf[ 1024 ];
                                    ImFormatString( buf, sizeof buf, _( "Ammocrate [%im]" ), distance );

                                    render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                        ImVec4(
                                            vars::visuals::world::ammocrate_color[ 0 ],
                                            vars::visuals::world::ammocrate_color[ 1 ],
                                            vars::visuals::world::ammocrate_color[ 2 ],
                                            1.f ) ), buf, font );

                                }
                            }
                        }
                    }
                }
                if ( vars::visuals::transport::enable )
                {
                    if ( distance < vars::visuals::transport::distance )
                    {
                        Vector2 screen_pos{};
                        if ( vars::visuals::transport::kamaz && vcruntime->wcsstr( actor_name, _( L"BP_Kamaz_5350_" ) ) )
                        {
                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {

                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "Kamaz [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::kamaz_color[ 0 ],
                                        vars::visuals::transport::kamaz_color[ 1 ],
                                        vars::visuals::transport::kamaz_color[ 2 ],
                                        1.f ) ), buf, font );

                            }
                        }
                        if ( vars::visuals::transport::minsk )
                        {
                            if ( vcruntime->wcsstr( actor_name, _( L"BP_minsk_" ) ) || vcruntime->wcsstr( actor_name, _( L"BP_Minsk_" ) ) )
                            {
                                if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                                {

                                    char buf[ 1024 ];
                                    ImFormatString( buf, sizeof buf, _( "Minsk [%im]" ), distance );

                                    render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                        ImVec4(
                                            vars::visuals::transport::minsk_color[ 0 ],
                                            vars::visuals::transport::minsk_color[ 1 ],
                                            vars::visuals::transport::minsk_color[ 2 ],
                                            1.f ) ), buf, font );
                                }
                            }
                        }
                        if ( vars::visuals::transport::tech_pickup )
                        {
                            if ( vcruntime->wcsstr( actor_name, _( L"BP_Technical_Transport_" ) ) || vcruntime->wcsstr( actor_name, _( L"BP_Technical_Dshk_" ) )
                                || vcruntime->wcsstr( actor_name, _( L"BP_Technical_Logi_" ) ) || vcruntime->wcsstr( actor_name, _( L"BP_Technical_SPG9_" ) )
                                || vcruntime->wcsstr( actor_name, _( L"BP_Technical_UB32_" ) ) || vcruntime->wcsstr( actor_name, _( L"BP_M1151_Technical_C" ) )
                                || vcruntime->wcsstr( actor_name, _( L"BP_Technical2Seater_" ) ) || vcruntime->wcsstr( actor_name, _( L"BP_Technical4Seater_" ) ) )
                            {
                                if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                                {
                                    char buf[ 1024 ];
                                    ImFormatString( buf, sizeof buf, _( "Pickup Truck [%im]" ), distance );

                                    render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                        ImVec4(
                                            vars::visuals::transport::tech_color[ 0 ],
                                            vars::visuals::transport::tech_color[ 1 ],
                                            vars::visuals::transport::tech_color[ 2 ],
                                            1.f ) ), buf, font );
                                }
                            }
                        }
                        if ( vars::visuals::transport::jeep && vcruntime->wcsstr( actor_name, _( L"BP_Safir_" ) ) )
                        {
                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "Jeep [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::jeep_color[ 0 ],
                                        vars::visuals::transport::jeep_color[ 1 ],
                                        vars::visuals::transport::jeep_color[ 2 ],
                                        1.f ) ), buf, font );

                            }
                        }
                        if ( vars::visuals::transport::luv && vcruntime->wcsstr( actor_name, _( L"BP_M-Gator_" ) ) )
                        {
                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "LUV-A1 [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::luv_color[ 0 ],
                                        vars::visuals::transport::luv_color[ 1 ],
                                        vars::visuals::transport::luv_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::m939 && vcruntime->wcsstr( actor_name, _( L"BP_US_Util_" ) ) )
                        {
                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "M939 [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::m939_color[ 0 ],
                                        vars::visuals::transport::m939_color[ 1 ],
                                        vars::visuals::transport::m939_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::man && vcruntime->wcsstr( actor_name, _( L"BP_Brit_Util_" ) ) )
                        {
                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "MAN [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::man_color[ 0 ],
                                        vars::visuals::transport::man_color[ 1 ],
                                        vars::visuals::transport::man_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::ural375D && vcruntime->wcsstr( actor_name, _( L"BP_Ural_375_" ) ) )
                        {
                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "Ural 375D [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::ural375D_color[ 0 ],
                                        vars::visuals::transport::ural375D_color[ 1 ],
                                        vars::visuals::transport::ural375D_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::ural4320 && vcruntime->wcsstr( actor_name, _( L"BP_Ural_4320_" ) ) )
                        {
                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "Ural 4320 [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::ural4320_color[ 0 ],
                                        vars::visuals::transport::ural4320_color[ 1 ],
                                        vars::visuals::transport::ural4320_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::matv && vcruntime->wcsstr( actor_name, _( L"BP_MATV_" ) ) )
                        {
                            if ( vcruntime->wcsstr( actor_name, _( L"BP_MATV_Turret" ) ) ) continue;

                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "M-ATV [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::matv_color[ 0 ],
                                        vars::visuals::transport::matv_color[ 1 ],
                                        vars::visuals::transport::matv_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::tigrm && vcruntime->wcsstr( actor_name, _( L"BP_Tigr_" ) ) )
                        {
                            if ( vcruntime->wcsstr( actor_name, _( L"BP_Tigr_Kord_Turret" ) ) ) continue;

                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "Tigr-M [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::tigrm_color[ 0 ],
                                        vars::visuals::transport::tigrm_color[ 1 ],
                                        vars::visuals::transport::tigrm_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::tapv && vcruntime->wcsstr( actor_name, _( L"BP_TAPV_" ) ) )
                        {
                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "TAPV [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::tapv_color[ 0 ],
                                        vars::visuals::transport::tapv_color[ 1 ],
                                        vars::visuals::transport::tapv_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::brdm && vcruntime->wcsstr( actor_name, _( L"BP_BRDM-2_" ) ) )
                        {
                            if ( vcruntime->wcsstr( actor_name, _( L"BP_BRDM-2_periscope_" ) ) ) continue;

                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "BRDM [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::brdm_color[ 0 ],
                                        vars::visuals::transport::brdm_color[ 1 ],
                                        vars::visuals::transport::brdm_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::lppv && vcruntime->wcsstr( actor_name, _( L"BP_LPPV_" ) ) )
                        {
                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "Ocelot [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::lppv_color[ 0 ],
                                        vars::visuals::transport::lppv_color[ 1 ],
                                        vars::visuals::transport::lppv_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::btr80 && vcruntime->wcsstr( actor_name, _( L"BP_BTR80_" ) ) )
                        {
                            if ( vcruntime->wcsstr( actor_name, _( L"BP_BTR80_Turret" ) ) ) continue;

                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "BTR-80 [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::btr80_color[ 0 ],
                                        vars::visuals::transport::btr80_color[ 1 ],
                                        vars::visuals::transport::btr80_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::btr82a && vcruntime->wcsstr( actor_name, _( L"BP_BTR82A_" ) ) )
                        {
                            if ( vcruntime->wcsstr( actor_name, _( L"BP_BTR82A_Turret" ) ) ) continue;

                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "BTR-82A [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::btr82a_color[ 0 ],
                                        vars::visuals::transport::btr82a_color[ 1 ],
                                        vars::visuals::transport::btr82a_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::lav6 && vcruntime->wcsstr( actor_name, _( L"BP_LAV6_" ) ) )
                        {
                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "LAV 6 [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::lav6_color[ 0 ],
                                        vars::visuals::transport::lav6_color[ 1 ],
                                        vars::visuals::transport::lav6_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::lav3 && vcruntime->wcsstr( actor_name, _( L"BP_LAV_RWS_" ) ) )
                        {
                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "LAV 3 [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::lav3_color[ 0 ],
                                        vars::visuals::transport::lav3_color[ 1 ],
                                        vars::visuals::transport::lav3_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::m113a3 && vcruntime->wcsstr( actor_name, _( L"BP_M113A3_" ) ) )
                        {
                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "M113A3 [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::m113a3_color[ 0 ],
                                        vars::visuals::transport::m113a3_color[ 1 ],
                                        vars::visuals::transport::m113a3_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::m1126 && vcruntime->wcsstr( actor_name, _( L"BP_M1126_" ) ) )
                        {
                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "M1126 [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::m1126_color[ 0 ],
                                        vars::visuals::transport::m1126_color[ 1 ],
                                        vars::visuals::transport::m1126_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::fv432 && vcruntime->wcsstr( actor_name, _( L"BP_FV432_" ) ) )
                        {
                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "FV432 [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::fv432_color[ 0 ],
                                        vars::visuals::transport::fv432_color[ 1 ],
                                        vars::visuals::transport::fv432_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::fv107 && vcruntime->wcsstr( actor_name, _( L"BP_FV107_" ) ) )
                        {
                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "FV107 [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::fv107_color[ 0 ],
                                        vars::visuals::transport::fv107_color[ 1 ],
                                        vars::visuals::transport::fv107_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::mtlb )
                        {
                            if ( vcruntime->wcsstr( actor_name, _( L"BP_MTLB_VMK_" ) ) || vcruntime->wcsstr( actor_name, _( L"BP_MTLBM_6MA_" ) )
                                || vcruntime->wcsstr( actor_name, _( L"BP_MTLB_ZU23_" ) ) || vcruntime->wcsstr( actor_name, _( L"BP_MTLB_INS_" ) )
                                || vcruntime->wcsstr( actor_name, _( L"BP_MTLBM_6MB_" ) ) )
                            {
                                if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                                {
                                    char buf[ 1024 ];
                                    ImFormatString( buf, sizeof buf, _( "MT-LB [%im]" ), distance );

                                    render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                        ImVec4(
                                            vars::visuals::transport::mtlb_color[ 0 ],
                                            vars::visuals::transport::mtlb_color[ 1 ],
                                            vars::visuals::transport::mtlb_color[ 2 ],
                                            1.f ) ), buf, font );
                                }
                            }
                        }
                        if ( vars::visuals::transport::bmp1 && vcruntime->wcsstr( actor_name, _( L"BP_BMP1_" ) ) )
                        {
                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "BMP-1 [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::bmp1_color[ 0 ],
                                        vars::visuals::transport::bmp1_color[ 1 ],
                                        vars::visuals::transport::bmp1_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::bmp2 && vcruntime->wcsstr( actor_name, _( L"BP_BMP2_" ) ) )
                        {
                            if ( vcruntime->wcsstr( actor_name, _( L"BP_BMP2_Turret" ) ) ) continue;

                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {

                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "BMP-2 [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::bmp2_color[ 0 ],
                                        vars::visuals::transport::bmp2_color[ 1 ],
                                        vars::visuals::transport::bmp2_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::m2a3 && vcruntime->wcsstr( actor_name, _( L"BP_BFV_" ) ) )
                        {
                            if ( vcruntime->wcsstr( actor_name, _( L"BP_BFV_Turret" ) ) ) continue;

                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "M2A3 [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::m2a3_color[ 0 ],
                                        vars::visuals::transport::m2a3_color[ 1 ],
                                        vars::visuals::transport::m2a3_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::fv510 )
                        {
                            if ( vcruntime->wcsstr( actor_name, _( L"BP_FV510_" ) ) || vcruntime->wcsstr( actor_name, _( L"BP_FV510UA_" ) ) )
                            {
                                if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                                {
                                    char buf[ 1024 ];
                                    ImFormatString( buf, sizeof buf, _( "FV510 [%im]" ), distance );

                                    render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                        ImVec4(
                                            vars::visuals::transport::fv510_color[ 0 ],
                                            vars::visuals::transport::fv510_color[ 1 ],
                                            vars::visuals::transport::fv510_color[ 2 ],
                                            1.f ) ), buf, font );
                                }
                            }
                        }
                        if ( vars::visuals::transport::fv4034 && vcruntime->wcsstr( actor_name, _( L"BP_2A6_" ) ) )
                        {
                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "FV4034 [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::fv4034_color[ 0 ],
                                        vars::visuals::transport::fv4034_color[ 1 ],
                                        vars::visuals::transport::fv4034_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::msvs && vcruntime->wcsstr( actor_name, _( L"BP_CAF_Util_" ) ) )
                        {
                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "MSVS [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::msvs_color[ 0 ],
                                        vars::visuals::transport::msvs_color[ 1 ],
                                        vars::visuals::transport::msvs_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::m1a2 && vcruntime->wcsstr( actor_name, _( L"BP_M1A2_" ) ) )
                        {
                            if ( vcruntime->wcsstr( actor_name, _( L"BP_M1A2_Loaders_Turret" ) ) ) continue;

                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "M1A2 [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::m1a2_color[ 0 ],
                                        vars::visuals::transport::m1a2_color[ 1 ],
                                        vars::visuals::transport::m1a2_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::t62 && vcruntime->wcsstr( actor_name, _( L"BP_T62_" ) ) )
                        {
                            if ( vcruntime->wcsstr( actor_name, _( L"BP_T62_Turret" ) ) ) continue;

                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "T-62 [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::t62_color[ 0 ],
                                        vars::visuals::transport::t62_color[ 1 ],
                                        vars::visuals::transport::t62_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::t72b3 )
                        {
                            if ( vcruntime->wcsstr( actor_name, _( L"BP_T72B3_" ) ) || vcruntime->wcsstr( actor_name, _( L"BP_T72AV_" ) ) )
                            {
                                if ( vcruntime->wcsstr( actor_name, _( L"BP_T72B3_Turret" ) ) ) continue;

                                if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                                {
                                    char buf[ 1024 ];
                                    ImFormatString( buf, sizeof buf, _( "T-72 [%im]" ), distance );

                                    render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                        ImVec4(
                                            vars::visuals::transport::t72b3_color[ 0 ],
                                            vars::visuals::transport::t72b3_color[ 1 ],
                                            vars::visuals::transport::t72b3_color[ 2 ],
                                            1.f ) ), buf, font );
                                }
                            }
                        }
                        if ( vars::visuals::transport::mi8 && vcruntime->wcsstr( actor_name, _( L"BP_MI8_C" ) ) )
                        {
                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "Mi-8 [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::mi8_color[ 0 ],
                                        vars::visuals::transport::mi8_color[ 1 ],
                                        vars::visuals::transport::mi8_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::mi17 && vcruntime->wcsstr( actor_name, _( L"BP_MI17_MEA_C" ) ) )
                        {
                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "Mi-17 [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::mi17_color[ 0 ],
                                        vars::visuals::transport::mi17_color[ 1 ],
                                        vars::visuals::transport::mi17_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::uh60 && vcruntime->wcsstr( actor_name, _( L"BP_UH60_C" ) ) )
                        {
                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "UH-60M [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ), ImGui::GetColorU32(
                                    ImVec4(
                                        vars::visuals::transport::uh60_color[ 0 ],
                                        vars::visuals::transport::uh60_color[ 1 ],
                                        vars::visuals::transport::uh60_color[ 2 ],
                                        1.f ) ), buf, font );
                            }
                        }
                        if ( vars::visuals::transport::sa330 && vcruntime->wcsstr( actor_name, _( L"BP_SA330_C" ) ) )
                        {
                            if ( sdk::world_to_screen( playercontroller, location, &screen_pos ) )
                            {
                                char buf[ 1024 ];
                                ImFormatString( buf, sizeof buf, _( "SA330 [%im]" ), distance );

                                render->esp_draw( ImGui::GetOverlayDrawList(), ImVec2( screen_pos.x, screen_pos.y ),
                                    ImGui::GetColorU32(
                                        ImVec4(
                                            vars::visuals::transport::sa330_color[ 0 ],
                                            vars::visuals::transport::sa330_color[ 1 ],
                                            vars::visuals::transport::sa330_color[ 2 ],
                                            1.f ) ),
                                    buf, font );
                            }
                        }
                    }
                }
            }
        }
    }
}