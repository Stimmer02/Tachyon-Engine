#ifndef IITERABLE_H
#define IITERABLE_H

template < class T >
class IIterable {
public:
    virtual bool HasMore() = 0;
    virtual T Next() = 0;
};

#endif
