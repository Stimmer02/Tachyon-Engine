#include "MeshLoader.h"

void MeshLoader::addSingleVertex(const std::string &vertexLine){
    
}
void MeshLoader::addSingleNormal(const std::string &normal){
    
}
void MeshLoader::addSingleIndex(const std::string &indexLine){
    
}
void MeshLoader::addSingleTexCoord(const std::string &texCoordLine){
    
}

void MeshLoader::doNothing(const std::string &comment){}

void MeshLoader::initFunctionsArray(){
    for(int i = 0; i < functionsArraySize; ++i){
        functionsArr[i] = doNothing;
    }
    functionsArr['v' * charSize + ' '] = addSingleVertex;
    functionsArr['v' * charSize + 'n'] = addSingleNormal;
    functionsArr['v' * charSize + 't'] = addSingleTexCoord;
    functionsArr['f' * charSize + ' '] = addSingleIndex;
}

void MeshLoader::ParseMesh(const std::string &pathToFile, Mesh* mesh){
    initFunctionsArray();
    
    std::ifstream file(pathToFile);
    std::string helper;
    
    if(!file){
        //tutaj coś trzeba by dodać innego ale najszybciej na razie jest dać cout;
        std::cout << "Brak Pliku";
    }
    
    while(std::getline(file, helper)){
        functionsArr[helper[0] * charSize + helper[1]](helper);
    }
}