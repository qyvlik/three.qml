#ifndef THREE_MATRIX3_H
#define THREE_MATRIX3_H

#include "math_forword_declar.h"

#include "matrix4.h"

namespace three {

class Matrix3
{
public:
    Matrix3():
        elements(Float32Array(9))
    {}

    Matrix3( const double& n11, const double& n12, const double& n13,
             const double& n21, const double& n22, const double& n23,
             const double& n31, const double& n32, const double& n33 ) :
        elements({
                 n11 ,n12 , n13,
                 n21 ,n22 , n23,
                 n31 ,n32 , n33 })
    {
    }

    Matrix3& set( const double& n11, const double& n12, const double& n13,
                  const double& n21, const double& n22, const double& n23,
                  const double& n31, const double& n32, const double& n33 )
    {
        auto& te = this->elements;

        te[ 0 ] = n11; te[ 3 ] = n12; te[ 6 ] = n13;
        te[ 1 ] = n21; te[ 4 ] = n22; te[ 7 ] = n23;
        te[ 2 ] = n31; te[ 5 ] = n32; te[ 8 ] = n33;

        return *this;
    }

    Matrix3& identity()
    {
        return this->set(
                    1, 0, 0,
                    0, 1, 0,
                    0, 0, 1
                    );
    }

    Matrix3 clone() const
    {
        return Matrix3().fromArray( this->elements );
    }

    Matrix3& copy(const Matrix3& m )
    {

        const auto& me = m.elements;

        return this->set( me[ 0 ], me[ 3 ], me[ 6 ],
                me[ 1 ], me[ 4 ], me[ 7 ],
                me[ 2 ], me[ 5 ], me[ 8 ]
                );

    }

    // TODO
//        Vector3Array applyToVector3Array(Vector3Array& array, int offset = 0,  const int length = 0) {
//            Vector3 v1;
//            if ( length == 0 )
//                length = array.size();
//            for ( int i = 0, j = offset; i < length; i += 3, j += 3 ) {
//                v1.fromArray( array, j );
//                v1.applyMatrix3( *this );
//                v1.toArray( array, j );
//            }
//            return array;
//        }

    // TODO
    //    applyToBuffer( buffer, int offset = 0, int length = 0) {

    //        Vector3 v1;
    //        if ( length == 0 )
    //            length = buffer.length / buffer.itemSize;

    //        for ( var i = 0, j = offset; i < length; i ++, j ++ ) {

    //            v1.x = buffer.getX( j );
    //            v1.y = buffer.getY( j );
    //            v1.z = buffer.getZ( j );

    //            v1.applyMatrix3( this );

    //            buffer.setXYZ( v1.x, v1.y, v1.z );
    //        }
    //        return buffer;
    //    }

    Matrix3& multiplyScalar(const double& s )
    {
        auto& te = this->elements;

        te[ 0 ] *= s; te[ 3 ] *= s; te[ 6 ] *= s;
        te[ 1 ] *= s; te[ 4 ] *= s; te[ 7 ] *= s;
        te[ 2 ] *= s; te[ 5 ] *= s; te[ 8 ] *= s;

        return *this;
    }

    double determinant() const
    {
        const auto& te = this->elements;

        double  a = te[ 0 ], b = te[ 1 ], c = te[ 2 ],
                d = te[ 3 ], e = te[ 4 ], f = te[ 5 ],
                g = te[ 6 ], h = te[ 7 ], i = te[ 8 ];

        return a * e * i - a * f * h - b * d * i + b * f * g + c * d * h - c * e * g;
    }

    Matrix3& getInverse(const Matrix4& matrix, const double& throwOnDegenerate = 0 )
    {
        // input: THREE.Matrix4
        // ( based on http://code.google.com/p/webgl-mjs/ )

        const auto& me = matrix.elements;
        auto& te = this->elements;

        te[ 0 ] =   me[ 10 ] * me[ 5 ] - me[ 6 ] * me[ 9 ];
        te[ 1 ] = - me[ 10 ] * me[ 1 ] + me[ 2 ] * me[ 9 ];
        te[ 2 ] =   me[ 6 ] * me[ 1 ] - me[ 2 ] * me[ 5 ];
        te[ 3 ] = - me[ 10 ] * me[ 4 ] + me[ 6 ] * me[ 8 ];
        te[ 4 ] =   me[ 10 ] * me[ 0 ] - me[ 2 ] * me[ 8 ];
        te[ 5 ] = - me[ 6 ] * me[ 0 ] + me[ 2 ] * me[ 4 ];
        te[ 6 ] =   me[ 9 ] * me[ 4 ] - me[ 5 ] * me[ 8 ];
        te[ 7 ] = - me[ 9 ] * me[ 0 ] + me[ 1 ] * me[ 8 ];
        te[ 8 ] =   me[ 5 ] * me[ 0 ] - me[ 1 ] * me[ 4 ];

        double det = me[ 0 ] * te[ 0 ] + me[ 1 ] * te[ 3 ] + me[ 2 ] * te[ 6 ];

        // no inverse
        if ( det == 0 ) { //提示用户该矩阵没有逆矩阵
            Q_ASSERT_X(!(throwOnDegenerate || false), "getInverse", "THREE.Matrix3.getInverse(): can't invert matrix, determinant is 0");
            this->identity();
            return *this;
        }

        this->multiplyScalar( 1.0 / det );

        return *this;
    }

    Matrix3& transpose()
    {
        double tmp;
        auto& m = this->elements;

        tmp = m[ 1 ]; m[ 1 ] = m[ 3 ]; m[ 3 ] = tmp;
        tmp = m[ 2 ]; m[ 2 ] = m[ 6 ]; m[ 6 ] = tmp;
        tmp = m[ 5 ]; m[ 5 ] = m[ 7 ]; m[ 7 ] = tmp;

        return *this;
    }

    // TODO
    //    flattenToArrayOffset( array, offset ) {

    //        auto& te = this->elements;

    //        array[ offset ] = te[ 0 ];
    //        array[ offset + 1 ] = te[ 1 ];
    //        array[ offset + 2 ] = te[ 2 ];

    //        array[ offset + 3 ] = te[ 3 ];
    //        array[ offset + 4 ] = te[ 4 ];
    //        array[ offset + 5 ] = te[ 5 ];

    //        array[ offset + 6 ] = te[ 6 ];
    //        array[ offset + 7 ] = te[ 7 ];
    //        array[ offset + 8 ]  = te[ 8 ];

    //        return array;

    //    }

    Matrix3& getNormalMatrix( const Matrix4& m )
    {
        // input: THREE.Matrix4

        this->getInverse( m ).transpose();

        return *this;
    }

    Matrix3& transposeIntoArray(Float32Array& r )
    {
        const auto& m = this->elements;

        r[ 0 ] = m[ 0 ];
        r[ 1 ] = m[ 3 ];
        r[ 2 ] = m[ 6 ];
        r[ 3 ] = m[ 1 ];
        r[ 4 ] = m[ 4 ];
        r[ 5 ] = m[ 7 ];
        r[ 6 ] = m[ 2 ];
        r[ 7 ] = m[ 5 ];
        r[ 8 ] = m[ 8 ];

        return *this;
    }

    Matrix3& fromArray( const Float32Array& array )
    {
        this->elements =  array;
        return *this;
    }

    Float32Array toArray() const
    {
        return this->elements;
    }

    //private:
    Float32Array elements;
};

} // namespace three

#endif // THREE_MATRIX3_H
