#include "misc.hh"

#include "../../variables.hh"

bool c_misc::exec( UWorld* world )
{
    auto gameinstance = world->gameinstance;
    if ( !gameinstance ) return false;

    auto localplayer = gameinstance->localplayer[ 0 ];
    if ( !localplayer ) return false;

    auto playercontroller = localplayer->playercontroller;
    if ( !playercontroller ) return false;

    auto localpawn = playercontroller->acknowledgedpawn;
    if ( !localpawn ) return false;

    auto localplayerstate = localpawn->playerstate;
    if ( !localplayerstate ) return false;

    auto localsoldier = localplayerstate->Soldier;
    if ( !localsoldier ) return false;

    auto soldiermovement = localsoldier->SoldierMovement;
    if ( !soldiermovement ) return false;

    if ( vars::misc::enable )
    {
        if ( vars::misc::speed_hack ) 
            soldiermovement->SprintSpeedMultiplier = vars::misc::speed_value;
        else
            soldiermovement->SprintSpeedMultiplier = 0.600f;
    }
}