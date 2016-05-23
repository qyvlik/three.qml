#include "quaternion.h"

#include "vector3.h"

namespace three {

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

