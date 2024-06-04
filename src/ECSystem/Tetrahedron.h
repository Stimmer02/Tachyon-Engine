#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

#include <vector>

#include "Vector3.h"
#include "MeshLoader.h"
#include "SquareMatrix.h"


struct Triangle;

struct Tetrahedron{
    int points[4];
    
    Tetrahedron(const int &p1, const int &p2, const int &p3, const int &p4);
    
    bool helperFun(const int &p1, const int &p2, const int &p3, const int &p4, const int &p5) const;
    
    bool isPointInsideTetrahedron(const int &point) const;
    void getCircumscribedSphere(Vector3 &centre, float &radius) const;

    bool operator<(const Tetrahedron &other) const;
    bool operator=(const Tetrahedron &other);
};


#endif