#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <string>
#include <vector>
#include <fstream>

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
    
    
    void addSingleVertex(const std::string &vertexLine);
    void addSingleNormal(const std::string &normal);
    void addSingleIndex(const std::string &indexLine);
    void addSingleTexCoord(const std::string &texCoordLine);
public:
    static void ParseMesh(const std::string &pathToFile, Mesh* mesh);
    
};

#endif