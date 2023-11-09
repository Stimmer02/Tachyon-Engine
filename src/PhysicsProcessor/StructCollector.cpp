#include "StructCollector.h"

char StructCollector::loadAllFromDirectory(const std::string direcory){
    bool fileFound = false;
    for (std::filesystem::directory_entry dirEntry : std::filesystem::directory_iterator(direcory)){
        const std::filesystem::path& extension = dirEntry.path().extension();
        if(dirEntry.is_regular_file() && (extension.compare("cl") == 0 || extension.compare("clcpp") == 0)) {
            fileFound = true;
            this->addStruct(dirEntry.path());
        }
    }
    if (fileFound == false){
        std::fprintf(stderr, "WARNING: specified kernel fragmet directory does not contain any '.pykn' files: %s\n", direcory.c_str());
        return 1;
    }
    return 0;
}

char StructCollector::addStruct(const std::string filePath){
    std::ifstream file;
    file.open(filePath);
    if(file.fail()){
        return 1;
    }
    structures.assign((std::istreambuf_iterator<char>(file)),(std::istreambuf_iterator<char>()));
    file.close();

    return 0;
}

cl::Program::Sources StructCollector::get(){
    cl::Program::Sources sources;
    sources.push_back(this->structures);
    return sources;
}
void StructCollector::get(cl::Program::Sources& sources){
    sources.push_back(this->structures);
}
