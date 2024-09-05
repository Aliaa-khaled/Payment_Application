#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
    printf("Please enter the cardholder's name: ");
    fgets((char*)cardData->cardHolderName, sizeof(cardData->cardHolderName), stdin);

    uint8 len = strlen((char*)cardData->cardHolderName);
    printf("\n%d",len);

    if (len > 0 &&cardData->cardHolderName[len - 1] == '\n')
    {
        cardData->cardHolderName[len - 1] = '\0';
        len--;
    }

    if (len < 20 || len > 24)
    {
        return WRONG_NAME;
    }

    for (uint8 i = 0; i < len; i++)
    {
        if (!isalpha(cardData->cardHolderName[i]))
        {
            return WRONG_NAME;
        }
    }

    return CARD_OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
    printf("Please enter the cardholder's Expiry Date (MM/YY): ");
    scanf("%s",cardData->cardExpirationDate);

    size_t len = strlen((char*)cardData->cardExpirationDate);

    if (len != 5)
    {
        return WRONG_EXP_DATE;

    }

    for (uint8 i = 0; i < 5; i++)
    {

        if (i == 2) {
            if (cardData->cardExpirationDate[i] != '/')
            {

                return WRONG_EXP_DATE;
            }
        }
        else
        {
            if (!isdigit(cardData->cardExpirationDate[i]))
            {
                return WRONG_EXP_DATE;
            }
        }
    }

    uint8 cardMonth  = ((cardData->cardExpirationDate[0] - '0') * 10) + (cardData->cardExpirationDate[1] - '0');
    uint8 cardYear  = ((cardData->cardExpirationDate[3] - '0') * 10) + (cardData->cardExpirationDate[4] - '0');

    if (cardMonth  < 1 || cardMonth  > 12 || cardYear  > 99)
    {
        return WRONG_EXP_DATE;
    }

    return CARD_OK;
}

EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{
    printf("Please enter the card's Primary Account Number (PAN): ");
    scanf("%s",cardData->primaryAccountNumber);

    uint8 len = strlen((char*)cardData->primaryAccountNumber);

    if (len < 16 || len > 19)
    {
        return WRONG_PAN;
    }

    for (uint8 i = 0; i < len; i++)
    {
        if (!isdigit(cardData->primaryAccountNumber[i]))
        {
            return WRONG_PAN;
        }
    }
    return CARD_OK;
}
void getCardHolderNameTest()
{
    ST_cardData_t cardData;
    EN_cardError_t result;

    // اختبار حالات مختلفة
    // حالة صحيحة
    strcpy((char*)cardData.cardHolderName, "Alexandra Maria Johnson"); // 24 حرف
    result = getCardHolderName(&cardData);
    printf("Test Case 1 - Expected: CARD_OK, Result: %d\n", result);

    // حالة خاطئة - طول أقل من 20
    strcpy((char*)cardData.cardHolderName, "J Doe");
    result = getCardHolderName(&cardData);
    printf("Test Case 2 - Expected: WRONG_NAME, Result: %d\n", result);

    // حالة خاطئة - طول أكثر من 24
    strcpy((char*)cardData.cardHolderName, "Johnathan Alexander Doe the Second");
    result = getCardHolderName(&cardData);
    printf("Test Case 3 - Expected: WRONG_NAME, Result: %d\n", result);

    // حالة خاطئة - يحتوي على أرقام
    strcpy((char*)cardData.cardHolderName, "John123 Doe");
    result = getCardHolderName(&cardData);
    printf("Test Case 4 - Expected: WRONG_NAME, Result: %d\n", result);

    // حالة خاطئة - يحتوي على علامات ترقيم
    strcpy((char*)cardData.cardHolderName, "John, Doe");
    result = getCardHolderName(&cardData);
    printf("Test Case 5 - Expected: WRONG_NAME, Result: %d\n", result);

    // حالة صحيحة - طول 20
    strcpy((char*)cardData.cardHolderName, "Alexander the Great");
    result = getCardHolderName(&cardData);
    printf("Test Case 6 - Expected: CARD_OK, Result: %d\n", result);
}
