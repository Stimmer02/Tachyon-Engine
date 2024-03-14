#include "ClStructParser.h"
#include <fstream>


int main(){
    std::ifstream file("/home/stimmer02/Documents/UMCS/SEMESTR_5/projekt_zespołowy/tachion-engine/engine_structs/2_voxel.clcpp");
    std::string fileContents;
    std::string line;
    while (getline(file, line)){
        fileContents += line + '\n';
    }

    ClStructParser clParser;

    engineStruct* structure = clParser.processStruct(fileContents);
    if (structure == nullptr){
        std::fprintf(stderr, "Error has occured\n");
        return -1;
    }

    std::printf("name: %s; field count: %d\n", structure->name.c_str(), structure->fieldCount);
    for (uint i = 0; i < structure->fieldCount; i++){
        if (structure->fields[i].type == engineStruct::cl_struct){
            std::printf(" %d) var: %s; type: %d; struct: %s; amount: %d\n", i, structure->fields[i].name.c_str(), structure->fields[i].type, structure->fields[i].subStructName.c_str(), structure->fields[i].arrSize);
        } else {
            std::printf(" %d) var: %s; type: %d\n", i, structure->fields[i].name.c_str(), structure->fields[i].type);
        }
    }

    delete structure;
    return 0;
}
