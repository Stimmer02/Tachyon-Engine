#include "Tetrahedron.h"
#include "Triangle.h"

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

void Tetrahedron::operator=(const Tetrahedron &other){
    points[0] = other.points[0];
    points[1] = other.points[1];
    points[2] = other.points[2];
    points[3] = other.points[3];
}

void Tetrahedron::getCircumscribedSphere(Vector3 &centre, float &radius) const{
    float alfa, gamma, dx, dy, dz, alfa2;
    const int size = 5;
    SquareMatrix<float> helperMatrix;
    float** helperArr;

    helperArr = new float*[size];
    for(int i = 0; i < size; ++i){
        helperArr[i] = new float[4];
    }

    //te wartości właściwie nie mają znaczenia, znaczy mają wielkie znaczenie ale nie zmieniają wyniku działania tego algorytmu
    helperArr[0][0] = 420;
    helperArr[0][1] = 44;
    helperArr[0][2] = 42;
    helperArr[0][3] = 2137;
    helperArr[0][4] = 37; //a to po prostu losowa liczba hehe

    helperArr[1][0] = pow(MeshLoader::verticesVector[points[0]].x, 2.0f) + pow(MeshLoader::verticesVector[points[0]].y, 2.0f) + pow(MeshLoader::verticesVector[points[0]].z, 2.0f);
    helperArr[1][1] = MeshLoader::verticesVector[points[0]].x;
    helperArr[1][2] = MeshLoader::verticesVector[points[0]].y;
    helperArr[1][3] = MeshLoader::verticesVector[points[0]].z;
    helperArr[1][4] = 1;

    helperArr[2][0] = pow(MeshLoader::verticesVector[points[1]].x, 2.0f) + pow(MeshLoader::verticesVector[points[1]].y, 2.0f) + pow(MeshLoader::verticesVector[points[1]].z, 2.0f);
    helperArr[2][1] = MeshLoader::verticesVector[points[1]].x;
    helperArr[2][2] = MeshLoader::verticesVector[points[1]].y;
    helperArr[2][3] = MeshLoader::verticesVector[points[1]].z;
    helperArr[2][4] = 1;

    helperArr[3][0] = pow(MeshLoader::verticesVector[points[2]].x, 2.0f) + pow(MeshLoader::verticesVector[points[2]].y, 2.0f) + pow(MeshLoader::verticesVector[points[2]].z, 2.0f);
    helperArr[3][1] = MeshLoader::verticesVector[points[2]].x;
    helperArr[3][2] = MeshLoader::verticesVector[points[2]].y;
    helperArr[3][3] = MeshLoader::verticesVector[points[2]].z;
    helperArr[3][4] = 1;

    helperArr[4][0] = pow(MeshLoader::verticesVector[points[3]].x, 2.0f) + pow(MeshLoader::verticesVector[points[3]].y, 2.0f) + pow(MeshLoader::verticesVector[points[3]].z, 2.0f);
    helperArr[4][1] = MeshLoader::verticesVector[points[3]].x;
    helperArr[4][2] = MeshLoader::verticesVector[points[3]].y;
    helperArr[4][3] = MeshLoader::verticesVector[points[3]].z;
    helperArr[4][4] = 1;

    helperMatrix.setArr(helperArr, size);

    alfa = helperMatrix.getSubDeterminant(0);
    gamma = helperMatrix.getSubDeterminant(4);
    dx = helperMatrix.getSubDeterminant(1);
    dy = helperMatrix.getSubDeterminant(2);
    dz = helperMatrix.getSubDeterminant(3);

    alfa2 = alfa * 2;

    centre = {dx / alfa2, dy / alfa2, dz / alfa2};
    radius = sqrt(dx * dx + dy * dy + dz * dz - 2 * alfa2 * gamma) / fabs(alfa2);
}
