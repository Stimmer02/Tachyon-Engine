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
        result.push_back(Converter::letterGlyf[str[i]]);
    }
    
    // std::cout << result.size() << '\n';
    // std::cout.flush();
    // for(auto i : result){
    //     if(i->sprite == NULL){
    //         std::cout <<"ASD";
    //         continue;
    //     }
    //     std::cout << i->sprite->GetTextureID() << ' ' << i->character << ' ' << i->height << ' ' << i->width << '\n';
    //     std::cout.flush();
    // }
    // std::cout.flush();
    
    return result;
}

void Converter::setDeatfulFont(){
    setFontFromFile("resources/sprites/charmap-oldschool_white.bmp", (uint32_t)7, (uint32_t)9);
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
    Converter::sprites = new Sprite*[lettersCount];
    Converter::glyfs = new Glyf*[lettersCount];
    for(int i = 0; i < lettersCount; ++i){
        Converter::sprites[i] = new Sprite(&(letters[i]));
        Converter::glyfs[i] = new Glyf(Converter::possibleSymbols[i], Converter::width, Converter::height, Converter::sprites[i]);
        std::cout << possibleSymbols[i] << '\n';
        Converter::letterGlyf[Converter::possibleSymbols[i]] = glyfs[i];
        if(possibleSymbols[i] == '~'){
            lettersCount = i + 1;
            break;
        }
    }
}