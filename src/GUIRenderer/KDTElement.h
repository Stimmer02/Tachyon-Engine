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
    static bool comparatorXsmaller(KDTElement &element, KDTElement &other);
    static bool comparatorXsmallerOrEqueql(KDTElement &element, KDTElement &other);
    static bool comparatorYsmaller(KDTElement &element, KDTElement &other);
    static bool comparatorYsmallerOrEqueql(KDTElement &element, KDTElement &other);
};

#endif
