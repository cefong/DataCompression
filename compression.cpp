#include <iostream>
#include <unordered_map> // to get std::uNodered_map
#include <queue> // to get std::priority_queue
#include <bitset> // to get std::bitset
#include <string> // to get std::string
#include <vector> // to get std::vector

using namespace std;

// Node declaration
struct Node {
    Node *left;
    Node *right;
    int freq;
    unsigned char byte;
};


// custom Node comparison
struct CompareFreq {
    bool operator()(Node *a, Node *b) {
        return (a -> freq) > (b -> freq);
    }
};


// function to create Huffman tree
Node* build_huffman( unsigned char data_ptr[], int data_size ) {
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
    return root;
}

void huffman_encode( Node *tree, string code, 
                     unordered_map<unsigned char, string> &huff_code) {
    if (tree == nullptr) return;

    // encode leaf Node
    if (!tree->left && !tree->right) {
        huff_code[tree->byte] = code;
        return;
    }

    // traverse right and left subtrees recursively
    huffman_encode(tree->left, code + "0", huff_code);
    huffman_encode(tree->right, code + "1", huff_code);
}

unsigned char string_to_byte(string str) {
    unsigned char c = 0x00;
    for (int i = 0; i < 8; i++) {
        if (str[i] == '1') c |= 1 << (7 - i); 
    }
    return c;
}

int byte_compress( unsigned char data_ptr[], int data_size) {
    // create Huffman tree
    Node *root = build_huffman(data_ptr, data_size);

    // encode each byte and modify the existing array
    unordered_map<unsigned char, string> huff_code;
    huffman_encode(root, "", huff_code);

    for (auto mapping: huff_code) {
        bitset<8> byte(mapping.first);
        cout << "byte: " << byte << " code: " << mapping.second << endl;
    };

    string buf;
    unsigned char new_byte;
    int new_size = 0;
    for (int i = 0; i < data_size; i++) {
        buf += huff_code[data_ptr[i]];
        if (buf.length() >= 8) {
            string str = buf.substr(0, 8);
            new_byte = string_to_byte(str);
            buf = buf.substr(8);
            data_ptr[new_size++] = new_byte;
        }
    }
    new_byte = string_to_byte(buf + huff_code[0xFF]); // 0xFF is the EOF sentinel
    data_ptr[new_size++] = new_byte;

    return new_size;
}

int main()
{
    unsigned char data_ptr[] = { 0x03, 0x74, 0x04, 0x04, 0x04, 0x35, 0x35, 0x64, 
                                 0x64, 0x64, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00,
                                 0x56, 0x45, 0x56, 0x56, 0x56, 0x09, 0x09, 0x09 };

    int data_size = 24;
    for (int i = 0; i < data_size; i++) {
        bitset <8> x(data_ptr[i]);
        cout << x << " ";
    }
    cout << endl;
    cout << "Number of bytes before compression: " << data_size << endl;
    cout << "Encoding used: " << endl;
    int num = byte_compress(data_ptr, data_size);
    for (int i = 0; i < num; i++) {
        bitset <8> x(data_ptr[i]);
        cout << x << " ";
    }
    cout << endl;
    cout << "Number of bytes after compression: " << num << endl;
    float compression_ratio = (float)data_size/(float)num;
    float space_savings = (1 - 1/compression_ratio) * 100;
    cout << "Compression ratio: " << compression_ratio << "x" << endl;
    cout << "Space savings: " << space_savings << "%" << endl;
}
