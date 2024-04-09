#ifndef WINDOWCONTEXT_H
#define WINDOWCONTEXT_H

#include <GL/glew.h>
#include <string>
#include <cassert>
#include <unordered_map>

#include "IShareableContext.h"

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

    void SetHints(){

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

        glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    }

public:
    WindowContext(){

        if ( !glfwInit() ){
            fprintf(stderr, "Failed to initialize GLFW!\n");
            return;
        }

        SetHints();

        this->window = nullptr;
        this->bufferbits = GL_COLOR_BUFFER_BIT;

    }

    void CreateWindow(const int & width = 800, const int & height = 600, const std::string & title = "Window"){
        assert(width > 0 && height > 0 && "Invalid dimensions");

        window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        glfwMakeContextCurrent(window);

        glewExperimental = true;
        glewInit();

#ifdef DEBUG
        const GLubyte * renderer = glGetString(GL_RENDERER);
        const GLubyte * version = glGetString(GL_VERSION);

        printf("[DEBUG] Renderer: %s\n", renderer);
        printf("[DEBUG] OpenGL version supported %s\n", version);
#endif

    }

    void SetVSync(const bool & vsync = true){
        glfwSwapInterval(vsync);
    }

    void SetZBuffer(const bool & depthBuffer = false){

        if( depthBuffer ){
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_ALWAYS);
        }else{
            glDisable(GL_DEPTH_TEST);
        }

        bufferbits ^= GL_DEPTH_BUFFER_BIT;

    }

    void ShareContext(IShareableContext * client) const{
        assert(window && "Window does not exits");
        client->AcceptGLFWContext(window);
    }

    void BindMouseButtonCallback(GLFWmousebuttonfun callback) const{
        assert(window && "Window does not exits");
        glfwSetMouseButtonCallback(window, callback);
    }

    void BindKeyCallback(GLFWkeyfun callback) const{
        assert(window && "Window does not exits");
        glfwSetKeyCallback(window, callback);
    }

    void BindScrollCallback(GLFWscrollfun callback) const{
        assert(window && "Window does not exits");
        glfwSetScrollCallback(window, callback);
    }

    void SetInternalPointer(void * pointer) const{
        assert(window && "Window does not exits");
        glfwSetWindowUserPointer(window, pointer);
    }

    void * GetInternalPointer() const{
        assert(window && "Window does not exits");
        void * ptr = glfwGetWindowUserPointer(window);
        return ptr;
    }

    void CheckErrors() const{
        GLenum error = glGetError();

        if( error == GL_NO_ERROR )
            return;

        std::unordered_map<GLenum, const char *>::const_iterator it = errorMap.find(error);

        if( it != errorMap.end() ){
            printf("Error occured : %s\n", it->second);
        }
    }

    bool ShouldClose() const{
        assert(window && "Window does not exits");
        return glfwWindowShouldClose(window);
    }

    void PoolEvents() const{
        glfwPollEvents();
    }

    void SwapBuffers() const{
        assert(window && "Window does not exits");
        glfwSwapBuffers(window);
        glClear(bufferbits);
    }

    void DestroyWindow(){
        assert(window && "Window does not exits");
        glfwDestroyWindow(window);
    }

    ~WindowContext(){

        DestroyWindow();
        glfwTerminate();

    }
};

#endif
