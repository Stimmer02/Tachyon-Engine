#ifndef SCENE_H
#define SCENE_H

#include "AttributeContainer.h"
#include "SceneObject.h"
#include "Sprite.h"
#include <stdio.h>

using IterationItem = std::list<SceneObject*>::iterator;

class Scene{
private:
	EntityContainer entities;
	std::list<SceneObject * > objects;

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

#ifdef DEBUG

	fprintf(stdout, "[DEBUG] Removed scene object %d\n", ID);

#endif

	}

	std::pair<IterationItem, IterationItem> GetSceneObjectsIterator(){
		return std::make_pair(objects.begin(), objects.end());
	}

};

#endif
