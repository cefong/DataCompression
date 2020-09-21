#include "huffman_bitcode.h"

/*
    This is the constructor for Huffman_Bitcode

    Parameters
    unsigned char bytecode: the bytecode for this Huffman_Bitcode instance
    unsigned int size: the size of the bytecode

    Returns 
    none
*/
Huffman_Bitcode::Huffman_Bitcode(unsigned char bytecode, unsigned size) {
    m_bytecode = bytecode;
    m_size = size;
}

Huffman_Bitcode Huffman_Bitcode::left_turn() {
    unsigned char bytecode = m_bytecode << 1;
    unsigned size = m_size + 1;
    return Huffman_Bitcode(bytecode, size);
}

Huffman_Bitcode Huffman_Bitcode::right_turn() {
    unsigned char bytecode = m_bytecode << 1 | 1;
    unsigned size = m_size + 1;
    return Huffman_Bitcode(bytecode, size);
}

unsigned char Huffman_Bitcode::get_bytecode() {
    return m_bytecode;
}

unsigned Huffman_Bitcode::get_size() {
    return m_size;
}
