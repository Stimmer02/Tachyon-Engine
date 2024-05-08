#include "KDT.h"

KDT::KDT(){
    root = NULL;
}

KDT::~KDT(){
    delete root;
}


#ifdef DEBUG
    // Only for debug purpose
    void Traverse(KDTElement * element){

        if( element == nullptr)
            return;

        Traverse(element->getLeftSon());

        Transform & transform = element->getValue()->transform;

        fprintf(stdout, "Element at position : %f %f\n", transform.position.x, transform.position.y);

        Traverse(element->getRightSon());

    }
#endif

void KDT::buildTree(const std::vector<InteractiveElement*> &components){
    const int elementNum = components.size();
    int mid, b, e, avg;
    KDTElement ** elements = new KDTElement*[components.size()];

	//Creating all nodes before adding them to The KDTree
	for(int i = 0; i < elementNum; ++i){
        elements[i] = new KDTElement(components[i]);
    }

	//sorting all nodes because it helps to chose the best root
    std::sort(elements, elements + elementNum, KDTElement::comparatorXsmaller);
    
	//chosing the best root
    mid = binSearchMiddleKDTElement(elements, elementNum, true);
      
   
    root = elements[mid];

	//Recursion bulding left and right subtrees
    buildLeftSubTreeRec(elements, root, mid, false);
    buildRightSubTreeRec(elements + mid + 1, root, elementNum - mid - 1, false);
    
    printTree(root);
    
#ifdef DEBUG
    Traverse(root);
#endif
}

void KDT::buildLeftSubTreeRec(KDTElement** elementsArray, KDTElement* subTreeRoot, const int &elementsArraySize, const bool xOrY){

	//checking if there is 0 elements in this subtree and stop if it's true
    if(elementsArraySize == 0){
        return;
    }
    if(elementsArraySize == 1){
        subTreeRoot->setLeftSon(elementsArray[0]);
        return;
    }
    
    int b, e, avg;
    int mid;
    
	//chose X or Y axis for comparing Nodes
    if(xOrY){
        std::sort(elementsArray, elementsArray + elementsArraySize, KDTElement::comparatorXsmaller);
    }
    else{
        std::sort(elementsArray, elementsArray + elementsArraySize, KDTElement::comparatorYsmaller);
    }
    
    mid = binSearchMiddleKDTElement(elementsArray, elementsArraySize, xOrY);
    

    
	//set root of left subtree
    subTreeRoot->setLeftSon(elementsArray[mid]);

	// //recursion build left and right subtrees
    buildLeftSubTreeRec(elementsArray, subTreeRoot->getLeftSon(), mid, !xOrY);
    buildRightSubTreeRec(elementsArray + mid + 1, subTreeRoot->getLeftSon(), elementsArraySize - mid - 1, !xOrY);
}

void KDT::buildRightSubTreeRec(KDTElement** elementsArray, KDTElement* subTreeRoot, const int &elementsArraySize, const bool xOrY){

	//checking if there is 0 elements in this subtree and stop if it's true
    if(elementsArraySize == 0){
        return;
    }
    if(elementsArraySize == 1){
        subTreeRoot->setRightSon(elementsArray[0]);
        return;
    }

    int e, b, avg;
    int mid;

	//chose X or Y axis for comparing Nodes
    if(xOrY){
        std::sort(elementsArray, elementsArray + elementsArraySize, KDTElement::comparatorXsmaller);
    }
    else{
        std::sort(elementsArray, elementsArray + elementsArraySize, KDTElement::comparatorYsmaller);
    }
    
    mid = binSearchMiddleKDTElement(elementsArray, elementsArraySize, xOrY);
    
	//set root of right subtree
    subTreeRoot->setRightSon(elementsArray[mid]);

	//recursion build left and right subtrees
    buildLeftSubTreeRec(elementsArray, subTreeRoot->getRightSon(), mid, !xOrY);
    buildRightSubTreeRec(elementsArray + mid + 1, subTreeRoot->getRightSon(), elementsArraySize - mid - 1, !xOrY);
}

InteractiveElement* KDT::find(const float &x, const float &y){
	//look for component where the x and y coordinates are located
    return findRecX(x, y, root);
}

InteractiveElement* KDT::findRecX(const float &x, const float &y, KDTElement* element){
   //this function chose left or right subtree, basing on X coordinate
    //return NULL if there is no such component where the x and y coordinates are located
	if(element == NULL){
        return NULL;
    }
    
	//compare component's coordinates with x and y value;
    InteractiveElement* helper = element->getValue();
    float & xH = helper->transform.position.x;
    float & yH = helper->transform.position.y;

    if(xH <= x){
        if(x <= xH + helper->width){
            if(yH <= y && y <= yH + helper->height){
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

InteractiveElement* KDT::findRecY(const float &x, const float &y, KDTElement* element){
    
	//this function chose left or right subtree, basing on Y coordinate
    //return NULL if there is no such component where the x and y coordinates are located
    if(element == NULL){
        return NULL;
    }
    InteractiveElement* helperElement;
    //compare component's coordinates with x and y value;
    InteractiveElement* helper = element->getValue();
    float & xH = helper->transform.position.x;
    float & yH = helper->transform.position.y;

    if(yH <= y){
        if(y <= yH + helper->height){
            if(xH <= x && x <= xH + helper->width){
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

void KDT::rebuild(const std::vector<InteractiveElement*> &components){
	//delete all nodes of a tree and create the new one
    clear();
    buildTree(components);
}

void KDT::printTree(KDTElement *subRoot){
    if(subRoot == NULL){
        return;
    }
    if(subRoot->getLeftSon() != NULL){
        printTree(subRoot->getLeftSon());
    }
    if(subRoot->getRightSon() != NULL){
        printTree(subRoot->getRightSon());
    }
}

int KDT::binSearchMiddleKDTElement(KDTElement **elementsArray, const int &elementsArraySize, const bool &xOrY){
    int mid, b, e, avg, x, y;
    mid = elementsArraySize / 2;
    
    b = 0;
    e = mid;
    avg = (b + e) / 2;
    
    if(xOrY){
        x = elementsArray[mid]->getValue()->transform.position.x;
        while(b + 1 != e && b != e){
            if(elementsArray[avg]->getValue()->transform.position.x == x){
                e = avg;
            }
            else{
                b = avg;
            }
            avg = (e + b) / 2;
        }
        if(elementsArray[b]->getValue()->transform.position.x == x){
            return b;
        }
        else{
            return e;
        }
    }
    else{
        y = elementsArray[mid]->getValue()->transform.position.y;
        while(b + 1 != e && b != e){
            if(elementsArray[avg]->getValue()->transform.position.y == y){
                e = avg;
            }
            else{
                b = avg;
            }
            avg = (e + b) / 2;
        }
        if(elementsArray[b]->getValue()->transform.position.y == y){
            return b;
        }
        else{
            return e;
        }
    }
}