#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "Mesh.h"
#include "Vector3.h"

class MeshLoader{
private:
    static Vector3* vertices;
    static unsigned int numVertices;
    static std::vector<Vector3> verticesVector;

    static Vector3* normals; 
    static unsigned int numNormals;
    static std::vector<Vector3> normalsVector;

    static unsigned int* indices; 
    static unsigned int numIndices;
    static std::vector<unsigned int> indicesVector;

    static float* texCoords;
    static unsigned int numTexCoords;
    static std::vector<float> texCoordsVector;
    
    static const int functionsArraySize = 256 * 256;
    static const int charSize = 256;
    
    static void (*functionsArr[functionsArraySize])(const std::string&);
    
    static void addSingleVertex(const std::string &vertexLine);
    static void addSingleNormal(const std::string &normal);
    static void addSingleIndex(const std::string &indexLine);
    static void addSingleTexCoord(const std::string &texCoordLine);
    static void doNothing(const std::string &comment);
    
    static void initFunctionsArray();
    
public:
    static void parseMesh(const std::string &pathToFile, Mesh* mesh);
};

#endif