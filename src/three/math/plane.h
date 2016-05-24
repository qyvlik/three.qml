#ifndef THREE_PLANE_H
#define THREE_PLANE_H

#include "math_forword_declar.h"

#include "vector3.h"
#include "line3.h"
#include "matrix3.h"


namespace three {

class Box3;
class Vector3;
class Line3;
class Matrix3;

class Plane
{
public:
    Plane():
        constant(0)
    { }

    Plane( const Vector3& normal, const double& constant ):
        normal(normal),
        constant(constant)
    { }

    Plane& set( const Vector3& normal, const double& constant )
    {
        this->normal.copy( normal );
        this->constant = constant;
        return *this;
    }

    Plane& setComponents(const double& x, const double& y, const double& z, const double& w )
    {
        this->normal.set( x, y, z );
        this->constant = w;
        return *this;
    }

    Plane& setFromNormalAndCoplanarPoint(const Vector3& normal, const Vector3& point )
    {
        this->normal.copy( normal );
        // must be this->normal, not normal, as this->normal is normalized
        this->constant = - point.dot( this->normal );
        return *this;
    }

    Plane& setFromCoplanarPoints (const Vector3& a,const Vector3& b, const Vector3& c )
    {
        Vector3 v1;
        Vector3 v2;
        auto normal = v1.subVectors( c, b ).cross( v2.subVectors( a, b ) ).normalize();

        // Q: should an error be thrown if normal is zero (e.g. degenerate plane)?

        this->setFromNormalAndCoplanarPoint( normal, a );
        return *this;
    }

    Plane clone() const
    {
        return Plane( this->normal, this->constant );
    }

    Plane& copy(const Plane& plane )
    {
        this->normal.copy( plane.normal );
        this->constant = plane.constant;
        return *this;
    }

    Plane& normalize()
    {
        // Note: will lead to a divide by zero if the plane is invalid.

        auto inverseNormalLength = 1.0 / this->normal.length();
        this->normal.multiplyScalar( inverseNormalLength );
        this->constant *= inverseNormalLength;

        return *this;
    }

    Plane& negate()
    {
        this->constant *= - 1;
        this->normal.negate();
        return *this;
    }

    double distanceToPoint(const Vector3& point ) const
    {
        return this->normal.dot( point ) + this->constant;
    }

    // TODO
    //    double distanceToSphere( const Sphere& sphere )
    //    {
    //        return this->distanceToPoint( sphere.center ) - sphere.radius;
    //    }

    Vector3 projectPoint(const Vector3& point, Vector3& optionalTarget )
    {
        return this->orthoPoint( point, optionalTarget ).sub( point ).negate();
    }

    Vector3 orthoPoint(const Vector3& point, Vector3& optionalTarget )
    {
        auto perpendicularMagnitude = this->distanceToPoint( point );

        Vector3& result = optionalTarget ;
        return result.copy( this->normal ).multiplyScalar( perpendicularMagnitude );
    }

    Vector3 intersectLine( const Line3& line, Vector3& optionalTarget ) const
    {
        Vector3 v1;
        Vector3& result = optionalTarget;

        Vector3 direction = line.delta( v1 );

        double denominator = this->normal.dot( direction );

        if ( denominator == 0 ) {
            // line is coplanar, return origin
            if ( this->distanceToPoint( line.start ) == 0 ) {
                return result.copy( line.start );
            }
            // Unsure if this is the correct method to handle this case.
            return Vector3();
        }

        double t = - ( line.start.dot( this->normal ) + this->constant ) / denominator;

        if ( t < 0 || t > 1 ) {
            return Vector3();
        }
        return result.copy( direction ).multiplyScalar( t ).add( line.start );
    }


    bool intersectsLine(const Line3& line ) const
    {
        // Note: this tests if a line intersects the plane, not whether it (or its end-points) are coplanar with it.
        double startSign = this->distanceToPoint( line.start );
        double endSign = this->distanceToPoint( line.end );
        return ( startSign < 0 && endSign > 0 ) || ( endSign < 0 && startSign > 0 );
    }

    // TODO
    bool intersectsBox(const Box3& box );

    // TODO
    //    intersectsSphere(const Sphere& sphere ) {
    //        return sphere.intersectsPlane( this );
    //    }

    Vector3 coplanarPoint(Vector3& optionalTarget ) const
    {
        auto result = optionalTarget;
        return result.copy( this->normal ).multiplyScalar( - this->constant );
    }

    Plane& applyMatrix4( const Matrix4& matrix)
    {
        Vector3 v1;
        Vector3 v2;
        Matrix3 m1;
        // compute new normal based on theory here:
        // http://www.songho.ca/opengl/gl_normaltransform.html
        Matrix3 normalMatrix = m1.getNormalMatrix( matrix );
        Vector3 newNormal = v1.copy( this->normal ).applyMatrix3( normalMatrix );

        Vector3 newCoplanarPoint = this->coplanarPoint( v2 );
        newCoplanarPoint.applyMatrix4( matrix );

        this->setFromNormalAndCoplanarPoint( newNormal, newCoplanarPoint );
        return *this;
    }

    Plane& applyMatrix4( const Matrix4& matrix, const Matrix3& optionalNormalMatrix )
    {
        Vector3 v1;
        Vector3 v2;
        // compute new normal based on theory here:
        // http://www.songho.ca/opengl/gl_normaltransform.html
        Matrix3 normalMatrix = optionalNormalMatrix ;
        Vector3 newNormal = v1.copy( this->normal ).applyMatrix3( normalMatrix );

        Vector3 newCoplanarPoint = this->coplanarPoint( v2 );
        newCoplanarPoint.applyMatrix4( matrix );

        this->setFromNormalAndCoplanarPoint( newNormal, newCoplanarPoint );
        return *this;
    }


    Plane& translate(const Vector3& offset )
    {
        this->constant = this->constant - offset.dot( this->normal );
        return *this;
    }

    bool equals(const Plane& plane )  const
    {
        return plane.normal.equals( this->normal ) && ( plane.constant == this->constant );
    }

    // private:

    Vector3 normal;
    double constant;
};

} // namespace three

#endif // THREE_PLANE_H
