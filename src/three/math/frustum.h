#ifndef THREE_FRUSTUM_H
#define THREE_FRUSTUM_H

#include "math_forword_declar.h"

#include "box3.h"
#include "plane.h"
#include "matrix4.h"
#include "sphere.h"

namespace three {

class Vector3;
class Plane;
class Matrix4;
class Sphere;
class Box3;

// 平截头体对象
class Frustum
{
public:
    Frustum()
    {}

    Frustum(const PlaneArray& planes):
        planes(planes)
    {}

    Frustum& set(const Plane& p0, const Plane& p1, const Plane& p2, const Plane& p3, const Plane& p4, const Plane& p5 )
    {
        auto& planes = this->planes;

        planes[ 0 ].copy( p0 );
        planes[ 1 ].copy( p1 );
        planes[ 2 ].copy( p2 );
        planes[ 3 ].copy( p3 );
        planes[ 4 ].copy( p4 );
        planes[ 5 ].copy( p5 );

        return *this;
    }

    Frustum clone() const
    {
        return Frustum( this->planes );
    }

    Frustum& copy( const Frustum& frustum )
    {
        this->planes = frustum.planes;
        return *this;
    }

    Frustum& setFromMatrix(const Matrix4& m )
    {
        auto& planes = this->planes;
        const auto& me = m.elements;
        double me0 = me[ 0 ], me1 = me[ 1 ], me2 = me[ 2 ], me3 = me[ 3 ];
        double me4 = me[ 4 ], me5 = me[ 5 ], me6 = me[ 6 ], me7 = me[ 7 ];
        double me8 = me[ 8 ], me9 = me[ 9 ], me10 = me[ 10 ], me11 = me[ 11 ];
        double me12 = me[ 12 ], me13 = me[ 13 ], me14 = me[ 14 ], me15 = me[ 15 ];

        planes[ 0 ].setComponents( me3 - me0, me7 - me4, me11 - me8, me15 - me12 ).normalize();
        planes[ 1 ].setComponents( me3 + me0, me7 + me4, me11 + me8, me15 + me12 ).normalize();
        planes[ 2 ].setComponents( me3 + me1, me7 + me5, me11 + me9, me15 + me13 ).normalize();
        planes[ 3 ].setComponents( me3 - me1, me7 - me5, me11 - me9, me15 - me13 ).normalize();
        planes[ 4 ].setComponents( me3 - me2, me7 - me6, me11 - me10, me15 - me14 ).normalize();
        planes[ 5 ].setComponents( me3 + me2, me7 + me6, me11 + me10, me15 + me14 ).normalize();

        return *this;
    }

    // TODO
    //    bool intersectsObject( object ) {
    //        auto sphere = new THREE.Sphere();
    //        auto geometry = object.geometry;
    //        if ( geometry.boundingSphere == null ) geometry.computeBoundingSphere();
    //        sphere.copy( geometry.boundingSphere );
    //        sphere.applyMatrix4( object.matrixWorld );
    //        return this->intersectsSphere( sphere );
    //    }

    // TODO
    bool intersectsSphere( const Sphere& sphere ) const
    {
        const auto& planes = this->planes;
        const Vector3&  center = sphere.center;
        double negRadius = - sphere.radius;
        for ( int i = 0; i < 6; i ++ ) {
            auto distance = planes[ i ].distanceToPoint( center );
            if ( distance < negRadius ) {
                return false;
            }
        }
        return true;
    }

    bool intersectsBox( const Box3& box ) const
    {
        Vector3 p1, p2;

        const auto& planes = this->planes;

        for ( auto i = 0; i < 6 ; i ++ ) {

            auto plane = planes[ i ];
            p1.x = plane.normal.x > 0 ? box.min.x : box.max.x;
            p2.x = plane.normal.x > 0 ? box.max.x : box.min.x;
            p1.y = plane.normal.y > 0 ? box.min.y : box.max.y;
            p2.y = plane.normal.y > 0 ? box.max.y : box.min.y;
            p1.z = plane.normal.z > 0 ? box.min.z : box.max.z;
            p2.z = plane.normal.z > 0 ? box.max.z : box.min.z;

            double d1 = plane.distanceToPoint( p1 );
            double d2 = plane.distanceToPoint( p2 );
            // if both outside plane, no intersection
            if ( d1 < 0 && d2 < 0 ) {
                return false;
            }
        }
        return true;
    }


    bool containsPoint(const Vector3& point ) const
    {
        const auto& planes = this->planes;
        // TODO assert planes.length() == 6
        for ( int i = 0; i < 6; i ++ ) {
            if ( planes[ i ].distanceToPoint( point ) < 0 ) {
                return false;
            }
        }
        return true;
    }


    //private:
    PlaneArray planes;
};

} // namespace three

#endif // THREE_FRUSTUM_H
