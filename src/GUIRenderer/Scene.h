#ifndef SCENE_H
#define SCENE_H

#include "AttributeContainer.h"
#include "EntityContainer.h"
#include "SceneObject.h"
#include "GUIElement.h"
#include "Sprite.h"
#include <stdio.h>


class Scene{
private:
	EntityContainer entities;
	std::list<SceneObject * > objects;
	std::list<GUIElement * > guiObjects;

public:

	SceneObject * CreateEntity(){
		Entity ID = entities.Create();

		SceneObject * object = new SceneObject(ID);

#ifdef DEBUG

	fprintf(stdout, "[DEBUG] New scene object %d created\n", ID);

#endif

		return object;
	}

	void AddEntityToScene(SceneObject * object){
		objects.push_back(object);
	}

	void RemoveEntityFromScene(SceneObject * object){
		Entity ID = object->GetEntityID();
		entities.Destroy( ID );


		auto it = std::find(objects.begin(), objects.end(), object);

		if (it == objects.end())
			return;

		objects.erase(it);
			delete object;

#ifdef DEBUG

		fprintf(stdout, "[DEBUG] Removed scene object %d\n", ID);

#endif


	}

	void AddGUIToScene(GUIElement * object){
		guiObjects.push_back(object);
	}

	void RemoveGUIFromScene(GUIElement * object){

		auto it = std::find(guiObjects.begin(), guiObjects.end(), object);

		if(it == guiObjects.end())
			return;

		guiObjects.erase(it);
		delete object;

#ifdef DEBUG

	fprintf(stdout, "[DEBUG] Removed gui object from scene\n");

#endif

	}

	std::list<SceneObject*>& GetSceneObjects(){
		return objects;
	}

	std::list<GUIElement*>& GetGUIElements(){
		return guiObjects;
	}

};

#endif
