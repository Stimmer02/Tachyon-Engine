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
