#ifndef MATRIX_H
#define MATRIX_H

#include <cstring>
#include <cassert>
#include <immintrin.h>

class Matrix{

private:

    float matrix[16] = {};

public:

    Matrix(){
        LoadIdentity();
    }

    Matrix(const Matrix & otherMatrix){
        memcpy(matrix, otherMatrix.matrix, 16 * sizeof(float));
    }

    void LoadIdentity(){
        memset(matrix, 0, 16 * sizeof(float));
        matrix[0] = 1.0f;
        matrix[5] = 1.0f;
        matrix[10] = 1.0f;
        matrix[15] = 1.0f;
    }

    void Zeros(){
        memset(matrix, 0, 16 * sizeof(float));
    }

    Matrix Transpose() const {

        Matrix result;

        float * data = result.Data();

    #ifdef __AVXINTRIN_H

        __m128 row1 = _mm_loadu_ps(matrix);
        __m128 row2 = _mm_loadu_ps(matrix + 4);
        __m128 row3 = _mm_loadu_ps(matrix + 8);
        __m128 row4 = _mm_loadu_ps(matrix + 12);

        __m128 tmp1 = _mm_unpacklo_ps(row1, row2);
        __m128 tmp2 = _mm_unpacklo_ps(row3, row4);
        __m128 tmp3 = _mm_unpackhi_ps(row1, row2);
        __m128 tmp4 = _mm_unpackhi_ps(row3, row4);

        _mm_storeu_ps(data, _mm_movelh_ps(tmp1, tmp2));
        _mm_storeu_ps(data+4, _mm_movehl_ps(tmp2, tmp1));
        _mm_storeu_ps(data+8, _mm_movelh_ps(tmp3, tmp4));
        _mm_storeu_ps(data+12, _mm_movehl_ps(tmp4, tmp3));

    #else

        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                data[i * 4 + j] = matrix[j * 4 + i];

    #endif

        return result;
    }

    float& operator[](const int & index){
        assert(index < 16 && "Index out of bound");
        return matrix[index];
    }

    void LUDecompositon(Matrix & L, Matrix & U){

        L.Zeros();
        U.Zeros();

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

    float Determinant() {

        float det1 = matrix[0] * matrix[5] - matrix[1] * matrix[4];
        float det2 = matrix[2] * matrix[7] - matrix[3] * matrix[6];
        float det3 = matrix[8] * matrix[13] - matrix[9] * matrix[12];
        float det4 = matrix[10] * matrix[15] - matrix[11] * matrix[14];

        float det = det1 * det4 - det2 * det3;

        return det;
    }

    Matrix Inverse() {
        Matrix result;

        float det = Determinant();

        assert(det != 0.0f && "Zero determinant");

        Matrix L, U;
        LUDecompositon(L, U);

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

    float * Data(){
        return matrix;
    }

};


#endif
