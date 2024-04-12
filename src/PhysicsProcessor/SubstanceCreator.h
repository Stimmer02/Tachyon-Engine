#ifndef SUBSTANCECREATOR_H
#define SUBSTANCECREATOR_H

#include <string>
#include <vector>

#include "substanceField.h"
#include "substance.h"

class SubstanceCreator{
public:
    std::string createSubstanceClStruct();
    void loadFromDir(std::string path);
    char addFromFile(std::string path);
    std::vector<std::string> getSubstanceNames();

private:
    std::vector<substanceField> fields;
    std::vector<substance> substanceTable;
    
    void updateSubstances();
};

#endif