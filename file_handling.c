#include "file_handling.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// تعريف قاعدة بيانات الحسابات باستخدام قائمة مرتبطة

extern AccountNode* accountsHead;

// تعريف قاعدة بيانات المعاملات (من نوع Queue)
extern Queue transactionQueue;

// دوال إدارة الحسابات

void loadAccountsFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    ST_accountsDB_t account;
    while (fscanf(file, "%19s %f %d\n",
                  account.primaryAccountNumber,
                  &account.balance,
                  (int*)&account.state) == 3) {

        // تخصيص ذاكرة لعقدة جديدة
        AccountNode* newNode = (AccountNode*)malloc(sizeof(AccountNode));
        if (newNode == NULL) {
            perror("Failed to allocate memory for new node");
            fclose(file);
            return;
        }

        // تخزين بيانات الحساب في العقدة
        newNode->accountData = account;
        newNode->next = NULL;

        // إذا كانت القائمة فارغة، اجعل العقدة الجديدة هي الرأس
        if (accountsHead == NULL) {
            accountsHead = newNode;
        } else {
            // اجعل current يبدأ من الرأس
            AccountNode* current = accountsHead;

            // التنقل للوصول إلى آخر عقدة في القائمة
            while (current->next != NULL) {
                current = current->next;
            }

            // إضافة العقدة الجديدة في نهاية القائمة
            current->next = newNode;
        }
    }

    fclose(file);
}



void saveAccountsToFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file for saving accounts");
        return;
    }

    AccountNode* current = accountsHead;
    while (current != NULL) {
        fprintf(file, "%s %.2f %d\n",
                current->accountData.primaryAccountNumber,
                current->accountData.balance,
                current->accountData.state);
        current = current->next;
    }

    fclose(file);
}


// دوال إدارة المعاملات

void loadTransactionsFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        ST_transaction_t transaction;
        char cardHolderName[25], primaryAccountNumber[20], cardExpirationDate[6];
        char transactionDate[11];
        float transAmount, maxTransAmount;

        // قراءة البيانات من السطر
        sscanf(line, "%[^,],%[^,],%[^,],%f,%[^,],%f",
               cardHolderName, primaryAccountNumber, cardExpirationDate, &transAmount, transactionDate, &maxTransAmount);

        // تعيين القيم إلى هيكل المعاملة
        strncpy(transaction.cardHolderData.cardHolderName, cardHolderName, sizeof(transaction.cardHolderData.cardHolderName) - 1);
        strncpy(transaction.cardHolderData.primaryAccountNumber, primaryAccountNumber, sizeof(transaction.cardHolderData.primaryAccountNumber) - 1);
        strncpy(transaction.cardHolderData.cardExpirationDate, cardExpirationDate, sizeof(transaction.cardHolderData.cardExpirationDate) - 1);
        transaction.terminalData.transAmount = transAmount;
        strncpy(transaction.terminalData.transactionDate, transactionDate, sizeof(transaction.terminalData.transactionDate) - 1);
        transaction.terminalData.maxTransAmount = maxTransAmount;
        transaction.transState = INTERNAL_SERVER_ERROR; // افتراضي
        transaction.transactionSequenceNumber = 0; // افتراضي

        // إضافة المعاملة إلى الطابور
        enqueue(&transactionQueue, transaction);
    }

    fclose(file);
}

void saveTransactionsToFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    // استخدم مؤشر مؤقت للتكرار عبر الطابور
    Node* current = transactionQueue.front;

    while (current != NULL) {
        ST_transaction_t transaction = current->transaction;

        fprintf(file, "%s,%s,%s,%f,%s,%f\n",
                transaction.cardHolderData.cardHolderName,
                transaction.cardHolderData.primaryAccountNumber,
                transaction.cardHolderData.cardExpirationDate,
                transaction.terminalData.transAmount,
                transaction.terminalData.transactionDate,
                transaction.terminalData.maxTransAmount);

        // انتقل إلى العقدة التالية
        current = current->next;
    }

    fclose(file);
}

