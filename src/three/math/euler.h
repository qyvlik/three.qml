#ifndef THREE_EULER_H
#define THREE_EULER_H

#include "math.hpp"
// #include <QtMath>
#include <QtDebug>
#include <QVector>

namespace three {

class Matrix4;
class Quaternion;
class Vector3;

class Euler
{
public:

    enum RotationOrders {
        XYZ,
        YZX,
        ZXY,
        XZY,
        YXZ,
        ZYX
    };
    Euler( ):
        x(0.0),
        y(0.0),
        z(0.0),
        order(DefaultOrder)
    {}

    Euler(const double& x, const double& y, const double& z, const RotationOrders& order):
        x(x),
        y(y),
        z(z),
        order(order)
    {}

    Euler& set(const double& x, const double& y, const double& z, const RotationOrders& order)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->order = order;
        return *this;
    }

    Euler clone()
    {
        return Euler (this->x, this->y, this->z, this->order );
    }

    Euler& copy(const Euler& euler )
    {

        this->x = euler.x;
        this->y = euler.y;
        this->z = euler.z;
        this->order = euler.order;
        return *this;
    }

    // TODO
    //   Euler& setFromRotationMatrix( const Matrix3& m,RotationOrders order = DefaultOrder, bool update = false )
    //    {
    //        auto clamp = Math::clamp<double>;
    //        // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)
    //        var te = m.elements;
    //        var m11 = te[ 0 ], m12 = te[ 4 ], m13 = te[ 8 ];
    //        var m21 = te[ 1 ], m22 = te[ 5 ], m23 = te[ 9 ];
    //        var m31 = te[ 2 ], m32 = te[ 6 ], m33 = te[ 10 ];
    //        if ( order == XYZ ) {
    //            this->y = std::asin( clamp( m13, - 1, 1 ) );
    //            if ( std::abs( m13 ) < 0.99999 ) {
    //                this->x = std::atan2( - m23, m33 );
    //                this->z = std::atan2( - m12, m11 );
    //            } else {
    //                this->x = std::atan2( m32, m22 );
    //                this->z = 0;
    //            }
    //        } else if ( order == YXZ ) {
    //            this->x = std::asin( - clamp( m23, - 1, 1 ) );
    //            if ( std::abs( m23 ) < 0.99999 ) {
    //                this->y = std::atan2( m13, m33 );
    //                this->z = std::atan2( m21, m22 );
    //            } else {
    //                this->y = std::atan2( - m31, m11 );
    //                this->z = 0;
    //            }
    //        } else if ( order == ZXY ) {
    //            this->x = std::asin( clamp( m32, - 1, 1 ) );
    //            if ( std::abs( m32 ) < 0.99999 ) {
    //                this->y = std::atan2( - m31, m33 );
    //                this->z = std::atan2( - m12, m22 );
    //            } else {
    //                this->y = 0;
    //                this->z = std::atan2( m21, m11 );
    //            }
    //        } else if ( order == ZYX ) {
    //            this->y = std::asin( - clamp( m31, - 1, 1 ) );
    //            if ( std::abs( m31 ) < 0.99999 ) {
    //                this->x = std::atan2( m32, m33 );
    //                this->z = std::atan2( m21, m11 );
    //            } else {
    //                this->x = 0;
    //                this->z = std::atan2( - m12, m22 );
    //            }
    //        } else if ( order == YZX ) {
    //            this->z = std::asin( clamp( m21, - 1, 1 ) );
    //            if ( std::abs( m21 ) < 0.99999 ) {
    //                this->x = std::atan2( - m23, m22 );
    //                this->y = std::atan2( - m31, m11 );
    //            } else {
    //                this->x = 0;
    //                this->y = std::atan2( m13, m33 );
    //            }
    //        } else if ( order == XZY ) {
    //            this->z = std::asin( - clamp( m12, - 1, 1 ) );
    //            if ( std::abs( m12 ) < 0.99999 ) {
    //                this->x = std::atan2( m32, m22 );
    //                this->y = std::atan2( m13, m11 );
    //            } else {
    //                this->x = std::atan2( - m23, m33 );
    //                this->y = 0;
    //            }
    //        } else {
    //            //console.warn( 'THREE.Euler: .setFromRotationMatrix() given unsupported order: ' + order )
    //            qDebug() <<  'THREE.Euler: .setFromRotationMatrix() given unsupported order: ' <<  order ;
    //        }
    //        this->order = order;
    //        return *this;
    //    }

    //    Euler& setFromQuaternion()
    //    {
    //        Matrix4 matrix;
    //        matrix.makeRotationFromQuaternion( q );
    //        this->setFromRotationMatrix( matrix, order, update );
    //        return *this;
    //    }

    Euler& setFromVector3( const Vector3&v, RotationOrders order );

    // TODO
    // void reorder(RotationOrders newOrder);

    bool equals( const Euler& euler ) const
    {
        return ( euler.x == this->x ) && ( euler.y == this->y ) && ( euler.z == this->z ) && ( euler.order == this->order );
    }

    // TODO
    //    Euler& fromArray( array ) {
    //        this->x = array[ 0 ];
    //        this->y = array[ 1 ];
    //        this->z = array[ 2 ];
    //        if ( array[ 3 ] !== undefined ) this->order = array[ 3 ];
    //        return *this;
    //    }

    // TODO
    //    toArray( array, offset ) {

    //        if ( array === undefined ) array = [];
    //        if ( offset === undefined ) offset = 0;

    //        array[ offset ] = this->x;
    //        array[ offset + 1 ] = this->y;
    //        array[ offset + 2 ] = this->z;
    //        array[ offset + 3 ] = this->order;

    //        return array;
    //    }

    Vector3 toVector3( );

    // private:

    double x;
    double y;
    double z;
    RotationOrders order;

    static const RotationOrders DefaultOrder = XYZ;

};

} // namespace three

#endif // THREE_EULER_H
