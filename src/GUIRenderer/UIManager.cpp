#include "UIManager.h"

UIManager::UIManager(const int &windowWidth, const int &windowHeight, const char* windowTitle, GLFWmonitor* windowMonitor, GLFWwindow* windowShare){
	window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, windowMonitor, windowShare);
	scene = new Scene();
}
UIManager::UIManager(){
	const int deatfullWindowWidth = 314;
	const int deatfullWindowHeigth = 161;
	const char* deatfullWindowTitle = "Tachion Engine";
	GLFWmonitor* deatfullWindowMonitor = NULL;
	GLFWwindow* deatfullWindowShare = NULL;
	window = glfwCreateWindow(deatfullWindowWidth, deatfullWindowHeigth, deatfullWindowTitle, deatfullWindowMonitor, deatfullWindowShare);
	scene = new Scene();
}
UIManager::~UIManager(){
	delete scene;
	glfwDestroyWindow(window);
}

void UIManager::AssignEventHandlingService(IEventHandlingService* _eventHandling){
	eventHandlingService = _eventHandling;
}
void UIManager::AssignInputHandlingService(IInputHandler* _inputHandler){
	inputHandlingService = _inputHandler;
}
void UIManager::AddComponentToScene(Component* component){
	scene->AddComponent(component);
}
void UIManager::HandleEvents(){
	{
		//this shouldn't be commented after name change
		//mouseXPosition=specialName.getX();
		//mouseYPosition=specialName.getY();
		//eventType=specialName.getEventType();
	}
	{
		//dlete this
		//forTests
		mouseXPosition=0;
		mouseYPosition=0;
		eventType=ONCLICK;
	}
	eventHandlingService->Publish(eventType, (IEventListener*)scene->GetComponent(mouseXPosition,mouseYPosition));
}
void UIManager::Update(){

	// if(specialName!=NULL){
	// 	HandleEvents();
	// }
}
void UIManager::Render(){
	scene->Render();
}

bool UIManager::ShouldClose(){
    return glfwWindowShouldClose(window);
}
