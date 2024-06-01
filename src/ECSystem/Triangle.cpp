#include "Triangle.h"
#include "Tetrahedron.h"

bool Triangle::getConfiguration(){
    if(tetrahedrons.size() < 2){
        return false;
    }
    
    Vector3 circleCenter;
    float radius;
    std::vector <Vector3> p;
    
    for(int i = 0 ; i < tetrahedrons.size(); ++i){
        for(int j = 0; j < 4; ++j){
            if(points.count(MeshLoader::verticesVector[tetrahedrons[i].points[j]]) != 0){
                p.push_back(MeshLoader::verticesVector[tetrahedrons[i].points[j]]);
            }
        }
    }
    
    //p.size() == 2
    
    for(int i = 0; i < tetrahedrons.size(); ++i){
        tetrahedrons[i].getCircumscribedSphere(circleCenter, radius);
        if(radius <= Vector3::Distance(circleCenter, p[tetrahedrons.size() - 1 - i])){
            return true;
        }
    }
    
    return false;
    
}