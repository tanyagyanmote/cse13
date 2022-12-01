#include "pq.h"
#include "node.h"
#include <stdio.h>
#include <stdlib.h>

struct PriorityQueue{
    uint32_t size;
    uint32_t capacity;
    Node **items;
};
//left helper function for heap
uint32_t l_child(uint32_t n){
    return (2*n+1);
}
//right helper function for heap
uint32_t r_child(uint32_t n){
    return (2*n+2);
}
//parent helper function for heap
uint32_t parent(uint32_t n){
    return ((n-1)/2);
}

PriorityQueue *pq_create(uint32_t capacity){
    PriorityQueue *q = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    if (q) {
        q->size = 0;
        q->capacity = capacity;
        q->items = (Node **) calloc(capacity, sizeof(Node *));
        if (!q->items) {
            free(q);
            q = NULL;
        }
    }
    return q;
}
//if the pq isn't null then free the items and the priority queue
void pq_delete(PriorityQueue **q){
    //checking if pq is true
    if (*q){
        //checking if items are true
        if((*q)->items){
            //if it is then free the items
            free((*q)->items);
            //free the pq
            free(*q);
            //set q to NULL
            *q = NULL;
        }
    }
    return;
}
//if the size of the pq is 0, then its empty
bool pq_empty(PriorityQueue *q){
    if (q->size == 0) {
        return true;
    }
    return false;
}
//if the size and capacity are equal to eachother its full
bool pq_full(PriorityQueue *q){
    if (q->size == q->capacity) {
        return true;
    }
    return false;
}
//getting the size using size from struct
uint32_t pq_size(PriorityQueue *q){
    return q->size;
}
//creating swap function
void swap(Node **a, Node **b) {
    //creating a temp node
    Node *x = *a;
    //swap the nodes using temp node
    *a = *b;
    *b = x;
}
//using up_heap implementation from assignment 4
void up_heap(PriorityQueue *q, uint32_t n){
    while(n > 0 && node_cmp(q->items[parent(n)],q->items[n])){
        swap(&q->items[parent(n)],&q->items[n]);
        n = parent(n);
    }
}
//logic from tutor office hours
bool enqueue(PriorityQueue *q, Node *n){
    //checking if pq is full
    if(pq_full(q)){
        return false;
    }
    //set the last item in the pq to the node passed
    q->items[q->size] = n;
    //increment the size of the pq, bc one node is added
    q->size +=1;
    //call helper up_heap function and subtract the size by 1
    up_heap(q,pq_size(q)-1);
    //returning true if success
    return true;
}
//using down_heap implementation from assignment 4
void down_heap(PriorityQueue *q, uint32_t size){
    uint32_t smaller;
    uint32_t n = 0;
    while(l_child(n) < size){
        if (r_child(n) == size){
            smaller = l_child(n);
        }
        else {
            if (node_cmp(q->items[l_child(n)],q->items[r_child(n)]) == false){
                smaller = l_child(n);
            }
            else{
                smaller = r_child(n);
            }
        }
        if (node_cmp(q->items[n],q->items[smaller]) == false){
            break;
        }
        swap(&q->items[n],&q->items[smaller]);
        n = smaller;
    }
}
//logic from tutor office hours
bool dequeue(PriorityQueue *q, Node **n){
    //checking if pq is empty
    if(pq_empty(q)){
        return false;
    }
    //making the node the first item in the pq
    *n = q -> items[0];
    //decrementing the size
    q->size -= 1;
    //setting the first item in the item as the last of the pq
    q->items[0] = q->items[q->size];
    //calling helper downheap fucntion
    down_heap(q,q->size);
    //returning true if successful
    return true;

}
//print statement to debug
void pq_print(PriorityQueue *q){
    for (uint32_t i = 0; i < q->size; i++) {
        node_print(q->items[i]);
    }
}

