#include "stack.h"
#include "node.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//stack struct
struct Stack {
    uint32_t top;
    uint32_t capacity;
    Node **items;
};
//create stack
Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    //allocate memory
    if (s) {
        //if the stack is true then set top to 0
        s->top = 0;
        //set capacity to the capacity passed
        s->capacity = capacity;
        //allocate memory for node items
        s->items = (Node **) calloc(capacity, sizeof(Node *));
        //if items is false
        if (!s->items) {
            //free the stack
            free(s);
            //set the stack to null
            s = NULL;
        }
    }
    //return the stack
    return s;
}

void stack_delete(Stack **s) {
    //if the stack and items in the stack is true
    if (*s && (*s)->items) {
        //free the items in the stack
        free((*s)->items);
        //free the stack
        free(*s);
        //set the stack to NULL
        *s = NULL;
    }
    return;
}

bool stack_empty(Stack *s) {
    //if the top of the stack is 0 then return true because it's empty
    if (s->top == 0) {
        return true;
    }
    //if not then return false 
    else {
        return false;
    }
}

bool stack_full(Stack *s) {
    //if the top and capacity is equal then it is full, return true
    if (s->top == s->capacity) {
        return true;
    } 
    //else return false
    else {
        return false;
    }
}
//return the stack size, which is top
uint32_t stack_size(Stack *s) {
    return s->top;
}
bool stack_push(Stack *s, Node *n) {
    //if the stack is full then return false
    if (stack_full(s)) {
        return false;
    } 
    //if not full then the last item in the stack is the node passed
    else {
        s->items[s->top] = n;
        //add one to top
        s->top++;
        //return true for success
        return true;
    }
}

bool stack_pop(Stack *s, Node **n) {
    //if the stack is empty
    if (stack_empty(s)) {
        //return false
        return false;
    } 
    else {
        //set the node to the last item in the stack
        *n = s->items[s->top];
        //decrement top
        s->top--;
        //return true for success
        return true;
    }
}

void stack_print(Stack *s){
    //print statement for dubbging, printing the items in the node
    for (uint32_t i = 0; i < s->top; i++) {
        node_print(s->items[i]);
    }
}
