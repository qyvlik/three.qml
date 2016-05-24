#ifndef THREE_BOX2_H
#define THREE_BOX2_H

#include "math_forword_declar.h"

#include "vector2.h"

namespace three {

class Vector2;

class Box2
{
public:
    Box2()
    { }

    Box2(const Vector2& min, const Vector2& max):
        min(min),
        max(max)
    { }

    Box2& set(const Vector2& min, const Vector2& max ) {

        this->min.copy( min );
        this->max.copy( max );

        return *this;

    }

    Box2& setFromPoints( const Vector2Array& points )
    {
        this->makeEmpty();
        for ( int i = 0, il = points.length(); i < il; i ++ ) {
            this->expandByPoint( points[ i ] );
        }
        return *this;
    }

    Box2&  setFromCenterAndSize(const Vector2& center, const Vector2& size )
    {
        Vector2 v1;
        auto halfSize = v1.copy( size ).multiplyScalar( 0.5 );
        this->min.copy( center ).sub( halfSize );
        this->max.copy( center ).add( halfSize );
        return *this;
    }

    Box2 clone() const
    {
        return Box2( this->min, this->max );
    }

    Box2& copy(const Box2& box )
    {
        this->min.copy( box.min );
        this->max.copy( box.max );
        return *this;
    }

    Box2& makeEmpty()
    {
        this->min.x = this->min.y = 0;
        this->max.x = this->max.y = 0;
        return *this;
    }

    bool isEmpty() const
    {
        // this is a more robust check for empty than ( volume <= 0 ) because volume can get positive with two negative axes
        return ( this->max.x < this->min.x ) || ( this->max.y < this->min.y );

    }

    Vector2 center( Vector2& optionalTarget ) const
    {

        Vector2 result = optionalTarget ;
        return result.addVectors( this->min, this->max ).multiplyScalar( 0.5 );

    }

    Vector2 size(Vector2& optionalTarget ) const
    {
        Vector2& result = optionalTarget;
        return result.subVectors( this->max, this->min );

    }

    Box2& expandByPoint( const Vector2& point )
    {
        this->min.min( point );
        this->max.max( point );
        return *this;
    }

    Box2& expandByVector( const Vector2& vector )
    {
        this->min.sub( vector );
        this->max.add( vector );
        return *this;
    }

    Box2& expandByScalar( const double& scalar )
    {
        this->min.addScalar( - scalar );
        this->max.addScalar( scalar );
        return *this;
    }

    bool containsPoint( const Vector2& point ) const
    {
        if ( point.x < this->min.x || point.x > this->max.x ||
             point.y < this->min.y || point.y > this->max.y )
        {
            return false;
        }
        return true;
    }

    bool containsBox( const Box2& box ) const
    {
        if ( ( this->min.x <= box.min.x ) && ( box.max.x <= this->max.x ) &&
             ( this->min.y <= box.min.y ) && ( box.max.y <= this->max.y ) )
        {
            return true;
        }
        return false;
    }

    Vector2 getParameter(const Vector2& point, Vector2& optionalTarget ) const
    {

        // This can potentially have a divide by zero if the box
        // has a size dimension of 0.

        Vector2& result = optionalTarget;

        return result.set(
                    ( point.x - this->min.x ) / ( this->max.x - this->min.x ),
                    ( point.y - this->min.y ) / ( this->max.y - this->min.y )
                    );

    }

    bool intersectsBox(const Box2& box ) const
    {
        // using 6 splitting planes to rule out intersections.
        if ( box.max.x < this->min.x || box.min.x > this->max.x ||
             box.max.y < this->min.y || box.min.y > this->max.y )
        {
            return false;
        }
        return true;
    }

    Vector2 clampPoint( const Vector2& point, Vector2& optionalTarget ) const
    {
        Vector2& result = optionalTarget ;
        return result.copy( point ).clamp( this->min, this->max );
    }

    double distanceToPoint ( const Vector2& point ) const
    {
        Vector2 v1;
        Vector2& clampedPoint = v1.copy( point ).clamp( this->min, this->max );
        return clampedPoint.sub( point ).length();
    }

    Box2& intersect( const Box2& box )
    {
        this->min.max( box.min );
        this->max.min( box.max );
        return *this;
    }

    Box2& union_( const Box2& box )
    {
        this->min.min( box.min );
        this->max.max( box.max );
        return *this;
    }

    Box2& translate(const Vector2& offset )
    {
        this->min.add( offset );
        this->max.add( offset );
        return *this;
    }

    bool equals(const Box2&  box ) const
    {
        return box.min.equals( this->min ) && box.max.equals( this->max );
    }


    // private:
    Vector2 min;
    Vector2 max;
};

} // namespace three

#endif // THREE_BOX2_H
