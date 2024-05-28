#include "Tetrahedron.h"

Tetrahedron::Tetrahedron(const int &p1, const int &p2, const int &p3, const int &p4){
    points[0] = p1;
    points[1] = p2;
    points[2] = p3;
    points[3] = p4;
    
}

float Tetrahedron::helperFun(const int &p1, const int &p2, const int &p3, const int &p4){
    Vector3 helperAD, helperBD, helperCD, helperCrossProduct;
    
    float helperValue;
    
    helperAD = MeshLoader::verticesVector[p1] - MeshLoader::verticesVector[p4];
    helperBD = MeshLoader::verticesVector[p2] - MeshLoader::verticesVector[p4];
    helperCD = MeshLoader::verticesVector[p3] - MeshLoader::verticesVector[p4];
    
    helperCrossProduct = Vector3::Cross(helperBD, helperCD);
    
    helperValue = fabs(Vector3::Dot(helperAD, helperCrossProduct) / 6.0f);
    
    return helperValue;
    
}

bool Tetrahedron::isPointInsideTetrahedron(const int &point){
    float v1, v2, v3, v4, v5;
    v1 = helperFun(points[0], points[1], points[2], points[3]);
    v2 = helperFun(point, points[1], points[2], points[3]);
    v3 = helperFun(points[0], point, points[2], points[3]);
    v4 = helperFun(points[0], points[1], point, points[3]);
    v5 = helperFun(points[0], points[1], points[2], point);
    if(v2 > 0 && v3 > 0 && v4 > 0 && v5 > 0 && (v2 + v3 + v4 + v5 == v1)){
        return true;
    }
    else{
        return false;
    }
}