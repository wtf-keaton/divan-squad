#include "vcruntime.hh"
#include <intrin.h>

#include "../engine/engine.hh"

void* c_vcruntime::memcpy( void* dest, const void* src, unsigned __int64 count )
{
    char* char_dest = ( char* ) dest;
    char* char_src = ( char* ) src;
    if ( ( char_dest <= char_src ) || ( char_dest >= ( char_src + count ) ) )
    {
        while ( count > 0 )
        {
            *char_dest = *char_src;
            char_dest++;
            char_src++;
            count--;
        }
    }
    else
    {
        char_dest = ( char* ) dest + count - 1;
        char_src = ( char* ) src + count - 1;
        while ( count > 0 )
        {
            *char_dest = *char_src;
            char_dest--;
            char_src--;
            count--;
        }
    }
    return dest;
}
void* c_vcruntime::memset( void* src, int val, unsigned __int64 count )
{
    __stosb( ( unsigned char* ) ( ( unsigned long long )( volatile char* )src ), val, count );
    return src;
}
void* c_vcruntime::memccpy( void* to, const void* from, int c, unsigned __int64 count )
{
    char t;
    unsigned __int64 i;
    char* dst = ( char* ) to;
    const char* src = ( const char* ) from;
    for ( i = 0; i < count; i++ )
    {
        dst[ i ] = t = src[ i ];
        if ( t == 0 ) break;
        if ( t == c ) return &dst[ i + 1 ];
    }
    return 0;
}

void* c_vcruntime::memchr( const void* s, int c, unsigned __int64 n )
{
    if ( n )
    {
        const char* p = ( const char* ) s;
        do
        {
            if ( *p++ == c ) return ( void* ) ( p - 1 );
        } while ( --n != 0 );
    }
    return 0;
}

int  c_vcruntime::memcmp( const void* s1, const void* s2, unsigned __int64 n )
{
    if ( n != 0 )
    {
        const unsigned char* p1 = ( unsigned char* ) s1, * p2 = ( unsigned char* ) s2;
        do
        {
            if ( *p1++ != *p2++ ) return ( *--p1 - *--p2 );
        } while ( --n != 0 );
    }
    return 0;
}
int  c_vcruntime::memicmp( const void* s1, const void* s2, unsigned __int64 n )
{
    if ( n != 0 )
    {
        const unsigned char* p1 = ( unsigned char* ) s1, * p2 = ( unsigned char* ) s2;
        do
        {
            if ( toupper( *p1 ) != toupper( *p2 ) ) return ( *p1 - *p2 );
            p1++;
            p2++;
        } while ( --n != 0 );
    }
    return 0;
}
void* c_vcruntime::memmove( void* dest, const void* src, unsigned __int64 count )
{
    char* char_dest = ( char* ) dest;
    char* char_src = ( char* ) src;
    if ( ( char_dest <= char_src ) || ( char_dest >= ( char_src + count ) ) )
    {
        while ( count > 0 )
        {
            *char_dest = *char_src;
            char_dest++;
            char_src++;
            count--;
        }
    }
    else
    {
        char_dest = ( char* ) dest + count - 1;
        char_src = ( char* ) src + count - 1;
        while ( count > 0 )
        {
            *char_dest = *char_src;
            char_dest--;
            char_src--;
            count--;
        }
    }
    return dest;
}

int c_vcruntime::toupper( int c )
{
    if ( c >= 'a' && c <= 'z' ) return c - 'a' + 'A';
    return c;
}

int c_vcruntime::tolower( int c )
{
    if ( c >= 'A' && c <= 'Z' ) return c - 'A' + 'a';
    return c;
}

char* c_vcruntime::_cslwr( char* x )
{
    char* y = x;
    while ( *y )
    {
        *y = tolower( *y );
        y++;
    }
    return x;
}

char* c_vcruntime::_csupr( char* x )
{
    char* y = x;
    while ( *y )
    {
        *y = tolower( *y );
        y++;
    }
    return x;
}

int c_vcruntime::strlen( const char* string )
{
    int cnt = 0;
    if ( string )
    {
        for ( ; *string != 0; ++string ) ++cnt;
    }
    return cnt;
}

const char* c_vcruntime::strcpy( char* buffer, const char* string )
{
    char* ret = buffer;
    while ( *string ) *buffer++ = *string++;
    *buffer = 0;
    return ret;
}

const char* c_vcruntime::strcpy( char* buffer, const wchar_t* string )
{
    char* ret = buffer;
    while ( *string ) *buffer++ = char( *string++ );
    *buffer = 0;
    return ret;
}

void c_vcruntime::strncpy( char* _dst, const char* _src, size_t _n )
{
    size_t i = 0;
    while ( i++ != _n && ( *_dst++ = *_src++ ) );
}

int c_vcruntime::strcmp( const char* cs, const char* ct )
{
    if ( cs && ct )
    {
        while ( *cs == *ct )
        {
            if ( *cs == 0 && *ct == 0 ) return 0;
            if ( *cs == 0 || *ct == 0 ) break;
            cs++;
            ct++;
        }
        return *cs - *ct;
    }
    return -1;
}

int c_vcruntime::stricmp( const char* cs, const char* ct )
{
    if ( cs && ct )
    {
        while ( tolower( *cs ) == tolower( *ct ) )
        {
            if ( *cs == 0 && *ct == 0 ) return 0;
            if ( *cs == 0 || *ct == 0 ) break;
            cs++;
            ct++;
        }
        return tolower( *cs ) - tolower( *ct );
    }
    return -1;
}

int c_vcruntime::stricmp( UCHAR cs, const char* ct )
{
    if ( ct )
    {
        while ( tolower( cs ) == tolower( *ct ) )
        {
            if ( cs == 0 && *ct == 0 ) return 0;
            if ( cs == 0 || *ct == 0 ) break;

            ct++;
        }
        return tolower( cs ) - tolower( *ct );
    }
    return -1;
}

wchar_t* c_vcruntime::_cslwr( wchar_t* x )
{
    wchar_t* y = x;
    while ( *y )
    {
        *y = towlower( *y );
        y++;
    }
    return x;
}

wchar_t* c_vcruntime::_csupr( wchar_t* x )
{
    wchar_t* y = x;
    while ( *y )
    {
        *y = towupper( *y );
        y++;
    }
    return x;
}

int c_vcruntime::strlen( const wchar_t* string )
{
    int cnt = 0;
    if ( string )
    {
        for ( ; *string != 0; ++string ) ++cnt;
    }
    return cnt;
}

const wchar_t* c_vcruntime::strcpy( wchar_t* buffer, const wchar_t* string )
{
    wchar_t* ret = buffer;
    while ( *string ) *buffer++ = *string++;
    *buffer = L'\0';
    return ret;
}

const wchar_t* c_vcruntime::strcpy( wchar_t* buffer, const char* string )
{
    wchar_t* ret = buffer;
    while ( *string ) *buffer++ = wchar_t( *string++ );
    *buffer = 0;
    return ret;
}

int c_vcruntime::strcmp( const wchar_t* cs, const wchar_t* ct )
{
    if ( cs && ct )
    {
        while ( *cs == *ct )
        {
            if ( *cs == 0 && *ct == 0 ) return 0;
            if ( *cs == 0 || *ct == 0 ) break;
            cs++;
            ct++;
        }
        return *cs - *ct;
    }
    return -1;
}
int c_vcruntime::stricmp( const wchar_t* cs, const wchar_t* ct )
{
    if ( cs && ct )
    {
        while ( towlower( *cs ) == towlower( *ct ) )
        {
            if ( *cs == 0 && *ct == 0 ) return 0;
            if ( *cs == 0 || *ct == 0 ) break;
            cs++;
            ct++;
        }
        return towlower( *cs ) - towlower( *ct );
    }
    return -1;
}

int c_vcruntime::wtoupper( int c )
{
    if ( c >= L'a' && c <= L'z' ) return c - L'a' + L'A';
    if ( c >= L'а' && c <= L'я' ) return c - L'а' + L'А';
    if ( c == L'ё' ) return L'Ё';
    return c;
}

int c_vcruntime::wtolower( int c )
{
    if ( c >= L'A' && c <= L'Z' ) return c - L'A' + L'a';
    if ( c >= L'А' && c <= L'Я' ) return c - L'А' + L'а';
    if ( c == L'Ё' ) return L'ё';
    return c;
}

int c_vcruntime::towupper( int c )
{
    if ( c >= L'a' && c <= L'z' ) return c - L'a' + L'A';
    if ( c >= L'а' && c <= L'я' ) return c - L'а' + L'А';
    if ( c == L'ё' ) return L'Ё';
    return c;
}
int c_vcruntime::towlower( int c )
{
    if ( c >= L'A' && c <= L'Z' ) return c - L'A' + L'a';
    if ( c >= L'А' && c <= L'Я' ) return c - L'А' + L'а';
    if ( c == L'Ё' ) return L'ё';
    return c;
}

int c_vcruntime::wstrlen( const wchar_t* s )
{
    int cnt = 0;
    if ( !s ) return 0;
    for ( ; *s != 0; ++s ) ++cnt;
    return cnt;
}

int c_vcruntime::wstrcmp( const wchar_t* cs, const wchar_t* ct )
{
    if ( cs && ct )
    {
        while ( *cs == *ct )
        {
            if ( *cs == 0 && *ct == 0 ) return 0;
            if ( *cs == 0 || *ct == 0 ) break;
            cs++;
            ct++;
        }
        return *cs - *ct;
    }
    return -1;
}
int c_vcruntime::wstricmp( const wchar_t* cs, const wchar_t* ct )
{
    if ( cs && ct )
    {
        while ( wtolower( *cs ) == wtolower( *ct ) )
        {
            if ( *cs == 0 && *ct == 0 ) return 0;
            if ( *cs == 0 || *ct == 0 ) break;
            cs++;
            ct++;
        }
        return wtolower( *cs ) - wtolower( *ct );
    }
    return -1;
}

wchar_t* c_vcruntime::wstrstr( const wchar_t* s, const wchar_t* find )
{
    wchar_t c, sc;
    if ( ( c = *find++ ) != 0 )
    {
        do
        {
            do
            {
                if ( ( sc = *s++ ) == 0 )
                    return 0;
            } while ( sc != c );
        } while ( wstricmp( s, find ) != 0 );
        s--;
    }
    return ( wchar_t* ) s;
}

wchar_t* c_vcruntime::wstrset( wchar_t* szToFill, int szFill )
{
    wchar_t* t = szToFill;
    while ( *szToFill != 0 )
    {
        *szToFill = szFill;
        szToFill++;
    }
    return t;
}
wchar_t* c_vcruntime::wstrnset( wchar_t* szToFill, int szFill, unsigned __int64 sizeMaxFill )
{
    wchar_t* t = szToFill;
    int i = 0;
    while ( *szToFill != 0 && i < ( int ) sizeMaxFill )
    {
        *szToFill = szFill;
        szToFill++;
        i++;
    }
    return t;
}
wchar_t* c_vcruntime::wstrrev( wchar_t* s )
{
    wchar_t a, * b, * e;
    b = e = s;
    while ( *e ) e++;
    e--;
    while ( b < e )
    {
        a = *b;
        *b = *e;
        *e = a;
        b++;
        e--;
    }
    return s;
}

int c_vcruntime::wstrnicmp( const wchar_t* s1, const wchar_t* s2, unsigned __int64 n )
{
    if ( n == 0 ) return 0;
    do
    {
        if ( wtoupper( *s1 ) != wtoupper( *s2++ ) ) return wtoupper( *( unsigned const char* ) s1 ) - wtoupper( *( unsigned const char* )--s2 );
        if ( *s1++ == 0 ) break;
    } while ( --n != 0 );
    return 0;
}

wchar_t* c_vcruntime::wstrlwr( wchar_t* x )
{
    wchar_t* y = x;
    while ( *y )
    {
        *y = wtolower( *y );
        y++;
    }
    return x;
}

wchar_t* c_vcruntime::wstrchr( const wchar_t* s, wchar_t c )
{
    wchar_t cc = c;
    const wchar_t* sp = ( wchar_t* ) 0;
    while ( *s )
    {
        if ( *s == cc ) sp = s;
        s++;
    }
    if ( cc == 0 ) sp = s;
    return ( wchar_t* ) sp;
}

wchar_t* c_vcruntime::wstrtok_s( wchar_t* str, const wchar_t* delim, wchar_t** ctx )
{
    if ( !str ) str = *ctx;
    while ( *str && wstrchr( delim, *str ) ) str++;
    if ( !*str )
    {
        *ctx = str;
        return 0;
    }
    *ctx = str + 1;
    while ( **ctx && !wstrchr( delim, **ctx ) ) ( *ctx )++;
    if ( **ctx ) *( *ctx )++ = 0;
    return str;
}

bool c_vcruntime::iswdigit( wchar_t c )
{
    return c >= L'0' && c <= L'9';
}

__int64 c_vcruntime::wtoi64( const wchar_t* nptr )
{
    wchar_t* s = ( wchar_t* ) nptr;
    __int64 acc = 0;
    int neg = 0;
    if ( nptr == 0 ) return 0;
    while ( *s = L' ' ) s++;
    if ( *s == L'-' )
    {
        neg = 1;
        s++;
    }
    else if ( *s == L'+' ) s++;
    while ( iswdigit( *s ) )
    {
        acc = 10 * acc + ( *s - L'0' );
        s++;
    }
    if ( neg ) acc *= -1;
    return acc;
}
int c_vcruntime::wtoi( const wchar_t* nptr )
{
    wchar_t* s = ( wchar_t* ) nptr;
    int acc = 0;
    int neg = 0;
    if ( nptr == 0 ) return 0;
    while ( *s = L' ' ) s++;
    if ( *s == L'-' )
    {
        neg = 1;
        s++;
    }
    else if ( *s == L'+' ) s++;
    while ( iswdigit( *s ) )
    {
        acc = 10 * acc + ( *s - L'0' );
        s++;
    }
    if ( neg ) acc *= -1;
    return acc;
}
wchar_t* c_vcruntime::itow( int number, wchar_t* destination, int base )
{
    int count = 0;
    do
    {
        int digit = number % base;
        destination[ count++ ] = ( digit > 9 ) ? digit - 10 + L'A' : digit + L'0';
    } while ( ( number /= base ) != 0 );
    destination[ count ] = 0;
    int i;
    for ( i = 0; i < count / 2; ++i )
    {
        wchar_t symbol = destination[ i ];
        destination[ i ] = destination[ count - i - 1 ];
        destination[ count - i - 1 ] = symbol;
    }
    return destination;
}


const char* c_vcruntime::strstr( char const* _Str, char const* _SubStr )
{
    const char* bp = _SubStr;
    const char* back_pos;
    while ( *_Str != 0 && _Str != 0 && _SubStr != 0 )
    {
        back_pos = _Str;
        while ( tolower( *back_pos++ ) == tolower( *_SubStr++ ) )
        {
            if ( *_SubStr == 0 )
            {
                return ( char* ) ( back_pos - strlen( bp ) );
            }
        }
        ++_Str;
        _SubStr = bp;
    }
    return 0;
}

char* c_vcruntime::strcatA( char* dest, const char* src )
{
    if ( ( dest == 0 ) || ( src == 0 ) )
        return dest;

    while ( *dest != 0 )
        dest++;

    while ( *src != 0 ) {
        *dest = *src;
        dest++;
        src++;
    }

    *dest = 0;
    return dest;
}

wchar_t* c_vcruntime::strcatW( wchar_t* dest, const wchar_t* src )
{
    if ( ( dest == 0 ) || ( src == 0 ) )
        return dest;

    while ( *dest != 0 )
        dest++;

    while ( *src != 0 ) {
        *dest = *src;
        dest++;
        src++;
    }

    *dest = 0;
    return dest;
}

unsigned int c_vcruntime::wcslen( const wchar_t* str )
{
    int counter = 0;
    if ( !str )
        return 0;
    for ( ; *str != '\0'; ++str )
        ++counter;

    return counter;
}

void __fastcall c_vcruntime::zero_mem( LPVOID a1, DWORD64 a2 ) {
    this->memset( ( a1 ), 0, ( a2 ) );
}

int c_vcruntime::wcsicmp( const wchar_t* a1, const wchar_t* a2 ) {

    unsigned __int16* v2; // r10
    DWORD64 v3; // r9
    unsigned __int16 v4; // r8
    unsigned __int16 v5; // cx
    unsigned __int16 v6; // dx
    unsigned __int16 v7; // r8

    v2 = ( unsigned __int16* ) a2;
    v3 = ( DWORD64 ) a1 - ( DWORD64 ) a2;
    do
    {
        v4 = *( unsigned __int16* ) ( ( char* ) v2 + v3 );
        v5 = *v2++;
        v6 = v4 + 32;
        if ( ( unsigned __int16 ) ( v4 - 65 ) > 0x19u )
            v6 = v4;
        v7 = v5 + 32;
        if ( ( unsigned __int16 ) ( v5 - 65 ) > 0x19u )
            v7 = v5;
    } while ( v6 && v6 == v7 );
    return v6 - ( unsigned int ) v7;
}

int c_vcruntime::abs( int x )
{
    int s = x >> 31;
    return ( x ^ s ) - s;
}

const char* c_vcruntime::w2c( const wchar_t* label )
{
    char data[ 256 ];
    for ( auto i = 0; i < this->wcslen( label ); i++ )
        data[ i ] = label[ i ];

    return data;
}

double c_vcruntime::floor( double x )
{
    return ( x >= 0 ) ? ( int ) x : ( x == ( int ) x ? x : ( int ) x - 1 );
}

double c_vcruntime::ceil( double x )
{
    return ( x >= 0 ) ? ( ( x == ( int ) x ) ? ( int ) x : ( int ) x + 1 ) : ( ( x == ( int ) x ) ? ( int ) x + 1 : ( int ) x );
}


char* c_vcruntime::itoa( int number, char* destination, int base )
{
    int count = 0;
    do
    {
        int digit = number % base;
        destination[ count++ ] = ( digit > 9 ) ? digit - 10 + 'A' : digit + '0';
    } while ( ( number /= base ) != 0 );
    destination[ count ] = 0;
    int i;
    for ( i = 0; i < count / 2; ++i )
    {
        wchar_t symbol = destination[ i ];
        destination[ i ] = destination[ count - i - 1 ];
        destination[ count - i - 1 ] = symbol;
    }
    return destination;
}

int c_vcruntime::dec( float val )
{
    int mult = floor( val );

    while ( floor( val ) != ceil( val ) )
    {
        mult *= 10;
        val *= 10;
    }

    return floor( val ) - mult;
}

wchar_t* c_vcruntime::ftoa( float val, wchar_t* str )
{

    wchar_t leading_integer[ 31 ] = { 0 };  // 63 instead of 31 for 64-bit systems
    wchar_t trailing_decimal[ 31 ] = { 0 };  // 63 instead of 31 for 64-bit systems

    /* fill string with leading integer */
    this->itow( floor( val ), leading_integer, 10 );

    /* fill string with the decimal part */
    this->itow( dec( val ), trailing_decimal, 10 );

    /* set given string to full decimal */
    this->strcpy( str, leading_integer );
    this->strcatW( str, L"." );
    this->strcatW( str, trailing_decimal );

    return str;
}



void c_vcruntime::sincos( float radians, float* sine, float* cosine )
{
    *sine = this->sin( radians );
    *cosine = this->cos( radians );
}



int c_vcruntime::strncmp( const char* s1, const char* s2, register size_t n )
{
    register unsigned char u1, u2;

    while ( n-- > 0 )
    {
        u1 = ( unsigned char ) *s1++;
        u2 = ( unsigned char ) *s2++;
        if ( u1 != u2 )
            return u1 - u2;
        if ( u1 == '\0' )
            return 0;
    }
    return 0;
}

size_t c_vcruntime::rfind( const char* s, const char* s2 )
{
    const size_t ln1 = this->strlen( s );
    const size_t ln2 = this->strlen( s2 );

    if ( ln1 < ln2 )
        return size_t( -1 );

    for ( auto* it = s + ln1 - ln2, *end = s - 1; it != end; --it )
        if ( this->strncmp( it, s2, ln2 ) == 0 )
            return size_t( it - s );

    return size_t( -1 );
}

const wchar_t* c_vcruntime::wcsstr( const wchar_t* haystack, const wchar_t* needle )
{
    wchar_t b, c;
    if ( ( b = *needle ) != L'\0' )
    {
        haystack--;
        do
            if ( ( c = *++haystack ) == L'\0' )
                goto ret0;
        while ( c != b );
        if ( !( c = *++needle ) )
            goto foundneedle;
        ++needle;
        goto jin;
        for ( ;;)
        {
            wchar_t a;
            const wchar_t* rhaystack, * rneedle;
            do
            {
                if ( !( a = *++haystack ) )
                    goto ret0;
                if ( a == b )
                    break;
                if ( ( a = *++haystack ) == L'\0' )
                    goto ret0;
            shloop:;
            } while ( a != b );
        jin:          if ( !( a = *++haystack ) )
            goto ret0;
        if ( a != c )
            goto shloop;
        if ( *( rhaystack = haystack-- + 1 ) == ( a = *( rneedle = needle ) ) )
            do
            {
                if ( a == L'\0' )
                    goto foundneedle;
                if ( *++rhaystack != ( a = *++needle ) )
                    break;
                if ( a == L'\0' )
                    goto foundneedle;
            } while ( *++rhaystack == ( a = *++needle ) );
            needle = rneedle;
            if ( a == L'\0' )
                break;
        }
    }
foundneedle:
    return ( wchar_t* ) haystack;
ret0:
    return NULL;
}



const double PI = 3.141592653589793;
const double HALF_PI = 1.570796326794897;
const double DOUBLE_PI = 6.283185307179586;
const double SIN_CURVE_A = 0.0415896;
const double SIN_CURVE_B = 0.00129810625032;

float c_vcruntime::cos( float x )
{
    if ( x < 0 )
    {
        int q = -x / DOUBLE_PI;
        q += 1;
        double y = q * DOUBLE_PI;
        x = -( x - y );
    }
    if ( x >= DOUBLE_PI )
    {
        int q = x / DOUBLE_PI;
        double y = q * DOUBLE_PI;
        x = x - y;
    }
    int s = 1;
    if ( x >= PI )
    {
        s = -1;
        x -= PI;
    }
    if ( x > HALF_PI )
    {
        x = PI - x;
        s = -s;
    }
    double z = x * x;
    double r = z * ( z * ( SIN_CURVE_A - SIN_CURVE_B * z ) - 0.5 ) + 1.0;
    if ( r > 1.0 )
        r = r - 2.0;
    if ( s > 0 )
        return r;
    else return -r;
}

float c_vcruntime::sin( float x )
{

    return cos( x - HALF_PI );
}

float c_vcruntime::pow( float a, float b )
{

    union {
        double d;
        int x[ 2 ];
    } u = { a };
    u.x[ 1 ] = ( int ) ( b * ( u.x[ 1 ] - 1072632447 ) + 1072632447 );
    u.x[ 0 ] = 0;
    return u.d;
}

float c_vcruntime::sqrt( float a )
{

    float b = a;
    int* x = ( int* ) ( &b );
    int c = ( ( *x >> 23 ) & 255 ) - 127;
    if ( c < 0 )
    {
        c = -( ( -c ) >> 1 );
    }
    else
    {
        c >>= 1;
    }

    *x &= ~( 255 << 23 );

    *x |= ( c + 127 ) << 23;

    for ( int i = 0; i < 5; i++ )
    {
        b = ( b + a / b ) / 2;
    }

    return b;
}

float c_vcruntime::fast_atan2( float y, float x )
{
    if ( x >= 0 )
    {
        if ( y >= 0 )
        {
            if ( y < x )
            {
                return fast_atan( y / x );
            }
            else
            {
                return M_PI_2 - fast_atan( x / y );
            }
        }
        else
        {
            if ( -y < x )
            {
                return fast_atan( y / x );
            }
            else
            {
                return -M_PI_2 - fast_atan( x / y );
            }
        }
    }
    else
    {
        if ( y >= 0 )
        {
            if ( y < -x )
            {
                return fast_atan( y / x ) + M_PI;
            }
            else
            {
                return M_PI_2 - fast_atan( x / y );
            }
        }
        else
        {
            if ( -y < -x )
            {
                return fast_atan( y / x ) - M_PI;
            }
            else
            {
                return -M_PI_2 - fast_atan( x / y );
            }
        }
    }
}

float c_vcruntime::fast_acos( const float x )
{
    return ( -0.69813170079773212 * x * x - 0.87266462599716477 ) * x + 1.5707963267948966;
}

float c_vcruntime::fast_atan( float v )
{
    float v2 = v * v;
    return ( v * ( 1.6867629106f + v2 * 0.4378497304f ) / ( 1.6867633134f + v2 ) );
}

char* const c_vcruntime::substr( const char* text, int beg, int end )
{
    int i;
    char* sub = 0;
    int len = end - beg;
    if ( text )//Проверяем не пустой ли ввод
        if ( text + beg )//Проверяем существование в тексте позиции beg
            if ( 0 < len )//Проверяем корректность параметров конец должен быть больше начала
                if ( ( sub = ( char* ) engine::functions::malloc( 1 + len ) ) )//Если end превосходит последнюю
                    //позицию текста ничего страшного выделим чуть больше памяти чем надо
                {
                    //Примитивное компирование, даже текстовых библиотек не надо будет
                    for ( i = beg; text[ i ] != '\0' && i < end; i++ )
                        sub[ i - beg ] = text[ i ];
                    sub[ i - beg ] = '\0';//Ноль терминатор вконце строки
                }

    static CHAR ret[ 128 ];
    this->strcpy( ret, sub );
    engine::functions::free( sub );
    return ret;
}

float c_vcruntime::get_distance( Vector3 x, Vector3 y )
{
    auto z = x - y;
    return vcruntime->sqrt( z.x * z.x + z.y * z.y + z.z * z.z );
}