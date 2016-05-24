#ifndef THREE_EULER_H
#define THREE_EULER_H

#include "math.hpp"
// #include <QtMath>
#include <QtDebug>
#include <QVector>

namespace three {

class Matrix3;
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

    Euler& setFromRotationMatrix(const Matrix4 &m, RotationOrders order = DefaultOrder, bool update = false );

    Euler& setFromQuaternion(const Quaternion& q, const Euler::RotationOrders newOrder);

    Euler& setFromVector3( const Vector3&v, RotationOrders order );

    // TODO

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
