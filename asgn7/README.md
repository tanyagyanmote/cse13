# Assignment 7 - A (Huffman Coding) Tree Grows in Santa Cruz

The purpose of this program is to implement the Huffman encode and decode. With encode we compress an input file and using decode it will read the given input file. The user will input a data file in encoder, and compress a file and the decoder reverses the compressed file back to its original state.
## Files

1. Encode.c - This file will contain your implementation of the Huffman encoder.

2. Decode.c - This file will contain your implementation of the Huffman decoder.

3. Defines.h - This file will contain the macro definitions used throughout the assignment.

4. Header.h - This will will contain the struct definition for a file header.
5. Node.h - This file contains the node ADT interface
6. Node.c - This file will contain your implementation of the node ADT.
7. Pq.h - This file will contain the priority queue ADT interface.
8. Pq.c - This file will contain your implementation of the priority queue ADT.
9. Code.h - This file will contain the code ADT interface.
10. Code.c - This file will contain your implementation of the code ADT.
11. Io.h - This file will contain the I/O module interface
12. Io.c - This file will contain your implementation of the I/O module.
13. Stack.h - This file will contain the stack ADT interface.
14. Stack.c - This file will contain your implementation of the stack ADT.
15. Huffman.h - This file will contain the Huffman coding module interface.
16. Huffman.c - This file will contain your implementation of the Huffman coding module interface.
17. MakeFile - Complies the program
18. Readme.pdf - describes how the program will run
19. Design.pdf - describes the design of my program


## How to build
1. Have all the .c and .h files to be able to build the program

To construct all the files 
```python
make 

```
## Usage
Once the program is built, encode and decode can be called using 

Encode

-h: prints out a help message

-i: specify the input file to encode

-o: specify the output file to write the compressed file to

-s: prints compression stats of the program

Decode

-h: prints out a help message

-i: specify the input file to decode

-o: specify the output file to write the decompressed file to

-s: prints compression stats of the program

## Example

```python
echo "Hello World" ./encode ./decode
> Hello World

echo "Hello World" ./encode ./decode -v
> Hello World
> Compressed file size: 53 bytes
> Decompressed file size: 12 bytes
> Space saving: -341.67%
```
