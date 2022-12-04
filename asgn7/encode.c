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
void spaces(int space) {
    int counter = 0;
    while(counter < space) {
        fprintf(stderr, " ");
        fflush(stdout);
        counter += 1;
    }
}

void printTree(Node *t, int depth) {
    if (t) {
        printTree(t->left, depth + 1);
        spaces(4 * depth);
        if (t->symbol != '$') {
            if (isgraph(t->symbol)) {
                fprintf(stderr, "'%c' (%" PRIu64 ")\n", t->symbol, t->frequency);
            } else {
                fprintf(stderr, "0x%02X (%" PRIu64 ")\n", t->symbol, t->frequency);
            }
        } else {
            fprintf(stderr, "$ (%" PRIu64 ")\n", t->frequency);
        }
        printTree(t->right, depth + 1);
    }
    return;
}
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
	//lev, nishant -> temp file
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
				compression_stats = true;
			default:
				print_help();
				return -1;
		}
	}

	for (uint16_t i = 0; i <ALPHABET; i += 1) {
		histogram[i] = 0;
	}

	while((bytes = read_bytes(inputfile,&letter,1)) != 0){
		write_bytes(tmp,&letter,1);
	}
	//resets the position
	lseek(tmp,0,SEEK_SET);

	while((bytes = read_bytes(tmp,&letter,1)) != 0){
		for (int i = 0; i < bytes; i++) {
            histogram[letter] += 1;
		}
	}

	lseek(tmp,0,SEEK_SET);

	if (histogram[0] == 0){
		histogram[0] = 1;
	}
	if (histogram[1] == 0){
		histogram[1] = 1;
	}

	Node *huff_tree = build_tree(histogram);
	//printTree(huff_tree,0);
	Code table_code[ALPHABET] = {0};
	build_codes(huff_tree,table_code);
	// for(int i =0; i< ALPHABET; i++){
	// 	printf("table_Code[%d]: ", i);
	// 	code_print(&(table_code[i]));
	// }

	fstat(tmp,&s);
  	Header m;
    m.magic = MAGIC;
	m.permissions = s.st_mode;
	fchmod(outputfile,m.permissions);

    for (int i = 0; i < ALPHABET; ++i) {
        if (histogram[i] > 0) {
            symbol += 1;
        }
    }

	m.tree_size = (3 * symbol) - 1;
    m.file_size = s.st_size; 

	write_bytes(outputfile, (uint8_t *) &m, sizeof(m));

    dump_tree(outputfile, huff_tree); //

	while ((bytes = read_bytes(tmp, buf, BLOCK)) > 0) {
        for (int i = 0; i < bytes; i++) {
            write_code(outputfile, &table_code[buf[i]]);
        }
    }

	flush_codes(outputfile); // 

	if (compression_stats == true){
        fprintf(stderr, "Uncompressed file size: %lu bytes\n", bytes_read);
        fprintf(stderr, "Compressed file size: %lu bytes\n", bytes_written);
        fprintf(stderr, "Space saving: %lu%c\n", 100 * (1 - (bytes_written / bytes_read)) / 100, '%');
	}

	delete_tree(&huff_tree);
	close(inputfile);
	close(outputfile);

}







