#ifndef MATRIX_H
#define MATRIX_H

#include <cstring>
#include <cassert>

class Matrix{

private:

    float matrix[16] = {};

public:

    Matrix(){
        loadIdentity();
    }

    Matrix(const Matrix & otherMatrix){
        memcpy(matrix, otherMatrix.matrix, 16 * sizeof(float));
    }

    void loadIdentity(){
        memset(matrix, 0, 16 * sizeof(float));
        matrix[0] = 1.0f;
        matrix[5] = 1.0f;
        matrix[10] = 1.0f;
        matrix[15] = 1.0f;
    }

    void zeros(){
        memset(matrix, 0, 16 * sizeof(float));
    }

    Matrix transpose() const {

        Matrix result;

        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                result.matrix[i * 4 + j] = matrix[j * 4 + i];

        return result;
    }

    float& operator[](const int & index){
        assert(index < 16 && "Index out of bound");
        return matrix[index];
    }

    void luDecompositon(Matrix & L, Matrix & U){

        L.zeros();
        U.zeros();

        for (int i = 0; i < 4; ++i) {

            for (int k = i; k < 4; ++k) {
                float sum = 0;

                for (int j = 0; j < i; ++j)
                    sum += L.matrix[i * 4 + j] * U.matrix[j * 4 + k];

                U.matrix[i * 4 + k] = matrix[i * 4 + k] - sum;
            }

            L.matrix[i * 4 + i] = 1;

            for (int k = i + 1; k < 4; ++k) {
                float sum = 0;

                for (int j = 0; j < i; ++j)
                    sum += L.matrix[k * 4 + j] * U.matrix[j * 4 +i];

                L.matrix[k * 4 + i] = (matrix[k * 4 + i] - sum) / U.matrix[i * 4 + i];
            }

        }

    }

    float determinant() {

        float det1 = matrix[0] * matrix[5] - matrix[1] * matrix[4];
        float det2 = matrix[2] * matrix[7] - matrix[3] * matrix[6];
        float det3 = matrix[8] * matrix[13] - matrix[9] * matrix[12];
        float det4 = matrix[10] * matrix[15] - matrix[11] * matrix[14];

        float det = det1 * det4 - det2 * det3;

        return det;
    }

    Matrix inverse() {
        Matrix result;

        float det = determinant();

        assert(det != 0.0f && "Zero determinant");

        Matrix L, U;
        luDecompositon(L, U);

        Matrix y;

        for (int j = 0; j < 4; ++j) {

            for (int i = 0; i < 4; ++i) {
                float sum = 0;

                for (int k = 0; k < i; ++k)
                    sum += L.matrix[i * 4 + k] * y.matrix[k * 4 + j];

                y.matrix[i * 4 + j] = (i == j) ? 1 - sum : -sum;
            }

            for (int i = 3; i >= 0; --i) {
                float sum = 0;

                for (int k = i + 1; k < 4; ++k)
                    sum += U.matrix[i * 4 + k] * result.matrix[k * 4 + j];

                result.matrix[i * 4 + j] = (y.matrix[i * 4 + j] - sum) / U.matrix[i * 4 + i];
            }
        }

        return result;
    }


    Matrix operator*(const Matrix & othermatrix) const{

        Matrix result = Matrix();

        for(int i=0; i<4; ++i)
            for(int j=0; j<4; ++j)
                for(int k=0; k<4; ++k)
                    result.matrix[i*4+j] += matrix[i*4+k] * othermatrix.matrix[k*4+j];

        return result;
    }

    Matrix operator+(const Matrix & othermatrix) const{

        Matrix result = Matrix();

        for(int i=0; i<4; ++i)
            for(int j=0; j<4; ++j)
                result.matrix[i*4+j] = matrix[i*4+j] + othermatrix.matrix[i*4+j];

        return result;
    }

    Matrix operator-(const Matrix & othermatrix) const{

        Matrix result = Matrix();

        for(int i=0; i<4; ++i)
            for(int j=0; j<4; ++j)
                result.matrix[i*4+j] = matrix[i*4+j] - othermatrix.matrix[i*4+j];

        return result;
    }

    float * data(){
        return matrix;
    }

};


#endif
