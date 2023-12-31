#include "ll.h"
#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint64_t seeks = 0;
uint64_t links = 0;

// lecture 05 inspiration
int my_strcmp(char string1[], char string2[]) {
  int iter = 0;
  while (string1[iter] || string2[iter]) {
    if (string1[iter] != string2[iter]) {
      char cmp = string1[iter] - string2[iter];
      return cmp;
    }
    iter += 1;
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

LinkedList *ll_create(bool mtf) {
  char dummy1[1024] = "fdfsdfwpm";
  char dummy2[1024] = "fwpfdsfsm";
  char dummy3[1024] = "fwopdffmw";
  char dummy4[1024] = "fwopfmffw";
  // creating dummy chars to allocate space when creating the node
  LinkedList *ll = (LinkedList *)malloc(sizeof(LinkedList));
  // null check
  if (!ll) {
    return NULL;
  } else {
    // creating the sentional head and tail
    ll->head = node_create(dummy1, dummy2);
    ll->head->prev = NULL;
    ll->tail = node_create(dummy3, dummy4);
    ll->tail->next = NULL;
    // setting pointers to point to node
    ll->head->next = ll->tail;
    ll->tail->prev = ll->head;
    // setting length to 0, mtf to mtf
    ll->length = 0;
    ll->mtf = mtf;
  }
  return ll;
}

void ll_delete(LinkedList **ll) {
  // delete the node
  Node *c = (*ll)->head;
  while (c) {
    Node *n = c;
    c = c->next;
    node_delete(&n);
  }
  free(*ll);
  *ll = NULL;
}

uint32_t ll_length(LinkedList *ll) { return ll->length; }

Node *ll_lookup(LinkedList *ll, char *oldspeak) {
  // add to seeks
  seeks += 1;
  if (ll != NULL) {
    // looping through the linked list
    for (Node *curr = ll->head->next; curr != ll->tail; curr = curr->next) {
      // checking if oldspeak is the one i'm looking for
      if (my_strcmp(curr->oldspeak, oldspeak) == 0) {
        if (ll->mtf != false) {
          // moving node from one position and inserting it at the front
          (curr->next)->prev = curr->prev;
          (curr->prev)->next = curr->next;
          (curr->next) = ll->head->next;
          ll->head->next->prev = curr;
          curr->prev = ll->head;
          ll->head->next = curr;
        }
        links += 1;
        return curr;
      }
      links += 1;
    }
  }
  return NULL;
}

void ll_insert(LinkedList *ll, char *oldspeak, char *newspeak) {
  // using ll_lookup, if its null then create node
  // inserting at the front
  if (ll_lookup(ll, oldspeak) == NULL) {
    Node *insert = node_create(oldspeak, newspeak);
    ll->length += 1;
    insert->prev = ll->head;
    insert->next = ll->head->next;
    ll->head->next = insert;
    insert->next->prev = insert;
  }
}

void ll_print(LinkedList *ll) {
  // printing linked list
  if (ll != NULL) {
    for (Node *temp = ll->head->next; temp != NULL; temp = temp->next) {
      if (temp != ll->tail && temp != ll->head) {
        node_print(temp);
      }
    }
  }
}

void ll_stats(uint32_t *n_seeks, uint32_t *n_links) {
  *n_seeks = seeks;
  *n_links = links;
}
