#include "Sprite.h"

#ifdef __APPLE__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <OpenGL/OpenGL.h>
#include <OpenCL/opencl.h>
#include <OpenCL/cl_gl.h>
#include "../OpenCL/include/CL/cl.hpp"

#elif __WIN32__

typedef unsigned int uint;

#include <windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <CL/opencl.hpp>
#include <CL/cl_gl.h>

#else

#include <CL/opencl.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <CL/cl_gl.h>
#include <GL/glx.h>

#endif

int main(){

    const unsigned width = 640, height = 480;

    if (!glfwInit()){
        std::printf("Failed to initialize GLFW!\n");
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(width, height, "Editor", NULL, NULL);

    if (!window){
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glViewport(0, 0, width, height);
    glfwSwapInterval(1);

    if(glewInit() != GLEW_OK)
        return -1;

    const Color pixels[] = {
        (Color){255, 0, 0, 255},
        (Color){0, 255, 0, 255},
        (Color){0, 0, 255, 255},
        (Color){255, 0, 0, 255}
    };

    Sprite *s = Sprite::Create(pixels, 2, 2);

    if(!s){
        glfwDestroyWindow(window);
        glfwTerminate();

        return -1;
    }

    while(!glfwWindowShouldClose(window)){

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        s->Load();

        glBegin(GL_QUADS);
         glTexCoord2d(1, 1); glVertex3f(0.0, 0.0, 0.0);
         glTexCoord2d(1, 0); glVertex3f(0.0, 1.0, 0.0);
         glTexCoord2d(0, 0); glVertex3f(1.0, 1.0, 0.0);
         glTexCoord2d(0, 1); glVertex3f(1.0, 0.0, 0.0);
        glEnd();

        s->UnLoad();

        glfwSwapBuffers(window);
        glfwPollEvents();

        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::printf("OpenGL error: %d\n", error);
        }
    }

    delete s;

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
