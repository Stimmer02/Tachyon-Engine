#include "PhysicsProcessor/PhysicsProcessorBuilder.h"
#include <filesystem>
#define CL_HPP_TARGET_OPENCL_VERSION 200

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
#include <CL/cl.hpp>
#include <CL/opencl.hpp>
#include <CL/cl_gl.h>

#else

#include <CL/opencl.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <CL/cl_gl.h>
#include <GL/glx.h>

#endif

#include "MouseInputService.h"

#include <string>
#include <vector>
#include <cstdio>

struct color{
    unsigned char R, G, B;
};

void processInput(GLFWwindow *window);

GLFWwindow* initializeGLFW(uint height, uint width);
cl::Program compileCopyKernel(cl::Context context, cl::Device default_device);

void glfwErrorCallback(int error, const char* description);

uint localXsize = 16;
uint localYsize = 16;
int width = 1024, height = 1024;

GLuint PBO;
GLuint texture;
GLuint fboId;

IPhysicsProcessor* physicsProcessor;



int main(){

    //Initialize GLFW

    GLFWwindow* window = initializeGLFW(height, width);

    if (!window){
         glfwTerminate();
         return -1;
    }


    //Initialize glew

    if(glewInit() != GLEW_OK)
        return -1;

    //Create PBO and texutres

    glGenBuffers(1, &PBO);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, sizeof(color)*3840*2160, NULL, GL_DYNAMIC_DRAW);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glGenFramebuffers(1, &fboId);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    //Initialize PhysicsProcessor
    engineConfig config;
    config.atmosphereViscosity = 1;
    config.gravity = 1;
    config.timefactor = 1;
    config.simulationHeight = height;
    config.simulationWidth = width;

    std::filesystem::path workingPath = std::filesystem::current_path();
    std::printf("Running in direcory: %s\n", workingPath.c_str());

    PhysicsProcessorBuilder PBB;
    physicsProcessor = PBB.build("./engine_kernel_fragments", "./engine_structs", PBO, config, 0, 0);
    if (physicsProcessor == nullptr){
        std::printf("Something went wrong...\n");
        return 1;
    }


    GLuint error = 0;
    uint x = 0, y = 0;
    while (!glfwWindowShouldClose(window)){

        physicsProcessor->generateFrame();

        glClear(GL_COLOR_BUFFER_BIT);

        for (uint i = 0; i < 21; ++i){
            physicsProcessor->spawnVoxel(x, y, 1);
            ++x;

            if (x == width){
                ++y;
                x = 0;

                if (y == height){
                    y = 0;
                }
            }
        }

        for (uint i = 0; i < 34; ++i){
            physicsProcessor->spawnVoxel(x, y, 2);
            ++x;

            if (x == width){
                ++y;
                x = 0;

                if (y == height){
                    y = 0;
                }
            }
        }

        error = glGetError();
        if (error != GL_NO_ERROR) {
            std::printf("OpenGL error: %d\n", error);
        }
    }

    delete physicsProcessor;

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDeleteBuffers(1, &PBO);

    glDeleteTextures(1, &texture);

    glfwDestroyWindow(window);
}

GLFWwindow* initializeGLFW(uint width, uint height){
    if (!glfwInit()){
        std::printf("Failed to initialize GLFW!\n");
        return nullptr;
    }
    glfwSetErrorCallback(glfwErrorCallback);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(width, height, "test", NULL, NULL);
    if (!window){
        std::printf("Failed to create GLFW window!\n");
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glViewport(0,0, width, height);
    glfwSwapInterval(1);

    return window;
}



void glfwErrorCallback(int error, const char* description){
    printf("Error: %s\n", description);
}
