#ifndef HUFFMAN_BITCODE
#define HUFFMAN_BITCODE

class Huffman_Bitcode {
    private:
        unsigned m_bytecode;
        unsigned m_size;
    public: 
        Huffman_Bitcode(unsigned bytecode, unsigned size); // constructor
        Huffman_Bitcode left_turn();
        Huffman_Bitcode right_turn();
        unsigned get_bytecode();
        unsigned get_size(); // getter method to get size of code
};

#endif