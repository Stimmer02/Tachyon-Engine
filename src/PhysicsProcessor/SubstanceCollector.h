#ifndef SUBSTANCECOLLECTOR_H
#define SUBSTANCECOLLECTOR_H

#include "substance.h"
#include "substanceField.h"
#include "engineStruct.h"

#include <vector>
#include <fstream>
#include <map>

class SubstanceCollector{
public:
    SubstanceCollector();

    char parseConfig(std::string path);
    char loadPropertiesFromFile(std::string path);
    char loadSubstancesFromFile(std::string path);

    std::string createSubstanceClStruct();
    std::vector<std::string> getSubstanceNames();
    const std::vector<substance>& getSubstances();
    const std::vector<substanceField>& getSubstancePhroperties();

    std::string getError();

    static engineStruct::cl_type stringToClType(std::string type);
    static std::string clTypeToString(engineStruct::cl_type type);

private:
    std::vector<substance> substanceTable;
    std::vector<substanceField> substancePhroperties;

    std::string error;

    uint idCount;

    // void updateSubstances(); may turn useful if working with multiple files
    void setDefaultPhroperties(substance& substance);
    void loadMandatorySubstances();
    void giveIdToSubstances(substance& substance); //mostly a placeholder for future implementation
};

#endif