#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

#include "Component.h"
#include "Image.h"
#include "Color.h"

class ImageEditor{
private:
public:
    Image* Split(const Image &lettersInOneImage, const uint32_t &slice_width, const uint32_t &slice_height, uint32_t &lettersArraySize);
};

#endif
