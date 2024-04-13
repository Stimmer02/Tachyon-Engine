#include <iostream>
#include "StructTree.h"

using namespace std;

StructTree::StructTree() {

}

StructTree::StructTree(std::string structDirectory, std::string rootStructName) {
    this->structDirectory = structDirectory;
    (*(this->root)).name = rootStructName;
}

StructTree::~StructTree() {

}

char StructTree::setRootStruct(std::string rootStructName) {
    (*(this->root)).name = rootStructName;
}

void StructTree::setStructDirectory(std::string structDirectory) {
    this->structDirectory = structDirectory;
}

char StructTree::build(ClStructParser* parser) {

}

std::string StructTree::getError() {
    return this.error;
}

std::string StructTree::getStructures() {

}
