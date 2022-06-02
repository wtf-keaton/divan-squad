#pragma once
#include "sdk.hh"

namespace sdk
{
    Vector3 get_bone_with_rotation( USkeletalMeshComponent* mesh, int id );

    FMatrix* get_bone_matrix( USkeletalMeshComponent*, FMatrix*, int );

    const wchar_t* get_player_name( uint64_t ActorAddress );
    const wchar_t* get_object_name( AActor* Actor );

    bool world_to_screen( APlayerController* controller, Vector3 world, Vector2* screen );
    FString GOB( AActor* actor );
}