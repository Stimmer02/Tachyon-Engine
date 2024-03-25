#ifndef MACROMANAGER_H
#define MACROMANAGER_H

#include <map>
#include <string>
#include <fstream>

class MacroManager {
public:
    void addMacro(std::string keyword, float value);
    float* getMacro(std::string keyword);
    char parseFile(std::string filename);

private:
    std::map<std::string, float> macros;
};
#endif