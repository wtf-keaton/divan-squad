#include "aimbot.hh""
#include "../../divancrt/winapi/winapi.hh"
#include "../../sdk/features.hh"

#include "../../spoofcall/spoof.hh"

bool c_aimbot::exec( UWorld* world )
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


    static float c_x = ( ImGui::GetIO().DisplaySize.x / 2.0f );
    static float c_y = ( ImGui::GetIO().DisplaySize.y / 2.0f );

    auto get_len = [&]( Vector2 screen_pos )
    {
        float pos_x = screen_pos.x > c_x ? screen_pos.x - c_x : c_x - screen_pos.x;
        float pos_y = screen_pos.y > c_y ? screen_pos.y - c_y : c_y - screen_pos.y;
        return vcruntime->sqrt( pos_x * pos_x + pos_y * pos_y );
    };

    auto  in_fov = [&]( Vector2 screen_pos, float size )
    {
        return ( vcruntime->sqrt( ( screen_pos.x - c_x ) * ( screen_pos.x - c_x ) + ( screen_pos.y - c_y ) * ( screen_pos.y - c_y ) ) <= size );
    };

    auto closest_actor = [&]( AActor* actor )
    {
        float d_limit = FLT_MAX;
        float length = 0;
        AActor* ret = nullptr;
        auto actor_name = sdk::get_object_name( actor );
        if ( vcruntime->wcsstr( actor_name, _( L"BP_Soldier_" ) ) )
        {
            Vector3 head_pos = sdk::get_bone_with_rotation( actor->instigator->controller->character->Mesh, bones::HEAD );
            Vector2 screen_pos{};

            if ( sdk::world_to_screen( playercontroller, head_pos, &screen_pos ) )
            {
                if ( in_fov( screen_pos, vars::aimbot::FOV ) )
                {
                    length = get_len( screen_pos );
                    if ( length < d_limit )
                    {
                        ret = actor;
                        d_limit = length;
                    }
                }
            }
        }

        return ret;
    };

    auto actors = level->actors;

    for ( size_t i = 0; i < actors.Num(); i++ )
    {
        if ( !actors.IsValidIndex( i ) )
            continue;

        const auto  actor = actors[ i ];

        if ( !actor )
            continue;
        if ( vars::aimbot::enable && winapi::GetAsyncKeyState( VK_RBUTTON ) )
        {
            if ( const auto set = closest_actor( actor ); set != nullptr )
            {
                if ( set->instigator != playercontroller->acknowledgedpawn )
                {
                    auto target_pos = sdk::get_bone_with_rotation( actor->instigator->controller->character->Mesh, bones::HEAD );

                    if ( vars::aimbot::draw_aimbone )
                    {
                        Vector2 screen_head{};
                        if ( sdk::world_to_screen( playercontroller, target_pos, &screen_head ) )
                        {
                            ImGui::GetOverlayDrawList()->AddCircleFilled( { screen_head.x,screen_head.y }, 5.f, ImColor( 255, 0, 0 ), 30 );
                        }
                    }

                    LookAt( localplayer->playercontroller, target_pos );

                }
            }
        }
    }
}

void c_aimbot::LookAt( APlayerController* m_Player, Vector3 position )
{
    Vector3 loc_pos = sdk::get_bone_with_rotation( m_Player->character->Mesh, bones::HEAD );
    Vector3 localRot = m_Player->ControlRotation;

    Vector3 relativePos = position - loc_pos;

    auto ideal = Clamp( Vec2Rot( relativePos ) );
    auto angDelta = Clamp( ideal - localRot );

    auto sets = localRot + angDelta / vars::aimbot::smooth;

    m_Player->ControlRotation = Clamp( sets );
}