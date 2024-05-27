#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h>
#include <string>

namespace GraphicConfig{
    std::string windowTitle;
    int32_t windowWidth;
    int32_t windowHeight;
    bool visible;
    bool resiazble;
    bool vsync;
    bool zbuffer;
    bool useOrthographicProjection;
};

namespace ApplicationConfig{
    bool internalGUIInteraction;
    std::string physicProcessorConfig;
}

#endif
