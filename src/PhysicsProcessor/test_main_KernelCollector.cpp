#include "KernelQueueBuilder.h"
#include "KernelCollector.h"


int main(){
    KernelQueueBuilder kqb;
    KernelCollector kc;
    
    if (kqb.parseConfig("./config/kernel_sequence.cfg")){
        std::printf("%s\n", kqb.getError().c_str());
        return -1;
    }

    if (kqb.collectKernels(kc)){
        std::printf("%s\n", kqb.getError().c_str());
        return -1;
    }

    const std::vector<kernelExecutionUnit>& kernelQueue = kqb.getKernelQueue();
    for (auto &keu : kernelQueue){
        std::printf("name: %s\n", keu.name.c_str());
        std::printf("functionName: %s\n", keu.functionName.c_str());
        std::printf("path: %s\n", keu.path.c_str());
        std::printf("executionCount: %d\n\n", keu.executionCount);
    }
    
    std::printf("\n\n%s\n", kc.getKernels().c_str());
  
    return 0;
}
