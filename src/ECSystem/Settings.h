#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h>

namespace GraphicConfig{
    const char * windowTitle = "Window";
    int32_t windowWidth = 800;
    int32_t windowHeight = 600;
    bool vsync = true;
    bool zbuffer = true;
};

#endif