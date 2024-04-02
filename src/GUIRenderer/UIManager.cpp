#include "UIManager.h"

UIManager::UIManager(const int &windowWidth, const int &windowHeight, const char* windowTitle, const bool & enableVSync){

	if ( !glfwInit() ){
        fprintf(stderr, "Failed to initialize GLFW!\n");
        return;
    }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);

    if ( window == nullptr ){
        glfwTerminate();
        fprintf(stderr, "Failed to create new window!\n");
        return ;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(enableVSync);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        glfwDestroyWindow(window);
        glfwTerminate();
        fprintf(stderr, "Failed to initialize GLEW\n");
        return;
    }

    const GLubyte * renderer = glGetString(GL_RENDERER);
    const GLubyte * version = glGetString(GL_VERSION);

    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    glEnable(GL_DEPTH_TEST);

	glLoadIdentity();
    glOrtho(0, windowWidth, 0, windowHeight, -1.0f, 1.0f);
    glViewport(0, 0, windowWidth, windowHeight);

    this->inputHandlingService = nullptr;

}

void UIManager::AssignEventHandlingService(IEventHandlingService * _eventHandling){
	eventHandlingService = _eventHandling;
}

void UIManager::AssignInputHandlingService(IInputHandler * _inputHandler){
	inputHandlingService = _inputHandler;
	_inputHandler->BindWindow(window);
}

void UIManager::AddComponentToScene(Component * component){
	scene.AddComponent(component);
}

void UIManager::HandleEvents(){

    if( inputHandlingService == nullptr )
        return;

    static float lastX, lastY;

	EventInfo info = inputHandlingService->Query();

	if(info.type == EventType::ONCLICK && lastX!=info.x && lastY != info.y){
        Component * component = scene.GetComponent(info.x, info.y);

        if(component != nullptr){
            InteractiveComponent * interactive = dynamic_cast<InteractiveComponent*>(component);

            eventHandlingService->Publish( info.type, interactive );
        }

        lastX = info.x;
        lastY = info.y;
    }

}

void UIManager::Update(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	HandleEvents();
	Render();

    glfwSwapBuffers(window);
    glfwPollEvents();

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        fprintf(stderr, "OpenGL error: %d\n", error);
		return ;
    }
}

void UIManager::Render(){
	scene.Render();
}

bool UIManager::ShouldClose(){
    return glfwWindowShouldClose(window);
}

UIManager::~UIManager(){

	glfwDestroyWindow(window);
	glfwTerminate();

	fprintf(stdout, "\nProgram exited succesfully.\n");
}
