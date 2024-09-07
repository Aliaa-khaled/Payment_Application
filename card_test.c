#include "card.h"

#define TEST_CASES_NUM 4



ST_cardData_t cardData;


void getCardHolderNameTest(void)
{
    printf("\nTester Name: Abdelrhman Mohamed\n");
    printf("Function Name: getCardHolderName()\n");
    for (int i = 1; i <= TEST_CASES_NUM; i++)
    {
        printf("Test Case %d:\n", i);
        EN_cardError_t error = getCardHolderName(&cardData);
        switch (i)
        {
        case 1:
            printf("Input Data: NULL\n");
            printf("Expected Result: %d\n", WRONG_NAME);
            printf("Actual Result: %d\n", error);
            break;
        case 2:
            printf("Input Data: Abdelrhman Mohamed\n");
            printf("Expected Result: %d\n", WRONG_NAME);
            printf("Actual Result: %d\n", error);
            break;
        case 3:
            printf("Input Data: Abdelrhman Mohamed Mohamed Ellawendi\n");
            printf("Expected Result: %d\n", WRONG_NAME);
            printf("Actual Result: %d\n", error);
            break;
        case 4:
            printf("Input Data: Abdelrhman Abdelrhmann \n");
            printf("Expected Result: %d\n", CARD_OK);
            printf("Actual Result: %d\n", error);
            break;
        }
    }
}

void getCardExpiryDateTest(void)
{
    printf("\nTester Name: Abdelrhman Mohamed\n");
    printf("Function Name: getCardExpiryDate()\n");
    for (int i = 1; i <= TEST_CASES_NUM; i++)
    {
        printf("Test Case %d:\n", i);
        EN_cardError_t error = getCardExpiryDate(&cardData);
        switch (i)
        {
        case 1:
            printf("Input Data: NULL\n");
            printf("Expected Result: %d\n", WRONG_EXP_DATE);
            printf("Actual Result: %d\n", error);
            break;
        case 2:
            printf("Input Data: 07-3\n");
            printf("Expected Result: %d\n", WRONG_EXP_DATE);
            printf("Actual Result: %d\n", error);
            break;
        case 3:
            printf("Input Data: 28/11/2023\n");
            printf("Expected Result: %d\n", WRONG_EXP_DATE);
            printf("Actual Result: %d\n", error);
            break;
        case 4:
            printf("Input Data: 07/23\n");
            printf("Expected Result: %d\n", CARD_OK);
            printf("Actual Result: %d\n", error);
            break;
        }
    }
}

void getCardPANTest(void)
{
    printf("\nTester Name: Abdelrhman Mohamed\n");
    printf("Function Name: getCardPAN()\n");
    for (int i = 1; i <= TEST_CASES_NUM; i++)
    {
        printf("Test Case %d:\n", i);
        EN_cardError_t error = getCardPAN(&cardData);
        switch (i)
        {
        case 1:
            printf("Input Data: NULL\n");
            printf("Expected Result: %d\n", WRONG_PAN);
            printf("Actual Result: %d\n", error);
            break;
        case 2:
            printf("Input Data: 123856956218\n");
            printf("Expected Result: %d\n", WRONG_PAN);
            printf("Actual Result: %d\n", error);
            break;
        case 3:
            printf("Input Data: 3216549846164946411964458\n");
            printf("Expected Result: %d\n", WRONG_PAN);
            printf("Actual Result: %d\n", error);
            break;
        case 4:
            printf("Input Data: 18569475321456987\n");
            printf("Expected Result: %d\n", CARD_OK);
            printf("Actual Result: %d\n", error);
            break;
        }
    }
}
