#ifndef SUBSTANCECOLLECTOR_H
#define SUBSTANCECOLLECTOR_H

#include "substance.h"
#include "substanceField.h"

#include <vector>

class SubstanceCollector{
public:
    char loadFromFile(std::string path);
    char loadFromDirecotry(std::string path);

    std::string createSubstanceClStruct();
    std::vector<std::string> getSubstanceNames();

private:
    std::vector<substance> substanceTable;
    std::vector<substanceField> substanceFields;

    void updateSubstances();
};

#endif