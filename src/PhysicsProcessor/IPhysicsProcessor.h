#ifndef _IPHYSICSPROCESSOR_H
#define _IPHYSICSPROCESSOR_H

#define CL_HPP_TARGET_OPENCL_VERSION 200

#ifdef __APPLE__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <OpenGL/OpenGL.h>
#include "OpenCL/include/CL/cl_gl.h"
#include "OpenCL/include/CL/cl.hpp"

#else

#include <CL/opencl.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <CL/cl_gl.h>
#include <GL/glx.h>

#endif

#ifndef _COLOR_H
#ifndef COLOR_H
struct color{
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;
};
#endif
#endif

#include "engineConfig.h"

class IPhysicsProcessor{
protected:
    virtual ~IPhysicsProcessor() = 0;
    virtual void generateFrame() = 0;
    virtual void spawnVoxel(uint x, uint y, uint substanceID) = 0;
    virtual uint countVoxels() = 0;
};

#endif
