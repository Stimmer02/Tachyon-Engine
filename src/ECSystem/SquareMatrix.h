#ifndef SQUAREMATRIX_H
#define SQUAREMATRIX_H


template <typename T>
struct SquareMatrix
{
    unsigned int size;
    T** arr;
    void setArr(T** values, const unsigned int &newSize);
    void fastSetArrR(const SquareMatrix<T> &sm, const unsigned int &newSize, const unsigned int &rowId);
    void fastSetArrC(const SquareMatrix<T> &sm, const unsigned int &newSize, const unsigned int &colId);
    T getDeterminant();
    T getSubDeterminant(const int &uselessCol);
};

#include "SquareMatrix.tpp"

#endif