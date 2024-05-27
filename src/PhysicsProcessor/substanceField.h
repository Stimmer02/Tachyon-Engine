#ifndef SUBSTANCEFIELD_H
#define SUBSTANCEFIELD_H

#include <string>
#include "engineStruct.h"

struct substanceField{
    std::string name;
    engineStruct::cl_type type;
    float defaultValue;
};

#endif