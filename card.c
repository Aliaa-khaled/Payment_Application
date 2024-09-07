#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
    uint8 card_Holder_Name[30];
    printf("Please enter the cardholder's name: ");
    fflush(stdin); fflush(stdout);
    gets(card_Holder_Name);

    uint8 len = strlen(card_Holder_Name);

    if (len < 20 || len > 24)
    {
        return WRONG_NAME;
    }

    for (uint8 i = 0; i < len; i++)
    {
        if (!isalpha(card_Holder_Name[i])&& card_Holder_Name[i] != ' ')
        {
            return WRONG_NAME;
        }
    }
    strcpy(cardData->cardHolderName, card_Holder_Name);
    return CARD_OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
    uint8 expiryDate[7];
    printf("Please enter the cardholder's Expiry Date (MM/YY): ");
    fflush(stdin); fflush(stdout);
    gets(expiryDate);

    int len = strlen((char*)expiryDate);

    if (len != 5)
    {
        return WRONG_EXP_DATE;

    }

    for (uint8 i = 0; i < 5; i++)
    {

        if (i == 2) {
            if (expiryDate[i] != '/')
            {

                return WRONG_EXP_DATE;
            }
        }
        else
        {
            if (!isdigit(expiryDate[i]))
            {
                return WRONG_EXP_DATE;
            }
        }
    }

    uint8 cardMonth  = ((expiryDate[0] - '0') * 10) + (expiryDate[1] - '0');
    uint8 cardYear  = ((expiryDate[3] - '0') * 10) + (expiryDate[4] - '0');

    if (cardMonth  < 1 || cardMonth  > 12 || cardYear  > 99)
    {
        return WRONG_EXP_DATE;
    }
    strcpy(cardData->cardExpirationDate, (char*)expiryDate);
    return CARD_OK;
}

EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{
    uint8 pan[22];
    printf("Please enter the card's Primary Account Number (PAN): ");
    fflush(stdin); fflush(stdout);
    gets(pan);

    uint8 len = strlen((char*)pan);

    if (len < 16 || len > 19)
    {
        return WRONG_PAN;
    }

    for (uint8 i = 0; i < len; i++)
    {
        if (!isdigit(pan[i]))
        {
            return WRONG_PAN;
        }
    }
    strcpy(cardData->primaryAccountNumber, pan);
    return CARD_OK;
}
