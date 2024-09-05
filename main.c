
#include <stdio.h>
#include <stdlib.h>
#include "server.h"
#include "file_handling.h"
int main() {
     loadAccountsFromFile("accounts.txt");

    // Display loaded accounts for demonstration
    AccountNode* current = accountsHead;
    while (current != NULL) {
        printf("Account PAN: %s\n", current->accountData.primaryAccountNumber);
        printf("Balance: %.2f\n", current->accountData.balance);
        printf("State: %s\n", current->accountData.state == RUNNING ? "RUNNING" : "BLOCKED");
        printf("-------------------\n");
        current = current->next;
    }

    return 0;
}

