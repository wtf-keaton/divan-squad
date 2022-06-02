#pragma once
#include <Windows.h>

#define M_PI                       3.14159265358979323846
#define M_PI_2                     1.5707963267948966
#define M_PI_4                     0.7853981633974483
#define M_PI_F		((float)(M_PI))
//#define DegreeToRadian( Degree )			( ( Degree ) * ( M_PI / 180.0f ) )
//#define RadianToDegree( Radian )			( ( Radian ) * ( 180.0f / M_PI ) )

#define DegreeToRadian(x)  ( (float)(x) * (float)(M_PI_F / 180.f) )
#define RadianToDegree(x)  ( (float)(x) * (float)(180.f / M_PI_F) )

extern class Vector3;

class c_vcruntime {
public:
    void* memcpy( void* dest, const void* src, unsigned __int64 count );
    void* memset( void* src, int val, unsigned __int64 count );
    void* memccpy( void* to, const void* from, int c, unsigned __int64 count );
    void* memchr( const void* s, int c, unsigned __int64 n );
    int  memcmp( const void* s1, const void* s2, unsigned __int64 n );
    int  memicmp( const void* s1, const void* s2, unsigned __int64 n );
    void* memmove( void* dest, const void* src, unsigned __int64 count );

    int toupper( int c );
    int tolower( int c );
    char* _cslwr( char* x );
    char* _csupr( char* x );

    int strlen( const char* string );
    const char* strcpy( char* buffer, const char* string );
    const char* strcpy( char* buffer, const wchar_t* string );
    void strncpy( char* _dst, const char* _src, size_t _n );

    int strcmp( const char* cs, const char* ct );
    int stricmp( const char* cs, const char* ct );

    int stricmp( UCHAR cs, const char* ct );
    wchar_t* _cslwr( wchar_t* x );
    wchar_t* _csupr( wchar_t* x );

    int strlen( const wchar_t* string );
    const wchar_t* strcpy( wchar_t* buffer, const wchar_t* string );
    const wchar_t* strcpy( wchar_t* buffer, const char* string );

    int strcmp( const wchar_t* cs, const wchar_t* ct );
    int stricmp( const wchar_t* cs, const wchar_t* ct );

    int wtoupper( int c );
    int wtolower( int c );

    int towupper( int c );
    int towlower( int c );
    int wstrlen( const wchar_t* s );
    int wstrcmp( const wchar_t* cs, const wchar_t* ct );
    int wstricmp( const wchar_t* cs, const wchar_t* ct );
    wchar_t* wstrstr( const wchar_t* s, const wchar_t* find );
    wchar_t* wstrset( wchar_t* szToFill, int szFill );
    wchar_t* wstrnset( wchar_t* szToFill, int szFill, unsigned __int64 sizeMaxFill );
    wchar_t* wstrrev( wchar_t* s );
    int wstrnicmp( const wchar_t* s1, const wchar_t* s2, unsigned __int64 n );
    wchar_t* wstrlwr( wchar_t* x );
    wchar_t* wstrchr( const wchar_t* s, wchar_t c );
    wchar_t* wstrtok_s( wchar_t* str, const wchar_t* delim, wchar_t** ctx );
    bool iswdigit( wchar_t c );
    __int64 wtoi64( const wchar_t* nptr );
    int wtoi( const wchar_t* nptr );
    wchar_t* itow( int number, wchar_t* destination, int base );
    const char* strstr( char const* _Str, char const* _SubStr );
    char* strcatA( char* dest, const char* src );
    wchar_t* strcatW( wchar_t* dest, const wchar_t* src );
    unsigned int wcslen( const wchar_t* str );

    void __fastcall zero_mem( LPVOID a1, DWORD64 a2 );
    int wcsicmp( const wchar_t* a1, const wchar_t* a2 );

    int abs( int x );
    const char* w2c( const wchar_t* label );

    double floor( double x );
    double ceil( double x );
    char* itoa( int number, char* destination, int base );
    int dec( float val );
    wchar_t* ftoa( float val, wchar_t* str );

    template<class T>
    void ClampAngles( T& angles )
    {
        if ( angles.x > 89.0f ) angles.x = 89.0f;
        else if ( angles.x < -89.0f ) angles.x = -89.0f;

        if ( angles.y > 180.0f ) angles.y = 180.0f;
        else if ( angles.y < -180.0f ) angles.y = -180.0f;

        angles.z = 0;
    }

    template<class T>
    void NormalizeAngles( T& vec )
    {
        for ( auto i = 0; i < 2; i++ )
        {
            while ( vec[ i ] < -180.0f ) vec[ i ] += 360.0f;
            while ( vec[ i ] > 180.0f ) vec[ i ] -= 360.0f;
        }
        vec[ 2 ] = 0.f;
    }

    void sincos( float radians, float* sine, float* cosine );
    int strncmp( const char* s1, const char* s2, register size_t n );
    size_t rfind( const char* s, const char* s2 );
    char* const substr( const char* text, int beg, int end );

    float sin( float x );
    float cos( float x );
    float pow( float x, float y );
    float sqrt( float x );

    const wchar_t* wcsstr( const wchar_t* haystack, const wchar_t* needle );

    float fast_acos( const float x );
    float fast_atan( float v );
    float fast_atan2( float y, float x );


    float get_distance( Vector3 x, Vector3 y );

};
inline c_vcruntime* vcruntime;