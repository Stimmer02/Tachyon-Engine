#ifndef STRUCTTREE_H
#define STRUCTTREE_H

#include "engineStruct.h"
#include "ClStructParser.h"
#include "SizeCalculator.h"

#include <map>
#include<fstream>
#include<sstream>
#include <iostream>
#include <queue>
#include <sys/stat.h>


class StructTree{
public:
    StructTree();
    StructTree(std::string structDirectory, std::string rootStructName, ClStructParser* parser);
    ~StructTree();

    char setRootStruct(std::string rootStructName, ClStructParser* parser); //Parses and saves the root struct
    void setStructDirectory(std::string structDirectory); //Sets the root directory where the structs are stored (they could be stored recursively in subdirectories)
    char build(ClStructParser* parser); //Builds the tree of structures starting from the root
    char calculateSizes(SizeCalculator* sCalc); //Calculates the sizes of the structures in the tree
    void printTree(); //Prints the structures in the tree
    std::string getError(); //Returns the error message
    std::string getStructures(); //Returns structures code in a single string in the order from leaves to the root (format that is ready to be compiled)

    const std::vector<const engineStruct*> unwindTree(); //Returns the structures in the tree in the order from root to leaves repeating enties if they are used in multiple places

private:
    std::string getStructuresRecursive(engineStruct* node); //Recursive function that returns structures code in a single string in the order from leaves to the root
    void setVisitedFalse(); //goes through parsedStructs and sets visited to false
    void printTreeRecursive(engineStruct* node); //Recursive function that prints the structures in the tree

    engineStruct* root; //Parsed root struct
    std::string structDirectory; //Root directory where the structs are stored
    std::string error; //Error message

    struct parsedStruct{
        engineStruct* node;
        bool visited;
    };

    std::map<std::string, parsedStruct> parsedStructs; //Map of parsed structs
    char status; //Status of the tree building process:  < -1 not ready to be builded, -1 - ready to be builded, 0 - builded, > 0 - build error code

    void unwindTreeRecursive(engineStruct* node, std::vector<const engineStruct*>& result); //Recursive function that unwinds the tree
};
#endif
