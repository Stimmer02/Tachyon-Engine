#include "SquareMatrix.h"

//podkreśla błąd ale kod się kompiluje
template<typename T>
void SquareMatrix<T>::setArr(T** values,const unsigned int &newSize){
    if(arr != nullptr){
        for(int i = 0; i < size; ++i){
            delete [] arr[i];
        }
        delete [] arr;
    }
    
    size = newSize;
    
    arr = new T*[size];
    for(int i = 0; i < size; ++i){
        arr[i] = new T[size];
        for(int j = 0; j < size; ++j){
            arr[i][j] = values[i][j];
        }
    }
}

template<typename T>
void SquareMatrix<T>::fastSetArrR(const SquareMatrix<T> &sm,const unsigned int &newSize, const unsigned int &rowId){
    if(arr != nullptr){
        for(int i = 0; i < size; ++i){
            delete [] arr[i];
        }
        delete [] arr;
    }
    
    size = newSize;
    
    arr = new T*[size];
    for(int i = 0, q = 0; i < size; ++i){
        arr[i] = new T[size];
        if(i == rowId){
            continue;
        }
        
        for(int j = 1; j < size; ++j){
            arr[q][j - 1] = sm.arr[i][j];
        }
        ++q;
    }
}


template<typename T>
void SquareMatrix<T>::fastSetArrC(const SquareMatrix<T> &sm,const unsigned int &newSize, const unsigned int &colId){
    if(arr != nullptr){
        for(int i = 0; i < size; ++i){
            delete [] arr[i];
        }
        delete [] arr;
    }
    
    size = newSize;
    
    arr = new T*[size];
    for(int i = 1; i < size; ++i){
        arr[i] = new T[size];
        
        for(int j = 0, q = 0; j < size; ++j){
            if(q == colId){
                continue;
            }
            arr[i - 1][q] = sm.arr[i][j];
            ++q;
        }
    }
}


template<typename T>
T SquareMatrix<T>::getDeterminant(){
    if(size == 1){
        return arr[0][0];
    }
    if(size == 2){
        return arr[0][0] * arr[1][1] - arr[1][0] * arr[0][1];
    }
    
    T result = (T)0;
    SquareMatrix<T> helper;
    for(int i = 0; i < size; ++i){
        helper.fastSetArrR(*this, size - 1, i);
        result += arr[i][0] * helper.getDeterminant() * (1 - 2 * (i % 2));
    }
    
    return result;
    
}

template<typename T>
T SquareMatrix<T>::getSubDeterminant(const int &uselessCol){
    SquareMatrix<T> helperMatrix;
    
    helperMatrix.fastSetArrR(*this, size - 1, uselessCol);
    
    return helperMatrix.getDeterminant();

}