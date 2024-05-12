#include "SubstanceCollector.h"

int main(){
    SubstanceCollector substCollector;
    if (substCollector.loadFromFile("substances.json")){
        std::printf("Substances loaded successfully\n");
    } else {
        std::printf("Failed to load substances\n");
        return 1;
    }
    
    std::printf("\nCL substance structure:\n%s\n\n", substCollector.createSubstanceClStruct());

    std::vector<std::string> names = substCollector.getSubstanceNames();
    std::printf("Substances:\n");
    for (std::string name : names){
        std::printf("%s\n", name.c_str());
    }
    return 0;
}