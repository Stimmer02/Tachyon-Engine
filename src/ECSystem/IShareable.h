#ifndef ISHAREABLE_H
#define ISHAREABLE_H

#include "SharedNameResolver.h"

class IShareable {
public:
    /// @brief Method registers internal components into shared name resolver class
    virtual void Share(SharedNameResolver * resourceManager) = 0;
};

#endif
