#ifndef INTERACTIONMANAGER_H
#define INTERACTIONMANAGER_H

#include "KDT.h"
#include "Scene.h"

class InteractionManager {
private:
    KDT interactiveElementContainer;
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

        interactiveElementContainer.buildTree(interactiveElements);
    }

    void Interact(const int & x, const int & y){
        InteractiveElement * element = interactiveElementContainer.find(x, y);

        if(element == nullptr)
            return;

        element->Interact();
    }

};



#endif
