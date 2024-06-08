#ifndef PARAMETRIZEDGLSHADER_H
#define PARAMETRIZEDGLSHADER_H

#include "GLShader.h"
#include <vector>

enum class DataType {
    Float,
    Int,
    Vector,
    Matrix
};

class ParametrizedGLShader : public  GLShader {
private:

    std::map< std::string, std::pair<DataType, const void*> > dataEntries;

    void MoveToShader() {

        for (const auto& entry : dataEntries) {

            auto & pair = entry.second;

            if( pair.second == nullptr )
                return;

            switch (pair.first) {
                case DataType::Float:
                    TransferToShader(entry.first, *((float*)pair.second) );
                    break;
                case DataType::Int:
                    TransferToShader(entry.first,  *((int*)pair.second) );
                    break;

            }

        }
    }

public:

    ParametrizedGLShader() : GLShader(){

    }

    void AddData(const std::string & uniformName, DataType type, const void * data ) {
        dataEntries[uniformName] = std::make_pair(type, data);
    }

    void Use() override{
        GLShader::Use();
        MoveToShader();
    }

};


#endif
