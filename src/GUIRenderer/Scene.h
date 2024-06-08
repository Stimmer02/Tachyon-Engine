#ifndef SCENE_H
#define SCENE_H

#include "AttributeContainer.h"
#include "EntityContainer.h"
#include "SceneObject.h"
#include "GUIElement.h"
#include "Sprite.h"
#include <stdio.h>
#include <vector>


class Scene{
private:
	EntityContainer entities;
	std::vector<SceneObject * > objects;
	std::vector<GUIElement * > guiObjects;

public:

	SceneObject * CreateEntity(){
		Entity ID = entities.Create();

		SceneObject * object = new SceneObject(ID);

#ifdef DEBUG

	fprintf(stdout, "[DEBUG] New scene object %d created\n", ID);

#endif

		return object;
	}

	void DestroyEntity(SceneObject * object){
		Entity ID = object->GetEntityID();
		entities.Destroy( ID );
	}

	void AddEntityToScene(SceneObject * object){
		objects.push_back(object);
	}

	void RemoveEntityFromScene(SceneObject * object){
		if (object == nullptr)
        	return;

		Entity ID = object->GetEntityID();
		entities.Destroy( ID );

    auto it = std::find(objects.begin(), objects.end(), object);
    if ( it == objects.end() )
		return;

    objects.erase(it);
    delete object;

#ifdef DEBUG
        fprintf(stdout, "[DEBUG] Removed entity %d\n", ID);
#endif

	}

	void AddGUIToScene(GUIElement * object){
		guiObjects.push_back(object);
	}

	void RemoveGUIFromScene(GUIElement * object){

		for (auto it = guiObjects.begin(); it != guiObjects.end(); ++it) {
			if (*it == object) {
				guiObjects.erase(it);
				delete object;

#ifdef DEBUG

		fprintf(stdout, "[DEBUG] Removed gui object\n");

#endif

				break;
			}
		}

	}

	std::vector<SceneObject*>& GetSceneObjects(){
		return objects;
	}

	std::vector<GUIElement*>& GetGUIElements(){
		return guiObjects;
	}

};

#endif
