#ifndef THREE_VECTOR4_H
#define THREE_VECTOR4_H

#include "math_forword_declar.h"

#include <QtDebug>
#include <QtMath>
#include <QVector>

#include "matrix3.h"
#include "matrix4.h"
#include "quaternion.h"

namespace three {

class Vector4
{
public:
    Vector4():
        x(0.0),
        y(0.0),
        z(0.0),
        w(1.0)
    { }

    Vector4(const double& x, const double& y, const double& z, const double& w):
        x(x),
        y(y),
        z(z),
        w(w)
    {}

    Vector4& set(const double& x,const double& y, const double& z,const double& w )
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
        return *this;
    }

    Vector4& setScalar(const double& scalar )
    {
        this->x = scalar;
        this->y = scalar;
        this->z = scalar;
        this->w = scalar;
        return *this;
    }

    Vector4& setX(const double& x )
    {
        this->x = x;
        return *this;
    }

    Vector4& setY(const double& y )
    {
        this->y = y;
        return *this;
    }

    Vector4& setZ(const double& z )
    {
        this->z = z;
        return *this;
    }

    Vector4& setW(const double& w )
    {
        this->w = w;
        return *this;
    }

    void setComponent(const int& index, const double& value ) {
        Q_ASSERT(index <= 3 && index >= 0);

        switch ( index )
        {
        case 0: this->x = value; break;
        case 1: this->y = value; break;
        case 2: this->z = value; break;
        case 3: this->w = value; break;
            // default: throw new Error( 'index is out of range: ' + index );
        }
    }

    double getComponent( const int& index ) {
        Q_ASSERT(index <= 3 && index >= 0);
        switch ( index )
        {
        case 0: return this->x;
        case 1: return this->y;
        case 2: return this->z;
        case 3: return this->w;
            // default: throw new Error( 'index is out of range: ' + index );
        }
    }

    Vector4 clone()
    {
        return Vector4( this->x, this->y, this->z, this->w );
    }

    Vector4& copy(const Vector4& v )
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        this->w = v.w;
        return *this;
    }

    Vector4& add(const Vector4& v )
    {
        this->x += v.x;
        this->y += v.y;
        this->z += v.z;
        this->w += v.w;
        return *this;
    }

    Vector4& add(const Vector4& v,const Vector4& w )
    {
        return this->addVectors( v, w );
    }

    Vector4& addScalar(const double& s )
    {
        this->x += s;
        this->y += s;
        this->z += s;
        this->w += s;
        return *this;
    }

    Vector4& addVectors(const Vector4& a, const Vector4& b )
    {
        this->x = a.x + b.x;
        this->y = a.y + b.y;
        this->z = a.z + b.z;
        this->w = a.w + b.w;
        return *this;
    }

    Vector4& addScaledVector(const Vector4& v, const double& s )
    {
        this->x += v.x * s;
        this->y += v.y * s;
        this->z += v.z * s;
        this->w += v.w * s;
        return *this;
    }

    Vector4& sub(const Vector4& v)
    {
        this->x -= v.x;
        this->y -= v.y;
        this->z -= v.z;
        this->w -= v.w;
        return *this;
    }

    Vector4& sub(const Vector4& v, const Vector4& w )
    {
        return this->subVectors( v, w );
    }


    Vector4& subScalar(const double& s )
    {
        this->x -= s;
        this->y -= s;
        this->z -= s;
        this->w -= s;
        return *this;
    }

    Vector4& subVectors(const Vector4& a, const Vector4& b )
    {
        this->x = a.x - b.x;
        this->y = a.y - b.y;
        this->z = a.z - b.z;
        this->w = a.w - b.w;
        return *this;
    }

    Vector4& multiplyScalar(const double& scalar)
    {
        this->x *= scalar;
        this->y *= scalar;
        this->z *= scalar;
        this->w *= scalar;
        return *this;
    }

    Vector4& applyMatrix4(const Matrix4& m )
    {
        double x = this->x;
        double y = this->y;
        double z = this->z;
        double w = this->w;
        const auto& e = m.elements;
        this->x = e[ 0 ] * x + e[ 4 ] * y + e[ 8 ] * z + e[ 12 ] * w;
        this->y = e[ 1 ] * x + e[ 5 ] * y + e[ 9 ] * z + e[ 13 ] * w;
        this->z = e[ 2 ] * x + e[ 6 ] * y + e[ 10 ] * z + e[ 14 ] * w;
        this->w = e[ 3 ] * x + e[ 7 ] * y + e[ 11 ] * z + e[ 15 ] * w;
        return *this;
    }

    Vector4& divideScalar(const double& scalar )
    {
        Q_ASSERT(scalar != 0);
        return this->multiplyScalar( 1 / scalar );
    }

    // TODO
    Vector4& setAxisAngleFromQuaternion( const Quaternion& q )
    {
        // http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToAngle/index.htm

        // q is assumed to be normalized
        this->w = 2 * std::acos( q.w );
        double s = std::sqrt( 1 - q.w * q.w );
        if ( s < 0.0001 ) {
            this->x = 1;
            this->y = 0;
            this->z = 0;
        } else {

            this->x = q.x / s;
            this->y = q.y / s;
            this->z = q.z / s;
        }
        return *this;
    }

    // TODO Matrix3 or Matrix4
    Vector4& setAxisAngleFromRotationMatrix(const Matrix3& m ) {

        // http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToAngle/index.htm

        // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)

        double  angle, x, y, z,		// variables for result
                epsilon = 0.01,		// margin to allow for rounding errors
                epsilon2 = 0.1;		// margin to distinguish between 0 and 180 degrees

        const auto&    te = m.elements;

        auto    m11 = te[ 0 ], m12 = te[ 3 ], m13 = te[ 6 ],
                m21 = te[ 1 ], m22 = te[ 4 ], m23 = te[ 7 ],
                m31 = te[ 2 ], m32 = te[ 5 ], m33 = te[ 8 ];

        /*
            m11 = te[ 0 ], m12 = te[ 4 ], m13 = te[ 8 ],
            m21 = te[ 1 ], m22 = te[ 5 ], m23 = te[ 9 ],
            m31 = te[ 2 ], m32 = te[ 6 ], m33 = te[ 10 ];
         */


        if ( ( std::abs( m12 - m21 ) < epsilon )
             && ( std::abs( m13 - m31 ) < epsilon )
             && ( std::abs( m23 - m32 ) < epsilon ) ) {

            // singularity found
            // first check for identity matrix which must have +1 for all terms
            // in leading diagonal and zero in other terms

            if ( ( std::abs( m12 + m21 ) < epsilon2 )
                 && ( std::abs( m13 + m31 ) < epsilon2 )
                 && ( std::abs( m23 + m32 ) < epsilon2 )
                 && ( std::abs( m11 + m22 + m33 - 3 ) < epsilon2 ) ) {

                // this singularity is identity matrix so angle = 0

                this->set( 1, 0, 0, 0 );

                return *this; // zero angle, arbitrary axis

            }

            // otherwise this singularity is angle = 180

            angle = M_PI;

            double xx = ( m11 + 1 ) / 2;
            double yy = ( m22 + 1 ) / 2;
            double zz = ( m33 + 1 ) / 2;
            double xy = ( m12 + m21 ) / 4;
            double xz = ( m13 + m31 ) / 4;
            double yz = ( m23 + m32 ) / 4;

            if ( ( xx > yy ) && ( xx > zz ) ) {

                // m11 is the largest diagonal term

                if ( xx < epsilon ) {

                    x = 0;
                    y = 0.707106781;
                    z = 0.707106781;

                } else {

                    x = std::sqrt( xx );
                    y = xy / x;
                    z = xz / x;

                }

            } else if ( yy > zz ) {

                // m22 is the largest diagonal term

                if ( yy < epsilon ) {

                    x = 0.707106781;
                    y = 0;
                    z = 0.707106781;

                } else {

                    y = std::sqrt( yy );
                    x = xy / y;
                    z = yz / y;

                }

            } else {

                // m33 is the largest diagonal term so base result on this

                if ( zz < epsilon ) {

                    x = 0.707106781;
                    y = 0.707106781;
                    z = 0;

                } else {

                    z = std::sqrt( zz );
                    x = xz / z;
                    y = yz / z;

                }

            }

            this->set( x, y, z, angle );

            return *this; // return 180 deg rotation

        }

        // as we have reached here there are no singularities so we can handle normally

        double s = std::sqrt( ( m32 - m23 ) * ( m32 - m23 )
                           + ( m13 - m31 ) * ( m13 - m31 )
                           + ( m21 - m12 ) * ( m21 - m12 ) ); // used to normalize

        if ( std::abs( s ) < 0.001 ) s = 1;

        // prevent divide by zero, should not happen if matrix is orthogonal and should be
        // caught by singularity test above, but I've left it in just in case

        this->x = ( m32 - m23 ) / s;
        this->y = ( m13 - m31 ) / s;
        this->z = ( m21 - m12 ) / s;
        this->w = std::acos( ( m11 + m22 + m33 - 1 ) / 2 );

        return *this;

    }


    Vector4& min(const Vector4& v )
    {
        this->x = std::min( this->x, v.x );
        this->y = std::min( this->y, v.y );
        this->z = std::min( this->z, v.z );
        this->w = std::min( this->w, v.w );
        return *this;
    }

    Vector4& max(const Vector4& v )
    {
        this->x = std::max( this->x, v.x );
        this->y = std::max( this->y, v.y );
        this->z = std::max( this->z, v.z );
        this->w = std::max( this->w, v.w );
        return *this;
    }

    Vector4& clamp(const Vector4& min, const Vector4& max )
    {
        // This function assumes min < max, if this assumption isn't true it will not operate correctly
        this->x = std::max( min.x, std::min( max.x, this->x ) );
        this->y = std::max( min.y, std::min( max.y, this->y ) );
        this->z = std::max( min.z, std::min( max.z, this->z ) );
        this->w = std::max( min.w, std::min( max.w, this->w ) );
        return *this;
    }

    Vector4& clampScalar(const double& minVal, const double&  maxVal )
    {
        // static
        Vector4 min, max;
        min.set( minVal, minVal, minVal, minVal );
        max.set( maxVal, maxVal, maxVal, maxVal );
        return this->clamp( min, max );
    }

    Vector4& floor()
    {
        this->x = std::floor( this->x );
        this->y = std::floor( this->y );
        this->z = std::floor( this->z );
        this->w = std::floor( this->w );
        return *this;
    }

    Vector4& ceil()
    {
        this->x = std::ceil( this->x );
        this->y = std::ceil( this->y );
        this->z = std::ceil( this->z );
        this->w = std::ceil( this->w );
        return *this;
    }

    Vector4& round()
    {
        this->x = std::round( this->x );
        this->y = std::round( this->y );
        this->z = std::round( this->z );
        this->w = std::round( this->w );
        return *this;
    }

    Vector4& roundToZero()
    {
        this->x = ( this->x < 0 ) ? std::ceil( this->x ) : std::floor( this->x );
        this->y = ( this->y < 0 ) ? std::ceil( this->y ) : std::floor( this->y );
        this->z = ( this->z < 0 ) ? std::ceil( this->z ) : std::floor( this->z );
        this->w = ( this->w < 0 ) ? std::ceil( this->w ) : std::floor( this->w );
        return *this;
    }

    Vector4& negate()
    {
        this->x = - this->x;
        this->y = - this->y;
        this->z = - this->z;
        this->w = - this->w;
        return *this;
    }

    double dot(const Vector4& v)
    {
        return this->x * v.x + this->y * v.y + this->z * v.z + this->w * v.w;
    }

    double lengthSq()
    {
        return this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w;
    }

    double length()
    {
        return std::sqrt( this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w );
    }

    double lengthManhattan()
    {
        return std::abs( this->x ) + std::abs( this->y ) + std::abs( this->z ) + std::abs( this->w );
    }

    Vector4& normalize()
    {
        return this->divideScalar( this->length() );
    }

    Vector4& setLength(const double& length )
    {
        return this->multiplyScalar( length / this->length() );
    }

    Vector4& lerp(const Vector4& v, const double& alpha )
    {
        this->x += ( v.x - this->x ) * alpha;
        this->y += ( v.y - this->y ) * alpha;
        this->z += ( v.z - this->z ) * alpha;
        this->w += ( v.w - this->w ) * alpha;
        return *this;
    }

    Vector4& lerpVectors( const Vector4& v1,const Vector4&  v2, const double& alpha )
    {
        this->subVectors( v2, v1 ).multiplyScalar( alpha ).add( v1 );
        return *this;
    }

    bool equals(const Vector4& v )
    {
        return ( ( v.x == this->x ) && ( v.y == this->y ) && ( v.z == this->z ) && ( v.w == this->w ) );
    }

    // TODO
    //    fromArray( array, offset ) {

    //        if ( offset == undefined ) offset = 0;
    //        this->x = array[ offset ];
    //        this->y = array[ offset + 1 ];
    //        this->z = array[ offset + 2 ];
    //        this->w = array[ offset + 3 ];
    //        return *this;
    //    }

    // TODO
    //    toArray( array, offset ) {
    //        if ( array == undefined ) array = [];
    //        if ( offset == undefined ) offset = 0;
    //        array[ offset ] = this->x;
    //        array[ offset + 1 ] = this->y;
    //        array[ offset + 2 ] = this->z;
    //        array[ offset + 3 ] = this->w;
    //        return array;
    //    }

    // TODO
    //    fromAttribute( attribute, index, offset ) {
    //        if ( offset == undefined ) offset = 0;
    //        index = index * attribute.itemSize + offset;
    //        this->x = attribute.array[ index ];
    //        this->y = attribute.array[ index + 1 ];
    //        this->z = attribute.array[ index + 2 ];
    //        this->w = attribute.array[ index + 3 ];
    //        return *this;
    //    }


private:
    double x;
    double y;
    double z;
    double w;
};

} // namespace three

#endif // THREE_VECTOR4_H
