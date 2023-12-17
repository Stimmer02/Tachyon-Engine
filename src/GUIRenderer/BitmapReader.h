#ifndef BITMAPREADER_H
#define BITMAPREADER_H

#include <fstream>

class BitmapReader {

public:

    void ReadFile(const char * filename);


private:

    void ParseData(char * destintation, const char * source, uint32_t & offset, const int32_t length);
    void ParseHeader(const char * source, uint32_t & offset);
    void ParseInfo(const char * source, uint32_t & offset);

};

struct{
    uint16_t signature;
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t dataOffset;
} header;

#endif
