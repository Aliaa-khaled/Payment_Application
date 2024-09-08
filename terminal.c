#include "terminal.h"
EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{
    uint8 Transaction_Date[12];
    printf("Enter the transaction date (DD/MM/YYYY): ");
    fflush(stdin); fflush(stdout);
    gets(Transaction_Date);

    uint8 len = strlen((char*)Transaction_Date);

    if (len != 10)
    {
        return WRONG_DATE;
    }

    for (uint8 i = 0; i < 10; i++)
    {
        if (i == 2 || i==5) {
            if (Transaction_Date[i] != '/')
            {

                return WRONG_DATE;
            }
        }
        else
        {
            if (!isdigit(Transaction_Date[i]))
            {
                return WRONG_DATE;
            }
        }
    }

    uint8 day   = ((Transaction_Date[0] - '0') * 10) + (Transaction_Date[1] - '0');
    uint8 month = ((Transaction_Date[3] - '0') * 10) + (Transaction_Date[4] - '0');
    uint16 year = ((Transaction_Date[6] - '0') * 1000) + ((Transaction_Date[7] - '0') * 100) +
                  ((Transaction_Date[8] - '0') * 10) + (Transaction_Date[9] - '0');

    // Check if the month is valid (1-12)
    if (month < 1 || month > 12)
    {
        return WRONG_DATE;
    }

    // Check if the day is valid (1-31)
    if (day < 1 || day > 31)
    {
        return WRONG_DATE;
    }

    strcpy(termData->transactionDate, (char*)Transaction_Date);
    return TERMINAL_OK;

}
EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData)
{
    uint8 cardMonth  = ((cardData->cardExpirationDate[0] - '0') * 10) + (cardData->cardExpirationDate[1] - '0');

    uint32 cardYear   = ((cardData->cardExpirationDate[3] - '0') * 10) + (cardData->cardExpirationDate[4] - '0');
    cardYear += 2000;

    uint8 transMonth = ((termData->transactionDate[3] - '0') * 10) + (termData->transactionDate[4] - '0');
    uint16 transYear = ((termData->transactionDate[6] - '0') * 1000) +
                        ((termData->transactionDate[7] - '0') * 100) +
                        ((termData->transactionDate[8] - '0') * 10) +
                        (termData->transactionDate[9] - '0');
    // تصحيح التحويلات والتأكد من أن السنة الشهر يتم حسابهما بشكل صحيح
    printf("cardYear=%d\n", cardYear);
    printf("cardMonth=%d\n", cardMonth);
    printf("transMonth=%d\n", transMonth);
    printf("transYear=%d\n", transYear);

    // تحقق من تاريخ انتهاء البطاقة ضد تاريخ المعاملة
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
    uint8 len = strlen(cardData->primaryAccountNumber);
    int sum = 0;
    int isSecond = 0;

    // Check if length is correct (typically 16 for most cards)
    if (len < 13 || len > 19) {
        printf("Invalid length: %d\n", len);
        return INVALID_CARD;
    }

    for (int i = len - 1; i >= 0; i--) {
        int digit = cardData->primaryAccountNumber[i] - '0';

        // Skip if the character is not a digit
        if (digit < 0 || digit > 9) {
            printf("Non-digit character found: %c\n", cardData->primaryAccountNumber[i]);
            return INVALID_CARD;
        }

        if (isSecond) {
            digit = digit * 2;
            // Add digits of the product (e.g., 18 becomes 1 + 8 = 9)
            sum += digit / 10;
            sum += digit % 10;
        } else {
            sum += digit;
        }

        isSecond = !isSecond;
    }

    printf("Sum: %d\n", sum);

    if (sum % 10 == 0) {
        return TERMINAL_OK;
    } else {
        return INVALID_CARD;
    }
}
