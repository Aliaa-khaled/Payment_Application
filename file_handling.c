#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_handling.h"
 // تأكد من تضمين server.h للحصول على تعريفات الهيكلية والموجهات

// تنفيذ الدالة لتحميل الحسابات من ملف
AccountNode* accountsHead;
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
        AccountNode* newNode = (AccountNode*)malloc(sizeof(AccountNode));
        if (newNode == NULL) {
            perror("Failed to allocate memory for new node");
            fclose(file);
            return;
        }
        newNode->accountData = account;
        newNode->next = accountsHead;
        accountsHead = newNode;
    }

    fclose(file);
}
