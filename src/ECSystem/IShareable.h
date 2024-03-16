#ifndef ISHAREABLE_H
#define ISHAREABLE_H

// TODO : replace commants with actual binds to specified classes
//#include "SharedNameResolver.h"

class IShareable {
public:
    virtual void Share( /*const SharedNameResolver * resolver*/ ) = 0; 
};

#endif