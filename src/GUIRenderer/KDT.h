#ifndef KDT_H
#define KDT_H

#include <vector>
#include <algorithm>
#include "KDTElement.h"

class KDT{
private:
    KDTElement* root;
public:
    KDT();
    ~KDT();
    void buildTree(const std::vector<InteractiveElement*> &components);
    void buildLeftSubTreeRec(KDTElement** elementsArray, KDTElement* subTreeRoot, const int &elementsArraySize, const bool xOrY);
    void buildRightSubTreeRec(KDTElement** elementsArray, KDTElement* subTreeRoot, const int &elementsArraySize, const bool xOrY);
    InteractiveElement* find(const float &x, const float &y);
    InteractiveElement* findRecX(const float &x, const float &y, KDTElement* element);
    InteractiveElement* findRecY(const float &x, const float &y, KDTElement* element);
    void clear();
    void rebuild(const std::vector<InteractiveElement*> &components);
};

#endif
