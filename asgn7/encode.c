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

//printing help 
void print_help(){
	fprintf(stderr, "SYNOPSIS\n");
	fprintf(stderr, "  A Huffman encoder.\n");
	fprintf(stderr, "  Compresses a file using the Huffman coding algorithm.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "USAGE\n");
	fprintf(stderr, "  ./encode [-h] [-i infile] [-o outfile]\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "OPTIONS\n");
	fprintf(stderr, "  -h             Program usage and help.\n");
	fprintf(stderr, "  -v             Print compression statistics.\n");
	fprintf(stderr, "  -i infile      Input file to compress.\n");
	fprintf(stderr, "  -o outfile     Output of compressed data.\n");
}

int main(int argc, char **argv) {
	int opt = 0;
	int inputfile = STDIN_FILENO;
 	int outputfile = STDOUT_FILENO;
	//usage of temo file from lev, nishant office hours
	int tmp = fileno(tmpfile());
	bool compression_stats = false;
	int symbol = 0;
	uint64_t histogram[ALPHABET];
	struct stat s;
	uint8_t buf[BLOCK] = { 0 };
	ssize_t bytes;
	uint8_t letter;
	while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
		switch (opt) {
			case 'h':
				//for help 
				print_help();
				return 0;
			case 'i':
				//opening inputfile
				inputfile = open(optarg, O_RDONLY);
				break;
			case 'o':
				//opening outfile file
			    outputfile = open(optarg, O_WRONLY | O_CREAT);
				break;
			case 'v':
				//if called setting boolean to true
				compression_stats = true;
				break;
			default:
				print_help();
				return -1;
		}
	}
	//initialzing the histogram, setting to 0
	for (uint16_t i = 0; i <ALPHABET; i += 1) {
		histogram[i] = 0;
	}
	//reading the bits of the input file, writing it to the output file
	while((bytes = read_bytes(inputfile,&letter,1)) != 0){
		//citation - nishant TA hours
		write_bytes(tmp,&letter,1);
	}
	//resets the position usage of lseek, Discord chat of asgn7: Heston#6999
	lseek(tmp,0,SEEK_SET);
	while((bytes = read_bytes(tmp,&letter,1)) != 0){
		for (int i = 0; i < bytes; i++) {
            histogram[letter] += 1;
		}
	}
	//resets the position
	lseek(tmp,0,SEEK_SET);
	//checking if the first two symbols have a count of 0
	//set to 1
	if (histogram[0] == 0){
		histogram[0] = 1;
	}
	if (histogram[1] == 0){
		histogram[1] = 1;
	}
	//constructing huffman tree
	Node *huff_tree = build_tree(histogram);
	//constructing code table
	Code table_code[ALPHABET] = {0};
	//transversing through the huffman tree
	build_codes(huff_tree,table_code);
	// for(int i =0; i< ALPHABET; i++){
	// 	printf("table_Code[%d]: ", i);
	// 	code_print(&(table_code[i]));
	// }
	//reseting stats because only want bytes for final read
	bytes_read = 0;
	bytes_written = 0; 
	//Discord chat of asgn7: Athanasius Kircher#1515
	fstat(tmp,&s);
	//creating header
  	Header m;
	//setting all header struct
	//magic to MAGIC from defines.h
    m.magic = MAGIC;
	//Discord chat of asgn7: Athanasius Kircher#1515
    m.file_size = s.st_size; 
	fchmod(outputfile,m.permissions);
    for (int i = 0; i < ALPHABET; i++) {
        if (histogram[i] > 0) {
            symbol += 1;
        }
    }
	//Discord chat of asgn7: Athanasius Kircher#1515
	m.permissions = s.st_mode;
	//tree size -> (3 times symbol) -1
	m.tree_size = (3 * symbol) - 1;
	write_bytes(outputfile, (uint8_t *) &m, sizeof(m));
	//dump tree once the tree is constructed 
    dump_tree(outputfile, huff_tree); 
	//writing corresponsding code for each symbol to outfile
	while ((bytes = read_bytes(tmp, buf, BLOCK)) > 0) {
        for (int i = 0; i < bytes; i++) {
            write_code(outputfile, &table_code[buf[i]]);
        }
    }
	//flush remaning buffered code
	flush_codes(outputfile); 
	//compression statistics
	if (compression_stats == true){
        fprintf(stderr, "Uncompressed file size: %lu bytes\n", bytes_read);
        fprintf(stderr, "Compressed file size: %lu bytes\n", bytes_written);
        fprintf(stderr, "Space saving: %.2f%%\n", 100 * (1 - ((float) bytes_written/ bytes_read)));
	}
	//deleting tree/closing files
	delete_tree(&huff_tree);
	close(inputfile);
	close(outputfile);

}







