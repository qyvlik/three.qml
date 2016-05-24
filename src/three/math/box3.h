#ifndef THREE_BOX3_H
#define THREE_BOX3_H

#include "math_forword_declar.h"

#include "math.hpp"
#include "vector3.h"
#include "plane.h"
#include "matrix4.h"

namespace three {

class Vector3;
class Sphere;
class Plane;
class Matrix4;

class Box3
{
public:
    Box3()
    { }

    Box3(const Vector3& min, const Vector3& max):
        min(min),
        max(max)
    { }

    Box3& set(const Vector3& min,const Vector3& max ) {

        this->min.copy( min );
        this->max.copy( max );

        return *this;

    }

    // TODO
    //    void setFromArray(const QVector<double> array )
    //    {
    //        this->makeEmpty();

    //        double minX = std::numeric_limits<double>::min();
    //        double minY = std::numeric_limits<double>::min();
    //        double minZ = std::numeric_limits<double>::min();

    //        double maxX = std::numeric_limits<double>::max();
    //        double maxY = std::numeric_limits<double>::max();
    //        double maxZ = std::numeric_limits<double>::max();

    //        for ( auto i = 0, il = array.length(); i < il; i += 3 ) {

    //            double x = array[ i ];
    //            double y = array[ i + 1 ];
    //            double z = array[ i + 2 ];

    //            if ( x < minX ) minX = x;
    //            if ( y < minY ) minY = y;
    //            if ( z < minZ ) minZ = z;

    //            if ( x > maxX ) maxX = x;
    //            if ( y > maxY ) maxY = y;
    //            if ( z > maxZ ) maxZ = z;

    //        }
    //        this->min.set( minX, minY, minZ );
    //        this->max.set( maxX, maxY, maxZ );
    //    }

    Box3& setFromPoints( const Vector3Array& points )
    {
        this->makeEmpty();
        for ( auto i = 0, il = points.length(); i < il; i ++ ) {
            this->expandByPoint( points[ i ] );
        }
        return *this;
    }

    Box3& setFromCenterAndSize(const Vector3& center, const Vector3& size )
    {
        Vector3 v1 ;

        auto halfSize = v1.copy( size ).multiplyScalar( 0.5 );

        this->min.copy( center ).sub( halfSize );
        this->max.copy( center ).add( halfSize );

        return *this;
    }

    // TODO
    //    Box3& setFromObject ( object ) {
    //        // Computes the world-axis-aligned bounding box of an object (including its children),
    //        // accounting for both the object's, and children's, world transforms

    //        Box3 box;

    //        auto scope = this;

    //        this->makeEmpty();

    //        object.updateMatrixWorld( true );

    //        object.traverse( function ( node ) {

    //                             auto geometry = node.geometry;

    //                             if ( geometry !== undefined ) {

    //                                 if ( geometry.boundingBox === null ) {

    //                                     geometry.computeBoundingBox();

    //                                 }

    //                                 box.copy( geometry.boundingBox );
    //                                 box.applyMatrix4( node.matrixWorld );
    //                                 scope.union( box );

    //                             }

    //                         } );

    //        return *this;
    //    }

    Box3 clone() const
    {
        return Box3( this->min, this->max );
    }

    Box3& copy(const Box3& box )
    {
        this->min.copy( box.min );
        this->max.copy( box.max );
        return *this;
    }

    Box3& makeEmpty()
    {
        this->min.x = this->min.y = this->min.z = 0;
        this->max.x = this->max.y = this->max.z = 0;
        return *this;
    }

    bool isEmpty() const
    {
        // this is a more robust check for empty than ( volume <= 0 ) because volume can get positive with two negative axes
        return ( this->max.x < this->min.x ) || ( this->max.y < this->min.y ) || ( this->max.z < this->min.z );
    }

    Vector3 center(Vector3& optionalTarget ) const
    {
        Vector3& result = optionalTarget;
        return result.addVectors( this->min, this->max ).multiplyScalar( 0.5 );
    }

    Vector3 center() const
    {
        Vector3 result;
        return result.addVectors( this->min, this->max ).multiplyScalar( 0.5 );
    }

    Vector3 size(Vector3& optionalTarget ) const
    {
        Vector3& result = optionalTarget ;
        return result.subVectors( this->max, this->min );
    }

    Box3& expandByPoint(const Vector3& point )
    {
        this->min.min( point );
        this->max.max( point );
        return *this;
    }

    Box3& expandByVector( const Vector3& vector )
    {
        this->min.sub( vector );
        this->max.add( vector );
        return *this;
    }

    Box3& expandByScalar(const double& scalar )
    {
        this->min.addScalar( - scalar );
        this->max.addScalar( scalar );
        return *this;
    }

    bool containsPoint(const Vector3& point ) const
    {
        if ( point.x < this->min.x || point.x > this->max.x ||
             point.y < this->min.y || point.y > this->max.y ||
             point.z < this->min.z || point.z > this->max.z )
        {
            return false;
        }
        return true;
    }

    bool containsBox(const Box3& box ) const
    {

        if ( ( this->min.x <= box.min.x ) && ( box.max.x <= this->max.x ) &&
             ( this->min.y <= box.min.y ) && ( box.max.y <= this->max.y ) &&
             ( this->min.z <= box.min.z ) && ( box.max.z <= this->max.z ) )
        {
            return true;
        }
        return false;
    }

    Vector3 getParameter(const Vector3& point, Vector3& optionalTarget )
    {
        // This can potentially have a divide by zero if the box
        // has a size dimension of 0.
        Vector3& result = optionalTarget ;
        return result.set(
                    ( point.x - this->min.x ) / ( this->max.x - this->min.x ),
                    ( point.y - this->min.y ) / ( this->max.y - this->min.y ),
                    ( point.z - this->min.z ) / ( this->max.z - this->min.z )
                    );
    }

    bool intersectsBox( const Box3& box ) const
    {
        // using 6 splitting planes to rule out intersections.

        if ( box.max.x < this->min.x || box.min.x > this->max.x ||
             box.max.y < this->min.y || box.min.y > this->max.y ||
             box.max.z < this->min.z || box.min.z > this->max.z )
        {
            return false;
        }
        return true;
    }

    // TODO
    //    bool intersectsSphere(const Sphere& sphere )
    //    {
    //        Vector3 closestPoint;
    //        // Find the point on the AABB closest to the sphere center.
    //        this->clampPoint( sphere.center, closestPoint );

    //        // If that point is inside the sphere, the AABB and sphere intersect.
    //        return closestPoint.distanceToSquared( sphere.center ) <= ( sphere.radius * sphere.radius );
    //    }

    bool intersectsPlane( const Plane& plane ) const
    {
        // We compute the minimum and maximum dot product values. If those values
        // are on the same side (back or front) of the plane, then there is no intersection.

        double min, max;

        if ( plane.normal.x > 0 ) {

            min = plane.normal.x * this->min.x;
            max = plane.normal.x * this->max.x;

        } else {

            min = plane.normal.x * this->max.x;
            max = plane.normal.x * this->min.x;

        }

        if ( plane.normal.y > 0 ) {

            min += plane.normal.y * this->min.y;
            max += plane.normal.y * this->max.y;

        } else {

            min += plane.normal.y * this->max.y;
            max += plane.normal.y * this->min.y;

        }

        if ( plane.normal.z > 0 ) {

            min += plane.normal.z * this->min.z;
            max += plane.normal.z * this->max.z;

        } else {

            min += plane.normal.z * this->max.z;
            max += plane.normal.z * this->min.z;

        }

        return ( min <= plane.constant && max >= plane.constant );
    }

    Vector3 clampPoint(const Vector3& point, Vector3& optionalTarget ) const
    {
        Vector3& result = optionalTarget;
        return result.copy( point ).clamp( this->min, this->max );
    }

    double distanceToPoint(const Vector3& point ) const
    {
        Vector3 v1 ;
        Vector3& clampedPoint = v1.copy( point ).clamp( this->min, this->max );
        return clampedPoint.sub( point ).length();
    }

    // TODO
    //    Sphere getBoundingSphere(Sphere& optionalTarget ) const
    //    {
    //        Vector3 v1 ;
    //        Sphere&  result = optionalTarget ;

    //        result.center = this->center();
    //        result.radius = this->size( v1 ).length() * 0.5;
    //        return result;
    //    }

    Box3& intersect( const Box3& box )
    {
        this->min.max( box.min );
        this->max.min( box.max );
        return *this;
    }

    Box3& union_ ( const Box3& box )
    {
        this->min.min( box.min );
        this->max.max( box.max );

        return *this;
    }

    Box3& applyMatrix4( const Matrix4& matrix )
    {
        Vector3Array points(8);
        // NOTE: I am using a binary pattern to specify all 2^3 combinations below
        points[ 0 ].set( this->min.x, this->min.y, this->min.z ).applyMatrix4( matrix ); // 000
        points[ 1 ].set( this->min.x, this->min.y, this->max.z ).applyMatrix4( matrix ); // 001
        points[ 2 ].set( this->min.x, this->max.y, this->min.z ).applyMatrix4( matrix ); // 010
        points[ 3 ].set( this->min.x, this->max.y, this->max.z ).applyMatrix4( matrix ); // 011
        points[ 4 ].set( this->max.x, this->min.y, this->min.z ).applyMatrix4( matrix ); // 100
        points[ 5 ].set( this->max.x, this->min.y, this->max.z ).applyMatrix4( matrix ); // 101
        points[ 6 ].set( this->max.x, this->max.y, this->min.z ).applyMatrix4( matrix ); // 110
        points[ 7 ].set( this->max.x, this->max.y, this->max.z ).applyMatrix4( matrix );	// 111

        this->makeEmpty();
        this->setFromPoints( points );

        return *this;
    }

    Box3& translate( const Vector3& offset )
    {
        this->min.add( offset );
        this->max.add( offset );
        return *this;
    }

    bool equals( const Box3& box ) const
    {
        return box.min.equals( this->min ) && box.max.equals( this->max );
    }


    // private:
    Vector3 min;
    Vector3 max;

};

} // namespace three

#endif // THREE_BOX3_H
