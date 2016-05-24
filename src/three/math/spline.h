#ifndef THREE_SPLINE_H
#define THREE_SPLINE_H

#include "math_forword_declar.h"

#include "math.hpp"
#include "vector3.h"

namespace three {


// 样条曲线对象
class Spline
{
public:
    Spline()
    { }

    Spline(const Vector3Array& points):
        points(points)
    { }

    void initFromArray(const Vector3Array& a)
    {
        this->points = a;
    }


    Vector3 getPoint(const double& k) const
    {
        Q_ASSERT( k > 0.0 && k < 1.0 );

        double point = (this->points.size()-1) * k;
        double intPoint = std::floor(point);
        double weight = point - intPoint;

        Float32Array c;

        c.push_back(intPoint == 0 ? intPoint : intPoint -1);
        c.push_back(intPoint);
        c.push_back(intPoint > this->points.size() - 2 ? this->points.size() - 1 : intPoint+1);
        c.push_back(intPoint > this->points.size() - 3 ? this->points.size() - 1 : intPoint + 2);

        Vector3 pa = this->points[ c[0] ];
        Vector3 pb = this->points[ c[1] ];
        Vector3 pc = this->points[ c[2] ];
        Vector3 pd = this->points[ c[3] ];

        double w2 = weight * weight;
        double w3 = weight * w2;


        Vector3 v3;

        v3.x = Spline::interpolate( pa.x, pb.x, pc.x, pd.x, weight, w2, w3 );
        v3.y = Spline::interpolate( pa.y, pb.y, pc.y, pd.y, weight, w2, w3 );
        v3.z = Spline::interpolate( pa.z, pb.z, pc.z, pd.z, weight, w2, w3 );

        return v3;

    }

    Vector3Array getControlPointsArray() const
    {
        //        int i;
        //        Vector3    p;
        //        int l = this->points.size();
        //        Vector3Array  coords;
        //        for ( i = 0; i < l; i ++ ) {	//遍历当前样条曲线points属性
        //            p = this->points[ i ];
        //            coords.push_back(p);
        //            //coords[ i ] = [ p.x, p.y, p.z ];	//将points属性直接赋值给coords数组
        //        }
        //        return coords;	//返回当前样条曲线节点坐标构成的数组
        return this->points;
    }

    // chunks: chunkLengths
    // total: chunkLengths[chunkLengths.length-1]
    QVector<int> getLength(int nSubDivisions = 100) {

        int i, index, nSamples;
        Vector3 position;

        int point = 0, intPoint = 0, oldIntPoint = 0;
        Vector3  oldPosition ,  tmpVec ;
        QVector<int> chunkLengths;
        int totalLength = 0;

        // first point has 0 length

        chunkLengths[ 0 ] = 0;

        if ( ! nSubDivisions ) nSubDivisions = 100;

        nSamples = this->points.size() * nSubDivisions;

        oldPosition.copy( this->points[ 0 ] );

        for ( i = 1; i < nSamples; i ++ ) {

            index = i / nSamples;

            position = this->getPoint( index );
            tmpVec.copy( position );

            totalLength += tmpVec.distanceTo( oldPosition );

            oldPosition.copy( position );

            point = ( this->points.size() - 1 ) * index;
            intPoint = std::floor( point );

            if ( intPoint != oldIntPoint ) {

                chunkLengths[ intPoint ] = totalLength;
                oldIntPoint = intPoint;

            }

        }

        // last point ends with total length

        // chunkLengths[ chunkLengths.size() ] = totalLength;
        chunkLengths.push_back(totalLength);

        //return { chunks: chunkLengths, total: totalLength };
        return chunkLengths;
    }


    void reparametrizeByArcLength( const double& samplingCoef ) {

        int i, j, index, indexCurrent, indexNext;
        //        int linearDistance;

        double realDistance,
                sampling;


        Vector3 position;

        Vector3Array newpoints;
        // Vector3 tmpVec ;

        // chunks: chunkLengths
        // total: chunkLengths[chunkLengths.length-1]

        QVector<int> sl = this->getLength();

        QVector<int> chunks = sl;
        chunks.takeLast();
        int total = sl.at(sl.size()-1);

        // newpoints.push_back( tmpVec.copy( this->points[ 0 ] ).clone() );
        newpoints.push_back( this->points[ 0 ]);

        for ( i = 1; i < this->points.size(); i ++ ) {

            //tmpVec.copy( this->points[ i - 1 ] );
            //linearDistance = tmpVec.distanceTo( this->points[ i ] );

            // realDistance = sl.chunks[ i ] - sl.chunks[ i - 1 ];

            // assert()...
            realDistance = chunks[ i ] - chunks[ i - 1 ];

            // sampling = std::ceil( samplingCoef * realDistance / sl.total );

            sampling = std::ceil( samplingCoef * realDistance / total );


            indexCurrent = ( i - 1 ) / ( this->points.size() - 1 );
            indexNext = i / ( this->points.size() - 1 );

            for ( j = 1; j < sampling - 1; j ++ ) {

                index = indexCurrent + j * ( 1 / sampling ) * ( indexNext - indexCurrent );

                position = this->getPoint( index );
                // newpoints.push( tmpVec.copy( position ).clone() );
                newpoints.push_back( position );
            }

            // newpoints.push_back( tmpVec.copy( this->points[ i ] ).clone() );
            newpoints.push_back( this->points[ i ] );
        }

        this->points = newpoints;
    }

    // interpolate方法是传说中的样条插值函数,这里是三次样条插值算法,返回计算位于参数值t的曲线点.
    static double interpolate(const double& p0, const double& p1,const double& p2,const double& p3,const double& t,const double& t2,const double& t3 ) {

        double  v0 = ( p2 - p0 ) * 0.5,
                v1 = ( p3 - p1 ) * 0.5;

        return ( 2 * ( p1 - p2 ) + v0 + v1 ) * t3 + ( - 3 * ( p1 - p2 ) - 2 * v0 - v1 ) * t2 + v0 * t + p1;		//返回计算位于参数值t的曲线点

    }

    // private:
    Vector3Array points;

};

} // namespace three

#endif // THREE_SPLINE_H
