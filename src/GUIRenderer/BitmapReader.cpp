#include "BitmapReader.h"


void BitmapReader::ChangeEndianess(char *data, const int & length){

  for(int i=0; i < (length>>1); i++){
    char temp = data[length-1-i];
    data[length-1-i] = data[i];
    data[i] = temp;
  }

}

void BitmapReader::ParseData(char * destination, const char *source, unsigned int & offset, const int length){

  memcpy(destination, source + offset, length);
  //ChangeEndianess(destination, length);

  offset += length;

}

void BitmapReader::ParseHeader(const char * source, unsigned int & offset){

    ParseData((char*)&header.signature, source, offset, 2);
    ParseData((char*)&header.fileSize, source, offset, 4);
    ParseData((char*)&header.reserved, source, offset, 4);
    ParseData((char*)&header.dataOffset, source, offset, 4);

}

void BitmapReader::ParseInfo(const char * source, unsigned int & offset){

    ParseData((char*)&infoHeader.size, source, offset, 4);

    if(infoHeader.size!=124){
        fprintf(stderr, "Invalid file format. Only BITMAPV5HEADER are currently handled.\n");
        return ;
    }

    ParseData((char*)&infoHeader.width, source, offset, 4);
    ParseData((char*)&infoHeader.height, source, offset, 4);
    ParseData((char*)&infoHeader.planes, source, offset, 2);
    ParseData((char*)&infoHeader.bits_per_pixel, source, offset, 2);
    ParseData((char*)&infoHeader.compression, source, offset, 4);
    ParseData((char*)&infoHeader.image_size, source, offset, 4);
    ParseData((char*)&infoHeader.x_pixels_per_m, source, offset, 4);
    ParseData((char*)&infoHeader.y_pixels_per_m, source, offset, 4);
    ParseData((char*)&infoHeader.colors_used, source, offset, 4);
    ParseData((char*)&infoHeader.important_colors, source, offset, 4);
    ParseData((char*)&infoHeader.red_mask, source, offset, 4);
    ParseData((char*)&infoHeader.green_mask, source, offset, 4);
    ParseData((char*)&infoHeader.blue_mask, source, offset, 4);
    ParseData((char*)&infoHeader.alpha_mask, source, offset, 4);
    ParseData((char*)&infoHeader.color_space_type, source, offset, 4);
    ParseData((char*)&infoHeader.red_x, source, offset, 4);
    ParseData((char*)&infoHeader.red_y, source, offset, 4);
    ParseData((char*)&infoHeader.red_z, source, offset, 4);
    ParseData((char*)&infoHeader.green_x, source, offset, 4);
    ParseData((char*)&infoHeader.green_y, source, offset, 4);
    ParseData((char*)&infoHeader.green_z, source, offset, 4);
    ParseData((char*)&infoHeader.blue_x, source, offset, 4);
    ParseData((char*)&infoHeader.blue_y, source, offset, 4);
    ParseData((char*)&infoHeader.blue_z, source, offset, 4);
    ParseData((char*)&infoHeader.gamma_red, source, offset, 4);
    ParseData((char*)&infoHeader.gamma_green, source, offset, 4);
    ParseData((char*)&infoHeader.gamma_blue, source, offset, 4);
    ParseData((char*)&infoHeader.intent, source, offset, 4);
    ParseData((char*)&infoHeader.profile_data, source, offset, 4);
    ParseData((char*)&infoHeader.profile_size, source, offset, 4);
    ParseData((char*)&infoHeader.reserved, source, offset, 4);

}


void BitmapReader::ReadFile(const char * filename){

    std::fstream input;
    input.open(filename, std::ios::in | std::ios::binary);

    if(!input){
        fprintf(stderr, "File can't be opened.\n");
        return;
    }

    //Determine file length
    input.seekg(0, std::ios::end);
    unsigned int file_size = input.tellg();
    input.seekg(0, std::ios::beg);

    //Allocate required space
    char * raw_data = new char[file_size];

    //Read whole file and close stream
    input.read(raw_data, file_size);
    input.close();

    unsigned int offset = 0;

    ParseHeader(raw_data, offset);
    ParseInfo(raw_data, offset);

    fprintf(stdout, "Image dimensions: %d x %d\n", infoHeader.width, infoHeader.height);

    unsigned int bytes_per_pixel = infoHeader.bits_per_pixel/8;
    unsigned int pixel_count = infoHeader.width * infoHeader.height;

    if(bytes_per_pixel!=3){
        fprintf(stderr, "Invalid pixel format.\n");
        delete[] raw_data;
    }

    Color **pixels = new Color*[infoHeader.width];

    for(int i=0; i< infoHeader.width; i++)
        pixels[i] = new Color[infoHeader.height];

    for(int y = 0; y<infoHeader.height; y++){
        for(int x = 0; x<infoHeader.width; x++){
            ParseData((char*)&pixels[x][y], raw_data, offset, sizeof(Color));
        }
    }


    for(int i=0; i< infoHeader.width; i++)
        delete[] pixels[i];

    delete[] pixels;
    delete[] raw_data;

}
