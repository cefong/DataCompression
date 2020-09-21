#include "huffman_tree.h"
#include "huffman_bitcode.h"

#include <queue> // for std::priority_queue
#include <iostream> // for std::cout
#include <bitset> // for std::bitset
#include <cstring> // for std::memcpy
#include <vector> // for std::vector

using namespace std;

// custom Node function comparison for ordering in priority_queue
struct CompareFreq {
    bool operator()(Node *a, Node *b) {
        return (a -> freq) > (b -> freq);
    }
};

/*
    Huffman_Tree constructor

    Parameters
    unsigned char data_ptr[]: a pointer to the data buffer
    int data_size: the number of bytes to compare

    Returns 
    none
*/
Huffman_Tree::Huffman_Tree(unsigned char data_ptr[], int data_size) {
    // get frequency of each character and store in unordered map
    unordered_map<unsigned char, int> freq;

    for (int i = 0; i < data_size; i++) {
        unsigned char byte = data_ptr[i];
        freq[byte]++;
    }

    // sort in ascending order of frequency
    priority_queue<Node*, vector<Node*>, CompareFreq> pq;

    for (auto& x: freq) {
        Node* new_Node = new Node();
        new_Node -> freq = x.second;
        new_Node -> byte = x.first;
        pq.push(new_Node);
    }

    // add EOF sentinel 
    Node *eof_Node = new Node();
    eof_Node -> freq = 1;
    eof_Node -> byte = 0xFF; // since we are guaranteed 0xFF is not a byte that will be in the array
    pq.push(eof_Node);
    
    // iterate through Nodes and construct binary tree
    while (pq.size() > 1) {
        Node *temp = new Node();

        // get minimum frequency and assign it to left child of Node
        Node *first = pq.top();
        temp -> left = first;
        temp -> freq = first -> freq;
        pq.pop();

        // get minimum frequency and assign it to right child of Node
        Node *second = pq.top();
        temp -> right = second;
        temp -> freq += second -> freq;
        pq.pop();

        // add new Node to the queue to be processed
        pq.push(temp);
    }

    // return root Node
    Node *root = pq.top();
    pq.pop();
    m_root = root;
}

// destructor
Huffman_Tree::~Huffman_Tree() {
    destroy(m_root);
}

// recursive destructor helper
void Huffman_Tree::destroy(Node *root) {
    if (root != nullptr) {
        destroy(root -> left);
        destroy(root -> right);
        delete root;
    }
}

// getter methods to get root to Huffman tree
Node *Huffman_Tree::get_root() {
    return m_root;
}

// front facing class method to generate the byte to bytecode mappings for this Huffman tree
void Huffman_Tree::generate_code() {
    Huffman_Bitcode code = Huffman_Bitcode(0, 0);
    generate_code(m_root, code);
}

/*
    Recursive function that generates the byte to bytecode mappings

    Parameters
    Node *root: the current node that is being examined
    Huffman_Bitcode code: the bytecode that is being generated

*/
void Huffman_Tree::generate_code(Node *root, Huffman_Bitcode code) {
    if (root == nullptr) return;

    // encode leaf Node
    if (!root->left && !root->right) {
        unsigned int bytecode = code.get_bytecode();
        unsigned int size = code.get_size();
        huff_code[root->byte] = {bytecode, size};
        return;
    }

    // traverse left and right subtrees recursively
    generate_code(root->left, code.left_turn());
    generate_code(root->right, code.right_turn());
}

/*
    Compresses a data buffer using the Huffman tree that was constructed

    Parameters
    unsigned char data_ptr[]: the pointer to the byte array to be compressed
    int data_size: the number of bytes to be compressed

    Returns
    int new_size: the size of the modified data buffer
*/
int Huffman_Tree::encode(unsigned char data_ptr[], int data_size) {
    // set the starting values of buffer, new_size, and offset
    unsigned char buff = 0x00;
    unsigned int new_size = 0;
    int offset = 7; // since there the 8 bits in a byte are "indexed" up till 7
    for (int i = 0; i <= data_size; i++) {
        // if i is = data_size, encode the EOF character (0xFF)
        unsigned char uncompressed = (i != data_size) ? data_ptr[i] : 0xFF;
        // get the bytecode and size from the byte to bytecode mapping
        pair<unsigned int, unsigned int> bytecode_pair = huff_code[uncompressed];
        unsigned int bytecode = bytecode_pair.first;
        unsigned int size = bytecode_pair.second;
        // calculate how much the bytecode needs to be shifted to fit into the buffer
        int shift = offset - (size - 1); // where the code needs to be shifted to fit into the byte
        offset -= size; // the new offset
        // since a negative shift value will cause an undefined outcome, change the direction of shift if it is negative
        unsigned char appended = (shift > 0) ? (bytecode << shift) : (bytecode >> -shift);
        buff += appended;

        if (offset <  0) {
            data_ptr[new_size++] = buff;
            buff = 0x00;
            // the size of the leftover bits from the last bytecode
            size = -offset - 1;
            // reset the offset
            offset = 7;
            buff += bytecode << (offset - (size - 1));
            offset -= size; // new offset
        }
        // final buffer push to get EOF character into compressed array
        if (i == data_size) data_ptr[new_size++] = buff; 
    }

    unsigned char* new_arr = new unsigned char[new_size]; // create dynamic array
    memcpy(new_arr, data_ptr, new_size);
    m_compressed_array = new_arr; // point compressed array member to newly created array
    delete[] new_arr; // release memory
    return new_size;
}

// getter method to get the resized, compressed dynamic array
unsigned char *Huffman_Tree::get_compressed_array() {
    return m_compressed_array;
}

// prints byte to bytecode mapping being used
void Huffman_Tree::print_code() {
    cout << "Encoding used: " << endl;
    for (auto mapping: huff_code) {
        bitset<8> byte(mapping.first);
        unsigned int bytecode = (mapping.second).first;
        bitset<8> x(bytecode);
        unsigned int size = (mapping.second).second;
        cout << "byte: " << byte << " code: " << x << " size: " << size << endl;
    }
}