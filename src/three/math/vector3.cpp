#include "vector3.h"

#include "matrix3.h"
#include "matrix4.h"
#include "quaternion.h"

namespace three {

Vector3 &Vector3::applyAxisAngle(const Vector3 &axis, const double &angle)
{
    Quaternion quaternion;
    this->applyQuaternion( quaternion.setFromAxisAngle(axis, angle ) );
    return *this;
}

Vector3 &Vector3::applyMatrix3(const Matrix3 &m)
{
    double x = this->x;
    double y = this->y;
    double z = this->z;
    const auto& e = m.elements;
    this->x = e[ 0 ] * x + e[ 3 ] * y + e[ 6 ] * z;
    this->y = e[ 1 ] * x + e[ 4 ] * y + e[ 7 ] * z;
    this->z = e[ 2 ] * x + e[ 5 ] * y + e[ 8 ] * z;
    return *this;
}

Vector3 &Vector3::applyMatrix4(const Matrix4 &m)
{
    double  x = this->x,
            y = this->y,
            z = this->z;
    const auto& e = m.elements;
    this->x = e[ 0 ] * x + e[ 4 ] * y + e[ 8 ]  * z + e[ 12 ];
    this->y = e[ 1 ] * x + e[ 5 ] * y + e[ 9 ]  * z + e[ 13 ];
    this->z = e[ 2 ] * x + e[ 6 ] * y + e[ 10 ] * z + e[ 14 ];
    return *this;
}

Vector3 &Vector3::applyProjection(const Matrix4 &m)
{
    double x = this->x, y = this->y, z = this->z;
    const auto& e = m.elements;
    double d = 1 / ( e[ 3 ] * x + e[ 7 ] * y + e[ 11 ] * z + e[ 15 ] ); // perspective divide
    this->x = ( e[ 0 ] * x + e[ 4 ] * y + e[ 8 ]  * z + e[ 12 ] ) * d;
    this->y = ( e[ 1 ] * x + e[ 5 ] * y + e[ 9 ]  * z + e[ 13 ] ) * d;
    this->z = ( e[ 2 ] * x + e[ 6 ] * y + e[ 10 ] * z + e[ 14 ] ) * d;
    return *this;
}

Vector3 &Vector3::applyQuaternion(const Quaternion &q)
{
    double x = this->x;
    double y = this->y;
    double z = this->z;
    double qx = q.x;
    double qy = q.y;
    double qz = q.z;
    double qw = q.w;
    // calculate quat * vector
    double ix =  qw * x + qy * z - qz * y;
    double iy =  qw * y + qz * x - qx * z;
    double iz =  qw * z + qx * y - qy * x;
    double iw = - qx * x - qy * y - qz * z;
    // calculate result * inverse quat
    this->x = ix * qw + iw * - qx + iy * - qz - iz * - qy;
    this->y = iy * qw + iw * - qy + iz * - qx - ix * - qz;
    this->z = iz * qw + iw * - qz + ix * - qy - iy * - qx;
    return *this;
}

Vector3 &Vector3::transformDirection(const Matrix4 &m)
{
    double x = this->x, y = this->y, z = this->z;
    const auto& e = m.elements;
    this->x = e[ 0 ] * x + e[ 4 ] * y + e[ 8 ]  * z;
    this->y = e[ 1 ] * x + e[ 5 ] * y + e[ 9 ]  * z;
    this->z = e[ 2 ] * x + e[ 6 ] * y + e[ 10 ] * z;
    this->normalize();
    return *this;
}

Vector3 &Vector3::setFromMatrixPosition(const Matrix4 &m)
{
    this->x = m.elements[ 12 ];
    this->y = m.elements[ 13 ];
    this->z = m.elements[ 14 ];
    return *this;
}

Vector3 &Vector3::setFromMatrixScale(const Matrix4 &m) {
    double sx = this->set( m.elements[ 0 ], m.elements[ 1 ], m.elements[ 2 ] ).length();
    double sy = this->set( m.elements[ 4 ], m.elements[ 5 ], m.elements[ 6 ] ).length();
    double sz = this->set( m.elements[ 8 ], m.elements[ 9 ], m.elements[ 10 ] ).length();
    this->x = sx;
    this->y = sy;
    this->z = sz;
    return *this;
}

Vector3 &Vector3::setFromMatrixColumn(int index, const Matrix4 &matrix) {
    int offset = index * 4;
    const auto& me = matrix.elements;

    this->x = me[ offset ];
    this->y = me[ offset + 1 ];
    this->z = me[ offset + 2 ];
    return *this;
}

Vector3 &Vector3::applyEuler(const Euler &euler)
{
    Quaternion quaternion;
    this->applyQuaternion( quaternion.setFromEuler( euler ) );
    return *this;
}



} // namespace three

