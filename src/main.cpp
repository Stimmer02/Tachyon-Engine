#include "PhysicsProcessor/PhysicsProcessorBuilder.h"
#include <filesystem>

void processInput(GLFWwindow *window);

GLFWwindow* initializeGLFW(uint height, uint width);
cl::Program compileCopyKernel(cl::Context context, cl::Device default_device);

void glfwErrorCallback(int error, const char* description);

int width = 1024, height = 1024;

GLuint PBO;
GLuint texture;
GLuint fboId;

IPhysicsProcessor* physicsProcessor;


bool pause = true;

int main(){

    //Initialize GLFW

    GLFWwindow* window = initializeGLFW(width, height);

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
    glBufferData(GL_PIXEL_UNPACK_BUFFER, sizeof(color)*width*height, NULL, GL_STATIC_DRAW);

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


    //Prepare simulation
    physicsProcessor->spawnVoxelInArea(0, 512, 1024, 512, 2);
    // for (uint i = 0; i+64 < config.simulationWidth; i+= 128){
    //     physicsProcessor->spawnVoxelInArea(i, 30, 64, 8, 1);
    // }


    GLuint error = 0;
    uint x = 0;

    while (!glfwWindowShouldClose(window)){
        processInput(window);

        if (!pause){
            physicsProcessor->generateFrame();
            glClear(GL_COLOR_BUFFER_BIT);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
            // for (uint i = 0; i < 3; i++){
            //     physicsProcessor->spawnVoxelInArea(x, config.simulationHeight-4, 8, 8, 2);
            //     ++x;
            //     if (x + 8 >= width){
            //         x = 0;
            //     }
            // }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();




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



//HERE YOU CAN PUT ANYTHING TO TEST YOUR METHODS
//IPhysicsProcessor is global so it is in this function's scoope

void processInput(GLFWwindow *window){
    static bool pressed = false;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
        pause = true;
    } else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
        pause = false;
    }
}
