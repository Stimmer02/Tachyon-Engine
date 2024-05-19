#ifndef WINDOWCONTEXT_H
#define WINDOWCONTEXT_H


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <cassert>
#include <unordered_map>

#include "EventRegister.h"

class WindowContext{

    const std::unordered_map<GLenum, const char*> errorMap = {
        {GL_NO_ERROR, "GL_NO_ERROR"},
        {GL_INVALID_ENUM, "GL_INVALID_ENUM"},
        {GL_INVALID_VALUE, "GL_INVALID_VALUE"},
        {GL_INVALID_OPERATION, "GL_INVALID_OPERATION"},
        {GL_STACK_OVERFLOW, "GL_STACK_OVERFLOW"},
        {GL_STACK_UNDERFLOW, "GL_STACK_UNDERFLOW"},
        {GL_OUT_OF_MEMORY, "GL_OUT_OF_MEMORY"},
        {GL_INVALID_FRAMEBUFFER_OPERATION, "GL_INVALID_FRAMEBUFFER_OPERATION"},
        {GL_CONTEXT_LOST, "GL_CONTEXT_LOST"}
    };

private:

    GLbitfield bufferbits;
    GLFWwindow * window;

    ILog * windowLogger;

    void SetHints(const bool & suppressVisibility, const bool & resizeable){

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

        if( suppressVisibility )
            glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

        if( !resizeable ){
            glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        }

    }

public:

    WindowContext(const bool & suppressVisibility = false, const bool & resizeable = false, const std::string & file = "runtime.log"){

        this->windowLogger = new EventRegister(file.c_str());

        windowLogger->Write(LogMessageType::M_INFO, "Initializing new context instance\n");

        if ( !glfwInit() ){
            windowLogger->Write(LogMessageType::M_DEBUG, "Failed to initialize GLFW!\n");
            windowLogger->Flush();
            delete windowLogger;
            exit(-1);
        }

        SetHints(suppressVisibility, resizeable);

        this->window = nullptr;
        this->bufferbits = GL_COLOR_BUFFER_BIT;

    }

    void Open(const int & width = 800, const int & height = 600, const std::string & title = "Window"){
        assert(width > 0 && height > 0 && "Invalid dimensions");

        windowLogger->Write(LogMessageType::M_INFO, "Opening new window instance with resolution %d x %d\n", width, height);

        window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        glfwMakeContextCurrent(window);

        glewExperimental = true;
        glewInit();

        const GLubyte * device = glGetString(GL_RENDERER);
        const GLubyte * version = glGetString(GL_VERSION);

        windowLogger->Write(LogMessageType::M_INFO, "Discovered new device : %s\n", device);
        windowLogger->Write(LogMessageType::M_INFO, "Supported OpenGL version : %s\n", version);

    }

    void SetVSync(const bool & vsync = true){

        windowLogger->Write(LogMessageType::M_INFO, "Current V-Sync state : %s\n", vsync?"Enabled":"Disabled");
        glfwSwapInterval(vsync);

    }

    void SetZBuffer(const bool & depthBuffer = false){

        if( depthBuffer ){
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
        }else{
            glDisable(GL_DEPTH_TEST);
        }

        windowLogger->Write(LogMessageType::M_INFO, "Current Z-Buffer state : %s\n", depthBuffer?"Enabled":"Disabled");

        bufferbits ^= GL_DEPTH_BUFFER_BIT;

    }

    void BindMouseButtonCallback(GLFWmousebuttonfun callback) const{
        if( !window ){
            windowLogger->Write(LogMessageType::M_ERROR, "Window instance does not exist\n");
            windowLogger->Flush();
            exit(-1);
        }
        glfwSetMouseButtonCallback(window, callback);
    }

    void BindKeyCallback(GLFWkeyfun callback) const{
        if( !window ){
            windowLogger->Write(LogMessageType::M_ERROR, "Window instance does not exist\n");
            windowLogger->Flush();
            exit(-1);
        }
        glfwSetKeyCallback(window, callback);
    }

    void BindResizeCallback(GLFWwindowsizefun callback) const {
        if( !window ){
            windowLogger->Write(LogMessageType::M_ERROR, "Window instance does not exist\n");
            windowLogger->Flush();
            exit(-1);
        }
        glfwSetWindowSizeCallback(window, callback);
    }

    void BindScrollCallback(GLFWscrollfun callback) const{
        if( !window ){
            windowLogger->Write(LogMessageType::M_ERROR, "Window instance does not exist\n");
            windowLogger->Flush();
            exit(-1);
        }
        glfwSetScrollCallback(window, callback);
    }

    void SetInternalPointer(void * pointer) const{
        if( !window ){
            windowLogger->Write(LogMessageType::M_ERROR, "Window instance does not exist\n");
            windowLogger->Flush();
            exit(-1);
        }
        glfwSetWindowUserPointer(window, pointer);
    }

    void * GetInternalPointer() const{
        if( !window ){
            windowLogger->Write(LogMessageType::M_ERROR, "Window instance does not exist\n");
            windowLogger->Flush();
            exit(-1);
        }
        void * ptr = glfwGetWindowUserPointer(window);
        return ptr;
    }

    void CheckErrors() const{
        GLenum error = glGetError();

        if( error == GL_NO_ERROR )
            return;

        std::unordered_map<GLenum, const char *>::const_iterator it = errorMap.find(error);

        if( it != errorMap.end() ){
            windowLogger->Write(LogMessageType::M_ERROR, "OpenGL error : %s\n", it->second);
            windowLogger->Flush();
            exit(-1);
        }

    }

    void Close() const{

        if( !window ){
            windowLogger->Write(LogMessageType::M_ERROR, "Window instance does not exist\n");
            windowLogger->Flush();
            exit(-1);
        }

        glfwSetWindowShouldClose(window, true);
    }

    bool ShouldClose() const{

        if( !window ){
            windowLogger->Write(LogMessageType::M_ERROR, "Window instance does not exist\n");
            windowLogger->Flush();
            exit(-1);
        }

        return glfwWindowShouldClose(window);
    }

    void PoolEvents() const{
        glfwPollEvents();
    }

    int GetMouseButton(const int & button) const{
        if( button >= GLFW_MOUSE_BUTTON_LAST)
            return 0;
        return glfwGetMouseButton(window, button);
    }

    int GetKeyboardKey(const int & key) const{
        if( key >= GLFW_KEY_LAST)
            return 0;
        return glfwGetKey(window, key);
    }

    void GetMousePos(double & x, double & y) const{
        glfwGetCursorPos(window, &x, &y);
    }

    void GetWindowSize(int & width, int & height) const{
        glfwGetWindowSize(window, &width, &height);
    }

    void SwapBuffers() const{
        if( !window ){
            windowLogger->Write(LogMessageType::M_ERROR, "Window instance does not exist\n");
            windowLogger->Flush();
            exit(-1);
        }

        glfwSwapBuffers(window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(bufferbits);
    }

    void DestroyWindow(){
        if( !window ){
            windowLogger->Write(LogMessageType::M_ERROR, "Window instance does not exist\n");
            windowLogger->Flush();
            exit(-1);
        }

        glfwDestroyWindow(window);
        windowLogger->Write(LogMessageType::M_INFO, "Window instance destroyed succesfully\n");
    }

    ILog * GetContextLogger(){
        return windowLogger;
    }

    ~WindowContext(){

        windowLogger->Write(LogMessageType::M_INFO, "Context destroyed succesfully\n");
        windowLogger->Flush();

        DestroyWindow();
        glfwTerminate();

    }
};

#endif
