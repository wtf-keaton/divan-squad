#include "misc.hh"

#include "../../variables.hh"

bool c_misc::exec( UWorld* world )
{
    auto gameinstance = world->getOwningGameInstance( );
    if ( !gameinstance ) return false;

    auto localplayer = gameinstance->getLocalPlayer( )[ 0 ];
    if ( !localplayer ) return false;

    auto playercontroller = localplayer->getPlayerController( );
    if ( !playercontroller ) return false;

    auto localpawn = playercontroller->getAcknowledgedPawn( );
    if ( !localpawn ) return false;

    auto localmesh = localpawn->getRootComponent( );
    if ( !localmesh ) return false;

    auto localplayerstate = localpawn->getPlayerState( );
    if ( !localplayerstate ) return false;

    auto localsoldier = localplayerstate->getSoldier( );
    if ( !localsoldier ) return false;

    auto soldiermovement = localsoldier->getSoldierMovement( );
    if ( !soldiermovement ) return false;

    if ( vars::misc::enable )
    {
        if ( vars::misc::speed_hack )
            soldiermovement->setSprintSpeedMultiplier( vars::misc::speed_value );
        else
            soldiermovement->setSprintSpeedMultiplier( 0.600f );
    }
}