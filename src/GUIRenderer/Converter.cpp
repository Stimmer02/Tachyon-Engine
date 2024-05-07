#include <Converter.h>

std::map<char, Glyf*> Converter::letterGlyf;
Sprite** Converter::sprites;
Image* Converter::letters;
uint32_t Converter::lettersCount;
uint32_t Converter::width;
uint32_t Converter::height;

std::list<Glyf *> Converter::stringToGlyfList(const std::string &str){
    if(letterGlyf.empty()){
        Converter::setDeatfulFont();
    }
    std::list<Glyf*> result;
    for(int i = 0; i < str.size(); ++i){
        result.push_back(Converter::letterGlyf[str[i]]);
    }
    
    
    return result;
}

void Converter::setDeatfulFont(){
    setFontFromFile("resources/sprites/charmap-oldschool_white.bmp", 7, 9);
}

void Converter::setFontFromFile(const std::string &path, const uint32_t &slice_width, const uint32_t &slice_height){
    ImageEditor ie;
    Image helper;
    Converter::width = slice_width;
    Converter::height = slice_height;
    helper = BitmapReader::ReadFile(path.c_str());
    Converter::letters = ie.Split(helper, slice_width, slice_height, Converter::lettersCount);
}

void Converter::makeGlyfArray(){
    Converter::sprites = new Sprite*[lettersCount];
    for(int i = 0; i < lettersCount; ++i){
        Converter::sprites[i] = new Sprite(&letters[i]);
        Converter::letterGlyf[Converter::possibleSymbols[i]] = new Glyf(Converter::possibleSymbols[i], Converter::width, Converter::height, Converter::sprites[i]);
    }
}