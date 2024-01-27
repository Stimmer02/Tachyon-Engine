#define GL_SILENCE_DEPRECATION
#include "PhysicsProcessor/PhysicsProcessorBuilder.h"
#include <filesystem>

GLFWwindow* initializeGLFW(uint height, uint width);
cl::Program compileCopyKernel(cl::Context context, cl::Device default_device);

void processInput(GLFWwindow *window);
void glfwErrorCallback(int error, const char* description);

uint localXsize = 16;
uint localYsize = 16;
int width = 1024, height = 1024;
bool isPaused = false;

GLuint PBO;
GLuint texture;
GLuint fboId;

IPhysicsProcessor* physicsProcessor;



int main(){

    //Initialize GLFW

    GLFWwindow* window = initializeGLFW(height, width);

    if (!window){
        glfwTerminate();
        return EXIT_FAILURE;
    }
    //Initialize glew

    if(glewInit() != GLEW_OK){
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    //Create PBO and texutres

    glGenBuffers(1, &PBO);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, sizeof(color)*width*height, NULL, GL_STATIC_DRAW);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glGenFramebuffers(1, &fboId);
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stderr, "Framebuffer is not complete\n");
        return -1;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
    physicsProcessor->spawnVoxelInArea(0, 0, 8, config.simulationWidth, 1);
    physicsProcessor->spawnVoxelInArea(0, 0, config.simulationHeight, 8, 1);
    physicsProcessor->spawnVoxelInArea(config.simulationHeight-8, 0, 8, config.simulationWidth, 1);
    physicsProcessor->spawnVoxelInArea(0, config.simulationWidth-8, config.simulationHeight, 8, 1);

    physicsProcessor->spawnVoxelInArea(0, config.simulationWidth/3, config.simulationHeight, 8, 1);
    physicsProcessor->spawnVoxelInArea(0, config.simulationWidth/3*2, config.simulationHeight, 8, 1);


    std::printf("play: 2; pause: 1\n");
    if (isPaused){
        physicsProcessor->generateFrame();
        std::printf("simulation paused\n");
    }


    GLuint error = 0;
    uint frames = 0;
    while (!glfwWindowShouldClose(window)){
        processInput(window);

        if (!isPaused){
            glClear(GL_COLOR_BUFFER_BIT);


            physicsProcessor->spawnVoxelInArea((config.simulationWidth>>1)-4, config.simulationHeight>>1, 8, 8, 2);
            physicsProcessor->spawnVoxelInArea((config.simulationWidth>>1)-4, (config.simulationHeight>>1) - config.simulationHeight/3, 8, 8, 3);
            physicsProcessor->spawnVoxelInArea((config.simulationWidth>>1)-4, (config.simulationHeight>>1) + config.simulationHeight/3, 8, 8, 4);
            if (frames % 10 == 0){
                uint voxelCount = physicsProcessor->countVoxels();
                std::printf("\33[2\rKF: %5d; V: %d", frames, voxelCount);
                fflush(stdout);
            }
            frames++;

            glBindFramebuffer(GL_FRAMEBUFFER, fboId);
            physicsProcessor->generateFrame();
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        glfwSwapBuffers(window);
        glfwPollEvents();

        error = glGetError();
        if (error != GL_NO_ERROR) {
            fprintf(stderr, "OpenGL error: %d\n", error);
            break;
        }
    }
    std::printf("\n");

    delete physicsProcessor;

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteBuffers(1, &PBO);
    glDeleteTextures(1, &texture);
    glDeleteFramebuffers(1, &fboId);

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}

GLFWwindow* initializeGLFW(uint width, uint height){
    if (!glfwInit()){
        std::printf("Failed to initialize GLFW!\n");
        return nullptr;
    }
    glfwSetErrorCallback(glfwErrorCallback);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(width, height, "test", NULL, NULL);
    if (!window){
        std::printf("Failed to create GLFW window!\n");
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glOrtho(0, width, 0, height, -1.0f, 1.0f);
    glViewport(0, 0, width, height);
    glfwSwapInterval(1);

    return window;
}

void processInput(GLFWwindow *window){
    static bool pressed = false;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
        isPaused = true;
    } else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
        isPaused = false;
    }
}

void glfwErrorCallback(int error, const char* description){
    printf("Error: %s\n", description);
}
