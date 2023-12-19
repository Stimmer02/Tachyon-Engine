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
cl::Device getDefaultClDevice();
cl::Program compileCopyKernel(cl::Context context, cl::Device default_device);

void glfwErrorCallback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// int width = 2160, height = 3840;
int width = 1000, height = 1000;

GLuint PBO;
GLuint texture;
GLuint fboId;


bool FALLBACK = false;

int main(){

    //Initialize GLFW

    GLFWwindow* window = initializeGLFW(width, height);

    if (!window){
         glfwTerminate();
         return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Initialize glew

    if(glewInit() != GLEW_OK)
        return -1;

    //Initialize OpenCL

    cl::Device default_device = getDefaultClDevice();

    if (!default_device()){
        return 1;
    }

    cl_platform_id platform;
    clGetPlatformIDs(1, &platform, NULL);

#ifdef __APPLE__

    CGLContextObj glContext = CGLGetCurrentContext();
    CGLShareGroupObj shareGroup = CGLGetShareGroup(glContext);

    cl_context_properties properties[] = {
        CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
        (cl_context_properties)shareGroup,
        0
    };

#elif __WIN32__

    cl_context_properties properties[] = {
        CL_GL_CONTEXT_KHR, (cl_context_properties) wglGetCurrentContext(),
        CL_WGL_HDC_KHR, (cl_context_properties) wglGetCurrentDC(),
        CL_CONTEXT_PLATFORM, (cl_context_properties) platform,
        0
    };


#else

     cl_context_properties properties[] = {
        CL_GL_CONTEXT_KHR, (cl_context_properties) glXGetCurrentContext(),
        CL_GLX_DISPLAY_KHR, (cl_context_properties) glXGetCurrentDisplay(),
        CL_CONTEXT_PLATFORM, (cl_context_properties) platform,
        0
    };

#endif


    cl::Context context(default_device, properties);

    cl::Program program = compileCopyKernel(context, default_device);

    if(!program()){
        context = cl::Context(default_device);
        program = compileCopyKernel(context, default_device);
        if(!program()){
            printf("Error: not able to compile kernel!\n");
            return 1;
        }
        FALLBACK = true;
        printf("Warning: entering fallback mode\n");
    }

    size_t maxLocalWorkSize;
    clGetDeviceInfo(default_device(), CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &maxLocalWorkSize, NULL);
    std::printf("max local work size: %ld\n", maxLocalWorkSize);

    cl::CommandQueue queue(context, default_device);

    int dims[2] = {width, height};
    int anim = 0;

    cl::Buffer buffer_dims(context, CL_MEM_READ_WRITE, sizeof(int)*2);
    cl::Buffer buffer_anim(context, CL_MEM_READ_WRITE, sizeof(int));

    queue.enqueueWriteBuffer(buffer_dims, CL_TRUE, 0, sizeof(int)*2, dims);
    queue.enqueueWriteBuffer(buffer_anim, CL_TRUE, 0, sizeof(int), &anim);

    int error = 0;

    glGenBuffers(1, &PBO);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, sizeof(color)*3840*2160, NULL, GL_STATIC_DRAW);

    cl_mem pbo_mem;
    unsigned char* hostFallbackBuffer;
    if (!FALLBACK){
        pbo_mem = clCreateFromGLBuffer(context(), CL_MEM_WRITE_ONLY, PBO, &error);
    } else {
        pbo_mem = clCreateBuffer(context(), CL_MEM_WRITE_ONLY, sizeof(color)*3840*2160, NULL, NULL);
        hostFallbackBuffer = new unsigned char[sizeof(color)*3840*2160];
        // glBindBuffer(GL_COPY_WRITE_BUFFER, PBO); something to look at later
    }
    cl::Buffer pbo_buff(pbo_mem);

    cl::Kernel create_gradient(program, "create_gradient");
    create_gradient.setArg(0, pbo_buff);
    create_gradient.setArg(1, buffer_dims);
    create_gradient.setArg(2, buffer_anim);


    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glGenFramebuffers(1, &fboId);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    //Main loop

    MouseInputService input(window);

    while (!glfwWindowShouldClose(window)){
        processInput(window);


        EventInfo info = input.Query();

        fprintf(stdout, "Event : %d\tPosition : (%f, %f)\n", info.type, info.x, info.y);

        dims[0] = width;
        dims[1] = height;
        anim++;
        queue.enqueueWriteBuffer(buffer_dims, CL_TRUE, 0, sizeof(int)*2, dims);
        queue.enqueueWriteBuffer(buffer_anim, CL_TRUE, 0, sizeof(int), &anim);


        if(!FALLBACK){
            clEnqueueAcquireGLObjects(queue(), 1, &pbo_mem, 0, NULL, NULL);
            queue.enqueueNDRangeKernel(create_gradient, cl::NullRange, cl::NDRange(width, height), cl::NDRange(16, 16));
            clEnqueueReleaseGLObjects(queue(), 1, &pbo_mem, 0, NULL, NULL);
            queue.finish();
        } else {
            queue.enqueueNDRangeKernel(create_gradient, cl::NullRange, cl::NDRange(width, height));
            clEnqueueReadBuffer(queue(), pbo_mem, CL_TRUE, 0, sizeof(color)*width*height, hostFallbackBuffer, 0, NULL, NULL);
            queue.finish();
            glBindBuffer(GL_ARRAY_BUFFER, PBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(color)*width*height, hostFallbackBuffer);
        }

        glClear(GL_COLOR_BUFFER_BIT);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        glfwSwapBuffers(window);
        glfwPollEvents();

        error = glGetError();
        if (error != GL_NO_ERROR) {
            std::printf("OpenGL error: %d\n", error);
        }
    }

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDeleteBuffers(1, &PBO);
    if(FALLBACK){
        delete[] hostFallbackBuffer;
    }
    glDeleteTextures(1, &texture);

    glfwDestroyWindow(window);

}

GLFWwindow* initializeGLFW(uint height, uint width){
    if (!glfwInit()){
        std::printf("Failed to initialize GLFW!\n");
        return nullptr;
    }
    glfwSetErrorCallback(glfwErrorCallback);

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


cl::Device getDefaultClDevice(){
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);

    if (all_platforms.empty()){
        std::fprintf(stderr, "No platforms found. Check OpenCL installation!\n");
        return cl::Device();
    }

    int selection=0;

    for(int i=0; i<all_platforms.size(); i++){
        printf("[%d] :\t%s\n",i,all_platforms[i].getInfo<CL_PLATFORM_NAME>().c_str());
    }
    // scanf("%d", &selection);

    cl::Platform default_platform = all_platforms[selection];
    std::printf("Using platform:\t%s\n", default_platform.getInfo<CL_PLATFORM_NAME>().c_str());

    std::vector<cl::Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);


    if (all_devices.size() == 0){
        std::fprintf(stderr, "No devices found. Check OpenCL installation!\n");
        return cl::Device();
    }

    for(int i=0; i<all_devices.size(); i++){
        printf("[%d] :\t%s\n",i,all_devices[i].getInfo<CL_DEVICE_NAME>().c_str());
    }
    // scanf("%d", &selection);

    cl::Device default_device;

#if __APPLE__
   default_device = all_devices[2];
#else
   default_device = all_devices[selection];
#endif

    std::printf("Using device:\t%s\n", default_device.getInfo<CL_DEVICE_NAME>().c_str());

    return default_device;
}

cl::Program compileCopyKernel(cl::Context context, cl::Device default_device){
    cl::Program::Sources sources;
    std::string color_structure =
        "   struct color {"
        "       unsigned char R, G, B;"
        "   };";

    std::string kernel_code =
        "   void kernel create_gradient(global struct color* A, global const int* dims, global const int* anim){"
        "       int ID, Nthreads, width, height;"
        "       float gradientStepX, gradientStepY;"
        ""
        "       ID = get_global_id(0);"
        "       Nthreads = get_global_size(0);"
        "       width = dims[0];"
        "       height = dims[1];"
        ""
        "       gradientStepX  = 3.14f / (float)width;"
        "       gradientStepY  = 3.14f / (float)height;"
        ""
        "       for (int i = ID; i < height; i += Nthreads){"
        "           for (int j = 0; j < width; j++){"
        "               A[i*width + j].R = (sin((i+*anim)*gradientStepY) + 1) * 127;"
        "               A[i*width + j].G = (cos((j+*anim*2)*gradientStepX) + 1) * 127;"
        "               A[i*width + j].B = (sin(((i+*anim*3)*gradientStepY + (j+*anim)*gradientStepX)/2) + 1) * 127;"
        "           }"
        "       }"
        "   }";
    std::string kernel2_code =
        "   void kernel create_gradient(global struct color* A, global const int* dims, global const int* anim){"
        "       int IDX, IDY, global_ID, anim2;"
        "       float gradientStepX, gradientStepY;"
        ""
        "       IDX = get_global_id(0);"
        "       IDY = get_global_id(1);"
        "       global_ID = IDX + IDY * get_global_size(0);"
        ""
        "       anim2 = *anim * (get_global_size(0) + get_global_size(1)) / 2 /1000;"
        "       gradientStepX  = 3.14159263f / (float)get_global_size(0);"
        "       gradientStepY  = 3.14159263f / (float)get_global_size(1);"
        ""
        "       A[global_ID].R = (sin((IDY+anim2)*gradientStepY) + 1) * 127;"
        "       A[global_ID].G = (cos((IDX+anim2*2)*gradientStepX) + 1) * 127;"
        "       A[global_ID].B = (sin(((IDY-anim2*2)*gradientStepY + (IDX-anim2)*gradientStepX)/2) + 1) * 127;"
        "   }";

    sources.push_back({color_structure.c_str(), color_structure.length()});
    sources.push_back({kernel2_code.c_str(), kernel2_code.length()});
    cl::Program program(context, sources);

    if (program.build() != CL_SUCCESS) {
        std::printf("Error building: %s\n", program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device).c_str());
        return cl::Program();
    }

    return program;
}

void glfwErrorCallback(int error, const char* description){
    printf("Error: %s\n", description);
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h){
#ifndef __APPLE__
    for (; w%4; w++);
#endif
    width = w;
    height = h;

    glViewport(0, 0, width, height);

#ifdef __APPLE__
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
#endif

}

void processInput(GLFWwindow *window){
    static bool pressed = false;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}
