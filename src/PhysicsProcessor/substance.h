#ifndef SUBSTANCE_H
#define SUBSTANCE_H

#include <string>
#include <vector>

struct substance{
    uint id;
    std::string  name;
    float R, G, B;
    bool movable;
    std::vector<float> values;
};

#endif