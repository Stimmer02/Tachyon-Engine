#ifndef UIMANAGER_H
#define UIMANAGER_H



#include "IEventHandlingService.h"
#include "IInputHandler.h"
#include "InteractiveComponent.h"
#include "Scene.h"
#include "Headers.h"

#include <stdio.h>


class UIManager{
private:

	IEventHandlingService* eventHandlingService;
	IInputHandler* inputHandlingService;

	GLFWwindow* window;

	// Scene scene;

	EventType eventType;

	void HandleEvents();
	void Render();

public:
	UIManager(const int &windowWidth, const int &windowHeight, const char* windowTitle, const bool & enableVSync);

	void AssignEventHandlingService(IEventHandlingService* _eventHandling);

	void AddComponentToScene(Component* component);

	void Update();

	bool ShouldClose();

	~UIManager();
};

#endif
