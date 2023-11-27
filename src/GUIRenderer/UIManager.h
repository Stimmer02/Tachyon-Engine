#ifndef UIMANAGER_H
#define UIMANAGER_H


#ifdef __APPLE__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <OpenGL/OpenGL.h>

#else

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glx.h>

#endif

#include "IEventHandlingService.h"
#include "IInputHandlingService.h"
#include "Scene.h"
#include "EventType.h"

class UIManager{
private:
	IEventHandlingService* eventHandlingService;
	IInputHandlingService* inputHandlingService;
	GLFWwindow* window;
	Scene* scene;
	int mouseXPosition,mouseYPosition;
	EventType eventType;
public:
	UIManager(const int &windowWidth, const int &windowHeight, const char* windowTitle, GLFWmonitor* windowMonitor, GLFWwindow* windowShare);
	UIManager();
	~UIManager();
	void AssignEventHandlingService(IEventHandlingService* _eventHandling);
	void AssignInputHandlingService(IInputHandlingService* _inputHandler);
	void AddComponentToScene(Component* component);
	void HandleEvents();
	void Update();
	void Render();
	bool ShouldClose();
};

#endif
