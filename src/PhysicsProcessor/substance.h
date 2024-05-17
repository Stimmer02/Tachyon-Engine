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

    void print(){
        std::printf("id: %d\n", id);
        std::printf("name: %s\n", name.c_str());
        std::printf("RGB: %.2f, %.2f, %.2f\n", R, G, B);
        std::printf("movable: %d\n", movable);
        std::printf("values: ");
        for(auto& value : values){
            std::printf(" %.2f ", value);
        }
        std::printf("\n");
    }
};

#endif