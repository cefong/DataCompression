# Data Compression Algorithm

This data compression algorithm was developed using Huffman coding, which is a lossless compression algorithm that relies on prefix codes, greedy algorithms, and bottom up construction.

The Huffman Compression algorithm is the driving force of the ```byte_compress``` function (in compression.cpp), which takes an data buffer (```unsigned char array[]```) and the size of the buffer (```int```) as its arguments.

## Huffman_Tree
To encapsulate the capabilities of the Huffman Tree, I created a custom class Huffman_Tree (in huffman_tree.cpp), which has the following API:

### ```Huffman_Tree(unsigned char data_ptr[], int data_size)```
This is the constructor method, and is used to build the Huffman Tree that will be used for encoding and decoding. To build the Huffman tree in we first start by determining the frequency of each byte in the array. This is used
to order the nodes of the tree to be constructed, from lowest frequency to highest frequency in a priority queue. It should be noted that each item in the priority queue is a pointer to a custom Node struct which holds its children (Node*), cumulative frequency (int), and byte value (unsigned char) it represents if it is a leaf node. A node is constructed for each entry in the frequency table and an extra node is added with a value of 0xFF that serves as the EOF sentinel. We then iterate through the priority queue while its length is still greater than 1. In each iteration, we pop the two smallest frequency nodes and combine them into a new Node whose cumulative frequency is equal to the sum of the frequencies of the nodes we popped and set the smaller of the two to be the left child of the new Node and the larger of the two to be the right child of the new Node. This new Node is then inserted into the priority queue, and a new iteration begins. When the priority queue has reached a size of 1, the remaining Node* is the root of the Huffman tree, which will be popped and stored as private member ```m_root```.  

### ```~Huffman_Tree()```
This is the class destructor, which calls the private method ```destroy(Node *root)``` which recursively destroys each ```Node *``` in the tree, so memory can be freed once the byte array has been encoded and decoded successfully.

### ```Node *get_root()```
This is the getter method to get the root of the tree. Since there is no public setter method for ```m_root``` this means that it is a read-only member, which is good since we don't want to point the root of the tree to an arbitrary location!

### ```void generate_code()```
This class method is called once the Huffman tree has been constructed, as each unique byte to be represented must be encoded using the tree. ```generate_code()``` is actually a front for the private class method ```generate_code(Node *root, Huffman_Bitcode code)`` which recursively determines the Huffman representation for each unique byte that will be represented. Starting at the root, it traverses down the tree, concatening a '1' to the code representation of a byte for each right turn, and a '0' for each left turn (the complexities of this bit concatenation are abstracted by the Huffman_Bitcode class, which will be described later) When it reaches a leaf node, it maps the byte in that leaf node to the final code representation or "bytecode" it determined, based on the order of left and right turns it made to get there, and notes the size of the bytecode.

### ```int encode(unsigned char data_ptr[], int data_size)```
To finally compress the data buffer given, I had to make a couple modifications to the classic Huffman encoding algorithm. Since the 
data buffer was to be modified in place, and the encoding for each byte was usually not a full byte itself, I set up a buffer to keep track of the length of the encoding, and only replaced the contents of data_ptr at the desired position when the buffer had reached 8 bits. Therefore, the memory used by this algorithm does not depend on the size of the data buffer that is given.

When ```encode``` is finished executing, it will return the size of the compressed array into ```new_size```. ```data_ptr``` will point to the same array, with 
the first ```new_size``` number of bytes modified and holding the contents of the compressed ```data_buffer```.

The contents of ```data_ptr``` were also copied over to a new dynamic array, which is pointed to by the private member ```m_compressed_array```. 

### ```unsigned char *get_compressed_array()```
This getter method gets the private ```m_compressed_array``` member, which points to the resized compressed array. 

## Huffman_Bitcode
To concatenate bits together I created a custom class called Huffman_Bitcode, which is in huffman_bitcode.cpp. The purpose of this class was to abstract away the need to do logical bit operations when encoding left or right turns in the ```generate_code``` Huffman_Tree class method. The API for the class is as follows:

### ```Huffman_Bitcode(unsigned bytecode, unsigned size)```
This is the constructor method to create an instance of Huffman_Bitcode. It initializes the private members ```m_bytecode``` and ```m_size``` to the specified bytecode and size. 

### ```Huffman_Bitcode left_turn()``` and ```Huffman_Bitcode right_turn()```
These class methods concatenate a 0 bit and a 1 bit to the end of ```m_bytecode``` respectively. A new Huffman_Bitcode instance with this new bytecode is created and returned.

### ```unsigned get_bytecode()``` and ```unsigned get_size()```
These are the getter methods for the ```m_bytecode``` and ```m_size``` members respectively. 

## Extras
To keep track of the byte encoding that is used with the given ```data_ptr[]```, I created the ```print_code()``` class method of the Huffman_Tree, which will print the byte and its given bytecode and size of bytecode when called. 
Additionally, when the compression.cpp, huffman_tree.cpp, and huffman_bitcode.cpp files are compiled and executed, the example data buffer
```c++
    unsigned char data_ptr[] = { 0x03, 0x74, 0x04, 0x04, 0x04, 0x35, 0x35, 0x64, 
                                 0x64, 0x64, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00,
                                 0x56, 0x45, 0x56, 0x56, 0x56, 0x09, 0x09, 0x09 };
```
will be compressed, and both the uncompressed and compressed array will be printed to the terminal, along with the compression ratio and space savings.

## Compilation instructions
This guide assumes that the gcc compiler is installed, and this repository has been cloned into your working directory.


To compile the program from the command line run the following in your working directory:

```g++ -g compression.cpp huffman_tree.cpp huffman_bitcode.cpp -o compression.exe```


Now, to execute the program:

```./compression.exe```

## Assumptions:
1. It was assumed that the ```data_size``` to compress was the same size as the array pointed to by ```data_ptr```
2. It is assumed the the size of the bytes given are 8 bits

## Known deficiencies:
1. It is known that the true size of the data_buffer does not change, due to the fact that C++ does not allow for the resizing of arrays. Since it was requested that the data within the byte array be modified, this goal was attained and if the old ```data_ptr``` is not in use any longer it is suggest that the data ptr be deleted and the Huffman_Tree get_compressed_array method is used to get the resized compressed array.
2. It is known that the class methods closely rely on each other and must be called in a specific order (i.e. the ```get_compressed_array()``` method should not be called until an array has been compressed). It is suggested that the ```byte_compress``` function be used to abstract the complexities of the Huffman compression algorithm, as it uses these methods in the order they are intended to be used.
3. The bytes in data_ptr are overriden as data_ptr is being iterated over. This is not a concern for the given design problem since the bytecodes will be less than a byte. This is because the most amount of unique bytes we can have is 128, since it is guaranteed the values of the bytes will only range from 0 to 127. This means we will only ever need 2^7 + 1 (for EOF) different representations, or 7 bits to represent these bytes. 