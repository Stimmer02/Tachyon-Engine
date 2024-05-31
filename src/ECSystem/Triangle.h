#ifndef TRIANLGE_H
#define TRIANGLE_H

#include <set>
#include "Vector3.h"

class Tetrahedron;


struct Triangle{
    std::set<Vector3> Points;
    std::set<Tetrahedron> tetrahedrons;
};


#endif