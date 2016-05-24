#include "box3.h"

#include "sphere.h"

namespace three {

bool Box3::intersectsSphere(const Sphere &sphere) const
{
    Vector3 closestPoint;
    // Find the point on the AABB closest to the sphere center.
    this->clampPoint( sphere.center, closestPoint );

    // If that point is inside the sphere, the AABB and sphere intersect.
    return closestPoint.distanceToSquared( sphere.center ) <= ( sphere.radius * sphere.radius );
}

Sphere Box3::getBoundingSphere(Sphere &optionalTarget) const
{
    Vector3 v1 ;
    Sphere& result = optionalTarget ;

    result.center = this->center();
    result.radius = this->size( v1 ).length() * 0.5;
    return result;
}



} // namespace three

