#include "terminal.h"

#define TEST_CASES_NUM 4

ST_terminalData_t terminalData;
ST_cardData_t cardData;

void getTransactionDateTest(void)
{
    printf("\nTester Name: Abdelrhman Mohamed\n");
    printf("Function Name: getTransactionDate()\n");
    for (int i = 1; i <= TEST_CASES_NUM; i++)
    {
        printf("Test Case %d:\n", i);
        EN_terminalError_t error = getTransactionDate(&terminalData);
        switch (i)
        {
        case 1:
            printf("Input Data: NULL\n");
            printf("Expected Result: %d\n", WRONG_DATE);
            printf("Actual Result: %d\n", error);
            break;
        case 2:
            printf("Input Data: 32/13/2023\n");
            printf("Expected Result: %d\n", WRONG_DATE);
            printf("Actual Result: %d\n", error);
            break;
        case 3:
            printf("Input Data: 12-07-2023\n");
            printf("Expected Result: %d\n", WRONG_DATE);
            printf("Actual Result: %d\n", error);
            break;
        case 4:
            printf("Input Data: 12/07/2023\n");
            printf("Expected Result: %d\n", TERMINAL_OK);
            printf("Actual Result: %d\n", error);
            break;
        }
    }
}

void isCardExpiredTest(void)
{
    printf("\nTester Name: Abdelrhman Mohamed\n");
    printf("Function Name: isCardExpired()\n");

    for (int i = 1; i <= TEST_CASES_NUM; i++)
    {
        printf("Test Case %d:\n", i);

        switch (i)
        {
        case 1:
            strcpy(cardData.cardExpirationDate, "01/20"); // تاريخ انتهاء البطاقة
            strcpy(terminalData.transactionDate, "05/01/2023"); // تاريخ المعاملة
            printf("cardData.cardExpirationDate=%s\n", cardData.cardExpirationDate);
            printf("terminalData.transactionDate=%s\n", terminalData.transactionDate);
            printf("Input Data: Expiry: 01/20, Transaction: 01/2023\n");
            printf("Expected Result: %d\n", EXPIRED_CARD);
            break;

        case 2:
            strcpy(cardData.cardExpirationDate, "01/23");
            strcpy(terminalData.transactionDate, "12/01/2023");
            printf("cardData.cardExpirationDate=%s\n", cardData.cardExpirationDate);
            printf("terminalData.transactionDate=%s\n", terminalData.transactionDate);
            printf("Input Data: Expiry: 01/23, Transaction: 01/2023\n");
            printf("Expected Result: %d\n", TERMINAL_OK);
            break;

        case 3:
            strcpy(cardData.cardExpirationDate, "01/24");
            strcpy(terminalData.transactionDate, "11/01/2023");
            printf("cardData.cardExpirationDate=%s\n", cardData.cardExpirationDate);
            printf("terminalData.transactionDate=%s\n", terminalData.transactionDate);
            printf("Input Data: Expiry: 01/24, Transaction: 01/2023\n");
            printf("Expected Result: %d\n", TERMINAL_OK);
            break;

        case 4:
            strcpy(cardData.cardExpirationDate, "01/23");
            strcpy(terminalData.transactionDate, "20/02/2023");
            printf("cardData.cardExpirationDate=%s\n", cardData.cardExpirationDate);
            printf("terminalData.transactionDate=%s\n", terminalData.transactionDate);
            printf("Input Data: Expiry: 01/23, Transaction: 02/2023\n");
            printf("Expected Result: %d\n", EXPIRED_CARD);
            break;
        }

        EN_terminalError_t error = isCardExpired(&cardData, &terminalData);
        printf("Actual Result: %d\n", error);
    }
}



void getTransactionAmountTest(void)
{
    printf("\nTester Name: Abdelrhman Mohamed\n");
    printf("Function Name: getTransactionAmount()\n");
    for (int i = 1; i <= TEST_CASES_NUM; i++)
    {
        printf("Test Case %d:\n", i);
        EN_terminalError_t error = getTransactionAmount(&terminalData);
        switch (i)
        {
        case 1:
            printf("Input Data: -100.0\n");
            printf("Expected Result: %d\n", INVALID_AMOUNT);
            printf("Actual Result: %d\n", error);
            break;
        case 2:
            printf("Input Data: 0.0\n");
            printf("Expected Result: %d\n", INVALID_AMOUNT);
            printf("Actual Result: %d\n", error);
            break;
        case 3:
            printf("Input Data: 500.0\n");
            printf("Expected Result: %d\n", TERMINAL_OK);
            printf("Actual Result: %d\n", error);
            break;
        case 4:
            printf("Input Data: 9999.99\n");
            printf("Expected Result: %d\n", TERMINAL_OK);
            printf("Actual Result: %d\n", error);
            break;
        }
    }
}

void isBelowMaxAmountTest(void)
{
    printf("\nTester Name: Abdelrhman Mohamed\n");
    printf("Function Name: isBelowMaxAmount()\n");

    // تعيين القيمة العليا للمعاملة
    terminalData.maxTransAmount = 1000.0;

    for (int i = 1; i <= 3; i++)
    {
        printf("Test Case %d:\n", i);

        // تعيين قيمة المعاملة وفقاً لكل حالة اختبار
        switch (i)
        {
        case 1:
            terminalData.transAmount = 500.0;
            printf("Input Data: 500.0\n");
            printf("Expected Result: %d\n", TERMINAL_OK);
            break;
        case 2:
            terminalData.transAmount = 1000.0;
            printf("Input Data: 1000.0\n");
            printf("Expected Result: %d\n", TERMINAL_OK);
            break;
        case 3:
            terminalData.transAmount = 1500.0;
            printf("Input Data: 1500.0\n");
            printf("Expected Result: %d\n", EXCEED_MAX_AMOUNT);
            break;
        }

        // استدعاء الدالة لاختبار النتائج
        EN_terminalError_t error = isBelowMaxAmount(&terminalData);
        printf("Actual Result: %d\n", error);
    }
}
void isValidCardPANTest(void)
{
    printf("\nTester Name: Abdelrhman Mohamed\n");
    printf("Function Name: isValidCardPAN()\n");

    for (int i = 1; i <= TEST_CASES_NUM; i++)
    {
        printf("Test Case %d:\n", i);
        EN_terminalError_t error;

        switch (i)
        {
        case 1:
            strcpy(cardData.primaryAccountNumber, "4532015112830366"); // Valid PAN
            break;

        case 2:
            strcpy(cardData.primaryAccountNumber, "4532015112830367"); // Invalid PAN
            break;

        case 3:
            strcpy(cardData.primaryAccountNumber, "1234567812345670"); // Invalid PAN
            break;

        case 4:
            strcpy(cardData.primaryAccountNumber, "1234567812345678"); // Valid PAN
            break;
        }

        error = isValidCardPAN(&cardData);
        printf("Input Data: %s\n", cardData.primaryAccountNumber);
        printf("Expected Result: %d\n", (i == 1 || i == 4) ? TERMINAL_OK : INVALID_CARD);
        printf("Actual Result: %d\n", error);
    }
}

