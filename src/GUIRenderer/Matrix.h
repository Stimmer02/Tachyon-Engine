#ifndef MATRIX_H
#define MATRIX_H

#include <cstring>
#include <cassert>
#include <stdio.h>
#include <math.h>
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

    #ifdef VECTORIZED

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

    Matrix operator*(Matrix & othermatrix) const{

        Matrix result;
        result.Zeros();

        float * data = result.Data();
        float * src = othermatrix.Data();

    #ifdef VECTORIZED

        __m128 row1 = _mm_loadu_ps(src);
        __m128 row2 = _mm_loadu_ps(src + 4);
        __m128 row3 = _mm_loadu_ps(src + 8);
        __m128 row4 = _mm_loadu_ps(src + 12);

        __m128 tmp1 = _mm_unpacklo_ps(row1, row2);
        __m128 tmp2 = _mm_unpacklo_ps(row3, row4);
        __m128 tmp3 = _mm_unpackhi_ps(row1, row2);
        __m128 tmp4 = _mm_unpackhi_ps(row3, row4);

        row1 = _mm_movelh_ps(tmp1, tmp2);
        row2 = _mm_movehl_ps(tmp2, tmp1);
        row3 = _mm_movelh_ps(tmp3, tmp4);
        row4 = _mm_movehl_ps(tmp4, tmp3);

        for (int i = 0; i < 4; ++i) {
            __m128 cur_row = _mm_loadu_ps(matrix + i * 4);

            __m128 prod1 = _mm_mul_ps(cur_row, row1);
            __m128 prod2 = _mm_mul_ps(cur_row, row2);
            __m128 prod3 = _mm_mul_ps(cur_row, row3);
            __m128 prod4 = _mm_mul_ps(cur_row, row4);

            __m128 sum1 = _mm_hadd_ps(prod1, prod1);
            sum1 = _mm_hadd_ps(sum1, sum1);

            __m128 sum2 = _mm_hadd_ps(prod2, prod2);
            sum2 = _mm_hadd_ps(sum2, sum2);

            __m128 sum3 = _mm_hadd_ps(prod3, prod3);
            sum3 = _mm_hadd_ps(sum3, sum3);

            __m128 sum4 = _mm_hadd_ps(prod4, prod4);
            sum4 = _mm_hadd_ps(sum4, sum4);

            __m128 final_result = _mm_set_ps(sum4[0], sum3[0], sum2[0], sum1[0]);
            _mm_storeu_ps(&result[i * 4], final_result);
        }

    #else

        for(int i=0; i<4; ++i)
            for(int j=0; j<4; ++j)
                for(int k=0; k<4; ++k)
                    result.matrix[i*4+j] += matrix[i*4+k] * othermatrix.matrix[k*4+j];

    #endif

        return result;
    }

    Matrix operator*(const float & scalar) const{

        Matrix result;

        float * data = result.Data();

    #ifdef VECTORIZED

        __m128 number = _mm_set1_ps(scalar);

        __m128 row1 = _mm_loadu_ps(matrix);
        __m128 row2 = _mm_loadu_ps(matrix + 4);
        __m128 row3 = _mm_loadu_ps(matrix + 8);
        __m128 row4 = _mm_loadu_ps(matrix + 12);

        __m128 result1 = _mm_mul_ps(row1, number);
        __m128 result2 = _mm_mul_ps(row2, number);
        __m128 result3 = _mm_mul_ps(row3, number);
        __m128 result4 = _mm_mul_ps(row4, number);

        _mm_storeu_ps(data, result1);
        _mm_storeu_ps(data+4, result2);
        _mm_storeu_ps(data+8, result3);
        _mm_storeu_ps(data+12, result4);

    #else

        for(int i=0; i<4; ++i)
            for(int j=0; j<4; ++j)
                result.matrix[i*4+j] = matrix[i*4+j] * scalar;

    #endif

        return result;
    }

    Matrix operator+(Matrix & othermatrix) const{

        Matrix result;

        float * data = result.Data();
        float * src = othermatrix.Data();

    #ifdef VECTORIZED

        __m128 row1a = _mm_loadu_ps(matrix);
        __m128 row2a = _mm_loadu_ps(matrix + 4);
        __m128 row3a = _mm_loadu_ps(matrix + 8);
        __m128 row4a = _mm_loadu_ps(matrix + 12);

        __m128 row1b = _mm_loadu_ps(src);
        __m128 row2b = _mm_loadu_ps(src + 4);
        __m128 row3b = _mm_loadu_ps(src + 8);
        __m128 row4b = _mm_loadu_ps(src + 12);

        _mm_storeu_ps(data, _mm_add_ps(row1a, row1b));
        _mm_storeu_ps(data+4, _mm_add_ps(row2a, row2b));
        _mm_storeu_ps(data+8, _mm_add_ps(row3a, row3b));
        _mm_storeu_ps(data+12, _mm_add_ps(row4a, row4b));

    #else

        for(int i=0; i<4; ++i)
            for(int j=0; j<4; ++j)
                result.matrix[i*4+j] = matrix[i*4+j] + othermatrix.matrix[i*4+j];

    #endif

        return result;
    }

    Matrix operator-( Matrix & othermatrix) const{

        Matrix result;

        float * data = result.Data();
        float * src = othermatrix.Data();

    #ifdef VECTORIZED

        __m128 row1a = _mm_loadu_ps(matrix);
        __m128 row2a = _mm_loadu_ps(matrix + 4);
        __m128 row3a = _mm_loadu_ps(matrix + 8);
        __m128 row4a = _mm_loadu_ps(matrix + 12);

        __m128 row1b = _mm_loadu_ps(src);
        __m128 row2b = _mm_loadu_ps(src + 4);
        __m128 row3b = _mm_loadu_ps(src + 8);
        __m128 row4b = _mm_loadu_ps(src + 12);

        _mm_storeu_ps(data, _mm_sub_ps(row1a, row1b));
        _mm_storeu_ps(data+4, _mm_sub_ps(row2a, row2b));
        _mm_storeu_ps(data+8, _mm_sub_ps(row3a, row3b));
        _mm_storeu_ps(data+12, _mm_sub_ps(row4a, row4b));

    #else

        for(int i=0; i<4; ++i)
            for(int j=0; j<4; ++j)
                result.matrix[i*4+j] = matrix[i*4+j] - othermatrix.matrix[i*4+j];

    #endif

        return result;
    }

    void operator=(const Matrix & othermatrix){
        memcpy(matrix, othermatrix.matrix, 16 * sizeof(float));
    }

    float * Data(){
        return matrix;
    }

    void Show() const{
        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                printf("%f ", matrix[i * 4 +j]);
            }
            printf("\n");
        }
        printf("\n");
    }

};


#endif
