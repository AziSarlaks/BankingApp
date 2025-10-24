#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

typedef struct ClientData {
    int id;
    char login[101];
    char password[101];
    char fullname[101];
    int is_banned;
    int balance;
    char phone_number[15];
} ClientData;

// Прототипы функций
int init_database();
int close_database();
int addClient(ClientData data);
int getBalance(int id);
int Deposit(int id, int sum);
int updateBalance(int id, int new_balance);

#endif