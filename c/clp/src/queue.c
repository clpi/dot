#include "queue.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef struct node {
    int data;
    struct node *next;
} NodeT;

typedef struct QueueRep {
    int length;
    NodeT *head;
    NodeT *tail;
} QueueRep;

queue newQueue() {
    queue Q = malloc(sizeof(QueueRep));
    Q->length = 0;
    return Q;
}
