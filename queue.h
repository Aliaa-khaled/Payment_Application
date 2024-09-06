#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include "server.h" // تأكد من تضمين server.h للوصول إلى تعريف ST_transaction_t

typedef struct Node {
    ST_transaction_t transaction;
    struct Node* next;
} Node;

typedef struct Queue {
    Node* front;
    Node* rear;
} Queue;

// دوال الطابور
void initializeQueue(Queue* q);
int isQueueEmpty(Queue* q);
void enqueue(Queue* q, ST_transaction_t transaction);
ST_transaction_t dequeue(Queue* q);
void displayQueue(Queue* q);

#endif // QUEUE_H
