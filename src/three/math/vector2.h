#ifndef THREE_VECTOR2_H
#define THREE_VECTOR2_H

#include <QtMath>
#include <QtDebug>
#include <QVector>

namespace three {

typedef QVector<double> NumberArray;

class Vector2
{
public:
    Vector2() :
        x(0),
        y(0)
    { }

    Vector2(const double& x, const double& y):
        x(x),
        y(y)
    { }

    double width() const {
        return this->x;
    }

    void setWidth(const double& value) {
        this->x = value;
    }

    double height() const {
        return this->y;
    }

    void setHight(const double& value) {
        this->y = value;
    }

    Vector2& set(const double& x, const double& y)
    {
        this->x = x;
        this->y = y;
        return *this;
    }

    Vector2& setScalar (const double& scalar )
    {
        this->x = scalar;
        this->y = scalar;
        return *this;
    }

    inline Vector2& setX(const double& value)
    {
        x = value;
        return *this;
    }

    inline Vector2& setY(const double& value)
    {
        y = value;
        return *this;
    }

    void setComponent(const int& index, const double& value )
    {
        Q_ASSERT(index == 0 || index == 1);
        switch ( index )
        {
        case 0: this->x = value; break;
        case 1: this->y = value; break;
            // default: throw new Error( 'index is out of range: ' + index );
        }
    }

    double getComponent (const int& index )
    {
        Q_ASSERT(index == 0 || index == 1);
        switch ( index )
        {
        case 0: return this->x;
        case 1: return this->y;
            // default: throw new Error( 'index is out of range: ' + index );
        }

    }

    Vector2 clone () {
        return Vector2( this->x, this->y );
    }

    Vector2& copy(const Vector2& v )
    {
        this->x = v.x;
        this->y = v.y;
        return *this;
    }

    Vector2& add(const Vector2& v)
    {
        this->x += v.x;
        this->y += v.y;
        return *this;
    }

    Vector2& add(const Vector2& v, const Vector2& w)
    {
        return this->addVectors( v, w );
    }

    Vector2& addScalar ( const double& s )
    {
        this->x += s;
        this->y += s;
        return *this;
    }

    Vector2&  addVectors (const Vector2& a, const Vector2& b )
    {
        this->x = a.x + b.x;
        this->y = a.y + b.y;
        return *this;
    }

    Vector2& addScaledVector (const Vector2& v, const double& s )
    {
        this->x += v.x * s;
        this->y += v.y * s;
        return *this;
    }

    Vector2& sub( const Vector2&v)
    {
        this->x -= v.x;
        this->y -= v.y;
        return *this;
    }

    Vector2& sub( const Vector2& v, const Vector2& w)
    {
        return this->subVectors(v, w);
    }

    Vector2& subScalar(const double& s )
    {
        this->x -= s;
        this->y -= s;
        return *this;
    }

    Vector2& subVectors(const Vector2& a, const Vector2& b )
    {
        this->x = a.x - b.x;
        this->y = a.y - b.y;
        return *this;
    }

    Vector2&  multiply(const Vector2& v )
    {
        this->x *= v.x;
        this->y *= v.y;
        return *this;
    }

    Vector2&  multiplyScalar(const double& s )
    {
        this->x *= s;
        this->y *= s;
        return *this;	//返回新坐标值的二维向量
    }

    Vector2& divide(const Vector2& v)
    {
        Q_ASSERT(v.x !=0 && v.y != 0);
        this->x /= v.x;
        this->y /= v.y;
        return *this;
    }

    Vector2& divideScalar (const double& scalar )
    {
        if ( scalar != 0 ) {
            double invScalar = 1 / scalar;

            this->x *= invScalar;
            this->y *= invScalar;
        } else {
            this->x = 0;
            this->y = 0;
        }
        return *this;
    }

    Vector2& min(const Vector2& v )
    {
        if ( this->x > v.x ) {
            this->x = v.x;
        }
        if ( this->y > v.y ) {
            this->y = v.y;
        }
        return *this;
    }

    Vector2& max(const Vector2& v )
    {
        if ( this->x < v.x ) {
            this->x = v.x;
        }
        if ( this->y < v.y ) {
            this->y = v.y;
        }
        return *this;
    }


    Vector2& clamp(const Vector2& min, const Vector2& max)
    {
        if ( this->x < min.x ) {
            this->x = min.x;
        } else if ( this->x > max.x ) {
            this->x = max.x;
        }
        if ( this->y < min.y ) {
            this->y = min.y;
        } else if ( this->y > max.y ) {
            this->y = max.y;
        }
        return *this;
    }

    Vector2& clampScalar(const double& minVal, const double& maxVal )
    {
        Vector2 min,max;
        min.set( minVal, minVal );
        max.set( maxVal, maxVal );
        return this->clamp(min, max);
    }

    Vector2& clampLength (const double& min,const double& max )
    {
        double length = this->length();

        Q_ASSERT(length != 0);

        this->multiplyScalar( std::max( min, std::min( max, length ) ) / length );
        return *this;
    }

    Vector2& floor()
    {
        this->x = std::floor(this->x);
        this->y = std::floor(this->y);
        return *this;
    }

    Vector2& ceil()
    {
        this->x = std::ceil( this->x );
        this->y = std::ceil( this->y );
        return *this;
    }

    Vector2& round ()
    {
        this->x = std::round( this->x );
        this->y = std::round( this->y );
        return *this;
    }

    Vector2& roundToZero ()
    {
        this->x = ( this->x < 0 ) ? std::ceil( this->x ) : std::floor( this->x );
        this->y = ( this->y < 0 ) ? std::ceil( this->y ) : std::floor( this->y );
        return *this;
    }

    Vector2& negate ()
    {
        this->x = - this->x;
        this->y = - this->y;
        return *this;
    }

    double dot (const Vector2& v ) const
    {
        return this->x * v.x + this->y * v.y;
    }

    double lengthSq () const
    {
        return this->x * this->x + this->y * this->y;
    }

    double length () const
    {
        return std::sqrt( this->x * this->x + this->y * this->y );
    }

    double lengthManhattan() const
    {
        return std::abs( this->x ) + std::abs( this->y );
    }

    Vector2&  normalize ()
    {
        return this->divideScalar( this->length());
    }

    double angle () const
    {
        // computes the angle in radians with respect to the positive x-axis
        double angle = std::atan2( this->y, this->x );
        if ( angle < 0 ) angle += 2 * M_PI;
        return angle;
    }

    double distanceTo (const Vector2&  v )
    {
        return std::sqrt( this->distanceToSquared( v ) );
    }

    double distanceToSquared (const Vector2&  v )
    {
        double dx = this->x - v.x, dy = this->y - v.y;
        return dx * dx + dy * dy;
    }

    Vector2& setLength ( const double& l )
    {
        double oldLength = this->length();
        if ( oldLength != 0 && l != oldLength ) {
            this->multiplyScalar( l / oldLength );
        }
        return *this;
    }

    Vector2& lerp (const Vector2& v, const double& alpha)
    {
        this->x += ( v.x - this->x ) * alpha;
        this->y += ( v.y - this->y ) * alpha;
        return *this;
    }

    bool equals (const Vector2&  v ) const
    {
        return ( ( v.x == this->x ) && ( v.y == this->y ) );
    }

    Vector2& fromArray (const NumberArray& array )
    {
        this->x = array[ 0 ];
        this->y = array[ 1 ];
        return *this;
    }

    NumberArray  toArray ()
    {
        return NumberArray { this->x, this->y };
    }

    // TODO
    //    Vector2& fromAttribute ( attribute, index, offset = 0)
    //    {
    //        index = index * attribute.itemSize + offset;

    //        this->x = attribute.array[ index ];
    //        this->y = attribute.array[ index + 1 ];
    //        return *this;
    //    }

    Vector2& rotateAround (const Vector2& center, const double& angle ) {
        double c = std::cos( angle ), s = std::sin( angle );
        double x = this->x - center.x;
        double y = this->y - center.y;

        this->x = x * c - y * s + center.x;
        this->y = x * s + y * c + center.y;
        return *this;
    }

    // private:
    double x;
    double y;
};

} // namespace three

#endif // THREE_VECTOR2_H
