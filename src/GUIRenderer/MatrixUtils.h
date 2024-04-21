#ifndef MATRIXUTILS_H
#define MATRIXUTILS_H

#include "Matrix.h"

namespace MatrixUtils{

    static Matrix Ortho(const float & left, const float & right, const float &bottom, const float & top, const float & near, const float & far){

        Matrix ortho;

        float * data = ortho.Data();

        data[0] = 2.0f / (right - left);
        data[3] = - (right+left)/(right-left);
        data[5] = 2.0f / (top - bottom);
        data[7] = - (top+bottom)/(top-bottom);
        data[10] = -2.0f /(far - near);
        data[11] = - (far + near)/(far - near);
        data[15] = 1.0f;

        return ortho;
    }

    static Matrix Translate(const float & xOffset, const float & yOffset, const float & zOffset){

        Matrix translation;

        float * data = translation.Data();

        data[3] = xOffset;
        data[7] = yOffset;
        data[11] = zOffset;

        return translation;
    }

    static Matrix Scale(const float & xScale, const float & yScale, const float & zScale){

        Matrix scaling;

        float * data = scaling.Data();

        data[0] = xScale;
        data[5] = yScale;
        data[10] = zScale;
        data[15] = 1.0f;

        return scaling;
    }

    Matrix Rotate( const float & angle, float xAxis, float yAxis, float zAxis){

        float length = sqrtf(xAxis * xAxis + yAxis * yAxis + zAxis * zAxis);

        assert( length != 0.0f && "Length of vector can't be equal to 0");

        if( length != 1){
            xAxis/=length;
            yAxis/=length;
            zAxis/=length;
        }

        Matrix rotation;

        float * data = rotation.Data();

        float deg2rad = angle * 3.1415926535f/180.0f;

        float c = cos(deg2rad);
        float s = sin(deg2rad);

        data[0] = xAxis * xAxis * (1 - c) + c;
        data[1] = xAxis * yAxis * (1 - c) - s * zAxis;
        data[2] = xAxis * zAxis * (1 - c) + s * yAxis;

        data[4] = xAxis * yAxis * (1 - c) + s * zAxis;
        data[5] = yAxis * yAxis * (1 - c) + c;
        data[6] = yAxis * zAxis * (1 - c) - s * xAxis;

        data[8] = xAxis * zAxis * (1 - c) - s * yAxis;
        data[9] = yAxis * zAxis * (1 - c) + s * xAxis;
        data[10] = zAxis * zAxis * (1 - c) + c;

        return rotation;

    }



};


#endif
