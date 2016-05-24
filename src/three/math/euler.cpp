#include "euler.h"

#include "quaternion.h"
#include "vector3.h"
#include "matrix3.h"
#include "matrix4.h"

namespace three {

void Euler::reorder(Euler::RotationOrders newOrder)
{
    Quaternion q ;
    q.setFromEuler( *this );
    this->setFromQuaternion( q, newOrder );
}

Euler &Euler::setFromRotationMatrix(const Matrix4 &m, Euler::RotationOrders order, bool update)
{
    Q_UNUSED(update)
    auto clamp = Math::clamp<double>;
    // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)
    const auto& te = m.elements;

    auto m11 = te[ 0 ], m12 = te[ 4 ], m13 = te[ 8 ];
    auto m21 = te[ 1 ], m22 = te[ 5 ], m23 = te[ 9 ];
    auto m31 = te[ 2 ], m32 = te[ 6 ], m33 = te[ 10 ];

    if ( order == XYZ ) {
        this->y = std::asin( clamp( m13, - 1, 1 ) );
        if ( std::abs( m13 ) < 0.99999 ) {
            this->x = std::atan2( - m23, m33 );
            this->z = std::atan2( - m12, m11 );
        } else {
            this->x = std::atan2( m32, m22 );
            this->z = 0;
        }
    } else if ( order == YXZ ) {
        this->x = std::asin( - clamp( m23, - 1, 1 ) );
        if ( std::abs( m23 ) < 0.99999 ) {
            this->y = std::atan2( m13, m33 );
            this->z = std::atan2( m21, m22 );
        } else {
            this->y = std::atan2( - m31, m11 );
            this->z = 0;
        }
    } else if ( order == ZXY ) {
        this->x = std::asin( clamp( m32, - 1, 1 ) );
        if ( std::abs( m32 ) < 0.99999 ) {
            this->y = std::atan2( - m31, m33 );
            this->z = std::atan2( - m12, m22 );
        } else {
            this->y = 0;
            this->z = std::atan2( m21, m11 );
        }
    } else if ( order == ZYX ) {
        this->y = std::asin( - clamp( m31, - 1, 1 ) );
        if ( std::abs( m31 ) < 0.99999 ) {
            this->x = std::atan2( m32, m33 );
            this->z = std::atan2( m21, m11 );
        } else {
            this->x = 0;
            this->z = std::atan2( - m12, m22 );
        }
    } else if ( order == YZX ) {
        this->z = std::asin( clamp( m21, - 1, 1 ) );
        if ( std::abs( m21 ) < 0.99999 ) {
            this->x = std::atan2( - m23, m22 );
            this->y = std::atan2( - m31, m11 );
        } else {
            this->x = 0;
            this->y = std::atan2( m13, m33 );
        }
    } else if ( order == XZY ) {
        this->z = std::asin( - clamp( m12, - 1, 1 ) );
        if ( std::abs( m12 ) < 0.99999 ) {
            this->x = std::atan2( m32, m22 );
            this->y = std::atan2( m13, m11 );
        } else {
            this->x = std::atan2( - m23, m33 );
            this->y = 0;
        }
    } else {
        //console.warn( 'THREE.Euler: .setFromRotationMatrix() given unsupported order: ' + order )
        qDebug() <<  "THREE.Euler: .setFromRotationMatrix() given unsupported order: " <<  order ;
    }
    this->order = order;
    return *this;
}

Euler &Euler::setFromQuaternion(const Quaternion &q, const RotationOrders newOrder)
{
    Q_UNUSED(newOrder)
    Matrix4 matrix;
    matrix.makeRotationFromQuaternion( q );
    this->setFromRotationMatrix( matrix, order);
    return *this;
}

Euler &Euler::setFromVector3(const Vector3 &v, Euler::RotationOrders order)
{
    return this->set( v.x, v.y, v.z, order);
}

Vector3 Euler::toVector3()
{
    return  Vector3( this->x, this->y, this->z );
}



} // namespace three

