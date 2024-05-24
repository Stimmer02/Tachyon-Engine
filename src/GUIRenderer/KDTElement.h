#ifndef KDTELEMENT_H
#define KDTELEMENT_H

#include "InteractiveElement.h"

class KDTElement{
private:
    InteractiveElement * value;
    KDTElement* leftSon;
    KDTElement* rightSon;
public:
    KDTElement();
    KDTElement(InteractiveElement* value);
    ~KDTElement();
    void setValue(InteractiveElement* value);
    void setLeftSon(KDTElement* leftSon);
    void setRightSon(KDTElement* rightSon);
    KDTElement* getLeftSon();
    KDTElement* getRightSon();
    InteractiveElement* getValue();
    static bool comparatorXsmaller(KDTElement* &element, KDTElement* &other);
    static bool comparatorXsmallerOrEqueql(KDTElement* &element, KDTElement* &other);
    static bool comparatorYsmaller(KDTElement* &element, KDTElement* &other);
    static bool comparatorYsmallerOrEqueql(KDTElement* &element, KDTElement* &other);
};

#endif
