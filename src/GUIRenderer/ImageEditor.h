#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

#include "Image.h"
#include <iostream>
#include <vector>

class ImageEditor{
public:
    static std::vector<Image> Split(const Image & image, const uint32_t & slice_width, const uint32_t & slice_height);
};

#endif
