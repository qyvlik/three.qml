#include "plane.h"

#include "box3.h"
#include "sphere.h"

namespace three {

double Plane::distanceToSphere(const Sphere &sphere) const
{
    return this->distanceToPoint( sphere.center ) - sphere.radius;
}

bool Plane::intersectsBox(const Box3 &box) const
{
    return box.intersectsPlane( *this );
}

bool Plane::intersectsSphere(const Sphere &sphere) const
{
    return sphere.intersectsPlane( *this );
}



} // namespace three

