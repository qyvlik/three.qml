#include "euler.h"

#include "quaternion.h"
#include "vector3.h"

namespace three {

//void Euler::reorder(Euler::RotationOrders newOrder)
//{
//    Quaternion q ;
//    q.setFromEuler( this );
//    this->setFromQuaternion( q, newOrder );
//}

Euler &Euler::setFromVector3(const Vector3 &v, Euler::RotationOrders order)
{
    return this->set( v.x, v.y, v.z, order);
}

Vector3 Euler::toVector3()
{
    return  Vector3( this->x, this->y, this->z );
}



} // namespace three

