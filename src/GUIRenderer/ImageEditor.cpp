#include "ImageEditor.h"
#include "Color.h"

Image* ImageEditor:: Split(const Image &lettersInOneImage, const uint32_t &slice_width, const uint32_t &slice_height, uint32_t &lettersArraySize){
    Image* result;


    lettersArraySize = (lettersInOneImage.width / slice_width) * (lettersInOneImage.height / slice_height);

    result = new Image[lettersArraySize];

    int heightInLetters = lettersInOneImage.width / slice_width;

    for(unsigned int i = 0; i < lettersArraySize; ++i){
        result[i].height = slice_height;
        result[i].width = slice_width;
        result[i].pixels = new Color[slice_height *slice_width];

        for(unsigned int j = 0 ; j < slice_height ; ++j){
            for(unsigned int k = 0; k < slice_width; ++k){
                result[i].pixels[j * slice_width + k] = lettersInOneImage.pixels[
                    (i % heightInLetters) * slice_width
                    + j * lettersInOneImage.width
                    + i / heightInLetters * slice_height * lettersInOneImage.width
                    + k];
            }
        }
    }



    return result;
}
