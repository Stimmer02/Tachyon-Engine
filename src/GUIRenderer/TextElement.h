#ifndef TEXTELEMENT_H
#define TEXTELEMENT_H

#include "Glyf.h"
#include "GUIElement.h"
#include "GLShader.h"
#include "Converter.h"

#include <list>

class TextElement : public GUIElement{

private:

    std::list<Glyf*> glyfs;

public:

    TextElement(const std::string & text){
        glyfs = Converter::stringToGlyfList(text);
        // TODO

    }

    void Pop(){
        if( glyfs.empty() )
            return;

        glyfs.pop_back();
    }

    void Push(Glyf * glyf){
        glyfs.push_back(glyf);
    }

    void Render() override{

        GLShader * shader = currentShader;

        Vector3 position = this->transform.position;

        for(Glyf * glyf : glyfs){

            shader->TransferToShader("u_translation", position);

            glyf->Render();

            position.x += glyf->width;

        }

    };

};


#endif
