#include "huffman_bitcode.h"

Huffman_Bitcode::Huffman_Bitcode(unsigned bytecode, unsigned size) {
    m_bytecode = bytecode;
    m_size = size;
}

Huffman_Bitcode Huffman_Bitcode::left_turn() {
    unsigned bytecode = m_bytecode << 1;
    unsigned size = m_size + 1;
    return Huffman_Bitcode(bytecode, size);
}

Huffman_Bitcode Huffman_Bitcode::right_turn() {
    unsigned bytecode = m_bytecode << 1 | 1;
    unsigned size = m_size + 1;
    return Huffman_Bitcode(bytecode, size);
}

unsigned Huffman_Bitcode::get_bytecode() {
    return m_bytecode;
}

unsigned Huffman_Bitcode::get_size() {
    return m_size;
}
