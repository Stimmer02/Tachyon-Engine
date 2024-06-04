// #include "Triangle.h"
// #include "Tetrahedron.h"

// Triangle::Triangle(){}

// Triangle::Triangle(Vector3 p1, Vector3 p2, Vector3 p3){
//     points.insert(p1);
//     points.insert(p2);
//     points.insert(p3);
// }
// Triangle::Triangle(int id1, int id2, int id3){
//     points.insert(MeshLoader::verticesVector[id1]);
//     points.insert(MeshLoader::verticesVector[id2]);
//     points.insert(MeshLoader::verticesVector[id3]);
    
// }

// Triangle::~Triangle(){
//     points.clear();
// }

// bool Triangle::getConfiguration(const std::vector<Tetrahedron> &tetrahedrons){
//     //return true if configuration should be changed 
//     if(tetrahedrons.size() < 2){
//         return false;
//     }
    
//     Vector3 circleCenter;
//     float radius;
//     std::vector <Vector3> p;
    
//     for(int i = 0 ; i < tetrahedrons.size(); ++i){
//         for(int j = 0; j < 4; ++j){
//             // if(points.count(MeshLoader::verticesVector[tetrahedrons[i].points[j]]) != 0){
//             //     p.push_back(MeshLoader::verticesVector[tetrahedrons[i].points[j]]);
//             // }
//         }
//     }
    
//     //p.size() == 2
    
//     for(int i = 0; i < tetrahedrons.size(); ++i){
//         tetrahedrons[i].getCircumscribedSphere(circleCenter, radius);
//         if(radius <= Vector3::Distance(circleCenter, p[tetrahedrons.size() - 1 - i])){
//             return true;
//         }
//     }
//     return false;
    
// }
