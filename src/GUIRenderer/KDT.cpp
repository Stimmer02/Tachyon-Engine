#include "KDT.h"
KDT::KDT(){
    root = NULL;
}

KDT::~KDT(){
    delete root;
}

void KDT::buildTree(const std::vector<Component*> &components){
    const int elementNum = components.size();
    int mid;
    KDTElement* elements[components.size()];
    for(int i = 0; i < elementNum; ++i){
        elements[i] = new KDTElement(components[i]);
    }
    std::sort(elements, elements + elementNum, KDTElement::comparatorXbigger);

    mid = elementNum / 2;
    root = elements[mid];

    buildTreeRec(elements, root, mid, false);
    buildTreeRec(elements + mid + 1, root, elementNum - mid - 1, false);
}

void KDT::buildTreeRec(KDTElement** elementsArray, KDTElement* subTreeRoot, const int &elementsArraySize, const bool xOrY){

}

Component* KDT::find(const int &x, const int &y){

}

void KDT::clear(){
    delete root;
}

void KDT::rebuild(const std::vector<Component*> &components){
    clear();
    buildTree(components);
}
