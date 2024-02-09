#ifndef TEXT_ASSEMBLER_H
#define TEXT_ASSEMBLER_H

#include "Sprite.h"
#include <vector>
#include <map>

class TextAssembler{
private:

    std::map<char, Sprite*> charset;

public:

    void CreateCharset(const Image * _images, const uint32_t & _characterCount);

    std::vector<Sprite*> BuildText(const char * _text);

    ~TextAssembler();
};


#endif
