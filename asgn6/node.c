#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "node.h"

// given to us in lecture 5 from professor miller

int my_strlen(char s[]){
    int length = 0;
    while (s[length]){
        length += 1;
    }
    return length;
}

void my_strcpy(char dst[], char src[]){
    int i;
    for (i = 0; !!src[i];i+=1){
        dst[i] = src[i];
    }
    dst[i] = '\0';
    return;
}

 
Node *node_create(char *oldspeak, char *newspeak){
    // allocating memory for word
    Node* node = (Node*) calloc(1, sizeof(Node));
    //null check
    if (node){
        // NULL set
        node -> prev = NULL;
        node -> next = NULL;
        char* o = (char*) calloc(my_strlen(oldspeak) + 1, sizeof (char));
        my_strcpy(o, oldspeak);
        node -> oldspeak = o;
        // null check
        if (newspeak == NULL){
            node -> newspeak = newspeak;
        } else {
            char* n = (char*) calloc(my_strlen(newspeak) + 1, sizeof (char));
            my_strcpy(n, newspeak);
            node -> newspeak = n;
        }
    }
    return node;
    
}

void node_delete(Node **n){
    if (*n) {
        free((*n)->oldspeak);
        free((*n)->newspeak);
        free(*n);
        *n = NULL;
    }
}

void node_print(Node *n){
    //functions to print node
    if(n != NULL){
        if (n->newspeak && n-> oldspeak) {
            printf("%s -> %s\n", n->oldspeak, n->newspeak);
        }
        if(n->newspeak == NULL && n-> oldspeak != NULL) {
            printf("%s\n", n->oldspeak);
        }
    }
}






