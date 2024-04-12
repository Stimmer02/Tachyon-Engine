#ifndef SUBSTANCE_H
#define SUBSTANCE_H

#include <string>
#include <vector>

struct substance{
    std::string  name;
    uint id;
    std::vector<float> values;
};

#endif