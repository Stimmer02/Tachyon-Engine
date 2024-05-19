#include <iostream>
#include "SharedNameResolver.h"
using namespace std;

SharedNameResolver::SharedNameResolver() {
    root = new Node();
}

void SharedNameResolver::Emplace(const std::string & _name, const void * _pointer, const int32_t & _size) {
    if (_name != "") {
        Node* path_destination_vestibule = root;
        Node* path_destination = root;
        for (size_t i = 0; i < _name.size(); ++i) {
            // Invalid name.
            if (_name[i] < 'a' || _name[i] > 'z') {
                break;
            }

            path_destination_vestibule = path_destination;

            // Name does not exist.
            if ((*(path_destination)).next_char[((int) (_name[i] - 'a'))] == nullptr) {
                // Not the last one.
                if (i != _name.size() - 1) {
                    // New empty Node.
                    Node newNode = { size_: 0, pointer: NULL };
                    for (int i = 0; i < ALPHABET_SIZE; ++i) {
                        newNode.next_char[i] = NULL;
                    }
                    Node* ptr = &newNode;

                    path_destination = ptr;
                    (*(path_destination_vestibule)).next_char[((int) (_name[i] - 'a'))] = path_destination;
                }
                // The last one.
                else {
                    // New empty Node.
                    Node newNode = { size_: _size, pointer: _pointer };
                    for (int i = 0; i < ALPHABET_SIZE; ++i) {
                        newNode.next_char[i] = NULL;
                    }
                    Node* ptr = &newNode;

                    path_destination = ptr;
                    (*(path_destination_vestibule)).next_char[((int) (_name[i] - 'a'))] = path_destination;
                }
            }
            else {
                path_destination = (*(path_destination)).next_char[((int) (_name[i] - 'a'))];

                // This already exists.
                if (i == _name.size() - 1) {
                    break;
                }
            }
        }
    }
}

void * SharedNameResolver::Find(const std::string & _name, const int32_t & _elem_size, const int32_t & _element) {
    if (_name != "") {
        Node* path_destination = root;
        for (size_t i = 0; i < _name.size(); ++i) {
            // Invalid name.
            if (_name[i] < 'a' || _name[i] > 'z') {
                return NULL;
            }

            // Name does not exist.
            if ((*(path_destination)).next_char[((int) (_name[i] - 'a'))] == nullptr) {
                return NULL;
            }

            path_destination = (*(path_destination)).next_char[((int) (_name[i] - 'a'))];
        }

        return (char*)path_destination->pointer + _element * _elem_size;
    }

    return NULL;
}

void SharedNameResolver::Resize(const std::string & _name, const int32_t & _size) {
    if (_name != "") {
        Node* path_destination_vestibule = root;
        Node* path_destination = root;
        for (size_t i = 0; i < _name.size(); ++i) {
            // Invalid name.
            if (_name[i] < 'a' || _name[i] > 'z') {
                break;
            }

            // Name does not exist.
            if ((*(path_destination)).next_char[((int) (_name[i] - 'a'))] == nullptr) {
                break;
            }

            path_destination_vestibule = path_destination;
            path_destination = (*(path_destination)).next_char[((int) (_name[i] - 'a'))];
        }

        // New similar node with different size.
        Node newNode = { size_: _size, pointer: (*(path_destination)).pointer };
        Node* ptr = &newNode;
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            newNode.next_char[i] = (*(path_destination)).next_char[i];
        }

        // Setting new node in the main tree.
        (*(path_destination_vestibule)).next_char[_name[_name.size() - 1] - 'a'] = ptr;
    }
}

