#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "file_handling.h"
#include "queue.h"
#include "terminal.h"
#include "card.h"

extern Queue transactionQueue;
AccountNode* accountsHead = NULL;

void appStart(void) {
    ST_transaction_t transaction;
    EN_transStat_t status;
    EN_terminalError_t terminalError;
    EN_cardError_t cardError;
    float amount;
    f32 maxAmount = 5000.0;  // الحد الأقصى للمبلغ

    // تحميل الحسابات من الملف
    printf("Loading accounts from file...\n");
    loadAccountsFromFile("accounts.txt");

    // تحميل المعاملات من الملف
    printf("Loading transactions from file...\n");
    loadTransactionsFromFile("transactions.txt");

    // معالجة المعاملات
    printf("Processing transactions...\n");
    while (!isQueueEmpty(&transactionQueue)) {
        transaction = dequeue(&transactionQueue);

        // الحصول على اسم صاحب البطاقة
        cardError = getCardHolderName(&transaction.cardHolderData);
        if (cardError != CARD_OK) {
            printf("Error: Invalid Card Holder Name.\n");
            continue;
        }

        // الحصول على تاريخ انتهاء صلاحية البطاقة
        cardError = getCardExpiryDate(&transaction.cardHolderData);
        if (cardError != CARD_OK) {
            printf("Error: Invalid Card Expiry Date.\n");
            continue;
        }

        // الحصول على رقم البطاقة (PAN)
        cardError = getCardPAN(&transaction.cardHolderData);
        if (cardError != CARD_OK) {
            printf("Error: Invalid PAN.\n");
            continue;
        }

        // التحقق من تاريخ المعاملة
        terminalError = getTransactionDate(&transaction.terminalData);
        if (terminalError != TERMINAL_OK) {
            printf("Error: Invalid Transaction Date.\n");
            continue;
        }

        // التحقق إذا كانت البطاقة منتهية الصلاحية
        terminalError = isCardExpired(&transaction.cardHolderData, &transaction.terminalData);
        if (terminalError != TERMINAL_OK) {
            printf("Error: Card Expired.\n");
            continue;
        }

        // الحصول على قيمة المعاملة
        terminalError = getTransactionAmount(&transaction.terminalData);
        if (terminalError != TERMINAL_OK) {
            printf("Error: Invalid Transaction Amount.\n");
            continue;
        }

        // ضبط الحد الأقصى للمبلغ
        terminalError = setMaxAmount(&transaction.terminalData, maxAmount);
        if (terminalError != TERMINAL_OK) {
            printf("Error: Unable to set max amount.\n");
            continue;
        }

        // التحقق إذا كان المبلغ أقل من الحد الأقصى
        terminalError = isBelowMaxAmount(&transaction.terminalData);
        if (terminalError != TERMINAL_OK) {
            printf("Error: Transaction Amount Exceeds Maximum Limit.\n");
            continue;
        }

        // التحقق من صحة الحساب
        status = isValidAccount(&transaction.cardHolderData);
        if (status != ACCOUNT_OK) {
            printf("Error: Invalid Account.\n");
            continue;
        }

        // التحقق من حالة الحساب
        status = isBlockedAccount(&transaction.cardHolderData);
        if (status != ACCOUNT_OK) {
            printf("Error: Account Blocked.\n");
            continue;
        }

        // التحقق من الرصيد المتاح
        status = isAmountAvailable(&transaction.terminalData);
        if (status != ACCOUNT_OK) {
            printf("Error: Low Balance.\n");
            continue;
        }

        // معالجة المعاملة
        printf("Starting transaction for PAN: %s\n", transaction.cardHolderData.primaryAccountNumber);
        status = recieveTransactionData(&transaction);

        if (status == APPROVED) {
            // حفظ المعاملة
            if (saveTransaction(&transaction) != SERVER_OK) {
                printf("Error: Failed to Save Transaction.\n");
                continue;
            }

            // تحديث الرصيد
            if (updateAccountBalance(&transaction) != SERVER_OK) {
                printf("Error: Failed to Update Balance.\n");
                continue;
            }

            printf("Transaction Approved - Seq Number: %d\n", transaction.transactionSequenceNumber);
        } else {
            printf("Transaction Failed: Seq Number: %d, Error Code: %d\n", transaction.transactionSequenceNumber, status);
        }
    }

    printf("Application finished.\n");
}
