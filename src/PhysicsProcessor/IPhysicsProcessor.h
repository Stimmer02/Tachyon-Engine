#ifndef _IPHYSICSPROCESSOR_H
#define _IPHYSICSPROCESSOR_H

#define CL_HPP_TARGET_OPENCL_VERSION 200

#ifdef APPLE

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
struct attribute ((packed)) color{
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;
};
#endif
#endif

struct attribute ((aligned)) vector2D{
    cl_uint x;
    cl_uint y;
};

struct attribute ((aligned)) voxel{
    cl_uint substanceID;
    struct vector2D forceVector;
};

struct attribute ((aligned)) chunk{
    struct voxel* voxels;
};

struct attribute ((aligned)) substance{
    struct color color;
    float mass;
    float jammingFactor;
};

struct attribute ((aligned)) engineResources{
    struct substanceTable* substanceTable;
    struct chunk* worldMap;
    struct color* PBO;
};

struct attribute ((aligned)) engineConfig{
    cl_uint simulationWidth;
    cl_uint simulationHeight;
    float gravity;
    float timefactor;
    float atmosphereViscosity;
};

struct attribute ((aligned)) substanceTable{
    struct substance* substances;
};

class IPhysicsProcessor{
public:
    ~IPhysicsProcessor(){};
    virtual void generateFrame() = 0;
    virtual void spawnVoxel(uint x, uint y, uint substanceID) = 0;
    virtual uint countVoxels() = 0;
};

#endif

#ifndef _ENGINECONFIG_H
#define _ENGINECONFIG_H

#include <CL/opencl.hpp>
typedef unsigned int uint;

struct engineConfig{
    uint simulationWidth;
    uint simulationHeight;
    float gravity;
    float timefactor;
    float atmosphereViscosity;
};

#endif
