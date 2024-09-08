#include <stdio.h>
#include <stdlib.h>

#include "server.h"
#include "file_handling.h"
#include "queue.h"
extern Queue transactionQueue;
extern AccountNode* accountsHead;

EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountReference) {
    if (cardData == NULL || accountReference == NULL) {
        return INTERNAL_SERVER_ERROR;
    }

    AccountNode* current = accountsHead;

    while (current != NULL) {
        if (strcmp(current->accountData.primaryAccountNumber, cardData->primaryAccountNumber) == 0) {
            *accountReference = current->accountData;
            return SERVER_OK;
        }
        current = current->next;
    }

    return ACCOUNT_NOT_FOUND;
}


EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountReference)
{
    if (accountReference == NULL)
    {
        return INTERNAL_SERVER_ERROR;
    }

    if (accountReference->state == BLOCKED)
    {
        return BLOCKED_ACCOUNT;
    }

    return SERVER_OK;
}


EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, ST_accountsDB_t* accountReference)
{
    if (termData == NULL || accountReference == NULL)
    {
        return INTERNAL_SERVER_ERROR;
    }

    if (termData->transAmount > accountReference->balance)
    {
        return LOW_BALANCE;
    }

    return SERVER_OK;
}




EN_serverError_t saveTransaction(ST_transaction_t* transaction)
{
    enqueue(&transactionQueue, *transaction);
    saveTransactionsToFile("transactions.txt");
    return SERVER_OK;
}


void listSavedTransactions(void) {
    if (isQueueEmpty(&transactionQueue)) {
        printf("The transaction queue is empty.\n");
        return;
    }

    Node* current = transactionQueue.front; // بداية الطابور

    printf("Listing Saved Transactions:\n");

    while (current != NULL) {
        ST_transaction_t transaction = current->transaction;

        printf("#########################\n");
        printf("Transaction Sequence Number: %d\n", transaction.transactionSequenceNumber);
        printf("Transaction Date: %s\n", transaction.terminalData.transactionDate);
        printf("Transaction Amount: %.2f\n", transaction.terminalData.transAmount);
        printf("Transaction State: %d\n", transaction.transState);
        printf("Terminal Max Amount: %.2f\n", transaction.terminalData.maxTransAmount);
        printf("Cardholder Name: %s\n", transaction.cardHolderData.cardHolderName);
        printf("PAN: %s\n", transaction.cardHolderData.primaryAccountNumber);
        printf("Card Expiration Date: %s\n", transaction.cardHolderData.cardExpirationDate);
        printf("#########################\n");

        current = current->next; // الانتقال إلى العنصر التالي في الطابور
    }
}


EN_transStat_t recieveTransactionData(ST_transaction_t *transData) {
    ST_accountsDB_t account;
    EN_serverError_t serverError;

    printf("Starting transaction for PAN: %s\n", transData->cardHolderData.primaryAccountNumber);

    // 1. Check account validity (using the PAN from the transaction)
    serverError = isValidAccount(&transData->cardHolderData.primaryAccountNumber, &account);
    if (serverError != SERVER_OK) {
        printf("Account not valid. Error: %d\n", serverError);
        return FRAUD_CARD;  // If account does not exist
    }

    // 2. Check if the account is blocked
    serverError = isBlockedAccount(&account);
    if (serverError != SERVER_OK) {
        printf("Account is blocked. Error: %d\n", serverError);
        return DECLINED_STOLEN_CARD;  // If the account is blocked
    }

    // 3. Check if the amount is available
    serverError = isAmountAvailable(&transData->terminalData, &account);
    if (serverError != SERVER_OK) {
        printf("Insufficient funds. Error: %d\n", serverError);
        return DECLINED_INSUFFECIENT_FUND;  // If insufficient funds
    }

    // 4. Update the account balance
    printf("Current balance: %.2f\n", account.balance);
    account.balance -= transData->terminalData.transAmount;
    printf("New balance: %.2f\n", account.balance);

    // Save the updated account information (if applicable)
    // updateAccount(&account);

    // 5. Save the transaction
    serverError = saveTransaction(transData);
    if (serverError != SERVER_OK) {
        printf("Failed to save transaction. Error: %d\n", serverError);
        return INTERNAL_SERVER_ERROR;  // If saving fails
    }

    return APPROVED;  // If all checks passed and the transaction was saved
}




