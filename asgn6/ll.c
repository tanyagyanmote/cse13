#include "ll.h"
#include <stdlib.h>
#include "node.h"
#include <stdio.h>
#include <string.h>

uint64_t seeks = 0;
uint64_t links = 0;

//lecture 05
int my_strcmp(char s[], char t[]){
    for(int i =0; s[i] || t[i]; i++){
        if (s[i] != t[i]){
            return s[i] - t[i];
        }
    }
    return 0;
}

struct LinkedList {
    // given to us
    uint32_t length;
    Node *head; 
    Node *tail; 
    bool mtf;
};

LinkedList *ll_create(bool mtf){
    char dummy1[1024] = "fdfsdfwpm";
    char dummy2[1024] = "fwpfdsfsm";
    char dummy3[1024] = "fwopdffmw";
    char dummy4[1024] = "fwopfmffw";

    LinkedList *ll = (LinkedList *) malloc(sizeof(LinkedList));
    if (!ll) {
        return NULL;
    }
    else{
        ll->head = node_create(dummy1, dummy2);
        ll->head->prev = NULL;
        ll->tail = node_create(dummy3, dummy4);
        ll->tail->next = NULL;

        ll->head->next = ll->tail;
        ll->tail->prev = ll->head;

        ll->length = 0;
        ll->mtf = mtf;
    }
    return ll;
}

void ll_delete(LinkedList **ll){
    Node *c = (*ll)->head;
    while (c) {
        Node *n = c;
        c = c->next;
        node_delete(&n);
    }
    free(*ll);
    *ll = NULL;
}

uint32_t ll_length(LinkedList *ll){
    return ll->length;
}

Node *ll_lookup(LinkedList *ll, char *oldspeak){
    //add to seeks
    seeks += 1;
    if (ll != NULL){
        for (Node *curr = ll -> head -> next; curr != ll->tail; curr = curr -> next){
            if (my_strcmp(curr->oldspeak,oldspeak)==0){
                if (ll -> mtf != false){
                    (curr -> next) -> prev = curr -> prev;
                    (curr ->prev) -> next = curr ->next;
                    (curr -> next) = ll -> head -> next;
                    ll -> head -> next ->prev = curr;
                    curr -> prev = ll -> head;
                    ll -> head -> next = curr; 
                }
            links += 1;
            return curr;
            }
            links += 1;
        } 
    }
    return NULL;
}

void ll_insert(LinkedList *ll, char *oldspeak, char *newspeak){
    if (ll_lookup(ll,oldspeak) == NULL) {
        Node *insert = node_create(oldspeak, newspeak);
        ll->length += 1;
        insert->prev = ll->head;
        insert->next = ll->head->next;
        ll->head->next = insert;
        insert->next->prev = insert;
    }
}

void ll_print(LinkedList *ll){
    if (ll != NULL){
        for(Node *temp = ll->head->next;temp != NULL; temp = temp -> next){
            if(temp != ll->tail && temp != ll->head){
                node_print(temp);
            }
        }
    }
}

void ll_stats(uint32_t *n_seeks, uint32_t *n_links){
    *n_seeks = seeks;
    *n_links = links;
}



