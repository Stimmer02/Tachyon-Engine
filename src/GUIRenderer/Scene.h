#ifndef SCENE_H
#define SCENE_H

#include "AttributeContainer.h"
#include "EntityContainer.h"
#include "SceneObject.h"
#include "Sprite.h"
#include <stdio.h>


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

	std::list<SceneObject*>& GetSceneObjects(){
		return objects;
	}

};

#endif
