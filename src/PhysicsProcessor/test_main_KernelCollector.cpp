#include "KernelQueueBuilder.h"


int main(){
    KernelQueueBuilder kqb;
    system("pwd");
    if (kqb.parseConfig("./config/kernel_sequence.cfg")){
        std::printf("%s\n", kqb.getError().c_str());
        return -1;
    }

    std::vector<kernelExecutionUnit>& kernelQueue = kqb.getKernelQueue();
    for (auto &keu : kernelQueue){
        std::printf("name: %s\n", keu.name.c_str());
        std::printf("path: %s\n", keu.path.c_str());
        std::printf("executionCount: %d\n\n", keu.executionCount);
    }
        
  
    return 0;
}
