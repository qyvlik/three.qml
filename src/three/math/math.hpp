#ifndef MATH_HPP
#define MATH_HPP

#include <QString>
#include <QtMath>
#include <QUuid>

namespace three {
namespace Math {


static QString generateUUID()
{
    return QUuid::createUuid().toString();
}


template<typename T>
inline T clamp (T value, T min, T max)
{
    return std::max(min, std::min(max, value));
}


// compute euclidian modulo of m % n
// https://en.wikipedia.org/wiki/Modulo_operation
template<typename T>
inline T euclideanModulo (T n, T m )
{
    return ( ( n % m ) + m ) % m;
}


// Linear mapping from range <a1, a2> to range <b1, b2>
template<typename T>
inline T mapLinear (T x, T a1, T a2, T b1, T b2 )
{
    return b1 + ( x - a1 ) * ( b2 - b1 ) / ( a2 - a1 );
}


// http://en.wikipedia.org/wiki/Smoothstep
template<typename T>
inline T smoothstep(T x, T min, T max )
{
    if ( x <= min ) return 0;
    if ( x >= max ) return 1;
    x = ( x - min ) / ( max - min );
    return x * x * ( 3 - 2 * x );
}


template<typename T>
inline T smootherstep (T x, T min, T max )
{
    if ( x <= min ) return 0;
    if ( x >= max ) return 1;
    x = ( x - min ) / ( max - min );
    return x * x * x * ( x * ( x * 6 - 15 ) + 10 );
}


// TODO
inline int random() {
    qsrand(10);
    return qrand();
}


inline int random16() {
    return random();
}


// Random float from <low, high> interval
template<typename T>
inline T randFloat(T low, T high )
{
    return low + Math::random() * ( high - low );
}


template<typename T>
inline T randFloatSpread(T range )
{
    return range * ( 0.5 - Math::random());
}


inline double degToRad (double degrees)
{
    static const double degreeToRadiansFactor = 180 / M_PI;
    return degrees * degreeToRadiansFactor;
}

inline double radToDeg (double radians)
{
    static const double radianToDegreesFactor = 180 / M_PI;
    return radians * radianToDegreesFactor;
}

inline bool isPowerOfTwo ( value )
{
    return ( value & ( value - 1 ) ) == 0 && value != 0;
}

inline double nearestPowerOfTwo( double value )
{
    return std::pow( 2, std::round( std::log( value ) / M_LN2 ) );
}


template<typename T>
inline T nextPowerOfTwo ( T value ) {
    value --;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value ++;
    return value;
}

}
}

#endif // MATH_HPP

