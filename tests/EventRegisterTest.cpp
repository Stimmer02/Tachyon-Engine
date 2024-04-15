#include "gtest/gtest.h"
#include <cassert>
#include <functional>
#include <cstring>

#include "EventRegister.h"

TEST(EventRegisterTest, SimpleTest){
    EventRegister *ER;
    ER = new EventRegister();
    ER->Write(INFO, "abc%u", (unsigned int)5);
    ER->Flush();
    delete ER;
}
