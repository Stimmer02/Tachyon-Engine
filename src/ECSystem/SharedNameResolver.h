#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include <string.h>
using namespace std;

#define ALPHABET_SIZE 26

struct Node {
    Node* next_char[ALPHABET_SIZE];
    int32_t size;
    void * pointer;

    Node(){

        for(int i = 0; i < ALPHABET_SIZE; i++)
            next_char[i] = nullptr;

        this->pointer = nullptr;
        this->size = 0;
    }
};

class SharedNameResolver {
protected:
    // Root.
    Node * root;

    void DeleteNode(Node * node);

public:
    SharedNameResolver();

    void Emplace(const std::string & _name, const void * _pointer, const int32_t & _size = 1);
    void * Find(const std::string & _name, const int32_t & _elem_size = 0, const int32_t & _element = 0);
    void Resize(const std::string & _name, const int32_t & _size);

    ~SharedNameResolver();
};

#endif
