#include "Scene.h"

void Scene::RemoveComponents(Component* _object){
	for(auto i = components.begin(); i != components.end(); ++i)
		if(*i == _object){
			components.erase(i);
		}
}
void Scene::AddComponent(Component* _object){
	components.push_back(_object);
}

//this returns only one component, if there is more components on that position, this funcion will not return them all
Component* Scene::GetComponent(const float &_x, const float &_y) const{

	for(unsigned int i = 0; i < components.size(); ++i){

		if(components[i]->IsInBound(_x, _y))
			return components[i];

	}

	return NULL;
}
void Scene::Render() const{

	for(unsigned int i = 0; i < components.size(); ++i){
		components[i]->Render();
	}

}

Scene::~Scene(){

	//TODO

}
