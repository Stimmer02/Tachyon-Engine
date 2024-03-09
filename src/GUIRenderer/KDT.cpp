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
    std::sort(elements, elements + elementNum, KDTElement::comparatorXsmaller);

    mid = elementNum / 2;
    root = elements[mid];

    buildLeftSubTreeRec(elements, root, mid, false);
    buildRightSubTreeRec(elements + mid + 1, root, elementNum - mid - 1, false);
}

void KDT::buildLeftSubTreeRec(KDTElement** elementsArray, KDTElement* subTreeRoot, const int &elementsArraySize, const bool xOrY){

    if(elementsArraySize <= 1){
        return;
    }

    const int mid = elementsArraySize / 2;

    if(xOrY){
        std::sort(elementsArray, elementsArray + elementsArraySize, KDTElement::comparatorXsmaller);
    }
    else{
        std::sort(elementsArray, elementsArray + elementsArraySize, KDTElement::comparatorYsmaller);
    }
    subTreeRoot->setLeftSon(elementsArray[mid]);

    buildLeftSubTreeRec(elementsArray, subTreeRoot, mid, !xOrY);
    buildRightSubTreeRec(elementsArray + mid + 1, subTreeRoot, elementsArraySize - mid - 1, !xOrY);
}
void KDT::buildRightSubTreeRec(KDTElement** elementsArray, KDTElement* subTreeRoot, const int &elementsArraySize, const bool xOrY){

    if(elementsArraySize <= 1){
        return;
    }

    const int mid = elementsArraySize / 2;

    if(xOrY){
        std::sort(elementsArray, elementsArray + elementsArraySize, KDTElement::comparatorXsmaller);
    }
    else{
        std::sort(elementsArray, elementsArray + elementsArraySize, KDTElement::comparatorYsmaller);
    }
    subTreeRoot->setRightSon(elementsArray[mid]);

    buildLeftSubTreeRec(elementsArray, subTreeRoot, mid, !xOrY);
    buildRightSubTreeRec(elementsArray + mid + 1, subTreeRoot, elementsArraySize - mid - 1, !xOrY);
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
