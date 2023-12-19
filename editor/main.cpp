#define CL_HPP_TARGET_OPENCL_VERSION 200

#include "Sprite.h"
#include "BitmapReader.h"
#include "MouseInputService.h"

#include <stdio.h>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef __APPLE__

#include <OpenGL/gl3.h>
#include <OpenGL/OpenGL.h>

#else

#include <GL/glx.h>

#endif

int main(){

    const unsigned width = 640, height = 480;

    if (!glfwInit()){
        printf("Failed to initialize GLFW!\n");
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

    BitmapReader reader;
    MouseInputService iohandler(window);

    Image im = reader.ReadFile("../../resources/sprites/test.bmp");
    Image cursor_idle = reader.ReadFile("../../resources/sprites/cursor_idle.bmp");

    iohandler.SetNormalCursor((unsigned char*)cursor_idle.pixels, cursor_idle.width, cursor_idle.height);

    delete[] cursor_idle.pixels;

    float vertex[] ={
        -0.5, -0.5, 0.0,
        -0.5, 0.5, 0.0,
        0.5, 0.5, 0.0,
        0.5, -0.5, 0.0
    };

    Sprite *smiley_face = Sprite::Create(&im);

    delete[] im.pixels;


    if(!smiley_face){
        glfwDestroyWindow(window);
        glfwTerminate();

        return -1;
    }

    fprintf(stdout, "Sprite Checksum : 0x%08X \n", smiley_face->GetChecksum());

    float angle = 0.0f;
    float diff = 2 * M_PI/4.0f;

    while(!glfwWindowShouldClose(window)){

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        smiley_face->Load();

        glBegin(GL_QUADS);
         glTexCoord2d(1, 1); glVertex3f(vertex[0], vertex[1], vertex[2]);
         glTexCoord2d(1, 0); glVertex3f(vertex[3], vertex[4], vertex[5]);
         glTexCoord2d(0, 0); glVertex3f(vertex[6], vertex[7], vertex[8]);
         glTexCoord2d(0, 1); glVertex3f(vertex[9], vertex[10], vertex[11]);
        glEnd();

        smiley_face->UnLoad();

        glfwSwapBuffers(window);
        glfwPollEvents();

        vertex[0] = cos(angle);
        vertex[1] = sin(angle);

        vertex[3] = cos(angle + diff);
        vertex[4] = sin(angle + diff);

        vertex[6] = cos(angle + 2*diff);
        vertex[7] = sin(angle + 2*diff);

        vertex[9] = cos(angle - diff);
        vertex[10] = sin(angle - diff);


        angle = (angle+0.01f) * (angle < 360.0f);

        EventInfo info = iohandler.Query();

        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            printf("OpenGL error: %d\n", error);
        }
    }

    delete smiley_face;


    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
