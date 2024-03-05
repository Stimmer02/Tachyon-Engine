#include <iostream>
#include "Component.h"
using namespace std;

struct Node {
    Component component;

    Node* parent;
    Node* left;
    Node* right;
    
    int color;
};

class RedBlackTree {
protected:
    // Root.
    Node* root;
    // Sneaky way of having node-like None.
    Node* nodeNull;

public:

}
