#ifndef HEADERS_H
#define HEADERS_H

#pragma once

#ifdef __APPLE__

#include <GL/glew.h>
#include <OpenGL/CGLCurrent.h>
#include <GLFW/glfw3.h>
#include "../OpenCL/include/CL/cl.hpp"
#include <OpenCL/cl_gl.h>

#elif __WIN32__

typedef unsigned int uint32_t;

#include <windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <CL/opencl.hpp>
#include <CL/cl_gl.h>

#else

#include <CL/opencl.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <CL/cl_gl.h>
#include <GL/glx.h>

#endif
#endif
