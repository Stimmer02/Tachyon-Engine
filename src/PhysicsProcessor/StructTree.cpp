#include <iostream>
#include <queue>
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
    return 0;
}

void StructTree::setStructDirectory(std::string structDirectory) {
    this->structDirectory = structDirectory;
}

char StructTree::build(ClStructParser* parser) {
    queue<engineStruct*> q;
    q.push(root);

    while (q.empty() == false) {
        engineStruct* firstStruct = q.front();
        q.pop();

        for (int i = 0; i < firstStruct->fieldCount; ++i) {
            if (firstStruct->fields[i].type == engineStruct::cl_struct) {
                for (int j = 0; j < firstStruct->fieldCount; ++j) {
                    if (firstStruct->fields[i].subStructName + ".cl" == firstStruct->fields[j].name || firstStruct->fields[i].subStructName + "clcpp" == firstStruct->fields[j].name) {
                        std::string code = firstStruct->fields[j].subStruct->rawCode;
                        firstStruct->fields[i].subStruct = parser->processStruct(code);
                        if (firstStruct->fields[i].subStruct == nullptr) {
                            status = 1;
                            return status;
                        }
                        q.push(firstStruct->fields[i].subStruct);
                    }
                }
            }
        }
    }

    this->status = 0;
    return status;
}

std::string StructTree::getError() {
    return this->error;
}

std::string StructTree::getStructuresHelper(engineStruct* node) {
    std::string result = "";
    for (int i = 0; i < node->fieldCount; ++i) {
        result += getStructuresHelper(node->fields[i].subStruct);
    }
    result += node->rawCode;
    return result;
}

std::string StructTree::getStructures() {
    return getStructuresHelper(root);
}
