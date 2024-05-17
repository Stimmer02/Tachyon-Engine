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

    std::vector<std::string> names = substCollector.getSubstanceNames();
    std::printf("Substances:\n");
    for (std::string name : names){
        std::printf("%s\n", name.c_str());
    }

    const std::vector<substance>& substances = substCollector.getSubstances();
    std::printf("\nSubstances:\n");
    for (substance subst : substances){
        subst.print();
        std::printf("\n");
    }

    const std::vector<substanceField>& properties = substCollector.getSubstancePhroperties();
    std::printf("\nSubstance properties:\n");
    for (substanceField prop : properties){
        std::printf("name: %s\n", prop.name.c_str());
        std::printf("type: %s\n", SubstanceCollector::clTypeToString(prop.type).c_str());
        std::printf("defaultValue: %.2f\n", prop.defaultValue);
        std::printf("\n");
    }
    return 0;
}