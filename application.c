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
AccountNode* findAccount(ST_cardData_t* cardData);

void appStart(void) {
    ST_transaction_t transaction;
    EN_transStat_t status;
    EN_terminalError_t terminalError;
    EN_cardError_t cardError;
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

        int validInput = 0;
        AccountNode* account = NULL;

        // الحصول على اسم صاحب البطاقة
        while (1) {
            cardError = getCardHolderName(&transaction.cardHolderData);
            if (cardError == CARD_OK) break;
            printf("Error: Invalid Card Holder Name. Please enter again.\n");
        }

        // الحصول على تاريخ انتهاء صلاحية البطاقة
        while (1) {
            cardError = getCardExpiryDate(&transaction.cardHolderData);
            if (cardError == CARD_OK) break;
            printf("Error: Invalid Card Expiry Date. Please enter again.\n");
        }

        // الحصول على رقم البطاقة (PAN)
        while (1) {
            cardError = getCardPAN(&transaction.cardHolderData);
            if (cardError == CARD_OK) break;
            printf("Error: Invalid PAN. Please enter again.\n");
        }

        // التحقق من تاريخ المعاملة
        while (1) {
            terminalError = getTransactionDate(&transaction.terminalData);
            if (terminalError == TERMINAL_OK) break;
            printf("Error: Invalid Transaction Date. Please enter again.\n");
        }

        // التحقق إذا كانت البطاقة منتهية الصلاحية
        while (1) {
            terminalError = isCardExpired(&transaction.cardHolderData, &transaction.terminalData);
            if (terminalError == TERMINAL_OK) break;
            printf("Error: Card Expired. Please enter again.\n");
            return;
        }

        // الحصول على قيمة المعاملة
        while (1) {
            terminalError = getTransactionAmount(&transaction.terminalData);
            if (terminalError == TERMINAL_OK) break;
            printf("Error: Invalid Transaction Amount. Please enter again.\n");
        }

        // ضبط الحد الأقصى للمبلغ
        while (1) {
            terminalError = setMaxAmount(&transaction.terminalData, maxAmount);
            if (terminalError == TERMINAL_OK) break;
            printf("Error: Unable to set max amount. Please enter again.\n");
        }

        // التحقق إذا كان المبلغ أقل من الحد الأقصى
        while (1) {
            terminalError = isBelowMaxAmount(&transaction.terminalData);
            if (terminalError == TERMINAL_OK) break;
            printf("Error: Transaction Amount Exceeds Maximum Limit. Please enter again.\n");
            return;
        }

        // التحقق من صحة الحساب
        while (1) {
            account = findAccount(&transaction.cardHolderData);
            if (account != NULL) break;
            printf("Error: Account Not Found. Please re-enter the transaction.\n");
            return;  // Exit or handle according to your program's needs
        }

        // التحقق من صحة الحساب
        while (1) {
            status = isValidAccount(&transaction.cardHolderData, &account->accountData);
            if (status == SERVER_OK) break;
            printf("Error: Invalid Account. Please re-enter the transaction.\n");
            return;  // Exit or handle according to your program's needs
        }

        // التحقق من حالة الحساب
        while (1) {
            status = isBlockedAccount(&account->accountData);
            if (status == SERVER_OK) break;
            printf("Error: Account Blocked. Please re-enter the transaction.\n");
            return;  // Exit or handle according to your program's needs
        }

        // التحقق من الرصيد المتاح
        while (1) {
            status = isAmountAvailable(&transaction.terminalData, &account->accountData);
            if (status == SERVER_OK) break;
            printf("Error: Low Balance. Please re-enter the transaction.\n");
            return;  // Exit or handle according to your program's needs
        }

        // معالجة المعاملة
        printf("Starting transaction for PAN: %s\n", transaction.cardHolderData.primaryAccountNumber);
        status = recieveTransactionData(&transaction);

        if (status == APPROVED) {
            // حفظ المعاملة
            if (saveTransaction(&transaction) != SERVER_OK) {
                printf("Error: Failed to Save Transaction. Please try again.\n");
                continue;
            }
            if (updateAccount(&account) != SERVER_OK) {
                printf("Error: Failed to Update Account. Please try again.\n");
                continue;
            }

            printf("Transaction Approved - Seq Number: %d\n", transaction.transactionSequenceNumber);
        } else {
            printf("Transaction Failed: Seq Number: %d, Error Code: %d\n", transaction.transactionSequenceNumber, status);
        }
    }

    printf("Application finished.\n");
}

AccountNode* findAccount(ST_cardData_t* cardData) {
    AccountNode* current = accountsHead;  // ابدأ من رأس قائمة الحسابات
    while (current != NULL) {  // تابع البحث حتى تصل إلى نهاية القائمة
        if (strcmp((const char*)current->accountData.primaryAccountNumber,
                   (const char*)cardData->primaryAccountNumber) == 0) {
            return current;  // إذا تم العثور على الحساب، أعد المؤشر إليه
        }
        current = current->next;  // الانتقال إلى العقدة التالية في القائمة
    }
    return NULL;  // أعد NULL إذا لم يتم العثور على الحساب
}

