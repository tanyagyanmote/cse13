#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "node.h"

// given to us in lecture 5

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
    char* old = (char*) calloc(my_strlen(oldspeak) + 1, sizeof (char));
    // string copy the word into the memory
    my_strcpy(old, oldspeak);
    node -> oldspeak = old;
    // null check
    if (newspeak){
        char* new = (char*) calloc(my_strlen(newspeak) + 1, sizeof (char));
        my_strcpy(new, newspeak);
        node -> newspeak = new;
    } else {
        node -> newspeak = newspeak;
    }
    // set next and prev to NULL
    node -> next = NULL;
    node -> prev = NULL;

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
    if(n != NULL){
        if (n->newspeak && n-> oldspeak) {
            printf("%s -> %s\n", n->oldspeak, n->newspeak);
        } if(n->newspeak == NULL && n-> oldspeak != NULL) {
            printf("%s\n", n->oldspeak);
        }
    }
}
