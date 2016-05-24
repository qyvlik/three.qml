#ifndef THREE_SPHERE_H
#define THREE_SPHERE_H

#include "math_forword_declar.h"

#include "vector3.h"
#include "box3.h"
#include "plane.h"
#include "matrix4.h"

namespace three {

class Vector3;
class Box3;
class Plane;
class Matrix4;

// 球体对象
class Sphere
{
public:
    Sphere():
        radius(0)
    { }

    Sphere(const Vector3& center, const double& radius):
        center(center),
        radius(radius)
    { }

    Sphere& set(const Vector3& center, const double& radius )
    {
        this->center.copy( center );
        this->radius = radius;
        return *this;
    }

    // TODO
    Sphere& setFromPoints(const Vector3Array& points)
    {
        Box3 box;
        auto& center = this->center;
        box.setFromPoints( points ).center( center );
        double maxRadiusSq = 0;

        for ( auto i = 0, il = points.length(); i < il; i ++ ) {
            maxRadiusSq = std::max( maxRadiusSq, center.distanceToSquared( points[ i ] ) );
        }

        this->radius = std::sqrt( maxRadiusSq );
        return *this;
    }

    // TODO
    Sphere& setFromPoints(const Vector3Array& points, const Vector3& optionalCenter )
    {
        Vector3& center = this->center;
        center.copy( optionalCenter );
        double maxRadiusSq = 0;

        for ( auto i = 0, il = points.length(); i < il; i ++ ) {
            maxRadiusSq = std::max( maxRadiusSq, center.distanceToSquared( points[ i ] ) );
        }

        this->radius = std::sqrt( maxRadiusSq );
        return *this;
    }

    Sphere clone() const
    {
        return Sphere( this->center, this->radius );
    }

    Sphere& copy( const Sphere& sphere )
    {
        this->center.copy( sphere.center );
        this->radius = sphere.radius;
        return *this;
    }

    bool empty() const
    {
        return ( this->radius <= 0 );
    }

    bool containsPoint( const Vector3& point ) const
    {
        return ( point.distanceToSquared( this->center ) <= ( this->radius * this->radius ) );
    }

    double distanceToPoint(const Vector3&  point ) const
    {
        return ( point.distanceTo( this->center ) - this->radius );
    }

    double intersectsSphere( const Sphere& sphere ) const
    {
        double radiusSum = this->radius + sphere.radius;
        return sphere.center.distanceToSquared( this->center ) <= ( radiusSum * radiusSum );
    }

    bool intersectsBox(const Box3& box ) const
    {
        return box.intersectsSphere( *this );
    }

    bool intersectsPlane( const Plane& plane ) const
    {
        // We use the following equation to compute the signed distance from
        // the center of the sphere to the plane.
        //
        // distance = q * n - d
        //
        // If this distance is greater than the radius of the sphere,
        // then there is no intersection.

        return std::abs( this->center.dot( plane.normal ) - plane.constant ) <= this->radius;

    }

    Vector3 clampPoint( const Vector3& point,Vector3&  optionalTarget ) const
    {

        auto deltaLengthSq = this->center.distanceToSquared( point );

        Vector3& result = optionalTarget;

        result.copy( point );

        if ( deltaLengthSq > ( this->radius * this->radius ) ) {

            result.sub( this->center ).normalize();
            result.multiplyScalar( this->radius ).add( this->center );
        }

        return result;
    }

    Box3 getBoundingBox( Box3& optionalTarget ) const
    {
        Box3& box = optionalTarget ;

        box.set( this->center, this->center );
        box.expandByScalar( this->radius );
        return box;
    }

    Sphere& applyMatrix4(const Matrix4& matrix )
    {
        this->center.applyMatrix4( matrix );
        this->radius = this->radius * matrix.getMaxScaleOnAxis();
        return *this;
    }

    Sphere& translate(const Vector3& offset )
    {
        this->center.add( offset );
        return *this;
    }

    bool equals( const Sphere& sphere ) const
    {
        return sphere.center.equals( this->center ) && ( sphere.radius == this->radius );
    }

    // private:
    Vector3 center;
    double radius;
};

} // namespace three

#endif // THREE_SPHERE_H
