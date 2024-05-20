#include "KernelCollector.h"

KernelCollector::KernelCollector(){
    error = "";
}

char KernelCollector::addKernel(std::string path, std::string& kernelName){
    std::ifstream file(path);
    if (!file.is_open()){
        error = "ERR: KernelCollector::addKernel could not open file " + path + "\n";
        return -1;
    }

    std::string kernel = "";
    std::string line;
    while (std::getline(file, line)){
        kernel += line + "\n";
    }

    if (checkKernel(&kernel, kernelName)){
        error = "ERR: KernelCollector::addKernel kernel " + path + " has invalid signature\n";
        return -1;
    }

    kernels.push_back(kernelData(kernel, kernelName));

    file.close();
    return 0;
}

std::string KernelCollector::getKernels(){
    std::string kernelsCode = "";
    for (auto &kernel : kernels){
        kernelsCode += kernel.kernel;
    }

    return kernelsCode;
}

std::string KernelCollector::getError(){
    return error;
}

char KernelCollector::checkKernel(std::string* kernel, std::string& kernelName){
    std::string signature1 = "void kernel ";
    std::string signature2 = "(global struct engineConfig* config, global struct engineResources* resources)";
    size_t pos = kernel->find(signature1);
    if (pos == std::string::npos){
        error += "ERR: KernelCollector:checkKernel signature not found: " + signature1 + "\n";
        return -1;
    }

    pos += signature1.size();
    size_t end = kernel->find(signature2);
    if (end == std::string::npos){
        error += "ERR: KernelCollector:checkKernel signature not found: " + signature2 + "\n";
        return -1;
    }

    kernelName = kernel->substr(pos, end - pos);
    return 0;
}