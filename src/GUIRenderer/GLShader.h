#ifndef GLSHADER_H
#define GLSHADER_H

#include "Vector3.h"

#include <GL/glew.h>
#include <fstream>
#include <string>
#include <cassert>
#include <list>
#include <unordered_map>

class GLShader;

static GLShader * currentShader;

class GLShader{
private:

    std::list<GLuint> shaders;
    std::unordered_map<std::string, GLuint> uniforms;

    GLuint shaderProgram;

public:

    GLShader(){

        this->shaderProgram = glCreateProgram();

#ifdef DEBUG

    printf("[DEBUG] Generating new OpenGL shader\n");

#endif

    }

    void LinkShader(const std::string & shaderPath, const GLenum type){

        std::ifstream input(shaderPath);

        assert(input && "File %s can't be loaded or does not exist");

        std::string rawCode;

        std::getline(input, rawCode, '\0');

        input.close();

        const char * rawBytesPtr = rawCode.c_str();
        int numBytes = rawCode.length();

        GLuint shader = glCreateShader(type);

        glShaderSource(shader, 1, &rawBytesPtr, &numBytes);
        glCompileShader(shader);

        GLenum error = glGetError();
        assert(error == GL_NO_ERROR && "Error during shader compilation" );

        glAttachShader(shaderProgram, shader);

        error = glGetError();
        assert(error == GL_NO_ERROR && "Error during shader attachment" );

        shaders.push_back(shader);

    }

    void Build(){

        assert(shaderProgram != 0 && "Program does not exist" );
        glLinkProgram(shaderProgram);

        GLenum error = glGetError();
        assert(error == GL_NO_ERROR && "Error during program linking" );

    }

    void Use(){
        currentShader = this;
        glUseProgram(shaderProgram);
    }

    void Dispose(){
        glUseProgram(0);
    }

    GLuint GetUniformLocation(const std::string & uniformName){

        std::unordered_map<std::string, GLuint>::iterator it = uniforms.find(uniformName);

        GLuint location = -1;

        if( it == uniforms.end()){

            location = glGetUniformLocation(shaderProgram, uniformName.c_str());
            uniforms[uniformName] = location;

        }else{

            location = it->second;

        }

        return location;

    }

    void TransferToShader(const std::string & uniformName, const Vector3 & vector){

        GLuint location = GetUniformLocation(uniformName);

        if( location == -1 )
            return;

        glUniform4fv(location, 1, (GLfloat*)&vector);

    }

    ~GLShader(){

#ifdef DEBUG

    printf("[DEBUG] Disposing OpenGL shader\n");

#endif

        glUseProgram(0);
        glDeleteProgram(shaderProgram);

        for(GLuint shader : shaders){

            if(shader)
                glDeleteShader(shader);

        }
    }

};



#endif
