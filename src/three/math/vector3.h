#ifndef THREE_VECTOR3_H
#define THREE_VECTOR3_H

#include <QtDebug>

#include <QVector>

#include "math_forword_declar.h"
#include "math.hpp"

namespace three {

class Matrix3;
class Quaternion;

class Vector3
{
public:

    Vector3():
        x(0),
        y(0),
        z(0)
    {}

    Vector3(const double& x, const double& y, const double& z):
        x(x),
        y(y),
        z(z)
    {}

    Vector3& set(const double& x, const double& y, const double& z )
    {
        this->x = x;
        this->y = y;
        this->z = z;
        return *this;
    }

    Vector3& setScalar(const double& scalar )
    {
        this->x = scalar;
        this->y = scalar;
        this->z = scalar;
        return *this;
    }

    Vector3& setX(const double& x )
    {
        this->x = x;
        return *this;
    }

    Vector3& setY(const double& y )
    {
        this->y = y;
        return *this;
    }

    Vector3& setZ(const double& z )
    {
        this->z = z;
        return *this;
    }

    void setComponent( const int& index, const double& value )
    {
        Q_ASSERT(index == 0 || index == 1 || index == 2);
        switch ( index )
        {
        case 0: this->x = value; break;
        case 1: this->y = value; break;
        case 2: this->z = value; break;
            // default: throw new Error( 'index is out of range: ' + index );
        }
    }

    double getComponent(const int& index ) const
    {

        Q_ASSERT(index == 0 || index == 1 || index == 2);
        switch ( index ) {
        case 0: return this->x;
        case 1: return this->y;
        case 2: return this->z;
            // default: throw new Error( 'index is out of range: ' + index );
        }

    }

    Vector3 clone() const
    {
        return Vector3( this->x, this->y, this->z );
    }

    Vector3& copy( const Vector3& v )
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        return *this;
    }

    Vector3& add(const Vector3& v)
    {
        this->x += v.x;
        this->y += v.y;
        this->z += v.z;
        return *this;
    }

    Vector3& add (const Vector3& v, const Vector3& w)
    {
        return this->addVectors( v, w );
    }

    Vector3& addScalar( const double& s )
    {
        this->x += s;
        this->y += s;
        this->z += s;
        return *this;
    }

    Vector3& addVectors( const Vector3& a, const Vector3& b)
    {
        this->x = a.x + b.x;
        this->y = a.y + b.y;
        this->z = a.z + b.z;
        return *this;
    }

    Vector3& addScaledVector(const Vector3& v, const double& s )
    {
        this->x += v.x * s;
        this->y += v.y * s;
        this->z += v.z * s;
        return *this;
    }

    Vector3& sub(const Vector3& v )
    {
        this->x -= v.x;
        this->y -= v.y;
        this->z -= v.z;
        return *this;
    }

    Vector3& sub(const Vector3& v , const Vector3& w)
    {
        return this->subVectors(v, w);
    }

    Vector3& subScalar( const double& s )
    {
        this->x -= s;
        this->y -= s;
        this->z -= s;
        return *this;
    }

    Vector3& subVectors(const Vector3& a, const Vector3& b )
    {
        this->x = a.x - b.x;
        this->y = a.y - b.y;
        this->z = a.z - b.z;
        return *this;
    }

    Vector3& multiply(const Vector3& v )
    {
        this->x *= v.x;
        this->y *= v.y;
        this->z *= v.z;
        return *this;
    }

    Vector3& multiply(const Vector3& v , const Vector3&w )
    {
        return  this->multiplyVectors( v, w );
    }

    Vector3& multiplyScalar(const double& scalar )
    {
        this->x *= scalar;
        this->y *= scalar;
        this->z *= scalar;
        return *this;
    }

    Vector3& multiplyVectors (const Vector3& a, const Vector3& b )
    {
        this->x = a.x * b.x;
        this->y = a.y * b.y;
        this->z = a.z * b.z;
        return *this;
    }

    Vector3& applyEuler(const Euler &euler);

    Vector3& applyAxisAngle (const Vector3& axis, const double& angle);

    Vector3& applyMatrix3 (const Matrix3& m );

    Vector3& applyMatrix4 (const Matrix4& m );

    Vector3& applyProjection (const Matrix4& m );

    Vector3& applyQuaternion (const Quaternion& q );

    // // TODO
    //    Vector3& project (camera)
    //    {
    //        Matrix4 matrix;
    //        matrix.multiplyMatrices( camera.projectionMatrix, matrix.getInverse( camera.matrixWorld ) );
    //        return this->applyProjection( matrix );
    //    }

    // // TODO
    //    Vector3& unproject (camera)
    //    {
    //        // static
    //        Matrix4 matrix;
    //        matrix.multiplyMatrices( camera.matrixWorld, matrix.getInverse( camera.projectionMatrix ) );
    //        return this->applyProjection( matrix );
    //    }

    Vector3& transformDirection ( const Matrix4& m );

    Vector3& divide(const Vector3& v )
    {
        Q_ASSERT(v.x != 0 && v.y != 0 && v.z != 0);
        this->x /= v.x;
        this->y /= v.y;
        this->z /= v.z;
        return *this;
    }

    Vector3& divideScalar(const double& scalar )
    {
        Q_ASSERT(scalar != 0);
        return this->multiplyScalar( 1 / scalar );
    }

    Vector3& min( const Vector3&v )
    {
        this->x = std::min( this->x, v.x );
        this->y = std::min( this->y, v.y );
        this->z = std::min( this->z, v.z );
        return *this;
    }

    Vector3& max( const Vector3&v )
    {
        this->x = std::max( this->x, v.x );
        this->y = std::max( this->y, v.y );
        this->z = std::max( this->z, v.z );
        return *this;
    }

    Vector3& clamp(const Vector3& min, const Vector3& max )
    {
        // This function assumes min < max, if this assumption isn't true it will not operate correctly
        this->x = std::max( min.x, std::min( max.x, this->x ) );
        this->y = std::max( min.y, std::min( max.y, this->y ) );
        this->z = std::max( min.z, std::min( max.z, this->z ) );
        return *this;
    }

    Vector3& clampScalar(const double& minVal, const double& maxVal)
    {
        Vector3 min, max;
        min.set( minVal, minVal, minVal );
        max.set( maxVal, maxVal, maxVal );
        return this->clamp( min, max );
    }

    Vector3& clampLength(const double& min, const double& max )
    {
        double length = this->length();

        Q_ASSERT(length != 0);

        this->multiplyScalar( std::max( min, std::min( max, length ) ) / length );
        return *this;
    }

    Vector3& floor()
    {
        this->x = std::floor( this->x );
        this->y = std::floor( this->y );
        this->z = std::floor( this->z );
        return *this;
    }

    Vector3& ceil()
    {
        this->x = std::ceil( this->x );
        this->y = std::ceil( this->y );
        this->z = std::ceil( this->z );
        return *this;
    }

    Vector3& round()
    {
        this->x = std::round( this->x );
        this->y = std::round( this->y );
        this->z = std::round( this->z );
        return *this;
    }

    Vector3& roundToZero()
    {
        this->x = ( this->x < 0 ) ? std::ceil( this->x ) : std::floor( this->x );
        this->y = ( this->y < 0 ) ? std::ceil( this->y ) : std::floor( this->y );
        this->z = ( this->z < 0 ) ? std::ceil( this->z ) : std::floor( this->z );
        return *this;
    }

    Vector3& negate()
    {
        this->x = - this->x;
        this->y = - this->y;
        this->z = - this->z;
        return *this;
    }

    double dot(const Vector3& v ) const
    {
        return this->x * v.x + this->y * v.y + this->z * v.z;
    }

    double lengthSq() const
    {
        return this->x * this->x + this->y * this->y + this->z * this->z;
    }

    double length() const
    {
        return std::sqrt( this->x * this->x + this->y * this->y + this->z * this->z );
    }

    double lengthManhattan ()
    {
        return std::abs( this->x ) + std::abs( this->y ) + std::abs( this->z );
    }

    Vector3& normalize ()
    {
        return this->divideScalar( this->length() );
    }

    Vector3& setLength( const double& length )
    {
        return this->multiplyScalar( length / this->length() );
    }

    Vector3& lerp(const  Vector3& v, const double&alpha )
    {
        this->x += ( v.x - this->x ) * alpha;
        this->y += ( v.y - this->y ) * alpha;
        this->z += ( v.z - this->z ) * alpha;

        return *this;
    }

    Vector3&  lerpVectors(const Vector3& v1, const Vector3& v2,const double& alpha )
    {
        this->subVectors( v2, v1 ).multiplyScalar( alpha ).add( v1 );
        return *this;
    }

    Vector3& cross(const Vector3& v)
    {
        double x = this->x, y = this->y, z = this->z;
        this->x = y * v.z - z * v.y;
        this->y = z * v.x - x * v.z;
        this->z = x * v.y - y * v.x;
        return *this;
    }

    Vector3& cross(const Vector3& v, const Vector3&  w)
    {
        return this->crossVectors(v,w);
    }

    Vector3& crossVectors(const Vector3& a, const Vector3& b )
    {
        double ax = a.x, ay = a.y, az = a.z;
        double bx = b.x, by = b.y, bz = b.z;
        this->x = ay * bz - az * by;
        this->y = az * bx - ax * bz;
        this->z = ax * by - ay * bx;
        return *this;
    }

    Vector3& projectOnVector(const Vector3& vector)
    {
        // static
        Vector3 v1;
        double dot;
        v1.copy( vector ).normalize();
        dot = this->dot( v1 );
        return this->copy( v1 ).multiplyScalar( dot );
    }

    Vector3&  projectOnPlane(const Vector3& planeNormal)
    {
        Vector3 v1;
        v1.copy( *this ).projectOnVector( planeNormal );
        return this->sub( v1 );
    }

    Vector3& reflect(const Vector3& normal )
    {
        // static
        Vector3 v1;
        return this->sub( v1.copy( normal ).multiplyScalar( 2 * this->dot( normal ) ) );
    }

    double angleTo(const Vector3& v )
    {
        double theta = this->dot( v ) / ( std::sqrt( this->lengthSq() * v.lengthSq() ) );
        // clamp, to handle numerical problems
        return std::acos( Math::clamp<double>( theta, - 1, 1 ) );
    }

    double distanceTo(const Vector3& v )
    {
        return std::sqrt( this->distanceToSquared( v ) );
    }

    double distanceToSquared(const Vector3& v ) const
    {
        double dx = this->x - v.x;
        double dy = this->y - v.y;
        double dz = this->z - v.z;
        return dx * dx + dy * dy + dz * dz;
    }

    Vector3& setFromMatrixPosition(const Matrix4& m );

    Vector3& setFromMatrixScale(const Matrix4& m );

    // TODO  Matrix3 or Matrix4;
    Vector3& setFromMatrixColumn( int index, const Matrix4& matrix );

    bool equals(const Vector3& v ) const
    {
        return ( ( v.x == this->x ) && ( v.y == this->y ) && ( v.z == this->z ) );
    }

    Vector3& fromArray( const Float32Array& array, const int& offset = 0 ) {
        this->x = array[ offset ];
        this->y = array[ offset + 1 ];
        this->z = array[ offset + 2 ];
        return *this;
    }

    Float32Array& toArray( Float32Array& array,  const int& offset = 0) {
        array[ offset ] = this->x;
        array[ offset + 1 ] = this->y;
        array[ offset + 2 ] = this->z;
        return array;
    }

    // TODO
    //    fromAttribute( attribute, index, offset = 0 ) {
    //        index = index * attribute.itemSize + offset;
    //        this->x = attribute.array[ index ];
    //        this->y = attribute.array[ index + 1 ];
    //        this->z = attribute.array[ index + 2 ];
    //        return *this;
    //    }


    // private:
    double x;
    double y;
    double z;
};

} // namespace three

#endif // THREE_VECTOR3_H
