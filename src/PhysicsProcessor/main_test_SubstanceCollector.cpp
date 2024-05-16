#include "SubstanceCollector.h"

int main(){
    SubstanceCollector substCollector;
    if (substCollector.parseConfig("./config/substances_to_load.cfg") == 0){
        std::printf("Substances loaded successfully\n");
    } else {
        std::printf("Failed to load substances\n");
        std::printf("%s\n", substCollector.getError().c_str());
        return 1;
    }
    
    std::printf("\nCL substance structure:\n%s\n\n", substCollector.createSubstanceClStruct().c_str());

    // std::vector<std::string> names = substCollector.getSubstanceNames();
    // std::printf("Substances:\n");
    // for (std::string name : names){
    //     std::printf("%s\n", name.c_str());
    // }
    return 0;
}