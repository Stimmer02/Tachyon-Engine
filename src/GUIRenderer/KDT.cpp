#include "KDT.h"
KDT::KDT(){
    root = NULL;
}

KDT::~KDT(){
    delete root;
}

void KDT::buildTree(const std::vector<Component*> &components){

}

void KDT::buildTreeRec(KDTElement* elementsArray, KDTElement subTreeRoot, const int &elementsArraySize){

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
