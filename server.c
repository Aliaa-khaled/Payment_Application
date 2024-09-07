#include <stdio.h>
#include <stdlib.h>

#include "server.h"
#include "file_handling.h"
//#include "queue.h"



EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t *accountReference) {
    // تحقق من أن بيانات البطاقة ومرجع الحساب ليسا فارغين
    if (cardData == NULL || accountReference == NULL) {
        return INTERNAL_SERVER_ERROR; // خطأ داخلي في الخادم
    }

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
    return ACCOUNT_NOT_FOUND;
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountReference) {
    // تحقق من أن مرجع الحساب ليس فارغًا
    if (accountReference == NULL) {
        return INTERNAL_SERVER_ERROR; // خطأ داخلي في الخادم
    }

    // تحقق مما إذا كان الحساب محظورًا
    if (accountReference->state == BLOCKED) {
        return BLOCKED_ACCOUNT; // الحساب محظور
    }

    // الحساب ليس محظورًا
    return SERVER_OK;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, ST_accountsDB_t* accountReference) {
    // تحقق من أن بيانات المحطة ومرجع الحساب ليسا فارغين
    if (termData == NULL || accountReference == NULL) {
        return INTERNAL_SERVER_ERROR; // خطأ داخلي في الخادم
    }

    // تحقق مما إذا كان مبلغ المعاملة يتجاوز رصيد الحساب
    if (termData->transAmount > accountReference->balance) {
        return LOW_BALANCE; // رصيد غير كافٍ
    }

    // إذا كان مبلغ المعاملة ضمن الرصيد، ارجع بنجاح
    return SERVER_OK;
}













