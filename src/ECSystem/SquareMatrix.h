#ifndef SQUAREMATRIX_H
#define SQUAREMATRIX_H


template <typename T>
struct SquareMatrix
{
    unsigned int size;
    T** arr;
    void setArr(T** values, const unsigned int &newSize);
    void fastSet(const SquareMatrix<T> &sm, const unsigned int &newSize, const unsigned int &rowId);
    T getDeterminant();
};

#endif