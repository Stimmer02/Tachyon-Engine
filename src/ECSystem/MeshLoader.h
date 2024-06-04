#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>
#include <queue>

class Mesh;

#include "Tetrahedron.h"
#include "Vector3.h"
#include "Triangle.cpp"

class MeshLoader{
public:
    static unsigned int numVertices;
    static std::vector<Vector3> verticesVector;

    static Vector3* normals;
    static unsigned int numNormals;
    static std::vector<Vector3> normalsVector;

    static int caseId;
    static unsigned int numIndices;
    static std::vector<unsigned int> indicesVector;

    static unsigned int numTexCoords;
    static std::vector<float> texCoordsVector;


    static std::vector<std::vector<Vector3> > normalsList;
    static std::vector<int> texturesIdx;

    static const int functionsArraySize = 256 * 256;
    static const int charSize = 256;

    static void (*functionsArr[functionsArraySize])(const std::string&);

    static void addSingleVertex(const std::string &vertexLine);
    static void addSingleNormal(const std::string &normal);
    static void addSingleIndex(const std::string &indexLine);
    static void addSingleTexCoord(const std::string &texCoordLine);
    static void doNothing(const std::string &comment);

    static std::vector<int> computeTriangulation(const std::vector<int> &inputPoints);
    static void init();
    static void finalizeParsing(Mesh* mesh);

    static void parseMesh(const std::string &pathToFile, Mesh* mesh);
};

#endif
