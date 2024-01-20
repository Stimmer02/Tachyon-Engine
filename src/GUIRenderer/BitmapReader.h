#ifndef BITMAPREADER_H
#define BITMAPREADER_H

#include <fstream>
#include <cstring>
#include "Image.h"


struct Header{
    uint16_t signature;
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t dataOffset;
};

struct Info{
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t image_size;
    int32_t x_pixels_per_m;
    int32_t y_pixels_per_m;
    uint32_t colors_used;
    uint32_t important_colors;
    uint32_t red_mask;
    uint32_t green_mask;
    uint32_t blue_mask;
    uint32_t alpha_mask;
    uint32_t color_space_type;
    uint32_t red_x;
    uint32_t red_y;
    uint32_t red_z;
    uint32_t green_x;
    uint32_t green_y;
    uint32_t green_z;
    uint32_t blue_x;
    uint32_t blue_y;
    uint32_t blue_z;
    uint32_t gamma_red;
    uint32_t gamma_green;
    uint32_t gamma_blue;
    uint32_t intent;
    uint32_t profile_data;
    uint32_t profile_size;
    uint32_t reserved;
};

class BitmapReader {

public:

    static Image ReadFile(const char * filename);


private:

    static void ParseData(char * destintation, const char * source, uint32_t & offset, const int32_t length);
    static void ParseHeader(const char * source, uint32_t & offset);
    static void ParseInfo(const char * source, uint32_t & offset);
    static void ChangeEndianess(char *data, const uint32_t & length);
};


#endif
