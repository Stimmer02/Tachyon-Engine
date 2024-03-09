#ifndef KDT_H
#define KDT_H

#include <vector>
#include <algorithm>
#include "Component.h"
#include "KDTElement.h"

class KDT{
private:
    KDTElement* root;
public:
    KDT();
    ~KDT();
    void buildTree(const std::vector<Component*> &components);
    void buildTreeRec(KDTElement** elementsArray, KDTElement* subTreeRoot, const int &elementsArraySize, const bool xOrY);
    Component* find(const int &x, const int &y);
    void clear();
    void rebuild(const std::vector<Component*> &components);
};

#endif
