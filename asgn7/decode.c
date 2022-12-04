#include <stdio.h>
#include <stdint.h>
#include <getopt.h>
#include "defines.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "node.h"
#include "code.h"
#include "stack.h"
#include "pq.h"
#include "io.h"
#include "huffman.h"
#include "header.h"
#include <fcntl.h>
#include <ctype.h>
#include <inttypes.h>


#define OPTIONS "i:o:vh"

//print help function 
void print_help2(){
	fprintf(stderr, "SYNOPSIS\n");
	fprintf(stderr, "  A Huffman decoder.\n");
	fprintf(stderr, "  Decompresses a file using the Huffman coding algorithm.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "USAGE\n");
	fprintf(stderr, "  ./decode [-h] [-i infile] [-o outfile]\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "OPTIONS\n");
	fprintf(stderr, "  -h             Program usage and help.\n");
	fprintf(stderr, "  -v             Print compression statistics.\n");
	fprintf(stderr, "  -i infile      Input file to decompress.\n");
	fprintf(stderr, "  -o outfile     Output of decompressed data.\n");
}

int main(int argc, char **argv) {
	//setting variables, giving it default values
	int opt = 0;
	int inputfile = STDIN_FILENO;
 	int outputfile = STDOUT_FILENO;
	bool compression_stats = false;
	uint64_t symbol = 0;
	while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
		switch (opt) {
			case 'h':
				//call help function
				print_help2();
				return 0;
			case 'i':
				//opening the inputfile
				inputfile = open(optarg, O_RDONLY);
				break;
			case 'o':
				//opening the output file
			    outputfile = open(optarg, O_WRONLY | O_CREAT);
				break;
			case 'v':
				//setting compression stats to true
				compression_stats = true;
				break;
			default:
				//default is to print help and return -1
				print_help2();
				return -1;
		}
	}
	//creating header m to read in the infile
	Header m;
	//reading in the infile with the header
	read_bytes(inputfile,(uint8_t *) &m,sizeof(Header));
	//creating a struct for stat s to get the permissions, and file size
	struct stat s;
	fstat(inputfile,&s);
	//setting the permissions, indicating what the outfile should be set to
	fchmod(outputfile,m.permissions);
	//creating a tree with the tree size of the header
	uint8_t d_tree[m.tree_size];
	//readinng the dumped tree from inputfile with the array which is tree_size long
	read_bytes(inputfile,d_tree,m.tree_size);
	//creating a huffman tree with rebuild tree
	Node *huff_tree = rebuild_tree(m.tree_size,d_tree);
    uint8_t bit;
	//creating the root of the huffman tree
	Node *root = huff_tree;
	//making sure the symbol is less than file size, checking if all symbols are read
	while(symbol < m.file_size){
		// fprintf(stderr, "got here\n");
		//checking if we could read something
		if (read_bit(inputfile, &bit)){
			// fprintf(stderr, "bit val: %u\n", bit);
			//checking if root of left and right are NULL
			if(root->left == NULL){
				if(root->right==NULL){
					//if the root we are at is a leaf node then write the symbol to the outputfile
					write_bytes(outputfile, &root->symbol,1);
					//adding one to symbol counter variable
					symbol += 1;
					//reseting the current node back to the root of the tree
					root = huff_tree;
				}
			}
			//if the bit value 1 is read, walk down the right child
			if(bit==1){
				root = root -> right;
			}
			//if the bit value 0 is read, walk down the left child
			if(bit==0){
				root = root -> left;
			}
		}
		else{
			break;
		}
	}
	//flag for compressions stats is true then print out the stats
	if (compression_stats == true){
		//getting stats from io write bytes and read bytes
        fprintf(stderr, "Compressed file size: %lu bytes\n", bytes_read);
        fprintf(stderr, "Decompressed file size: %lu bytes\n", bytes_written);
		//getting the space saving from the given equation
        fprintf(stderr, "Space saving: %.2f%%\n", 100 * (1 - ((float)bytes_read / bytes_written)));
	}
	//deleting tree at the end
	delete_tree(&huff_tree);
	close(inputfile);
	close(outputfile);

}

