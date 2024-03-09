#ifndef KDTELEMENT_H
#define KDTELEMENT_H

#include "Component.h"

class KDTElement{
private:
    Component* value;
    KDTElement* leftSon;
    KDTElement* rightSon;
public:
    KDTElement();
    KDTElement(Component* value);
    ~KDTElement();
    void setValue(Component* value);
    void setLeftSon(KDTElement* leftSon);
    void setRightSon(KDTElement* rightSon);
    KDTElement* getLeftSon();
    KDTElement* getRightSon();
    Component* getValue();
    static bool comparatorXbigger(KDTElement &element, KDTElement &other);
    static bool comparatorXbiggerOrEqueql(KDTElement &element, KDTElement &other);
    static bool comparatorYbigger(KDTElement &element, KDTElement &other);
    static bool comparatorYbiggerOrEqueql(KDTElement &element, KDTElement &other);
};

#endif
