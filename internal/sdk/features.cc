#include "features.hh"

#include "../divancrt/engine/engine.hh"
#include "../spoofcall/spoof.hh"

FMatrix* sdk::get_bone_matrix( USkeletalMeshComponent* mesh, FMatrix* matrix, int bone )
{
    static uintptr_t address = 0;
    if ( !address )
        address = utils->absolute_address( reinterpret_cast< uintptr_t >( utils->find_pattern( _( "E8 ? ? ? ? 0F 57 DB 0F 2F 9B ? ? ? ?" ) ) ), 1, 5 );

    using GetBoneMatrix = FMatrix * ( * )( USkeletalMeshComponent*, FMatrix*, int );

    const auto getbonematrix = ( GetBoneMatrix ) ( address );

    return getbonematrix( mesh, matrix, bone );
}

Vector3 sdk::get_bone_with_rotation( USkeletalMeshComponent* mesh, int id )
{
    FMatrix v_matrix;
    FMatrix* v_temp_matrix = get_bone_matrix( mesh, &v_matrix, id );
    return v_matrix.WPlane;
}

const wchar_t* sdk::get_player_name( uint64_t ActorAddress )
{
    auto player_pawn = *( uint64_t* ) ( ActorAddress + 0x148 );
    if ( player_pawn != 0x0 )
    {
        auto player_state_info = *( uint64_t* ) ( player_pawn + 0x268 );
        if ( player_state_info != 0x0 )
        {
            auto player_name = *( FString* ) ( player_state_info + 0x250 );
            if ( player_name.IsValid() )
            {
                auto player_name_str = player_name.c_str();
                return player_name_str;
            }
        }
    }
    return _( L"None" );
}

bool sdk::world_to_screen( APlayerController* controller, Vector3 world, Vector2* screen )
{
    static uintptr_t address = 0;
    if ( !address )
        address = reinterpret_cast< uintptr_t >( utils->find_pattern( _( "48 89 5C ? ? 48 89 6C ? ? 48 89 74 ? ? 57 48 81 EC ? ? ? ? 41 0F B6 E9" ) ) );

    using WorldToScreen = bool( * )( APlayerController*, Vector3, Vector2* );

    const auto worldtoscreen = ( WorldToScreen ) ( address );

    return worldtoscreen( controller, world, screen );
}

FString sdk::GOB( AActor* actor )
{
    static uintptr_t address = 0;
    if ( !address )
        address = reinterpret_cast< uintptr_t >( utils->find_pattern( _( "48 89 4C 24 ? 53 48 83 EC 30 48 C7 44 24 ? ? ? ? ? 48 8B D9 48 85 D2 75 31 33 C0 48 89 01 48 C7 41 ? ? ? ? ? E8 ? ? ? ? 41 B8 ? ? ? ? 48 8D 15 ? ? ? ? 48 8B 0B E8 ? ? ? ? 90 48 8B C3 48 83 C4 30 5B C3 48 8B 42 18" ) ) );

    using GetObjectName = FString( * )( AActor* Actor );

    auto get_object_name = ( GetObjectName ) ( address );

    return get_object_name( actor );
}

const wchar_t* sdk::get_object_name( AActor* actor )
{
    FString gob = GOB( actor );

    if ( !gob.IsValid() )
        return L"\0";

    static WCHAR actors_name[ 128 ];
    vcruntime->strcpy( actors_name, gob.c_str() );
    engine::functions::free( *( void** ) &gob );

    return actors_name;
}