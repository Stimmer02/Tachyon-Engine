#ifndef CONVERTER_H
#define CONVERTER_H

#include <Glyf.h>
#include <ImageEditor.h>
#include <map>
#include <list>

class Converter{
private:
    static constexpr std::string_view possibleSymbols = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    static std::map<char, Glyf*> letterGlyf;
    static Sprite** sprites;
    static Glyf** glyfs;
    static Image* letters;
    static uint32_t lettersCount;
    static uint32_t width;
    static uint32_t height;
    static void setDeatfulFont();
    static void makeGlyfArray();
public:
    static std::list<Glyf*> stringToGlyfList(const std::string &str);
    static void setFontFromFile(const std::string &path, const uint32_t &slice_width, const uint32_t &slice_height);
};

#endif