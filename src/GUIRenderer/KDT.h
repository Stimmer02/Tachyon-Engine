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
    void buildLeftSubTreeRec(KDTElement** elementsArray, KDTElement* subTreeRoot, const int &elementsArraySize, const bool xOrY);
    void buildRightSubTreeRec(KDTElement** elementsArray, KDTElement* subTreeRoot, const int &elementsArraySize, const bool xOrY);
    Component* find(const float &x, const float &y);
    Component* findRecX(const float &x, const float &y, KDTElement* element);
    Component* findRecY(const float &x, const float &y, KDTElement* element);
    void clear();
    void rebuild(const std::vector<Component*> &components);
};

#endif
