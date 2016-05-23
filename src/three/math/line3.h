#ifndef THREE_LINE3_H
#define THREE_LINE3_H

#include "math_forword_declar.h"

#include "math.hpp"
#include "vector3.h"

namespace three {

class Line3
{
public:
    Line3()
    { }

    Line3(const Vector3& start, const Vector3& end):
        start(start),
        end(end)
    { }

    Line3& set(const Vector3& start, const Vector3& end )
    {
        this->start.copy( start );
        this->end.copy( end );
        return *this;
    }

    Line3 clone()
    {
        return Line3( this->start, this->end );
    }

    Line3& copy( const Line3& line )
    {
        this->start.copy( line.start );
        this->end.copy( line.end );
        return *this;
    }


    Vector3 center( const Vector3& optionalTarget) const
    {
        Vector3 result = optionalTarget;
        return result.addVectors( this->start, this->end ).multiplyScalar( 0.5 );
    }

    Vector3 delta( const Vector3& optionalTarget ) const
    {
        Vector3 result = optionalTarget;
        return result.subVectors( this->end, this->start );
    }

    double distanceSq() const
    {
        return this->start.distanceToSquared( this->end );
    }

    double distance()
    {
        return this->start.distanceTo( this->end );
    }

    Vector3 at( double t, const Vector3& optionalTarget )
    {
        Vector3 result = optionalTarget;
        return this->delta( result ).multiplyScalar( t ).add( this->start );
    }

    double closestPointToPointParameter(const Vector3& point, bool clampToLine )
    {
        Vector3 startP;
        Vector3 startEnd;

        startP.subVectors( point, this->start );
        startEnd.subVectors( this->end, this->start );

        double startEnd2 = startEnd.dot( startEnd );
        double startEnd_startP = startEnd.dot( startP );

        double t = startEnd_startP / startEnd2;

        if ( clampToLine ) {
            t = Math::clamp<double>( t, 0, 1 );
        }
        return t;
    }

    Vector3 closestPointToPoint( const Vector3& point, bool clampToLine, const Vector3& optionalTarget )
    {
        double t = this->closestPointToPointParameter( point, clampToLine );
        Vector3 result = optionalTarget;
        return this->delta( result ).multiplyScalar( t ).add( this->start );
    }

    Line3& applyMatrix4(const Matrix4& matrix )
    {
        this->start.applyMatrix4( matrix );
        this->end.applyMatrix4( matrix );
        return *this;
    }

    bool equals(const Line3& line ) const
    {
        return line.start.equals( this->start ) && line.end.equals( this->end );
    }

private:
    Vector3 start;
    Vector3 end;

};

} // namespace three

#endif // THREE_LINE3_H
