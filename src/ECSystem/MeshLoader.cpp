#include "MeshLoader.h"

void MeshLoader::addSingleVertex(const std::string &vertexLine){
    const int startId = 3;
    std::string singleCoord;
    std::vector <float> coords;
    
    for(int i = startId; vertexLine[i] != '\n'; ++i){
        if(vertexLine[i] == ' ' || vertexLine[i] == '\n' || vertexLine[i] == '\0'){
            coords.push_back(std::atof(singleCoord.c_str()));
            singleCoord.clear();
        }
        else{    
           singleCoord+=vertexLine[i];
        }
    }
    
    verticesVector.push_back({coords[0], coords[1], coords[2]});
    if(coords.size() == 4){
        verticesVector[verticesVector.size() - 1].w = coords[3];
    }
    
}
void MeshLoader::addSingleNormal(const std::string &normalLine){
    const int startId = 3;
    std::string singleCoord;
    std::vector <float> coords;
    
    for(int i = startId; normalLine[i] != '\n'; ++i){
        if(normalLine[i] == ' ' || normalLine[i] == '\n' || normalLine[i] == '\0'){
            coords.push_back(std::atof(singleCoord.c_str()));
            singleCoord.clear();
        }
        else{   
            singleCoord+=normalLine[i];
        }
    }
    
    normalsVector.push_back({coords[0], coords[1], coords[2]});
    
}

void MeshLoader::addSingleTexCoord(const std::string &texCoordLine){
    const int startId = 3;
    std::string singleCoord;
    std::vector <float> coords;
    
    for(int i = startId; texCoordLine[i] != '\n'; ++i){
        if(texCoordLine[i] == ' ' || texCoordLine[i] == '\n' || texCoordLine[i] == '\0'){
            coords.push_back(std::atof(singleCoord.c_str()));
            singleCoord.clear();
        }
        else{
            singleCoord+=texCoordLine[i];
        }
    }
    
    texCoordsVector.push_back(coords[0]);
    texCoordsVector.push_back(coords[1]);
    //pomijam 3 współrzędną jeśli owa istnieje bo tak można zrobić a jest prościej 
}

void MeshLoader::addSingleIndex(const std::string &indexLine){
    
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

void MeshLoader::parseMesh(const std::string &pathToFile, Mesh* mesh){
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