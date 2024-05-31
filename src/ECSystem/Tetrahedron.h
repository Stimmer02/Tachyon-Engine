#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

#include "Vector3.h"
#include "MeshLoader.h"

struct Tetrahedron{
    int points[4];
    
    Tetrahedron(const int &p1, const int &p2, const int &p3, const int &p4);
    
    bool helperFun(const int &p1, const int &p2, const int &p3, const int &p4, const int &p5) const;
    
    bool isPointInsideTetrahedron(const int &point) const;

    bool operator<(const Tetrahedron &other) const;
    bool operator=(const Tetrahedron &other);
};


#endif