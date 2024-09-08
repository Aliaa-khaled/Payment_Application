#ifndef SERVER_H
#define SERVER_H

#include "terminal.h"
#include "card.h"

// تعريف حالات المعاملة
typedef enum EN_transState_t
{
    APPROVED,
    DECLINED_INSUFFECIENT_FUND,
    DECLINED_STOLEN_CARD,
    FRAUD_CARD,
    INTERNAL_SERVER_ERROR
}EN_transStat_t;

// هيكل المعاملة
typedef struct ST_transaction_t
{
    ST_cardData_t cardHolderData;
    ST_terminalData_t terminalData;
    EN_transStat_t transState;
    uint32 transactionSequenceNumber;
}ST_transaction_t;

// أخطاء السيرفر
typedef enum EN_serverError_t
{
    SERVER_OK,
    SAVING_FAILED,
    TRANSACTION_NOT_FOUND,
    ACCOUNT_NOT_FOUND,
    LOW_BALANCE,
    BLOCKED_ACCOUNT
}EN_serverError_t ;

// حالة الحساب
typedef enum EN_accountState_t
{
    RUNNING,
    BLOCKED
}EN_accountState_t;

// هيكل قاعدة بيانات الحسابات
typedef struct ST_accountsDB_t
{
    float balance;
    EN_accountState_t state;
    uint8 primaryAccountNumber[20];
}ST_accountsDB_t;

typedef struct AccountNode {
    ST_accountsDB_t accountData;
    struct AccountNode* next;
} AccountNode;



EN_transStat_t recieveTransactionData(ST_transaction_t *transData);
EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t*accountRefrence);
EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence);
EN_serverError_t isAmountAvailable(ST_terminalData_t *termData,ST_accountsDB_t* accountRefrence);
EN_serverError_t saveTransaction(ST_transaction_t *transData);
void listSavedTransactions(void);

#endif // SERVER_H
