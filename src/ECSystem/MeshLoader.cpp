#include "MeshLoader.h"

Vector3* MeshLoader::vertices;
unsigned int MeshLoader::numVertices;
std::vector<Vector3> MeshLoader::verticesVector;

Vector3* MeshLoader::normals; 
unsigned int MeshLoader::numNormals;
std::vector<Vector3> MeshLoader::normalsVector;

unsigned int* MeshLoader::indices; 
unsigned int MeshLoader::numIndices;
int MeshLoader::caseId = 0;
std::vector<unsigned int> MeshLoader::indicesVector;

float* MeshLoader::texCoords;
unsigned int MeshLoader::numTexCoords;
std::vector<float> MeshLoader::texCoordsVector;

std::vector<std::vector<Vector3> > MeshLoader::normalsList;
std::vector<int> MeshLoader::texturesIdx;
    
void (*MeshLoader::functionsArr[functionsArraySize])(const std::string&);

void MeshLoader::addSingleVertex(const std::string &vertexLine){
    const int startId = 2;
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
    
    MeshLoader::verticesVector.push_back({coords[0], coords[1], coords[2]});
    if(coords.size() == 4){
        MeshLoader::verticesVector[MeshLoader::verticesVector.size() - 1].w = coords[3];
    }
    
    numVertices = verticesVector.size();
    vertices = new Vector3[numVertices];
    for(int i = 0; i < numVertices; ++i){
        vertices[i] = verticesVector[i];
    }
    
    
    std::vector<Vector3> emptyVec;
    while(normalsList.size() < verticesVector.size()){
        normalsList.push_back(emptyVec);
    }
    while(texturesIdx.size() < verticesVector.size()){
        texturesIdx.push_back(0);
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
    const int startId = 2;
    std::string singleCoord;
    std::vector<int> helperIdxArray;
    std::vector<int> triangulationResult;
    std::vector<int> triangulationInput;
    Vector3 helperNormal, edge1, edge2;
    
    
    for(int i = 2; indexLine[i] != ' ' && indexLine[i] != '\0' && indexLine[i] != '\n'; ++i){
        if(indexLine[i] == ' ' || indexLine[i] == '\n' || indexLine[i] == '\0' || indexLine[i] == '/'){
            helperIdxArray.push_back(std::atoi(singleCoord.c_str()));
            singleCoord.clear();
        }
        else{
            singleCoord+=indexLine[i];
        }
    }
    
    for(int i = 2; indexLine[i] != ' ' && indexLine[i] != '\0' && indexLine[i] != '\n'; ++i){
        if(indexLine[i] == '/'){
            if(indexLine[i + 1] == '/'){
                caseId = 1;
            }
            else{
                caseId = 2;
            }
            break;
        }
    }
    
    if(caseId == 0){
        triangulationInput = helperIdxArray;
    }
    else if(caseId == 1){
        for(int i = 0 ; i < helperIdxArray.size(); i+=2){
            normalsList[helperIdxArray[i]].push_back(normalsVector[helperIdxArray[i + 1]]);
            triangulationInput.push_back(helperIdxArray[i]);
        }
        
    }
    else{
        for(int i = 0 ; i < helperIdxArray.size(); i+=3){
            normalsList[helperIdxArray[i]].push_back(normalsVector[helperIdxArray[i + 2]]);
            texturesIdx[helperIdxArray[i]] = helperIdxArray[i + 1];
            triangulationInput.push_back(helperIdxArray[i]);
        }   
    }
    triangulationResult = computeTriangulation(triangulationInput);
    
    if(caseId == 0){
        for(int i = 0; i < triangulationResult.size(); i += 3){
            edge1 = (verticesVector[triangulationResult[i + 2]] - verticesVector[triangulationResult[i]]).Normalize();
            edge2 = (verticesVector[triangulationResult[i + 1]] - verticesVector[triangulationResult[i]]).Normalize();
            
            helperNormal = Vector3::Cross(edge1, edge2).Normalize();
            
            normalsList[triangulationResult[i]].push_back(helperNormal);
            
            
            edge1 = (verticesVector[triangulationResult[i]] - verticesVector[triangulationResult[i + 1]]).Normalize();
            edge2 = (verticesVector[triangulationResult[i + 2]] - verticesVector[triangulationResult[i + 1]]).Normalize();
            
            helperNormal = Vector3::Cross(edge1, edge2).Normalize();
            
            normalsList[triangulationResult[i + 1]].push_back(helperNormal);
            
            
            edge1 = (verticesVector[triangulationResult[i + 1]] - verticesVector[triangulationResult[i + 2]]).Normalize();
            edge2 = (verticesVector[triangulationResult[i]] - verticesVector[triangulationResult[i + 2]]).Normalize();
            
            helperNormal = Vector3::Cross(edge1, edge2).Normalize();
            
            normalsList[triangulationResult[i]].push_back(helperNormal);
        }
    }
    
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

void MeshLoader::finalizeParsing(){
    
}

std::vector<int> MeshLoader::computeTriangulation(const std::vector<int> &inputPoints){
    
    std::vector<Tetrahedron> tetrahedrons;
    std::map< std::pair< std::pair<int, int>, int >, int > trianglesCount;
    std::vector<int> result;
    
    float helper = 0.0f;
    
    for(int i = 0; i < numVertices; ++i){
        helper = std::max(helper, fabs(verticesVector[inputPoints[i]].x));
        helper = std::max(helper, fabs(verticesVector[inputPoints[i]].y));
        helper = std::max(helper, fabs(verticesVector[inputPoints[i]].z));
    }
    
    
    
    verticesVector.push_back({-helper, -helper, -helper});
    verticesVector.push_back({helper, -helper, -helper});
    verticesVector.push_back({0, helper, -helper});
    verticesVector.push_back({0, 0, helper});
    
    tetrahedrons.push_back({numVertices, numVertices + 1, numVertices + 2, numVertices + 3});
    
    
    for(int i = 0; i < inputPoints.size(); ++i){
        for(int j = 0; j < tetrahedrons.size(); ++j){
            if(tetrahedrons[j].isPointInsideTetrahedron(inputPoints[i])){
                
                tetrahedrons.push_back({tetrahedrons[j].points[0], tetrahedrons[j].points[1], tetrahedrons[j].points[2], inputPoints[i]});
                tetrahedrons.push_back({tetrahedrons[j].points[0], tetrahedrons[j].points[1], tetrahedrons[j].points[3], inputPoints[i]});
                tetrahedrons.push_back({tetrahedrons[j].points[0], tetrahedrons[j].points[2], tetrahedrons[j].points[3], inputPoints[i]});
                tetrahedrons.push_back({tetrahedrons[j].points[1], tetrahedrons[j].points[2], tetrahedrons[j].points[3], inputPoints[i]});
                
                std::swap(tetrahedrons[j], tetrahedrons[tetrahedrons.size() - 1]);
                
                tetrahedrons.pop_back();
                
                break;
            }
        }
    }
    
    for(Tetrahedron &t: tetrahedrons){
        
        std::sort(t.points, t.points + 4);
        
        trianglesCount[{{t.points[0], t.points[1]}, t.points[2]}]++;
        trianglesCount[{{t.points[0], t.points[1]}, t.points[3]}]++;
        trianglesCount[{{t.points[0], t.points[2]}, t.points[3]}]++;
        trianglesCount[{{t.points[1], t.points[2]}, t.points[3]}]++;
    }
    
    
    for(auto i = trianglesCount.begin(); i != trianglesCount.end(); ++i){
        if(i->second == 1 && i->first.first.first <= numVertices && i->first.first.second <= numVertices && i->first.second <= numVertices){
            result.push_back(i->first.first.first);
            result.push_back(i->first.first.second);
            result.push_back(i->first.second);
        }
    }
    
    verticesVector.pop_back();
    verticesVector.pop_back();
    verticesVector.pop_back();
    verticesVector.pop_back();
    
    return result;
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