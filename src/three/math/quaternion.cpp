#include "quaternion.h"

#include "euler.h"
#include "vector3.h"
#include "matrix4.h"

namespace three {

Quaternion &Quaternion::setFromEuler(const Euler &euler, bool update)
{
    // http://www.mathworks.com/matlabcentral/fileexchange/
    // 	20696-function-to-convert-between-dcm-euler-angles-quaternions-and-euler-vectors/
    //	content/SpinCalc.m

    Q_UNUSED(update);

    double c1 = std::cos( euler.x / 2 );
    double c2 = std::cos( euler.y / 2 );
    double c3 = std::cos( euler.z / 2 );
    double s1 = std::sin( euler.x / 2 );
    double s2 = std::sin( euler.y / 2 );
    double s3 = std::sin( euler.z / 2 );
    Euler::RotationOrders order = euler.order;

    if ( order == Euler::XYZ ) {
        this->x = s1 * c2 * c3 + c1 * s2 * s3;
        this->y = c1 * s2 * c3 - s1 * c2 * s3;
        this->z = c1 * c2 * s3 + s1 * s2 * c3;
        this->w = c1 * c2 * c3 - s1 * s2 * s3;
    } else if ( order == Euler::YXZ ) {
        this->x = s1 * c2 * c3 + c1 * s2 * s3;
        this->y = c1 * s2 * c3 - s1 * c2 * s3;
        this->z = c1 * c2 * s3 - s1 * s2 * c3;
        this->w = c1 * c2 * c3 + s1 * s2 * s3;
    } else if ( order == Euler::ZXY ) {
        this->x = s1 * c2 * c3 - c1 * s2 * s3;
        this->y = c1 * s2 * c3 + s1 * c2 * s3;
        this->z = c1 * c2 * s3 + s1 * s2 * c3;
        this->w = c1 * c2 * c3 - s1 * s2 * s3;
    } else if ( order == Euler::ZYX ) {
        this->x = s1 * c2 * c3 - c1 * s2 * s3;
        this->y = c1 * s2 * c3 + s1 * c2 * s3;
        this->z = c1 * c2 * s3 - s1 * s2 * c3;
        this->w = c1 * c2 * c3 + s1 * s2 * s3;
    } else if ( order == Euler::YZX ) {
        this->x = s1 * c2 * c3 + c1 * s2 * s3;
        this->y = c1 * s2 * c3 + s1 * c2 * s3;
        this->z = c1 * c2 * s3 - s1 * s2 * c3;
        this->w = c1 * c2 * c3 - s1 * s2 * s3;
    } else if ( order == Euler::XZY ) {
        this->x = s1 * c2 * c3 - c1 * s2 * s3;
        this->y = c1 * s2 * c3 - s1 * c2 * s3;
        this->z = c1 * c2 * s3 + s1 * s2 * c3;
        this->w = c1 * c2 * c3 + s1 * s2 * s3;
    }
    //    if ( update != false )
    return *this;
}

Quaternion &Quaternion::setFromAxisAngle(const Vector3 &axis, const double &angle)
{
    // http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToQuaternion/index.htm
    // assumes axis is normalized
    double halfAngle = angle / 2
            , s = std::sin( halfAngle );
    this->x = axis.x * s;
    this->y = axis.y * s;
    this->z = axis.z * s;
    this->w = std::cos( halfAngle );
    return *this;
}

Quaternion &Quaternion::setFromRotationMatrix(const Matrix4 &m) {
    // http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
    // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)
    const auto& te = m.elements;

    double  m11 = te[ 0 ], m12 = te[ 4 ], m13 = te[ 8 ],
            m21 = te[ 1 ], m22 = te[ 5 ], m23 = te[ 9 ],
            m31 = te[ 2 ], m32 = te[ 6 ], m33 = te[ 10 ],

            trace = m11 + m22 + m33,
            s;
    if ( trace > 0 ) {
        s = 0.5 / std::sqrt( trace + 1.0 );
        this->w = 0.25 / s;
        this->x = ( m32 - m23 ) * s;
        this->y = ( m13 - m31 ) * s;
        this->z = ( m21 - m12 ) * s;
    } else if ( m11 > m22 && m11 > m33 ) {
        s = 2.0 * std::sqrt( 1.0 + m11 - m22 - m33 );
        this->w = ( m32 - m23 ) / s;
        this->x = 0.25 * s;
        this->y = ( m12 + m21 ) / s;
        this->z = ( m13 + m31 ) / s;
    } else if ( m22 > m33 ) {
        s = 2.0 * std::sqrt( 1.0 + m22 - m11 - m33 );
        this->w = ( m13 - m31 ) / s;
        this->x = ( m12 + m21 ) / s;
        this->y = 0.25 * s;
        this->z = ( m23 + m32 ) / s;
    } else {
        s = 2.0 * std::sqrt( 1.0 + m33 - m11 - m22 );
        this->w = ( m21 - m12 ) / s;
        this->x = ( m13 + m31 ) / s;
        this->y = ( m23 + m32 ) / s;
        this->z = 0.25 * s;
    }
    return *this;
}

Quaternion &Quaternion::setFromUnitVectors(const Vector3 &vFrom, const Vector3 &vTo)
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



} // namespace three

