#include "ImageEditor.h"

std::vector<Image> ImageEditor::Split(const Image & image, const uint32_t & slice_width, const uint32_t & slice_height){
    std::vector<Image> images;

    uint32_t arraySize = (image.width / slice_width) * (image.height / slice_height);

    int height = image.width / slice_width;

    for(unsigned int i = 0; i < arraySize; ++i){
        Image temp;
        temp.height = slice_height;
        temp.width = slice_width;
        temp.pixels = new Color[slice_height *slice_width];

        for(unsigned int j = 0 ; j < slice_height ; ++j){
            for(unsigned int k = 0; k < slice_width; ++k){
                temp.pixels[j * slice_width + k] = image.pixels[
                    (i % height) * slice_width
                    + j * image.width
                    + i / height * slice_height * image.width
                    + k];
            }
        }
        images.emplace_back(temp);
    }

    return images;
}
