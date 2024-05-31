#include "Tetrahedron.h"

Tetrahedron::Tetrahedron(const int &p1, const int &p2, const int &p3, const int &p4){
    points[0] = p1;
    points[1] = p2;
    points[2] = p3;
    points[3] = p4;
    
}

bool Tetrahedron::helperFun(const int &p1, const int &p2, const int &p3, const int &p4, const int & p5) const{
    //kod tej funkcji jest z https://stackoverflow.com/questions/25179693/how-to-check-whether-the-point-is-in-the-tetrahedron-or-not
    Vector3 helperNormal;
    float helperDot, helperDot2;
    
    helperNormal = Vector3::Cross(MeshLoader::verticesVector[p2] - MeshLoader::verticesVector[p1], MeshLoader::verticesVector[p3] - MeshLoader::verticesVector[p1]);
    
    helperDot = Vector3::Dot(helperNormal, MeshLoader::verticesVector[p4] - MeshLoader::verticesVector[p1]);
    
    helperDot2 = Vector3::Dot(helperNormal,MeshLoader::verticesVector[p5] - MeshLoader::verticesVector[p1]);
    
    return helperDot * helperDot2 >= 0;
}

bool Tetrahedron::isPointInsideTetrahedron(const int &point) const{
    //kod tej funkcji jest z https://stackoverflow.com/questions/25179693/how-to-check-whether-the-point-is-in-the-tetrahedron-or-not
    // std::cout << MeshLoader::verticesVector[points[0]].x << ' ' << MeshLoader::verticesVector[points[0]].y  <<  ' ' << MeshLoader::verticesVector[points[0]].z << '\n';
    // std::cout << MeshLoader::verticesVector[points[1]].x << ' ' << MeshLoader::verticesVector[points[1]].y  <<  ' ' << MeshLoader::verticesVector[points[1]].z << '\n';
    // std::cout << MeshLoader::verticesVector[points[2]].x << ' ' << MeshLoader::verticesVector[points[2]].y  <<  ' ' << MeshLoader::verticesVector[points[2]].z << '\n';
    // std::cout << MeshLoader::verticesVector[points[3]].x << ' ' << MeshLoader::verticesVector[points[3]].y  <<  ' ' << MeshLoader::verticesVector[points[3]].z << '\n';
    // std::cout << MeshLoader::verticesVector[point].x << ' ' << MeshLoader::verticesVector[point].y  <<  ' ' << MeshLoader::verticesVector[point].z << '\n' << '\n';
    
    return helperFun(points[0], points[1], points[2], points[3], point) && helperFun(points[1], points[2], points[3], points[0], point) && helperFun(points[2], points[3], points[0], points[1], point) && helperFun(points[3], points[0], points[1], points[2], point);
    
}

bool Tetrahedron::operator<(const Tetrahedron& other)const {
    for(int i = 0; i < 4; ++i){
        if(points[i] < other.points[i]){
            return true;
        }
        else if(points[i] > other.points[i]){
            return false;
        }
    }
    return false;
}

bool Tetrahedron::operator=(const Tetrahedron &other){
    points[0] = other.points[0];
    points[1] = other.points[1];
    points[2] = other.points[2];
    points[3] = other.points[3];
}