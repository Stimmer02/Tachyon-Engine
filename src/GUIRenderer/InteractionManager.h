#ifndef INTERACTIONMANAGER_H
#define INTERACTIONMANAGER_H

#include "KDT.h"
#include "Scene.h"
#include <stdint.h>

class InteractionManager {
private:
    KDT interactiveElementContainer;
    uint32_t numElements;
public:

    void LoadScene(Scene & scene){
        std::vector<GUIElement*> & elements = scene.GetGUIElements();

        std::vector<InteractiveElement*> interactiveElements;
        interactiveElements.reserve(elements.size());

        for(GUIElement * element : elements){

            InteractiveElement * interactiveElement = (InteractiveElement*) element;

            if( interactiveElement != nullptr)
                interactiveElements.emplace_back(interactiveElement);

        }

        this->numElements = interactiveElements.size();

        if(numElements == 0)
            return;

        interactiveElementContainer.buildTree(interactiveElements);
    }

    void Interact(const int & x, const int & y){

        if(numElements == 0)
            return;

        InteractiveElement * element = interactiveElementContainer.find(x, y);

        fprintf(stdout, "Element status : %s\n", element?"FOUND":"LOST");

        if(element == nullptr)
            return;

        element->Interact();
    }

};



#endif
