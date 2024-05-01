#include "gtest/gtest.h"
#include <cassert>
#include <functional>
#include <cstring>

#include "EventRegister.h"

TEST(EventRegisterTest, SimpleTest){
    EventRegister *ER;
    ER = new EventRegister("xlog.txt");
    ER->Write(M_INFO, "abc%u", (unsigned int)5);
    ER->Flush();
    delete ER;
}
