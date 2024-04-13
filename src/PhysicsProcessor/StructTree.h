#ifndef STRUCTTREE_H
#define STRUCTTREE_H

#include "engineStruct.h"
#include "ClStructParser.h"

class StructTree{
    public:
        StructTree();
        StructTree(std::string structDirectory, std::string rootStructName);
        ~StructTree();

        char setRootStruct(std::string rootStructName); //Parses and saves the root struct
        void setStructDirectory(std::string structDirectory); //Sets the root directory where the structs are stored (they could be stored recursively in subdirectories)
        char build(ClStructParser* parser); //Builds the tree of structures starting from the root
        std::string getError(); //Returns the error message
        std::string getStructures(); //Returns structures code in a single string in the order from leaves to the root (format that is ready to be compiled)

    private:
        engineStruct* root; //Parsed root struct
        std::string structDirectory; //Root directory where the structs are stored
        std::string error; //Error message
        char status; //Status of the tree building process:  < -1 not ready to be builded, -1 - ready to be builded, 0 - builded, > 0 - build error code
};
#endif
