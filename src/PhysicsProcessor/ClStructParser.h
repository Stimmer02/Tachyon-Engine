#ifndef CL_STRUCT_SIZE_CALLCULATOR_H
#define CL_STRUCT_SIZE_CALLCULATOR_H

#include "engineStruct.h"
#include "MacroManager.h"
#include <vector>
#include <map>
#include <cstring>
#include <stdexcept>



class ClStructParser{
public:
    engineStruct* processStruct(std::string structCode, MacroManager& macroManager);

private:
    engineStruct::cl_type stringToClType(const char* word);
};


#endif
