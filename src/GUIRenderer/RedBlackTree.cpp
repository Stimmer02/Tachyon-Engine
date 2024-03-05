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


    Node* query_rec(Component comp, Node* suspect) {
        // If we found nothing we return nodeNull.
        if (suspect == nodeNull) {
            return suspect;
        }


        // If suspect is guilty of being Node we search for.
        if (suspect->component.getX() == comp.getX()) {
            return suspect;
        }

        // Reccurency. (Red Black Tree is BST tree)
        if (suspect->component.getX() < comp.getX()) {
            return query_rec(comp, suspect->right);
        }
        else {
            return query_rec(comp, suspect->left);
        }
    }

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

    Node* query(Component comp) {
        return query_rec(comp, this->root);
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

    void add(Component comp) {
        Node* newNode = new Node;

        newNode->component = comp;
        newNode->parent = nullptr;
        newNode->left = nodeNull;
        newNode->right = nodeNull;
        // Red.
        newNode->color = 1;

        // Looking for correct leaf and it's parent.
        Node* leaf = this->root;
        Node* leafParent = nullptr;
        while (leaf != nodeNull) {
        leafParent = leaf;

        if (leaf->component.getX() < newNode->component.getX()) {
            leaf = leaf->right;
        }
        else {
            leaf = leaf->left;
        }
        }

        // Placing new node on this leaf.
        newNode->parent = leafParent;

        // And other way around.
        // Root.
        if (leafParent == nullptr) {
        root = newNode;
        }
        // Not root.
        else if (leafParent->component.getX() < newNode->component.getX()) {
        leafParent->right = newNode;
        }
        else {
        leafParent->left = newNode;
        }

        // No problems with colors - can be just black. (root has to be black)
        if (newNode->parent == nullptr) {
        // Black.
        newNode->color = 0;
        return;
        }

        // Root is always black, so red is fine.
        if (newNode->parent->parent == nullptr) {
        return;
        }

        // Harder to solve problems with colors.
        harderPaintingProblems(newNode);
    }
}
