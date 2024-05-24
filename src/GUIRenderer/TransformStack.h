#ifndef TRANSFORMSTACK_H
#define TRANSFORMSTACK_H

#include "Matrix.h"
#include <stack>

static std::stack<Matrix> matrices;

class TransformStack{
public:

    /// @brief Initializes transformation stack with identity matrix
    static void Initialize(){

        while( !matrices.empty() )
            matrices.pop();

        Matrix identity;
        identity.LoadIdentity();
        matrices.push(identity);

    }

    /// @brief Creates copy of matrix at the top and pushes it to the stack
    static void Push(){
        Matrix atTopMatrix = matrices.top();
        matrices.push(atTopMatrix);
    }

    /// @brief Multiplies top matix with orthogonal projection matrix. Action occurs only if stack is initialized
    /// @param left
    /// @param right
    /// @param bottom
    /// @param top
    /// @param near
    /// @param far
    static void Ortho(const float & left, const float & right, const float &bottom, const float & top, const float & near, const float & far){

        if( matrices.size() < 2 )
            return;

        Matrix& atTopMatrix = matrices.top();
        Matrix ortho;

        float * data = ortho.Data();

        data[0] = 2.0f / (right - left);
        data[3] = - (right+left)/(right-left);
        data[5] = 2.0f / (top - bottom);
        data[7] = - (top+bottom)/(top-bottom);
        data[10] = -2.0f /(far - near);
        data[11] = - (far + near)/(far - near);
        data[15] = 1.0f;

        atTopMatrix = atTopMatrix * ortho;
    }

    /// @brief Multiplies top matix with frustum projection matrix. Action occurs only if stack is initialized
    /// @param left
    /// @param right
    /// @param bottom
    /// @param top
    /// @param near
    /// @param far
    static void Frustum(const float & left, const float & right, const float &bottom, const float & top, const float & near, const float & far){

        if( matrices.size() < 2 )
            return;

        Matrix& atTopMatrix = matrices.top();
        Matrix frustum;

        float * data = frustum.Data();

        data[0] = 2.0f * near / (right - left);
        data[2] = (right+left)/(right-left);
        data[5] = 2.0f * near / (top - bottom);
        data[6] = (top + bottom)/(top-bottom);
        data[10] = -(far + near) /(far - near);
        data[11] = - 2.0f * far * near/(far - near);
        data[14] = -1.0f;

        atTopMatrix = atTopMatrix * frustum;
    }

    /// @brief Multiplies top matix with translation matrix. Action occurs only if stack is initialized
    /// @param xOffset
    /// @param yOffset
    /// @param zOffset
    static void Translate(const float & xOffset, const float & yOffset, const float & zOffset){

        if( matrices.size() < 2 )
            return;

        Matrix& atTopMatrix = matrices.top();
        Matrix translation;

        float * data = translation.Data();

        data[3] = xOffset;
        data[7] = yOffset;
        data[11] = zOffset;

        atTopMatrix = atTopMatrix * translation;
    }

    /// @brief Multiplies top matix with scaling matrix. Action occurs only if stack is initialized
    /// @param xScale
    /// @param yScale
    /// @param zScale
    static void Scale(const float & xScale, const float & yScale, const float & zScale){

        if( matrices.size() < 2 )
            return;

        Matrix& atTopMatrix = matrices.top();
        Matrix scaling;

        float * data = scaling.Data();

        data[0] = xScale;
        data[5] = yScale;
        data[10] = zScale;
        data[15] = 1.0f;

        atTopMatrix = atTopMatrix * scaling;
    }

    /// @brief Multiplies top matix with rotation matrix. Action occurs only if stack is initialized. If |XYZ|!=1 then vector will be normalized
    /// @param angle Angle of rotation
    /// @param xAxis X coordinate of rotation axis
    /// @param yAxis Y coordinate of rotation axis
    /// @param zAxis Z coordinate of rotation axis
    static void Rotate( const float & angle, float xAxis, float yAxis, float zAxis){

        if( matrices.size() < 2 )
            return;

        float length = sqrtf(xAxis * xAxis + yAxis * yAxis + zAxis * zAxis);

        assert( length != 0.0f && "Length of vector can't be equal to 0");

        if( length != 1){
            xAxis/=length;
            yAxis/=length;
            zAxis/=length;
        }


        Matrix& atTopMatrix = matrices.top();
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

        atTopMatrix = atTopMatrix * rotation;

    }

    /// @brief Multiplies matrix at the top of stack with provided matrix
    static void MulMatrix(Matrix & other){

        if( matrices.size() < 2)
            return;

        Matrix& b = matrices.top();

        b = b * other;

    }

    /// @brief Transposes matrix at the top of stack
    static void Transpose(){

        if( matrices.size() < 2)
            return;

        Matrix& b = matrices.top();

        b = b.Transpose();
    }

    /// @brief Replaces matrix at the top of stack with its inverse
    static void Inverse(){

        if( matrices.size() < 2)
            return;

        Matrix& b = matrices.top();

        b = b.Inverse();

    }

    /// @brief Returns copy of matrix at the top
    /// @return Copy of the top matrix or blank matrix if stack is empty
    static Matrix Top(){

        if( !matrices.empty() )
            return matrices.top();

        return Matrix();
    }

    /// @brief Pops matrix at the top of the stack
    static void Pop(){
        if( matrices.size() > 1)
            matrices.pop();
    }



};

#endif
