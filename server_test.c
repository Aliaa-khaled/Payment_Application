#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "file_handling.h"
#include "queue.h"
extern Queue transactionQueue;

extern AccountNode* accountsHead;
// تُعتبر هذه الدالة مهمة وتستدعي ملفات البيانات وتختبر كل الحالات الممكنة


void isValidAccountTest(void) {
    ST_cardData_t cardData;
    ST_accountsDB_t account;
    EN_serverError_t result;

    // تأكد من أن الحسابات تم تحميلها مسبقًا إلى القائمة المرتبطة
    if (accountsHead == NULL) {
        printf("Error: No accounts loaded. Please load accounts before running tests.\n");
        return;
    }

    // Test Case 1: حساب موجود
    strcpy(cardData.primaryAccountNumber, "1234567812345678");  // رقم حساب موجود (افتراضًا)
    result = isValidAccount(&cardData, &account);
    printf("Test Case 1: Expected: SERVER_OK, Actual: %d\n", result);  // SERVER_OK متوقع

    // Test Case 2: حساب غير موجود
    strcpy(cardData.primaryAccountNumber, "0000000000000000");  // رقم حساب غير موجود
    result = isValidAccount(&cardData, &account);
    printf("Test Case 2: Expected: ACCOUNT_NOT_FOUND, Actual: %d\n", result);  // ACCOUNT_NOT_FOUND متوقع
}
void isBlockedAccountTest(void) {
    ST_accountsDB_t account;
    EN_serverError_t result;

    // Test Case 1: حساب غير محظور
    account.state = RUNNING;
    result = isBlockedAccount(&account);
    printf("Test Case 1: Account State: %d, Expected: SERVER_OK, Actual: %d\n", account.state, result);

    // Test Case 2: حساب محظور
    account.state = BLOCKED;
    result = isBlockedAccount(&account);
    printf("Test Case 2: Account State: %d, Expected: BLOCKED_ACCOUNT, Actual: %d\n", account.state, result);
}
void isAmountAvailableTest(void) {
    ST_terminalData_t termData;
    ST_accountsDB_t account;
    EN_serverError_t result;

    // Test Case 1: رصيد كافٍ
    termData.transAmount = 50.0;
    account.balance = 100.0;
    result = isAmountAvailable(&termData, &account);
    printf("Test Case 1: Transaction Amount: %.2f, Account Balance: %.2f, Expected: SERVER_OK, Actual: %d\n", termData.transAmount, account.balance, result);

    // Test Case 2: رصيد غير كافٍ
    termData.transAmount = 150.0;
    account.balance = 100.0;
    result = isAmountAvailable(&termData, &account);
    printf("Test Case 2: Transaction Amount: %.2f, Account Balance: %.2f, Expected: LOW_BALANCE, Actual: %d\n", termData.transAmount, account.balance, result);
}
void saveTransactionTest(void) {
    // تعريف المعاملات لاختبار حفظها
    ST_transaction_t transaction1 = {
        .transactionSequenceNumber = 1,
        .terminalData.transAmount = 100.0,
        .terminalData.maxTransAmount = 500.0,
        .terminalData.transactionDate = "2024/09/08",
        .cardHolderData.cardHolderName = "aliaa khaled",
        .cardHolderData.primaryAccountNumber = "4502015112330366",
        .cardHolderData.cardExpirationDate = "12/25",
        .transState = APPROVED
    };

    ST_transaction_t transaction2 = {
        .transactionSequenceNumber = 2,
        .terminalData.transAmount = 100.0,
        .terminalData.maxTransAmount = 500.0,
        .terminalData.transactionDate = "2024/09/08",
        .cardHolderData.cardHolderName = "ahmed ahmed",
        .cardHolderData.primaryAccountNumber = "4532017512830366",
        .cardHolderData.cardExpirationDate = "12/25",
        .transState = APPROVED
    };

    // اختبار حفظ المعاملة الأولى
    printf("Test Case 1: Saving Transaction 1:\n");
    EN_serverError_t result1 = saveTransaction(&transaction1);
    if (result1 == SERVER_OK) {
        printf("Transaction 1 saved successfully.\n");
    } else {
        printf("Failed to save Transaction 1. Error code: %d\n", result1);
    }

    // اختبار حفظ المعاملة الثانية
    printf("Test Case 2: Saving Transaction 2:\n");
    EN_serverError_t result2 = saveTransaction(&transaction2);
    if (result2 == SERVER_OK) {
        printf("Transaction 2 saved successfully.\n");
    } else {
        printf("Failed to save Transaction 2. Error code: %d\n", result2);
    }



}
void testRecieveTransactionData() {
    ST_transaction_t transaction;
    EN_transStat_t transStatus;

    // Example test: processing a transaction
    // Ensure to replace these values with the corresponding test cases

    // Test Case: Valid transaction
    strcpy(transaction.cardHolderData.primaryAccountNumber, "1234567812345678"); // Ensure this account exists in your loaded accounts
    strcpy(transaction.cardHolderData.cardHolderName, "Mohamed Ahmed");
    strcpy(transaction.cardHolderData.cardExpirationDate, "12/25");
    strcpy(transaction.terminalData.transactionDate, "2024/09/08");
    transaction.terminalData.transAmount = 200.00;
    transaction.transactionSequenceNumber = 1;

    // Process the transaction
    transStatus = recieveTransactionData(&transaction);
    if (transStatus == APPROVED) {
        printf("Transaction Approved - Passed\n");
    } else {
        printf("Transaction Failed - Status: %d\n", transStatus);  // Print failure status for debugging
    }

    // Repeat with other cases like blocked accounts, insufficient funds, etc.
}

void printAccounts() {
    AccountNode* current = accountsHead;

    if (current == NULL) {
        printf("No accounts to display. The list is empty.\n");
        return;
    }

    printf("Accounts loaded from file:\n");

    while (current != NULL) {
        printf("Account Number: %s\n", current->accountData.primaryAccountNumber);
        // إذا كانت الحالة عبارة عن عدد صحيح، فاجعلها أكثر وضوحًا بإظهار القيم العددية
        printf("Account State: %s\n", (current->accountData.state == RUNNING) ? "RUNNING" : "BLOCKED");
        printf("Balance: %.2f\n", current->accountData.balance);
        printf("\n");
        current = current->next;
    }
}
