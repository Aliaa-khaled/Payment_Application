#include "application.h"
#include "file_handling.h"
#include "server.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern Queue transactionQueue;
extern AccountNode* accountsHead;

// دالة بدء التطبيق
void appStart(void) {
    // 1. تهيئة النظام
    initializeQueue(&transactionQueue);

    // 2. تحميل البيانات
    printf("Loading accounts from file...\n");
    loadAccountsFromFile("accounts.txt"); // قم بتعديل اسم الملف إذا لزم الأمر

    printf("Loading transactions from file...\n");
    loadTransactionsFromFile("transactions.txt"); // قم بتعديل اسم الملف إذا لزم الأمر

    // 3. معالجة المعاملات
    printf("Processing transactions...\n");
    while (!isQueueEmpty(&transactionQueue)) {
        ST_transaction_t transaction = dequeue(&transactionQueue);
        EN_transStat_t status = recieveTransactionData(&transaction);

        // 4. عرض النتائج
        if (status == APPROVED) {
            printf("Transaction Approved: Seq Number: %u\n", transaction.transactionSequenceNumber);
        } else {
            printf("Transaction Failed: Seq Number: %u, Error Code: %d\n", transaction.transactionSequenceNumber, status);
        }
    }

    // 5. تنظيف الموارد
    // تأكد من إجراء أي تنظيفات ضرورية مثل إغلاق الملفات وتحرير الذاكرة
    printf("Application finished.\n");
}
