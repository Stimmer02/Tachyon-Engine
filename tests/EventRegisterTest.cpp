#include "gtest/gtest.h"
#include <cassert>
#include <functional>
#include <cstring>

#include "EventRegister.h"

TEST(EventRegisterTest, SimpleTest){
    EventRegister *ER;
    ER = new EventRegister("xlog.txt");
    for(unsigned int i = 0; i < 10000; ++i){
        ER->Write(M_INFO, "%u", i);
    }
    ER->Write(M_INFO, "abc%u", (unsigned int)123);
    ER->Flush();
    delete ER;
}

