#ifndef THREE_QUATERNION_H
#define THREE_QUATERNION_H

#include <QtDebug>
#include <QtMath>
#include <QVector>

#include <functional>

#include "vector3.h"

namespace three {

class Vector3;

class Quaternion
{
public:
    Quaternion():
        x(0),
        y(0),
        z(0),
        w(1)
    {}

    Quaternion(const double& x,const double& y, const double& z, const double& w ):
        x(x),
        y(y),
        z(z),
        w(w)
    {}

    Quaternion& set(const double& x, const double& y, const double& z,const double& w )
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;

        return *this;
    }

    Quaternion clone()
    {
        return Quaternion( this->x, this->y, this->z, this->w );
    }

    Quaternion& copy( const Quaternion& quaternion )
    {
        this->x = quaternion.x;
        this->y = quaternion.y;
        this->z = quaternion.z;
        this->w = quaternion.w;
        return *this;
    }

    // TODO
    //   Quaternion& setFromEuler( euler, update ) {
    //        if ( euler instanceof THREE.Euler == false ) {
    //            throw new Error( 'THREE.Quaternion: .setFromEuler() now expects a Euler rotation rather than a Vector3 and order.' );
    //        }
    //        // http://www.mathworks.com/matlabcentral/fileexchange/
    //        // 	20696-function-to-convert-between-dcm-euler-angles-quaternions-and-euler-vectors/
    //        //	content/SpinCalc.m
    //        var c1 = std::cos( euler.x / 2 );
    //        var c2 = std::cos( euler.y / 2 );
    //        var c3 = std::cos( euler.z / 2 );
    //        var s1 = std::sin( euler.x / 2 );
    //        var s2 = std::sin( euler.y / 2 );
    //        var s3 = std::sin( euler.z / 2 );
    //        var order = euler.order;
    //        if ( order == 'XYZ' ) {
    //            this->x = s1 * c2 * c3 + c1 * s2 * s3;
    //            this->y = c1 * s2 * c3 - s1 * c2 * s3;
    //            this->z = c1 * c2 * s3 + s1 * s2 * c3;
    //            this->w = c1 * c2 * c3 - s1 * s2 * s3;
    //        } else if ( order == 'YXZ' ) {
    //            this->x = s1 * c2 * c3 + c1 * s2 * s3;
    //            this->y = c1 * s2 * c3 - s1 * c2 * s3;
    //            this->z = c1 * c2 * s3 - s1 * s2 * c3;
    //            this->w = c1 * c2 * c3 + s1 * s2 * s3;
    //        } else if ( order == 'ZXY' ) {
    //            this->x = s1 * c2 * c3 - c1 * s2 * s3;
    //            this->y = c1 * s2 * c3 + s1 * c2 * s3;
    //            this->z = c1 * c2 * s3 + s1 * s2 * c3;
    //            this->w = c1 * c2 * c3 - s1 * s2 * s3;
    //        } else if ( order == 'ZYX' ) {
    //            this->x = s1 * c2 * c3 - c1 * s2 * s3;
    //            this->y = c1 * s2 * c3 + s1 * c2 * s3;
    //            this->z = c1 * c2 * s3 - s1 * s2 * c3;
    //            this->w = c1 * c2 * c3 + s1 * s2 * s3;
    //        } else if ( order == 'YZX' ) {
    //            this->x = s1 * c2 * c3 + c1 * s2 * s3;
    //            this->y = c1 * s2 * c3 + s1 * c2 * s3;
    //            this->z = c1 * c2 * s3 - s1 * s2 * c3;
    //            this->w = c1 * c2 * c3 - s1 * s2 * s3;
    //        } else if ( order == 'XZY' ) {
    //            this->x = s1 * c2 * c3 - c1 * s2 * s3;
    //            this->y = c1 * s2 * c3 - s1 * c2 * s3;
    //            this->z = c1 * c2 * s3 + s1 * s2 * c3;
    //            this->w = c1 * c2 * c3 + s1 * s2 * s3;
    //        }
    //        if ( update != false )
    //            return *this;
    //    }

    Quaternion& setFromAxisAngle(const Vector3& axis,const double& angle )
    {
        // http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToQuaternion/index.htm
        // assumes axis is normalized
        double halfAngle = angle / 2, s = std::sin( halfAngle );
        this->x = axis.x * s;
        this->y = axis.y * s;
        this->z = axis.z * s;
        this->w = std::cos( halfAngle );
        return *this;
    }

    // TODO
    //   Quaternion& setFromRotationMatrix( m ) {
    //        // http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
    //        // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)
    //        var te = m.elements,

    //                m11 = te[ 0 ], m12 = te[ 4 ], m13 = te[ 8 ],
    //                m21 = te[ 1 ], m22 = te[ 5 ], m23 = te[ 9 ],
    //                m31 = te[ 2 ], m32 = te[ 6 ], m33 = te[ 10 ],

    //                trace = m11 + m22 + m33,
    //                s;
    //        if ( trace > 0 ) {
    //            s = 0.5 / std::sqrt( trace + 1.0 );
    //            this->w = 0.25 / s;
    //            this->x = ( m32 - m23 ) * s;
    //            this->y = ( m13 - m31 ) * s;
    //            this->z = ( m21 - m12 ) * s;
    //        } else if ( m11 > m22 && m11 > m33 ) {
    //            s = 2.0 * std::sqrt( 1.0 + m11 - m22 - m33 );
    //            this->w = ( m32 - m23 ) / s;
    //            this->x = 0.25 * s;
    //            this->y = ( m12 + m21 ) / s;
    //            this->z = ( m13 + m31 ) / s;
    //        } else if ( m22 > m33 ) {
    //            s = 2.0 * std::sqrt( 1.0 + m22 - m11 - m33 );
    //            this->w = ( m13 - m31 ) / s;
    //            this->x = ( m12 + m21 ) / s;
    //            this->y = 0.25 * s;
    //            this->z = ( m23 + m32 ) / s;
    //        } else {
    //            s = 2.0 * std::sqrt( 1.0 + m33 - m11 - m22 );
    //            this->w = ( m21 - m12 ) / s;
    //            this->x = ( m13 + m31 ) / s;
    //            this->y = ( m23 + m32 ) / s;
    //            this->z = 0.25 * s;
    //        }
    //        return *this;
    //    }

    Quaternion& setFromUnitVectors(const Vector3& vFrom, const Vector3& vTo )
    {
        // http://lolengine.net/blog/2014/02/24/quaternion-from-two-vectors-final
        // assumes direction vectors vFrom and vTo are normalized
        Vector3 v1;
        double r;
        double EPS = 0.000001;
        r = vFrom.dot( vTo ) + 1;
        if ( r < EPS ) {
            r = 0;
            if ( std::abs( vFrom.x ) > std::abs( vFrom.z ) ) {
                v1.set( - vFrom.y, vFrom.x, 0 );
            } else {
                v1.set( 0, - vFrom.z, vFrom.y );
            }
        } else {
            v1.crossVectors( vFrom, vTo );
        }
        this->x = v1.x;
        this->y = v1.y;
        this->z = v1.z;
        this->w = r;

        this->normalize();
        return *this;
    }

    Quaternion& inverse()
    {
        this->conjugate().normalize();
        return *this;
    }

    Quaternion& conjugate()
    {
        this->x *= - 1;
        this->y *= - 1;
        this->z *= - 1;
        return *this;
    }

    double dot( const Quaternion& v )
    {
        return   this->x * v.x
                + this->y * v.y
                + this->z * v.z
                + this->w * v.w;
    }

    double lengthSq()
    {
        return    this->x * this->x
                + this->y * this->y
                + this->z * this->z
                + this->w * this->w;

    }

    double length()
    {
        return std::sqrt(   this->x * this->x
                            + this->y * this->y
                            + this->z * this->z
                            + this->w * this->w );

    }

    Quaternion& normalize()
    {
        double l = this->length();
        if ( l == 0 ) {
            this->x = 0;
            this->y = 0;
            this->z = 0;
            this->w = 1;
        } else {
            l = 1 / l;
            this->x = this->x * l;
            this->y = this->y * l;
            this->z = this->z * l;
            this->w = this->w * l;
        }
        return *this;
    }

    Quaternion& multiply( const Quaternion& q)
    {
        return this->multiplyQuaternions( *this, q );
    }


    Quaternion& multiply( const Quaternion& q, const Quaternion&  p )
    {
        return this->multiplyQuaternions( q, p );
    }

    Quaternion& multiplyQuaternions(const Quaternion& a, const Quaternion& b )
    {

        // from http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/code/index.htm
        double qax = a.x, qay = a.y, qaz = a.z, qaw = a.w;
        double qbx = b.x, qby = b.y, qbz = b.z, qbw = b.w;
        this->x = qax * qbw + qaw * qbx + qay * qbz - qaz * qby;
        this->y = qay * qbw + qaw * qby + qaz * qbx - qax * qbz;
        this->z = qaz * qbw + qaw * qbz + qax * qby - qay * qbx;
        this->w = qaw * qbw - qax * qbx - qay * qby - qaz * qbz;
        return *this;
    }

    Quaternion& slerp(const Quaternion& qb, const double& t )
    {

        if ( t == 0 ) return *this;
        if ( t == 1 ) return this->copy( qb );

        double x = this->x, y = this->y, z = this->z, w = this->w;

        // http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/

        double cosHalfTheta = w * qb.w + x * qb.x + y * qb.y + z * qb.z;
        if ( cosHalfTheta < 0 ) {
            this->w = - qb.w;
            this->x = - qb.x;
            this->y = - qb.y;
            this->z = - qb.z;
            cosHalfTheta = - cosHalfTheta;
        } else {
            this->copy( qb );
        }
        if ( cosHalfTheta >= 1.0 ) {
            this->w = w;
            this->x = x;
            this->y = y;
            this->z = z;
            return *this;
        }

        double sinHalfTheta = std::sqrt( 1.0 - cosHalfTheta * cosHalfTheta );
        if ( std::abs( sinHalfTheta ) < 0.001 ) {
            this->w = 0.5 * ( w + this->w );
            this->x = 0.5 * ( x + this->x );
            this->y = 0.5 * ( y + this->y );
            this->z = 0.5 * ( z + this->z );
            return *this;
        }

        double halfTheta = std::atan2( sinHalfTheta, cosHalfTheta );
        double ratioA = std::sin( ( 1 - t ) * halfTheta ) / sinHalfTheta,
                ratioB = std::sin( t * halfTheta ) / sinHalfTheta;

        this->w = ( w * ratioA + this->w * ratioB );
        this->x = ( x * ratioA + this->x * ratioB );
        this->y = ( y * ratioA + this->y * ratioB );
        this->z = ( z * ratioA + this->z * ratioB );
        return *this;
    }

    bool equals(const Quaternion& quaternion ) {

        return     ( quaternion.x == this->x )
                && ( quaternion.y == this->y )
                && ( quaternion.z == this->z )
                && ( quaternion.w == this->w );

    }

    static Quaternion& slerp( Quaternion& qa,  Quaternion&qb,  Quaternion&qm, const double& t ) {
        return qm.copy( qa ).slerp( qb, t );

    }

    // TODO
    //   Quaternion& fromArray( array, const int & offset = 0 ) {
    //        this->x = array[ offset ];
    //        this->y = array[ offset + 1 ];
    //        this->z = array[ offset + 2 ];
    //        this->w = array[ offset + 3 ];
    //        return *this;
    //    }

    // TODO
    //    toArray( array,  const int & offset = 0 ) {
    //        if ( array == undefined ) array = [];
    //        array[ offset ] = this->x;
    //        array[ offset + 1 ] = this->y;
    //        array[ offset + 2 ] = this->z;
    //        array[ offset + 3 ] = this->w;
    //        return array;
    //    }


    // TODO
    //    Quaternion& slerpFlat( dst, dstOffset, src0, srcOffset0, src1, srcOffset1, t ) {

    //		// fuzz-free, array-based Quaternion SLERP operation

    //		var x0 = src0[ srcOffset0 + 0 ],
    //			y0 = src0[ srcOffset0 + 1 ],
    //			z0 = src0[ srcOffset0 + 2 ],
    //			w0 = src0[ srcOffset0 + 3 ],

    //			x1 = src1[ srcOffset1 + 0 ],
    //			y1 = src1[ srcOffset1 + 1 ],
    //			z1 = src1[ srcOffset1 + 2 ],
    //			w1 = src1[ srcOffset1 + 3 ];

    //		if ( w0 != w1 || x0 != x1 || y0 != y1 || z0 != z1 ) {

    //			var s = 1 - t,

    //				cos = x0 * x1 + y0 * y1 + z0 * z1 + w0 * w1,

    //				dir = ( cos >= 0 ? 1 : - 1 ),
    //				sqrSin = 1 - cos * cos;

    //			// Skip the Slerp for tiny steps to avoid numeric problems:
    //			if ( sqrSin > Number.EPSILON ) {

    //				var sin = Math.sqrt( sqrSin ),
    //					len = Math.atan2( sin, cos * dir );

    //				s = Math.sin( s * len ) / sin;
    //				t = Math.sin( t * len ) / sin;

    //			}

    //			var tDir = t * dir;

    //			x0 = x0 * s + x1 * tDir;
    //			y0 = y0 * s + y1 * tDir;
    //			z0 = z0 * s + z1 * tDir;
    //			w0 = w0 * s + w1 * tDir;

    //			// Normalize in case we just did a lerp:
    //			if ( s == 1 - t ) {

    //				var f = 1 / Math.sqrt( x0 * x0 + y0 * y0 + z0 * z0 + w0 * w0 );

    //				x0 *= f;
    //				y0 *= f;
    //				z0 *= f;
    //				w0 *= f;

    //			}

    //		}

    //		dst[ dstOffset ] = x0;
    //		dst[ dstOffset + 1 ] = y0;
    //		dst[ dstOffset + 2 ] = z0;
    //		dst[ dstOffset + 3 ] = w0;

    //	}

    // private:
    double x;
    double y;
    double z;
    double w;

};

} // namespace three

#endif // THREE_QUATERNION_H
