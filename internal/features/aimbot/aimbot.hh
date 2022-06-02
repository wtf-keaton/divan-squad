#pragma once
#include "../../sdk/sdk.hh"

#include "../../variables.hh"

class c_aimbot {
public:
    bool exec( UWorld* world );

private:
    const wchar_t* GOB( AActor* Actor );

    void VectorAngles( const Vector3& forward, Vector3& angles )
    {
        float	tmp, yaw, pitch;
        if ( forward[ 1 ] == 0 && forward[ 0 ] == 0 )
        {
            yaw = 0;
            if ( forward[ 2 ] > 0 )
                pitch = 270;
            else
                pitch = 90;
        }
        else
        {
            yaw = ( vcruntime->fast_atan2( forward[ 1 ], forward[ 0 ] ) * 180 / M_PI );
            if ( yaw < 0 )
                yaw += 360;

            tmp = vcruntime->sqrt( forward[ 0 ] * forward[ 0 ] + forward[ 1 ] * forward[ 1 ] );
            pitch = ( vcruntime->fast_atan2( -forward[ 2 ], tmp ) * 180 / M_PI );
            if ( pitch < 0 )
                pitch += 360;
        }

        angles[ 0 ] = pitch;
        angles[ 1 ] = yaw;
        angles[ 2 ] = 0;
    }

    void AngleVectors( const Vector3& angles, Vector3& forward )
    {
        float	sp, sy, cp, cy;

        vcruntime->sincos( DegreeToRadian( angles[ 0 ] ), &sp, &cp );
        vcruntime->sincos( DegreeToRadian( angles[ 1 ] ), &sy, &cy );

        forward.x = cp * cy;
        forward.y = cp * sy;
        forward.z = -sp;
    }

    auto smooth( Vector3 cur_angle, Vector3 aim_angle, Vector3& angle )
    {
        Vector3 vAimAngle = Vector3{};
        Vector3 vCurrentAngle = Vector3{};

        AngleVectors( aim_angle, vAimAngle );
        AngleVectors( cur_angle, vCurrentAngle );

        Vector3 delta = vAimAngle - vCurrentAngle;
        Vector3 smoothed = vCurrentAngle + delta / vars::aimbot::smooth;
        VectorAngles( smoothed, angle );
        vcruntime->NormalizeAngles( angle );
        vcruntime->ClampAngles( angle );
    }

    Vector3 Clamp( Vector3 r )
    {
        if ( r.y > 180.f )
            r.y -= 360.f;
        else if ( r.y < -180.f )
            r.y += 360.f;

        if ( r.x > 180.f )
            r.x -= 360.f;
        else if ( r.x < -180.f )
            r.x += 360.f;

        if ( r.x < -89.f )
            r.x = -89.f;
        else if ( r.x > 89.f )
            r.x = 89.f;

        r.z = 0.f;

        return r;
    }

    Vector3 Vec2Rot( Vector3 vec )
    {
        Vector3 rot = Vector3( 0, 0, 0 );

        rot.y = RadianToDegree( vcruntime->fast_atan2( vec.y, vec.x ) );
        rot.x = RadianToDegree( vcruntime->fast_atan2( vec.z, vcruntime->sqrt( vec.x * vec.x + vec.y * vec.y ) ) );
        rot.z = 0.f;

        return rot;
    }

    void LookAt( APlayerController* m_Player, Vector3 position );

};

inline c_aimbot* aimbot;