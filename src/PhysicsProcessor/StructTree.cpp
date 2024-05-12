#include "StructTree.h"

using namespace std;

StructTree::StructTree() {
    this->status = -1;
    this->root = nullptr;
}

StructTree::StructTree(std::string structDirectory, std::string rootStructName, ClStructParser* parser) {
    this->status = -1;
    setStructDirectory(structDirectory);
    setRootStruct(rootStructName, parser);
}

StructTree::~StructTree() {

}

char StructTree::setRootStruct(std::string rootStructName, ClStructParser* parser){
    // Checking if file exists.
    string path = structDirectory + "/" + rootStructName;
    const char* file = path.c_str();
    struct stat sb;
    if (stat(file, &sb) != 0 || (sb.st_mode & S_IFDIR)) {
        status = -2;
        error = "Root not found.";
        return status;
    }

    // Setting root.
    ifstream f(path);
    string code;
    if(f) {
        ostringstream s;
        s << f.rdbuf();
        code = s.str();
    }

    root = parser->processStruct(code);
    parsedStructs[root->name].node = root;
    status = -1;
    return status;
}

void StructTree::setStructDirectory(std::string structDirectory) {
    this->structDirectory = structDirectory;
}

char StructTree::build(ClStructParser* parser) {
    queue<engineStruct*> q;
    q.push(root);

    while (q.empty() == false) {
        engineStruct* currentStructure = q.front();
        q.pop();

        for (int i = 0; i < currentStructure->fieldCount; ++i) {
            if (currentStructure->fields[i].type == engineStruct::cl_struct) {

                if (parsedStructs.find(currentStructure->fields[i].subStructName) != parsedStructs.end()) {
                    currentStructure->fields[i].subStruct = parsedStructs[currentStructure->fields[i].subStructName].node;
                    // q.push(currentStructure->fields[i].subStruct); not sure if this is needed
                    continue;
                }

                string path = this->structDirectory + "/" + currentStructure->fields[i].subStructName + ".cl";
                string path2 = this->structDirectory + "/" + currentStructure->fields[i].subStructName + ".clcpp";

                const char* file = path.c_str();
                const char* file2 = path2.c_str();

                ifstream f(path);
                string code;
                if(f) {
                    ostringstream s;
                    s << f.rdbuf();
                    code = s.str();
                } else {
                    f.close();
                    f.open(path2);
                    if (f) {
                        ostringstream s;
                        s << f.rdbuf();
                        code = s.str();
                    } else {
                        status = 1;
                        return status;
                    }
                }

                currentStructure->fields[i].subStruct = parser->processStruct(code);
                if (currentStructure->fields[i].subStruct == nullptr) {
                    status = 1;
                    return status;
                }
                parsedStructs[currentStructure->fields[i].subStructName].node = currentStructure->fields[i].subStruct;
                q.push(currentStructure->fields[i].subStruct);
            }
        }
    }

    status = 0;
    return status;
}

char StructTree::calculateSizes(SizeCalculator* sCalc) {
    if (status != 0) {
        return status;
    }

    queue<engineStruct*> q;
    q.push(root);

    while (q.empty() == false) {
        engineStruct* currentStructure = q.front();
        q.pop();

        for (int i = 0; i < currentStructure->fieldCount; ++i) {
            if (currentStructure->fields[i].type == engineStruct::cl_struct) {
                q.push(currentStructure->fields[i].subStruct);
            }
        }

        if (sCalc->calculate(currentStructure) != 0) {
            status = 2;
            return status;
        }
    }

    status = 0;
    return status;
}

void StructTree::printTree() {
    if (status != 0) {
        return;
    }
    setVisitedFalse();
    printTreeRecursive(root);
}

void StructTree::printTreeRecursive(engineStruct* node) {
    for (int i = 0; i < node->fieldCount; ++i) {
        if (node->fields[i].subStruct != nullptr) {
            printTreeRecursive(node->fields[i].subStruct);
        }
    }

    parsedStruct& parsed = parsedStructs[node->name];

    if (parsed.visited == false) {
        engineStruct* structure = parsed.node;
        std::printf("name: %s; size: %d; field count: %d\n", structure->name.c_str(), structure->byteSize, structure->fieldCount);
        for (uint i = 0; i < structure->fieldCount; i++){
            if (structure->fields[i].type == engineStruct::cl_struct){
                std::printf(" %d) var: %s; type: %d; size: %d; struct: %s; amount: %d\n", i, structure->fields[i].name.c_str(), structure->fields[i].type, structure->fields[i].byteSize, structure->fields[i].subStructName.c_str(), structure->fields[i].arrSize);
            } else if (structure->fields[i].arrSize > 0){
                std::printf(" %d) var: %s; type: %d; size: %d; pointer: %d\n", i, structure->fields[i].name.c_str(), structure->fields[i].type, structure->fields[i].byteSize, structure->fields[i].arrSize);
            } else {
                std::printf(" %d) var: %s; type: %d; size: %d\n", i, structure->fields[i].name.c_str(), structure->fields[i].type, structure->fields[i].byteSize);
            }
        }
        parsed.visited = true;
    }
}

std::string StructTree::getStructuresRecursive(engineStruct* node) {
    std::string result = "";
    for (int i = 0; i < node->fieldCount; ++i) {
        if (node->fields[i].subStruct != nullptr) {
            result += getStructuresRecursive(node->fields[i].subStruct);
        }
    }

    parsedStruct& parsed = parsedStructs[node->name];

    if (parsed.visited == false) {
        result += node->rawCode;
        parsed.visited = false;
    }
    return result;
}

std::string StructTree::getError() {
    return this->error;
}

std::string StructTree::getStructures() {
    if (status != 0) {
        return "";
    }
    setVisitedFalse();
    return getStructuresRecursive(root);
}

void StructTree::setVisitedFalse() {
    for (auto& it : parsedStructs) {
        it.second.visited = false;
    }
}


