#include "huffman_tree.h"
#include "huffman_bitcode.h"

#include <iostream> // to get std::cout
#include <bitset> // to get std::bitset

using namespace std;

/*
    This function compresses a data buffer of a given size using the Huffman compression algorithm

    Parameters
    unsigned char data_ptr[]: a pointer to the data buffer
    int data_size: the number of bytes to compress

    Returns
    int new_size: the number of bytes in the compressed buffer
*/
int byte_compress(unsigned char data_ptr[], int data_size) {
    // create Huffman tree
    Huffman_Tree tree = Huffman_Tree(data_ptr, data_size);
    Node *root = tree.get_root();

    // encode each byte and modify the existing array
    tree.generate_code();
    int new_size = tree.encode(data_ptr, data_size);

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
