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

        kernelQueue.push_back(kernelExecutionUnit(kernelDirectory + name, kernelPath, kernelExecutionCount));
    }

    file.close();
    return 0;
}

std::vector<kernelExecutionUnit>& KernelQueueBuilder::getKernelQueue(){
    return kernelQueue;
}

std::string KernelQueueBuilder::getError(){
    return error;
}