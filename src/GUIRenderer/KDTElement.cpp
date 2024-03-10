#include "KDTElement.h"
#include "Component.h"

KDTElement::KDTElement(){
    value = NULL;
    leftSon = NULL;
    rightSon = NULL;
}


KDTElement::KDTElement(Component* value){
    this->value = value;
    this->leftSon = NULL;
    this->rightSon = NULL;
}

KDTElement::~KDTElement(){
    value = NULL;
    if(leftSon != NULL){
        delete leftSon;
    }
    if(rightSon != NULL){
        delete rightSon;
    }
}

void KDTElement::setValue(Component* value){
    this->value = value;
}

void KDTElement::setLeftSon(KDTElement* leftSon){
    this->leftSon = leftSon;
}

void KDTElement::setRightSon(KDTElement* rightSon){
    this->rightSon = rightSon;
}

KDTElement* KDTElement::getLeftSon(){
    return leftSon;
}

KDTElement* KDTElement::getRightSon(){
    return rightSon;
}

Component* KDTElement::getValue(){
    return value;
}

bool KDTElement::comparatorXsmaller(KDTElement* &element, KDTElement* &other){
    return element->getValue()->getX() < other->getValue()->getX();
}

bool KDTElement::comparatorXsmallerOrEqueql(KDTElement* &element, KDTElement* &other){
    return element->getValue()->getX() <= other->getValue()->getX();
}

bool KDTElement::comparatorYsmaller(KDTElement* &element, KDTElement* &other){
    return element->getValue()->getX() < other->getValue()->getX();
}

bool KDTElement::comparatorYsmallerOrEqueql(KDTElement* &element, KDTElement* &other){
    return element->getValue()->getX() >= other->getValue()->getX();
}