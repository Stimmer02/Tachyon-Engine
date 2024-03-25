#include "ClStructParser.h"
#include "SizeCalculator.h"
#include <fstream>


int main(){
    std::ifstream file("/home/stimmer02/Documents/UMCS/SEMESTR_6/projetk_zespolowy/tachion-engine/engine_structs/8_engineConfig.clcpp");
    std::string fileContents;
    std::string line;
    while (getline(file, line)){
        fileContents += line + '\n';
    }

    ClStructParser clParser;
    SizeCalculator sCalc(8);

    MacroManager macroManager;
    if (macroManager.parseFile("/home/stimmer02/Documents/UMCS/SEMESTR_6/projetk_zespolowy/tachion-engine/config/macros.cfg")){
        std::fprintf(stderr, "Error has occured\n");
        return -1;
    }

    engineStruct* structure = clParser.processStruct(fileContents, macroManager);
    if (structure == nullptr){
        std::fprintf(stderr, "Error has occured\n");
        return -1;
    }

    sCalc.calculate(structure);

    std::printf("name: %s; size: %d; field count: %d\n", structure->name.c_str(), structure->byteSize, structure->fieldCount);
    for (uint i = 0; i < structure->fieldCount; i++){
        if (structure->fields[i].type == engineStruct::cl_struct){
            std::printf(" %d) var: %s; type: %d; size: %d; struct: %s; amount: %d\n", i, structure->fields[i].name.c_str(), structure->fields[i].type, structure->fields[i].byteSize, structure->fields[i].subStructName.c_str(), structure->fields[i].arrSize);
        } else if (structure->fields[i].arrSize > 0){
            std::printf(" %d) var: %s; type: %d; size: %d; pointer: %d\n", i, structure->fields[i].name.c_str(), structure->fields[i].type, structure->fields[i].byteSize, structure->fields[i].arrSize);
        } else {
            std::printf(" %d) var: %s; type: %d; size: %d\n", i, structure->fields[i].name.c_str(), structure->fields[i].type, structure->fields[i].byteSize);
        }
    }

    delete structure;
    return 0;
}
