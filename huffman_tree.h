#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include "huffman_bitcode.h"

#include <string> // to get string
#include <unordered_map> // to get unordered_map

using namespace std;

// Node declaration
struct Node 
{
    Node *left;
    Node *right;
    int freq;
    unsigned char byte;
};

class Huffman_Tree 
{
    private:
        Node *m_root;
        unsigned char *m_compressed_array; // holds the resized compressed array 
        void generate_code(Node *root, Huffman_Bitcode code); // recursive function that generates the byte to bytecode mappings 
        unordered_map<unsigned char, pair<unsigned int, unsigned int>> huff_code; // used to store byte to bytecode mappings
        void destroy(Node *root); // helper function for destructor
    public:
        Huffman_Tree(unsigned char data_ptr[], int data_size); // constructor
        ~Huffman_Tree(); // destructor
        Node *get_root(); // getter method to get the root of the tree
        void generate_code(); // generates the byte to bytecode (sets off recursive generate_code)
        int encode(unsigned char data_ptr[], int data_size); // compresses the bytes in data_ptr[]
        unsigned char *get_compressed_array(); // getter method to get compressed array
        void print_code(); // prints the byte encoding that was used
};

#endif
