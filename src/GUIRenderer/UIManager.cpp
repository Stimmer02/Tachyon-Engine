#include "UIManager.h"

UIManager::UIManager(const int &windowWidth, const int &windowHeight, const char* windowTitle, const bool & enableVSync){

	if ( !glfwInit() ){
        fprintf(stderr, "Failed to initialize GLFW!\n");
        return;
    }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

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

    //glViewport(0, windowWidth, 0, windowHeight);

    if (glewInit() != GLEW_OK) {
        glfwDestroyWindow(window);
        glfwTerminate();
        fprintf(stderr, "Failed to initialize GLEW\n");
        return;
    }

    CompileShaders();
    glEnable(GL_DEPTH_TEST);
}

void UIManager::CompileShaders(){

    const char* vertexShaderSource = R"(
            #version 330 core
            in vec3 pos;

            void main() {
                gl_Position = vec4(pos, 1);
            }
        )";

    const char* fragmentShaderSource = R"(
            #version 330 core
            out vec4 color;

            void main(){
                color = vec4(1, 0, 0, 1);
            }
        )";

    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "Vertex shader compilation failed:\n%s\n", infoLog);
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "Fragment shader compilation failed:\n%s\n", infoLog);
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "Shader program linking failed:\n%s\n", infoLog);
    }


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

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

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    glUniform3f(glGetUniformLocation(shaderProgram, "pos"), 0.0f, 0.0f, 0.0f);

	HandleEvents();
	Render();

    glUseProgram(0);

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

    glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();

	fprintf(stdout, "\nProgram exited succesfully.\n");
}
