#ifndef ISHAREABLE_H
#define ISHAREABLE_H

// TODO : replace commants with actual binds to specified classes
//#include "SharedNameResolver.h"

class IShareable {
public:
    /// @brief Method registers internal components into shared name resolver class
    virtual void Share( /*const SharedNameResolver * resolver*/) = 0; 
};

#endif