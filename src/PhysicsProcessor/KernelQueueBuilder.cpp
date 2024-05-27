#include "KernelQueueBuilder.h"


KernelQueueBuilder::KernelQueueBuilder(){
    error = "";
}

char KernelQueueBuilder::parseConfig(std::string path){
    std::ifstream file(path);
    if (!file.is_open()){
        error += "ERR: KernelQueueBuilder::parseConfig - file '" + path + "' not found\n";
        return -1;
    }

    std::string line;
    std::string kernelDirectory = "";

    while (std::getline(file, line)){
        if (line[0] == '#' || line.empty()){
            continue;
        }

        std::string name;
        std::string kernelPath;
        uint kernelExecutionCount;

        std::size_t position1 = line.find("=");
        if (position1 == std::string::npos){
            continue;
        }
        name = line.substr(0, position1);

        if (name == "KERNEL_DIRECTORY"){
            kernelDirectory = line.substr(position1+1);
            continue;
        }

        std::size_t position2 = line.find(",");
        if (position2 == std::string::npos){
            kernelExecutionCount = 1;
            position2 = position1;
        } else {
            try{
                kernelExecutionCount = std::stoi(line.substr(position1+1, position2));
            } catch (std::invalid_argument){
                error += "ERR: KernelQueueBuilder::parseConfig - invalid argument in line '" + line + "'\n";
                exit(-1);
            }
        }

        kernelPath = line.substr(position2+1);

        kernelQueue.push_back(kernelExecutionUnit(name, kernelDirectory + kernelPath, kernelExecutionCount));
    }

    file.close();
    return 0;
}

char KernelQueueBuilder::collectKernels(KernelCollector& kc){
    for (auto &keu : kernelQueue){
        if (kc.addKernel(keu.path, keu.functionName)){
            error += "ERR: KernelQueueBuilder::collectKernels could not continue due to following error - " + kc.getError() + "\n";
            return -1;
        }
    }
    return 0;
}

const std::vector<kernelExecutionUnit>& KernelQueueBuilder::getKernelQueue(){
    return kernelQueue;
}

uint KernelQueueBuilder::getKernelQueueSize(){
    uint size = 0;
    for (auto &keu : kernelQueue){
        size += keu.executionCount;
    }

    return size;
}

std::string KernelQueueBuilder::getError(){
    return error;
}