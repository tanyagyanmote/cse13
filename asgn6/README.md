# Assignment 6 - The Great Firewall of Santa Cruz: Bloom Filters, Linked Lists and Hash Tables

The purpose of this assignment is to keep the citizens of Santa Cruz in check as the glorious leader of the Glorious People's Republic of Santa Cruz. Creating a system for filtering out using the bloom filter, linked lists, nodes, bit vectors, and hashtables. The bloom filter, which is made up of bit vectors, is used. A bit vector is a one-dimensional array represented by an abstract data type. A hash table is a data structure that maps keys to values; it is a node that interacts with other data structures. A linked list is made up of nodes, each of which has a data field and a reference to the next node. Words such as bad speak and newspeak are being filtered out, in banhammer which responds to certain messages

## Files

1. Banhammer.c - contains main()
2. Messages.h - Defines the mixspeak, badspeak, and goodspeak messages that are used in banhammer
3. Cityhash.h - Defines the interface for the hash function using CityHash
4. Cityhash.c - Contains the implementation of the hash function using CityHash
5. Ht.h - Defines the interface for the hash table ADT
6. Ht.c - Contains the implementation of the hash table ADT.
7. Ll.h - Defines the interface for the linked list ADT
8. Ll.c - Contains the implementation of the linked list ADT.
9. Node.h - Defines the interface for the node ADT
10. Node.c - Contains the implementation of the node ADT
11. Bf.h - Defines the interface for the Bloom filter ADT.
12. Bf.c - Contains the implementation of the Bloom filter ADT.
13. Bv.h - Defines the interface for the bit vector ADT.
14. Bv.c - Contains the implementation of the bit vector ADT.
15. Paser.h - Defines the interface for the parsing module
16. Paser.c - Contains the implementation of the parsing module.
17. Makefile - Contains program compliation
18. Readme.md - Contains a description and usage of program
19. Writeup -Analysis of program

## How to build
1. Have all the .c and .h files to be able to build the program

To construct all the files 
```python
make 

```

## Usage
Once the program is built, banhammer can be called using 
```python
./banhammer 

-h: prints out help message
-t: sets the hash table size
-f: sets the bloom filter size
-s: prints stats of the program
```