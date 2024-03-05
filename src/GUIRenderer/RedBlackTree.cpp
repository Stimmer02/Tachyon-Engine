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

    void rotationLeft(Node* center) {
        Node* rightNode = center->right;

        // Left son of right transfered to center's right son.
        center->right = rightNode->left;
        if (rightNode->left != nodeNull) {
            rightNode->left->parent = center;
        }

        // Making right node into new center.
        rightNode->parent = center->parent;
        // If center is root.
        if (center->parent == nullptr) {
            this->root = rightNode;
        }
        // If center is left son.
        else if (center == center->parent->left) {
            center->parent->left = rightNode;
        }
        // If center is right son.
        else {
            center->parent->right = rightNode;
        }

        // Making left node into new center.
        rightNode->left = center;
        center->parent = rightNode;
    }

    // The same as left rotate, but everything other way around.
    void rotationRight(Node* center) {
        Node* leftNode = center->left;

        // Right son of left transfered to center's left son.
        center->left = leftNode->right;
        if (leftNode->right != nodeNull) {
            leftNode->right->parent = center;
        }

        // Making left node into new center.
        leftNode->parent = center->parent;
        // If center is root.
        if (center->parent == nullptr) {
            this->root = leftNode;
        }
        // If center is left son.
        else if (center == center->parent->left) {
            center->parent->left = leftNode;
        }
        // If center is right son.
        else {
            center->parent->right = leftNode;
        }

        // Making right node into new center.
        leftNode->right = center;
        center->parent = leftNode;
    }
}
