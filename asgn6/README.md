# Assignment 6 - The Great Firewall of Santa Cruz: Bloom Filters, Linked Lists and Hash Tables

The purpose of this assignment is to keep the citizens of Santa Cruz in check as the glorious leader of the Glorious People's Republic of Santa Cruz. Creating a system for filtering out using the bloom filter, linked lists, nodes, bit vectors, and hashtables. In the bloom filter, which is composed of bit vectors, that is a one-dimensional array. A hash table is a structure that converts keys to values; nodes communicate with other data structures. A linked list consists of nodes, each of which refers to the next node. In the Bloom filter, the hash of a word is found using salts and indexes it using bit vectors. The hash table incorporates linked lists and is stored by nodes. All of the badspeak or newspeak phrases will be filtered in the message, similar to the bloom filter. Then in banhammer, it either punishes or rewards the user for their poor or good message.

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
Example  output 
```python
echo "Hi! How was your day?" | ./banhammer -s

ht keys: 14569
ht hits: 0
ht misses: 0
ht probes: 0
bf keys: 14680
bf hits: 0
bf misses: 5
bf bits examined: 5
Bits examined per miss: 1.000000
False positives: 0.000000
Average seek length: 0.000000
Bloom filter load: 0.129900
```
