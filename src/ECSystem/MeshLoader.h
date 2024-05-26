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
    Vector3* vertices;
    unsigned int numVertices;
    std::vector<Vector3> verticesVector;
    
    Vector3* normals; 
    unsigned int numNormals;
    std::vector<Vector3> normalsVector;
    
    unsigned int* indices; 
    unsigned int numIndices;
    std::vector<unsigned int> indicesVector;
    
    float* texCoords;
    unsigned int numTexCoords;
    std::vector<float> texCoordsVector;
    
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
    static void ParseMesh(const std::string &pathToFile, Mesh* mesh);
};

#endif