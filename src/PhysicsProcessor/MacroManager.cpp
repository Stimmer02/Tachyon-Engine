#include "MacroManager.h"

void MacroManager::addMacro(std::string keyword, float value){
    macros[keyword] = value;
}

float* MacroManager::getMacro(std::string keyword){
    if (macros.find(keyword) == macros.end()){
        return nullptr;
    }
    return &macros[keyword];
}

char MacroManager::parseFile(std::string filename){
    std::ifstream file(filename);
    if (!file.is_open()){
        error += "ERR: MacroManager::parseFile could not parse file" + filename +"\n";
        return -1;
    }

    std::string line;
    while (std::getline(file, line)){
        if (line[0] == '#'){
            continue;
        }
        size_t pos = line.find('=');
        if (pos == std::string::npos){
            continue;
        }
        std::string keyword = line.substr(0, pos);
        try {
            float value = std::stof(line.substr(pos + 1));
            addMacro(keyword, value);
        }
        catch (const std::invalid_argument&){
            error += "ERR: MacroManager::parseFile could not parse value " + line + "\n";
            return -1;
        }
    }

    file.close();
    return 0;
}

std::string MacroManager::getError(){
    return error;
}