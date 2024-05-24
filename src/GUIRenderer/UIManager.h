#ifndef UIMANAGER_H
#define UIMANAGER_H


#ifdef __APPLE__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <OpenGL/OpenGL.h>

#elif _WIN32

#include <windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#else

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glx.h>

#endif

#include <stdio.h>
#include "IEventHandlingService.h"
#include "IInputHandler.h"
#include "InteractiveComponent.h"
#include "Scene.h"

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
