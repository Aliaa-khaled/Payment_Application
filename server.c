#include <stdio.h>
#include <stdlib.h>

#include "server.h"
#include "file_handling.h"
//#include "queue.h"



EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t *accountReference) {
    AccountNode* current = accountsHead;  // بدء البحث من رأس القائمة

    // البحث عن الحساب في القائمة المرتبطة
    while (current != NULL) {
        if (strcmp(current->accountData.primaryAccountNumber, cardData->primaryAccountNumber) == 0) {
            // الحساب موجود، إعادة مرجع الحساب
            *accountReference = current->accountData;
            return SERVER_OK;
        }
        current = current->next;  // الانتقال إلى العقدة التالية
    }

    // إذا لم يتم العثور على الحساب
    accountReference = NULL;
    return ACCOUNT_NOT_FOUND;
}

void isValidAccountTest(void) {
    ST_cardData_t testCard;
    ST_accountsDB_t* account;
    EN_serverError_t result;

    printf("Tester Name: Your Name\n");
    printf("Function Name: isValidAccount\n");

    // اختبار الحالة الأولى: PAN صالح موجود في قاعدة البيانات
    printf("Test Case 1:\n");
    printf("Input Data: Valid PAN\n");
    strcpy(testCard.primaryAccountNumber, "1234567890123456");  // PAN صالح
    result = isValidAccount(&testCard, &account);
    printf("Expected Result: SERVER_OK\n");
    printf("Actual Result: %s\n", (result == SERVER_OK) ? "SERVER_OK" : "ACCOUNT_NOT_FOUND");

    // اختبار الحالة الثانية: PAN غير موجود في قاعدة البيانات
    printf("Test Case 2:\n");
    printf("Input Data: Invalid PAN\n");
    strcpy(testCard.primaryAccountNumber, "1111222233334444");  // PAN غير صالح
    result = isValidAccount(&testCard, &account);
    printf("Expected Result: ACCOUNT_NOT_FOUND\n");
    printf("Actual Result: %s\n", (result == ACCOUNT_NOT_FOUND) ? "ACCOUNT_NOT_FOUND" : "SERVER_OK");

    // يمكنك إضافة المزيد من الحالات إذا كان هناك حالات إضافية تحتاج للاختبار.
}












