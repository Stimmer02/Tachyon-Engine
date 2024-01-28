#define GL_SILENCE_DEPRECATION
#include "PhysicsProcessor/PhysicsProcessorBuilder.h"
#include <filesystem>

#include "Sprite.h"

GLFWwindow* initializeGLFW(uint height, uint width);
cl::Program compileCopyKernel(cl::Context context, cl::Device default_device);

void processInput(GLFWwindow *window);
void glfwErrorCallback(int error, const char* description);


bool isPaused = true;

IPhysicsProcessor* physicsProcessor;

int main(){

    const int width = 512;
    const int height = 512;

    const int texture_width = 512;
    const int texture_height = 512;

    //Initialize GLFW

    GLFWwindow* window = initializeGLFW(height, width);

    //Create texutre

    Color pixels[texture_width * texture_height] = {0};

    Sprite * texture = Sprite::Create(pixels, texture_width, texture_height);

    //Initialize PhysicsProcessor
    engineConfig config;
    config.atmosphereViscosity = 1;
    config.gravity = 1;
    config.timefactor = 1;
    config.simulationHeight = height;
    config.simulationWidth = width;

    PhysicsProcessorBuilder PBB;
    physicsProcessor = PBB.build("./engine_kernel_fragments", "./engine_structs", texture->GetTexture(), config, 0, 0);
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
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        physicsProcessor->generateFrame();
        std::printf("simulation paused\n");
    }


    GLuint error = 0;
    uint frames = 0;
    while (!glfwWindowShouldClose(window)){
        processInput(window);

        if (!isPaused){
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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

            physicsProcessor->generateFrame();

            texture->Load();

            glBegin(GL_QUADS);
            glTexCoord2i(0, 0); glVertex2f(0, 0);
            glTexCoord2i(1, 0); glVertex2f(width, 0);
            glTexCoord2i(1, 1); glVertex2f(width, height);
            glTexCoord2i(0, 1); glVertex2f(0, height);
            glEnd();

            texture->UnLoad();

            glfwSwapBuffers(window);
        }

        glfwPollEvents();

        error = glGetError();
        if (error != GL_NO_ERROR) {
            fprintf(stderr, "OpenGL error: %d\n", error);
            break;
        }
    }
    std::printf("\n");

    delete physicsProcessor;

    delete texture;

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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(width, height, "test", NULL, NULL);

    if (!window){
        std::printf("Failed to create GLFW window!\n");
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glLoadIdentity();
    glOrtho(0, width, 0, height, -1.0f, 1.0f);
    glViewport(0, 0, width, height);

    //Initialize glew

    if(glewInit() != GLEW_OK){
        glfwDestroyWindow(window);
        glfwTerminate();
        return nullptr;
    }

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
