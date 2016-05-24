#include "plane.h"

#include "box3.h"

namespace three {

bool Plane::intersectsBox(const Box3 &box) {
    return box.intersectsPlane( *this );
}



} // namespace three

