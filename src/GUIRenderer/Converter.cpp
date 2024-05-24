#include <Converter.h>

std::map<char, Glyf*> Converter::letterGlyf;
Sprite** Converter::sprites;
Glyf** Converter::glyfs;
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
        if(Converter::letterGlyf.count(str[i]) == 0){
            result.push_back(Converter::letterGlyf[' ']);
        }
        else{
            result.push_back(Converter::letterGlyf[str[i]]);
        }
    }
    

    
    return result;
}

void Converter::setDeatfulFont(){
    setFontFromFile("resources/sprites/abc.bmp", (uint32_t)7, (uint32_t)9);
}

void Converter::setFontFromFile(const std::string &path, const uint32_t &slice_width, const uint32_t &slice_height){
    ImageEditor ie;
    Image helper;
    Converter::width = slice_width;
    Converter::height = slice_height;
    helper = BitmapReader::ReadFile(path.c_str());
    Converter::letters = ie.Split(helper, slice_width, slice_height, Converter::lettersCount);
    makeGlyfArray();
}

void Converter::makeGlyfArray(){
    if(sprites != NULL){
        delete [] sprites;
    }
    if(glyfs != NULL){
        delete [] glyfs;
    }
    Converter::sprites = new Sprite*[lettersCount];
    Converter::glyfs = new Glyf*[lettersCount];
    for(int i = 0; i < lettersCount; ++i){
        Converter::sprites[i] = new Sprite(&(letters[i]));
        delete [] letters[i].pixels;
        Converter::glyfs[i] = new Glyf(Converter::possibleSymbols[i], 10 * Converter::width, 10 * Converter::height, Converter::sprites[i]);
        Converter::letterGlyf[Converter::possibleSymbols[i]] = glyfs[i];
        
        if(i == possibleSymbols.size()){
            lettersCount = i + 1;
            break;
        }
    }
    
}