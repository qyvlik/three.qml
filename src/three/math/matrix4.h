#ifndef THREE_MATRIX4_H
#define THREE_MATRIX4_H

#include "math_forword_declar.h"
#include "math.hpp"

#include "vector3.h"
#include "quaternion.h"
#include "euler.h"

namespace three {

class Matrix4
{
public:
    Matrix4():
        elements ( {
                   1, 0, 0, 0,
                   0, 1, 0, 0,
                   0, 0, 1, 0,
                   0, 0, 0, 1 })
    {
    }



    Matrix4& set(const double& n11, const double& n12, const double& n13, const double& n14,
                 const double& n21, const double& n22, const double& n23, const double& n24,
                 const double& n31, const double& n32, const double& n33, const double& n34,
                 const double& n41, const double& n42, const double& n43, const double& n44 )
    {
        auto& te = this->elements;

        te[ 0 ] = n11; te[ 4 ] = n12; te[ 8 ] = n13; te[ 12 ] = n14;
        te[ 1 ] = n21; te[ 5 ] = n22; te[ 9 ] = n23; te[ 13 ] = n24;
        te[ 2 ] = n31; te[ 6 ] = n32; te[ 10 ] = n33; te[ 14 ] = n34;
        te[ 3 ] = n41; te[ 7 ] = n42; te[ 11 ] = n43; te[ 15 ] = n44;

        return *this;
    }

    Matrix4& identity()
    {
        return this->set(  1, 0, 0, 0,
                           0, 1, 0, 0,
                           0, 0, 1, 0,
                           0, 0, 0, 1 );
    }

    Matrix4 clone()
    {

        return  Matrix4().fromArray( this->elements );

    }

    Matrix4& copy(const Matrix4& m )
    {
        this->elements =  m.elements;
        return *this;
    }

    Matrix4& copyPosition(const Matrix4& m )
    {
        auto& te = this->elements;
        const auto& me = m.elements;

        te[ 12 ] = me[ 12 ];
        te[ 13 ] = me[ 13 ];
        te[ 14 ] = me[ 14 ];

        return *this;
    }

    Matrix4& extractBasis(Vector3& xAxis, Vector3& yAxis, Vector3& zAxis )
    {
        auto& te = this->elements;

        xAxis.set( te[ 0 ], te[ 1 ], te[ 2 ] );
        yAxis.set( te[ 4 ], te[ 5 ], te[ 6 ] );
        zAxis.set( te[ 8 ], te[ 9 ], te[ 10 ] );

        return *this;
    }

    Matrix4& makeBasis(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis)
    {
        return this->set(
                    xAxis.x, yAxis.x, zAxis.x, 0,
                    xAxis.y, yAxis.y, zAxis.y, 0,
                    xAxis.z, yAxis.z, zAxis.z, 0,
                    0,       0,       0,       1
                    );
    }

    Matrix4& extractRotation(const Matrix4& m) {

        Vector3 v1;

        auto& te = this->elements;
        const auto& me = m.elements;

        double scaleX = 1 / v1.set( me[ 0 ], me[ 1 ], me[ 2 ] ).length();
        double scaleY = 1 / v1.set( me[ 4 ], me[ 5 ], me[ 6 ] ).length();
        double scaleZ = 1 / v1.set( me[ 8 ], me[ 9 ], me[ 10 ] ).length();

        te[ 0 ] = me[ 0 ] * scaleX;
        te[ 1 ] = me[ 1 ] * scaleX;
        te[ 2 ] = me[ 2 ] * scaleX;

        te[ 4 ] = me[ 4 ] * scaleY;
        te[ 5 ] = me[ 5 ] * scaleY;
        te[ 6 ] = me[ 6 ] * scaleY;

        te[ 8 ] = me[ 8 ] * scaleZ;
        te[ 9 ] = me[ 9 ] * scaleZ;
        te[ 10 ] = me[ 10 ] * scaleZ;

        return *this;
    }

    Matrix4& makeRotationFromEuler(const Euler& euler )
    {

        auto& te = this->elements;

        double x = euler.x, y = euler.y, z = euler.z;
        double a = std::cos( x ), b = std::sin( x );
        double c = std::cos( y ), d = std::sin( y );
        double e = std::cos( z ), f = std::sin( z );

        if ( euler.order == Euler::XYZ ) {

            double ae = a * e, af = a * f, be = b * e, bf = b * f;

            te[ 0 ] = c * e;
            te[ 4 ] = - c * f;
            te[ 8 ] = d;

            te[ 1 ] = af + be * d;
            te[ 5 ] = ae - bf * d;
            te[ 9 ] = - b * c;

            te[ 2 ] = bf - ae * d;
            te[ 6 ] = be + af * d;
            te[ 10 ] = a * c;

        } else if ( euler.order == Euler::YXZ ) {

            double ce = c * e, cf = c * f, de = d * e, df = d * f;

            te[ 0 ] = ce + df * b;
            te[ 4 ] = de * b - cf;
            te[ 8 ] = a * d;

            te[ 1 ] = a * f;
            te[ 5 ] = a * e;
            te[ 9 ] = - b;

            te[ 2 ] = cf * b - de;
            te[ 6 ] = df + ce * b;
            te[ 10 ] = a * c;

        } else if ( euler.order == Euler::ZXY ) {

            double ce = c * e, cf = c * f, de = d * e, df = d * f;

            te[ 0 ] = ce - df * b;
            te[ 4 ] = - a * f;
            te[ 8 ] = de + cf * b;

            te[ 1 ] = cf + de * b;
            te[ 5 ] = a * e;
            te[ 9 ] = df - ce * b;

            te[ 2 ] = - a * d;
            te[ 6 ] = b;
            te[ 10 ] = a * c;

        } else if ( euler.order == Euler::ZYX ) {

            double ae = a * e, af = a * f, be = b * e, bf = b * f;

            te[ 0 ] = c * e;
            te[ 4 ] = be * d - af;
            te[ 8 ] = ae * d + bf;

            te[ 1 ] = c * f;
            te[ 5 ] = bf * d + ae;
            te[ 9 ] = af * d - be;

            te[ 2 ] = - d;
            te[ 6 ] = b * c;
            te[ 10 ] = a * c;

        } else if ( euler.order == Euler::YZX ) {

            double ac = a * c, ad = a * d, bc = b * c, bd = b * d;

            te[ 0 ] = c * e;
            te[ 4 ] = bd - ac * f;
            te[ 8 ] = bc * f + ad;

            te[ 1 ] = f;
            te[ 5 ] = a * e;
            te[ 9 ] = - b * e;

            te[ 2 ] = - d * e;
            te[ 6 ] = ad * f + bc;
            te[ 10 ] = ac - bd * f;

        } else if ( euler.order == Euler::XZY ) {

            double ac = a * c, ad = a * d, bc = b * c, bd = b * d;

            te[ 0 ] = c * e;
            te[ 4 ] = - f;
            te[ 8 ] = d * e;

            te[ 1 ] = ac * f + bd;
            te[ 5 ] = a * e;
            te[ 9 ] = ad * f - bc;

            te[ 2 ] = bc * f - ad;
            te[ 6 ] = b * e;
            te[ 10 ] = bd * f + ac;

        }

        // last column
        te[ 3 ] = 0;
        te[ 7 ] = 0;
        te[ 11 ] = 0;

        // bottom row
        te[ 12 ] = 0;
        te[ 13 ] = 0;
        te[ 14 ] = 0;
        te[ 15 ] = 1;

        return *this;
    }

    Matrix4& makeRotationFromQuaternion( const Quaternion& q )
    {
        auto& te = this->elements;

        double x = q.x, y = q.y, z = q.z, w = q.w;
        double x2 = x + x, y2 = y + y, z2 = z + z;
        double xx = x * x2, xy = x * y2, xz = x * z2;
        double yy = y * y2, yz = y * z2, zz = z * z2;
        double wx = w * x2, wy = w * y2, wz = w * z2;

        te[ 0 ] = 1 - ( yy + zz );
        te[ 4 ] = xy - wz;
        te[ 8 ] = xz + wy;

        te[ 1 ] = xy + wz;
        te[ 5 ] = 1 - ( xx + zz );
        te[ 9 ] = yz - wx;

        te[ 2 ] = xz - wy;
        te[ 6 ] = yz + wx;
        te[ 10 ] = 1 - ( xx + yy );

        // last column
        te[ 3 ] = 0;
        te[ 7 ] = 0;
        te[ 11 ] = 0;

        // bottom row
        te[ 12 ] = 0;
        te[ 13 ] = 0;
        te[ 14 ] = 0;
        te[ 15 ] = 1;

        return *this;
    }

    Matrix4& lookAt(const Vector3& eye, const Vector3& target, const Vector3& up )
    {
        Vector3 x, y, z;

        auto& te = this->elements;

        z.subVectors( eye, target ).normalize();

        if ( z.lengthSq() == 0 ) {
            z.z = 1;
        }

        x.crossVectors( up, z ).normalize();

        if ( x.lengthSq() == 0 ) {
            z.x += 0.0001;
            x.crossVectors( up, z ).normalize();
        }

        y.crossVectors( z, x );

        te[ 0 ] = x.x; te[ 4 ] = y.x; te[ 8 ] = z.x;
        te[ 1 ] = x.y; te[ 5 ] = y.y; te[ 9 ] = z.y;
        te[ 2 ] = x.z; te[ 6 ] = y.z; te[ 10 ] = z.z;

        return *this;
    }

    Matrix4&  multiply( const Matrix4& m)
    {
        return this->multiplyMatrices( *this, m );
    }

    Matrix4&  multiply( const Matrix4& m, const Matrix4& n )
    {
        return this->multiplyMatrices( m, n );
    }

    Matrix4&  multiplyMatrices( const Matrix4& a, const Matrix4& b )
    {
        const auto& ae = a.elements;
        const auto& be = b.elements;
        auto& te = this->elements;

        double a11 = ae[ 0 ], a12 = ae[ 4 ], a13 = ae[ 8 ], a14 = ae[ 12 ];
        double a21 = ae[ 1 ], a22 = ae[ 5 ], a23 = ae[ 9 ], a24 = ae[ 13 ];
        double a31 = ae[ 2 ], a32 = ae[ 6 ], a33 = ae[ 10 ], a34 = ae[ 14 ];
        double a41 = ae[ 3 ], a42 = ae[ 7 ], a43 = ae[ 11 ], a44 = ae[ 15 ];

        double b11 = be[ 0 ], b12 = be[ 4 ], b13 = be[ 8 ], b14 = be[ 12 ];
        double b21 = be[ 1 ], b22 = be[ 5 ], b23 = be[ 9 ], b24 = be[ 13 ];
        double b31 = be[ 2 ], b32 = be[ 6 ], b33 = be[ 10 ], b34 = be[ 14 ];
        double b41 = be[ 3 ], b42 = be[ 7 ], b43 = be[ 11 ], b44 = be[ 15 ];

        te[ 0 ] = a11 * b11 + a12 * b21 + a13 * b31 + a14 * b41;
        te[ 4 ] = a11 * b12 + a12 * b22 + a13 * b32 + a14 * b42;
        te[ 8 ] = a11 * b13 + a12 * b23 + a13 * b33 + a14 * b43;
        te[ 12 ] = a11 * b14 + a12 * b24 + a13 * b34 + a14 * b44;

        te[ 1 ] = a21 * b11 + a22 * b21 + a23 * b31 + a24 * b41;
        te[ 5 ] = a21 * b12 + a22 * b22 + a23 * b32 + a24 * b42;
        te[ 9 ] = a21 * b13 + a22 * b23 + a23 * b33 + a24 * b43;
        te[ 13 ] = a21 * b14 + a22 * b24 + a23 * b34 + a24 * b44;

        te[ 2 ] = a31 * b11 + a32 * b21 + a33 * b31 + a34 * b41;
        te[ 6 ] = a31 * b12 + a32 * b22 + a33 * b32 + a34 * b42;
        te[ 10 ] = a31 * b13 + a32 * b23 + a33 * b33 + a34 * b43;
        te[ 14 ] = a31 * b14 + a32 * b24 + a33 * b34 + a34 * b44;

        te[ 3 ] = a41 * b11 + a42 * b21 + a43 * b31 + a44 * b41;
        te[ 7 ] = a41 * b12 + a42 * b22 + a43 * b32 + a44 * b42;
        te[ 11 ] = a41 * b13 + a42 * b23 + a43 * b33 + a44 * b43;
        te[ 15 ] = a41 * b14 + a42 * b24 + a43 * b34 + a44 * b44;

        return *this;
    }

    Matrix4&  multiplyToArray(const Matrix4& a, const Matrix4& b, Float32Array& r )
    {
        auto& te = this->elements;

        this->multiplyMatrices( a, b );

        r[ 0 ] = te[ 0 ]; r[ 1 ] = te[ 1 ]; r[ 2 ] = te[ 2 ]; r[ 3 ] = te[ 3 ];
        r[ 4 ] = te[ 4 ]; r[ 5 ] = te[ 5 ]; r[ 6 ] = te[ 6 ]; r[ 7 ] = te[ 7 ];
        r[ 8 ]  = te[ 8 ]; r[ 9 ]  = te[ 9 ]; r[ 10 ] = te[ 10 ]; r[ 11 ] = te[ 11 ];
        r[ 12 ] = te[ 12 ]; r[ 13 ] = te[ 13 ]; r[ 14 ] = te[ 14 ]; r[ 15 ] = te[ 15 ];

        return *this;
    }

    Matrix4& multiplyScalar( const double& s )
    {
        auto& te = this->elements;

        te[ 0 ] *= s; te[ 4 ] *= s; te[ 8 ] *= s; te[ 12 ] *= s;
        te[ 1 ] *= s; te[ 5 ] *= s; te[ 9 ] *= s; te[ 13 ] *= s;
        te[ 2 ] *= s; te[ 6 ] *= s; te[ 10 ] *= s; te[ 14 ] *= s;
        te[ 3 ] *= s; te[ 7 ] *= s; te[ 11 ] *= s; te[ 15 ] *= s;

        return *this;
    }

    // TODO Vector3Array or Float32Array
    Float32Array& applyToVector3Array( Float32Array & array, int offset = 0, int length = 0)
    {
        Vector3 v1;
        if ( length == 0 )
            length = array.size();
        for ( int i = 0, j = offset; i < length; i += 3, j += 3 ) {
            v1.fromArray( array, j );
            v1.applyMatrix4( *this );
            v1.toArray( array, j );
        }

        return array;
    }

    // TODO
    //    applyToBuffer( buffer, int offset = 0, int length = 0 ) {

    //        Vector3 v1;
    //        if ( offset == undefined ) offset = 0;
    //        if ( length == undefined ) length = buffer.length / buffer.itemSize;

    //        for ( var i = 0, j = offset; i < length; i ++, j ++ ) {

    //            v1.x = buffer.getX( j );
    //            v1.y = buffer.getY( j );
    //            v1.z = buffer.getZ( j );

    //            v1.applyMatrix4( this );

    //            buffer.setXYZ( v1.x, v1.y, v1.z );

    //        }

    //        return buffer;
    //    }

    double determinant()
    {

        Float32Array& te = this->elements;

        double n11 = te[ 0 ], n12 = te[ 4 ], n13 = te[ 8 ], n14 = te[ 12 ];
        double n21 = te[ 1 ], n22 = te[ 5 ], n23 = te[ 9 ], n24 = te[ 13 ];
        double n31 = te[ 2 ], n32 = te[ 6 ], n33 = te[ 10 ], n34 = te[ 14 ];
        double n41 = te[ 3 ], n42 = te[ 7 ], n43 = te[ 11 ], n44 = te[ 15 ];

        //TODO: make this more efficient
        //( based on http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/fourD/index.htm )

        return (
                    n41 * (
                        + n14 * n23 * n32
                        - n13 * n24 * n32
                        - n14 * n22 * n33
                        + n12 * n24 * n33
                        + n13 * n22 * n34
                        - n12 * n23 * n34
                        ) +
                    n42 * (
                        + n11 * n23 * n34
                        - n11 * n24 * n33
                        + n14 * n21 * n33
                        - n13 * n21 * n34
                        + n13 * n24 * n31
                        - n14 * n23 * n31
                        ) +
                    n43 * (
                        + n11 * n24 * n32
                        - n11 * n22 * n34
                        - n14 * n21 * n32
                        + n12 * n21 * n34
                        + n14 * n22 * n31
                        - n12 * n24 * n31
                        ) +
                    n44 * (
                        - n13 * n22 * n31
                        - n11 * n23 * n32
                        + n11 * n22 * n33
                        + n13 * n21 * n32
                        - n12 * n21 * n33
                        + n12 * n23 * n31
                        )

                    );

    }

    Matrix4&  transpose()
    {
        auto& te = this->elements;
        double tmp;

        tmp = te[ 1 ]; te[ 1 ] = te[ 4 ]; te[ 4 ] = tmp;
        tmp = te[ 2 ]; te[ 2 ] = te[ 8 ]; te[ 8 ] = tmp;
        tmp = te[ 6 ]; te[ 6 ] = te[ 9 ]; te[ 9 ] = tmp;

        tmp = te[ 3 ]; te[ 3 ] = te[ 12 ]; te[ 12 ] = tmp;
        tmp = te[ 7 ]; te[ 7 ] = te[ 13 ]; te[ 13 ] = tmp;
        tmp = te[ 11 ]; te[ 11 ] = te[ 14 ]; te[ 14 ] = tmp;

        return *this;
    }

    Float32Array& flattenToArrayOffset( Float32Array& array, int offset )
    {
        auto& te = this->elements;

        array[ offset ] = te[ 0 ];
        array[ offset + 1 ] = te[ 1 ];
        array[ offset + 2 ] = te[ 2 ];
        array[ offset + 3 ] = te[ 3 ];

        array[ offset + 4 ] = te[ 4 ];
        array[ offset + 5 ] = te[ 5 ];
        array[ offset + 6 ] = te[ 6 ];
        array[ offset + 7 ] = te[ 7 ];

        array[ offset + 8 ]  = te[ 8 ];
        array[ offset + 9 ]  = te[ 9 ];
        array[ offset + 10 ] = te[ 10 ];
        array[ offset + 11 ] = te[ 11 ];

        array[ offset + 12 ] = te[ 12 ];
        array[ offset + 13 ] = te[ 13 ];
        array[ offset + 14 ] = te[ 14 ];
        array[ offset + 15 ] = te[ 15 ];

        return array;
    }

    Vector3 getPosition()
    {
        auto& te = this->elements;
        return Vector3( te[ 12 ], te[ 13 ], te[ 14 ] );
    }

    Matrix4&  setPosition( const Vector3& v)
    {
        auto& te = this->elements;
        te[ 12 ] = v.x;
        te[ 13 ] = v.y;
        te[ 14 ] = v.z;
        return *this;
    }

    Matrix4& getInverse( const Matrix4& m, double throwOnInvertible )
    {
        // based on http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/fourD/index.htm
        auto& te = this->elements;
        const auto& me = m.elements;

        double n11 = me[ 0 ], n12 = me[ 4 ], n13 = me[ 8 ], n14 = me[ 12 ];
        double n21 = me[ 1 ], n22 = me[ 5 ], n23 = me[ 9 ], n24 = me[ 13 ];
        double n31 = me[ 2 ], n32 = me[ 6 ], n33 = me[ 10 ], n34 = me[ 14 ];
        double n41 = me[ 3 ], n42 = me[ 7 ], n43 = me[ 11 ], n44 = me[ 15 ];

        te[ 0 ] = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
        te[ 4 ] = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
        te[ 8 ] = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
        te[ 12 ] = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;
        te[ 1 ] = n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44;
        te[ 5 ] = n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44;
        te[ 9 ] = n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44;
        te[ 13 ] = n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34;
        te[ 2 ] = n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44;
        te[ 6 ] = n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44;
        te[ 10 ] = n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44;
        te[ 14 ] = n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34;
        te[ 3 ] = n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43;
        te[ 7 ] = n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43;
        te[ 11 ] = n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43;
        te[ 15 ] = n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33;

        double det = n11 * te[ 0 ] + n21 * te[ 4 ] + n31 * te[ 8 ] + n41 * te[ 12 ];

        if ( det == 0 ) {
            Q_ASSERT_X(!(throwOnInvertible || false), "getInverse", "THREE.Matrix4.getInverse(): can't invert matrix, determinant is 0");
            this->identity();
            return *this;
        }

        this->multiplyScalar( 1 / det );

        return *this;

    }

    Matrix4& scale( const Vector3& v )
    {
        auto& te = this->elements;
        double x = v.x, y = v.y, z = v.z;

        te[ 0 ] *= x; te[ 4 ] *= y; te[ 8 ] *= z;
        te[ 1 ] *= x; te[ 5 ] *= y; te[ 9 ] *= z;
        te[ 2 ] *= x; te[ 6 ] *= y; te[ 10 ] *= z;
        te[ 3 ] *= x; te[ 7 ] *= y; te[ 11 ] *= z;

        return *this;
    }

    double getMaxScaleOnAxis()
    {
        auto& te = this->elements;

        double scaleXSq = te[ 0 ] * te[ 0 ] + te[ 1 ] * te[ 1 ] + te[ 2 ] * te[ 2 ];
        double scaleYSq = te[ 4 ] * te[ 4 ] + te[ 5 ] * te[ 5 ] + te[ 6 ] * te[ 6 ];
        double scaleZSq = te[ 8 ] * te[ 8 ] + te[ 9 ] * te[ 9 ] + te[ 10 ] * te[ 10 ];

        return std::sqrt( std::max(std::max( scaleXSq, scaleYSq ), scaleZSq) );
    }

    Matrix4& makeTranslation( const double& x, const double& y, const double& z )
    {
        return this->set(
                    1, 0, 0, x,
                    0, 1, 0, y,
                    0, 0, 1, z,
                    0, 0, 0, 1
                    );

    }

    Matrix4& makeRotationX(const double& theta )
    {
        double c = std::cos( theta ), s = std::sin( theta );

        return this->set(
                    1, 0,  0, 0,
                    0, c, - s, 0,
                    0, s,  c, 0,
                    0, 0,  0, 1
                    );
    }

    Matrix4& makeRotationY(const double& theta )
    {

        double c = std::cos( theta ), s = std::sin( theta );

        return this->set(
                    c, 0, s, 0,
                    0, 1, 0, 0,
                    - s, 0, c, 0,
                    0, 0, 0, 1
                    );
    }

    Matrix4& makeRotationZ(const double& theta ) {

        double c = std::cos( theta ), s = std::sin( theta );
        return this->set(
                    c, - s, 0, 0,
                    s,  c, 0, 0,
                    0,  0, 1, 0,
                    0,  0, 0, 1
                    );

    }

    Matrix4& makeRotationAxis(const Vector3& axis, const double& angle )
    {
        // Based on http://www.gamedev.net/reference/articles/article1199.asp

        double c = std::cos( angle );
        double s = std::sin( angle );
        double t = 1 - c;
        double x = axis.x, y = axis.y, z = axis.z;
        double tx = t * x, ty = t * y;

        return this->set(
                    tx * x + c, tx * y - s * z, tx * z + s * y, 0,
                    tx * y + s * z, ty * y + c, ty * z - s * x, 0,
                    tx * z - s * y, ty * z + s * x, t * z * z + c, 0,
                    0, 0, 0, 1
                    );
    }

    Matrix4& makeScale(const double& x, const double& y, const double& z )
    {
        return this->set(
                    x, 0, 0, 0,
                    0, y, 0, 0,
                    0, 0, z, 0,
                    0, 0, 0, 1
                    );

    }

    Matrix4& compose( const Vector3& position, const Quaternion& quaternion, const Vector3& scale  )
    {
        this->makeRotationFromQuaternion( quaternion );
        this->scale( scale );
        this->setPosition( position );

        return *this;
    }

    Matrix4& decompose( Vector3& position, Quaternion& quaternion,  Vector3& scale )
    {
        // static
        Vector3 vector;
        Matrix4 matrix;

        auto& te = this->elements;

        double sx = vector.set( te[ 0 ], te[ 1 ], te[ 2 ] ).length();
        double sy = vector.set( te[ 4 ], te[ 5 ], te[ 6 ] ).length();
        double sz = vector.set( te[ 8 ], te[ 9 ], te[ 10 ] ).length();

        // if determine is negative, we need to invert one scale
        double det = this->determinant();
        if ( det < 0 ) {
            sx = - sx;
        }

        position.x = te[ 12 ];
        position.y = te[ 13 ];
        position.z = te[ 14 ];

        // scale the rotation part

        matrix.elements = this->elements ; // at this point matrix is incomplete so we can't use .copy()

        double invSX = 1 / sx;
        double invSY = 1 / sy;
        double invSZ = 1 / sz;

        matrix.elements[ 0 ] *= invSX;
        matrix.elements[ 1 ] *= invSX;
        matrix.elements[ 2 ] *= invSX;

        matrix.elements[ 4 ] *= invSY;
        matrix.elements[ 5 ] *= invSY;
        matrix.elements[ 6 ] *= invSY;

        matrix.elements[ 8 ] *= invSZ;
        matrix.elements[ 9 ] *= invSZ;
        matrix.elements[ 10 ] *= invSZ;

        quaternion.setFromRotationMatrix( matrix );

        scale.x = sx;
        scale.y = sy;
        scale.z = sz;

        return *this;
    }

    Matrix4& makeFrustum(const double& left, const double&right,
                         const double&bottom, const double&top,
                         const double&near, const double&far )
    {
        auto& te = this->elements;
        double x = 2 * near / ( right - left );
        double y = 2 * near / ( top - bottom );

        double a = ( right + left ) / ( right - left );
        double b = ( top + bottom ) / ( top - bottom );
        double c = - ( far + near ) / ( far - near );
        double d = - 2 * far * near / ( far - near );

        te[ 0 ] = x;	te[ 4 ] = 0;	te[ 8 ] = a;	te[ 12 ] = 0;
        te[ 1 ] = 0;	te[ 5 ] = y;	te[ 9 ] = b;	te[ 13 ] = 0;
        te[ 2 ] = 0;	te[ 6 ] = 0;	te[ 10 ] = c;	te[ 14 ] = d;
        te[ 3 ] = 0;	te[ 7 ] = 0;	te[ 11 ] = - 1;	te[ 15 ] = 0;

        return *this;
    }

    Matrix4& makePerspective( const double& fov, const double& aspect, const double& near, const double& far )
    {

        double ymax = near * std::tan( Math::degToRad( fov * 0.5 ) );
        double ymin = - ymax;
        double xmin = ymin * aspect;
        double xmax = ymax * aspect;

        return this->makeFrustum( xmin, xmax, ymin, ymax, near, far );
    }

    Matrix4& makeOrthographic(const double& left, const double& right,
                              const double& top, const double& bottom,
                              const double& near, const double& far )
    {

        auto& te = this->elements;
        double w = right - left;
        double h = top - bottom;
        double p = far - near;

        double x = ( right + left ) / w;
        double y = ( top + bottom ) / h;
        double z = ( far + near ) / p;

        te[ 0 ] = 2 / w;	te[ 4 ] = 0;	te[ 8 ] = 0;	te[ 12 ] = - x;
        te[ 1 ] = 0;	te[ 5 ] = 2 / h;	te[ 9 ] = 0;	te[ 13 ] = - y;
        te[ 2 ] = 0;	te[ 6 ] = 0;	te[ 10 ] = - 2 / p;	te[ 14 ] = - z;
        te[ 3 ] = 0;	te[ 7 ] = 0;	te[ 11 ] = 0;	te[ 15 ] = 1;

        return *this;
    }

    bool equals( const Matrix4& matrix )
    {
        auto& te = this->elements;
        const auto& me = matrix.elements;
        for ( int i = 0; i < 16; i ++ ) {
            if ( te[ i ] != me[ i ] )
                return false;
        }
        return true;
    }

    Matrix4& fromArray(const Float32Array& array )
    {
        Q_ASSERT(array.size() >= 16);
        this->elements = array ;
        return *this;
    }

    Float32Array toArray() {
        return this->elements;
    }

    // private:
    Float32Array elements;
};

} // namespace three

#endif // THREE_MATRIX4_H
