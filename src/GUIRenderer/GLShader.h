#ifndef GLSHADER_H
#define GLSHADER_H

#include "Vector3.h"
#include "Matrix.h"

#include <GL/glew.h>
#include <fstream>
#include <string>
#include <cassert>
#include <list>
#include <unordered_map>

class GLShader;

static GLShader * currentShader;
static GLShader * previousShader;

class GLShader{
private:

    std::list<GLuint> shaders;
    std::unordered_map<std::string, GLuint> uniforms;

    GLuint shaderProgram;

    bool CheckProgramStatus(){

        GLint success;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

        if (!success) {

            char infoLog[512];
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
            fprintf(stdout,"Shader linking error : %s", infoLog);

            return false;
        }

        return true;
    }

    bool CheckShaderStatus(GLuint shader){

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {

            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            fprintf(stdout, "Shader compilation error : %s", infoLog);

            return false;
        }

        return true;
    }

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

        if( CheckShaderStatus(shader) == false )
            exit(1);

        glAttachShader(shaderProgram, shader);

        shaders.push_back(shader);

    }

    void Build(){

        assert(shaderProgram != 0 && "Program does not exist" );
        glLinkProgram(shaderProgram);

        if( CheckProgramStatus() == false )
            exit(1);

    }

    void Use(){
        previousShader = currentShader;
        currentShader = this;
        glUseProgram(shaderProgram);
    }

    void Dispose(){
        glUseProgram(0);
        currentShader = previousShader;
        if( currentShader )
            currentShader->Use();
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

    void TransferToShader(const std::string & uniformName, Vector3 & vector){

        GLuint location = GetUniformLocation(uniformName);

        if( location == -1 )
            return;

        glUniform4fv(location, 1, (GLfloat*)&vector);

    }

    void TransferToShader(const std::string & uniformName, Matrix & matrix){

        GLuint location = GetUniformLocation(uniformName);

        if( location == -1 )
            return;

        glUniformMatrix4fv(location, 1, GL_FALSE, matrix.Data());

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