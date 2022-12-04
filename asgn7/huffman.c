#include <stdio.h>
#include <stdlib.h>
#include "pq.h"
#include "code.h"
#include "node.h"
#include "io.h"
#include "stack.h"

//puesdocode given
Node *build_tree(uint64_t hist[static ALPHABET]){
    PriorityQueue *q = pq_create(ALPHABET);
    Node *parent_node;
    Node *n;
    Node *left;
    Node *right;
    Node *root;
    //itering though the histogram
    for(uint64_t i = 0; i < ALPHABET; i++){
        //creating node if the element in the hist is greater than 0
        if(hist[i]>0){
            n = node_create(i,hist[i]);
            //enqueuing it into the pq
            enqueue(q,n);
        }
    }
    //while the length of the pq is greater than or equal to 2
    while(pq_size(q) >= 2){
        //dequeue the left and right of the pq
        dequeue(q,&left);
        dequeue(q,&right);
        //join the left and right
        parent_node = node_join(left,right);
        //enqueu the joined node to the pq
        enqueue(q,parent_node);
    }
    //dequeue the root
    dequeue(q,&root);
    //delete the pq
    pq_delete(&q);
    //return the root
    return root;
}
//creating outside the fcn
Code c = {0,{0}};
//puesdocode given
void build_codes(Node *root, Code table[static ALPHABET]){
    //creating bc of pop bit
    uint8_t x;
    //checking if root isn't NULL
    if(root){
        //if left and right are false
        if(!root->left && !root->right){
            table[root->symbol] = c;
        }
        else{
            code_push_bit(&c,0);
            build_codes(root->left,table);
            code_pop_bit(&c,&x);

            code_push_bit(&c,1);
            build_codes(root->right,table);
            code_pop_bit(&c,&x);
        }
    }
    
}
//puesdocode given
void dump_tree(int outfile, Node *root){
    //creating variables so I can use them in writebytes
    uint8_t l = 'L';
    uint8_t i = 'I';
    //checking if root is valid
    if(root){
        //dumping the tree with left and right root to outfile
        dump_tree(outfile,root->left);
        dump_tree(outfile,root->right);
        //if they both are nULL then write the lead node
        if(!root->left && !root->right){
            write_bytes(outfile,&l,1);
            write_bytes(outfile,&root->symbol,1);
        }
        else{
            //write the interior node
            write_bytes(outfile,&i,1);
        }
    }
}

Node *rebuild_tree(uint16_t nbytes, uint8_t tree_dump[static nbytes]){
    //creating variable to set symbol of tree_dump to
    uint8_t s;
    //creating stack of nodes to reconstruct the tree
    Node *right = NULL;
    Node *left = NULL;
    Node *root = NULL;
    //creating the stack with nybtes
    Stack *stack = stack_create(nbytes);
    //for loop to iterate thorugh the tree_dump
    for(int i = 0; i < nbytes; i += 1){
        //setting the element to s so I can compare
        s = tree_dump[i];
        if(s == 'L'){
            //if s is 'L', make the next element a symbol for the leaf node
            //using that symbol create a new node
            Node *l = node_create(tree_dump[i+1], 0);
            //push the node into the stack
            stack_push(stack, l);
            //edge case
            i=i+1;
        }
        if(s == 'I'){
            //if the element in the array is an 'I'
            //pop the right child, and left in that order
            stack_pop(stack, &right);
            stack_pop(stack, &left);
            //join the left and right nodes
            Node *parent_node = node_join(left, right);
            //pushing the parent node into the stack
            stack_push(stack, parent_node);
        }
    }
    //popping the root
    stack_pop(stack,&root);
    //deleting the stack
    stack_delete(&stack);
    //retuning the root
    return root;
}

void delete_tree(Node **root){
    //checking if root is not NULL
    if(*root){
        //deleting the left and right of the root
        delete_tree(&(*root)->left);
        delete_tree(&(*root)->right);
        //node delete the root
        node_delete(root);
    }
}
