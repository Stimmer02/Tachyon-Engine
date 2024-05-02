#ifndef KDT_H
#define KDT_H

#include <vector>
#include <algorithm>
#include "KDTElement.h"

class KDT{
private:
    KDTElement* root;

    InteractiveElement* findRecX(const float &x, const float &y, KDTElement* element);
    InteractiveElement* findRecY(const float &x, const float &y, KDTElement* element);

    void buildLeftSubTreeRec(KDTElement** elementsArray, KDTElement* subTreeRoot, const int &elementsArraySize, const bool xOrY);
    void buildRightSubTreeRec(KDTElement** elementsArray, KDTElement* subTreeRoot, const int &elementsArraySize, const bool xOrY);
public:
    KDT();
    ~KDT();
    void buildTree(const std::vector<InteractiveElement*> &components);
    InteractiveElement* find(const float &x, const float &y);
    void clear();
    void rebuild(const std::vector<InteractiveElement*> &components);
};

#endif
