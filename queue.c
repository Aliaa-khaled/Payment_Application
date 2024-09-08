#include "queue.h"
Queue transactionQueue;
// تهيئة الطابور
void initializeQueue(Queue* q) {
    q->front = NULL;
    q->rear = NULL;
}

// التحقق مما إذا كان الطابور فارغًا
int isQueueEmpty(Queue* q) {
    return q->front == NULL;
}

// إضافة عنصر إلى الطابور
void enqueue(Queue* q, ST_transaction_t transaction) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    newNode->transaction = transaction;
    newNode->next = NULL;

    if (isQueueEmpty(q)) {
        q->front = newNode;
        q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

// إزالة عنصر من الطابور
ST_transaction_t dequeue(Queue* q) {
    ST_transaction_t emptyTransaction;
    memset(&emptyTransaction, 0, sizeof(emptyTransaction));

    if (isQueueEmpty(q)) {
        printf("Queue is empty. Cannot dequeue.\n");
        return emptyTransaction;
    }

    Node* temp = q->front;
    ST_transaction_t transaction = temp->transaction;
    q->front = q->front->next;

    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(temp);
    return transaction;
}

// عرض محتويات الطابور (اختياري)
void displayQueue(Queue* q) {
    if (isQueueEmpty(q)) {
        printf("Queue is empty.\n");
        return;
    }

    Node* temp = q->front;
    while (temp != NULL) {
        printf("Transaction Sequence Number: %u, Card Holder: %s, Amount: %.2f\n",
               temp->transaction.transactionSequenceNumber,
               temp->transaction.cardHolderData.cardHolderName,
               temp->transaction.terminalData.transAmount);
        temp = temp->next;
    }
}

