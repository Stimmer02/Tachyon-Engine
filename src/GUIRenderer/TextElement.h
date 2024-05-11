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
    Vector3 color;

public:

    TextElement(const std::string & text, const Vector3 & color = Vector3(1.0f, 1.0f, 1.0f)){
        this->glyfs = Converter::stringToGlyfList(text);
        this->color = color;
        this->transform.scale = Vector3(0.5f, 0.5f, 0.5f);
    }

    void Pop(){
        if( glyfs.empty() )
            return;

        glyfs.pop_back();
    }

    void Push(Glyf * glyf){
        glyfs.push_back(glyf);
    }

    void SetColor(const Vector3 & color ){
        this->color = color;
    }

    void SetFontSize(const float & ptSize = 12, const float & minScale = 0.1f, const float & maxScale=1000.0f){
        float scale = ptSize/48.0f;
        float normalzied = std::min( std::max(minScale, scale), maxScale);
        transform.scale = Vector3(normalzied, normalzied, normalzied);
    }

    void Render() override{

        GLShader * shader = currentShader;

        shader->TransferToShader("u_color", color);
        Matrix localModel;
        Vector3 position = this->transform.position;

        for(Glyf * glyf : glyfs){

            if(glyf->character == '\n'){
                position.x = transform.position.x;
                position.y -= glyf->height * transform.scale.y;
                continue;
            }

            Matrix translation = Matrix::Translate(position.x, position.y, position.z);
            Matrix rotation = Matrix::Rotate(transform.rotation);
            Matrix scale = Matrix::Scale(transform.scale.x, transform.scale.y, transform.scale.z);

            localModel = translation * rotation * scale;

            currentShader->TransferToShader("u_model", localModel);

            glyf->Render();

            position.x += glyf->width * transform.scale.y;

        }

    };

};


#endif
