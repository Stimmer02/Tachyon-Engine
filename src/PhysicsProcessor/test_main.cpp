#include "StructTree.h"


int main(){
    MacroManager macroManager;
    SizeCalculator sCalc(8);
    ClStructParser clParser(&macroManager);

    if (macroManager.parseFile("/home/stimmer02/Documents/UMCS/SEMESTR_6/projetk_zespolowy/tachion-engine/config/macros.cfg")){
        std::fprintf(stderr, "Error has occured\n");
        return -1;
    }

    StructTree tree("/home/stimmer02/Documents/UMCS/SEMESTR_6/projetk_zespolowy/tachion-engine/engine_structs", "engineResources.clcpp", &clParser);
    tree.build(&clParser);
    tree.calculateSizes(&sCalc);
    tree.printTree();

    std::cout << '\n' << tree.getStructures() << std::endl;
  
    return 0;
}
