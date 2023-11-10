#include "StructCollector.h"
#include <algorithm>

char StructCollector::loadAllFromDirectory(const std::string direcory){
    bool fileFound = false;
    std::vector<std::string> paths;
    for (std::filesystem::directory_entry dirEntry : std::filesystem::directory_iterator(direcory)){
        const std::filesystem::path& extension = dirEntry.path().extension();
        if (dirEntry.is_regular_file() && (extension.compare(".cl") == 0 || extension.compare(".clcpp") == 0)) {
            fileFound = true;
            paths.push_back(dirEntry.path().string());
        }
    }
    if (fileFound == false){
        std::fprintf(stderr, "WARNING: specified kernel fragmet directory does not contain any '.pykn' files: %s\n", direcory.c_str());
        return 1;
    }
    std::sort(paths.begin(), paths.end());

    for (std::string& path : paths){
        this->addStruct(path);
        std::printf("structure found: %s\n", path.c_str());
    }

    std::printf("\n");
    return 0;
}

char StructCollector::addStruct(const std::string filePath){
    std::ifstream file;
    file.open(filePath);
    if(file.fail()){
        return 1;
    }
    structures.append((std::istreambuf_iterator<char>(file)),(std::istreambuf_iterator<char>()));
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
