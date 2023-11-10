#include "KernelBuilder.h"

char KernelBuilder::loadAllFromDirectory(const std::string direcory){
    bool fileFound = false;
    for (std::filesystem::directory_entry dirEntry : std::filesystem::directory_iterator(direcory)){
        if(dirEntry.is_regular_file() && dirEntry.path().extension().compare(".pykn") == 0){
            fileFound = true;
            this->addKernelFragment(dirEntry.path().string());
            std::printf("kernel found: %s\n", dirEntry.path().string().c_str());
        }
    }
    if (fileFound == false){
        std::fprintf(stderr, "WARNING: specified kernel fragmet directory does not contain any '.pykn' files: %s\n", direcory.c_str());
        return 1;
    }
    std::printf("\n");
    return 0;
}

char KernelBuilder::addKernelFragment(const kernelFragment& kernelFragment){
    this->fragments.push_back(kernelFragment);
    return 0;
}

char KernelBuilder::addKernelFragment(const std::string filePath){
    this->fragments.push_back(kernelFragment(filePath));
    return 0;
}

char KernelBuilder::validateLoadedFragments(std::string& outMessage){
    char returnCode = 0;
    outMessage.clear();
    if (fragments.empty()){
        outMessage = "[Error 0xf] No fragents found";
        return 0xf;
    }

    std::vector<kernelFragment*>* usedPriorities = new std::vector<kernelFragment*>[fragments.size()];
    uint prioritiesCount = 0;
    bool kernelHeader = false;
    bool kernelEnd = false;
    bool repeat;


    for (kernelFragment& fragment : fragments){
        repeat = false;

        if (fragment.priority == 0){
            kernelHeader = true;
        }  else if (fragment.priority == -1){
            kernelEnd = true;
        }

        for (uint j = 0; j < prioritiesCount; j++){
            if (usedPriorities[j].at(0)->priority == fragment.priority){
                repeat = true;
                usedPriorities[j].push_back(&fragment);
                break;
            }
        }
        if (repeat == false){
            usedPriorities[prioritiesCount].push_back(&fragment);
            prioritiesCount++;
        }
    }

    if (kernelHeader == false){
        returnCode |= 0x1;
        outMessage += "[Error 0x1] Priority violation (0): no fragment found\n";
    }

    if (kernelEnd == false){
        returnCode |= 0x2;
        outMessage += "[Error 0x2] Priority violation (-1): no fragment found\n";
    }

    for (uint i = 0; i < prioritiesCount; i++){
        if (usedPriorities[i].size() > 1){
            if (usedPriorities[i].front()->priority == 0){
                returnCode |= 0x4;
                outMessage += "[Error 0x4] Priority violation (0): ";
            } else if (usedPriorities[i].front()->priority == -1){
                returnCode |= 0x8;
                outMessage += "[Error 0x8] Priority violation (-1): ";
            } else {
                outMessage += "[Warning] Priority duplication (" + std::to_string(usedPriorities[i].front()->priority) + "): ";
            }
            for (kernelFragment* fragment : usedPriorities[i]){
                outMessage += fragment->name + ", ";
            }
            outMessage += '\n';
        }
    }

    delete[] usedPriorities;

    if (outMessage.empty()){
        outMessage += "No violations found\n";
    }

    return returnCode;
}

std::string KernelBuilder::to_string(){
    std::string out;
    std::sort(fragments.begin(), fragments.end(), kernelFragment::comp);

    for (kernelFragment& fragent : fragments){
        out += "//" + std::to_string(fragent.priority) + "   " + fragent.name + "\n" + fragent.rawKernel + "\n";
    }

    return out;
}

std::string KernelBuilder::build(cl::Program::Sources& sources){
    std::string message = "Kernel fragments loadded in order:\n";
    std::sort(fragments.begin(), fragments.end(), kernelFragment::comp);

    for (kernelFragment& fragent : fragments){
        sources.push_back(fragent.rawKernel);
        message += "   " + std::to_string(fragent.priority) + "\t" + fragent.name + "\n";
    }

    return message;
}

