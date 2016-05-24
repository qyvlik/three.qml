#ifndef THREE_TRIANGLE_H
#define THREE_TRIANGLE_H

#include "math_forword_declar.h"
#include "math.hpp"
#include "vector3.h"
#include "plane.h"

namespace three {

class Plane;

class Triangle
{
public:
    Triangle()
    { }

    Triangle(const Vector3& a, const Vector3& b, const Vector3& c):
        a(a),
        b(b),
        c(c)
    { }

    static Vector3 normal(const Vector3& a,const Vector3& b,const Vector3& c, Vector3& optionalTarget )
    {
        Vector3 v0;
        Vector3& result = optionalTarget;

        result.subVectors( c, b );
        v0.subVectors( a, b );
        result.cross( v0 );

        double resultLengthSq = result.lengthSq();
        if ( resultLengthSq > 0 ) {
            return result.multiplyScalar( 1 / std::sqrt( resultLengthSq ) );
        }
        return result.set( 0, 0, 0 );
    }

    static Vector3 barycoordFromPoint(const Vector3& point,
                                      const Vector3& a, const Vector3& b, const Vector3&c,
                                      Vector3& optionalTarget )
    {
        Vector3 v0;
        Vector3 v1;
        Vector3 v2;

        v0.subVectors( c, a );
        v1.subVectors( b, a );
        v2.subVectors( point, a );

        double dot00 = v0.dot( v0 );
        double dot01 = v0.dot( v1 );
        double dot02 = v0.dot( v2 );
        double dot11 = v1.dot( v1 );
        double dot12 = v1.dot( v2 );

        double denom = ( dot00 * dot11 - dot01 * dot01 );

        Vector3& result = optionalTarget;

        // collinear or singular triangle
        if ( denom == 0 ) {

            // arbitrary location outside of triangle?
            // not sure if this is the best idea, maybe should be returning undefined
            return result.set( - 2, - 1, - 1 );

        }

        double invDenom = 1 / denom;
        double u = ( dot11 * dot02 - dot01 * dot12 ) * invDenom;
        double v = ( dot00 * dot12 - dot01 * dot02 ) * invDenom;

        // barycentric coordinates must always sum to 1
        return result.set( 1 - u - v, v, u );
    }

    static Vector3 barycoordFromPoint(const Vector3& point, const Vector3& a, const Vector3& b, const Vector3&c)
    {
        Vector3 t;
        return Triangle::barycoordFromPoint(point, a, b, c, t);
    }

    static bool containsPoint(const Vector3& point, const Vector3& a,const Vector3&  b,const Vector3&  c )
    {
        Vector3 result = Triangle::barycoordFromPoint( point, a, b, c );
        return     ( result.x >= 0 )
                && ( result.y >= 0 )
                && ( ( result.x + result.y ) <= 1 );
    }

    Triangle& set(const Vector3& a, const Vector3& b, const Vector3& c )
    {
        this->a.copy( a );
        this->b.copy( b );
        this->c.copy( c );
        return *this;
    }

    Triangle& setFromPointsAndIndices( const Vector3Array& points, int i0, int i1, int i2 )
    {
        this->a.copy( points[ i0 ] );
        this->b.copy( points[ i1 ] );
        this->c.copy( points[ i2 ] );
        return *this;
    }

    Triangle clone() const
    {
        return Triangle(this->a, this->b, this->c);
    }

    Triangle& copy( const Triangle& triangle )
    {
        this->a.copy( triangle.a );
        this->b.copy( triangle.b );
        this->c.copy( triangle.c );

        return *this;
    }

    double area() const
    {
        Vector3 v0;
        Vector3 v1;
        v0.subVectors( this->c, this->b );
        v1.subVectors( this->a, this->b );
        return v0.cross( v1 ).length() * 0.5;
    }

    Vector3& midpoint(  Vector3& optionalTarget ) {

        Vector3& result = optionalTarget;
        return result.addVectors( this->a, this->b ).add( this->c ).multiplyScalar( 1 / 3 );

    }

    Vector3 normal(Vector3& optionalTarget ) const
    {
        return Triangle::normal( this->a, this->b, this->c, optionalTarget );
    }

    Vector3 normal() const
    {
        Vector3 t;
        return Triangle::normal( this->a, this->b, this->c, t);
    }

    Plane plane( Plane& optionalTarget) const
    {
        Plane&  result = optionalTarget;
        return result.setFromCoplanarPoints( this->a, this->b, this->c );
    }

    Plane plane() const
    {
        Plane result;
        return result.setFromCoplanarPoints( this->a, this->b, this->c );
    }

    Vector3 barycoordFromPoint(const Vector3& point ) const
    {
        return Triangle::barycoordFromPoint( point, this->a, this->b, this->c );
    }

    Vector3 barycoordFromPoint(const Vector3& point, Vector3& optionalTarget ) const
    {
        return Triangle::barycoordFromPoint( point, this->a, this->b, this->c, optionalTarget );
    }

    bool containsPoint(const Vector3& point ) const
    {
        return containsPoint( point, this->a, this->b, this->c );
    }

    bool equals( const Triangle& triangle)
    {
        return     triangle.a.equals( this->a )
                && triangle.b.equals( this->b )
                && triangle.c.equals( this->c );
    }


    // private:
    Vector3 a;
    Vector3 b;
    Vector3 c;
};

} // namespace three

#endif // THREE_TRIANGLE_H
