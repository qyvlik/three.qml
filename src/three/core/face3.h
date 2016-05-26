#ifndef THREE_FACE3_H
#define THREE_FACE3_H

#include <QVector>
#include "../math/color.h"
#include "../math/vector3.h"


namespace three {

class Vector3;
typedef QVector<Vector3> Vector3Array;
typedef QVector<Color> ColorArray;

class Face3
{
public:
    Face3(const double& a, const double& b, const double& c ):
        a(a),
        b(b),
        c(c)
    { }

    Face3(const double& a, const double& b, const double& c, const Color& color, const Vector3Array& materialIndex ):
        a(a),
        b(b),
        c(c),
        color(color),
        materialIndex(materialIndex)
    { }

    Face3 clone() const
    {
        Face3 face(this->a, this->b, this->c);
        face.vertexNormals = this->vertexNormals;
        face.vertexColors = this->vertexColors;
        face.vertexTangents = this->vertexTangents;
        return face;
    }

// private:
    double a;
    double b;
    double c;
    Vector3Array normal;
    Vector3Array vertexNormals;
    Color color;
    ColorArray vertexColors;
    Vector3Array vertexTangents;
    Vector3Array materialIndex;
};

} // namespace three

#endif // THREE_FACE3_H
