#include "TextAssembler.h"


std::vector<Sprite*> TextAssembler::BuildText(const char * _text){
    std::vector<Sprite *> text;

    for( uint32_t i =0; _text[i]!='\0'; ++i){

        if( charset.find(_text[i]) == charset.end() )
            return text;

        Sprite * letter = charset[ _text[i] ];

        text.emplace_back(letter);

    }


    return text;
}


void TextAssembler::CreateCharset(const Image * _images, const uint32_t & _characterCount){

    char base = ' ';

    for(uint32_t i = 0; i< _characterCount; ++i){
        Sprite * sprite = Sprite::Create(&_images[i]);

        charset.insert({base, sprite});
        base+=1;
    }

}

TextAssembler::~TextAssembler(){

    for( auto pair : charset)
        delete pair.second;

}
