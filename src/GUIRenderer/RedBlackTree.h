#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include <string.h>
using namespace std;

struct Node {
    Component* component;

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

    Node* query_rec(Component* comp, Node* suspect);

    void harderPaintingProblems(Node* repaintingNode);

public:
    RedBlackTree();

    Node* query(Component* comp);

    void rotationLeft(Node* center);

    void rotationRight(Node* center);

    void add(Component* comp);
};

#endif
