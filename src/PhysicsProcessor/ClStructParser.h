#ifndef CL_STRUCT_SIZE_CALLCULATOR_H
#define CL_STRUCT_SIZE_CALLCULATOR_H

#include "engineStruct.h"
#include "MacroManager.h"
#include "engineStruct.h"

#include <vector>
#include <map>
#include <cstring>
#include <stdexcept>



class ClStructParser{
public:
    ClStructParser(MacroManager* macroManager);
    engineStruct* processStruct(std::string structCode);

private:
    engineStruct::cl_type stringToClType(const char* word);

    MacroManager* macroManager;
};


#endif
