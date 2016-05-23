#include "vector3.h"

#include "quaternion.h"

namespace three {

Vector3 &Vector3::applyAxisAngle(Vector3 &axis, const double &angle)
{
    Quaternion quaternion;
    this->applyQuaternion( quaternion.setFromAxisAngle(axis, angle ) );
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

//Vector3 &Vector3::applyEuler(const Quaternion &euler)
//{
//    Quaternion quaternion;
//    this->applyQuaternion( quaternion.setFromEuler( euler ) );
//    return *this;
//}



} // namespace three

