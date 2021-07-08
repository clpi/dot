#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct Graph Graph;

typedef struct Node Node;

struct Node {
    int vertex;
    int key;
    Node* next;
};

struct Graph {
    int* key_list;
    int v;
    Node** adj_lists;
};

typedef struct StackNode StackNode;

struct StackNode{
    int data;
    StackNode* next;
};

int is_empty(StackNode* stack) {
    if (!stack)
        return 1;
    return 0;
}

StackNode* push(StackNode* stack, int data) {
    StackNode* node = (StackNode*) malloc (sizeof(StackNode));
    StackNode* temp = stack;
    node->data = data;
    node->next = temp;
    stack = node;
    return stack;
}

StackNode* pop(StackNode* stack) {
    if (!stack)
        return 0;
    StackNode* temp = stack;
    stack = stack->next;
    temp->next = 0;
    free(temp);
    return stack;
}

int top(StackNode* stack) {
    if (!stack)
        return 0;
    return stack->data;
}

StackNode* init_stack(int data) {
    StackNode* stack = (StackNode*) malloc (sizeof(StackNode));
    stack->data = data;
    stack->next = 0;
    return stack;
}

void free_stack(StackNode* stack) {
    if (!stack)
        return;
    StackNode* temp = stack;
    stack = stack->next;
    temp->next = 0;
    free(temp);
    free_stack(stack);
}

void print_stack(StackNode* stack) {
    if (!stack)
        return;
    StackNode* temp = stack;
    printf("Stack: \n");
    while(temp) {
        printf("Data: %d -> ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}
