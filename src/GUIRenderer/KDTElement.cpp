#include "KDTElement.h"

KDTElement::KDTElement(){
    value = NULL;
    leftSon = NULL;
    rightSon = NULL;
}


KDTElement::KDTElement(InteractiveElement * value){
    this->value = value;
    this->leftSon = NULL;
    this->rightSon = NULL;
}

KDTElement::~KDTElement(){
	//recursive delete nodes;
    value = NULL;
    if(leftSon != NULL){
        delete leftSon;
    }
    if(rightSon != NULL){
        delete rightSon;
    }
}

void KDTElement::setValue(InteractiveElement * value){
    this->value = value;
}

void KDTElement::setLeftSon(KDTElement * leftSon){
    this->leftSon = leftSon;
}

void KDTElement::setRightSon(KDTElement * rightSon){
    this->rightSon = rightSon;
}

KDTElement* KDTElement::getLeftSon(){
    return leftSon;
}

KDTElement* KDTElement::getRightSon(){
    return rightSon;
}

InteractiveElement * KDTElement::getValue(){
    return value;
}

bool KDTElement::comparatorXsmaller(KDTElement* &element, KDTElement* &other){
	//compare 2 KDTElement and return true if first has smaller X coordinate
    if(element->getValue()->transform.position.x == other->getValue()->transform.position.x){
        element->getValue()->transform.position.y < other->getValue()->transform.position.y;
    }
    return element->getValue()->transform.position.x < other->getValue()->transform.position.x;
}

bool KDTElement::comparatorXsmallerOrEqueql(KDTElement* &element, KDTElement* &other){
	//compare 2 KDTElement and return true if first hasn't bigger X coordinate
    return element->getValue()->transform.position.x <= other->getValue()->transform.position.x;
}

bool KDTElement::comparatorYsmaller(KDTElement* &element, KDTElement* &other){
	//compare 2 KDTElement and return true if first has smaller Y coordinate
    if(element->getValue()->transform.position.y == other->getValue()->transform.position.y){
        element->getValue()->transform.position.x < other->getValue()->transform.position.x;
    }
    return element->getValue()->transform.position.y < other->getValue()->transform.position.y;
}

bool KDTElement::comparatorYsmallerOrEqueql(KDTElement* &element, KDTElement* &other){
	//compare 2 KDTElement and return true if first hasn't bigger Y coordinate
    return element->getValue()->transform.position.y >= other->getValue()->transform.position.y;
}
