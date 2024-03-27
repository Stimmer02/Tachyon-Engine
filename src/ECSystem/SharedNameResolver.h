#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include <string.h>

using namespace std;

#define ALPHABET_SIZE 26

struct Node {
    Node* next_char[ALPHABET_SIZE];
    const int32_t size_ = 0;
    const void * pointer;
};

class SharedNameResolver {
protected:
    // Root.
    Node* root;

public:
    SharedNameResolver();

    void Emplace(const std::string & _name, const void * _pointer, const int32_t & _size = 1);
    void * Find(const std::string & _name, const int32_t & _elem_size = 0, const int32_t & _element = 0);
    void Resize(const std::string & _name, const int32_t & _size);
};

#endif
