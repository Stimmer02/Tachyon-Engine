#ifndef TRIANLGE_H
#define TRIANGLE_H

#include <set>
#include <vector>
#include "Vector3.h"

struct Tetrahedron;


struct Triangle{
    std::set<Vector3> points;
    std::vector<Tetrahedron> tetrahedrons;
    bool getConfiguration();
};


#endif