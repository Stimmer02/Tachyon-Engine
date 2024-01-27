#include "StructCollector.h"
#include <algorithm>

char StructCollector::loadAllFromDirectory(const std::string directoryPath){
    bool fileFound = false;
    std::filesystem::path directory(directoryPath);
    if (std::filesystem::is_directory(directory) == false){
        std::fprintf(stderr, "ERROR: specified kernel structure directory does not exist:%s\n", directory.c_str());
        return 2;
    }
    std::vector<std::string> paths;
    for (std::filesystem::directory_entry dirEntry : std::filesystem::directory_iterator(directory)){
        const std::filesystem::path& extension = dirEntry.path().extension();
        if (dirEntry.is_regular_file() && ( extension.compare(".cl") == 0 || extension.compare(".clcpp") == 0)) {
            fileFound = true;
            paths.push_back(dirEntry.path().string());
        }
    }
    if (fileFound == false){
        std::fprintf(stderr, "WARNING: specified kernel structure directory does not contain any '.cl' or '.clcpp' files: %s\n", directory.c_str());
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
    sources.push_back({this->structures.c_str(), this->structures.length()});
    return sources;
}
void StructCollector::get(cl::Program::Sources& sources){
    sources.push_back({this->structures.c_str(), this->structures.length()});
}
