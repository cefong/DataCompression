# Data Compression Algorithm
## Developed by: @cefong

This data compression algorithm was developed using Huffman coding, which is a lossless 
compression algorithm that relies on prefix codes, greedy algorithms, and bottom up construction.

The algorithm is the driving force of the byte_compress function, which takes an data buffer (unsigned char array[]) and the
size of the buffer (int) as its arguments. It begins by building up the Huffman tree that is used to encode bytes into their 
shortened representation. 

To build the Huffman tree in build_huffman, we first start by determining the frequency of each byte in the array. This is used
to order the nodes of the tree to be constructed, from lowest frequency to highest frequency in a priority queue. It should be
noted that each item in the priority queue is a pointer to a custom Node struct which holds its children (Node*), cumulative frequency (int),
and byte value (unsigned char) it represents if it is a leaf node. A node is constructed for each entry in the frequency table
and an extra node is added with a value of 0xFF that serves as the EOF sentinel. We then iterate through the priority queue while it's 
length is still greater than 1. In each iteration, we pop the two smallest frequency nodes and combine them into a new Node whose
cumulative frequency is equal to the sum of the frequencies of the nodes we popped and set the smaller of the two to be the left child
of the new Node and the larger of the two to be the right child of the new Node. This new Node is then inserted into the priority
queue, and a new iteration begins. When the priority queue has reached a size of 1, the remaining Node* is the root of the Huffman
tree, which will be popped and returned as the result of build_huffman.

Once the Huffman tree has been constructed, each byte to be represented must be encoded using the tree. To accomplish this, we use
huffman_encode to recursively determine the Huffman representation for each unique byte that will be represented. Starting at the
root, it traverses down the tree, concatening a '1' to the code representation of a byte for each right turn, and a '0' for each 
left turn. When it reaches a leaf node, it maps the byte in that leaf node to the final code representation it determined, based
on the order of left and right turns it made to get there. 

To finally compress the data buffer given, I had to make a couple modifications to the classic Huffman encoding algorithm. Since the 
data buffer was to be modified in place, and the encoding for each byte was usually not a full byte itself, I set up a buffer to keep
track of the length of the encoding, and only replaced the contents of data_ptr at the desired position when the buffer had reached 8
characters. Therefore, the memory used by this algorithm does not depend on the size of the data buffer that is given.

When byte_compress is finished executing, it will return the size of the compressed array into new_size. data_ptr will point to the same array, with 
the first new_size number of bytes modified and holding the contents of the compressed data_buffer.

### Assumptions:
1. It was assumed that the data_size to compress was the same size as the array pointed to by data_ptr

### Know deficiencies:
1. It is known that the true size of the data_buffer does not change, due to the fact that C++ does not allow for the resizing of arrays.
Since it was requested that the data within the data buffer be modified, this goal was attained and if the old data_ptr is not in use any longer
it is suggested that the first new_size elements of data_ptr be copied to another byte array and data_ptr be discarded. 