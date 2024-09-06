#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H

#include "server.h"  // Include server.h to access ST_accountsDB_t and other definitions
#include "queue.h"

// دوال إدارة الحسابات
void loadAccountsFromFile(const char* filename);
void saveAccountsToFile(const char* filename);

// دوال إدارة المعاملات
void loadTransactionsFromFile(const char* filename);
void saveTransactionsToFile(const char* filename);

#endif // FILE_HANDLING_H
