#include "kernelFragment.h"

kernelFragment::kernelFragment(const int priority, const std::string rawKernel, std::string name){
    this->priority = priority;
    this->rawKernel = rawKernel;
    this->name = name;
}

kernelFragment::kernelFragment(const std::string filePath){
    std::ifstream file;
    file.open(filePath);
    file >> this->name >> this->priority;
    rawKernel.assign((std::istreambuf_iterator<char>(file)),(std::istreambuf_iterator<char>()));
    file.close();
}

bool kernelFragment::comp(const kernelFragment& a, const kernelFragment& b){
    if (a.priority < 0 && b.priority < 0){
        return a.priority < b.priority;
    } else if (a.priority < 0){
        return false;
    } else if (b.priority < 0){
        return true;
    } else {
        return a.priority < b.priority;
    }
}


