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
    RedBlackTree() {
        nodeNull = new Node();
        nodeNull->left = nullptr;
        nodeNull->right = nullptr;
        // Black. Root and leaves are always black (will make sense in a few lines).
        nodeNull->color = 0;

        // We want to have an empty root, but no irritating errors.
        root = nodeNull;
    }
}
