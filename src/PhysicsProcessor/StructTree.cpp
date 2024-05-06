#include <iostream>
#include <queue>
#include "StructTree.h"
#include <sys/stat.h>
#include<fstream>
#include<sstream>
#include "ClStructParser.h"
#include <set>

using namespace std;

StructTree::StructTree() {

}

StructTree::StructTree(std::string structDirectory, std::string rootStructName) {
    setStructDirectory(structDirectory);
    setRootStruct(rootStructName);
}

StructTree::~StructTree() {

}

char StructTree::setRootStruct(std::string rootStructName) {
    // Checking if file exists.
    string path = this->structDirectory + "/" + rootStructName;
    const char* file = path.c_str();
    struct stat sb;
    if (stat(file, &sb) != 0 || (sb.st_mode & S_IFDIR)) {
        this->status = -2;
        this->error = "Root not found.";
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

    MacroManager macroManager;
    SizeCalculator sCalc(8);
    ClStructParser clParser(&macroManager, &sCalc);

    if (macroManager.parseFile("macro/macros.cfg")){
        std::fprintf(stderr, "Error has occured\n");
        return -1;
    }

    this->root = clParser.processStruct(code);
    this->status = -1;
    return status;
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
                string path = this->structDirectory + "/" + firstStruct->fields[i].subStructName + ".cl";
                string path2 = this->structDirectory + "/" + firstStruct->fields[i].subStructName + ".clcpp";

                const char* file = path.c_str();
                const char* file2 = path2.c_str();

                ifstream f(path);
                string code;
                if(f) {
                    ostringstream s;
                    s << f.rdbuf();
                    code = s.str();
                }

                ifstream f2(path2);
                string code2;
                if(f2) {
                    ostringstream s;
                    s << f2.rdbuf();
                    code2 = s.str();
                }

                struct stat sb;
                if (stat(file, &sb) == 0 and !(sb.st_mode & S_IFDIR)) {
                    firstStruct->fields[i].subStruct = parser->processStruct(code);
                    if (firstStruct->fields[i].subStruct == nullptr) {
                        status = 1;
                        return status;
                    }
                    q.push(firstStruct->fields[i].subStruct);
                }
                else if (stat(file2, &sb) == 0 and !(sb.st_mode & S_IFDIR)) {
                    firstStruct->fields[i].subStruct = parser->processStruct(code2);
                    if (firstStruct->fields[i].subStruct == nullptr) {
                        status = 1;
                        return status;
                    }
                    q.push(firstStruct->fields[i].subStruct);
                }
                else{
                    this->status = 1;
                    return status;
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

set<string> used;

std::string StructTree::getStructuresHelper(engineStruct* node) {
    std::string result = "";
    for (int i = 0; i < node->fieldCount; ++i) {
        if (node->fields[i].subStruct != nullptr) {
            result += getStructuresHelper(node->fields[i].subStruct);
        }
    }

    if (used.find(node->name) == used.end()) {
        result += node->rawCode;
        used.insert(node->name);
    }
    return result;
}

std::string StructTree::getStructures() {
    return getStructuresHelper(root);
}
