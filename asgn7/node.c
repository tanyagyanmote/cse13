#include "node.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <inttypes.h>

Node *node_create(uint8_t symbol, uint64_t frequency){
    //allocating memeory to create a node
    Node *n = (Node *) malloc(sizeof(Node));
    if (n) {
        //setting left and right ot NULL
        n->left = NULL;
        n->right = NULL;
        //setting symbol to symbol
        n->symbol = symbol;
        //setting frequency to frequency
        n->frequency = frequency;
    }
    return n;
}

void node_delete(Node **n){
    //freeing the node and setting it to NULL
    free(*n);
    *n = NULL;
}

Node *node_join(Node *left, Node *right){
    //creating a joined node called parent, adding the left and right frequency
    Node *parent = node_create('$', (left->frequency + right->frequency));
    //setting the joined left to left 
    parent->left = left;
    //setting the joined right to right
    parent->right = right;
    //returing the parent node
    return parent;
}

void node_print(Node *n){
    //if iscntrl and isprint is false then print with the hex
    if(!iscntrl(n->symbol) && !isprint(n->symbol)){
        printf("0x%02"PRIx8"\n", n->symbol);
    }
    //else print it without the hex
    else{
        printf("%c\n", n->symbol);
    }
    // print frequency regardless
    printf("%lu\n", n->frequency);

}

bool node_cmp(Node *n, Node *m){
    //comparing the frequencies
    if(n-> frequency > m-> frequency){
        return true;
    }
    return false;
}


void node_print_sym(Node *n){
    //if iscntrl and isprint of the symbol is false then print the symbol
    if(!iscntrl(n->symbol) && !isprint(n->symbol)){
        printf("%d", n->symbol);
    }
    //else print it with the 
    else{
        printf("0x%02"PRIx8, n->symbol);

    }
}
