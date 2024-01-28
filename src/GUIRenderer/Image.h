#ifndef IMAGE_H
#define IMAGE_H

#include "Color.h"

struct Image {
    uint32_t width;
    uint32_t height;
    uint32_t image_size;
    int32_t x_pixels_per_m;
    int32_t y_pixels_per_m;
    Color * pixels;
};


#endif
