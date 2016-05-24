#ifndef THREE_RAY_H
#define THREE_RAY_H

#include "math_forword_declar.h"

#include "vector3.h"
#include "sphere.h"
#include "plane.h"
#include "box3.h"

#include <tuple>

namespace three {

// 射线对象

class Box3;
class Vector3;
class Sphere;

class Ray
{
public:
    Ray()
    {

    }

    Ray(const Vector3& origin, const Vector3& direction ) :
        origin(origin),
        direction(direction)
    { }

    Ray& set(const Vector3& origin, const Vector3& direction )
    {
        this->origin.copy( origin );
        this->direction.copy( direction );
        return *this;
    }

    Ray clone() const
    {
        return Ray( this->origin, this->direction );
    }

    Ray& copy( const Ray& ray )
    {
        this->origin.copy( ray.origin );
        this->direction.copy( ray.direction );

        return *this;
    }

    Vector3 at(const double& t) const
    {
        Vector3 result ;
        return result.copy( this->direction ).multiplyScalar( t ).add( this->origin );
    }

    Vector3 at(const double& t, Vector3& optionalTarget ) const
    {
        Vector3& result = optionalTarget ;
        return result.copy( this->direction ).multiplyScalar( t ).add( this->origin );
    }

    Vector3 lookAt(const Vector3& v )
    {
        return this->direction.copy( v ).sub( this->origin ).normalize();
    }

    Ray& recast(const double& t )
    {
        Vector3 v1;
        this->origin.copy( this->at( t, v1 ) );
        return *this;
    }

    Vector3 closestPointToPoint( const Vector3& point, Vector3& optionalTarget ) const
    {

        Vector3& result = optionalTarget;
        result.subVectors( point, this->origin );
        auto directionDistance = result.dot( this->direction );

        if ( directionDistance < 0 ) {
            return result.copy( this->origin );
        }

        return result.copy( this->direction ).multiplyScalar( directionDistance ).add( this->origin );
    }

    Vector3 closestPointToPoint( const Vector3& point) const
    {
        Vector3 result;
        result.subVectors( point, this->origin );
        auto directionDistance = result.dot( this->direction );

        if ( directionDistance < 0 ) {
            return result.copy( this->origin );
        }
        return result.copy( this->direction ).multiplyScalar( directionDistance ).add( this->origin );
    }

    double distanceToPoint(const Vector3& point ) const
    {
        return std::sqrt( this->distanceSqToPoint( point ) );
    }

    double distanceSqToPoint(const Vector3& point ) const
    {
        Vector3 v1 ;

        double directionDistance = v1.subVectors( point, this->origin ).dot( this->direction );

        // point behind the ray

        if ( directionDistance < 0 ) {

            return this->origin.distanceTo(point);

        }

        v1.copy( this->direction ).multiplyScalar( directionDistance ).add( this->origin );

        return v1.distanceTo( point );

    }

    double distanceSqToSegment(const Vector3& v0, const Vector3& v1, Vector3& optionalPointOnRay, Vector3& optionalPointOnSegment )
    {

        // from http://www.geometrictools.com/LibMathematics/Distance/Wm5DistRay3Segment3.cpp
        // It returns the min distance between the ray and the segment
        // defined by v0 and v1
        // It can also set two optional targets :
        // - The closest point on the ray
        // - The closest point on the segment

        //		return function ( v0, v1, optionalPointOnRay, optionalPointOnSegment ) {

        Vector3 segCenter = v0.clone().add( v1 ).multiplyScalar( 0.5 );
        Vector3 segDir = v1.clone().sub( v0 ).normalize();
        double segExtent = v0.distanceTo( v1 ) * 0.5;
        Vector3 diff = this->origin.clone().sub( segCenter );
        double a01 = - this->direction.dot( segDir );
        double b0 = diff.dot( this->direction );
        double b1 = - diff.dot( segDir );
        double c = diff.lengthSq();
        double det = std::abs( 1 - a01 * a01 );
        double s0, s1, sqrDist, extDet;

        if ( det >= 0 ) {

            // The ray and segment are not parallel.

            s0 = a01 * b1 - b0;
            s1 = a01 * b0 - b1;
            extDet = segExtent * det;

            if ( s0 >= 0 ) {

                if ( s1 >= - extDet ) {

                    if ( s1 <= extDet ) {

                        // region 0
                        // Minimum at interior points of ray and segment.

                        auto invDet = 1 / det;
                        s0 *= invDet;
                        s1 *= invDet;
                        sqrDist = s0 * ( s0 + a01 * s1 + 2 * b0 ) + s1 * ( a01 * s0 + s1 + 2 * b1 ) + c;

                    } else {

                        // region 1

                        s1 = segExtent;
                        s0 = std::max( 0.0, - ( a01 * s1 + b0 ) );
                        sqrDist = - s0 * s0 + s1 * ( s1 + 2 * b1 ) + c;

                    }

                } else {

                    // region 5

                    s1 = - segExtent;
                    s0 = std::max( 0.0, - ( a01 * s1 + b0 ) );
                    sqrDist = - s0 * s0 + s1 * ( s1 + 2 * b1 ) + c;

                }

            } else {

                if ( s1 <= - extDet ) {

                    // region 4

                    s0 = std::max( 0.0, - ( - a01 * segExtent + b0 ) );
                    s1 = ( s0 > 0 ) ? - segExtent : std::min( std::max( - segExtent, - b1 ), segExtent );
                    sqrDist = - s0 * s0 + s1 * ( s1 + 2 * b1 ) + c;

                } else if ( s1 <= extDet ) {

                    // region 3

                    s0 = 0;
                    s1 = std::min( std::max( - segExtent, - b1 ), segExtent );
                    sqrDist = s1 * ( s1 + 2 * b1 ) + c;

                } else {

                    // region 2

                    s0 = std::max( 0.0, - ( a01 * segExtent + b0 ) );
                    s1 = ( s0 > 0 ) ? segExtent : std::min( std::max( - segExtent, - b1 ), segExtent );
                    sqrDist = - s0 * s0 + s1 * ( s1 + 2 * b1 ) + c;

                }

            }

        } else {

            // Ray and segment are parallel.

            s1 = ( a01 > 0 ) ? - segExtent : segExtent;
            s0 = std::max( 0.0, - ( a01 * s1 + b0 ) );
            sqrDist = - s0 * s0 + s1 * ( s1 + 2 * b1 ) + c;

        }

        // 这个是参数可以略去
        optionalPointOnRay.copy( this->direction.clone().multiplyScalar( s0 ).add( this->origin ) );

        // 这个是参数可以略去
        optionalPointOnSegment.copy( segDir.clone().multiplyScalar( s1 ).add( segCenter ) );

        return sqrDist;
    }

    bool isIntersectionSphere( const Sphere& sphere ) const
    {
        return this->distanceToPoint( sphere.center ) <= sphere.radius;
    }


    std::tuple<bool, Vector3> intersectSphere(const Sphere& sphere, Vector3& optionalTarget )
    {
        Vector3 v1;

        v1.subVectors( sphere.center, this->origin );
        double tca = v1.dot( this->direction );
        double d2 = v1.dot( v1 ) - tca * tca;
        double radius2 = sphere.radius * sphere.radius;

        // if ( d2 > radius2 ) return null;
        if ( d2 > radius2 ) return std::tuple<bool, Vector3>(false, Vector3());

        double thc = std::sqrt( radius2 - d2 );

        // t0 = first intersect point - entrance on front of sphere
        double t0 = tca - thc;

        // t1 = second intersect point - exit point on back of sphere
        double t1 = tca + thc;

        // test to see if both t0 and t1 are behind the ray - if so, return null
        if ( t0 < 0 && t1 < 0 ) return std::tuple<bool, Vector3>(false, Vector3());

        // test to see if t0 is behind the ray:
        // if it is, the ray is inside the sphere, so return the second exit point scaled by t1,
        // in order to always return an intersect point that is in front of the ray.
        // if ( t0 < 0 ) return this->at( t1, optionalTarget );
        if ( t0 < 0 ) return std::tuple<bool, Vector3>(false,  this->at( t1, optionalTarget ));

        // else t0 is in front of the ray, so return the first collision point scaled by t0
        //        return this->at( t0, optionalTarget );
        if ( t0 < 0 ) return std::tuple<bool, Vector3>(false,  this->at( t0, optionalTarget ));
    }

    bool intersectsSphere(const Sphere& sphere ) const
    {
        return this->distanceToPoint( sphere.center ) <= sphere.radius;
    }

    // (false, double) -> null
    std::tuple<bool, double> distanceToPlane(const Plane& plane ) const
    {
        double denominator = plane.normal.dot( this->direction );

        if ( denominator == 0 ) {
            // line is coplanar, return origin
            if ( plane.distanceToPoint( this->origin ) == 0 ) {
                // return 0;
                return  std::tuple<bool, double>(true, 0);
            } else {
                // TODO
                // Null is preferable to undefined since undefined means.... it is undefined
                qDebug() << "Null is preferable to undefined since undefined means.... it is undefined";
                // return null;
                return  std::tuple<bool, double>(false, -1);
            }
        }

        double t = - ( this->origin.dot( plane.normal ) + plane.constant ) / denominator;

        // Return if the ray never intersects the plane

        // return t >= 0 ? t :  null;
        if(t >= 0) {
            return std::tuple<bool, double>(true, t);
        } else {
            return std::tuple<bool, double>(false, t);
        }

    }

    std::tuple<bool, Vector3> intersectPlane( const Plane& plane, Vector3& optionalTarget ) const
    {

        std::tuple<bool, double> t = this->distanceToPlane( plane );
        //        if ( t ==null ) {	//如果射线与平面永不相交或其他未知定义返回null
        //            return null;	//返回null
        //        }

        if (std::get<0>(t) == false) {
            return std::tuple<bool, Vector3>(false, Vector3());
        }
        //        return this->at( t, optionalTarget );
        return std::tuple<bool, Vector3>(true, this->at( std::get<1>(t), optionalTarget ));
    }


    bool intersectsPlane( const Plane& plane ) const
    {
        // check if the ray lies on the plane first

        double distToPoint = plane.distanceToPoint( this->origin );

        if ( distToPoint == 0 ) {
            return true;
        }

        double denominator = plane.normal.dot( this->direction );

        if ( denominator * distToPoint < 0 ) {
            return true;
        }
        // ray origin is behind the plane (and is pointing behind it)

        return false;
    }


    std::tuple<bool, Vector3> intersectBox( const Box3& box, Vector3& optionalTarget ) const
    {
        double tmin, tmax;

        double tymin, tymax, tzmin, tzmax;

        double  invdirx = 1 / this->direction.x,
                invdiry = 1 / this->direction.y,
                invdirz = 1 / this->direction.z;

        auto origin = this->origin;

        if ( invdirx >= 0 ) {

            tmin = ( box.min.x - origin.x ) * invdirx;
            tmax = ( box.max.x - origin.x ) * invdirx;

        } else {

            tmin = ( box.max.x - origin.x ) * invdirx;
            tmax = ( box.min.x - origin.x ) * invdirx;

        }

        if ( invdiry >= 0 ) {

            tymin = ( box.min.y - origin.y ) * invdiry;
            tymax = ( box.max.y - origin.y ) * invdiry;

        } else {

            tymin = ( box.max.y - origin.y ) * invdiry;
            tymax = ( box.min.y - origin.y ) * invdiry;

        }

        // if ( ( tmin > tymax ) || ( tymin > tmax ) ) return null;
        if ( ( tmin > tymax ) || ( tymin > tmax ) ) return std::tuple<bool, Vector3>(false, Vector3());

        // These lines also handle the case where tmin or tmax is NaN
        // (result of 0 * Infinity). x != x returns true if x is NaN

        if ( tymin > tmin || tmin != tmin ) tmin = tymin;

        if ( tymax < tmax || tmax != tmax ) tmax = tymax;

        if ( invdirz >= 0 ) {

            tzmin = ( box.min.z - origin.z ) * invdirz;
            tzmax = ( box.max.z - origin.z ) * invdirz;

        } else {

            tzmin = ( box.max.z - origin.z ) * invdirz;
            tzmax = ( box.min.z - origin.z ) * invdirz;

        }

        // if ( ( tmin > tzmax ) || ( tzmin > tmax ) ) return null;
        if ( ( tmin > tzmax ) || ( tzmin > tmax ) ) return std::tuple<bool, Vector3>(false, Vector3());

        if ( tzmin > tmin || tmin != tmin ) tmin = tzmin;

        if ( tzmax < tmax || tmax != tmax ) tmax = tzmax;

        //return point closest to the ray (positive side)

        // if ( tmax < 0 ) return null;
        if ( tmax < 0 ) return std::tuple<bool, Vector3>(false, Vector3());

        // return this->at( tmin >= 0 ? tmin : tmax, optionalTarget );
        return std::tuple<bool, Vector3>(false,  this->at( tmin >= 0 ? tmin : tmax, optionalTarget ));
    }

    bool intersectsBox( const Box3& box )
    {
        Vector3 v ;
        // TODO
        return std::get<0>(this->intersectBox( box, v )) != false;
    }

    std::tuple<bool, Vector3> intersectTriangle(const Vector3& a,const Vector3& b,const Vector3& c, bool backfaceCulling, Vector3& optionalTarget ) const
    {

        // from http://www.geometrictools.com/LibMathematics/Intersection/Wm5IntrRay3Triangle3.cpp
        // Compute the offset origin, edges, and normal.
        Vector3 diff ;
        Vector3 edge1;
        Vector3 edge2;
        Vector3 normal;

        edge1.subVectors( b, a );
        edge2.subVectors( c, a );
        normal.crossVectors( edge1, edge2 );

        // Solve Q + t*D = b1*E1 + b2*E2 (Q = kDiff, D = ray direction,
        // E1 = kEdge1, E2 = kEdge2, N = Cross(E1,E2)) by
        //   |Dot(D,N)|*b1 = sign(Dot(D,N))*Dot(D,Cross(Q,E2))
        //   |Dot(D,N)|*b2 = sign(Dot(D,N))*Dot(D,Cross(E1,Q))
        //   |Dot(D,N)|*t = -sign(Dot(D,N))*Dot(Q,N)
        double DdN = this->direction.dot( normal );
        int sign;

        if ( DdN > 0 ) {
            // if ( backfaceCulling ) return null;
            if ( backfaceCulling )  return std::tuple<bool, Vector3>(false, Vector3());
            sign = 1;
        } else if ( DdN < 0 ) {

            sign = - 1;
            DdN = - DdN;

        } else {
            // return null;
            return std::tuple<bool, Vector3>(false, Vector3());
        }

        diff.subVectors( this->origin, a );
        double DdQxE2 = sign * this->direction.dot( edge2.crossVectors( diff, edge2 ) );

        // b1 < 0, no intersection
        if ( DdQxE2 < 0 ) {

            // return null;
            return std::tuple<bool, Vector3>(false, Vector3());
        }

        auto DdE1xQ = sign * this->direction.dot( edge1.cross( diff ) );

        // b2 < 0, no intersection
        if ( DdE1xQ < 0 ) {

            // return null;
            return std::tuple<bool, Vector3>(false, Vector3());
        }

        // b1+b2 > 1, no intersection
        if ( DdQxE2 + DdE1xQ > DdN ) {

            // return null;
            return std::tuple<bool, Vector3>(false, Vector3());

        }

        // Line intersects triangle, check if ray does.
        auto QdN = - sign * diff.dot( normal );

        // t < 0, no intersection
        if ( QdN < 0 ) {

            // return null;
            return std::tuple<bool, Vector3>(false, Vector3());

        }
        // Ray intersects triangle.
        // return this->at( QdN / DdN, optionalTarget );
        return std::tuple<bool, Vector3>(true, this->at( QdN / DdN, optionalTarget ));
    }

    Ray& applyMatrix4( const Matrix4& matrix4 )
    {
        this->direction.add( this->origin ).applyMatrix4( matrix4 );
        this->origin.applyMatrix4( matrix4 );
        this->direction.sub( this->origin );
        this->direction.normalize();
        return *this;
    }

    bool equals( const Ray& ray ) const
    {
        return ray.origin.equals( this->origin ) && ray.direction.equals( this->direction );
    }

    // private:
    Vector3 origin;
    Vector3 direction;
};

} // namespace three

#endif // THREE_RAY_H
