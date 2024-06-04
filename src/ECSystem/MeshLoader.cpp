#include "MeshLoader.h"
#include "Mesh.h"

unsigned int MeshLoader::numVertices;
std::vector<Vector3> MeshLoader::verticesVector;

Vector3* MeshLoader::normals;
unsigned int MeshLoader::numNormals;
std::vector<Vector3> MeshLoader::normalsVector;

unsigned int MeshLoader::numIndices;
int MeshLoader::caseId = 0;
std::vector<unsigned int> MeshLoader::indicesVector;

unsigned int MeshLoader::numTexCoords;
std::vector<float> MeshLoader::texCoordsVector;

std::vector<std::vector<Vector3> > MeshLoader::normalsList;
std::vector<int> MeshLoader::texturesIdx;

void (*MeshLoader::functionsArr[functionsArraySize])(const std::string&);

void MeshLoader::addSingleVertex(const std::string &vertexLine){
    const int startId = 2;
    std::string singleCoord;
    std::vector <float> coords;

    for(int i = startId;; ++i){
        if(vertexLine[i] == ' ' || vertexLine[i] == '\n' || vertexLine[i] == '\0'){
            coords.push_back(std::atof(singleCoord.c_str()));
            singleCoord.clear();
            if(vertexLine[i] == '\n' || vertexLine[i] == '\0'){
                break;
            }
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

    for(int i = startId;; ++i){
        if(normalLine[i] == ' ' || normalLine[i] == '\n' || normalLine[i] == '\0'){
            coords.push_back(std::atof(singleCoord.c_str()));
            singleCoord.clear();
            if(normalLine[i] == '\n' || normalLine[i] == '\0'){
                break;
            }
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

    for(int i = startId;; ++i){
        if(texCoordLine[i] == ' ' || texCoordLine[i] == '\n' || texCoordLine[i] == '\0'){
            coords.push_back(std::atof(singleCoord.c_str()));
            singleCoord.clear();
            if(texCoordLine[i] == '\n' || texCoordLine[i] == '\0'){
                break;
            }
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

    // std::cout << indexLine << '\n';
    for(int i = 2;; ++i){
        if(indexLine[i] == ' ' || indexLine[i] == '\n' || indexLine[i] == '\0' || indexLine[i] == '/'){
            if(singleCoord.size() != 0){
                helperIdxArray.push_back(std::atoi(singleCoord.c_str()));
            }
            singleCoord.clear();
            if(indexLine[i] == '\n' || indexLine[i] == '\0'){
                break;
            }
        }
        else{
            singleCoord+=indexLine[i];
        }
        // std::cout << singleCoord << '\n';
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

    // for(int i = 0; i < helperIdxArray.size(); ++i){
    //     std::cout << helperIdxArray[i] << ' ';
    // }

    if(caseId == 0){
        for(int i = 0; i < helperIdxArray.size(); ++i){
            helperIdxArray[i]--;
        }
        triangulationInput = helperIdxArray;

    }
    else if(caseId == 1){
        for(int i = 0 ; i < helperIdxArray.size(); i+=2){
            helperIdxArray[i]--;
            // std::cout << "ASD: " << helperIdxArray[i + 1] << '\n';
            normalsList[helperIdxArray[i]].push_back(normalsVector[helperIdxArray[i + 1] - 1]);
            triangulationInput.push_back(helperIdxArray[i]);
        }

    }
    else{
        for(int i = 0 ; i < helperIdxArray.size(); i+=3){
            helperIdxArray[i]--;
            normalsList[helperIdxArray[i]].push_back(normalsVector[helperIdxArray[i + 2] - 1]);
            texturesIdx[helperIdxArray[i]] = helperIdxArray[i + 1];
            triangulationInput.push_back(helperIdxArray[i]);
        }
    }
    // std::cout << triangulationInput.size() << '\n';
    // std::cout.flush();
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

            normalsList[triangulationResult[i + 2]].push_back(helperNormal);
        }
    }

    // std::cout << triangulationResult.size() << '\n';


    // for(int i = 0; i < triangulationResult.size(); ++i){
    //     std::cout << triangulationResult[i] << '\n';
    // }
    // std::cout << '\n';
    // std::cout << '\n';
    // std::cout.flush();



    for(int i: triangulationResult){
        indicesVector.push_back(i);
    }


}


void MeshLoader::doNothing(const std::string &comment){}

void MeshLoader::init(){
    for(int i = 0; i < functionsArraySize; ++i){
        functionsArr[i] = doNothing;
    }
    functionsArr['v' * charSize + ' '] = addSingleVertex;
    functionsArr['v' * charSize + 'n'] = addSingleNormal;
    functionsArr['v' * charSize + 't'] = addSingleTexCoord;
    functionsArr['f' * charSize + ' '] = addSingleIndex;

    //to żeby na początku pod indeksem 0 była tekstura o współrzędnych 0 0 i żeby reszta była indeksowana od 1
    texCoordsVector.push_back(0.0f);
    texCoordsVector.push_back(0.0f);

    numVertices = 0;
    numNormals = 0;
    numIndices = 0;
    numTexCoords = 0;
}

void MeshLoader::finalizeParsing(Mesh* mesh){
    numIndices = indicesVector.size();
    numVertices = verticesVector.size();
    numNormals = numVertices;
    numTexCoords = numVertices;

    normals = new Vector3[numNormals];

    Vector3 helperNormal;

    for(int i = 0; i < numVertices; ++i){
        helperNormal = {0.0f, 0.0f, 0.0f};
        for(int j = 0; j < normalsList[i].size(); ++j){
            helperNormal += normalsList[i][j];
        }
        // if(normalsList[i].size() != 0){
        //     helperNormal /= float(normalsList[i].size());
        // }
        helperNormal = helperNormal.Normalize();
        normals[i] = helperNormal;
    }



    // std::cout << normalsList.size() << '\n';
    // std::cout.flush();
    // for(int i = 0; i < normalsList.size(); ++i){
    //     for(int j = 0; j < normalsList[i].size(); ++j){
    //         std::cout << normalsList[i][j].x << ' ' << normalsList[i][j].y << ' ' << normalsList[i][j].z << '\n';
    //     }
    //     std::cout << '\n';
    // }
    // std::cout.flush();

    // for(int i = 0; i < verticesVector.size(); ++i){
    //     std::cout << verticesVector[i].x << ' ' << verticesVector[i].y << ' ' << verticesVector[i].z << '\n';
    // }


    // for(int i = 0; i < numIndices; ++i){
    //     std::cout << indicesVector[i] << ' ' << indicesVector[i] << ' ' << indicesVector[i] << '\n';
    // }

    // for(int i = 0; i < numNormals; ++i){
    //     std::cout << normals[i].x << ' ' << normals[i].y << ' ' << normals[i].z << '\n';
    // }

    mesh->SetVertices(verticesVector.data(), numVertices);
    mesh->SetNormals(normals, numNormals);
    mesh->SetTexCoords(texCoordsVector.data(), numTexCoords);
    mesh->SetIndices(indicesVector.data(), numIndices);

    delete [] normals;

    verticesVector.clear();
    normalsVector.clear();
    indicesVector.clear();
    texCoordsVector.clear();
    normalsList.clear();
    texturesIdx.clear();

}

std::vector<int> MeshLoader::computeTriangulation(const std::vector<int> &inputPoints){

    if(inputPoints.size() == 3){
        return inputPoints;
    }

    //Tutaj jest miejsce na ewentualny rozwój projektu.
    std::vector<int> empty;
    return empty;
}

void MeshLoader::parseMesh(const std::string &pathToFile, Mesh* mesh){
    init();

    std::ifstream file(pathToFile);
    std::string helper;

    if(!file){
        //tutaj coś trzeba by dodać innego ale najszybciej na razie jest dać cout;
        // std::cout << "Brak Pliku";
    }

    while(std::getline(file, helper)){
        functionsArr[helper[0] * charSize + helper[1]](helper);
    }

    finalizeParsing(mesh);
}
