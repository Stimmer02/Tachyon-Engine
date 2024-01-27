#ifndef SCENE_H
#define SCENE_H

#include "Component.h"
#include <vector>

class Scene{
private:
	std::vector <Component*> components;
public:
	void RemoveComponents(Component* _object);
	Component* GetComponent(const float &_x, const float &_y) const;
	void AddComponent(Component* _object);
	void Render() const;
	~Scene();
};

#endif
