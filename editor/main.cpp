#include "Sprite.h"
#include <cmath>

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

    float vertex[] ={
        -0.5, -0.5, 0.0,
        -0.5, 0.5, 0.0,
        0.5, 0.5, 0.0,
        0.5, -0.5, 0.0
    };

    Sprite *s = Sprite::Create(pixels, 2, 2);

    if(!s){
        glfwDestroyWindow(window);
        glfwTerminate();

        return -1;
    }

    float angle = 0.0f;
    float diff = 2 * M_PI/4.0f;

    while(!glfwWindowShouldClose(window)){

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        s->Load();

        glBegin(GL_QUADS);
         glTexCoord2d(1, 1); glVertex3f(vertex[0], vertex[1], vertex[2]);
         glTexCoord2d(1, 0); glVertex3f(vertex[3], vertex[4], vertex[5]);
         glTexCoord2d(0, 0); glVertex3f(vertex[6], vertex[7], vertex[8]);
         glTexCoord2d(0, 1); glVertex3f(vertex[9], vertex[10], vertex[11]);
        glEnd();

        s->UnLoad();

        glfwSwapBuffers(window);
        glfwPollEvents();

        vertex[0] = cos(angle);
        vertex[1] = sin(angle);

        vertex[3] = cos(angle + diff);
        vertex[4] = sin(angle + diff);

        vertex[6] = cos(angle + 2*diff);
        vertex[7] = sin(angle + 2*diff);

        vertex[9] = cos(angle + 3*diff);
        vertex[10] = sin(angle + 3*diff);


        angle = (angle+0.01f) * (angle < 360.0f);

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
