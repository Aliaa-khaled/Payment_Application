#include "terminal.h"
EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{
    printf("Enter the transaction date (DD/MM/YYYY): ");
    scanf("%10s", termData->transactionDate);

    uint8 len = strlen((char*)termData->transactionDate);

    if (len != 10)
    {
        return WRONG_DATE;
    }

    for (uint8 i = 0; i < 10; i++)
    {
        if (i == 2 || i==5) {
            if (termData->transactionDate[i] != '/')
            {

                return WRONG_DATE;
            }
        }
        else
        {
            if (!isdigit(termData->transactionDate[i]))
            {
                return WRONG_DATE;
            }
        }
    }

    return TERMINAL_OK;

}

EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData)
{
    uint8 cardMonth  = ((cardData->cardExpirationDate[0] - '0') * 10) + (cardData->cardExpirationDate[1] - '0');
    uint8 cardYear  = ((cardData->cardExpirationDate[3] - '0') * 10) + (cardData->cardExpirationDate[4] - '0');

    uint8 transMonth  = ((termData->transactionDate[3] - '0') * 10) + (termData->transactionDate[4] - '0');
    uint8 transYear  = ((termData->transactionDate[6] - '0') * 10) + (termData->transactionDate[7] - '0');
    if (cardYear < transYear)
    {
        return EXPIRED_CARD;
    }
    else if (cardYear == transYear)
    {
        if (cardMonth < transMonth)
        {
            return EXPIRED_CARD;
        }
    }
    return TERMINAL_OK;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData)
{
    printf("Enter the transaction amount: ");
    scanf("%f", &(termData->transAmount));

    if (termData->transAmount <= 0)
    {
        return INVALID_AMOUNT;
    }
    return TERMINAL_OK;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData)
{
    if (termData->transAmount > termData->maxTransAmount) {
        return EXCEED_MAX_AMOUNT;
    }
    return TERMINAL_OK;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float maxAmount)
{
    if (maxAmount <= 0) {
        return INVALID_MAX_AMOUNT;
    }
    termData->maxTransAmount = maxAmount;
    return TERMINAL_OK;

}

EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData)
{

    uint8 len = strlen((char*)cardData->primaryAccountNumber);
    int sum = 0;
    int isSecond = 0;

    for (int i = len - 1; i >= 0; i--) {
        int digit = cardData->primaryAccountNumber[i] - '0';

        if (isSecond) {
            digit = digit * 2;
        }

        sum += digit / 10;
        sum += digit % 10;

        isSecond = !isSecond;
    }

    if (sum % 10 == 0) {
        return TERMINAL_OK;
    } else {
        return INVALID_CARD;
    }
}
