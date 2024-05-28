#include "KDT.h"
#include "Component.h"
#include "KDTElement.h"
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
    
	//Creating all nodes before adding them to The KDTree
	for(int i = 0; i < elementNum; ++i){
        elements[i] = new KDTElement(components[i]);
    }
	
	//sorting all nodes because it helps to chose the best root
    std::sort(elements, elements + elementNum, KDTElement::comparatorXsmaller);

	//chosing the best root
    mid = elementNum / 2;
    root = elements[mid];

	//Recursion bulding left and right subtrees
    buildLeftSubTreeRec(elements, root, mid, false);
    buildRightSubTreeRec(elements + mid + 1, root, elementNum - mid - 1, false);
}

void KDT::buildLeftSubTreeRec(KDTElement** elementsArray, KDTElement* subTreeRoot, const int &elementsArraySize, const bool xOrY){

	//checking if there is 0 elements in this subtree and stop if it's true
    if(elementsArraySize <= 1){
        return;
    }

    const int mid = elementsArraySize / 2;

	//chose X or Y axis for comparing Nodes
    if(xOrY){
        std::sort(elementsArray, elementsArray + elementsArraySize, KDTElement::comparatorXsmaller);
    }
    else{
        std::sort(elementsArray, elementsArray + elementsArraySize, KDTElement::comparatorYsmaller);
    }
	//set root of left subtree
    subTreeRoot->setLeftSon(elementsArray[mid]);

	//recursion build left and right subtrees
    buildLeftSubTreeRec(elementsArray, subTreeRoot, mid, !xOrY);
    buildRightSubTreeRec(elementsArray + mid + 1, subTreeRoot, elementsArraySize - mid - 1, !xOrY);
}
void KDT::buildRightSubTreeRec(KDTElement** elementsArray, KDTElement* subTreeRoot, const int &elementsArraySize, const bool xOrY){

	//checking if there is 0 elements in this subtree and stop if it's true
    if(elementsArraySize <= 1){
        return;
    }

    const int mid = elementsArraySize / 2;

	//chose X or Y axis for comparing Nodes
    if(xOrY){
        std::sort(elementsArray, elementsArray + elementsArraySize, KDTElement::comparatorXsmaller);
    }
    else{
        std::sort(elementsArray, elementsArray + elementsArraySize, KDTElement::comparatorYsmaller);
    }
	//set root of right subtree
    subTreeRoot->setRightSon(elementsArray[mid]);

	//recursion build left and right subtrees
    buildLeftSubTreeRec(elementsArray, subTreeRoot, mid, !xOrY);
    buildRightSubTreeRec(elementsArray + mid + 1, subTreeRoot, elementsArraySize - mid - 1, !xOrY);
}

Component* KDT::find(const float &x, const float &y){
	//look for component where the x and y coordinates are located
    if(root == NULL){
        return NULL;
    }
    return findRecX(x, y, root);
}

Component* KDT::findRecX(const float &x, const float &y, KDTElement* element){
	//this function chose left or right subtree, basing on X coordinate
    //return NULL if there is no such component where the x and y coordinates are located
	if(element == NULL){
        return NULL;
    }
	
	//compare component's coordinates with x and y value;
    Component* helper = element->getValue();
    if(helper->getX() <= x){
        if(x <= helper->getX() + helper->getWidth()){
            if(helper->getY() <= y && y <= helper->getY() + helper->getHeight()){
				//return component if the x and y coordinates are located inside them
                return helper;
            }
            else{
				//look for component in rigth subtree
                return findRecY(x, y, element->getRightSon());
            }
        }
        else{
			//look for component in rigth subtree
            return findRecY(x, y, element->getRightSon());
        }
    }
    else{
		//look for component in left subtree
        return findRecY(x, y, element->getLeftSon());
    }
}

Component* KDT::findRecY(const float &x, const float &y, KDTElement* element){
	//this function chose left or right subtree, basing on Y coordinate
    //return NULL if there is no such component where the x and y coordinates are located
    if(element == NULL){
        return NULL;
    }

	//compare component's coordinates with x and y value;
    Component* helper = element->getValue();
    if(helper->getY() <= y){
        if(x <= helper->getY() + helper->getHeight()){
            if(helper->getX() <= x && x <= helper->getX() + helper->getWidth()){
				//return component if the x and y coordinates are located inside them
                return helper;
            }
            else{
				//look for component in rigth subtree
                return findRecX(x, y, element->getRightSon());
            }
        }
        else{
				//look for component in rigth subtree
            return findRecX(x, y, element->getRightSon());
        }
    }
    else{
		//look for component in left subtree
        return findRecX(x, y, element->getLeftSon());
    }
}

void KDT::clear(){
	//clear KDT;
    delete root;
}

void KDT::rebuild(const std::vector<Component*> &components){
	//delete all nodes of a tree and create the new one
    clear();
    buildTree(components);
}
